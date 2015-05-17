/************************************************************************//**
 * @file DatabaseSqlitePch.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 03/14/2014 11:00:50 AM
 *
 *
 * @brief Database SQLITE precompiled header.
 *
 * @details This file generates the Database SQLITE precompiled headers.
 *
 ***************************************************************************/

#include "DatabaseSqlitePch.h"

BEGIN_NAMESPACE_DATABASE_SQLITE
{
	DatabaseFieldInfosPtrArray SqliteGetColumns( SQLite::Statement * pStatement, DatabaseConnectionPtr pConnexion )
	{
		DatabaseFieldInfosPtrArray arrayReturn;
		StringArray aColumns;
		String strColumnName;
		char const * pszName;
		char const * pszType;
		int iColumnCount = SQLite::ColumnCount( pStatement );

		for ( int i = 0; i < iColumnCount; i++ )
		{
			pszName = SQLite::ColumnName( pStatement, i );

			if ( pszName )
			{
				strColumnName = CStrUtils::ToString( pszName );
				aColumns.push_back( strColumnName );
				strColumnName = CStrUtils::LowerCase( strColumnName );
				pszType = SQLite::ColumnDecltype( pStatement, i );

				if ( pszType )
				{
					arrayReturn.push_back( std::make_shared< CDatabaseFieldInfos >( pConnexion, aColumns[i], CStrUtils::ToString( pszType ) ) );
				}
				else if ( strColumnName.find( "max(" ) != String::npos )
				{
					arrayReturn.push_back( std::make_shared< CDatabaseFieldInfos >( pConnexion, aColumns[i], "max" ) );
				}
				else if ( strColumnName.find( "min(" ) != String::npos )
				{
					arrayReturn.push_back( std::make_shared< CDatabaseFieldInfos >( pConnexion, aColumns[i], "min" ) );
				}
				else if ( strColumnName.find( "count(" ) != String::npos )
				{
					arrayReturn.push_back( std::make_shared< CDatabaseFieldInfos >( pConnexion, aColumns[i], "count" ) );
				}
				else if ( strColumnName.find( "sum(" ) != String::npos )
				{
					arrayReturn.push_back( std::make_shared< CDatabaseFieldInfos >( pConnexion, aColumns[i], "sum" ) );
				}
				else if ( strColumnName.find( "calculfloat" ) != String::npos )
				{
					arrayReturn.push_back( std::make_shared< CDatabaseFieldInfos >( pConnexion, aColumns[i], "FLOAT" ) );
				}
				else if ( strColumnName.find( "calculint" ) != String::npos )
				{
					arrayReturn.push_back( std::make_shared< CDatabaseFieldInfos >( pConnexion, aColumns[i], "INTEGER" ) );
				}
				else if ( strColumnName.find( "intitule" ) != String::npos )
				{
					arrayReturn.push_back( std::make_shared< CDatabaseFieldInfos >( pConnexion, aColumns[i], "TEXT" ) );
				}
				else
				{
					arrayReturn.push_back( std::make_shared< CDatabaseFieldInfos >( pConnexion, aColumns[i], "TEXT" ) );
				}
			}
		}

		return arrayReturn;
	}

	template< typename Value >
	DatabaseFieldPtr ConstructField( SQLite::Statement * pStatement, int i, DatabaseFieldInfosPtr pInfos, Value value )
	{
		int iSize = SQLite::ColumnBytes( pStatement, i );
		DatabaseFieldPtr pField;

		if ( iSize == 0 )
		{
			pField = std::make_shared< CDatabaseField >( pInfos, true, String() );
		}
		else
		{
			pField = std::make_shared< CDatabaseField >( pInfos, value );
		}

		return pField;
	}
	
	template< EFieldType Type > DatabaseFieldPtr GetValue( SQLite::Statement * pStatement, int i, DatabaseFieldInfosPtr pInfos );

	template<>
	DatabaseFieldPtr GetValue< EFieldType_BOOL >( SQLite::Statement * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		bool value = SQLite::ColumnInt( pStatement, i ) != 0;
		return ConstructField( pStatement, i, pInfos, value );
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_SMALL_INTEGER >( SQLite::Statement * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		int value = SQLite::ColumnInt( pStatement, i );
		return ConstructField( pStatement, i, pInfos, value );
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_INTEGER >( SQLite::Statement * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		int value = SQLite::ColumnInt( pStatement, i );
		return ConstructField( pStatement, i, pInfos, value );
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_LONG_INTEGER >( SQLite::Statement * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		int64_t value = SQLite::ColumnInt64( pStatement, i );
		return ConstructField( pStatement, i, pInfos, value );
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_FLOAT >( SQLite::Statement * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		float value = float( SQLite::ColumnDouble( pStatement, i ) );
		return ConstructField( pStatement, i, pInfos, value );
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_DOUBLE >( SQLite::Statement * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		double value = SQLite::ColumnDouble( pStatement, i );
		return ConstructField( pStatement, i, pInfos, value );
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_VARCHAR >( SQLite::Statement * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		DatabaseFieldPtr pField;
		char * value = ( char * )SQLite::ColumnText( pStatement, i );
		int iSize = SQLite::ColumnBytes( pStatement, i );

		if ( !value || iSize == 0 )
		{
			pField = std::make_shared< CDatabaseField >( pInfos, true, String() );
		}
		else
		{
			pField = std::make_shared< CDatabaseField >( pInfos, value );
		}

		return pField;
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_TEXT >( SQLite::Statement * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		DatabaseFieldPtr pField;
		char * value = ( char * )SQLite::ColumnText( pStatement, i );
		int iSize = SQLite::ColumnBytes( pStatement, i );

		if ( !value || iSize == 0 )
		{
			pField = std::make_shared< CDatabaseField >( pInfos, true, String() );
		}
		else
		{
			pField = std::make_shared< CDatabaseField >( pInfos, std::string( value ) );
		}

		return pField;
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_NVARCHAR >( SQLite::Statement * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		DatabaseFieldPtr pField;
		wchar_t * value = ( wchar_t * )SQLite::ColumnText16( pStatement, i );
		int iSize = SQLite::ColumnBytes( pStatement, i );

		if ( !value || iSize == 0 )
		{
			pField = std::make_shared< CDatabaseField >( pInfos, true, String() );
		}
		else
		{
			pField = std::make_shared< CDatabaseField >( pInfos, value );
		}

		return pField;
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_NTEXT >( SQLite::Statement * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		DatabaseFieldPtr pField;
		wchar_t * value = ( wchar_t * )SQLite::ColumnText16( pStatement, i );
		int iSize = SQLite::ColumnBytes( pStatement, i );

		if ( !value || iSize == 0 )
		{
			pField = std::make_shared< CDatabaseField >( pInfos, true, String() );
		}
		else
		{
			pField = std::make_shared< CDatabaseField >( pInfos, std::wstring( value ) );
		}

		return pField;
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_BINARY >( SQLite::Statement * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		DatabaseFieldPtr pField;
		uint8_t * value = ( uint8_t * )SQLite::ColumnBlob( pStatement, i );
		int iSize = SQLite::ColumnBytes( pStatement, i );

		if ( !value || iSize == 0 )
		{
			pField = std::make_shared< CDatabaseField >( pInfos, true, String() );
		}
		else
		{
			pField = std::make_shared< CDatabaseField >( pInfos, value, value + iSize );
		}

		return pField;
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_VARBINARY >( SQLite::Statement * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		DatabaseFieldPtr pField;
		uint8_t * value = ( uint8_t * )SQLite::ColumnBlob( pStatement, i );
		int iSize = SQLite::ColumnBytes( pStatement, i );

		if ( !value || iSize == 0 )
		{
			pField = std::make_shared< CDatabaseField >( pInfos, true, String() );
		}
		else
		{
			pField = std::make_shared< CDatabaseField >( pInfos, value, value + iSize );
		}

		return pField;
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_LONG_VARBINARY >( SQLite::Statement * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		DatabaseFieldPtr pField;
		uint8_t * value = ( uint8_t * )SQLite::ColumnBlob( pStatement, i );
		int iSize = SQLite::ColumnBytes( pStatement, i );

		if ( !value || iSize == 0 )
		{
			pField = std::make_shared< CDatabaseField >( pInfos, true, String() );
		}
		else
		{
			pField = std::make_shared< CDatabaseField >( pInfos, value, value + iSize );
		}

		return pField;
	}

	template<>
	DatabaseFieldPtr GetValue< EFieldType_NULL >( SQLite::Statement * pStatement, int i, DatabaseFieldInfosPtr pInfos )
	{
		DatabaseFieldPtr pField;
		char * value = ( char * )SQLite::ColumnText( pStatement, i );
		int iSize = SQLite::ColumnBytes( pStatement, i );

		if ( !value || iSize == 0 )
		{
			pField = std::make_shared< CDatabaseField >( pInfos, true, String() );
		}
		else
		{
			pField = std::make_shared< CDatabaseField >( pInfos, !value, CStrUtils::ToString( value ) );
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
						case EFieldType_BOOL:
							pField = GetValue< EFieldType_BOOL >( pStatement, i, pInfos );
							break;

						case EFieldType_INTEGER:
							pField = GetValue< EFieldType_INTEGER >( pStatement, i, pInfos );
							break;

						case EFieldType_LONG_INTEGER:
							pField = GetValue< EFieldType_LONG_INTEGER >( pStatement, i, pInfos );
							break;

						case EFieldType_FLOAT:
							pField = GetValue< EFieldType_FLOAT >( pStatement, i, pInfos );
							break;

						case EFieldType_DOUBLE:
							pField = GetValue< EFieldType_DOUBLE >( pStatement, i, pInfos );
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
}
END_NAMESPACE_DATABASE_SQLITE
