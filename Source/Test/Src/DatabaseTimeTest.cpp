/************************************************************************//**
* @file DatabaseTimeTest.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 12/02/2014 14:29:35
*
*
* @brief Class testing Time functions
*
***************************************************************************/

#include "DatabaseTestPch.h"

#if defined( TESTING_INTERFACE )

#include "DatabaseTimeTest.h"

#include "DatabaseTestHelpers.h"

BEGIN_NAMESPACE_DATABASE_TEST
{
	CDatabaseTimeTest::CDatabaseTimeTest()
	{
	}

	CDatabaseTimeTest::~CDatabaseTimeTest()
	{
	}

	boost::unit_test::test_suite * CDatabaseTimeTest::Init_Test_Suite()
	{
		//!@remarks Create the internal TS instance.
		testSuite = new boost::unit_test::test_suite( "CDatabaseTimeTest" );

		//!@remarks Add the TC to the internal TS.
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTimeTest::TestCase_FormatTime, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTimeTest::TestCase_PrintTime, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTimeTest::TestCase_IsTime, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTimeTest::TestCase_IsTimeWithTime, this ) ) );

		//!@remarks Return the TS instance.
		return testSuite;
	}

	void CDatabaseTimeTest::TestCase_FormatTime()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_FormatTime ****" );
		std::string sformat = "%H:%M:%S";
		std::wstring wformat = L"%H:%M:%S";

		CLogger::LogInfo( StringStream() << "  Normal case" );
		{
			TimeType time( 10, 8, 2 );
			BOOST_CHECK_EQUAL( Time::Format( time, sformat ), "10:08:02" );
			BOOST_CHECK_EQUAL( Time::Format( time, wformat ), L"10:08:02" );
		}
		CLogger::LogInfo( StringStream() << "  Invalid time (negative)" );
		{
			TimeType time( -1, -1, -1 );
			BOOST_CHECK_THROW( Time::Format( time, sformat ), std::out_of_range );
			BOOST_CHECK_THROW( Time::Format( time, wformat ), std::out_of_range );
		}
		CLogger::LogInfo( StringStream() << "  Invalid time (not a date time)" );
		{
			TimeType time( boost::gregorian::not_a_date_time );
			BOOST_CHECK_THROW( Time::Format( time, sformat ), std::out_of_range );
			BOOST_CHECK_THROW( Time::Format( time, wformat ), std::out_of_range );
		}

		CLogger::LogInfo( StringStream() << "**** End TestCase_FormatTime ****" );
	}

	void CDatabaseTimeTest::TestCase_PrintTime()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_PrintTime ****" );
		std::string sprint = "%02i:%02i:%02i";
		std::wstring wprint = L"%02i:%02i:%02i";

		CLogger::LogInfo( StringStream() << "  Normal case" );
		{
			TimeType time( 10, 8, 2 );
			BOOST_CHECK_EQUAL( Time::Print( time, sprint ), "10:08:02" );
			BOOST_CHECK_EQUAL( Time::Print( time, wprint ), L"10:08:02" );
		}
		CLogger::LogInfo( StringStream() << "  Invalid time (negative)" );
		{
			TimeType time( -1, -1, -1 );
			BOOST_CHECK_THROW( Time::Print( time, sprint ), std::out_of_range );
			BOOST_CHECK_THROW( Time::Print( time, wprint ), std::out_of_range );
		}
		CLogger::LogInfo( StringStream() << "  Invalid time (not a date time)" );
		{
			TimeType time( boost::gregorian::not_a_date_time );
			BOOST_CHECK_THROW( Time::Print( time, sprint ), std::out_of_range );
			BOOST_CHECK_THROW( Time::Print( time, wprint ), std::out_of_range );
		}

		CLogger::LogInfo( StringStream() << "**** End TestCase_PrintTime ****" );
	}

	void CDatabaseTimeTest::TestCase_IsTime()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_IsTime ****" );
		std::string sformat = "%H:%M:%S";
		std::wstring wformat = L"%H:%M:%S";

		CLogger::LogInfo( StringStream() << "  Normal case" );
		{
			BOOST_CHECK( Time::IsTime( "10:08:02", sformat ) );
			BOOST_CHECK( Time::IsTime( L"10:08:02", wformat ) );
		}
		CLogger::LogInfo( StringStream() << "  Invalid format" );
		{
			BOOST_CHECK( !Time::IsTime( "1008:02", sformat ) );
			BOOST_CHECK( !Time::IsTime( L"1008:02", wformat ) );
		}
		CLogger::LogInfo( StringStream() << "  Invalid hour" );
		{
			BOOST_CHECK( !Time::IsTime( "27:01:01", sformat ) );
			BOOST_CHECK( !Time::IsTime( L"27:01:01", wformat ) );
		}
		CLogger::LogInfo( StringStream() << "  Invalid minute" );
		{
			BOOST_CHECK( !Time::IsTime( "01:73:01", sformat ) );
			BOOST_CHECK( !Time::IsTime( L"01:73:01", wformat ) );
		}
		CLogger::LogInfo( StringStream() << "  Invalid second" );
		{
			BOOST_CHECK( !Time::IsTime( "01:01:73", sformat ) );
			BOOST_CHECK( !Time::IsTime( L"01:01:73", wformat ) );
		}

		CLogger::LogInfo( StringStream() << "**** End TestCase_IsTime ****" );
	}

	void CDatabaseTimeTest::TestCase_IsTimeWithTime()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_IsTimeWithTime ****" );
		std::string sformat = "%H:%M:%S";
		std::wstring wformat = L"%H:%M:%S";

		CLogger::LogInfo( StringStream() << "  Normal case" );
		{
			TimeType time;
			BOOST_CHECK( Time::IsTime( "01:01:01", sformat, time ) && !time.is_not_a_date_time() );
			BOOST_CHECK( Time::IsTime( L"01:01:01", wformat, time ) && !time.is_not_a_date_time() );
		}
		CLogger::LogInfo( StringStream() << "  Invalid format" );
		{
			TimeType time;
			BOOST_CHECK( !Time::IsTime( "0101:01", sformat, time ) && time.is_not_a_date_time() );
			BOOST_CHECK( !Time::IsTime( L"0101:01", wformat, time ) && time.is_not_a_date_time() );
		}
		CLogger::LogInfo( StringStream() << "  Invalid hour" );
		{
			TimeType time;
			BOOST_CHECK( !Time::IsTime( "27:01:01", sformat, time ) && time.is_not_a_date_time() );
			BOOST_CHECK( !Time::IsTime( L"27:01:01", wformat, time ) && time.is_not_a_date_time() );
		}
		CLogger::LogInfo( StringStream() << "  Invalid minute" );
		{
			TimeType time;
			BOOST_CHECK( !Time::IsTime( "01:73:01", sformat, time ) && time.is_not_a_date_time() );
			BOOST_CHECK( !Time::IsTime( L"01:73:01", wformat, time ) && time.is_not_a_date_time() );
		}
		CLogger::LogInfo( StringStream() << "  Invalid second" );
		{
			TimeType time;
			BOOST_CHECK( !Time::IsTime( "01:01:73", sformat, time ) && time.is_not_a_date_time() );
			BOOST_CHECK( !Time::IsTime( L"01:01:73", wformat, time ) && time.is_not_a_date_time() );
		}

		CLogger::LogInfo( StringStream() << "**** End TestCase_IsTimeWithTime ****" );
	}
}
END_NAMESPACE_DATABASE_TEST

#endif
