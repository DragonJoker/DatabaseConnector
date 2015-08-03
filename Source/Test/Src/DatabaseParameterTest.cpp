/************************************************************************//**
 * @file DatabaseParameterTest.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 12/02/2014 14:29:35
 *
 *
* @brief Class testing CDatabaseParameter class
*
***************************************************************************/

#include "DatabaseTestPch.h"

#include "DatabaseParameterTest.h"

#include "DatabaseTestConnection.h"
#include "DatabaseTestUtils.h"
#include "DatabaseTestValuedObject.h"

#include <DatabaseValuedObjectInfos.h>

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
	namespace
	{
		struct SParameterValueUpdater
			: public CDatabaseParameter::SValueUpdater
		{
			virtual void Update( const CDatabaseParameter & parameter )
			{
			}
		};

		template< EFieldType FieldType, typename Enable=void >
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
				CDatabaseParameter object( connection, infos, 0, EParameterType_IN, std::make_unique< SParameterValueUpdater >() );
				BOOST_CHECK_NO_THROW( object.SetValue( DatabaseUtils::Helpers< FieldTypeA >::GetRandomValue( generator ) ) );
				DatabaseUtils::Helpers< FieldTypeB >::ParamType value;

				if ( AreTypesCompatibleGet( FieldTypeA, FieldTypeB ) )
				{
					BOOST_CHECK_NO_THROW( object.GetValue( value ) );
				}
				else
				{
					BOOST_CHECK_THROW( object.GetValue( value ), CDatabaseException );
				}
			}
		};

		template< EFieldType FieldTypeA, EFieldType FieldTypeB >
		struct GetValueFastCheck
		{
			static void Check( std::random_device & generator, DatabaseConnectionSPtr connection, DatabaseValuedObjectInfosSPtr infos )
			{
				CDatabaseParameter object( connection, infos, 0, EParameterType_IN, std::make_unique< SParameterValueUpdater >() );
				DatabaseUtils::Helpers< FieldTypeA >::ParamType valueIn = DatabaseUtils::Helpers< FieldTypeA >::GetRandomValue( generator );
				BOOST_CHECK_NO_THROW( object.SetValue( valueIn ) );
				DatabaseUtils::Helpers< FieldTypeB >::ParamType valueOut;
				BOOST_CHECK_NO_THROW( object.GetValueFast( valueOut ) );
				BOOST_CHECK_EQUAL( valueIn, valueOut );
			}
		};

		template< EFieldType FieldTypeA, EFieldType FieldTypeB >
		struct SetValueCheck
		{
			static void Check( std::random_device & generator, DatabaseConnectionSPtr connection, DatabaseValuedObjectInfosSPtr infos )
			{
				CDatabaseParameter object( connection, infos, 0, EParameterType_IN, std::make_unique< SParameterValueUpdater >() );

				if ( AreTypesCompatibleSet( FieldTypeB, FieldTypeA ) )
				{
					BOOST_CHECK_NO_THROW( object.SetValue( DatabaseUtils::Helpers< FieldTypeB >::GetRandomValue( generator ) ) );
				}
				else
				{
					BOOST_CHECK_THROW( object.SetValue( DatabaseUtils::Helpers< FieldTypeB >::GetRandomValue( generator ) ), CDatabaseException );
				}
			}
		};

		template< EFieldType FieldTypeA, EFieldType FieldTypeB >
		struct SetValueFastCheck
		{
			static void Check( std::random_device & generator, DatabaseConnectionSPtr connection, DatabaseValuedObjectInfosSPtr infos )
			{
				CDatabaseParameter object( connection, infos, 0, EParameterType_IN, std::make_unique< SParameterValueUpdater >() );
				DatabaseUtils::Helpers< FieldTypeB >::ParamType valueIn = DatabaseUtils::Helpers< FieldTypeB >::GetRandomValue( generator );
				BOOST_CHECK_NO_THROW( object.SetValueFast( valueIn ) );
				DatabaseUtils::Helpers< FieldTypeA >::ParamType valueOut;
				object.GetValue( valueOut );
				BOOST_CHECK_EQUAL( valueIn, valueOut );
			}
		};

		template< EFieldType FieldTypeA, EFieldType FieldTypeB >
		struct SetFromValueCheck
		{
			static void Check( std::random_device & generator, DatabaseConnectionSPtr connection, DatabaseValuedObjectInfosSPtr infos )
			{
				CDatabaseParameter object( connection, infos, 0, EParameterType_IN, std::make_unique< SParameterValueUpdater >() );
				CDatabaseValue< FieldTypeB > value( connection );
				BOOST_CHECK_NO_THROW( object.SetValue( FieldTypeB, value ) );
				value.SetValue( DatabaseUtils::Helpers< FieldTypeB >::GetRandomValue( generator ) );

				if ( AreTypesCompatibleSet( FieldTypeB, FieldTypeA ) )
				{
					BOOST_CHECK_NO_THROW( object.SetValue( FieldTypeB, value ) );
				}
				else
				{
					BOOST_CHECK_THROW( object.SetValue( FieldTypeB, value ), CDatabaseException );
				}
			}
		};

		template< EFieldType FieldTypeA, EFieldType FieldTypeB >
		struct SetFromValuedObjectCheck
		{
			static void Check( std::random_device & generator, DatabaseConnectionSPtr connection, DatabaseValuedObjectInfosSPtr infosA )
			{
				CDatabaseParameter objectA( connection, infosA, 0, EParameterType_IN, std::make_unique< SParameterValueUpdater >() );
				DatabaseValuedObjectInfosSPtr infosB = InfosCreator< FieldTypeB >::Create();
				CDatabaseTestValuedObject< FieldTypeB > objectB( connection, infosB );
				BOOST_CHECK_NO_THROW( objectA.SetValue( static_cast< const CDatabaseValuedObject & >( objectB ) ) );
				objectB.SetValue( DatabaseUtils::Helpers< FieldTypeB >::GetRandomValue( generator ) );

				if ( AreTypesCompatibleSet( FieldTypeB, FieldTypeA ) )
				{
					BOOST_CHECK_NO_THROW( objectA.SetValue( static_cast< const CDatabaseValuedObject & >( objectB ) ) );
				}
				else
				{
					BOOST_CHECK_THROW( objectA.SetValue( static_cast< const CDatabaseValuedObject & >( objectB ) ), CDatabaseException );
				}
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

			static void GetValueFastChecks( std::random_device & generator )
			{
				String connectionString;
				DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( TEST_GOOD_SERVER, TEST_GOOD_USER, TEST_GOOD_PASSWORD, connectionString );
				connection->SelectDatabase( TEST_GOOD_DATABASE );
				DatabaseValuedObjectInfosSPtr infos = InfosCreator< FieldType >::Create();

				GetValueFastCheck< FieldType, FieldType >::Check( generator, connection, infos );
			}

			static void SetValueChecks( std::random_device & generator )
			{
				String connectionString;
				DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( TEST_GOOD_SERVER, TEST_GOOD_USER, TEST_GOOD_PASSWORD, connectionString );
				connection->SelectDatabase( TEST_GOOD_DATABASE );
				DatabaseValuedObjectInfosSPtr infos = InfosCreator< FieldType >::Create();

				SetValueCheck< FieldType, EFieldType_BIT >::Check( generator, connection, infos );
				SetValueCheck< FieldType, EFieldType_SINT8 >::Check( generator, connection, infos );
				SetValueCheck< FieldType, EFieldType_SINT16 >::Check( generator, connection, infos );
				SetValueCheck< FieldType, EFieldType_SINT24 >::Check( generator, connection, infos );
				SetValueCheck< FieldType, EFieldType_SINT32 >::Check( generator, connection, infos );
				SetValueCheck< FieldType, EFieldType_SINT64 >::Check( generator, connection, infos );
				SetValueCheck< FieldType, EFieldType_UINT8 >::Check( generator, connection, infos );
				SetValueCheck< FieldType, EFieldType_UINT16 >::Check( generator, connection, infos );
				SetValueCheck< FieldType, EFieldType_UINT24 >::Check( generator, connection, infos );
				SetValueCheck< FieldType, EFieldType_UINT32 >::Check( generator, connection, infos );
				SetValueCheck< FieldType, EFieldType_UINT64 >::Check( generator, connection, infos );
				SetValueCheck< FieldType, EFieldType_FLOAT32 >::Check( generator, connection, infos );
				SetValueCheck< FieldType, EFieldType_FLOAT64 >::Check( generator, connection, infos );
				SetValueCheck< FieldType, EFieldType_FIXED_POINT >::Check( generator, connection, infos );
				SetValueCheck< FieldType, EFieldType_CHAR >::Check( generator, connection, infos );
				SetValueCheck< FieldType, EFieldType_VARCHAR >::Check( generator, connection, infos );
				SetValueCheck< FieldType, EFieldType_TEXT >::Check( generator, connection, infos );
				SetValueCheck< FieldType, EFieldType_NCHAR >::Check( generator, connection, infos );
				SetValueCheck< FieldType, EFieldType_NVARCHAR >::Check( generator, connection, infos );
				SetValueCheck< FieldType, EFieldType_NTEXT >::Check( generator, connection, infos );
				SetValueCheck< FieldType, EFieldType_DATE >::Check( generator, connection, infos );
				SetValueCheck< FieldType, EFieldType_DATETIME >::Check( generator, connection, infos );
				SetValueCheck< FieldType, EFieldType_TIME >::Check( generator, connection, infos );
				SetValueCheck< FieldType, EFieldType_BINARY >::Check( generator, connection, infos );
				SetValueCheck< FieldType, EFieldType_VARBINARY >::Check( generator, connection, infos );
				SetValueCheck< FieldType, EFieldType_BLOB >::Check( generator, connection, infos );
			}

			static void SetValueFastChecks( std::random_device & generator )
			{
				String connectionString;
				DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( TEST_GOOD_SERVER, TEST_GOOD_USER, TEST_GOOD_PASSWORD, connectionString );
				connection->SelectDatabase( TEST_GOOD_DATABASE );
				DatabaseValuedObjectInfosSPtr infos = InfosCreator< FieldType >::Create();

				SetValueFastCheck< FieldType, FieldType >::Check( generator, connection, infos );
			}

			static void SetValueFromValueChecks( std::random_device & generator )
			{
				String connectionString;
				DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( TEST_GOOD_SERVER, TEST_GOOD_USER, TEST_GOOD_PASSWORD, connectionString );
				connection->SelectDatabase( TEST_GOOD_DATABASE );
				DatabaseValuedObjectInfosSPtr infos = InfosCreator< FieldType >::Create();

				SetFromValueCheck< FieldType, EFieldType_BIT >::Check( generator, connection, infos );
				SetFromValueCheck< FieldType, EFieldType_SINT8 >::Check( generator, connection, infos );
				SetFromValueCheck< FieldType, EFieldType_SINT16 >::Check( generator, connection, infos );
				SetFromValueCheck< FieldType, EFieldType_SINT24 >::Check( generator, connection, infos );
				SetFromValueCheck< FieldType, EFieldType_SINT32 >::Check( generator, connection, infos );
				SetFromValueCheck< FieldType, EFieldType_SINT64 >::Check( generator, connection, infos );
				SetFromValueCheck< FieldType, EFieldType_UINT8 >::Check( generator, connection, infos );
				SetFromValueCheck< FieldType, EFieldType_UINT16 >::Check( generator, connection, infos );
				SetFromValueCheck< FieldType, EFieldType_UINT24 >::Check( generator, connection, infos );
				SetFromValueCheck< FieldType, EFieldType_UINT32 >::Check( generator, connection, infos );
				SetFromValueCheck< FieldType, EFieldType_UINT64 >::Check( generator, connection, infos );
				SetFromValueCheck< FieldType, EFieldType_FLOAT32 >::Check( generator, connection, infos );
				SetFromValueCheck< FieldType, EFieldType_FLOAT64 >::Check( generator, connection, infos );
				SetFromValueCheck< FieldType, EFieldType_FIXED_POINT >::Check( generator, connection, infos );
				SetFromValueCheck< FieldType, EFieldType_CHAR >::Check( generator, connection, infos );
				SetFromValueCheck< FieldType, EFieldType_VARCHAR >::Check( generator, connection, infos );
				SetFromValueCheck< FieldType, EFieldType_TEXT >::Check( generator, connection, infos );
				SetFromValueCheck< FieldType, EFieldType_NCHAR >::Check( generator, connection, infos );
				SetFromValueCheck< FieldType, EFieldType_NVARCHAR >::Check( generator, connection, infos );
				SetFromValueCheck< FieldType, EFieldType_NTEXT >::Check( generator, connection, infos );
				SetFromValueCheck< FieldType, EFieldType_DATE >::Check( generator, connection, infos );
				SetFromValueCheck< FieldType, EFieldType_DATETIME >::Check( generator, connection, infos );
				SetFromValueCheck< FieldType, EFieldType_TIME >::Check( generator, connection, infos );
				SetFromValueCheck< FieldType, EFieldType_BINARY >::Check( generator, connection, infos );
				SetFromValueCheck< FieldType, EFieldType_VARBINARY >::Check( generator, connection, infos );
				SetFromValueCheck< FieldType, EFieldType_BLOB >::Check( generator, connection, infos );
			}

			static void SetValueFromValuedObjectChecks( std::random_device & generator )
			{
				String connectionString;
				DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( TEST_GOOD_SERVER, TEST_GOOD_USER, TEST_GOOD_PASSWORD, connectionString );
				connection->SelectDatabase( TEST_GOOD_DATABASE );
				DatabaseValuedObjectInfosSPtr infos = InfosCreator< FieldType >::Create();

				SetFromValuedObjectCheck< FieldType, EFieldType_BIT >::Check( generator, connection, infos );
				SetFromValuedObjectCheck< FieldType, EFieldType_SINT8 >::Check( generator, connection, infos );
				SetFromValuedObjectCheck< FieldType, EFieldType_SINT16 >::Check( generator, connection, infos );
				SetFromValuedObjectCheck< FieldType, EFieldType_SINT24 >::Check( generator, connection, infos );
				SetFromValuedObjectCheck< FieldType, EFieldType_SINT32 >::Check( generator, connection, infos );
				SetFromValuedObjectCheck< FieldType, EFieldType_SINT64 >::Check( generator, connection, infos );
				SetFromValuedObjectCheck< FieldType, EFieldType_UINT8 >::Check( generator, connection, infos );
				SetFromValuedObjectCheck< FieldType, EFieldType_UINT16 >::Check( generator, connection, infos );
				SetFromValuedObjectCheck< FieldType, EFieldType_UINT24 >::Check( generator, connection, infos );
				SetFromValuedObjectCheck< FieldType, EFieldType_UINT32 >::Check( generator, connection, infos );
				SetFromValuedObjectCheck< FieldType, EFieldType_UINT64 >::Check( generator, connection, infos );
				SetFromValuedObjectCheck< FieldType, EFieldType_FLOAT32 >::Check( generator, connection, infos );
				SetFromValuedObjectCheck< FieldType, EFieldType_FLOAT64 >::Check( generator, connection, infos );
				SetFromValuedObjectCheck< FieldType, EFieldType_FIXED_POINT >::Check( generator, connection, infos );
				SetFromValuedObjectCheck< FieldType, EFieldType_CHAR >::Check( generator, connection, infos );
				SetFromValuedObjectCheck< FieldType, EFieldType_VARCHAR >::Check( generator, connection, infos );
				SetFromValuedObjectCheck< FieldType, EFieldType_TEXT >::Check( generator, connection, infos );
				SetFromValuedObjectCheck< FieldType, EFieldType_NCHAR >::Check( generator, connection, infos );
				SetFromValuedObjectCheck< FieldType, EFieldType_NVARCHAR >::Check( generator, connection, infos );
				SetFromValuedObjectCheck< FieldType, EFieldType_NTEXT >::Check( generator, connection, infos );
				SetFromValuedObjectCheck< FieldType, EFieldType_DATE >::Check( generator, connection, infos );
				SetFromValuedObjectCheck< FieldType, EFieldType_DATETIME >::Check( generator, connection, infos );
				SetFromValuedObjectCheck< FieldType, EFieldType_TIME >::Check( generator, connection, infos );
				SetFromValuedObjectCheck< FieldType, EFieldType_BINARY >::Check( generator, connection, infos );
				SetFromValuedObjectCheck< FieldType, EFieldType_VARBINARY >::Check( generator, connection, infos );
				SetFromValuedObjectCheck< FieldType, EFieldType_BLOB >::Check( generator, connection, infos );
			}
		};
	}

	CDatabaseParameterTest::CDatabaseParameterTest()
	{
	}

	CDatabaseParameterTest::~CDatabaseParameterTest()
	{
	}

	boost::unit_test::test_suite * CDatabaseParameterTest::Init_Test_Suite()
	{
		//!@remarks Create the internal TS instance.
		testSuite = new boost::unit_test::test_suite( "CDatabaseParameterTest" );

		//!@remarks Add the TC to the internal TS.
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseParameterTest::TestCase_ParameterGetValue, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseParameterTest::TestCase_ParameterSetValue, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseParameterTest::TestCase_ParameterSetValueFast, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseParameterTest::TestCase_ParameterSetValueFromValue, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseParameterTest::TestCase_ParameterSetValueFromValuedObject, this ) ) );

		//!@remarks Return the TS instance.
		return testSuite;
	}

	void CDatabaseParameterTest::TestCase_ParameterGetValue()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_ParameterGetValue ****" );
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

		CLogger::LogInfo( StringStream() << "**** End TestCase_ParameterGetValue ****" );
	}

	void CDatabaseParameterTest::TestCase_ParameterSetValue()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_ParameterSetValue ****" );
		std::random_device generator;

		CLogger::LogInfo( StringStream() << "  EFieldType_BIT" );
		SValuedObjectChecks< EFieldType_BIT >::SetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT8" );
		SValuedObjectChecks< EFieldType_SINT8 >::SetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT16" );
		SValuedObjectChecks< EFieldType_SINT16 >::SetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT24" );
		SValuedObjectChecks< EFieldType_SINT24 >::SetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT32" );
		SValuedObjectChecks< EFieldType_SINT32 >::SetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT64" );
		SValuedObjectChecks< EFieldType_SINT64 >::SetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT8" );
		SValuedObjectChecks< EFieldType_UINT8 >::SetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT16" );
		SValuedObjectChecks< EFieldType_UINT16 >::SetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT24" );
		SValuedObjectChecks< EFieldType_UINT24 >::SetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT32" );
		SValuedObjectChecks< EFieldType_UINT32 >::SetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT64" );
		SValuedObjectChecks< EFieldType_UINT64 >::SetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_FLOAT32" );
		SValuedObjectChecks< EFieldType_FLOAT32 >::SetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_FLOAT64" );
		SValuedObjectChecks< EFieldType_FLOAT64 >::SetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_FIXED_POINT" );
		SValuedObjectChecks< EFieldType_FIXED_POINT >::SetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_CHAR" );
		SValuedObjectChecks< EFieldType_CHAR >::SetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_VARCHAR" );
		SValuedObjectChecks< EFieldType_VARCHAR >::SetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_TEXT" );
		SValuedObjectChecks< EFieldType_TEXT >::SetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_NCHAR" );
		SValuedObjectChecks< EFieldType_NCHAR >::SetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_NVARCHAR" );
		SValuedObjectChecks< EFieldType_NVARCHAR >::SetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_NTEXT" );
		SValuedObjectChecks< EFieldType_NTEXT >::SetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_DATE" );
		SValuedObjectChecks< EFieldType_DATE >::SetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_DATETIME" );
		SValuedObjectChecks< EFieldType_DATETIME >::SetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_TIME" );
		SValuedObjectChecks< EFieldType_TIME >::SetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_BINARY" );
		SValuedObjectChecks< EFieldType_BINARY >::SetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_VARBINARY" );
		SValuedObjectChecks< EFieldType_VARBINARY >::SetValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_BLOB" );
		SValuedObjectChecks< EFieldType_BLOB >::SetValueChecks( generator );

		CLogger::LogInfo( StringStream() << "**** End TestCase_ParameterSetValue ****" );
	}

	void CDatabaseParameterTest::TestCase_ParameterSetValueFast()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_ParameterSetValueFast ****" );
		std::random_device generator;

		CLogger::LogInfo( StringStream() << "  EFieldType_BIT" );
		SValuedObjectChecks< EFieldType_BIT >::SetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT8" );
		SValuedObjectChecks< EFieldType_SINT8 >::SetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT16" );
		SValuedObjectChecks< EFieldType_SINT16 >::SetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT24" );
		SValuedObjectChecks< EFieldType_SINT24 >::SetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT32" );
		SValuedObjectChecks< EFieldType_SINT32 >::SetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT64" );
		SValuedObjectChecks< EFieldType_SINT64 >::SetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT8" );
		SValuedObjectChecks< EFieldType_UINT8 >::SetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT16" );
		SValuedObjectChecks< EFieldType_UINT16 >::SetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT24" );
		SValuedObjectChecks< EFieldType_UINT24 >::SetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT32" );
		SValuedObjectChecks< EFieldType_UINT32 >::SetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT64" );
		SValuedObjectChecks< EFieldType_UINT64 >::SetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_FLOAT32" );
		SValuedObjectChecks< EFieldType_FLOAT32 >::SetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_FLOAT64" );
		SValuedObjectChecks< EFieldType_FLOAT64 >::SetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_FIXED_POINT" );
		SValuedObjectChecks< EFieldType_FIXED_POINT >::SetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_CHAR" );
		SValuedObjectChecks< EFieldType_CHAR >::SetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_VARCHAR" );
		SValuedObjectChecks< EFieldType_VARCHAR >::SetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_TEXT" );
		SValuedObjectChecks< EFieldType_TEXT >::SetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_NCHAR" );
		SValuedObjectChecks< EFieldType_NCHAR >::SetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_NVARCHAR" );
		SValuedObjectChecks< EFieldType_NVARCHAR >::SetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_NTEXT" );
		SValuedObjectChecks< EFieldType_NTEXT >::SetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_DATE" );
		SValuedObjectChecks< EFieldType_DATE >::SetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_DATETIME" );
		SValuedObjectChecks< EFieldType_DATETIME >::SetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_TIME" );
		SValuedObjectChecks< EFieldType_TIME >::SetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_BINARY" );
		SValuedObjectChecks< EFieldType_BINARY >::SetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_VARBINARY" );
		SValuedObjectChecks< EFieldType_VARBINARY >::SetValueFastChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_BLOB" );
		SValuedObjectChecks< EFieldType_BLOB >::SetValueFastChecks( generator );

		CLogger::LogInfo( StringStream() << "**** End TestCase_ParameterSetValueFast ****" );
	}

	void CDatabaseParameterTest::TestCase_ParameterSetValueFromValue()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_ParameterSetValueFromValue ****" );
		std::random_device generator;

		CLogger::LogInfo( StringStream() << "  EFieldType_BIT" );
		SValuedObjectChecks< EFieldType_BIT >::SetValueFromValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT8" );
		SValuedObjectChecks< EFieldType_SINT8 >::SetValueFromValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT16" );
		SValuedObjectChecks< EFieldType_SINT16 >::SetValueFromValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT24" );
		SValuedObjectChecks< EFieldType_SINT24 >::SetValueFromValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT32" );
		SValuedObjectChecks< EFieldType_SINT32 >::SetValueFromValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT64" );
		SValuedObjectChecks< EFieldType_SINT64 >::SetValueFromValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT8" );
		SValuedObjectChecks< EFieldType_UINT8 >::SetValueFromValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT16" );
		SValuedObjectChecks< EFieldType_UINT16 >::SetValueFromValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT24" );
		SValuedObjectChecks< EFieldType_UINT24 >::SetValueFromValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT32" );
		SValuedObjectChecks< EFieldType_UINT32 >::SetValueFromValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT64" );
		SValuedObjectChecks< EFieldType_UINT64 >::SetValueFromValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_FLOAT32" );
		SValuedObjectChecks< EFieldType_FLOAT32 >::SetValueFromValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_FLOAT64" );
		SValuedObjectChecks< EFieldType_FLOAT64 >::SetValueFromValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_FIXED_POINT" );
		SValuedObjectChecks< EFieldType_FIXED_POINT >::SetValueFromValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_CHAR" );
		SValuedObjectChecks< EFieldType_CHAR >::SetValueFromValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_VARCHAR" );
		SValuedObjectChecks< EFieldType_VARCHAR >::SetValueFromValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_TEXT" );
		SValuedObjectChecks< EFieldType_TEXT >::SetValueFromValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_NCHAR" );
		SValuedObjectChecks< EFieldType_NCHAR >::SetValueFromValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_NVARCHAR" );
		SValuedObjectChecks< EFieldType_NVARCHAR >::SetValueFromValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_NTEXT" );
		SValuedObjectChecks< EFieldType_NTEXT >::SetValueFromValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_DATE" );
		SValuedObjectChecks< EFieldType_DATE >::SetValueFromValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_DATETIME" );
		SValuedObjectChecks< EFieldType_DATETIME >::SetValueFromValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_TIME" );
		SValuedObjectChecks< EFieldType_TIME >::SetValueFromValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_BINARY" );
		SValuedObjectChecks< EFieldType_BINARY >::SetValueFromValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_VARBINARY" );
		SValuedObjectChecks< EFieldType_VARBINARY >::SetValueFromValueChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_BLOB" );
		SValuedObjectChecks< EFieldType_BLOB >::SetValueFromValueChecks( generator );

		CLogger::LogInfo( StringStream() << "**** End TestCase_ParameterSetValueFromValue ****" );
	}

	void CDatabaseParameterTest::TestCase_ParameterSetValueFromValuedObject()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_ParameterSetValueFromValuedObject ****" );
		std::random_device generator;

		CLogger::LogInfo( StringStream() << "  EFieldType_BIT" );
		SValuedObjectChecks< EFieldType_BIT >::SetValueFromValuedObjectChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT8" );
		SValuedObjectChecks< EFieldType_SINT8 >::SetValueFromValuedObjectChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT16" );
		SValuedObjectChecks< EFieldType_SINT16 >::SetValueFromValuedObjectChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT24" );
		SValuedObjectChecks< EFieldType_SINT24 >::SetValueFromValuedObjectChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT32" );
		SValuedObjectChecks< EFieldType_SINT32 >::SetValueFromValuedObjectChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_SINT64" );
		SValuedObjectChecks< EFieldType_SINT64 >::SetValueFromValuedObjectChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT8" );
		SValuedObjectChecks< EFieldType_UINT8 >::SetValueFromValuedObjectChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT16" );
		SValuedObjectChecks< EFieldType_UINT16 >::SetValueFromValuedObjectChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT24" );
		SValuedObjectChecks< EFieldType_UINT24 >::SetValueFromValuedObjectChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT32" );
		SValuedObjectChecks< EFieldType_UINT32 >::SetValueFromValuedObjectChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_UINT64" );
		SValuedObjectChecks< EFieldType_UINT64 >::SetValueFromValuedObjectChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_FLOAT32" );
		SValuedObjectChecks< EFieldType_FLOAT32 >::SetValueFromValuedObjectChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_FLOAT64" );
		SValuedObjectChecks< EFieldType_FLOAT64 >::SetValueFromValuedObjectChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_FIXED_POINT" );
		SValuedObjectChecks< EFieldType_FIXED_POINT >::SetValueFromValuedObjectChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_CHAR" );
		SValuedObjectChecks< EFieldType_CHAR >::SetValueFromValuedObjectChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_VARCHAR" );
		SValuedObjectChecks< EFieldType_VARCHAR >::SetValueFromValuedObjectChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_TEXT" );
		SValuedObjectChecks< EFieldType_TEXT >::SetValueFromValuedObjectChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_NCHAR" );
		SValuedObjectChecks< EFieldType_NCHAR >::SetValueFromValuedObjectChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_NVARCHAR" );
		SValuedObjectChecks< EFieldType_NVARCHAR >::SetValueFromValuedObjectChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_NTEXT" );
		SValuedObjectChecks< EFieldType_NTEXT >::SetValueFromValuedObjectChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_DATE" );
		SValuedObjectChecks< EFieldType_DATE >::SetValueFromValuedObjectChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_DATETIME" );
		SValuedObjectChecks< EFieldType_DATETIME >::SetValueFromValuedObjectChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_TIME" );
		SValuedObjectChecks< EFieldType_TIME >::SetValueFromValuedObjectChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_BINARY" );
		SValuedObjectChecks< EFieldType_BINARY >::SetValueFromValuedObjectChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_VARBINARY" );
		SValuedObjectChecks< EFieldType_VARBINARY >::SetValueFromValuedObjectChecks( generator );
		CLogger::LogInfo( StringStream() << "  EFieldType_BLOB" );
		SValuedObjectChecks< EFieldType_BLOB >::SetValueFromValuedObjectChecks( generator );

		CLogger::LogInfo( StringStream() << "**** End TestCase_ParameterSetValueFromValuedObject ****" );
	}
}
END_NAMESPACE_DATABASE_TEST
