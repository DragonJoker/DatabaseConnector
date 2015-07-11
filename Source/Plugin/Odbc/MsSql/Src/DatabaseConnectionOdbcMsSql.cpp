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
#include <DatabaseQueryOdbc.h>

BEGIN_NAMESPACE_DATABASE_ODBC_MSSQL
{
	static const String ERROR_ODBC_NOT_CONNECTED = STR( "Not connected" );

	CDatabaseConnectionOdbcMsSql::CDatabaseConnectionOdbcMsSql( SQLHENV sqlEnvironmentHandle, const String & server, const String & userName, const String & password, String & connectionString )
		:   CDatabaseConnectionOdbc( sqlEnvironmentHandle, server, userName, password, connectionString )
	{
		DoConnect( connectionString );
	}

	CDatabaseConnectionOdbcMsSql::~CDatabaseConnectionOdbcMsSql()
	{
		DoDisconnect();
	}

	void CDatabaseConnectionOdbcMsSql::CreateDatabase( const String & database )
	{
		if ( !IsConnected() )
		{
			CLogger::LogError( ERROR_ODBC_NOT_CONNECTED );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_ODBC_NOT_CONNECTED, __FUNCTION__, __FILE__, __LINE__ );
		}

		DoExecuteUpdate( STR( "CREATE DATABASE " ) + database + STR( " COLLATE utf8_BIN" ), NULL );
	}

	void CDatabaseConnectionOdbcMsSql::SelectDatabase( const String & database )
	{
		if ( !IsConnected() )
		{
			CLogger::LogError( ERROR_ODBC_NOT_CONNECTED );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_ODBC_NOT_CONNECTED, __FUNCTION__, __FILE__, __LINE__ );
		}

		_database = database;
		DoExecuteUpdate( STR( "USE " ) + database, NULL );
	}

	void CDatabaseConnectionOdbcMsSql::DestroyDatabase( const String & database )
	{
		if ( !IsConnected() )
		{
			CLogger::LogError( ERROR_ODBC_NOT_CONNECTED );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_ODBC_NOT_CONNECTED, __FUNCTION__, __FILE__, __LINE__ );
		}

		DoExecuteUpdate( STR( "DROP DATABASE " ) + database, NULL );
	}

	EErrorType CDatabaseConnectionOdbcMsSql::DoConnect( String & connectionString )
	{
		EErrorType  eReturn     = EErrorType_ERROR;
		SQLHWND     sqlHwnd     = NULL;
		SQLRETURN   sqlReturn;

		connectionString = STR( "DRIVER={SQL Server};SERVER=(" ) + connectionString + STR( ")" );

		if ( _userName.size() > 0 )
		{
			connectionString += STR( ";UID=" ) + _userName + STR( ";PWD=" ) + _password;
		}
		else
		{
			connectionString += STR( ";INTEGRATED SECURITY=true;Trusted_Connection=yes" );
		}

		if ( SqlSuccess( SQLAllocHandle( SQL_HANDLE_DBC, _environmentHandle, &_connectionHandle ), SQL_HANDLE_ENV, _environmentHandle, STR( "SQLAllocHandle" ) ) == EErrorType_NONE )
		{
			sqlReturn = SQLDriverConnectA( _connectionHandle, sqlHwnd, ( SqlChar * )connectionString.c_str(), SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE_REQUIRED );

			if ( SqlSuccess( sqlReturn, SQL_HANDLE_DBC, _connectionHandle, STR( "SQLDriverConnect" ) ) == EErrorType_NONE )
			{
				DoSetConnected( true );
			}
		}

		return eReturn;
	}

	DatabaseStatementPtr CDatabaseConnectionOdbcMsSql::DoCreateStatement( const String & request, EErrorType * result )
	{
		DatabaseStatementPtr pReturn;
		EErrorType eResult = EErrorType_ERROR;

		if ( IsConnected() )
		{
			pReturn = std::make_shared< CDatabaseStatementOdbc >( shared_from_this(), request );
			eResult = EErrorType_NONE;
		}

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

		if ( IsConnected() )
		{
			l_pReturn = std::make_shared< CDatabaseQueryOdbc >( shared_from_this(), request );
			eResult = EErrorType_NONE;
		}

		if ( result )
		{
			*result = eResult;
		}

		return l_pReturn;
	}
}
END_NAMESPACE_DATABASE_ODBC_MSSQL
