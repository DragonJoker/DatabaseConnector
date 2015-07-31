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

BEGIN_NAMESPACE_DATABASE_TEST
{
	template< EFieldType ValueType > struct ValueChecks
	{
		static void Copy()
		{
			String connectionString;
			DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( TEST_GOOD_SERVER, TEST_GOOD_USER, TEST_GOOD_PASSWORD, connectionString );
			BOOST_CHECK( connection->SelectDatabase( TEST_GOOD_DATABASE ) );
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

		//!@remarks Return the TS instance.
		return testSuite;
	}

	void CDatabaseValueTest::TestCase_ValueCopy()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_ValueCopy ****" );

		ValueChecks< EFieldType_NULL >::Copy();
		ValueChecks< EFieldType_BIT >::Copy();
		ValueChecks< EFieldType_SINT8 >::Copy();
		ValueChecks< EFieldType_SINT16 >::Copy();
		ValueChecks< EFieldType_SINT24 >::Copy();
		ValueChecks< EFieldType_SINT32 >::Copy();
		ValueChecks< EFieldType_SINT64 >::Copy();
		ValueChecks< EFieldType_UINT8 >::Copy();
		ValueChecks< EFieldType_UINT16 >::Copy();
		ValueChecks< EFieldType_UINT24 >::Copy();
		ValueChecks< EFieldType_UINT32 >::Copy();
		ValueChecks< EFieldType_UINT64 >::Copy();
		ValueChecks< EFieldType_FLOAT32 >::Copy();
		ValueChecks< EFieldType_FLOAT64 >::Copy();
		ValueChecks< EFieldType_FIXED_POINT >::Copy();
		ValueChecks< EFieldType_CHAR >::Copy();
		ValueChecks< EFieldType_VARCHAR >::Copy();
		ValueChecks< EFieldType_TEXT >::Copy();
		ValueChecks< EFieldType_NCHAR >::Copy();
		ValueChecks< EFieldType_NVARCHAR >::Copy();
		ValueChecks< EFieldType_NTEXT >::Copy();
		ValueChecks< EFieldType_DATE >::Copy();
		ValueChecks< EFieldType_DATETIME >::Copy();
		ValueChecks< EFieldType_TIME >::Copy();
		ValueChecks< EFieldType_BINARY >::Copy();
		ValueChecks< EFieldType_VARBINARY >::Copy();
		ValueChecks< EFieldType_LONG_VARBINARY >::Copy();

		CLogger::LogInfo( StringStream() << "**** End TestCase_ValueCopy ****" );
	}
}
END_NAMESPACE_DATABASE_TEST
