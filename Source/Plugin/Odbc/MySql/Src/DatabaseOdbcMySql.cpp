/************************************************************************//**
* @file DatabaseOdbcMySql.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDatabaseOdbcMySql class declaration.
*
* @details Describes an ODBC database.
*
***************************************************************************/

#include "DatabaseOdbcMySqlPch.h"

#include "DatabaseOdbcMySql.h"
#include "DatabaseConnectionOdbcMySql.h"

#include <DatabaseOdbcHelper.h>

BEGIN_NAMESPACE_DATABASE_ODBC_MYSQL
{
	static const String INFO_ODBC_AllocHandle = STR( "SQLAllocHandle" );
	static const String INFO_ODBC_SetEnvAttr = STR( "SQLSetEnvAttr" );

	CDatabaseOdbcMySql::CDatabaseOdbcMySql()
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

	CDatabaseOdbcMySql::~CDatabaseOdbcMySql()
	{
		if ( _sqlEnvironmentHandle != SQL_NULL_HENV )
		{
			SQLFreeHandle( SQL_HANDLE_ENV, _sqlEnvironmentHandle );
			_sqlEnvironmentHandle = SQL_NULL_HENV;
		}
	}

	const String & CDatabaseOdbcMySql::GetType()const
	{
		return DATABASE_ODBC_MYSQL_TYPE;
	}

	CDatabase * CDatabaseOdbcMySql::Create()
	{
		return new CDatabaseOdbcMySql;
	}

	DatabaseConnectionSPtr CDatabaseOdbcMySql::DoCreateConnection( String & connectionString ) const
	{
		DatabaseConnectionSPtr pReturn;

		if ( _sqlEnvironmentHandle != SQL_NULL_HENV )
		{
			pReturn = std::make_shared< CDatabaseConnectionOdbcMySql >( _sqlEnvironmentHandle, _server, _userName, _password, connectionString );
		}

		return pReturn;
	}
}
END_NAMESPACE_DATABASE_ODBC_MYSQL
