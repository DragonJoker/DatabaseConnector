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

#include <DatabaseFileUtils.h>
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
	static const String SQLITE_SQL_NULL = STR( "NULL" );

	static const TChar * INFO_SQLITE_STATEMENT_FINALISATION = STR( "Statement finalisation" );
	static const TChar * INFO_SQLITE_EXECUTING_SELECT = STR( "Executing select : " );
	static const TChar * INFO_SQLITE_SELECTION = STR( "Database selection" );

	static const String ERROR_SQLITE_CONNECTION = STR( "Couldn't create the connection" );
	static const String ERROR_SQLITE_DESTRUCTION = STR( "Couldn't destroy the database: " );
	static const String ERROR_SQLITE_EXECUTION_ERROR = STR( "Couldn't execute the query" );
	static const String ERROR_SQLITE_UNKNOWN_ERROR = STR( "Unknown error" );
	static const String ERROR_SQLITE_NOT_CONNECTED = STR( "Not connected" );
	static const String ERROR_SQLITE_ALREADY_IN_TRANSACTION = STR( "Already in a transaction" );
	static const String ERROR_SQLITE_NOT_IN_TRANSACTION = STR( "Not in a transaction" );

	static const String SQLITE_FORMAT_DATE = STR( "STRFTIME('%%Y-%%m-%%d','%04i-%02i-%02i')" );
	static const String SQLITE_FORMAT_TIME = STR( "STRFTIME('%%H:%%M:%%S','%02i:%02i:%02i')" );
	static const String SQLITE_FORMAT_DATETIME = STR( "STRFTIME('%%Y-%%m-%%d %%H:%%M:%%S','%04i-%02i-%02i %02i:%02i:%02i')" );
	static const String SQLITE_FORMAT_DATETIME_DATE = STR( "STRFTIME('%%Y-%%m-%%d 00:00:00','%04i-%02i-%02i')" );
	static const String SQLITE_FORMAT_DATETIME_TIME = STR( "STRFTIME('0000-00-00 %%H:%%M:%%S','%02i:%02i:%02i')" );
	static const String SQLITE_FORMAT_STMT_DATE = STR( "%Y-%m-%d" );
	static const String SQLITE_FORMAT_STMT_TIME = STR( "%H:%M:%S" );
	static const String SQLITE_FORMAT_STMT_DATETIME = STR( "%Y-%m-%d %H:%M:%S" );

	//YYYY-mm-dd
	static const unsigned long SQLITE_STMT_DATE_SIZE = 10;
	//YYYY-mm-dd HH:MM:SS
	static const unsigned long SQLITE_STMT_DATETIME_SIZE = 19;
	//HH:MM:SS
	static const unsigned long SQLITE_STMT_TIME_SIZE = 8;

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
		catch ( CDatabaseException & exc )
		{
			CLogger::LogError( std::string( exc.what() ) );
		}

		return result;
	}

	void CDatabaseConnectionSqlite::DoCreateDatabase( const String & database )
	{
		String filePath = _server + PATH_SEP + database;
		String serverPath = filePath;

		StringUtils::Replace( serverPath, STR( "\\" ), PATH_SEP );
		StringUtils::Replace( serverPath, STR( "/" ), PATH_SEP );
		serverPath = serverPath.substr( 0, serverPath.find_last_of( PATH_SEP ) + 1 );

		if ( !FileUtils::FileExists( filePath ) )
		{
			if ( !FileUtils::FolderExists( serverPath ) )
			{
				FileUtils::CreateFolder( serverPath );
			}

			FILE * file;
			FileUtils::FOpen( file, StringUtils::ToStr( filePath ).c_str(), "w" );

			if ( file )
			{
				fclose( file );
				file = NULL;
			}
		}
	}

	void CDatabaseConnectionSqlite::DoSelectDatabase( const String & database )
	{
		if ( !_database.empty() )
		{
			sqlite3_close( _connection );
		}

		SQLiteCheck( sqlite3_open( StringUtils::ToStr( _server + PATH_SEP + database ).c_str(), &_connection ), StringStream() << INFO_SQLITE_SELECTION, EDatabaseExceptionCodes_ConnectionError, _connection );
		_database = database;
	}

	void CDatabaseConnectionSqlite::DoDestroyDatabase( const String & database )
	{
		String filePath = _server + PATH_SEP + database;

		if ( FileUtils::FileExists( filePath ) )
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

	std::string CDatabaseConnectionSqlite::DoWriteText( const std::string & text ) const
	{
		std::string strReturn( text );

		if ( strReturn != SQLITE_SQL_NULL )
		{
			StringUtils::Replace( strReturn, "'", "''" );
			StringUtils::Replace( strReturn, "\\", "\\\\" );
			strReturn = "'" + strReturn + "'";
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionSqlite::DoWriteNText( const std::wstring & text ) const
	{
		String strReturn( StringUtils::ToString( text ) );

		if ( strReturn != SQLITE_SQL_NULL )
		{
			StringUtils::Replace( strReturn, STR( "'" ), STR( "''" ) );
			StringUtils::Replace( strReturn, STR( "\\" ), STR( "\\\\" ) );
			strReturn = STR( "'" ) + strReturn + STR( "'" );
		}

		return StringUtils::ToWStr( strReturn );
	}

	String CDatabaseConnectionSqlite::DoWriteBinary( const ByteArray & array ) const
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

	String CDatabaseConnectionSqlite::DoWriteName( const String & text ) const
	{
		return STR( "[" ) + text + STR( "]" );
	}

	String CDatabaseConnectionSqlite::DoWriteDate( const DateType & date ) const
	{
		String strReturn;

		if ( Date::IsValid( date ) )
		{
			strReturn = Date::Print( date, SQLITE_FORMAT_DATE );
		}
		else
		{
			strReturn += SQLITE_SQL_NULL;
		}

		return strReturn;
	}

	String CDatabaseConnectionSqlite::DoWriteTime( const TimeType & time ) const
	{
		String strReturn;

		if ( Time::IsValid( time ) )
		{
			strReturn = Time::Print( time, SQLITE_FORMAT_TIME );
		}
		else
		{
			strReturn = SQLITE_SQL_NULL;
		}

		return strReturn;
	}

	String CDatabaseConnectionSqlite::DoWriteDateTime( const DateTimeType & dateTime ) const
	{
		String strReturn;

		if ( DateTime::IsValid( dateTime ) )
		{
			strReturn = DateTime::Print( dateTime, SQLITE_FORMAT_DATETIME );
		}
		else
		{
			strReturn = SQLITE_SQL_NULL;
		}

		return strReturn;
	}

	String CDatabaseConnectionSqlite::DoWriteDateTime( const DateType & date ) const
	{
		String strReturn;

		if ( Date::IsValid( date ) )
		{
			strReturn = Date::Print( date, SQLITE_FORMAT_DATETIME_DATE );
		}
		else
		{
			strReturn += SQLITE_SQL_NULL;
		}

		return strReturn;
	}

	String CDatabaseConnectionSqlite::DoWriteDateTime( const TimeType & time ) const
	{
		String strReturn;

		if ( Time::IsValid( time ) )
		{
			strReturn = Time::Print( time, SQLITE_FORMAT_DATETIME_TIME );
		}
		else
		{
			strReturn = SQLITE_SQL_NULL;
		}

		return strReturn;
	}

	String CDatabaseConnectionSqlite::DoWriteStmtDate( const DateType & date ) const
	{
		String strReturn;

		if ( Date::IsValid( date ) )
		{
			strReturn = Date::Format( date, SQLITE_FORMAT_STMT_DATE );
		}
		else
		{
			strReturn += SQLITE_SQL_NULL;
		}

		return strReturn;
	}

	String CDatabaseConnectionSqlite::DoWriteStmtTime( const TimeType & time ) const
	{
		String strReturn;

		if ( Time::IsValid( time ) )
		{
			strReturn = Time::Format( time, SQLITE_FORMAT_STMT_TIME );
		}
		else
		{
			strReturn = SQLITE_SQL_NULL;
		}

		return strReturn;
	}

	String CDatabaseConnectionSqlite::DoWriteStmtDateTime( const DateTimeType & dateTime ) const
	{
		String strReturn;

		if ( DateTime::IsValid( dateTime ) )
		{
			strReturn = DateTime::Format( dateTime, SQLITE_FORMAT_STMT_DATETIME );
		}
		else
		{
			strReturn = SQLITE_SQL_NULL;
		}

		return strReturn;
	}

	String CDatabaseConnectionSqlite::DoWriteBool( bool value ) const
	{
		return ( value ? STR( "1" ) : STR( "0" ) );
	}

	DateType CDatabaseConnectionSqlite::DoParseDate( const String & date ) const
	{
		DateType dateObj;
		Date::IsDate( date, SQLITE_FORMAT_STMT_DATE, dateObj );
		return dateObj;
	}

	TimeType CDatabaseConnectionSqlite::DoParseTime( const String & time ) const
	{
		TimeType timeObj;
		Time::IsTime( time, SQLITE_FORMAT_STMT_TIME, timeObj );
		return timeObj;
	}

	DateTimeType CDatabaseConnectionSqlite::DoParseDateTime( const String & dateTime ) const
	{
		DateTimeType dateTimeObj;
		DateTime::IsDateTime( dateTime, SQLITE_FORMAT_STMT_DATETIME, dateTimeObj );
		return dateTimeObj;
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
			sqlite3_stmt * statement = SqlitePrepareStatement( query, _connection );
			ret = ExecuteUpdate( statement );
			SQLiteCheck( sqlite3_finalize( statement ), INFO_SQLITE_STATEMENT_FINALISATION, EDatabaseExceptionCodes_ConnectionError, _connection );
		}
		catch ( CDatabaseException & exc )
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
			sqlite3_stmt * statement = SqlitePrepareStatement( query, _connection );
			ret = ExecuteSelect( statement );
			SQLiteCheck( sqlite3_finalize( statement ), INFO_SQLITE_STATEMENT_FINALISATION, EDatabaseExceptionCodes_ConnectionError, _connection );
		}
		catch ( CDatabaseException & exc )
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
