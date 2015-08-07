/************************************************************************//**
 * @file DatabaseStatementTest.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 12/02/2014 14:29:35
 *
 *
 * @brief Class testing CDatabaseStatement class
*
***************************************************************************/

#include "DatabaseTestPch.h"

#if defined( TESTING_INTERFACE )

#include "DatabaseStatementTest.h"

#include "DatabaseTestConnection.h"
#include "DatabaseTestStatement.h"

#include <DatabaseStatement.h>

BEGIN_NAMESPACE_DATABASE_TEST
{
	CDatabaseStatementTest::CDatabaseStatementTest()
	{
	}

	CDatabaseStatementTest::~CDatabaseStatementTest()
	{
	}

	boost::unit_test::test_suite * CDatabaseStatementTest::Init_Test_Suite()
	{
		//!@remarks Create the internal TS instance.
		testSuite = new boost::unit_test::test_suite( "CDatabaseStatementTest" );

		//!@remarks Add the TC to the internal TS.
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseStatementTest::TestCase_StatementInitialisation, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseStatementTest::TestCase_StatementParameterManagement, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseStatementTest::TestCase_StatementExecution, this ) ) );

		//!@remarks Return the TS instance.
		return testSuite;
	}

	void CDatabaseStatementTest::TestCase_StatementInitialisation()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_StatementInitialisation ****" );
		String connectionString;
		DatabaseTestConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( TEST_GOOD_SERVER, TEST_GOOD_USER, TEST_GOOD_PASSWORD, connectionString );
		connection->SelectDatabase( TEST_GOOD_DATABASE );

		CLogger::LogInfo( StringStream() << "  Empty query" );
		{
			CDatabaseTestStatement query( connection, String() );
			BOOST_CHECK( query.Initialise() == EErrorType_ERROR );
		}
		CLogger::LogInfo( StringStream() << "  Wrong number of parameters" );
		{
			CDatabaseTestStatement query( connection, STR( "TestParam ?;" ) );
			BOOST_CHECK( query.Initialise() == EErrorType_ERROR );
			query.CreateParameter( STR( "Param1" ), EFieldType_BIT );
			query.CreateParameter( STR( "Param2" ), EFieldType_BIT );
			BOOST_CHECK( query.Initialise() == EErrorType_ERROR );
		}
		CLogger::LogInfo( StringStream() << "  Invalid query string" );
		{
			CDatabaseTestStatement query( connection, STR( "??" ) );
			query.CreateParameter( STR( "Param1" ), EFieldType_BIT );
			query.CreateParameter( STR( "Param2" ), EFieldType_BIT );
			BOOST_CHECK( query.Initialise() == EErrorType_ERROR );
		}
		CLogger::LogInfo( StringStream() << "  Optimal case" );
		{
			CDatabaseTestStatement query( connection, STR( "TestParam ?;" ) );
			query.CreateParameter( STR( "Param1" ), EFieldType_BIT );
			BOOST_CHECK( query.Initialise() == EErrorType_NONE );
		}

		CLogger::LogInfo( StringStream() << "**** End TestCase_StatementInitialisation ****" );
	}

	void CDatabaseStatementTest::TestCase_StatementParameterManagement()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_StatementParameterManagement ****" );
		String connectionString;
		DatabaseTestConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( TEST_GOOD_SERVER, TEST_GOOD_USER, TEST_GOOD_PASSWORD, connectionString );
		connection->SelectDatabase( TEST_GOOD_DATABASE );

		CDatabaseTestStatement query( connection, STR( "TestParams ?, ?;" ) );
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

		CLogger::LogInfo( StringStream() << "**** End TestCase_StatementParameterManagement ****" );
	}

	void CDatabaseStatementTest::TestCase_StatementExecution()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_StatementExecution ****" );
		String connectionString;
		DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( TEST_GOOD_SERVER, TEST_GOOD_USER, TEST_GOOD_PASSWORD, connectionString );
		connection->SelectDatabase( TEST_GOOD_DATABASE );
		String goodselect = TEST_GOOD_SELECT;
		String goodupdate = TEST_GOOD_UPDATE;
		String wrongquery = STR( "WrongQuery" );

		DatabaseStatementSPtr query;
		BOOST_CHECK_NO_THROW( query = connection->CreateStatement( wrongquery ) );
		BOOST_CHECK_THROW( query->ExecuteSelect(), CDatabaseException );
		BOOST_CHECK_THROW( query->ExecuteUpdate(), CDatabaseException );
		BOOST_CHECK( query->Initialise() == EErrorType_NONE );
		BOOST_CHECK( !query->ExecuteSelect() );
		BOOST_CHECK( !query->ExecuteUpdate() );
		BOOST_CHECK_NO_THROW( query = connection->CreateStatement( goodselect ) );
		BOOST_CHECK( query->Initialise() == EErrorType_NONE );
		BOOST_CHECK( query->ExecuteSelect() );
		BOOST_CHECK( !query->ExecuteUpdate() );
		BOOST_CHECK_NO_THROW( query = connection->CreateStatement( goodupdate ) );
		BOOST_CHECK( query->Initialise() == EErrorType_NONE );
		BOOST_CHECK( !query->ExecuteSelect() );
		BOOST_CHECK( query->ExecuteUpdate() );

		CLogger::LogInfo( StringStream() << "**** End TestCase_StatementExecution ****" );
	}
}
END_NAMESPACE_DATABASE_TEST

#endif
