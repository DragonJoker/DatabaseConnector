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

void Startup()
{
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

#if STATIC_LIB
/** Main Entry Point in static mode.
@param  argc    Arguments number
@param  argv    Aruments' list
@return test_suite  Test Suite Pointer
*/
boost::unit_test::test_suite * init_unit_test_suite( int argc, char * argv[] )
{
	g_path = NAMESPACE_DATABASE::CStrUtils::ToString( argv[0] );
	NAMESPACE_DATABASE::CStrUtils::Replace( g_path, STR( '\\' ), NAMESPACE_DATABASE_TEST::PATH_DELIM );
	NAMESPACE_DATABASE::CStrUtils::Replace( g_path, STR( '/' ), NAMESPACE_DATABASE_TEST::PATH_DELIM );
	g_path = g_path.substr( 0, g_path.rfind( NAMESPACE_DATABASE_TEST::PATH_DELIM ) + 1 );

	Startup();

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
	g_path = NAMESPACE_DATABASE::CStrUtils::ToString( argv[0] );
	NAMESPACE_DATABASE::CStrUtils::Replace( g_path, STR( '\\' ), NAMESPACE_DATABASE::PATH_SEP );
	NAMESPACE_DATABASE::CStrUtils::Replace( g_path, STR( '/' ), NAMESPACE_DATABASE::PATH_SEP );
	g_path = g_path.substr( 0, g_path.rfind( NAMESPACE_DATABASE::PATH_SEP ) + 1 );
	srand( uint32_t( time( NULL ) ) );

	Startup();

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
		TS_List.push_back( DatabaseMySqlTest->Init_Test_Suite() );
		//TS_List.push_back( DatabaseSqliteTest->Init_Test_Suite() );
		//TS_List.push_back( DatabaseOdbcMySqlTest->Init_Test_Suite() );
		//TS_List.push_back( DatabaseOdbcMsSqlTest->Init_Test_Suite() );

		///@remarks Add the TS' sequences into the Master TS
		for ( TestVector::iterator it = TS_List.begin() ; it != TS_List.end(); ++it )
		{
			boost::unit_test::framework::master_test_suite().add( *it );
		}
	}
}
END_NAMESPACE_DATABASE_TEST
