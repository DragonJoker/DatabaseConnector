/************************************************************************//**
* @file DatabaseFieldInfosTest.h
* @author Sylvain Doremus
* @version 1.0
* @date 12/02/2014 14:29:35
*
*
* @brief Class testing CDatabaseFieldInfos class
*
***************************************************************************/

#include "DatabaseTestPch.h"

#include "DatabaseFieldInfosTest.h"

#include <DatabaseFieldInfos.h>

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
		template< EFieldType FieldType, typename Enable=void >
		struct FieldInfosChecks
		{
			static void All()
			{
				static const String FieldName = STR( "coin" );
				DatabaseFieldInfosSPtr infos;
				BOOST_CHECK_NO_THROW( infos = std::make_shared< CDatabaseFieldInfos >( FieldName ) );
				BOOST_CHECK( infos );
				BOOST_CHECK( infos->GetName() == FieldName );
				BOOST_CHECK( infos->GetType() == EFieldType_NULL );
				BOOST_CHECK_NO_THROW( infos->SetType( FieldType ) );
				BOOST_CHECK( infos->GetType() == FieldType );
				BOOST_CHECK_NO_THROW( infos = std::make_shared< CDatabaseFieldInfos >( FieldName, FieldType ) );
				BOOST_CHECK( infos->GetType() == FieldType );
				BOOST_CHECK_NO_THROW( infos = std::make_shared< CDatabaseFieldInfos >( FieldName, FieldType, 0 ) );
				BOOST_CHECK( infos->GetType() == FieldType );
				BOOST_CHECK_NO_THROW( infos = std::make_shared< CDatabaseFieldInfos >( FieldName, FieldType, std::make_pair( 0, 0 ) ) );
				BOOST_CHECK( infos->GetType() == FieldType );
			}
		};
		
		template< EFieldType FieldType >
		struct FieldInfosChecks< FieldType, typename std::enable_if< SFieldTypeNeedsLimits< FieldType >::value >::type >
		{
			static void All()
			{
				static const String FieldName = STR( "coin" );
				static const uint32_t FieldLimits = 10;
				DatabaseFieldInfosSPtr infos;
				BOOST_CHECK_NO_THROW( infos = std::make_shared< CDatabaseFieldInfos >( FieldName ) );
				BOOST_CHECK( infos );
				BOOST_CHECK( infos->GetName() == FieldName );
				BOOST_CHECK( infos->GetType() == EFieldType_NULL );
				BOOST_CHECK_THROW( infos->SetType( FieldType ), CDatabaseException );
				BOOST_CHECK_NO_THROW( infos->SetType( FieldType, FieldLimits ) );
				BOOST_CHECK( infos->GetType() == FieldType );
				BOOST_CHECK( infos->GetLimits() == FieldLimits );
				BOOST_CHECK_THROW( infos = std::make_shared< CDatabaseFieldInfos >( FieldName, FieldType ), CDatabaseException );
				BOOST_CHECK_NO_THROW( infos = std::make_shared< CDatabaseFieldInfos >( FieldName, FieldType, FieldLimits ) );
				BOOST_CHECK( infos->GetType() == FieldType );
				BOOST_CHECK( infos->GetLimits() == FieldLimits );
				BOOST_CHECK_NO_THROW( infos = std::make_shared< CDatabaseFieldInfos >( FieldName, FieldType, std::make_pair( FieldLimits, 0 ) ) );
				BOOST_CHECK( infos->GetType() == FieldType );
				BOOST_CHECK( infos->GetLimits() == FieldLimits );
			}
		};
		
		template< EFieldType FieldType >
		struct FieldInfosChecks< FieldType, typename std::enable_if< SFieldTypeNeedsPrecision< FieldType >::value >::type >
		{
			static void All()
			{
				static const String FieldName = STR( "coin" );
				static const std::pair< uint32_t, uint32_t > FieldPrecision = std::make_pair( 10, 5 );
				DatabaseFieldInfosSPtr infos;
				BOOST_CHECK_NO_THROW( infos = std::make_shared< CDatabaseFieldInfos >( FieldName ) );
				BOOST_CHECK( infos );
				BOOST_CHECK( infos->GetName() == FieldName );
				BOOST_CHECK( infos->GetType() == EFieldType_NULL );
				BOOST_CHECK_THROW( infos->SetType( FieldType ), CDatabaseException );
				BOOST_CHECK_THROW( infos->SetType( FieldType, FieldPrecision.first ), CDatabaseException );
				BOOST_CHECK_NO_THROW( infos->SetType( FieldType, FieldPrecision ) );
				BOOST_CHECK( infos->GetType() == FieldType );
				BOOST_CHECK( infos->GetPrecision() == FieldPrecision );
				BOOST_CHECK_THROW( infos = std::make_shared< CDatabaseFieldInfos >( FieldName, FieldType ), CDatabaseException );
				BOOST_CHECK_THROW( infos = std::make_shared< CDatabaseFieldInfos >( FieldName, FieldType, FieldPrecision.first ), CDatabaseException );
				BOOST_CHECK_NO_THROW( infos = std::make_shared< CDatabaseFieldInfos >( FieldName, FieldType, FieldPrecision ) );
				BOOST_CHECK( infos->GetType() == FieldType );
				BOOST_CHECK( infos->GetPrecision() == FieldPrecision );
			}
		};
	}

	CDatabaseFieldInfosTest::CDatabaseFieldInfosTest()
	{
	}

	CDatabaseFieldInfosTest::~CDatabaseFieldInfosTest()
	{
	}

	boost::unit_test::test_suite * CDatabaseFieldInfosTest::Init_Test_Suite()
	{
		//!@remarks Create the internal TS instance.
		testSuite = new boost::unit_test::test_suite( "CDatabaseFieldInfosTest" );

		//!@remarks Add the TC to the internal TS.
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseFieldInfosTest::TestCase_FieldInfos, this ) ) );

		//!@remarks Return the TS instance.
		return testSuite;
	}

	void CDatabaseFieldInfosTest::TestCase_FieldInfos()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_FieldInfos ****" );

		FieldInfosChecks< EFieldType_NULL >::All();
		FieldInfosChecks< EFieldType_BIT >::All();
		FieldInfosChecks< EFieldType_SINT8 >::All();
		FieldInfosChecks< EFieldType_SINT16 >::All();
		FieldInfosChecks< EFieldType_SINT24 >::All();
		FieldInfosChecks< EFieldType_SINT32 >::All();
		FieldInfosChecks< EFieldType_SINT64 >::All();
		FieldInfosChecks< EFieldType_UINT8 >::All();
		FieldInfosChecks< EFieldType_UINT16 >::All();
		FieldInfosChecks< EFieldType_UINT24 >::All();
		FieldInfosChecks< EFieldType_UINT32 >::All();
		FieldInfosChecks< EFieldType_UINT64 >::All();
		FieldInfosChecks< EFieldType_FLOAT32 >::All();
		FieldInfosChecks< EFieldType_FLOAT64 >::All();
		FieldInfosChecks< EFieldType_FIXED_POINT >::All();
		FieldInfosChecks< EFieldType_CHAR >::All();
		FieldInfosChecks< EFieldType_VARCHAR >::All();
		FieldInfosChecks< EFieldType_TEXT >::All();
		FieldInfosChecks< EFieldType_NCHAR >::All();
		FieldInfosChecks< EFieldType_NVARCHAR >::All();
		FieldInfosChecks< EFieldType_NTEXT >::All();
		FieldInfosChecks< EFieldType_DATE >::All();
		FieldInfosChecks< EFieldType_DATETIME >::All();
		FieldInfosChecks< EFieldType_TIME >::All();
		FieldInfosChecks< EFieldType_BINARY >::All();
		FieldInfosChecks< EFieldType_VARBINARY >::All();
		FieldInfosChecks< EFieldType_LONG_VARBINARY >::All();

		CLogger::LogInfo( StringStream() << "**** End TestCase_FieldInfos ****" );
	}
}
END_NAMESPACE_DATABASE_TEST
