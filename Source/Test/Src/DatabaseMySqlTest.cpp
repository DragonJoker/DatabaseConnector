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
    
    static const String DATABASE_MYSQL_TYPE = STR( "Aria.Database.MySql" );
    
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
#if defined( PERF_TEST )
        testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseMySqlTest::TestCase_DatabaseQueryPerformances, this ) ) );
        testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseMySqlTest::TestCase_DatabaseStatementPerformances, this ) ) );
#endif
        
        ///@remarks Return the TS instance.
        return testSuite;
    }
    
    void CDatabaseMySqlTest::TestCase_DatabaseQueryDirectQuery()
    {
        CLogger::LogMessage( StringStream() << "**** Start TestCase_DatabaseQueryDirectQuery ****" );
        InstallDatabase( DB_DATABASE );
        LoadPlugins( InitializeSingletons(), true, false, false );
        Database::CDatabase * database = CreateDatabase( DATABASE_MYSQL_TYPE );
        
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
        LoadPlugins( InitializeSingletons(), true, false, false );
        Database::CDatabase * database = CreateDatabase( DATABASE_MYSQL_TYPE );
        
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
        UninstallDatabase( DB_DATABASE );
        CLogger::LogMessage( StringStream() << "**** End TestCase_DatabaseQueryStoredProcedure ****" );
    }
    
    void CDatabaseMySqlTest::TestCase_DatabaseStatementDirectQuery()
    {
        CLogger::LogMessage( StringStream() << "**** Start TestCase_DatabaseStatementDirectQuery ****" );
        InstallDatabase( DB_DATABASE );
        LoadPlugins( InitializeSingletons(), true, false, false );
        Database::CDatabase * database = CreateDatabase( DATABASE_MYSQL_TYPE );
        
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
        LoadPlugins( InitializeSingletons(), true, false, false );
        Database::CDatabase * database = CreateDatabase( DATABASE_MYSQL_TYPE );
        
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
        UninstallDatabase( DB_DATABASE );
        CLogger::LogMessage( StringStream() << "**** End TestCase_DatabaseStatementStoredProcedure ****" );
    }
    
