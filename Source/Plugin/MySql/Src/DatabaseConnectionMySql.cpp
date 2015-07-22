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
#include <DatabaseFieldInfos.h>

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

	String CDatabaseConnectionMySql::WriteBinary( const ByteArray & array ) const
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
		&& !CDate::IsDate( date, MYSQL_FORMAT_STMT_SDATE, dateObj ) )
		{
			dateObj = CDate( 0, EDateMonth_UNDEF, 0 );
		}

		return dateObj;
	}

	CTime CDatabaseConnectionMySql::ParseTime( const std::string & time ) const
	{
		CTime timeObj;

		if ( !CTime::IsTime( time, MYSQL_FORMAT_STIME, timeObj )
		&& !CTime::IsTime( time, MYSQL_FORMAT_STMT_STIME, timeObj ) )
		{
			timeObj = CTime();
		}

		return timeObj;
	}

	CDateTime CDatabaseConnectionMySql::ParseDateTime( const std::string & dateTime ) const
	{
		CDateTime dateTimeObj;

		if ( !CDateTime::IsDateTime( dateTime, MYSQL_FORMAT_SDATETIME, dateTimeObj )
		&& !CDateTime::IsDateTime( dateTime, MYSQL_FORMAT_STMT_SDATETIME, dateTimeObj )
		&& !CDateTime::IsDateTime( dateTime, MYSQL_FORMAT_SDATE, dateTimeObj )
		&& !CDateTime::IsDateTime( dateTime, MYSQL_FORMAT_STMT_SDATE, dateTimeObj ) )
		{
			dateTimeObj = CDateTime();
		}

		return dateTimeObj;
	}

	CDate CDatabaseConnectionMySql::ParseDate( const std::wstring & date ) const
	{
		CDate dateObj;

		if ( !CDate::IsDate( date, MYSQL_FORMAT_WDATE, dateObj )
		&& !CDate::IsDate( date, MYSQL_FORMAT_STMT_WDATE, dateObj ) )
		{
			dateObj = CDate( 0, EDateMonth_UNDEF, 0 );
		}

		return dateObj;
	}

	CTime CDatabaseConnectionMySql::ParseTime( const std::wstring & time ) const
	{
		CTime timeObj;

		if ( !CTime::IsTime( time, MYSQL_FORMAT_WTIME, timeObj )
		&& !CTime::IsTime( time, MYSQL_FORMAT_STMT_WTIME, timeObj ) )
		{
			timeObj = CTime();
		}

		return timeObj;
	}

	CDateTime CDatabaseConnectionMySql::ParseDateTime( const std::wstring & dateTime ) const
	{
		CDateTime dateTimeObj;

		if ( !CDateTime::IsDateTime( dateTime, MYSQL_FORMAT_WDATETIME, dateTimeObj )
		&& !CDateTime::IsDateTime( dateTime, MYSQL_FORMAT_STMT_WDATETIME, dateTimeObj )
		&& !CDateTime::IsDateTime( dateTime, MYSQL_FORMAT_WDATE, dateTimeObj )
		&& !CDateTime::IsDateTime( dateTime, MYSQL_FORMAT_STMT_WDATE, dateTimeObj ) )
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
			MySQLCheck( mysql_stmt_execute( statement ), INFO_MYSQL_STATEMENT_EXECUTION, EDatabaseExceptionCodes_StatementError, _connection );
			uint64_t affected = mysql_stmt_affected_rows( statement );
			result = true;
		}
		catch ( CExceptionDatabase & exc )
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
		catch ( CExceptionDatabase & exc )
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

	void CDatabaseConnectionMySql::CreateDatabase( const String & database )
	{
		if ( !IsConnected() )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_MYSQL_NOT_CONNECTED );
		}

		DoExecuteUpdate( MYSQL_SQL_CREATE_DATABASE + database + MYSQL_SQL_CHARSET );
	}

	void CDatabaseConnectionMySql::SelectDatabase( const String & database )
	{
		if ( !IsConnected() )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_MYSQL_NOT_CONNECTED );
		}

		if ( _connection )
		{
			std::string query = CStrUtils::ToStr( MYSQL_SQL_USE + database );
			MySQLCheck( mysql_real_query( _connection, query.c_str(), uint32_t( query.size() ) ), INFO_MYSQL_DATABASE_SELECTION, EDatabaseExceptionCodes_ConnectionError, _connection );
			_database = database;
		}
	}

	void CDatabaseConnectionMySql::DestroyDatabase( const String & database )
	{
		if ( !IsConnected() )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_MYSQL_NOT_CONNECTED );
		}

		DoExecuteUpdate( MYSQL_SQL_DROP_DATABASE + database );
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
				DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, error.str() );
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
		std::string strQuery = CStrUtils::ToStr( query );
		MySQLCheck( mysql_stmt_prepare( _statement, strQuery.c_str(), uint32_t( strQuery.size() ) ), INFO_MYSQL_STATEMENT_PREPARATION, EDatabaseExceptionCodes_StatementError, _connection );
		return ExecuteUpdate( _statement );
	}

	DatabaseResultSPtr CDatabaseConnectionMySql::DoExecuteSelect( const String & query )
	{
		std::string strQuery = CStrUtils::ToStr( query );
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
		catch ( CExceptionDatabase & exc )
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
