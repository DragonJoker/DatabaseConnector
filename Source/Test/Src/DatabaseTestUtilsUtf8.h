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
	namespace DatabaseUtilsUtf8
	{
		template< class StmtType >
		inline void CreateParameters( std::shared_ptr< StmtType > stmt )
		{
			BOOST_CHECK( stmt->CreateParameter( STR( "IntField" ), EFieldType_SINT32 ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "IntegerField" ), EFieldType_SINT32 ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "TinyIntField" ), EFieldType_UINT8 ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "SmallIntField" ), EFieldType_SINT16 ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "MediumIntField" ), EFieldType_SINT24 ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "BigIntField" ), EFieldType_SINT64 ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "Int2Field" ), EFieldType_SINT16 ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "Int8Field" ), EFieldType_SINT64 ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "RealField" ), EFieldType_FLOAT64 ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "DoubleField" ), EFieldType_FLOAT64 ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "DoublePrecisionField" ), EFieldType_FLOAT64 ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "FloatField" ), EFieldType_FLOAT32 ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "NumericField" ), EFieldType_FIXED_POINT, std::make_pair( 10, 0 ) ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "DecimalField" ), EFieldType_FIXED_POINT, std::make_pair( 10, 5 ) ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "BooleanField" ), EFieldType_BIT ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "DateField" ), EFieldType_DATE ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "DateTimeField" ), EFieldType_DATETIME ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "CharacterField" ), EFieldType_CHAR, 20 ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "VarcharField" ), EFieldType_VARCHAR, 255 ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "NcharField" ), EFieldType_NCHAR, 55 ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "NVarcharField" ), EFieldType_NVARCHAR, 100 ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "TextField" ), EFieldType_TEXT ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "BlobField" ), EFieldType_VARBINARY ) );
		}

		template< class StmtType >
		inline void SetParametersValue( uint32_t & index, int mult, int i, std::shared_ptr< StmtType > stmt )
		{
			stmt->SetParameterValue( index++, DatabaseUtils::Helpers< EFieldType_SINT32 >::InitialiseValue() );
			stmt->SetParameterValue( index++, DatabaseUtils::Helpers< EFieldType_SINT32 >::InitialiseValue() );
			stmt->SetParameterValue( index++, DatabaseUtils::Helpers< EFieldType_UINT8 >::InitialiseValue() );
			stmt->SetParameterValue( index++, DatabaseUtils::Helpers< EFieldType_SINT16 >::InitialiseValue() );
			stmt->SetParameterValue( index++, DatabaseUtils::Helpers< EFieldType_SINT24 >::InitialiseValue() );
			stmt->SetParameterValue( index++, DatabaseUtils::Helpers< EFieldType_SINT64 >::InitialiseValue() );
			stmt->SetParameterValue( index++, DatabaseUtils::Helpers< EFieldType_SINT16 >::InitialiseValue() );
			stmt->SetParameterValue( index++, DatabaseUtils::Helpers< EFieldType_SINT64 >::InitialiseValue() );
			stmt->SetParameterValue( index++, DatabaseUtils::Helpers< EFieldType_FLOAT64 >::InitialiseValue() );
			stmt->SetParameterValue( index++, DatabaseUtils::Helpers< EFieldType_FLOAT64 >::InitialiseValue() );
			stmt->SetParameterValue( index++, DatabaseUtils::Helpers< EFieldType_FLOAT64 >::InitialiseValue() );
			stmt->SetParameterValue( index++, DatabaseUtils::Helpers< EFieldType_FLOAT32 >::InitialiseValue() );
			stmt->SetParameterValue( index++, DatabaseUtils::Helpers< EFieldType_FIXED_POINT >::InitialiseValue( 10, 0 ) );
			stmt->SetParameterValue( index++, DatabaseUtils::Helpers< EFieldType_FIXED_POINT >::InitialiseValue( 10, 5 ) );
			stmt->SetParameterValue( index++, DatabaseUtils::Helpers< EFieldType_BIT >::InitialiseValue() );
			stmt->SetParameterValue( index++, DatabaseUtils::Helpers< EFieldType_DATE >::InitialiseValue() );
			stmt->SetParameterValue( index++, DatabaseUtils::Helpers< EFieldType_DATETIME >::InitialiseValue() );
			stmt->SetParameterValue( index++, "CHAR: Areva Intercontrôle " + std::to_string( mult * i + index ) );
			stmt->SetParameterValue( index++, "VARCHAR: Areva Intercontrôle " + std::to_string( mult * i + index ) );
			stmt->SetParameterValue( index++, "NCHAR: Areva Intercontrôle " + std::to_string( mult * i + index ) );
			stmt->SetParameterValue( index++, "NVARCHAR: Areva Intercontrôle " + std::to_string( mult * i + index ) );
			stmt->SetParameterValue( index++, "TEXT: Areva Intercontrôle " + std::to_string( mult * i + index ) );
			stmt->SetParameterValue( index++, DatabaseUtils::Helpers< EFieldType_VARBINARY >::InitialiseValue() );
		}

		inline void DisplayValues( uint32_t & index, DatabaseRowSPtr row )
		{
			CLogger::LogInfo( StringStream() << STR( "IntField : " ) << row->Get< int32_t >( index++ ) );
			CLogger::LogInfo( StringStream() << STR( "IntegerField : " ) << row->Get< int32_t >( index++ ) );
			CLogger::LogInfo( StringStream() << STR( "TinyIntField : " ) << int16_t( row->Get< int8_t >( index++ ) ) );
			CLogger::LogInfo( StringStream() << STR( "SmallIntField : " ) << row->Get< int16_t >( index++ ) );
			CLogger::LogInfo( StringStream() << STR( "MediumIntField : " ) << row->Get< int32_t >( index++ ) );
			CLogger::LogInfo( StringStream() << STR( "BigIntField : " ) << row->Get< int64_t >( index++ ) );
			CLogger::LogInfo( StringStream() << STR( "Int2Field : " ) << row->Get< int16_t >( index++ ) );
			CLogger::LogInfo( StringStream() << STR( "Int8Field : " ) << row->Get< int64_t >( index++ ) );
			CLogger::LogInfo( StringStream() << STR( "RealField : " ) << row->Get< double >( index++ ) );
			CLogger::LogInfo( StringStream() << STR( "DoubleField : " ) << row->Get< double >( index++ ) );
			CLogger::LogInfo( StringStream() << STR( "DoublePrecisionField : " ) << row->Get< double >( index++ ) );
			CLogger::LogInfo( StringStream() << STR( "FloatField : " ) << row->Get< float >( index++ ) );
			CLogger::LogInfo( StringStream() << STR( "NumericField : " ) << row->Get< CFixedPoint >( index++ ).ToString() );
			CLogger::LogInfo( StringStream() << STR( "DecimalField : " ) << row->Get< CFixedPoint >( index++ ).ToString() );
			CLogger::LogInfo( StringStream() << STR( "BooleanField : " ) << row->Get< bool >( index++ ) );
			CLogger::LogInfo( StringStream() << STR( "DateField : " ) << row->Get< CDate >( index++ ) );
			CLogger::LogInfo( StringStream() << STR( "DateTimeField : " ) << row->Get< DateTimeType >( index++ ) );
			CLogger::LogInfo( StringStream() << STR( "CharacterField : " ) << row->Get< std::string >( index++ ) );
			CLogger::LogInfo( StringStream() << STR( "VarcharField : " ) << row->Get< std::string >( index++ ) );
			CLogger::LogInfo( std::wstringstream() << L"NcharField : " << row->Get< std::wstring >( index++ ) );
			CLogger::LogInfo( std::wstringstream() << L"NVarcharField : " << row->Get< std::wstring >( index++ ) );
			CLogger::LogInfo( StringStream() << STR( "TextField : " ) << row->Get< std::string >( index++ ) );
			CLogger::LogInfo( StringStream() << STR( "BlobField : " ) << row->Get< ByteArray >( index++ ) );
		}

		template< typename StmtType >
		void PerfDirectInsertActors( DatabaseConnectionSPtr connection, uint32_t testCount )
		{
			try
			{
				ByteArray blob = { /*0x00, */0x02, 0x04, 0x06, 0x08, 0x10, 0x15, 0x20, 0x25, 0x30, 0x35, 0x40, 0x45, 0x50, 0x50, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0xF0, 0xFF };
				auto stmtGetCount = DatabaseUtils::CreateStmt< StmtType >( connection, QUERY_GET_COUNT );
				int64_t count = -1;

				if ( stmtGetCount )
				{
					stmtGetCount->Initialize();
					DatabaseResultSPtr result = stmtGetCount->ExecuteSelect();

					if ( result && result->GetRowCount() )
					{
						result->GetFirstRow()->Get( 0, count );
					}
				}

				count++;

				if ( count )
				{
					auto stmtInsert = DatabaseUtils::CreateStmt< StmtType >( connection, QUERY_INSERT_ELEMENT );

					if ( stmtInsert )
					{
						CreateParameters( stmtInsert );
						stmtInsert->Initialize();
						std::clock_t const start = std::clock();
						int16_t type( 1 );

						for ( uint32_t i = 0; i < testCount; i++ )
						{
							uint32_t index = 0;
							SetParametersValue( index, i + 20, i, stmtInsert );
							stmtInsert->ExecuteUpdate();
						}

						CLogger::LogInfo( StringStream() << "    Inserted " << testCount << " elements in " << float( std::clock() - start ) / CLOCKS_PER_SEC << "seconds" );
					}
				}
			}
			catch ( ... )
			{
				BOOST_CHECK( false );
			}
		}

		template< typename StmtType >
		void PerfStoredProcedureInsertActors( DatabaseConnectionSPtr connection, uint32_t testCount )
		{
			try
			{
				ByteArray blob = { /*0x00, */0x02, 0x04, 0x06, 0x08, 0x10, 0x15, 0x20, 0x25, 0x30, 0x35, 0x40, 0x45, 0x50, 0x50, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0xF0, 0xFF };
				auto stmtGetCount = DatabaseUtils::CreateStmt< StmtType >( connection, QUERY_GET_COUNT );
				int64_t count = -1;

				if ( stmtGetCount )
				{
					stmtGetCount->Initialize();
					DatabaseResultSPtr result = stmtGetCount->ExecuteSelect();

					if ( result && result->GetRowCount() )
					{
						result->GetFirstRow()->Get( 0, count );
					}
				}

				count++;

				if ( count )
				{
					auto stmtAddUpdate = DatabaseUtils::CreateStmt< StmtType >( connection, STR( "CALL SpAddUpdateActor(?,?,?,?,?,?,?);" ) );

					if ( stmtAddUpdate )
					{
						CreateParameters( stmtAddUpdate );
						stmtAddUpdate->Initialize();
						std::clock_t const start = std::clock();
						int16_t type( 1 );

						for ( uint32_t i = 0; i < testCount; i++ )
						{
							uint32_t index = 0;
							SetParametersValue( index, i + 20, i, stmtAddUpdate );
							stmtAddUpdate->ExecuteUpdate();
						}

						CLogger::LogInfo( StringStream() << "    Inserted " << testCount << " elements in " << float( std::clock() - start ) / CLOCKS_PER_SEC << "seconds" );
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
