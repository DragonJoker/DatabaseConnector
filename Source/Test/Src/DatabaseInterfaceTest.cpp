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
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseInterfaceTest::TestCase_DatabaseDate, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseInterfaceTest::TestCase_DatabaseDateTime, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseInterfaceTest::TestCase_DatabaseTime, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseInterfaceTest::TestCase_DatabaseDateTimeSpan, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseInterfaceTest::TestCase_DatabaseFixedPoint, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseInterfaceTest::TestCase_DatabaseInt24, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseInterfaceTest::TestCase_DatabaseNullable, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseInterfaceTest::TestCase_DatabaseStringUtils, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseInterfaceTest::TestCase_DatabaseConnection, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseInterfaceTest::TestCase_DatabaseFieldInfos, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseInterfaceTest::TestCase_DatabaseValue, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseInterfaceTest::TestCase_DatabaseValuedObject, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseInterfaceTest::TestCase_DatabaseParameter, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseInterfaceTest::TestCase_DatabaseField, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseInterfaceTest::TestCase_DatabaseRow, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseInterfaceTest::TestCase_DatabaseResult, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseInterfaceTest::TestCase_DatabaseQuery, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseInterfaceTest::TestCase_DatabaseStatement, this ) ) );

		//!@remarks Return the TS instance.
		return testSuite;
	}

	void CDatabaseInterfaceTest::TestCase_DatabaseDate()
	{
		//CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseDate ****" );



		//CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseDate ****" );
	}

	void CDatabaseInterfaceTest::TestCase_DatabaseDateTime()
	{
		//CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseDateTime ****" );



		//CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseDateTime ****" );
	}

	void CDatabaseInterfaceTest::TestCase_DatabaseTime()
	{
		//CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseTime ****" );



		//CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseTime ****" );
	}

	void CDatabaseInterfaceTest::TestCase_DatabaseDateTimeSpan()
	{
		//CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseDateTimeSpan ****" );



		//CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseDateTimeSpan ****" );
	}

	void CDatabaseInterfaceTest::TestCase_DatabaseFixedPoint()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseFixedPoint ****" );



		CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseFixedPoint ****" );
	}

	void CDatabaseInterfaceTest::TestCase_DatabaseInt24()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseInt24 ****" );



		CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseInt24 ****" );
	}

	void CDatabaseInterfaceTest::TestCase_DatabaseNullable()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseNullable ****" );



		CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseNullable ****" );
	}

	void CDatabaseInterfaceTest::TestCase_DatabaseStringUtils()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseStringUtils ****" );



		CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseStringUtils ****" );
	}

	void CDatabaseInterfaceTest::TestCase_DatabaseConnection()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseConnection ****" );



		CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseConnection ****" );
	}

	void CDatabaseInterfaceTest::TestCase_DatabaseFieldInfos()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseFieldInfos ****" );



		CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseFieldInfos ****" );
	}

	void CDatabaseInterfaceTest::TestCase_DatabaseValue()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseValue ****" );



		CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseValue ****" );
	}

	void CDatabaseInterfaceTest::TestCase_DatabaseValuedObject()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseValuedObject ****" );



		CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseValuedObject ****" );
	}

	void CDatabaseInterfaceTest::TestCase_DatabaseParameter()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseParameter ****" );



		CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseParameter ****" );
	}

	void CDatabaseInterfaceTest::TestCase_DatabaseField()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseField ****" );



		CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseField ****" );
	}

	void CDatabaseInterfaceTest::TestCase_DatabaseRow()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseRow ****" );



		CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseRow ****" );
	}

	void CDatabaseInterfaceTest::TestCase_DatabaseResult()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseResult ****" );



		CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseResult ****" );
	}

	void CDatabaseInterfaceTest::TestCase_DatabaseQuery()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseQuery ****" );



		CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseQuery ****" );
	}

	void CDatabaseInterfaceTest::TestCase_DatabaseStatement()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseStatement ****" );



		CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseStatement ****" );
	}
}
END_NAMESPACE_DATABASE_TEST
