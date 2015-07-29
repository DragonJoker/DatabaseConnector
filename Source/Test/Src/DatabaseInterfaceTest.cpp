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

		template< typename Int24Type, typename T, typename Enable = void > struct Int24Check;

		template< typename Int24Type, typename T >
		struct Int24Check< Int24Type, T, typename std::enable_if< ( sizeof( T ) <= sizeof( Int24Type ) ) && std::is_signed< T >::value && std::is_integral< T >::value >::type >
		{
			static void Construction()
			{
				{
					T value = std::numeric_limits< T >::max();
					BOOST_CHECK_EQUAL( T( int64_t( Int24Type( value ) ) ), value );
				}
				{
					T value = std::numeric_limits< T >::min();
					BOOST_CHECK_EQUAL( T( int64_t( Int24Type( value ) ) ), value );
				}
				{
					T value = std::numeric_limits< T >::lowest();
					BOOST_CHECK_EQUAL( T( int64_t( Int24Type( value ) ) ), value );
				}
			}
		};

		template< typename Int24Type, typename T >
		struct Int24Check< Int24Type, T, typename std::enable_if< ( sizeof( T ) <= sizeof( Int24Type ) ) && std::is_unsigned< T >::value >::type >
		{
			static void Construction()
			{
				{
					T value = std::numeric_limits< T >::max();
					BOOST_CHECK_EQUAL( T( uint64_t( Int24Type( value ) ) ), value );
				}
				{
					T value = std::numeric_limits< T >::min();
					BOOST_CHECK_EQUAL( T( uint64_t( Int24Type( value ) ) ), value );
				}
				{
					T value = std::numeric_limits< T >::lowest();
					BOOST_CHECK_EQUAL( T( uint64_t( Int24Type( value ) ) ), value );
				}
			}
		};

		template< typename T >
		struct Int24Check< int24_t, T, typename std::enable_if< ( sizeof( T ) > sizeof( int24_t ) ) && std::is_signed< T >::value && std::is_integral< T >::value >::type >
		{
			static void Construction()
			{
				{
					T value = std::numeric_limits< T >::max();
					BOOST_CHECK_EQUAL( int32_t( int24_t( value ) ), int32_t( 0xFFFFFFFF | value ) );
				}
				{
					T value = std::numeric_limits< T >::min();
					BOOST_CHECK_EQUAL( int32_t( int24_t( value ) ), int32_t( 0x00FFFFFF & value ) );
				}
				{
					T value = std::numeric_limits< T >::lowest();
					BOOST_CHECK_EQUAL( int32_t( int24_t( value ) ), int32_t( 0x00FFFFFF & value ) );
				}
			}
		};

		template< typename T >
		struct Int24Check< int24_t, T, typename std::enable_if< ( sizeof( T ) > sizeof( int24_t ) ) && std::is_unsigned< T >::value >::type >
		{
			static void Construction()
			{
				{
					T value = std::numeric_limits< T >::max();
					BOOST_CHECK_EQUAL( T( uint64_t( int24_t( value ) ) ), value );
				}
				{
					T value = std::numeric_limits< T >::min();
					BOOST_CHECK_EQUAL( T( uint64_t( int24_t( value ) ) ), value );
				}
				{
					T value = std::numeric_limits< T >::lowest();
					BOOST_CHECK_EQUAL( T( uint64_t( int24_t( value ) ) ), value );
				}
			}
		};

		template< typename T >
		struct Int24Check< uint24_t, T, typename std::enable_if< ( sizeof( T ) > sizeof( uint24_t ) ) && std::is_integral< T >::value >::type >
		{
			static void Construction()
			{
				{
					T value = std::numeric_limits< T >::max();
					BOOST_CHECK_EQUAL( T( uint64_t( uint24_t( value ) ) ), int32_t( 0x00FFFFFF & value ) );
				}
				{
					T value = std::numeric_limits< T >::min();
					BOOST_CHECK_EQUAL( T( uint64_t( uint24_t( value ) ) ), int32_t( 0x00FFFFFF & value ) );
				}
				{
					T value = std::numeric_limits< T >::lowest();
					BOOST_CHECK_EQUAL( T( uint64_t( uint24_t( value ) ) ), int32_t( 0x00FFFFFF & value ) );
				}
			}
		};

		template< typename Int24Type, typename T >
		struct Int24Check< Int24Type, T, typename std::enable_if< std::is_floating_point< T >::value >::type >
		{
			static void Construction()
			{
				{
					T value = std::numeric_limits< T >::max();
					BOOST_CHECK_EQUAL( int64_t( Int24Type( value ) ), int64_t( Int24Type( int64_t( value ) ) ) );
				}
				{
					T value = std::numeric_limits< T >::min();
					BOOST_CHECK_EQUAL( int64_t( Int24Type( value ) ), int64_t( value ) );
				}
				{
					T value = std::numeric_limits< T >::lowest();
					BOOST_CHECK_EQUAL( int64_t( Int24Type( value ) ), int64_t( Int24Type( int64_t( value ) ) ) );
				}
			}
		};
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

		CLogger::LogInfo( StringStream() << "  Construction" );
		{
			Int24Check< int24_t, int8_t >::Construction();
			Int24Check< int24_t, uint8_t >::Construction();
			Int24Check< int24_t, int16_t >::Construction();
			Int24Check< int24_t, uint16_t >::Construction();
			Int24Check< int24_t, int24_t >::Construction();
			Int24Check< int24_t, uint24_t >::Construction();
			Int24Check< int24_t, int32_t >::Construction();
			Int24Check< int24_t, uint32_t >::Construction();
			Int24Check< int24_t, int64_t >::Construction();
			Int24Check< int24_t, uint64_t >::Construction();
			Int24Check< int24_t, float >::Construction();
			Int24Check< int24_t, double >::Construction();
		}
		CLogger::LogInfo( StringStream() << "  Addition" );
		{
			int32_t a, b;
			CLogger::LogInfo( StringStream() << "    No overflow" );
			a = 996;
			b = 3;
			BOOST_CHECK_EQUAL( int24_t( a + b ), int24_t( a ) + int24_t( b ) );
			BOOST_CHECK_EQUAL( int24_t( a + b ), int24_t( 999 ) );
			CLogger::LogInfo( StringStream() << "    Integer overflow" );
			a = int32_t( std::numeric_limits< int24_t >::max() ) - 2;
			BOOST_CHECK_EQUAL( int24_t( a + b ), int24_t( a ) + int24_t( b ) );
			BOOST_CHECK_EQUAL( int24_t( a + b ), std::numeric_limits< int24_t >::lowest() );
		}
		CLogger::LogInfo( StringStream() << "  Subtraction" );
		{
			int32_t a, b;
			CLogger::LogInfo( StringStream() << "    No underflow" );
			a = 996;
			b = 3;
			BOOST_CHECK_EQUAL( int24_t( b - a ), int24_t( b ) - int24_t( a ) );
			BOOST_CHECK_EQUAL( int24_t( b - a ), int24_t( -993 ) );
			CLogger::LogInfo( StringStream() << "    Integer underflow" );
			a = int32_t( std::numeric_limits< int24_t >::lowest() ) + 2;
			BOOST_CHECK_EQUAL( int24_t( a - b ), int24_t( a ) - int24_t( b ) );
			BOOST_CHECK_EQUAL( int24_t( a - b ), std::numeric_limits< int24_t >::max() );
		}
		CLogger::LogInfo( StringStream() << "  Multiplication" );
		{
			int32_t a, b;
			CLogger::LogInfo( StringStream() << "    No overflow" );
			a = 333;
			b = 3;
			BOOST_CHECK_EQUAL( int24_t( a * b ), int24_t( a ) * int24_t( b ) );
			BOOST_CHECK_EQUAL( int24_t( a * b ), int24_t( 999 ) );
			CLogger::LogInfo( StringStream() << "    Integer overflow" );
			a = int32_t( std::numeric_limits< int24_t >::max() ) / 2;
			BOOST_CHECK_EQUAL( int24_t( a * b ), int24_t( a ) * int24_t( b ) );
			BOOST_CHECK_EQUAL( int24_t( a * b ), int24_t( int32_t( std::numeric_limits< int24_t >::lowest() ) / 2 - 3 ) );
		}
		CLogger::LogInfo( StringStream() << "  Division" );
		{
			int32_t a, b;
			CLogger::LogInfo( StringStream() << "    No error" );
			a = 333;
			b = 3;
			BOOST_CHECK_EQUAL( int24_t( a / b ), int24_t( a ) / int24_t( b ) );
			BOOST_CHECK_EQUAL( int24_t( a / b ), int24_t( 111 ) );
			CLogger::LogInfo( StringStream() << "    Division by zero" );
			b = 0;
			CDatabaseException::LinkSystemErrors();
			BOOST_CHECK_THROW( int24_t( a ) / int24_t( b ), CDatabaseException );
			CDatabaseException::UnlinkSystemErrors();
		}
		CLogger::LogInfo( StringStream() << "  Left shift" );
		{
			int32_t a;
			int b;
			CLogger::LogInfo( StringStream() << "    No overflow" );
			a = 8;
			b = 3;
			BOOST_CHECK_EQUAL( int24_t( a << b ), int24_t( a ) << b );
			BOOST_CHECK_EQUAL( int24_t( a ) << b, int24_t( 64 ) );
			CLogger::LogInfo( StringStream() << "    Integer overflow" );
			a = 0x007FFFFF >> 2;
			BOOST_CHECK_EQUAL( int24_t( a << b ), int24_t( a ) << b );
			BOOST_CHECK_EQUAL( int24_t( a ) << b, int24_t( -8 ) );
		}
		CLogger::LogInfo( StringStream() << "  Right shift" );
		{
			int32_t a;
			int b;
			CLogger::LogInfo( StringStream() << "    No underflow" );
			a = 64;
			b = 3;
			BOOST_CHECK_EQUAL( int24_t( a >> b ), int24_t( a ) >> b );
			BOOST_CHECK_EQUAL( int24_t( a ) >> b, int24_t( 8 ) );
			CLogger::LogInfo( StringStream() << "    Underflow" );
			a = 0x00000001 << 2;
			BOOST_CHECK_EQUAL( int24_t( a >> b ), int24_t( a ) >> b );
			BOOST_CHECK_EQUAL( int24_t( a ) >> b, int24_t( 0 ) );
		}

		CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseInt24 ****" );
	}

	void CDatabaseInterfaceTest::TestCase_DatabaseUInt24()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseUInt24 ****" );

		CLogger::LogInfo( StringStream() << "  Construction" );
		{
			Int24Check< uint24_t, int8_t >::Construction();
			Int24Check< uint24_t, uint8_t >::Construction();
			Int24Check< uint24_t, int16_t >::Construction();
			Int24Check< uint24_t, uint16_t >::Construction();
			Int24Check< uint24_t, int24_t >::Construction();
			Int24Check< uint24_t, uint24_t >::Construction();
			Int24Check< uint24_t, int32_t >::Construction();
			Int24Check< uint24_t, uint32_t >::Construction();
			Int24Check< uint24_t, int64_t >::Construction();
			Int24Check< uint24_t, uint64_t >::Construction();
			Int24Check< uint24_t, float >::Construction();
			Int24Check< uint24_t, double >::Construction();
		}
		CLogger::LogInfo( StringStream() << "  Addition" );
		{
			uint32_t a, b;
			CLogger::LogInfo( StringStream() << "    No overflow" );
			a = 996;
			b = 3;
			BOOST_CHECK_EQUAL( uint24_t( a + b ), uint24_t( a ) + uint24_t( b ) );
			BOOST_CHECK_EQUAL( uint24_t( a + b ), uint24_t( 999 ) );
			CLogger::LogInfo( StringStream() << "    Integer overflow" );
			a = uint32_t( std::numeric_limits< uint24_t >::max() ) - 2;
			BOOST_CHECK_EQUAL( uint24_t( a + b ), uint24_t( a ) + uint24_t( b ) );
			BOOST_CHECK_EQUAL( uint24_t( a + b ), std::numeric_limits< uint24_t >::lowest() );
		}
		CLogger::LogInfo( StringStream() << "  Subtraction" );
		{
			uint32_t a, b;
			CLogger::LogInfo( StringStream() << "    No underflow" );
			a = 996;
			b = 3;
			BOOST_CHECK_EQUAL( uint24_t( b - a ), uint24_t( b ) - uint24_t( a ) );
			BOOST_CHECK_EQUAL( uint24_t( b - a ), uint24_t( -993 ) );
			CLogger::LogInfo( StringStream() << "    Integer underflow" );
			a = uint32_t( std::numeric_limits< uint24_t >::lowest() ) + 2;
			BOOST_CHECK_EQUAL( uint24_t( a - b ), uint24_t( a ) - uint24_t( b ) );
			BOOST_CHECK_EQUAL( uint24_t( a - b ), std::numeric_limits< uint24_t >::max() );
		}
		CLogger::LogInfo( StringStream() << "  Multiplication" );
		{
			uint32_t a, b;
			CLogger::LogInfo( StringStream() << "    No overflow" );
			a = 333;
			b = 3;
			BOOST_CHECK_EQUAL( uint24_t( a * b ), uint24_t( a ) * uint24_t( b ) );
			BOOST_CHECK_EQUAL( uint24_t( a * b ), uint24_t( 999 ) );
			CLogger::LogInfo( StringStream() << "    Integer overflow" );
			a = uint32_t( std::numeric_limits< uint24_t >::max() ) / 2;
			BOOST_CHECK_EQUAL( uint24_t( a * b ), uint24_t( a ) * uint24_t( b ) );
			BOOST_CHECK_EQUAL( uint24_t( a * b ), uint24_t( uint32_t( std::numeric_limits< uint24_t >::max() ) / 2 - 2 ) );
		}
		CLogger::LogInfo( StringStream() << "  Division" );
		{
			uint32_t a, b;
			CLogger::LogInfo( StringStream() << "    No error" );
			a = 333;
			b = 3;
			BOOST_CHECK_EQUAL( uint24_t( a / b ), uint24_t( a ) / uint24_t( b ) );
			BOOST_CHECK_EQUAL( uint24_t( a / b ), uint24_t( 111 ) );
			CLogger::LogInfo( StringStream() << "    Division by zero" );
			b = 0;
			CDatabaseException::LinkSystemErrors();
			BOOST_CHECK_THROW( uint24_t( a ) / uint24_t( b ), CDatabaseException );
			CDatabaseException::UnlinkSystemErrors();
		}
		CLogger::LogInfo( StringStream() << "  Left shift" );
		{
			uint32_t a;
			int b;
			CLogger::LogInfo( StringStream() << "    No overflow" );
			a = 8;
			b = 3;
			BOOST_CHECK_EQUAL( uint24_t( a << b ), uint24_t( a ) << b );
			BOOST_CHECK_EQUAL( uint24_t( a ) << b, uint24_t( 64 ) );
			CLogger::LogInfo( StringStream() << "    Integer overflow" );
			a = 0x007FFFFF >> 2;
			BOOST_CHECK_EQUAL( uint24_t( a << b ), uint24_t( a ) << b );
			BOOST_CHECK_EQUAL( uint24_t( a ) << b, uint24_t( -8 ) );
		}
		CLogger::LogInfo( StringStream() << "  Right shift" );
		{
			uint32_t a;
			int b;
			CLogger::LogInfo( StringStream() << "    No underflow" );
			a = 64;
			b = 3;
			BOOST_CHECK_EQUAL( uint24_t( a >> b ), uint24_t( a ) >> b );
			BOOST_CHECK_EQUAL( uint24_t( a ) >> b, uint24_t( 8 ) );
			CLogger::LogInfo( StringStream() << "    underflow" );
			a = 0x00000001 << 2;
			BOOST_CHECK_EQUAL( uint24_t( a >> b ), uint24_t( a ) >> b );
			BOOST_CHECK_EQUAL( uint24_t( a ) >> b, uint24_t( 0 ) );
		}

		CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseUInt24 ****" );
	}

	void CDatabaseInterfaceTest::TestCase_DatabaseNullable()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseNullable ****" );

		CDatabaseNullable< int32_t > value;
		CLogger::LogInfo( StringStream() << "  Unset" );
		BOOST_CHECK( !value );
		BOOST_CHECK_THROW( *value, CDatabaseException );
		CLogger::LogInfo( StringStream() << "  Set to value" );
		value = 42;
		BOOST_CHECK( (bool)value );
		BOOST_CHECK_EQUAL( *value, 42 );
		CLogger::LogInfo( StringStream() << "  Set to None" );
		value = None;
		BOOST_CHECK( !value );
		BOOST_CHECK_THROW( *value, CDatabaseException );

		CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseNullable ****" );
	}

	void CDatabaseInterfaceTest::TestCase_DatabaseStringUtils()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseStringUtils ****" );

		CLogger::LogInfo( StringStream() << "  IsUpperCase/IsLowerCase" );
		BOOST_CHECK( !StringUtils::IsUpperCase( "NoTuPpErCaSe" ) );
		BOOST_CHECK( StringUtils::IsUpperCase( "UPPERCASE" ) );
		BOOST_CHECK( !StringUtils::IsLowerCase( "NoTlOwErCaSe" ) );
		BOOST_CHECK( StringUtils::IsLowerCase( "lowercase" ) );
		BOOST_CHECK( !StringUtils::IsUpperCase( L"NoTuPpErCaSe" ) );
		BOOST_CHECK( StringUtils::IsUpperCase( L"UPPERCASE" ) );
		BOOST_CHECK( !StringUtils::IsLowerCase( L"NoTlOwErCaSe" ) );
		BOOST_CHECK( StringUtils::IsLowerCase( L"lowercase" ) );

		CLogger::LogInfo( StringStream() << "  UpperCase/LowerCase" );
		BOOST_CHECK_EQUAL( StringUtils::UpperCase( "NoTuPpErCaSe" ), "NOTUPPERCASE" );
		BOOST_CHECK_EQUAL( StringUtils::UpperCase( "UPPERCASE" ), "UPPERCASE" );
		BOOST_CHECK_EQUAL( StringUtils::LowerCase( "NoTlOwErCaSe" ), "notlowercase" );
		BOOST_CHECK_EQUAL( StringUtils::LowerCase( "lowercase" ), "lowercase" );
		BOOST_CHECK_EQUAL( StringUtils::UpperCase( L"NoTuPpErCaSe" ), L"NOTUPPERCASE" );
		BOOST_CHECK_EQUAL( StringUtils::UpperCase( L"UPPERCASE" ), L"UPPERCASE" );
		BOOST_CHECK_EQUAL( StringUtils::LowerCase( L"NoTlOwErCaSe" ), L"notlowercase" );
		BOOST_CHECK_EQUAL( StringUtils::LowerCase( L"lowercase" ), L"lowercase" );

		CLogger::LogInfo( StringStream() << "  ToUpperCase/ToLowerCase" );
		std::string snotupper = "NoTuPpErCaSe";
		std::string supper = "UPPERCASE";
		std::string snotlower = "NoTlOwErCaSe";
		std::string slower = "lowercase";
		BOOST_CHECK_EQUAL( StringUtils::ToUpperCase( snotupper ), "NOTUPPERCASE" );
		BOOST_CHECK_EQUAL( StringUtils::ToUpperCase( supper ), "UPPERCASE" );
		BOOST_CHECK_EQUAL( StringUtils::ToLowerCase( snotlower ), "notlowercase" );
		BOOST_CHECK_EQUAL( StringUtils::ToLowerCase( slower ), "lowercase" );
		BOOST_CHECK_EQUAL( snotupper, "NOTUPPERCASE" );
		BOOST_CHECK_EQUAL( supper, "UPPERCASE" );
		BOOST_CHECK_EQUAL( snotlower, "notlowercase" );
		BOOST_CHECK_EQUAL( slower, "lowercase" );

		std::wstring wnotupper = L"NoTuPpErCaSe";
		std::wstring wupper = L"UPPERCASE";
		std::wstring wnotlower = L"NoTlOwErCaSe";
		std::wstring wlower = L"lowercase";
		BOOST_CHECK_EQUAL( StringUtils::ToUpperCase( wnotupper ), L"NOTUPPERCASE" );
		BOOST_CHECK_EQUAL( StringUtils::ToUpperCase( wupper ), L"UPPERCASE" );
		BOOST_CHECK_EQUAL( StringUtils::ToLowerCase( wnotlower ), L"notlowercase" );
		BOOST_CHECK_EQUAL( StringUtils::ToLowerCase( wlower ), L"lowercase" );
		BOOST_CHECK_EQUAL( wnotupper, L"NOTUPPERCASE" );
		BOOST_CHECK_EQUAL( wupper, L"UPPERCASE" );
		BOOST_CHECK_EQUAL( wnotlower, L"notlowercase" );
		BOOST_CHECK_EQUAL( wlower, L"lowercase" );

		CLogger::LogInfo( StringStream() << "  ToWStr/ToStr/ToString/ToUtf8" );
		std::wstring ws = L"ÂÄÊËÎÏÔÖÛÜ";
		std::string s = "ÂÄÊËÎÏÔÖÛÜ";
		std::string utf8 = "Ã‚Ã„ÃŠÃ‹ÃŽÃÃ”Ã–Ã›Ãœ";
		BOOST_CHECK_EQUAL( ws, StringUtils::ToWStr( s ) );
		BOOST_CHECK_EQUAL( ws, StringUtils::ToWStr( ws ) );
		BOOST_CHECK_EQUAL( s, StringUtils::ToStr( ws ) );
		BOOST_CHECK_EQUAL( s, StringUtils::ToStr( s ) );
		BOOST_CHECK_EQUAL( StringUtils::ToString( ws ), s );
		BOOST_CHECK_EQUAL( StringUtils::ToString( s ), s );
		BOOST_CHECK_EQUAL( StringUtils::ToUtf8( s, "Windows-1252" ), utf8 );
		BOOST_CHECK_EQUAL( StringUtils::ToUtf8( ws, L"Windows-1252" ), utf8 );

		CLogger::LogInfo( StringStream() << "  Split" );
		std::string stosplit = "dsfs,gsdg,,sdfh,sdh,dshgh,dfh,dsfh,dsfhsdfhsd,fhsdfh,dfhdsh";
		auto ssplitted = StringUtils::Split( stosplit, ",", 1, false );
		BOOST_CHECK_EQUAL( ssplitted.size(), 2 );
		BOOST_CHECK_EQUAL( ssplitted[0], "dsfs" );
		BOOST_CHECK_EQUAL( ssplitted[1], "gsdg,,sdfh,sdh,dshgh,dfh,dsfh,dsfhsdfhsd,fhsdfh,dfhdsh" );
		uint32_t scount = uint32_t( std::count( stosplit.begin(), stosplit.end(), ',' ) );
		BOOST_CHECK_NO_THROW( ssplitted = StringUtils::Split( stosplit, ",", scount, false ) );
		BOOST_CHECK_EQUAL( ssplitted.size(), scount );
		BOOST_CHECK_NO_THROW( ssplitted = StringUtils::Split( stosplit, ",", scount, true ) );
		BOOST_CHECK_EQUAL( ssplitted.size(), scount + 1 );
		std::wstring wtosplit = L"dsfs,gsdg,,sdfh,sdh,dshgh,dfh,dsfh,dsfhsdfhsd,fhsdfh,dfhdsh";
		auto wsplitted = StringUtils::Split( wtosplit, L",", 1, false );
		BOOST_CHECK_EQUAL( wsplitted.size(), 2 );
		BOOST_CHECK_EQUAL( wsplitted[0], L"dsfs" );
		BOOST_CHECK_EQUAL( wsplitted[1], L"gsdg,,sdfh,sdh,dshgh,dfh,dsfh,dsfhsdfhsd,fhsdfh,dfhdsh" );
		uint32_t wcount = uint32_t( std::count( wtosplit.begin(), wtosplit.end(), L',' ) );
		BOOST_CHECK_NO_THROW( wsplitted = StringUtils::Split( wtosplit, L",", wcount, false ) );
		BOOST_CHECK_EQUAL( wsplitted.size(), wcount );
		BOOST_CHECK_NO_THROW( wsplitted = StringUtils::Split( wtosplit, L",", wcount, true ) );
		BOOST_CHECK_EQUAL( wsplitted.size(), wcount + 1 );

		CLogger::LogInfo( StringStream() << "  Trim" );
		std::string stotrim = "AA";
		BOOST_CHECK_EQUAL( StringUtils::Trim( stotrim ), "AA" );
		BOOST_CHECK_EQUAL( stotrim, "AA" );
		stotrim = "AA ";
		BOOST_CHECK_EQUAL( StringUtils::Trim( stotrim ), "AA" );
		BOOST_CHECK_EQUAL( stotrim, "AA" );
		stotrim = " AA";
		BOOST_CHECK_EQUAL( StringUtils::Trim( stotrim ), "AA" );
		BOOST_CHECK_EQUAL( stotrim, "AA" );
		stotrim = " AA ";
		BOOST_CHECK_EQUAL( StringUtils::Trim( stotrim, false ), " AA" );
		BOOST_CHECK_EQUAL( stotrim, " AA" );
		stotrim = " AA ";
		BOOST_CHECK_EQUAL( StringUtils::Trim( stotrim, true, false ), "AA " );
		BOOST_CHECK_EQUAL( stotrim, "AA " );
		stotrim = " AA ";
		BOOST_CHECK_EQUAL( StringUtils::Trim( stotrim, false, false ), " AA " );
		BOOST_CHECK_EQUAL( stotrim, " AA " );
		std::wstring wtotrim = L"AA";
		BOOST_CHECK_EQUAL( StringUtils::Trim( wtotrim ), L"AA" );
		BOOST_CHECK_EQUAL( wtotrim, L"AA" );
		wtotrim = L"AA ";
		BOOST_CHECK_EQUAL( StringUtils::Trim( wtotrim ), L"AA" );
		BOOST_CHECK_EQUAL( wtotrim, L"AA" );
		wtotrim = L" AA";
		BOOST_CHECK_EQUAL( StringUtils::Trim( wtotrim ), L"AA" );
		BOOST_CHECK_EQUAL( wtotrim, L"AA" );
		wtotrim = L" AA ";
		BOOST_CHECK_EQUAL( StringUtils::Trim( wtotrim, false ), L" AA" );
		BOOST_CHECK_EQUAL( wtotrim, L" AA" );
		wtotrim = L" AA ";
		BOOST_CHECK_EQUAL( StringUtils::Trim( wtotrim, true, false ), L"AA " );
		BOOST_CHECK_EQUAL( wtotrim, L"AA " );
		wtotrim = L" AA ";
		BOOST_CHECK_EQUAL( StringUtils::Trim( wtotrim, false, false ), L" AA " );
		BOOST_CHECK_EQUAL( wtotrim, L" AA " );

		CLogger::LogInfo( StringStream() << "  Replace" );
		std::string sreplace = "gsdg,,sdfh,sdh,dshgh,dfh,dsfh,dsfhsdfhsd,fhsdfh,dfhdsh";
		BOOST_CHECK_EQUAL( StringUtils::Replace( sreplace, ",", " " ), "gsdg  sdfh sdh dshgh dfh dsfh dsfhsdfhsd fhsdfh dfhdsh" );
		BOOST_CHECK_EQUAL( sreplace, "gsdg  sdfh sdh dshgh dfh dsfh dsfhsdfhsd fhsdfh dfhdsh" );
		BOOST_CHECK_EQUAL( StringUtils::Replace( sreplace, "gsdg", "je" ), "je  sdfh sdh dshgh dfh dsfh dsfhsdfhsd fhsdfh dfhdsh" );
		BOOST_CHECK_EQUAL( sreplace, "je  sdfh sdh dshgh dfh dsfh dsfhsdfhsd fhsdfh dfhdsh" );
		BOOST_CHECK_EQUAL( StringUtils::Replace( sreplace, "dfhdsh", "voilà" ), "je  sdfh sdh dshgh dfh dsfh dsfhsdfhsd fhsdfh voilà" );
		BOOST_CHECK_EQUAL( sreplace, "je  sdfh sdh dshgh dfh dsfh dsfhsdfhsd fhsdfh voilà" );
		BOOST_CHECK_EQUAL( StringUtils::Replace( sreplace, "coin", "glop" ), "je  sdfh sdh dshgh dfh dsfh dsfhsdfhsd fhsdfh voilà" );
		BOOST_CHECK_EQUAL( sreplace, "je  sdfh sdh dshgh dfh dsfh dsfhsdfhsd fhsdfh voilà" );
		std::wstring wreplace = L"gsdg,,sdfh,sdh,dshgh,dfh,dsfh,dsfhsdfhsd,fhsdfh,dfhdsh";
		BOOST_CHECK_EQUAL( StringUtils::Replace( wreplace, L",", L" " ), L"gsdg  sdfh sdh dshgh dfh dsfh dsfhsdfhsd fhsdfh dfhdsh" );
		BOOST_CHECK_EQUAL( wreplace, L"gsdg  sdfh sdh dshgh dfh dsfh dsfhsdfhsd fhsdfh dfhdsh" );
		BOOST_CHECK_EQUAL( StringUtils::Replace( wreplace, L"gsdg", L"je" ), L"je  sdfh sdh dshgh dfh dsfh dsfhsdfhsd fhsdfh dfhdsh" );
		BOOST_CHECK_EQUAL( wreplace, L"je  sdfh sdh dshgh dfh dsfh dsfhsdfhsd fhsdfh dfhdsh" );
		BOOST_CHECK_EQUAL( StringUtils::Replace( wreplace, L"dfhdsh", L"voilà" ), L"je  sdfh sdh dshgh dfh dsfh dsfhsdfhsd fhsdfh voilà" );
		BOOST_CHECK_EQUAL( wreplace, L"je  sdfh sdh dshgh dfh dsfh dsfhsdfhsd fhsdfh voilà" );
		BOOST_CHECK_EQUAL( StringUtils::Replace( wreplace, L"coin", L"glop" ), L"je  sdfh sdh dshgh dfh dsfh dsfhsdfhsd fhsdfh voilà" );
		BOOST_CHECK_EQUAL( wreplace, L"je  sdfh sdh dshgh dfh dsfh dsfhsdfhsd fhsdfh voilà" );

		CLogger::LogInfo( StringStream() << "  Formalize" );
		std::string sformalized;
		CDatabaseException::LinkSystemErrors();
		BOOST_CHECK_NO_THROW( StringUtils::Formalize( sformalized, 10, "%010i", 100 ) );
		BOOST_CHECK_EQUAL( sformalized, "0000000100" );
		BOOST_CHECK_NO_THROW( StringUtils::Formalize( sformalized, 9, "%010i", 100 ) );
		BOOST_CHECK_EQUAL( sformalized, "000000010" );
		BOOST_CHECK_NO_THROW( StringUtils::Formalize( sformalized, 9, "%.2f", 100 ) );
		BOOST_CHECK_EQUAL( sformalized, "0.00" );
		BOOST_CHECK_NO_THROW( StringUtils::Formalize( sformalized, 9, "%.2f", 100.0f ) );
		BOOST_CHECK_EQUAL( sformalized, "100.00" );
		BOOST_CHECK_THROW( StringUtils::Formalize( sformalized, 9, "%s", 100 ), CDatabaseException );
		CDatabaseException::UnlinkSystemErrors();

		CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseStringUtils ****" );
	}

	void CDatabaseInterfaceTest::TestCase_DatabaseConnection()
	{
		//CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseConnection ****" );



		//CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseConnection ****" );
	}

	void CDatabaseInterfaceTest::TestCase_DatabaseFieldInfos()
	{
		//CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseFieldInfos ****" );



		//CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseFieldInfos ****" );
	}

	void CDatabaseInterfaceTest::TestCase_DatabaseValue()
	{
		//CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseValue ****" );



		//CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseValue ****" );
	}

	void CDatabaseInterfaceTest::TestCase_DatabaseValuedObject()
	{
		//CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseValuedObject ****" );



		//CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseValuedObject ****" );
	}

	void CDatabaseInterfaceTest::TestCase_DatabaseParameter()
	{
		//CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseParameter ****" );



		//CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseParameter ****" );
	}

	void CDatabaseInterfaceTest::TestCase_DatabaseField()
	{
		//CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseField ****" );



		//CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseField ****" );
	}

	void CDatabaseInterfaceTest::TestCase_DatabaseRow()
	{
		//CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseRow ****" );



		//CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseRow ****" );
	}

	void CDatabaseInterfaceTest::TestCase_DatabaseResult()
	{
		//CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseResult ****" );



		//CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseResult ****" );
	}

	void CDatabaseInterfaceTest::TestCase_DatabaseQuery()
	{
		//CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseQuery ****" );



		//CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseQuery ****" );
	}

	void CDatabaseInterfaceTest::TestCase_DatabaseStatement()
	{
		//CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseStatement ****" );



		//CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseStatement ****" );
	}
}
END_NAMESPACE_DATABASE_TEST
