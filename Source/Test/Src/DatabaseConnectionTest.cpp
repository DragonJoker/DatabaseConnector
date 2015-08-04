/************************************************************************//**
 * @file DatabaseConnectionTest.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 12/02/2014 14:29:35
 *
 *
 * @brief Class testing connection common methods
*
***************************************************************************/

#include "DatabaseTestPch.h"

#include "DatabaseConnectionTest.h"

#include "DatabaseTestUtils.h"
#include "DatabaseTestConnection.h"
#include "DatabaseTestStatement.h"

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
	CDatabaseConnectionTest::CDatabaseConnectionTest()
	{
	}

	CDatabaseConnectionTest::~CDatabaseConnectionTest()
	{
	}

	boost::unit_test::test_suite * CDatabaseConnectionTest::Init_Test_Suite()
	{
		//!@remarks Create the internal TS instance.
		testSuite = new boost::unit_test::test_suite( "CDatabaseConnectionTest" );

		//!@remarks Add the TC to the internal TS.
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseConnectionTest::TestCase_ConnectionChecks, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseConnectionTest::TestCase_DatabaseManagementChecks, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseConnectionTest::TestCase_TransactionChecks, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseConnectionTest::TestCase_QueryStatementChecks, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseConnectionTest::TestCase_StringDataWrite, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseConnectionTest::TestCase_StringDataParse, this ) ) );

		//!@remarks Return the TS instance.
		return testSuite;
	}

	void CDatabaseConnectionTest::TestCase_ConnectionChecks()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_ConnectionChecks ****" );
		String goodserver = TEST_GOOD_SERVER;
		String gooduser = TEST_GOOD_USER;
		String goodpassword = TEST_GOOD_PASSWORD;
		String wrongserver = STR( "WrongServer" );
		String wronguser = STR( "WrongUser" );
		String wrongpassword = STR( "WrongPassword" );
		String connectionString;

		CLogger::LogInfo( StringStream() << "  Wrong server" );
		{
			DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( wrongserver, gooduser, goodpassword, connectionString );
			BOOST_CHECK_EQUAL( connectionString, STR( "server=" ) + wrongserver + STR( ";user=" ) + gooduser + STR( ";password=" ) + goodpassword );
			BOOST_CHECK( !connection->IsConnected() );
		}
		CLogger::LogInfo( StringStream() << "  Wrong user" );
		{
			DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( goodserver, wronguser, goodpassword, connectionString );
			BOOST_CHECK_EQUAL( connectionString, STR( "server=" ) + goodserver + STR( ";user=" ) + wronguser + STR( ";password=" ) + goodpassword );
			BOOST_CHECK( !connection->IsConnected() );
		}
		CLogger::LogInfo( StringStream() << "  Wrong password" );
		{
			DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( goodserver, gooduser, wrongpassword, connectionString );
			BOOST_CHECK_EQUAL( connectionString, STR( "server=" ) + goodserver + STR( ";user=" ) + gooduser + STR( ";password=" ) + wrongpassword );
			BOOST_CHECK( !connection->IsConnected() );
		}

		CLogger::LogInfo( StringStream() << "**** End TestCase_ConnectionChecks ****" );
	}

	void CDatabaseConnectionTest::TestCase_DatabaseManagementChecks()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseManagementChecks ****" );
		String goodserver = TEST_GOOD_SERVER;
		String gooduser = TEST_GOOD_USER;
		String goodpassword = TEST_GOOD_PASSWORD;
		String gooddatabase = TEST_GOOD_DATABASE;
		String wrongserver = STR( "WrongServer" );
		String wronguser = STR( "WrongUser" );
		String wrongpassword = STR( "WrongPassword" );
		String wrongdatabase = STR( "WrongDatabase" );
		String connectionString;

		CLogger::LogInfo( StringStream() << "  Creation of invalid database" );
		{
			DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( goodserver, gooduser, goodpassword, connectionString );
			BOOST_CHECK( !connection->CreateDatabase( wrongdatabase ) );
		}
		CLogger::LogInfo( StringStream() << "  Creation of valid database" );
		{
			DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( goodserver, gooduser, goodpassword, connectionString );
			BOOST_CHECK( connection->CreateDatabase( gooddatabase ) );
		}
		CLogger::LogInfo( StringStream() << "  Selection of invalid database" );
		{
			DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( goodserver, gooduser, goodpassword, connectionString );
			BOOST_CHECK( !connection->SelectDatabase( wrongdatabase ) );
		}
		CLogger::LogInfo( StringStream() << "  Selection of valid database" );
		{
			DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( goodserver, gooduser, goodpassword, connectionString );
			BOOST_CHECK( connection->SelectDatabase( gooddatabase ) );
		}
		CLogger::LogInfo( StringStream() << "  Destruction of invalid database" );
		{
			DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( goodserver, gooduser, goodpassword, connectionString );
			BOOST_CHECK( !connection->DestroyDatabase( wrongdatabase ) );
		}
		CLogger::LogInfo( StringStream() << "  Destruction of valid database" );
		{
			DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( goodserver, gooduser, goodpassword, connectionString );
			BOOST_CHECK( connection->DestroyDatabase( gooddatabase ) );
		}
		CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseManagementChecks ****" );
	}

	void CDatabaseConnectionTest::TestCase_TransactionChecks()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_TransactionChecks ****" );
		String goodserver = TEST_GOOD_SERVER;
		String gooduser = TEST_GOOD_USER;
		String goodpassword = TEST_GOOD_PASSWORD;
		String gooddatabase = TEST_GOOD_DATABASE;
		String wrongserver = STR( "WrongServer" );
		String wronguser = STR( "WrongUser" );
		String wrongpassword = STR( "WrongPassword" );
		String connectionString;
		String wrongtran = STR( "WrongTransaction" );
		String goodtran = STR( "TestTransaction" );

		CLogger::LogInfo( StringStream() << "  Not connected" );
		{
			DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( wrongserver, wronguser, wrongpassword, connectionString );
			BOOST_CHECK_THROW( connection->BeginTransaction( String() ), CDatabaseException );
			BOOST_CHECK_THROW( connection->BeginTransaction( goodtran ), CDatabaseException );
			BOOST_CHECK_THROW( connection->Commit( goodtran ), CDatabaseException );
			BOOST_CHECK_THROW( connection->RollBack( goodtran ), CDatabaseException );
		}
		CLogger::LogInfo( StringStream() << "  Not database selected" );
		{
			DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( goodserver, goodserver, goodserver, connectionString );
			BOOST_CHECK_THROW( connection->BeginTransaction( String() ), CDatabaseException );
			BOOST_CHECK_THROW( connection->BeginTransaction( goodtran ), CDatabaseException );
			BOOST_CHECK_THROW( connection->Commit( goodtran ), CDatabaseException );
			BOOST_CHECK_THROW( connection->RollBack( goodtran ), CDatabaseException );
		}
		CLogger::LogInfo( StringStream() << "  Commit/Rollback without transaction" );
		{
			DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( goodserver, gooduser, goodpassword, connectionString );
			BOOST_CHECK( connection->SelectDatabase( gooddatabase ) );
			BOOST_CHECK_THROW( connection->Commit( goodtran ), CDatabaseException );
			BOOST_CHECK_THROW( connection->RollBack( goodtran ), CDatabaseException );
		}
		CLogger::LogInfo( StringStream() << "  Invalid transaction name" );
		{
			DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( goodserver, gooduser, goodpassword, connectionString );
			BOOST_CHECK( connection->SelectDatabase( gooddatabase ) );
			BOOST_CHECK_THROW( connection->BeginTransaction( wrongtran ), CDatabaseException );
			BOOST_CHECK( !connection->IsInTransaction() );
			BOOST_CHECK( !connection->IsInTransaction( wrongtran ) );
		}
		CLogger::LogInfo( StringStream() << "  Commit/Rollback without corresponding transaction" );
		{
			DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( goodserver, gooduser, goodpassword, connectionString );
			BOOST_CHECK( connection->SelectDatabase( gooddatabase ) );
			BOOST_CHECK_NO_THROW( connection->BeginTransaction( goodtran ) );
			BOOST_CHECK( connection->IsInTransaction() );
			BOOST_CHECK( connection->IsInTransaction( goodtran ) );
			BOOST_CHECK( !connection->IsInTransaction( wrongtran ) );
			BOOST_CHECK_THROW( connection->RollBack( wrongtran ), CDatabaseException );
			BOOST_CHECK_THROW( connection->Commit( wrongtran ), CDatabaseException );
			BOOST_CHECK_NO_THROW( connection->RollBack( goodtran ) );
		}
		CLogger::LogInfo( StringStream() << "  Valid Begin/Rollback" );
		{
			DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( goodserver, gooduser, goodpassword, connectionString );
			BOOST_CHECK( connection->SelectDatabase( gooddatabase ) );
			BOOST_CHECK_NO_THROW( connection->BeginTransaction( goodtran ) );
			BOOST_CHECK( connection->IsInTransaction() );
			BOOST_CHECK( connection->IsInTransaction( goodtran ) );
			BOOST_CHECK_NO_THROW( connection->RollBack( goodtran ) );
			BOOST_CHECK( !connection->IsInTransaction( goodtran ) );
			BOOST_CHECK( !connection->IsInTransaction() );
			CLogger::LogInfo( StringStream() << "    Invalid afterwards Commit/Rollback" );
			BOOST_CHECK_THROW( connection->Commit( goodtran ), CDatabaseException );
			BOOST_CHECK_THROW( connection->RollBack( goodtran ), CDatabaseException );
		}
		CLogger::LogInfo( StringStream() << "  Valid Begin/Commit" );
		{
			DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( goodserver, gooduser, goodpassword, connectionString );
			BOOST_CHECK( connection->SelectDatabase( gooddatabase ) );
			BOOST_CHECK_NO_THROW( connection->BeginTransaction( goodtran ) );
			BOOST_CHECK( connection->IsInTransaction() );
			BOOST_CHECK( connection->IsInTransaction( goodtran ) );
			BOOST_CHECK_NO_THROW( connection->Commit( goodtran ) );
			BOOST_CHECK( !connection->IsInTransaction( goodtran ) );
			BOOST_CHECK( !connection->IsInTransaction() );
			CLogger::LogInfo( StringStream() << "    Invalid afterwards Commit/Rollback" );
			BOOST_CHECK_THROW( connection->RollBack( goodtran ), CDatabaseException );
			BOOST_CHECK_THROW( connection->Commit( goodtran ), CDatabaseException );
		}
		CLogger::LogInfo( StringStream() << "  Multiple transactions" );
		{
			DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( goodserver, gooduser, goodpassword, connectionString );
			BOOST_CHECK( connection->SelectDatabase( gooddatabase ) );
			CLogger::LogInfo( StringStream() << "    Start named one" );
			BOOST_CHECK_NO_THROW( connection->BeginTransaction( goodtran ) );
			BOOST_CHECK( connection->IsInTransaction() );
			BOOST_CHECK( connection->IsInTransaction( goodtran ) );
			BOOST_CHECK( !connection->IsInTransaction( String() ) );
			CLogger::LogInfo( StringStream() << "    Start unnamed one" );
			BOOST_CHECK_NO_THROW( connection->BeginTransaction( String() ) );
			BOOST_CHECK( connection->IsInTransaction() );
			BOOST_CHECK( connection->IsInTransaction( goodtran ) );
			BOOST_CHECK( connection->IsInTransaction( String() ) );
			CLogger::LogInfo( StringStream() << "    Commit named one" );
			BOOST_CHECK_NO_THROW( connection->Commit( goodtran ) );
			BOOST_CHECK( !connection->IsInTransaction( goodtran ) );
			BOOST_CHECK( connection->IsInTransaction( String() ) );
			BOOST_CHECK( connection->IsInTransaction() );
			CLogger::LogInfo( StringStream() << "    Restart named one" );
			BOOST_CHECK_NO_THROW( connection->BeginTransaction( goodtran ) );
			BOOST_CHECK( connection->IsInTransaction() );
			BOOST_CHECK( connection->IsInTransaction( goodtran ) );
			BOOST_CHECK( connection->IsInTransaction( String() ) );
			CLogger::LogInfo( StringStream() << "    RollBack named one" );
			BOOST_CHECK_NO_THROW( connection->RollBack( goodtran ) );
			BOOST_CHECK( !connection->IsInTransaction( goodtran ) );
			BOOST_CHECK( connection->IsInTransaction( String() ) );
			BOOST_CHECK( connection->IsInTransaction() );
			CLogger::LogInfo( StringStream() << "    Commit unnamed one" );
			BOOST_CHECK_NO_THROW( connection->Commit( String() ) );
			BOOST_CHECK( !connection->IsInTransaction( goodtran ) );
			BOOST_CHECK( !connection->IsInTransaction( String() ) );
			BOOST_CHECK( !connection->IsInTransaction() );
			CLogger::LogInfo( StringStream() << "    Invalid afterwards Commit/Rollback" );
			BOOST_CHECK_THROW( connection->RollBack( goodtran ), CDatabaseException );
			BOOST_CHECK_THROW( connection->Commit( goodtran ), CDatabaseException );
			BOOST_CHECK_THROW( connection->RollBack( String() ), CDatabaseException );
			BOOST_CHECK_THROW( connection->Commit( String() ), CDatabaseException );
		}

		CLogger::LogInfo( StringStream() << "**** End TestCase_TransactionChecks ****" );
	}

	void CDatabaseConnectionTest::TestCase_QueryStatementChecks()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_QueryStatementChecks ****" );
		String goodserver = TEST_GOOD_SERVER;
		String gooduser = TEST_GOOD_USER;
		String goodpassword = TEST_GOOD_PASSWORD;
		String gooddatabase = TEST_GOOD_DATABASE;
		String goodselect = TEST_GOOD_SELECT;
		String goodupdate = TEST_GOOD_UPDATE;
		String wrongserver = STR( "WrongServer" );
		String wronguser = STR( "WrongUser" );
		String wrongpassword = STR( "WrongPassword" );
		String connectionString;
		String wrongquery = STR( "WrongQuery" );

		CLogger::LogInfo( StringStream() << "  Not connected" );
		{
			DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( wrongserver, wronguser, wrongpassword, connectionString );
			BOOST_CHECK_THROW( connection->CreateQuery( goodupdate ), CDatabaseException );
			BOOST_CHECK_THROW( connection->CreateStatement( goodupdate ), CDatabaseException );
			BOOST_CHECK_THROW( connection->ExecuteSelect( goodselect ), CDatabaseException );
			BOOST_CHECK_THROW( connection->ExecuteUpdate( goodupdate ), CDatabaseException );
		}
		CLogger::LogInfo( StringStream() << "  Not database selected" );
		{
			DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( goodserver, goodserver, goodserver, connectionString );
			BOOST_CHECK_THROW( connection->CreateQuery( goodupdate ), CDatabaseException );
			BOOST_CHECK_THROW( connection->CreateStatement( goodupdate ), CDatabaseException );
			BOOST_CHECK_THROW( connection->ExecuteSelect( goodselect ), CDatabaseException );
			BOOST_CHECK_THROW( connection->ExecuteUpdate( goodupdate ), CDatabaseException );
		}
		CLogger::LogInfo( StringStream() << "  Execute queries direct" );
		{
			DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( goodserver, gooduser, goodpassword, connectionString );
			BOOST_CHECK( connection->SelectDatabase( gooddatabase ) );
			BOOST_CHECK( connection->ExecuteSelect( goodselect ) );
			BOOST_CHECK( !connection->ExecuteSelect( goodupdate ) );
			BOOST_CHECK( !connection->ExecuteUpdate( goodselect ) );
			BOOST_CHECK( connection->ExecuteUpdate( goodupdate ) );
		}
		CLogger::LogInfo( StringStream() << "  Query creation and execution" );
		{
			DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( goodserver, gooduser, goodpassword, connectionString );
			BOOST_CHECK( connection->SelectDatabase( gooddatabase ) );
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
		}
		CLogger::LogInfo( StringStream() << "  Statement creation and execution" );
		{
			DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( goodserver, gooduser, goodpassword, connectionString );
			BOOST_CHECK( connection->SelectDatabase( gooddatabase ) );
			DatabaseStatementSPtr statement;
			BOOST_CHECK_NO_THROW( statement = connection->CreateStatement( wrongquery ) );
			BOOST_CHECK_THROW( statement->ExecuteSelect(), CDatabaseException );
			BOOST_CHECK_THROW( statement->ExecuteUpdate(), CDatabaseException );
			BOOST_CHECK( statement->Initialise() == EErrorType_NONE );
			BOOST_CHECK( !statement->ExecuteSelect() );
			BOOST_CHECK( !statement->ExecuteUpdate() );
			BOOST_CHECK_NO_THROW( statement = connection->CreateStatement( goodselect ) );
			BOOST_CHECK( statement->Initialise() == EErrorType_NONE );
			BOOST_CHECK( statement->ExecuteSelect() );
			BOOST_CHECK( !statement->ExecuteUpdate() );
			BOOST_CHECK_NO_THROW( statement = connection->CreateStatement( goodupdate ) );
			BOOST_CHECK( statement->Initialise() == EErrorType_NONE );
			BOOST_CHECK( !statement->ExecuteSelect() );
			BOOST_CHECK( statement->ExecuteUpdate() );
		}

		CLogger::LogInfo( StringStream() << "**** End TestCase_QueryStatementChecks ****" );
	}

	void CDatabaseConnectionTest::TestCase_StringDataWrite()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_StringDataWrite ****" );
		String goodserver = TEST_GOOD_SERVER;
		String gooduser = TEST_GOOD_USER;
		String goodpassword = TEST_GOOD_PASSWORD;
		String wrongserver = STR( "WrongServer" );
		String wronguser = STR( "WrongUser" );
		String wrongpassword = STR( "WrongPassword" );
		String connectionString;

		CLogger::LogInfo( StringStream() << "  Not connected" );
		{
			DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( wrongserver, wronguser, wrongpassword, connectionString );
			BOOST_CHECK_THROW( connection->WriteName( String() ), CDatabaseException );
			BOOST_CHECK_THROW( connection->WriteText( std::string() ), CDatabaseException );
			BOOST_CHECK_THROW( connection->WriteNText( std::wstring() ), CDatabaseException );
			BOOST_CHECK_THROW( connection->WriteBool( false ), CDatabaseException );
			BOOST_CHECK_THROW( connection->WriteDate( DateType() ), CDatabaseException );
			BOOST_CHECK_THROW( connection->WriteTime( TimeType() ), CDatabaseException );
			BOOST_CHECK_THROW( connection->WriteDateTime( DateType() ), CDatabaseException );
			BOOST_CHECK_THROW( connection->WriteDateTime( TimeType() ), CDatabaseException );
			BOOST_CHECK_THROW( connection->WriteDateTime( DateTimeType() ), CDatabaseException );
			BOOST_CHECK_THROW( connection->WriteStmtDate( DateType() ), CDatabaseException );
			BOOST_CHECK_THROW( connection->WriteStmtTime( TimeType() ), CDatabaseException );
			BOOST_CHECK_THROW( connection->WriteStmtDateTime( DateTimeType() ), CDatabaseException );
		}
		CLogger::LogInfo( StringStream() << "  Name/Text/NText/Bool" );
		{
			DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( goodserver, gooduser, goodpassword, connectionString );
			String name = STR( "name" );
			std::string text = "text";
			std::wstring ntxt = L"ntext";
			String rname;
			std::string rtext;
			std::wstring rntxt;
			String rbool;
			BOOST_CHECK_NO_THROW( rname = connection->WriteName( name ) );
			BOOST_CHECK_EQUAL( rname, STR( "[" ) + name + STR( "]" ) );
			BOOST_CHECK_NO_THROW( rtext = connection->WriteText( text ) );
			BOOST_CHECK_EQUAL( rtext, STR( "'" ) + text + STR( "'" ) );
			BOOST_CHECK_NO_THROW( rntxt = connection->WriteNText( ntxt ) );
			BOOST_CHECK_EQUAL( rntxt, L"N'" + ntxt + L"'" );
			BOOST_CHECK_NO_THROW( rbool = connection->WriteBool( false ) );
			BOOST_CHECK_EQUAL( rbool, STR( "0" ) );
		}
		CLogger::LogInfo( StringStream() << "  Date/Time/DateTime" );
		{
			DateType wrongdate;
			TimeType wrongtime( -1, -1, -1 );
			DateTimeType wrongdateTime( wrongdate, wrongtime );
			DateType gooddate( 2015, 1, 1 );
			TimeType goodtime( 1, 1, 1 );
			DateTimeType gooddateTime( gooddate, goodtime );
			String result;
			DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( goodserver, gooduser, goodpassword, connectionString );
			CLogger::LogInfo( StringStream() << "    Invalid values" );
			{
				BOOST_CHECK_NO_THROW( result = connection->WriteDate( wrongdate ) );
				BOOST_CHECK_EQUAL( result, TEST_SQL_NULL );
				BOOST_CHECK_NO_THROW( result = connection->WriteTime( wrongtime ) );
				BOOST_CHECK_EQUAL( result, TEST_SQL_NULL );
				BOOST_CHECK_NO_THROW( result = connection->WriteDateTime( wrongdate ) );
				BOOST_CHECK_EQUAL( result, TEST_SQL_NULL );
				BOOST_CHECK_NO_THROW( result = connection->WriteDateTime( wrongtime ) );
				BOOST_CHECK_EQUAL( result, TEST_SQL_NULL );
				BOOST_CHECK_NO_THROW( result = connection->WriteDateTime( wrongdateTime ) );
				BOOST_CHECK_EQUAL( result, TEST_SQL_NULL );
				BOOST_CHECK_NO_THROW( result = connection->WriteStmtDate( wrongdate ) );
				BOOST_CHECK_EQUAL( result, TEST_SQL_NULL );
				BOOST_CHECK_NO_THROW( result = connection->WriteStmtTime( wrongtime ) );
				BOOST_CHECK_EQUAL( result, TEST_SQL_NULL );
				BOOST_CHECK_NO_THROW( result = connection->WriteStmtDateTime( wrongdateTime ) );
				BOOST_CHECK_EQUAL( result, TEST_SQL_NULL );
			}
			CLogger::LogInfo( StringStream() << "    Valid values" );
			{
				BOOST_CHECK_NO_THROW( result = connection->WriteDate( gooddate ) );
				BOOST_CHECK_EQUAL( result, Date::Format( gooddate, TEST_FORMAT_DATE ) );
				BOOST_CHECK_NO_THROW( result = connection->WriteTime( goodtime ) );
				BOOST_CHECK_EQUAL( result, Time::Format( goodtime, TEST_FORMAT_TIME ) );
				BOOST_CHECK_NO_THROW( result = connection->WriteDateTime( gooddate ) );
				BOOST_CHECK_EQUAL( result, Date::Format( gooddate, TEST_FORMAT_DATETIME_DATE ) );
				BOOST_CHECK_NO_THROW( result = connection->WriteDateTime( goodtime ) );
				BOOST_CHECK_EQUAL( result, Time::Format( goodtime, TEST_FORMAT_DATETIME_TIME ) );
				BOOST_CHECK_NO_THROW( result = connection->WriteDateTime( gooddateTime ) );
				BOOST_CHECK_EQUAL( result, DateTime::Format( gooddateTime, TEST_FORMAT_DATETIME ) );
				BOOST_CHECK_NO_THROW( result = connection->WriteStmtDate( gooddate ) );
				BOOST_CHECK_EQUAL( result, Date::Format( gooddate, TEST_FORMAT_STMT_DATE ) );
				BOOST_CHECK_NO_THROW( result = connection->WriteStmtTime( goodtime ) );
				BOOST_CHECK_EQUAL( result, Time::Format( goodtime, TEST_FORMAT_STMT_TIME ) );
				BOOST_CHECK_NO_THROW( result = connection->WriteStmtDateTime( gooddateTime ) );
				BOOST_CHECK_EQUAL( result, DateTime::Format( gooddateTime, TEST_FORMAT_STMT_DATETIME ) );
			}
		}

		CLogger::LogInfo( StringStream() << "**** End TestCase_StringDataWrite ****" );
	}

	void CDatabaseConnectionTest::TestCase_StringDataParse()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_StringDataParse ****" );
		String goodserver = TEST_GOOD_SERVER;
		String gooduser = TEST_GOOD_USER;
		String goodpassword = TEST_GOOD_PASSWORD;
		String wrongserver = STR( "WrongServer" );
		String wronguser = STR( "WrongUser" );
		String wrongpassword = STR( "WrongPassword" );
		String connectionString;

		String gooddate = Date::Format( DateType( 2015, 1, 1 ), TEST_FORMAT_DATE );
		String goodtime = Time::Format( TimeType( 1, 1, 1 ), TEST_FORMAT_TIME );
		String gooddatetime = DateTime::Format( DateTimeType( DateType( 2015, 1, 1 ), TimeType( 1, 1, 1 ) ), TEST_FORMAT_DATETIME );
		String wrongdate;
		String wrongtime;
		String wrongdatetime;
		CLogger::LogInfo( StringStream() << "    Not connected" );
		{
			DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( wrongserver, wronguser, wrongpassword, connectionString );
			BOOST_CHECK_THROW( connection->ParseDate( gooddate ), CDatabaseException );
			BOOST_CHECK_THROW( connection->ParseTime( goodtime ), CDatabaseException );
			BOOST_CHECK_THROW( connection->ParseDateTime( gooddatetime ), CDatabaseException );
		}
		CLogger::LogInfo( StringStream() << "    Invalid values" );
		{
			DateType date;
			TimeType time;
			DateTimeType dateTime;
			DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( goodserver, gooduser, goodpassword, connectionString );
			BOOST_CHECK_NO_THROW( date = connection->ParseDate( wrongdate ) );
			BOOST_CHECK( !Date::IsValid( date ) );
			BOOST_CHECK_NO_THROW( time = connection->ParseTime( wrongtime ) );
			BOOST_CHECK( !Time::IsValid( time ) );
			BOOST_CHECK_NO_THROW( dateTime = connection->ParseDateTime( wrongdatetime ) );
			BOOST_CHECK( !DateTime::IsValid( dateTime ) );
		}
		CLogger::LogInfo( StringStream() << "    Valid values" );
		{
			DateType date;
			TimeType time;
			DateTimeType dateTime;
			DatabaseConnectionSPtr connection = std::make_shared< CDatabaseTestConnection >( goodserver, gooduser, goodpassword, connectionString );
			BOOST_CHECK_NO_THROW( date = connection->ParseDate( gooddate ) );
			BOOST_CHECK( Date::IsValid( date ) );
			BOOST_CHECK_NO_THROW( time = connection->ParseTime( goodtime ) );
			BOOST_CHECK( Time::IsValid( time ) );
			BOOST_CHECK_NO_THROW( dateTime = connection->ParseDateTime( gooddatetime ) );
			BOOST_CHECK( DateTime::IsValid( dateTime ) );
		}

		CLogger::LogInfo( StringStream() << "**** End TestCase_StringDataParse ****" );
	}
}
END_NAMESPACE_DATABASE_TEST
