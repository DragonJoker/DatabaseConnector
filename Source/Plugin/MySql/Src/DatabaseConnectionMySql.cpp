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

	static const String MYSQL_TRANSACTION_BEGIN = STR( "BEGIN TRANSACTION " );
	static const String MYSQL_TRANSACTION_COMMIT = STR( "COMMIT TRANSACTION " );
	static const String MYSQL_TRANSACTION_ROLLBACK = STR( "ROLLBACK TRANSACTION " );

	static const String WARNING_MYSQL_UNKNOWN_OPTION = STR( "Unsupported option: " );

	static const std::string MYSQL_NULL_STDSTRING = "NULL";
	static const std::wstring MYSQL_NULL_STDWSTRING = L"NULL";
	static const String MYSQL_NULL_STRING = STR( "NULL" );

	static const std::string MYSQL_FORMAT_STMT_DATE = "{-d %Y-%m-%d}";
	static const std::string MYSQL_FORMAT_STMT_DATETIME = "{-ts %Y-%m-%d %H:%M:%S}";
	static const std::string MYSQL_FORMAT_STMT_TIME = "{-t %H:%M:%S}";
	static const std::string MYSQL_FORMAT_DATE = "CAST('%Y-%m-%d' AS DATE)";
	static const std::string MYSQL_FORMAT_TIME = "CAST('%H:%M:%S' AS TIME)";
	static const std::string MYSQL_FORMAT_DATETIME = "CAST('%Y-%m-%d %H:%M:%S' AS DATETIME)";
	static const std::string MYSQL_FORMAT_DATETIME_DATE = "CAST('%Y-%m-%d 00:00:00' AS DATETIME)";
	static const std::string MYSQL_FORMAT_DATETIME_TIME = "CAST('0000-00-00 %H:%M:%S' AS DATETIME)";

	static const std::wstring WMYSQL_FORMAT_STMT_DATE = L"{-d %Y-%m-%d}";
	static const std::wstring WMYSQL_FORMAT_STMT_DATETIME = L"{-ts %Y-%m-%d %H:%M:%S}";
	static const std::wstring WMYSQL_FORMAT_STMT_TIME = L"{-t %H:%M:%S}";
	static const std::wstring WMYSQL_FORMAT_DATE = L"CAST('%Y-%m-%d' AS DATE)";
	static const std::wstring WMYSQL_FORMAT_TIME = L"CAST('%H:%M:%S' AS TIME)";
	static const std::wstring WMYSQL_FORMAT_DATETIME = L"CAST('%Y-%m-%d %H:%M:%S' AS DATETIME)";
	static const std::wstring WMYSQL_FORMAT_DATETIME_DATE = L"CAST('%Y-%m-%d 00:00:00' AS DATETIME)";
	static const std::wstring WMYSQL_FORMAT_DATETIME_TIME = L"CAST('0000-00-00 %H:%M:%S' AS DATETIME)";

	// cf. https://dev.mysql.com/doc/refman/5.1/en/c-api-data-structures.html
	static const int MYSQL_BINARY_CHARSET = 63;

	namespace
	{
		EFieldType GetFieldType( enum_field_types sqlType, int charset )
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
				result = EFieldType_TINY_INTEGER;
				break;

			case MYSQL_TYPE_SHORT:
				result = EFieldType_SMALL_INTEGER;
				break;

			case MYSQL_TYPE_INT24:
			case MYSQL_TYPE_LONG:
				result = EFieldType_INTEGER;
				break;

			case MYSQL_TYPE_LONGLONG:
				result = EFieldType_LONG_INTEGER;
				break;

			case MYSQL_TYPE_FLOAT:
				result = EFieldType_FLOATING_POINT_SIMPLE;
				break;

			case MYSQL_TYPE_DOUBLE:
				result = EFieldType_FLOATING_POINT_DOUBLE;
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
				result = std::make_unique< CInMySqlBind< int8_t > >( bind );
				bind.is_unsigned = false;
				break;

			case MYSQL_TYPE_SHORT:
				result = std::make_unique< CInMySqlBind< int16_t > >( bind );
				bind.is_unsigned = false;
				break;

			case MYSQL_TYPE_INT24:
			case MYSQL_TYPE_LONG:
				result = std::make_unique< CInMySqlBind< int32_t > >( bind );
				bind.is_unsigned = false;
				break;

			case MYSQL_TYPE_LONGLONG:
				result = std::make_unique< CInMySqlBind< int64_t > >( bind );
				bind.is_unsigned = false;
				break;

			case MYSQL_TYPE_FLOAT:
				result = std::make_unique< CInMySqlBind< float > >( bind );
				bind.is_unsigned = false;
				break;

			case MYSQL_TYPE_DOUBLE:
				result = std::make_unique< CInMySqlBind< double > >( bind );
				bind.is_unsigned = false;
				break;

			case MYSQL_TYPE_NEWDECIMAL:
				result = std::make_unique< CInMySqlBind< char *, CFixedPoint > >( bind, precision );
				bind.is_unsigned = false;
				break;

			case MYSQL_TYPE_TIMESTAMP:
			case MYSQL_TYPE_DATE:
			case MYSQL_TYPE_DATETIME:
			case MYSQL_TYPE_TIME:
				result = std::make_unique< CInMySqlBind< MYSQL_TIME > >( bind );
				bind.is_unsigned = false;
				break;

			case MYSQL_TYPE_VAR_STRING:
			case MYSQL_TYPE_STRING:
			case MYSQL_TYPE_BLOB:
			case MYSQL_TYPE_TINY_BLOB:
			case MYSQL_TYPE_MEDIUM_BLOB:
			case MYSQL_TYPE_LONG_BLOB:
				result = std::make_unique< CInMySqlBind< char * > >( bind );
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
				arrayReturn.push_back( std::make_shared< CDatabaseFieldInfos >( pConnexion, CStrUtils::ToString( field->name ), GetFieldType( field->type, field->charsetnr ), field->length ) );
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
									static_cast< CDatabaseValue< EFieldType_BIT > & >( field->GetObjectValue() ).SetValue( static_cast< CInMySqlBind< bool > const & >( *bind ).GetValue() != 0 );
									break;

								case EFieldType_TINY_INTEGER:
									static_cast< CDatabaseValue< EFieldType_TINY_INTEGER > & >( field->GetObjectValue() ).SetValue( static_cast< CInMySqlBind< int8_t > const & >( *bind ).GetValue() );
									break;

								case EFieldType_SMALL_INTEGER:
									static_cast< CDatabaseValue< EFieldType_SMALL_INTEGER > & >( field->GetObjectValue() ).SetValue( static_cast< CInMySqlBind< int16_t > const & >( *bind ).GetValue() );
									break;

								case EFieldType_INTEGER:
									static_cast< CDatabaseValue< EFieldType_INTEGER > & >( field->GetObjectValue() ).SetValue( static_cast< CInMySqlBind< int32_t > const & >( *bind ).GetValue() );
									break;

								case EFieldType_LONG_INTEGER:
									static_cast< CDatabaseValue< EFieldType_LONG_INTEGER > & >( field->GetObjectValue() ).SetValue( static_cast< CInMySqlBind< int64_t > const & >( *bind ).GetValue() );
									break;

								case EFieldType_FLOATING_POINT_SIMPLE:
									static_cast< CDatabaseValue< EFieldType_FLOATING_POINT_SIMPLE > & >( field->GetObjectValue() ).SetValue( static_cast< CInMySqlBind< float > const & >( *bind ).GetValue() );
									break;

								case EFieldType_FLOATING_POINT_DOUBLE:
									static_cast< CDatabaseValue< EFieldType_FLOATING_POINT_DOUBLE > & >( field->GetObjectValue() ).SetValue( static_cast< CInMySqlBind< double > const & >( *bind ).GetValue() );
									break;

								case EFieldType_FIXED_POINT:
									static_cast< CDatabaseValue< EFieldType_FIXED_POINT > & >( field->GetObjectValue() ).SetValue( static_cast< CInMySqlBind< char *, CFixedPoint > const & >( *bind ).GetValue() );
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
									static_cast< CDatabaseValue< EFieldType_DATE > & >( field->GetObjectValue() ).SetValue( CDateFromMySqlTime( static_cast< CInMySqlBind< MYSQL_TIME > const & >( *bind ).GetValue() ) );
									break;

								case EFieldType_DATETIME:
									static_cast< CDatabaseValue< EFieldType_DATETIME > & >( field->GetObjectValue() ).SetValue( CDateTimeFromMySqlTime( static_cast< CInMySqlBind< MYSQL_TIME > const & >( *bind ).GetValue() ) );
									break;

								case EFieldType_TIME:
									static_cast< CDatabaseValue< EFieldType_TIME > & >( field->GetObjectValue() ).SetValue( CTimeFromMySqlTime( static_cast< CInMySqlBind< MYSQL_TIME > const & >( *bind ).GetValue() ) );
									break;

								case EFieldType_BINARY:
								{
									auto value = static_cast< CInMySqlBind< uint8_t * > const & >( *bind ).GetValue();
									static_cast< CDatabaseValue< EFieldType_BINARY > & >( field->GetObjectValue() ).SetValue( ByteArray( value, value + bind->_length ) );
									break;
								}

								case EFieldType_VARBINARY:
								{
									auto value = static_cast< CInMySqlBind< uint8_t * > const & >( *bind ).GetValue();
									static_cast< CDatabaseValue< EFieldType_VARBINARY > & >( field->GetObjectValue() ).SetValue( ByteArray( value, value + bind->_length ) );
									break;
								}

								case EFieldType_LONG_VARBINARY:
								{
									auto value = static_cast< CInMySqlBind< uint8_t * > const & >( *bind ).GetValue();
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

		if ( strReturn != MYSQL_NULL_STDSTRING )
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

		if ( strReturn != MYSQL_NULL_STRING )
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

	std::string CDatabaseConnectionMySql::WriteDate( const CDate & date ) const
	{
		std::string strReturn;

		if ( date.IsValid() )
		{
			strReturn = date.Format( MYSQL_FORMAT_DATE );
		}
		else
		{
			strReturn += MYSQL_NULL_STDSTRING;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionMySql::WriteTime( const CTime & time ) const
	{
		std::string strReturn;

		if ( time.IsValid() )
		{
			strReturn = time.Format( MYSQL_FORMAT_TIME );
		}
		else
		{
			strReturn += MYSQL_NULL_STDSTRING;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionMySql::WriteDateTime( const CDateTime & dateTime ) const
	{
		std::string strReturn;

		if ( dateTime.GetYear() <= 0 )
		{
			strReturn += MYSQL_NULL_STDSTRING;
		}
		else
		{
			strReturn = dateTime.Format( MYSQL_FORMAT_DATETIME );
		}

		return strReturn;
	}

	std::string CDatabaseConnectionMySql::WriteDateTime( const CDate & date ) const
	{
		std::string strReturn;

		if ( !date.IsValid() || date.GetYear() <= 0 )
		{
			strReturn += MYSQL_NULL_STDSTRING;
		}
		else
		{
			strReturn = date.Format( MYSQL_FORMAT_DATETIME_DATE );
		}

		return strReturn;
	}

	std::string CDatabaseConnectionMySql::WriteDateTime( const CTime & time ) const
	{
		std::string strReturn;

		if ( !time.IsValid() )
		{
			strReturn += MYSQL_NULL_STDSTRING;
		}
		else
		{
			strReturn = time.Format( MYSQL_FORMAT_DATETIME_TIME );
		}

		return strReturn;
	}

	std::string CDatabaseConnectionMySql::WriteStmtDate( const CDate & date ) const
	{
		std::string strReturn;

		if ( date.IsValid() )
		{
			strReturn = date.Format( MYSQL_FORMAT_STMT_DATE );
		}
		else
		{
			strReturn += MYSQL_NULL_STDSTRING;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionMySql::WriteStmtTime( const CTime & time ) const
	{
		std::string strReturn;

		if ( time.IsValid() )
		{
			strReturn = time.Format( MYSQL_FORMAT_STMT_TIME );
		}
		else
		{
			strReturn += MYSQL_NULL_STDSTRING;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionMySql::WriteStmtDateTime( const CDateTime & dateTime ) const
	{
		std::string strReturn;

		if ( dateTime.GetYear() > 0 )
		{
			strReturn = dateTime.Format( MYSQL_FORMAT_STMT_DATETIME );
		}
		else
		{
			strReturn += MYSQL_NULL_STDSTRING;
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

		if ( !CDate::IsDate( date, MYSQL_FORMAT_DATE, dateObj )
		&& !CDate::IsDate( date, MYSQL_FORMAT_STMT_DATE, dateObj )
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

		if ( !CDateTime::IsDateTime( dateTime, MYSQL_FORMAT_DATETIME, dateTimeObj )
		&& !CDateTime::IsDateTime( dateTime, MYSQL_FORMAT_STMT_DATETIME, dateTimeObj )
		&& !CDateTime::IsDateTime( dateTime, "%Y%m%d %H:%M:%S", dateTimeObj )
		&& !CDateTime::IsDateTime( dateTime, "%Y-%m-%d %H:%M:%S", dateTimeObj )
		&& !CDateTime::IsDateTime( dateTime, "%d/%m/%Y %H:%M:%S", dateTimeObj )
		&& !CDateTime::IsDateTime( dateTime, MYSQL_FORMAT_DATE, dateTimeObj )
		&& !CDateTime::IsDateTime( dateTime, MYSQL_FORMAT_STMT_DATE, dateTimeObj )
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

		if ( !CDate::IsDate( date, WMYSQL_FORMAT_DATE, dateObj )
		&& !CDate::IsDate( date, WMYSQL_FORMAT_STMT_DATE, dateObj )
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

		if ( !CDateTime::IsDateTime( dateTime, WMYSQL_FORMAT_DATETIME, dateTimeObj )
		&& !CDateTime::IsDateTime( dateTime, WMYSQL_FORMAT_STMT_DATETIME, dateTimeObj )
		&& !CDateTime::IsDateTime( dateTime, L"%Y%m%d %H:%M:%S", dateTimeObj )
		&& !CDateTime::IsDateTime( dateTime, L"%Y-%m-%d %H:%M:%S", dateTimeObj )
		&& !CDateTime::IsDateTime( dateTime, L"%d/%m/%Y %H:%M:%S", dateTimeObj )
		&& !CDateTime::IsDateTime( dateTime, WMYSQL_FORMAT_DATE, dateTimeObj )
		&& !CDateTime::IsDateTime( dateTime, WMYSQL_FORMAT_STMT_DATE, dateTimeObj )
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
			MySQLTry( mysql_stmt_execute( statement ), STR( "Statement execution" ), EDatabaseExceptionCodes_StatementError, _connection );
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
			MySQLTry( mysql_stmt_execute( statement ), STR( "Statement execution" ), EDatabaseExceptionCodes_StatementError, _connection );
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

		DoExecuteUpdate( STR( "CREATE DATABASE " ) + database + STR( " CHARACTER SET utf8 COLLATE utf8_general_ci" ), NULL );
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
			std::string query = CStrUtils::ToStr( STR( "USE " ) + database );
			MySQLTry( mysql_real_query( _connection, query.c_str(), uint32_t( query.size() ) ), STR( "Database selection" ), EDatabaseExceptionCodes_ConnectionError, _connection );
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

		DoExecuteUpdate( STR( "DROP DATABASE " ) + database, NULL );
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

			if ( mysql_options( _connection, MYSQL_INIT_COMMAND, "SET NAMES 'utf8'" ) )
			{
				CLogger::LogWarning( WARNING_MYSQL_UNKNOWN_OPTION + STR( "MYSQL_INIT_COMMAND" ) );
			}

			if ( mysql_options( _connection, MYSQL_SET_CHARSET_NAME, "utf8" ) )
			{
				CLogger::LogWarning( WARNING_MYSQL_UNKNOWN_OPTION + STR( "MYSQL_SET_CHARSET_NAME" ) );
			}

			uint32_t timeout = 180;

			if ( mysql_options( _connection, MYSQL_OPT_CONNECT_TIMEOUT, &timeout ) )
			{
				CLogger::LogWarning( WARNING_MYSQL_UNKNOWN_OPTION + STR( "MYSQL_OPT_CONNECT_TIMEOUT" ) );
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

			CLogger::LogDebug( StringStream() << STR( "Connected to database" ) );
			CLogger::LogDebug( StringStream() << STR( "Client Version: " ) << mysql_get_client_info() );
			CLogger::LogDebug( StringStream() << STR( "Server Host: " ) << mysql_get_host_info( _connection ) );
			CLogger::LogDebug( StringStream() << STR( "Server version: " ) << mysql_get_server_info( _connection ) );

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

		CLogger::LogMessage( STR( "Executing update : " ) + query );
		std::string strQuery = CStrUtils::ToStr( query );
		MySQLTry( mysql_stmt_prepare( _statement, strQuery.c_str(), uint32_t( strQuery.size() ) ), STR( "Statement preparation" ), EDatabaseExceptionCodes_StatementError, _connection );
		return ExecuteUpdate( _statement );
	}

	DatabaseResultPtr CDatabaseConnectionMySql::DoExecuteSelect( const String & query, EErrorType * result )
	{
		if ( !IsConnected() )
		{
			CLogger::LogError( ERROR_MYSQL_NOT_CONNECTED );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_MYSQL_NOT_CONNECTED, __FUNCTION__, __FILE__, __LINE__ );
		}

		CLogger::LogMessage( STR( "Executing select : " ) + query );
		std::string strQuery = CStrUtils::ToStr( query );
		MySQLTry( mysql_stmt_prepare( _statement, strQuery.c_str(), uint32_t( strQuery.size() ) ), STR( "Statement preparation" ), EDatabaseExceptionCodes_StatementError, _connection );
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
