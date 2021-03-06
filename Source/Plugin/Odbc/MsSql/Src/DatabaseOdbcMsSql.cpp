/************************************************************************//**
* @file DatabaseOdbcMsSql.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDatabaseOdbcMsSql class declaration.
*
* @details Describes an ODBC database.
*
***************************************************************************/

#include "DatabaseOdbcMsSqlPch.h"

#include "DatabaseOdbcMsSql.h"
#include "DatabaseConnectionOdbcMsSql.h"

#include <DatabaseOdbcHelper.h>

BEGIN_NAMESPACE_DATABASE_ODBC_MSSQL
{
	static const String INFO_ODBC_AllocHandle = STR( "SQLAllocHandle" );
	static const String INFO_ODBC_SetEnvAttr = STR( "SQLSetEnvAttr" );

	CDatabaseOdbcMsSql::CDatabaseOdbcMsSql()
		:   CDatabase()
		,   _sqlEnvironmentHandle( SQL_NULL_HENV )
	{
		if ( SqlSuccess( SQLAllocHandle( SQL_HANDLE_ENV, SQL_NULL_HANDLE, & _sqlEnvironmentHandle ), 0, NULL, INFO_ODBC_AllocHandle ) == EErrorType_NONE )
		{
			if ( SqlSuccess( SQLSetEnvAttr( _sqlEnvironmentHandle, SQL_ATTR_ODBC_VERSION, ( void * )SQL_OV_ODBC3, SQL_IS_INTEGER ), SQL_HANDLE_ENV, _sqlEnvironmentHandle, INFO_ODBC_SetEnvAttr ) != EErrorType_NONE )
			{
				SQLFreeHandle( SQL_HANDLE_ENV, _sqlEnvironmentHandle );
				_sqlEnvironmentHandle = SQL_NULL_HENV;
			}
		}
		else
		{
			_sqlEnvironmentHandle = SQL_NULL_HENV;
		}
	}

	CDatabaseOdbcMsSql::~CDatabaseOdbcMsSql()
	{
		if ( _sqlEnvironmentHandle != SQL_NULL_HENV )
		{
			SQLFreeHandle( SQL_HANDLE_ENV, _sqlEnvironmentHandle );
			_sqlEnvironmentHandle = SQL_NULL_HENV;
		}
	}

	const String & CDatabaseOdbcMsSql::GetType()const
	{
		return DATABASE_ODBC_MSSQL_TYPE;
	}

	CDatabase * CDatabaseOdbcMsSql::Create()
	{
		return new CDatabaseOdbcMsSql;
	}

	DatabaseConnectionSPtr CDatabaseOdbcMsSql::DoCreateConnection( String & connectionString ) const
	{
		DatabaseConnectionSPtr pReturn;

		if ( _sqlEnvironmentHandle != SQL_NULL_HENV )
		{
			pReturn = std::make_shared< CDatabaseConnectionOdbcMsSql >( _sqlEnvironmentHandle, _server, _userName, _password, connectionString );
		}

		return pReturn;
	}
}
END_NAMESPACE_DATABASE_ODBC_MSSQL
