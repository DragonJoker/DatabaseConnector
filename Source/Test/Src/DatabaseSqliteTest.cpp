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

#include "DatabaseSqliteTest.h"
#include "DatabaseTestUtilsUtf8.h"

#include <Database.h>
#include <DatabaseResult.h>
#include <DatabaseRow.h>
#include <DatabaseDateTime.h>

#include <DatabaseFactoryManager.h>
#include <DatabaseStringUtils.h>

BEGIN_NAMESPACE_DATABASE_TEST
{
	static const String DB_DATABASE = STR( "db_test.sqlite" );
	static const String DB_SERVER = STR( "./" );
	static const String DB_USER = STR( "ariaUser" );
	static const String DB_PASS = STR( "ariaPwd" );

	static const String DATABASE_MYSQL_TYPE = STR( "Database.SQLite" );

	CDatabaseSqliteTest::CDatabaseSqliteTest()
	{
	}

	CDatabaseSqliteTest::~CDatabaseSqliteTest()
	{
	}

	boost::unit_test::test_suite * CDatabaseSqliteTest::Init_Test_Suite()
	{
		///@remarks Create the internal TS instance.
		testSuite = new boost::unit_test::test_suite( "CDatabaseSqliteTest" );

		///@remarks Add the TC to the internal TS.
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseSqliteTest::TestCase_CreateDatabase, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseSqliteTest::TestCase_DatabaseQueryDirectQuery, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseSqliteTest::TestCase_DatabaseQueryStoredProcedure, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseSqliteTest::TestCase_DatabaseStatementDirectQuery, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseSqliteTest::TestCase_DatabaseStatementStoredProcedure, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseSqliteTest::TestCase_DestroyDatabase, this ) ) );

		///@remarks Return the TS instance.
		return testSuite;
	}

	void CDatabaseSqliteTest::TestCase_CreateDatabase()
	{
		CLogger::LogMessage( StringStream() << "**** Start TestCase_CreateDatabase ****" );
		InstallSqliteDatabase( DB_DATABASE );
		LoadPlugins( InitializeSingletons(), false, true, false, false );
		Database::CDatabase * database = InstantiateDatabase( DATABASE_MYSQL_TYPE );

		if ( database )
		{
			Database::DatabaseConnectionPtr connection = CreateConnection( database, DB_SERVER, DB_DATABASE, DB_USER, DB_PASS );

			if ( connection )
			{
				String query;
				query += STR( "CREATE TABLE [Test]" );
				query += STR( "(	IDTest INTEGER PRIMARY KEY" );
				query += STR( ",	IntField INT" );
				query += STR( ",	IntegerField INTEGER" );
				query += STR( ",	TinyIntField TINYINT" );
				query += STR( ",	SmallIntField SMALLINT" );
				query += STR( ",	MediumIntField MEDIUMINT" );
				query += STR( ",	BigIntField BIGINT" );
				query += STR( ",	UnsignedBigIntField UNSIGNED BIG INT" );
				query += STR( ",	Int2Field INT2" );
				query += STR( ",	Int8Field INT8" );
				query += STR( ",	RealField REAL" );
				query += STR( ",	DoubleField DOUBLE" );
				query += STR( ",	DoublePrecisionField DOUBLE PRECISION" );
				query += STR( ",	FloatField FLOAT" );
				query += STR( ",	NumericField NUMERIC" );
				query += STR( ",	DecimalField DECIMAL(10,5)" );
				query += STR( ",	BooleanField BOOLEAN" );
				query += STR( ",	DateField DATE" );
				query += STR( ",	DateTimeField DATETIME" );
				query += STR( ",	CharacterField CHARACTER(20)" );
				query += STR( ",	VarcharField VARCHAR(255)" );
				query += STR( ",	VaryingcharField VARYING CHARACTER(255)" );
				query += STR( ",	NcharField NCHAR(55)" );
				query += STR( ",	NativeCharField NATIVE CHARACTER(70)" );
				query += STR( ",	NVarcharField NVARCHAR(100)" );
				query += STR( ",	TextField TEXT" );
				query += STR( ",	ClobField CLOB" );
				query += STR( ",	BlobField BLOB" );
				query += STR( ");" );
				connection->ExecuteUpdate( query );
			}
		}

		delete database;
		UnloadPlugins();
		CLogger::LogMessage( StringStream() << "**** End TestCase_CreateDatabase ****" );
	}

	void CDatabaseSqliteTest::TestCase_DatabaseQueryDirectQuery()
	{
		CLogger::LogMessage( StringStream() << "**** Start TestCase_DatabaseQueryDirectQuery ****" );
		InstallMySqlDatabase( DB_DATABASE );
		LoadPlugins( InitializeSingletons(), false, true, false, false );
		Database::CDatabase * database = InstantiateDatabase( DATABASE_MYSQL_TYPE );

		if ( database )
		{
			Database::DatabaseConnectionPtr connection = CreateConnection( database, DB_SERVER, DB_DATABASE, DB_USER, DB_PASS );

			if ( connection )
			{
				CLogger::LogMessage( StringStream() << " Insertions" );
				DatabaseUtils::TestDirectInsert< CDatabaseQuery >( connection );
				CLogger::LogMessage( StringStream() << " Selection" );
				DatabaseUtils::TestDirectSelect< CDatabaseQuery >( connection );
				CLogger::LogMessage( StringStream() << " Update" );
				DatabaseUtils::TestDirectUpdate< CDatabaseQuery >( connection );
				CLogger::LogMessage( StringStream() << " Deletion" );
				DatabaseUtils::TestDirectDelete< CDatabaseQuery >( connection );

				DatabaseUtils::InsertLanguage< CDatabaseQuery >( connection );
			}

			database->RemoveConnection();
		}

		delete database;
		UnloadPlugins();
		CLogger::LogMessage( StringStream() << "**** End TestCase_DatabaseQueryDirectQuery ****" );
	}

	void CDatabaseSqliteTest::TestCase_DatabaseQueryStoredProcedure()
	{
		CLogger::LogMessage( StringStream() << "**** Start TestCase_DatabaseQueryStoredProcedure ****" );
		LoadPlugins( InitializeSingletons(), false, true, false, false );
		Database::CDatabase * database = InstantiateDatabase( DATABASE_MYSQL_TYPE );

		if ( database )
		{
			Database::DatabaseConnectionPtr connection = CreateConnection( database, DB_SERVER, DB_DATABASE, DB_USER, DB_PASS );

			if ( connection )
			{
				CLogger::LogMessage( StringStream() << " No parameter, No Return" );
				DatabaseUtils::TestStoredNoParamNoReturn< CDatabaseQuery >( connection );
				CLogger::LogMessage( StringStream() << " No parameter, Return" );
				DatabaseUtils::TestStoredNoParamReturn< CDatabaseQuery >( connection, STR( "" ) );
				CLogger::LogMessage( StringStream() << " In parameters, No Return" );
				DatabaseUtils::TestStoredInParamNoReturn< CDatabaseQuery >( connection );
				CLogger::LogMessage( StringStream() << " In parameters, Return" );
				DatabaseUtils::TestStoredNoParamReturn< CDatabaseQuery >( connection, STR( "WHERE ACTOR_ID > 5" ) );
				CLogger::LogMessage( StringStream() << " In/Out INTEGER parameter, No Return" );
				DatabaseUtils::TestStoredInOutParamNoReturn< CDatabaseQuery >( connection );
				CLogger::LogMessage( StringStream() << " In/Out INTEGER and DATETIME parameters, No Return" );
				DatabaseUtils::TestStoredInOutDtParamNoReturn< CDatabaseQuery >( connection );
				CLogger::LogMessage( StringStream() << " Update" );
				DatabaseUtils::TestStoredUpdate< CDatabaseQuery >( connection );
			}

			database->RemoveConnection();
		}

		delete database;
		UnloadPlugins();
		UninstallMySqlDatabase( DB_DATABASE );
		CLogger::LogMessage( StringStream() << "**** End TestCase_DatabaseQueryStoredProcedure ****" );
	}

	void CDatabaseSqliteTest::TestCase_DatabaseStatementDirectQuery()
	{
		CLogger::LogMessage( StringStream() << "**** Start TestCase_DatabaseStatementDirectQuery ****" );
		InstallMySqlDatabase( DB_DATABASE );
		LoadPlugins( InitializeSingletons(), false, true, false, false );
		Database::CDatabase * database = InstantiateDatabase( DATABASE_MYSQL_TYPE );

		if ( database )
		{
			Database::DatabaseConnectionPtr connection = CreateConnection( database, DB_SERVER, DB_DATABASE, DB_USER, DB_PASS );

			if ( connection )
			{
				CLogger::LogMessage( StringStream() << " Insertions" );
				DatabaseUtils::TestDirectInsert< CDatabaseStatement >( connection );
				CLogger::LogMessage( StringStream() << " Selection" );
				DatabaseUtils::TestDirectSelect< CDatabaseStatement >( connection );
				CLogger::LogMessage( StringStream() << " Update" );
				DatabaseUtils::TestDirectUpdate< CDatabaseStatement >( connection );
				CLogger::LogMessage( StringStream() << " Deletion" );
				DatabaseUtils::TestDirectDelete< CDatabaseStatement >( connection );

				DatabaseUtils::InsertLanguage< CDatabaseStatement >( connection );
			}

			database->RemoveConnection();
		}

		delete database;
		UnloadPlugins();
		CLogger::LogMessage( StringStream() << "**** End TestCase_DatabaseStatementDirectQuery ****" );
	}

	void CDatabaseSqliteTest::TestCase_DatabaseStatementStoredProcedure()
	{
		CLogger::LogMessage( StringStream() << "**** Start TestCase_DatabaseStatementStoredProcedure ****" );
		LoadPlugins( InitializeSingletons(), false, true, false, false );
		Database::CDatabase * database = InstantiateDatabase( DATABASE_MYSQL_TYPE );

		if ( database )
		{
			Database::DatabaseConnectionPtr connection = CreateConnection( database, DB_SERVER, DB_DATABASE, DB_USER, DB_PASS );

			if ( connection )
			{
				CLogger::LogMessage( StringStream() << " No parameter, No Return" );
				DatabaseUtils::TestStoredNoParamNoReturn< CDatabaseStatement >( connection );
				CLogger::LogMessage( StringStream() << " No parameter, Return" );
				DatabaseUtils::TestStoredNoParamReturn< CDatabaseStatement >( connection, STR( "" ) );
				CLogger::LogMessage( StringStream() << " In parameters, No Return" );
				DatabaseUtils::TestStoredInParamNoReturn< CDatabaseStatement >( connection );
				CLogger::LogMessage( StringStream() << " In parameters, Return" );
				DatabaseUtils::TestStoredNoParamReturn< CDatabaseStatement >( connection, STR( "WHERE ACTOR_ID > 5" ) );
				CLogger::LogMessage( StringStream() << " In/Out INTEGER parameter, No Return" );
				DatabaseUtils::TestStoredInOutParamNoReturn< CDatabaseStatement >( connection );
				CLogger::LogMessage( StringStream() << " In/Out INTEGER and DATETIME parameters, No Return" );
				DatabaseUtils::TestStoredInOutDtParamNoReturn< CDatabaseStatement >( connection );
				CLogger::LogMessage( StringStream() << " Update" );
				DatabaseUtils::TestStoredUpdate< CDatabaseStatement >( connection );
			}

			database->RemoveConnection();
		}

		delete database;
		UnloadPlugins();
		UninstallMySqlDatabase( DB_DATABASE );
		CLogger::LogMessage( StringStream() << "**** End TestCase_DatabaseStatementStoredProcedure ****" );
	}

	void CDatabaseSqliteTest::TestCase_DestroyDatabase()
	{
		CLogger::LogMessage( StringStream() << "**** Start TestCase_DestroyDatabase ****" );
		UninstallSqliteDatabase( DB_DATABASE );
		CLogger::LogMessage( StringStream() << "**** End TestCase_DestroyDatabase ****" );
	}
}
END_NAMESPACE_DATABASE_TEST
