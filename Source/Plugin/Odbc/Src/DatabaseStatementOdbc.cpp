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
#include "DatabaseResultOdbc.h"
#include "DatabaseStatementOdbcHelper.h"
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
	static const String ODBC_BIND_PARAMETER_NAME_MSG = STR( "Parameter:" );
	static const String ODBC_BIND_PARAMETER_VALUE_MSG = STR( "Value:" );
	static const String ODBC_EXECUTE_STATEMENT_MSG = STR( "Execute: " );

	static const String ODBC_PREPARE_MSG = STR( "SQLPrepare" );
	static const String ODBC_PUTDATA_MSG = STR( "SQLPutData" );
	static const String ODBC_GETDATA_MSG = STR( "SQLGetData" );
	static const String ODBC_SETSTMTATTR_MSG = STR( "SQLSetStmtAttr" );
	static const String ODBC_CLOSECURSOR_MSG = STR( "SQLCloseCursor" );
	static const String ODBC_FREESTMT_MSG = STR( "SQLFreeStmt" );
	static const String ODBC_NUMPARAMS_MSG = STR( "SQLNumParams" );
	static const String ODBC_PARAMDATA_MSG = STR( "SQLParamData " );

	CDatabaseStatementOdbc::CDatabaseStatementOdbc( DatabaseConnectionOdbcPtr connection, const String & query )
		:   CDatabaseStatement( connection, query )
		,   _statementHandle( SQL_NULL_HSTMT )
		,   _strLenIndPtr( SQL_NULL_DATA )
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
		int attemptCount;
		HDBC hParentStmt = _connectionOdbc->GetHdbc();
		CLogger::LogMessage( STR( "Preparing statement for query : " ) + _query );

		errorType = SqlSuccess( SQLAllocHandle( SQL_HANDLE_STMT, hParentStmt, & _statementHandle ), SQL_HANDLE_STMT, hParentStmt, ODBC_HANDLE_ALLOCATION_MSG );
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
		return DoExecute( result ) != nullptr;
	}

	DatabaseResultPtr CDatabaseStatementOdbc::ExecuteSelect( EErrorType * result )
	{
		return DoExecute( result );
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

	EErrorType CDatabaseStatementOdbc::DoBindParameter( DatabaseStatementParameterOdbcPtr parameter )
	{
		EErrorType eReturn = EErrorType_NONE;
		_strLenIndPtr = SQL_NULL_DATA;
		StringStream message;
		message << ODBC_BIND_PARAMETER_NAME_MSG << parameter->GetName()
				<< STR( ", " )
				<< ODBC_BIND_PARAMETER_VALUE_MSG << STR( "[" ) << parameter->GetObjectValue().GetPtrValue() << STR( "] : " ) << parameter->GetObjectValue().GetStrValue();

		switch ( parameter->GetType() )
		{
		case EFieldType_VARCHAR:
		case EFieldType_TEXT:
		case EFieldType_NVARCHAR:
		case EFieldType_NTEXT:
		case EFieldType_BINARY:
		case EFieldType_VARBINARY:
		case EFieldType_LONG_VARBINARY:
			eReturn = SDatabaseParamBinderExecOdbc()( _statementHandle, parameter, &_strLenIndPtr, message.str(), _query );
			break;

		default:
			eReturn = SDatabaseParamBinderDirectOdbc()( _statementHandle, parameter, &_strLenIndPtr, message.str(), _query );
			break;
		}

		return eReturn;
	}

	EErrorType CDatabaseStatementOdbc::DoPutParameterData( DatabaseStatementParameterOdbcPtr parameter )
	{
		char * pData = static_cast <char *>( parameter->GetObjectValue().GetPtrValue() );
		long lSize = parameter->GetObjectValue().GetPtrSize();
		long lOffset = 0;
		long lBatch = 5000;
		EErrorType errorType = EErrorType_NONE;
		int attemptCount;

		while ( lSize > lBatch && errorType == EErrorType_NONE )
		{
			SqlTry( SQLPutData( _statementHandle, pData + lOffset, lBatch ), SQL_HANDLE_STMT, _statementHandle, ODBC_PUTDATA_MSG );
			lOffset += lBatch;
			lSize -= lBatch;
		}

		if ( errorType == EErrorType_NONE && lSize > 0 )
		{
			SqlTry( SQLPutData( _statementHandle, pData + lOffset, lSize ), SQL_HANDLE_STMT, _statementHandle, ODBC_PUTDATA_MSG );
		}

		return errorType;
	}

	EErrorType CDatabaseStatementOdbc::DoGetParameterData( DatabaseStatementParameterOdbcPtr parameter )
	{
		uint8_t * pData = static_cast< uint8_t * >( parameter->GetObjectValue().GetPtrValue() );
		long lSize = parameter->GetObjectValue().GetPtrSize();
		long lOffset = 0;
		long lBatch = 5000;
		SQLLEN lRetrieved;
		EErrorType errorType = EErrorType_NONE;
		int attemptCount;

		while ( lSize > lBatch && errorType == EErrorType_NONE )
		{
			SqlTry( SQLGetData( _statementHandle, parameter->GetIndex(), parameter->GetValueType(), pData + lOffset, lBatch, &lRetrieved ), SQL_HANDLE_STMT, _statementHandle, ODBC_GETDATA_MSG );
			lOffset += lBatch;
			lSize -= lBatch;
		}

		if ( errorType == EErrorType_NONE && lSize > 0 )
		{
			SqlTry( SQLGetData( _statementHandle, parameter->GetIndex(), parameter->GetValueType(), pData + lOffset, lSize, &lRetrieved ), SQL_HANDLE_STMT, _statementHandle, ODBC_GETDATA_MSG );
		}

		return errorType;
	}

	EErrorType CDatabaseStatementOdbc::DoPreExecute()
	{
		EErrorType eResult = EErrorType_NONE;

		for ( DatabaseParameterPtrArray::iterator it = _arrayParams.begin() ; it != _arrayParams.end() && eResult == EErrorType_NONE ; ++it )
		{
			eResult = DoBindParameter( std::static_pointer_cast< CDatabaseStatementParameterOdbc >( *it ) );
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
						eResult = DoPutParameterData( std::static_pointer_cast< CDatabaseStatementParameterOdbc >( it->second ) );
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

	DatabaseResultPtr CDatabaseStatementOdbc::DoExecute( EErrorType * result )
	{
		DatabaseResultPtr pReturn;
		EErrorType eResult = DoPreExecute();

		if ( eResult == EErrorType_NONE )
		{
			DatabaseResultOdbcPtr pRet = std::make_shared< CDatabaseResultOdbc >( _connection );
			pRet->sResultSetFullyFetched.connect( std::bind( &CDatabaseStatementOdbc::OnResultSetFullyFetched, this, std::placeholders::_1, std::placeholders::_2 ) );
			pRet->Initialize( _statementHandle );
			pReturn = pRet;
		}

		if ( result )
		{
			*result = eResult;
		}

		return pReturn;
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
					eResult = DoGetParameterData( std::static_pointer_cast< CDatabaseStatementParameterOdbc >( it->second ) );
				}
				else
				{
					CLogger::LogError( DATABASE_STATEMENT_UNKNOWN_POINTER );
				}
			}

			SqlSuccess( retCode, SQL_HANDLE_STMT, _statementHandle, ODBC_PARAMDATA_MSG );
		}

#endif
		int attemptCount;
		EErrorType errorType = EErrorType_NONE;
		SqlTry( SQLCloseCursor( _statementHandle ), SQL_HANDLE_STMT, _statementHandle, ODBC_CLOSECURSOR_MSG );
		SqlTry( SQLFreeStmt( _statementHandle, SQL_CLOSE ), SQL_HANDLE_STMT, _statementHandle, ODBC_FREESTMT_MSG + STR( " (Close)" ) );
		SqlTry( SQLFreeStmt( _statementHandle, SQL_UNBIND ), SQL_HANDLE_STMT, _statementHandle, ODBC_FREESTMT_MSG + STR( " (Unbind)" ) );
		SqlTry( SQLFreeStmt( _statementHandle, SQL_RESET_PARAMS ), SQL_HANDLE_STMT, _statementHandle, ODBC_FREESTMT_MSG + STR( " (ResetParams)" ) );
	}
}
END_NAMESPACE_DATABASE_ODBC
