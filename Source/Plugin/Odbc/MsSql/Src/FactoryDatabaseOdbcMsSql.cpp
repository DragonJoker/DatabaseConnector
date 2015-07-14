/************************************************************************//**
* @file FactoryDatabaseOdbcMsSql.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 4/08/2014 1:21:00 PM
*
*
* @brief ODBC database factory class.
*
* @details CObject factory used to manage ODBC database creation.
*
***************************************************************************/

#include "DatabaseOdbcMsSqlPch.h"

#include "FactoryDatabaseOdbcMsSql.h"
#include "DatabaseOdbcMsSql.h"

BEGIN_NAMESPACE_DATABASE_ODBC_MSSQL
{
	CFactoryDatabaseOdbcMsSql::CFactoryDatabaseOdbcMsSql()
		: CFactoryDatabase()
	{
		// Empty
	}

	CFactoryDatabaseOdbcMsSql::~CFactoryDatabaseOdbcMsSql()
	{
		// Empty
	}

	void CFactoryDatabaseOdbcMsSql::RegisterServices()
	{
		//!@remarks Register function for creation of a DatabaseOdbcMsSql object.
		RegisterCreatorFunction( DATABASE_ODBC_MSSQL_TYPE, CDatabaseOdbcMsSql::Create );
	}

	void CFactoryDatabaseOdbcMsSql::RegisterObjectTypes()
	{
		_objTypes.push_back( DATABASE_ODBC_MSSQL_TYPE );
	}
}
END_NAMESPACE_DATABASE_ODBC_MSSQL
