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
	static const String ERROR_NULL_TYPE_FIELD = STR( "Null type field" );

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

	DatabaseFieldInfosPtrArray SqliteGetColumns( SQLite::Statement * pStatement, DatabaseConnectionPtr pConnexion )
	{
		DatabaseFieldInfosPtrArray arrayReturn;
		int iColumnCount = SQLite::ColumnCount( pStatement );

		for ( int i = 0; i < iColumnCount; i++ )
		{
			char const * pszName = SQLite::ColumnName( pStatement, i );

			if ( pszName )
			{
				String strColumnName = CStrUtils::ToString( pszName );
				String strLowerName = CStrUtils::LowerCase( strColumnName );
				char const * pszType = SQLite::ColumnDecltype( pStatement, i );
				int dataType = SQLite::ColumnType( pStatement, i );
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
						infos = std::make_shared< CDatabaseFieldInfos >( pConnexion, strColumnName, EFieldType_LONG_INTEGER );
					}
					else if ( type.find( "SMALLINT" ) != std::string::npos )
					{
						infos = std::make_shared< CDatabaseFieldInfos >( pConnexion, strColumnName, EFieldType_SMALL_INTEGER );
					}
					else if ( type.find( "TINYINT" ) != std::string::npos )
					{
						infos = std::make_shared< CDatabaseFieldInfos >( pConnexion, strColumnName, EFieldType_TINY_INTEGER );
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
									infos = std::make_shared< CDatabaseFieldInfos >( pConnexion, strColumnName, EFieldType_TINY_INTEGER );
								}
								else if ( prec == 2 )
								{
									infos = std::make_shared< CDatabaseFieldInfos >( pConnexion, strColumnName, EFieldType_SMALL_INTEGER );
								}
								else if ( prec <= 4 )
								{
									infos = std::make_shared< CDatabaseFieldInfos >( pConnexion, strColumnName, EFieldType_INTEGER );
								}
								else
								{
									infos = std::make_shared< CDatabaseFieldInfos >( pConnexion, strColumnName, EFieldType_LONG_INTEGER );
								}
							}
							else
							{
								infos = std::make_shared< CDatabaseFieldInfos >( pConnexion, strColumnName, EFieldType_INTEGER );
							}
						}
						else
						{
							infos = std::make_shared< CDatabaseFieldInfos >( pConnexion, strColumnName, EFieldType_INTEGER );
						}
					}

					break;

				case SQLITE_FLOAT:
					if ( type.find( "FLOA" ) != std::string::npos
							|| type == "SUM" )
					{
						infos = std::make_shared< CDatabaseFieldInfos >( pConnexion, strColumnName, EFieldType_FLOATING_POINT_SIMPLE );
					}
					else if ( type.find( "DOUB" ) != std::string::npos
							  || type.find( "REAL" ) != std::string::npos )
					{
						infos = std::make_shared< CDatabaseFieldInfos >( pConnexion, strColumnName, EFieldType_FLOATING_POINT_DOUBLE );
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
					if ( strLowerName.find( "max(" ) != std::string::npos )
					{
						arrayReturn.push_back( std::make_shared< CDatabaseFieldInfos >( pConnexion, strColumnName, "max" ) );
					}
					else if ( strLowerName.find( "min(" ) != std::string::npos )
					{
						arrayReturn.push_back( std::make_shared< CDatabaseFieldInfos >( pConnexion, strColumnName, "min" ) );
					}
					else if ( strLowerName.find( "count(" ) != std::string::npos )
					{
						arrayReturn.push_back( std::make_shared< CDatabaseFieldInfos >( pConnexion, strColumnName, "count" ) );
					}
					else if ( strLowerName.find( "sum(" ) != std::string::npos )
					{
						arrayReturn.push_back( std::make_shared< CDatabaseFieldInfos >( pConnexion, strColumnName, "sum" ) );
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
	DatabaseFieldPtr ConstructField( SQLite::Statement * pStatement, int i, DatabaseFieldInfosPtr pInfos, Value value )
	{
		int iSize = SQLite::ColumnBytes( pStatement, i );
		DatabaseFieldPtr pField;
		pField = std::make_shared< CDatabaseField >( pInfos );

		if ( iSize != 0 )
		{
			static_cast< CDatabaseValue< Type > & >( pField->GetObjectValue() ).SetValue( value );
		}

		return pField;
	}

	template< EFieldType Type > DatabaseFieldPtr GetValue( SQLite::Statement * pStatement, int i, DatabaseFieldInfosPtr pInfos );

	template<>
	DatabaseFieldPtr GetValue< EFieldType_BIT >( SQLite::Statement * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		bool value = SQLite::ColumnInt( pStatement, i ) != 0;
		return ConstructField< EFieldType_BIT >( pStatement, i, pInfos, value );
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_TINY_INTEGER >( SQLite::Statement * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		int value = SQLite::ColumnInt( pStatement, i );
		return ConstructField< EFieldType_TINY_INTEGER >( pStatement, i, pInfos, value );
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_SMALL_INTEGER >( SQLite::Statement * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		int value = SQLite::ColumnInt( pStatement, i );
		return ConstructField< EFieldType_SMALL_INTEGER >( pStatement, i, pInfos, value );
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_INTEGER >( SQLite::Statement * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		int value = SQLite::ColumnInt( pStatement, i );
		return ConstructField< EFieldType_INTEGER >( pStatement, i, pInfos, value );
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_LONG_INTEGER >( SQLite::Statement * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		int64_t value = SQLite::ColumnInt64( pStatement, i );
		return ConstructField< EFieldType_LONG_INTEGER >( pStatement, i, pInfos, value );
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_FLOATING_POINT_SIMPLE >( SQLite::Statement * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		float value = float( SQLite::ColumnDouble( pStatement, i ) );
		return ConstructField< EFieldType_FLOATING_POINT_SIMPLE >( pStatement, i, pInfos, value );
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_FLOATING_POINT_DOUBLE >( SQLite::Statement * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		double value = SQLite::ColumnDouble( pStatement, i );
		return ConstructField< EFieldType_FLOATING_POINT_DOUBLE >( pStatement, i, pInfos, value );
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_FIXED_POINT >( SQLite::Statement * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		DatabaseFieldPtr pField;
		double value = SQLite::ColumnDouble( pStatement, i );
		pField = std::make_shared< CDatabaseField >( pInfos );
		static_cast< CDatabaseValue< EFieldType_FIXED_POINT > & >( pField->GetObjectValue() ).SetValue( CFixedPoint( value, pInfos->GetPrecision().first ) );
		return pField;
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_VARCHAR >( SQLite::Statement * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		DatabaseFieldPtr pField;
		char * value = ( char * )SQLite::ColumnText( pStatement, i );
		int iSize = SQLite::ColumnBytes( pStatement, i );
		pField = std::make_shared< CDatabaseField >( pInfos );

		if ( value && iSize != 0 )
		{
			static_cast< CDatabaseValue< EFieldType_VARCHAR > & >( pField->GetObjectValue() ).SetValue( value );
		}

		return pField;
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_TEXT >( SQLite::Statement * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		DatabaseFieldPtr pField;
		char * value = ( char * )SQLite::ColumnText( pStatement, i );
		int iSize = SQLite::ColumnBytes( pStatement, i );
		pField = std::make_shared< CDatabaseField >( pInfos );

		if ( value && iSize != 0 )
		{
			static_cast< CDatabaseValue< EFieldType_TEXT > & >( pField->GetObjectValue() ).SetValue( value );
		}

		return pField;
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_DATE >( SQLite::Statement * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		DatabaseFieldPtr pField;
		char * value = ( char * )SQLite::ColumnText( pStatement, i );
		int iSize = SQLite::ColumnBytes( pStatement, i );
		pField = std::make_shared< CDatabaseField >( pInfos );

		if ( value && iSize != 0 )
		{
			static_cast< CDatabaseValue< EFieldType_DATE > & >( pField->GetObjectValue() ).SetValue( pInfos->GetConnection()->ParseDate( value ) );
		}

		return pField;
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_DATETIME >( SQLite::Statement * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		DatabaseFieldPtr pField;
		char * value = ( char * )SQLite::ColumnText( pStatement, i );
		int iSize = SQLite::ColumnBytes( pStatement, i );
		pField = std::make_shared< CDatabaseField >( pInfos );

		if ( value && iSize != 0 )
		{
			static_cast< CDatabaseValue< EFieldType_DATETIME > & >( pField->GetObjectValue() ).SetValue( pInfos->GetConnection()->ParseDateTime( value ) );
		}

		return pField;
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_TIME >( SQLite::Statement * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		DatabaseFieldPtr pField;
		char * value = ( char * )SQLite::ColumnText( pStatement, i );
		int iSize = SQLite::ColumnBytes( pStatement, i );
		pField = std::make_shared< CDatabaseField >( pInfos );

		if ( value && iSize != 0 )
		{
			static_cast< CDatabaseValue< EFieldType_TIME > & >( pField->GetObjectValue() ).SetValue( pInfos->GetConnection()->ParseTime( value ) );
		}

		return pField;
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_NVARCHAR >( SQLite::Statement * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		DatabaseFieldPtr pField;
		wchar_t * value = ( wchar_t * )SQLite::ColumnText16( pStatement, i );
		int iSize = SQLite::ColumnBytes16( pStatement, i );
		pField = std::make_shared< CDatabaseField >( pInfos );

		if ( value && iSize != 0 )
		{
			static_cast< CDatabaseValue< EFieldType_NVARCHAR > & >( pField->GetObjectValue() ).SetValue( value );
		}

		return pField;
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_NTEXT >( SQLite::Statement * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		DatabaseFieldPtr pField;
		wchar_t * value = ( wchar_t * )SQLite::ColumnText16( pStatement, i );
		int iSize = SQLite::ColumnBytes16( pStatement, i );
		pField = std::make_shared< CDatabaseField >( pInfos );

		if ( value && iSize != 0 )
		{
			static_cast< CDatabaseValue< EFieldType_NTEXT > & >( pField->GetObjectValue() ).SetValue( value );
		}

		return pField;
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_BINARY >( SQLite::Statement * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		DatabaseFieldPtr pField;
		uint8_t * value = ( uint8_t * )SQLite::ColumnBlob( pStatement, i );
		int iSize = SQLite::ColumnBytes( pStatement, i );
		pField = std::make_shared< CDatabaseField >( pInfos );

		if ( value && iSize != 0 )
		{
			static_cast< CDatabaseValue< EFieldType_BINARY > & >( pField->GetObjectValue() ).SetValue( value, std::min( uint32_t( iSize ), pInfos->GetLimits() ) );
		}

		return pField;
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_VARBINARY >( SQLite::Statement * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		DatabaseFieldPtr pField;
		uint8_t * value = ( uint8_t * )SQLite::ColumnBlob( pStatement, i );
		int iSize = SQLite::ColumnBytes( pStatement, i );
		pField = std::make_shared< CDatabaseField >( pInfos );

		if ( value && iSize != 0 )
		{
			static_cast< CDatabaseValue< EFieldType_VARBINARY > & >( pField->GetObjectValue() ).SetValue( value, std::min( uint32_t( iSize ), pInfos->GetLimits() ) );
		}

		return pField;
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_LONG_VARBINARY >( SQLite::Statement * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		DatabaseFieldPtr pField;
		uint8_t * value = ( uint8_t * )SQLite::ColumnBlob( pStatement, i );
		int iSize = SQLite::ColumnBytes( pStatement, i );
		pField = std::make_shared< CDatabaseField >( pInfos );

		if ( value && iSize != 0 )
		{
			static_cast< CDatabaseValue< EFieldType_LONG_VARBINARY > & >( pField->GetObjectValue() ).SetValue( value, std::min( uint32_t( iSize ), pInfos->GetLimits() ) );
		}

		return pField;
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_NULL >( SQLite::Statement * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		assert( pInfos->GetType() == EFieldType_NULL );
		DatabaseFieldPtr pField;
		SQLite::Value value = SQLite::ColumnValue( pStatement, i );

		switch ( value.GetType() )
		{
		case SQLITE_INTEGER:
			pInfos->SetType( EFieldType_INTEGER );
			pField = GetValue< EFieldType_INTEGER >( pStatement, i, pInfos );
			break;

		case SQLITE_FLOAT:
			pInfos->SetType( EFieldType_FLOATING_POINT_SIMPLE );
			pField = GetValue< EFieldType_FLOATING_POINT_SIMPLE >( pStatement, i, pInfos );
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
			char * value = ( char * )SQLite::ColumnText( pStatement, i );
			int iSize = SQLite::ColumnBytes( pStatement, i );

			if ( value && iSize )
			{
				pInfos->SetType( EFieldType_TEXT );
				pField = GetValue< EFieldType_TEXT >( pStatement, i, pInfos );
			}
			else
			{
				pInfos->SetType( EFieldType_INTEGER );
				pField = GetValue< EFieldType_INTEGER >( pStatement, i, pInfos );
			}
		}
		break;
		}

		return pField;
	}

	DatabaseResultPtr SqliteExecute( SQLite::Statement * pStatement, SQLite::eCODE & code, DatabaseFieldInfosPtrArray const & arrayColumns, DatabaseConnectionPtr pConnexion )
	{
		DatabaseResultPtr pReturn;
		code = SQLite::eCODE_ERROR;

		if ( pConnexion->IsConnected() )
		{
			pReturn = std::make_unique< CDatabaseResult >( pConnexion, arrayColumns );
			code = SQLite::eCODE_OK;
			int iResult;
			int iNbColumns = int( arrayColumns.size() );

			while ( ( iResult = SQLite::Step( pStatement ) ) != SQLITE_DONE )
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

						case EFieldType_TINY_INTEGER:
							pField = GetValue< EFieldType_TINY_INTEGER >( pStatement, i, pInfos );
							break;

						case EFieldType_SMALL_INTEGER:
							pField = GetValue< EFieldType_SMALL_INTEGER >( pStatement, i, pInfos );
							break;

						case EFieldType_INTEGER:
							pField = GetValue< EFieldType_INTEGER >( pStatement, i, pInfos );
							break;

						case EFieldType_LONG_INTEGER:
							pField = GetValue< EFieldType_LONG_INTEGER >( pStatement, i, pInfos );
							break;

						case EFieldType_FLOATING_POINT_SIMPLE:
							pField = GetValue< EFieldType_FLOATING_POINT_SIMPLE >( pStatement, i, pInfos );
							break;

						case EFieldType_FLOATING_POINT_DOUBLE:
							pField = GetValue< EFieldType_FLOATING_POINT_DOUBLE >( pStatement, i, pInfos );
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

	void SQLiteTry( SQLite::eCODE code, TChar const * msg, EDatabaseExceptionCodes exc, SQLite::Database * database )
	{
		if ( code != SQLite::eCODE_OK )
		{
			StringStream error;
			error << STR( "Error : " ) << msg << STR( " - " ) << CStrUtils::ToString( SQLite::Errmsg( database ) );
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

	void SQLiteTry( SQLite::eCODE code, std::ostream const & stream, EDatabaseExceptionCodes exc, SQLite::Database * database )
	{
		std::stringstream ss;
		ss << stream.rdbuf();
		SQLiteTry( code, ss.str().c_str(), exc, database );
	}
}
END_NAMESPACE_DATABASE_SQLITE
