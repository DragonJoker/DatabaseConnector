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

#include "DatabaseValueTest.h"

#include "DatabaseTestUtils.h"
#include "DatabaseTestConnection.h"

namespace std
{
	inline ostream& operator <<( ostream & out, const wstring & value )
	{
		 out << NAMESPACE_DATABASE::StringUtils::ToStr( value );
		 return out;
	}

	inline ostream& operator <<( ostream & out, const vector< uint8_t > & value )
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
	template< EFieldType ValueType > struct QueryValueChecker
	{
		static void Compare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const typename DatabaseUtils::Helpers< ValueType >::ParamType & val )
		{
			BOOST_CHECK_EQUAL( value.GetQueryValue(), StringUtils::ToString( val ) );
		};
	};

	template<> struct QueryValueChecker< EFieldType_BIT >
	{
		static const EFieldType ValueType = EFieldType_BIT;

		static void Compare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const DatabaseUtils::Helpers< ValueType >::ParamType & val )
		{
			BOOST_CHECK_EQUAL( value.GetQueryValue(), connection->WriteBool( val ) );
		};
	};

	template<> struct QueryValueChecker< EFieldType_SINT8 >
	{
		static const EFieldType ValueType = EFieldType_SINT8;

		static void Compare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const DatabaseUtils::Helpers< ValueType >::ParamType & val )
		{
			BOOST_CHECK_EQUAL( value.GetQueryValue(), StringUtils::ToString( int16_t( val ) ) );
		};
	};

	template<> struct QueryValueChecker< EFieldType_UINT8 >
	{
		static const EFieldType ValueType = EFieldType_UINT8;

		static void Compare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const DatabaseUtils::Helpers< ValueType >::ParamType & val )
		{
			BOOST_CHECK_EQUAL( value.GetQueryValue(), StringUtils::ToString( uint16_t( val ) ) );
		};
	};

	template<> struct QueryValueChecker< EFieldType_FLOAT64 >
	{
		static const EFieldType ValueType = EFieldType_FLOAT64;

		static void Compare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const DatabaseUtils::Helpers< ValueType >::ParamType & val )
		{
			BOOST_CHECK_EQUAL( value.GetQueryValue(), connection->WriteDouble( val ) );
		};
	};

	template<> struct QueryValueChecker< EFieldType_FIXED_POINT >
	{
		static const EFieldType ValueType = EFieldType_FIXED_POINT;

		static void Compare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const DatabaseUtils::Helpers< ValueType >::ParamType & val )
		{
			BOOST_CHECK_EQUAL( value.GetQueryValue(), val.ToString() );
		};
	};

	template<> struct QueryValueChecker< EFieldType_CHAR >
	{
		static const EFieldType ValueType = EFieldType_CHAR;

		static void Compare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const DatabaseUtils::Helpers< ValueType >::ParamType & val )
		{
			BOOST_CHECK_EQUAL( value.GetQueryValue(), connection->WriteText( val ) );
		};
	};

	template<> struct QueryValueChecker< EFieldType_VARCHAR >
	{
		static const EFieldType ValueType = EFieldType_VARCHAR;

		static void Compare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const DatabaseUtils::Helpers< ValueType >::ParamType & val )
		{
			BOOST_CHECK_EQUAL( value.GetQueryValue(), connection->WriteText( val ) );
		};
	};

	template<> struct QueryValueChecker< EFieldType_TEXT >
	{
		static const EFieldType ValueType = EFieldType_TEXT;

		static void Compare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const DatabaseUtils::Helpers< ValueType >::ParamType & val )
		{
			BOOST_CHECK_EQUAL( value.GetQueryValue(), connection->WriteText( val ) );
		};
	};

	template<> struct QueryValueChecker< EFieldType_NCHAR >
	{
		static const EFieldType ValueType = EFieldType_NCHAR;

		static void Compare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const DatabaseUtils::Helpers< ValueType >::ParamType & val )
		{
			BOOST_CHECK_EQUAL( value.GetQueryValue(), StringUtils::ToString( connection->WriteNText( val ) ) );
		};
	};

	template<> struct QueryValueChecker< EFieldType_NVARCHAR >
	{
		static const EFieldType ValueType = EFieldType_NVARCHAR;

		static void Compare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const DatabaseUtils::Helpers< ValueType >::ParamType & val )
		{
			BOOST_CHECK_EQUAL( value.GetQueryValue(), StringUtils::ToString( connection->WriteNText( val ) ) );
		};
	};

	template<> struct QueryValueChecker< EFieldType_NTEXT >
	{
		static const EFieldType ValueType = EFieldType_NTEXT;

		static void Compare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const DatabaseUtils::Helpers< ValueType >::ParamType & val )
		{
			BOOST_CHECK_EQUAL( value.GetQueryValue(), StringUtils::ToString( connection->WriteNText( val ) ) );
		};
	};

	template<> struct QueryValueChecker< EFieldType_DATE >
	{
		static const EFieldType ValueType = EFieldType_DATE;

		static void Compare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const DatabaseUtils::Helpers< ValueType >::ParamType & val )
		{
			BOOST_CHECK_EQUAL( value.GetQueryValue(), connection->WriteDate( val ) );
		};
	};

	template<> struct QueryValueChecker< EFieldType_TIME >
	{
		static const EFieldType ValueType = EFieldType_TIME;

		static void Compare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const DatabaseUtils::Helpers< ValueType >::ParamType & val )
		{
			BOOST_CHECK_EQUAL( value.GetQueryValue(), connection->WriteTime( val ) );
		};
	};

	template<> struct QueryValueChecker< EFieldType_DATETIME >
	{
		static const EFieldType ValueType = EFieldType_DATETIME;

		static void Compare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const DatabaseUtils::Helpers< ValueType >::ParamType & val )
		{
			BOOST_CHECK_EQUAL( value.GetQueryValue(), connection->WriteDateTime( val ) );
		};
	};

	template<> struct QueryValueChecker< EFieldType_BINARY >
	{
		static const EFieldType ValueType = EFieldType_BINARY;

		static void Compare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const DatabaseUtils::Helpers< ValueType >::ParamType & val )
		{
			BOOST_CHECK_EQUAL( value.GetQueryValue(), connection->WriteBinary( val ) );
		};
	};

	template<> struct QueryValueChecker< EFieldType_VARBINARY >
	{
		static const EFieldType ValueType = EFieldType_VARBINARY;

		static void Compare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const DatabaseUtils::Helpers< ValueType >::ParamType & val )
		{
			BOOST_CHECK_EQUAL( value.GetQueryValue(), connection->WriteBinary( val ) );
		};
	};

	template<> struct QueryValueChecker< EFieldType_BLOB >
	{
		static const EFieldType ValueType = EFieldType_BLOB;

		static void Compare( DatabaseConnectionSPtr connection, const CDatabaseValue< ValueType > & value, const DatabaseUtils::Helpers< ValueType >::ParamType & val )
		{
			BOOST_CHECK_EQUAL( value.GetQueryValue(), connection->WriteBinary( val ) );
		};
	};

	template< EFieldType ValueType > struct ValueChecks
	{
		static void Copy( std::random_device & generator )
		{
			String connectionString;
			DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( TEST_GOOD_SERVER, TEST_GOOD_USER, TEST_GOOD_PASSWORD, connectionString );
			connection->SelectDatabase( TEST_GOOD_DATABASE );
			CDatabaseValue< ValueType > valueA( connection );
			BOOST_CHECK_NO_THROW( valueA.SetValue( DatabaseUtils::Helpers< ValueType >::InitialiseValue( generator ) ) );
			CDatabaseValue< ValueType > valueB( connection );
			BOOST_CHECK_NO_THROW( valueB.SetValue( DatabaseUtils::Helpers< ValueType >::InitialiseValue( generator ) ) );
			BOOST_CHECK_NO_THROW( valueA.SetValue( valueB ) );
			BOOST_CHECK_EQUAL( valueA.GetValue(), valueB.GetValue() );
		}

		static void QueryValue( std::random_device & generator )
		{
			String connectionString;
			DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( TEST_GOOD_SERVER, TEST_GOOD_USER, TEST_GOOD_PASSWORD, connectionString );
			connection->SelectDatabase( TEST_GOOD_DATABASE );
			CDatabaseValue< ValueType > value( connection );
			auto val = DatabaseUtils::Helpers< ValueType >::InitialiseValue( generator );
			BOOST_CHECK_NO_THROW( value.SetValue( val ) );
			QueryValueChecker< ValueType >::Compare( connection, value, val );
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
		CLogger::LogInfo( StringStream() << "**** Start TestCase_ValueCopy ****" );
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

		CLogger::LogInfo( StringStream() << "**** End TestCase_ValueCopy ****" );
	}
}
END_NAMESPACE_DATABASE_TEST
