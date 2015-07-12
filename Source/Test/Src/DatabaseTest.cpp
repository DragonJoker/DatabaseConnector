/************************************************************************//**
 * @file DatabaseTest.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 12/02/2014 14:29:35
 *
 *
 * @brief Main application for boost tests with Suites.
 *
 * @details This is the implementation to include tests in Boost suites.
 *
 ***************************************************************************/

#include "DatabaseTestPch.h"

#include "DatabaseTest.h"
#include "DatabaseMySqlTest.h"
#include "DatabaseSqliteTest.h"
#include "DatabaseOdbcMySqlTest.h"
#include "DatabaseOdbcMsSqlTest.h"
#include "DatabaseTestPluginsStaticLoader.h"

#include <boost/test/unit_test.hpp>

NAMESPACE_DATABASE::String g_path;

std::shared_ptr< NAMESPACE_DATABASE_TEST::CDatabaseMySqlTest > DatabaseMySqlTest; //!<A shared pointer to the CDatabaseMySqlTest class.
std::shared_ptr< NAMESPACE_DATABASE_TEST::CDatabaseSqliteTest > DatabaseSqliteTest; //!<A shared pointer to the CDatabasePluginSqlite class.
std::shared_ptr< NAMESPACE_DATABASE_TEST::CDatabaseOdbcMySqlTest > DatabaseOdbcMySqlTest; //!<A shared pointer to the CDatabaseOdbcMySqlTest class.
std::shared_ptr< NAMESPACE_DATABASE_TEST::CDatabaseOdbcMsSqlTest > DatabaseOdbcMsSqlTest; //!<A shared pointer to the CDatabaseOdbcMsSqlTest class.

void Startup( char * arg )
{
	g_path = NAMESPACE_DATABASE::CStrUtils::ToString( arg );
	NAMESPACE_DATABASE::CStrUtils::Replace( g_path, STR( '\\' ), NAMESPACE_DATABASE::PATH_SEP );
	NAMESPACE_DATABASE::CStrUtils::Replace( g_path, STR( '/' ), NAMESPACE_DATABASE::PATH_SEP );
	g_path = g_path.substr( 0, g_path.rfind( NAMESPACE_DATABASE::PATH_SEP ) + 1 );
	srand( uint32_t( time( NULL ) ) );

	// Configure logger
#if defined( NDEBUG )
	NAMESPACE_DATABASE::CLogger::Initialise( NAMESPACE_DATABASE::eLOG_TYPE_MESSAGE );
#else
	NAMESPACE_DATABASE::CLogger::Initialise( NAMESPACE_DATABASE::eLOG_TYPE_DEBUG );
#endif
	NAMESPACE_DATABASE::CLogger::SetFileName( g_path + STR( "DatabaseTest.log" ) );

	DatabaseMySqlTest = std::make_shared< NAMESPACE_DATABASE_TEST::CDatabaseMySqlTest >();
	DatabaseSqliteTest = std::make_shared< NAMESPACE_DATABASE_TEST::CDatabaseSqliteTest >();
	DatabaseOdbcMySqlTest = std::make_shared< NAMESPACE_DATABASE_TEST::CDatabaseOdbcMySqlTest >();
	DatabaseOdbcMsSqlTest = std::make_shared< NAMESPACE_DATABASE_TEST::CDatabaseOdbcMsSqlTest >();
}

void Shutdown()
{
	DatabaseMySqlTest.reset();
	DatabaseSqliteTest.reset();
	DatabaseOdbcMySqlTest.reset();
	DatabaseOdbcMsSqlTest.reset();
	NAMESPACE_DATABASE::CLogger::Cleanup();
}

#if BOOST_STATIC_LIB
/** Main Entry Point in static mode.
@param  argc    Arguments number
@param  argv    Aruments' list
@return test_suite  Test Suite Pointer
*/
boost::unit_test::test_suite * init_unit_test_suite( int argc, char * argv[] )
{
	Startup( argv[0] );

	NAMESPACE_DATABASE_TEST::Tests_Creation();

	Shutdown();
	///@remarks Exit the test application.
	return 0;
}
#else
/** Main Entry Point in dynamic mode.
@param  argc    Arguments number
@param  argv    Arguments' list
@return int     Execution return code
*/
int main( int argc, char * argv[] )
{
	Startup( argv[0] );

	///@remarks Master TS initialization.
	boost::unit_test::master_test_suite_t & masterTestSuite = boost::unit_test::framework::master_test_suite();
	boost::unit_test::init_unit_test_func();

	///@remarks Master TS Execution.
	boost::unit_test::unit_test_main( &NAMESPACE_DATABASE_TEST::boost_main_init_function, argc, argv );

	Shutdown();

	///@remarks Exit the test application.
	return 0;
}

BEGIN_NAMESPACE_DATABASE_TEST
{
	bool boost_main_init_function()
	{
		///@remarks execute the test suite.
		Tests_Creation();

		///@remarks Exit the Master TS initialization.
		return true;
	}
}
END_NAMESPACE_DATABASE_TEST
#endif

BEGIN_NAMESPACE_DATABASE_TEST
{
	void Tests_Creation()
	{
		///@remarks Clear the TS' List
		TS_List.clear();

		///@remarks Create the TS' sequences
#if defined( TESTING_PLUGIN_MYSQL )
		//TS_List.push_back( DatabaseMySqlTest->Init_Test_Suite() );
#endif
#if defined( TESTING_PLUGIN_SQLITE )
		//TS_List.push_back( DatabaseSqliteTest->Init_Test_Suite() );
#endif
#if defined( TESTING_PLUGIN_ODBC )
		TS_List.push_back( DatabaseOdbcMySqlTest->Init_Test_Suite() );
		//TS_List.push_back( DatabaseOdbcMsSqlTest->Init_Test_Suite() );
#endif

		///@remarks Add the TS' sequences into the Master TS
		for ( auto suite : TS_List )
		{
			boost::unit_test::framework::master_test_suite().add( suite );
		}
	}
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
		StringStream modulePath;
		size_t index = g_path.substr( 0, g_path.size() - 1 ).rfind( PATH_SEP );

		if ( index != String::npos )
		{
			modulePath << g_path.substr( 0, index + 1 );
		}
		else
		{
			modulePath << STR( ".." ) << PATH_SEP;
		}

		modulePath << STR( "lib" ) << PATH_SEP << STR( "DatabaseConnector" ) << PATH_SEP;
		String result = modulePath.str();

		CPluginManager::Instance().SetApplicationPath( result );
		CPluginManager::Instance().SetPluginsPath( result );
		CPluginManager::Instance().SetTranslationsPath( result );

		return result;
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
