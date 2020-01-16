/************************************************************************//**
 * @file DatabaseDateTest.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 12/02/2014 14:29:35
 *
 *
 * @brief Class testing Date functions
*
***************************************************************************/

#include "DatabaseTestPch.h"

#include "DatabaseDateTest.h"
#include "DatabaseTestHelpers.h"

#include <DatabaseDate.h>

BEGIN_NAMESPACE_DATABASE_TEST
{
	CDatabaseDateTest::CDatabaseDateTest()
	{
	}

	CDatabaseDateTest::~CDatabaseDateTest()
	{
	}

	boost::unit_test::test_suite * CDatabaseDateTest::Init_Test_Suite()
	{
		//!@remarks Create the internal TS instance.
		testSuite = new boost::unit_test::test_suite( "CDatabaseDateTest", __FILE__, __LINE__ );

		//!@remarks Add the TC to the internal TS.
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseDateTest::TestCase_FormatDate, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseDateTest::TestCase_PrintDate, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseDateTest::TestCase_IsDate, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseDateTest::TestCase_IsDateWithDate, this ) ) );

		//!@remarks Return the TS instance.
		return testSuite;
	}

	void CDatabaseDateTest::TestCase_FormatDate()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_FormatDate ****" );
		std::string sformat = "%d/%m/%Y";
		std::wstring wformat = L"%d/%m/%Y";

		CLogger::LogInfo( StringStream() << "  Normal case" );
		{
			DateType date( 2015, 10, 7 );
			BOOST_CHECK_EQUAL( Date::Format( date, sformat ), "07/10/2015" );
			BOOST_CHECK_EQUAL( Date::Format( date, wformat ), L"07/10/2015" );
		}
		CLogger::LogInfo( StringStream() << "  Invalid date" );
		{
			DateType date;
			BOOST_CHECK_THROW( Date::Format( date, sformat ), std::out_of_range );
			BOOST_CHECK_THROW( Date::Format( date, wformat ), std::out_of_range );
		}

		CLogger::LogInfo( StringStream() << "**** End TestCase_FormatDate ****" );
	}

	void CDatabaseDateTest::TestCase_PrintDate()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_PrintDate ****" );
		std::string sprint = "%04i/%02i/%02i";
		std::wstring wprint = L"%04i/%02i/%02i";

		CLogger::LogInfo( StringStream() << "  Normal case" );
		{
			DateType date( 2015, 10, 7 );
			BOOST_CHECK_EQUAL( Date::Print( date, sprint ), "2015/10/07" );
			BOOST_CHECK_EQUAL( Date::Print( date, wprint ), L"2015/10/07" );
		}
		CLogger::LogInfo( StringStream() << "  Invalid date" );
		{
			DateType date;
			BOOST_CHECK_THROW( Date::Print( date, sprint ), std::out_of_range );
			BOOST_CHECK_THROW( Date::Print( date, wprint ), std::out_of_range );
		}

		CLogger::LogInfo( StringStream() << "**** End TestCase_PrintDate ****" );
	}

	void CDatabaseDateTest::TestCase_IsDate()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_IsDate ****" );
		std::string sformat = "%d/%m/%Y";
		std::wstring wformat = L"%d/%m/%Y";

		CLogger::LogInfo( StringStream() << "  Normal case" );
		{
			BOOST_CHECK( Date::IsDate( "10/07/2015", sformat ) );
			BOOST_CHECK( Date::IsDate( L"10/07/2015", wformat ) );
		}
		CLogger::LogInfo( StringStream() << "  Invalid format" );
		{
			BOOST_CHECK( !Date::IsDate( "2015/10/07", sformat ) );
			BOOST_CHECK( !Date::IsDate( L"2015/10/07", wformat ) );
		}
		CLogger::LogInfo( StringStream() << "  Invalid day" );
		{
			BOOST_CHECK( !Date::IsDate( "53/10/2015", sformat ) );
			BOOST_CHECK( !Date::IsDate( L"53/10/2015", wformat ) );
		}
		CLogger::LogInfo( StringStream() << "  Invalid month" );
		{
			BOOST_CHECK( !Date::IsDate( "07/24/2015", sformat ) );
			BOOST_CHECK( !Date::IsDate( L"07/24/2015", wformat ) );
		}
		CLogger::LogInfo( StringStream() << "  Invalid year" );
		{
			BOOST_CHECK( !Date::IsDate( "01/10/0000", sformat ) );
			BOOST_CHECK( !Date::IsDate( L"01/10/0000", wformat ) );
		}

		CLogger::LogInfo( StringStream() << "**** End TestCase_IsDate ****" );
	}

	void CDatabaseDateTest::TestCase_IsDateWithDate()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_IsDateWithDate ****" );
		std::string sformat = "%d/%m/%Y";
		std::wstring wformat = L"%d/%m/%Y";

		CLogger::LogInfo( StringStream() << "  Normal case" );
		{
			DateType date;
			BOOST_CHECK( Date::IsDate( "10/07/2015", sformat, date ) && !date.is_not_a_date() );
			BOOST_CHECK( Date::IsDate( L"10/07/2015", wformat, date ) && !date.is_not_a_date() );
		}
		CLogger::LogInfo( StringStream() << "  Invalid format" );
		{
			DateType date;
			BOOST_CHECK( !Date::IsDate( "2015/10/07", sformat, date ) && date.is_not_a_date() );
			BOOST_CHECK( !Date::IsDate( L"2015/10/07", wformat, date ) && date.is_not_a_date() );
		}
		CLogger::LogInfo( StringStream() << "  Invalid day" );
		{
			DateType date;
			BOOST_CHECK( !Date::IsDate( "53/10/2015", sformat, date ) && date.is_not_a_date() );
			BOOST_CHECK( !Date::IsDate( L"53/10/2015", wformat, date ) && date.is_not_a_date() );
		}
		CLogger::LogInfo( StringStream() << "  Invalid month" );
		{
			DateType date;
			BOOST_CHECK( !Date::IsDate( "07/24/2015", sformat, date ) && date.is_not_a_date() );
			BOOST_CHECK( !Date::IsDate( L"07/24/2015", wformat, date ) && date.is_not_a_date() );
		}
		CLogger::LogInfo( StringStream() << "  Invalid year" );
		{
			DateType date;
			BOOST_CHECK( !Date::IsDate( "01/10/0000", sformat, date ) && date.is_not_a_date() );
			BOOST_CHECK( !Date::IsDate( L"01/10/0000", wformat, date ) && date.is_not_a_date() );
		}

		CLogger::LogInfo( StringStream() << "**** End TestCase_IsDateWithDate ****" );
	}
}
END_NAMESPACE_DATABASE_TEST
