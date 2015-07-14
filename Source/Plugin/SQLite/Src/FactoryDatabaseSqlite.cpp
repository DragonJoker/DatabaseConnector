/************************************************************************//**
* @file FactoryDatabaseSqlite.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 4/08/2014 1:21:00 PM
*
*
* @brief SQLITE database factory class.
*
* @details CObject factory used to manage SQLITE database creation.
*
***************************************************************************/

#include "DatabaseSqlitePch.h"

#include "FactoryDatabaseSqlite.h"
#include "DatabaseSqlite.h"

BEGIN_NAMESPACE_DATABASE_SQLITE
{
	CFactoryDatabaseSqlite::CFactoryDatabaseSqlite()
		: CFactoryDatabase()
	{
		// Empty
	}

	CFactoryDatabaseSqlite::~CFactoryDatabaseSqlite()
	{
		// Empty
	}

	void CFactoryDatabaseSqlite::RegisterServices()
	{
		//!@remarks Register function for creation of a DatabaseSqlite object.
		RegisterCreatorFunction( DATABASE_SQLITE_TYPE, CDatabaseSqlite::Create );
	}

	void CFactoryDatabaseSqlite::RegisterObjectTypes()
	{
		_objTypes.push_back( DATABASE_SQLITE_TYPE );
	}
}
END_NAMESPACE_DATABASE_SQLITE
