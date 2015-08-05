/************************************************************************//**
 * @file DatabaseNullableTest.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 12/02/2014 14:29:35
 *
 *
 * @brief Class testing CDatabaseNullable class
*
***************************************************************************/

#include "DatabaseTestPch.h"

#include "DatabaseNullableTest.h"

#include "DatabaseTestHelpers.h"

BEGIN_NAMESPACE_DATABASE_TEST
{
	CDatabaseNullableTest::CDatabaseNullableTest()
	{
	}

	CDatabaseNullableTest::~CDatabaseNullableTest()
	{
	}

	boost::unit_test::test_suite * CDatabaseNullableTest::Init_Test_Suite()
	{
		//!@remarks Create the internal TS instance.
		testSuite = new boost::unit_test::test_suite( "CDatabaseNullableTest" );

		//!@remarks Add the TC to the internal TS.
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseNullableTest::TestCase_DatabaseNullable, this ) ) );

		//!@remarks Return the TS instance.
		return testSuite;
	}

	void CDatabaseNullableTest::TestCase_DatabaseNullable()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseNullable ****" );

		CDatabaseNullable< int32_t > value;
		CLogger::LogInfo( StringStream() << "  Unset" );
		BOOST_CHECK( !value );
		BOOST_CHECK_THROW( *value, CDatabaseException );
		CLogger::LogInfo( StringStream() << "  Set to value" );
		value = 42;
		BOOST_CHECK( ( bool )value );
		BOOST_CHECK_EQUAL( *value, 42 );
		CLogger::LogInfo( StringStream() << "  Set to None" );
		value = None;
		BOOST_CHECK( !value );
		BOOST_CHECK_THROW( *value, CDatabaseException );

		CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseNullable ****" );
	}
}
END_NAMESPACE_DATABASE_TEST
