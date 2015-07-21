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
#include "DatabasePostgreSqlTest.h"
#include "DatabaseOdbcMySqlTest.h"
#include "DatabaseOdbcMsSqlTest.h"
#include "DatabaseTestPluginsStaticLoader.h"

#include <boost/test/unit_test.hpp>

NAMESPACE_DATABASE::String g_path;

std::unique_ptr< NAMESPACE_DATABASE_TEST::CDatabaseMySqlTest > g_databaseMySqlTest; //!<A pointer to the CDatabaseMySqlTest class.
std::unique_ptr< NAMESPACE_DATABASE_TEST::CDatabaseSqliteTest > g_databaseSqliteTest; //!<A pointer to the CDatabasePluginSqlite class.
std::unique_ptr< NAMESPACE_DATABASE_TEST::CDatabasePostgreSqlTest > g_databasePostgreSqlTest; //!<A pointer to the CDatabasePostgreSqlTest class.
std::unique_ptr< NAMESPACE_DATABASE_TEST::CDatabaseOdbcMySqlTest > g_databaseOdbcMySqlTest; //!<A pointer to the CDatabaseOdbcMySqlTest class.
std::unique_ptr< NAMESPACE_DATABASE_TEST::CDatabaseOdbcMsSqlTest > g_databaseOdbcMsSqlTest; //!<A pointer to the CDatabaseOdbcMsSqlTest class.
std::unique_ptr< NAMESPACE_DATABASE_TEST::CTestPluginsLoader > g_pluginsLoader;

void Startup( char * arg )
{
	g_path = NAMESPACE_DATABASE::CStrUtils::ToString( arg );
	NAMESPACE_DATABASE::CStrUtils::Replace( g_path, STR( '\\' ), NAMESPACE_DATABASE::PATH_SEP );
	NAMESPACE_DATABASE::CStrUtils::Replace( g_path, STR( '/' ), NAMESPACE_DATABASE::PATH_SEP );
	g_path = g_path.substr( 0, g_path.rfind( NAMESPACE_DATABASE::PATH_SEP ) + 1 );
	srand( uint32_t( time( NULL ) ) );

	// Configure logger
#if defined( NDEBUG )
	NAMESPACE_DATABASE::CLogger::Initialise( NAMESPACE_DATABASE::ELogType_INFO );
#else
	NAMESPACE_DATABASE::CLogger::Initialise( NAMESPACE_DATABASE::ELogType_DEBUG );
#endif
	NAMESPACE_DATABASE::CLogger::SetFileName( g_path + STR( "DatabaseTest.log" ) );

	g_databaseMySqlTest = std::make_unique< NAMESPACE_DATABASE_TEST::CDatabaseMySqlTest >();
	g_databaseSqliteTest = std::make_unique< NAMESPACE_DATABASE_TEST::CDatabaseSqliteTest >();
	g_databasePostgreSqlTest = std::make_unique< NAMESPACE_DATABASE_TEST::CDatabasePostgreSqlTest >();
	g_databaseOdbcMySqlTest = std::make_unique< NAMESPACE_DATABASE_TEST::CDatabaseOdbcMySqlTest >();
	g_databaseOdbcMsSqlTest = std::make_unique< NAMESPACE_DATABASE_TEST::CDatabaseOdbcMsSqlTest >();
	g_pluginsLoader = std::make_unique< NAMESPACE_DATABASE_TEST::CTestPluginsLoader >();
}

void Shutdown()
{
	g_pluginsLoader.reset();
	g_databaseMySqlTest.reset();
	g_databaseSqliteTest.reset();
	g_databasePostgreSqlTest.reset();
	g_databaseOdbcMySqlTest.reset();
	g_databaseOdbcMsSqlTest.reset();
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
	//!@remarks Exit the test application.
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

	//!@remarks Master TS initialization.
	boost::unit_test::master_test_suite_t & masterTestSuite = boost::unit_test::framework::master_test_suite();
	boost::unit_test::init_unit_test_func();

	//!@remarks Master TS Execution.
	boost::unit_test::unit_test_main( &NAMESPACE_DATABASE_TEST::boost_main_init_function, argc, argv );

	Shutdown();

	//!@remarks Exit the test application.
	return 0;
}

