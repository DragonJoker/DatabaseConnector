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
#include <DatabaseInt24.h>

BEGIN_NAMESPACE_DATABASE_TEST
{
	/** Creates a MySql database.
	@param database
		Database name.
	@param user
		User name.
	@param pass
		User password.
	@return
		Exit code of the CProcess executing the script file.
	*/
	int InstallDatabaseMySql( const String & database, const String & user, const String & pass );

	/** Deletes a MySql database.
	@param database
		Database name.
	@param user
		User name.
	@param pass
		User password.
	@return
		Exit code of the CProcess executing the script file.
	*/
	int UninstallDatabaseMySql( const String & database, const String & user, const String & pass );

	/** Creates an ODBC source for a MySql database.
	@param dsn
		ODBC source name.
	@return
		Exit code of the CProcess executing the script file.
	*/
	int InstallSourceOdbcMySql( const String & database );

	/** Deletes an ODBC source for a MySql database.
	@param dsn
		ODBC source name.
	@return
		Exit code of the CProcess executing the script file.
	*/
	int UninstallSourceOdbcMySql( const String & database );

	/** Creates a MsSql database.
	@param database
		Database name.
	@param user
		User name.
	@param pass
		User password.
	@return
		Exit code of the CProcess executing the script file.
	*/
	int InstallDatabaseMsSql( const String & database, const String & user, const String & pass );

	/** Deletes a MsSql database.
	@param database
		Database name.
	@param user
		User name.
	@param pass
		User password.
	@return
		Exit code of the CProcess executing the script file.
	*/
	int UninstallDatabaseMsSql( const String & database, const String & user, const String & pass );

	/** Creates an ODBC source for a MsSql database.
	@param dsn
		ODBC source name.
	@return
		Exit code of the CProcess executing the script file.
	*/
	int InstallSourceOdbcMsSql( const String & database );

	/** Deletes an ODBC source for a MsSql database.
	@param dsn
		ODBC source name.
	@return
		Exit code of the CProcess executing the script file.
	*/
	int UninstallSourceOdbcMsSql( const String & database );

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
				CLogger::LogInfo( e.what() );
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
				CLogger::LogInfo( e.what() );
			}

			return result;
		}

		template< EFieldType FieldType > struct Helpers;;

		template<> struct Helpers< EFieldType_BIT >
		{
			static const uint32_t Limit = -1;
			typedef bool ParamType;
			typedef ParamType FieldType;

			static ParamType InitialiseValue()
			{
				return bool( 0 == rand() % 2 );
			}
		};

		template<> struct Helpers< EFieldType_INT8 >
		{
			static const uint32_t Limit = -1;
			typedef int8_t ParamType;
			typedef ParamType FieldType;

			static ParamType InitialiseValue()
			{
				return int8_t( rand() % 256 );
			}
		};

		template<> struct Helpers< EFieldType_INT16 >
		{
			static const uint32_t Limit = -1;
			typedef int16_t ParamType;
			typedef ParamType FieldType;

			static ParamType InitialiseValue()
			{
				return int16_t( rand() );
			}
		};

		template<> struct Helpers< EFieldType_INT24 >
		{
			static const uint32_t Limit = -1;
			typedef int24_t ParamType;
			typedef ParamType FieldType;

			static ParamType InitialiseValue()
			{
				return int24_t( rand() );
			}
		};

		template<> struct Helpers< EFieldType_INT32 >
		{
			static const uint32_t Limit = -1;
			typedef int32_t ParamType;
			typedef ParamType FieldType;

			static ParamType InitialiseValue()
			{
				return int32_t( rand() );
			}
		};

		template<> struct Helpers< EFieldType_INT64 >
		{
			static const uint32_t Limit = -1;
			typedef int64_t ParamType;
			typedef ParamType FieldType;

			static ParamType InitialiseValue()
			{
				return int64_t( rand() );
			}
		};

		template<> struct Helpers< EFieldType_FLOAT32 >
		{
			static const uint32_t Limit = -1;
			typedef float ParamType;
			typedef ParamType FieldType;

			static ParamType InitialiseValue()
			{
				return float( rand() ) / ( std::abs( rand() ) + 1.0f );
			}
		};

		template<> struct Helpers< EFieldType_FLOAT64 >
		{
			static const uint32_t Limit = -1;
			typedef double ParamType;
			typedef ParamType FieldType;

			static ParamType InitialiseValue()
			{
				return double( rand() ) / ( std::abs( rand() ) + 1.0 );
			}
		};

		template<> struct Helpers< EFieldType_FIXED_POINT >
		{
			static const uint32_t Limit = -1;
			typedef CFixedPoint ParamType;
			typedef ParamType FieldType;

			static ParamType InitialiseValue( uint8_t precision = 3 )
			{
				return CFixedPoint( int64_t( rand() ) * int64_t( rand() ), precision );
			}
		};

		template<> struct Helpers< EFieldType_DATE >
		{
			static const uint32_t Limit = -1;
			typedef CDate ParamType;
			typedef ParamType FieldType;

			static ParamType InitialiseValue()
			{
				return CDate::Now();
			}
		};

		template<> struct Helpers< EFieldType_DATETIME >
		{
			static const uint32_t Limit = -1;
			typedef CDateTime ParamType;
			typedef ParamType FieldType;

			static ParamType InitialiseValue()
			{
				return CDateTime::Now();
			}
		};

		template<> struct Helpers< EFieldType_TIME >
		{
			static const uint32_t Limit = -1;
			typedef CTime ParamType;
			typedef ParamType FieldType;

			static ParamType InitialiseValue()
			{
				return CTime::Now();
			}
		};

		template<> struct Helpers< EFieldType_VARCHAR >
		{
			static const uint32_t Limit = 20;
			typedef std::string ParamType;
			typedef std::string FieldType;

			static ParamType InitialiseValue()
			{
				static char l_return[] = "Bonjour, comment va?";
				return l_return;
			}

			static ParamType InitialiseValue( size_t size )
			{
				std::stringstream text;

				for ( size_t i = 0; i < size; ++i )
				{
					char c = char( 32 + ( rand() % 95 ) );

					if ( c == '\\' )
					{
						c = '/';
					}

					text << c;
				};

				return text.str();
			}
		};

		template<> struct Helpers< EFieldType_NVARCHAR >
		{
			static const uint32_t Limit = 55;
			typedef std::wstring ParamType;
			typedef std::wstring FieldType;

			static ParamType InitialiseValue()
			{
				static wchar_t l_return[] = L"Ca va bien, et toi?";
				return l_return;
			}

			static ParamType InitialiseValue( size_t size )
			{
				std::wstringstream text;

				for ( size_t i = 0; i < size; ++i )
				{
					wchar_t c = wchar_t( 32 + ( rand() % 95 ) );

					if ( c == L'\\' )
					{
						c = L'/';
					}

					text << c;
				};

				return text.str();
			}
		};

		template<> struct Helpers< EFieldType_TEXT >
		{
			static const uint32_t Limit = -1;
			typedef std::string ParamType;
			typedef std::string FieldType;

			static ParamType InitialiseValue()
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

			static ParamType InitialiseValue( size_t size )
			{
				std::stringstream text;

				for ( size_t i = 0; i < size; ++i )
				{
					char c = char( 32 + ( rand() % 95 ) );

					if ( c == '\\' )
					{
						c = '/';
					}

					text << c;
				};

				return text.str();
			}
		};

		template<> struct Helpers< EFieldType_VARBINARY >
		{
			static const uint32_t Limit = -1;
			typedef ByteArray ParamType;
			typedef ParamType FieldType;

			static ParamType InitialiseValue()
			{
				ByteArray blob =
				{
					0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
					0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF
				};
				return blob;
			}

			static ParamType InitialiseValue( size_t size )
			{
				ByteArray blob( size );

				for ( auto & value : blob )
				{
					char c = char( 32 + ( rand() % 95 ) );

					if ( c == '\\' )
					{
						c = '/';
					}

					value = c;
				};

				return blob;
			}
		};

		template< class Stmt >
		inline void CreateParameters( std::shared_ptr< Stmt > stmt )
		{
			BOOST_CHECK( stmt->CreateParameter( STR( "IntField" ), EFieldType_INT32, EParameterType_IN ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "IntegerField" ), EFieldType_INT32, EParameterType_IN ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "TinyIntField" ), EFieldType_INT8, EParameterType_IN ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "SmallIntField" ), EFieldType_INT16, EParameterType_IN ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "MediumIntField" ), EFieldType_INT24, EParameterType_IN ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "BigIntField" ), EFieldType_INT64, EParameterType_IN ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "Int2Field" ), EFieldType_INT16, EParameterType_IN ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "Int8Field" ), EFieldType_INT64, EParameterType_IN ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "RealField" ), EFieldType_FLOAT64, EParameterType_IN ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "DoubleField" ), EFieldType_FLOAT64, EParameterType_IN ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "DoublePrecisionField" ), EFieldType_FLOAT64, EParameterType_IN ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "FloatField" ), EFieldType_FLOAT32, EParameterType_IN ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "NumericField" ), EFieldType_FIXED_POINT, EParameterType_IN ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "DecimalField" ), EFieldType_FIXED_POINT, EParameterType_IN ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "BooleanField" ), EFieldType_BIT, EParameterType_IN ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "DateField" ), EFieldType_DATE, EParameterType_IN ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "DateTimeField" ), EFieldType_DATETIME, EParameterType_IN ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "CharacterField" ), EFieldType_VARCHAR, 20, EParameterType_IN ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "VarcharField" ), EFieldType_VARCHAR, 255, EParameterType_IN ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "NcharField" ), EFieldType_NVARCHAR, 55, EParameterType_IN ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "NVarcharField" ), EFieldType_NVARCHAR, 100, EParameterType_IN ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "TextField" ), EFieldType_TEXT, EParameterType_IN ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "BlobField" ), EFieldType_VARBINARY, EParameterType_IN ) );
		}

		template< class Stmt >
		inline void SetParametersValue( uint32_t & index, int mult, int i, std::shared_ptr< Stmt > stmt )
		{
			stmt->SetParameterValue( index++, Helpers< EFieldType_INT32 >::InitialiseValue() );
			stmt->SetParameterValue( index++, Helpers< EFieldType_INT32 >::InitialiseValue() );
			stmt->SetParameterValue( index++, Helpers< EFieldType_INT8 >::InitialiseValue() );
			stmt->SetParameterValue( index++, Helpers< EFieldType_INT16 >::InitialiseValue() );
			stmt->SetParameterValue( index++, Helpers< EFieldType_INT24 >::InitialiseValue() );
			stmt->SetParameterValue( index++, Helpers< EFieldType_INT64 >::InitialiseValue() );
			stmt->SetParameterValue( index++, Helpers< EFieldType_INT16 >::InitialiseValue() );
			stmt->SetParameterValue( index++, Helpers< EFieldType_INT64 >::InitialiseValue() );
			stmt->SetParameterValue( index++, Helpers< EFieldType_FLOAT64 >::InitialiseValue() );
			stmt->SetParameterValue( index++, Helpers< EFieldType_FLOAT64 >::InitialiseValue() );
			stmt->SetParameterValue( index++, Helpers< EFieldType_FLOAT64 >::InitialiseValue() );
			stmt->SetParameterValue( index++, Helpers< EFieldType_FLOAT32 >::InitialiseValue() );
			stmt->SetParameterValue( index++, Helpers< EFieldType_FIXED_POINT >::InitialiseValue() );
			stmt->SetParameterValue( index++, Helpers< EFieldType_FIXED_POINT >::InitialiseValue() );
			stmt->SetParameterValue( index++, Helpers< EFieldType_BIT >::InitialiseValue() );
			stmt->SetParameterValue( index++, Helpers< EFieldType_DATE >::InitialiseValue() );
			stmt->SetParameterValue( index++, Helpers< EFieldType_DATETIME >::InitialiseValue() );
			stmt->SetParameterValue( index++, Helpers< EFieldType_VARCHAR >::InitialiseValue() );
			stmt->SetParameterValue( index++, Helpers< EFieldType_VARCHAR >::InitialiseValue() );
			stmt->SetParameterValue( index++, Helpers< EFieldType_NVARCHAR >::InitialiseValue() );
			stmt->SetParameterValue( index++, Helpers< EFieldType_NVARCHAR >::InitialiseValue() );
			stmt->SetParameterValue( index++, Helpers< EFieldType_TEXT >::InitialiseValue() );
			stmt->SetParameterValue( index++, Helpers< EFieldType_VARBINARY >::InitialiseValue() );
		}

		inline void DisplayValues( uint32_t & index, DatabaseRowPtr row )
		{
			CLogger::LogInfo( StringStream() << STR( "IntField : " ) << row->Get< int32_t >( index++ ) );
			CLogger::LogInfo( StringStream() << STR( "IntegerField : " ) << row->Get< int32_t >( index++ ) );
			CLogger::LogInfo( StringStream() << STR( "TinyIntField : " ) << int( row->Get< int8_t >( index++ ) ) );
			CLogger::LogInfo( StringStream() << STR( "SmallIntField : " ) << row->Get< int16_t >( index++ ) );
			CLogger::LogInfo( StringStream() << STR( "MediumIntField : " ) << int32_t( row->Get< int24_t >( index++ ) ) );
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
			CLogger::LogInfo( StringStream() << STR( "DateTimeField : " ) << row->Get< CDateTime >( index++ ) );
			CLogger::LogInfo( StringStream() << STR( "CharacterField : " ) << row->Get< std::string >( index++ ) );
			CLogger::LogInfo( StringStream() << STR( "VarcharField : " ) << row->Get< std::string >( index++ ) );
			CLogger::LogInfo( std::wstringstream() << L"NcharField : " << row->Get< std::wstring >( index++ ) );
			CLogger::LogInfo( std::wstringstream() << L"NVarcharField : " << row->Get< std::wstring >( index++ ) );
			CLogger::LogInfo( StringStream() << STR( "TextField : " ) << row->Get< std::string >( index++ ) );
			CLogger::LogInfo( StringStream() << STR( "BlobField : " ) << row->Get< ByteArray >( index++ ) );
		}

		template< EFieldType FieldType >
		struct Compare
		{
			typedef typename Helpers< FieldType >::ParamType ParamType;
			typedef typename Helpers< FieldType >::FieldType FieldType;

			static void Equal( ParamType const & a, FieldType const & b )
			{
				BOOST_CHECK_EQUAL( a, b );
			}
			static void Different( ParamType const & a, FieldType const & b )
			{
				BOOST_CHECK_NE( a, b );
			}
			void operator()( bool equal, ParamType const & a, FieldType const & b )
			{
				if ( equal )
				{
					Equal( a, b );
				}
				else
				{
					Different( a, b );
				}
			}
		};

		template<>
		struct Compare< EFieldType_INT8 >
		{
			typedef Helpers< EFieldType_INT8 >::ParamType ParamType;
			typedef Helpers< EFieldType_INT8 >::FieldType FieldType;

			static void Equal( ParamType const & a, FieldType const & b )
			{
				BOOST_CHECK_EQUAL( int16_t( a ), int16_t( b ) );
			}
			static void Different( ParamType const & a, FieldType const & b )
			{
				BOOST_CHECK_NE( int16_t( a ), int16_t( b ) );
			}
			void operator()( bool equal, ParamType const & a, FieldType const & b )
			{
				if ( equal )
				{
					Equal( a, b );
				}
				else
				{
					Different( a, b );
				}
			}
		};

		template<>
		struct Compare< EFieldType_FLOAT32 >
		{
			typedef Helpers< EFieldType_FLOAT32 >::ParamType ParamType;
			typedef Helpers< EFieldType_FLOAT32 >::FieldType FieldType;

			static void Equal( ParamType const & a, FieldType const & b )
			{
				if ( a != b )
				{
					if ( std::abs( a - b ) > std::numeric_limits< ParamType >::epsilon() )
					{
						BOOST_CHECK_EQUAL( a, b );
					}
				}
			}
			static void Different( ParamType const & a, FieldType const & b )
			{
				BOOST_CHECK_NE( a, b );
			}
			void operator()( bool equal, ParamType const & a, FieldType const & b )
			{
				if ( equal )
				{
					Equal( a, b );
				}
				else
				{
					Different( a, b );
				}
			}
		};

		template<>
		struct Compare< EFieldType_FLOAT64 >
		{
			typedef Helpers< EFieldType_FLOAT64 >::ParamType ParamType;
			typedef Helpers< EFieldType_FLOAT64 >::FieldType FieldType;

			static void Equal( ParamType const & a, FieldType const & b )
			{
				if ( a != b )
				{
					if ( std::abs( a - b ) > std::numeric_limits< ParamType >::epsilon() )
					{
						BOOST_CHECK_EQUAL( a, b );
					}
				}
			}
			static void Different( ParamType const & a, FieldType const & b )
			{
				BOOST_CHECK_NE( a, b );
			}
			void operator()( bool equal, ParamType const & a, FieldType const & b )
			{
				if ( equal )
				{
					Equal( a, b );
				}
				else
				{
					Different( a, b );
				}
			}
		};

		template<>
		struct Compare< EFieldType_VARCHAR >
		{
			typedef Helpers< EFieldType_VARCHAR >::ParamType ParamType;
			typedef Helpers< EFieldType_VARCHAR >::FieldType FieldType;

			static void Equal( ParamType const & a, FieldType const & b )
			{
				BOOST_CHECK_EQUAL( std::string( a ), std::string( b ) );
			}
			static void Different( ParamType const & a, FieldType const & b )
			{
				BOOST_CHECK_NE( std::string( a ), std::string( b ) );
			}
			void operator()( bool equal, ParamType const & a, FieldType const & b )
			{
				if ( equal )
				{
					Equal( a, b );
				}
				else
				{
					Different( a, b );
				}
			}
		};

		template<>
		struct Compare< EFieldType_NVARCHAR >
		{
			typedef Helpers< EFieldType_NVARCHAR >::ParamType ParamType;
			typedef Helpers< EFieldType_NVARCHAR >::FieldType FieldType;

			static void Equal( ParamType const & a, FieldType const & b )
			{
				BOOST_CHECK_EQUAL( CStrUtils::ToStr( a ), CStrUtils::ToStr( b ) );
			}
			static void Different( ParamType const & a, FieldType const & b )
			{
				BOOST_CHECK_NE( CStrUtils::ToStr( a ), CStrUtils::ToStr( b ) );
			}
			void operator()( bool equal, ParamType const & a, FieldType const & b )
			{
				if ( equal )
				{
					Equal( a, b );
				}
				else
				{
					Different( a, b );
				}
			}
		};

		template<>
		struct Compare< EFieldType_VARBINARY >
		{
			typedef Helpers< EFieldType_VARBINARY >::ParamType ParamType;
			typedef Helpers< EFieldType_VARBINARY >::FieldType FieldType;

			static void Equal( ParamType const & a, FieldType const & b )
			{
				BOOST_CHECK_EQUAL_COLLECTIONS( a.begin(), a.end(), b.begin(), b.end() );
			}
			static void Different( ParamType const & a, FieldType const & b )
			{
				BOOST_CHECK( a != b );
			}
			void operator()( bool equal, ParamType const & a, FieldType const & b )
			{
				if ( equal )
				{
					Equal( a, b );
				}
				else
				{
					Different( a, b );
				}
			}
		};

		template< class Stmt, EFieldType FieldType >
		inline void InsertAndRetrieve( DatabaseConnectionPtr connection, const String & name, typename Helpers< FieldType >::ParamType * valueIn, bool equal, String const & is )
		{
			try
			{
				auto && stmtInsert = CreateStmt< Stmt >( connection, STR( "INSERT INTO Test (" ) + name + STR( ") VALUES (?)" ) );
				std::shared_ptr< Stmt > stmtSelect;

				if ( valueIn )
				{
					stmtSelect = CreateStmt< Stmt >( connection, STR( "SELECT " ) + name + STR( " FROM Test WHERE " ) + name + STR( " = ?" ) );
				}
				else
				{
					stmtSelect = CreateStmt< Stmt >( connection, STR( "SELECT " ) + name + STR( " FROM Test WHERE " ) + name + STR( " " ) + is + STR( " ?" ) );
				}

				BOOST_CHECK( stmtInsert );
				BOOST_CHECK( stmtSelect );

				if ( stmtInsert && stmtSelect )
				{
					BOOST_CHECK( stmtInsert->CreateParameter( name, FieldType, Helpers< FieldType >::Limit, EParameterType_IN ) );
					BOOST_CHECK( stmtSelect->CreateParameter( name, FieldType, Helpers< FieldType >::Limit, EParameterType_IN ) );

					BOOST_CHECK( stmtInsert->Initialize() == EErrorType_NONE );
					BOOST_CHECK( stmtSelect->Initialize() == EErrorType_NONE );

					if ( valueIn )
					{
						BOOST_CHECK_NO_THROW( stmtInsert->SetParameterValue( 0, *valueIn ) );
						BOOST_CHECK_NO_THROW( stmtSelect->SetParameterValue( 0, *valueIn ) );
					}
					else
					{
						BOOST_CHECK_NO_THROW( stmtInsert->SetParameterNull( 0 ) );
						BOOST_CHECK_NO_THROW( stmtSelect->SetParameterNull( 0 ) );
					}

					BOOST_CHECK( stmtInsert->ExecuteUpdate() );
					DatabaseResultPtr result = stmtSelect->ExecuteSelect();

					if ( result && result->GetRowCount() )
					{
						if ( valueIn )
						{
							typename Helpers< FieldType >::FieldType valueOut;
							BOOST_CHECK_NO_THROW( result->GetFirstRow()->Get( 0, valueOut ) );
							Compare< FieldType >()( equal, *valueIn, valueOut );
						}
						else
						{
							try
							{
								DatabaseFieldPtr field = result->GetFirstRow()->GetField( 0 );
								BOOST_CHECK( field->GetObjectValue().IsNull() );
							}
							catch ( CExceptionDatabase & exc )
							{
								CLogger::LogError( exc.GetFullDescription() );
								BOOST_CHECK_NO_THROW( result->GetFirstRow()->GetField( 0 ) );
							}
						}
					}
					else
					{
						BOOST_CHECK( result );
						BOOST_CHECK( result && result->GetRowCount() );
					}
				}
			}
			catch ( CExceptionDatabase & exc )
			{
				CLogger::LogError( exc.GetFullDescription() );
				BOOST_CHECK( false );
			}
			catch ( std::exception & exc )
			{
				CLogger::LogError( exc.what() );
				BOOST_CHECK( false );
			}
			catch ( ... )
			{
				BOOST_CHECK( false );
			}
		}

		template< class Stmt, EFieldType FieldType >
		inline void InsertAndRetrieveOtherIndex( DatabaseConnectionPtr connection, const String & name, typename Helpers< FieldType >::ParamType * valueIn, bool equal, String const & is )
		{
			try
			{
				auto && stmtInsert = CreateStmt< Stmt >( connection, STR( "INSERT INTO Test (IntField, " ) + name + STR( ") VALUES (?, ?)" ) );
				std::shared_ptr< Stmt > stmtSelect;

				if ( valueIn )
				{
					stmtSelect = CreateStmt< Stmt >( connection, STR( "SELECT " ) + name + STR( " FROM Test WHERE IntField = ? AND " ) + name + STR( " = ?" ) );
				}
				else
				{
					stmtSelect = CreateStmt< Stmt >( connection, STR( "SELECT " ) + name + STR( " FROM Test WHERE IntField = ? AND " ) + name + STR( " " ) + is + STR( " ?" ) );
				}

				BOOST_CHECK( stmtInsert );
				BOOST_CHECK( stmtSelect );

				if ( stmtInsert && stmtSelect )
				{
					BOOST_CHECK( stmtInsert->CreateParameter( STR( "IntField" ), EFieldType_INT32, EParameterType_IN ) );
					BOOST_CHECK( stmtInsert->CreateParameter( name, FieldType, Helpers< FieldType >::Limit, EParameterType_IN ) );
					BOOST_CHECK( stmtSelect->CreateParameter( STR( "IntField" ), EFieldType_INT32, EParameterType_IN ) );
					BOOST_CHECK( stmtSelect->CreateParameter( name, FieldType, Helpers< FieldType >::Limit, EParameterType_IN ) );

					BOOST_CHECK( stmtInsert->Initialize() == EErrorType_NONE );
					BOOST_CHECK( stmtSelect->Initialize() == EErrorType_NONE );

					BOOST_CHECK_NO_THROW( stmtInsert->SetParameterValue( 0, 18 ) );
					BOOST_CHECK_NO_THROW( stmtSelect->SetParameterValue( 0, 18 ) );

					if ( valueIn )
					{
						BOOST_CHECK_NO_THROW( stmtInsert->SetParameterValue( 1, *valueIn ) );
						BOOST_CHECK_NO_THROW( stmtSelect->SetParameterValue( 1, *valueIn ) );
					}
					else
					{
						BOOST_CHECK_NO_THROW( stmtInsert->SetParameterNull( 1 ) );
						BOOST_CHECK_NO_THROW( stmtSelect->SetParameterNull( 1 ) );
					}

					BOOST_CHECK( stmtInsert->ExecuteUpdate() );
					DatabaseResultPtr result = stmtSelect->ExecuteSelect();

					if ( result && result->GetRowCount() )
					{
						if ( valueIn )
						{
							typename Helpers< FieldType >::FieldType valueOut;
							BOOST_CHECK_NO_THROW( result->GetFirstRow()->Get( 0, valueOut ) );
							Compare< FieldType >()( equal, *valueIn, valueOut );
						}
						else
						{
							try
							{
								DatabaseFieldPtr field = result->GetFirstRow()->GetField( 0 );
								BOOST_CHECK( field->GetObjectValue().IsNull() );
							}
							catch ( CExceptionDatabase & exc )
							{
								CLogger::LogError( exc.GetFullDescription() );
								BOOST_CHECK_NO_THROW( result->GetFirstRow()->GetField( 0 ) );
							}
						}
					}
					else
					{
						BOOST_CHECK( result );
						BOOST_CHECK( result && result->GetRowCount() );
					}
				}
			}
			catch ( CExceptionDatabase & exc )
			{
				CLogger::LogError( exc.GetFullDescription() );
				BOOST_CHECK( false );
			}
			catch ( std::exception & exc )
			{
				CLogger::LogError( exc.what() );
				BOOST_CHECK( false );
			}
			catch ( ... )
			{
				BOOST_CHECK( false );
			}
		}

		template< class Stmt, EFieldType FieldType >
		inline void InsertAndRetrieveFast( DatabaseConnectionPtr connection, const String & name, typename Helpers< FieldType >::ParamType * valueIn, bool equal, String const & is )
		{
			try
			{
				auto && stmtInsert = CreateStmt< Stmt >( connection, STR( "INSERT INTO Test (" ) + name + STR( ") VALUES (?)" ) );
				std::shared_ptr< Stmt > stmtSelect;

				if ( valueIn )
				{
					stmtSelect = CreateStmt< Stmt >( connection, STR( "SELECT " ) + name + STR( " FROM Test WHERE " ) + name + STR( " = ?" ) );
				}
				else
				{
					stmtSelect = CreateStmt< Stmt >( connection, STR( "SELECT " ) + name + STR( " FROM Test WHERE " ) + name + STR( " " ) + is + STR( " ?" ) );
				}

				BOOST_CHECK( stmtInsert );
				BOOST_CHECK( stmtSelect );

				if ( stmtInsert && stmtSelect )
				{
					BOOST_CHECK( stmtInsert->CreateParameter( name, FieldType, Helpers< FieldType >::Limit, EParameterType_IN ) );
					BOOST_CHECK( stmtSelect->CreateParameter( name, FieldType, Helpers< FieldType >::Limit, EParameterType_IN ) );

					BOOST_CHECK( stmtInsert->Initialize() == EErrorType_NONE );
					BOOST_CHECK( stmtSelect->Initialize() == EErrorType_NONE );

					if ( valueIn )
					{
						BOOST_CHECK_NO_THROW( stmtInsert->SetParameterValueFast( 0, *valueIn ) );
						BOOST_CHECK_NO_THROW( stmtSelect->SetParameterValueFast( 0, *valueIn ) );
					}
					else
					{
						BOOST_CHECK_NO_THROW( stmtInsert->SetParameterNull( 0 ) );
						BOOST_CHECK_NO_THROW( stmtSelect->SetParameterNull( 0 ) );
					}

					BOOST_CHECK( stmtInsert->ExecuteUpdate() );
					DatabaseResultPtr result = stmtSelect->ExecuteSelect();

					if ( result && result->GetRowCount() )
					{
						if ( valueIn )
						{
							typename Helpers< FieldType >::FieldType valueOut;
							BOOST_CHECK_NO_THROW( result->GetFirstRow()->GetFast( 0, valueOut ) );
							Compare< FieldType >()( equal, *valueIn, valueOut );
						}
						else
						{
							try
							{
								DatabaseFieldPtr field = result->GetFirstRow()->GetField( 0 );
								BOOST_CHECK( field->GetObjectValue().IsNull() );
							}
							catch ( CExceptionDatabase & exc )
							{
								CLogger::LogError( exc.GetFullDescription() );
								BOOST_CHECK_NO_THROW( result->GetFirstRow()->GetField( 0 ) );
							}
						}
					}
					else
					{
						BOOST_CHECK( result );
						BOOST_CHECK( result && result->GetRowCount() );
					}
				}
			}
			catch ( CExceptionDatabase & exc )
			{
				CLogger::LogError( exc.GetFullDescription() );
				BOOST_CHECK( false );
			}
			catch ( std::exception & exc )
			{
				CLogger::LogError( exc.what() );
				BOOST_CHECK( false );
			}
			catch ( ... )
			{
				BOOST_CHECK( false );
			}
		}

		template< class Stmt, EFieldType FieldType >
		inline void InsertAndRetrieveFastOtherIndex( DatabaseConnectionPtr connection, const String & name, typename Helpers< FieldType >::ParamType * valueIn, bool equal, String const & is )
		{
			try
			{
				auto && stmtInsert = CreateStmt< Stmt >( connection, STR( "INSERT INTO Test (IntField, " ) + name + STR( ") VALUES (?, ?)" ) );
				std::shared_ptr< Stmt > stmtSelect;

				if ( valueIn )
				{
					stmtSelect = CreateStmt< Stmt >( connection, STR( "SELECT " ) + name + STR( " FROM Test WHERE IntField = ? AND " ) + name + STR( " = ?" ) );
				}
				else
				{
					stmtSelect = CreateStmt< Stmt >( connection, STR( "SELECT " ) + name + STR( " FROM Test WHERE IntField = ? AND " ) + name + STR( " " ) + is + STR( " ?" ) );
				}

				BOOST_CHECK( stmtInsert );
				BOOST_CHECK( stmtSelect );

				if ( stmtInsert && stmtSelect )
				{
					BOOST_CHECK( stmtInsert->CreateParameter( STR( "IntField" ), EFieldType_INT32, EParameterType_IN ) );
					BOOST_CHECK( stmtInsert->CreateParameter( name, FieldType, Helpers< FieldType >::Limit, EParameterType_IN ) );
					BOOST_CHECK( stmtSelect->CreateParameter( STR( "IntField" ), EFieldType_INT32, EParameterType_IN ) );
					BOOST_CHECK( stmtSelect->CreateParameter( name, FieldType, Helpers< FieldType >::Limit, EParameterType_IN ) );

					BOOST_CHECK( stmtInsert->Initialize() == EErrorType_NONE );
					BOOST_CHECK( stmtSelect->Initialize() == EErrorType_NONE );

					BOOST_CHECK_NO_THROW( stmtInsert->SetParameterValueFast( 0, 18 ) );
					BOOST_CHECK_NO_THROW( stmtSelect->SetParameterValueFast( 0, 18 ) );

					if ( valueIn )
					{
						BOOST_CHECK_NO_THROW( stmtInsert->SetParameterValueFast( 1, *valueIn ) );
						BOOST_CHECK_NO_THROW( stmtSelect->SetParameterValueFast( 1, *valueIn ) );
					}
					else
					{
						BOOST_CHECK_NO_THROW( stmtInsert->SetParameterNull( 1 ) );
						BOOST_CHECK_NO_THROW( stmtSelect->SetParameterNull( 1 ) );
					}

					BOOST_CHECK( stmtInsert->ExecuteUpdate() );
					DatabaseResultPtr result = stmtSelect->ExecuteSelect();

					if ( result && result->GetRowCount() )
					{
						if ( valueIn )
						{
							typename Helpers< FieldType >::FieldType valueOut;
							BOOST_CHECK_NO_THROW( result->GetFirstRow()->GetFast( 0, valueOut ) );
							Compare< FieldType >()( equal, *valueIn, valueOut );
						}
						else
						{
							try
							{
								DatabaseFieldPtr field = result->GetFirstRow()->GetField( 0 );
								BOOST_CHECK( field->GetObjectValue().IsNull() );
							}
							catch ( CExceptionDatabase & exc )
							{
								CLogger::LogError( exc.GetFullDescription() );
								BOOST_CHECK_NO_THROW( result->GetFirstRow()->GetField( 0 ) );
							}
						}
					}
					else
					{
						BOOST_CHECK( result );
						BOOST_CHECK( result && result->GetRowCount() );
					}
				}
			}
			catch ( CExceptionDatabase & exc )
			{
				CLogger::LogError( exc.GetFullDescription() );
				BOOST_CHECK( false );
			}
			catch ( std::exception & exc )
			{
				CLogger::LogError( exc.what() );
				BOOST_CHECK( false );
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
				auto && stmtGetCount = CreateStmt< T >( connection, QUERY_GET_COUNT );
				BOOST_CHECK( stmtGetCount );
				int64_t count = -1;

				if ( stmtGetCount )
				{
					BOOST_CHECK( stmtGetCount->Initialize() == EErrorType_NONE );
					DatabaseResultPtr result = stmtGetCount->ExecuteSelect();

					if ( result && result->GetRowCount() )
					{
						BOOST_CHECK_NO_THROW( result->GetFirstRow()->Get( 0, count ) );
					}
					else
					{
						BOOST_CHECK( result );
						BOOST_CHECK( result && result->GetRowCount() );
					}
				}

				count++;

				if ( count )
				{
					auto && stmtInsert = CreateStmt< T >( connection, QUERY_INSERT_ELEMENT );
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

						if ( result && result->GetRowCount() )
						{
							int64_t field = 0;
							BOOST_CHECK_NO_THROW( result->GetFirstRow()->Get( 0, field ) );
							BOOST_CHECK_EQUAL( field, count + inserts );
							count++;
						}
						else
						{
							BOOST_CHECK( result );
							BOOST_CHECK( result && result->GetRowCount() );
						}
					}
				}
			}
			catch ( CExceptionDatabase & exc )
			{
				CLogger::LogError( exc.GetFullDescription() );
				BOOST_CHECK( false );
			}
			catch ( std::exception & exc )
			{
				CLogger::LogError( exc.what() );
				BOOST_CHECK( false );
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
				auto && stmtSelect = CreateStmt< T >( connection, QUERY_DIRECT_SELECT_ALL_ELEMENTS );
				BOOST_CHECK( stmtSelect );

				if ( stmtSelect )
				{
					BOOST_CHECK( stmtSelect->Initialize() == EErrorType_NONE );
					DatabaseResultPtr result = stmtSelect->ExecuteSelect();

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
					else
					{
						BOOST_CHECK( result );
						BOOST_CHECK( result && result->GetRowCount() );
					}
				}
			}
			catch ( CExceptionDatabase & exc )
			{
				CLogger::LogError( exc.GetFullDescription() );
				BOOST_CHECK( false );
			}
			catch ( std::exception & exc )
			{
				CLogger::LogError( exc.what() );
				BOOST_CHECK( false );
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
				auto && stmtGetMin = CreateStmt< T >( connection, QUERY_SELECT_MIN );
				BOOST_CHECK( stmtGetMin );
				int64_t min = -1;

				if ( stmtGetMin )
				{
					BOOST_CHECK( stmtGetMin->Initialize() == EErrorType_NONE );
					DatabaseResultPtr result = stmtGetMin->ExecuteSelect();

					if ( result && result->GetRowCount() )
					{
						BOOST_CHECK_NO_THROW( result->GetFirstRow()->Get( 0, min ) );
					}
					else
					{
						BOOST_CHECK( result );
						BOOST_CHECK( result && result->GetRowCount() );
					}
				}

				if ( min )
				{
					auto && stmtUpdate = CreateStmt< T >( connection, QUERY_DIRECT_UPDATE_ELEMENT );
					BOOST_CHECK( stmtUpdate );

					if ( stmtUpdate )
					{
						CreateParameters( stmtUpdate );
						BOOST_CHECK( stmtUpdate->CreateParameter( STR( "IDTest" ), EFieldType_INT64, EParameterType_IN ) );
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
			catch ( CExceptionDatabase & exc )
			{
				CLogger::LogError( exc.GetFullDescription() );
				BOOST_CHECK( false );
			}
			catch ( std::exception & exc )
			{
				CLogger::LogError( exc.what() );
				BOOST_CHECK( false );
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
				auto && stmtGetMin = CreateStmt< T >( connection, QUERY_SELECT_MIN );
				BOOST_CHECK( stmtGetMin );
				int64_t min = -1;

				if ( stmtGetMin )
				{
					BOOST_CHECK( stmtGetMin->Initialize() == EErrorType_NONE );
					DatabaseResultPtr result = stmtGetMin->ExecuteSelect();

					if ( result && result->GetRowCount() )
					{
						BOOST_CHECK_NO_THROW( result->GetFirstRow()->Get( 0, min ) );
					}
					else
					{
						BOOST_CHECK( result );
						BOOST_CHECK( result && result->GetRowCount() );
					}
				}

				if ( min )
				{
					auto && stmtDelete = CreateStmt< T >( connection, QUERY_DIRECT_DELETE_ELEMENT );
					BOOST_CHECK( stmtDelete );

					if ( stmtDelete )
					{
						BOOST_CHECK( stmtDelete->CreateParameter( STR( "IDTest" ), EFieldType_INT64, EParameterType_IN ) );
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
			catch ( CExceptionDatabase & exc )
			{
				CLogger::LogError( exc.GetFullDescription() );
				BOOST_CHECK( false );
			}
			catch ( std::exception & exc )
			{
				CLogger::LogError( exc.what() );
				BOOST_CHECK( false );
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
				auto && stmtClear = CreateStmt< T >( connection, STR( "CALL ClearElementsTables" ) );
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
			catch ( CExceptionDatabase & exc )
			{
				CLogger::LogError( exc.GetFullDescription() );
				BOOST_CHECK( false );
			}
			catch ( std::exception & exc )
			{
				CLogger::LogError( exc.what() );
				BOOST_CHECK( false );
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
				auto && stmtGetElements = CreateStmt< T >( connection, STR( "CALL GetElements(?)" ) );
				BOOST_CHECK( stmtGetElements );

				if ( stmtGetElements )
				{
					BOOST_CHECK( stmtGetElements->CreateParameter( STR( "Where" ), EFieldType_VARCHAR, 500, EParameterType_IN ) );
					BOOST_CHECK( stmtGetElements->Initialize() == EErrorType_NONE );

					for ( int i = 0; i < 2; i++ )
					{
						stmtGetElements->SetParameterValue( 0, where );
						DatabaseResultPtr result = stmtGetElements->ExecuteSelect();

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
						else
						{
							BOOST_CHECK( result );
							BOOST_CHECK( result && result->GetRowCount() );
						}
					}
				}
			}
			catch ( CExceptionDatabase & exc )
			{
				CLogger::LogError( exc.GetFullDescription() );
				BOOST_CHECK( false );
			}
			catch ( std::exception & exc )
			{
				CLogger::LogError( exc.what() );
				BOOST_CHECK( false );
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
				auto && stmtGetMin = CreateStmt< T >( connection, QUERY_SELECT_MIN );
				BOOST_CHECK( stmtGetMin );
				int64_t min = 0;

				if ( stmtGetMin )
				{
					BOOST_CHECK( stmtGetMin->Initialize() == EErrorType_NONE );
					DatabaseResultPtr result = stmtGetMin->ExecuteSelect();

					if ( result && result->GetRowCount() )
					{
						BOOST_CHECK_NO_THROW( result->GetFirstRow()->Get( 0, min ) );
					}
					else
					{
						BOOST_CHECK( result );
						BOOST_CHECK( result && result->GetRowCount() );
					}
				}

				if ( min )
				{
					auto && stmtDelete = CreateStmt< T >( connection, STR( "CALL DeleteElements(?)" ) );
					BOOST_CHECK( stmtDelete );

					if ( stmtDelete )
					{
						BOOST_CHECK( stmtDelete->CreateParameter( STR( "IDTest" ), EFieldType_INT64, EParameterType_IN ) );
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
			catch ( CExceptionDatabase & exc )
			{
				CLogger::LogError( exc.GetFullDescription() );
				BOOST_CHECK( false );
			}
			catch ( std::exception & exc )
			{
				CLogger::LogError( exc.what() );
				BOOST_CHECK( false );
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
				auto && stmtGetCount = CreateStmt< T >( connection, QUERY_GET_COUNT );
				BOOST_CHECK( stmtGetCount );
				int64_t count = -1;

				if ( stmtGetCount )
				{
					BOOST_CHECK( stmtGetCount->Initialize() == EErrorType_NONE );
					DatabaseResultPtr result = stmtGetCount->ExecuteSelect();

					if ( result && result->GetRowCount() )
					{
						BOOST_CHECK_NO_THROW( result->GetFirstRow()->Get( 0, count ) );
					}
					else
					{
						BOOST_CHECK( result );
						BOOST_CHECK( result && result->GetRowCount() );
					}
				}

				count++;

				if ( count )
				{
					auto && stmtAddElement = CreateStmt< T >( connection, STR( "CALL AddElement(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);" ) );
					BOOST_CHECK( stmtAddElement );

					if ( stmtAddElement )
					{
						BOOST_CHECK( stmtAddElement->CreateParameter( STR( "IDTest" ), EFieldType_INT64, EParameterType_INOUT ) );
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
			catch ( CExceptionDatabase & exc )
			{
				CLogger::LogError( exc.GetFullDescription() );
				BOOST_CHECK( false );
			}
			catch ( std::exception & exc )
			{
				CLogger::LogError( exc.what() );
				BOOST_CHECK( false );
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
				auto && stmtGetCount = CreateStmt< T >( connection, QUERY_GET_COUNT );
				BOOST_CHECK( stmtGetCount );
				int64_t count = -1;

				if ( stmtGetCount )
				{
					BOOST_CHECK( stmtGetCount->Initialize() == EErrorType_NONE );
					DatabaseResultPtr result = stmtGetCount->ExecuteSelect();

					if ( result && result->GetRowCount() )
					{
						BOOST_CHECK_NO_THROW( result->GetFirstRow()->Get( 0, count ) );
					}
					else
					{
						BOOST_CHECK( result );
						BOOST_CHECK( result && result->GetRowCount() );
					}
				}

				count++;

				if ( count )
				{
					auto && stmtUpdateElement = CreateStmt< T >( connection, STR( "CALL UpdateElement(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);" ) );
					BOOST_CHECK( stmtUpdateElement );

					if ( stmtUpdateElement )
					{
						BOOST_CHECK( stmtUpdateElement->CreateParameter( STR( "IDTest" ), EFieldType_INT64, EParameterType_INOUT ) );
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
			catch ( CExceptionDatabase & exc )
			{
				CLogger::LogError( exc.GetFullDescription() );
				BOOST_CHECK( false );
			}
			catch ( std::exception & exc )
			{
				CLogger::LogError( exc.what() );
				BOOST_CHECK( false );
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
					stmtGetActors = CreateStmt< T >( connection, QUERY_DIRECT_SELECT_ALL_ELEMENTS );
				}
				else
				{
					stmtGetActors = CreateStmt< T >( connection, QUERY_DIRECT_SELECT_ALL_ELEMENTS + STR( " " ) + whereClause );
				}

				if ( stmtGetActors )
				{
					stmtGetActors->Initialize();
					std::clock_t start = std::clock();
					DatabaseResultPtr result = stmtGetActors->ExecuteSelect();
					CLogger::LogInfo( StringStream() << "    Selected the elements in " << float( std::clock() - start ) / CLOCKS_PER_SEC << "seconds (no fetch)" );

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

					CLogger::LogInfo( StringStream() << "    Fetched " << count << " elements in " << float( std::clock() - start ) / CLOCKS_PER_SEC << "seconds" );
				}
			}
			catch ( CExceptionDatabase & exc )
			{
				CLogger::LogError( exc.GetFullDescription() );
				BOOST_CHECK( false );
			}
			catch ( std::exception & exc )
			{
				CLogger::LogError( exc.what() );
				BOOST_CHECK( false );
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
				auto && stmtGetActors = CreateStmt< T >( connection, STR( "CALL SpGetActors(?)" ) );

				if ( stmtGetActors )
				{
					stmtGetActors->CreateParameter( STR( "Where" ), EFieldType_VARCHAR, 500, EParameterType_IN );
					stmtGetActors->Initialize();
					std::clock_t start = std::clock();
					stmtGetActors->SetParameterValue( 0, whereClause );
					DatabaseResultPtr result = stmtGetActors->ExecuteSelect();
					CLogger::LogInfo( StringStream() << "    Selected the elements in " << float( std::clock() - start ) / CLOCKS_PER_SEC << "seconds (no fetch)" );

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

					CLogger::LogInfo( StringStream() << "    Fetched " << count << " elements in " << float( std::clock() - start ) / CLOCKS_PER_SEC << "seconds" );
				}
			}
			catch ( CExceptionDatabase & exc )
			{
				CLogger::LogError( exc.GetFullDescription() );
				BOOST_CHECK( false );
			}
			catch ( std::exception & exc )
			{
				CLogger::LogError( exc.what() );
				BOOST_CHECK( false );
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
				auto && stmtGetMin = CreateStmt< T >( connection, QUERY_SELECT_MIN );
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
					auto && stmtDelete = CreateStmt< T >( connection, QUERY_DIRECT_DELETE_ELEMENT );

					if ( stmtDelete )
					{
						stmtDelete->CreateParameter( STR( "id" ), EFieldType_INT64, EParameterType_IN );
						stmtDelete->Initialize();
						std::clock_t const start = std::clock();

						for ( uint32_t i = 0; i < testCount; i++ )
						{
							int64_t id( min + i );
							stmtDelete->SetParameterValue( 0, id );
							stmtDelete->ExecuteUpdate();
						}

						CLogger::LogInfo( StringStream() << "    Deleted " << testCount << " elements in " << float( std::clock() - start ) / CLOCKS_PER_SEC << "seconds" );
					}
				}
			}
			catch ( CExceptionDatabase & exc )
			{
				CLogger::LogError( exc.GetFullDescription() );
				BOOST_CHECK( false );
			}
			catch ( std::exception & exc )
			{
				CLogger::LogError( exc.what() );
				BOOST_CHECK( false );
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
				auto && stmtGetMin = CreateStmt< T >( connection, QUERY_SELECT_MIN );
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
					auto && stmtDelete = CreateStmt< T >( connection, STR( "CALL SpDeleteActor(?)" ) );

					if ( stmtDelete )
					{
						stmtDelete->CreateParameter( STR( "id" ), EFieldType_INT64, EParameterType_IN );
						stmtDelete->Initialize();
						std::clock_t const start = std::clock();

						for ( uint32_t i = 0; i < testCount; i++ )
						{
							int64_t id( min + i );
							stmtDelete->SetParameterValue( 0, id );
							stmtDelete->ExecuteUpdate();
						}

						CLogger::LogInfo( StringStream() << "    Deleted " << testCount << " elements in " << float( std::clock() - start ) / CLOCKS_PER_SEC << "seconds" );
					}
				}
			}
			catch ( CExceptionDatabase & exc )
			{
				CLogger::LogError( exc.GetFullDescription() );
				BOOST_CHECK( false );
			}
			catch ( std::exception & exc )
			{
				CLogger::LogError( exc.what() );
				BOOST_CHECK( false );
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
