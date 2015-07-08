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
#include <DatabaseResult.h>
#include <DatabaseRow.h>

BEGIN_NAMESPACE_DATABASE_TEST
{
	extern String const QUERY_SELECT_MIN;
	extern String const QUERY_GET_COUNT;
	extern String const QUERY_INSERT_ELEMENT;
	extern String const QUERY_DIRECT_SELECT_ELEMENT;
	extern String const QUERY_DIRECT_SELECT_ALL_ELEMENTS;
	extern String const QUERY_DIRECT_UPDATE_ELEMENT;

	/** Deletes a SQLite database.
	@param database
	    Database name.
	@return
	    Exit code of the CProcess executing the batch file.
	*/
	int UninstallSqliteDatabase( const String & database );

	/** Creates a MsSql database.
	@param database
	    Database name.
	@return
	    Exit code of the CProcess executing the batch file.
	*/
	int InstallMsSqlDatabase( const String & database );

	/** Deletes a MsSql database.
	@param database
	    Database name.
	@return
	    Exit code of the CProcess executing the batch file.
	*/
	int UninstallMsSqlDatabase( const String & database );

	/** Creates an ODBC source for a test database.
	@param dsn
	    ODBC source name.
	@return
	    Exit code of the CProcess executing the batch file.
	*/
	int InstallSourceOdbc( const String & dsn );

	/** Deletes an ODBC source for a test database.
	@param dsn
	    ODBC source name.
	@return
	    Exit code of the CProcess executing the batch file.
	*/
	int UninstallSourceOdbc( const String & dsn );

	namespace DatabaseUtils
	{
		template< typename T >
		std::shared_ptr< T > CreateStmt( DatabaseConnectionPtr connection, const String & query );

		template<>
		inline std::shared_ptr< CDatabaseStatement > CreateStmt< CDatabaseStatement >( DatabaseConnectionPtr connection, const String & query )
		{
			std::shared_ptr< CDatabaseStatement > result;

			try
			{
				result = connection->CreateStatement( query );
			}
			catch ( std::exception & e )
			{
				CLogger::LogMessage( e.what() );
			}

			return result;
		}

		template<>
		inline std::shared_ptr< CDatabaseQuery > CreateStmt< CDatabaseQuery >( DatabaseConnectionPtr connection, const String & query )
		{
			std::shared_ptr< CDatabaseQuery > result;

			try
			{
				result = connection->CreateQuery( query );
			}
			catch ( std::exception & e )
			{
				CLogger::LogMessage( e.what() );
			}

			return result;
		}

		template< typename Type > struct Helpers
		{
			static const uint32_t Limit = 0;
			Type InitialiseValue() = delete;
		};

		template<> struct Helpers< bool >
		{
			static const uint32_t Limit = -1;
			typedef bool Type;

			static Type InitialiseValue()
			{
				return bool( 0 == rand() % 2 );
			}
		};

		template<> struct Helpers< int16_t >
		{
			static const uint32_t Limit = -1;
			typedef int16_t Type;

			static Type InitialiseValue()
			{
				return int16_t( rand() );
			}
		};

		template<> struct Helpers< int32_t >
		{
			static const uint32_t Limit = -1;
			typedef int32_t Type;

			static Type InitialiseValue()
			{
				return int32_t( rand() );
			}
		};

		template<> struct Helpers< int64_t >
		{
			static const uint32_t Limit = -1;
			typedef int64_t Type;

			static Type InitialiseValue()
			{
				return int64_t( rand() );
			}
		};

		template<> struct Helpers< float >
		{
			static const uint32_t Limit = -1;
			typedef float Type;

			static Type InitialiseValue()
			{
				return float( rand() ) / rand();
			}
		};

		template<> struct Helpers< double >
		{
			static const uint32_t Limit = -1;
			typedef double Type;

			static Type InitialiseValue()
			{
				return double( rand() ) / rand();
			}
		};

		template<> struct Helpers< CDate >
		{
			static const uint32_t Limit = -1;
			typedef CDate Type;

			static Type InitialiseValue()
			{
				return CDate::Now();
			}
		};

		template<> struct Helpers< CDateTime >
		{
			static const uint32_t Limit = -1;
			typedef CDateTime Type;

			static Type InitialiseValue()
			{
				return CDateTime::Now();
			}
		};

		template<> struct Helpers< CTime >
		{
			static const uint32_t Limit = -1;
			typedef CTime Type;

			static Type InitialiseValue()
			{
				return CTime::Now();
			}
		};

		template<> struct Helpers< char * >
		{
			static const uint32_t Limit = 20;
			typedef std::string Type;

			static Type InitialiseValue()
			{
				static char l_return[] = "Bonjour, comment va?";
				return l_return;
			}
		};

		template<> struct Helpers< wchar_t * >
		{
			static const uint32_t Limit = 55;
			typedef std::wstring Type;

			static Type InitialiseValue()
			{
				static wchar_t l_return[] = L"Ca va bien, et toi?";
				return l_return;
			}
		};

		template<> struct Helpers< std::string >
		{
			static const uint32_t Limit = -1;
			typedef std::string Type;

			static Type InitialiseValue()
			{
				return std::string( "\n\
				template< class Stmt, typename Type >\n\
				inline void InsertAndRetrieve( DatabaseConnectionPtr connection, const String & name )\n\
				{\n\
					try\n\
					{\n\
						auto && stmtInsert = DatabaseUtils::CreateStmt< T >( connection, STR( \"INSERT INTO Test (\" ) + name + STR( \") VALUES (?)\" ) );\n\
						auto && stmtSelect = DatabaseUtils::CreateStmt< T >( connection, STR( \"SELECT \" ) + name + STR( \"FROM Test WHERE name = ?\" ) );\n\
						Type valueIn = InitialiseValue< Type >();\n\
					}\n\
					catch ( ... )\n\
					{\n\
						BOOST_CHECK( false );\n\
					}\n\
				}" );
			}
		};

		template<> struct Helpers< std::vector< uint8_t > >
		{
			static const uint32_t Limit = 32;
			typedef std::vector< uint8_t > Type;

			static Type InitialiseValue()
			{
				std::vector< uint8_t > blob =
				{
					0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
					0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF
				};
				return blob;
			}
		};

		template< class Stmt >
		inline void CreateParameters( std::shared_ptr< Stmt > stmt )
		{
			BOOST_CHECK( stmt->CreateParameter( STR( "IntField" ), EFieldType_INTEGER, EParameterType_IN ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "IntegerField" ), EFieldType_INTEGER, EParameterType_IN ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "TinyIntField" ), EFieldType_BOOL, EParameterType_IN ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "SmallIntField" ), EFieldType_SMALL_INTEGER, EParameterType_IN ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "MediumIntField" ), EFieldType_INTEGER, EParameterType_IN ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "BigIntField" ), EFieldType_LONG_INTEGER, EParameterType_IN ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "Int2Field" ), EFieldType_SMALL_INTEGER, EParameterType_IN ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "Int8Field" ), EFieldType_LONG_INTEGER, EParameterType_IN ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "RealField" ), EFieldType_FLOAT, EParameterType_IN ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "DoubleField" ), EFieldType_DOUBLE, EParameterType_IN ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "DoublePrecisionField" ), EFieldType_DOUBLE, EParameterType_IN ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "FloatField" ), EFieldType_FLOAT, EParameterType_IN ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "NumericField" ), EFieldType_INTEGER, EParameterType_IN ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "DecimalField" ), EFieldType_DOUBLE, EParameterType_IN ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "BooleanField" ), EFieldType_BOOL, EParameterType_IN ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "DateField" ), EFieldType_DATE, EParameterType_IN ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "DateTimeField" ), EFieldType_DATETIME, EParameterType_IN ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "CharacterField" ), EFieldType_VARCHAR, 20, EParameterType_IN ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "VarcharField" ), EFieldType_VARCHAR, 255, EParameterType_IN ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "NcharField" ), EFieldType_NVARCHAR, 55, EParameterType_IN ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "NVarcharField" ), EFieldType_NVARCHAR, 100, EParameterType_IN ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "TextField" ), EFieldType_TEXT, EParameterType_IN ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "BlobField" ), EFieldType_VARBINARY, 32, EParameterType_IN ) );
		}

		template< class Stmt >
		inline void SetParametersValue( uint32_t & index, int mult, int i, std::shared_ptr< Stmt > stmt )
		{
			stmt->SetParameterValue( index++, Helpers< int32_t >::InitialiseValue() );
			stmt->SetParameterValue( index++, Helpers< int32_t >::InitialiseValue() );
			stmt->SetParameterValue( index++, Helpers< bool >::InitialiseValue() );
			stmt->SetParameterValue( index++, Helpers< int16_t >::InitialiseValue() );
			stmt->SetParameterValue( index++, Helpers< int32_t >::InitialiseValue() );
			stmt->SetParameterValue( index++, Helpers< int64_t >::InitialiseValue() );
			stmt->SetParameterValue( index++, Helpers< int16_t >::InitialiseValue() );
			stmt->SetParameterValue( index++, Helpers< int64_t >::InitialiseValue() );
			stmt->SetParameterValue( index++, Helpers< float >::InitialiseValue() );
			stmt->SetParameterValue( index++, Helpers< double >::InitialiseValue() );
			stmt->SetParameterValue( index++, Helpers< double >::InitialiseValue() );
			stmt->SetParameterValue( index++, Helpers< float >::InitialiseValue() );
			stmt->SetParameterValue( index++, Helpers< int32_t >::InitialiseValue() );
			stmt->SetParameterValue( index++, Helpers< double >::InitialiseValue() );
			stmt->SetParameterValue( index++, Helpers< bool >::InitialiseValue() );
			stmt->SetParameterValue( index++, Helpers< CDate >::InitialiseValue() );
			stmt->SetParameterValue( index++, Helpers< CDateTime >::InitialiseValue() );
			stmt->SetParameterValue( index++, Helpers< char * >::InitialiseValue() );
			stmt->SetParameterValue( index++, Helpers< char * >::InitialiseValue() );
			stmt->SetParameterValue( index++, Helpers< wchar_t * >::InitialiseValue() );
			stmt->SetParameterValue( index++, ToUtf8( "NVARCHAR: Areva Intercontr\\00\\F4le", "Windows-1252" ) );
			stmt->SetParameterValue( index++, Helpers< std::string >::InitialiseValue() );
			stmt->SetParameterValue( index++, Helpers< std::vector< uint8_t > >::InitialiseValue() );
		}

		inline void DisplayValues( uint32_t & index, DatabaseRowPtr row )
		{
			CLogger::LogMessage( StringStream() << STR( "IntField : " ) << row->Get< int32_t >( index++ ) );
			CLogger::LogMessage( StringStream() << STR( "IntegerField : " ) << row->Get< int32_t >( index++ ) );
			CLogger::LogMessage( StringStream() << STR( "TinyIntField : " ) << row->Get< bool >( index++ ) );
			CLogger::LogMessage( StringStream() << STR( "SmallIntField : " ) << row->Get< int16_t >( index++ ) );
			CLogger::LogMessage( StringStream() << STR( "MediumIntField : " ) << row->Get< int32_t >( index++ ) );
			CLogger::LogMessage( StringStream() << STR( "BigIntField : " ) << row->Get< int64_t >( index++ ) );
			CLogger::LogMessage( StringStream() << STR( "Int2Field : " ) << row->Get< int16_t >( index++ ) );
			CLogger::LogMessage( StringStream() << STR( "Int8Field : " ) << row->Get< int64_t >( index++ ) );
			CLogger::LogMessage( StringStream() << STR( "RealField : " ) << row->Get< float >( index++ ) );
			CLogger::LogMessage( StringStream() << STR( "DoubleField : " ) << row->Get< double >( index++ ) );
			CLogger::LogMessage( StringStream() << STR( "DoublePrecisionField : " ) << row->Get< double >( index++ ) );
			CLogger::LogMessage( StringStream() << STR( "FloatField : " ) << row->Get< float >( index++ ) );
			CLogger::LogMessage( StringStream() << STR( "NumericField : " ) << row->Get< int32_t >( index++ ) );
			CLogger::LogMessage( StringStream() << STR( "DecimalField : " ) << row->Get< double >( index++ ) );
			CLogger::LogMessage( StringStream() << STR( "BooleanField : " ) << row->Get< bool >( index++ ) );
			CLogger::LogMessage( StringStream() << STR( "DateField : " ) << row->Get< CDate >( index++ ) );
			CLogger::LogMessage( StringStream() << STR( "DateTimeField : " ) << row->Get< CDateTime >( index++ ) );
			CLogger::LogMessage( StringStream() << STR( "CharacterField : " ) << row->Get< std::string >( index++ ) );
			CLogger::LogMessage( StringStream() << STR( "VarcharField : " ) << row->Get< std::string >( index++ ) );
			CLogger::LogMessage( std::wstringstream() << L"NcharField : " << row->Get< std::wstring >( index++ ) );
			CLogger::LogMessage( std::wstringstream() << L"NVarcharField : " << row->Get< std::wstring >( index++ ) );
			CLogger::LogMessage( StringStream() << STR( "TextField : " ) << row->Get< std::string >( index++ ) );
			CLogger::LogMessage( StringStream() << STR( "BlobField : " ) << row->Get< std::vector< uint8_t > >( index++ ) );
		}

		template< class Stmt, typename Type >
		inline void InsertAndRetrieve( DatabaseConnectionPtr connection, const String & name )
		{
			try
			{
				auto && stmtInsert = DatabaseUtils::CreateStmt< Stmt >( connection, STR( "INSERT INTO Test (" ) + name + STR( ") VALUES (?)" ) );
				auto && stmtSelect = DatabaseUtils::CreateStmt< Stmt >( connection, STR( "SELECT " ) + name + STR( " FROM Test WHERE " ) + name + STR( " = ?" ) );
				BOOST_CHECK( stmtInsert );
				BOOST_CHECK( stmtSelect );

				if ( stmtInsert && stmtSelect )
				{
					BOOST_CHECK( stmtInsert->CreateParameter( name, SDataTypeFieldTyper< Type >::Value, Helpers< Type >::Limit, EParameterType_IN ) );
					BOOST_CHECK( stmtSelect->CreateParameter( name, SDataTypeFieldTyper< Type >::Value, Helpers< Type >::Limit, EParameterType_IN ) );

					BOOST_CHECK( stmtInsert->Initialize() == EErrorType_NONE );
					BOOST_CHECK( stmtSelect->Initialize() == EErrorType_NONE );

					Helpers< Type >::Type valueIn = Helpers< Type >::InitialiseValue();
					stmtInsert->SetParameterValue( 0, valueIn );
					stmtSelect->SetParameterValue( 0, valueIn );

					BOOST_CHECK( stmtInsert->ExecuteUpdate() );
					DatabaseResultPtr result = stmtSelect->ExecuteSelect();
					BOOST_CHECK( result );
					BOOST_CHECK( result && result->GetRowCount() );

					if ( result && result->GetRowCount() )
					{
						Helpers< Type >::Type valueOut;
						BOOST_CHECK_NO_THROW( result->GetFirstRow()->Get( 0, valueOut ) );
						BOOST_CHECK( valueIn == valueOut );
					}
					else
					{
						BOOST_CHECK( false );
					}
				}
			}
			catch ( ... )
			{
				BOOST_CHECK( false );
			}
		}

		template< typename T >
		inline void TestDirectInsert( DatabaseConnectionPtr connection )
		{
			try
			{
				auto && stmtGetCount = DatabaseUtils::CreateStmt< T >( connection, QUERY_GET_COUNT );
				BOOST_CHECK( stmtGetCount );
				int64_t count = -1;

				if ( stmtGetCount )
				{
					BOOST_CHECK( stmtGetCount->Initialize() == EErrorType_NONE );
					DatabaseResultPtr result = stmtGetCount->ExecuteSelect();
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
					auto && stmtInsert = DatabaseUtils::CreateStmt< T >( connection, QUERY_INSERT_ELEMENT );
					BOOST_CHECK( stmtInsert );

					if ( stmtInsert )
					{
						CreateParameters( stmtInsert );
						BOOST_CHECK( stmtInsert->Initialize() == EErrorType_NONE );
						int const inserts = 20;

						for ( int i = 0; i < inserts; i++ )
						{
							uint32_t index = 0;
							SetParametersValue( index, i + 1, i, stmtInsert );
							BOOST_CHECK( stmtInsert->ExecuteUpdate() );
						}

						count--;
						DatabaseResultPtr result = stmtGetCount->ExecuteSelect();
						BOOST_CHECK( result );
						BOOST_CHECK( result && result->GetRowCount() );

						if ( result && result->GetRowCount() )
						{
							int64_t field = 0;
							BOOST_CHECK_NO_THROW( result->GetFirstRow()->Get( 0, field ) );
							BOOST_CHECK_EQUAL( field, count + inserts );
							count++;
						}
					}
				}
			}
			catch ( ... )
			{
				BOOST_CHECK( false );
			}
		}

		template< typename T >
		inline void TestDirectSelect( DatabaseConnectionPtr connection )
		{
			try
			{
				auto && stmtSelect = DatabaseUtils::CreateStmt< T >( connection, QUERY_DIRECT_SELECT_ALL_ELEMENTS );
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
							DisplayValues( index, row );
							row = result->GetNextRow();
						}
					}
				}
			}
			catch ( ... )
			{
				BOOST_CHECK( false );
			}
		}

		template< typename T >
		inline void TestDirectUpdate( DatabaseConnectionPtr connection )
		{
			try
			{
				auto && stmtGetMin = DatabaseUtils::CreateStmt< T >( connection, QUERY_SELECT_MIN );
				BOOST_CHECK( stmtGetMin );
				int64_t min = -1;

				if ( stmtGetMin )
				{
					BOOST_CHECK( stmtGetMin->Initialize() == EErrorType_NONE );
					DatabaseResultPtr result = stmtGetMin->ExecuteSelect();
					BOOST_CHECK( result );
					BOOST_CHECK( result && result->GetRowCount() );

					if ( result && result->GetRowCount() )
					{
						BOOST_CHECK_NO_THROW( result->GetFirstRow()->Get( 0, min ) );
					}
				}

				if ( min )
				{
					auto && stmtUpdate = DatabaseUtils::CreateStmt< T >( connection, QUERY_DIRECT_UPDATE_ELEMENT );
					BOOST_CHECK( stmtUpdate );

					if ( stmtUpdate )
					{
						CreateParameters( stmtUpdate );
						BOOST_CHECK( stmtUpdate->CreateParameter( STR( "IDTest" ), EFieldType_LONG_INTEGER, EParameterType_IN ) );
						BOOST_CHECK( stmtUpdate->Initialize() == EErrorType_NONE );

						for ( int i = 0; i < 10; i++ )
						{
							uint32_t index = 0;
							SetParametersValue( index, i + 40, i, stmtUpdate );
							stmtUpdate->SetParameterValue( index++, min + i );
							BOOST_CHECK( stmtUpdate->ExecuteUpdate() );
						}
					}
				}
			}
			catch ( ... )
			{
				BOOST_CHECK( false );
			}
		}

		template< typename T >
		inline void TestDirectDelete( DatabaseConnectionPtr connection )
		{
			try
			{
				auto && stmtGetMin = DatabaseUtils::CreateStmt< T >( connection, QUERY_SELECT_MIN );
				BOOST_CHECK( stmtGetMin );
				int64_t min = -1;

				if ( stmtGetMin )
				{
					BOOST_CHECK( stmtGetMin->Initialize() == EErrorType_NONE );
					DatabaseResultPtr result = stmtGetMin->ExecuteSelect();
					BOOST_CHECK( result );
					BOOST_CHECK( result && result->GetRowCount() );

					if ( result && result->GetRowCount() )
					{
						BOOST_CHECK_NO_THROW( result->GetFirstRow()->Get( 0, min ) );
					}
				}

				if ( min )
				{
					auto && stmtDelete = DatabaseUtils::CreateStmt< T >( connection, QUERY_DIRECT_DELETE_ELEMENT );
					BOOST_CHECK( stmtDelete );

					if ( stmtDelete )
					{
						BOOST_CHECK( stmtDelete->CreateParameter( STR( "IDTest" ), EFieldType_LONG_INTEGER, EParameterType_IN ) );
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
			catch ( ... )
			{
				BOOST_CHECK( false );
			}
		}

		template< typename T >
		inline void TestStoredNoParamNoReturn( DatabaseConnectionPtr connection )
		{
			try
			{
				auto && stmtClear = DatabaseUtils::CreateStmt< T >( connection, STR( "CALL ClearElementsTables" ) );
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
			catch ( ... )
			{
				BOOST_CHECK( false );
			}
		}

		template< typename T >
		inline void TestStoredNoParamReturn( DatabaseConnectionPtr connection, const String & where )
		{
			try
			{
				auto && stmtGetElements = DatabaseUtils::CreateStmt< T >( connection, STR( "CALL GetElements(?)" ) );
				BOOST_CHECK( stmtGetElements );

				if ( stmtGetElements )
				{
					BOOST_CHECK( stmtGetElements->CreateParameter( STR( "Where" ), EFieldType_VARCHAR, 500, EParameterType_IN ) );
					BOOST_CHECK( stmtGetElements->Initialize() == EErrorType_NONE );

					for ( int i = 0; i < 2; i++ )
					{
						stmtGetElements->SetParameterValue( 0, where );
						DatabaseResultPtr result = stmtGetElements->ExecuteSelect();
						BOOST_CHECK( result );
						BOOST_CHECK( result && result->GetRowCount() );

						if ( result && result->GetRowCount() )
						{
							DatabaseRowPtr row = result->GetFirstRow();

							while ( row )
							{
								uint32_t index = 0;
								DisplayValues( index, row );
								row = result->GetNextRow();
							}
						}
					}
				}
			}
			catch ( ... )
			{
				BOOST_CHECK( false );
			}
		}

		template< typename T >
		inline void TestStoredInParamNoReturn( DatabaseConnectionPtr connection )
		{
			try
			{
				auto && stmtGetMin = DatabaseUtils::CreateStmt< T >( connection, QUERY_SELECT_MIN );
				BOOST_CHECK( stmtGetMin );
				int64_t min = 0;

				if ( stmtGetMin )
				{
					BOOST_CHECK( stmtGetMin->Initialize() == EErrorType_NONE );
					DatabaseResultPtr result = stmtGetMin->ExecuteSelect();
					BOOST_CHECK( result );
					BOOST_CHECK( result && result->GetRowCount() );

					if ( result && result->GetRowCount() )
					{
						BOOST_CHECK_NO_THROW( result->GetFirstRow()->Get( 0, min ) );
					}
				}

				if ( min )
				{
					auto && stmtDelete = DatabaseUtils::CreateStmt< T >( connection, STR( "CALL DeleteElements(?)" ) );
					BOOST_CHECK( stmtDelete );

					if ( stmtDelete )
					{
						BOOST_CHECK( stmtDelete->CreateParameter( STR( "IDTest" ), EFieldType_LONG_INTEGER, EParameterType_IN ) );
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
			catch ( ... )
			{
				BOOST_CHECK( false );
			}
		}

		template< typename T >
		inline void TestStoredInOutParamNoReturn( DatabaseConnectionPtr connection )
		{
			try
			{
				auto && stmtGetCount = DatabaseUtils::CreateStmt< T >( connection, QUERY_GET_COUNT );
				BOOST_CHECK( stmtGetCount );
				int64_t count = -1;

				if ( stmtGetCount )
				{
					BOOST_CHECK( stmtGetCount->Initialize() == EErrorType_NONE );
					DatabaseResultPtr result = stmtGetCount->ExecuteSelect();
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
					auto && stmtAddElement = DatabaseUtils::CreateStmt< T >( connection, STR( "CALL AddElement(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);" ) );
					BOOST_CHECK( stmtAddElement );

					if ( stmtAddElement )
					{
						BOOST_CHECK( stmtAddElement->CreateParameter( STR( "IDTest" ), EFieldType_LONG_INTEGER, EParameterType_INOUT ) );
						CreateParameters( stmtAddElement );
						BOOST_CHECK( stmtAddElement->Initialize() == EErrorType_NONE );
						int64_t id( 1 );

						for ( int i = 0; i < 10; i++ )
						{
							uint32_t index = 0;
							stmtAddElement->SetParameterNull( index++ );
							SetParametersValue( index, i + 40, i, stmtAddElement );
							BOOST_CHECK( stmtAddElement->ExecuteUpdate() );
							int64_t tmp;
							stmtAddElement->GetOutputValue( 0, tmp );
							BOOST_CHECK_EQUAL( i + count, tmp );
						}
					}
				}
			}
			catch ( ... )
			{
				BOOST_CHECK( false );
			}
		}

		template< typename T >
		inline void TestStoredInOutDtParamNoReturn( DatabaseConnectionPtr connection )
		{
			try
			{
				auto && stmtGetCount = DatabaseUtils::CreateStmt< T >( connection, QUERY_GET_COUNT );
				BOOST_CHECK( stmtGetCount );
				int64_t count = -1;

				if ( stmtGetCount )
				{
					BOOST_CHECK( stmtGetCount->Initialize() == EErrorType_NONE );
					DatabaseResultPtr result = stmtGetCount->ExecuteSelect();
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
					auto && stmtUpdateElement = DatabaseUtils::CreateStmt< T >( connection, STR( "CALL UpdateElement(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);" ) );
					BOOST_CHECK( stmtUpdateElement );

					if ( stmtUpdateElement )
					{
						BOOST_CHECK( stmtUpdateElement->CreateParameter( STR( "IDTest" ), EFieldType_LONG_INTEGER, EParameterType_INOUT ) );
						CreateParameters( stmtUpdateElement );
						BOOST_CHECK( stmtUpdateElement->CreateParameter( STR( "Date" ), EFieldType_DATETIME, EParameterType_OUT ) );
						BOOST_CHECK( stmtUpdateElement->Initialize() == EErrorType_NONE );
						int16_t type( 1 );

						for ( int i = 0; i < 10; i++ )
						{
							uint32_t index = 0;
							stmtUpdateElement->SetParameterNull( index++ );
							SetParametersValue( index, i + 40, i, stmtUpdateElement );
							stmtUpdateElement->SetParameterNull( index++ );
							BOOST_CHECK( stmtUpdateElement->ExecuteUpdate() );
							int64_t tmp;
							stmtUpdateElement->GetOutputValue( 0, tmp );
							BOOST_CHECK_EQUAL( i + count, tmp );
						}
					}
				}
			}
			catch ( ... )
			{
				BOOST_CHECK( false );
			}
		}

		template< typename T >
		inline void PerfDirectSelectActors( DatabaseConnectionPtr connection, uint32_t testCount, const String & whereClause )
		{
			try
			{
				std::shared_ptr< T > stmtGetActors;

				if ( whereClause.empty() )
				{
					stmtGetActors = DatabaseUtils::CreateStmt< T >( connection, QUERY_DIRECT_SELECT_ALL_ELEMENTS );
				}
				else
				{
					stmtGetActors = DatabaseUtils::CreateStmt< T >( connection, QUERY_DIRECT_SELECT_ALL_ELEMENTS + STR( " " ) + whereClause );
				}

				if ( stmtGetActors )
				{
					stmtGetActors->Initialize();
					std::clock_t start = std::clock();
					DatabaseResultPtr result = stmtGetActors->ExecuteSelect();
					CLogger::LogMessage( StringStream() << "    Selected the elements in " << float( std::clock() - start ) / CLOCKS_PER_SEC << "seconds (no fetch)" );

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

					CLogger::LogMessage( StringStream() << "    Fetched " << count << " elements in " << float( std::clock() - start ) / CLOCKS_PER_SEC << "seconds" );
				}
			}
			catch ( ... )
			{
				BOOST_CHECK( false );
			}
		}

		template< typename T >
		inline void PerfStoredProcedureSelectActors( DatabaseConnectionPtr connection, uint32_t testCount, const String & whereClause )
		{
			try
			{
				auto && stmtGetActors = DatabaseUtils::CreateStmt< T >( connection, STR( "CALL SpGetActors(?)" ) );

				if ( stmtGetActors )
				{
					stmtGetActors->CreateParameter( STR( "Where" ), EFieldType_VARCHAR, 500, EParameterType_IN );
					stmtGetActors->Initialize();
					std::clock_t start = std::clock();
					stmtGetActors->SetParameterValue( 0, whereClause );
					DatabaseResultPtr result = stmtGetActors->ExecuteSelect();
					CLogger::LogMessage( StringStream() << "    Selected the elements in " << float( std::clock() - start ) / CLOCKS_PER_SEC << "seconds (no fetch)" );

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

					CLogger::LogMessage( StringStream() << "    Fetched " << count << " elements in " << float( std::clock() - start ) / CLOCKS_PER_SEC << "seconds" );
				}
			}
			catch ( ... )
			{
				BOOST_CHECK( false );
			}
		}

		template< typename T >
		inline void PerfDirectDeleteActors( DatabaseConnectionPtr connection, uint32_t testCount )
		{
			try
			{
				auto && stmtGetMin = DatabaseUtils::CreateStmt< T >( connection, QUERY_SELECT_MIN );
				int64_t min = 0;

				if ( stmtGetMin )
				{
					stmtGetMin->Initialize();
					DatabaseResultPtr result = stmtGetMin->ExecuteSelect();

					if ( result && result->GetRowCount() )
					{
						result->GetFirstRow()->Get( 0, min );
					}
				}

				if ( min )
				{
					auto && stmtDelete = DatabaseUtils::CreateStmt< T >( connection, QUERY_DIRECT_DELETE_ELEMENT );

					if ( stmtDelete )
					{
						stmtDelete->CreateParameter( STR( "id" ), EFieldType_LONG_INTEGER, EParameterType_IN );
						stmtDelete->Initialize();
						std::clock_t const start = std::clock();

						for ( uint32_t i = 0; i < testCount; i++ )
						{
							int64_t id( min + i );
							stmtDelete->SetParameterValue( 0, id );
							stmtDelete->ExecuteUpdate();
						}

						CLogger::LogMessage( StringStream() << "    Deleted " << testCount << " elements in " << float( std::clock() - start ) / CLOCKS_PER_SEC << "seconds" );
					}
				}
			}
			catch ( ... )
			{
				BOOST_CHECK( false );
			}
		}

		template< typename T >
		inline void PerfStoredProcedureDeleteActors( DatabaseConnectionPtr connection, uint32_t testCount )
		{
			try
			{
				auto && stmtGetMin = DatabaseUtils::CreateStmt< T >( connection, QUERY_SELECT_MIN );
				int64_t min = 0;

				if ( stmtGetMin )
				{
					stmtGetMin->Initialize();
					DatabaseResultPtr result = stmtGetMin->ExecuteSelect();

					if ( result && result->GetRowCount() )
					{
						result->GetFirstRow()->Get( 0, min );
					}
				}

				if ( min )
				{
					auto && stmtDelete = DatabaseUtils::CreateStmt< T >( connection, STR( "CALL SpDeleteActor(?)" ) );

					if ( stmtDelete )
					{
						stmtDelete->CreateParameter( STR( "id" ), EFieldType_LONG_INTEGER, EParameterType_IN );
						stmtDelete->Initialize();
						std::clock_t const start = std::clock();

						for ( uint32_t i = 0; i < testCount; i++ )
						{
							int64_t id( min + i );
							stmtDelete->SetParameterValue( 0, id );
							stmtDelete->ExecuteUpdate();
						}

						CLogger::LogMessage( StringStream() << "    Deleted " << testCount << " elements in " << float( std::clock() - start ) / CLOCKS_PER_SEC << "seconds" );
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

#endif // ___DATABASE_TEST_UTILS_H___
