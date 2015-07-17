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

#if defined( _WIN32 ) && !defined ( __MINGW32__ ) && !defined ( STATIC_LIB )
#	ifdef DatabasePluginSqlite_EXPORTS
#	    define DatabaseSqliteExport __declspec ( dllexport )
#	else
#		if defined ( __MINGW32__ )
#			define DatabaseSqliteExport
#		else
#			define DatabaseSqliteExport __declspec ( dllimport )
#		endif
#	endif
#else
#    define DatabaseSqliteExport
#endif

#if defined( STATIC_LIB )
#	define CPPCONN_PUBLIC_FUNC
#endif

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
		class CExceptionDatabaseSqlite;
		class CPluginDatabaseSqlite;
		class CFactoryDatabaseSqlite;
		struct SSqliteBindingBase;

		// Pointers
		typedef std::shared_ptr< CDatabaseConnectionSqlite > DatabaseConnectionSqlitePtr;
		typedef std::shared_ptr< CDatabaseStatementSqlite > DatabaseStatementSqlitePtr;
		typedef std::shared_ptr< CDatabaseStatementParameterSqlite > DatabaseStatementParameterSqlitePtr;

		// Factory constants
		const String FACTORY_DATABASE_SQLITE = STR( "Factory Database SQLite" );

		// Plugin constants
		const String DATABASE_SQLITE_TYPE = STR( "Database.SQLite" );
		const String PLUGIN_NAME_DATABASE_SQLITE = STR( "Plugin Database SQLite" );
	}
}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_SQLITE_PREREQUISITES_H___
