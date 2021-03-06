/************************************************************************//**
* @file DatabaseSqliteHelper.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDatabaseSqlite class declaration.
*
* @details Describes an SQLITE database.
*
***************************************************************************/

#include "DatabaseSqlitePch.h"

#include "DatabaseSqliteHelper.h"
#include "DatabaseConnectionSqlite.h"

#include <DatabaseValuedObjectInfos.h>

BEGIN_NAMESPACE_DATABASE_SQLITE
{
	namespace
	{
		static const String ERROR_SQLITE_UNSUPPORTED_TYPE = STR( "Unsupported type, for column " );

		static const TChar * INFO_SQLITE_PREPARATION = STR( "Statement preparation" );

		std::pair< int, int > RetrieveLimits( String const & type )
		{
			std::pair< int, int > result( -1, -1 );
			size_t index = type.find( STR( "(" ) );

			if ( index != String::npos )
			{
				size_t dotIndex = type.find( STR( "," ), index );

				if ( dotIndex == String::npos )
				{
					String limit = type.substr( index + 1, type.find( STR( ")" ) ) - index );
					result.first = std::stoi( StringUtils::Trim( limit ) );
				}
				else
				{
					String limit1 = type.substr( index + 1, dotIndex - index );
					result.first = std::stoi( StringUtils::Trim( limit1 ) );
					String limit2 = type.substr( dotIndex + 1, type.find( STR( ")" ) ) - dotIndex );
					result.second = std::stoi( StringUtils::Trim( limit2 ) );
				}
			}

			return result;
		}

		template< EFieldType Type, typename Value >
		DatabaseFieldSPtr ConstructField( sqlite3_stmt * statement, int i, DatabaseConnectionSPtr connection, DatabaseValuedObjectInfosSPtr infos, Value value )
		{
			int size = sqlite3_column_bytes( statement, i );
			DatabaseFieldSPtr field = std::make_shared< CDatabaseField >( connection, infos );

			if ( size > 0 )
			{
				static_cast< CDatabaseValue< Type > & >( field->GetObjectValue() ).SetValue( value );
			}

			return field;
		}

		template< EFieldType Type > DatabaseFieldSPtr GetValue( sqlite3_stmt * statement, int i, DatabaseConnectionSPtr connection, DatabaseValuedObjectInfosSPtr infos );

		template<>
		DatabaseFieldSPtr GetValue< EFieldType_BIT >( sqlite3_stmt * statement, int i, DatabaseConnectionSPtr connection, DatabaseValuedObjectInfosSPtr infos )
		{
			bool value = sqlite3_column_int( statement, i ) != 0;
			return ConstructField< EFieldType_BIT >( statement, i, connection, infos, value );
		}

		template<>
		DatabaseFieldSPtr GetValue< EFieldType_SINT8 >( sqlite3_stmt * statement, int i, DatabaseConnectionSPtr connection, DatabaseValuedObjectInfosSPtr infos )
		{
			int32_t value = sqlite3_column_int( statement, i );
			return ConstructField< EFieldType_SINT8 >( statement, i, connection, infos, value );
		}

		template<>
		DatabaseFieldSPtr GetValue< EFieldType_SINT16 >( sqlite3_stmt * statement, int i, DatabaseConnectionSPtr connection, DatabaseValuedObjectInfosSPtr infos )
		{
			int32_t value = sqlite3_column_int( statement, i );
			return ConstructField< EFieldType_SINT16 >( statement, i, connection, infos, value );
		}

		template<>
		DatabaseFieldSPtr GetValue< EFieldType_SINT24 >( sqlite3_stmt * statement, int i, DatabaseConnectionSPtr connection, DatabaseValuedObjectInfosSPtr infos )
		{
			int32_t value = sqlite3_column_int( statement, i );
			return ConstructField< EFieldType_SINT24 >( statement, i, connection, infos, int24_t( value ) );
		}

		template<>
		DatabaseFieldSPtr GetValue< EFieldType_SINT32 >( sqlite3_stmt * statement, int i, DatabaseConnectionSPtr connection, DatabaseValuedObjectInfosSPtr infos )
		{
			int32_t value = sqlite3_column_int( statement, i );
			return ConstructField< EFieldType_SINT32 >( statement, i, connection, infos, value );
		}

		template<>
		DatabaseFieldSPtr GetValue< EFieldType_SINT64 >( sqlite3_stmt * statement, int i, DatabaseConnectionSPtr connection, DatabaseValuedObjectInfosSPtr infos )
		{
			int64_t value = sqlite3_column_int64( statement, i );
			return ConstructField< EFieldType_SINT64 >( statement, i, connection, infos, value );
		}

		template<>
		DatabaseFieldSPtr GetValue< EFieldType_UINT8 >( sqlite3_stmt * statement, int i, DatabaseConnectionSPtr connection, DatabaseValuedObjectInfosSPtr infos )
		{
			uint32_t value = sqlite3_column_int( statement, i );
			return ConstructField< EFieldType_UINT8 >( statement, i, connection, infos, value );
		}

		template<>
		DatabaseFieldSPtr GetValue< EFieldType_UINT16 >( sqlite3_stmt * statement, int i, DatabaseConnectionSPtr connection, DatabaseValuedObjectInfosSPtr infos )
		{
			uint32_t value = sqlite3_column_int( statement, i );
			return ConstructField< EFieldType_UINT16 >( statement, i, connection, infos, value );
		}

		template<>
		DatabaseFieldSPtr GetValue< EFieldType_UINT24 >( sqlite3_stmt * statement, int i, DatabaseConnectionSPtr connection, DatabaseValuedObjectInfosSPtr infos )
		{
			uint32_t value = sqlite3_column_int( statement, i );
			return ConstructField< EFieldType_UINT24 >( statement, i, connection, infos, uint24_t( value ) );
		}

		template<>
		DatabaseFieldSPtr GetValue< EFieldType_UINT32 >( sqlite3_stmt * statement, int i, DatabaseConnectionSPtr connection, DatabaseValuedObjectInfosSPtr infos )
		{
			uint32_t value = sqlite3_column_int( statement, i );
			return ConstructField< EFieldType_UINT32 >( statement, i, connection, infos, value );
		}

		template<>
		DatabaseFieldSPtr GetValue< EFieldType_UINT64 >( sqlite3_stmt * statement, int i, DatabaseConnectionSPtr connection, DatabaseValuedObjectInfosSPtr infos )
		{
			uint64_t value = sqlite3_column_int64( statement, i );
			return ConstructField< EFieldType_UINT64 >( statement, i, connection, infos, value );
		}

		template<>
		DatabaseFieldSPtr GetValue< EFieldType_FLOAT32 >( sqlite3_stmt * statement, int i, DatabaseConnectionSPtr connection, DatabaseValuedObjectInfosSPtr infos )
		{
			float value = float( sqlite3_column_double( statement, i ) );
			return ConstructField< EFieldType_FLOAT32 >( statement, i, connection, infos, value );
		}

		template<>
		DatabaseFieldSPtr GetValue< EFieldType_FLOAT64 >( sqlite3_stmt * statement, int i, DatabaseConnectionSPtr connection, DatabaseValuedObjectInfosSPtr infos )
		{
			double value = sqlite3_column_double( statement, i );
			return ConstructField< EFieldType_FLOAT64 >( statement, i, connection, infos, value );
		}

		template<>
		DatabaseFieldSPtr GetValue< EFieldType_FIXED_POINT >( sqlite3_stmt * statement, int i, DatabaseConnectionSPtr connection, DatabaseValuedObjectInfosSPtr infos )
		{
			const char * value = reinterpret_cast< const char * >( sqlite3_column_text( statement, i ) );
			int size = sqlite3_column_bytes( statement, i );
			DatabaseFieldSPtr field = std::make_shared< CDatabaseField >( connection, infos );

			if ( value && size > 0 )
			{
				static_cast< CDatabaseValue< EFieldType_FIXED_POINT > & >( field->GetObjectValue() ).SetValue( CFixedPoint( String( value, value + size ), infos->GetPrecision().first, infos->GetPrecision().second ) );
			}

			return field;
		}

		template<>
		DatabaseFieldSPtr GetValue< EFieldType_CHAR >( sqlite3_stmt * statement, int i, DatabaseConnectionSPtr connection, DatabaseValuedObjectInfosSPtr infos )
		{
			const char * value = reinterpret_cast< const char * >( sqlite3_column_text( statement, i ) );
			int size = sqlite3_column_bytes( statement, i );
			DatabaseFieldSPtr field = std::make_shared< CDatabaseField >( connection, infos );

			if ( value && size > 0 )
			{
				static_cast< CDatabaseValue< EFieldType_CHAR > & >( field->GetObjectValue() ).SetValue( std::string( value, value + size ) );
			}

			return field;
		}

		template<>
		DatabaseFieldSPtr GetValue< EFieldType_VARCHAR >( sqlite3_stmt * statement, int i, DatabaseConnectionSPtr connection, DatabaseValuedObjectInfosSPtr infos )
		{
			const char * value = reinterpret_cast< const char * >( sqlite3_column_text( statement, i ) );
			int size = sqlite3_column_bytes( statement, i );
			DatabaseFieldSPtr field = std::make_shared< CDatabaseField >( connection, infos );

			if ( value && size > 0 )
			{
				static_cast< CDatabaseValue< EFieldType_VARCHAR > & >( field->GetObjectValue() ).SetValue( std::string( value, value + size ) );
			}

			return field;
		}

		template<>
		DatabaseFieldSPtr GetValue< EFieldType_TEXT >( sqlite3_stmt * statement, int i, DatabaseConnectionSPtr connection, DatabaseValuedObjectInfosSPtr infos )
		{
			const char * value = reinterpret_cast< const char * >( sqlite3_column_text( statement, i ) );
			int size = sqlite3_column_bytes( statement, i );
			DatabaseFieldSPtr field = std::make_shared< CDatabaseField >( connection, infos );

			if ( value && size > 0 )
			{
				static_cast< CDatabaseValue< EFieldType_TEXT > & >( field->GetObjectValue() ).SetValue( std::string( value, value + size ) );
			}

			return field;
		}

		template<>
		DatabaseFieldSPtr GetValue< EFieldType_NCHAR >( sqlite3_stmt * statement, int i, DatabaseConnectionSPtr connection, DatabaseValuedObjectInfosSPtr infos )
		{
			const char * value = reinterpret_cast< const char * >( sqlite3_column_text( statement, i ) );
			int size = sqlite3_column_bytes( statement, i );
			DatabaseFieldSPtr field = std::make_shared< CDatabaseField >( connection, infos );

			if ( value && size > 0 )
			{
				static_cast< CDatabaseValue< EFieldType_NCHAR > & >( field->GetObjectValue() ).SetValue( StringUtils::ToWStr( std::string( value, value + size ) ) );
			}

			return field;
		}

		template<>
		DatabaseFieldSPtr GetValue< EFieldType_NVARCHAR >( sqlite3_stmt * statement, int i, DatabaseConnectionSPtr connection, DatabaseValuedObjectInfosSPtr infos )
		{
			const char * value = reinterpret_cast< const char * >( sqlite3_column_text( statement, i ) );
			int size = sqlite3_column_bytes( statement, i );
			DatabaseFieldSPtr field = std::make_shared< CDatabaseField >( connection, infos );

			if ( value && size > 0 )
			{
				static_cast< CDatabaseValue< EFieldType_NVARCHAR > & >( field->GetObjectValue() ).SetValue( StringUtils::ToWStr( std::string( value, value + size ) ) );
			}

			return field;
		}

		template<>
		DatabaseFieldSPtr GetValue< EFieldType_NTEXT >( sqlite3_stmt * statement, int i, DatabaseConnectionSPtr connection, DatabaseValuedObjectInfosSPtr infos )
		{
			const char * value = reinterpret_cast< const char * >( sqlite3_column_text( statement, i ) );
			int size = sqlite3_column_bytes( statement, i );
			DatabaseFieldSPtr field = std::make_shared< CDatabaseField >( connection, infos );

			if ( value && size > 0 )
			{
				static_cast< CDatabaseValue< EFieldType_NTEXT > & >( field->GetObjectValue() ).SetValue( StringUtils::ToWStr( std::string( value, value + size ) ) );
			}

			return field;
		}

		template<>
		DatabaseFieldSPtr GetValue< EFieldType_DATE >( sqlite3_stmt * statement, int i, DatabaseConnectionSPtr connection, DatabaseValuedObjectInfosSPtr infos )
		{
			const char * value = reinterpret_cast< const char * >( sqlite3_column_text( statement, i ) );
			int size = sqlite3_column_bytes( statement, i );
			DatabaseFieldSPtr field = std::make_shared< CDatabaseField >( connection, infos );

			if ( value && size > 0 )
			{
				static_cast< CDatabaseValue< EFieldType_DATE > & >( field->GetObjectValue() ).SetValue( connection->ParseDate( std::string( value, value + size ) ) );
			}

			return field;
		}

		template<>
		DatabaseFieldSPtr GetValue< EFieldType_DATETIME >( sqlite3_stmt * statement, int i, DatabaseConnectionSPtr connection, DatabaseValuedObjectInfosSPtr infos )
		{
			const char * value = reinterpret_cast< const char * >( sqlite3_column_text( statement, i ) );
			int size = sqlite3_column_bytes( statement, i );
			DatabaseFieldSPtr field = std::make_shared< CDatabaseField >( connection, infos );

			if ( value && size > 0 )
			{
				static_cast< CDatabaseValue< EFieldType_DATETIME > & >( field->GetObjectValue() ).SetValue( connection->ParseDateTime( std::string( value, value + size ) ) );
			}

			return field;
		}

		template<>
		DatabaseFieldSPtr GetValue< EFieldType_TIME >( sqlite3_stmt * statement, int i, DatabaseConnectionSPtr connection, DatabaseValuedObjectInfosSPtr infos )
		{
			const char * value = reinterpret_cast< const char * >( sqlite3_column_text( statement, i ) );
			int size = sqlite3_column_bytes( statement, i );
			DatabaseFieldSPtr field = std::make_shared< CDatabaseField >( connection, infos );

			if ( value && size > 0 )
			{
				static_cast< CDatabaseValue< EFieldType_TIME > & >( field->GetObjectValue() ).SetValue( connection->ParseTime( std::string( value, value + size ) ) );
			}

			return field;
		}

		template<>
		DatabaseFieldSPtr GetValue< EFieldType_BINARY >( sqlite3_stmt * statement, int i, DatabaseConnectionSPtr connection, DatabaseValuedObjectInfosSPtr infos )
		{
			const uint8_t * value = reinterpret_cast< const uint8_t * >( sqlite3_column_blob( statement, i ) );
			int size = sqlite3_column_bytes( statement, i );
			DatabaseFieldSPtr field = std::make_shared< CDatabaseField >( connection, infos );

			if ( value && size > 0 )
			{
				static_cast< CDatabaseValue< EFieldType_BINARY > & >( field->GetObjectValue() ).SetValue( value, std::min( uint32_t( size ), infos->GetLimits() ) );
			}

			return field;
		}

		template<>
		DatabaseFieldSPtr GetValue< EFieldType_VARBINARY >( sqlite3_stmt * statement, int i, DatabaseConnectionSPtr connection, DatabaseValuedObjectInfosSPtr infos )
		{
			const uint8_t * value = reinterpret_cast< const uint8_t * >( sqlite3_column_blob( statement, i ) );
			int size = sqlite3_column_bytes( statement, i );
			DatabaseFieldSPtr field = std::make_shared< CDatabaseField >( connection, infos );

			if ( value && size > 0 )
			{
				static_cast< CDatabaseValue< EFieldType_VARBINARY > & >( field->GetObjectValue() ).SetValue( value, std::min( uint32_t( size ), infos->GetLimits() ) );
			}

			return field;
		}

		template<>
		DatabaseFieldSPtr GetValue< EFieldType_BLOB >( sqlite3_stmt * statement, int i, DatabaseConnectionSPtr connection, DatabaseValuedObjectInfosSPtr infos )
		{
			const uint8_t * value = reinterpret_cast< const uint8_t * >( sqlite3_column_blob( statement, i ) );
			int size = sqlite3_column_bytes( statement, i );
			DatabaseFieldSPtr field = std::make_shared< CDatabaseField >( connection, infos );

			if ( value && size > 0 )
			{
				static_cast< CDatabaseValue< EFieldType_BLOB > & >( field->GetObjectValue() ).SetValue( value, std::min( uint32_t( size ), infos->GetLimits() ) );
			}

			return field;
		}

		template<>
		DatabaseFieldSPtr GetValue< EFieldType_NULL >( sqlite3_stmt * statement, int i, DatabaseConnectionSPtr connection, DatabaseValuedObjectInfosSPtr infos )
		{
			DatabaseFieldSPtr field;
			sqlite3_value * value = sqlite3_column_value( statement, i );
			int type = sqlite3_value_type( value );

			switch ( type )
			{
			case SQLITE_INTEGER:
				infos->SetType( EFieldType_UINT64 );
				field = GetValue< EFieldType_UINT64 >( statement, i, connection, infos );
				break;

			case SQLITE_FLOAT:
				infos->SetType( EFieldType_FLOAT64 );
				field = GetValue< EFieldType_FLOAT64 >( statement, i, connection, infos );
				break;

			case SQLITE_TEXT:
				infos->SetType( EFieldType_TEXT );
				field = GetValue< EFieldType_TEXT >( statement, i, connection, infos );
				break;

			case SQLITE_BLOB:
				infos->SetType( EFieldType_VARBINARY );
				field = GetValue< EFieldType_VARBINARY >( statement, i, connection, infos );
				break;

			case SQLITE_NULL:
			{
				const char * value = reinterpret_cast< const char * >( sqlite3_column_text( statement, i ) );
				int size = sqlite3_column_bytes( statement, i );

				if ( value && size > 0 )
				{
					infos->SetType( EFieldType_TEXT );
					DatabaseFieldSPtr field = std::make_shared< CDatabaseField >( connection, infos );
					static_cast< CDatabaseValue< EFieldType_TEXT > & >( field->GetObjectValue() ).SetValue( std::string( value, value + size ) );
				}
				else
				{
					infos->SetType( EFieldType_SINT32 );
					uint32_t value = sqlite3_column_int( statement, i );
					return ConstructField< EFieldType_SINT32 >( statement, i, connection, infos, value );
				}
			}
			break;
			}

			return field;
		}

		DatabaseValuedObjectInfosSPtr GetIntegerFieldInfos( const std::string & type, const String & columnName, const std::string & lowerName )
		{
			DatabaseValuedObjectInfosSPtr infos;

			if ( type.find( "BIGINT" ) != std::string::npos
					|| lowerName.find( "max(" ) != std::string::npos
					|| lowerName.find( "min(" ) != std::string::npos
					|| lowerName.find( "count(" ) != std::string::npos
					|| lowerName.find( "sum(" ) != std::string::npos )
			{
				infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_SINT64 );
			}
			else if ( type.find( "SMALLINT" ) != std::string::npos )
			{
				infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_SINT16 );
			}
			else if ( type.find( "TINYINT" ) != std::string::npos )
			{
				infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_SINT8 );
			}
			else if ( type.find( "bool" ) != std::string::npos )
			{
				infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_BIT );
			}
			else
			{
				size_t index = type.find( "INT" );

				if ( index != std::string::npos && type.size() > 3 )
				{
					int prec = 0;
					std::stringstream stream( type.substr( index + 3 ) );
					stream >> prec;

					if ( prec )
					{
						if ( prec == 1 )
						{
							infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_SINT8 );
						}
						else if ( prec == 2 )
						{
							infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_SINT16 );
						}
						else if ( prec <= 3 )
						{
							infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_SINT24 );
						}
						else if ( prec <= 4 )
						{
							infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_SINT32 );
						}
						else
						{
							infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_SINT64 );
						}
					}
					else
					{
						infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_SINT32 );
					}
				}
				else
				{
					infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_SINT32 );
				}
			}

			return infos;
		}

		DatabaseValuedObjectInfosSPtr GetFloatFieldInfos( const std::string & type, const String & columnName, const std::string & lowerName )
		{
			DatabaseValuedObjectInfosSPtr infos;

			if ( type.find( "DOUB" ) != std::string::npos
					|| type.find( "REAL" ) != std::string::npos )
			{
				infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_FLOAT64 );
			}
			else if ( type.find( "DECIMAL" ) != std::string::npos )
			{
				infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_FIXED_POINT );
			}
			else
			{
				infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_FLOAT32 );
			}

			return infos;
		}

		DatabaseValuedObjectInfosSPtr GetStringFieldInfos( const std::string & type, const String & columnName, const std::string & lowerName )
		{
			DatabaseValuedObjectInfosSPtr infos;

			if ( type.find( "NCHAR" ) != std::string::npos )
			{
				infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_NCHAR, RetrieveLimits( type ) );
			}
			else if ( type.find( "NVARCHAR" ) != std::string::npos )
			{
				infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_NVARCHAR, RetrieveLimits( type ) );
			}
			else if ( type.find( "VARCHAR" ) != std::string::npos )
			{
				infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_VARCHAR, RetrieveLimits( type ) );
			}
			else if ( type.find( "CHAR" ) != std::string::npos )
			{
				infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_CHAR, RetrieveLimits( type ) );
			}
			else if ( type.find( "NTEXT" ) != std::string::npos )
			{
				infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_NTEXT );
			}
			else
			{
				infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_TEXT );
			}

			return infos;
		}

		DatabaseValuedObjectInfosSPtr GetBlobFieldInfos( const std::string & type, const String & columnName, const std::string & lowerName )
		{
			DatabaseValuedObjectInfosSPtr infos;

			if ( type.find( "VARBINARY" ) != std::string::npos )
			{
				infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_VARBINARY, RetrieveLimits( type ) );
			}
			else if ( type.find( "BINARY" ) != std::string::npos )
			{
				infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_BINARY, RetrieveLimits( type ) );
			}
			else
			{
				infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_BLOB );
			}

			return infos;
		}

		DatabaseValuedObjectInfosSPtr GetNullFieldInfos( const std::string & type, const String & columnName, const std::string & lowerName )
		{
			DatabaseValuedObjectInfosSPtr infos;
			std::string upperType = StringUtils::UpperCase( type );
			size_t index;

			if ( upperType == "DATETIME" )
			{
				infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_DATETIME );
			}
			else if ( upperType == "DATE" )
			{
				infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_DATE );
			}
			else if ( upperType == "TIME" )
			{
				infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_TIME );
			}
			else if ( upperType.find( "BIGINT" ) != std::string::npos
					  || lowerName.find( "max(" ) != std::string::npos
					  || lowerName.find( "min(" ) != std::string::npos
					  || lowerName.find( "count(" ) != std::string::npos )
			{
				infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_SINT64 );
			}
			else if ( upperType.find( "FLOA" ) != String::npos
					  || lowerName.find( "sum(" ) != std::string::npos )
			{
				infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_FLOAT32 );
			}
			else if ( upperType.find( "NUMERIC" ) != std::string::npos )
			{
				infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_FIXED_POINT, std::make_pair( 10, 0 ) );
			}
			else if ( upperType.find( "DECIMAL" ) != String::npos )
			{
				infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_FIXED_POINT, RetrieveLimits( upperType ) );
			}
			else if ( upperType.find( "SMALLINT" ) != String::npos )
			{
				infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_SINT16 );
			}
			else if ( upperType.find( "BOOL" ) != String::npos
					  || upperType.find( "BIT" ) != String::npos )
			{
				infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_BIT );
			}
			else if ( upperType.find( "TINYINT" ) != String::npos )
			{
				infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_SINT8 );
			}
			else if ( ( index = upperType.find( "MEDIUMINT" ) ) != String::npos )
			{
				infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_SINT24 );
			}
			else if ( ( index = upperType.find( "INT" ) ) != String::npos )
			{
				if ( index != std::string::npos && upperType.size() > 3 )
				{
					int prec = 0;
					std::stringstream stream( upperType.substr( index + 3 ) );
					stream >> prec;

					if ( prec )
					{
						if ( prec == 1 )
						{
							infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_SINT8 );
						}
						else if ( prec == 2 )
						{
							infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_SINT16 );
						}
						else if ( prec <= 3 )
						{
							infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_SINT24 );
						}
						else if ( prec <= 4 )
						{
							infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_SINT32 );
						}
						else
						{
							infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_SINT64 );
						}
					}
					else
					{
						infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_SINT32 );
					}
				}
				else
				{
					infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_SINT32 );
				}
			}
			else if ( upperType.find( "NCHAR" ) != String::npos )
			{
				infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_NCHAR, RetrieveLimits( upperType ) );
			}
			else if ( upperType.find( "NVARCHAR" ) != String::npos )
			{
				infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_NVARCHAR, RetrieveLimits( upperType ) );
			}
			else if ( upperType.find( "VARCHAR" ) != String::npos )
			{
				infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_VARCHAR, RetrieveLimits( upperType ) );
			}
			else if ( upperType.find( "CHAR" ) != String::npos )
			{
				infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_CHAR, RetrieveLimits( upperType ) );
			}
			else if ( upperType.find( "NTEXT" ) != String::npos )
			{
				infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_NTEXT, -1 );
			}
			else if ( upperType.find( "CLOB" ) != String::npos
					  || upperType.find( "TEXT" ) != String::npos )
			{
				infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_TEXT, -1 );
			}
			else if ( upperType.find( "REAL" ) != String::npos
					  || upperType.find( "DOUB" ) != String::npos )
			{
				infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_FLOAT64 );
			}
			else if ( upperType.find( "BLOB" ) != String::npos )
			{
				infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_BLOB, -1 );
			}
			else if ( upperType.find( "VARBINARY" ) != String::npos )
			{
				infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_VARBINARY, RetrieveLimits( upperType ) );
			}
			else if ( upperType.find( "BINARY" ) != String::npos )
			{
				infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_BINARY, RetrieveLimits( upperType ) );
			}
			else
			{
				CLogger::LogWarning( StringStream() << ERROR_SQLITE_UNSUPPORTED_TYPE << columnName << STR( "(" ) << type << STR( ")" ) );
				infos = std::make_shared< CDatabaseValuedObjectInfos >( columnName, EFieldType_NULL, -1 );
			}

			return infos;
		}
	}

	DatabaseValuedObjectInfosPtrArray SqliteGetColumns( sqlite3_stmt * statement )
	{
		DatabaseValuedObjectInfosPtrArray arrayReturn;
		int iColumnCount = sqlite3_column_count( statement );

		for ( int i = 0; i < iColumnCount; i++ )
		{
			char const * pszName = sqlite3_column_name( statement, i );

			if ( pszName )
			{
				char const * pszType = sqlite3_column_decltype( statement, i );
				std::string type( pszType ? pszType : "" );

				switch ( sqlite3_column_type( statement, i ) )
				{
				case SQLITE_INTEGER:
					arrayReturn.push_back( GetIntegerFieldInfos( type, pszName, StringUtils::LowerCase( pszName ) ) );
					break;

				case SQLITE_FLOAT:
					arrayReturn.push_back( GetFloatFieldInfos( type, pszName, StringUtils::LowerCase( pszName ) ) );
					break;

				case SQLITE_TEXT:
					arrayReturn.push_back( GetStringFieldInfos( type, pszName, StringUtils::LowerCase( pszName ) ) );
					break;

				case SQLITE_BLOB:
					arrayReturn.push_back( GetBlobFieldInfos( type, pszName, StringUtils::LowerCase( pszName ) ) );
					break;

				case SQLITE_NULL:
					arrayReturn.push_back( GetNullFieldInfos( type, pszName, StringUtils::LowerCase( pszName ) ) );
					break;

				default:
					DB_EXCEPT( EDatabaseExceptionCodes_StatementError, ERROR_SQLITE_UNSUPPORTED_TYPE );
					break;
				}
			}
		}

		return arrayReturn;
	}

	DatabaseFieldSPtr SetFieldValue( sqlite3_stmt * statement, int index, DatabaseConnectionSqliteSPtr connection, DatabaseValuedObjectInfosSPtr infos )
	{
		DatabaseFieldSPtr field;

		switch ( infos->GetType() )
		{
		case EFieldType_NULL:
			field = GetValue< EFieldType_NULL >( statement, index, connection, infos );
			break;

		case EFieldType_BIT:
			field = GetValue< EFieldType_BIT >( statement, index, connection, infos );
			break;

		case EFieldType_SINT8:
			field = GetValue< EFieldType_SINT8 >( statement, index, connection, infos );
			break;

		case EFieldType_SINT16:
			field = GetValue< EFieldType_SINT16 >( statement, index, connection, infos );
			break;

		case EFieldType_SINT24:
			field = GetValue< EFieldType_SINT24 >( statement, index, connection, infos );
			break;

		case EFieldType_SINT32:
			field = GetValue< EFieldType_SINT32 >( statement, index, connection, infos );
			break;

		case EFieldType_SINT64:
			field = GetValue< EFieldType_SINT64 >( statement, index, connection, infos );
			break;

		case EFieldType_UINT8:
			field = GetValue< EFieldType_UINT8 >( statement, index, connection, infos );
			break;

		case EFieldType_UINT16:
			field = GetValue< EFieldType_UINT16 >( statement, index, connection, infos );
			break;

		case EFieldType_UINT24:
			field = GetValue< EFieldType_UINT24 >( statement, index, connection, infos );
			break;

		case EFieldType_UINT32:
			field = GetValue< EFieldType_UINT32 >( statement, index, connection, infos );
			break;

		case EFieldType_UINT64:
			field = GetValue< EFieldType_UINT64 >( statement, index, connection, infos );
			break;

		case EFieldType_FLOAT32:
			field = GetValue< EFieldType_FLOAT32 >( statement, index, connection, infos );
			break;

		case EFieldType_FLOAT64:
			field = GetValue< EFieldType_FLOAT64 >( statement, index, connection, infos );
			break;

		case EFieldType_FIXED_POINT:
			field = GetValue< EFieldType_FIXED_POINT >( statement, index, connection, infos );
			break;

		case EFieldType_CHAR:
			field = GetValue< EFieldType_CHAR >( statement, index, connection, infos );
			break;

		case EFieldType_VARCHAR:
			field = GetValue< EFieldType_VARCHAR >( statement, index, connection, infos );
			break;

		case EFieldType_TEXT:
			field = GetValue< EFieldType_TEXT >( statement, index, connection, infos );
			break;

		case EFieldType_NCHAR:
			field = GetValue< EFieldType_NCHAR >( statement, index, connection, infos );
			break;

		case EFieldType_NVARCHAR:
			field = GetValue< EFieldType_NVARCHAR >( statement, index, connection, infos );
			break;

		case EFieldType_NTEXT:
			field = GetValue< EFieldType_NTEXT >( statement, index, connection, infos );
			break;

		case EFieldType_DATE:
			field = GetValue< EFieldType_DATE >( statement, index, connection, infos );
			break;

		case EFieldType_DATETIME:
			field = GetValue< EFieldType_DATETIME >( statement, index, connection, infos );
			break;

		case EFieldType_TIME:
			field = GetValue< EFieldType_TIME >( statement, index, connection, infos );
			break;

		case EFieldType_BINARY:
			field = GetValue< EFieldType_BINARY >( statement, index, connection, infos );
			break;

		case EFieldType_VARBINARY:
			field = GetValue< EFieldType_VARBINARY >( statement, index, connection, infos );
			break;

		case EFieldType_BLOB:
			field = GetValue< EFieldType_BLOB >( statement, index, connection, infos );
			break;

		default:
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, ERROR_SQLITE_UNSUPPORTED_TYPE );
			break;
		}

		return field;
	}

	DatabaseResultSPtr SqliteFetchResult( sqlite3_stmt * statement, DatabaseValuedObjectInfosPtrArray const & columns, DatabaseConnectionSqliteSPtr connection )
	{
		DatabaseResultSPtr pReturn;
		int code = SQLITE_ERROR;

		if ( connection->IsConnected() )
		{
			pReturn = std::make_unique< CDatabaseResult >( columns );
			code = SQLITE_OK;
			int iResult;
			int iNbColumns = int( columns.size() );

			while ( ( iResult = sqlite3_step( statement ) ) != SQLITE_DONE )
			{
				if ( iResult == SQLITE_ROW )
				{
					DatabaseRowSPtr pRow = std::make_shared< CDatabaseRow >();
					int i = 0;

					for ( auto infos : columns )
					{
						pRow->AddField( SetFieldValue( statement, i++, connection, infos ) );
					}

					pReturn->AddRow( pRow );
				}
			}
		}

		return pReturn;
	}

	sqlite3_stmt * SqlitePrepareStatement( const String & query, sqlite3 * connection )
	{
		sqlite3_stmt * statement = NULL;
		int code = sqlite3_prepare_v2( connection, query.c_str(), int( query.size() ), &statement, NULL );

		while ( code == SQLITE_BUSY || code == SQLITE_LOCKED )
		{
			// Retry as long as the database is locked or busy
			std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
			code = sqlite3_prepare_v2( connection, query.c_str(), int( query.size() ), &statement, NULL );
		}

		SQLiteCheck( code, INFO_SQLITE_PREPARATION, EDatabaseExceptionCodes_ConnectionError, connection );
		return statement;
	}

	void SQLiteCheck( int code, TChar const * msg, EDatabaseExceptionCodes exc, sqlite3 * database )
	{
		if ( code != SQLITE_OK )
		{
			StringStream error;
			error << STR( "Error : " ) << msg << STR( " - " ) << sqlite3_errmsg( database );
			DB_EXCEPT( exc, error.str() );
		}

#if !defined( _NDEBUG )

		else
		{
			CLogger::LogDebug( StringStream() << STR( "Success : " ) << msg );
		}

#endif
	}

	void SQLiteCheck( int code, std::ostream const & stream, EDatabaseExceptionCodes exc, sqlite3 * database )
	{
		std::stringstream ss;
		ss << stream.rdbuf();
		SQLiteCheck( code, ss.str().c_str(), exc, database );
	}
}
END_NAMESPACE_DATABASE_SQLITE
