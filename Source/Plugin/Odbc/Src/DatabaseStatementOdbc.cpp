/************************************************************************//**
 * @file DatabaseStatementOdbc.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 *
 *
 * @brief CDatabaseStatementOdbc class definition.
 *
 * @details Describes a statement for ODBC database.
 *
 ***************************************************************************/

#include "DatabaseOdbcPch.h"

#include "DatabaseStatementOdbc.h"

#include "DatabaseConnectionOdbc.h"
#include "DatabaseOdbcHelper.h"
#include "DatabaseStatementParameterOdbc.h"
#include "ExceptionDatabaseOdbc.h"

#include <DatabaseStringUtils.h>
#include <functional>

#define SQL_SOPT_SS_DEFER_PREPARE (SQL_SOPT_SS_BASE+7)

BEGIN_NAMESPACE_DATABASE_ODBC
{
	static const String ERROR_ODBC_MISSING_INITIALIZATION = STR( "Method Initialise must be called before calling method CreateParameter" );
	static const String ERROR_ODBC_STATEMENT_UNKNOWN_POINTER = STR( "The pointer given by SQLParamData is unknown to the statement" );
	static const String ERROR_ODBC_LOST_CONNECTION = STR( "The statement has lost his connection" );

	static const String INFO_ODBC_EXECUTE_STATEMENT = STR( "Execute statement 0x" );
	
	static const String DEBUG_ODBC_PREPARING_STATEMENT = STR( "Preparing statement 0x%08X" );

	static const String INFO_ODBC_Prepare = STR( "SQLPrepare" );
	static const String INFO_ODBC_AllocHandle = STR( "SQLAllocHandle: " );
	static const String INFO_ODBC_SetStmtAttr = STR( "SQLSetStmtAttr" );
	static const String INFO_ODBC_CloseCursor = STR( "SQLCloseCursor" );
	static const String INFO_ODBC_FreeStmt = STR( "SQLFreeStmt" );
	static const String INFO_ODBC_NumParams = STR( "SQLNumParams" );
	static const String INFO_ODBC_ParamData = STR( "SQLParamData " );

	static const String DEBUG_ODBC_EXPECTED_PARAMETERS = STR( "Expected parameters : " );

	CDatabaseStatementOdbc::CDatabaseStatementOdbc( DatabaseConnectionOdbcSPtr connection, const String & query )
		:   CDatabaseStatement( connection, query )
		,   _statementHandle( SQL_NULL_HSTMT )
		,   _connectionOdbc( connection )
	{
	}

	CDatabaseStatementOdbc::~CDatabaseStatementOdbc()
	{
		Cleanup();
	}

	DatabaseParameterSPtr CDatabaseStatementOdbc::DoCreateParameter( DatabaseValuedObjectInfosSPtr infos, EParameterType parameterType )
	{
		DatabaseConnectionOdbcSPtr connection = DoGetConnectionOdbc();

		if ( !connection )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, ERROR_ODBC_LOST_CONNECTION );
		}

		return DoAddParameter( std::make_shared< CDatabaseStatementParameterOdbc >( connection, infos, uint16_t( _arrayParams.size() + 1 ), parameterType, std::make_unique< SValueUpdater >( this ) ) );
	}

	EErrorType CDatabaseStatementOdbc::DoInitialise()
	{
		DatabaseConnectionOdbcSPtr connection = DoGetConnectionOdbc();

		if ( !connection )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, ERROR_ODBC_LOST_CONNECTION );
		}

		EErrorType errorType;
		HDBC hParentStmt = connection->GetHdbc();
		CLogger::LogDebug( ( Format( DEBUG_ODBC_PREPARING_STATEMENT ) % this ).str() );

		errorType = SqlSuccess( SQLAllocHandle( SQL_HANDLE_STMT, hParentStmt, &_statementHandle ), SQL_HANDLE_STMT, hParentStmt, INFO_ODBC_AllocHandle );
#if defined( WIN32 )

		if ( errorType == EErrorType_NONE )
		{
			OdbcCheck( SQLSetStmtAttr( _statementHandle, SQL_SOPT_SS_DEFER_PREPARE, SQL_DP_OFF, SQL_IS_UINTEGER ), SQL_HANDLE_STMT, _statementHandle, INFO_ODBC_SetStmtAttr );
		}

