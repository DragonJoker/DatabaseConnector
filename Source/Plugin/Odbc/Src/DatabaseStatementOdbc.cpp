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
	static const String ODBC_MISSING_INITIALIZATION_ERROR = STR( "Method Initialize must be called before calling method CreateParameter" );
	static const String DATABASE_STATEMENT_UNKNOWN_POINTER = STR( "The pointer given by SQLParamData is unknown to the statement" );

	static const String ODBC_HANDLE_ALLOCATION_MSG = STR( "New statement allocation: " );
	static const String ODBC_EXECUTE_STATEMENT_MSG = STR( "Execute: " );

	static const String ODBC_PREPARE_MSG = STR( "SQLPrepare" );
	static const String ODBC_SETSTMTATTR_MSG = STR( "SQLSetStmtAttr" );
	static const String ODBC_CLOSECURSOR_MSG = STR( "SQLCloseCursor" );
	static const String ODBC_FREESTMT_MSG = STR( "SQLFreeStmt" );
	static const String ODBC_NUMPARAMS_MSG = STR( "SQLNumParams" );
	static const String ODBC_PARAMDATA_MSG = STR( "SQLParamData " );

	CDatabaseStatementOdbc::CDatabaseStatementOdbc( DatabaseConnectionOdbcPtr connection, const String & query )
		:   CDatabaseStatement( connection, query )
		,   _statementHandle( SQL_NULL_HSTMT )
		,   _connectionOdbc( connection )
	{
	}

	CDatabaseStatementOdbc::~CDatabaseStatementOdbc()
	{
		Cleanup();
	}

	EErrorType CDatabaseStatementOdbc::Initialize()
	{
		EErrorType errorType;
		HDBC hParentStmt = _connectionOdbc->GetHdbc();
		CLogger::LogMessage( STR( "Preparing statement for query : " ) + _query );

		errorType = SqlSuccess( SQLAllocHandle( SQL_HANDLE_STMT, hParentStmt, &_statementHandle ), SQL_HANDLE_STMT, hParentStmt, ODBC_HANDLE_ALLOCATION_MSG );
#if defined( WIN32 )

		if ( errorType == EErrorType_NONE )
		{
			SqlTry( SQLSetStmtAttr( _statementHandle, SQL_SOPT_SS_DEFER_PREPARE, SQL_DP_OFF, SQL_IS_UINTEGER ), SQL_HANDLE_STMT, _statementHandle, ODBC_SETSTMTATTR_MSG );
		}

#endif

		if ( errorType == EErrorType_NONE )
		{
			SqlTry( SQLPrepareA( _statementHandle, ( SqlChar * )_query.c_str(), SQLINTEGER( _query.size() ) ), SQL_HANDLE_STMT, _statementHandle, ODBC_PREPARE_MSG );
		}

		if ( errorType == EErrorType_NONE )
		{
			SQLSMALLINT count = 0;
			SqlTry( SQLNumParams( _statementHandle, &count ), SQL_HANDLE_STMT, _statementHandle, ODBC_NUMPARAMS_MSG );
			CLogger::LogDebug( STR( "Expected parameters : " ) + CStrUtils::ToString( count ) );
		}

		for ( DatabaseParameterPtrArray::iterator it = _arrayParams.begin(); it != _arrayParams.end(); ++it )
		{
			std::static_pointer_cast< CDatabaseStatementParameterOdbc >( *it )->Initialize( _statementHandle );
		}

		return errorType;
	}

	bool CDatabaseStatementOdbc::ExecuteUpdate( EErrorType * result )
	{
		DatabaseResultPtr rs;
		EErrorType error = DoExecute( rs );

		if ( result )
		{
			*result = error;
		}

		return error == EErrorType_NONE;
	}

	DatabaseResultPtr CDatabaseStatementOdbc::ExecuteSelect( EErrorType * result )
	{
		DatabaseResultPtr rs;
		EErrorType error = DoExecute( rs );

		if ( result )
		{
			*result = error;
		}

		return rs;
	}

	void CDatabaseStatementOdbc::Cleanup()
	{
		if ( _statementHandle != SQL_NULL_HSTMT )
		{
			SQLFreeHandle( SQL_HANDLE_STMT, _statementHandle );
			_statementHandle = SQL_NULL_HSTMT;
		}
	}

	DatabaseParameterPtr CDatabaseStatementOdbc::CreateParameter( const String & name, EFieldType fieldType, EParameterType parameterType )
	{
		DatabaseParameterPtr pReturn = std::make_shared< CDatabaseStatementParameterOdbc >( std::static_pointer_cast< CDatabaseConnectionOdbc >( _connectionOdbc ), name, ( unsigned short )_arrayParams.size() + 1, fieldType, parameterType, new SValueUpdater( this ) );

		if ( !DoAddParameter( pReturn ) )
		{
			pReturn.reset();
		}

		return pReturn;
	}

	DatabaseParameterPtr CDatabaseStatementOdbc::CreateParameter( const String & name, EFieldType fieldType, uint32_t limits, EParameterType parameterType )
	{
		DatabaseParameterPtr pReturn = std::make_shared< CDatabaseStatementParameterOdbc >( std::static_pointer_cast< CDatabaseConnectionOdbc >( _connectionOdbc ), name, ( unsigned short )_arrayParams.size() + 1, fieldType, limits, parameterType, new SValueUpdater( this ) );

		if ( !DoAddParameter( pReturn ) )
		{
			pReturn.reset();
		}

		return pReturn;
	}

	EErrorType CDatabaseStatementOdbc::DoPreExecute()
	{
		EErrorType eResult = EErrorType_NONE;

		for ( auto && it = _arrayParams.begin() ; it != _arrayParams.end() && eResult == EErrorType_NONE ; ++it )
		{
			eResult = std::static_pointer_cast< CDatabaseStatementParameterOdbc >( *it )->GetBinding().BindValue();
		}

		if ( eResult == EErrorType_NONE )
		{
			SQLRETURN retCode = SQLExecute( _statementHandle );

			if ( retCode == SQL_NEED_DATA )
			{
				SQLPOINTER pAddress;
				std::map< void *, DatabaseParameterPtr >::iterator it;

				while ( ( retCode = SQLParamData( _statementHandle, &pAddress ) == SQL_NEED_DATA ) && eResult == EErrorType_NONE )
				{
					it = _mapParamsByPointer.find( pAddress );

					if ( it != _mapParamsByPointer.end() )
					{
						eResult = std::static_pointer_cast< CDatabaseStatementParameterOdbc >( it->second )->GetBinding().PutData();
					}
					else
					{
						CLogger::LogError( DATABASE_STATEMENT_UNKNOWN_POINTER );
					}
				}
			}

			SqlSuccess( retCode, SQL_HANDLE_STMT, _statementHandle, ODBC_EXECUTE_STATEMENT_MSG + _query );
		}

		return eResult;
	}

	EErrorType CDatabaseStatementOdbc::DoExecute( DatabaseResultPtr & result )
	{
		DatabaseResultPtr pReturn;
		EErrorType eResult = DoPreExecute();

		if ( eResult == EErrorType_NONE )
		{
			eResult = SqlExecute( _connection, _statementHandle, std::bind( &CDatabaseStatementOdbc::OnResultSetFullyFetched, this, std::placeholders::_1, std::placeholders::_2 ), result );
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
			std::map< void *, DatabaseParameterPtr >::iterator it;

			while ( ( retCode = SQLParamData( _statementHandle, &pAddress ) == SQL_PARAM_DATA_AVAILABLE ) && eResult == EErrorType_NONE )
			{
				it = _mapParamsByPointer.find( pAddress );

				if ( it != _mapParamsByPointer.end() )
				{
					eResult = std::static_pointer_cast< CDatabaseStatementParameterOdbc >( it->second )->GetBinding().GetData();
				}
				else
				{
					CLogger::LogError( DATABASE_STATEMENT_UNKNOWN_POINTER );
				}
			}

			SqlSuccess( retCode, SQL_HANDLE_STMT, _statementHandle, ODBC_PARAMDATA_MSG );
		}

#endif
	}
}
END_NAMESPACE_DATABASE_ODBC
