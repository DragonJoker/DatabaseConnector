/************************************************************************//**
 * @file DatabaseTestUtilsUtf8.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 29/07/2014 10:55:21
 *
 *
 * @brief MySQL databases utilities, in UTF8
 *
 * @details This file is encoded in UTF8 without BOM, to have less conversions
 *
 ***************************************************************************/

#ifndef ___DATABASE_TEST_MYSQL_UTILS_UTF8_H___
#define ___DATABASE_TEST_MYSQL_UTILS_UTF8_H___

#include "DatabaseTestPrerequisites.h"
#include "DatabaseTestUtils.h"

#include <DatabaseConnection.h>
#include <DatabaseStatement.h>
#include <DatabaseQuery.h>

BEGIN_NAMESPACE_DATABASE_TEST
{
	namespace DatabaseUtils
	{
		/** Used to stream a byte array into an std::istream
		*/
		struct membuf
				: std::streambuf
		{
			/** Conectructor
			*/
			membuf( char * begin, char * end )
			{
				this->setg( begin, begin, end );
			}
		};

		template< typename T >
		void PerfDirectInsertActors( DatabaseConnectionPtr connection, uint32_t testCount )
		{
			uint8_t blobArray[] = { /*0x00, */0x02, 0x04, 0x06, 0x08, 0x10, 0x15, 0x20, 0x25, 0x30, 0x35, 0x40, 0x45, 0x50, 0x50, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0xF0, 0xFF };
			std::vector< uint8_t > blob;
			blob.insert( blob.end(), blobArray, blobArray + 25 );

			std::shared_ptr< T > stmtGetCount = DatabaseUtils::CreateStmt< T >( connection, STR( "SELECT MAX( ACTOR_ID ) AS ActorID FROM actors" ) );
			int64_t count = -1;

			if ( stmtGetCount )
			{
				stmtGetCount->Initialize();
				Database::DatabaseResultPtr result = stmtGetCount->ExecuteSelect();

				if ( result && result->GetRowCount() )
				{
					result->GetFirstRow()->Get( 0, count );
				}
			}

			count++;

			if ( count )
			{
				std::shared_ptr< T > stmtInsert = DatabaseUtils::CreateStmt< T >( connection, STR( "INSERT INTO actors (ACTOR_TYPE, ACTOR_NAME, ACTOR_LOCATION, ACTOR_LOGO, ACTOR_PHONE_NUMBER, ACTOR_CREATION_DATE, ACTOR_MODIFICATION_DATE) VALUES (?, ?, ?, ?, ?, NOW(), NOW())" ) );

				if ( stmtInsert )
				{
					DatabaseParameterPtr paramType = stmtInsert->CreateParameter( STR( "ACTOR_TYPE" ), Database::EFieldType_SMALL_INTEGER, Database::EParameterType_IN );
					DatabaseParameterPtr paramName = stmtInsert->CreateParameter( STR( "ACTOR_NAME" ), Database::EFieldType_VARCHAR, 50, Database::EParameterType_IN );
					DatabaseParameterPtr paramLoca = stmtInsert->CreateParameter( STR( "ACTOR_LOCATION" ), Database::EFieldType_VARCHAR, 255, Database::EParameterType_IN );
					DatabaseParameterPtr paramLogo = stmtInsert->CreateParameter( STR( "ACTOR_LOGO" ), Database::EFieldType_BINARY, -1, Database::EParameterType_IN );
					DatabaseParameterPtr paramPhon = stmtInsert->CreateParameter( STR( "ACTOR_PHONE_NUMBER" ), Database::EFieldType_VARCHAR, 50, Database::EParameterType_IN );
					stmtInsert->Initialize();
					const std::clock_t start = std::clock();
					int16_t type( 1 );

					for ( uint32_t i = 0; i < testCount; i++ )
					{
						paramType->SetValue( type );
						paramName->SetValue( "Acteur_" + std::to_string( count + i ) );
						paramLoca->SetValue( "Areva Intercontrôle" );
						paramLogo->SetValue( blob );
						paramPhon->SetValue( "0123456789" );
						stmtInsert->ExecuteUpdate();
					}

					CLogger::LogMessage( StringStream() << "    Inserted " << testCount << " actors in " << float( std::clock() - start ) / CLOCKS_PER_SEC << "seconds" );
				}
			}
		}

		template< typename T >
		void PerfStoredProcedureInsertActors( DatabaseConnectionPtr connection, uint32_t testCount )
		{
			uint8_t blobArray[] = { /*0x00, */0x02, 0x04, 0x06, 0x08, 0x10, 0x15, 0x20, 0x25, 0x30, 0x35, 0x40, 0x45, 0x50, 0x50, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0xF0, 0xFF };
			std::vector< uint8_t > blob;
			blob.insert( blob.end(), blobArray, blobArray + 25 );

			std::shared_ptr< T > stmtGetCount = DatabaseUtils::CreateStmt< T >( connection, STR( "SELECT MAX( ACTOR_ID ) AS ActorID FROM actors" ) );
			int64_t count = -1;

			if ( stmtGetCount )
			{
				stmtGetCount->Initialize();
				Database::DatabaseResultPtr result = stmtGetCount->ExecuteSelect();

				if ( result && result->GetRowCount() )
				{
					result->GetFirstRow()->Get( 0, count );
				}
			}

			count++;

			if ( count )
			{
				std::shared_ptr< T > stmtAddUpdate = DatabaseUtils::CreateStmt< T >( connection, STR( "CALL SpAddUpdateActor(?,?,?,?,?,?,?);" ) );

				if ( stmtAddUpdate )
				{
					DatabaseParameterPtr paramAcId = stmtAddUpdate->CreateParameter( STR( "ACTOR_ID" ), Database::EFieldType_LONG_INTEGER, Database::EParameterType_INOUT );
					DatabaseParameterPtr paramType = stmtAddUpdate->CreateParameter( STR( "ACTOR_TYPE" ), Database::EFieldType_SMALL_INTEGER, Database::EParameterType_IN );
					DatabaseParameterPtr paramName = stmtAddUpdate->CreateParameter( STR( "ACTOR_NAME" ), Database::EFieldType_VARCHAR, 50, Database::EParameterType_IN );
					DatabaseParameterPtr paramLoca = stmtAddUpdate->CreateParameter( STR( "ACTOR_LOCATION" ), Database::EFieldType_VARCHAR, 255, Database::EParameterType_IN );
					DatabaseParameterPtr paramPhon = stmtAddUpdate->CreateParameter( STR( "ACTOR_PHONE_NUMBER" ), Database::EFieldType_VARCHAR, 50, Database::EParameterType_IN );
					DatabaseParameterPtr paramLogo = stmtAddUpdate->CreateParameter( STR( "ACTOR_LOGO" ), Database::EFieldType_BINARY, -1, Database::EParameterType_IN );
					DatabaseParameterPtr paramDate = stmtAddUpdate->CreateParameter( STR( "ACTOR_CREATION_DATE" ), Database::EFieldType_DATETIME, Database::EParameterType_OUT );
					stmtAddUpdate->Initialize();
					const std::clock_t start = std::clock();
					int16_t type( 1 );

					for ( uint32_t i = 0; i < testCount; i++ )
					{
						paramAcId->SetNull();
						paramType->SetValue( type );
						paramName->SetValue( "Acteur_" + std::to_string( count + i ) );
						paramLoca->SetValue( "Areva Intercontrôle" );
						paramPhon->SetValue( "0123456789" );
						paramLogo->SetValue( blob );
						paramDate->SetNull();
						stmtAddUpdate->ExecuteUpdate();
					}

					CLogger::LogMessage( StringStream() << "    Inserted " << testCount << " actors in " << float( std::clock() - start ) / CLOCKS_PER_SEC << "seconds" );
				}
			}
		}
	}
}
END_NAMESPACE_DATABASE_TEST

#endif // ___DATABASE_TEST_MYSQL_UTILS_UTF8_H___
