/************************************************************************//**
 * @file DatabaseRowTest.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 12/02/2014 14:29:35
 *
 *
 * @brief Class testing CDatabaseRow class
*
***************************************************************************/

#include "DatabaseTestPch.h"

#include "DatabaseRowTest.h"

#include "DatabaseTestConnection.h"
#include "DatabaseTestHelpers.h"
#include "DatabaseTestValuedObject.h"

#include <DatabaseValuedObjectInfos.h>

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
	namespace
	{
		template< EFieldType FieldType, typename Enable = void >
		struct InfosCreator
		{
			static DatabaseValuedObjectInfosSPtr Create()
			{
				return std::make_shared< CDatabaseValuedObjectInfos >( STR( "Infos" ), FieldType );
			}
		};

		template< EFieldType FieldType >
		struct InfosCreator< FieldType, typename std::enable_if< SFieldTypeNeedsLimits< FieldType >::value >::type >
		{
			static DatabaseValuedObjectInfosSPtr Create()
			{
				return std::make_shared< CDatabaseValuedObjectInfos >( STR( "Infos" ), FieldType, DatabaseUtils::Helpers< FieldType >::Limit );
			}
		};

		template< EFieldType FieldType >
		struct InfosCreator< FieldType, typename std::enable_if< SFieldTypeNeedsPrecision< FieldType >::value >::type >
		{
			static DatabaseValuedObjectInfosSPtr Create()
			{
				return std::make_shared< CDatabaseValuedObjectInfos >( STR( "Infos" ), FieldType, DatabaseUtils::Helpers< FieldType >::Precision );
			}
		};

		template< EFieldType FieldTypeA, EFieldType FieldTypeB >
		struct GetValueCheck
		{
			static void Check( std::random_device & generator, DatabaseConnectionSPtr connection, DatabaseValuedObjectInfosSPtr infos )
			{
				CDatabaseRow row;
				row.AddField( std::make_shared< CDatabaseField >( connection, infos ) );
				auto valueIn = DatabaseUtils::Helpers< FieldTypeA >::GetRandomValue( generator );
				BOOST_CHECK_NO_THROW( static_cast< CDatabaseValue< FieldTypeA > & >( row.GetField( 0 )->GetObjectValue() ).SetValue( valueIn ) );
				typename DatabaseUtils::Helpers< FieldTypeB >::ParamType valueOut;

				if ( AreTypesCompatibleGet( FieldTypeA, FieldTypeB ) )
				{
					BOOST_CHECK_NO_THROW( row.Get( 0, valueOut ) );
				}
				else
				{
					BOOST_CHECK_THROW( row.Get( 0, valueOut ), CDatabaseException );
				}
			}
		};

		template< EFieldType FieldTypeA, EFieldType FieldTypeB >
		struct GetValueOptCheck
		{
			static void Check( std::random_device & generator, DatabaseConnectionSPtr connection, DatabaseValuedObjectInfosSPtr infos )
			{
				CDatabaseRow row;
				row.AddField( std::make_shared< CDatabaseField >( connection, infos ) );
				CDatabaseNullable< typename DatabaseUtils::Helpers< FieldTypeB >::ParamType > valueOpt;
				BOOST_CHECK_NO_THROW( row.GetOpt( 0, valueOpt ) );
				BOOST_CHECK( !valueOpt );
				auto value = DatabaseUtils::Helpers< FieldTypeA >::GetRandomValue( generator );
				BOOST_CHECK_NO_THROW( static_cast< CDatabaseValue< FieldTypeA > & >( row.GetField( 0 )->GetObjectValue() ).SetValue( value ) );

				if ( AreTypesCompatibleGet( FieldTypeA, FieldTypeB ) )
				{
					BOOST_CHECK_NO_THROW( row.GetOpt( 0, valueOpt ) );
					BOOST_CHECK( ( bool )valueOpt );
				}
				else
				{
					BOOST_CHECK_THROW( row.GetOpt( 0, valueOpt ), CDatabaseException );
				}
			}
		};

		template< EFieldType FieldTypeA, EFieldType FieldTypeB >
		struct GetValueFastCheck
		{
			static void Check( std::random_device & generator, DatabaseConnectionSPtr connection, DatabaseValuedObjectInfosSPtr infos )
			{
				CDatabaseRow row;
				row.AddField( std::make_shared< CDatabaseField >( connection, infos ) );
				auto valueIn = DatabaseUtils::Helpers< FieldTypeA >::GetRandomValue( generator );
				BOOST_CHECK_NO_THROW( static_cast< CDatabaseValue< FieldTypeA > & >( row.GetField( 0 )->GetObjectValue() ).SetValue( valueIn ) );
				typename DatabaseUtils::Helpers< FieldTypeB >::ParamType valueOut;
				BOOST_CHECK_NO_THROW( row.GetFast( 0, valueOut ) );
				BOOST_CHECK_EQUAL( valueIn, valueOut );
			}
		};

		template< EFieldType FieldTypeA, EFieldType FieldTypeB >
		struct GetValueOptFastCheck
		{
			static void Check( std::random_device & generator, DatabaseConnectionSPtr connection, DatabaseValuedObjectInfosSPtr infos )
			{
				CDatabaseRow row;
				row.AddField( std::make_shared< CDatabaseField >( connection, infos ) );
				CDatabaseNullable< typename DatabaseUtils::Helpers< FieldTypeB >::ParamType > valueOpt;
				BOOST_CHECK_NO_THROW( row.GetOptFast( 0, valueOpt ) );
				BOOST_CHECK( !valueOpt );
				auto value = DatabaseUtils::Helpers< FieldTypeA >::GetRandomValue( generator );
				BOOST_CHECK_NO_THROW( static_cast< CDatabaseValue< FieldTypeA > & >( row.GetField( 0 )->GetObjectValue() ).SetValue( value ) );
				BOOST_CHECK_NO_THROW( row.GetOptFast( 0, valueOpt ) );
				BOOST_CHECK( ( bool )valueOpt );
				BOOST_CHECK_EQUAL( *valueOpt, value );
			}
		};

		template< EFieldType FieldType >
		struct SValuedObjectChecks
		{
			static void GetValueChecks( std::random_device & generator )
			{
				String connectionString;
				DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( TEST_GOOD_SERVER, TEST_GOOD_USER, TEST_GOOD_PASSWORD, connectionString );
				connection->SelectDatabase( TEST_GOOD_DATABASE );
				DatabaseValuedObjectInfosSPtr infos = InfosCreator< FieldType >::Create();

				GetValueCheck< FieldType, EFieldType_BIT >::Check( generator, connection, infos );
				GetValueCheck< FieldType, EFieldType_SINT8 >::Check( generator, connection, infos );
				GetValueCheck< FieldType, EFieldType_SINT16 >::Check( generator, connection, infos );
				GetValueCheck< FieldType, EFieldType_SINT24 >::Check( generator, connection, infos );
				GetValueCheck< FieldType, EFieldType_SINT32 >::Check( generator, connection, infos );
				GetValueCheck< FieldType, EFieldType_SINT64 >::Check( generator, connection, infos );
				GetValueCheck< FieldType, EFieldType_UINT8 >::Check( generator, connection, infos );
				GetValueCheck< FieldType, EFieldType_UINT16 >::Check( generator, connection, infos );
				GetValueCheck< FieldType, EFieldType_UINT24 >::Check( generator, connection, infos );
				GetValueCheck< FieldType, EFieldType_UINT32 >::Check( generator, connection, infos );
				GetValueCheck< FieldType, EFieldType_UINT64 >::Check( generator, connection, infos );
				GetValueCheck< FieldType, EFieldType_FLOAT32 >::Check( generator, connection, infos );
				GetValueCheck< FieldType, EFieldType_FLOAT64 >::Check( generator, connection, infos );
				GetValueCheck< FieldType, EFieldType_FIXED_POINT >::Check( generator, connection, infos );
				GetValueCheck< FieldType, EFieldType_CHAR >::Check( generator, connection, infos );
				GetValueCheck< FieldType, EFieldType_VARCHAR >::Check( generator, connection, infos );
				GetValueCheck< FieldType, EFieldType_TEXT >::Check( generator, connection, infos );
				GetValueCheck< FieldType, EFieldType_NCHAR >::Check( generator, connection, infos );
				GetValueCheck< FieldType, EFieldType_NVARCHAR >::Check( generator, connection, infos );
				GetValueCheck< FieldType, EFieldType_NTEXT >::Check( generator, connection, infos );
				GetValueCheck< FieldType, EFieldType_DATE >::Check( generator, connection, infos );
				GetValueCheck< FieldType, EFieldType_DATETIME >::Check( generator, connection, infos );
				GetValueCheck< FieldType, EFieldType_TIME >::Check( generator, connection, infos );
				GetValueCheck< FieldType, EFieldType_BINARY >::Check( generator, connection, infos );
				GetValueCheck< FieldType, EFieldType_VARBINARY >::Check( generator, connection, infos );
				GetValueCheck< FieldType, EFieldType_BLOB >::Check( generator, connection, infos );
			}

			static void GetValueOptChecks( std::random_device & generator )
			{
				String connectionString;
				DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( TEST_GOOD_SERVER, TEST_GOOD_USER, TEST_GOOD_PASSWORD, connectionString );
				connection->SelectDatabase( TEST_GOOD_DATABASE );
				DatabaseValuedObjectInfosSPtr infos = InfosCreator< FieldType >::Create();

				GetValueOptCheck< FieldType, EFieldType_BIT >::Check( generator, connection, infos );
				GetValueOptCheck< FieldType, EFieldType_SINT8 >::Check( generator, connection, infos );
				GetValueOptCheck< FieldType, EFieldType_SINT16 >::Check( generator, connection, infos );
				GetValueOptCheck< FieldType, EFieldType_SINT24 >::Check( generator, connection, infos );
				GetValueOptCheck< FieldType, EFieldType_SINT32 >::Check( generator, connection, infos );
				GetValueOptCheck< FieldType, EFieldType_SINT64 >::Check( generator, connection, infos );
				GetValueOptCheck< FieldType, EFieldType_UINT8 >::Check( generator, connection, infos );
				GetValueOptCheck< FieldType, EFieldType_UINT16 >::Check( generator, connection, infos );
				GetValueOptCheck< FieldType, EFieldType_UINT24 >::Check( generator, connection, infos );
				GetValueOptCheck< FieldType, EFieldType_UINT32 >::Check( generator, connection, infos );
				GetValueOptCheck< FieldType, EFieldType_UINT64 >::Check( generator, connection, infos );
				GetValueOptCheck< FieldType, EFieldType_FLOAT32 >::Check( generator, connection, infos );
				GetValueOptCheck< FieldType, EFieldType_FLOAT64 >::Check( generator, connection, infos );
				GetValueOptCheck< FieldType, EFieldType_FIXED_POINT >::Check( generator, connection, infos );
				GetValueOptCheck< FieldType, EFieldType_CHAR >::Check( generator, connection, infos );
				GetValueOptCheck< FieldType, EFieldType_VARCHAR >::Check( generator, connection, infos );
				GetValueOptCheck< FieldType, EFieldType_TEXT >::Check( generator, connection, infos );
				GetValueOptCheck< FieldType, EFieldType_NCHAR >::Check( generator, connection, infos );
				GetValueOptCheck< FieldType, EFieldType_NVARCHAR >::Check( generator, connection, infos );
				GetValueOptCheck< FieldType, EFieldType_NTEXT >::Check( generator, connection, infos );
				GetValueOptCheck< FieldType, EFieldType_DATE >::Check( generator, connection, infos );
				GetValueOptCheck< FieldType, EFieldType_DATETIME >::Check( generator, connection, infos );
				GetValueOptCheck< FieldType, EFieldType_TIME >::Check( generator, connection, infos );
				GetValueOptCheck< FieldType, EFieldType_BINARY >::Check( generator, connection, infos );
				GetValueOptCheck< FieldType, EFieldType_VARBINARY >::Check( generator, connection, infos );
				GetValueOptCheck< FieldType, EFieldType_BLOB >::Check( generator, connection, infos );
			}

			static void GetValueFastChecks( std::random_device & generator )
			{
				String connectionString;
				DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( TEST_GOOD_SERVER, TEST_GOOD_USER, TEST_GOOD_PASSWORD, connectionString );
				connection->SelectDatabase( TEST_GOOD_DATABASE );
				DatabaseValuedObjectInfosSPtr infos = InfosCreator< FieldType >::Create();

				GetValueFastCheck< FieldType, FieldType >::Check( generator, connection, infos );
			}

			static void GetValueOptFastChecks( std::random_device & generator )
			{
				String connectionString;
				DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( TEST_GOOD_SERVER, TEST_GOOD_USER, TEST_GOOD_PASSWORD, connectionString );
				connection->SelectDatabase( TEST_GOOD_DATABASE );
				DatabaseValuedObjectInfosSPtr infos = InfosCreator< FieldType >::Create();

				GetValueOptFastCheck< FieldType, FieldType >::Check( generator, connection, infos );
			}
		};
	}

	CDatabaseRowTest::CDatabaseRowTest()
	{
	}

	CDatabaseRowTest::~CDatabaseRowTest()
	{
	}

	boost::unit_test::test_suite * CDatabaseRowTest::Init_Test_Suite()
	{
		//!@remarks Create the internal TS instance.
		testSuite = new boost::unit_test::test_suite( "CDatabaseRowTest", __FILE__, __LINE__ );

		//!@remarks Add the TC to the internal TS.
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseRowTest::TestCase_RowFieldManagement, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseRowTest::TestCase_RowGet, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseRowTest::TestCase_RowGetOpt, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseRowTest::TestCase_RowGetFast, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseRowTest::TestCase_RowGetOptFast, this ) ) );

		//!@remarks Return the TS instance.
		return testSuite;
	}

	void CDatabaseRowTest::TestCase_RowFieldManagement()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_RowFieldManagement ****" );
		String goodname = STR( "TestField" );
		String wrongname = STR( "RowField" );
		bool valueOut;
		CDatabaseNullable< bool > valueOpt;
		String connectionString;
		DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( TEST_GOOD_SERVER, TEST_GOOD_USER, TEST_GOOD_PASSWORD, connectionString );
		connection->SelectDatabase( TEST_GOOD_DATABASE );

		CDatabaseRow row;
		BOOST_CHECK_THROW( row.GetField( 0 ), CDatabaseException );
		BOOST_CHECK_THROW( row.GetField( goodname ), CDatabaseException );
		BOOST_CHECK_THROW( row.IsNull( 0 ), CDatabaseException );
		BOOST_CHECK_THROW( row.IsNull( goodname ), CDatabaseException );
		BOOST_CHECK_THROW( row.Get< bool >( 0 ), CDatabaseException );
		BOOST_CHECK_THROW( row.Get< bool >( goodname ), CDatabaseException );
		BOOST_CHECK_THROW( row.GetOpt< bool >( 0 ), CDatabaseException );
		BOOST_CHECK_THROW( row.GetOpt< bool >( goodname ), CDatabaseException );
		BOOST_CHECK_THROW( row.GetFast( 0, valueOut ), CDatabaseException );
		BOOST_CHECK_THROW( row.GetFast( goodname, valueOut ), CDatabaseException );
		BOOST_CHECK_THROW( row.GetOptFast( 0, valueOpt ), CDatabaseException );
		BOOST_CHECK_THROW( row.GetOptFast( goodname, valueOpt ), CDatabaseException );

		row.AddField( std::make_shared< CDatabaseField >( connection, std::make_shared< CDatabaseValuedObjectInfos >( goodname, EFieldType_BIT ) ) );
		BOOST_CHECK_THROW( row.GetField( 1 ), CDatabaseException );
		BOOST_CHECK_THROW( row.GetField( wrongname ), CDatabaseException );
		BOOST_CHECK_THROW( row.IsNull( 1 ), CDatabaseException );
		BOOST_CHECK_THROW( row.IsNull( wrongname ), CDatabaseException );
		BOOST_CHECK_THROW( row.Get< bool >( 1 ), CDatabaseException );
		BOOST_CHECK_THROW( row.Get< bool >( wrongname ), CDatabaseException );
		BOOST_CHECK_THROW( row.GetOpt< bool >( 1 ), CDatabaseException );
		BOOST_CHECK_THROW( row.GetOpt< bool >( wrongname ), CDatabaseException );
		BOOST_CHECK_THROW( row.GetFast( 1, valueOut ), CDatabaseException );
		BOOST_CHECK_THROW( row.GetFast( wrongname, valueOut ), CDatabaseException );
		BOOST_CHECK_THROW( row.GetOptFast( 1, valueOpt ), CDatabaseException );
		BOOST_CHECK_THROW( row.GetOptFast( wrongname, valueOpt ), CDatabaseException );
		BOOST_CHECK_NO_THROW( row.GetField( 0 ) );
		BOOST_CHECK_NO_THROW( row.GetField( goodname ) );
		BOOST_CHECK_EQUAL( row.GetField( 0 ), row.GetField( goodname ) );
		BOOST_CHECK_NO_THROW( row.IsNull( 0 ) );
		BOOST_CHECK( row.IsNull( 0 ) );
		BOOST_CHECK_NO_THROW( row.Get< bool >( 0 ) );
		BOOST_CHECK_NO_THROW( row.GetOpt< bool >( 0 ) );
		BOOST_CHECK( !row.GetOpt< bool >( 0 ) );
		BOOST_CHECK_NO_THROW( row.GetFast( 0, valueOut ) );
		BOOST_CHECK_NO_THROW( row.GetOptFast( 0, valueOpt ) );
		BOOST_CHECK( !valueOpt );

		BOOST_CHECK_NO_THROW( static_cast< CDatabaseValue< EFieldType_BIT > & >( row.GetField( 0 )->GetObjectValue() ).SetValue( true ) );
		BOOST_CHECK( !row.IsNull( 0 ) );
		BOOST_CHECK( row.Get< bool >( 0 ) );
		BOOST_CHECK( ( bool )row.GetOpt< bool >( 0 ) );
		BOOST_CHECK( *row.GetOpt< bool >( 0 ) );
		BOOST_CHECK_NO_THROW( row.GetFast( 0, valueOut ) );
		BOOST_CHECK( valueOut );
		BOOST_CHECK_NO_THROW( row.GetOptFast( 0, valueOpt ) );
		BOOST_CHECK( ( bool )valueOpt );
		BOOST_CHECK( *valueOpt );

		CLogger::LogInfo( StringStream() << "**** End TestCase_RowFieldManagement ****" );
	}

	void CDatabaseRowTest::TestCase_RowGet()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_RowGet ****" );
		std::random_device generator;

		CLogger::LogInfo( StringStream() << "  EFieldType_BIT" );
		SValuedObjectChecks< EFieldType_BIT >::GetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT8" );
		SValuedObjectChecks< EFieldType_SINT8 >::GetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT16" );
		SValuedObjectChecks< EFieldType_SINT16 >::GetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT24" );
		SValuedObjectChecks< EFieldType_SINT24 >::GetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT32" );
		SValuedObjectChecks< EFieldType_SINT32 >::GetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT64" );
		SValuedObjectChecks< EFieldType_SINT64 >::GetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT8" );
		SValuedObjectChecks< EFieldType_UINT8 >::GetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT16" );
		SValuedObjectChecks< EFieldType_UINT16 >::GetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT24" );
		SValuedObjectChecks< EFieldType_UINT24 >::GetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT32" );
		SValuedObjectChecks< EFieldType_UINT32 >::GetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT64" );
		SValuedObjectChecks< EFieldType_UINT64 >::GetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_FLOAT32" );
		SValuedObjectChecks< EFieldType_FLOAT32 >::GetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_FLOAT64" );
		SValuedObjectChecks< EFieldType_FLOAT64 >::GetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_FIXED_POINT" );
		SValuedObjectChecks< EFieldType_FIXED_POINT >::GetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_CHAR" );
		SValuedObjectChecks< EFieldType_CHAR >::GetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_VARCHAR" );
		SValuedObjectChecks< EFieldType_VARCHAR >::GetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_TEXT" );
		SValuedObjectChecks< EFieldType_TEXT >::GetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_NCHAR" );
		SValuedObjectChecks< EFieldType_NCHAR >::GetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_NVARCHAR" );
		SValuedObjectChecks< EFieldType_NVARCHAR >::GetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_NTEXT" );
		SValuedObjectChecks< EFieldType_NTEXT >::GetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_DATE" );
		SValuedObjectChecks< EFieldType_DATE >::GetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_DATETIME" );
		SValuedObjectChecks< EFieldType_DATETIME >::GetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_TIME" );
		SValuedObjectChecks< EFieldType_TIME >::GetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_BINARY" );
		SValuedObjectChecks< EFieldType_BINARY >::GetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_VARBINARY" );
		SValuedObjectChecks< EFieldType_VARBINARY >::GetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_BLOB" );
		SValuedObjectChecks< EFieldType_BLOB >::GetValueChecks( generator );

		CLogger::LogInfo( StringStream() << "**** End TestCase_RowGet ****" );
	}

	void CDatabaseRowTest::TestCase_RowGetOpt()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_RowGetOpt ****" );
		std::random_device generator;

		CLogger::LogInfo( StringStream() << "  EFieldType_BIT" );
		SValuedObjectChecks< EFieldType_BIT >::GetValueOptChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT8" );
		SValuedObjectChecks< EFieldType_SINT8 >::GetValueOptChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT16" );
		SValuedObjectChecks< EFieldType_SINT16 >::GetValueOptChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT24" );
		SValuedObjectChecks< EFieldType_SINT24 >::GetValueOptChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT32" );
		SValuedObjectChecks< EFieldType_SINT32 >::GetValueOptChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT64" );
		SValuedObjectChecks< EFieldType_SINT64 >::GetValueOptChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT8" );
		SValuedObjectChecks< EFieldType_UINT8 >::GetValueOptChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT16" );
		SValuedObjectChecks< EFieldType_UINT16 >::GetValueOptChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT24" );
		SValuedObjectChecks< EFieldType_UINT24 >::GetValueOptChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT32" );
		SValuedObjectChecks< EFieldType_UINT32 >::GetValueOptChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT64" );
		SValuedObjectChecks< EFieldType_UINT64 >::GetValueOptChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_FLOAT32" );
		SValuedObjectChecks< EFieldType_FLOAT32 >::GetValueOptChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_FLOAT64" );
		SValuedObjectChecks< EFieldType_FLOAT64 >::GetValueOptChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_FIXED_POINT" );
		SValuedObjectChecks< EFieldType_FIXED_POINT >::GetValueOptChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_CHAR" );
		SValuedObjectChecks< EFieldType_CHAR >::GetValueOptChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_VARCHAR" );
		SValuedObjectChecks< EFieldType_VARCHAR >::GetValueOptChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_TEXT" );
		SValuedObjectChecks< EFieldType_TEXT >::GetValueOptChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_NCHAR" );
		SValuedObjectChecks< EFieldType_NCHAR >::GetValueOptChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_NVARCHAR" );
		SValuedObjectChecks< EFieldType_NVARCHAR >::GetValueOptChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_NTEXT" );
		SValuedObjectChecks< EFieldType_NTEXT >::GetValueOptChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_DATE" );
		SValuedObjectChecks< EFieldType_DATE >::GetValueOptChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_DATETIME" );
		SValuedObjectChecks< EFieldType_DATETIME >::GetValueOptChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_TIME" );
		SValuedObjectChecks< EFieldType_TIME >::GetValueOptChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_BINARY" );
		SValuedObjectChecks< EFieldType_BINARY >::GetValueOptChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_VARBINARY" );
		SValuedObjectChecks< EFieldType_VARBINARY >::GetValueOptChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_BLOB" );
		SValuedObjectChecks< EFieldType_BLOB >::GetValueOptChecks( generator );

		CLogger::LogInfo( StringStream() << "**** End TestCase_RowGetOpt ****" );
	}

	void CDatabaseRowTest::TestCase_RowGetFast()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_RowGetFast ****" );
		std::random_device generator;

		CLogger::LogInfo( StringStream() << "  EFieldType_BIT" );
		SValuedObjectChecks< EFieldType_BIT >::GetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT8" );
		SValuedObjectChecks< EFieldType_SINT8 >::GetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT16" );
		SValuedObjectChecks< EFieldType_SINT16 >::GetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT24" );
		SValuedObjectChecks< EFieldType_SINT24 >::GetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT32" );
		SValuedObjectChecks< EFieldType_SINT32 >::GetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT64" );
		SValuedObjectChecks< EFieldType_SINT64 >::GetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT8" );
		SValuedObjectChecks< EFieldType_UINT8 >::GetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT16" );
		SValuedObjectChecks< EFieldType_UINT16 >::GetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT24" );
		SValuedObjectChecks< EFieldType_UINT24 >::GetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT32" );
		SValuedObjectChecks< EFieldType_UINT32 >::GetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT64" );
		SValuedObjectChecks< EFieldType_UINT64 >::GetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_FLOAT32" );
		SValuedObjectChecks< EFieldType_FLOAT32 >::GetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_FLOAT64" );
		SValuedObjectChecks< EFieldType_FLOAT64 >::GetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_FIXED_POINT" );
		SValuedObjectChecks< EFieldType_FIXED_POINT >::GetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_CHAR" );
		SValuedObjectChecks< EFieldType_CHAR >::GetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_VARCHAR" );
		SValuedObjectChecks< EFieldType_VARCHAR >::GetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_TEXT" );
		SValuedObjectChecks< EFieldType_TEXT >::GetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_NCHAR" );
		SValuedObjectChecks< EFieldType_NCHAR >::GetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_NVARCHAR" );
		SValuedObjectChecks< EFieldType_NVARCHAR >::GetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_NTEXT" );
		SValuedObjectChecks< EFieldType_NTEXT >::GetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_DATE" );
		SValuedObjectChecks< EFieldType_DATE >::GetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_DATETIME" );
		SValuedObjectChecks< EFieldType_DATETIME >::GetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_TIME" );
		SValuedObjectChecks< EFieldType_TIME >::GetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_BINARY" );
		SValuedObjectChecks< EFieldType_BINARY >::GetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_VARBINARY" );
		SValuedObjectChecks< EFieldType_VARBINARY >::GetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_BLOB" );
		SValuedObjectChecks< EFieldType_BLOB >::GetValueFastChecks( generator );

		CLogger::LogInfo( StringStream() << "**** End TestCase_RowGetFast ****" );
	}

	void CDatabaseRowTest::TestCase_RowGetOptFast()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_RowGetOptFast ****" );
		std::random_device generator;

		CLogger::LogInfo( StringStream() << "  EFieldType_BIT" );
		SValuedObjectChecks< EFieldType_BIT >::GetValueOptFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT8" );
		SValuedObjectChecks< EFieldType_SINT8 >::GetValueOptFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT16" );
		SValuedObjectChecks< EFieldType_SINT16 >::GetValueOptFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT24" );
		SValuedObjectChecks< EFieldType_SINT24 >::GetValueOptFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT32" );
		SValuedObjectChecks< EFieldType_SINT32 >::GetValueOptFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT64" );
		SValuedObjectChecks< EFieldType_SINT64 >::GetValueOptFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT8" );
		SValuedObjectChecks< EFieldType_UINT8 >::GetValueOptFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT16" );
		SValuedObjectChecks< EFieldType_UINT16 >::GetValueOptFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT24" );
		SValuedObjectChecks< EFieldType_UINT24 >::GetValueOptFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT32" );
		SValuedObjectChecks< EFieldType_UINT32 >::GetValueOptFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT64" );
		SValuedObjectChecks< EFieldType_UINT64 >::GetValueOptFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_FLOAT32" );
		SValuedObjectChecks< EFieldType_FLOAT32 >::GetValueOptFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_FLOAT64" );
		SValuedObjectChecks< EFieldType_FLOAT64 >::GetValueOptFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_FIXED_POINT" );
		SValuedObjectChecks< EFieldType_FIXED_POINT >::GetValueOptFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_CHAR" );
		SValuedObjectChecks< EFieldType_CHAR >::GetValueOptFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_VARCHAR" );
		SValuedObjectChecks< EFieldType_VARCHAR >::GetValueOptFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_TEXT" );
		SValuedObjectChecks< EFieldType_TEXT >::GetValueOptFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_NCHAR" );
		SValuedObjectChecks< EFieldType_NCHAR >::GetValueOptFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_NVARCHAR" );
		SValuedObjectChecks< EFieldType_NVARCHAR >::GetValueOptFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_NTEXT" );
		SValuedObjectChecks< EFieldType_NTEXT >::GetValueOptFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_DATE" );
		SValuedObjectChecks< EFieldType_DATE >::GetValueOptFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_DATETIME" );
		SValuedObjectChecks< EFieldType_DATETIME >::GetValueOptFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_TIME" );
		SValuedObjectChecks< EFieldType_TIME >::GetValueOptFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_BINARY" );
		SValuedObjectChecks< EFieldType_BINARY >::GetValueOptFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_VARBINARY" );
		SValuedObjectChecks< EFieldType_VARBINARY >::GetValueOptFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_BLOB" );
		SValuedObjectChecks< EFieldType_BLOB >::GetValueOptFastChecks( generator );

		CLogger::LogInfo( StringStream() << "**** End TestCase_RowGetOptFast ****" );
	}
}
END_NAMESPACE_DATABASE_TEST
