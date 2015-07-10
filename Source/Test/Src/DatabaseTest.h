/************************************************************************//**
 * @file DatabaseTest.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 12/02/2014 14:28:15
 *
*
* @brief Main application for boost tests with Suites.
*
* @details This is the implementation to include tests in Boost suites.
*
***************************************************************************/

#include "DatabaseTestPrerequisites.h"

#include <vector>
#include <boost/test/unit_test_suite.hpp>

BEGIN_NAMESPACE_DATABASE_TEST
{
	typedef std::vector< boost::unit_test::test_suite * > TestVector; //!< Typedef to a vector of test_suite
	TestVector TS_List; //!<List of all TS which will be included into the Master TS.

#ifdef BOOST_STATIC_LIB
	/** Main Init Function for the framework's Suite.
	    @return true if executed successfully else false if an error occured.
	*/
	boost::unit_test::test_suite * init_unit_test_suite( int argc, char * argv[] );
#else
	/** Main Init Function for the framework's Suite.
	    @return Test Suite Pointer
	*/
	bool boost_main_init_function();
#endif

	/** Build and Execute the test suite. */
	void Tests_Creation();
}
END_NAMESPACE_DATABASE_TEST
