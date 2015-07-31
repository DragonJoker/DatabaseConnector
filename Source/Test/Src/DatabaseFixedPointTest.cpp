/************************************************************************//**
 * @file DatabaseFixedPointTest.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 12/02/2014 14:29:35
 *
 *
 * @brief Class testing CFixedPoint class
*
***************************************************************************/

#include "DatabaseTestPch.h"

#include "DatabaseFixedPointTest.h"

#include "DatabaseTestUtils.h"

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

	CDatabaseFixedPointTest::CDatabaseFixedPointTest()
	{
	}

	CDatabaseFixedPointTest::~CDatabaseFixedPointTest()
	{
	}

	boost::unit_test::test_suite * CDatabaseFixedPointTest::Init_Test_Suite()
	{
		//!@remarks Create the internal TS instance.
		testSuite = new boost::unit_test::test_suite( "CDatabaseFixedPointTest" );

		//!@remarks Add the TC to the internal TS.
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseFixedPointTest::TestCase_FixedPointOutOfRange, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseFixedPointTest::TestCase_FixedPointMinPrecision, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseFixedPointTest::TestCase_FixedPointMaxPrecisionMinDecimals, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseFixedPointTest::TestCase_FixedPointMaxPrecisionMaxDecimals, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseFixedPointTest::TestCase_FixedPointMedPrecisionMedDecimals, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseFixedPointTest::TestCase_FixedPointConstruction, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseFixedPointTest::TestCase_FixedPointAddition, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseFixedPointTest::TestCase_FixedPointSubtraction, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseFixedPointTest::TestCase_FixedPointMultiplication, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseFixedPointTest::TestCase_FixedPointDivision, this ) ) );

		//!@remarks Return the TS instance.
		return testSuite;
	}

	void CDatabaseFixedPointTest::TestCase_FixedPointOutOfRange()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_FixedPointOutOfRange ****" );

		CLogger::LogInfo( StringStream() << "  Invalid Precision inferior to min" );
		BOOST_CHECK_THROW( CFixedPoint( 0, CFixedPoint::GetMinPrecision() - 1, 0 ), CDatabaseException );

		CLogger::LogInfo( StringStream() << "  Invalid Precision superior to max" );
		BOOST_CHECK_THROW( CFixedPoint( 0, CFixedPoint::GetMaxPrecision() + 1, 0 ), CDatabaseException );

		CLogger::LogInfo( StringStream() << "  Invalid dDecimals equal to precision" );
		BOOST_CHECK_THROW( CFixedPoint( 0, CFixedPoint::GetMaxPrecision() / 2, CFixedPoint::GetMaxPrecision() / 2 ), CDatabaseException );

		CLogger::LogInfo( StringStream() << "  Invalid Decimals superior to precision" );
		BOOST_CHECK_THROW( CFixedPoint( 0, CFixedPoint::GetMaxPrecision() / 2, 1 + CFixedPoint::GetMaxPrecision() / 2 ), CDatabaseException );

		CLogger::LogInfo( StringStream() << "  Min Precision: " << int( CFixedPoint::GetMinPrecision() ) );

		CLogger::LogInfo( StringStream() << "**** End TestCase_FixedPointOutOfRange ****" );
	}

	void CDatabaseFixedPointTest::TestCase_FixedPointMinPrecision()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_FixedPointMinPrecision ****" );

		int precision = CFixedPoint::GetMinPrecision();
		int decimals = 0;
		int64_t value;
		CLogger::LogInfo( StringStream() << "  Max Value" );
		value = int64_t( std::numeric_limits< int64_t >::max() % int256_t( pow( 10, precision ) ) );
		CheckFixedPoint( precision, decimals, value, STR( "0" ) );
		CLogger::LogInfo( StringStream() << "  Min Value" );
		value = int64_t( std::numeric_limits< int64_t >::min() % int256_t( pow( 10, precision ) ) );
		CheckFixedPoint( precision, decimals, value, STR( "0" ) );
		CLogger::LogInfo( StringStream() << "  Lowest Value" );
		value = int64_t( std::numeric_limits< int64_t >::lowest() % int256_t( pow( 10, precision ) ) );
		CheckFixedPoint( precision, decimals, value, STR( "0" ) );
		CLogger::LogInfo( StringStream() << "  Invalid Value (greater precision than allowed)" );
		BOOST_CHECK_THROW( CFixedPoint( std::numeric_limits< int64_t >::max(), precision, decimals ), CDatabaseException );

		CLogger::LogInfo( StringStream() << "**** End TestCase_FixedPointMinPrecision ****" );
	}

	void CDatabaseFixedPointTest::TestCase_FixedPointMaxPrecisionMinDecimals()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_FixedPointMaxPrecisionMinDecimals ****" );
		int precision = CFixedPoint::GetMaxPrecision();
		int decimals = 0;
		int64_t value;
		CLogger::LogInfo( StringStream() << "  Max Value" );
		value = int64_t( std::numeric_limits< int64_t >::max() % int256_t( pow( 10, precision ) ) );
		CheckFixedPoint( precision, decimals, value, StringUtils::ToString( value ) );
		CLogger::LogInfo( StringStream() << "  Min Value" );
		value = int64_t( std::numeric_limits< int64_t >::min() % int256_t( pow( 10, precision ) ) );
		CheckFixedPoint( precision, decimals, value, StringUtils::ToString( value ) );
		CLogger::LogInfo( StringStream() << "  Lowest Value" );
		value = int64_t( std::numeric_limits< int64_t >::lowest() % int256_t( pow( 10, precision ) ) );
		CheckFixedPoint( precision, decimals, value, StringUtils::ToString( value ) );

		CLogger::LogInfo( StringStream() << "**** End TestCase_FixedPointMaxPrecisionMinDecimals ****" );
	}

	void CDatabaseFixedPointTest::TestCase_FixedPointMaxPrecisionMaxDecimals()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_FixedPointMaxPrecisionMaxDecimals ****" );

		int precision = CFixedPoint::GetMaxPrecision();
		int decimals = precision - 1;
		int64_t value;
		String sval;
		CLogger::LogInfo( StringStream() << "  Max Value" );
		value = int64_t( std::numeric_limits< int64_t >::max() % int256_t( pow( 10, precision ) ) );
		sval = "0." + StringUtils::ToString( value );
		CheckFixedPoint( precision, decimals, value, sval );
		CLogger::LogInfo( StringStream() << "  Min Value" );
		value = int64_t( std::numeric_limits< int64_t >::min() % int256_t( pow( 10, precision ) ) );
		sval = "-0." + StringUtils::ToString( uint64_t( abs( value ) ) );
		CheckFixedPoint( precision, decimals, value, sval );
		CLogger::LogInfo( StringStream() << "  Lowest Value" );
		value = int64_t( std::numeric_limits< int64_t >::lowest() % int256_t( pow( 10, precision ) ) );
		sval = "-0." + StringUtils::ToString( uint64_t( abs( value ) ) );
		CheckFixedPoint( precision, decimals, value, sval );

		CLogger::LogInfo( StringStream() << "**** End TestCase_FixedPointMaxPrecisionMinDecimals ****" );
	}

	void CDatabaseFixedPointTest::TestCase_FixedPointMedPrecisionMedDecimals()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_FixedPointMedPrecisionMedDecimals ****" );

		int precision = CFixedPoint::GetMaxPrecision() / 2;
		int decimals = precision / 2;
		int64_t value;
		String sval;
		CLogger::LogInfo( StringStream() << "  Max Value" );
		value = int64_t( std::numeric_limits< int64_t >::max() % int256_t( pow( 10, precision ) ) );
		sval = StringUtils::ToString( value );
		sval.insert( ( sval.rbegin() + decimals ).base(), STR( '.' ) );
		CheckFixedPoint( precision, decimals, value, sval );
		CLogger::LogInfo( StringStream() << "  Min Value" );
		value = int64_t( std::numeric_limits< int64_t >::min() % int256_t( pow( 10, precision ) ) );
		sval = StringUtils::ToString( value );
		sval.insert( ( sval.rbegin() + decimals ).base(), STR( '.' ) );
		CheckFixedPoint( precision, decimals, value, sval );
		CLogger::LogInfo( StringStream() << "  Lowest Value" );
		value = int64_t( std::numeric_limits< int64_t >::lowest() % int256_t( pow( 10, precision ) ) );
		sval = StringUtils::ToString( value );
		sval.insert( ( sval.rbegin() + decimals ).base(), STR( '.' ) );
		CheckFixedPoint( precision, decimals, value, sval );
		CLogger::LogInfo( StringStream() << "  Invalid Value (greater precision than wanted)" );
		BOOST_CHECK_THROW( CFixedPoint( std::numeric_limits< int64_t >::max(), precision, decimals ), CDatabaseException );

		CLogger::LogInfo( StringStream() << "**** End TestCase_FixedPointMaxPrecisionMinDecimals ****" );
	}

	void CDatabaseFixedPointTest::TestCase_FixedPointConstruction()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_FixedPointConstruction ****" );

		int precision = CFixedPoint::GetMaxPrecision() / 2;
		int decimals = precision / 2;
		CLogger::LogInfo( StringStream() << "  From double" );
		{
			CLogger::LogInfo( StringStream() << "    Max Value (Precision overflow)" );
			BOOST_CHECK_THROW( CFixedPoint( std::numeric_limits< double >::max(), precision, decimals ), CDatabaseException );
			CLogger::LogInfo( StringStream() << "    Min Value" );
			BOOST_CHECK_EQUAL( CFixedPoint( std::numeric_limits< double >::min(), precision, decimals ).ToDouble(), 0.0 );
			CLogger::LogInfo( StringStream() << "    Lowest Value (Precision overflow)" );
			BOOST_CHECK_THROW( CFixedPoint( std::numeric_limits< double >::lowest(), precision, decimals ), CDatabaseException );
			CLogger::LogInfo( StringStream() << "    Random valid Value" );
			{
				double value = DatabaseUtils::Helpers< EFieldType_FLOAT64 >::InitialiseValue();
				CFixedPoint fp( value, precision, decimals );
				BOOST_CHECK_LT( value - fp.ToDouble(), pow( 10, -decimals ) );
			}
		}
		CLogger::LogInfo( StringStream() << "  From float" );
		{
			CLogger::LogInfo( StringStream() << "    Max Value (Precision overflow)" );
			BOOST_CHECK_THROW( CFixedPoint( std::numeric_limits< float >::max(), precision, decimals ), CDatabaseException );
			CLogger::LogInfo( StringStream() << "    Min Value" );
			BOOST_CHECK_EQUAL( CFixedPoint( std::numeric_limits< float >::min(), precision, decimals ).ToDouble(), 0.0 );
			CLogger::LogInfo( StringStream() << "    Lowest Value (Precision overflow)" );
			BOOST_CHECK_THROW( CFixedPoint( std::numeric_limits< float >::lowest(), precision, decimals ), CDatabaseException );
			CLogger::LogInfo( StringStream() << "    Random valid Value" );
			{
				float value = DatabaseUtils::Helpers< EFieldType_FLOAT32 >::InitialiseValue();
				CFixedPoint fp( value, precision, decimals );
				BOOST_CHECK_LT( value - fp.ToFloat(), pow( 10, -decimals ) );
			}
		}
		CLogger::LogInfo( StringStream() << "  From int32_t" );
		{
			CLogger::LogInfo( StringStream() << "    Max Value" );
			{
				int32_t value = std::numeric_limits< int32_t >::max();
				BOOST_CHECK_EQUAL( CFixedPoint( value, precision, decimals ).ToInt32(), int32_t( value / pow( 10, decimals ) ) );
			}
			CLogger::LogInfo( StringStream() << "    Min Value" );
			{
				int32_t value = std::numeric_limits< int32_t >::min();
				BOOST_CHECK_EQUAL( CFixedPoint( value, precision, decimals ).ToInt32(), int32_t( value / pow( 10, decimals ) ) );
			}
			CLogger::LogInfo( StringStream() << "    Lowest Value" );
			{
				int32_t value = std::numeric_limits< int32_t >::lowest();
				BOOST_CHECK_EQUAL( CFixedPoint( value, precision, decimals ).ToInt32(), int32_t( value / pow( 10, decimals ) ) );
			}
		}
		CLogger::LogInfo( StringStream() << "  From uint32_t" );
		{
			CLogger::LogInfo( StringStream() << "    Max Value" );
			{
				uint32_t value = std::numeric_limits< uint32_t >::max();
				BOOST_CHECK_EQUAL( CFixedPoint( value, precision, decimals ).ToInt32(), uint32_t( value / pow( 10, decimals ) ) );
			}
			CLogger::LogInfo( StringStream() << "    Min Value" );
			{
				uint32_t value = std::numeric_limits< uint32_t >::min();
				BOOST_CHECK_EQUAL( CFixedPoint( value, precision, decimals ).ToInt32(), uint32_t( value / pow( 10, decimals ) ) );
			}
			CLogger::LogInfo( StringStream() << "    Lowest Value" );
			{
				uint32_t value = std::numeric_limits< uint32_t >::lowest();
				BOOST_CHECK_EQUAL( CFixedPoint( value, precision, decimals ).ToInt32(), uint32_t( value / pow( 10, decimals ) ) );
			}
		}
		CLogger::LogInfo( StringStream() << "  From uint64_t" );
		{
			CLogger::LogInfo( StringStream() << "    Max Value (Precision overflow)" );
			BOOST_CHECK_THROW( CFixedPoint( std::numeric_limits< uint64_t >::max(), precision, decimals ), CDatabaseException );
			CLogger::LogInfo( StringStream() << "    Min Value" );
			BOOST_CHECK_EQUAL( CFixedPoint( std::numeric_limits< uint64_t >::min(), precision, decimals ).ToUInt64(), 0 );
			CLogger::LogInfo( StringStream() << "    Lowest Value" );
			BOOST_CHECK_EQUAL( CFixedPoint( std::numeric_limits< uint64_t >::lowest(), precision, decimals ).ToUInt64(), 0 );
			CLogger::LogInfo( StringStream() << "    Random valid Value" );
			uint64_t value = DatabaseUtils::Helpers< EFieldType_UINT64 >::InitialiseValue() % uint64_t( pow( 10, precision ) );
			BOOST_CHECK_EQUAL( CFixedPoint( value, precision, decimals ).ToUInt64(), uint64_t( value / pow( 10, decimals ) ) );
		}
		CLogger::LogInfo( StringStream() << "  From String" );
		{
			CLogger::LogInfo( StringStream() << "    Precision overflow in integer value" );
			BOOST_CHECK_THROW( CFixedPoint( STR( "12345678901" ), precision, decimals ), CDatabaseException );
			CLogger::LogInfo( StringStream() << "    Precision overflow in floating point value" );
			BOOST_CHECK_THROW( CFixedPoint( STR( "12345678901.23" ), precision, decimals ), CDatabaseException );
			CLogger::LogInfo( StringStream() << "    Precision overflow, hidden by decimals" );
			BOOST_CHECK_THROW( CFixedPoint( STR( "1234567890" ), precision, decimals ), CDatabaseException );
			CLogger::LogInfo( StringStream() << "    Negative integer Value" );
			BOOST_CHECK_EQUAL( CFixedPoint( STR( "-12345" ), precision, decimals ).ToInt64(), -12345 );
			CLogger::LogInfo( StringStream() << "    Positive integer Value" );
			BOOST_CHECK_EQUAL( CFixedPoint( STR( "12345" ), precision, decimals ).ToInt64(), 12345 );
			CLogger::LogInfo( StringStream() << "    Negative floating point Value" );
			BOOST_CHECK_EQUAL( CFixedPoint( STR( "-12345.6789" ), precision, decimals ).ToDouble(), -12345.6789 );
			CLogger::LogInfo( StringStream() << "    Positive floating point Value, not enough decimals" );
			BOOST_CHECK_EQUAL( CFixedPoint( STR( "12345.6789" ), precision, decimals ).ToDouble(), 12345.6789 );
			CLogger::LogInfo( StringStream() << "    Positive floating point Value, good decimals count" );
			BOOST_CHECK_EQUAL( CFixedPoint( STR( "1234.56789" ), precision, decimals ).ToDouble(), 1234.56789 );
			CLogger::LogInfo( StringStream() << "    Positive floating point Value, more decimals than needed" );
			BOOST_CHECK_EQUAL( CFixedPoint( STR( "123.456789" ), precision, decimals ).ToDouble(), 123.45678 );
		}

		CLogger::LogInfo( StringStream() << "**** End TestCase_FixedPointConstruction ****" );
	}

	void CDatabaseFixedPointTest::TestCase_FixedPointAddition()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_FixedPointAddition ****" );

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

		CLogger::LogInfo( StringStream() << "**** End TestCase_FixedPointAddition ****" );
	}

	void CDatabaseFixedPointTest::TestCase_FixedPointSubtraction()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_FixedPointSubtraction ****" );

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

		CLogger::LogInfo( StringStream() << "**** End TestCase_FixedPointSubtraction ****" );
	}

	void CDatabaseFixedPointTest::TestCase_FixedPointMultiplication()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_FixedPointMultiplication ****" );

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

		CLogger::LogInfo( StringStream() << "**** End TestCase_FixedPointMultiplication ****" );
	}

	void CDatabaseFixedPointTest::TestCase_FixedPointDivision()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_FixedPointDivision ****" );

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

		CLogger::LogInfo( StringStream() << "**** End TestCase_FixedPointDivision ****" );
	}
}
END_NAMESPACE_DATABASE_TEST
