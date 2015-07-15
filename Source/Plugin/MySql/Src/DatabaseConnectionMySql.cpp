/************************************************************************//**
* @file DatabaseConnectionMySql.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 3/14/2014 5:03:15 PM
*
*
* @brief CConnectionMySql class definition.
*
* @details Describes a connection to a database via an MYSQL driver.
*
***************************************************************************/

#include "DatabaseMySqlPch.h"

#include "DatabaseConnectionMySql.h"

#include "DatabaseStatementMySql.h"
#include "ExceptionDatabaseMySql.h"

#include <DatabaseQuery.h>
#include <DatabaseDate.h>
#include <DatabaseDateTime.h>
#include <DatabaseDateTimeHelper.h>
#include <DatabaseTime.h>
#include <DatabaseFieldInfos.h>

#include <DatabaseStringUtils.h>
#include <DatabaseLogger.h>

#include <mysql.h>
#include <mysql_time.h>

BEGIN_NAMESPACE_DATABASE_MYSQL
{
	static const String ERROR_MYSQL_CONNECTION = STR( "Couldn't create the connection" );
	static const String ERROR_MYSQL_CONNECT = STR( "Couldn't initialise the connection: " );
	static const String ERROR_MYSQL_EXECUTION_ERROR = STR( "Couldn't execute the query" );
	static const String ERROR_MYSQL_UNKNOWN_ERROR = STR( "Unknown error" );
	static const String ERROR_MYSQL_NOT_CONNECTED = STR( "Not connected" );
	static const String ERROR_MYSQL_ALREADY_IN_TRANSACTION = STR( "Already in a transaction" );
	static const String ERROR_MYSQL_NOT_IN_TRANSACTION = STR( "Not in a transaction" );
	static const String ERROR_MYSQL_DRIVER = STR( "MYSQL Driver error : " );
	static const String ERROR_MYSQL_UNKNOWN = STR( "Unknown error encountered while executing query" );
	static const String ERROR_MYSQL_STMT_METADATA = STR( "Could not retrieve metadata from the statement" );

	static const String WARNING_MYSQL_UNKNOWN_OPTION = STR( "Unsupported option: " );
	static const String WARNING_MYSQL_INIT_COMMAND = STR( "MYSQL_INIT_COMMAND" );
	static const String WARNING_MYSQL_SET_CHARSET_NAME = STR( "MYSQL_SET_CHARSET_NAME" );
	static const String WARNING_MYSQL_OPT_CONNECT_TIMEOUT = STR( "MYSQL_OPT_CONNECT_TIMEOUT" );

	static const TChar * INFO_MYSQL_STATEMENT_EXECUTION = STR( "Statement execution" );
	static const TChar * INFO_MYSQL_STATEMENT_PREPARATION = STR( "Statement preparation" );
	static const TChar * INFO_MYSQL_DATABASE_SELECTION = STR( "Database selection" );
	static const String INFO_MYSQL_EXECUTING_UPDATE = STR( "Executing update: " );
	static const String INFO_MYSQL_EXECUTING_SELECT = STR( "Executing select: " );

	static const String DEBUG_MYSQL_CONNECTED = STR( "Connected to database" );
	static const String DEBUG_MYSQL_CLIENT_VERSION = STR( "Client Version: " );
	static const String DEBUG_MYSQL_SERVER_HOST = STR( "Server Host: " );
	static const String DEBUG_MYSQL_SERVER_VERSION = STR( "Server Version: " );

	static const String MYSQL_SQL_CREATE_DATABASE = STR( "CREATE DATABASE " );
	static const String MYSQL_SQL_CHARSET = STR( " CHARACTER SET utf8 COLLATE utf8_general_ci" );
	static const String MYSQL_SQL_USE = STR( "USE " );
	static const String MYSQL_SQL_DROP_DATABASE = STR( "DROP DATABASE " );
	static const String MYSQL_SQL_TRANSACTION_BEGIN = STR( "BEGIN TRANSACTION " );
	static const String MYSQL_SQL_TRANSACTION_COMMIT = STR( "COMMIT TRANSACTION " );
	static const String MYSQL_SQL_TRANSACTION_ROLLBACK = STR( "ROLLBACK TRANSACTION " );
	static const String MYSQL_SQL_NULL = STR( "NULL" );

	static const std::string MYSQL_SQL_SNULL = "NULL";
	static const std::wstring MYSQL_SQL_WNULL = L"NULL";

	static const char * MYSQL_OPTION_NAMES = "SET NAMES 'utf8'";
	static const char * MYSQL_OPTION_UTF8 = "utf8";

	static const std::string MYSQL_FORMAT_SDATE = "CAST('%Y-%m-%d' AS DATE)";
	static const std::string MYSQL_FORMAT_STIME = "CAST('%H:%M:%S' AS TIME)";
	static const std::string MYSQL_FORMAT_SDATETIME = "CAST('%Y-%m-%d %H:%M:%S' AS DATETIME)";
	static const std::string MYSQL_FORMAT_SDATETIME_DATE = "CAST('%Y-%m-%d 00:00:00' AS DATETIME)";
	static const std::string MYSQL_FORMAT_SDATETIME_TIME = "CAST('0000-00-00 %H:%M:%S' AS DATETIME)";
	static const std::string MYSQL_FORMAT_STMT_SDATE = "{-d %Y-%m-%d}";
	static const std::string MYSQL_FORMAT_STMT_STIME = "{-t %H:%M:%S}";
	static const std::string MYSQL_FORMAT_STMT_SDATETIME = "{-ts %Y-%m-%d %H:%M:%S}";

	static const std::wstring MYSQL_FORMAT_WDATE = L"CAST('%Y-%m-%d' AS DATE)";
	static const std::wstring MYSQL_FORMAT_WTIME = L"CAST('%H:%M:%S' AS TIME)";
	static const std::wstring MYSQL_FORMAT_WDATETIME = L"CAST('%Y-%m-%d %H:%M:%S' AS DATETIME)";
	static const std::wstring MYSQL_FORMAT_WDATETIME_DATE = L"CAST('%Y-%m-%d 00:00:00' AS DATETIME)";
	static const std::wstring MYSQL_FORMAT_WDATETIME_TIME = L"CAST('0000-00-00 %H:%M:%S' AS DATETIME)";
	static const std::wstring MYSQL_FORMAT_STMT_WDATE = L"{-d %Y-%m-%d}";
	static const std::wstring MYSQL_FORMAT_STMT_WTIME = L"{-t %H:%M:%S}";
	static const std::wstring MYSQL_FORMAT_STMT_WDATETIME = L"{-ts %Y-%m-%d %H:%M:%S}";

	// cf. https://dev.mysql.com/doc/refman/5.1/en/c-api-data-structures.html
	static const int MYSQL_BINARY_CHARSET = 63;

	namespace
	{
		EFieldType GetFieldType( enum_field_types sqlType, int charset, int length )
		{
			EFieldType result = EFieldType_NULL;

			switch ( sqlType )
			{
			case MYSQL_TYPE_NULL:
				result = EFieldType_NULL;
				break;

			case MYSQL_TYPE_BIT:
				result = EFieldType_BIT;
				break;

			case MYSQL_TYPE_TINY:
				if ( length > 1 )
				{
					result = EFieldType_INT8;
				}
				else
				{
					result = EFieldType_BIT;
				}

				break;

			case MYSQL_TYPE_SHORT:
				result = EFieldType_INT16;
				break;

			case MYSQL_TYPE_INT24:
				result = EFieldType_INT24;
				break;

			case MYSQL_TYPE_LONG:
				result = EFieldType_INT32;
				break;

			case MYSQL_TYPE_LONGLONG:
				result = EFieldType_INT64;
				break;

			case MYSQL_TYPE_FLOAT:
				result = EFieldType_FLOAT32;
				break;

			case MYSQL_TYPE_DOUBLE:
				result = EFieldType_FLOAT64;
				break;

			case MYSQL_TYPE_NEWDECIMAL:
				result = EFieldType_FIXED_POINT;
				break;

			case MYSQL_TYPE_VAR_STRING:
				result = EFieldType_VARCHAR;
				break;

			case MYSQL_TYPE_STRING:
				result = EFieldType_TEXT;
				break;

			case MYSQL_TYPE_DATE:
				result = EFieldType_DATE;
				break;

			case MYSQL_TYPE_DATETIME:
				result = EFieldType_DATETIME;
				break;

			case MYSQL_TYPE_TIME:
				result = EFieldType_TIME;
				break;

			case MYSQL_TYPE_BLOB:
				if ( charset == MYSQL_BINARY_CHARSET )
				{
					result = EFieldType_VARBINARY;
				}
				else
				{
					result = EFieldType_TEXT;
				}

				break;
			}

			return result;
		}

		std::unique_ptr< CInMySqlBindBase > GetInBind( enum_field_types sqlType, MYSQL_BIND & bind, std::uint32_t precision )
		{
			std::unique_ptr< CInMySqlBindBase > result;
			bind.buffer_type = sqlType;

			switch ( sqlType )
			{
			case MYSQL_TYPE_NULL:
				break;

			case MYSQL_TYPE_TINY:
				result = std::make_unique< SInMySqlBind< int8_t > >( bind );
				bind.is_unsigned = false;
				break;

			case MYSQL_TYPE_SHORT:
				result = std::make_unique< SInMySqlBind< int16_t > >( bind );
				bind.is_unsigned = false;
				break;

			case MYSQL_TYPE_INT24:
				result = std::make_unique< SInMySqlBind< int24_t > >( bind );
				bind.is_unsigned = false;
				break;

			case MYSQL_TYPE_LONG:
				result = std::make_unique< SInMySqlBind< int32_t > >( bind );
				bind.is_unsigned = false;
				break;

			case MYSQL_TYPE_LONGLONG:
				result = std::make_unique< SInMySqlBind< int64_t > >( bind );
				bind.is_unsigned = false;
				break;

			case MYSQL_TYPE_FLOAT:
				result = std::make_unique< SInMySqlBind< float > >( bind );
				bind.is_unsigned = false;
				break;

			case MYSQL_TYPE_DOUBLE:
				result = std::make_unique< SInMySqlBind< double > >( bind );
				bind.is_unsigned = false;
				break;

			case MYSQL_TYPE_NEWDECIMAL:
				result = std::make_unique< SInMySqlBind< char *, CFixedPoint > >( bind, precision );
				bind.is_unsigned = false;
				break;

			case MYSQL_TYPE_TIMESTAMP:
			case MYSQL_TYPE_DATE:
			case MYSQL_TYPE_DATETIME:
			case MYSQL_TYPE_TIME:
				result = std::make_unique< SInMySqlBind< MYSQL_TIME > >( bind );
				bind.is_unsigned = false;
				break;

			case MYSQL_TYPE_VAR_STRING:
			case MYSQL_TYPE_STRING:
			case MYSQL_TYPE_BLOB:
			case MYSQL_TYPE_TINY_BLOB:
			case MYSQL_TYPE_MEDIUM_BLOB:
			case MYSQL_TYPE_LONG_BLOB:
				result = std::make_unique< SInMySqlBind< char * > >( bind );
				bind.is_unsigned = false;
				break;
			}

			return result;
		}

		DatabaseFieldInfosPtrArray GetColumns( MYSQL_STMT * stmt, DatabaseConnectionPtr pConnexion, std::vector< std::unique_ptr< CInMySqlBindBase > > & inbinds, std::vector< MYSQL_BIND > & binds )
		{
			MYSQL_RES * data = mysql_stmt_result_metadata( stmt );

			if ( !data )
			{
				CLogger::LogError( ERROR_MYSQL_STMT_METADATA );
				throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_MYSQL_STMT_METADATA, __FUNCTION__, __FILE__, __LINE__ );
			}

			DatabaseFieldInfosPtrArray arrayReturn;
			uint32_t columnCount = mysql_num_fields( data );
			binds.resize( columnCount, { 0 } );

			for ( auto && bind : binds )
			{
				bind = { 0 };
				MYSQL_FIELD * field = mysql_fetch_field( data );
				arrayReturn.push_back( std::make_shared< CDatabaseFieldInfos >( pConnexion, CStrUtils::ToString( field->name ), GetFieldType( field->type, field->charsetnr, field->length ), field->length ) );
				inbinds.emplace_back( GetInBind( field->type, bind, field->decimals ) );
			}

			mysql_free_result( data );
			return arrayReturn;
		}

		DatabaseResultPtr Execute( MYSQL_STMT * statement, DatabaseFieldInfosPtrArray const & arrayColumns, DatabaseConnectionPtr pConnexion, std::vector< std::unique_ptr< CInMySqlBindBase > > const & inbinds, std::vector< MYSQL_BIND > & binds )
		{
			DatabaseResultPtr pReturn;

			try
			{
				if ( pConnexion->IsConnected() )
				{
					pReturn = std::make_unique< CDatabaseResult >( pConnexion, arrayColumns );
					int iNbColumns = int( arrayColumns.size() );
					mysql_stmt_bind_result( statement, binds.data() );
					int result = 0;

					while ( ( result = mysql_stmt_fetch( statement ) ) == 0 || result == MYSQL_DATA_TRUNCATED )
					{
						DatabaseRowPtr pRow = std::make_shared< CDatabaseRow >( pConnexion );
						int index = 0;

						for ( auto && bind : inbinds )
						{
							DatabaseFieldInfosPtr pInfos;

							try
							{
								pInfos = pReturn->GetFieldInfos( index++ );
							}
							catch ( CExceptionDatabase & )
							{
								throw;
							}

							DatabaseFieldPtr field = std::make_shared< CDatabaseField >( pInfos );

							if ( !bind->_null )
							{
								switch ( pInfos->GetType() )
								{
								case EFieldType_BIT:
									static_cast< CDatabaseValue< EFieldType_BIT > & >( field->GetObjectValue() ).SetValue( static_cast< SInMySqlBind< bool > const & >( *bind ).GetValue() != 0 );
									break;

								case EFieldType_INT8:
									static_cast< CDatabaseValue< EFieldType_INT8 > & >( field->GetObjectValue() ).SetValue( static_cast< SInMySqlBind< int8_t > const & >( *bind ).GetValue() );
									break;

								case EFieldType_INT16:
									static_cast< CDatabaseValue< EFieldType_INT16 > & >( field->GetObjectValue() ).SetValue( static_cast< SInMySqlBind< int16_t > const & >( *bind ).GetValue() );
									break;

								case EFieldType_INT24:
									static_cast< CDatabaseValue< EFieldType_INT24 > & >( field->GetObjectValue() ).SetValue( static_cast< SInMySqlBind< int24_t > const & >( *bind ).GetValue() );
									break;

								case EFieldType_INT32:
									static_cast< CDatabaseValue< EFieldType_INT32 > & >( field->GetObjectValue() ).SetValue( static_cast< SInMySqlBind< int32_t > const & >( *bind ).GetValue() );
									break;

								case EFieldType_INT64:
									static_cast< CDatabaseValue< EFieldType_INT64 > & >( field->GetObjectValue() ).SetValue( static_cast< SInMySqlBind< int64_t > const & >( *bind ).GetValue() );
									break;

								case EFieldType_FLOAT32:
									static_cast< CDatabaseValue< EFieldType_FLOAT32 > & >( field->GetObjectValue() ).SetValue( static_cast< SInMySqlBind< float > const & >( *bind ).GetValue() );
									break;

								case EFieldType_FLOAT64:
									static_cast< CDatabaseValue< EFieldType_FLOAT64 > & >( field->GetObjectValue() ).SetValue( static_cast< SInMySqlBind< double > const & >( *bind ).GetValue() );
									break;

								case EFieldType_FIXED_POINT:
									static_cast< CDatabaseValue< EFieldType_FIXED_POINT > & >( field->GetObjectValue() ).SetValue( static_cast< SInMySqlBind< char *, CFixedPoint > const & >( *bind ).GetValue() );
									break;

								case EFieldType_VARCHAR:
								{
									std::string value = StringFromMySqlString( bind->_bind, result != 0 );
									static_cast< CDatabaseValue< EFieldType_VARCHAR > & >( field->GetObjectValue() ).SetValue( value.c_str() );
									break;
								}

								case EFieldType_TEXT:
								{
									std::string value = StringFromMySqlString( bind->_bind, result != 0 );
									static_cast< CDatabaseValue< EFieldType_TEXT > & >( field->GetObjectValue() ).SetValue( value );
									break;
								}

								case EFieldType_NVARCHAR:
								{
									std::string value = StringFromMySqlString( bind->_bind, result != 0 );
									static_cast< CDatabaseValue< EFieldType_NVARCHAR > & >( field->GetObjectValue() ).SetValue( CStrUtils::ToWStr( value ).c_str() );
									break;
								}

								case EFieldType_NTEXT:
								{
									std::string value = StringFromMySqlString( bind->_bind, result != 0 );
									static_cast< CDatabaseValue< EFieldType_NTEXT > & >( field->GetObjectValue() ).SetValue( CStrUtils::ToWStr( value ) );
									break;
								}

								case EFieldType_DATE:
									static_cast< CDatabaseValue< EFieldType_DATE > & >( field->GetObjectValue() ).SetValue( CDateFromMySqlTime( static_cast< SInMySqlBind< MYSQL_TIME > const & >( *bind ).GetValue() ) );
									break;

								case EFieldType_DATETIME:
									static_cast< CDatabaseValue< EFieldType_DATETIME > & >( field->GetObjectValue() ).SetValue( CDateTimeFromMySqlTime( static_cast< SInMySqlBind< MYSQL_TIME > const & >( *bind ).GetValue() ) );
									break;

								case EFieldType_TIME:
									static_cast< CDatabaseValue< EFieldType_TIME > & >( field->GetObjectValue() ).SetValue( CTimeFromMySqlTime( static_cast< SInMySqlBind< MYSQL_TIME > const & >( *bind ).GetValue() ) );
									break;

								case EFieldType_BINARY:
								{
									auto value = static_cast< SInMySqlBind< uint8_t * > const & >( *bind ).GetValue();
									static_cast< CDatabaseValue< EFieldType_BINARY > & >( field->GetObjectValue() ).SetValue( ByteArray( value, value + bind->_length ) );
									break;
								}

								case EFieldType_VARBINARY:
								{
									auto value = static_cast< SInMySqlBind< uint8_t * > const & >( *bind ).GetValue();
									static_cast< CDatabaseValue< EFieldType_VARBINARY > & >( field->GetObjectValue() ).SetValue( ByteArray( value, value + bind->_length ) );
									break;
								}

								case EFieldType_LONG_VARBINARY:
								{
									auto value = static_cast< SInMySqlBind< uint8_t * > const & >( *bind ).GetValue();
									static_cast< CDatabaseValue< EFieldType_LONG_VARBINARY > & >( field->GetObjectValue() ).SetValue( ByteArray( value, value + bind->_length ) );
									break;
								}

								default:
									break;
								}
							}

							pRow->AddField( field );
						}

						pReturn->AddRow( pRow );
					}

					if ( statement )
					{
						///@remarks Consume the result set
						while ( !mysql_stmt_fetch( statement ) );
					}

					///@remarks Flush the statement
					//try
					//{
					//	bool hasResult = false;

					//	while ( hasResult = statement->getMoreResults() )
					//	{
					//		rs.reset( statement->getResultSet() );
					//		CLogger::LogDebug( "Success : Result set retrieval" );
					//	}
					//}
					//catch ( sql::SQLException & e )
					//{
					//	StringStream stream;
					//	stream << "Failure : " << "Get more results" << std::endl;
					//	stream << "    MySQL Error code : " << e.getErrorCode() << std::endl;
					//	stream << "    SQL State : " << e.getSQLState().c_str() << std::endl;
					//	stream << "    Why : " << e.what() << std::endl;
					//	CLogger::LogDebug( stream.str() );
					//}
				}
			}
			catch ( const CExceptionDatabase & e )
			{
				StringStream message;
				message << ERROR_MYSQL_DRIVER << STR( " - " )
						<< e.what();
				CLogger::LogError( message );
				throw CExceptionDatabaseMySql( EDatabaseMySqlExceptionCodes_GenericError, message.str(), __FUNCTION__, __FILE__, __LINE__ );
			}
			catch ( const std::exception & e )
			{
				StringStream message;
				message << ERROR_MYSQL_DRIVER << STR( " - " )
						<< e.what();
				CLogger::LogError( message );
				throw CExceptionDatabaseMySql( EDatabaseMySqlExceptionCodes_GenericError, message.str(), __FUNCTION__, __FILE__, __LINE__ );
			}
			catch ( ... )
			{
				StringStream message;
				message << ERROR_MYSQL_DRIVER << STR( " - " )
						<< ERROR_MYSQL_UNKNOWN;
				CLogger::LogError( message );
				throw CExceptionDatabaseMySql( EDatabaseMySqlExceptionCodes_UnknownError, message.str(), __FUNCTION__, __FILE__, __LINE__ );
			}

			return pReturn;
		}
	}

	CDatabaseConnectionMySql::CDatabaseConnectionMySql( const String & server, const String & userName, const String & password, String & connectionString )
		: CDatabaseConnection( server, userName, password )
		, _connection( NULL )
		, _statement( NULL )
	{
		DoConnect( connectionString );
	}

	CDatabaseConnectionMySql::~CDatabaseConnectionMySql()
	{
		DoDisconnect();
	}

	EErrorType CDatabaseConnectionMySql::BeginTransaction( const String & name )
	{
		EErrorType eReturn = EErrorType_ERROR;

		if ( !IsConnected() )
		{
			CLogger::LogError( ERROR_MYSQL_NOT_CONNECTED );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_MYSQL_NOT_CONNECTED, __FUNCTION__, __FILE__, __LINE__ );
		}

		if ( IsInTransaction() )
		{
			CLogger::LogError( ERROR_MYSQL_ALREADY_IN_TRANSACTION );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_MYSQL_ALREADY_IN_TRANSACTION, __FUNCTION__, __FILE__, __LINE__ );
		}

		mysql_autocommit( _connection, false );
		DoSetInTransaction( true );
		eReturn = EErrorType_NONE;
		return eReturn;
	}

	EErrorType CDatabaseConnectionMySql::Commit( const String & name )
	{
		EErrorType eReturn = EErrorType_ERROR;

		if ( !IsConnected() )
		{
			CLogger::LogError( ERROR_MYSQL_NOT_CONNECTED );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_MYSQL_NOT_CONNECTED, __FUNCTION__, __FILE__, __LINE__ );
		}

		if ( !IsInTransaction() )
		{
			CLogger::LogError( ERROR_MYSQL_NOT_IN_TRANSACTION );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_MYSQL_NOT_IN_TRANSACTION, __FUNCTION__, __FILE__, __LINE__ );
		}

		mysql_commit( _connection );
		mysql_autocommit( _connection, true );
		DoSetInTransaction( false );
		eReturn = EErrorType_NONE;
		return eReturn;
	}

	EErrorType CDatabaseConnectionMySql::RollBack( const String & name )
	{
		EErrorType eReturn = EErrorType_ERROR;

		if ( !IsConnected() )
		{
			CLogger::LogError( ERROR_MYSQL_NOT_CONNECTED );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_MYSQL_NOT_CONNECTED, __FUNCTION__, __FILE__, __LINE__ );
		}

		if ( !IsInTransaction() )
		{
			CLogger::LogError( ERROR_MYSQL_NOT_IN_TRANSACTION );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_MYSQL_NOT_IN_TRANSACTION, __FUNCTION__, __FILE__, __LINE__ );
		}

		mysql_rollback( _connection );
		mysql_autocommit( _connection, true );
		DoSetInTransaction( false );
		eReturn = EErrorType_NONE;
		return eReturn;
	}

	std::string CDatabaseConnectionMySql::WriteText( const std::string & text ) const
	{
		std::string strReturn( text );

		if ( strReturn != MYSQL_SQL_SNULL )
		{
			Replace( strReturn, "'", "''" );
			Replace( strReturn, "\\", "\\\\" );
			strReturn = "'" + strReturn + "'";
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionMySql::WriteNText( const std::wstring & text ) const
	{
		String strReturn( CStrUtils::ToString( text ) );

		if ( strReturn != MYSQL_SQL_SNULL )
		{
			Replace( strReturn, STR( "'" ), STR( "''" ) );
			Replace( strReturn, STR( "\\" ), STR( "\\\\" ) );
			strReturn = STR( "N'" ) + strReturn + STR( "'" );
		}

		return CStrUtils::ToWStr( strReturn );
	}

	String CDatabaseConnectionMySql::WriteName( const String & text ) const
	{
		return STR( "[" ) + text + STR( "]" );
	}

	std::string CDatabaseConnectionMySql::WriteDateS( const CDate & date ) const
	{
		std::string strReturn;

		if ( date.IsValid() )
		{
			strReturn = date.Format( MYSQL_FORMAT_SDATE );
		}
		else
		{
			strReturn += MYSQL_SQL_SNULL;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionMySql::WriteTimeS( const CTime & time ) const
	{
		std::string strReturn;

		if ( time.IsValid() )
		{
			strReturn = time.Format( MYSQL_FORMAT_STIME );
		}
		else
		{
			strReturn += MYSQL_SQL_SNULL;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionMySql::WriteDateTimeS( const CDateTime & dateTime ) const
	{
		std::string strReturn;

		if ( dateTime.GetYear() <= 0 )
		{
			strReturn += MYSQL_SQL_SNULL;
		}
		else
		{
			strReturn = dateTime.Format( MYSQL_FORMAT_SDATETIME );
		}

		return strReturn;
	}

	std::string CDatabaseConnectionMySql::WriteDateTimeS( const CDate & date ) const
	{
		std::string strReturn;

		if ( !date.IsValid() || date.GetYear() <= 0 )
		{
			strReturn += MYSQL_SQL_SNULL;
		}
		else
		{
			strReturn = date.Format( MYSQL_FORMAT_SDATETIME_DATE );
		}

		return strReturn;
	}

	std::string CDatabaseConnectionMySql::WriteDateTimeS( const CTime & time ) const
	{
		std::string strReturn;

		if ( !time.IsValid() )
		{
			strReturn += MYSQL_SQL_SNULL;
		}
		else
		{
			strReturn = time.Format( MYSQL_FORMAT_SDATETIME_TIME );
		}

		return strReturn;
	}

	std::string CDatabaseConnectionMySql::WriteStmtDateS( const CDate & date ) const
	{
		std::string strReturn;

		if ( date.IsValid() )
		{
			strReturn = date.Format( MYSQL_FORMAT_STMT_SDATE );
		}
		else
		{
			strReturn += MYSQL_SQL_SNULL;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionMySql::WriteStmtTimeS( const CTime & time ) const
	{
		std::string strReturn;

		if ( time.IsValid() )
		{
			strReturn = time.Format( MYSQL_FORMAT_STMT_STIME );
		}
		else
		{
			strReturn += MYSQL_SQL_SNULL;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionMySql::WriteStmtDateTimeS( const CDateTime & dateTime ) const
	{
		std::string strReturn;

		if ( dateTime.GetYear() > 0 )
		{
			strReturn = dateTime.Format( MYSQL_FORMAT_STMT_SDATETIME );
		}
		else
		{
			strReturn += MYSQL_SQL_SNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionMySql::WriteDateW( const CDate & date ) const
	{
		std::wstring strReturn;

		if ( date.IsValid() )
		{
			strReturn = date.Format( MYSQL_FORMAT_WDATE );
		}
		else
		{
			strReturn += MYSQL_SQL_WNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionMySql::WriteTimeW( const CTime & time ) const
	{
		std::wstring strReturn;

		if ( time.IsValid() )
		{
			strReturn = time.Format( MYSQL_FORMAT_WTIME );
		}
		else
		{
			strReturn += MYSQL_SQL_WNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionMySql::WriteDateTimeW( const CDateTime & dateTime ) const
	{
		std::wstring strReturn;

		if ( dateTime.GetYear() <= 0 )
		{
			strReturn += MYSQL_SQL_WNULL;
		}
		else
		{
			strReturn = dateTime.Format( MYSQL_FORMAT_WDATETIME );
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionMySql::WriteDateTimeW( const CDate & date ) const
	{
		std::wstring strReturn;

		if ( !date.IsValid() || date.GetYear() <= 0 )
		{
			strReturn += MYSQL_SQL_WNULL;
		}
		else
		{
			strReturn = date.Format( MYSQL_FORMAT_WDATETIME_DATE );
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionMySql::WriteDateTimeW( const CTime & time ) const
	{
		std::wstring strReturn;

		if ( !time.IsValid() )
		{
			strReturn += MYSQL_SQL_WNULL;
		}
		else
		{
			strReturn = time.Format( MYSQL_FORMAT_WDATETIME_TIME );
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionMySql::WriteStmtDateW( const CDate & date ) const
	{
		std::wstring strReturn;

		if ( date.IsValid() )
		{
			strReturn = date.Format( MYSQL_FORMAT_STMT_WDATE );
		}
		else
		{
			strReturn += MYSQL_SQL_WNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionMySql::WriteStmtTimeW( const CTime & time ) const
	{
		std::wstring strReturn;

		if ( time.IsValid() )
		{
			strReturn = time.Format( MYSQL_FORMAT_STMT_WTIME );
		}
		else
		{
			strReturn += MYSQL_SQL_WNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionMySql::WriteStmtDateTimeW( const CDateTime & dateTime ) const
	{
		std::wstring strReturn;

		if ( dateTime.GetYear() > 0 )
		{
			strReturn = dateTime.Format( MYSQL_FORMAT_STMT_WDATETIME );
		}
		else
		{
			strReturn += MYSQL_SQL_WNULL;
		}

		return strReturn;
	}

	String CDatabaseConnectionMySql::WriteBool( bool value ) const
	{
		return ( value ? STR( "1" ) : STR( "0" ) );
	}

	String CDatabaseConnectionMySql::WriteBool( const String & value ) const
	{
		const String lowerCaseValue = CStrUtils::LowerCase( value );
		return ( lowerCaseValue == STR( "x" ) || lowerCaseValue == STR( "oui" ) || lowerCaseValue == STR( "yes" ) || lowerCaseValue == STR( "y" ) || value == STR( "1" ) ? STR( "1" ) : STR( "0" ) );
	}

	CDate CDatabaseConnectionMySql::ParseDate( const std::string & date ) const
	{
		CDate dateObj;

		if ( !CDate::IsDate( date, MYSQL_FORMAT_SDATE, dateObj )
		&& !CDate::IsDate( date, MYSQL_FORMAT_STMT_SDATE, dateObj )
		&& !CDate::IsDate( date, "%Y%m%d", dateObj )
		&& !CDate::IsDate( date, "%Y-%m-%d", dateObj )
		&& !CDate::IsDate( date, "%d/%m/%Y", dateObj ) )
		{
			dateObj = CDate( 0, EDateMonth_UNDEF, 0 );
		}

		return dateObj;
	}

	CTime CDatabaseConnectionMySql::ParseTime( const std::string & time ) const
	{
		CTime timeObj;
		CTime::IsTime( time, timeObj );
		return timeObj;
	}

	CDateTime CDatabaseConnectionMySql::ParseDateTime( const std::string & dateTime ) const
	{
		CDateTime dateTimeObj;

		if ( !CDateTime::IsDateTime( dateTime, MYSQL_FORMAT_SDATETIME, dateTimeObj )
		&& !CDateTime::IsDateTime( dateTime, MYSQL_FORMAT_STMT_SDATETIME, dateTimeObj )
		&& !CDateTime::IsDateTime( dateTime, "%Y%m%d %H:%M:%S", dateTimeObj )
		&& !CDateTime::IsDateTime( dateTime, "%Y-%m-%d %H:%M:%S", dateTimeObj )
		&& !CDateTime::IsDateTime( dateTime, "%d/%m/%Y %H:%M:%S", dateTimeObj )
		&& !CDateTime::IsDateTime( dateTime, MYSQL_FORMAT_SDATE, dateTimeObj )
		&& !CDateTime::IsDateTime( dateTime, MYSQL_FORMAT_STMT_SDATE, dateTimeObj )
		&& !CDateTime::IsDateTime( dateTime, "%Y%m%d", dateTimeObj )
		&& !CDateTime::IsDateTime( dateTime, "%Y-%m-%d", dateTimeObj )
		&& !CDateTime::IsDateTime( dateTime, "%d/%m/%Y", dateTimeObj ) )
		{
			dateTimeObj = CDateTime();
		}

		return dateTimeObj;
	}

	CDate CDatabaseConnectionMySql::ParseDate( const std::wstring & date ) const
	{
		CDate dateObj;

		if ( !CDate::IsDate( date, MYSQL_FORMAT_WDATE, dateObj )
		&& !CDate::IsDate( date, MYSQL_FORMAT_STMT_WDATE, dateObj )
		&& !CDate::IsDate( date, L"%Y%m%d", dateObj )
		&& !CDate::IsDate( date, L"%Y-%m-%d", dateObj )
		&& !CDate::IsDate( date, L"%d/%m/%Y", dateObj ) )
		{
			dateObj = CDate( 0, EDateMonth_UNDEF, 0 );
		}

		return dateObj;
	}

	CTime CDatabaseConnectionMySql::ParseTime( const std::wstring & time ) const
	{
		CTime timeObj;
		CTime::IsTime( time, timeObj );
		return timeObj;
	}

	CDateTime CDatabaseConnectionMySql::ParseDateTime( const std::wstring & dateTime ) const
	{
		CDateTime dateTimeObj;

		if ( !CDateTime::IsDateTime( dateTime, MYSQL_FORMAT_WDATETIME, dateTimeObj )
		&& !CDateTime::IsDateTime( dateTime, MYSQL_FORMAT_STMT_WDATETIME, dateTimeObj )
		&& !CDateTime::IsDateTime( dateTime, L"%Y%m%d %H:%M:%S", dateTimeObj )
		&& !CDateTime::IsDateTime( dateTime, L"%Y-%m-%d %H:%M:%S", dateTimeObj )
		&& !CDateTime::IsDateTime( dateTime, L"%d/%m/%Y %H:%M:%S", dateTimeObj )
		&& !CDateTime::IsDateTime( dateTime, MYSQL_FORMAT_WDATE, dateTimeObj )
		&& !CDateTime::IsDateTime( dateTime, MYSQL_FORMAT_STMT_WDATE, dateTimeObj )
		&& !CDateTime::IsDateTime( dateTime, L"%Y%m%d", dateTimeObj )
		&& !CDateTime::IsDateTime( dateTime, L"%Y-%m-%d", dateTimeObj )
		&& !CDateTime::IsDateTime( dateTime, L"%d/%m/%Y", dateTimeObj ) )
		{
			dateTimeObj = CDateTime();
		}

		return dateTimeObj;
	}

	unsigned long CDatabaseConnectionMySql::GetStmtDateSize()const
	{
		return ( unsigned long )sizeof( MYSQL_TIME );
	}

	unsigned long CDatabaseConnectionMySql::GetStmtDateTimeSize()const
	{
		return ( unsigned long )sizeof( MYSQL_TIME );
	}

	unsigned long CDatabaseConnectionMySql::GetStmtTimeSize()const
	{
		return ( unsigned long )sizeof( MYSQL_TIME );
	}

	MYSQL * CDatabaseConnectionMySql::GetConnection() const
	{
		return _connection;
	}

	bool CDatabaseConnectionMySql::ExecuteUpdate( MYSQL_STMT * statement )
	{
		bool result = false;

		try
		{
			MySQLTry( mysql_stmt_execute( statement ), INFO_MYSQL_STATEMENT_EXECUTION, EDatabaseExceptionCodes_StatementError, _connection );
			uint64_t affected = mysql_stmt_affected_rows( statement );
			result = true;
		}
		catch ( CExceptionDatabase & exc )
		{
			StringStream stream;
			stream << ERROR_MYSQL_EXECUTION_ERROR << STR( " - " ) << exc.GetFullDescription();
			CLogger::LogError( stream );
		}
		catch ( std::exception & exc )
		{
			StringStream stream;
			stream << ERROR_MYSQL_EXECUTION_ERROR << STR( " - " ) << exc.what();
			CLogger::LogError( stream );
		}

		return result;
	}

	DatabaseResultPtr CDatabaseConnectionMySql::ExecuteSelect( MYSQL_STMT * statement )
	{
		DatabaseResultPtr result;

		try
		{
			bool hasResults = false;
			MySQLTry( mysql_stmt_execute( statement ), INFO_MYSQL_STATEMENT_EXECUTION, EDatabaseExceptionCodes_StatementError, _connection );
			result = DoRetrieveResults( statement );
		}
		catch ( CExceptionDatabase & exc )
		{
			StringStream stream;
			stream << ERROR_MYSQL_EXECUTION_ERROR << STR( " - " ) << exc.GetFullDescription();
			CLogger::LogError( stream );
		}
		catch ( std::exception & exc )
		{
			StringStream stream;
			stream << ERROR_MYSQL_EXECUTION_ERROR << STR( " - " ) << exc.what();
			CLogger::LogError( stream );
		}

		return result;
	}

	void CDatabaseConnectionMySql::CreateDatabase( const String & database )
	{
		if ( !IsConnected() )
		{
			CLogger::LogError( ERROR_MYSQL_NOT_CONNECTED );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_MYSQL_NOT_CONNECTED, __FUNCTION__, __FILE__, __LINE__ );
		}

		DoExecuteUpdate( MYSQL_SQL_CREATE_DATABASE + database + MYSQL_SQL_CHARSET, NULL );
	}

	void CDatabaseConnectionMySql::SelectDatabase( const String & database )
	{
		if ( !IsConnected() )
		{
			CLogger::LogError( ERROR_MYSQL_NOT_CONNECTED );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_MYSQL_NOT_CONNECTED, __FUNCTION__, __FILE__, __LINE__ );
		}

		if ( _connection )
		{
			std::string query = CStrUtils::ToStr( MYSQL_SQL_USE + database );
			MySQLTry( mysql_real_query( _connection, query.c_str(), uint32_t( query.size() ) ), INFO_MYSQL_DATABASE_SELECTION, EDatabaseExceptionCodes_ConnectionError, _connection );
			_database = database;
		}
	}

	void CDatabaseConnectionMySql::DestroyDatabase( const String & database )
	{
		if ( !IsConnected() )
		{
			CLogger::LogError( ERROR_MYSQL_NOT_CONNECTED );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_MYSQL_NOT_CONNECTED, __FUNCTION__, __FILE__, __LINE__ );
		}

		DoExecuteUpdate( MYSQL_SQL_DROP_DATABASE + database, NULL );
	}

	EErrorType CDatabaseConnectionMySql::DoConnect( String & connectionString )
	{
		EErrorType ret = EErrorType_NONE;

		try
		{
			_connection = mysql_init( NULL );

			if ( !_connection )
			{
				CLogger::LogError( ERROR_MYSQL_CONNECTION );
				throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_MYSQL_CONNECTION, __FUNCTION__, __FILE__, __LINE__ );
			}

			if ( mysql_options( _connection, MYSQL_INIT_COMMAND, MYSQL_OPTION_NAMES ) )
			{
				CLogger::LogWarning( WARNING_MYSQL_UNKNOWN_OPTION + WARNING_MYSQL_INIT_COMMAND );
			}

			if ( mysql_options( _connection, MYSQL_SET_CHARSET_NAME, MYSQL_OPTION_UTF8 ) )
			{
				CLogger::LogWarning( WARNING_MYSQL_UNKNOWN_OPTION + WARNING_MYSQL_SET_CHARSET_NAME );
			}

			uint32_t timeout = 180;

			if ( mysql_options( _connection, MYSQL_OPT_CONNECT_TIMEOUT, &timeout ) )
			{
				CLogger::LogWarning( WARNING_MYSQL_UNKNOWN_OPTION + WARNING_MYSQL_OPT_CONNECT_TIMEOUT );
			}

			if ( !mysql_real_connect( _connection,
									  CStrUtils::ToStr( _server ).c_str(),
									  CStrUtils::ToStr( _userName ).c_str(),
									  CStrUtils::ToStr( _password ).c_str(),
									  NULL,
									  0,
									  NULL,
									  CLIENT_REMEMBER_OPTIONS | CLIENT_MULTI_RESULTS ) )
			{
				StringStream error( ERROR_MYSQL_CONNECT );
				error << mysql_error( _connection );
				CLogger::LogError( error );
				throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, error.str(), __FUNCTION__, __FILE__, __LINE__ );
			}

			CLogger::LogDebug( StringStream() << DEBUG_MYSQL_CONNECTED );
			CLogger::LogDebug( StringStream() << DEBUG_MYSQL_CLIENT_VERSION << mysql_get_client_info() );
			CLogger::LogDebug( StringStream() << DEBUG_MYSQL_SERVER_HOST << mysql_get_host_info( _connection ) );
			CLogger::LogDebug( StringStream() << DEBUG_MYSQL_SERVER_VERSION << mysql_get_server_info( _connection ) );

			_statement = mysql_stmt_init( _connection );

			DoSetConnected( true );
		}
		catch ( CExceptionDatabase & exc )
		{
			CLogger::LogError( StringStream() << ERROR_MYSQL_CONNECTION << STR( " - " ) << exc.GetFullDescription() );
			ret = EErrorType_ERROR;
		}
		catch ( std::exception & exc )
		{
			CLogger::LogError( StringStream() << ERROR_MYSQL_CONNECTION << STR( " - " ) << exc.what() );
			ret = EErrorType_ERROR;
		}
		catch ( ... )
		{
			CLogger::LogError( ERROR_MYSQL_CONNECTION );
			ret = EErrorType_ERROR;
		}

		return ret;
	}

	void CDatabaseConnectionMySql::DoDisconnect()
	{
		DoSetConnected( false );

		if ( _statement )
		{
			mysql_stmt_close( _statement );
			_statement = NULL;
		}

		if ( _connection )
		{
			mysql_close( _connection );
			_connection = NULL;
		}
	}

	bool CDatabaseConnectionMySql::DoExecuteUpdate( const String & query, EErrorType * result )
	{
		if ( !IsConnected() )
		{
			CLogger::LogError( ERROR_MYSQL_NOT_CONNECTED );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_MYSQL_NOT_CONNECTED, __FUNCTION__, __FILE__, __LINE__ );
		}

		CLogger::LogInfo( INFO_MYSQL_EXECUTING_UPDATE + query );
		std::string strQuery = CStrUtils::ToStr( query );
		MySQLTry( mysql_stmt_prepare( _statement, strQuery.c_str(), uint32_t( strQuery.size() ) ), INFO_MYSQL_STATEMENT_PREPARATION, EDatabaseExceptionCodes_StatementError, _connection );
		return ExecuteUpdate( _statement );
	}

	DatabaseResultPtr CDatabaseConnectionMySql::DoExecuteSelect( const String & query, EErrorType * result )
	{
		if ( !IsConnected() )
		{
			CLogger::LogError( ERROR_MYSQL_NOT_CONNECTED );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_MYSQL_NOT_CONNECTED, __FUNCTION__, __FILE__, __LINE__ );
		}

		CLogger::LogInfo( INFO_MYSQL_EXECUTING_SELECT + query );
		std::string strQuery = CStrUtils::ToStr( query );
		MySQLTry( mysql_stmt_prepare( _statement, strQuery.c_str(), uint32_t( strQuery.size() ) ), INFO_MYSQL_STATEMENT_PREPARATION, EDatabaseExceptionCodes_StatementError, _connection );
		return ExecuteSelect( _statement );
	}

	DatabaseStatementPtr CDatabaseConnectionMySql::DoCreateStatement( const String & request, EErrorType * result )
	{
		DatabaseStatementPtr pReturn;
		EErrorType eResult = EErrorType_ERROR;

		if ( !IsConnected() )
		{
			CLogger::LogError( ERROR_MYSQL_NOT_CONNECTED );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_MYSQL_NOT_CONNECTED, __FUNCTION__, __FILE__, __LINE__ );
		}

		pReturn = std::make_shared< CDatabaseStatementMySql >( shared_from_this(), request );
		eResult = EErrorType_NONE;

		if ( result )
		{
			*result = eResult;
		}

		return pReturn;
	}

	DatabaseQueryPtr CDatabaseConnectionMySql::DoCreateQuery( const String & request, EErrorType * result )
	{
		DatabaseQueryPtr pReturn;
		EErrorType eResult = EErrorType_ERROR;

		if ( !IsConnected() )
		{
			CLogger::LogError( ERROR_MYSQL_NOT_CONNECTED );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_MYSQL_NOT_CONNECTED, __FUNCTION__, __FILE__, __LINE__ );
		}

		pReturn = std::make_shared< CDatabaseQuery >( shared_from_this(), request );
		eResult = EErrorType_NONE;

		if ( result )
		{
			*result = eResult;
		}

		return pReturn;
	}

	DatabaseResultPtr CDatabaseConnectionMySql::DoRetrieveResults( MYSQL_STMT * statement )
	{
		DatabaseResultPtr pResult;

		try
		{
			if ( statement )
			{
				DatabaseConnectionPtr connection = shared_from_this();
				std::vector< MYSQL_BIND > binds;
				std::vector< std::unique_ptr< CInMySqlBindBase > > inbinds;
				DatabaseFieldInfosPtrArray columns = GetColumns( statement, connection, inbinds, binds );
				pResult = Execute( statement, columns, connection, inbinds, binds );
			}
		}
		catch ( CExceptionDatabase & exc )
		{
			StringStream stream;
			stream << ERROR_MYSQL_EXECUTION_ERROR << exc.GetFullDescription();
			CLogger::LogError( stream.str() );
		}
		catch ( std::exception & exc )
		{
			StringStream stream;
			stream << ERROR_MYSQL_EXECUTION_ERROR << exc.what();
			CLogger::LogError( stream.str() );
		}

		return pResult;
	}
}
END_NAMESPACE_DATABASE_MYSQL
