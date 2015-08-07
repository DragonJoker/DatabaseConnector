/************************************************************************//**
* @file DatabaseValueTest.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 12/02/2014 14:29:35
*
*
* @brief Class testing CDatabaseValue class
*
***************************************************************************/

#include "DatabaseTestPch.h"

#if defined( TESTING_INTERFACE )

#include "DatabaseValueTest.h"

#include "DatabaseTestHelpers.h"
#include "DatabaseTestConnection.h"

namespace std
{
	inline ostream & operator <<( ostream & out, const NAMESPACE_DATABASE::ByteArray & value )
	{
		auto flags = out.setf( std::ios::hex, std::ios::basefield );

		for ( auto && it : value )
		{
			out.width( 2 );
			out.fill( '0' );
			out << int( it );
		}

		out.setf( flags );
		return out;
	}
}

BEGIN_NAMESPACE_DATABASE_TEST
{
	template< EFieldType ValueType > struct ValueChecker
	{
		typedef typename DatabaseUtils::Helpers< ValueType >::ParamType ParamType;

		static void QueryValueCompare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const ParamType & val )
		{
			BOOST_CHECK_EQUAL( value.GetQueryValue(), StringUtils::ToString( val ) );
		}
		static void PtrValueCompare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const ParamType & val )
		{
			BOOST_CHECK_EQUAL( *reinterpret_cast< const ParamType * >( value.GetPtrValue() ), value.GetValue() );
			BOOST_CHECK_EQUAL( *reinterpret_cast< const ParamType * >( value.GetPtrValue() ), val );
			BOOST_CHECK_EQUAL( value.GetPtrSize(), sizeof( val ) );
		}
	};

	template<> struct ValueChecker< EFieldType_BIT >
	{
		static const EFieldType ValueType = EFieldType_BIT;
		typedef DatabaseUtils::Helpers< ValueType >::ParamType ParamType;

		static void QueryValueCompare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const ParamType & val )
		{
			BOOST_CHECK_EQUAL( value.GetQueryValue(), connection->WriteBool( val ) );
		}
		static void PtrValueCompare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const ParamType & val )
		{
			BOOST_CHECK_EQUAL( *reinterpret_cast< const ParamType * >( value.GetPtrValue() ), value.GetValue() );
			BOOST_CHECK_EQUAL( *reinterpret_cast< const ParamType * >( value.GetPtrValue() ), val );
			BOOST_CHECK_EQUAL( value.GetPtrSize(), sizeof( val ) );
		}
	};

	template<> struct ValueChecker< EFieldType_SINT8 >
	{
		static const EFieldType ValueType = EFieldType_SINT8;
		typedef DatabaseUtils::Helpers< ValueType >::ParamType ParamType;

		static void QueryValueCompare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const ParamType & val )
		{
			BOOST_CHECK_EQUAL( value.GetQueryValue(), StringUtils::ToString( int16_t( val ) ) );
		}
		static void PtrValueCompare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const ParamType & val )
		{
			BOOST_CHECK_EQUAL( *reinterpret_cast< const ParamType * >( value.GetPtrValue() ), value.GetValue() );
			BOOST_CHECK_EQUAL( *reinterpret_cast< const ParamType * >( value.GetPtrValue() ), val );
			BOOST_CHECK_EQUAL( value.GetPtrSize(), sizeof( val ) );
		}
	};

	template<> struct ValueChecker< EFieldType_UINT8 >
	{
		static const EFieldType ValueType = EFieldType_UINT8;
		typedef DatabaseUtils::Helpers< ValueType >::ParamType ParamType;

		static void QueryValueCompare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const ParamType & val )
		{
			BOOST_CHECK_EQUAL( value.GetQueryValue(), StringUtils::ToString( uint16_t( val ) ) );
		}
		static void PtrValueCompare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const ParamType & val )
		{
			BOOST_CHECK_EQUAL( *reinterpret_cast< const ParamType * >( value.GetPtrValue() ), value.GetValue() );
			BOOST_CHECK_EQUAL( *reinterpret_cast< const ParamType * >( value.GetPtrValue() ), val );
			BOOST_CHECK_EQUAL( value.GetPtrSize(), sizeof( val ) );
		}
	};

	template<> struct ValueChecker< EFieldType_FLOAT64 >
	{
		static const EFieldType ValueType = EFieldType_FLOAT64;
		typedef DatabaseUtils::Helpers< ValueType >::ParamType ParamType;

		static void QueryValueCompare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const ParamType & val )
		{
			BOOST_CHECK_EQUAL( value.GetQueryValue(), connection->WriteDouble( val ) );
		}
		static void PtrValueCompare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const ParamType & val )
		{
			BOOST_CHECK_EQUAL( *reinterpret_cast< const ParamType * >( value.GetPtrValue() ), value.GetValue() );
			BOOST_CHECK_EQUAL( *reinterpret_cast< const ParamType * >( value.GetPtrValue() ), val );
			BOOST_CHECK_EQUAL( value.GetPtrSize(), sizeof( val ) );
		}
	};

	template<> struct ValueChecker< EFieldType_FIXED_POINT >
	{
		static const EFieldType ValueType = EFieldType_FIXED_POINT;
		typedef DatabaseUtils::Helpers< ValueType >::ParamType ParamType;

		static void QueryValueCompare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const ParamType & val )
		{
			BOOST_CHECK_EQUAL( value.GetQueryValue(), val.ToString() );
		}
		static void PtrValueCompare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const ParamType & val )
		{
			BOOST_CHECK_EQUAL( *reinterpret_cast< const ParamType * >( value.GetPtrValue() ), value.GetValue() );
			BOOST_CHECK_EQUAL( *reinterpret_cast< const ParamType * >( value.GetPtrValue() ), val );
			BOOST_CHECK_EQUAL( value.GetPtrSize(), sizeof( val ) );
		}
	};

	template<> struct ValueChecker< EFieldType_CHAR >
	{
		static const EFieldType ValueType = EFieldType_CHAR;
		typedef DatabaseUtils::Helpers< ValueType >::ParamType ParamType;

		static void QueryValueCompare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const ParamType & val )
		{
			BOOST_CHECK_EQUAL( value.GetQueryValue(), connection->WriteText( val ) );
		}
		static void PtrValueCompare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const ParamType & val )
		{
			ParamType ptr( reinterpret_cast< const char * >( value.GetPtrValue() ) );
			BOOST_CHECK_EQUAL( ptr, value.GetValue() );
			BOOST_CHECK_EQUAL( ptr, val );
			BOOST_CHECK_EQUAL( value.GetPtrSize(), val.size() );
		}
	};

	template<> struct ValueChecker< EFieldType_VARCHAR >
	{
		static const EFieldType ValueType = EFieldType_VARCHAR;
		typedef DatabaseUtils::Helpers< ValueType >::ParamType ParamType;

		static void QueryValueCompare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const ParamType & val )
		{
			BOOST_CHECK_EQUAL( value.GetQueryValue(), connection->WriteText( val ) );
		}
		static void PtrValueCompare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const ParamType & val )
		{
			ParamType ptr( reinterpret_cast< const char * >( value.GetPtrValue() ) );
			BOOST_CHECK_EQUAL( ptr, value.GetValue() );
			BOOST_CHECK_EQUAL( ptr, val );
			BOOST_CHECK_EQUAL( value.GetPtrSize(), val.size() );
		}
	};

	template<> struct ValueChecker< EFieldType_TEXT >
	{
		static const EFieldType ValueType = EFieldType_TEXT;
		typedef DatabaseUtils::Helpers< ValueType >::ParamType ParamType;

		static void QueryValueCompare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const ParamType & val )
		{
			BOOST_CHECK_EQUAL( value.GetQueryValue(), connection->WriteText( val ) );
		}
		static void PtrValueCompare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const ParamType & val )
		{
			ParamType ptr( reinterpret_cast< const char * >( value.GetPtrValue() ) );
			BOOST_CHECK_EQUAL( ptr, value.GetValue() );
			BOOST_CHECK_EQUAL( ptr, val );
			BOOST_CHECK_EQUAL( value.GetPtrSize(), val.size() );
		}
	};

	template<> struct ValueChecker< EFieldType_NCHAR >
	{
		static const EFieldType ValueType = EFieldType_NCHAR;
		typedef DatabaseUtils::Helpers< ValueType >::ParamType ParamType;

		static void QueryValueCompare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const ParamType & val )
		{
			BOOST_CHECK_EQUAL( value.GetQueryValue(), StringUtils::ToStr( connection->WriteNText( val ) ) );
		}
		static void PtrValueCompare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const ParamType & val )
		{
			ParamType ptr( reinterpret_cast< const wchar_t * >( value.GetPtrValue() ) );
			BOOST_CHECK_EQUAL( ptr, value.GetValue() );
			BOOST_CHECK_EQUAL( ptr, val );
			BOOST_CHECK_EQUAL( ptr.size(), val.size() );
		}
	};

	template<> struct ValueChecker< EFieldType_NVARCHAR >
	{
		static const EFieldType ValueType = EFieldType_NVARCHAR;
		typedef DatabaseUtils::Helpers< ValueType >::ParamType ParamType;

		static void QueryValueCompare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const ParamType & val )
		{
			BOOST_CHECK_EQUAL( value.GetQueryValue(), StringUtils::ToStr( connection->WriteNText( val ) ) );
		}
		static void PtrValueCompare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const ParamType & val )
		{
			ParamType ptr( reinterpret_cast< const wchar_t * >( value.GetPtrValue() ) );
			BOOST_CHECK_EQUAL( ptr, value.GetValue() );
			BOOST_CHECK_EQUAL( ptr, val );
			BOOST_CHECK_EQUAL( ptr.size(), val.size() );
		}
	};

	template<> struct ValueChecker< EFieldType_NTEXT >
	{
		static const EFieldType ValueType = EFieldType_NTEXT;
		typedef DatabaseUtils::Helpers< ValueType >::ParamType ParamType;

		static void QueryValueCompare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const ParamType & val )
		{
			BOOST_CHECK_EQUAL( value.GetQueryValue(), StringUtils::ToStr( connection->WriteNText( val ) ) );
		}
		static void PtrValueCompare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const ParamType & val )
		{
			ParamType ptr( reinterpret_cast< const wchar_t * >( value.GetPtrValue() ) );
			BOOST_CHECK_EQUAL( ptr, value.GetValue() );
			BOOST_CHECK_EQUAL( ptr, val );
			BOOST_CHECK_EQUAL( ptr.size(), val.size() );
		}
	};

	template<> struct ValueChecker< EFieldType_DATE >
	{
		static const EFieldType ValueType = EFieldType_DATE;
		typedef DatabaseUtils::Helpers< ValueType >::ParamType ParamType;

		static void QueryValueCompare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const ParamType & val )
		{
			BOOST_CHECK_EQUAL( value.GetQueryValue(), connection->WriteDate( val ) );
		}
		static void PtrValueCompare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const ParamType & val )
		{
			BOOST_CHECK_EQUAL( *reinterpret_cast< const ParamType * >( value.GetPtrValue() ), value.GetValue() );
			BOOST_CHECK_EQUAL( *reinterpret_cast< const ParamType * >( value.GetPtrValue() ), val );
			BOOST_CHECK_EQUAL( value.GetPtrSize(), sizeof( val ) );
		}
	};

	template<> struct ValueChecker< EFieldType_TIME >
	{
		static const EFieldType ValueType = EFieldType_TIME;
		typedef DatabaseUtils::Helpers< ValueType >::ParamType ParamType;

		static void QueryValueCompare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const ParamType & val )
		{
			BOOST_CHECK_EQUAL( value.GetQueryValue(), connection->WriteTime( val ) );
		}
		static void PtrValueCompare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const ParamType & val )
		{
			BOOST_CHECK_EQUAL( *reinterpret_cast< const ParamType * >( value.GetPtrValue() ), value.GetValue() );
			BOOST_CHECK_EQUAL( *reinterpret_cast< const ParamType * >( value.GetPtrValue() ), val );
			BOOST_CHECK_EQUAL( value.GetPtrSize(), sizeof( val ) );
		}
	};

	template<> struct ValueChecker< EFieldType_DATETIME >
	{
		static const EFieldType ValueType = EFieldType_DATETIME;
		typedef DatabaseUtils::Helpers< ValueType >::ParamType ParamType;

		static void QueryValueCompare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const ParamType & val )
		{
			BOOST_CHECK_EQUAL( value.GetQueryValue(), connection->WriteDateTime( val ) );
		}
		static void PtrValueCompare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const ParamType & val )
		{
			BOOST_CHECK_EQUAL( *reinterpret_cast< const ParamType * >( value.GetPtrValue() ), value.GetValue() );
			BOOST_CHECK_EQUAL( *reinterpret_cast< const ParamType * >( value.GetPtrValue() ), val );
			BOOST_CHECK_EQUAL( value.GetPtrSize(), sizeof( val ) );
		}
	};

	template<> struct ValueChecker< EFieldType_BINARY >
	{
		static const EFieldType ValueType = EFieldType_BINARY;
		typedef DatabaseUtils::Helpers< ValueType >::ParamType ParamType;

		static void QueryValueCompare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const ParamType & val )
		{
			BOOST_CHECK_EQUAL( value.GetQueryValue(), connection->WriteBinary( val ) );
		}
		static void PtrValueCompare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const ParamType & val )
		{
			const uint8_t * raw = reinterpret_cast< const uint8_t * >( value.GetPtrValue() );
			ParamType ptr( raw, raw + value.GetPtrSize() );
			BOOST_CHECK_EQUAL( ptr, value.GetValue() );
			BOOST_CHECK_EQUAL( ptr, val );
			BOOST_CHECK_EQUAL( value.GetPtrSize(), val.size() );
		}
	};

	template<> struct ValueChecker< EFieldType_VARBINARY >
	{
		static const EFieldType ValueType = EFieldType_VARBINARY;
		typedef DatabaseUtils::Helpers< ValueType >::ParamType ParamType;

		static void QueryValueCompare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const ParamType & val )
		{
			BOOST_CHECK_EQUAL( value.GetQueryValue(), connection->WriteBinary( val ) );
		}
		static void PtrValueCompare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const ParamType & val )
		{
			const uint8_t * raw = reinterpret_cast< const uint8_t * >( value.GetPtrValue() );
			ParamType ptr( raw, raw + value.GetPtrSize() );
			BOOST_CHECK_EQUAL( ptr, value.GetValue() );
			BOOST_CHECK_EQUAL( ptr, val );
			BOOST_CHECK_EQUAL( value.GetPtrSize(), val.size() );
		}
	};

	template<> struct ValueChecker< EFieldType_BLOB >
	{
		static const EFieldType ValueType = EFieldType_BLOB;
		typedef DatabaseUtils::Helpers< ValueType >::ParamType ParamType;

		static void QueryValueCompare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const ParamType & val )
		{
			BOOST_CHECK_EQUAL( value.GetQueryValue(), connection->WriteBinary( val ) );
		}
		static void PtrValueCompare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const ParamType & val )
		{
			const uint8_t * raw = reinterpret_cast< const uint8_t * >( value.GetPtrValue() );
			ParamType ptr( raw, raw + value.GetPtrSize() );
			BOOST_CHECK_EQUAL( ptr, value.GetValue() );
			BOOST_CHECK_EQUAL( ptr, val );
			BOOST_CHECK_EQUAL( value.GetPtrSize(), val.size() );
		}
	};

	template< EFieldType ValueType > struct ValueChecks
	{
		static void Copy( std::random_device & generator )
		{
			String connectionString;
			DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( TEST_GOOD_SERVER, TEST_GOOD_USER, TEST_GOOD_PASSWORD, connectionString );
			connection->SelectDatabase( TEST_GOOD_DATABASE );
			CDatabaseValue< ValueType > valueA( connection );
			BOOST_CHECK_NO_THROW( valueA.SetValue( DatabaseUtils::Helpers< ValueType >::GetRandomValue( generator ) ) );
			CDatabaseValue< ValueType > valueB( connection );
			BOOST_CHECK_NO_THROW( valueB.SetValue( DatabaseUtils::Helpers< ValueType >::GetRandomValue( generator ) ) );
			BOOST_CHECK_NO_THROW( valueA.SetValue( valueB ) );
			BOOST_CHECK_EQUAL( valueA.GetValue(), valueB.GetValue() );
		}

		static void QueryValue( std::random_device & generator )
		{
			String connectionString;
			DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( TEST_GOOD_SERVER, TEST_GOOD_USER, TEST_GOOD_PASSWORD, connectionString );
			connection->SelectDatabase( TEST_GOOD_DATABASE );
			CDatabaseValue< ValueType > value( connection );
			auto val = DatabaseUtils::Helpers< ValueType >::GetRandomValue( generator );
			BOOST_CHECK_NO_THROW( value.SetValue( val ) );
			ValueChecker< ValueType >::QueryValueCompare( connection, value, val );
		}

		static void PtrValue( std::random_device & generator )
		{
			String connectionString;
			DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( TEST_GOOD_SERVER, TEST_GOOD_USER, TEST_GOOD_PASSWORD, connectionString );
			connection->SelectDatabase( TEST_GOOD_DATABASE );
			CDatabaseValue< ValueType > value( connection );
			BOOST_CHECK( value.IsNull() );
			BOOST_CHECK_EQUAL( value.GetPtrSize(), 0 );
			auto val = DatabaseUtils::Helpers< ValueType >::GetRandomValue( generator );
			BOOST_CHECK_NO_THROW( value.SetValue( val ) );
			BOOST_CHECK( !value.IsNull() );
			BOOST_CHECK( value.GetPtrValue() );
			ValueChecker< ValueType >::PtrValueCompare( connection, value, val );
		}
	};

	CDatabaseValueTest::CDatabaseValueTest()
	{
	}

	CDatabaseValueTest::~CDatabaseValueTest()
	{
	}

	boost::unit_test::test_suite * CDatabaseValueTest::Init_Test_Suite()
	{
		//!@remarks Create the internal TS instance.
		testSuite = new boost::unit_test::test_suite( "CDatabaseValueTest" );

		//!@remarks Add the TC to the internal TS.
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseValueTest::TestCase_ValueCopy, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseValueTest::TestCase_ValueForQuery, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseValueTest::TestCase_ValueGetPtr, this ) ) );

		//!@remarks Return the TS instance.
		return testSuite;
	}

	void CDatabaseValueTest::TestCase_ValueCopy()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_ValueCopy ****" );
		std::random_device generator;

		CLogger::LogInfo( StringStream() << "  EFieldType_BIT" );
		ValueChecks< EFieldType_BIT >::Copy( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT8" );
		ValueChecks< EFieldType_SINT8 >::Copy( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT16" );
		ValueChecks< EFieldType_SINT16 >::Copy( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT24" );
		ValueChecks< EFieldType_SINT24 >::Copy( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT32" );
		ValueChecks< EFieldType_SINT32 >::Copy( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT64" );
		ValueChecks< EFieldType_SINT64 >::Copy( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT8" );
		ValueChecks< EFieldType_UINT8 >::Copy( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT16" );
		ValueChecks< EFieldType_UINT16 >::Copy( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT24" );
		ValueChecks< EFieldType_UINT24 >::Copy( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT32" );
		ValueChecks< EFieldType_UINT32 >::Copy( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT64" );
		ValueChecks< EFieldType_UINT64 >::Copy( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_FLOAT32" );
		ValueChecks< EFieldType_FLOAT32 >::Copy( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_FLOAT64" );
		ValueChecks< EFieldType_FLOAT64 >::Copy( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_FIXED_POINT" );
		ValueChecks< EFieldType_FIXED_POINT >::Copy( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_CHAR" );
		ValueChecks< EFieldType_CHAR >::Copy( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_VARCHAR" );
		ValueChecks< EFieldType_VARCHAR >::Copy( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_TEXT" );
		ValueChecks< EFieldType_TEXT >::Copy( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_NCHAR" );
		ValueChecks< EFieldType_NCHAR >::Copy( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_NVARCHAR" );
		ValueChecks< EFieldType_NVARCHAR >::Copy( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_NTEXT" );
		ValueChecks< EFieldType_NTEXT >::Copy( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_DATE" );
		ValueChecks< EFieldType_DATE >::Copy( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_DATETIME" );
		ValueChecks< EFieldType_DATETIME >::Copy( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_TIME" );
		ValueChecks< EFieldType_TIME >::Copy( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_BINARY" );
		ValueChecks< EFieldType_BINARY >::Copy( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_VARBINARY" );
		ValueChecks< EFieldType_VARBINARY >::Copy( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_BLOB" );
		ValueChecks< EFieldType_BLOB >::Copy( generator );

		CLogger::LogInfo( StringStream() << "**** End TestCase_ValueCopy ****" );
	}

	void CDatabaseValueTest::TestCase_ValueForQuery()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_ValueForQuery ****" );
		std::random_device generator;

		CLogger::LogInfo( StringStream() << "  EFieldType_BIT" );
		ValueChecks< EFieldType_BIT >::QueryValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT8" );
		ValueChecks< EFieldType_SINT8 >::QueryValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT16" );
		ValueChecks< EFieldType_SINT16 >::QueryValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT24" );
		ValueChecks< EFieldType_SINT24 >::QueryValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT32" );
		ValueChecks< EFieldType_SINT32 >::QueryValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT64" );
		ValueChecks< EFieldType_SINT64 >::QueryValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT8" );
		ValueChecks< EFieldType_UINT8 >::QueryValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT16" );
		ValueChecks< EFieldType_UINT16 >::QueryValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT24" );
		ValueChecks< EFieldType_UINT24 >::QueryValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT32" );
		ValueChecks< EFieldType_UINT32 >::QueryValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT64" );
		ValueChecks< EFieldType_UINT64 >::QueryValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_FLOAT32" );
		ValueChecks< EFieldType_FLOAT32 >::QueryValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_FLOAT64" );
		ValueChecks< EFieldType_FLOAT64 >::QueryValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_FIXED_POINT" );
		ValueChecks< EFieldType_FIXED_POINT >::QueryValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_CHAR" );
		ValueChecks< EFieldType_CHAR >::QueryValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_VARCHAR" );
		ValueChecks< EFieldType_VARCHAR >::QueryValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_TEXT" );
		ValueChecks< EFieldType_TEXT >::QueryValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_NCHAR" );
		ValueChecks< EFieldType_NCHAR >::QueryValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_NVARCHAR" );
		ValueChecks< EFieldType_NVARCHAR >::QueryValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_NTEXT" );
		ValueChecks< EFieldType_NTEXT >::QueryValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_DATE" );
		ValueChecks< EFieldType_DATE >::QueryValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_DATETIME" );
		ValueChecks< EFieldType_DATETIME >::QueryValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_TIME" );
		ValueChecks< EFieldType_TIME >::QueryValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_BINARY" );
		ValueChecks< EFieldType_BINARY >::QueryValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_VARBINARY" );
		ValueChecks< EFieldType_VARBINARY >::QueryValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_BLOB" );
		ValueChecks< EFieldType_BLOB >::QueryValue( generator );

		CLogger::LogInfo( StringStream() << "**** End TestCase_ValueForQuery ****" );
	}

	void CDatabaseValueTest::TestCase_ValueGetPtr()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_ValueGetPtr ****" );
		std::random_device generator;

		CLogger::LogInfo( StringStream() << "  EFieldType_BIT" );
		ValueChecks< EFieldType_BIT >::PtrValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT8" );
		ValueChecks< EFieldType_SINT8 >::PtrValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT16" );
		ValueChecks< EFieldType_SINT16 >::PtrValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT24" );
		ValueChecks< EFieldType_SINT24 >::PtrValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT32" );
		ValueChecks< EFieldType_SINT32 >::PtrValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT64" );
		ValueChecks< EFieldType_SINT64 >::PtrValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT8" );
		ValueChecks< EFieldType_UINT8 >::PtrValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT16" );
		ValueChecks< EFieldType_UINT16 >::PtrValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT24" );
		ValueChecks< EFieldType_UINT24 >::PtrValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT32" );
		ValueChecks< EFieldType_UINT32 >::PtrValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT64" );
		ValueChecks< EFieldType_UINT64 >::PtrValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_FLOAT32" );
		ValueChecks< EFieldType_FLOAT32 >::PtrValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_FLOAT64" );
		ValueChecks< EFieldType_FLOAT64 >::PtrValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_FIXED_POINT" );
		ValueChecks< EFieldType_FIXED_POINT >::PtrValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_CHAR" );
		ValueChecks< EFieldType_CHAR >::PtrValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_VARCHAR" );
		ValueChecks< EFieldType_VARCHAR >::PtrValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_TEXT" );
		ValueChecks< EFieldType_TEXT >::PtrValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_NCHAR" );
		ValueChecks< EFieldType_NCHAR >::PtrValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_NVARCHAR" );
		ValueChecks< EFieldType_NVARCHAR >::PtrValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_NTEXT" );
		ValueChecks< EFieldType_NTEXT >::PtrValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_DATE" );
		ValueChecks< EFieldType_DATE >::PtrValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_DATETIME" );
		ValueChecks< EFieldType_DATETIME >::PtrValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_TIME" );
		ValueChecks< EFieldType_TIME >::PtrValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_BINARY" );
		ValueChecks< EFieldType_BINARY >::PtrValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_VARBINARY" );
		ValueChecks< EFieldType_VARBINARY >::PtrValue( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_BLOB" );
		ValueChecks< EFieldType_BLOB >::PtrValue( generator );

		CLogger::LogInfo( StringStream() << "**** End TestCase_ValueGetPtr ****" );
	}
}
END_NAMESPACE_DATABASE_TEST

#endif
