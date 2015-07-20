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

#include <DatabaseFieldInfos.h>

BEGIN_NAMESPACE_DATABASE_SQLITE
{
	namespace
	{
		static const String ERROR_SQLITE_UNSUPPORTED_TYPE = STR( "Unsupported type" );

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
					result.first = CStrUtils::ToInt( CStrUtils::Trim( limit ) );
				}
				else
				{
					String limit1 = type.substr( index + 1, dotIndex - index );
					result.first = CStrUtils::ToInt( CStrUtils::Trim( limit1 ) );
					String limit2 = type.substr( dotIndex + 1, type.find( STR( ")" ) ) - dotIndex );
					result.second = CStrUtils::ToInt( CStrUtils::Trim( limit2 ) );
				}
			}

			return result;
		}

		template< EFieldType Type, typename Value >
		DatabaseFieldPtr ConstructField( sqlite3_stmt * statement, int i, DatabaseFieldInfosPtr infos, Value value )
		{
			int iSize = sqlite3_column_bytes( statement, i );
			DatabaseFieldPtr field;
			field = std::make_shared< CDatabaseField >( infos );

			if ( iSize != 0 )
			{
				static_cast< CDatabaseValue< Type > & >( field->GetObjectValue() ).SetValue( value );
			}

			return field;
		}

		template< EFieldType Type > DatabaseFieldPtr GetValue( sqlite3_stmt * statement, int i, DatabaseFieldInfosPtr infos );

		template<>
		DatabaseFieldPtr GetValue< EFieldType_BIT >( sqlite3_stmt * statement, int i, DatabaseFieldInfosPtr infos )
		{
			bool value = sqlite3_column_int( statement, i ) != 0;
			return ConstructField< EFieldType_BIT >( statement, i, infos, value );
		}

		template<>
		DatabaseFieldPtr GetValue< EFieldType_SINT8 >( sqlite3_stmt * statement, int i, DatabaseFieldInfosPtr infos )
		{
			int32_t value = sqlite3_column_int( statement, i );
			return ConstructField< EFieldType_SINT8 >( statement, i, infos, value );
		}

		template<>
		DatabaseFieldPtr GetValue< EFieldType_SINT16 >( sqlite3_stmt * statement, int i, DatabaseFieldInfosPtr infos )
		{
			int32_t value = sqlite3_column_int( statement, i );
			return ConstructField< EFieldType_SINT16 >( statement, i, infos, value );
		}

		template<>
		DatabaseFieldPtr GetValue< EFieldType_SINT24 >( sqlite3_stmt * statement, int i, DatabaseFieldInfosPtr infos )
		{
			int32_t value = sqlite3_column_int( statement, i );
			return ConstructField< EFieldType_SINT24 >( statement, i, infos, int24_t( value ) );
		}

		template<>
		DatabaseFieldPtr GetValue< EFieldType_SINT32 >( sqlite3_stmt * statement, int i, DatabaseFieldInfosPtr infos )
		{
			int32_t value = sqlite3_column_int( statement, i );
			return ConstructField< EFieldType_SINT32 >( statement, i, infos, value );
		}

		template<>
		DatabaseFieldPtr GetValue< EFieldType_SINT64 >( sqlite3_stmt * statement, int i, DatabaseFieldInfosPtr infos )
		{
			int64_t value = sqlite3_column_int64( statement, i );
			return ConstructField< EFieldType_SINT64 >( statement, i, infos, value );
		}

		template<>
		DatabaseFieldPtr GetValue< EFieldType_UINT8 >( sqlite3_stmt * statement, int i, DatabaseFieldInfosPtr infos )
		{
			uint32_t value = sqlite3_column_int( statement, i );
			return ConstructField< EFieldType_UINT8 >( statement, i, infos, value );
		}

		template<>
		DatabaseFieldPtr GetValue< EFieldType_UINT16 >( sqlite3_stmt * statement, int i, DatabaseFieldInfosPtr infos )
		{
			uint32_t value = sqlite3_column_int( statement, i );
			return ConstructField< EFieldType_UINT16 >( statement, i, infos, value );
		}

		template<>
		DatabaseFieldPtr GetValue< EFieldType_UINT24 >( sqlite3_stmt * statement, int i, DatabaseFieldInfosPtr infos )
		{
			uint32_t value = sqlite3_column_int( statement, i );
			return ConstructField< EFieldType_UINT24 >( statement, i, infos, uint24_t( value ) );
		}

		template<>
		DatabaseFieldPtr GetValue< EFieldType_UINT32 >( sqlite3_stmt * statement, int i, DatabaseFieldInfosPtr infos )
		{
			uint32_t value = sqlite3_column_int( statement, i );
			return ConstructField< EFieldType_UINT32 >( statement, i, infos, value );
		}

		template<>
		DatabaseFieldPtr GetValue< EFieldType_UINT64 >( sqlite3_stmt * statement, int i, DatabaseFieldInfosPtr infos )
		{
			uint64_t value = sqlite3_column_int64( statement, i );
			return ConstructField< EFieldType_UINT64 >( statement, i, infos, value );
		}

		template<>
		DatabaseFieldPtr GetValue< EFieldType_FLOAT32 >( sqlite3_stmt * statement, int i, DatabaseFieldInfosPtr infos )
		{
			float value = float( sqlite3_column_double( statement, i ) );
			return ConstructField< EFieldType_FLOAT32 >( statement, i, infos, value );
		}

		template<>
		DatabaseFieldPtr GetValue< EFieldType_FLOAT64 >( sqlite3_stmt * statement, int i, DatabaseFieldInfosPtr infos )
		{
			double value = sqlite3_column_double( statement, i );
			return ConstructField< EFieldType_FLOAT64 >( statement, i, infos, value );
		}

		template<>
		DatabaseFieldPtr GetValue< EFieldType_FIXED_POINT >( sqlite3_stmt * statement, int i, DatabaseFieldInfosPtr infos )
		{
			DatabaseFieldPtr field;
			const char * value = reinterpret_cast< const char * >( sqlite3_column_text( statement, i ) );
			int iSize = sqlite3_column_bytes( statement, i );
			field = std::make_shared< CDatabaseField >( infos );

			if ( iSize )
			{
				static_cast< CDatabaseValue< EFieldType_FIXED_POINT > & >( field->GetObjectValue() ).SetValue( CFixedPoint( value, infos->GetPrecision().first, infos->GetPrecision().second ) );
			}

			return field;
		}

		template<>
		DatabaseFieldPtr GetValue< EFieldType_VARCHAR >( sqlite3_stmt * statement, int i, DatabaseFieldInfosPtr infos )
		{
			DatabaseFieldPtr field;
			const char * value = reinterpret_cast< const char * >( sqlite3_column_text( statement, i ) );
			int iSize = sqlite3_column_bytes( statement, i );
			field = std::make_shared< CDatabaseField >( infos );

			if ( value && iSize != 0 )
			{
				static_cast< CDatabaseValue< EFieldType_VARCHAR > & >( field->GetObjectValue() ).SetValue( value );
			}

			return field;
		}

		template<>
		DatabaseFieldPtr GetValue< EFieldType_TEXT >( sqlite3_stmt * statement, int i, DatabaseFieldInfosPtr infos )
		{
			DatabaseFieldPtr field;
			const char * value = reinterpret_cast< const char * >( sqlite3_column_text( statement, i ) );
			int iSize = sqlite3_column_bytes( statement, i );
			field = std::make_shared< CDatabaseField >( infos );

			if ( value && iSize != 0 )
			{
				static_cast< CDatabaseValue< EFieldType_TEXT > & >( field->GetObjectValue() ).SetValue( value );
			}

			return field;
		}

		template<>
		DatabaseFieldPtr GetValue< EFieldType_DATE >( sqlite3_stmt * statement, int i, DatabaseFieldInfosPtr infos )
		{
			DatabaseFieldPtr field;
			const char * value = reinterpret_cast< const char * >( sqlite3_column_text( statement, i ) );
			int iSize = sqlite3_column_bytes( statement, i );
			field = std::make_shared< CDatabaseField >( infos );

			if ( value && iSize != 0 )
			{
				static_cast< CDatabaseValue< EFieldType_DATE > & >( field->GetObjectValue() ).SetValue( infos->GetConnection()->ParseDate( value ) );
			}

			return field;
		}

		template<>
		DatabaseFieldPtr GetValue< EFieldType_DATETIME >( sqlite3_stmt * statement, int i, DatabaseFieldInfosPtr infos )
		{
			DatabaseFieldPtr field;
			const char * value = reinterpret_cast< const char * >( sqlite3_column_text( statement, i ) );
			int iSize = sqlite3_column_bytes( statement, i );
			field = std::make_shared< CDatabaseField >( infos );

			if ( value && iSize != 0 )
			{
				static_cast< CDatabaseValue< EFieldType_DATETIME > & >( field->GetObjectValue() ).SetValue( infos->GetConnection()->ParseDateTime( value ) );
			}

			return field;
		}

		template<>
		DatabaseFieldPtr GetValue< EFieldType_TIME >( sqlite3_stmt * statement, int i, DatabaseFieldInfosPtr infos )
		{
			DatabaseFieldPtr field;
			const char * value = reinterpret_cast< const char * >( sqlite3_column_text( statement, i ) );
			int iSize = sqlite3_column_bytes( statement, i );
			field = std::make_shared< CDatabaseField >( infos );

			if ( value && iSize != 0 )
			{
				static_cast< CDatabaseValue< EFieldType_TIME > & >( field->GetObjectValue() ).SetValue( infos->GetConnection()->ParseTime( value ) );
			}

			return field;
		}

		template<>
		DatabaseFieldPtr GetValue< EFieldType_NVARCHAR >( sqlite3_stmt * statement, int i, DatabaseFieldInfosPtr infos )
		{
			DatabaseFieldPtr field;
			const wchar_t * value = reinterpret_cast< const wchar_t * >( sqlite3_column_text16( statement, i ) );
			int iSize = sqlite3_column_bytes16( statement, i );
			field = std::make_shared< CDatabaseField >( infos );

			if ( value && iSize != 0 )
			{
				static_cast< CDatabaseValue< EFieldType_NVARCHAR > & >( field->GetObjectValue() ).SetValue( value );
			}

			return field;
		}

		template<>
		DatabaseFieldPtr GetValue< EFieldType_NTEXT >( sqlite3_stmt * statement, int i, DatabaseFieldInfosPtr infos )
		{
			DatabaseFieldPtr field;
			const wchar_t * value = reinterpret_cast< const wchar_t * >( sqlite3_column_text16( statement, i ) );
			int iSize = sqlite3_column_bytes16( statement, i );
			field = std::make_shared< CDatabaseField >( infos );

			if ( value && iSize != 0 )
			{
				static_cast< CDatabaseValue< EFieldType_NTEXT > & >( field->GetObjectValue() ).SetValue( value );
			}

			return field;
		}

		template<>
		DatabaseFieldPtr GetValue< EFieldType_BINARY >( sqlite3_stmt * statement, int i, DatabaseFieldInfosPtr infos )
		{
			DatabaseFieldPtr field;
			const uint8_t * value = reinterpret_cast< const uint8_t * >( sqlite3_column_blob( statement, i ) );
			int iSize = sqlite3_column_bytes( statement, i );
			field = std::make_shared< CDatabaseField >( infos );

			if ( value && iSize != 0 )
			{
				static_cast< CDatabaseValue< EFieldType_BINARY > & >( field->GetObjectValue() ).SetValue( value, std::min( uint32_t( iSize ), infos->GetLimits() ) );
			}

			return field;
		}

		template<>
		DatabaseFieldPtr GetValue< EFieldType_VARBINARY >( sqlite3_stmt * statement, int i, DatabaseFieldInfosPtr infos )
		{
			DatabaseFieldPtr field;
			const uint8_t * value = reinterpret_cast< const uint8_t * >( sqlite3_column_blob( statement, i ) );
			int iSize = sqlite3_column_bytes( statement, i );
			field = std::make_shared< CDatabaseField >( infos );

			if ( value && iSize != 0 )
			{
				static_cast< CDatabaseValue< EFieldType_VARBINARY > & >( field->GetObjectValue() ).SetValue( value, std::min( uint32_t( iSize ), infos->GetLimits() ) );
			}

			return field;
		}

		template<>
		DatabaseFieldPtr GetValue< EFieldType_LONG_VARBINARY >( sqlite3_stmt * statement, int i, DatabaseFieldInfosPtr infos )
		{
			DatabaseFieldPtr field;
			const uint8_t * value = reinterpret_cast< const uint8_t * >( sqlite3_column_blob( statement, i ) );
			int iSize = sqlite3_column_bytes( statement, i );
			field = std::make_shared< CDatabaseField >( infos );

			if ( value && iSize != 0 )
			{
				static_cast< CDatabaseValue< EFieldType_LONG_VARBINARY > & >( field->GetObjectValue() ).SetValue( value, std::min( uint32_t( iSize ), infos->GetLimits() ) );
			}

			return field;
		}

		template<>
		DatabaseFieldPtr GetValue< EFieldType_NULL >( sqlite3_stmt * statement, int i, DatabaseFieldInfosPtr infos )
		{
			DatabaseFieldPtr field;
			sqlite3_value * value = sqlite3_column_value( statement, i );
			int type = sqlite3_value_type( value );

			switch ( type )
			{
			case SQLITE_INTEGER:
				infos->SetType( EFieldType_SINT32 );
				field = GetValue< EFieldType_SINT32 >( statement, i, infos );
				break;

			case SQLITE_FLOAT:
				infos->SetType( EFieldType_FLOAT32 );
				field = GetValue< EFieldType_FLOAT32 >( statement, i, infos );
				break;

			case SQLITE_TEXT:
				infos->SetType( EFieldType_TEXT );
				field = GetValue< EFieldType_TEXT >( statement, i, infos );
				break;

			case SQLITE_BLOB:
				infos->SetType( EFieldType_VARBINARY );
				field = GetValue< EFieldType_VARBINARY >( statement, i, infos );
				break;

			case SQLITE_NULL:
			{
				const char * value = reinterpret_cast< const char * >( sqlite3_column_text( statement, i ) );
				int iSize = sqlite3_column_bytes( statement, i );

				if ( value && iSize )
				{
					infos->SetType( EFieldType_TEXT );
					field = GetValue< EFieldType_TEXT >( statement, i, infos );
				}
				else
				{
					infos->SetType( EFieldType_SINT32 );
					field = GetValue< EFieldType_SINT32 >( statement, i, infos );
				}
			}
			break;
			}

			return field;
		}

		DatabaseFieldInfosPtr GetIntegerFieldInfos( const std::string & type, const String & columnName, const std::string & lowerName, DatabaseConnectionSqlitePtr connection )
		{
			DatabaseFieldInfosPtr infos;

			if ( type.find( "BIGINT" ) != std::string::npos
					|| lowerName.find( "max(" ) != std::string::npos
					|| lowerName.find( "min(" ) != std::string::npos
					|| lowerName.find( "count(" ) != std::string::npos
					|| lowerName.find( "sum(" ) != std::string::npos )
			{
				infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_SINT64 );
			}
			else if ( type.find( "SMALLINT" ) != std::string::npos )
			{
				infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_SINT16 );
			}
			else if ( type.find( "TINYINT" ) != std::string::npos )
			{
				infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_SINT8 );
			}
			else if ( type.find( "bool" ) != std::string::npos )
			{
				infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_BIT );
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
							infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_SINT8 );
						}
						else if ( prec == 2 )
						{
							infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_SINT16 );
						}
						else if ( prec <= 3 )
						{
							infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_SINT24 );
						}
						else if ( prec <= 4 )
						{
							infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_SINT32 );
						}
						else
						{
							infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_SINT64 );
						}
					}
					else
					{
						infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_SINT32 );
					}
				}
				else
				{
					infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_SINT32 );
				}
			}

			return infos;
		}

		DatabaseFieldInfosPtr GetFloatFieldInfos( const std::string & type, const String & columnName, const std::string & lowerName, DatabaseConnectionSqlitePtr connection )
		{
			DatabaseFieldInfosPtr infos;

			if ( type.find( "DOUB" ) != std::string::npos
					|| type.find( "REAL" ) != std::string::npos )
			{
				infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_FLOAT64 );
			}
			else if ( type.find( "DECIMAL" ) != std::string::npos )
			{
				infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_FIXED_POINT );
			}
			else
			{
				infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_FLOAT32 );
			}

			return infos;
		}

		DatabaseFieldInfosPtr GetStringFieldInfos( const std::string & type, const String & columnName, const std::string & lowerName, DatabaseConnectionSqlitePtr connection )
		{
			DatabaseFieldInfosPtr infos;

			if ( type.find( "NCHAR" ) != std::string::npos
					|| type.find( "NVARCHAR" ) != std::string::npos )
			{
				infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_NVARCHAR, RetrieveLimits( type ) );
			}
			else if ( type.find( "CHAR" ) != std::string::npos )
			{
				infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_VARCHAR, RetrieveLimits( type ) );
			}
			else if ( type.find( "NTEXT" ) != std::string::npos )
			{
				infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_NTEXT );
			}
			else
			{
				infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_TEXT );
			}

			return infos;
		}

		DatabaseFieldInfosPtr GetBlobFieldInfos( const std::string & type, const String & columnName, const std::string & lowerName, DatabaseConnectionSqlitePtr connection )
		{
			DatabaseFieldInfosPtr infos;

			if ( type.find( "VARBINARY" ) != std::string::npos )
			{
				infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_VARBINARY, RetrieveLimits( type ) );
			}
			else if ( type.find( "BINARY" ) != std::string::npos )
			{
				infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_BINARY, RetrieveLimits( type ) );
			}
			else
			{
				infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_LONG_VARBINARY );
			}

			return infos;
		}

		DatabaseFieldInfosPtr GetNullFieldInfos( const std::string & type, const String & columnName, const std::string & lowerName, DatabaseConnectionSqlitePtr connection )
		{
			DatabaseFieldInfosPtr infos;
			std::string upperType = CStrUtils::UpperCase( type );
			size_t index;

			if ( upperType == "DATETIME" )
			{
				infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_DATETIME );
			}
			else if ( upperType == "DATE" )
			{
				infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_DATE );
			}
			else if ( upperType == "TIME" )
			{
				infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_TIME );
			}
			else if ( upperType.find( "BIGINT" ) != std::string::npos
					  || lowerName.find( "max(" ) != std::string::npos
					  || lowerName.find( "min(" ) != std::string::npos
					  || lowerName.find( "count(" ) != std::string::npos )
			{
				infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_SINT64 );
			}
			else if ( upperType.find( "FLOA" ) != String::npos
					  || lowerName.find( "sum(" ) != std::string::npos )
			{
				infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_FLOAT32 );
			}
			else if ( upperType.find( "NUMERIC" ) != std::string::npos )
			{
				infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_FIXED_POINT, std::make_pair( 0, 0 ) );
			}
			else if ( upperType.find( "DECIMAL" ) != String::npos )
			{
				infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_FIXED_POINT, RetrieveLimits( upperType ) );
			}
			else if ( upperType.find( "SMALLINT" ) != String::npos )
			{
				infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_SINT16 );
			}
			else if ( upperType.find( "BOOL" ) != String::npos
					  || upperType.find( "BIT" ) != String::npos )
			{
				infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_BIT );
			}
			else if ( upperType.find( "TINYINT" ) != String::npos )
			{
				infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_SINT8 );
			}
			else if ( ( index = upperType.find( "MEDIUMINT" ) ) != String::npos )
			{
				infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_SINT24 );
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
							infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_SINT8 );
						}
						else if ( prec == 2 )
						{
							infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_SINT16 );
						}
						else if ( prec <= 3 )
						{
							infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_SINT24 );
						}
						else if ( prec <= 4 )
						{
							infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_SINT32 );
						}
						else
						{
							infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_SINT64 );
						}
					}
					else
					{
						infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_SINT32 );
					}
				}
				else
				{
					infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_SINT32 );
				}
			}
			else if ( upperType.find( "NCHAR" ) != String::npos
					  || upperType.find( "NVARCHAR" ) != String::npos )
			{
				infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_NVARCHAR, RetrieveLimits( upperType ) );
			}
			else if ( upperType.find( "CHAR" ) != String::npos )
			{
				infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_VARCHAR, RetrieveLimits( upperType ) );
			}
			else if ( upperType.find( "NTEXT" ) != String::npos )
			{
				infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_NTEXT, -1 );
			}
			else if ( upperType.find( "CLOB" ) != String::npos
					  || upperType.find( "TEXT" ) != String::npos )
			{
				infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_TEXT, -1 );
			}
			else if ( upperType.find( "REAL" ) != String::npos
					  || upperType.find( "DOUB" ) != String::npos )
			{
				infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_FLOAT64 );
			}
			else if ( upperType.find( "BLOB" ) != String::npos )
			{
				infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_LONG_VARBINARY, -1 );
			}
			else if ( upperType.find( "VARBINARY" ) != String::npos )
			{
				infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_VARBINARY, RetrieveLimits( upperType ) );
			}
			else if ( upperType.find( "BINARY" ) != String::npos )
			{
				infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_BINARY, RetrieveLimits( upperType ) );
			}
			else
			{
				CLogger::LogWarning( ERROR_SQLITE_UNSUPPORTED_TYPE );
				infos = std::make_shared< CDatabaseFieldInfos >( connection, columnName, EFieldType_NULL, -1 );
			}

			return infos;
		}
	}

	DatabaseFieldInfosPtrArray SqliteGetColumns( sqlite3_stmt * statement, DatabaseConnectionSqlitePtr connection )
	{
		DatabaseFieldInfosPtrArray arrayReturn;
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
					arrayReturn.push_back( GetIntegerFieldInfos( type, CStrUtils::ToString( pszName ), CStrUtils::LowerCase( pszName ), connection ) );
					break;

				case SQLITE_FLOAT:
					arrayReturn.push_back( GetFloatFieldInfos( type, CStrUtils::ToString( pszName ), CStrUtils::LowerCase( pszName ), connection ) );
					break;

				case SQLITE_TEXT:
					arrayReturn.push_back( GetStringFieldInfos( type, CStrUtils::ToString( pszName ), CStrUtils::LowerCase( pszName ), connection ) );
					break;

				case SQLITE_BLOB:
					arrayReturn.push_back( GetBlobFieldInfos( type, CStrUtils::ToString( pszName ), CStrUtils::LowerCase( pszName ), connection ) );
					break;

				case SQLITE_NULL:
					arrayReturn.push_back( GetNullFieldInfos( type, CStrUtils::ToString( pszName ), CStrUtils::LowerCase( pszName ), connection ) );
					break;

				default:
					DB_EXCEPT( EDatabaseExceptionCodes_StatementError, ERROR_SQLITE_UNSUPPORTED_TYPE );
					break;
				}
			}
		}

		return arrayReturn;
	}

	DatabaseFieldPtr SetFieldValue( sqlite3_stmt * statement, int index, DatabaseFieldInfosPtr infos )
	{
		DatabaseFieldPtr field;

		switch ( infos->GetType() )
		{
		case EFieldType_NULL:
			field = GetValue< EFieldType_NULL >( statement, index, infos );
			break;

		case EFieldType_BIT:
			field = GetValue< EFieldType_BIT >( statement, index, infos );
			break;

		case EFieldType_SINT8:
			field = GetValue< EFieldType_SINT8 >( statement, index, infos );
			break;

		case EFieldType_SINT16:
			field = GetValue< EFieldType_SINT16 >( statement, index, infos );
			break;

		case EFieldType_SINT24:
			field = GetValue< EFieldType_SINT24 >( statement, index, infos );
			break;

		case EFieldType_SINT32:
			field = GetValue< EFieldType_SINT32 >( statement, index, infos );
			break;

		case EFieldType_SINT64:
			field = GetValue< EFieldType_SINT64 >( statement, index, infos );
			break;

		case EFieldType_UINT8:
			field = GetValue< EFieldType_UINT8 >( statement, index, infos );
			break;

		case EFieldType_UINT16:
			field = GetValue< EFieldType_UINT16 >( statement, index, infos );
			break;

		case EFieldType_UINT24:
			field = GetValue< EFieldType_UINT24 >( statement, index, infos );
			break;

		case EFieldType_UINT32:
			field = GetValue< EFieldType_UINT32 >( statement, index, infos );
			break;

		case EFieldType_UINT64:
			field = GetValue< EFieldType_UINT64 >( statement, index, infos );
			break;

		case EFieldType_FLOAT32:
			field = GetValue< EFieldType_FLOAT32 >( statement, index, infos );
			break;

		case EFieldType_FLOAT64:
			field = GetValue< EFieldType_FLOAT64 >( statement, index, infos );
			break;

		case EFieldType_FIXED_POINT:
			field = GetValue< EFieldType_FIXED_POINT >( statement, index, infos );
			break;

		case EFieldType_VARCHAR:
			field = GetValue< EFieldType_VARCHAR >( statement, index, infos );
			break;

		case EFieldType_TEXT:
			field = GetValue< EFieldType_TEXT >( statement, index, infos );
			break;

		case EFieldType_NVARCHAR:
			field = GetValue< EFieldType_NVARCHAR >( statement, index, infos );
			break;

		case EFieldType_NTEXT:
			field = GetValue< EFieldType_NTEXT >( statement, index, infos );
			break;

		case EFieldType_DATE:
			field = GetValue< EFieldType_DATE >( statement, index, infos );
			break;

		case EFieldType_DATETIME:
			field = GetValue< EFieldType_DATETIME >( statement, index, infos );
			break;

		case EFieldType_TIME:
			field = GetValue< EFieldType_TIME >( statement, index, infos );
			break;

		case EFieldType_BINARY:
			field = GetValue< EFieldType_BINARY >( statement, index, infos );
			break;

		case EFieldType_VARBINARY:
			field = GetValue< EFieldType_VARBINARY >( statement, index, infos );
			break;

		case EFieldType_LONG_VARBINARY:
			field = GetValue< EFieldType_LONG_VARBINARY >( statement, index, infos );
			break;

		default:
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, ERROR_SQLITE_UNSUPPORTED_TYPE );
			break;
		}

		return field;
	}

	DatabaseResultPtr SqliteFetchResult( sqlite3_stmt * statement, DatabaseFieldInfosPtrArray const & columns, DatabaseConnectionSqlitePtr connection )
	{
		DatabaseResultPtr pReturn;
		int code = SQLITE_ERROR;

		if ( connection->IsConnected() )
		{
			pReturn = std::make_unique< CDatabaseResult >( connection, columns );
			code = SQLITE_OK;
			int iResult;
			int iNbColumns = int( columns.size() );

			while ( ( iResult = sqlite3_step( statement ) ) != SQLITE_DONE )
			{
				if ( iResult == SQLITE_ROW )
				{
					DatabaseRowPtr pRow = std::make_shared< CDatabaseRow >( connection );

					for ( int i = 0; i < iNbColumns; i++ )
					{
						DatabaseFieldInfosPtr infos;
						DatabaseFieldPtr field;

						try
						{
							infos = pReturn->GetFieldInfos( i );
						}
						catch ( CExceptionDatabase & )
						{
							throw;
						}

						pRow->AddField( SetFieldValue( statement, i, infos ) );
					}

					pReturn->AddRow( pRow );
				}
			}
		}

		return pReturn;
	}

	void SQLiteCheck( int code, TChar const * msg, EDatabaseExceptionCodes exc, sqlite3 * database )
	{
		if ( code != SQLITE_OK )
		{
			StringStream error;
			error << STR( "Error : " ) << msg << STR( " - " ) << CStrUtils::ToString( sqlite3_errmsg( database ) );
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
