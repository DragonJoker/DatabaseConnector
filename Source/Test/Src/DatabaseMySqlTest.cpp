/************************************************************************//**
 * @file DatabaseMySqlTest.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 16/07/2014 12:08:57
 *
 *
 * @brief Database MySQL plugin test
 *
 * @details Database MySQL plugin test
 *
 ***************************************************************************/

#include "DatabaseTestPch.h"

#include "DatabaseMySqlTest.h"
#include "DatabaseTestUtilsUtf8.h"

#include <Database.h>
#include <DatabaseResult.h>
#include <DatabaseRow.h>
#include <DatabaseDateTime.h>

#include <DatabaseFactoryManager.h>
#include <DatabaseStringUtils.h>

BEGIN_NAMESPACE_DATABASE_TEST
{
	static const String DB_DATABASE = STR( "db_test_mysql" );
	static const String DB_SERVER = STR( "tcp://127.0.0.1:3306" );
	static const String DB_USER = STR( "ariaUser" );
	static const String DB_PASS = STR( "ariaPwd" );

	static const String DATABASE_MYSQL_TYPE = STR( "Database.MySql" );

	CDatabaseMySqlTest::CDatabaseMySqlTest()
	{
	}

	CDatabaseMySqlTest::~CDatabaseMySqlTest()
	{
	}

	boost::unit_test::test_suite * CDatabaseMySqlTest::Init_Test_Suite()
	{
		///@remarks Create the internal TS instance.
		testSuite = new boost::unit_test::test_suite( "CDatabaseMySqlTest" );

		///@remarks Add the TC to the internal TS.
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseMySqlTest::TestCase_DatabaseQueryDirectQuery, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseMySqlTest::TestCase_DatabaseQueryStoredProcedure, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseMySqlTest::TestCase_DatabaseStatementDirectQuery, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseMySqlTest::TestCase_DatabaseStatementStoredProcedure, this ) ) );

		///@remarks Return the TS instance.
		return testSuite;
	}

	void CDatabaseMySqlTest::TestCase_DatabaseQueryDirectQuery()
	{
		CLogger::LogMessage( StringStream() << "**** Start TestCase_DatabaseQueryDirectQuery ****" );
		InstallMySqlDatabase( DB_DATABASE );
		LoadPlugins( InitializeSingletons(), true, false, false, false );
		Database::CDatabase * database = InstantiateDatabase( DATABASE_MYSQL_TYPE );

		if ( database )
		{
			Database::DatabaseConnectionPtr connection = CreateConnection( database, DB_SERVER, DB_DATABASE, DB_USER, DB_PASS );

			if ( connection )
			{
				CLogger::LogMessage( StringStream() << "    Insertions" );
				DatabaseUtils::TestDirectInsert< CDatabaseQuery >( connection );
				CLogger::LogMessage( StringStream() << "    Selection" );
				DatabaseUtils::TestDirectSelect< CDatabaseQuery >( connection );
				CLogger::LogMessage( StringStream() << "    Update" );
				DatabaseUtils::TestDirectUpdate< CDatabaseQuery >( connection );
				CLogger::LogMessage( StringStream() << "    Deletion" );
				DatabaseUtils::TestDirectDelete< CDatabaseQuery >( connection );

				DatabaseUtils::InsertLanguage< CDatabaseQuery >( connection );
			}

			database->RemoveConnection();
		}

		delete database;
		UnloadPlugins();
		CLogger::LogMessage( StringStream() << "**** End TestCase_DatabaseQueryDirectQuery ****" );
	}

	void CDatabaseMySqlTest::TestCase_DatabaseQueryStoredProcedure()
	{
		CLogger::LogMessage( StringStream() << "**** Start TestCase_DatabaseQueryStoredProcedure ****" );
		LoadPlugins( InitializeSingletons(), true, false, false, false );
		Database::CDatabase * database = InstantiateDatabase( DATABASE_MYSQL_TYPE );

		if ( database )
		{
			Database::DatabaseConnectionPtr connection = CreateConnection( database, DB_SERVER, DB_DATABASE, DB_USER, DB_PASS );

			if ( connection )
			{
				CLogger::LogMessage( StringStream() << "    No parameter, No Return" );
				DatabaseUtils::TestStoredNoParamNoReturn< CDatabaseQuery >( connection );
				CLogger::LogMessage( StringStream() << "    No parameter, Return" );
				DatabaseUtils::TestStoredNoParamReturn< CDatabaseQuery >( connection, STR( "" ) );
				CLogger::LogMessage( StringStream() << "    In parameters, No Return" );
				DatabaseUtils::TestStoredInParamNoReturn< CDatabaseQuery >( connection );
				CLogger::LogMessage( StringStream() << "    In parameters, Return" );
				DatabaseUtils::TestStoredNoParamReturn< CDatabaseQuery >( connection, STR( "WHERE ACTOR_ID > 5" ) );
				CLogger::LogMessage( StringStream() << "    In/Out INTEGER parameter, No Return" );
				DatabaseUtils::TestStoredInOutParamNoReturn< CDatabaseQuery >( connection );
				CLogger::LogMessage( StringStream() << "    In/Out INTEGER and DATETIME parameters, No Return" );
				DatabaseUtils::TestStoredInOutDtParamNoReturn< CDatabaseQuery >( connection );
				CLogger::LogMessage( StringStream() << "    Update" );
				DatabaseUtils::TestStoredUpdate< CDatabaseQuery >( connection );
			}

			database->RemoveConnection();
		}

		delete database;
		UnloadPlugins();
		UninstallMySqlDatabase( DB_DATABASE );
		CLogger::LogMessage( StringStream() << "**** End TestCase_DatabaseQueryStoredProcedure ****" );
	}

	void CDatabaseMySqlTest::TestCase_DatabaseStatementDirectQuery()
	{
		CLogger::LogMessage( StringStream() << "**** Start TestCase_DatabaseStatementDirectQuery ****" );
		InstallMySqlDatabase( DB_DATABASE );
		LoadPlugins( InitializeSingletons(), true, false, false, false );
		Database::CDatabase * database = InstantiateDatabase( DATABASE_MYSQL_TYPE );

		if ( database )
		{
			Database::DatabaseConnectionPtr connection = CreateConnection( database, DB_SERVER, DB_DATABASE, DB_USER, DB_PASS );

			if ( connection )
			{
				CLogger::LogMessage( StringStream() << "    Insertions" );
				DatabaseUtils::TestDirectInsert< CDatabaseStatement >( connection );
				CLogger::LogMessage( StringStream() << "    Selection" );
				DatabaseUtils::TestDirectSelect< CDatabaseStatement >( connection );
				CLogger::LogMessage( StringStream() << "    Update" );
				DatabaseUtils::TestDirectUpdate< CDatabaseStatement >( connection );
				CLogger::LogMessage( StringStream() << "    Deletion" );
				DatabaseUtils::TestDirectDelete< CDatabaseStatement >( connection );

				DatabaseUtils::InsertLanguage< CDatabaseStatement >( connection );
			}

			database->RemoveConnection();
		}

		delete database;
		UnloadPlugins();
		CLogger::LogMessage( StringStream() << "**** End TestCase_DatabaseStatementDirectQuery ****" );
	}

	void CDatabaseMySqlTest::TestCase_DatabaseStatementStoredProcedure()
	{
		CLogger::LogMessage( StringStream() << "**** Start TestCase_DatabaseStatementStoredProcedure ****" );
		LoadPlugins( InitializeSingletons(), true, false, false, false );
		Database::CDatabase * database = InstantiateDatabase( DATABASE_MYSQL_TYPE );

		if ( database )
		{
			Database::DatabaseConnectionPtr connection = CreateConnection( database, DB_SERVER, DB_DATABASE, DB_USER, DB_PASS );

			if ( connection )
			{
				CLogger::LogMessage( StringStream() << "    No parameter, No Return" );
				DatabaseUtils::TestStoredNoParamNoReturn< CDatabaseStatement >( connection );
				CLogger::LogMessage( StringStream() << "    No parameter, Return" );
				DatabaseUtils::TestStoredNoParamReturn< CDatabaseStatement >( connection, STR( "" ) );
				CLogger::LogMessage( StringStream() << "    In parameters, No Return" );
				DatabaseUtils::TestStoredInParamNoReturn< CDatabaseStatement >( connection );
				CLogger::LogMessage( StringStream() << "    In parameters, Return" );
				DatabaseUtils::TestStoredNoParamReturn< CDatabaseStatement >( connection, STR( "WHERE ACTOR_ID > 5" ) );
				CLogger::LogMessage( StringStream() << "    In/Out INTEGER parameter, No Return" );
				DatabaseUtils::TestStoredInOutParamNoReturn< CDatabaseStatement >( connection );
				CLogger::LogMessage( StringStream() << "    In/Out INTEGER and DATETIME parameters, No Return" );
				DatabaseUtils::TestStoredInOutDtParamNoReturn< CDatabaseStatement >( connection );
				CLogger::LogMessage( StringStream() << "    Update" );
				DatabaseUtils::TestStoredUpdate< CDatabaseStatement >( connection );
			}

			database->RemoveConnection();
		}

		delete database;
		UnloadPlugins();
		UninstallMySqlDatabase( DB_DATABASE );
		CLogger::LogMessage( StringStream() << "**** End TestCase_DatabaseStatementStoredProcedure ****" );
	}
}
END_NAMESPACE_DATABASE_TEST
