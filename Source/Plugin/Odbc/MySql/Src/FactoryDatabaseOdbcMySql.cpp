/************************************************************************//**
* @file FactoryDatabaseOdbcMySql.cpp
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

#include "DatabaseOdbcMySqlPch.h"

#include "FactoryDatabaseOdbcMySql.h"
#include "DatabaseOdbcMySql.h"

BEGIN_NAMESPACE_DATABASE_ODBC_MYSQL
{
	CFactoryDatabaseOdbcMySql::CFactoryDatabaseOdbcMySql()
		: CFactoryDatabase()
	{
		// Empty
	}

	CFactoryDatabaseOdbcMySql::~CFactoryDatabaseOdbcMySql()
	{
		// Empty
	}

	void CFactoryDatabaseOdbcMySql::RegisterServices()
	{
		//!@remarks Register function for creation of a DatabaseOdbcMySql object.
		RegisterCreatorFunction( DATABASE_ODBC_MYSQL_TYPE, CDatabaseOdbcMySql::Create );
	}

	void CFactoryDatabaseOdbcMySql::RegisterObjectTypes()
	{
		_objTypes.push_back( DATABASE_ODBC_MYSQL_TYPE );
	}
}
END_NAMESPACE_DATABASE_ODBC_MYSQL
