/************************************************************************//**
 * @file DatabaseInterfaceTest.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 12/02/2014 14:29:35
 *
 *
 * @brief Class testing all Interface members
*
***************************************************************************/

#include "DatabaseTestPch.h"

#include "DatabaseInterfaceTest.h"

BEGIN_NAMESPACE_DATABASE_TEST
{
	CDatabaseInterfaceTest::CDatabaseInterfaceTest()
	{
	}

	CDatabaseInterfaceTest::~CDatabaseInterfaceTest()
	{
	}

	boost::unit_test::test_suite * CDatabaseInterfaceTest::Init_Test_Suite()
	{
		//!@remarks Create the internal TS instance.
		testSuite = new boost::unit_test::test_suite( "CDatabaseInterfaceTest" );

		//!@remarks Add the TC to the internal TS.
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseInterfaceTest::TestCase_DatabaseQuery, this ) ) );

		//!@remarks Return the TS instance.
		return testSuite;
	}

	void CDatabaseInterfaceTest::TestCase_DatabaseQuery()
	{
		//CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseQuery ****" );



		//CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseQuery ****" );
	}
}
END_NAMESPACE_DATABASE_TEST
