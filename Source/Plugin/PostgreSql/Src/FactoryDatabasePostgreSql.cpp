/************************************************************************//**
* @file FactoryDatabasePostgreSql.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 4/08/2014 1:21:00 PM
*
*
* @brief MYSQL database factory class.
*
* @details CObject factory used to manage MYSQL database creation.
*
***************************************************************************/

#include "DatabasePostgreSqlPch.h"

#include "FactoryDatabasePostgreSql.h"
#include "DatabasePostgreSql.h"

BEGIN_NAMESPACE_DATABASE_POSTGRESQL
{
	CFactoryDatabasePostgreSql::CFactoryDatabasePostgreSql()
		: CFactoryDatabase()
	{
		// Empty
	}

	CFactoryDatabasePostgreSql::~CFactoryDatabasePostgreSql()
	{
		// Empty
	}

	void CFactoryDatabasePostgreSql::RegisterServices()
	{
		///@remarks Register function for creation of a DatabasePostgreSql object.
		RegisterCreatorFunction( DATABASE_POSTGRESQL_TYPE, CDatabasePostgreSql::Create );
	}

	void CFactoryDatabasePostgreSql::RegisterObjectTypes()
	{
		_objTypes.push_back( DATABASE_POSTGRESQL_TYPE );
	}

}
END_NAMESPACE_DATABASE_POSTGRESQL
