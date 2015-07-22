/************************************************************************//**
* @file DatabaseSqlite.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDatabaseSqlite class declaration.
*
* @details Describes an SQLITE database.
*
***************************************************************************/

#include "DatabaseSqlitePch.h"

#include "DatabaseSqlite.h"
#include "DatabaseConnectionSqlite.h"

#include <DatabaseFieldInfos.h>

BEGIN_NAMESPACE_DATABASE_SQLITE
{
	CDatabaseSqlite::CDatabaseSqlite()
		: CDatabase()
	{
	}

	CDatabaseSqlite::~CDatabaseSqlite()
	{
	}

	const String & CDatabaseSqlite::GetType()const
	{
		return DATABASE_SQLITE_TYPE;
	}

	CDatabase * CDatabaseSqlite::Create()
	{
		return new CDatabaseSqlite;
	}

	DatabaseConnectionSPtr CDatabaseSqlite::DoCreateConnection( String & connectionString ) const
	{
		return std::make_shared< CDatabaseConnectionSqlite >( _server, _userName, _password, connectionString );
	}
}
END_NAMESPACE_DATABASE_SQLITE
