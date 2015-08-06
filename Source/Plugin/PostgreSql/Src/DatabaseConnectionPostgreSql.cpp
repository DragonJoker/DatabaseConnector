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
#include <DatabaseValuedObjectInfos.h>

#include <DatabaseStringUtils.h>
#include <DatabaseLogger.h>

#include <postgres.h>
#include <libpq-fe.h>
#include <pgtypes_date.h>
#include <pgtypes_timestamp.h>
#include <catalog/pg_type.h>

#ifdef max
#	undef min
#	undef max
#	undef abs
#endif

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

	static const String POSTGRESQL_SQL_NULL = STR( "NULL" );

	static const String POSTGRESQL_FORMAT_DATE = STR( "CAST('%Y-%m-%d' AS DATE)" );
	static const String POSTGRESQL_FORMAT_TIME = STR( "CAST('%H:%M:%S' AS TIME)" );
	static const String POSTGRESQL_FORMAT_DATETIME = STR( "CAST('%Y-%m-%d %H:%M:%S' AS TIMESTAMP)" );
	static const String POSTGRESQL_FORMAT_DATETIME_DATE = STR( "CAST('%Y-%m-%d 00:00:00' AS TIMESTAMP)" );
	static const String POSTGRESQL_FORMAT_DATETIME_TIME = STR( "CAST('0000-00-00 %H:%M:%S' AS TIMESTAMP)" );
	static const String POSTGRESQL_FORMAT_STMT_DATE = STR( "{-d %Y-%m-%d}" );
	static const String POSTGRESQL_FORMAT_STMT_TIME = STR( "{-t %H:%M:%S}" );
	static const String POSTGRESQL_FORMAT_STMT_DATETIME = STR( "{-ts %Y-%m-%d %H:%M:%S}" );

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

	void CDatabaseConnectionPostgreSql::DoCreateDatabase( const String & database )
	{
		DoExecuteUpdate( POSTGRESQL_SQL_CREATE_DATABASE + database + POSTGRESQL_SQL_CHARSET );
	}

	void CDatabaseConnectionPostgreSql::DoSelectDatabase( const String & database )
	{
		DoDisconnect();
		String connectionString;

		if ( DoConnect( database, connectionString ) )
		{
			_database = database;
		}
	}

	void CDatabaseConnectionPostgreSql::DoDestroyDatabase( const String & database )
	{
		if ( database == _database )
		{
			DoDisconnect();
			String connectionString;
			DoConnect( String(), connectionString );
			_database.clear();
		}

		DoExecuteUpdate( POSTGRESQL_SQL_DROP_DATABASE + database );
	}

	std::string CDatabaseConnectionPostgreSql::DoWriteText( const std::string & text ) const
	{
		std::string result( text );

		if ( result != POSTGRESQL_SQL_NULL )
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

	std::wstring CDatabaseConnectionPostgreSql::DoWriteNText( const std::wstring & text ) const
	{
		std::string result( StringUtils::ToUtf8( text, "UTF-8" ) );

		if ( result != POSTGRESQL_SQL_NULL )
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

	String CDatabaseConnectionPostgreSql::DoWriteBinary( const ByteArray & text ) const
	{
		String result;
		size_t length = 0;
		uint8_t * escaped = PQescapeByteaConn( _connection, text.data(), text.size(), &length );

		if ( escaped )
		{
			result = STR( "'" ) + String( reinterpret_cast< char * >( escaped ) ) + STR( "'" );
			PQfreemem( escaped );
		}
		else
		{
			PostgreSQLCheck( NULL, INFO_ESCAPING_BINARY, EDatabaseExceptionCodes_ConnectionError, _connection );
		}

		return result;
	}

	String CDatabaseConnectionPostgreSql::DoWriteName( const String & text ) const
	{
		std::string result( text );

		if ( result != POSTGRESQL_SQL_NULL )
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

	String CDatabaseConnectionPostgreSql::DoWriteDate( const DateType & date ) const
	{
		String strReturn;

		if ( Date::IsValid( date ) )
		{
			strReturn = Date::Format( date, POSTGRESQL_FORMAT_DATE );
		}
		else
		{
			strReturn += POSTGRESQL_SQL_NULL;
		}

		return strReturn;
	}

	String CDatabaseConnectionPostgreSql::DoWriteTime( const TimeType & time ) const
	{
		String strReturn;

		if ( Time::IsValid( time ) )
		{
			strReturn = Time::Format( time, POSTGRESQL_FORMAT_TIME );
		}
		else
		{
			strReturn += POSTGRESQL_SQL_NULL;
		}

		return strReturn;
	}

	String CDatabaseConnectionPostgreSql::DoWriteDateTime( const DateTimeType & dateTime ) const
	{
		String strReturn;

		if ( DateTime::IsValid( dateTime ) )
		{
			strReturn = DateTime::Format( dateTime, POSTGRESQL_FORMAT_DATETIME );
		}
		else
		{
			strReturn += POSTGRESQL_SQL_NULL;
		}

		return strReturn;
	}

	String CDatabaseConnectionPostgreSql::DoWriteDateTime( const DateType & date ) const
	{
		String strReturn;

		if ( Date::IsValid( date ) )
		{
			strReturn = Date::Format( date, POSTGRESQL_FORMAT_DATETIME_DATE );
		}
		else
		{
			strReturn += POSTGRESQL_SQL_NULL;
		}

		return strReturn;
	}

	String CDatabaseConnectionPostgreSql::DoWriteDateTime( const TimeType & time ) const
	{
		String strReturn;

		if ( Time::IsValid( time ) )
		{
			strReturn = Time::Format( time, POSTGRESQL_FORMAT_DATETIME_TIME );
		}
		else
		{
			strReturn += POSTGRESQL_SQL_NULL;
		}

		return strReturn;
	}

	String CDatabaseConnectionPostgreSql::DoWriteStmtDate( const DateType & date ) const
	{
		String strReturn;

		if ( Date::IsValid( date ) )
		{
			strReturn = Date::Format( date, POSTGRESQL_FORMAT_STMT_DATE );
		}
		else
		{
			strReturn += POSTGRESQL_SQL_NULL;
		}

		return strReturn;
	}

	String CDatabaseConnectionPostgreSql::DoWriteStmtTime( const TimeType & time ) const
	{
		String strReturn;

		if ( Time::IsValid( time ) )
		{
			strReturn = Time::Format( time, POSTGRESQL_FORMAT_STMT_TIME );
		}
		else
		{
			strReturn += POSTGRESQL_SQL_NULL;
		}

		return strReturn;
	}

	String CDatabaseConnectionPostgreSql::DoWriteStmtDateTime( const DateTimeType & dateTime ) const
	{
		String strReturn;

		if ( DateTime::IsValid( dateTime ) )
		{
			strReturn = DateTime::Format( dateTime, POSTGRESQL_FORMAT_STMT_DATETIME );
		}
		else
		{
			strReturn += POSTGRESQL_SQL_NULL;
		}

		return strReturn;
	}

	String CDatabaseConnectionPostgreSql::DoWriteBool( bool value ) const
	{
		return ( value ? STR( "true" ) : STR( "false" ) );
	}

	String CDatabaseConnectionPostgreSql::DoWriteFloat( float value ) const
	{
		StringStream stream;
		uint32_t decimals = GetPrecision( EFieldType_FLOAT32 );
		stream << STR( "CAST( " );
		stream.precision( decimals );
		stream << value;
		// float min == -3.40282e+038 => 39 digits before decimals separator
		stream << STR( " AS DECIMAL( " ) << ( 39 + decimals ) << STR( ", " ) << decimals << STR( " ) )" );
		return stream.str();
	}

	DateType CDatabaseConnectionPostgreSql::DoParseDate( const String & date ) const
	{
		DateType dateObj;

		if ( !Date::IsDate( date, POSTGRESQL_FORMAT_DATE, dateObj ) )
		{
			Date::IsDate( date, POSTGRESQL_FORMAT_STMT_DATE, dateObj );
		}

		return dateObj;
	}

	TimeType CDatabaseConnectionPostgreSql::DoParseTime( const String & time ) const
	{
		TimeType timeObj;

		if ( !Time::IsTime( time, POSTGRESQL_FORMAT_TIME, timeObj ) )
		{
			Time::IsTime( time, POSTGRESQL_FORMAT_STMT_TIME, timeObj );
		}

		return timeObj;
	}

	DateTimeType CDatabaseConnectionPostgreSql::DoParseDateTime( const String & dateTime ) const
	{
		DateTimeType dateTimeObj;

		if ( !DateTime::IsDateTime( dateTime, POSTGRESQL_FORMAT_DATETIME, dateTimeObj )
		&& !DateTime::IsDateTime( dateTime, POSTGRESQL_FORMAT_STMT_DATETIME, dateTimeObj )
		&& !DateTime::IsDateTime( dateTime, POSTGRESQL_FORMAT_DATE, dateTimeObj ) )
		{
			DateTime::IsDateTime( dateTime, POSTGRESQL_FORMAT_STMT_DATE, dateTimeObj );
		}

		return dateTimeObj;
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
			connectionString = stream.str();

			ret = true;
		}
		catch ( CDatabaseException & exc )
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
		PGresult * result = PQexec( _connection, query.c_str() );
		PostgreSQLCheck( result, INFO_POSTGRESQL_QUERY_EXECUTION, EDatabaseExceptionCodes_StatementError, _connection );
		PQclear( result );
		return true;
	}

	DatabaseResultSPtr CDatabaseConnectionPostgreSql::DoExecuteSelect( const String & query, DatabaseValuedObjectInfosPtrArray & infos )
	{
		PGresult * result = PQexec( _connection, query.c_str() );
		PostgreSQLCheck( result, INFO_POSTGRESQL_QUERY_EXECUTION, EDatabaseExceptionCodes_StatementError, _connection );

		if ( infos.empty() )
		{
			infos = PostgreSqlGetColumns( result );
		}

		return PostgreSqlFetchResult( result, infos, std::static_pointer_cast< CDatabaseConnectionPostgreSql >( shared_from_this() ) );
	}

	DatabaseStatementSPtr CDatabaseConnectionPostgreSql::DoCreateStatement( const String & request )
	{
		return std::make_shared< CDatabaseStatementPostgreSql >( std::static_pointer_cast< CDatabaseConnectionPostgreSql >( shared_from_this() ), request );
	}
}
END_NAMESPACE_DATABASE_POSTGRESQL
