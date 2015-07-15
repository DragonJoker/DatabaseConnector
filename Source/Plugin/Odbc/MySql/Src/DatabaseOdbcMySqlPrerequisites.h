/************************************************************************//**
* @file DatabaseOdbcMySqlPrerequisites.h
* @author Sylvain Doremus
* @version 1.0
* @date 03/14/2014 11:48:00 AM
*
*
* @brief Database ODBC prerequisite header.
*
* @details This file contains all Database ODBC prerequisite instructions.
*
***************************************************************************/

#ifndef ___DATABASE_ODBC_MYSQL_PREREQUISITES_H___
#define ___DATABASE_ODBC_MYSQL_PREREQUISITES_H___

#include <DatabaseOdbcPrerequisites.h>

#if defined( _WIN32 ) && !defined ( __MINGW32__ ) && !defined ( STATIC_LIB )
#	ifdef DatabasePluginOdbcMySql_EXPORTS
#	    define DatabaseOdbcMySqlExport __declspec ( dllexport )
#	else
#		if defined ( __MINGW32__ )
#			define DatabaseOdbcMySqlExport
#		else
#			define DatabaseOdbcMySqlExport __declspec ( dllimport )
#		endif
#	endif
#else
#    define DatabaseOdbcMySqlExport
#endif

#if defined( _WIN32 )
#	include <windows.h>
#	undef min
#	undef max
#	undef abs
#endif

#include <sql.h>
#include <sqlext.h>

#define BEGIN_NAMESPACE_DATABASE_ODBC_MYSQL BEGIN_NAMESPACE_DATABASE_ODBC { namespace MySql
#define NAMESPACE_DATABASE_ODBC_MYSQL NAMESPACE_DATABASE_ODBC::MySql
#define END_NAMESPACE_DATABASE_ODBC_MYSQL } END_NAMESPACE_DATABASE_ODBC

BEGIN_NAMESPACE_DATABASE_ODBC
{
	namespace MySql
	{
		// Pre-declare classes
		// Allows use of pointers in header files without including individual .h
		// so decreases dependencies between files
		class CDatabaseConnectionOdbcMySql;
		class CPluginDatabaseOdbcMySql;
		class CFactoryDatabaseOdbcMySql;

		// Pointers
		typedef std::shared_ptr< CDatabaseConnectionOdbcMySql > DatabaseConnectionOdbcMySqlPtr;

		// Factory constants
		const String FACTORY_DATABASE_ODBC_MYSQL = STR( "Factory Database Odbc MySql" );

		// Plugin constants
		const String DATABASE_ODBC_MYSQL_TYPE = STR( "Database.Odbc.MySql" );
		const String PLUGIN_NAME_DATABASE_ODBC_MYSQL = STR( "Plugin Database Odbc MySql" );
	}
}
END_NAMESPACE_DATABASE_ODBC

#endif // ___DATABASE_ODBC_MYSQL_PREREQUISITES_H___
