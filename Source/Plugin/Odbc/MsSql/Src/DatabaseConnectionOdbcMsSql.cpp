/************************************************************************//**
* @file DatabaseConnectionOdbcMsSql.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 3/14/2014 5:03:15 PM
*
*
* @brief CConnectionOdbcMsSql class definition.
*
* @details Describes a connection to a database via an ODBC driver.
*
***************************************************************************/

#include "DatabaseOdbcMsSqlPch.h"

#include "DatabaseConnectionOdbcMsSql.h"

#include <DatabaseOdbcHelper.h>
#include <DatabaseStatementOdbc.h>
#include <DatabaseQuery.h>

BEGIN_NAMESPACE_DATABASE_ODBC_MSSQL
{
	static const String ERROR_ODBC_NOT_CONNECTED = STR( "Not connected" );

	static const String ODBC_SQL_CREATE_DATABASE = STR( "CREATE DATABASE " );
	static const String ODBC_SQL_COLLATE = STR( " COLLATE utf8_BIN" );
	static const String ODBC_SQL_USE_DATABASE = STR( "USE " );
	static const String ODBC_SQL_DROP_DATABASE = STR( "DROP DATABASE " );

	static const String ODBC_DSN_DRIVER = STR( "DRIVER={SQL Server};SERVER=" );
	static const String ODBC_DSN_UID = STR( ";UID=" );
	static const String ODBC_DSN_PWD = STR( ";PWD=" );
	static const String ODBC_DSN_INTEGRATED = STR( ";INTEGRATED SECURITY=true;Trusted_Connection=yes" );

	static const String INFO_ODBC_AllocHandle = STR( "SQLAllocHandle" );
	static const String INFO_ODBC_DriverConnect = STR( "SQLDriverConnect" );

	CDatabaseConnectionOdbcMsSql::CDatabaseConnectionOdbcMsSql( SQLHENV sqlEnvironmentHandle, const String & server, const String & userName, const String & password, String & connectionString )
		:   CDatabaseConnectionOdbc( sqlEnvironmentHandle, server, userName, password, connectionString )
	{
		DoConnect( connectionString );
	}

	CDatabaseConnectionOdbcMsSql::~CDatabaseConnectionOdbcMsSql()
	{
		DoDisconnect();
	}

	uint32_t CDatabaseConnectionOdbcMsSql::GetPrecision( EFieldType type ) const
	{
		uint32_t result = 0;

		switch ( type )
		{
		case EFieldType_FLOAT32:
			result = 6;
			break;

		case EFieldType_FLOAT64:
			result = 17;
			break;
		}

		return result;
	}

	void CDatabaseConnectionOdbcMsSql::CreateDatabase( const String & database )
	{
		if ( !IsConnected() )
		{
			CLogger::LogError( ERROR_ODBC_NOT_CONNECTED );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_ODBC_NOT_CONNECTED, __FUNCTION__, __FILE__, __LINE__ );
		}

		DoExecuteUpdate( ODBC_SQL_CREATE_DATABASE + database + ODBC_SQL_COLLATE, NULL );
	}

	void CDatabaseConnectionOdbcMsSql::SelectDatabase( const String & database )
	{
		if ( !IsConnected() )
		{
			CLogger::LogError( ERROR_ODBC_NOT_CONNECTED );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_ODBC_NOT_CONNECTED, __FUNCTION__, __FILE__, __LINE__ );
		}

		_database = database;
		DoExecuteUpdate( ODBC_SQL_USE_DATABASE + database, NULL );
	}

	void CDatabaseConnectionOdbcMsSql::DestroyDatabase( const String & database )
	{
		if ( !IsConnected() )
		{
			CLogger::LogError( ERROR_ODBC_NOT_CONNECTED );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_ODBC_NOT_CONNECTED, __FUNCTION__, __FILE__, __LINE__ );
		}

		DoExecuteUpdate( ODBC_SQL_DROP_DATABASE + database, NULL );
	}

	EErrorType CDatabaseConnectionOdbcMsSql::DoConnect( String & connectionString )
	{
		EErrorType eReturn = EErrorType_ERROR;
		connectionString = ODBC_DSN_DRIVER + STR( "(" ) + connectionString + STR( ")" );

		if ( _userName.size() > 0 )
		{
			connectionString += ODBC_DSN_UID + _userName + ODBC_DSN_PWD + _password;
		}
		else
		{
			connectionString += ODBC_DSN_INTEGRATED;
		}

		if ( SqlSuccess( SQLAllocHandle( SQL_HANDLE_DBC, _environmentHandle, &_connectionHandle ), SQL_HANDLE_ENV, _environmentHandle, INFO_ODBC_AllocHandle ) == EErrorType_NONE )
		{
			SQLHWND sqlHwnd = NULL;
			SQLRETURN sqlReturn = SQLDriverConnectA( _connectionHandle, sqlHwnd, ( SqlChar * )connectionString.c_str(), SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE_REQUIRED );

			if ( SqlSuccess( sqlReturn, SQL_HANDLE_DBC, _connectionHandle, INFO_ODBC_DriverConnect ) == EErrorType_NONE )
			{
				DoSetConnected( true );
				eReturn = EErrorType_NONE;
			}
		}

		return eReturn;
	}

	DatabaseStatementPtr CDatabaseConnectionOdbcMsSql::DoCreateStatement( const String & request, EErrorType * result )
	{
		DatabaseStatementPtr pReturn;
		EErrorType eResult = EErrorType_ERROR;

		if ( !IsConnected() )
		{
			CLogger::LogError( ERROR_ODBC_NOT_CONNECTED );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_ODBC_NOT_CONNECTED, __FUNCTION__, __FILE__, __LINE__ );
		}

		pReturn = std::make_shared< CDatabaseStatementOdbc >( shared_from_this(), request );
		eResult = EErrorType_NONE;

		if ( result )
		{
			*result = eResult;
		}

		return pReturn;
	}

	DatabaseQueryPtr CDatabaseConnectionOdbcMsSql::DoCreateQuery( const String & request, EErrorType * result )
	{
		DatabaseQueryPtr l_pReturn;
		EErrorType eResult = EErrorType_ERROR;

		if ( !IsConnected() )
		{
			CLogger::LogError( ERROR_ODBC_NOT_CONNECTED );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_ODBC_NOT_CONNECTED, __FUNCTION__, __FILE__, __LINE__ );
		}

		l_pReturn = std::make_shared< CDatabaseQuery >( shared_from_this(), request );
		eResult = EErrorType_NONE;

		if ( result )
		{
			*result = eResult;
		}

		return l_pReturn;
	}
}
END_NAMESPACE_DATABASE_ODBC_MSSQL
