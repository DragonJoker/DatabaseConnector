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

#ifdef STATIC_LIB
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
#if defined( _WIN32 )
        static const NAMESPACE_DATABASE::TChar PATH_DELIM = STR( '\\' );
#else
        static const NAMESPACE_DATABASE::TChar PATH_DELIM = STR( '/' );
#endif
        
        String InitializeSingletons();
        void LoadPlugins( const String & path, bool mySql, bool odbcMySql, bool odbcMsSql );
        void UnloadPlugins();
        Database::CDatabase * CreateDatabase( const String & type );
        Database::DatabaseConnectionPtr CreateConnection( Database::CDatabase * database, const String & server, const String & name, const String & user, const String & pwd );
    }
}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_TEST_PREREQUISITES_H___
