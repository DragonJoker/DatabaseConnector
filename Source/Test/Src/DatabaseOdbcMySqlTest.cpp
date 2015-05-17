/************************************************************************//**
 * @file DatabaseOdbcMySqlTest.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 16/07/2014 12:08:57
 *
 *
 * @brief Database ODBC plugin test
 *
 * @details Database ODBC plugin test
 *
 ***************************************************************************/

#include "DatabaseTestPch.h"

#include "DatabaseOdbcMySqlTest.h"
#include "DatabaseTestUtilsUtf8.h"

#include <Database.h>
#include <DatabaseConnection.h>
#include <DatabaseStatement.h>
#include <DatabaseQuery.h>
#include <DatabaseResult.h>
#include <DatabaseRow.h>
#include <DatabaseDateTime.h>

#include <DatabaseFactoryManager.h>
#include <DatabaseStringUtils.h>

BEGIN_NAMESPACE_DATABASE_TEST
{
	static const String DB_DATABASE = STR( "db_test_odbc_mysql" );
	static const String DB_SERVER = STR( "127.0.0.1" );
	static const String DB_USER = STR( "ariaUser" );
	static const String DB_PASS = STR( "ariaPwd" );

	static const String DATABASE_ODBC_MYSQL_TYPE = STR( "Database.Odbc.MySql" );

	CDatabaseOdbcMySqlTest::CDatabaseOdbcMySqlTest()
	{
	}

	CDatabaseOdbcMySqlTest::~CDatabaseOdbcMySqlTest()
	{
	}

	boost::unit_test::test_suite * CDatabaseOdbcMySqlTest::Init_Test_Suite()
	{
		///@remarks Create the internal TS instance.
		testSuite = new boost::unit_test::test_suite( "CDatabaseOdbcMySqlTest" );

		///@remarks Add the TC to the internal TS.
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseOdbcMySqlTest::TestCase_DatabaseQueryDirectQuery, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseOdbcMySqlTest::TestCase_DatabaseQueryStoredProcedure, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseOdbcMySqlTest::TestCase_DatabaseStatementDirectQuery, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseOdbcMySqlTest::TestCase_DatabaseStatementStoredProcedure, this ) ) );
		///@remarks Return the TS instance.
		return testSuite;
	}

	void CDatabaseOdbcMySqlTest::TestCase_DatabaseQueryDirectQuery()
	{
		CLogger::LogMessage( StringStream() << "**** Start TestCase_DatabaseQueryDirectQuery ****" );
		InstallMySqlDatabase( DB_DATABASE );
		InstallSourceOdbcAria( DB_DATABASE );
		LoadPlugins( InitializeSingletons(), false, false, true, false );
		Database::CDatabase * database = InstantiateDatabase( DATABASE_ODBC_MYSQL_TYPE );

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

	void CDatabaseOdbcMySqlTest::TestCase_DatabaseQueryStoredProcedure()
	{
		CLogger::LogMessage( StringStream() << "**** Start TestCase_DatabaseQueryStoredProcedure ****" );
		LoadPlugins( InitializeSingletons(), false, false, true, false );
		Database::CDatabase * database = InstantiateDatabase( DATABASE_ODBC_MYSQL_TYPE );

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
				DatabaseUtils::TestStoredNoParamReturn< CDatabaseQuery >( connection, STR( "WHERE ACTOR_ID > 10" ) );
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
		UninstallSourceOdbcAria( DB_DATABASE );
		UninstallMySqlDatabase( DB_DATABASE );
		CLogger::LogMessage( StringStream() << "**** End TestCase_DatabaseQueryStoredProcedure ****" );
	}

	void CDatabaseOdbcMySqlTest::TestCase_DatabaseStatementDirectQuery()
	{
		CLogger::LogMessage( StringStream() << "**** Start TestCase_DatabaseStatementDirectQuery ****" );
		InstallMySqlDatabase( DB_DATABASE );
		InstallSourceOdbcAria( DB_DATABASE );
		LoadPlugins( InitializeSingletons(), false, false, true, false );
		Database::CDatabase * database = InstantiateDatabase( DATABASE_ODBC_MYSQL_TYPE );

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

	void CDatabaseOdbcMySqlTest::TestCase_DatabaseStatementStoredProcedure()
	{
		CLogger::LogMessage( StringStream() << "**** Start TestCase_DatabaseStatementStoredProcedure ****" );
		LoadPlugins( InitializeSingletons(), false, false, true, false );
		Database::CDatabase * database = InstantiateDatabase( DATABASE_ODBC_MYSQL_TYPE );

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
				DatabaseUtils::TestStoredNoParamReturn< CDatabaseStatement >( connection, STR( "WHERE ACTOR_ID > 10" ) );
				//CLogger::LogMessage( StringStream() << "    In/Out INTEGER parameter, No Return" );
				//DatabaseUtils::TestStoredInOutParamNoReturn< CDatabaseStatement >( connection );
				//CLogger::LogMessage( StringStream() << "    In/Out INTEGER and DATETIME parameters, No Return" );
				//DatabaseUtils::TestStoredInOutDtParamNoReturn< CDatabaseStatement >( connection );
				CLogger::LogMessage( StringStream() << "    Update" );
				DatabaseUtils::TestStoredUpdate< CDatabaseStatement >( connection );
			}

			database->RemoveConnection();
		}

		delete database;
		UnloadPlugins();
		UninstallSourceOdbcAria( DB_DATABASE );
		UninstallMySqlDatabase( DB_DATABASE );
		CLogger::LogMessage( StringStream() << "**** End TestCase_DatabaseStatementStoredProcedure ****" );
	}
}
END_NAMESPACE_DATABASE_TEST
