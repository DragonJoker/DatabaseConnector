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

#include <DatabaseQuery.h>
#include <DatabaseDate.h>
#include <DatabaseDateTime.h>
#include <DatabaseDateTimeHelper.h>
#include <DatabaseTime.h>

#include <DatabaseStringUtils.h>
#include <DatabaseLogger.h>

#include "sqlite3.h"

#if defined( _WIN32 )
#   include <Windows.h>
#endif

BEGIN_NAMESPACE_DATABASE_SQLITE
{
	static const String SQLITE_TRANSACTION_BEGIN = STR( "BEGIN TRANSACTION " );
	static const String SQLITE_TRANSACTION_COMMIT = STR( "COMMIT TRANSACTION " );
	static const String SQLITE_TRANSACTION_ROLLBACK = STR( "ROLLBACK TRANSACTION " );

	static const String ERROR_SQLITE_CONNECTION = STR( "Couldn't create the connection" );
	static const String ERROR_SQLITE_EXECUTION_ERROR = STR( "Couldn't execute the query" );
	static const String ERROR_SQLITE_UNKNOWN_ERROR = STR( "Unknown error" );
	static const String ERROR_SQLITE_NOT_CONNECTED = STR( "Not connected" );
	static const String ERROR_SQLITE_ALREADY_IN_TRANSACTION = STR( "Already in a transaction" );
	static const String ERROR_SQLITE_NOT_IN_TRANSACTION = STR( "Not in a transaction" );

	static const std::string SQLITE_NULL_STDSTRING = "NULL";
	static const std::wstring SQLITE_NULL_STDWSTRING = L"NULL";
	static const String SQLITE_NULL_STRING = STR( "NULL" );

	CDatabaseConnectionSqlite::CDatabaseConnectionSqlite( const String & server, const String & database, const String & userName, const String & password, String & connectionString )
		:   CDatabaseConnection( server, database, userName, password )
		,   _connection( NULL )
	{
		DoConnect( connectionString );
	}

	CDatabaseConnectionSqlite::~CDatabaseConnectionSqlite()
	{
		DoDisconnect();
	}

	EErrorType CDatabaseConnectionSqlite::BeginTransaction( const String & name )
	{
		EErrorType eReturn = EErrorType_ERROR;

		if ( !IsConnected() )
		{
			CLogger::LogError( ERROR_SQLITE_NOT_CONNECTED );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_SQLITE_NOT_CONNECTED, __FUNCTION__, __FILE__, __LINE__ );
		}

		if ( IsInTransaction() )
		{
			CLogger::LogError( ERROR_SQLITE_ALREADY_IN_TRANSACTION );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_SQLITE_ALREADY_IN_TRANSACTION, __FUNCTION__, __FILE__, __LINE__ );
		}

		if ( name.empty() )
		{
			CDatabaseConnection::ExecuteUpdate( STR( "BEGIN TRANSACTION" ), &eReturn );
		}
		else
		{
			CDatabaseConnection::ExecuteUpdate( STR( "SAVEPOINT " ) + name, &eReturn );
		}

		return eReturn;
	}

	EErrorType CDatabaseConnectionSqlite::Commit( const String & name )
	{
		EErrorType eReturn = EErrorType_ERROR;

		if ( !IsConnected() )
		{
			CLogger::LogError( ERROR_SQLITE_NOT_CONNECTED );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_SQLITE_NOT_CONNECTED, __FUNCTION__, __FILE__, __LINE__ );
		}

		if ( !IsInTransaction() )
		{
			CLogger::LogError( ERROR_SQLITE_NOT_IN_TRANSACTION );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_SQLITE_NOT_IN_TRANSACTION, __FUNCTION__, __FILE__, __LINE__ );
		}

		if ( name.empty() )
		{
			CDatabaseConnection::ExecuteUpdate( STR( "COMMIT" ), &eReturn );
		}
		else
		{
			CDatabaseConnection::ExecuteUpdate( STR( "RELEASE " ) + name, &eReturn );
		}

		return eReturn;
	}

	EErrorType CDatabaseConnectionSqlite::RollBack( const String & name )
	{
		EErrorType eReturn = EErrorType_ERROR;

		if ( !IsConnected() )
		{
			CLogger::LogError( ERROR_SQLITE_NOT_CONNECTED );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_SQLITE_NOT_CONNECTED, __FUNCTION__, __FILE__, __LINE__ );
		}

		if ( !IsInTransaction() )
		{
			CLogger::LogError( ERROR_SQLITE_NOT_IN_TRANSACTION );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_SQLITE_NOT_IN_TRANSACTION, __FUNCTION__, __FILE__, __LINE__ );
		}

		if ( name.empty() )
		{
			CDatabaseConnection::ExecuteUpdate( STR( "ROLLBACK" ), &eReturn );
		}
		else
		{
			CDatabaseConnection::ExecuteUpdate( STR( "ROLLBACK TO " ) + name, &eReturn );
		}

		return eReturn;
	}

	std::string CDatabaseConnectionSqlite::WriteText( const std::string & text ) const
	{
		std::string strReturn( text );

		if ( strReturn != SQLITE_NULL_STDSTRING )
		{
			Replace( strReturn, "'", "''" );
			Replace( strReturn, "\\", "\\\\" );
			strReturn = "'" + strReturn + "'";
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionSqlite::WriteNText( const std::wstring & text ) const
	{
		String strReturn( CStrUtils::ToString( text ) );

		if ( strReturn != SQLITE_NULL_STRING )
		{
			Replace( strReturn, STR( "'" ), STR( "''" ) );
			Replace( strReturn, STR( "\\" ), STR( "\\\\" ) );
			strReturn = STR( "N'" ) + strReturn + STR( "'" );
		}

		return CStrUtils::ToWStr( strReturn );
	}

	String CDatabaseConnectionSqlite::WriteName( const String & text ) const
	{
		return STR( "[" ) + text + STR( "]" );
	}

	std::string CDatabaseConnectionSqlite::WriteDate( const CDate & date ) const
	{
		std::string strReturn;

		if ( date.IsValid() )
		{
			Formalize( strReturn, 1024, "STRFTIME('%%Y-%%m-%%d','%04i-%02i-%02i')", date.GetYear(), date.GetMonth(), date.GetMonthDay() );
		}
		else
		{
			strReturn += SQLITE_NULL_STDSTRING;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionSqlite::WriteDate( const std::string & date, const std::string & format ) const
	{
		std::string strReturn;
		CDate dateObj;

		if ( CDate::IsDate( date, format, dateObj ) )
		{
			strReturn = WriteDate( dateObj );
		}
		else
		{
			strReturn += SQLITE_NULL_STDSTRING;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionSqlite::WriteTime( const CTime & time ) const
	{
		std::string strReturn;

		if ( time.IsValid() )
		{
			Formalize( strReturn, 1024, "STRFTIME('%%H:%%M:%%S','%02i:%02i:%02i')", time.GetHour(), time.GetMinute(), time.GetSecond() );
		}
		else
		{
			strReturn += SQLITE_NULL_STDSTRING;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionSqlite::WriteTime( const std::string & time, const std::string & format ) const
	{
		std::string strReturn;
		CTime timeObj;

		if ( CTime::IsTime( time, format, timeObj ) )
		{
			strReturn = WriteTime( timeObj );
		}
		else
		{
			strReturn += SQLITE_NULL_STDSTRING;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionSqlite::WriteDateTime( const CDateTime & dateTime ) const
	{
		std::string strReturn;

		if ( dateTime.GetYear() <= 0 )
		{
			strReturn += SQLITE_NULL_STDSTRING;
		}
		else
		{
			Formalize( strReturn, 1024, "STRFTIME('%%Y-%%m-%%d %%H:%%M:%%S','%04i-%02i-%02i %02i:%02i:%02i')", dateTime.GetYear(), dateTime.GetMonth(), dateTime.GetMonthDay(), dateTime.GetHour(), dateTime.GetMinute(), dateTime.GetSecond() );
		}

		return strReturn;
	}

	std::string CDatabaseConnectionSqlite::WriteDateTime( const CDate & date ) const
	{
		std::string strReturn;

		if ( !date.IsValid() || date.GetYear() <= 0 )
		{
			strReturn += SQLITE_NULL_STDSTRING;
		}
		else
		{
			Formalize( strReturn, 1024, "STRFTIME('%%Y-%%m-%%d 00:00:00','%04i-%02i-%02i %02i:%02i:%02i')", date.GetYear(), date.GetMonth(), date.GetMonthDay() );
		}

		return strReturn;
	}

	std::string CDatabaseConnectionSqlite::WriteDateTime( const CTime & time ) const
	{
		std::string strReturn;

		if ( !time.IsValid() )
		{
			strReturn += SQLITE_NULL_STDSTRING;
		}
		else
		{
			Formalize( strReturn, 1024, "STRFTIME('0000-00-00 %%H:%%M:%%S','%02i:%02i:%02i')", time.GetHour(), time.GetMinute(), time.GetSecond() );
		}

		return strReturn;
	}

	std::string CDatabaseConnectionSqlite::WriteDateTime( const std::string & dateTime, const std::string & format ) const
	{
		std::string strReturn;
		CDateTime dateTimeObj;

		if ( CDateTime::IsDateTime( dateTime,  dateTimeObj ) )
		{
			strReturn = WriteDateTime( dateTimeObj );
		}
		else
		{
			strReturn += SQLITE_NULL_STDSTRING;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionSqlite::WriteStmtDate( const CDate & date ) const
	{
		return WriteDate( date );
	}

	std::string CDatabaseConnectionSqlite::WriteStmtDate( const std::string & date, const std::string & format ) const
	{
		std::string strReturn;
		CDate dateObj;

		if ( CDate::IsDate( date, format, dateObj ) )
		{
			strReturn = WriteStmtDate( dateObj );
		}
		else
		{
			strReturn += SQLITE_NULL_STDSTRING;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionSqlite::WriteStmtTime( const CTime & time ) const
	{
		return WriteTime( time );
	}

	std::string CDatabaseConnectionSqlite::WriteStmtTime( const std::string & time, const std::string & format ) const
	{
		std::string strReturn;
		CTime timeObj;

		if ( CTime::IsTime( time, format, timeObj ) )
		{
			strReturn = WriteStmtTime( timeObj );
		}
		else
		{
			strReturn += SQLITE_NULL_STDSTRING;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionSqlite::WriteStmtDateTime( const CDateTime & dateTime ) const
	{
		return WriteDateTime( dateTime );
	}

	std::string CDatabaseConnectionSqlite::WriteStmtDateTime( const std::string & dateTime, const std::string & format ) const
	{
		std::string strReturn;
		CDateTime dateTimeObj;

		if ( CDateTime::IsDateTime( dateTime, dateTimeObj ) )
		{
			strReturn = WriteStmtDateTime( dateTimeObj );
		}
		else
		{
			strReturn += SQLITE_NULL_STDSTRING;
		}

		return strReturn;
	}

	String CDatabaseConnectionSqlite::WriteBool( bool value ) const
	{
		return ( value ? STR( "1" ) : STR( "0" ) );
	}

	String CDatabaseConnectionSqlite::WriteBool( const String & value ) const
	{
		const String lowerCaseValue = CStrUtils::LowerCase( value );
		return ( lowerCaseValue == STR( "x" ) || lowerCaseValue == STR( "oui" ) || lowerCaseValue == STR( "yes" ) || lowerCaseValue == STR( "y" ) || value == STR( "1" ) ? STR( "1" ) : STR( "0" ) );
	}

	CDate CDatabaseConnectionSqlite::ParseDate( const std::string & date ) const
	{
		CDate dtReturn;

		if ( date.size() >= 10 )
		{
			if ( date != "1800-01-01" )
			{
				std::string stdDate = CStrUtils::ToStr( date );
				dtReturn = CDate( stoi( stdDate.substr( 0, 4 ) ), EDateMonth( stoi( stdDate.substr( 5, 2 ) ) ), stoi( stdDate.substr( 8, 2 ) ) );
			}
		}

		return dtReturn;
	}

	CTime CDatabaseConnectionSqlite::ParseTime( const std::string & time ) const
	{
		CTime timeObj;
		CTime::IsTime( time, timeObj );
		return timeObj;
	}

	CDateTime CDatabaseConnectionSqlite::ParseDateTime( const std::string & dateTime ) const
	{
		CDateTime dateTimeObj;
		CDateTime::IsDateTime( dateTime, dateTimeObj );
		return dateTimeObj;
	}

	CDate CDatabaseConnectionSqlite::ParseDate( const std::wstring & date ) const
	{
		CDate dtReturn;

		if ( date.size() >= 10 )
		{
			if ( date != L"1800-01-01" )
			{
				std::wstring stdDate = CStrUtils::ToWStr( date );
				dtReturn = CDate( stoi( stdDate.substr( 0, 4 ) ), EDateMonth( stoi( stdDate.substr( 5, 2 ) ) ), stoi( stdDate.substr( 8, 2 ) ) );
			}
		}

		return dtReturn;
	}

	CTime CDatabaseConnectionSqlite::ParseTime( const std::wstring & time ) const
	{
		CTime timeObj;
		CTime::IsTime( time, timeObj );
		return timeObj;
	}

	CDateTime CDatabaseConnectionSqlite::ParseDateTime( const std::wstring & dateTime ) const
	{
		CDateTime dateTimeObj;
		CDateTime::IsDateTime( dateTime, dateTimeObj );
		return dateTimeObj;
	}

	SQLite::Database * CDatabaseConnectionSqlite::GetConnection() const
	{
		return _connection;
	}

	bool CDatabaseConnectionSqlite::ExecuteUpdate( SQLite::Statement * statement )
	{
		return ExecuteSelect( statement ) != nullptr;
	}

	DatabaseResultPtr CDatabaseConnectionSqlite::ExecuteSelect( SQLite::Statement * statement )
	{
		DatabaseResultPtr result;

		try
		{
			SQLite::eCODE code = SQLite::eCODE_ERROR;
			DatabaseConnectionPtr connection = shared_from_this();
			DatabaseFieldInfosPtrArray columns = SqliteGetColumns( statement, connection );
			result = SqliteExecute( statement, code, columns, connection );
		}
		catch ( CExceptionDatabase & exc )
		{
			CLogger::LogError( std::string( exc.what() ) );
		}

		return result;
	}

	EErrorType CDatabaseConnectionSqlite::DoConnect( String & connectionString )
	{
		EErrorType eReturn = EErrorType_ERROR;
		String filePath = _server + PATH_SEP + _database;
		String serverPath = filePath;

		CStrUtils::Replace( serverPath, STR( "\\" ), PATH_SEP );
		CStrUtils::Replace( serverPath, STR( "/" ), PATH_SEP );
		serverPath = serverPath.substr( 0, serverPath.find_last_of( PATH_SEP ) + 1 );

		if ( !FileExists( filePath ) )
		{
			if ( !FolderExists( serverPath ) )
			{
				CreateFolder( serverPath );
			}

			FILE * file;
#if defined( _MSC_VER )
			fopen_s( &file, CStrUtils::ToStr( filePath ).c_str(), "w" );
#else
			file = fopen( CStrUtils::ToStr( filePath ).c_str(), "w" );
#endif

			if ( file )
			{
				fclose( file );
				file = NULL;
			}
		}

		SQLite::eCODE code = SQLite::Open( CStrUtils::ToStr( filePath ).c_str(), &_connection );

		if ( code != SQLite::eCODE_OK )
		{
			connectionString = STR( "Erreur de connexion à la base de données : \n" );
			connectionString += CStrUtils::ToString( SQLite::Errmsg( _connection ) );
			CLogger::LogMessage( connectionString );
			DoSetConnected( false );
		}
		else
		{
			DoSetConnected( true );
			eReturn = EErrorType_NONE;
		}

		return eReturn;
	}

	void CDatabaseConnectionSqlite::DoDisconnect()
	{
		if ( IsConnected() )
		{
			DoSetConnected( false );
			SQLite::Close( _connection );
		}

		_connection = NULL;
	}

	bool CDatabaseConnectionSqlite::DoExecuteUpdate( const String & query, EErrorType * result )
	{
		if ( !IsConnected() )
		{
			CLogger::LogError( ERROR_SQLITE_NOT_CONNECTED );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_SQLITE_NOT_CONNECTED, __FUNCTION__, __FILE__, __LINE__ );
		}

		CLogger::LogMessage( STR( "Executing update : " ) + query );
		bool ret = false;

		try
		{
			SQLite::Statement * statement = NULL;
			SQLite::eCODE code = SQLite::Prepare( _connection, CStrUtils::ToStr( query ).c_str(), int( query.size() ), &statement, NULL );

			while ( code == SQLite::eCODE_BUSY || code == SQLite::eCODE_LOCKED )
			{
				// Tant qu'on n'a pas réussi à compiler la requête parce que la BDD est lockée ou occupée, on retente le coup
#if !defined( _WIN32 )
				usleep( 1000 );
#else
				Sleep( 1 );
#endif
				code = SQLite::Prepare( _connection, CStrUtils::ToStr( query ).c_str(), int( query.size() ), &statement, NULL );
			}

			if ( !statement )
			{
				CLogger::LogMessage( STR( "Erreur de requête : \n" ) + query + STR( "\n" ) + CStrUtils::ToString( SQLite::Errmsg( _connection ) ) );
			}
			else
			{
				ret = ExecuteUpdate( statement );
				SQLite::Finalize( statement );
			}
		}
		catch ( std::exception & exc )
		{
			StringStream stream;
			stream << ERROR_SQLITE_EXECUTION_ERROR << STR( " - " ) << exc.what();
			CLogger::LogError( stream );
		}

		return ret;
	}

	DatabaseResultPtr CDatabaseConnectionSqlite::DoExecuteSelect( const String & query, EErrorType * result )
	{
		if ( !IsConnected() )
		{
			CLogger::LogError( ERROR_SQLITE_NOT_CONNECTED );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_SQLITE_NOT_CONNECTED, __FUNCTION__, __FILE__, __LINE__ );
		}

		CLogger::LogMessage( STR( "Executing update : " ) + query );
		DatabaseResultPtr ret;

		try
		{
			SQLite::Statement * statement = NULL;
			SQLite::eCODE code = SQLite::Prepare( _connection, CStrUtils::ToStr( query ).c_str(), int( query.size() ), &statement, NULL );

			while ( code == SQLite::eCODE_BUSY || code == SQLite::eCODE_LOCKED )
			{
				// Tant qu'on n'a pas réussi à compiler la requête parce que la BDD est lockée ou occupée, on retente le coup
#if !defined( _WIN32 )
				usleep( 1000 );
#else
				Sleep( 1 );
#endif
				code = SQLite::Prepare( _connection, CStrUtils::ToStr( query ).c_str(), int( query.size() ), &statement, NULL );
			}

			if ( !statement )
			{
				CLogger::LogMessage( STR( "Erreur de requête : \n" ) + query + STR( "\n" ) + CStrUtils::ToString( SQLite::Errmsg( _connection ) ) );
			}
			else
			{
				ret = ExecuteSelect( statement );
				SQLite::Finalize( statement );
			}
		}
		catch ( std::exception & exc )
		{
			StringStream stream;
			stream << ERROR_SQLITE_EXECUTION_ERROR << STR( " - " ) << exc.what();
			CLogger::LogError( stream );
		}

		return ret;
	}

	DatabaseStatementPtr CDatabaseConnectionSqlite::DoCreateStatement( const String & request, EErrorType * result )
	{
		DatabaseStatementPtr pReturn;
		EErrorType eResult = EErrorType_ERROR;

		if ( !IsConnected() )
		{
			CLogger::LogError( ERROR_SQLITE_NOT_CONNECTED );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_SQLITE_NOT_CONNECTED, __FUNCTION__, __FILE__, __LINE__ );
		}

		pReturn = std::make_shared< CDatabaseStatementSqlite >( shared_from_this(), request );
		eResult = EErrorType_NONE;

		if ( result )
		{
			*result = eResult;
		}

		return pReturn;
	}

	DatabaseQueryPtr CDatabaseConnectionSqlite::DoCreateQuery( const String & request, EErrorType * result )
	{
		DatabaseQueryPtr pReturn;
		EErrorType eResult = EErrorType_ERROR;

		if ( !IsConnected() )
		{
			CLogger::LogError( ERROR_SQLITE_NOT_CONNECTED );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_SQLITE_NOT_CONNECTED, __FUNCTION__, __FILE__, __LINE__ );
		}

		pReturn = std::make_shared< CDatabaseQuery >( shared_from_this(), request );
		eResult = EErrorType_NONE;

		if ( result )
		{
			*result = eResult;
		}

		return pReturn;
	}
}
END_NAMESPACE_DATABASE_SQLITE
