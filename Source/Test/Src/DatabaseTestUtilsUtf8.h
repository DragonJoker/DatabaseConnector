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
	extern String const QUERY_SELECT_MIN;
	extern String const QUERY_GET_COUNT;
	extern String const QUERY_INSERT_ELEMENT;

	namespace DatabaseUtils
	{
		/** Used to stream a byte array into an std::istream
		*/
		struct membuf
				: std::streambuf
		{
			/** Constructor
			*/
			membuf( char * begin, char * end )
			{
				this->setg( begin, begin, end );
			}
		};

		template< typename T >
		void PerfDirectInsertActors( DatabaseConnectionPtr connection, uint32_t testCount )
		{
			try
			{
				std::vector< uint8_t > blob = { /*0x00, */0x02, 0x04, 0x06, 0x08, 0x10, 0x15, 0x20, 0x25, 0x30, 0x35, 0x40, 0x45, 0x50, 0x50, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0xF0, 0xFF };
				auto && stmtGetCount = DatabaseUtils::CreateStmt< T >( connection, QUERY_GET_COUNT );
				int64_t count = -1;

				if ( stmtGetCount )
				{
					stmtGetCount->Initialize();
					DatabaseResultPtr result = stmtGetCount->ExecuteSelect();

					if ( result && result->GetRowCount() )
					{
						result->GetFirstRow()->Get( 0, count );
					}
				}

				count++;

				if ( count )
				{
					auto && stmtInsert = DatabaseUtils::CreateStmt< T >( connection, QUERY_INSERT_ELEMENT );

					if ( stmtInsert )
					{
						BOOST_CHECK( stmtInsert->CreateParameter( STR( "IntField" ), EFieldType_INTEGER, EParameterType_IN ) );
						BOOST_CHECK( stmtInsert->CreateParameter( STR( "IntegerField" ), EFieldType_INTEGER, EParameterType_IN ) );
						BOOST_CHECK( stmtInsert->CreateParameter( STR( "TinyIntField" ), EFieldType_BOOL, EParameterType_IN ) );
						BOOST_CHECK( stmtInsert->CreateParameter( STR( "SmallIntField" ), EFieldType_SMALL_INTEGER, EParameterType_IN ) );
						BOOST_CHECK( stmtInsert->CreateParameter( STR( "MediumIntField" ), EFieldType_INTEGER, EParameterType_IN ) );
						BOOST_CHECK( stmtInsert->CreateParameter( STR( "BigIntField" ), EFieldType_LONG_INTEGER, EParameterType_IN ) );
						BOOST_CHECK( stmtInsert->CreateParameter( STR( "Int2Field" ), EFieldType_SMALL_INTEGER, EParameterType_IN ) );
						BOOST_CHECK( stmtInsert->CreateParameter( STR( "Int8Field" ), EFieldType_LONG_INTEGER, EParameterType_IN ) );
						BOOST_CHECK( stmtInsert->CreateParameter( STR( "RealField" ), EFieldType_FLOAT, EParameterType_IN ) );
						BOOST_CHECK( stmtInsert->CreateParameter( STR( "DoubleField" ), EFieldType_DOUBLE, EParameterType_IN ) );
						BOOST_CHECK( stmtInsert->CreateParameter( STR( "DoublePrecisionField" ), EFieldType_DOUBLE, EParameterType_IN ) );
						BOOST_CHECK( stmtInsert->CreateParameter( STR( "FloatField" ), EFieldType_FLOAT, EParameterType_IN ) );
						BOOST_CHECK( stmtInsert->CreateParameter( STR( "NumericField" ), EFieldType_DOUBLE, EParameterType_IN ) );
						BOOST_CHECK( stmtInsert->CreateParameter( STR( "DecimalField" ), EFieldType_DOUBLE, EParameterType_IN ) );
						BOOST_CHECK( stmtInsert->CreateParameter( STR( "BooleanField" ), EFieldType_BOOL, EParameterType_IN ) );
						BOOST_CHECK( stmtInsert->CreateParameter( STR( "DateField" ), EFieldType_DATE, EParameterType_IN ) );
						BOOST_CHECK( stmtInsert->CreateParameter( STR( "DateTimeField" ), EFieldType_DATETIME, EParameterType_IN ) );
						BOOST_CHECK( stmtInsert->CreateParameter( STR( "CharacterField" ), EFieldType_VARCHAR, 20, EParameterType_IN ) );
						BOOST_CHECK( stmtInsert->CreateParameter( STR( "VarcharField" ), EFieldType_VARCHAR, 255, EParameterType_IN ) );
						BOOST_CHECK( stmtInsert->CreateParameter( STR( "NcharField" ), EFieldType_NVARCHAR, 55, EParameterType_IN ) );
						BOOST_CHECK( stmtInsert->CreateParameter( STR( "NVarcharField" ), EFieldType_NVARCHAR, 100, EParameterType_IN ) );
						BOOST_CHECK( stmtInsert->CreateParameter( STR( "TextField" ), EFieldType_TEXT, EParameterType_IN ) );
						BOOST_CHECK( stmtInsert->CreateParameter( STR( "BlobField" ), EFieldType_VARBINARY, 25, EParameterType_IN ) );
						stmtInsert->Initialize();
						std::clock_t const start = std::clock();
						int16_t type( 1 );

						for ( uint32_t i = 0; i < testCount; i++ )
						{
							uint32_t index = 0;
							int16_t type( i + 20 );
							stmtUpdate->SetParameterValue( index++, int32_t( type * i ) );
							stmtUpdate->SetParameterValue( index++, int32_t( type * i + index ) );
							stmtUpdate->SetParameterValue( index++, ( i % 2 ) == 0 );
							stmtUpdate->SetParameterValue( index++, int16_t( type * i + index ) );
							stmtUpdate->SetParameterValue( index++, int32_t( type * i + index ) );
							stmtUpdate->SetParameterValue( index++, int64_t( type * i + index ) );
							stmtUpdate->SetParameterValue( index++, int16_t( type * i + index ) );
							stmtUpdate->SetParameterValue( index++, int64_t( type * i + index ) );
							stmtUpdate->SetParameterValue( index++, float( type * i + index ) );
							stmtUpdate->SetParameterValue( index++, double( type * i + index ) );
							stmtUpdate->SetParameterValue( index++, double( type * i + index ) );
							stmtUpdate->SetParameterValue( index++, float( type * i + index ) );
							stmtUpdate->SetParameterValue( index++, double( type * i + index ) );
							stmtUpdate->SetParameterValue( index++, double( type * i + index ) );
							stmtUpdate->SetParameterValue( index++, ( i % 2 ) == 1 );
							stmtUpdate->SetParameterValue( index++, CDate( 2015, EDateMonth_FEBRUARY, 27 ) );
							stmtUpdate->SetParameterValue( index++, CDateTime( CDate( 2015, EDateMonth_AUGUST, 12 ) ) );
							stmtUpdate->SetParameterValue( index++, "CHAR: Areva Intercontrôle" + std::to_string( type * i + index ) );
							stmtUpdate->SetParameterValue( index++, "VARCHAR: Areva Intercontrôle" + std::to_string( type * i + index ) );
							stmtUpdate->SetParameterValue( index++, L"NCHAR: Areva Intercontrôle" + CStrUtils::ToWStr( CStrUtils::ToString( type * i + index ) ) );
							stmtUpdate->SetParameterValue( index++, L"NVARCHAR: Areva Intercontrôle" + CStrUtils::ToWStr( CStrUtils::ToString( type * i + index ) ) );
							stmtUpdate->SetParameterValue( index++, "TEXT: Areva Intercontrôle" + CStrUtils::ToString( type * i + index ) );
							stmtUpdate->SetParameterValue( index++, blob );
							stmtUpdate->SetParameterValue( index++, min + i );
							stmtInsert->ExecuteUpdate();
						}

						CLogger::LogMessage( StringStream() << "    Inserted " << testCount << " elements in " << float( std::clock() - start ) / CLOCKS_PER_SEC << "seconds" );
					}
				}
			}
			catch ( ... )
			{
				BOOST_CHECK( false );
			}
		}

		template< typename T >
		void PerfStoredProcedureInsertActors( DatabaseConnectionPtr connection, uint32_t testCount )
		{
			try
			{
				std::vector< uint8_t > blob = { /*0x00, */0x02, 0x04, 0x06, 0x08, 0x10, 0x15, 0x20, 0x25, 0x30, 0x35, 0x40, 0x45, 0x50, 0x50, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0xF0, 0xFF };
				auto && stmtGetCount = DatabaseUtils::CreateStmt< T >( connection, QUERY_GET_COUNT );
				int64_t count = -1;

				if ( stmtGetCount )
				{
					stmtGetCount->Initialize();
					DatabaseResultPtr result = stmtGetCount->ExecuteSelect();

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
						BOOST_CHECK( stmtInsert->CreateParameter( STR( "IntField" ), EFieldType_INTEGER, EParameterType_IN ) );
						BOOST_CHECK( stmtInsert->CreateParameter( STR( "IntegerField" ), EFieldType_INTEGER, EParameterType_IN ) );
						BOOST_CHECK( stmtInsert->CreateParameter( STR( "TinyIntField" ), EFieldType_BOOL, EParameterType_IN ) );
						BOOST_CHECK( stmtInsert->CreateParameter( STR( "SmallIntField" ), EFieldType_SMALL_INTEGER, EParameterType_IN ) );
						BOOST_CHECK( stmtInsert->CreateParameter( STR( "MediumIntField" ), EFieldType_INTEGER, EParameterType_IN ) );
						BOOST_CHECK( stmtInsert->CreateParameter( STR( "BigIntField" ), EFieldType_LONG_INTEGER, EParameterType_IN ) );
						BOOST_CHECK( stmtInsert->CreateParameter( STR( "Int2Field" ), EFieldType_SMALL_INTEGER, EParameterType_IN ) );
						BOOST_CHECK( stmtInsert->CreateParameter( STR( "Int8Field" ), EFieldType_LONG_INTEGER, EParameterType_IN ) );
						BOOST_CHECK( stmtInsert->CreateParameter( STR( "RealField" ), EFieldType_FLOAT, EParameterType_IN ) );
						BOOST_CHECK( stmtInsert->CreateParameter( STR( "DoubleField" ), EFieldType_DOUBLE, EParameterType_IN ) );
						BOOST_CHECK( stmtInsert->CreateParameter( STR( "DoublePrecisionField" ), EFieldType_DOUBLE, EParameterType_IN ) );
						BOOST_CHECK( stmtInsert->CreateParameter( STR( "FloatField" ), EFieldType_FLOAT, EParameterType_IN ) );
						BOOST_CHECK( stmtInsert->CreateParameter( STR( "NumericField" ), EFieldType_DOUBLE, EParameterType_IN ) );
						BOOST_CHECK( stmtInsert->CreateParameter( STR( "DecimalField" ), EFieldType_DOUBLE, EParameterType_IN ) );
						BOOST_CHECK( stmtInsert->CreateParameter( STR( "BooleanField" ), EFieldType_BOOL, EParameterType_IN ) );
						BOOST_CHECK( stmtInsert->CreateParameter( STR( "DateField" ), EFieldType_DATE, EParameterType_IN ) );
						BOOST_CHECK( stmtInsert->CreateParameter( STR( "DateTimeField" ), EFieldType_DATETIME, EParameterType_IN ) );
						BOOST_CHECK( stmtInsert->CreateParameter( STR( "CharacterField" ), EFieldType_VARCHAR, 20, EParameterType_IN ) );
						BOOST_CHECK( stmtInsert->CreateParameter( STR( "VarcharField" ), EFieldType_VARCHAR, 255, EParameterType_IN ) );
						BOOST_CHECK( stmtInsert->CreateParameter( STR( "NcharField" ), EFieldType_NVARCHAR, 55, EParameterType_IN ) );
						BOOST_CHECK( stmtInsert->CreateParameter( STR( "NVarcharField" ), EFieldType_NVARCHAR, 100, EParameterType_IN ) );
						BOOST_CHECK( stmtInsert->CreateParameter( STR( "TextField" ), EFieldType_TEXT, EParameterType_IN ) );
						BOOST_CHECK( stmtInsert->CreateParameter( STR( "BlobField" ), EFieldType_VARBINARY, 25, EParameterType_IN ) );
						stmtAddUpdate->Initialize();
						std::clock_t const start = std::clock();
						int16_t type( 1 );

						for ( uint32_t i = 0; i < testCount; i++ )
						{
							uint32_t index = 0;
							int16_t type( i + 20 );
							stmtUpdate->SetParameterValue( index++, int32_t( type * i ) );
							stmtUpdate->SetParameterValue( index++, int32_t( type * i + index ) );
							stmtUpdate->SetParameterValue( index++, ( i % 2 ) == 0 );
							stmtUpdate->SetParameterValue( index++, int16_t( type * i + index ) );
							stmtUpdate->SetParameterValue( index++, int32_t( type * i + index ) );
							stmtUpdate->SetParameterValue( index++, int64_t( type * i + index ) );
							stmtUpdate->SetParameterValue( index++, int16_t( type * i + index ) );
							stmtUpdate->SetParameterValue( index++, int64_t( type * i + index ) );
							stmtUpdate->SetParameterValue( index++, float( type * i + index ) );
							stmtUpdate->SetParameterValue( index++, double( type * i + index ) );
							stmtUpdate->SetParameterValue( index++, double( type * i + index ) );
							stmtUpdate->SetParameterValue( index++, float( type * i + index ) );
							stmtUpdate->SetParameterValue( index++, double( type * i + index ) );
							stmtUpdate->SetParameterValue( index++, double( type * i + index ) );
							stmtUpdate->SetParameterValue( index++, ( i % 2 ) == 1 );
							stmtUpdate->SetParameterValue( index++, CDate( 2015, EDateMonth_FEBRUARY, 27 ) );
							stmtUpdate->SetParameterValue( index++, CDateTime( CDate( 2015, EDateMonth_AUGUST, 12 ) ) );
							stmtUpdate->SetParameterValue( index++, "CHAR: Areva Intercontrôle" + std::to_string( type * i + index ) );
							stmtUpdate->SetParameterValue( index++, "VARCHAR: Areva Intercontrôle" + std::to_string( type * i + index ) );
							stmtUpdate->SetParameterValue( index++, L"NCHAR: Areva Intercontrôle" + CStrUtils::ToWStr( CStrUtils::ToString( type * i + index ) ) );
							stmtUpdate->SetParameterValue( index++, L"NVARCHAR: Areva Intercontrôle" + CStrUtils::ToWStr( CStrUtils::ToString( type * i + index ) ) );
							stmtUpdate->SetParameterValue( index++, "TEXT: Areva Intercontrôle" + CStrUtils::ToString( type * i + index ) );
							stmtUpdate->SetParameterValue( index++, blob );
							stmtUpdate->SetParameterValue( index++, min + i );
							stmtAddUpdate->ExecuteUpdate();
						}

						CLogger::LogMessage( StringStream() << "    Inserted " << testCount << " elements in " << float( std::clock() - start ) / CLOCKS_PER_SEC << "seconds" );
					}
				}
			}
			catch ( ... )
			{
				BOOST_CHECK( false );
			}
		}
	}
}
END_NAMESPACE_DATABASE_TEST

#endif // ___DATABASE_TEST_MYSQL_UTILS_UTF8_H___