#if defined( PERF_TEST )
    void CDatabaseMySqlTest::TestCase_DatabaseQueryPerformances()
    {
        CLogger::LogMessage( StringStream() << "**** Start TestCase_DatabaseQueryPerformances ****" );
        InstallDatabase( DB_DATABASE );
        LoadPlugins( InitializeSingletons(), true, false, false );
        Database::CDatabase * database = CreateDatabase( DATABASE_MYSQL_TYPE );
        
        if ( database )
        {
            Database::DatabaseConnectionPtr connection = CreateConnection( database, DB_SERVER, DB_DATABASE, DB_USER, DB_PASS );
            
            if ( connection )
            {
#   if defined( NDEBUG )
                const uint32_t testCount = 10000;
#   else
                const uint32_t testCount = 10;
#   endif
                CLogger::LogMessage( StringStream() << "    Inserting mass actors through direct inserts" );
                DatabaseUtils::PerfDirectInsertActors< CDatabaseQuery >( connection, testCount );
                CLogger::LogMessage( StringStream() << "    Inserting mass actors through stored procedures" );
                DatabaseUtils::PerfStoredProcedureInsertActors< CDatabaseQuery >( connection, testCount );
                CLogger::LogMessage( StringStream() << "    Selecting actors through direct select (no where clause)" );
                DatabaseUtils::PerfDirectSelectActors< CDatabaseQuery >( connection, testCount, STR( "" ) );
                CLogger::LogMessage( StringStream() << "    Selecting actors through stored procedures (no where clause)" );
                DatabaseUtils::PerfStoredProcedureSelectActors< CDatabaseQuery >( connection, testCount, STR( "" ) );
                CLogger::LogMessage( StringStream() << "    Selecting actors through direct select (with a where clause)" );
                DatabaseUtils::PerfDirectSelectActors< CDatabaseQuery >( connection, testCount, STR( "WHERE ACTOR_ID < " ) + CStrUtils::ToString( testCount ) );
                CLogger::LogMessage( StringStream() << "    Selecting actors through stored procedures (with a where clause)" );
                DatabaseUtils::PerfStoredProcedureSelectActors< CDatabaseQuery >( connection, testCount, STR( "WHERE ACTOR_ID < " ) + CStrUtils::ToString( testCount ) );
                CLogger::LogMessage( StringStream() << "    Deleting mass actors through direct deletes" );
                DatabaseUtils::PerfDirectDeleteActors< CDatabaseQuery >( connection, testCount );
                CLogger::LogMessage( StringStream() << "    Deleting mass actors through stored procedures" );
                DatabaseUtils::PerfStoredProcedureDeleteActors< CDatabaseQuery >( connection, testCount );
            }
            
            database->RemoveConnection();
        }
        
        delete database;
        UnloadPlugins();
        UninstallDatabase( DB_DATABASE );
        CLogger::LogMessage( StringStream() << "**** End TestCase_DatabaseQueryPerformances ****" );
    }
    
    void CDatabaseMySqlTest::TestCase_DatabaseStatementPerformances()
    {
        CLogger::LogMessage( StringStream() << "**** Start TestCase_DatabaseStatementPerformances ****" );
        InstallDatabase( DB_DATABASE );
        LoadPlugins( InitializeSingletons(), true, false, false );
        Database::CDatabase * database = CreateDatabase( DATABASE_MYSQL_TYPE );
        
        if ( database )
        {
            Database::DatabaseConnectionPtr connection = CreateConnection( database, DB_SERVER, DB_DATABASE, DB_USER, DB_PASS );
            
            if ( connection )
            {
#   if defined( NDEBUG )
                const uint32_t testCount = 10000;
#   else
                const uint32_t testCount = 10;
#   endif
                CLogger::LogMessage( StringStream() << "    Inserting mass actors through direct inserts" );
                DatabaseUtils::PerfDirectInsertActors< CDatabaseStatement >( connection, testCount );
                CLogger::LogMessage( StringStream() << "    Inserting mass actors through stored procedures" );
                DatabaseUtils::PerfStoredProcedureInsertActors< CDatabaseStatement >( connection, testCount );
                CLogger::LogMessage( StringStream() << "    Selecting actors through direct select (no where clause)" );
                DatabaseUtils::PerfDirectSelectActors< CDatabaseStatement >( connection, testCount, STR( "" ) );
                CLogger::LogMessage( StringStream() << "    Selecting actors through stored procedures (no where clause)" );
                DatabaseUtils::PerfStoredProcedureSelectActors< CDatabaseStatement >( connection, testCount, STR( "" ) );
                CLogger::LogMessage( StringStream() << "    Selecting actors through direct select (with a where clause)" );
                DatabaseUtils::PerfDirectSelectActors< CDatabaseStatement >( connection, testCount, STR( "WHERE ACTOR_ID < " ) + CStrUtils::ToString( testCount ) );
                CLogger::LogMessage( StringStream() << "    Selecting actors through stored procedures (with a where clause)" );
                DatabaseUtils::PerfStoredProcedureSelectActors< CDatabaseStatement >( connection, testCount, STR( "WHERE ACTOR_ID < " ) + CStrUtils::ToString( testCount ) );
                CLogger::LogMessage( StringStream() << "    Deleting mass actors through direct deletes" );
                DatabaseUtils::PerfDirectDeleteActors< CDatabaseStatement >( connection, testCount );
                CLogger::LogMessage( StringStream() << "    Deleting mass actors through stored procedures" );
                DatabaseUtils::PerfStoredProcedureDeleteActors< CDatabaseStatement >( connection, testCount );
            }
            
            database->RemoveConnection();
        }
        
        delete database;
        UnloadPlugins();
        UninstallDatabase( DB_DATABASE );
        CLogger::LogMessage( StringStream() << "**** End TestCase_DatabaseStatementPerformances ****" );
    }
#endif
}
END_NAMESPACE_DATABASE_TEST
