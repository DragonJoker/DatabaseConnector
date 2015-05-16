/************************************************************************//**
 * @file DatabaseTestUtils.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 06/18/2014 12:04:36
 * 
 *
 * @brief Utils for Database tests.
 *
 * @details Utils for Database tests.
 *
 ***************************************************************************/

#ifndef ___DATABASE_TEST_UTILS_H___
#define ___DATABASE_TEST_UTILS_H___

#include "DatabaseTestPrerequisites.h"

#include <DatabaseConnection.h>
#include <DatabaseStatement.h>
#include <DatabaseQuery.h>

BEGIN_NAMESPACE_DATABASE_TEST
{
    /** Creates an ARIA database.
    @param database
        Database name.
    @return
        Exit code of the CProcess executing the batch file.
    */
    int InstallDatabase( const String & database );
    
    /** Deletes an ARIA database.
    @param database
        Database name.
    @return
        Exit code of the CProcess executing the batch file.
    */
    int UninstallDatabase( const String & database );
    
    /** Creates an ODBC source for an ARIA database.
    @param dsn
        ODBC source name.
    @return
        Exit code of the CProcess executing the batch file.
    */
    int InstallSourceOdbcAria( const String & dsn );
    
    /** Deletes an ODBC source for an ARIA database.
    @param dsn
        ODBC source name.
    @return
        Exit code of the CProcess executing the batch file.
    */
    int UninstallSourceOdbcAria( const String & dsn );
    
    namespace DatabaseUtils
    {
        template< typename T >
        static std::shared_ptr< T > CreateStmt( DatabaseConnectionPtr connection, const String & query );
        
        template<>
        static std::shared_ptr< CDatabaseStatement > CreateStmt< CDatabaseStatement >( DatabaseConnectionPtr connection, const String & query )
        {
            std::shared_ptr< CDatabaseStatement > result;
            try
            {
                result = connection->CreateStatement( query );
            }
            catch ( std::exception & e )
            {
                CLogger::LogMessage( StringStream() << e.what() );
            }
            
            return result;
        }
        
        template<>
        static std::shared_ptr< CDatabaseQuery > CreateStmt< CDatabaseQuery >( DatabaseConnectionPtr connection, const String & query )
        {
            std::shared_ptr< CDatabaseQuery > result;
            try
            {
                result = connection->CreateQuery( query );
            }
            catch ( std::exception & e )
            {
                CLogger::LogMessage( StringStream() << e.what() );
            }
            
            return result;
        }
        
        template< typename T >
        static void TestDirectInsert( DatabaseConnectionPtr connection )
        {
            uint8_t blobArray[] = { /*0x00, */0x02, 0x04, 0x06, 0x08, 0x10, 0x15, 0x20, 0x25, 0x30, 0x35, 0x40, 0x45, 0x50, 0x50, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0xF0, 0xFF };
            std::vector< uint8_t > blob;
            blob.insert( blob.end(), blobArray, blobArray + 25 );
            int inserts = 20;
            
            std::shared_ptr< T > stmtGetCount = DatabaseUtils::CreateStmt< T >( connection, STR( "SELECT MAX( ACTOR_ID ) AS ActorID FROM actors" ) );
            BOOST_CHECK( stmtGetCount );
            int64_t count = -1;
            
            if ( stmtGetCount )
            {
                BOOST_CHECK( stmtGetCount->Initialize() == EErrorType_NONE );
                Database::DatabaseResultPtr result = stmtGetCount->ExecuteSelect();
                BOOST_CHECK( result );
                BOOST_CHECK( result && result->GetRowCount() );
                
                if ( result && result->GetRowCount() )
                {
                    BOOST_CHECK_NO_THROW( result->GetFirstRow()->Get( 0, count ) );
                }
            }
            
            count++;
            
            if ( count )
            {
                std::shared_ptr< T > stmtInsert = DatabaseUtils::CreateStmt< T >( connection, STR( "INSERT INTO actors (ACTOR_TYPE, ACTOR_NAME, ACTOR_LOCATION, ACTOR_LOGO, ACTOR_PHONE_NUMBER, ACTOR_CREATION_DATE, ACTOR_MODIFICATION_DATE) VALUES (?, ?, ?, ?, ?, NOW(), NOW())" ) );
                BOOST_CHECK( stmtInsert );
                
                if ( stmtInsert )
                {
                    BOOST_CHECK( stmtInsert->CreateParameter( STR( "ACTOR_TYPE" ), Database::EFieldType_SMALL_INTEGER, Database::EParameterType_IN ) );
                    BOOST_CHECK( stmtInsert->CreateParameter( STR( "ACTOR_NAME" ), Database::EFieldType_VARCHAR, 50, Database::EParameterType_IN ) );
                    BOOST_CHECK( stmtInsert->CreateParameter( STR( "ACTOR_LOCATION" ), Database::EFieldType_VARCHAR, 255, Database::EParameterType_IN ) );
                    BOOST_CHECK( stmtInsert->CreateParameter( STR( "ACTOR_LOGO" ), Database::EFieldType_BINARY, -1, Database::EParameterType_IN ) );
                    BOOST_CHECK( stmtInsert->CreateParameter( STR( "ACTOR_PHONE_NUMBER" ), Database::EFieldType_VARCHAR, 50, Database::EParameterType_IN ) );
                    BOOST_CHECK( stmtInsert->Initialize() == EErrorType_NONE );
                    
                    for ( int i = 0; i < inserts; i++ )
                    {
                        uint32_t index = 0;
                        int16_t type( i + 1 );
                        stmtInsert->SetParameterValue( index++, type );
                        stmtInsert->SetParameterValue( index++, STR( "Acteur_" ) + CStrUtils::ToString( count + i ) );
                        
                        if ( i % 2 )
                        {
                            stmtInsert->SetParameterValue( index++, Database::ToUtf8( L"Areva Intercontrôle", L"Windows-1252" ) );
                        }
                        else
                        {
                            stmtInsert->SetParameterValue( index++, Database::ToUtf8( "Areva Intercontrôle", "Windows-1252" ) );
                        }
                        
                        stmtInsert->SetParameterValue( index++, blob );
                        stmtInsert->SetParameterValue( index++, STR( "0123456789" ) );
                        BOOST_CHECK( stmtInsert->ExecuteUpdate() );
                    }
                    
                    count--;
                    Database::DatabaseResultPtr result = stmtGetCount->ExecuteSelect();
                    BOOST_CHECK( result );
                    BOOST_CHECK( result && result->GetRowCount() );
                    
                    if ( result && result->GetRowCount() )
                    {
                        long long field = 0;
                        BOOST_CHECK_NO_THROW( result->GetFirstRow()->Get( 0, field ) );
                        BOOST_CHECK_EQUAL( field, count + inserts );
                        count++;
                    }
                }
            }
        }
        
        template< typename T >
        static void TestDirectSelect( DatabaseConnectionPtr connection )
        {
            std::shared_ptr< T > stmtSelect = DatabaseUtils::CreateStmt< T >( connection, STR( "SELECT ACTOR_ID, ACTOR_TYPE, ACTOR_NAME, ACTOR_LOCATION FROM ACTORS" ) );
            BOOST_CHECK( stmtSelect );
            
            if ( stmtSelect )
            {
                BOOST_CHECK( stmtSelect->Initialize() == EErrorType_NONE );
                DatabaseResultPtr result = stmtSelect->ExecuteSelect();
                BOOST_CHECK( result );
                BOOST_CHECK( result && result->GetRowCount() );
                
                if ( result && result->GetRowCount() )
                {
                    DatabaseRowPtr row = result->GetFirstRow();
                    
                    while ( row )
                    {
                        uint32_t index = 0;
                        CLogger::LogMessage( StringStream() << STR( "      ID : " ) << row->Get< int64_t >( index++ ) );
                        CLogger::LogMessage( StringStream() << STR( "      TYPE : " ) << row->Get< int16_t >( index++ ) );
                        CLogger::LogMessage( StringStream() << STR( "      NAME : " ) << row->Get< std::string >( index++ ).c_str() );
                        CLogger::LogMessage( StringStream() << STR( "      LOCATION : " ) << row->Get< std::string >( index++ ).c_str() );
                        row = result->GetNextRow();
                    }
                }
            }
        }
        
        template< typename T >
        static void TestDirectUpdate( DatabaseConnectionPtr connection )
        {
            std::shared_ptr< T > stmtGetMin = DatabaseUtils::CreateStmt< T >( connection, STR( "SELECT MIN( ACTOR_ID ) AS ActorID FROM actors" ) );
            BOOST_CHECK( stmtGetMin );
            int64_t min = -1;
            
            if ( stmtGetMin )
            {
                BOOST_CHECK( stmtGetMin->Initialize() == EErrorType_NONE );
                Database::DatabaseResultPtr result = stmtGetMin->ExecuteSelect();
                BOOST_CHECK( result );
                BOOST_CHECK( result && result->GetRowCount() );
                
                if ( result && result->GetRowCount() )
                {
                    BOOST_CHECK_NO_THROW( result->GetFirstRow()->Get( 0, min ) );
                }
            }
            
            if ( min )
            {
                std::shared_ptr< T > stmtUpdate = DatabaseUtils::CreateStmt< T >( connection, STR( "UPDATE actors SET ACTOR_LOCATION = ? WHERE ACTOR_ID = ?" ) );
                BOOST_CHECK( stmtUpdate );
                
                if ( stmtUpdate )
                {
                    BOOST_CHECK( stmtUpdate->CreateParameter( STR( "ACTOR_LOCATION" ), Database::EFieldType_VARCHAR, 255, Database::EParameterType_IN ) );
                    BOOST_CHECK( stmtUpdate->CreateParameter( STR( "ACTOR_ID" ), Database::EFieldType_LONG_INTEGER, Database::EParameterType_IN ) );
                    BOOST_CHECK( stmtUpdate->Initialize() == EErrorType_NONE );
                    
                    for ( int i = 0; i < 10; i++ )
                    {
                        int64_t id = min + i;
                        stmtUpdate->SetParameterValue( 0, ToUtf8( "Areva Intercontrôle NP", "Windows-1252" ) );
                        stmtUpdate->SetParameterValue( 1, id );
                        BOOST_CHECK( stmtUpdate->ExecuteUpdate() );
                    }
                }
            }
        }
        
        template< typename T >
        static void TestDirectDelete( DatabaseConnectionPtr connection )
        {
            std::shared_ptr< T > stmtGetMin = DatabaseUtils::CreateStmt< T >( connection, STR( "SELECT MIN( ACTOR_ID ) AS ActorID FROM actors" ) );
            BOOST_CHECK( stmtGetMin );
            int64_t min = -1;
            
            if ( stmtGetMin )
            {
                BOOST_CHECK( stmtGetMin->Initialize() == EErrorType_NONE );
                Database::DatabaseResultPtr result = stmtGetMin->ExecuteSelect();
                BOOST_CHECK( result );
                BOOST_CHECK( result && result->GetRowCount() );
                
                if ( result && result->GetRowCount() )
                {
                    BOOST_CHECK_NO_THROW( result->GetFirstRow()->Get( 0, min ) );
                }
            }
            
            if ( min )
            {
                std::shared_ptr< T > stmtDelete = DatabaseUtils::CreateStmt< T >( connection, STR( "DELETE FROM actors WHERE ACTOR_ID = ?" ) );
                BOOST_CHECK( stmtDelete );
                
                if ( stmtDelete )
                {
                    BOOST_CHECK( stmtDelete->CreateParameter( STR( "ACTOR_ID" ), Database::EFieldType_LONG_INTEGER, Database::EParameterType_IN ) );
                    BOOST_CHECK( stmtDelete->Initialize() == EErrorType_NONE );
                    
                    for ( int i = 0; i < 5; i++ )
                    {
                        int64_t id = min + i;
                        stmtDelete->SetParameterValue( 0, id );
                        BOOST_CHECK( stmtDelete->ExecuteUpdate() );
                    }
                }
            }
        }
        
        template< typename T >
        static void InsertLanguage( DatabaseConnectionPtr connection )
        {
            std::shared_ptr< T > stmtLanguage = DatabaseUtils::CreateStmt< T >( connection, STR( "CALL SpAddUpdateLanguage(?,?,?,?)" ) );
            BOOST_CHECK( stmtLanguage );
            
            if ( stmtLanguage )
            {
                BOOST_CHECK( stmtLanguage->CreateParameter( STR( "id" ), EFieldType_LONG_INTEGER, EParameterType_INOUT ) );
                BOOST_CHECK( stmtLanguage->CreateParameter( STR( "name" ), EFieldType_VARCHAR, 5, EParameterType_IN ) );
                BOOST_CHECK( stmtLanguage->CreateParameter( STR( "desc" ), EFieldType_VARCHAR, 100, EParameterType_IN ) );
                BOOST_CHECK( stmtLanguage->CreateParameter( STR( "date" ), EFieldType_DATETIME, -1, EParameterType_OUT ) );
                BOOST_CHECK( stmtLanguage->Initialize() == EErrorType_NONE );
                uint32_t index = 0;
                stmtLanguage->SetParameterNull( index++ );
                stmtLanguage->SetParameterValue( index++, STR( "Klong" ) );
                stmtLanguage->SetParameterValue( index++, STR( "Start Trek Klongin" ) );
                stmtLanguage->SetParameterNull( index++ );
                BOOST_CHECK( stmtLanguage->ExecuteUpdate() );
            }
        }
        
        template< typename T >
        static void TestStoredNoParamNoReturn( DatabaseConnectionPtr connection )
        {
            std::shared_ptr< T > stmtClear = DatabaseUtils::CreateStmt< T >( connection, STR( "CALL SpClearAcquisitionWorkspaceTables" ) );
            BOOST_CHECK( stmtClear );
            
            if ( stmtClear )
            {
                BOOST_CHECK( stmtClear->Initialize() == EErrorType_NONE );
                
                for ( int i = 0; i < 2; i++ )
                {
                    BOOST_CHECK( stmtClear->ExecuteUpdate() );
                }
            }
        }
        
        template< typename T >
        static void TestStoredNoParamReturn( DatabaseConnectionPtr connection, const String & where )
        {
            std::shared_ptr< T > stmtGetActors = DatabaseUtils::CreateStmt< T >( connection, STR( "CALL SpGetActors(?)" ) );
            BOOST_CHECK( stmtGetActors );
            
            if ( stmtGetActors )
            {
                BOOST_CHECK( stmtGetActors->CreateParameter( STR( "Where" ), Database::EFieldType_VARCHAR, 500, Database::EParameterType_IN ) );
                BOOST_CHECK( stmtGetActors->Initialize() == EErrorType_NONE );
                
                for ( int i = 0; i < 2; i++ )
                {
                    stmtGetActors->SetParameterValue( 0, where );
                    DatabaseResultPtr result = stmtGetActors->ExecuteSelect();
                    BOOST_CHECK( result );
                    BOOST_CHECK( result && result->GetRowCount() );
                    
                    if ( result && result->GetRowCount() )
                    {
                        DatabaseRowPtr row = result->GetFirstRow();
                        
                        while ( row )
                        {
                            uint32_t index = 0;
                            CLogger::LogMessage( StringStream() << STR( "      ID : " ) << row->Get< int64_t >( index++ ) );
                            CLogger::LogMessage( StringStream() << STR( "      TYPE : " ) << row->Get< int16_t >( index++ ) );
                            CLogger::LogMessage( StringStream() << STR( "      NAME : " ) << row->Get< std::string >( index++ ).c_str() );
                            CLogger::LogMessage( StringStream() << STR( "      LOCATION : " ) << row->Get< std::string >( index++ ).c_str() );
                            row = result->GetNextRow();
                        }
                    }
                }
            }
        }
        
        template< typename T >
        static void TestStoredInParamNoReturn( DatabaseConnectionPtr connection )
        {
            std::shared_ptr< T > stmtGetMin = DatabaseUtils::CreateStmt< T >( connection, STR( "SELECT MIN( ACTOR_ID ) AS ActorID FROM actors" ) );
            BOOST_CHECK( stmtGetMin );
            int64_t min = 0;
            
            if ( stmtGetMin )
            {
                BOOST_CHECK( stmtGetMin->Initialize() == EErrorType_NONE );
                Database::DatabaseResultPtr result = stmtGetMin->ExecuteSelect();
                BOOST_CHECK( result );
                BOOST_CHECK( result && result->GetRowCount() );
                
                if ( result && result->GetRowCount() )
                {
                    BOOST_CHECK_NO_THROW( result->GetFirstRow()->Get( 0, min ) );
                }
            }
            
            if ( min )
            {
                std::shared_ptr< T > stmtDelete = DatabaseUtils::CreateStmt< T >( connection, STR( "CALL SpDeleteActor(?)" ) );
                BOOST_CHECK( stmtDelete );
                
                if ( stmtDelete )
                {
                    BOOST_CHECK( stmtDelete->CreateParameter( STR( "id" ), Database::EFieldType_LONG_INTEGER, Database::EParameterType_IN ) );
                    BOOST_CHECK( stmtDelete->Initialize() == EErrorType_NONE );
                    
                    for ( int i = 0; i < 5; i++ )
                    {
                        int64_t id( min + i );
                        stmtDelete->SetParameterValue( 0, id );
                        BOOST_CHECK( stmtDelete->ExecuteUpdate() );
                    }
                }
            }
        }
        
        template< typename T >
        static void TestStoredInOutParamNoReturn( DatabaseConnectionPtr connection )
        {
            std::shared_ptr< T > stmtGetCount = DatabaseUtils::CreateStmt< T >( connection, STR( "SELECT MAX( TEXT_ID ) AS TextID FROM TEXTS" ) );
            BOOST_CHECK( stmtGetCount );
            int64_t count = -1;
            
            if ( stmtGetCount )
            {
                BOOST_CHECK( stmtGetCount->Initialize() == EErrorType_NONE );
                Database::DatabaseResultPtr result = stmtGetCount->ExecuteSelect();
                BOOST_CHECK( result );
                BOOST_CHECK( result && result->GetRowCount() );
                
                if ( result && result->GetRowCount() )
                {
                    BOOST_CHECK_NO_THROW( result->GetFirstRow()->Get( 0, count ) );
                }
            }
            
            count++;
            
            if ( count )
            {
                std::shared_ptr< T > stmtAddUpdate = DatabaseUtils::CreateStmt< T >( connection, STR( "CALL SpAddUpdateText(?,?,?,?,?,?,?,?);" ) );
                BOOST_CHECK( stmtAddUpdate );
                
                if ( stmtAddUpdate )
                {
                    BOOST_CHECK( stmtAddUpdate->CreateParameter( STR( "TEXT_ID" ), Database::EFieldType_LONG_INTEGER, Database::EParameterType_INOUT ) );
                    BOOST_CHECK( stmtAddUpdate->CreateParameter( STR( "TEXT_COMPONENT_ID" ), Database::EFieldType_LONG_INTEGER, Database::EParameterType_IN ) );
                    BOOST_CHECK( stmtAddUpdate->CreateParameter( STR( "TEXT_GROUP_ID" ), Database::EFieldType_LONG_INTEGER, Database::EParameterType_IN ) );
                    BOOST_CHECK( stmtAddUpdate->CreateParameter( STR( "TEXT_LANGUAGE_ID" ), Database::EFieldType_LONG_INTEGER, Database::EParameterType_IN ) );
                    BOOST_CHECK( stmtAddUpdate->CreateParameter( STR( "TEXT_KEY" ), Database::EFieldType_VARCHAR, 100, Database::EParameterType_IN ) );
                    BOOST_CHECK( stmtAddUpdate->CreateParameter( STR( "TEXT_VALUE" ), Database::EFieldType_TEXT, Database::EParameterType_IN ) );
                    BOOST_CHECK( stmtAddUpdate->CreateParameter( STR( "TEXT_ERROR_CODE" ), Database::EFieldType_VARCHAR, 10, Database::EParameterType_IN ) );
                    BOOST_CHECK( stmtAddUpdate->CreateParameter( STR( "TEXT_GROUP_CODE" ), Database::EFieldType_VARCHAR, 10, Database::EParameterType_IN ) );
                    BOOST_CHECK( stmtAddUpdate->Initialize() == EErrorType_NONE );
                    int64_t id( 1 );
                    
                    for ( int i = 0; i < 10; i++ )
                    {
                        uint32_t index = 0;
                        stmtAddUpdate->SetParameterNull( index++ );
                        stmtAddUpdate->SetParameterNull( index++ );
                        stmtAddUpdate->SetParameterNull( index++ );
                        stmtAddUpdate->SetParameterValue( index++, id );
                        stmtAddUpdate->SetParameterValue( index++, STR( "Klingon_" ) + CStrUtils::ToString( i + count ) );
                        stmtAddUpdate->SetParameterValue( index++, STR( "Coin Glop Miaou" ) );
                        stmtAddUpdate->SetParameterValue( index++, STR( "18" ) );
                        stmtAddUpdate->SetParameterValue( index++, STR( "189" ) );
                        BOOST_CHECK( stmtAddUpdate->ExecuteUpdate() );
                        BOOST_CHECK_EQUAL( i + count, stmtAddUpdate->GetParameterValue< long long >( 0 ) );
                    }
                }
            }
        }
        
        template< typename T >
        static void TestStoredInOutDtParamNoReturn( DatabaseConnectionPtr connection )
        {
            uint8_t blobArray[] = { /*0x00, */0x02, 0x04, 0x06, 0x08, 0x10, 0x15, 0x20, 0x25, 0x30, 0x35, 0x40, 0x45, 0x50, 0x50, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0xF0, 0xFF };
            std::vector< uint8_t > blob;
            blob.insert( blob.end(), blobArray, blobArray + 25 );
            
            std::shared_ptr< T > stmtGetCount = DatabaseUtils::CreateStmt< T >( connection, STR( "SELECT MAX( ACTOR_ID ) AS ActorID FROM actors" ) );
            BOOST_CHECK( stmtGetCount );
            int64_t count = -1;
            
            if ( stmtGetCount )
            {
                BOOST_CHECK( stmtGetCount->Initialize() == EErrorType_NONE );
                Database::DatabaseResultPtr result = stmtGetCount->ExecuteSelect();
                BOOST_CHECK( result );
                BOOST_CHECK( result && result->GetRowCount() );
                
                if ( result && result->GetRowCount() )
                {
                    BOOST_CHECK_NO_THROW( result->GetFirstRow()->Get( 0, count ) );
                }
            }
            
            count++;
            
            if ( count )
            {
                std::shared_ptr< T > stmtAddUpdate = DatabaseUtils::CreateStmt< T >( connection, STR( "CALL SpAddUpdateActor(?,?,?,?,?,?,?);" ) );
                BOOST_CHECK( stmtAddUpdate );
                
                if ( stmtAddUpdate )
                {
                    BOOST_CHECK( stmtAddUpdate->CreateParameter( STR( "ACTOR_ID" ), Database::EFieldType_LONG_INTEGER, Database::EParameterType_INOUT ) );
                    BOOST_CHECK( stmtAddUpdate->CreateParameter( STR( "ACTOR_TYPE" ), Database::EFieldType_SMALL_INTEGER, Database::EParameterType_IN ) );
                    BOOST_CHECK( stmtAddUpdate->CreateParameter( STR( "ACTOR_NAME" ), Database::EFieldType_VARCHAR, 50, Database::EParameterType_IN ) );
                    BOOST_CHECK( stmtAddUpdate->CreateParameter( STR( "ACTOR_LOCATION" ), Database::EFieldType_VARCHAR, 255, Database::EParameterType_IN ) );
                    BOOST_CHECK( stmtAddUpdate->CreateParameter( STR( "ACTOR_PHONE_NUMBER" ), Database::EFieldType_VARCHAR, 50, Database::EParameterType_IN ) );
                    BOOST_CHECK( stmtAddUpdate->CreateParameter( STR( "ACTOR_LOGO" ), Database::EFieldType_BINARY, -1, Database::EParameterType_IN ) );
                    BOOST_CHECK( stmtAddUpdate->CreateParameter( STR( "ACTOR_CREATION_DATE" ), Database::EFieldType_DATETIME, Database::EParameterType_OUT ) );
                    BOOST_CHECK( stmtAddUpdate->Initialize() == EErrorType_NONE );
                    int16_t type( 1 );
                    
                    for ( int i = 0; i < 10; i++ )
                    {
                        uint32_t index = 0;
                        stmtAddUpdate->SetParameterNull( index++ );
                        stmtAddUpdate->SetParameterValue( index++, type );
                        stmtAddUpdate->SetParameterValue( index++, STR( "Acteur_" ) + CStrUtils::ToString( count + i ) );
                        
                        if ( i % 2 )
                        {
                            stmtAddUpdate->SetParameterValue( index++, Database::ToUtf8( L"Areva Intercontrôle", L"Windows-1252" ) );
                        }
                        else
                        {
                            stmtAddUpdate->SetParameterValue( index++, Database::ToUtf8( "Areva Intercontrôle", "Windows-1252" ) );
                        }
                        
                        stmtAddUpdate->SetParameterValue( index++, STR( "0123456789" ) );
                        stmtAddUpdate->SetParameterValue( index++, blob );
                        stmtAddUpdate->SetParameterNull( index++ );
                        BOOST_CHECK( stmtAddUpdate->ExecuteUpdate() );
                        BOOST_CHECK_EQUAL( stmtAddUpdate->GetParameterValue< long long >( 0 ), count + i );
                    }
                }
            }
        }
        
        template< typename T >
        static void TestStoredUpdate( DatabaseConnectionPtr connection )
        {
            std::shared_ptr< T > stmtLanguage = DatabaseUtils::CreateStmt< T >( connection, STR( "CALL SpAddUpdateLanguage(?,?,?,?)" ) );
            BOOST_CHECK( stmtLanguage );
            
            if ( stmtLanguage )
            {
                BOOST_CHECK( stmtLanguage->CreateParameter( STR( "id" ), EFieldType_LONG_INTEGER, EParameterType_INOUT ) );
                BOOST_CHECK( stmtLanguage->CreateParameter( STR( "name" ), EFieldType_VARCHAR, 5, EParameterType_IN ) );
                BOOST_CHECK( stmtLanguage->CreateParameter( STR( "desc" ), EFieldType_VARCHAR, 100, EParameterType_IN ) );
                BOOST_CHECK( stmtLanguage->CreateParameter( STR( "date" ), EFieldType_DATETIME, -1, EParameterType_OUT ) );
                BOOST_CHECK( stmtLanguage->Initialize() == EErrorType_NONE );
                uint32_t index = 0;
                int64_t id = 1;
                stmtLanguage->SetParameterValue( index++, id );
                stmtLanguage->SetParameterValue( index++, STR( "Kling" ) );
                stmtLanguage->SetParameterValue( index++, STR( "Start Trek Klingon" ) );
                stmtLanguage->SetParameterNull( index++ );
                BOOST_CHECK( stmtLanguage->ExecuteUpdate() );
            }
        }
        
        template< typename T >
        void PerfDirectSelectActors( DatabaseConnectionPtr connection, uint32_t testCount, const String & whereClause )
        {
            std::shared_ptr< T > stmtGetActors;
            
            if ( whereClause.empty() )
            {
                stmtGetActors = DatabaseUtils::CreateStmt< T >( connection, STR( "SELECT * FROM ACTORS" ) );
            }
            else
            {
                stmtGetActors = DatabaseUtils::CreateStmt< T >( connection, STR( "SELECT * FROM ACTORS " ) + whereClause );
            }
            
            if ( stmtGetActors )
            {
                stmtGetActors->Initialize();
                std::clock_t start = std::clock();
                DatabaseResultPtr result = stmtGetActors->ExecuteSelect();
                CLogger::LogMessage( StringStream() << "    Selected the actors in " << float( std::clock() - start ) / CLOCKS_PER_SEC << "seconds (no fetch)" );
                
                start = std::clock();
                uint32_t count = result->GetRowCount();
                
                if ( result && count )
                {
                    DatabaseRowPtr row = result->GetFirstRow();
                    
                    while ( row )
                    {
                        row = result->GetNextRow();
                    }
                }
                
                CLogger::LogMessage( StringStream() << "    Fetched " << count << " actors in " << float( std::clock() - start ) / CLOCKS_PER_SEC << "seconds" );
            }
        }
        
        template< typename T >
        void PerfStoredProcedureSelectActors( DatabaseConnectionPtr connection, uint32_t testCount, const String & whereClause )
        {
            std::shared_ptr< T > stmtGetActors = DatabaseUtils::CreateStmt< T >( connection, STR( "CALL SpGetActors(?)" ) );
            
            if ( stmtGetActors )
            {
                stmtGetActors->CreateParameter( STR( "Where" ), Database::EFieldType_VARCHAR, 500, Database::EParameterType_IN );
                stmtGetActors->Initialize();
                std::clock_t start = std::clock();
                stmtGetActors->SetParameterValue( 0, whereClause );
                DatabaseResultPtr result = stmtGetActors->ExecuteSelect();
                CLogger::LogMessage( StringStream() << "    Selected the actors in " << float( std::clock() - start ) / CLOCKS_PER_SEC << "seconds (no fetch)" );
                
                start = std::clock();
                uint32_t count = result->GetRowCount();
                
                if ( result && count )
                {
                    DatabaseRowPtr row = result->GetFirstRow();
                    
                    while ( row )
                    {
                        row = result->GetNextRow();
                    }
                }
                
                CLogger::LogMessage( StringStream() << "    Fetched " << count << " actors in " << float( std::clock() - start ) / CLOCKS_PER_SEC << "seconds" );
            }
        }
        
        template< typename T >
        void PerfDirectDeleteActors( DatabaseConnectionPtr connection, uint32_t testCount )
        {
            std::shared_ptr< T > stmtGetMin = DatabaseUtils::CreateStmt< T >( connection, STR( "SELECT MIN( ACTOR_ID ) AS ActorID FROM actors" ) );
            int64_t min = 0;
            
            if ( stmtGetMin )
            {
                stmtGetMin->Initialize();
                Database::DatabaseResultPtr result = stmtGetMin->ExecuteSelect();
                
                if ( result && result->GetRowCount() )
                {
                    result->GetFirstRow()->Get( 0, min );
                }
            }
            
            if ( min )
            {
                std::shared_ptr< T > stmtDelete = DatabaseUtils::CreateStmt< T >( connection, STR( "DELETE FROM ACTORS WHERE ACTOR_ID=?" ) );
                
                if ( stmtDelete )
                {
                    stmtDelete->CreateParameter( STR( "id" ), Database::EFieldType_LONG_INTEGER, Database::EParameterType_IN );
                    stmtDelete->Initialize();
                    const std::clock_t start = std::clock();
                    
                    for ( uint32_t i = 0; i < testCount; i++ )
                    {
                        int64_t id( min + i );
                        stmtDelete->SetParameterValue( 0, id );
                        stmtDelete->ExecuteUpdate();
                    }
                    
                    CLogger::LogMessage( StringStream() << "    Deleted " << testCount << " actors in " << float( std::clock() - start ) / CLOCKS_PER_SEC << "seconds" );
                }
            }
        }
        
        template< typename T >
        void PerfStoredProcedureDeleteActors( DatabaseConnectionPtr connection, uint32_t testCount )
        {
            std::shared_ptr< T > stmtGetMin = DatabaseUtils::CreateStmt< T >( connection, STR( "SELECT MIN( ACTOR_ID ) AS ActorID FROM actors" ) );
            int64_t min = 0;
            
            if ( stmtGetMin )
            {
                stmtGetMin->Initialize();
                Database::DatabaseResultPtr result = stmtGetMin->ExecuteSelect();
                
                if ( result && result->GetRowCount() )
                {
                    result->GetFirstRow()->Get( 0, min );
                }
            }
            
            if ( min )
            {
                std::shared_ptr< T > stmtDelete = DatabaseUtils::CreateStmt< T >( connection, STR( "CALL SpDeleteActor(?)" ) );
                
                if ( stmtDelete )
                {
                    stmtDelete->CreateParameter( STR( "id" ), Database::EFieldType_LONG_INTEGER, Database::EParameterType_IN );
                    stmtDelete->Initialize();
                    const std::clock_t start = std::clock();
                    
                    for ( uint32_t i = 0; i < testCount; i++ )
                    {
                        int64_t id( min + i );
                        stmtDelete->SetParameterValue( 0, id );
                        stmtDelete->ExecuteUpdate();
                    }
                    
                    CLogger::LogMessage( StringStream() << "    Deleted " << testCount << " actors in " << float( std::clock() - start ) / CLOCKS_PER_SEC << "seconds" );
                }
            }
        }
    }
}
END_NAMESPACE_DATABASE_TEST

#endif // ___DATABASE_TEST_UTILS_H___