#endif

		if ( errorType == EErrorType_NONE )
		{
			OdbcCheck( SQLPrepareA( _statementHandle, ( SqlChar * )_query.c_str(), SQLINTEGER( _query.size() ) ), SQL_HANDLE_STMT, _statementHandle, INFO_ODBC_Prepare );
		}

		if ( errorType == EErrorType_NONE )
		{
			SQLSMALLINT count = 0;
			OdbcCheck( SQLNumParams( _statementHandle, &count ), SQL_HANDLE_STMT, _statementHandle, INFO_ODBC_NumParams );
			CLogger::LogDebug( DEBUG_ODBC_EXPECTED_PARAMETERS + StringUtils::ToString( count ) );
		}

		for ( DatabaseParameterPtrArray::iterator it = _arrayParams.begin(); it != _arrayParams.end(); ++it )
		{
			std::static_pointer_cast< CDatabaseStatementParameterOdbc >( *it )->Initialise( _statementHandle );
		}

		return errorType;
	}

	bool CDatabaseStatementOdbc::DoExecuteUpdate()
	{
		DatabaseResultSPtr rs;
		return DoExecute( rs ) == EErrorType_NONE;
	}

	DatabaseResultSPtr CDatabaseStatementOdbc::DoExecuteSelect()
	{
		DatabaseResultSPtr rs;
		DoExecute( rs );
		return rs;
	}

	void CDatabaseStatementOdbc::DoCleanup()
	{
		if ( _statementHandle != SQL_NULL_HSTMT )
		{
			SQLFreeHandle( SQL_HANDLE_STMT, _statementHandle );
			_statementHandle = SQL_NULL_HSTMT;
		}
	}

	EErrorType CDatabaseStatementOdbc::DoPreExecute()
	{
		EErrorType eResult = EErrorType_NONE;

		if ( eResult == EErrorType_NONE )
		{
			SQLRETURN retCode = SQLExecute( _statementHandle );

			if ( retCode == SQL_NEED_DATA )
			{
				SQLPOINTER pAddress;
				std::map< const void *, CDatabaseParameter const * >::iterator it;

				while ( ( retCode = SQLParamData( _statementHandle, &pAddress ) == SQL_NEED_DATA ) && eResult == EErrorType_NONE )
				{
					it = _mapParamsByPointer.find( pAddress );

					if ( it != _mapParamsByPointer.end() )
					{
						eResult = static_cast< CDatabaseStatementParameterOdbc const * >( it->second )->GetBinding().PutData();
					}
					else
					{
						CLogger::LogError( ERROR_ODBC_STATEMENT_UNKNOWN_POINTER );
					}
				}
			}

			SqlSuccess( retCode, SQL_HANDLE_STMT, _statementHandle, StringStream() << INFO_ODBC_EXECUTE_STATEMENT << this );
		}

		return eResult;
	}

	EErrorType CDatabaseStatementOdbc::DoExecute( DatabaseResultSPtr & result )
	{
		DatabaseConnectionOdbcSPtr connection = DoGetConnectionOdbc();

		if ( !connection )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, ERROR_ODBC_LOST_CONNECTION );
		}

		DatabaseResultSPtr pReturn;
		EErrorType eResult = DoPreExecute();

		if ( eResult == EErrorType_NONE )
		{
			eResult = SqlExecute( connection, _statementHandle, std::bind( &CDatabaseStatementOdbc::OnResultSetFullyFetched, this, std::placeholders::_1, std::placeholders::_2 ), result );
		}

		return eResult;
	}

	void CDatabaseStatementOdbc::OnResultSetFullyFetched( HSTMT statementHandle, SQLRETURN info )
	{
		assert( statementHandle == _statementHandle );

#if defined( _WIN32 )

		if ( info == SQL_PARAM_DATA_AVAILABLE )
		{
			EErrorType eResult = EErrorType_NONE;
			SQLPOINTER pAddress = NULL;
			SQLRETURN retCode = 0;
			std::map< const void *, CDatabaseParameter const * >::iterator it;

			while ( ( retCode = SQLParamData( _statementHandle, &pAddress ) == SQL_PARAM_DATA_AVAILABLE ) && eResult == EErrorType_NONE )
			{
				it = _mapParamsByPointer.find( pAddress );

				if ( it != _mapParamsByPointer.end() )
				{
					eResult = static_cast< CDatabaseStatementParameterOdbc const * >( it->second )->GetBinding().GetData();
				}
				else
				{
					CLogger::LogError( ERROR_ODBC_STATEMENT_UNKNOWN_POINTER );
				}
			}

			SqlSuccess( retCode, SQL_HANDLE_STMT, _statementHandle, INFO_ODBC_ParamData );
		}

		EErrorType errorType = EErrorType_NONE;
		OdbcCheck( SQLFreeStmt( statementHandle, SQL_CLOSE ), SQL_HANDLE_STMT, statementHandle, INFO_ODBC_FreeStmt << STR( " (Close)" ) );
		OdbcCheck( SQLFreeStmt( statementHandle, SQL_UNBIND ), SQL_HANDLE_STMT, statementHandle, INFO_ODBC_FreeStmt << STR( " (Unbind)" ) );
		OdbcCheck( SQLFreeStmt( statementHandle, SQL_RESET_PARAMS ), SQL_HANDLE_STMT, statementHandle, INFO_ODBC_FreeStmt << STR( " (ResetParams)" ) );

#endif
	}
}
END_NAMESPACE_DATABASE_ODBC
