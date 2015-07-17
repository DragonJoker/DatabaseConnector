/************************************************************************//**
* @file DatabaseSqlite.cpp
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

#include "DatabaseSqlite.h"
#include "DatabaseConnectionSqlite.h"

#include <DatabaseFieldInfos.h>

BEGIN_NAMESPACE_DATABASE_SQLITE
{
	namespace
	{
		int GetLimits( std::string const & type )
		{
			int result = -1;
			std::size_t index = type.find( STR( "(" ) );

			if ( index != String::npos )
			{
				std::string limit = type.substr( index + 1, type.find( STR( ")" ) ) - index );
				result = CStrUtils::ToInt( CStrUtils::Trim( limit ) );
			}
			else
			{
				result = -1;
			}

			return result;
		}
	}

	static const String ERROR_SQLITE_NO_VALUE_FIELD = STR( "Field as no value" );

	CDatabaseSqlite::CDatabaseSqlite()
		: CDatabase()
	{
	}

	CDatabaseSqlite::~CDatabaseSqlite()
	{
	}

	const String & CDatabaseSqlite::GetType()const
	{
		return DATABASE_SQLITE_TYPE;
	}

	CDatabase * CDatabaseSqlite::Create()
	{
		return new CDatabaseSqlite;
	}

	DatabaseConnectionPtr CDatabaseSqlite::DoCreateConnection( String & connectionString ) const
	{
		return std::make_shared< CDatabaseConnectionSqlite >( _server, _userName, _password, connectionString );
	}

	DatabaseFieldInfosPtrArray SqliteGetColumns( sqlite3_stmt * pStatement, DatabaseConnectionPtr pConnexion )
	{
		DatabaseFieldInfosPtrArray arrayReturn;
		int iColumnCount = sqlite3_column_count( pStatement );

		for ( int i = 0; i < iColumnCount; i++ )
		{
			char const * pszName = sqlite3_column_name( pStatement, i );

			if ( pszName )
			{
				String strColumnName = CStrUtils::ToString( pszName );
				String strLowerName = CStrUtils::LowerCase( strColumnName );
				char const * pszType = sqlite3_column_decltype( pStatement, i );
				int dataType = sqlite3_column_type( pStatement, i );
				DatabaseFieldInfosPtr infos;
				std::string type( pszType ? pszType : "" );

				switch ( dataType )
				{
				case SQLITE_INTEGER:
					if ( type.find( "BIGINT" ) != std::string::npos
					|| strLowerName.find( "max(" ) != std::string::npos
					|| strLowerName.find( "min(" ) != String::npos
					|| strLowerName.find( "count(" ) != String::npos
					|| strLowerName.find( "sum(" ) != String::npos )
					{
						infos = std::make_shared< CDatabaseFieldInfos >( pConnexion, strColumnName, EFieldType_INT64 );
					}
					else if ( type.find( "SMALLINT" ) != std::string::npos )
					{
						infos = std::make_shared< CDatabaseFieldInfos >( pConnexion, strColumnName, EFieldType_INT16 );
					}
					else if ( type.find( "TINYINT" ) != std::string::npos )
					{
						infos = std::make_shared< CDatabaseFieldInfos >( pConnexion, strColumnName, EFieldType_INT8 );
					}
					else if ( type.find( "bool" ) != std::string::npos )
					{
						infos = std::make_shared< CDatabaseFieldInfos >( pConnexion, strColumnName, EFieldType_BIT );
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
									infos = std::make_shared< CDatabaseFieldInfos >( pConnexion, strColumnName, EFieldType_INT8 );
								}
								else if ( prec == 2 )
								{
									infos = std::make_shared< CDatabaseFieldInfos >( pConnexion, strColumnName, EFieldType_INT16 );
								}
								else if ( prec <= 3 )
								{
									infos = std::make_shared< CDatabaseFieldInfos >( pConnexion, strColumnName, EFieldType_INT24 );
								}
								else if ( prec <= 4 )
								{
									infos = std::make_shared< CDatabaseFieldInfos >( pConnexion, strColumnName, EFieldType_INT32 );
								}
								else
								{
									infos = std::make_shared< CDatabaseFieldInfos >( pConnexion, strColumnName, EFieldType_INT64 );
								}
							}
							else
							{
								infos = std::make_shared< CDatabaseFieldInfos >( pConnexion, strColumnName, EFieldType_INT32 );
							}
						}
						else
						{
							infos = std::make_shared< CDatabaseFieldInfos >( pConnexion, strColumnName, EFieldType_INT32 );
						}
					}

					break;

				case SQLITE_FLOAT:
					if ( type.find( "FLOA" ) != std::string::npos
							|| type == "SUM" )
					{
						infos = std::make_shared< CDatabaseFieldInfos >( pConnexion, strColumnName, EFieldType_FLOAT32 );
					}
					else if ( type.find( "DOUB" ) != std::string::npos
							  || type.find( "REAL" ) != std::string::npos )
					{
						infos = std::make_shared< CDatabaseFieldInfos >( pConnexion, strColumnName, EFieldType_FLOAT64 );
					}
					else if ( type.find( "DECIMAL" ) != std::string::npos )
					{
						infos = std::make_shared< CDatabaseFieldInfos >( pConnexion, strColumnName, EFieldType_FIXED_POINT );
					}

					break;

				case SQLITE_TEXT:
					if ( type.find( "NCHAR" ) != std::string::npos
							|| type.find( "NVARCHAR" ) != std::string::npos )
					{
						infos = std::make_shared< CDatabaseFieldInfos >( pConnexion, strColumnName, EFieldType_NVARCHAR, GetLimits( type ) );
					}
					else if ( type.find( "CHAR" ) != std::string::npos )
					{
						infos = std::make_shared< CDatabaseFieldInfos >( pConnexion, strColumnName, EFieldType_VARCHAR, GetLimits( type ) );
					}
					else if ( type.find( "NTEXT" ) != std::string::npos )
					{
						infos = std::make_shared< CDatabaseFieldInfos >( pConnexion, strColumnName, EFieldType_NTEXT );
					}
					else if ( type.find( "CLOB" ) != std::string::npos
							  || type.find( "TEXT" ) != std::string::npos )
					{
						infos = std::make_shared< CDatabaseFieldInfos >( pConnexion, strColumnName, EFieldType_TEXT );
					}

					break;

				case SQLITE_BLOB:
					if ( type.find( "BLOB" ) != std::string::npos )
					{
						infos = std::make_shared< CDatabaseFieldInfos >( pConnexion, strColumnName, EFieldType_LONG_VARBINARY );
					}
					else if ( type.find( "VARBINARY" ) != std::string::npos )
					{
						infos = std::make_shared< CDatabaseFieldInfos >( pConnexion, strColumnName, EFieldType_VARBINARY, GetLimits( type ) );
					}
					else if ( type.find( "BINARY" ) != std::string::npos )
					{
						infos = std::make_shared< CDatabaseFieldInfos >( pConnexion, strColumnName, EFieldType_BINARY, GetLimits( type ) );
					}

					break;

				case SQLITE_NULL:
					if ( type == "DATETIME" )
					{
						infos = std::make_shared< CDatabaseFieldInfos >( pConnexion, strColumnName, EFieldType_DATETIME );
					}
					else if ( type == "DATE" )
					{
						infos = std::make_shared< CDatabaseFieldInfos >( pConnexion, strColumnName, EFieldType_DATE );
					}
					else if ( type == "TIME" )
					{
						infos = std::make_shared< CDatabaseFieldInfos >( pConnexion, strColumnName, EFieldType_TIME );
					}

					break;

				default:
					break;
				}

				if ( !infos )
				{
					if ( strLowerName.find( STR( "max(" ) ) != std::string::npos )
					{
						arrayReturn.push_back( std::make_shared< CDatabaseFieldInfos >( pConnexion, strColumnName, STR( "max" ) ) );
					}
					else if ( strLowerName.find( STR( "min(" ) ) != std::string::npos )
					{
						arrayReturn.push_back( std::make_shared< CDatabaseFieldInfos >( pConnexion, strColumnName, STR( "min" ) ) );
					}
					else if ( strLowerName.find( STR( "count(" ) ) != std::string::npos )
					{
						arrayReturn.push_back( std::make_shared< CDatabaseFieldInfos >( pConnexion, strColumnName, STR( "count" ) ) );
					}
					else if ( strLowerName.find( STR( "sum(" ) ) != std::string::npos )
					{
						arrayReturn.push_back( std::make_shared< CDatabaseFieldInfos >( pConnexion, strColumnName, STR( "sum" ) ) );
					}
					else if ( type.find( "NUMERIC" ) != std::string::npos )
					{
						arrayReturn.push_back( std::make_shared< CDatabaseFieldInfos >( pConnexion, strColumnName, EFieldType_FIXED_POINT, std::make_pair( 0, 0 ) ) );
					}
					else
					{
						arrayReturn.push_back( std::make_shared< CDatabaseFieldInfos >( pConnexion, strColumnName, type ) );
					}
				}
				else
				{
					arrayReturn.push_back( infos );
				}
			}
		}

		return arrayReturn;
	}

	template< EFieldType Type, typename Value >
	DatabaseFieldPtr ConstructField( sqlite3_stmt * pStatement, int i, DatabaseFieldInfosPtr pInfos, Value value )
	{
		int iSize = sqlite3_column_bytes( pStatement, i );
		DatabaseFieldPtr pField;
		pField = std::make_shared< CDatabaseField >( pInfos );

		if ( iSize != 0 )
		{
			static_cast< CDatabaseValue< Type > & >( pField->GetObjectValue() ).SetValue( value );
		}

		return pField;
	}

	template< EFieldType Type > DatabaseFieldPtr GetValue( sqlite3_stmt * pStatement, int i, DatabaseFieldInfosPtr pInfos );

	template<>
	DatabaseFieldPtr GetValue< EFieldType_BIT >( sqlite3_stmt * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		bool value = sqlite3_column_int( pStatement, i ) != 0;
		return ConstructField< EFieldType_BIT >( pStatement, i, pInfos, value );
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_INT8 >( sqlite3_stmt * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		int value = sqlite3_column_int( pStatement, i );
		return ConstructField< EFieldType_INT8 >( pStatement, i, pInfos, value );
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_INT16 >( sqlite3_stmt * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		int value = sqlite3_column_int( pStatement, i );
		return ConstructField< EFieldType_INT16 >( pStatement, i, pInfos, value );
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_INT24 >( sqlite3_stmt * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		int value = sqlite3_column_int( pStatement, i );
		return ConstructField< EFieldType_INT24 >( pStatement, i, pInfos, int24_t( value ) );
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_INT32 >( sqlite3_stmt * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		int value = sqlite3_column_int( pStatement, i );
		return ConstructField< EFieldType_INT32 >( pStatement, i, pInfos, value );
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_INT64 >( sqlite3_stmt * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		int64_t value = sqlite3_column_int64( pStatement, i );
		return ConstructField< EFieldType_INT64 >( pStatement, i, pInfos, value );
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_FLOAT32 >( sqlite3_stmt * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		float value = float( sqlite3_column_double( pStatement, i ) );
		return ConstructField< EFieldType_FLOAT32 >( pStatement, i, pInfos, value );
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_FLOAT64 >( sqlite3_stmt * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		double value = sqlite3_column_double( pStatement, i );
		return ConstructField< EFieldType_FLOAT64 >( pStatement, i, pInfos, value );
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_FIXED_POINT >( sqlite3_stmt * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		DatabaseFieldPtr pField;
		char * value = ( char * )sqlite3_column_text( pStatement, i );
		int iSize = sqlite3_column_bytes( pStatement, i );
		pField = std::make_shared< CDatabaseField >( pInfos );

		if ( iSize )
		{
			static_cast< CDatabaseValue< EFieldType_FIXED_POINT > & >( pField->GetObjectValue() ).SetValue( CFixedPoint( value, pInfos->GetPrecision().first ) );
		}

		return pField;
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_VARCHAR >( sqlite3_stmt * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		DatabaseFieldPtr pField;
		char * value = ( char * )sqlite3_column_text( pStatement, i );
		int iSize = sqlite3_column_bytes( pStatement, i );
		pField = std::make_shared< CDatabaseField >( pInfos );

		if ( value && iSize != 0 )
		{
			static_cast< CDatabaseValue< EFieldType_VARCHAR > & >( pField->GetObjectValue() ).SetValue( value );
		}

		return pField;
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_TEXT >( sqlite3_stmt * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		DatabaseFieldPtr pField;
		char * value = ( char * )sqlite3_column_text( pStatement, i );
		int iSize = sqlite3_column_bytes( pStatement, i );
		pField = std::make_shared< CDatabaseField >( pInfos );

		if ( value && iSize != 0 )
		{
			static_cast< CDatabaseValue< EFieldType_TEXT > & >( pField->GetObjectValue() ).SetValue( value );
		}

		return pField;
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_DATE >( sqlite3_stmt * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		DatabaseFieldPtr pField;
		char * value = ( char * )sqlite3_column_text( pStatement, i );
		int iSize = sqlite3_column_bytes( pStatement, i );
		pField = std::make_shared< CDatabaseField >( pInfos );

		if ( value && iSize != 0 )
		{
			static_cast< CDatabaseValue< EFieldType_DATE > & >( pField->GetObjectValue() ).SetValue( pInfos->GetConnection()->ParseDate( value ) );
		}

		return pField;
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_DATETIME >( sqlite3_stmt * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		DatabaseFieldPtr pField;
		char * value = ( char * )sqlite3_column_text( pStatement, i );
		int iSize = sqlite3_column_bytes( pStatement, i );
		pField = std::make_shared< CDatabaseField >( pInfos );

		if ( value && iSize != 0 )
		{
			static_cast< CDatabaseValue< EFieldType_DATETIME > & >( pField->GetObjectValue() ).SetValue( pInfos->GetConnection()->ParseDateTime( value ) );
		}

		return pField;
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_TIME >( sqlite3_stmt * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		DatabaseFieldPtr pField;
		char * value = ( char * )sqlite3_column_text( pStatement, i );
		int iSize = sqlite3_column_bytes( pStatement, i );
		pField = std::make_shared< CDatabaseField >( pInfos );

		if ( value && iSize != 0 )
		{
			static_cast< CDatabaseValue< EFieldType_TIME > & >( pField->GetObjectValue() ).SetValue( pInfos->GetConnection()->ParseTime( value ) );
		}

		return pField;
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_NVARCHAR >( sqlite3_stmt * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		DatabaseFieldPtr pField;
		wchar_t * value = ( wchar_t * )sqlite3_column_text16( pStatement, i );
		int iSize = sqlite3_column_bytes16( pStatement, i );
		pField = std::make_shared< CDatabaseField >( pInfos );

		if ( value && iSize != 0 )
		{
			static_cast< CDatabaseValue< EFieldType_NVARCHAR > & >( pField->GetObjectValue() ).SetValue( value );
		}

		return pField;
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_NTEXT >( sqlite3_stmt * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		DatabaseFieldPtr pField;
		wchar_t * value = ( wchar_t * )sqlite3_column_text16( pStatement, i );
		int iSize = sqlite3_column_bytes16( pStatement, i );
		pField = std::make_shared< CDatabaseField >( pInfos );

		if ( value && iSize != 0 )
		{
			static_cast< CDatabaseValue< EFieldType_NTEXT > & >( pField->GetObjectValue() ).SetValue( value );
		}

		return pField;
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_BINARY >( sqlite3_stmt * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		DatabaseFieldPtr pField;
		uint8_t * value = ( uint8_t * )sqlite3_column_blob( pStatement, i );
		int iSize = sqlite3_column_bytes( pStatement, i );
		pField = std::make_shared< CDatabaseField >( pInfos );

		if ( value && iSize != 0 )
		{
			static_cast< CDatabaseValue< EFieldType_BINARY > & >( pField->GetObjectValue() ).SetValue( value, std::min( uint32_t( iSize ), pInfos->GetLimits() ) );
		}

		return pField;
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_VARBINARY >( sqlite3_stmt * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		DatabaseFieldPtr pField;
		uint8_t * value = ( uint8_t * )sqlite3_column_blob( pStatement, i );
		int iSize = sqlite3_column_bytes( pStatement, i );
		pField = std::make_shared< CDatabaseField >( pInfos );

		if ( value && iSize != 0 )
		{
			static_cast< CDatabaseValue< EFieldType_VARBINARY > & >( pField->GetObjectValue() ).SetValue( value, std::min( uint32_t( iSize ), pInfos->GetLimits() ) );
		}

		return pField;
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_LONG_VARBINARY >( sqlite3_stmt * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		DatabaseFieldPtr pField;
		uint8_t * value = ( uint8_t * )sqlite3_column_blob( pStatement, i );
		int iSize = sqlite3_column_bytes( pStatement, i );
		pField = std::make_shared< CDatabaseField >( pInfos );

		if ( value && iSize != 0 )
		{
			static_cast< CDatabaseValue< EFieldType_LONG_VARBINARY > & >( pField->GetObjectValue() ).SetValue( value, std::min( uint32_t( iSize ), pInfos->GetLimits() ) );
		}

		return pField;
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_NULL >( sqlite3_stmt * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		assert( pInfos->GetType() == EFieldType_NULL );
		DatabaseFieldPtr pField;
		sqlite3_value * value = sqlite3_column_value( pStatement, i );
		int type = sqlite3_value_type( value );

		switch ( type )
		{
		case SQLITE_INTEGER:
			pInfos->SetType( EFieldType_INT32 );
			pField = GetValue< EFieldType_INT32 >( pStatement, i, pInfos );
			break;

		case SQLITE_FLOAT:
			pInfos->SetType( EFieldType_FLOAT32 );
			pField = GetValue< EFieldType_FLOAT32 >( pStatement, i, pInfos );
			break;

		case SQLITE_TEXT:
			pInfos->SetType( EFieldType_TEXT );
			pField = GetValue< EFieldType_TEXT >( pStatement, i, pInfos );
			break;

		case SQLITE_BLOB:
			pInfos->SetType( EFieldType_VARBINARY );
			pField = GetValue< EFieldType_VARBINARY >( pStatement, i, pInfos );
			break;

		case SQLITE_NULL:
		{
			char * value = ( char * )sqlite3_column_text( pStatement, i );
			int iSize = sqlite3_column_bytes( pStatement, i );

			if ( value && iSize )
			{
				pInfos->SetType( EFieldType_TEXT );
				pField = GetValue< EFieldType_TEXT >( pStatement, i, pInfos );
			}
			else
			{
				pInfos->SetType( EFieldType_INT32 );
				pField = GetValue< EFieldType_INT32 >( pStatement, i, pInfos );
			}
		}
		break;
		}

		return pField;
	}

	DatabaseResultPtr SqliteExecute( sqlite3_stmt * pStatement, int & code, DatabaseFieldInfosPtrArray const & arrayColumns, DatabaseConnectionPtr pConnexion )
	{
		DatabaseResultPtr pReturn;
		code = SQLite::eCODE_ERROR;

		if ( pConnexion->IsConnected() )
		{
			pReturn = std::make_unique< CDatabaseResult >( pConnexion, arrayColumns );
			code = SQLite::eCODE_OK;
			int iResult;
			int iNbColumns = int( arrayColumns.size() );

			while ( ( iResult = sqlite3_step( pStatement ) ) != SQLITE_DONE )
			{
				if ( iResult == SQLITE_ROW )
				{
					DatabaseRowPtr pRow = std::make_shared< CDatabaseRow >( pConnexion );

					for ( int i = 0; i < iNbColumns; i++ )
					{
						DatabaseFieldInfosPtr pInfos;
						DatabaseFieldPtr pField;

						try
						{
							pInfos = pReturn->GetFieldInfos( i );
						}
						catch ( CExceptionDatabase & )
						{
							throw;
						}

						switch ( pInfos->GetType() )
						{
						case EFieldType_BIT:
							pField = GetValue< EFieldType_BIT >( pStatement, i, pInfos );
							break;

						case EFieldType_INT8:
							pField = GetValue< EFieldType_INT8 >( pStatement, i, pInfos );
							break;

						case EFieldType_INT16:
							pField = GetValue< EFieldType_INT16 >( pStatement, i, pInfos );
							break;

						case EFieldType_INT24:
							pField = GetValue< EFieldType_INT24 >( pStatement, i, pInfos );
							break;

						case EFieldType_INT32:
							pField = GetValue< EFieldType_INT32 >( pStatement, i, pInfos );
							break;

						case EFieldType_INT64:
							pField = GetValue< EFieldType_INT64 >( pStatement, i, pInfos );
							break;

						case EFieldType_FLOAT32:
							pField = GetValue< EFieldType_FLOAT32 >( pStatement, i, pInfos );
							break;

						case EFieldType_FLOAT64:
							pField = GetValue< EFieldType_FLOAT64 >( pStatement, i, pInfos );
							break;

						case EFieldType_FIXED_POINT:
							pField = GetValue< EFieldType_FIXED_POINT >( pStatement, i, pInfos );
							break;

						case EFieldType_VARCHAR:
							pField = GetValue< EFieldType_VARCHAR >( pStatement, i, pInfos );
							break;

						case EFieldType_TEXT:
							pField = GetValue< EFieldType_TEXT >( pStatement, i, pInfos );
							break;

						case EFieldType_NVARCHAR:
							pField = GetValue< EFieldType_NVARCHAR >( pStatement, i, pInfos );
							break;

						case EFieldType_NTEXT:
							pField = GetValue< EFieldType_NTEXT >( pStatement, i, pInfos );
							break;

						case EFieldType_DATE:
							pField = GetValue< EFieldType_DATE >( pStatement, i, pInfos );
							break;

						case EFieldType_DATETIME:
							pField = GetValue< EFieldType_DATETIME >( pStatement, i, pInfos );
							break;

						case EFieldType_TIME:
							pField = GetValue< EFieldType_TIME >( pStatement, i, pInfos );
							break;

						case EFieldType_BINARY:
							pField = GetValue< EFieldType_BINARY >( pStatement, i, pInfos );
							break;

						case EFieldType_VARBINARY:
							pField = GetValue< EFieldType_VARBINARY >( pStatement, i, pInfos );
							break;

						case EFieldType_LONG_VARBINARY:
							pField = GetValue< EFieldType_LONG_VARBINARY >( pStatement, i, pInfos );
							break;

						default:
							pField = GetValue< EFieldType_NULL >( pStatement, i, pInfos );
							break;
						}

						pRow->AddField( pField );
					}

					pReturn->AddRow( pRow );
				}
			}
		}

		return pReturn;
	}

	void SQLiteTry( int code, TChar const * msg, EDatabaseExceptionCodes exc, sqlite3 * database )
	{
		if ( code != SQLite::eCODE_OK )
		{
			StringStream error;
			error << STR( "Error : " ) << msg << STR( " - " ) << CStrUtils::ToString( sqlite3_errmsg( database ) );
			CLogger::LogError( error );
			DB_EXCEPT( exc, error.str() );
		}

#if !defined( _NDEBUG )

		else
		{
			CLogger::LogDebug( StringStream() << STR( "Success : " ) << msg );
		}

#endif
	}

	void SQLiteTry( int code, std::ostream const & stream, EDatabaseExceptionCodes exc, sqlite3 * database )
	{
		std::stringstream ss;
		ss << stream.rdbuf();
		SQLiteTry( code, ss.str().c_str(), exc, database );
	}
}
END_NAMESPACE_DATABASE_SQLITE
