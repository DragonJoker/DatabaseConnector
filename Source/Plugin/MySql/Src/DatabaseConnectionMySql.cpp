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
#include "DatabaseMySqlHelper.h"

#include <DatabaseDate.h>
#include <DatabaseDateTime.h>
#include <DatabaseDateTimeHelper.h>
#include <DatabaseTime.h>
#include <DatabaseValuedObjectInfos.h>

#include <DatabaseStringUtils.h>
#include <DatabaseLogger.h>

#include <mysql.h>
#include <mysql_time.h>

#if defined( _WIN32 )
#	undef min
#	undef max
#	undef abs
#endif

BEGIN_NAMESPACE_DATABASE_MYSQL
{
	static const String ERROR_MYSQL_CONNECTION = STR( "Couldn't create the connection" );
	static const String ERROR_MYSQL_CONNECT = STR( "Couldn't initialise the connection: " );
	static const String ERROR_MYSQL_EXECUTION = STR( "Couldn't execute the query" );
	static const String ERROR_MYSQL_NOT_CONNECTED = STR( "Not connected" );
	static const String ERROR_MYSQL_ALREADY_IN_TRANSACTION = STR( "Already in a transaction" );
	static const String ERROR_MYSQL_NOT_IN_TRANSACTION = STR( "Not in a transaction" );

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

	static const String MYSQL_SQL_NULL = STR( "NULL" );

	static const char * MYSQL_OPTION_NAMES = "SET NAMES 'utf8'";
	static const char * MYSQL_OPTION_UTF8 = "utf8";

	static const String MYSQL_FORMAT_DATE = STR( "CAST('%Y-%m-%d' AS DATE)" );
	static const String MYSQL_FORMAT_TIME = STR( "CAST('%H:%M:%S' AS TIME)" );
	static const String MYSQL_FORMAT_DATETIME = STR( "CAST('%Y-%m-%d %H:%M:%S' AS DATETIME)" );
	static const String MYSQL_FORMAT_DATETIME_DATE = STR( "CAST('%Y-%m-%d 00:00:00' AS DATETIME)" );
	static const String MYSQL_FORMAT_DATETIME_TIME = STR( "CAST('0000-00-00 %H:%M:%S' AS DATETIME)" );
	static const String MYSQL_FORMAT_STMT_DATE = STR( "{-d %Y-%m-%d}" );
	static const String MYSQL_FORMAT_STMT_TIME = STR( "{-t %H:%M:%S}" );
	static const String MYSQL_FORMAT_STMT_DATETIME = STR( "{-ts %Y-%m-%d %H:%M:%S}" );

	// cf. https://dev.mysql.com/doc/refman/5.1/en/c-api-data-structures.html
	static const int MYSQL_BINARY_CHARSET = 63;


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

	uint32_t CDatabaseConnectionMySql::GetPrecision( EFieldType type ) const
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
			MySQLCheck( mysql_stmt_execute( statement ), INFO_MYSQL_STATEMENT_EXECUTION, EDatabaseExceptionCodes_StatementError, _connection );
			uint64_t affected = mysql_stmt_affected_rows( statement );
			result = true;
		}
		catch ( CDatabaseException & exc )
		{
			StringStream stream;
			stream << ERROR_MYSQL_EXECUTION << STR( " - " ) << exc.GetFullDescription();
			CLogger::LogError( stream );
		}
		catch ( std::exception & exc )
		{
			StringStream stream;
			stream << ERROR_MYSQL_EXECUTION << STR( " - " ) << exc.what();
			CLogger::LogError( stream );
		}

		return result;
	}

	DatabaseResultSPtr CDatabaseConnectionMySql::ExecuteSelect( MYSQL_STMT * statement )
	{
		DatabaseResultSPtr result;

		try
		{
			bool hasResults = false;
			MySQLCheck( mysql_stmt_execute( statement ), INFO_MYSQL_STATEMENT_EXECUTION, EDatabaseExceptionCodes_StatementError, _connection );
			result = DoRetrieveResults( statement );
		}
		catch ( CDatabaseException & exc )
		{
			StringStream stream;
			stream << ERROR_MYSQL_EXECUTION << STR( " - " ) << exc.GetFullDescription();
			CLogger::LogError( stream );
		}
		catch ( std::exception & exc )
		{
			StringStream stream;
			stream << ERROR_MYSQL_EXECUTION << STR( " - " ) << exc.what();
			CLogger::LogError( stream );
		}

		return result;
	}

	void CDatabaseConnectionMySql::DoCreateDatabase( const String & database )
	{
		DoExecuteUpdate( MYSQL_SQL_CREATE_DATABASE + database + MYSQL_SQL_CHARSET );
	}

	void CDatabaseConnectionMySql::DoSelectDatabase( const String & database )
	{
		if ( _connection )
		{
			std::string query = StringUtils::ToStr( MYSQL_SQL_USE + database );
			MySQLCheck( mysql_real_query( _connection, query.c_str(), uint32_t( query.size() ) ), INFO_MYSQL_DATABASE_SELECTION, EDatabaseExceptionCodes_ConnectionError, _connection );
			_database = database;
		}
	}

	void CDatabaseConnectionMySql::DoDestroyDatabase( const String & database )
	{
		DoExecuteUpdate( MYSQL_SQL_DROP_DATABASE + database );
	}

	std::string CDatabaseConnectionMySql::DoWriteText( const std::string & text ) const
	{
		std::string strReturn( text );

		if ( strReturn != MYSQL_SQL_NULL )
		{
			StringUtils::Replace( strReturn, "'", "''" );
			StringUtils::Replace( strReturn, "\\", "\\\\" );
			strReturn = "'" + strReturn + "'";
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionMySql::DoWriteNText( const std::wstring & text ) const
	{
		String strReturn( StringUtils::ToString( text ) );

		if ( strReturn != MYSQL_SQL_NULL )
		{
			StringUtils::Replace( strReturn, STR( "'" ), STR( "''" ) );
			StringUtils::Replace( strReturn, STR( "\\" ), STR( "\\\\" ) );
			strReturn = STR( "N'" ) + strReturn + STR( "'" );
		}

		return StringUtils::ToWStr( strReturn );
	}

	String CDatabaseConnectionMySql::DoWriteBinary( const ByteArray & array ) const
	{
		StringStream stream;
		stream.setf( std::ios::hex, std::ios::basefield );

		for ( auto && it = array.begin(); it != array.end(); ++it )
		{
			stream.width( 2 );
			stream.fill( STR( '0' ) );
			stream << int( *it );
		}

		return STR( "X'" ) + stream.str() + STR( "'" );
	}

	String CDatabaseConnectionMySql::DoWriteName( const String & text ) const
	{
		return STR( "[" ) + text + STR( "]" );
	}

	String CDatabaseConnectionMySql::DoWriteDate( const DateType & date ) const
	{
		String strReturn;

		if ( Date::IsValid( date ) )
		{
			strReturn = Date::Format( date, MYSQL_FORMAT_DATE );
		}
		else
		{
			strReturn += MYSQL_SQL_NULL;
		}

		return strReturn;
	}

	String CDatabaseConnectionMySql::DoWriteTime( const TimeType & time ) const
	{
		String strReturn;

		if ( Time::IsValid( time ) )
		{
			strReturn = Time::Format( time, MYSQL_FORMAT_TIME );
		}
		else
		{
			strReturn += MYSQL_SQL_NULL;
		}

		return strReturn;
	}

	String CDatabaseConnectionMySql::DoWriteDateTime( const DateTimeType & dateTime ) const
	{
		String strReturn;

		if ( DateTime::IsValid( dateTime ) )
		{
			strReturn = DateTime::Format( dateTime, MYSQL_FORMAT_DATETIME );
		}
		else
		{
			strReturn += MYSQL_SQL_NULL;
		}

		return strReturn;
	}

	String CDatabaseConnectionMySql::DoWriteDateTime( const DateType & date ) const
	{
		String strReturn;

		if ( Date::IsValid( date ) )
		{
			strReturn = Date::Format( date, MYSQL_FORMAT_DATETIME_DATE );
		}
		else
		{
			strReturn += MYSQL_SQL_NULL;
		}

		return strReturn;
	}

	String CDatabaseConnectionMySql::DoWriteDateTime( const TimeType & time ) const
	{
		String strReturn;

		if ( Time::IsValid( time ) )
		{
			strReturn = Time::Format( time, MYSQL_FORMAT_DATETIME_TIME );
		}
		else
		{
			strReturn += MYSQL_SQL_NULL;
		}

		return strReturn;
	}

	String CDatabaseConnectionMySql::DoWriteStmtDate( const DateType & date ) const
	{
		String strReturn;

		if ( Date::IsValid( date ) )
		{
			strReturn = Date::Format( date, MYSQL_FORMAT_STMT_DATE );
		}
		else
		{
			strReturn += MYSQL_SQL_NULL;
		}

		return strReturn;
	}

	String CDatabaseConnectionMySql::DoWriteStmtTime( const TimeType & time ) const
	{
		String strReturn;

		if ( Time::IsValid( time ) )
		{
			strReturn = Time::Format( time, MYSQL_FORMAT_STMT_TIME );
		}
		else
		{
			strReturn += MYSQL_SQL_NULL;
		}

		return strReturn;
	}

	String CDatabaseConnectionMySql::DoWriteStmtDateTime( const DateTimeType & dateTime ) const
	{
		String strReturn;

		if ( DateTime::IsValid( dateTime ) )
		{
			strReturn = DateTime::Format( dateTime, MYSQL_FORMAT_STMT_DATETIME );
		}
		else
		{
			strReturn += MYSQL_SQL_NULL;
		}

		return strReturn;
	}

	String CDatabaseConnectionMySql::DoWriteBool( bool value ) const
	{
		return ( value ? STR( "1" ) : STR( "0" ) );
	}

	DateType CDatabaseConnectionMySql::DoParseDate( const String & date ) const
	{
		DateType dateObj;

		if ( !Date::IsDate( date, MYSQL_FORMAT_DATE, dateObj )
		&& !Date::IsDate( date, MYSQL_FORMAT_STMT_DATE, dateObj ) )
		{
			// date is already invalid
		}

		return dateObj;
	}

	TimeType CDatabaseConnectionMySql::DoParseTime( const String & time ) const
	{
		TimeType timeObj;

		if ( !Time::IsTime( time, MYSQL_FORMAT_TIME, timeObj )
		&& !Time::IsTime( time, MYSQL_FORMAT_STMT_TIME, timeObj ) )
		{
			// time is already invalid
		}

		return timeObj;
	}

	DateTimeType CDatabaseConnectionMySql::DoParseDateTime( const String & dateTime ) const
	{
		DateTimeType dateTimeObj;

		if ( !DateTime::IsDateTime( dateTime, MYSQL_FORMAT_DATETIME, dateTimeObj )
		&& !DateTime::IsDateTime( dateTime, MYSQL_FORMAT_STMT_DATETIME, dateTimeObj )
		&& !DateTime::IsDateTime( dateTime, MYSQL_FORMAT_DATE, dateTimeObj )
		&& !DateTime::IsDateTime( dateTime, MYSQL_FORMAT_STMT_DATE, dateTimeObj ) )
		{
			// date/time is already invalid
		}

		return dateTimeObj;
	}

	EErrorType CDatabaseConnectionMySql::DoConnect( String & connectionString )
	{
		EErrorType ret = EErrorType_NONE;

		try
		{
			_connection = mysql_init( NULL );

			if ( !_connection )
			{
				DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_MYSQL_CONNECTION );
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
									  StringUtils::ToStr( _server ).c_str(),
									  StringUtils::ToStr( _userName ).c_str(),
									  StringUtils::ToStr( _password ).c_str(),
									  NULL,
									  0,
									  NULL,
									  CLIENT_REMEMBER_OPTIONS | CLIENT_MULTI_RESULTS ) )
			{
				StringStream error( ERROR_MYSQL_CONNECT );
				error << mysql_error( _connection );
				DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, error.str() );
			}

			CLogger::LogDebug( StringStream() << DEBUG_MYSQL_CONNECTED );
			CLogger::LogDebug( StringStream() << DEBUG_MYSQL_CLIENT_VERSION << mysql_get_client_info() );
			CLogger::LogDebug( StringStream() << DEBUG_MYSQL_SERVER_HOST << mysql_get_host_info( _connection ) );
			CLogger::LogDebug( StringStream() << DEBUG_MYSQL_SERVER_VERSION << mysql_get_server_info( _connection ) );

			_statement = mysql_stmt_init( _connection );

			DoSetConnected( true );
		}
		catch ( CDatabaseException & exc )
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

	bool CDatabaseConnectionMySql::DoBeginTransaction( const String & name )
	{
		return mysql_autocommit( _connection, false ) == 0;
	}

	bool CDatabaseConnectionMySql::DoCommit( const String & name )
	{
		bool result = mysql_commit( _connection ) == 0;

		if ( result )
		{
			result = mysql_autocommit( _connection, true ) == 0;
		}

		return result;
	}

	bool CDatabaseConnectionMySql::DoRollBack( const String & name )
	{
		bool result = mysql_rollback( _connection ) == 0;

		if ( result )
		{
			result = mysql_autocommit( _connection, true ) == 0;
		}

		return result;
	}

	bool CDatabaseConnectionMySql::DoExecuteUpdate( const String & query )
	{
		std::string strQuery = StringUtils::ToStr( query );
		MySQLCheck( mysql_stmt_prepare( _statement, strQuery.c_str(), uint32_t( strQuery.size() ) ), INFO_MYSQL_STATEMENT_PREPARATION, EDatabaseExceptionCodes_StatementError, _connection );
		return ExecuteUpdate( _statement );
	}

	DatabaseResultSPtr CDatabaseConnectionMySql::DoExecuteSelect( const String & query )
	{
		std::string strQuery = StringUtils::ToStr( query );
		MySQLCheck( mysql_stmt_prepare( _statement, strQuery.c_str(), uint32_t( strQuery.size() ) ), INFO_MYSQL_STATEMENT_PREPARATION, EDatabaseExceptionCodes_StatementError, _connection );
		return ExecuteSelect( _statement );
	}

	DatabaseStatementSPtr CDatabaseConnectionMySql::DoCreateStatement( const String & request )
	{
		return std::make_shared< CDatabaseStatementMySql >( std::static_pointer_cast< CDatabaseConnectionMySql >( shared_from_this() ), request );
	}

	DatabaseResultSPtr CDatabaseConnectionMySql::DoRetrieveResults( MYSQL_STMT * statement )
	{
		DatabaseResultSPtr pResult;

		try
		{
			if ( statement )
			{
				DatabaseConnectionMySqlSPtr connection = std::static_pointer_cast< CDatabaseConnectionMySql >( shared_from_this() );
				std::vector< MYSQL_BIND > binds;
				std::vector< std::unique_ptr< SInMySqlBindBase > > inbinds;
				pResult = MySqlFetchResult( statement, MySqlGetColumns( statement, connection, inbinds, binds ), connection, inbinds, binds );
			}
		}
		catch ( CDatabaseException & exc )
		{
			StringStream stream;
			stream << ERROR_MYSQL_EXECUTION << exc.GetFullDescription();
			CLogger::LogError( stream.str() );
		}
		catch ( std::exception & exc )
		{
			StringStream stream;
			stream << ERROR_MYSQL_EXECUTION << exc.what();
			CLogger::LogError( stream.str() );
		}

		return pResult;
	}
}
END_NAMESPACE_DATABASE_MYSQL
