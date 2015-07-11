/************************************************************************//**
 * @file DatabaseTestPch.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 02/11/2013 05:09:00 PM
 *
 *
 * @brief DatabaseTest precompiled header.
 *
 * @details This file contains all DatabaseTest precompiled header.
 *
 ***************************************************************************/

#include "DatabaseTestPch.h"

#include <DatabaseFactoryManager.h>
#include <DatabasePluginManager.h>

extern NAMESPACE_DATABASE::String g_path;

BEGIN_NAMESPACE_DATABASE_TEST
{
	static const String MYSQL_PLUGIN = STR( "DatabasePluginMySql" );
	static const String SQLITE_PLUGIN = STR( "DatabasePluginSqlite" );
	static const String ODBC_MYSQL_PLUGIN = STR( "DatabasePluginOdbcMySql" );
	static const String ODBC_MSSQL_PLUGIN = STR( "DatabasePluginOdbcMsSql" );

#if defined( _WIN32 )
	static const String LIB_PREFIX;
	static const String LIB_EXT = STR( ".dll" );
#else
	static const String LIB_PREFIX = STR( "lib" );
	static const String LIB_EXT = STR( ".so" );
#endif
#if defined( NDEBUG )
	static const String LIB_SUFFIX;
#else
	static const String LIB_SUFFIX = "d";
#endif
	static struct SPluginsConfig
	{
		String _path;
		bool _mySql;
		bool _sqlite;
		bool _odbcMySql;
		bool _odbcMsSql;
#if defined( STATIC_LIB )
		CTestPluginsStaticLoader _loader;
#endif
	} pluginsConfig;

	String GetLibName( const String & name )
	{
		return LIB_PREFIX + name + LIB_SUFFIX + LIB_EXT;
	}

	String InitializeSingletons()
	{
		String modulePath = g_path.substr( 0, g_path.size() - 1 );
		
#if !defined( WIN32 )
		size_t index = modulePath.rfind( STR( '/' ) );

		if ( index != String::npos )
		{
			modulePath = modulePath.substr( 0, index + 1 );
		}
		else
		{
			modulePath = STR( "../" );
		}

		modulePath += STR( "lib" );
#endif

		modulePath += NAMESPACE_DATABASE::PATH_SEP;

		CPluginManager::Instance().SetApplicationPath( modulePath );
		CPluginManager::Instance().SetPluginsPath( modulePath );
		CPluginManager::Instance().SetTranslationsPath( modulePath );

		return modulePath;
	}

	void LoadPlugins( const String & path, bool mySql, bool sqlite, bool odbcMySql, bool odbcMsSql )
	{
		pluginsConfig._path = path;
		pluginsConfig._mySql = mySql;
		pluginsConfig._sqlite = sqlite;
		pluginsConfig._odbcMySql = odbcMySql;
		pluginsConfig._odbcMsSql = odbcMsSql;
#if !defined( STATIC_LIB )

		if ( pluginsConfig._odbcMsSql )
		{
			CPluginManager::Instance().LoadPlugin( pluginsConfig._path + GetLibName( ODBC_MSSQL_PLUGIN ) );
		}

		if ( pluginsConfig._odbcMySql )
		{
			CPluginManager::Instance().LoadPlugin( pluginsConfig._path + GetLibName( ODBC_MYSQL_PLUGIN ) );
		}

		if ( pluginsConfig._sqlite )
		{
			CPluginManager::Instance().LoadPlugin( pluginsConfig._path + GetLibName( SQLITE_PLUGIN ) );
		}

		if ( pluginsConfig._mySql )
		{
			CPluginManager::Instance().LoadPlugin( pluginsConfig._path + GetLibName( MYSQL_PLUGIN ) );
		}

#else
		pluginsConfig._loader.Load( mySql, odbcMySql, odbcMsSql );
#endif
	}

	void UnloadPlugins()
	{
#if !defined( STATIC_LIB )

		if ( pluginsConfig._odbcMsSql )
		{
			CPluginManager::Instance().UnloadPlugin( pluginsConfig._path + GetLibName( ODBC_MSSQL_PLUGIN ) );
		}

		if ( pluginsConfig._odbcMySql )
		{
			CPluginManager::Instance().UnloadPlugin( pluginsConfig._path + GetLibName( ODBC_MYSQL_PLUGIN ) );
		}

		if ( pluginsConfig._sqlite )
		{
			CPluginManager::Instance().UnloadPlugin( pluginsConfig._path + GetLibName( SQLITE_PLUGIN ) );
		}

		if ( pluginsConfig._mySql )
		{
			CPluginManager::Instance().UnloadPlugin( pluginsConfig._path + GetLibName( MYSQL_PLUGIN ) );
		}

#else
		pluginsConfig._loader.Unload();
#endif
	}

	CDatabase * InstantiateDatabase( const String & type )
	{
		return static_cast< CDatabase * >( CFactoryManager::Instance().CreateInstance( type ) );
	}

	DatabaseConnectionPtr CreateConnection( CDatabase & database, const String & server, const String & user, const String & pwd )
	{
		String connectionResult;
		database.Initialize( server, user, pwd );
		database.CreateConnection( connectionResult );
		DatabaseConnectionPtr  result = database.RetrieveConnection();
		return result;
	}
}
END_NAMESPACE_DATABASE_TEST
