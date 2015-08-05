/************************************************************************//**
* @file DatabaseSqlitePrerequisites.h
* @author Sylvain Doremus
* @version 1.0
* @date 03/14/2014 11:48:00 AM
*
*
* @brief Database SQLite prerequisite header.
*
* @details This file contains all Database SQLite prerequisite instructions.
*
***************************************************************************/

#ifndef ___DATABASE_SQLITE_PREREQUISITES_H___
#define ___DATABASE_SQLITE_PREREQUISITES_H___

#include <DatabasePrerequisites.h>

#define BEGIN_NAMESPACE_DATABASE_SQLITE BEGIN_NAMESPACE_DATABASE { namespace Sqlite
#define NAMESPACE_DATABASE_SQLITE NAMESPACE_DATABASE::Sqlite
#define END_NAMESPACE_DATABASE_SQLITE } END_NAMESPACE_DATABASE

BEGIN_NAMESPACE_DATABASE
{
#	include "sqlite3.h"
	namespace Sqlite
	{
		// Pre-declare classes
		// Allows use of pointers in header files without including individual .h
		// so decreases dependencies between files
		class CDatabaseConnectionSqlite;
		class CDatabaseSqlite;
		class CDatabaseStatementSqlite;
		class CDatabaseStatementParameterSqlite;
		class CDatabaseExceptionSqlite;
		class CPluginDatabaseSqlite;
		class CFactoryDatabaseSqlite;
		struct SSqliteBindingBase;

		// Pointers
		DECLARE_SMART_PTR( DatabaseConnectionSqlite );
		DECLARE_SMART_PTR( DatabaseStatementSqlite );
		DECLARE_SMART_PTR( DatabaseStatementParameterSqlite );

		// Factory constants
		const String FACTORY_DATABASE_SQLITE = STR( "Factory Database SQLite" );

		// Plugin constants
		const String DATABASE_SQLITE_TYPE = STR( "Database.SQLite" );
		const String PLUGIN_NAME_DATABASE_SQLITE = STR( "Plugin Database SQLite" );
	}
}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_SQLITE_PREREQUISITES_H___
