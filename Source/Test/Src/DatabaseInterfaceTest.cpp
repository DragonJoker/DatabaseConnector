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

namespace std
{
	inline ostream& operator <<( ostream & out, const wstring & value )
	{
		 out << NAMESPACE_DATABASE::StringUtils::ToStr( value );
		 return out;
	}
}

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
		CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseDate ****" );
		std::string sformat = "%d/%m/%Y";
		std::wstring wformat = L"%d/%m/%Y";
		std::string sprint = "%04i/%02i/%02i";
		std::wstring wprint = L"%04i/%02i/%02i";

		CLogger::LogInfo( StringStream() << "  Format - Normal case" );
		{
			DateType date( 2015, 10, 7 );
			BOOST_CHECK_EQUAL( Date::Format( date, sformat ), "07/10/2015" );
			BOOST_CHECK_EQUAL( Date::Format( date, wformat ), L"07/10/2015" );
		}
		CLogger::LogInfo( StringStream() << "  Format - Invalid date" );
		{
			DateType date;
			BOOST_CHECK_THROW( Date::Format( date, sformat ), std::out_of_range );
			BOOST_CHECK_THROW( Date::Format( date, wformat ), std::out_of_range );
		}

		CLogger::LogInfo( StringStream() << "  Print - Normal case" );
		{
			DateType date( 2015, 10, 7 );
			BOOST_CHECK_EQUAL( Date::Print( date, sprint ), "2015/10/07" );
			BOOST_CHECK_EQUAL( Date::Print( date, wprint ), L"2015/10/07" );
		}
		CLogger::LogInfo( StringStream() << "  Print - Invalid date" );
		{
			DateType date;
			BOOST_CHECK_THROW( Date::Print( date, sprint ), std::out_of_range );
			BOOST_CHECK_THROW( Date::Print( date, wprint ), std::out_of_range );
		}

		CLogger::LogInfo( StringStream() << "  IsDate - Normal case" );
		{
			BOOST_CHECK( Date::IsDate( "10/07/2015", sformat ) );
			BOOST_CHECK( Date::IsDate( L"10/07/2015", wformat ) );
		}
		CLogger::LogInfo( StringStream() << "  IsDate - Invalid format" );
		{
			BOOST_CHECK( !Date::IsDate( "2015/10/07", sformat ) );
			BOOST_CHECK( !Date::IsDate( L"2015/10/07", wformat ) );
		}
		CLogger::LogInfo( StringStream() << "  IsDate - Invalid day" );
		{
			BOOST_CHECK( !Date::IsDate( "53/10/2015", sformat ) );
			BOOST_CHECK( !Date::IsDate( L"53/10/2015", wformat ) );
		}
		CLogger::LogInfo( StringStream() << "  IsDate - Invalid month" );
		{
			BOOST_CHECK( !Date::IsDate( "07/24/2015", sformat ) );
			BOOST_CHECK( !Date::IsDate( L"07/24/2015", wformat ) );
		}
		CLogger::LogInfo( StringStream() << "  IsDate - Invalid year" );
		{
			BOOST_CHECK( !Date::IsDate( "01/10/0000", sformat ) );
			BOOST_CHECK( !Date::IsDate( L"01/10/0000", wformat ) );
		}

		CLogger::LogInfo( StringStream() << "  IsDate (date) - Normal case" );
		{
			DateType date;
			BOOST_CHECK( Date::IsDate( "10/07/2015", sformat, date ) && !date.is_not_a_date() );
			BOOST_CHECK( Date::IsDate( L"10/07/2015", wformat, date ) && !date.is_not_a_date() );
		}
		CLogger::LogInfo( StringStream() << "  IsDate (date) - Invalid format" );
		{
			DateType date;
			BOOST_CHECK( !Date::IsDate( "2015/10/07", sformat, date ) && date.is_not_a_date() );
			BOOST_CHECK( !Date::IsDate( L"2015/10/07", wformat, date ) && date.is_not_a_date() );
		}
		CLogger::LogInfo( StringStream() << "  IsDate (date) - Invalid day" );
		{
			DateType date;
			BOOST_CHECK( !Date::IsDate( "53/10/2015", sformat, date ) && date.is_not_a_date() );
			BOOST_CHECK( !Date::IsDate( L"53/10/2015", wformat, date ) && date.is_not_a_date() );
		}
		CLogger::LogInfo( StringStream() << "  IsDate (date) - Invalid month" );
		{
			DateType date;
			BOOST_CHECK( !Date::IsDate( "07/24/2015", sformat, date ) && date.is_not_a_date() );
			BOOST_CHECK( !Date::IsDate( L"07/24/2015", wformat, date ) && date.is_not_a_date() );
		}
		CLogger::LogInfo( StringStream() << "  IsDate (date) - Invalid year" );
		{
			DateType date;
			BOOST_CHECK( !Date::IsDate( "01/10/0000", sformat, date ) && date.is_not_a_date() );
			BOOST_CHECK( !Date::IsDate( L"01/10/0000", wformat, date ) && date.is_not_a_date() );
		}

		CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseDate ****" );
	}

	void CDatabaseInterfaceTest::TestCase_DatabaseDateTime()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseDateTime ****" );
		std::string sformat = "%d/%m/%Y %H:%M:%S";
		std::wstring wformat = L"%d/%m/%Y %H:%M:%S";
		std::string sprint = "%04i/%02i/%02i %02i:%02i:%02i";
		std::wstring wprint = L"%04i/%02i/%02i %02i:%02i:%02i";

		CLogger::LogInfo( StringStream() << "  Format - Normal case" );
		{
			DateTimeType date( DateType( 2015, 10, 7 ), TimeType( 10, 8, 2 ) );
			BOOST_CHECK_EQUAL( DateTime::Format( date, sformat ), "07/10/2015 10:08:02" );
			BOOST_CHECK_EQUAL( DateTime::Format( date, wformat ), L"07/10/2015 10:08:02" );
		}
		CLogger::LogInfo( StringStream() << "  Format - Invalid date" );
		{
			DateTimeType date( DateType(), TimeType( 10, 8, 2 ) );
			BOOST_CHECK_THROW( DateTime::Format( date, sformat ), std::out_of_range );
			BOOST_CHECK_THROW( DateTime::Format( date, wformat ), std::out_of_range );
		}
		CLogger::LogInfo( StringStream() << "  Format - Invalid time" );
		{
			DateTimeType date( DateType( 2015, 10, 7 ), TimeType( -1, -1, -1 ) );
			BOOST_CHECK_THROW( DateTime::Format( date, sformat ), std::out_of_range );
			BOOST_CHECK_THROW( DateTime::Format( date, wformat ), std::out_of_range );
		}

		CLogger::LogInfo( StringStream() << "  Print - Normal case" );
		{
			DateTimeType date( DateType( 2015, 10, 7 ), TimeType( 10, 8, 2 ) );
			BOOST_CHECK_EQUAL( DateTime::Print( date, sprint ), "2015/10/07 10:08:02" );
			BOOST_CHECK_EQUAL( DateTime::Print( date, wprint ), L"2015/10/07 10:08:02" );
		}
		CLogger::LogInfo( StringStream() << "  Print - Invalid date" );
		{
			DateTimeType date( DateType(), TimeType( 10, 8, 2 ) );
			BOOST_CHECK_THROW( DateTime::Print( date, sprint ), std::out_of_range );
			BOOST_CHECK_THROW( DateTime::Print( date, wprint ), std::out_of_range );
		}
		CLogger::LogInfo( StringStream() << "  Print - Invalid time" );
		{
			DateTimeType date( DateType( 2015, 10, 7 ), TimeType( -1, -1, -1 ) );
			BOOST_CHECK_THROW( DateTime::Print( date, sprint ), std::out_of_range );
			BOOST_CHECK_THROW( DateTime::Print( date, wprint ), std::out_of_range );
		}

		CLogger::LogInfo( StringStream() << "  IsDateTime - Normal case" );
		{
			BOOST_CHECK( DateTime::IsDateTime( "10/07/2015", sformat ) );
			BOOST_CHECK( DateTime::IsDateTime( L"10/07/2015", wformat ) );
		}
		CLogger::LogInfo( StringStream() << "  IsDateTime - Invalid format" );
		{
			BOOST_CHECK( !DateTime::IsDateTime( "2015/10/07", sformat ) );
			BOOST_CHECK( !DateTime::IsDateTime( L"2015/10/07", wformat ) );
		}
		CLogger::LogInfo( StringStream() << "  IsDateTime - Invalid day" );
		{
			BOOST_CHECK( !DateTime::IsDateTime( "53/10/2015", sformat ) );
			BOOST_CHECK( !DateTime::IsDateTime( L"53/10/2015", wformat ) );
		}
		CLogger::LogInfo( StringStream() << "  IsDateTime - Invalid month" );
		{
			BOOST_CHECK( !DateTime::IsDateTime( "07/24/2015", sformat ) );
			BOOST_CHECK( !DateTime::IsDateTime( L"07/24/2015", wformat ) );
		}
		CLogger::LogInfo( StringStream() << "  IsDateTime - Invalid year" );
		{
			BOOST_CHECK( !DateTime::IsDateTime( "01/10/0000", sformat ) );
			BOOST_CHECK( !DateTime::IsDateTime( L"01/10/0000", wformat ) );
		}

		CLogger::LogInfo( StringStream() << "  IsDateTime (dateTime) - Normal case" );
		{
			DateTimeType dateTime;
			BOOST_CHECK( DateTime::IsDateTime( "10/07/2015 01:01:01", sformat, dateTime ) && !dateTime.date().is_not_a_date() );
			BOOST_CHECK( DateTime::IsDateTime( L"10/07/2015 01:01:01", wformat, dateTime ) && !dateTime.date().is_not_a_date() );
		}
		CLogger::LogInfo( StringStream() << "  IsDateTime (dateTime) - Invalid format" );
		{
			DateTimeType dateTime;
			BOOST_CHECK( !DateTime::IsDateTime( "2015/10/07 01:01:01", sformat, dateTime ) && dateTime.date().is_not_a_date() );
			BOOST_CHECK( !DateTime::IsDateTime( L"2015/10/07 01:01:01", wformat, dateTime ) && dateTime.date().is_not_a_date() );
		}
		CLogger::LogInfo( StringStream() << "  IsDateTime (dateTime) - Invalid day" );
		{
			DateTimeType dateTime;
			BOOST_CHECK( !DateTime::IsDateTime( "53/10/2015 01:01:01", sformat, dateTime ) && dateTime.date().is_not_a_date() );
			BOOST_CHECK( !DateTime::IsDateTime( L"53/10/2015 01:01:01", wformat, dateTime ) && dateTime.date().is_not_a_date() );
		}
		CLogger::LogInfo( StringStream() << "  IsDateTime (dateTime) - Invalid month" );
		{
			DateTimeType dateTime;
			BOOST_CHECK( !DateTime::IsDateTime( "07/24/2015 01:01:01", sformat, dateTime ) && dateTime.date().is_not_a_date() );
			BOOST_CHECK( !DateTime::IsDateTime( L"07/24/2015 01:01:01", wformat, dateTime ) && dateTime.date().is_not_a_date() );
		}
		CLogger::LogInfo( StringStream() << "  IsDateTime (dateTime) - Invalid year" );
		{
			DateTimeType dateTime;
			BOOST_CHECK( !DateTime::IsDateTime( "01/10/0000 01:01:01", sformat, dateTime ) && dateTime.date().is_not_a_date() );
			BOOST_CHECK( !DateTime::IsDateTime( L"01/10/0000 01:01:01", wformat, dateTime ) && dateTime.date().is_not_a_date() );
		}
		CLogger::LogInfo( StringStream() << "  IsDateTime (dateTime) - Invalid hour" );
		{
			DateTimeType dateTime;
			BOOST_CHECK( !DateTime::IsDateTime( "10/07/2015 27:01:01", sformat, dateTime ) && dateTime.date().is_not_a_date() );
			BOOST_CHECK( !DateTime::IsDateTime( L"10/07/2015 27:01:01", wformat, dateTime ) && dateTime.date().is_not_a_date() );
		}
		CLogger::LogInfo( StringStream() << "  IsDateTime (dateTime) - Invalid minute" );
		{
			DateTimeType dateTime;
			BOOST_CHECK( !DateTime::IsDateTime( "10/07/2015 01:73:01", sformat, dateTime ) && dateTime.date().is_not_a_date() );
			BOOST_CHECK( !DateTime::IsDateTime( L"10/07/2015 01:73:01", wformat, dateTime ) && dateTime.date().is_not_a_date() );
		}
		CLogger::LogInfo( StringStream() << "  IsDateTime (dateTime) - Invalid second" );
		{
			DateTimeType dateTime;
			BOOST_CHECK( !DateTime::IsDateTime( "10/07/2015 01:01:73", sformat, dateTime ) && dateTime.date().is_not_a_date() );
			BOOST_CHECK( !DateTime::IsDateTime( L"10/07/2015 01:01:73", wformat, dateTime ) && dateTime.date().is_not_a_date() );
		}

		CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseDateTime ****" );
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
