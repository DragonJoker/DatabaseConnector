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
#include <random>

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
		template< typename StmtType >
		std::shared_ptr< StmtType > CreateStmt( DatabaseConnectionSPtr connection, const String & query );

		template<>
		inline std::shared_ptr< CDatabaseStatement > CreateStmt< CDatabaseStatement >( DatabaseConnectionSPtr connection, const String & query )
		{
			std::shared_ptr< CDatabaseStatement > result;

			try
			{
				result = connection->CreateStatement( query );
			}
			catch ( std::exception & e )
			{
				CLogger::LogWarning( e.what() );
			}

			return result;
		}

		template<>
		inline std::shared_ptr< CDatabaseQuery > CreateStmt< CDatabaseQuery >( DatabaseConnectionSPtr connection, const String & query )
		{
			std::shared_ptr< CDatabaseQuery > result;

			try
			{
				result = connection->CreateQuery( query );
			}
			catch ( std::exception & e )
			{
				CLogger::LogWarning( e.what() );
			}

			return result;
		}

		template< EFieldType FieldType > struct Helpers;

		template<> struct Helpers< EFieldType_BIT >
		{
			static const uint32_t Limit = -1;
			typedef bool ParamType;
			typedef ParamType FieldType;

			static ParamType GetRandomValue( std::random_device & generator )
			{
				std::uniform_int_distribution< int > distribution( 0, 1 );
				return distribution( generator ) == 1;
			}
		};

		template<> struct Helpers< EFieldType_SINT8 >
		{
			static const uint32_t Limit = -1;
			typedef int8_t ParamType;
			typedef ParamType FieldType;

			static ParamType GetRandomValue( std::random_device & generator )
			{
				std::uniform_int_distribution< int > distribution( std::numeric_limits< ParamType >::lowest(), std::numeric_limits< ParamType >::max() );
				return ParamType( distribution( generator ) );
			}
		};

		template<> struct Helpers< EFieldType_UINT8 >
		{
			static const uint32_t Limit = -1;
			typedef uint8_t ParamType;
			typedef ParamType FieldType;

			static ParamType GetRandomValue( std::random_device & generator )
			{
				std::uniform_int_distribution< int > distribution( std::numeric_limits< ParamType >::lowest(), std::numeric_limits< ParamType >::max() );
				return distribution( generator );
			}
		};

		template<> struct Helpers< EFieldType_SINT16 >
		{
			static const uint32_t Limit = -1;
			typedef int16_t ParamType;
			typedef ParamType FieldType;

			static ParamType GetRandomValue( std::random_device & generator )
			{
				std::uniform_int_distribution< ParamType > distribution( std::numeric_limits< ParamType >::lowest(), std::numeric_limits< ParamType >::max() );
				return distribution( generator );
			}
		};

		template<> struct Helpers< EFieldType_UINT16 >
		{
			static const uint32_t Limit = -1;
			typedef uint16_t ParamType;
			typedef ParamType FieldType;

			static ParamType GetRandomValue( std::random_device & generator )
			{
				std::uniform_int_distribution< ParamType > distribution( std::numeric_limits< ParamType >::lowest(), std::numeric_limits< ParamType >::max() );
				return distribution( generator );
			}
		};

		template<> struct Helpers< EFieldType_SINT24 >
		{
			static const uint32_t Limit = -1;
			typedef int24_t ParamType;
			typedef ParamType FieldType;

			static ParamType GetRandomValue( std::random_device & generator )
			{
				std::uniform_int_distribution< int32_t > distribution( 0xFF800000, 0x007FFFFF );
				return ParamType( distribution( generator ) );
			}
		};

		template<> struct Helpers< EFieldType_UINT24 >
		{
			static const uint32_t Limit = -1;
			typedef uint24_t ParamType;
			typedef ParamType FieldType;

			static ParamType GetRandomValue( std::random_device & generator )
			{
				std::uniform_int_distribution< uint32_t > distribution( 0, 0x00FFFFFF );
				return ParamType( distribution( generator ) );
			}
		};

		template<> struct Helpers< EFieldType_SINT32 >
		{
			static const uint32_t Limit = -1;
			typedef int32_t ParamType;
			typedef ParamType FieldType;

			static ParamType GetRandomValue( std::random_device & generator )
			{
				std::uniform_int_distribution< ParamType > distribution( std::numeric_limits< ParamType >::lowest(), std::numeric_limits< ParamType >::max() );
				return distribution( generator );
			}
		};

		template<> struct Helpers< EFieldType_UINT32 >
		{
			static const uint32_t Limit = -1;
			typedef uint32_t ParamType;
			typedef ParamType FieldType;

			static ParamType GetRandomValue( std::random_device & generator )
			{
				std::uniform_int_distribution< ParamType > distribution( std::numeric_limits< ParamType >::lowest(), std::numeric_limits< ParamType >::max() );
				return distribution( generator );
			}
		};

		template<> struct Helpers< EFieldType_SINT64 >
		{
			static const uint32_t Limit = -1;
			typedef int64_t ParamType;
			typedef ParamType FieldType;

			static ParamType GetRandomValue( std::random_device & generator )
			{
				std::uniform_int_distribution< ParamType > distribution( std::numeric_limits< ParamType >::lowest(), std::numeric_limits< ParamType >::max() );
				return distribution( generator );
			}
		};

		template<> struct Helpers< EFieldType_UINT64 >
		{
			static const uint32_t Limit = -1;
			typedef uint64_t ParamType;
			typedef ParamType FieldType;

			static ParamType GetRandomValue( std::random_device & generator )
			{
				std::uniform_int_distribution< ParamType > distribution( std::numeric_limits< ParamType >::lowest(), std::numeric_limits< ParamType >::max() );
				return distribution( generator );
			}
		};

		template<> struct Helpers< EFieldType_FLOAT32 >
		{
			static const uint32_t Limit = -1;
			typedef float ParamType;
			typedef ParamType FieldType;

			static ParamType GetRandomValue( std::random_device & generator )
			{
				std::uniform_int_distribution< int > bdistribution( 0, 1 );
				std::uniform_real_distribution< ParamType > fdistribution( std::numeric_limits< ParamType >::lowest() / 2, std::numeric_limits< ParamType >::max() / 2 );
				return ParamType( fmod( fdistribution( generator ), 1000000.0000 ) * ( bdistribution( generator ) ? 1 : -1 ) );
			}
		};

		template<> struct Helpers< EFieldType_FLOAT64 >
		{
			static const uint32_t Limit = -1;
			typedef double ParamType;
			typedef ParamType FieldType;

			static ParamType GetRandomValue( std::random_device & generator )
			{
				std::uniform_int_distribution< int > bdistribution( 0, 1 );
				std::uniform_real_distribution< ParamType > fdistribution( std::numeric_limits< ParamType >::lowest() / 2, std::numeric_limits< ParamType >::max() / 2 );
				return ParamType( fmod( fdistribution( generator ), 1000000000.0000000 ) * ( bdistribution( generator ) ? 1 : -1 ) );
			}
		};

		template<> struct Helpers< EFieldType_FIXED_POINT >
		{
			static const std::pair< uint32_t, uint32_t > Precision;
			typedef CFixedPoint ParamType;
			typedef ParamType FieldType;

			static ParamType GetRandomValue( std::random_device & generator, const uint8_t precision = 10, const uint8_t decimals = 3 )
			{
				std::uniform_int_distribution< int64_t > distribution( std::numeric_limits< int64_t >::lowest(), std::numeric_limits< int64_t >::max() );
				return CFixedPoint( distribution( generator ) % int64_t( pow( 10, precision ) ), precision, decimals );
			}

			static ParamType GetRandomValue( std::random_device & generator, const std::pair< uint32_t, uint32_t > & precision )
			{
				return GetRandomValue( generator, precision.first, precision.second );
			}
		};

		template<> struct Helpers< EFieldType_DATE >
		{
			static const uint32_t Limit = -1;
			typedef DateType ParamType;
			typedef ParamType FieldType;

			static ParamType GetRandomValue( std::random_device & generator )
			{
				return boost::gregorian::day_clock::local_day();
			}
		};

		template<> struct Helpers< EFieldType_DATETIME >
		{
			static const uint32_t Limit = -1;
			typedef DateTimeType ParamType;
			typedef ParamType FieldType;

			static ParamType GetRandomValue( std::random_device & generator )
			{
				return boost::posix_time::second_clock::local_time();
			}
		};

		template<> struct Helpers< EFieldType_TIME >
		{
			static const uint32_t Limit = -1;
			typedef TimeType ParamType;
			typedef ParamType FieldType;

			static ParamType GetRandomValue( std::random_device & generator )
			{
				return boost::posix_time::second_clock::local_time().time_of_day();
			}
		};

		template<> struct Helpers< EFieldType_CHAR >
		{
			static const uint32_t Limit = 20;
			typedef std::string ParamType;
			typedef std::string FieldType;

			static ParamType GetRandomValue( std::random_device & generator, size_t size )
			{
				std::uniform_int_distribution< int > distribution( 32, 126 );
				std::stringstream text;

				for ( size_t i = 0; i < size; ++i )
				{
					char c = char( distribution( generator ) );

					if ( c == '\\' )
					{
						c = '/';
					}

					text << c;
				};

				return text.str();
			}

			static ParamType GetRandomValue( std::random_device & generator )
			{
				return GetRandomValue( generator, Limit );
			}
		};

		template<> struct Helpers< EFieldType_VARCHAR >
		{
			static const uint32_t Limit = 20;
			typedef std::string ParamType;
			typedef std::string FieldType;

			static ParamType GetRandomValue( std::random_device & generator, size_t size )
			{
				std::uniform_int_distribution< int > distribution( 32, 126 );
				std::stringstream text;

				for ( size_t i = 0; i < size; ++i )
				{
					char c = char( distribution( generator ) );

					if ( c == '\\' )
					{
						c = '/';
					}

					text << c;
				};

				return text.str();
			}

			static ParamType GetRandomValue( std::random_device & generator )
			{
				return GetRandomValue( generator, Limit );
			}
		};

		template<> struct Helpers< EFieldType_NCHAR >
		{
			static const uint32_t Limit = 55;
			typedef std::wstring ParamType;
			typedef std::wstring FieldType;

			static ParamType GetRandomValue( std::random_device & generator, size_t size )
			{
				std::uniform_int_distribution< int > distribution( 32, 126 );
				std::wstringstream text;

				for ( size_t i = 0; i < size; ++i )
				{
					wchar_t c = wchar_t( distribution( generator ) );

					if ( c == L'\\' )
					{
						c = L'/';
					}

					text << c;
				};

				return text.str();
			}

			static ParamType GetRandomValue( std::random_device & generator )
			{
				return GetRandomValue( generator, Limit );
			}
		};

		template<> struct Helpers< EFieldType_NVARCHAR >
		{
			static const uint32_t Limit = 55;
			typedef std::wstring ParamType;
			typedef std::wstring FieldType;

			static ParamType GetRandomValue( std::random_device & generator, size_t size )
			{
				std::uniform_int_distribution< int > distribution( 32, 126 );
				std::wstringstream text;

				for ( size_t i = 0; i < size; ++i )
				{
					wchar_t c = wchar_t( distribution( generator ) );

					if ( c == L'\\' )
					{
						c = L'/';
					}

					text << c;
				};

				return text.str();
			}

			static ParamType GetRandomValue( std::random_device & generator )
			{
				return GetRandomValue( generator, Limit );
			}
		};

		template<> struct Helpers< EFieldType_TEXT >
		{
			static const uint32_t Limit = -1;
			typedef std::string ParamType;
			typedef std::string FieldType;

			static ParamType GetRandomValue( std::random_device & generator, size_t size )
			{
				std::uniform_int_distribution< int > distribution( 32, 126 );
				std::stringstream text;

				for ( size_t i = 0; i < size; ++i )
				{
					char c = char( distribution( generator ) );

					if ( c == '\\' )
					{
						c = '/';
					}

					text << c;
				};

				return text.str();
			}

			static ParamType GetRandomValue( std::random_device & generator )
			{
				return GetRandomValue( generator, 500 );
			}
		};

		template<> struct Helpers< EFieldType_NTEXT >
		{
			static const uint32_t Limit = -1;
			typedef std::wstring ParamType;
			typedef std::wstring FieldType;

			static ParamType GetRandomValue( std::random_device & generator, size_t size )
			{
				std::uniform_int_distribution< int > distribution( 32, 126 );
				std::wstringstream text;

				for ( size_t i = 0; i < size; ++i )
				{
					char c = char( distribution( generator ) );

					if ( c == '\\' )
					{
						c = '/';
					}

					text << c;
				};

				return text.str();
			}

			static ParamType GetRandomValue( std::random_device & generator )
			{
				return GetRandomValue( generator, 500 );
			}
		};

		template<> struct Helpers< EFieldType_BINARY >
		{
			static const uint32_t Limit = 20;
			typedef ByteArray ParamType;
			typedef ParamType FieldType;

			static ParamType GetRandomValue( std::random_device & generator, size_t size )
			{
				std::uniform_int_distribution< int > distribution( 32, 126 );
				ByteArray blob( size );

				for ( auto & value : blob )
				{
					char c = char( distribution( generator ) );

					if ( c == '\\' )
					{
						c = '/';
					}

					value = c;
				};

				return blob;
			}

			static ParamType GetRandomValue( std::random_device & generator )
			{
				return GetRandomValue( generator, Limit );
			}
		};

		template<> struct Helpers< EFieldType_VARBINARY >
		{
			static const uint32_t Limit = 255;
			typedef ByteArray ParamType;
			typedef ParamType FieldType;

			static ParamType GetRandomValue( std::random_device & generator, size_t size )
			{
				std::uniform_int_distribution< int > distribution( 32, 126 );
				ByteArray blob( size );

				for ( auto & value : blob )
				{
					char c = char( distribution( generator ) );

					if ( c == '\\' )
					{
						c = '/';
					}

					value = c;
				};

				return blob;
			}

			static ParamType GetRandomValue( std::random_device & generator )
			{
				return GetRandomValue( generator, Limit );
			}
		};

		template<> struct Helpers< EFieldType_BLOB >
		{
			static const uint32_t Limit = -1;
			typedef ByteArray ParamType;
			typedef ParamType FieldType;

			static ParamType GetRandomValue( std::random_device & generator, size_t size )
			{
				std::uniform_int_distribution< int > distribution( 32, 126 );
				ByteArray blob( size );

				for ( auto & value : blob )
				{
					char c = char( distribution( generator ) );

					if ( c == '\\' )
					{
						c = '/';
					}

					value = c;
				};

				return blob;
			}

			static ParamType GetRandomValue( std::random_device & generator )
			{
				return GetRandomValue( generator, 1024 );
			}
		};

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
		inline void UncheckedCreateParameters( std::shared_ptr< StmtType > stmt )
		{
			stmt->CreateParameter( STR( "IntField" ), EFieldType_SINT32 );
			stmt->CreateParameter( STR( "IntegerField" ), EFieldType_SINT32 );
			stmt->CreateParameter( STR( "TinyIntField" ), EFieldType_UINT8 );
			stmt->CreateParameter( STR( "SmallIntField" ), EFieldType_SINT16 );
			stmt->CreateParameter( STR( "MediumIntField" ), EFieldType_SINT24 );
			stmt->CreateParameter( STR( "BigIntField" ), EFieldType_SINT64 );
			stmt->CreateParameter( STR( "Int2Field" ), EFieldType_SINT16 );
			stmt->CreateParameter( STR( "Int8Field" ), EFieldType_SINT64 );
			stmt->CreateParameter( STR( "RealField" ), EFieldType_FLOAT64 );
			stmt->CreateParameter( STR( "DoubleField" ), EFieldType_FLOAT64 );
			stmt->CreateParameter( STR( "DoublePrecisionField" ), EFieldType_FLOAT64 );
			stmt->CreateParameter( STR( "FloatField" ), EFieldType_FLOAT32 );
			stmt->CreateParameter( STR( "NumericField" ), EFieldType_FIXED_POINT, std::make_pair( 10, 0 ) );
			stmt->CreateParameter( STR( "DecimalField" ), EFieldType_FIXED_POINT, std::make_pair( 10, 5 ) );
			stmt->CreateParameter( STR( "BooleanField" ), EFieldType_BIT );
			stmt->CreateParameter( STR( "DateField" ), EFieldType_DATE );
			stmt->CreateParameter( STR( "DateTimeField" ), EFieldType_DATETIME );
			stmt->CreateParameter( STR( "CharacterField" ), EFieldType_CHAR, 20 );
			stmt->CreateParameter( STR( "VarcharField" ), EFieldType_VARCHAR, 255 );
			stmt->CreateParameter( STR( "NcharField" ), EFieldType_NCHAR, 55 );
			stmt->CreateParameter( STR( "NVarcharField" ), EFieldType_NVARCHAR, 100 );
			stmt->CreateParameter( STR( "TextField" ), EFieldType_TEXT );
			stmt->CreateParameter( STR( "BlobField" ), EFieldType_VARBINARY );
		}

		template< class StmtType >
		inline void SetParametersValue( std::random_device & generator, uint32_t & index, int mult, int i, std::shared_ptr< StmtType > stmt )
		{
			stmt->SetParameterValue( index++, Helpers< EFieldType_SINT32 >::GetRandomValue( generator ) );
			stmt->SetParameterValue( index++, Helpers< EFieldType_SINT32 >::GetRandomValue( generator ) );
			stmt->SetParameterValue( index++, Helpers< EFieldType_UINT8 >::GetRandomValue( generator ) );
			stmt->SetParameterValue( index++, Helpers< EFieldType_SINT16 >::GetRandomValue( generator ) );
			stmt->SetParameterValue( index++, Helpers< EFieldType_SINT24 >::GetRandomValue( generator ) );
			stmt->SetParameterValue( index++, Helpers< EFieldType_SINT64 >::GetRandomValue( generator ) );
			stmt->SetParameterValue( index++, Helpers< EFieldType_SINT16 >::GetRandomValue( generator ) );
			stmt->SetParameterValue( index++, Helpers< EFieldType_SINT64 >::GetRandomValue( generator ) );
			stmt->SetParameterValue( index++, Helpers< EFieldType_FLOAT64 >::GetRandomValue( generator ) );
			stmt->SetParameterValue( index++, Helpers< EFieldType_FLOAT64 >::GetRandomValue( generator ) );
			stmt->SetParameterValue( index++, Helpers< EFieldType_FLOAT64 >::GetRandomValue( generator ) );
			stmt->SetParameterValue( index++, Helpers< EFieldType_FLOAT32 >::GetRandomValue( generator ) );
			stmt->SetParameterValue( index++, Helpers< EFieldType_FIXED_POINT >::GetRandomValue( generator, 10, 0 ) );
			stmt->SetParameterValue( index++, Helpers< EFieldType_FIXED_POINT >::GetRandomValue( generator, 10, 5 ) );
			stmt->SetParameterValue( index++, Helpers< EFieldType_BIT >::GetRandomValue( generator ) );
			stmt->SetParameterValue( index++, Helpers< EFieldType_DATE >::GetRandomValue( generator ) );
			stmt->SetParameterValue( index++, Helpers< EFieldType_DATETIME >::GetRandomValue( generator ) );
			stmt->SetParameterValue( index++, Helpers< EFieldType_CHAR >::GetRandomValue( generator ) );
			stmt->SetParameterValue( index++, Helpers< EFieldType_VARCHAR >::GetRandomValue( generator ) );
			stmt->SetParameterValue( index++, Helpers< EFieldType_NCHAR >::GetRandomValue( generator ) );
			stmt->SetParameterValue( index++, Helpers< EFieldType_NVARCHAR >::GetRandomValue( generator ) );
			stmt->SetParameterValue( index++, Helpers< EFieldType_TEXT >::GetRandomValue( generator ) );
			stmt->SetParameterValue( index++, Helpers< EFieldType_VARBINARY >::GetRandomValue( generator ) );
		}

		inline void DisplayValues( uint32_t & index, DatabaseRowSPtr row )
		{
			CLogger::LogInfo( StringStream() << STR( "IntField : " ) << row->Get< int32_t >( index++ ) );
			CLogger::LogInfo( StringStream() << STR( "IntegerField : " ) << row->Get< int32_t >( index++ ) );
			CLogger::LogInfo( StringStream() << STR( "TinyIntField : " ) << int( row->Get< int16_t >( index++ ) ) );
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
			CLogger::LogInfo( StringStream() << STR( "DateField : " ) << row->Get< DateType >( index++ ) );
			CLogger::LogInfo( StringStream() << STR( "DateTimeField : " ) << row->Get< DateTimeType >( index++ ) );
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
			typedef typename Helpers< FieldType >::ParamType param_type;
			typedef typename Helpers< FieldType >::FieldType field_type;

			static void Equal( param_type const & a, field_type const & b )
			{
				BOOST_CHECK_EQUAL( a, b );
			}
			static void Different( param_type const & a, field_type const & b )
			{
				BOOST_CHECK_NE( a, b );
			}
			void operator()( bool equal, param_type const & a, field_type const & b )
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
		struct Compare< EFieldType_SINT8 >
		{
			typedef Helpers< EFieldType_SINT8 >::ParamType param_type;
			typedef Helpers< EFieldType_SINT8 >::FieldType field_type;

			static void Equal( param_type const & a, field_type const & b )
			{
				BOOST_CHECK_EQUAL( int16_t( a ), int16_t( b ) );
			}
			static void Different( param_type const & a, field_type const & b )
			{
				BOOST_CHECK_NE( int16_t( a ), int16_t( b ) );
			}
			void operator()( bool equal, param_type const & a, field_type const & b )
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
		struct Compare< EFieldType_UINT8 >
		{
			typedef Helpers< EFieldType_UINT8 >::ParamType param_type;
			typedef Helpers< EFieldType_UINT8 >::FieldType field_type;

			static void Equal( param_type const & a, field_type const & b )
			{
				BOOST_CHECK_EQUAL( uint16_t( a ), uint16_t( b ) );
			}
			static void Different( param_type const & a, field_type const & b )
			{
				BOOST_CHECK_NE( int16_t( a ), uint16_t( b ) );
			}
			void operator()( bool equal, param_type const & a, field_type const & b )
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
			typedef Helpers< EFieldType_FLOAT32 >::ParamType param_type;
			typedef Helpers< EFieldType_FLOAT32 >::FieldType field_type;

			static void Equal( param_type const & a, field_type const & b )
			{
				// Can't compare, due to precision errors
				//if ( a != b )
				//{
				//	if ( std::abs( a - b ) > std::numeric_limits< param_type >::epsilon() )
				//	{
				//		BOOST_CHECK_EQUAL( a, b );
				//	}
				//}
			}
			static void Different( param_type const & a, field_type const & b )
			{
				BOOST_CHECK_NE( a, b );
			}
			void operator()( bool equal, param_type const & a, field_type const & b )
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
			typedef Helpers< EFieldType_FLOAT64 >::ParamType param_type;
			typedef Helpers< EFieldType_FLOAT64 >::FieldType field_type;

			static void Equal( param_type const & a, field_type const & b )
			{
				// Can't compare, due to precision errors
				//if ( a != b )
				//{
				//	if ( std::abs( a - b ) > std::numeric_limits< param_type >::epsilon() )
				//	{
				//		BOOST_CHECK_EQUAL( a, b );
				//	}
				//}
			}
			static void Different( param_type const & a, field_type const & b )
			{
				BOOST_CHECK_NE( a, b );
			}
			void operator()( bool equal, param_type const & a, field_type const & b )
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
		struct Compare< EFieldType_CHAR >
		{
			typedef Helpers< EFieldType_CHAR >::ParamType param_type;
			typedef Helpers< EFieldType_CHAR >::FieldType field_type;

			static void Equal( param_type const & a, field_type const & b )
			{
				BOOST_CHECK_EQUAL( std::string( a ), std::string( b ) );
			}
			static void Different( param_type const & a, field_type const & b )
			{
				BOOST_CHECK_NE( std::string( a ), std::string( b ) );
			}
			void operator()( bool equal, param_type const & a, field_type const & b )
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
			typedef Helpers< EFieldType_VARCHAR >::ParamType param_type;
			typedef Helpers< EFieldType_VARCHAR >::FieldType field_type;

			static void Equal( param_type const & a, field_type const & b )
			{
				BOOST_CHECK_EQUAL( std::string( a ), std::string( b ) );
			}
			static void Different( param_type const & a, field_type const & b )
			{
				BOOST_CHECK_NE( std::string( a ), std::string( b ) );
			}
			void operator()( bool equal, param_type const & a, field_type const & b )
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
		struct Compare< EFieldType_NCHAR >
		{
			typedef Helpers< EFieldType_NCHAR >::ParamType param_type;
			typedef Helpers< EFieldType_NCHAR >::FieldType field_type;

			static void Equal( param_type const & a, field_type const & b )
			{
				BOOST_CHECK_EQUAL( StringUtils::ToStr( a ), StringUtils::ToStr( b ) );
			}
			static void Different( param_type const & a, field_type const & b )
			{
				BOOST_CHECK_NE( StringUtils::ToStr( a ), StringUtils::ToStr( b ) );
			}
			void operator()( bool equal, param_type const & a, field_type const & b )
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
			typedef Helpers< EFieldType_NVARCHAR >::ParamType param_type;
			typedef Helpers< EFieldType_NVARCHAR >::FieldType field_type;

			static void Equal( param_type const & a, field_type const & b )
			{
				BOOST_CHECK_EQUAL( StringUtils::ToStr( a ), StringUtils::ToStr( b ) );
			}
			static void Different( param_type const & a, field_type const & b )
			{
				BOOST_CHECK_NE( StringUtils::ToStr( a ), StringUtils::ToStr( b ) );
			}
			void operator()( bool equal, param_type const & a, field_type const & b )
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
			typedef Helpers< EFieldType_VARBINARY >::ParamType param_type;
			typedef Helpers< EFieldType_VARBINARY >::FieldType field_type;

			static void Equal( param_type const & a, field_type const & b )
			{
				BOOST_CHECK_EQUAL_COLLECTIONS( a.begin(), a.end(), b.begin(), b.end() );
			}
			static void Different( param_type const & a, field_type const & b )
			{
				BOOST_CHECK( a != b );
			}
			void operator()( bool equal, param_type const & a, field_type const & b )
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

		template< EFieldType FieldType, typename Enable=void >
		struct ParameterCreator
		{
			template< typename StmtType >
			static void Create( std::shared_ptr< StmtType > stmt, const String & name )
			{
				BOOST_CHECK( stmt->CreateParameter( name, FieldType ) );
			}
		};

		template< EFieldType FieldType >
		struct ParameterCreator< FieldType, typename std::enable_if< SFieldTypeNeedsLimits< FieldType >::value >::type >
		{
			template< typename StmtType >
			static void Create( std::shared_ptr< StmtType > stmt, const String & name )
			{
				BOOST_CHECK( stmt->CreateParameter( name, FieldType, Helpers< FieldType >::Limit ) );
			}
		};

		template< EFieldType FieldType >
		struct ParameterCreator< FieldType, typename std::enable_if< SFieldTypeNeedsPrecision< FieldType >::value >::type >
		{
			template< typename StmtType >
			static void Create( std::shared_ptr< StmtType > stmt, const String & name )
			{
				BOOST_CHECK( stmt->CreateParameter( name, FieldType, Helpers< FieldType >::Precision ) );
			}
		};

		template< EFieldType FieldType >
		inline void InsertAndRetrieve( DatabaseConnectionSPtr connection, const String & name, typename Helpers< FieldType >::ParamType const * valueIn, bool equal, String const & is )
		{
		};

		template< class StmtType, EFieldType FieldType >
		inline void InsertAndRetrieve( DatabaseConnectionSPtr connection, const String & name, typename Helpers< FieldType >::ParamType const * valueIn, bool equal, String const & is )
		{
			try
			{
				auto stmtInsert = CreateStmt< StmtType >( connection, STR( "INSERT INTO Test (" ) + name + STR( ") VALUES (?)" ) );
				std::shared_ptr< StmtType > stmtSelect;

				if ( valueIn )
				{
					stmtSelect = CreateStmt< StmtType >( connection, STR( "SELECT " ) + name + STR( " FROM Test WHERE " ) + name + STR( " = ?" ) );
				}
				else
				{
					stmtSelect = CreateStmt< StmtType >( connection, STR( "SELECT " ) + name + STR( " FROM Test WHERE " ) + name + STR( " " ) + is + STR( " ?" ) );
				}

				BOOST_CHECK( stmtInsert );
				BOOST_CHECK( stmtSelect );

				if ( stmtInsert && stmtSelect )
				{
					ParameterCreator< FieldType >::Create( stmtInsert, name );
					ParameterCreator< FieldType >::Create( stmtSelect, name );

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
					DatabaseResultSPtr result = stmtSelect->ExecuteSelect();

					if ( result )
					{
						if ( result->GetRowCount() )
						{
							if ( valueIn )
							{
								typename Helpers< FieldType >::FieldType valueOut;
								BOOST_CHECK_NO_THROW( result->GetFirstRow()->Get( 0, valueOut ) );
								Compare< FieldType >()( equal, static_cast< CDatabaseValue< FieldType > const & >( stmtInsert->GetParameter( 0 )->GetObjectValue() ).GetValue(), valueOut );
							}
							else
							{
								try
								{
									DatabaseFieldSPtr field = result->GetFirstRow()->GetField( 0 );
									BOOST_CHECK( field->GetObjectValue().IsNull() );
								}
								catch ( CDatabaseException & exc )
								{
									CLogger::LogError( exc.GetFullDescription() );
									BOOST_CHECK_NO_THROW( result->GetFirstRow()->GetField( 0 ) );
								}
							}
						}
						else
						{
							BOOST_CHECK( result->GetRowCount() );
						}
					}
					else
					{
						BOOST_CHECK( result );
					}
				}
			}
			catch ( CDatabaseException & exc )
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

		template< class StmtType, EFieldType FieldType >
		inline void InsertAndRetrieveOtherIndex( DatabaseConnectionSPtr connection, const String & name, typename Helpers< FieldType >::ParamType const * valueIn, bool equal, String const & is )
		{
			try
			{
				auto stmtInsert = CreateStmt< StmtType >( connection, STR( "INSERT INTO Test (IntField, " ) + name + STR( ") VALUES (?, ?)" ) );
				std::shared_ptr< StmtType > stmtSelect;

				if ( valueIn )
				{
					stmtSelect = CreateStmt< StmtType >( connection, STR( "SELECT " ) + name + STR( " FROM Test WHERE IntField = ? AND " ) + name + STR( " = ?" ) );
				}
				else
				{
					stmtSelect = CreateStmt< StmtType >( connection, STR( "SELECT " ) + name + STR( " FROM Test WHERE IntField = ? AND " ) + name + STR( " " ) + is + STR( " ?" ) );
				}

				BOOST_CHECK( stmtInsert );
				BOOST_CHECK( stmtSelect );

				if ( stmtInsert && stmtSelect )
				{
					ParameterCreator< EFieldType_SINT32 >::Create( stmtInsert, STR( "IntField" ) );
					ParameterCreator< FieldType >::Create( stmtInsert, name );
					ParameterCreator< EFieldType_SINT32 >::Create( stmtSelect, STR( "IntField" ) );
					ParameterCreator< FieldType >::Create( stmtSelect, name );

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
					DatabaseResultSPtr result = stmtSelect->ExecuteSelect();

					if ( result )
					{
						if ( result->GetRowCount() )
						{
							if ( valueIn )
							{
								typename Helpers< FieldType >::FieldType valueOut;
								BOOST_CHECK_NO_THROW( result->GetFirstRow()->Get( 0, valueOut ) );
								Compare< FieldType >()( equal, static_cast< CDatabaseValue< FieldType > const & >( stmtInsert->GetParameter( 1 )->GetObjectValue() ).GetValue(), valueOut );
							}
							else
							{
								try
								{
									DatabaseFieldSPtr field = result->GetFirstRow()->GetField( 0 );
									BOOST_CHECK( field->GetObjectValue().IsNull() );
								}
								catch ( CDatabaseException & exc )
								{
									CLogger::LogError( exc.GetFullDescription() );
									BOOST_CHECK_NO_THROW( result->GetFirstRow()->GetField( 0 ) );
								}
							}
						}
						else
						{
							BOOST_CHECK( result->GetRowCount() );
						}
					}
					else
					{
						BOOST_CHECK( result );
					}
				}
			}
			catch ( CDatabaseException & exc )
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

		template< class StmtType, EFieldType FieldType >
		inline void InsertAndRetrieveOtherIndex( DatabaseConnectionSPtr connection, const String & name, typename Helpers< FieldType >::ParamType const & valueIn, bool equal, String const & is )
		{
			InsertAndRetrieveOtherIndex< StmtType, FieldType >( connection, name, &valueIn, equal, is );
		}

		template< class StmtType, EFieldType FieldType >
		inline void InsertAndRetrieveOtherIndex( DatabaseConnectionSPtr connection, const String & name, bool equal, String const & is, std::random_device & generator )
		{
			InsertAndRetrieveOtherIndex< StmtType, FieldType >( connection, name, Helpers< FieldType >::GetRandomValue( generator ), equal, is );
		}

		template< class StmtType, EFieldType FieldType >
		inline void InsertAndRetrieveFast( DatabaseConnectionSPtr connection, const String & name, typename Helpers< FieldType >::ParamType const * valueIn, bool equal, String const & is )
		{
			try
			{
				auto stmtInsert = CreateStmt< StmtType >( connection, STR( "INSERT INTO Test (" ) + name + STR( ") VALUES (?)" ) );
				std::shared_ptr< StmtType > stmtSelect;

				if ( valueIn )
				{
					stmtSelect = CreateStmt< StmtType >( connection, STR( "SELECT " ) + name + STR( " FROM Test WHERE " ) + name + STR( " = ?" ) );
				}
				else
				{
					stmtSelect = CreateStmt< StmtType >( connection, STR( "SELECT " ) + name + STR( " FROM Test WHERE " ) + name + STR( " " ) + is + STR( " ?" ) );
				}

				BOOST_CHECK( stmtInsert );
				BOOST_CHECK( stmtSelect );

				if ( stmtInsert && stmtSelect )
				{
					ParameterCreator< FieldType >::Create( stmtInsert, name );
					ParameterCreator< FieldType >::Create( stmtSelect, name );

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
					DatabaseResultSPtr result = stmtSelect->ExecuteSelect();

					if ( result )
					{
						if ( result->GetRowCount() )
						{
							if ( valueIn )
							{
								typename Helpers< FieldType >::FieldType valueOut;
								BOOST_CHECK_NO_THROW( result->GetFirstRow()->GetFast( 0, valueOut ) );
								Compare< FieldType >()( equal, static_cast< CDatabaseValue< FieldType > const & >( stmtInsert->GetParameter( 0 )->GetObjectValue() ).GetValue(), valueOut );
							}
							else
							{
								try
								{
									DatabaseFieldSPtr field = result->GetFirstRow()->GetField( 0 );
									BOOST_CHECK( field->GetObjectValue().IsNull() );
								}
								catch ( CDatabaseException & exc )
								{
									CLogger::LogError( exc.GetFullDescription() );
									BOOST_CHECK_NO_THROW( result->GetFirstRow()->GetField( 0 ) );
								}
							}
						}
						else
						{
							BOOST_CHECK( result->GetRowCount() );
						}
					}
					else
					{
						BOOST_CHECK( result );
					}
				}
			}
			catch ( CDatabaseException & exc )
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

		template< class StmtType, EFieldType FieldType >
		inline void InsertAndRetrieveFast( DatabaseConnectionSPtr connection, const String & name, typename Helpers< FieldType >::ParamType const & valueIn, bool equal, String const & is )
		{
			InsertAndRetrieveFast< StmtType, FieldType >( connection, name, &valueIn, equal, is );
		}

		template< class StmtType, EFieldType FieldType >
		inline void InsertAndRetrieveFast( DatabaseConnectionSPtr connection, const String & name, bool equal, String const & is, std::random_device & generator )
		{
			InsertAndRetrieveFast< StmtType, FieldType >( connection, name, Helpers< FieldType >::GetRandomValue( generator ), equal, is );
		}

		template< class StmtType, EFieldType FieldType >
		inline void InsertAndRetrieveFastOtherIndex( DatabaseConnectionSPtr connection, const String & name, typename Helpers< FieldType >::ParamType const * valueIn, bool equal, String const & is )
		{
			try
			{
				auto stmtInsert = CreateStmt< StmtType >( connection, STR( "INSERT INTO Test (IntField, " ) + name + STR( ") VALUES (?, ?)" ) );
				std::shared_ptr< StmtType > stmtSelect;

				if ( valueIn )
				{
					stmtSelect = CreateStmt< StmtType >( connection, STR( "SELECT " ) + name + STR( " FROM Test WHERE IntField = ? AND " ) + name + STR( " = ?" ) );
				}
				else
				{
					stmtSelect = CreateStmt< StmtType >( connection, STR( "SELECT " ) + name + STR( " FROM Test WHERE IntField = ? AND " ) + name + STR( " " ) + is + STR( " ?" ) );
				}

				BOOST_CHECK( stmtInsert );
				BOOST_CHECK( stmtSelect );

				if ( stmtInsert && stmtSelect )
				{
					ParameterCreator< EFieldType_SINT32 >::Create( stmtInsert, STR( "IntField" ) );
					ParameterCreator< FieldType >::Create( stmtInsert, name );
					ParameterCreator< EFieldType_SINT32 >::Create( stmtSelect, STR( "IntField" ) );
					ParameterCreator< FieldType >::Create( stmtSelect, name );

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
					DatabaseResultSPtr result = stmtSelect->ExecuteSelect();

					if ( result )
					{
						if ( result->GetRowCount() )
						{
							if ( valueIn )
							{
								typename Helpers< FieldType >::FieldType valueOut;
								BOOST_CHECK_NO_THROW( result->GetFirstRow()->GetFast( 0, valueOut ) );
								Compare< FieldType >()( equal, static_cast< CDatabaseValue< FieldType > const & >( stmtInsert->GetParameter( 1 )->GetObjectValue() ).GetValue(), valueOut );
							}
							else
							{
								try
								{
									DatabaseFieldSPtr field = result->GetFirstRow()->GetField( 0 );
									BOOST_CHECK( field->GetObjectValue().IsNull() );
								}
								catch ( CDatabaseException & exc )
								{
									CLogger::LogError( exc.GetFullDescription() );
									BOOST_CHECK_NO_THROW( result->GetFirstRow()->GetField( 0 ) );
								}
							}
						}
						else
						{
							BOOST_CHECK( result->GetRowCount() );
						}
					}
					else
					{
						BOOST_CHECK( result );
					}
				}
			}
			catch ( CDatabaseException & exc )
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

		template< class StmtType, EFieldType FieldType >
		inline void InsertAndRetrieveFastOtherIndex( DatabaseConnectionSPtr connection, const String & name, typename Helpers< FieldType >::ParamType const & valueIn, bool equal, String const & is )
		{
			InsertAndRetrieveFastOtherIndex< StmtType, FieldType >( connection, name, &valueIn, equal, is );
		}

		template< class StmtType, EFieldType FieldType >
		inline void InsertAndRetrieveFastOtherIndex( DatabaseConnectionSPtr connection, const String & name, bool equal, String const & is, std::random_device & generator )
		{
			InsertAndRetrieveFastOtherIndex< StmtType, FieldType >( connection, name, Helpers< FieldType >::GetRandomValue( generator ), equal, is );
		}

		template< typename StmtType >
		inline void TestDirectInsert( std::random_device & generator, DatabaseConnectionSPtr connection )
		{
			try
			{
				auto stmtGetCount = CreateStmt< StmtType >( connection, QUERY_GET_COUNT );
				BOOST_CHECK( stmtGetCount );
				int64_t count = -1;

				if ( stmtGetCount )
				{
					BOOST_CHECK( stmtGetCount->Initialize() == EErrorType_NONE );
					DatabaseResultSPtr result = stmtGetCount->ExecuteSelect();

					if ( result )
					{
						if ( result->GetRowCount() )
						{
							BOOST_CHECK_NO_THROW( result->GetFirstRow()->Get( 0, count ) );
						}
						else
						{
							BOOST_CHECK( result->GetRowCount() );
						}
					}
					else
					{
						BOOST_CHECK( result );
					}
				}

				count++;

				if ( count )
				{
					auto stmtInsert = CreateStmt< StmtType >( connection, QUERY_DIRECT_INSERT_ELEMENT );
					BOOST_CHECK( stmtInsert );

					if ( stmtInsert )
					{
						CreateParameters( stmtInsert );
						BOOST_CHECK( stmtInsert->Initialize() == EErrorType_NONE );
						int const inserts = 20;

						for ( int i = 0; i < inserts; i++ )
						{
							uint32_t index = 0;
							SetParametersValue( generator, index, i + 1, i, stmtInsert );
							BOOST_CHECK( stmtInsert->ExecuteUpdate() );
						}

						count--;
						DatabaseResultSPtr result = stmtGetCount->ExecuteSelect();

						if ( result )
						{
							if ( result->GetRowCount() )
							{
								int64_t field = 0;
								BOOST_CHECK_NO_THROW( result->GetFirstRow()->Get( 0, field ) );
								BOOST_CHECK_EQUAL( field, count + inserts );
								count++;
							}
							else
							{
								BOOST_CHECK( result->GetRowCount() );
							}
						}
						else
						{
							BOOST_CHECK( result );
						}
					}
				}
			}
			catch ( CDatabaseException & exc )
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

		template< typename StmtType >
		inline void TestDirectSelect( std::random_device & generator, DatabaseConnectionSPtr connection )
		{
			try
			{
				auto stmtSelect = CreateStmt< StmtType >( connection, QUERY_DIRECT_SELECT_ALL_ELEMENTS );
				BOOST_CHECK( stmtSelect );

				if ( stmtSelect )
				{
					BOOST_CHECK( stmtSelect->Initialize() == EErrorType_NONE );
					DatabaseResultSPtr result = stmtSelect->ExecuteSelect();

					if ( result )
					{
						if ( result->GetRowCount() )
						{
							DatabaseRowSPtr row = result->GetFirstRow();

							while ( row )
							{
								uint32_t index = 0;
								DisplayValues( index, row );
								row = result->GetNextRow();
							}
						}
						else
						{
							BOOST_CHECK( result->GetRowCount() );
						}
					}
					else
					{
						BOOST_CHECK( result );
					}
				}
			}
			catch ( CDatabaseException & exc )
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

		template< typename StmtType >
		inline void TestDirectUpdate( std::random_device & generator, DatabaseConnectionSPtr connection )
		{
			try
			{
				auto stmtGetMin = CreateStmt< StmtType >( connection, QUERY_SELECT_MIN );
				BOOST_CHECK( stmtGetMin );
				int64_t min = 0;

				if ( stmtGetMin )
				{
					BOOST_CHECK( stmtGetMin->Initialize() == EErrorType_NONE );
					DatabaseResultSPtr result = stmtGetMin->ExecuteSelect();

					if ( result )
					{
						if ( result->GetRowCount() )
						{
							BOOST_CHECK_NO_THROW( result->GetFirstRow()->Get( 0, min ) );
						}
						else
						{
							BOOST_CHECK( result->GetRowCount() );
						}
					}
					else
					{
						BOOST_CHECK( result );
					}
				}

				if ( min )
				{
					auto stmtUpdate = CreateStmt< StmtType >( connection, QUERY_DIRECT_UPDATE_ELEMENT );
					BOOST_CHECK( stmtUpdate );

					if ( stmtUpdate )
					{
						CreateParameters( stmtUpdate );
						BOOST_CHECK( stmtUpdate->CreateParameter( STR( "IDTest" ), EFieldType_SINT64, EParameterType_IN ) );
						BOOST_CHECK( stmtUpdate->Initialize() == EErrorType_NONE );

						for ( int i = 0; i < 10; i++ )
						{
							uint32_t index = 0;
							SetParametersValue( generator, index, i + 40, i, stmtUpdate );
							stmtUpdate->SetParameterValue( index++, min + i );
							BOOST_CHECK( stmtUpdate->ExecuteUpdate() );
						}
					}
				}
			}
			catch ( CDatabaseException & exc )
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

		template< typename StmtType >
		inline void TestDirectDelete( std::random_device & generator, DatabaseConnectionSPtr connection )
		{
			try
			{
				auto stmtGetMin = CreateStmt< StmtType >( connection, QUERY_SELECT_MIN );
				BOOST_CHECK( stmtGetMin );
				int64_t min = -1;

				if ( stmtGetMin )
				{
					BOOST_CHECK( stmtGetMin->Initialize() == EErrorType_NONE );
					DatabaseResultSPtr result = stmtGetMin->ExecuteSelect();

					if ( result )
					{
						if ( result->GetRowCount() )
						{
							BOOST_CHECK_NO_THROW( result->GetFirstRow()->Get( 0, min ) );
						}
						else
						{
							BOOST_CHECK( result->GetRowCount() );
						}
					}
					else
					{
						BOOST_CHECK( result );
					}
				}

				if ( min )
				{
					auto stmtDelete = CreateStmt< StmtType >( connection, QUERY_DIRECT_DELETE_ELEMENT );
					BOOST_CHECK( stmtDelete );

					if ( stmtDelete )
					{
						BOOST_CHECK( stmtDelete->CreateParameter( STR( "IDTest" ), EFieldType_SINT64, EParameterType_IN ) );
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
			catch ( CDatabaseException & exc )
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

		template< typename StmtType >
		inline void TestStoredNoParamNoReturn( std::random_device & generator, DatabaseConnectionSPtr connection )
		{
			try
			{
				auto stmtClear = CreateStmt< StmtType >( connection, STR( "CALL ClearElementsTables" ) );
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
			catch ( CDatabaseException & exc )
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

		template< typename StmtType >
		inline void TestStoredNoParamReturn( std::random_device & generator, DatabaseConnectionSPtr connection, const String & where )
		{
			try
			{
				auto stmtGetElements = CreateStmt< StmtType >( connection, STR( "CALL GetElements(?)" ) );
				BOOST_CHECK( stmtGetElements );

				if ( stmtGetElements )
				{
					BOOST_CHECK( stmtGetElements->CreateParameter( STR( "Where" ), EFieldType_VARCHAR, 500 ) );
					BOOST_CHECK( stmtGetElements->Initialize() == EErrorType_NONE );

					for ( int i = 0; i < 2; i++ )
					{
						stmtGetElements->SetParameterValue( 0, where );
						DatabaseResultSPtr result = stmtGetElements->ExecuteSelect();

						if ( result )
						{
							if ( result->GetRowCount() )
							{
								DatabaseRowSPtr row = result->GetFirstRow();

								while ( row )
								{
									uint32_t index = 0;
									DisplayValues( index, row );
									row = result->GetNextRow();
								}
							}
							else
							{
								BOOST_CHECK( result->GetRowCount() );
							}
						}
						else
						{
							BOOST_CHECK( result );
						}
					}
				}
			}
			catch ( CDatabaseException & exc )
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

		template< typename StmtType >
		inline void TestStoredInParamNoReturn( std::random_device & generator, DatabaseConnectionSPtr connection )
		{
			try
			{
				auto stmtGetMin = CreateStmt< StmtType >( connection, QUERY_SELECT_MIN );
				BOOST_CHECK( stmtGetMin );
				int64_t min = 0;

				if ( stmtGetMin )
				{
					BOOST_CHECK( stmtGetMin->Initialize() == EErrorType_NONE );
					DatabaseResultSPtr result = stmtGetMin->ExecuteSelect();

					if ( result )
					{
						if ( result->GetRowCount() )
						{
							BOOST_CHECK_NO_THROW( result->GetFirstRow()->Get( 0, min ) );
						}
						else
						{
							BOOST_CHECK( result->GetRowCount() );
						}
					}
					else
					{
						BOOST_CHECK( result );
					}
				}

				if ( min )
				{
					auto stmtDelete = CreateStmt< StmtType >( connection, STR( "CALL DeleteElements(?)" ) );
					BOOST_CHECK( stmtDelete );

					if ( stmtDelete )
					{
						BOOST_CHECK( stmtDelete->CreateParameter( STR( "IDTest" ), EFieldType_SINT64 ) );
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
			catch ( CDatabaseException & exc )
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

		template< typename StmtType >
		inline void TestStoredInOutParamNoReturn( std::random_device & generator, DatabaseConnectionSPtr connection )
		{
			try
			{
				auto stmtGetCount = CreateStmt< StmtType >( connection, QUERY_GET_COUNT );
				BOOST_CHECK( stmtGetCount );
				int64_t count = -1;

				if ( stmtGetCount )
				{
					BOOST_CHECK( stmtGetCount->Initialize() == EErrorType_NONE );
					DatabaseResultSPtr result = stmtGetCount->ExecuteSelect();

					if ( result )
					{
						if ( result->GetRowCount() )
						{
							BOOST_CHECK_NO_THROW( result->GetFirstRow()->Get( 0, count ) );
						}
						else
						{
							BOOST_CHECK( result->GetRowCount() );
						}
					}
					else
					{
						BOOST_CHECK( result );
					}
				}

				count++;

				if ( count )
				{
					auto stmtAddElement = CreateStmt< StmtType >( connection, STR( "CALL AddElement(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);" ) );
					BOOST_CHECK( stmtAddElement );

					if ( stmtAddElement )
					{
						BOOST_CHECK( stmtAddElement->CreateParameter( STR( "IDTest" ), EFieldType_SINT64, EParameterType_INOUT ) );
						CreateParameters( stmtAddElement );
						BOOST_CHECK( stmtAddElement->Initialize() == EErrorType_NONE );
						int64_t id( 1 );

						for ( int i = 0; i < 10; i++ )
						{
							uint32_t index = 0;
							stmtAddElement->SetParameterNull( index++ );
							SetParametersValue( generator, index, i + 40, i, stmtAddElement );
							BOOST_CHECK( stmtAddElement->ExecuteUpdate() );
							int64_t tmp;
							stmtAddElement->GetOutputValue( 0, tmp );
							BOOST_CHECK_EQUAL( i + count, tmp );
						}
					}
				}
			}
			catch ( CDatabaseException & exc )
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

		template< typename StmtType >
		inline void TestStoredInOutDtParamNoReturn( std::random_device & generator, DatabaseConnectionSPtr connection )
		{
			try
			{
				auto stmtGetCount = CreateStmt< StmtType >( connection, QUERY_GET_COUNT );
				BOOST_CHECK( stmtGetCount );
				int64_t count = -1;

				if ( stmtGetCount )
				{
					BOOST_CHECK( stmtGetCount->Initialize() == EErrorType_NONE );
					DatabaseResultSPtr result = stmtGetCount->ExecuteSelect();

					if ( result )
					{
						if ( result->GetRowCount() )
						{
							BOOST_CHECK_NO_THROW( result->GetFirstRow()->Get( 0, count ) );
						}
						else
						{
							BOOST_CHECK( result->GetRowCount() );
						}
					}
					else
					{
						BOOST_CHECK( result );
					}
				}

				count++;

				if ( count )
				{
					auto stmtUpdateElement = CreateStmt< StmtType >( connection, STR( "CALL UpdateElement(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);" ) );
					BOOST_CHECK( stmtUpdateElement );

					if ( stmtUpdateElement )
					{
						BOOST_CHECK( stmtUpdateElement->CreateParameter( STR( "IDTest" ), EFieldType_SINT64, EParameterType_INOUT ) );
						CreateParameters( stmtUpdateElement );
						BOOST_CHECK( stmtUpdateElement->CreateParameter( STR( "Date" ), EFieldType_DATETIME, EParameterType_OUT ) );
						BOOST_CHECK( stmtUpdateElement->Initialize() == EErrorType_NONE );
						int16_t type( 1 );

						for ( int i = 0; i < 10; i++ )
						{
							uint32_t index = 0;
							stmtUpdateElement->SetParameterNull( index++ );
							SetParametersValue( generator, index, i + 40, i, stmtUpdateElement );
							stmtUpdateElement->SetParameterNull( index++ );
							BOOST_CHECK( stmtUpdateElement->ExecuteUpdate() );
							int64_t tmp;
							stmtUpdateElement->GetOutputValue( 0, tmp );
							BOOST_CHECK_EQUAL( i + count, tmp );
						}
					}
				}
			}
			catch ( CDatabaseException & exc )
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

		template< typename StmtType >
		inline void PerfDirectSelectActors( DatabaseConnectionSPtr connection, uint32_t testCount, const String & whereClause )
		{
			try
			{
				std::shared_ptr< StmtType > stmtGetActors;

				if ( whereClause.empty() )
				{
					stmtGetActors = CreateStmt< StmtType >( connection, QUERY_DIRECT_SELECT_ALL_ELEMENTS );
				}
				else
				{
					stmtGetActors = CreateStmt< StmtType >( connection, QUERY_DIRECT_SELECT_ALL_ELEMENTS + STR( " " ) + whereClause );
				}

				if ( stmtGetActors )
				{
					stmtGetActors->Initialize();
					std::clock_t start = std::clock();
					DatabaseResultSPtr result = stmtGetActors->ExecuteSelect();
					CLogger::LogInfo( StringStream() << "    Selected the elements in " << float( std::clock() - start ) / CLOCKS_PER_SEC << "seconds (no fetch)" );

					start = std::clock();

					if ( result )
					{
						uint32_t count = result->GetRowCount();

						if ( count )
						{
							DatabaseRowSPtr row = result->GetFirstRow();

							while ( row )
							{
								row = result->GetNextRow();
							}
						}

						CLogger::LogInfo( StringStream() << "    Fetched " << count << " elements in " << float( std::clock() - start ) / CLOCKS_PER_SEC << "seconds" );
					}
				}
			}
			catch ( CDatabaseException & exc )
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

		template< typename StmtType >
		inline void PerfStoredProcedureSelectActors( DatabaseConnectionSPtr connection, uint32_t testCount, const String & whereClause )
		{
			try
			{
				auto stmtGetActors = CreateStmt< StmtType >( connection, STR( "CALL SpGetActors(?)" ) );

				if ( stmtGetActors )
				{
					stmtGetActors->CreateParameter( STR( "Where" ), EFieldType_VARCHAR, 500 );
					stmtGetActors->Initialize();
					std::clock_t start = std::clock();
					stmtGetActors->SetParameterValue( 0, whereClause );
					DatabaseResultSPtr result = stmtGetActors->ExecuteSelect();
					CLogger::LogInfo( StringStream() << "    Selected the elements in " << float( std::clock() - start ) / CLOCKS_PER_SEC << "seconds (no fetch)" );

					start = std::clock();

					if ( result )
					{
						uint32_t count = result->GetRowCount();

						if ( count )
						{
							DatabaseRowSPtr row = result->GetFirstRow();

							while ( row )
							{
								row = result->GetNextRow();
							}
						}

						CLogger::LogInfo( StringStream() << "    Fetched " << count << " elements in " << float( std::clock() - start ) / CLOCKS_PER_SEC << "seconds" );
					}
				}
			}
			catch ( CDatabaseException & exc )
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

		template< typename StmtType >
		inline void PerfDirectDeleteActors( DatabaseConnectionSPtr connection, uint32_t testCount )
		{
			try
			{
				auto stmtGetMin = CreateStmt< StmtType >( connection, QUERY_SELECT_MIN );
				int64_t min = 0;

				if ( stmtGetMin )
				{
					stmtGetMin->Initialize();
					DatabaseResultSPtr result = stmtGetMin->ExecuteSelect();

					if ( result && result->GetRowCount() )
					{
						result->GetFirstRow()->Get( 0, min );
					}
				}

				if ( min )
				{
					auto stmtDelete = CreateStmt< StmtType >( connection, QUERY_DIRECT_DELETE_ELEMENT );

					if ( stmtDelete )
					{
						stmtDelete->CreateParameter( STR( "id" ), EFieldType_SINT64 );
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
			catch ( CDatabaseException & exc )
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

		template< typename StmtType >
		inline void PerfStoredProcedureDeleteActors( DatabaseConnectionSPtr connection, uint32_t testCount )
		{
			try
			{
				auto stmtGetMin = CreateStmt< StmtType >( connection, QUERY_SELECT_MIN );
				int64_t min = 0;

				if ( stmtGetMin )
				{
					stmtGetMin->Initialize();
					DatabaseResultSPtr result = stmtGetMin->ExecuteSelect();

					if ( result )
					{
						if ( result->GetRowCount() )
						{
							result->GetFirstRow()->Get( 0, min );
						}
					}
				}

				if ( min )
				{
					auto stmtDelete = CreateStmt< StmtType >( connection, STR( "CALL SpDeleteActor(?)" ) );

					if ( stmtDelete )
					{
						stmtDelete->CreateParameter( STR( "id" ), EFieldType_SINT64 );
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
			catch ( CDatabaseException & exc )
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
