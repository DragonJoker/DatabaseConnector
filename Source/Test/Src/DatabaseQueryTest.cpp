/************************************************************************//**
 * @file DatabaseQueryTest.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 12/02/2014 14:29:35
 *
 *
 * @brief Class testing CDatabaseQuery class
*
***************************************************************************/

#include "DatabaseTestPch.h"

#include "DatabaseQueryTest.h"

#include "DatabaseTestConnection.h"

#include <DatabaseQuery.h>

BEGIN_NAMESPACE_DATABASE_TEST
{
	CDatabaseQueryTest::CDatabaseQueryTest()
	{
	}

	CDatabaseQueryTest::~CDatabaseQueryTest()
	{
	}

	boost::unit_test::test_suite * CDatabaseQueryTest::Init_Test_Suite()
	{
		//!@remarks Create the internal TS instance.
		testSuite = new boost::unit_test::test_suite( "CDatabaseQueryTest" );

		//!@remarks Add the TC to the internal TS.
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseQueryTest::TestCase_QueryInitialisation, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseQueryTest::TestCase_QueryParameterManagement, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseQueryTest::TestCase_QueryExecution, this ) ) );

		//!@remarks Return the TS instance.
		return testSuite;
	}

	void CDatabaseQueryTest::TestCase_QueryInitialisation()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_QueryInitialisation ****" );
		String connectionString;
		DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( TEST_GOOD_SERVER, TEST_GOOD_USER, TEST_GOOD_PASSWORD, connectionString );
		connection->SelectDatabase( TEST_GOOD_DATABASE );

		CLogger::LogInfo( StringStream() << "  Empty query" );
		{
			CDatabaseQuery query( connection, String() );
			BOOST_CHECK( query.Initialise() == EErrorType_ERROR );
		}
		CLogger::LogInfo( StringStream() << "  Wrong number of parameters" );
		{
			CDatabaseQuery query( connection, STR( "TestParam ?;" ) );
			BOOST_CHECK( query.Initialise() == EErrorType_ERROR );
			query.CreateParameter( STR( "Param1" ), EFieldType_BIT );
			query.CreateParameter( STR( "Param2" ), EFieldType_BIT );
			BOOST_CHECK( query.Initialise() == EErrorType_ERROR );
		}
		CLogger::LogInfo( StringStream() << "  Invalid query string" );
		{
			CDatabaseQuery query( connection, STR( "??" ) );
			query.CreateParameter( STR( "Param1" ), EFieldType_BIT );
			query.CreateParameter( STR( "Param2" ), EFieldType_BIT );
			BOOST_CHECK( query.Initialise() == EErrorType_ERROR );
		}
		CLogger::LogInfo( StringStream() << "  Optimal case" );
		{
			CDatabaseQuery query( connection, STR( "TestParam ?;" ) );
			query.CreateParameter( STR( "Param1" ), EFieldType_BIT );
			BOOST_CHECK( query.Initialise() == EErrorType_NONE );
		}

		CLogger::LogInfo( StringStream() << "**** End TestCase_QueryInitialisation ****" );
	}

	void CDatabaseQueryTest::TestCase_QueryParameterManagement()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_QueryParameterManagement ****" );
		String connectionString;
		DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( TEST_GOOD_SERVER, TEST_GOOD_USER, TEST_GOOD_PASSWORD, connectionString );
		connection->SelectDatabase( TEST_GOOD_DATABASE );

		CDatabaseQuery query( connection, STR( "TestParams ?, ?;" ) );
		BOOST_CHECK_NO_THROW( query.CreateParameter( STR( "Param1" ), EFieldType_BIT ) );
		BOOST_CHECK_THROW( query.CreateParameter( STR( "Param1" ), EFieldType_BLOB ), CDatabaseException );

		BOOST_CHECK_THROW( query.GetParameter( STR( "Param2" ) ), CDatabaseException );
		BOOST_CHECK_THROW( query.GetParameter( 1 ), CDatabaseException );
		BOOST_CHECK_THROW( query.GetParameterType( STR( "Param2" ) ), CDatabaseException );
		BOOST_CHECK_THROW( query.GetParameterType( 1 ), CDatabaseException );
		BOOST_CHECK_THROW( query.SetParameterNull( STR( "Param2" ) ), CDatabaseException );
		BOOST_CHECK_THROW( query.SetParameterNull( 1 ), CDatabaseException );
		BOOST_CHECK_THROW( query.SetParameterValue( STR( "Param2" ), false ), CDatabaseException );
		BOOST_CHECK_THROW( query.SetParameterValue( 1, false ), CDatabaseException );
		BOOST_CHECK_THROW( query.SetParameterValueFast( STR( "Param2" ), false ), CDatabaseException );
		BOOST_CHECK_THROW( query.SetParameterValueFast( 1, false ), CDatabaseException );
		BOOST_CHECK_THROW( query.SetParameterValue( STR( "Param2" ), static_cast< const CDatabaseValuedObject & >( *query.GetParameter( STR( "Param1" ) ) ) ), CDatabaseException );
		BOOST_CHECK_THROW( query.SetParameterValue( 1, static_cast< const CDatabaseValuedObject & >( *query.GetParameter( 0 ) ) ), CDatabaseException );

		BOOST_CHECK_NO_THROW( query.GetParameter( STR( "Param1" ) ) );
		BOOST_CHECK_NO_THROW( query.GetParameter( 0 ) );
		BOOST_CHECK( query.GetParameterType( STR( "Param1" ) ) == EFieldType_BIT );
		BOOST_CHECK( query.GetParameterType( 0 ) == EFieldType_BIT );
		BOOST_CHECK_NO_THROW( query.SetParameterNull( STR( "Param1" ) ) );
		BOOST_CHECK_NO_THROW( query.SetParameterNull( 0 ) );
		BOOST_CHECK_NO_THROW( query.SetParameterValue( STR( "Param1" ), false ) );
		BOOST_CHECK_NO_THROW( query.SetParameterValue( 0, false ) );
		BOOST_CHECK_NO_THROW( query.SetParameterValueFast( STR( "Param1" ), false ) );
		BOOST_CHECK_NO_THROW( query.SetParameterValueFast( 0, false ) );
		BOOST_CHECK_NO_THROW( query.SetParameterValue( STR( "Param1" ), static_cast< const CDatabaseValuedObject & >( *query.GetParameter( 0 ) ) ) );
		BOOST_CHECK_NO_THROW( query.SetParameterValue( 0, static_cast< const CDatabaseValuedObject & >( *query.GetParameter( 0 ) ) ) );

		BOOST_CHECK_NO_THROW( query.CreateParameter( STR( "Param2" ), EFieldType_BIT ) );
		BOOST_CHECK_NO_THROW( query.GetParameter( STR( "Param2" ) ) );
		BOOST_CHECK_NO_THROW( query.GetParameter( 1 ) );
		BOOST_CHECK( query.GetParameterType( STR( "Param2" ) ) == EFieldType_BIT );
		BOOST_CHECK( query.GetParameterType( 1 ) == EFieldType_BIT );
		BOOST_CHECK_NO_THROW( query.SetParameterNull( STR( "Param2" ) ) );
		BOOST_CHECK_NO_THROW( query.SetParameterNull( 1 ) );
		BOOST_CHECK_NO_THROW( query.SetParameterValue( STR( "Param2" ), false ) );
		BOOST_CHECK_NO_THROW( query.SetParameterValue( 1, false ) );
		BOOST_CHECK_NO_THROW( query.SetParameterValueFast( STR( "Param2" ), false ) );
		BOOST_CHECK_NO_THROW( query.SetParameterValueFast( 1, false ) );
		BOOST_CHECK_NO_THROW( query.SetParameterValue( STR( "Param2" ), static_cast< const CDatabaseValuedObject & >( *query.GetParameter( STR( "Param1" ) ) ) ) );
		BOOST_CHECK_NO_THROW( query.SetParameterValue( 1, static_cast< const CDatabaseValuedObject & >( *query.GetParameter( 0 ) ) ) );

		BOOST_CHECK( query.Initialise() == EErrorType_NONE );

		CLogger::LogInfo( StringStream() << "**** End TestCase_QueryParameterManagement ****" );
	}

	void CDatabaseQueryTest::TestCase_QueryExecution()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_QueryExecution ****" );
		String connectionString;
		DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( TEST_GOOD_SERVER, TEST_GOOD_USER, TEST_GOOD_PASSWORD, connectionString );
		connection->SelectDatabase( TEST_GOOD_DATABASE );
		String goodselect = TEST_GOOD_SELECT;
		String goodupdate = TEST_GOOD_UPDATE;
		String wrongquery = STR( "WrongQuery" );

		DatabaseQuerySPtr query;
		BOOST_CHECK_NO_THROW( query = connection->CreateQuery( wrongquery ) );
		BOOST_CHECK_THROW( query->ExecuteSelect(), CDatabaseException );
		BOOST_CHECK_THROW( query->ExecuteUpdate(), CDatabaseException );
		BOOST_CHECK( query->Initialise() == EErrorType_NONE );
		BOOST_CHECK( !query->ExecuteSelect() );
		BOOST_CHECK( !query->ExecuteUpdate() );
		BOOST_CHECK_NO_THROW( query = connection->CreateQuery( goodselect ) );
		BOOST_CHECK( query->Initialise() == EErrorType_NONE );
		BOOST_CHECK( query->ExecuteSelect() );
		BOOST_CHECK( !query->ExecuteUpdate() );
		BOOST_CHECK_NO_THROW( query = connection->CreateQuery( goodupdate ) );
		BOOST_CHECK( query->Initialise() == EErrorType_NONE );
		BOOST_CHECK( !query->ExecuteSelect() );
		BOOST_CHECK( query->ExecuteUpdate() );

		CLogger::LogInfo( StringStream() << "**** End TestCase_QueryExecution ****" );
	}
}
END_NAMESPACE_DATABASE_TEST