BEGIN_NAMESPACE_DATABASE_TEST
{
	bool boost_main_init_function()
	{
		//!@remarks execute the test suite.
		Tests_Creation();

		//!@remarks Exit the Master TS initialization.
		return true;
	}
}
END_NAMESPACE_DATABASE_TEST
#endif

BEGIN_NAMESPACE_DATABASE_TEST
{
	void Tests_Creation()
	{
		//!@remarks Clear the TS' List
		TS_List.clear();

		//!@remarks Create the TS' sequences
#if defined( TESTING_PLUGIN_SQLITE )
		//TS_List.push_back( g_databaseSqliteTest->Init_Test_Suite() );
#endif
#if defined( TESTING_PLUGIN_MYSQL )
		TS_List.push_back( g_databaseMySqlTest->Init_Test_Suite() );
#endif
#if defined( TESTING_PLUGIN_POSTGRE )
		//TS_List.push_back( g_databasePostgreSqlTest->Init_Test_Suite() );
#endif
#if defined( TESTING_PLUGIN_ODBC )
		//TS_List.push_back( g_databaseOdbcMySqlTest->Init_Test_Suite() );
		//TS_List.push_back( g_databaseOdbcMsSqlTest->Init_Test_Suite() );
#endif

		//!@remarks Add the TS' sequences into the Master TS
		for ( auto suite : TS_List )
		{
			boost::unit_test::framework::master_test_suite().add( suite );
		}
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

	static const String MYSQL_PLUGIN = STR( "DatabasePluginMySql" );
	static const String SQLITE_PLUGIN = STR( "DatabasePluginSqlite" );
	static const String ODBC_MYSQL_PLUGIN = STR( "DatabasePluginOdbcMySql" );
	static const String ODBC_MSSQL_PLUGIN = STR( "DatabasePluginOdbcMsSql" );
	static const String POSTGRESQL_PLUGIN = STR( "DatabasePluginPostgreSql" );

	void LoadPlugins( const String & path, bool mySql, bool sqlite, bool odbcMySql, bool odbcMsSql, bool postgreSql )
	{
		SPluginsConfig pluginsConfig;
#if !defined( STATIC_LIB )
		pluginsConfig._mySql = std::make_unique< CPluginConfig >( mySql, path, MYSQL_PLUGIN );
		pluginsConfig._sqlite = std::make_unique< CPluginConfig >( sqlite, path, SQLITE_PLUGIN );
		pluginsConfig._odbcMySql = std::make_unique< CPluginConfig >( odbcMySql, path, ODBC_MYSQL_PLUGIN );
		pluginsConfig._odbcMsSql = std::make_unique< CPluginConfig >( odbcMsSql, path, ODBC_MSSQL_PLUGIN );
		pluginsConfig._postgreSql = std::make_unique< CPluginConfig >( postgreSql, path, POSTGRESQL_PLUGIN );
#else
		pluginsConfig._mySql = std::make_unique< CPluginConfig< MySql::CDatabasePluginMySql > >( mySql );
		pluginsConfig._sqlite = std::make_unique< CPluginConfig< Sqlite::CDatabasePluginSqlite > >( sqlite );
		pluginsConfig._odbcMySql = std::make_unique< CPluginConfig< Odbc::MySql::CDatabasePluginOdbcMySql > >( odbcMySql );
		pluginsConfig._odbcMsSql = std::make_unique< CPluginConfig< Odbc::MsSql::CDatabasePluginOdbcMsSql > >( odbcMsSql );
		pluginsConfig._postgreSql = std::make_unique< CPluginConfig< PostgreSql::CDatabasePluginPostgreSql > >( postgreSql );
#endif
		g_pluginsLoader->Load( std::move( pluginsConfig ) );
	}

	void UnloadPlugins()
	{
		g_pluginsLoader->Unload();
	}

	CDatabase * InstantiateDatabase( const String & type )
	{
		return static_cast< CDatabase * >( CFactoryManager::Instance().CreateInstance( type ) );
	}

	DatabaseConnectionSPtr CreateConnection( CDatabase & database, const String & server, const String & user, const String & pwd )
	{
		String connectionResult;
		database.Initialize( server, user, pwd );
		database.CreateConnection( connectionResult );
		DatabaseConnectionSPtr  result = database.RetrieveConnection();
		return result;
	}
}
END_NAMESPACE_DATABASE_TEST
