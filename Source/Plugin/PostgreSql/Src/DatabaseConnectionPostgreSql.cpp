/************************************************************************//**
* @file DatabaseConnectionPostgreSql.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 3/14/2014 5:03:15 PM
*
*
* @brief CConnectionPostgreSql class definition.
*
* @details Describes a connection to a database via an MYSQL driver.
*
***************************************************************************/

#include "DatabasePostgreSqlPch.h"

#include "DatabaseConnectionPostgreSql.h"

#include "DatabaseStatementPostgreSql.h"
#include "ExceptionDatabasePostgreSql.h"
#include "DatabasePostgreSqlHelper.h"

#include <DatabaseDate.h>
#include <DatabaseDateTime.h>
#include <DatabaseDateTimeHelper.h>
#include <DatabaseTime.h>
#include <DatabaseFieldInfos.h>

#include <DatabaseStringUtils.h>
#include <DatabaseLogger.h>

#include <postgres.h>
#include <libpq-fe.h>
#include <pgtypes_date.h>
#include <pgtypes_timestamp.h>
#include <catalog/pg_type.h>

BEGIN_NAMESPACE_DATABASE_POSTGRESQL
{
	static const String ERROR_POSTGRESQL_CONNECTION = STR( "Couldn't create the connection" );
	static const String ERROR_POSTGRESQL_CONNECT = STR( "Couldn't initialise the connection: " );
	static const String ERROR_POSTGRESQL_NOT_CONNECTED = STR( "Not connected" );

	static const TChar * INFO_POSTGRESQL_QUERY_EXECUTION = STR( "Query execution" );
	static const TChar * INFO_ESCAPING_TEXT = STR( "Escaping text" );
	static const TChar * INFO_ESCAPING_IDENTIFIER = STR( "Escaping identifier" );
	static const TChar * INFO_ESCAPING_BINARY = STR( "Escaping byte array" );

	static const String POSTGRESQL_SQL_CREATE_DATABASE = STR( "CREATE DATABASE " );
	static const String POSTGRESQL_SQL_CHARSET = STR( " ENCODING = 'UTF8'" );
	static const String POSTGRESQL_SQL_DROP_DATABASE = STR( "DROP DATABASE " );

	static const std::string POSTGRESQL_SQL_SNULL = "NULL";
	static const std::wstring POSTGRESQL_SQL_WNULL = L"NULL";

	static const std::string POSTGRESQL_FORMAT_SDATE = "CAST('%Y-%m-%d' AS DATE)";
	static const std::string POSTGRESQL_FORMAT_STIME = "CAST('%H:%M:%S' AS TIME)";
	static const std::string POSTGRESQL_FORMAT_SDATETIME = "CAST('%Y-%m-%d %H:%M:%S' AS TIMESTAMP)";
	static const std::string POSTGRESQL_FORMAT_SDATETIME_DATE = "CAST('%Y-%m-%d 00:00:00' AS TIMESTAMP)";
	static const std::string POSTGRESQL_FORMAT_SDATETIME_TIME = "CAST('0000-00-00 %H:%M:%S' AS TIMESTAMP)";
	static const std::string POSTGRESQL_FORMAT_STMT_SDATE = "{-d %Y-%m-%d}";
	static const std::string POSTGRESQL_FORMAT_STMT_STIME = "{-t %H:%M:%S}";
	static const std::string POSTGRESQL_FORMAT_STMT_SDATETIME = "{-ts %Y-%m-%d %H:%M:%S}";

	static const std::wstring POSTGRESQL_FORMAT_WDATE = L"CAST('%Y-%m-%d' AS DATE)";
	static const std::wstring POSTGRESQL_FORMAT_WTIME = L"CAST('%H:%M:%S' AS TIME)";
	static const std::wstring POSTGRESQL_FORMAT_WDATETIME = L"CAST('%Y-%m-%d %H:%M:%S' AS TIMESTAMP)";
	static const std::wstring POSTGRESQL_FORMAT_WDATETIME_DATE = L"CAST('%Y-%m-%d 00:00:00' AS TIMESTAMP)";
	static const std::wstring POSTGRESQL_FORMAT_WDATETIME_TIME = L"CAST('0000-00-00 %H:%M:%S' AS TIMESTAMP)";
	static const std::wstring POSTGRESQL_FORMAT_STMT_WDATE = L"{-d %Y-%m-%d}";
	static const std::wstring POSTGRESQL_FORMAT_STMT_WTIME = L"{-t %H:%M:%S}";
	static const std::wstring POSTGRESQL_FORMAT_STMT_WDATETIME = L"{-ts %Y-%m-%d %H:%M:%S}";

	// cf. https://dev.mysql.com/doc/refman/5.1/en/c-api-data-structures.html
	static const int POSTGRESQL_BINARY_CHARSET = 63;


	CDatabaseConnectionPostgreSql::CDatabaseConnectionPostgreSql( const String & server, const String & userName, const String & password, String & connectionString )
		: CDatabaseConnection( server, userName, password )
		, _connection( NULL )
	{
		DoConnect( connectionString );
	}

	CDatabaseConnectionPostgreSql::~CDatabaseConnectionPostgreSql()
	{
		DoDisconnect();
	}

	uint32_t CDatabaseConnectionPostgreSql::GetPrecision( EFieldType type ) const
	{
		uint32_t result = 0;

		switch ( type )
		{
		case EFieldType_FLOAT32:
			result = 6;
			break;

		case EFieldType_FLOAT64:
			result = 17;
			break;
		}

		return result;
	}

	std::string CDatabaseConnectionPostgreSql::WriteText( const std::string & text ) const
	{
		std::string result( text );

		if ( result != POSTGRESQL_SQL_SNULL )
		{
			char * escaped = PQescapeLiteral( _connection, result.c_str(), result.size() );

			if ( escaped )
			{
				result = escaped;
				PQfreemem( escaped );
			}
			else
			{
				PostgreSQLCheck( NULL, INFO_ESCAPING_TEXT, EDatabaseExceptionCodes_ConnectionError, _connection );
			}
		}

		return result;
	}

	std::wstring CDatabaseConnectionPostgreSql::WriteNText( const std::wstring & text ) const
	{
		std::string result( StringUtils::ToStr( text ) );

		if ( result != POSTGRESQL_SQL_SNULL )
		{
			char * escaped = PQescapeLiteral( _connection, result.c_str(), result.size() );

			if ( escaped )
			{
				result = escaped;
				PQfreemem( escaped );
			}
			else
			{
				PostgreSQLCheck( NULL, INFO_ESCAPING_TEXT, EDatabaseExceptionCodes_ConnectionError, _connection );
			}
		}

		return StringUtils::ToWStr( result );
	}

	String CDatabaseConnectionPostgreSql::WriteBinary( const ByteArray & text ) const
	{
		String result;
		size_t length = 0;
		uint8_t * escaped = PQescapeByteaConn( _connection, text.data(), text.size(), &length );

		if ( escaped )
		{
			result = STR( "'" ) + StringUtils::ToString( reinterpret_cast< char * >( escaped ) ) + STR( "'" );
			PQfreemem( escaped );
		}
		else
		{
			PostgreSQLCheck( NULL, INFO_ESCAPING_BINARY, EDatabaseExceptionCodes_ConnectionError, _connection );
		}

		return result;
	}

	String CDatabaseConnectionPostgreSql::WriteName( const String & text ) const
	{
		std::string result( text );

		if ( result != POSTGRESQL_SQL_SNULL )
		{
			char * escaped = PQescapeIdentifier( _connection, result.c_str(), result.size() );

			if ( escaped )
			{
				result = escaped;
				PQfreemem( escaped );
			}
			else
			{
				PostgreSQLCheck( NULL, INFO_ESCAPING_IDENTIFIER, EDatabaseExceptionCodes_ConnectionError, _connection );
			}
		}

		return result;
	}

	std::string CDatabaseConnectionPostgreSql::WriteDateS( const DateType & date ) const
	{
		std::string strReturn;

		if ( Date::IsValid( date ) )
		{
			strReturn = Date::Format( date, POSTGRESQL_FORMAT_SDATE );
		}
		else
		{
			strReturn += POSTGRESQL_SQL_SNULL;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionPostgreSql::WriteTimeS( const TimeType & time ) const
	{
		std::string strReturn;

		if ( Time::IsValid( time ) )
		{
			strReturn = Time::Format( time, POSTGRESQL_FORMAT_STIME );
		}
		else
		{
			strReturn += POSTGRESQL_SQL_SNULL;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionPostgreSql::WriteDateTimeS( const DateTimeType & dateTime ) const
	{
		std::string strReturn;

		if ( DateTime::IsValid( dateTime ) )
		{
			strReturn = DateTime::Format( dateTime, POSTGRESQL_FORMAT_SDATETIME );
		}
		else
		{
			strReturn += POSTGRESQL_SQL_SNULL;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionPostgreSql::WriteDateTimeS( const DateType & date ) const
	{
		std::string strReturn;

		if ( Date::IsValid( date ) )
		{
			strReturn = Date::Format( date, POSTGRESQL_FORMAT_SDATETIME_DATE );
		}
		else
		{
			strReturn += POSTGRESQL_SQL_SNULL;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionPostgreSql::WriteDateTimeS( const TimeType & time ) const
	{
		std::string strReturn;

		if ( Time::IsValid( time ) )
		{
			strReturn = Time::Format( time, POSTGRESQL_FORMAT_SDATETIME_TIME );
		}
		else
		{
			strReturn += POSTGRESQL_SQL_SNULL;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionPostgreSql::WriteStmtDateS( const DateType & date ) const
	{
		std::string strReturn;

		if ( Date::IsValid( date ) )
		{
			strReturn = Date::Format( date, POSTGRESQL_FORMAT_STMT_SDATE );
		}
		else
		{
			strReturn += POSTGRESQL_SQL_SNULL;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionPostgreSql::WriteStmtTimeS( const TimeType & time ) const
	{
		std::string strReturn;

		if ( Time::IsValid( time ) )
		{
			strReturn = Time::Format( time, POSTGRESQL_FORMAT_STMT_STIME );
		}
		else
		{
			strReturn += POSTGRESQL_SQL_SNULL;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionPostgreSql::WriteStmtDateTimeS( const DateTimeType & dateTime ) const
	{
		std::string strReturn;

		if ( DateTime::IsValid( dateTime ) )
		{
			strReturn = DateTime::Format( dateTime, POSTGRESQL_FORMAT_STMT_SDATETIME );
		}
		else
		{
			strReturn += POSTGRESQL_SQL_SNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionPostgreSql::WriteDateW( const DateType & date ) const
	{
		std::wstring strReturn;

		if ( Date::IsValid( date ) )
		{
			strReturn = Date::Format( date, POSTGRESQL_FORMAT_WDATE );
		}
		else
		{
			strReturn += POSTGRESQL_SQL_WNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionPostgreSql::WriteTimeW( const TimeType & time ) const
	{
		std::wstring strReturn;

		if ( Time::IsValid( time ) )
		{
			strReturn = Time::Format( time, POSTGRESQL_FORMAT_WTIME );
		}
		else
		{
			strReturn += POSTGRESQL_SQL_WNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionPostgreSql::WriteDateTimeW( const DateTimeType & dateTime ) const
	{
		std::wstring strReturn;

		if ( DateTime::IsValid( dateTime ) )
		{
			strReturn = DateTime::Format( dateTime, POSTGRESQL_FORMAT_WDATETIME );
		}
		else
		{
			strReturn += POSTGRESQL_SQL_WNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionPostgreSql::WriteDateTimeW( const DateType & date ) const
	{
		std::wstring strReturn;

		if ( Date::IsValid( date ) )
		{
			strReturn = Date::Format( date, POSTGRESQL_FORMAT_WDATETIME_DATE );
		}
		else
		{
			strReturn += POSTGRESQL_SQL_WNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionPostgreSql::WriteDateTimeW( const TimeType & time ) const
	{
		std::wstring strReturn;

		if ( Time::IsValid( time ) )
		{
			strReturn = Time::Format( time, POSTGRESQL_FORMAT_WDATETIME_TIME );
		}
		else
		{
			strReturn += POSTGRESQL_SQL_WNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionPostgreSql::WriteStmtDateW( const DateType & date ) const
	{
		std::wstring strReturn;

		if ( Date::IsValid( date ) )
		{
			strReturn = Date::Format( date, POSTGRESQL_FORMAT_STMT_WDATE );
		}
		else
		{
			strReturn += POSTGRESQL_SQL_WNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionPostgreSql::WriteStmtTimeW( const TimeType & time ) const
	{
		std::wstring strReturn;

		if ( Time::IsValid( time ) )
		{
			strReturn = Time::Format( time, POSTGRESQL_FORMAT_STMT_WTIME );
		}
		else
		{
			strReturn += POSTGRESQL_SQL_WNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionPostgreSql::WriteStmtDateTimeW( const DateTimeType & dateTime ) const
	{
		std::wstring strReturn;

		if ( DateTime::IsValid( dateTime ) )
		{
			strReturn = DateTime::Format( dateTime, POSTGRESQL_FORMAT_STMT_WDATETIME );
		}
		else
		{
			strReturn += POSTGRESQL_SQL_WNULL;
		}

		return strReturn;
	}

	String CDatabaseConnectionPostgreSql::WriteBool( bool value ) const
	{
		return ( value ? STR( "true" ) : STR( "false" ) );
	}

	String CDatabaseConnectionPostgreSql::WriteBool( const String & value ) const
	{
		const String lowerCaseValue = StringUtils::LowerCase( value );
		return ( lowerCaseValue == STR( "x" ) || lowerCaseValue == STR( "oui" ) || lowerCaseValue == STR( "yes" ) || lowerCaseValue == STR( "y" ) || value == STR( "1" ) ? STR( "1" ) : STR( "0" ) );
	}

	DateType CDatabaseConnectionPostgreSql::ParseDate( const std::string & date ) const
	{
		DateType dateObj;

		if ( !Date::IsDate( date, POSTGRESQL_FORMAT_SDATE, dateObj )
		&& !Date::IsDate( date, POSTGRESQL_FORMAT_STMT_SDATE, dateObj ) )
		{
			// date is already invalid
		}

		return dateObj;
	}

	TimeType CDatabaseConnectionPostgreSql::ParseTime( const std::string & time ) const
	{
		TimeType timeObj;

		if ( !Time::IsTime( time, POSTGRESQL_FORMAT_STIME, timeObj )
		&& !Time::IsTime( time, POSTGRESQL_FORMAT_STMT_STIME, timeObj ) )
		{
			timeObj = TimeType();
		}

		return timeObj;
	}

	DateTimeType CDatabaseConnectionPostgreSql::ParseDateTime( const std::string & dateTime ) const
	{
		DateTimeType dateTimeObj;

		if ( !DateTime::IsDateTime( dateTime, POSTGRESQL_FORMAT_SDATETIME, dateTimeObj )
		&& !DateTime::IsDateTime( dateTime, POSTGRESQL_FORMAT_STMT_SDATETIME, dateTimeObj )
		&& !DateTime::IsDateTime( dateTime, POSTGRESQL_FORMAT_SDATE, dateTimeObj )
		&& !DateTime::IsDateTime( dateTime, POSTGRESQL_FORMAT_STMT_SDATE, dateTimeObj ) )
		{
			dateTimeObj = DateTimeType();
		}

		return dateTimeObj;
	}

	DateType CDatabaseConnectionPostgreSql::ParseDate( const std::wstring & date ) const
	{
		DateType dateObj;

		if ( !Date::IsDate( date, POSTGRESQL_FORMAT_WDATE, dateObj )
		&& !Date::IsDate( date, POSTGRESQL_FORMAT_STMT_WDATE, dateObj ) )
		{
			// date is already invalid
		}

		return dateObj;
	}

	TimeType CDatabaseConnectionPostgreSql::ParseTime( const std::wstring & time ) const
	{
		TimeType timeObj;

		if ( !Time::IsTime( time, POSTGRESQL_FORMAT_WTIME, timeObj )
		&& !Time::IsTime( time, POSTGRESQL_FORMAT_STMT_WTIME, timeObj ) )
		{
			timeObj = TimeType();
		}

		return timeObj;
	}

	DateTimeType CDatabaseConnectionPostgreSql::ParseDateTime( const std::wstring & dateTime ) const
	{
		DateTimeType dateTimeObj;

		if ( !DateTime::IsDateTime( dateTime, POSTGRESQL_FORMAT_WDATETIME, dateTimeObj )
		&& !DateTime::IsDateTime( dateTime, POSTGRESQL_FORMAT_STMT_WDATETIME, dateTimeObj )
		&& !DateTime::IsDateTime( dateTime, POSTGRESQL_FORMAT_WDATE, dateTimeObj )
		&& !DateTime::IsDateTime( dateTime, POSTGRESQL_FORMAT_STMT_WDATE, dateTimeObj ) )
		{
			dateTimeObj = DateTimeType();
		}

		return dateTimeObj;
	}

	unsigned long CDatabaseConnectionPostgreSql::GetStmtDateSize()const
	{
		return ( unsigned long )sizeof( date );
	}

	unsigned long CDatabaseConnectionPostgreSql::GetStmtDateTimeSize()const
	{
		return ( unsigned long )sizeof( timestamp );
	}

	unsigned long CDatabaseConnectionPostgreSql::GetStmtTimeSize()const
	{
		return ( unsigned long )sizeof( time_t );
	}

	PGconn * CDatabaseConnectionPostgreSql::GetConnection() const
	{
		return _connection;
	}

	void CDatabaseConnectionPostgreSql::CreateDatabase( const String & database )
	{
		if ( !IsConnected() )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_POSTGRESQL_NOT_CONNECTED );
		}

		ExecuteUpdate( POSTGRESQL_SQL_CREATE_DATABASE + database + POSTGRESQL_SQL_CHARSET );
	}

	void CDatabaseConnectionPostgreSql::SelectDatabase( const String & database )
	{
		if ( IsConnected() )
		{
			DoDisconnect();
		}

		String connectionString;

		if ( DoConnect( database, connectionString ) )
		{
			_database = database;
		}
	}

	void CDatabaseConnectionPostgreSql::DestroyDatabase( const String & database )
	{
		if ( !IsConnected() )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_POSTGRESQL_NOT_CONNECTED );
		}

		ExecuteUpdate( POSTGRESQL_SQL_DROP_DATABASE + database );
	}

	bool CDatabaseConnectionPostgreSql::DoConnect( String const & database, String & connectionString )
	{
		bool ret = false;

		try
		{
			std::stringstream stream;
			stream << "host=" << _server;

			if ( database.empty() )
			{
				stream << " dbname=template1";
			}
			else
			{
				stream << " dbname=" << database;
			}

			if ( !_userName.empty() )
			{
				stream << " user=" << _userName;
			}

			if ( !_password.empty() )
			{
				stream << " password=" << _password;
			}

			_connection = PQconnectdb( stream.str().c_str() );

			if ( !_connection )
			{
				DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_POSTGRESQL_CONNECTION );
			}

			ConnStatusType status = PQstatus( _connection );
			CLogger::LogDebug( STR( "Connection status: " ) + GetStatusName( status ) );

			if ( status != CONNECTION_OK )
			{
				DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_POSTGRESQL_CONNECTION );
			}
			
			PQconninfoOption * info = PQconninfo( _connection );

			if ( !info )
			{
				DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_POSTGRESQL_CONNECTION );
			}

			PQconninfoOption * array( info );

			while ( array->keyword )
			{
				if ( array->val )
				{
					CLogger::LogDebug( std::stringstream() << array->keyword << ": " << array->val );
				}
				else
				{
					CLogger::LogDebug( std::stringstream() << array->keyword << ": Not displayed" );
				}

				++array;
			}

			PQconninfoFree( info );
			connectionString = StringUtils::ToStr( stream.str() );

			ret = true;
		}
		catch ( CExceptionDatabase & exc )
		{
			CLogger::LogError( StringStream() << ERROR_POSTGRESQL_CONNECTION << STR( " - " ) << exc.GetFullDescription() );
		}
		catch ( std::exception & exc )
		{
			CLogger::LogError( StringStream() << ERROR_POSTGRESQL_CONNECTION << STR( " - " ) << exc.what() );
		}
		catch ( ... )
		{
			CLogger::LogError( ERROR_POSTGRESQL_CONNECTION );
		}

		DoSetConnected( ret );
		return ret;
	}

	EErrorType CDatabaseConnectionPostgreSql::DoConnect( String & connectionString )
	{
		return DoConnect( String(), connectionString ) ? EErrorType_NONE : EErrorType_ERROR;
	}

	void CDatabaseConnectionPostgreSql::DoDisconnect()
	{
		DoSetConnected( false );

		if ( _connection )
		{
			PQfinish( _connection );
			_connection = NULL;
		}
	}

	bool CDatabaseConnectionPostgreSql::DoBeginTransaction( const String & name )
	{
		bool ret = ExecuteUpdate( STR( "BEGIN" ) );

		if ( ret && !name.empty() )
		{
			ret = ExecuteUpdate( STR( "SAVEPOINT " ) + name );
		}

		return ret;
	}

	bool CDatabaseConnectionPostgreSql::DoCommit( const String & name )
	{
		return ExecuteUpdate( STR( "COMMIT" ) );
	}

	bool CDatabaseConnectionPostgreSql::DoRollBack( const String & name )
	{
		bool ret = false;

		if ( !name.empty() )
		{
			ret = ExecuteUpdate( STR( "ROLLBACK TO " ) + name );
		}
		else
		{
			ret = ExecuteUpdate( STR( "ROLLBACK" ) );
		}

		return ret;
	}

	bool CDatabaseConnectionPostgreSql::DoExecuteUpdate( const String & query )
	{
		std::string strQuery = StringUtils::ToStr( query );
		PGresult * result = PQexec( _connection, strQuery.c_str() );
		PostgreSQLCheck( result, INFO_POSTGRESQL_QUERY_EXECUTION, EDatabaseExceptionCodes_StatementError, _connection );
		PQclear( result );
		return true;
	}

	DatabaseResultSPtr CDatabaseConnectionPostgreSql::DoExecuteSelect( const String & query )
	{
		std::string strQuery = StringUtils::ToStr( query );
		PGresult * result = PQexec( _connection, strQuery.c_str() );
		PostgreSQLCheck( result, INFO_POSTGRESQL_QUERY_EXECUTION, EDatabaseExceptionCodes_StatementError, _connection );

		std::vector< std::unique_ptr< SInPostgreSqlBindBase > > binds;
		return PostgreSqlFetchResult( result, PostgreSqlGetColumns( result, binds ), std::static_pointer_cast< CDatabaseConnectionPostgreSql >( shared_from_this() ), binds );
	}

	DatabaseStatementSPtr CDatabaseConnectionPostgreSql::DoCreateStatement( const String & request )
	{
		return std::make_shared< CDatabaseStatementPostgreSql >( std::static_pointer_cast< CDatabaseConnectionPostgreSql >( shared_from_this() ), request );
	}
}
END_NAMESPACE_DATABASE_POSTGRESQL
