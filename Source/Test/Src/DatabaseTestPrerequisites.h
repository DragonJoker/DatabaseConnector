/************************************************************************//**
 * @file DatabaseTestPrerequisites.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 02/11/2013 05:09:00 PM
 *
 *
 * @brief DatabaseTest prerequisite header.
 *
 * @details This file contains all DatabaseTest prerequisite instructions.
 *
 ***************************************************************************/

#ifndef ___DATABASE_TEST_PREREQUISITES_H___
#define ___DATABASE_TEST_PREREQUISITES_H___

#include <DatabasePrerequisites.h>
#include <Database.h>
#include <DatabaseConnection.h>

#define BEGIN_NAMESPACE_DATABASE_TEST   BEGIN_NAMESPACE_DATABASE { namespace Test
#define NAMESPACE_DATABASE_TEST         NAMESPACE_DATABASE::Test
#define END_NAMESPACE_DATABASE_TEST     END_NAMESPACE_DATABASE }

#ifdef BOOST_STATIC_LIB
#   define BOOST_TEST_MAIN
#else
#   define BOOST_TEST_NO_MAIN
#endif

#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/framework.hpp>
#include <boost/test/execution_monitor.hpp>
#include <boost/test/debug.hpp>
#include <boost/test/unit_test_suite_impl.hpp>
#include <boost/test/unit_test_log.hpp>
#include <boost/test/unit_test_monitor.hpp>
#include <boost/test/test_observer.hpp>
#include <boost/test/results_collector.hpp>
#include <boost/test/progress_monitor.hpp>
#include <boost/test/results_reporter.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/test/detail/unit_test_parameters.hpp>
#include <boost/test/detail/global_typedef.hpp>

BEGIN_NAMESPACE_DATABASE
{
	namespace Test
	{
		String InitializeSingletons();
		void LoadPlugins( const String & path, bool mySql, bool sqlite, bool odbcMySql, bool odbcMsSql );
		void UnloadPlugins();
		CDatabase * InstantiateDatabase( const String & type );
		DatabaseConnectionPtr CreateConnection( CDatabase & database, const String & server, const String & user, const String & pwd );
		
		extern String const QUERY_DROP_TABLE;
		extern String const QUERY_SELECT_MIN;
		extern String const QUERY_GET_COUNT;
		extern String const QUERY_INSERT_ELEMENT;
		extern String const QUERY_DIRECT_SELECT_ELEMENT;
		extern String const QUERY_DIRECT_SELECT_ALL_ELEMENTS;
		extern String const QUERY_DIRECT_UPDATE_ELEMENT;
		extern String const QUERY_DIRECT_DELETE_ELEMENT;
	}
}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_TEST_PREREQUISITES_H___
