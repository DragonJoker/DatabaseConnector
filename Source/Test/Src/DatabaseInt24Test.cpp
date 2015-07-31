/************************************************************************//**
* @file DatabaseInt24Test.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 12/02/2014 14:29:35
*
*
* @brief Class testing int24_t type
*
***************************************************************************/

#include "DatabaseTestPch.h"

#include "DatabaseInt24Test.h"

#include "DatabaseTestUtils.h"

BEGIN_NAMESPACE_DATABASE_TEST
{
	namespace
	{
		template< typename T, typename Enable = void > struct Int24Check;

		template< typename T >
		struct Int24Check< T, typename std::enable_if< ( sizeof( T ) <= sizeof( int24_t ) ) && std::is_signed< T >::value && std::is_integral< T >::value >::type >
		{
			static void Construction()
			{
				{
					T value = std::numeric_limits< T >::max();
					BOOST_CHECK_EQUAL( T( int64_t( int24_t( value ) ) ), value );
				}
				{
					T value = std::numeric_limits< T >::min();
					BOOST_CHECK_EQUAL( T( int64_t( int24_t( value ) ) ), value );
				}
				{
					T value = std::numeric_limits< T >::lowest();
					BOOST_CHECK_EQUAL( T( int64_t( int24_t( value ) ) ), value );
				}
			}
		};

		template< typename T >
		struct Int24Check< T, typename std::enable_if< ( sizeof( T ) <= sizeof( int24_t ) ) && std::is_unsigned< T >::value >::type >
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
		struct Int24Check< T, typename std::enable_if< ( sizeof( T ) > sizeof( int24_t ) ) && std::is_signed< T >::value && std::is_integral< T >::value >::type >
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
		struct Int24Check< T, typename std::enable_if< ( sizeof( T ) > sizeof( int24_t ) ) && std::is_unsigned< T >::value >::type >
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
		struct Int24Check< T, typename std::enable_if< std::is_floating_point< T >::value >::type >
		{
			static void Construction()
			{
				{
					T value = std::numeric_limits< T >::max();
					BOOST_CHECK_EQUAL( int64_t( int24_t( value ) ), int64_t( int24_t( int64_t( value ) ) ) );
				}
				{
					T value = std::numeric_limits< T >::min();
					BOOST_CHECK_EQUAL( int64_t( int24_t( value ) ), int64_t( value ) );
				}
				{
					T value = std::numeric_limits< T >::lowest();
					BOOST_CHECK_EQUAL( int64_t( int24_t( value ) ), int64_t( int24_t( int64_t( value ) ) ) );
				}
			}
		};
	}

	CDatabaseInt24Test::CDatabaseInt24Test()
	{
	}

	CDatabaseInt24Test::~CDatabaseInt24Test()
	{
	}

	boost::unit_test::test_suite * CDatabaseInt24Test::Init_Test_Suite()
	{
		//!@remarks Create the internal TS instance.
		testSuite = new boost::unit_test::test_suite( "CDatabaseInt24Test" );

		//!@remarks Add the TC to the internal TS.
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseInt24Test::TestCase_Int24Construction, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseInt24Test::TestCase_Int24Addition, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseInt24Test::TestCase_Int24Subtraction, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseInt24Test::TestCase_Int24Multiplication, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseInt24Test::TestCase_Int24Division, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseInt24Test::TestCase_Int24LeftShift, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseInt24Test::TestCase_Int24RightShift, this ) ) );

		//!@remarks Return the TS instance.
		return testSuite;
	}

	void CDatabaseInt24Test::TestCase_Int24Construction()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_Int24Construction ****" );

		Int24Check< int8_t >::Construction();
		Int24Check< uint8_t >::Construction();
		Int24Check< int16_t >::Construction();
		Int24Check< uint16_t >::Construction();
		Int24Check< int24_t >::Construction();
		Int24Check< uint24_t >::Construction();
		Int24Check< int32_t >::Construction();
		Int24Check< uint32_t >::Construction();
		Int24Check< int64_t >::Construction();
		Int24Check< uint64_t >::Construction();
		Int24Check< float >::Construction();
		Int24Check< double >::Construction();

		CLogger::LogInfo( StringStream() << "**** End TestCase_Int24Construction ****" );
	}

	void CDatabaseInt24Test::TestCase_Int24Addition()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_Int24Addition ****" );

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

		CLogger::LogInfo( StringStream() << "**** End TestCase_Int24Addition ****" );
	}

	void CDatabaseInt24Test::TestCase_Int24Subtraction()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_Int24Subtraction ****" );

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

		CLogger::LogInfo( StringStream() << "**** End TestCase_Int24Subtraction ****" );
	}

	void CDatabaseInt24Test::TestCase_Int24Multiplication()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_Int24Multiplication ****" );

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

		CLogger::LogInfo( StringStream() << "**** End TestCase_Int24Multiplication ****" );
	}

	void CDatabaseInt24Test::TestCase_Int24Division()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_Int24Division ****" );

		int32_t a, b;
		CLogger::LogInfo( StringStream() << "    No error" );
		a = 333;
		b = 3;
		BOOST_CHECK_EQUAL( int24_t( a / b ), int24_t( a ) / int24_t( b ) );
		BOOST_CHECK_EQUAL( int24_t( a / b ), int24_t( 111 ) );
		CLogger::LogInfo( StringStream() << "    Division by zero" );
		b = 0;
		auto previousHandler = CDatabaseException::LinkSystemErrors();
		BOOST_CHECK_THROW( int24_t( a ) / int24_t( b ), CDatabaseException );
		CDatabaseException::UnlinkSystemErrors( std::move( previousHandler ) );

		CLogger::LogInfo( StringStream() << "**** End TestCase_Int24Division ****" );
	}

	void CDatabaseInt24Test::TestCase_Int24LeftShift()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_Int24LeftShift ****" );

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

		CLogger::LogInfo( StringStream() << "**** End TestCase_Int24LeftShift ****" );
	}

	void CDatabaseInt24Test::TestCase_Int24RightShift()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_Int24RightShift ****" );

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

		CLogger::LogInfo( StringStream() << "**** End TestCase_Int24RightShift ****" );
	}
}
END_NAMESPACE_DATABASE_TEST
