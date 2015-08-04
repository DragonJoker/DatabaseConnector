/************************************************************************//**
* @file DatabaseUInt24Test.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 12/02/2014 14:29:35
*
*
* @brief Class testing uint24_t type
*
***************************************************************************/

#include "DatabaseTestPch.h"

#include "DatabaseUInt24Test.h"

#include "DatabaseTestHelpers.h"

BEGIN_NAMESPACE_DATABASE_TEST
{
	namespace
	{
		template< typename T, typename Enable = void > struct UInt24Check;

		template< typename T >
		struct UInt24Check < T, typename std::enable_if < ( sizeof( T ) <= sizeof( uint24_t ) ) && std::is_signed< T >::value && std::is_integral< T >::value >::type >
		{
			static void Construction()
			{
				{
					T value = std::numeric_limits< T >::max();
					BOOST_CHECK_EQUAL( T( int64_t( uint24_t( value ) ) ), value );
				}
				{
					T value = std::numeric_limits< T >::min();
					BOOST_CHECK_EQUAL( T( int64_t( uint24_t( value ) ) ), value );
				}
				{
					T value = std::numeric_limits< T >::lowest();
					BOOST_CHECK_EQUAL( T( int64_t( uint24_t( value ) ) ), value );
				}
			}
		};

		template< typename T >
		struct UInt24Check < T, typename std::enable_if < ( sizeof( T ) <= sizeof( uint24_t ) ) && std::is_unsigned< T >::value >::type >
		{
			static void Construction()
			{
				{
					T value = std::numeric_limits< T >::max();
					BOOST_CHECK_EQUAL( T( uint64_t( uint24_t( value ) ) ), value );
				}
				{
					T value = std::numeric_limits< T >::min();
					BOOST_CHECK_EQUAL( T( uint64_t( uint24_t( value ) ) ), value );
				}
				{
					T value = std::numeric_limits< T >::lowest();
					BOOST_CHECK_EQUAL( T( uint64_t( uint24_t( value ) ) ), value );
				}
			}
		};

		template< typename T >
		struct UInt24Check < T, typename std::enable_if < ( sizeof( T ) > sizeof( uint24_t ) ) && std::is_integral< T >::value >::type >
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

		template< typename T >
		struct UInt24Check< T, typename std::enable_if< std::is_floating_point< T >::value >::type >
		{
			static void Construction()
			{
				{
					T value = std::numeric_limits< T >::max();
					BOOST_CHECK_EQUAL( int64_t( uint24_t( value ) ), int64_t( uint24_t( int64_t( value ) ) ) );
				}
				{
					T value = std::numeric_limits< T >::min();
					BOOST_CHECK_EQUAL( int64_t( uint24_t( value ) ), int64_t( value ) );
				}
				{
					T value = std::numeric_limits< T >::lowest();
					BOOST_CHECK_EQUAL( int64_t( uint24_t( value ) ), int64_t( uint24_t( int64_t( value ) ) ) );
				}
			}
		};
	}

	CDatabaseUInt24Test::CDatabaseUInt24Test()
	{
	}

	CDatabaseUInt24Test::~CDatabaseUInt24Test()
	{
	}

	boost::unit_test::test_suite * CDatabaseUInt24Test::Init_Test_Suite()
	{
		//!@remarks Create the internal TS instance.
		testSuite = new boost::unit_test::test_suite( "CDatabaseUInt24Test" );

		//!@remarks Add the TC to the internal TS.
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseUInt24Test::TestCase_UInt24Construction, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseUInt24Test::TestCase_UInt24Addition, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseUInt24Test::TestCase_UInt24Subtraction, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseUInt24Test::TestCase_UInt24Multiplication, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseUInt24Test::TestCase_UInt24Division, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseUInt24Test::TestCase_UInt24LeftShift, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseUInt24Test::TestCase_UInt24RightShift, this ) ) );

		//!@remarks Return the TS instance.
		return testSuite;
	}

	void CDatabaseUInt24Test::TestCase_UInt24Construction()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_UInt24Construction ****" );

		UInt24Check< int8_t >::Construction();
		UInt24Check< uint8_t >::Construction();
		UInt24Check< int16_t >::Construction();
		UInt24Check< uint16_t >::Construction();
		UInt24Check< int24_t >::Construction();
		UInt24Check< uint24_t >::Construction();
		UInt24Check< int32_t >::Construction();
		UInt24Check< uint32_t >::Construction();
		UInt24Check< int64_t >::Construction();
		UInt24Check< uint64_t >::Construction();
		UInt24Check< float >::Construction();
		UInt24Check< double >::Construction();

		CLogger::LogInfo( StringStream() << "**** End TestCase_UInt24Construction ****" );
	}

	void CDatabaseUInt24Test::TestCase_UInt24Addition()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_UInt24Addition ****" );

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

		CLogger::LogInfo( StringStream() << "**** End TestCase_UInt24Addition ****" );
	}

	void CDatabaseUInt24Test::TestCase_UInt24Subtraction()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_UInt24Subtraction ****" );

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

		CLogger::LogInfo( StringStream() << "**** End TestCase_UInt24Subtraction ****" );
	}

	void CDatabaseUInt24Test::TestCase_UInt24Multiplication()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_UInt24Multiplication ****" );

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

		CLogger::LogInfo( StringStream() << "**** End TestCase_UInt24Multiplication ****" );
	}

	void CDatabaseUInt24Test::TestCase_UInt24Division()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_UInt24Division ****" );

		uint32_t a, b;
		CLogger::LogInfo( StringStream() << "    No error" );
		a = 333;
		b = 3;
		BOOST_CHECK_EQUAL( uint24_t( a / b ), uint24_t( a ) / uint24_t( b ) );
		BOOST_CHECK_EQUAL( uint24_t( a / b ), uint24_t( 111 ) );
		CLogger::LogInfo( StringStream() << "    Division by zero" );
		b = 0;
		auto previousHandler = CDatabaseException::LinkSystemErrors();
		BOOST_CHECK_THROW( uint24_t( a ) / uint24_t( b ), CDatabaseException );
		CDatabaseException::UnlinkSystemErrors( std::move( previousHandler ) );

		CLogger::LogInfo( StringStream() << "**** End TestCase_UInt24Division ****" );
	}

	void CDatabaseUInt24Test::TestCase_UInt24LeftShift()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_UInt24LeftShift ****" );

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

		CLogger::LogInfo( StringStream() << "**** End TestCase_UInt24LeftShift ****" );
	}

	void CDatabaseUInt24Test::TestCase_UInt24RightShift()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_UInt24RightShift ****" );

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

		CLogger::LogInfo( StringStream() << "**** End TestCase_UInt24RightShift ****" );
	}
}
END_NAMESPACE_DATABASE_TEST
