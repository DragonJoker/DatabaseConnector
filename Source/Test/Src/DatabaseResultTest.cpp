/************************************************************************//**
 * @file DatabaseResultTest.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 12/02/2014 14:29:35
 *
 *
 * @brief Class testing CDatabaseResult class
*
***************************************************************************/

#include "DatabaseTestPch.h"

#include "DatabaseResultTest.h"

#include "DatabaseTestConnection.h"

#include <DatabaseValuedObjectInfos.h>

BEGIN_NAMESPACE_DATABASE_TEST
{
	extern DatabaseRowSPtr CreateRow( DatabaseConnectionSPtr connection, const DatabaseValuedObjectInfosPtrArray & fieldInfos );
	extern DatabaseValuedObjectInfosPtrArray CreateFieldsInfos();

	CDatabaseResultTest::CDatabaseResultTest()
	{
	}

	CDatabaseResultTest::~CDatabaseResultTest()
	{
	}

	boost::unit_test::test_suite * CDatabaseResultTest::Init_Test_Suite()
	{
		//!@remarks Create the internal TS instance.
		testSuite = new boost::unit_test::test_suite( "CDatabaseResultTest", __FILE__, __LINE__ );

		//!@remarks Add the TC to the internal TS.
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseResultTest::TestCase_ResultFieldsManagement, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseResultTest::TestCase_ResultRowsManagement, this ) ) );

		//!@remarks Return the TS instance.
		return testSuite;
	}

	void CDatabaseResultTest::TestCase_ResultFieldsManagement()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_ResultFieldsManagement ****" );
		String connectionString;
		DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( TEST_GOOD_SERVER, TEST_GOOD_USER, TEST_GOOD_PASSWORD, connectionString );
		connection->SelectDatabase( TEST_GOOD_DATABASE );
		DatabaseValuedObjectInfosPtrArray fieldsInfos = CreateFieldsInfos();
		DatabaseResultSPtr result = std::make_shared< CDatabaseResult >( fieldsInfos );
		BOOST_CHECK_EQUAL( result->GetFieldCount(), uint32_t( EFieldType_COUNT ) );

		for ( int i = EFieldType_BIT; i < EFieldType_COUNT; ++i )
		{
			DatabaseValuedObjectInfosSPtr infos;
			BOOST_CHECK_NO_THROW( infos = result->GetFieldInfos( i - 1 ) );

			if ( i <= EFieldType_FIXED_POINT )
			{
				BOOST_CHECK( infos->GetType() == EFieldType( i ) );
			}
			else
			{
				BOOST_CHECK( infos->GetType() == EFieldType( i - 1 ) );
			}
		}

		BOOST_CHECK_THROW( result->GetFieldInfos( EFieldType_COUNT ), CDatabaseException );

		CLogger::LogInfo( StringStream() << "**** End TestCase_ResultFieldsManagement ****" );
	}

	void CDatabaseResultTest::TestCase_ResultRowsManagement()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_ResultRowsManagement ****" );

		String connectionString;
		DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( TEST_GOOD_SERVER, TEST_GOOD_USER, TEST_GOOD_PASSWORD, connectionString );
		connection->SelectDatabase( TEST_GOOD_DATABASE );
		DatabaseValuedObjectInfosPtrArray fieldsInfos = CreateFieldsInfos();
		DatabaseResultSPtr result = std::make_shared< CDatabaseResult >( fieldsInfos );
		BOOST_CHECK_EQUAL( result->GetRowCount(), 0 );
		auto row1 = CreateRow( connection, fieldsInfos );
		result->AddRow( row1 );
		BOOST_CHECK_EQUAL( result->GetRowCount(), 1 );
		BOOST_CHECK( result->GetFirstRow() );
		auto row2 = CreateRow( connection, fieldsInfos );
		result->AddRow( row2 );
		BOOST_CHECK_EQUAL( result->GetRowCount(), 2 );
		BOOST_CHECK_EQUAL( result->GetFirstRow(), row1 );
		BOOST_CHECK_EQUAL( result->GetNextRow(), row2 );
		BOOST_CHECK_EQUAL( result->GetNextRow(), DatabaseRowSPtr() );
		BOOST_CHECK_EQUAL( result->GetFirstRow(), row1 );

		CLogger::LogInfo( StringStream() << "**** End TestCase_ResultRowsManagement ****" );
	}
}
END_NAMESPACE_DATABASE_TEST
