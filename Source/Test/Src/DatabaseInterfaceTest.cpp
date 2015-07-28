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
#include "DatabaseTestUtils.h"

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
	namespace
	{
		void CheckFixedPoint( uint8_t precision, uint8_t decimals, int64_t value, const String & sval )
		{
			double dval = value / pow( 10, decimals );
			int64_t ival = int64_t( value / int256_t( pow( 10, decimals ) ) );
			CFixedPoint fp( value, precision, decimals );
			BOOST_CHECK_EQUAL( fp.GetRawValue(), value );
			BOOST_CHECK_EQUAL( fp.ToDouble(), dval );
			BOOST_CHECK_EQUAL( fp.ToFloat(), float( dval ) );
			BOOST_CHECK_EQUAL( fp.ToInt32(), int32_t( ival ) );
			BOOST_CHECK_EQUAL( fp.ToUInt32(), uint32_t( ival ) );
			BOOST_CHECK_EQUAL( fp.ToInt64(), int64_t( ival ) );
			BOOST_CHECK_EQUAL( fp.ToUInt64(), uint64_t( ival ) );
			BOOST_CHECK_EQUAL( fp.ToString(), sval );
		}
	}

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
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseInterfaceTest::TestCase_DatabaseFixedPoint, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseInterfaceTest::TestCase_DatabaseInt24, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseInterfaceTest::TestCase_DatabaseUInt24, this ) ) );
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

		CLogger::LogInfo( StringStream() << "  IsDateTime - Normal case" );
		{
			BOOST_CHECK( DateTime::IsDateTime( "10/07/2015 10:08:02", sformat ) );
			BOOST_CHECK( DateTime::IsDateTime( L"10/07/2015 10:08:02", wformat ) );
		}
		CLogger::LogInfo( StringStream() << "  IsDateTime - Invalid format" );
		{
			BOOST_CHECK( !DateTime::IsDateTime( "2015/10/07 10:08:02", sformat ) );
			BOOST_CHECK( !DateTime::IsDateTime( L"2015/10/07 10:08:02", wformat ) );
		}
		CLogger::LogInfo( StringStream() << "  IsDateTime - Invalid day" );
		{
			BOOST_CHECK( !DateTime::IsDateTime( "53/10/2015 10:08:02", sformat ) );
			BOOST_CHECK( !DateTime::IsDateTime( L"53/10/2015 10:08:02", wformat ) );
		}
		CLogger::LogInfo( StringStream() << "  IsDateTime - Invalid month" );
		{
			BOOST_CHECK( !DateTime::IsDateTime( "07/24/2015 10:08:02", sformat ) );
			BOOST_CHECK( !DateTime::IsDateTime( L"07/24/2015 10:08:02", wformat ) );
		}
		CLogger::LogInfo( StringStream() << "  IsDateTime - Invalid year" );
		{
			BOOST_CHECK( !DateTime::IsDateTime( "01/10/0000 10:08:02", sformat ) );
			BOOST_CHECK( !DateTime::IsDateTime( L"01/10/0000 10:08:02", wformat ) );
		}
		CLogger::LogInfo( StringStream() << "  IsDateTime - Invalid hour" );
		{
			BOOST_CHECK( !DateTime::IsDateTime( "10/07/2015 27:01:01", sformat ) );
			BOOST_CHECK( !DateTime::IsDateTime( L"10/07/2015 27:01:01", wformat ) );
		}
		CLogger::LogInfo( StringStream() << "  IsDateTime - Invalid minute" );
		{
			BOOST_CHECK( !DateTime::IsDateTime( "10/07/2015 01:73:01", sformat ) );
			BOOST_CHECK( !DateTime::IsDateTime( L"10/07/2015 01:73:01", wformat ) );
		}
		CLogger::LogInfo( StringStream() << "  IsDateTime - Invalid second" );
		{
			BOOST_CHECK( !DateTime::IsDateTime( "10/07/2015 01:01:73", sformat ) );
			BOOST_CHECK( !DateTime::IsDateTime( L"10/07/2015 01:01:73", wformat ) );
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
		CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseTime ****" );
		std::string sformat = "%H:%M:%S";
		std::wstring wformat = L"%H:%M:%S";
		std::string sprint = "%02i:%02i:%02i";
		std::wstring wprint = L"%02i:%02i:%02i";

		CLogger::LogInfo( StringStream() << "  Format - Normal case" );
		{
			TimeType time( 10, 8, 2 );
			BOOST_CHECK_EQUAL( Time::Format( time, sformat ), "10:08:02" );
			BOOST_CHECK_EQUAL( Time::Format( time, wformat ), L"10:08:02" );
		}
		CLogger::LogInfo( StringStream() << "  Format - Invalid time (negative)" );
		{
			TimeType time( -1, -1, -1 );
			BOOST_CHECK_THROW( Time::Format( time, sformat ), std::out_of_range );
			BOOST_CHECK_THROW( Time::Format( time, wformat ), std::out_of_range );
		}
		CLogger::LogInfo( StringStream() << "  Format - Invalid time (not a date time)" );
		{
			TimeType time( boost::gregorian::not_a_date_time );
			BOOST_CHECK_THROW( Time::Format( time, sformat ), std::out_of_range );
			BOOST_CHECK_THROW( Time::Format( time, wformat ), std::out_of_range );
		}

		CLogger::LogInfo( StringStream() << "  Print - Normal case" );
		{
			TimeType time( 10, 8, 2 );
			BOOST_CHECK_EQUAL( Time::Print( time, sprint ), "10:08:02" );
			BOOST_CHECK_EQUAL( Time::Print( time, wprint ), L"10:08:02" );
		}
		CLogger::LogInfo( StringStream() << "  Print - Invalid time (negative)" );
		{
			TimeType time( -1, -1, -1 );
			BOOST_CHECK_THROW( Time::Print( time, sformat ), std::out_of_range );
			BOOST_CHECK_THROW( Time::Print( time, wformat ), std::out_of_range );
		}
		CLogger::LogInfo( StringStream() << "  Print - Invalid time (not a date time)" );
		{
			TimeType time( boost::gregorian::not_a_date_time );
			BOOST_CHECK_THROW( Time::Print( time, sformat ), std::out_of_range );
			BOOST_CHECK_THROW( Time::Print( time, wformat ), std::out_of_range );
		}

		CLogger::LogInfo( StringStream() << "  IsTime - Normal case" );
		{
			BOOST_CHECK( Time::IsTime( "10:08:02", sformat ) );
			BOOST_CHECK( Time::IsTime( L"10:08:02", wformat ) );
		}
		CLogger::LogInfo( StringStream() << "  IsTime - Invalid format" );
		{
			BOOST_CHECK( !Time::IsTime( "1008:02", sformat ) );
			BOOST_CHECK( !Time::IsTime( L"1008:02", wformat ) );
		}
		CLogger::LogInfo( StringStream() << "  IsTime - Invalid hour" );
		{
			BOOST_CHECK( !Time::IsTime( "27:01:01", sformat ) );
			BOOST_CHECK( !Time::IsTime( L"27:01:01", wformat ) );
		}
		CLogger::LogInfo( StringStream() << "  IsTime - Invalid minute" );
		{
			BOOST_CHECK( !Time::IsTime( "01:73:01", sformat ) );
			BOOST_CHECK( !Time::IsTime( L"01:73:01", wformat ) );
		}
		CLogger::LogInfo( StringStream() << "  IsTime - Invalid second" );
		{
			BOOST_CHECK( !Time::IsTime( "01:01:73", sformat ) );
			BOOST_CHECK( !Time::IsTime( L"01:01:73", wformat ) );
		}

		CLogger::LogInfo( StringStream() << "  IsTime (dateTime) - Normal case" );
		{
			TimeType time;
			BOOST_CHECK( Time::IsTime( "01:01:01", sformat, time ) && !time.is_not_a_date_time() );
			BOOST_CHECK( Time::IsTime( L"01:01:01", wformat, time ) && !time.is_not_a_date_time() );
		}
		CLogger::LogInfo( StringStream() << "  IsTime (dateTime) - Invalid format" );
		{
			TimeType time;
			BOOST_CHECK( !Time::IsTime( "0101:01", sformat, time ) && time.is_not_a_date_time() );
			BOOST_CHECK( !Time::IsTime( L"0101:01", wformat, time ) && time.is_not_a_date_time() );
		}
		CLogger::LogInfo( StringStream() << "  IsTime (dateTime) - Invalid hour" );
		{
			TimeType time;
			BOOST_CHECK( !Time::IsTime( "27:01:01", sformat, time ) && time.is_not_a_date_time() );
			BOOST_CHECK( !Time::IsTime( L"27:01:01", wformat, time ) && time.is_not_a_date_time() );
		}
		CLogger::LogInfo( StringStream() << "  IsTime (dateTime) - Invalid minute" );
		{
			TimeType time;
			BOOST_CHECK( !Time::IsTime( "01:73:01", sformat, time ) && time.is_not_a_date_time() );
			BOOST_CHECK( !Time::IsTime( L"01:73:01", wformat, time ) && time.is_not_a_date_time() );
		}
		CLogger::LogInfo( StringStream() << "  IsTime (dateTime) - Invalid second" );
		{
			TimeType time;
			BOOST_CHECK( !Time::IsTime( "01:01:73", sformat, time ) && time.is_not_a_date_time() );
			BOOST_CHECK( !Time::IsTime( L"01:01:73", wformat, time ) && time.is_not_a_date_time() );
		}

		CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseDateTime ****" );
	}

	void CDatabaseInterfaceTest::TestCase_DatabaseFixedPoint()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseFixedPoint ****" );

		CLogger::LogInfo( StringStream() << "  Invalid Precision inferior to min" );
		BOOST_CHECK_THROW( CFixedPoint( 0, CFixedPoint::GetMinPrecision() - 1, 0 ), CDatabaseException );

		CLogger::LogInfo( StringStream() << "  Invalid Precision superior to max" );
		BOOST_CHECK_THROW( CFixedPoint( 0, CFixedPoint::GetMaxPrecision() + 1, 0 ), CDatabaseException );

		CLogger::LogInfo( StringStream() << "  Invalid dDecimals equal to precision" );
		BOOST_CHECK_THROW( CFixedPoint( 0, CFixedPoint::GetMaxPrecision() / 2, CFixedPoint::GetMaxPrecision() / 2 ), CDatabaseException );

		CLogger::LogInfo( StringStream() << "  Invalid Decimals superior to precision" );
		BOOST_CHECK_THROW( CFixedPoint( 0, CFixedPoint::GetMaxPrecision() / 2, 1 + CFixedPoint::GetMaxPrecision() / 2 ), CDatabaseException );

		CLogger::LogInfo( StringStream() << "  Min Precision: " << int( CFixedPoint::GetMinPrecision() ) );
		{
			int precision = CFixedPoint::GetMinPrecision();
			int decimals = 0;
			int64_t value;
			CLogger::LogInfo( StringStream() << "    Max Value" );
			value = int64_t( std::numeric_limits< int64_t >::max() % int256_t( pow( 10, precision ) ) );
			CheckFixedPoint( precision, decimals, value, STR( "0" ) );
			CLogger::LogInfo( StringStream() << "    Min Value" );
			value = int64_t( std::numeric_limits< int64_t >::min() % int256_t( pow( 10, precision ) ) );
			CheckFixedPoint( precision, decimals, value, STR( "0" ) );
			CLogger::LogInfo( StringStream() << "    Lowest Value" );
			value = int64_t( std::numeric_limits< int64_t >::lowest() % int256_t( pow( 10, precision ) ) );
			CheckFixedPoint( precision, decimals, value, STR( "0" ) );
			CLogger::LogInfo( StringStream() << "    Invalid Value (greater precision than allowed)" );
			BOOST_CHECK_THROW( CFixedPoint( std::numeric_limits< int64_t >::max(), precision, decimals ), CDatabaseException );
		}

		CLogger::LogInfo( StringStream() << "  Max Precision: " << int( CFixedPoint::GetMaxPrecision() ) << ", minimum decimals" );
		{
			int precision = CFixedPoint::GetMaxPrecision();
			int decimals = 0;
			int64_t value;
			CLogger::LogInfo( StringStream() << "    Max Value" );
			value = int64_t( std::numeric_limits< int64_t >::max() % int256_t( pow( 10, precision ) ) );
			CheckFixedPoint( precision, decimals, value, StringUtils::ToString( value ) );
			CLogger::LogInfo( StringStream() << "    Min Value" );
			value = int64_t( std::numeric_limits< int64_t >::min() % int256_t( pow( 10, precision ) ) );
			CheckFixedPoint( precision, decimals, value, StringUtils::ToString( value ) );
			CLogger::LogInfo( StringStream() << "    Lowest Value" );
			value = int64_t( std::numeric_limits< int64_t >::lowest() % int256_t( pow( 10, precision ) ) );
			CheckFixedPoint( precision, decimals, value, StringUtils::ToString( value ) );
		}

		CLogger::LogInfo( StringStream() << "  Max Precision: " << int( CFixedPoint::GetMaxPrecision() ) << ", maximum decimals" );
		{
			int precision = CFixedPoint::GetMaxPrecision();
			int decimals = precision - 1;
			int64_t value;
			String sval;
			CLogger::LogInfo( StringStream() << "    Max Value" );
			value = int64_t( std::numeric_limits< int64_t >::max() % int256_t( pow( 10, precision ) ) );
			sval = "0." + StringUtils::ToString( value );
			CheckFixedPoint( precision, decimals, value, sval );
			CLogger::LogInfo( StringStream() << "    Min Value" );
			value = int64_t( std::numeric_limits< int64_t >::min() % int256_t( pow( 10, precision ) ) );
			sval = "-0." + StringUtils::ToString( uint64_t( abs( value ) ) );
			CheckFixedPoint( precision, decimals, value, sval );
			CLogger::LogInfo( StringStream() << "    Lowest Value" );
			value = int64_t( std::numeric_limits< int64_t >::lowest() % int256_t( pow( 10, precision ) ) );
			sval = "-0." + StringUtils::ToString( uint64_t( abs( value ) ) );
			CheckFixedPoint( precision, decimals, value, sval );
		}

		CLogger::LogInfo( StringStream() << "  Medium Precision: " << int( CFixedPoint::GetMaxPrecision() / 2 ) << ", decimals is half the precision" );
		{
			int precision = CFixedPoint::GetMaxPrecision() / 2;
			int decimals = precision / 2;
			int64_t value;
			String sval;
			CLogger::LogInfo( StringStream() << "    Max Value" );
			value = int64_t( std::numeric_limits< int64_t >::max() % int256_t( pow( 10, precision ) ) );
			sval = StringUtils::ToString( value );
			sval.insert( ( sval.rbegin() + decimals ).base(), STR( '.' ) );
			CheckFixedPoint( precision, decimals, value, sval );
			CLogger::LogInfo( StringStream() << "    Min Value" );
			value = int64_t( std::numeric_limits< int64_t >::min() % int256_t( pow( 10, precision ) ) );
			sval = StringUtils::ToString( value );
			sval.insert( ( sval.rbegin() + decimals ).base(), STR( '.' ) );
			CheckFixedPoint( precision, decimals, value, sval );
			CLogger::LogInfo( StringStream() << "    Lowest Value" );
			value = int64_t( std::numeric_limits< int64_t >::lowest() % int256_t( pow( 10, precision ) ) );
			sval = StringUtils::ToString( value );
			sval.insert( ( sval.rbegin() + decimals ).base(), STR( '.' ) );
			CheckFixedPoint( precision, decimals, value, sval );
			CLogger::LogInfo( StringStream() << "    Invalid Value (greater precision than wanted)" );
			BOOST_CHECK_THROW( CFixedPoint( std::numeric_limits< int64_t >::max(), precision, decimals ), CDatabaseException );
		}

		CLogger::LogInfo( StringStream() << "  Construction from other types, Medium precision: " << int( CFixedPoint::GetMaxPrecision() / 2 ) << ", decimals is half the precision" );
		{
			int precision = CFixedPoint::GetMaxPrecision() / 2;
			int decimals = precision / 2;
			CLogger::LogInfo( StringStream() << "    From double" );
			{
				CLogger::LogInfo( StringStream() << "      Max Value (Precision overflow)" );
				BOOST_CHECK_THROW( CFixedPoint( std::numeric_limits< double >::max(), precision, decimals ), CDatabaseException );
				CLogger::LogInfo( StringStream() << "      Min Value" );
				BOOST_CHECK_EQUAL( CFixedPoint( std::numeric_limits< double >::min(), precision, decimals ).ToDouble(), 0.0 );
				CLogger::LogInfo( StringStream() << "      Lowest Value (Precision overflow)" );
				BOOST_CHECK_THROW( CFixedPoint( std::numeric_limits< double >::lowest(), precision, decimals ), CDatabaseException );
				CLogger::LogInfo( StringStream() << "      Random valid Value" );
				{
					double value = DatabaseUtils::Helpers< EFieldType_FLOAT64 >::InitialiseValue();
					CFixedPoint fp( value, precision, decimals );
					BOOST_CHECK_LT( value - fp.ToDouble(), pow( 10, -decimals ) );
				}
			}
			CLogger::LogInfo( StringStream() << "    From float" );
			{
				CLogger::LogInfo( StringStream() << "      Max Value (Precision overflow)" );
				BOOST_CHECK_THROW( CFixedPoint( std::numeric_limits< float >::max(), precision, decimals ), CDatabaseException );
				CLogger::LogInfo( StringStream() << "      Min Value" );
				BOOST_CHECK_EQUAL( CFixedPoint( std::numeric_limits< float >::min(), precision, decimals ).ToDouble(), 0.0 );
				CLogger::LogInfo( StringStream() << "      Lowest Value (Precision overflow)" );
				BOOST_CHECK_THROW( CFixedPoint( std::numeric_limits< float >::lowest(), precision, decimals ), CDatabaseException );
				CLogger::LogInfo( StringStream() << "      Random valid Value" );
				{
					float value = DatabaseUtils::Helpers< EFieldType_FLOAT32 >::InitialiseValue();
					CFixedPoint fp( value, precision, decimals );
					BOOST_CHECK_LT( value - fp.ToFloat(), pow( 10, -decimals ) );
				}
			}
			CLogger::LogInfo( StringStream() << "    From int32_t" );
			{
				CLogger::LogInfo( StringStream() << "      Max Value" );
				{
					int32_t value = std::numeric_limits< int32_t >::max();
					BOOST_CHECK_EQUAL( CFixedPoint( value, precision, decimals ).ToInt32(), int32_t( value / pow( 10, decimals ) ) );
				}
				CLogger::LogInfo( StringStream() << "      Min Value" );
				{
					int32_t value = std::numeric_limits< int32_t >::min();
					BOOST_CHECK_EQUAL( CFixedPoint( value, precision, decimals ).ToInt32(), int32_t( value / pow( 10, decimals ) ) );
				}
				CLogger::LogInfo( StringStream() << "      Lowest Value" );
				{
					int32_t value = std::numeric_limits< int32_t >::lowest();
					BOOST_CHECK_EQUAL( CFixedPoint( value, precision, decimals ).ToInt32(), int32_t( value / pow( 10, decimals ) ) );
				}
			}
			CLogger::LogInfo( StringStream() << "    From uint32_t" );
			{
				CLogger::LogInfo( StringStream() << "      Max Value" );
				{
					uint32_t value = std::numeric_limits< uint32_t >::max();
					BOOST_CHECK_EQUAL( CFixedPoint( value, precision, decimals ).ToInt32(), uint32_t( value / pow( 10, decimals ) ) );
				}
				CLogger::LogInfo( StringStream() << "      Min Value" );
				{
					uint32_t value = std::numeric_limits< uint32_t >::min();
					BOOST_CHECK_EQUAL( CFixedPoint( value, precision, decimals ).ToInt32(), uint32_t( value / pow( 10, decimals ) ) );
				}
				CLogger::LogInfo( StringStream() << "      Lowest Value" );
				{
					uint32_t value = std::numeric_limits< uint32_t >::lowest();
					BOOST_CHECK_EQUAL( CFixedPoint( value, precision, decimals ).ToInt32(), uint32_t( value / pow( 10, decimals ) ) );
				}
			}
			CLogger::LogInfo( StringStream() << "    From uint64_t" );
			{
				CLogger::LogInfo( StringStream() << "      Max Value (Precision overflow)" );
				BOOST_CHECK_THROW( CFixedPoint( std::numeric_limits< uint64_t >::max(), precision, decimals ), CDatabaseException );
				CLogger::LogInfo( StringStream() << "      Min Value" );
				BOOST_CHECK_EQUAL( CFixedPoint( std::numeric_limits< uint64_t >::min(), precision, decimals ).ToUInt64(), 0 );
				CLogger::LogInfo( StringStream() << "      Lowest Value" );
				BOOST_CHECK_EQUAL( CFixedPoint( std::numeric_limits< uint64_t >::lowest(), precision, decimals ).ToUInt64(), 0 );
				CLogger::LogInfo( StringStream() << "      Random valid Value" );
				uint64_t value = DatabaseUtils::Helpers< EFieldType_UINT64 >::InitialiseValue() % uint64_t( pow( 10, precision ) );
				BOOST_CHECK_EQUAL( CFixedPoint( value, precision, decimals ).ToUInt64(), uint64_t( value / pow( 10, decimals ) ) );
			}
			CLogger::LogInfo( StringStream() << "    From String" );
			{
				CLogger::LogInfo( StringStream() << "      Precision overflow in integer value" );
				BOOST_CHECK_THROW( CFixedPoint( STR( "12345678901" ), precision, decimals ), CDatabaseException );
				CLogger::LogInfo( StringStream() << "      Precision overflow in floating point value" );
				BOOST_CHECK_THROW( CFixedPoint( STR( "12345678901.23" ), precision, decimals ), CDatabaseException );
				CLogger::LogInfo( StringStream() << "      Precision overflow, hidden by decimals" );
				BOOST_CHECK_THROW( CFixedPoint( STR( "1234567890" ), precision, decimals ), CDatabaseException );
				CLogger::LogInfo( StringStream() << "      Negative integer Value" );
				BOOST_CHECK_EQUAL( CFixedPoint( STR( "-12345" ), precision, decimals ).ToInt64(), -12345 );
				CLogger::LogInfo( StringStream() << "      Positive integer Value" );
				BOOST_CHECK_EQUAL( CFixedPoint( STR( "12345" ), precision, decimals ).ToInt64(), 12345 );
				CLogger::LogInfo( StringStream() << "      Negative floating point Value" );
				BOOST_CHECK_EQUAL( CFixedPoint( STR( "-12345.6789" ), precision, decimals ).ToDouble(), -12345.6789 );
				CLogger::LogInfo( StringStream() << "      Positive floating point Value, not enough decimals" );
				BOOST_CHECK_EQUAL( CFixedPoint( STR( "12345.6789" ), precision, decimals ).ToDouble(), 12345.6789 );
				CLogger::LogInfo( StringStream() << "      Positive floating point Value, good decimals count" );
				BOOST_CHECK_EQUAL( CFixedPoint( STR( "1234.56789" ), precision, decimals ).ToDouble(), 1234.56789 );
				CLogger::LogInfo( StringStream() << "      Positive floating point Value, more decimals than needed" );
				BOOST_CHECK_EQUAL( CFixedPoint( STR( "123.456789" ), precision, decimals ).ToDouble(), 123.45678 );
			}
		}
		CLogger::LogInfo( StringStream() << "  Addition" );
		{
			CFixedPoint a, b, c;
			CLogger::LogInfo( StringStream() << "    No overflow" );
			a = CFixedPoint( 996, 3, 0 );
			b = CFixedPoint( 3, 3, 0 );
			BOOST_CHECK_EQUAL( a + b, CFixedPoint( 999, 3, 0 ) );
			CLogger::LogInfo( StringStream() << "    Precision overflow" );
			b = CFixedPoint( 4, 3, 0 );
			BOOST_CHECK_THROW( a + b, CDatabaseException );
			CLogger::LogInfo( StringStream() << "    Integer overflow" );
			c = CFixedPoint( 3, CFixedPoint::GetMaxPrecision(), 0 );
			b = CFixedPoint( std::numeric_limits< int64_t >::max() - 2, CFixedPoint::GetMaxPrecision(), 0 );
			BOOST_CHECK_THROW( c + b, CDatabaseException );
			CLogger::LogInfo( StringStream() << "    Different precisions" );
			b = CFixedPoint( 55, 5, 2 );
			a = CFixedPoint( 334, 3, 1 );
			BOOST_CHECK_EQUAL( a + b, CFixedPoint( 339, 3, 1 ) );
		}
		CLogger::LogInfo( StringStream() << "  Subtraction" );
		{
			CFixedPoint a, b, c;
			CLogger::LogInfo( StringStream() << "    No underflow" );
			a = CFixedPoint( 996, 3, 0 );
			b = CFixedPoint( 3, 3, 0 );
			BOOST_CHECK_EQUAL( b - a, CFixedPoint( -993, 3, 0 ) );
			CLogger::LogInfo( StringStream() << "    Integer underflow" );
			c = CFixedPoint( 3, CFixedPoint::GetMaxPrecision(), 0 );
			b = CFixedPoint( std::numeric_limits< int64_t >::lowest() + 2, CFixedPoint::GetMaxPrecision(), 0 );
			BOOST_CHECK_THROW( b - c, CDatabaseException );
			CLogger::LogInfo( StringStream() << "    Different precisions" );
			b = CFixedPoint( 55, 5, 2 );
			a = CFixedPoint( 334, 3, 1 );
			BOOST_CHECK_EQUAL( a - b, CFixedPoint( 329, 3, 1 ) );
		}
		CLogger::LogInfo( StringStream() << "  Multiplication" );
		{
			CFixedPoint a, b, c;
			CLogger::LogInfo( StringStream() << "    No overflow" );
			a = CFixedPoint( 333, 3, 0 );
			b = CFixedPoint( 3, 3, 0 );
			BOOST_CHECK_EQUAL( a * b, CFixedPoint( 999, 3, 0 ) );
			CLogger::LogInfo( StringStream() << "    Precision overflow" );
			b = CFixedPoint( 4, 3, 0 );
			BOOST_CHECK_THROW( a * b, CDatabaseException );
			CLogger::LogInfo( StringStream() << "    Integer overflow" );
			c = CFixedPoint( 3, CFixedPoint::GetMaxPrecision(), 0 );
			b = CFixedPoint( std::numeric_limits< int64_t >::max() / 2, CFixedPoint::GetMaxPrecision(), 0 );
			BOOST_CHECK_THROW( c * b, CDatabaseException );
			CLogger::LogInfo( StringStream() << "    Different precisions" );
			b = CFixedPoint( 3, 5, 2 );
			BOOST_CHECK_EQUAL( a * b, CFixedPoint( 9, 3, 0 ) );
			a = CFixedPoint( 334, 3, 0 );
			BOOST_CHECK_EQUAL( a * b, CFixedPoint( 10, 5, 0 ) );
		}
		CLogger::LogInfo( StringStream() << "  Division" );
		{
			CFixedPoint a, b, c;
			CLogger::LogInfo( StringStream() << "    No error, same precision" );
			a = CFixedPoint( 996, 3, 0 );
			b = CFixedPoint( 3, 3, 0 );
			BOOST_CHECK_EQUAL( a / b, CFixedPoint( 332, 3, 0 ) );
			CLogger::LogInfo( StringStream() << "    Division by zero" );
			c = CFixedPoint( 0, 1, 0 );
			BOOST_CHECK_THROW( b / c, CDatabaseException );
			CLogger::LogInfo( StringStream() << "    Different precisions" );
			b = CFixedPoint( 55, 5, 2 );
			a = CFixedPoint( 334, 3, 1 );
			BOOST_CHECK_EQUAL( a / b, CFixedPoint( 60, 3, 1 ) );
		}

		CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseFixedPoint ****" );
	}

	void CDatabaseInterfaceTest::TestCase_DatabaseInt24()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseInt24 ****" );



		CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseInt24 ****" );
	}

	void CDatabaseInterfaceTest::TestCase_DatabaseUInt24()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseUInt24 ****" );



		CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseUInt24 ****" );
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
