/************************************************************************//**
 * @file DatabaseDateTimeTest.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 12/02/2014 14:29:35
 *
 *
 * @brief Class testing DateTime functions
*
***************************************************************************/

#include "DatabaseTestPch.h"

#include "DatabaseDateTimeTest.h"

#include "DatabaseTestHelpers.h"

BEGIN_NAMESPACE_DATABASE_TEST
{
	CDatabaseDateTimeTest::CDatabaseDateTimeTest()
	{
	}

	CDatabaseDateTimeTest::~CDatabaseDateTimeTest()
	{
	}

	boost::unit_test::test_suite * CDatabaseDateTimeTest::Init_Test_Suite()
	{
		//!@remarks Create the internal TS instance.
		testSuite = new boost::unit_test::test_suite( "CDatabaseInterfaceTest", __FILE__, __LINE__ );

		//!@remarks Add the TC to the internal TS.
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseDateTimeTest::TestCase_FormatDateTime, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseDateTimeTest::TestCase_PrintDateTime, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseDateTimeTest::TestCase_IsDateTime, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseDateTimeTest::TestCase_IsDateTimeWithDateTime, this ) ) );

		//!@remarks Return the TS instance.
		return testSuite;
	}

	void CDatabaseDateTimeTest::TestCase_FormatDateTime()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_FormatDateTime ****" );
		std::string sformat = "%d/%m/%Y %H:%M:%S";
		std::wstring wformat = L"%d/%m/%Y %H:%M:%S";

		CLogger::LogInfo( StringStream() << "  Normal case" );
		{
			DateTimeType date( DateType( 2015, 10, 7 ), TimeType( 10, 8, 2 ) );
			BOOST_CHECK_EQUAL( DateTime::Format( date, sformat ), "07/10/2015 10:08:02" );
			BOOST_CHECK_EQUAL( DateTime::Format( date, wformat ), L"07/10/2015 10:08:02" );
		}
		CLogger::LogInfo( StringStream() << "  Invalid date" );
		{
			DateTimeType date( DateType(), TimeType( 10, 8, 2 ) );
			BOOST_CHECK_THROW( DateTime::Format( date, sformat ), std::out_of_range );
			BOOST_CHECK_THROW( DateTime::Format( date, wformat ), std::out_of_range );
		}

		CLogger::LogInfo( StringStream() << "**** End TestCase_FormatDateTime ****" );
	}

	void CDatabaseDateTimeTest::TestCase_PrintDateTime()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_PrintDateTime ****" );
		std::string sprint = "%04i/%02i/%02i %02i:%02i:%02i";
		std::wstring wprint = L"%04i/%02i/%02i %02i:%02i:%02i";

		CLogger::LogInfo( StringStream() << "  Normal case" );
		{
			DateTimeType date( DateType( 2015, 10, 7 ), TimeType( 10, 8, 2 ) );
			BOOST_CHECK_EQUAL( DateTime::Print( date, sprint ), "2015/10/07 10:08:02" );
			BOOST_CHECK_EQUAL( DateTime::Print( date, wprint ), L"2015/10/07 10:08:02" );
		}
		CLogger::LogInfo( StringStream() << "  Invalid date" );
		{
			DateTimeType date( DateType(), TimeType( 10, 8, 2 ) );
			BOOST_CHECK_THROW( DateTime::Print( date, sprint ), std::out_of_range );
			BOOST_CHECK_THROW( DateTime::Print( date, wprint ), std::out_of_range );
		}

		CLogger::LogInfo( StringStream() << "**** End TestCase_PrintDateTime ****" );
	}

	void CDatabaseDateTimeTest::TestCase_IsDateTime()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_IsDateTime ****" );
		std::string sformat = "%d/%m/%Y %H:%M:%S";
		std::wstring wformat = L"%d/%m/%Y %H:%M:%S";

		CLogger::LogInfo( StringStream() << "  Normal case" );
		{
			BOOST_CHECK( DateTime::IsDateTime( "10/07/2015 10:08:02", sformat ) );
			BOOST_CHECK( DateTime::IsDateTime( L"10/07/2015 10:08:02", wformat ) );
		}
		CLogger::LogInfo( StringStream() << "  Invalid format" );
		{
			BOOST_CHECK( !DateTime::IsDateTime( "2015/10/07 10:08:02", sformat ) );
			BOOST_CHECK( !DateTime::IsDateTime( L"2015/10/07 10:08:02", wformat ) );
		}
		CLogger::LogInfo( StringStream() << "  Invalid day" );
		{
			BOOST_CHECK( !DateTime::IsDateTime( "53/10/2015 10:08:02", sformat ) );
			BOOST_CHECK( !DateTime::IsDateTime( L"53/10/2015 10:08:02", wformat ) );
		}
		CLogger::LogInfo( StringStream() << "  Invalid month" );
		{
			BOOST_CHECK( !DateTime::IsDateTime( "07/24/2015 10:08:02", sformat ) );
			BOOST_CHECK( !DateTime::IsDateTime( L"07/24/2015 10:08:02", wformat ) );
		}
		CLogger::LogInfo( StringStream() << "  Invalid year" );
		{
			BOOST_CHECK( !DateTime::IsDateTime( "01/10/0000 10:08:02", sformat ) );
			BOOST_CHECK( !DateTime::IsDateTime( L"01/10/0000 10:08:02", wformat ) );
		}
		CLogger::LogInfo( StringStream() << "  Invalid hour" );
		{
			BOOST_CHECK( !DateTime::IsDateTime( "10/07/2015 27:01:01", sformat ) );
			BOOST_CHECK( !DateTime::IsDateTime( L"10/07/2015 27:01:01", wformat ) );
		}
		CLogger::LogInfo( StringStream() << "  Invalid minute" );
		{
			BOOST_CHECK( !DateTime::IsDateTime( "10/07/2015 01:73:01", sformat ) );
			BOOST_CHECK( !DateTime::IsDateTime( L"10/07/2015 01:73:01", wformat ) );
		}
		CLogger::LogInfo( StringStream() << "  Invalid second" );
		{
			BOOST_CHECK( !DateTime::IsDateTime( "10/07/2015 01:01:73", sformat ) );
			BOOST_CHECK( !DateTime::IsDateTime( L"10/07/2015 01:01:73", wformat ) );
		}

		CLogger::LogInfo( StringStream() << "**** End TestCase_IsDateTime ****" );
	}

	void CDatabaseDateTimeTest::TestCase_IsDateTimeWithDateTime()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_IsDateTimeWithDateTime ****" );
		std::string sformat = "%d/%m/%Y %H:%M:%S";
		std::wstring wformat = L"%d/%m/%Y %H:%M:%S";

		CLogger::LogInfo( StringStream() << "  Normal case" );
		{
			DateTimeType dateTime;
			BOOST_CHECK( DateTime::IsDateTime( "10/07/2015 01:01:01", sformat, dateTime ) && !dateTime.date().is_not_a_date() );
			BOOST_CHECK( DateTime::IsDateTime( L"10/07/2015 01:01:01", wformat, dateTime ) && !dateTime.date().is_not_a_date() );
		}
		CLogger::LogInfo( StringStream() << "  Invalid format" );
		{
			DateTimeType dateTime;
			BOOST_CHECK( !DateTime::IsDateTime( "2015/10/07 01:01:01", sformat, dateTime ) && dateTime.date().is_not_a_date() );
			BOOST_CHECK( !DateTime::IsDateTime( L"2015/10/07 01:01:01", wformat, dateTime ) && dateTime.date().is_not_a_date() );
		}
		CLogger::LogInfo( StringStream() << "  Invalid day" );
		{
			DateTimeType dateTime;
			BOOST_CHECK( !DateTime::IsDateTime( "53/10/2015 01:01:01", sformat, dateTime ) && dateTime.date().is_not_a_date() );
			BOOST_CHECK( !DateTime::IsDateTime( L"53/10/2015 01:01:01", wformat, dateTime ) && dateTime.date().is_not_a_date() );
		}
		CLogger::LogInfo( StringStream() << "  Invalid month" );
		{
			DateTimeType dateTime;
			BOOST_CHECK( !DateTime::IsDateTime( "07/24/2015 01:01:01", sformat, dateTime ) && dateTime.date().is_not_a_date() );
			BOOST_CHECK( !DateTime::IsDateTime( L"07/24/2015 01:01:01", wformat, dateTime ) && dateTime.date().is_not_a_date() );
		}
		CLogger::LogInfo( StringStream() << "  Invalid year" );
		{
			DateTimeType dateTime;
			BOOST_CHECK( !DateTime::IsDateTime( "01/10/0000 01:01:01", sformat, dateTime ) && dateTime.date().is_not_a_date() );
			BOOST_CHECK( !DateTime::IsDateTime( L"01/10/0000 01:01:01", wformat, dateTime ) && dateTime.date().is_not_a_date() );
		}
		CLogger::LogInfo( StringStream() << "  Invalid hour" );
		{
			DateTimeType dateTime;
			BOOST_CHECK( !DateTime::IsDateTime( "10/07/2015 27:01:01", sformat, dateTime ) && dateTime.date().is_not_a_date() );
			BOOST_CHECK( !DateTime::IsDateTime( L"10/07/2015 27:01:01", wformat, dateTime ) && dateTime.date().is_not_a_date() );
		}
		CLogger::LogInfo( StringStream() << "  Invalid minute" );
		{
			DateTimeType dateTime;
			BOOST_CHECK( !DateTime::IsDateTime( "10/07/2015 01:73:01", sformat, dateTime ) && dateTime.date().is_not_a_date() );
			BOOST_CHECK( !DateTime::IsDateTime( L"10/07/2015 01:73:01", wformat, dateTime ) && dateTime.date().is_not_a_date() );
		}
		CLogger::LogInfo( StringStream() << "  Invalid second" );
		{
			DateTimeType dateTime;
			BOOST_CHECK( !DateTime::IsDateTime( "10/07/2015 01:01:73", sformat, dateTime ) && dateTime.date().is_not_a_date() );
			BOOST_CHECK( !DateTime::IsDateTime( L"10/07/2015 01:01:73", wformat, dateTime ) && dateTime.date().is_not_a_date() );
		}

		CLogger::LogInfo( StringStream() << "**** End TestCase_IsDateTimeWithDateTime ****" );
	}
}
END_NAMESPACE_DATABASE_TEST
