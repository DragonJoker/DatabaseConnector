/************************************************************************//**
* @file DatabaseConnectionSqlite.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 3/14/2014 5:03:15 PM
*
*
* @brief CConnectionSqlite class definition.
*
* @details Describes a connection to a database via an SQLITE driver.
*
***************************************************************************/

#include "DatabaseSqlitePch.h"

#include "DatabaseConnectionSqlite.h"

#include "DatabaseStatementSqlite.h"
#include "ExceptionDatabaseSqlite.h"
#include "DatabaseSqliteHelper.h"

#include <DatabaseDate.h>
#include <DatabaseDateTime.h>
#include <DatabaseDateTimeHelper.h>
#include <DatabaseTime.h>

#include <DatabaseStringUtils.h>
#include <DatabaseLogger.h>

#include "sqlite3.h"

#include <boost/filesystem.hpp>

#if defined( _WIN32 )
#   include <Windows.h>
#endif

BEGIN_NAMESPACE_DATABASE_SQLITE
{
	static const String SQLITE_SQL_TRANSACTION_BEGIN = STR( "BEGIN TRANSACTION" );
	static const String SQLITE_SQL_TRANSACTION_COMMIT = STR( "COMMIT" );
	static const String SQLITE_SQL_TRANSACTION_ROLLBACK = STR( "SAVEPOINT " );
	static const String SQLITE_SQL_NAMED_TRANSACTION_BEGIN = STR( "BEGIN TRANSACTION " );
	static const String SQLITE_SQL_NAMED_TRANSACTION_COMMIT = STR( "RELEASE " );
	static const String SQLITE_SQL_NAMED_TRANSACTION_ROLLBACK = STR( "ROLLBACK TO " );
	static const String SQLITE_SQL_NULL_STRING = STR( "NULL" );

	static const std::string SQLITE_SQL_SNULL = "NULL";
	static const std::wstring SQLITE_SQL_WNULL = L"NULL";

	static const TChar * INFO_SQLITE_STATEMENT_FINALISATION = STR( "Statement finalisation" );
	static const TChar * INFO_SQLITE_EXECUTING_SELECT = STR( "Executing select : " );

	static const String ERROR_SQLITE_CONNECTION = STR( "Couldn't create the connection" );
	static const String ERROR_SQLITE_SELECTION = STR( "Couldn't select the database: " );
	static const String ERROR_SQLITE_DESTRUCTION = STR( "Couldn't destroy the database: " );
	static const String ERROR_SQLITE_PREPARATION_ERROR = STR( "Couldn't prepare the query" );
	static const String ERROR_SQLITE_EXECUTION_ERROR = STR( "Couldn't execute the query" );
	static const String ERROR_SQLITE_UNKNOWN_ERROR = STR( "Unknown error" );
	static const String ERROR_SQLITE_NOT_CONNECTED = STR( "Not connected" );
	static const String ERROR_SQLITE_ALREADY_IN_TRANSACTION = STR( "Already in a transaction" );
	static const String ERROR_SQLITE_NOT_IN_TRANSACTION = STR( "Not in a transaction" );

	static const std::string SQLITE_FORMAT_SDATE = "STRFTIME('%%Y-%%m-%%d','%04i-%02i-%02i')";
	static const std::string SQLITE_FORMAT_STIME = "STRFTIME('%%H:%%M:%%S','%02i:%02i:%02i')";
	static const std::string SQLITE_FORMAT_SDATETIME = "STRFTIME('%%Y-%%m-%%d %%H:%%M:%%S','%04i-%02i-%02i %02i:%02i:%02i')";
	static const std::string SQLITE_FORMAT_SDATETIME_DATE = "STRFTIME('%%Y-%%m-%%d 00:00:00','%04i-%02i-%02i')";
	static const std::string SQLITE_FORMAT_SDATETIME_TIME = "STRFTIME('0000-00-00 %%H:%%M:%%S','%02i:%02i:%02i')";
	static const std::string SQLITE_FORMAT_STMT_SDATE = "%Y-%m-%d";
	static const std::string SQLITE_FORMAT_STMT_STIME = "%H:%M:%S";
	static const std::string SQLITE_FORMAT_STMT_SDATETIME = "%Y-%m-%d %H:%M:%S";

	static const std::wstring SQLITE_FORMAT_WDATE = L"STRFTIME('%%Y-%%m-%%d','%04i-%02i-%02i')";
	static const std::wstring SQLITE_FORMAT_WTIME = L"STRFTIME('%%H:%%M:%%S','%02i:%02i:%02i')";
	static const std::wstring SQLITE_FORMAT_WDATETIME = L"STRFTIME('%%Y-%%m-%%d %%H:%%M:%%S','%04i-%02i-%02i %02i:%02i:%02i')";
	static const std::wstring SQLITE_FORMAT_WDATETIME_DATE = L"STRFTIME('%%Y-%%m-%%d 00:00:00','%04i-%02i-%02i')";
	static const std::wstring SQLITE_FORMAT_WDATETIME_TIME = L"STRFTIME('0000-00-00 %%H:%%M:%%S','%02i:%02i:%02i')";
	static const std::wstring SQLITE_FORMAT_STMT_WDATE = L"%Y-%m-%d";
	static const std::wstring SQLITE_FORMAT_STMT_WTIME = L"%H:%M:%S";
	static const std::wstring SQLITE_FORMAT_STMT_WDATETIME = L"%Y-%m-%d %H:%M:%S";

	//YYYY-mm-dd
	static const unsigned long SQLITE_STMT_DATE_SIZE = 10;
	//YYYY-mm-dd HH:MM:SS
	static const unsigned long SQLITE_STMT_DATETIME_SIZE = 19;
	//HH:MM:SS
	static const unsigned long SQLITE_STMT_TIME_SIZE = 8;

	namespace
	{
		sqlite3_stmt * PrepareStatement( const String & query, sqlite3 * connection )
		{
			sqlite3_stmt * statement = NULL;
			int code = sqlite3_prepare_v2( connection, StringUtils::ToStr( query ).c_str(), int( query.size() ), &statement, NULL );

			while ( code == SQLITE_BUSY || code == SQLITE_LOCKED )
			{
				// Tant qu'on n'a pas réussi à compiler la requête parce que la BDD est lockée ou occupée, on retente le coup
				std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
				code = sqlite3_prepare_v2( connection, StringUtils::ToStr( query ).c_str(), int( query.size() ), &statement, NULL );
			}

			if ( !statement )
			{
				StringStream stream;
				stream << ERROR_SQLITE_PREPARATION_ERROR << STR( " - " ) << StringUtils::ToString( sqlite3_errmsg( connection ) );
				DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, stream.str() );
			}

			return statement;
		}
	}

	CDatabaseConnectionSqlite::CDatabaseConnectionSqlite( const String & server, const String & userName, const String & password, String & connectionString )
		:   CDatabaseConnection( server, userName, password )
		,   _connection( NULL )
	{
		DoConnect( connectionString );
	}

	CDatabaseConnectionSqlite::~CDatabaseConnectionSqlite()
	{
		DoDisconnect();
	}

	uint32_t CDatabaseConnectionSqlite::GetPrecision( EFieldType type ) const
	{
		uint32_t result = 0;

		switch ( type )
		{
		case EFieldType_FLOAT32:
			result = 7;
			break;

		case EFieldType_FLOAT64:
			result = 15;
			break;
		}

		return result;
	}

	void CDatabaseConnectionSqlite::CreateDatabase( const String & database )
	{
		if ( !IsConnected() )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_SQLITE_NOT_CONNECTED );
		}

		String filePath = _server + PATH_SEP + database;
		String serverPath = filePath;

		StringUtils::Replace( serverPath, STR( "\\" ), PATH_SEP );
		StringUtils::Replace( serverPath, STR( "/" ), PATH_SEP );
		serverPath = serverPath.substr( 0, serverPath.find_last_of( PATH_SEP ) + 1 );

		if ( !FileExists( filePath ) )
		{
			if ( !FolderExists( serverPath ) )
			{
				CreateFolder( serverPath );
			}

			FILE * file;
#if defined( _MSC_VER )
			fopen_s( &file, StringUtils::ToStr( filePath ).c_str(), "w" );
#else
			file = fopen( StringUtils::ToStr( filePath ).c_str(), "w" );
#endif

			if ( file )
			{
				fclose( file );
				file = NULL;
			}
		}
	}

	void CDatabaseConnectionSqlite::SelectDatabase( const String & database )
	{
		if ( !IsConnected() )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_SQLITE_NOT_CONNECTED );
		}

		if ( !_database.empty() )
		{
			sqlite3_close( _connection );
		}

		SQLiteCheck( sqlite3_open( StringUtils::ToStr( _server + PATH_SEP + database ).c_str(), &_connection ), StringStream() << ERROR_SQLITE_SELECTION, EDatabaseExceptionCodes_ConnectionError, _connection );
		_database = database;
	}

	void CDatabaseConnectionSqlite::DestroyDatabase( const String & database )
	{
		if ( !IsConnected() )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_SQLITE_NOT_CONNECTED );
		}

		String filePath = _server + PATH_SEP + database;

		if ( FileExists( filePath ) )
		{
			StringUtils::Replace( filePath, STR( "\\" ), PATH_SEP );
			StringUtils::Replace( filePath, STR( "/" ), PATH_SEP );

			if ( !_database.empty() )
			{
				sqlite3_close( _connection );
			}

			try
			{
				boost::filesystem::remove( filePath );
			}
			catch ( boost::filesystem::filesystem_error & exc )
			{
				String error = ERROR_SQLITE_DESTRUCTION + StringUtils::ToString( exc.what() );
				DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, error );
			}
		}
	}

	std::string CDatabaseConnectionSqlite::WriteText( const std::string & text ) const
	{
		std::string strReturn( text );

		if ( strReturn != SQLITE_SQL_SNULL )
		{
			StringUtils::Replace( strReturn, "'", "''" );
			StringUtils::Replace( strReturn, "\\", "\\\\" );
			strReturn = "'" + strReturn + "'";
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionSqlite::WriteNText( const std::wstring & text ) const
	{
		String strReturn( StringUtils::ToString( text ) );

		if ( strReturn != SQLITE_SQL_SNULL )
		{
			StringUtils::Replace( strReturn, STR( "'" ), STR( "''" ) );
			StringUtils::Replace( strReturn, STR( "\\" ), STR( "\\\\" ) );
			strReturn = STR( "'" ) + strReturn + STR( "'" );
		}

		return StringUtils::ToWStr( strReturn );
	}

	String CDatabaseConnectionSqlite::WriteBinary( const ByteArray & array ) const
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

	String CDatabaseConnectionSqlite::WriteName( const String & text ) const
	{
		return STR( "[" ) + text + STR( "]" );
	}

	std::string CDatabaseConnectionSqlite::WriteDateS( const DateType & date ) const
	{
		std::string strReturn;

		if ( Date::IsValid( date ) )
		{
			strReturn = Date::Print( date, SQLITE_FORMAT_SDATE );
		}
		else
		{
			strReturn += SQLITE_SQL_SNULL;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionSqlite::WriteTimeS( const TimeType & time ) const
	{
		std::string strReturn;

		if ( Time::IsValid( time ) )
		{
			strReturn = Time::Print( time, SQLITE_FORMAT_STIME );
		}
		else
		{
			strReturn = SQLITE_SQL_SNULL;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionSqlite::WriteDateTimeS( const DateTimeType & dateTime ) const
	{
		std::string strReturn;

		if ( DateTime::IsValid( dateTime ) )
		{
			strReturn = DateTime::Print( dateTime, SQLITE_FORMAT_SDATETIME );
		}
		else
		{
			strReturn = SQLITE_SQL_SNULL;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionSqlite::WriteDateTimeS( const DateType & date ) const
	{
		std::string strReturn;

		if ( Date::IsValid( date ) )
		{
			strReturn = Date::Print( date, SQLITE_FORMAT_SDATETIME_DATE );
		}
		else
		{
			strReturn += SQLITE_SQL_SNULL;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionSqlite::WriteDateTimeS( const TimeType & time ) const
	{
		std::string strReturn;

		if ( Time::IsValid( time ) )
		{
			strReturn = Time::Print( time, SQLITE_FORMAT_SDATETIME_TIME );
		}
		else
		{
			strReturn = SQLITE_SQL_SNULL;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionSqlite::WriteStmtDateS( const DateType & date ) const
	{
		std::string strReturn;

		if ( Date::IsValid( date ) )
		{
			strReturn = Date::Format( date, SQLITE_FORMAT_STMT_SDATE );
		}
		else
		{
			strReturn += SQLITE_SQL_SNULL;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionSqlite::WriteStmtTimeS( const TimeType & time ) const
	{
		std::string strReturn;

		if ( Time::IsValid( time ) )
		{
			strReturn = Time::Format( time, SQLITE_FORMAT_STMT_STIME );
		}
		else
		{
			strReturn = SQLITE_SQL_SNULL;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionSqlite::WriteStmtDateTimeS( const DateTimeType & dateTime ) const
	{
		std::string strReturn;

		if ( DateTime::IsValid( dateTime ) )
		{
			strReturn = DateTime::Format( dateTime, SQLITE_FORMAT_STMT_SDATETIME );
		}
		else
		{
			strReturn = SQLITE_SQL_SNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionSqlite::WriteDateW( const DateType & date ) const
	{
		std::wstring strReturn;

		if ( Date::IsValid( date ) )
		{
			strReturn = Date::Print( date, SQLITE_FORMAT_WDATE );
		}
		else
		{
			strReturn += SQLITE_SQL_WNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionSqlite::WriteTimeW( const TimeType & time ) const
	{
		std::wstring strReturn;

		if ( Time::IsValid( time ) )
		{
			strReturn = Time::Print( time, SQLITE_FORMAT_WTIME );
		}
		else
		{
			strReturn = SQLITE_SQL_WNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionSqlite::WriteDateTimeW( const DateTimeType & dateTime ) const
	{
		std::wstring strReturn;

		if ( DateTime::IsValid( dateTime ) )
		{
			strReturn = DateTime::Print( dateTime, SQLITE_FORMAT_WDATETIME );
		}
		else
		{
			strReturn = SQLITE_SQL_WNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionSqlite::WriteDateTimeW( const DateType & date ) const
	{
		std::wstring strReturn;

		if ( Date::IsValid( date ) )
		{
			strReturn = Date::Print( date, SQLITE_FORMAT_WDATETIME_DATE );
		}
		else
		{
			strReturn += SQLITE_SQL_WNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionSqlite::WriteDateTimeW( const TimeType & time ) const
	{
		std::wstring strReturn;

		if ( Time::IsValid( time ) )
		{
			strReturn = Time::Print( time, SQLITE_FORMAT_WDATETIME_TIME );
		}
		else
		{
			strReturn = SQLITE_SQL_WNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionSqlite::WriteStmtDateW( const DateType & date ) const
	{
		std::wstring strReturn;

		if ( Date::IsValid( date ) )
		{
			strReturn = Date::Format( date, SQLITE_FORMAT_STMT_WDATE );
		}
		else
		{
			strReturn += SQLITE_SQL_WNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionSqlite::WriteStmtTimeW( const TimeType & time ) const
	{
		std::wstring strReturn;

		if ( Time::IsValid( time ) )
		{
			strReturn = Time::Format( time, SQLITE_FORMAT_STMT_WTIME );
		}
		else
		{
			strReturn = SQLITE_SQL_WNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionSqlite::WriteStmtDateTimeW( const DateTimeType & dateTime ) const
	{
		std::wstring strReturn;

		if ( DateTime::IsValid( dateTime ) )
		{
			strReturn = DateTime::Format( dateTime, SQLITE_FORMAT_STMT_WDATETIME );
		}
		else
		{
			strReturn = SQLITE_SQL_WNULL;
		}

		return strReturn;
	}

	String CDatabaseConnectionSqlite::WriteBool( bool value ) const
	{
		return ( value ? STR( "1" ) : STR( "0" ) );
	}

	String CDatabaseConnectionSqlite::WriteBool( const String & value ) const
	{
		const String lowerCaseValue = StringUtils::LowerCase( value );
		return ( lowerCaseValue == STR( "x" ) || lowerCaseValue == STR( "oui" ) || lowerCaseValue == STR( "yes" ) || lowerCaseValue == STR( "y" ) || value == STR( "1" ) ? STR( "1" ) : STR( "0" ) );
	}

	DateType CDatabaseConnectionSqlite::ParseDate( const std::string & date ) const
	{
		DateType dateObj;
		Date::IsDate( date, SQLITE_FORMAT_STMT_SDATE, dateObj );
		return dateObj;
	}

	TimeType CDatabaseConnectionSqlite::ParseTime( const std::string & time ) const
	{
		TimeType timeObj;
		Time::IsTime( time, SQLITE_FORMAT_STMT_STIME, timeObj );
		return timeObj;
	}

	DateTimeType CDatabaseConnectionSqlite::ParseDateTime( const std::string & dateTime ) const
	{
		DateTimeType dateTimeObj;
		DateTime::IsDateTime( dateTime, SQLITE_FORMAT_STMT_SDATETIME, dateTimeObj );
		return dateTimeObj;
	}

	DateType CDatabaseConnectionSqlite::ParseDate( const std::wstring & date ) const
	{
		DateType dateObj;
		Date::IsDate( date, SQLITE_FORMAT_STMT_WDATE, dateObj );
		return dateObj;
	}

	TimeType CDatabaseConnectionSqlite::ParseTime( const std::wstring & time ) const
	{
		TimeType timeObj;
		Time::IsTime( time, SQLITE_FORMAT_STMT_WTIME, timeObj );
		return timeObj;
	}

	DateTimeType CDatabaseConnectionSqlite::ParseDateTime( const std::wstring & dateTime ) const
	{
		DateTimeType dateTimeObj;
		DateTime::IsDateTime( dateTime, SQLITE_FORMAT_STMT_WDATETIME, dateTimeObj );
		return dateTimeObj;
	}

	unsigned long CDatabaseConnectionSqlite::GetStmtDateSize()const
	{
		return SQLITE_STMT_DATE_SIZE;
	}

	unsigned long CDatabaseConnectionSqlite::GetStmtDateTimeSize()const
	{
		return SQLITE_STMT_DATETIME_SIZE;
	}

	unsigned long CDatabaseConnectionSqlite::GetStmtTimeSize()const
	{
		return SQLITE_STMT_TIME_SIZE;
	}

	sqlite3 * CDatabaseConnectionSqlite::GetConnection() const
	{
		return _connection;
	}

	bool CDatabaseConnectionSqlite::ExecuteUpdate( sqlite3_stmt * statement )
	{
		return ExecuteSelect( statement ) != nullptr;
	}

	DatabaseResultSPtr CDatabaseConnectionSqlite::ExecuteSelect( sqlite3_stmt * statement )
	{
		DatabaseResultSPtr result;

		try
		{
			result = SqliteFetchResult( statement, SqliteGetColumns( statement ), std::static_pointer_cast< CDatabaseConnectionSqlite >( shared_from_this() ) );
		}
		catch ( CExceptionDatabase & exc )
		{
			CLogger::LogError( std::string( exc.what() ) );
		}

		return result;
	}

	EErrorType CDatabaseConnectionSqlite::DoConnect( String & connectionString )
	{
		DoSetConnected( true );
		return EErrorType_NONE;
	}

	void CDatabaseConnectionSqlite::DoDisconnect()
	{
		if ( IsConnected() )
		{
			DoSetConnected( false );
			sqlite3_close( _connection );
		}

		_connection = NULL;
	}

	bool CDatabaseConnectionSqlite::DoBeginTransaction( const String & name )
	{
		bool result = false;

		if ( name.empty() )
		{
			result = CDatabaseConnection::ExecuteUpdate( SQLITE_SQL_TRANSACTION_BEGIN );
		}
		else
		{
			result = CDatabaseConnection::ExecuteUpdate( SQLITE_SQL_NAMED_TRANSACTION_BEGIN + name );
		}

		return result;
	}

	bool CDatabaseConnectionSqlite::DoCommit( const String & name )
	{
		bool result = false;

		if ( name.empty() )
		{
			result = CDatabaseConnection::ExecuteUpdate( SQLITE_SQL_TRANSACTION_COMMIT );
		}
		else
		{
			result = CDatabaseConnection::ExecuteUpdate( SQLITE_SQL_NAMED_TRANSACTION_COMMIT + name );
		}

		return result;
	}

	bool CDatabaseConnectionSqlite::DoRollBack( const String & name )
	{
		bool result = false;

		if ( name.empty() )
		{
			result = CDatabaseConnection::ExecuteUpdate( SQLITE_SQL_TRANSACTION_ROLLBACK );
		}
		else
		{
			result = CDatabaseConnection::ExecuteUpdate( SQLITE_SQL_NAMED_TRANSACTION_ROLLBACK + name );
		}

		return result;
	}

	bool CDatabaseConnectionSqlite::DoExecuteUpdate( const String & query)
	{
		bool ret = false;

		try
		{
			sqlite3_stmt * statement = PrepareStatement( query, _connection );
			ret = ExecuteUpdate( statement );
			SQLiteCheck( sqlite3_finalize( statement ), INFO_SQLITE_STATEMENT_FINALISATION, EDatabaseExceptionCodes_ConnectionError, _connection );
		}
		catch ( CExceptionDatabase & exc )
		{
			CLogger::LogError( exc.what() );
		}
		catch ( std::exception & exc )
		{
			StringStream stream;
			stream << ERROR_SQLITE_EXECUTION_ERROR << STR( " - " ) << exc.what();
			CLogger::LogError( stream );
		}

		return ret;
	}

	DatabaseResultSPtr CDatabaseConnectionSqlite::DoExecuteSelect( const String & query)
	{
		DatabaseResultSPtr ret;

		try
		{
			sqlite3_stmt * statement = PrepareStatement( query, _connection );
			ret = ExecuteSelect( statement );
			SQLiteCheck( sqlite3_finalize( statement ), INFO_SQLITE_STATEMENT_FINALISATION, EDatabaseExceptionCodes_ConnectionError, _connection );
		}
		catch ( CExceptionDatabase & exc )
		{
			CLogger::LogError( exc.what() );
		}
		catch ( std::exception & exc )
		{
			StringStream stream;
			stream << ERROR_SQLITE_EXECUTION_ERROR << STR( " - " ) << exc.what();
			CLogger::LogError( stream );
		}

		return ret;
	}

	DatabaseStatementSPtr CDatabaseConnectionSqlite::DoCreateStatement( const String & query )
	{
		return std::make_shared< CDatabaseStatementSqlite >( std::static_pointer_cast< CDatabaseConnectionSqlite >( shared_from_this() ), query );
	}
}
END_NAMESPACE_DATABASE_SQLITE
