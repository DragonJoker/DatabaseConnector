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
	static const String ERROR_POSTGRESQL_EXECUTION = STR( "Couldn't execute the query" );
	static const String ERROR_POSTGRESQL_NOT_CONNECTED = STR( "Not connected" );
	static const String ERROR_POSTGRESQL_ALREADY_IN_TRANSACTION = STR( "Already in a transaction" );
	static const String ERROR_POSTGRESQL_NOT_IN_TRANSACTION = STR( "Not in a transaction" );

	static const String WARNING_POSTGRESQL_UNKNOWN_OPTION = STR( "Unsupported option: " );
	static const String WARNING_POSTGRESQL_INIT_COMMAND = STR( "POSTGRESQL_INIT_COMMAND" );
	static const String WARNING_POSTGRESQL_SET_CHARSET_NAME = STR( "POSTGRESQL_SET_CHARSET_NAME" );
	static const String WARNING_POSTGRESQL_OPT_CONNECT_TIMEOUT = STR( "POSTGRESQL_OPT_CONNECT_TIMEOUT" );

	static const TChar * INFO_POSTGRESQL_STATEMENT_EXECUTION = STR( "Statement execution" );
	static const TChar * INFO_POSTGRESQL_STATEMENT_PREPARATION = STR( "Statement preparation" );
	static const TChar * INFO_POSTGRESQL_DATABASE_SELECTION = STR( "Database selection" );
	static const TChar * INFO_ESCAPING_TEXT = STR( "Escaping text" );
	static const TChar * INFO_ESCAPING_IDENTIFIER = STR( "Escaping identifier" );
	static const String INFO_POSTGRESQL_EXECUTING_UPDATE = STR( "Executing update: " );
	static const String INFO_POSTGRESQL_EXECUTING_SELECT = STR( "Executing select: " );

	static const String DEBUG_POSTGRESQL_CONNECTED = STR( "Connected to database" );
	static const String DEBUG_POSTGRESQL_CLIENT_VERSION = STR( "Client Version: " );
	static const String DEBUG_POSTGRESQL_SERVER_HOST = STR( "Server Host: " );
	static const String DEBUG_POSTGRESQL_SERVER_VERSION = STR( "Server Version: " );

	static const String POSTGRESQL_SQL_CREATE_DATABASE = STR( "CREATE DATABASE " );
	static const String POSTGRESQL_SQL_CHARSET = STR( " CHARACTER SET utf8 COLLATE utf8_general_ci" );
	static const String POSTGRESQL_SQL_USE = STR( "USE " );
	static const String POSTGRESQL_SQL_DROP_DATABASE = STR( "DROP DATABASE " );

	static const std::string POSTGRESQL_SQL_SNULL = "NULL";
	static const std::wstring POSTGRESQL_SQL_WNULL = L"NULL";

	static const char * POSTGRESQL_OPTION_NAMES = "SET NAMES 'utf8'";
	static const char * POSTGRESQL_OPTION_UTF8 = "utf8";

	static const std::string POSTGRESQL_FORMAT_SDATE = "CAST('%Y-%m-%d' AS DATE)";
	static const std::string POSTGRESQL_FORMAT_STIME = "CAST('%H:%M:%S' AS TIME)";
	static const std::string POSTGRESQL_FORMAT_SDATETIME = "CAST('%Y-%m-%d %H:%M:%S' AS DATETIME)";
	static const std::string POSTGRESQL_FORMAT_SDATETIME_DATE = "CAST('%Y-%m-%d 00:00:00' AS DATETIME)";
	static const std::string POSTGRESQL_FORMAT_SDATETIME_TIME = "CAST('0000-00-00 %H:%M:%S' AS DATETIME)";
	static const std::string POSTGRESQL_FORMAT_STMT_SDATE = "{-d %Y-%m-%d}";
	static const std::string POSTGRESQL_FORMAT_STMT_STIME = "{-t %H:%M:%S}";
	static const std::string POSTGRESQL_FORMAT_STMT_SDATETIME = "{-ts %Y-%m-%d %H:%M:%S}";

	static const std::wstring POSTGRESQL_FORMAT_WDATE = L"CAST('%Y-%m-%d' AS DATE)";
	static const std::wstring POSTGRESQL_FORMAT_WTIME = L"CAST('%H:%M:%S' AS TIME)";
	static const std::wstring POSTGRESQL_FORMAT_WDATETIME = L"CAST('%Y-%m-%d %H:%M:%S' AS DATETIME)";
	static const std::wstring POSTGRESQL_FORMAT_WDATETIME_DATE = L"CAST('%Y-%m-%d 00:00:00' AS DATETIME)";
	static const std::wstring POSTGRESQL_FORMAT_WDATETIME_TIME = L"CAST('0000-00-00 %H:%M:%S' AS DATETIME)";
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
		std::string result( CStrUtils::ToStr( text ) );

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

		return CStrUtils::ToWStr( result );
	}

	String CDatabaseConnectionPostgreSql::WriteBinary( const ByteArray & text ) const
	{
		String result;
		size_t length = 0;
		uint8_t * escaped = PQescapeByteaConn( _connection, text.data(), text.size(), &length );

		if ( escaped )
		{
			result = reinterpret_cast< char * >( escaped );
			PQfreemem( escaped );
		}
		else
		{
			PostgreSQLCheck( NULL, INFO_ESCAPING_TEXT, EDatabaseExceptionCodes_ConnectionError, _connection );
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

	std::string CDatabaseConnectionPostgreSql::WriteDateS( const CDate & date ) const
	{
		std::string strReturn;

		if ( date.IsValid() )
		{
			strReturn = date.Format( POSTGRESQL_FORMAT_SDATE );
		}
		else
		{
			strReturn += POSTGRESQL_SQL_SNULL;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionPostgreSql::WriteTimeS( const CTime & time ) const
	{
		std::string strReturn;

		if ( time.IsValid() )
		{
			strReturn = time.Format( POSTGRESQL_FORMAT_STIME );
		}
		else
		{
			strReturn += POSTGRESQL_SQL_SNULL;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionPostgreSql::WriteDateTimeS( const CDateTime & dateTime ) const
	{
		std::string strReturn;

		if ( dateTime.GetYear() <= 0 )
		{
			strReturn += POSTGRESQL_SQL_SNULL;
		}
		else
		{
			strReturn = dateTime.Format( POSTGRESQL_FORMAT_SDATETIME );
		}

		return strReturn;
	}

	std::string CDatabaseConnectionPostgreSql::WriteDateTimeS( const CDate & date ) const
	{
		std::string strReturn;

		if ( !date.IsValid() || date.GetYear() <= 0 )
		{
			strReturn += POSTGRESQL_SQL_SNULL;
		}
		else
		{
			strReturn = date.Format( POSTGRESQL_FORMAT_SDATETIME_DATE );
		}

		return strReturn;
	}

	std::string CDatabaseConnectionPostgreSql::WriteDateTimeS( const CTime & time ) const
	{
		std::string strReturn;

		if ( !time.IsValid() )
		{
			strReturn += POSTGRESQL_SQL_SNULL;
		}
		else
		{
			strReturn = time.Format( POSTGRESQL_FORMAT_SDATETIME_TIME );
		}

		return strReturn;
	}

	std::string CDatabaseConnectionPostgreSql::WriteStmtDateS( const CDate & date ) const
	{
		std::string strReturn;

		if ( date.IsValid() )
		{
			strReturn = date.Format( POSTGRESQL_FORMAT_STMT_SDATE );
		}
		else
		{
			strReturn += POSTGRESQL_SQL_SNULL;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionPostgreSql::WriteStmtTimeS( const CTime & time ) const
	{
		std::string strReturn;

		if ( time.IsValid() )
		{
			strReturn = time.Format( POSTGRESQL_FORMAT_STMT_STIME );
		}
		else
		{
			strReturn += POSTGRESQL_SQL_SNULL;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionPostgreSql::WriteStmtDateTimeS( const CDateTime & dateTime ) const
	{
		std::string strReturn;

		if ( dateTime.GetYear() > 0 )
		{
			strReturn = dateTime.Format( POSTGRESQL_FORMAT_STMT_SDATETIME );
		}
		else
		{
			strReturn += POSTGRESQL_SQL_SNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionPostgreSql::WriteDateW( const CDate & date ) const
	{
		std::wstring strReturn;

		if ( date.IsValid() )
		{
			strReturn = date.Format( POSTGRESQL_FORMAT_WDATE );
		}
		else
		{
			strReturn += POSTGRESQL_SQL_WNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionPostgreSql::WriteTimeW( const CTime & time ) const
	{
		std::wstring strReturn;

		if ( time.IsValid() )
		{
			strReturn = time.Format( POSTGRESQL_FORMAT_WTIME );
		}
		else
		{
			strReturn += POSTGRESQL_SQL_WNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionPostgreSql::WriteDateTimeW( const CDateTime & dateTime ) const
	{
		std::wstring strReturn;

		if ( dateTime.GetYear() <= 0 )
		{
			strReturn += POSTGRESQL_SQL_WNULL;
		}
		else
		{
			strReturn = dateTime.Format( POSTGRESQL_FORMAT_WDATETIME );
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionPostgreSql::WriteDateTimeW( const CDate & date ) const
	{
		std::wstring strReturn;

		if ( !date.IsValid() || date.GetYear() <= 0 )
		{
			strReturn += POSTGRESQL_SQL_WNULL;
		}
		else
		{
			strReturn = date.Format( POSTGRESQL_FORMAT_WDATETIME_DATE );
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionPostgreSql::WriteDateTimeW( const CTime & time ) const
	{
		std::wstring strReturn;

		if ( !time.IsValid() )
		{
			strReturn += POSTGRESQL_SQL_WNULL;
		}
		else
		{
			strReturn = time.Format( POSTGRESQL_FORMAT_WDATETIME_TIME );
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionPostgreSql::WriteStmtDateW( const CDate & date ) const
	{
		std::wstring strReturn;

		if ( date.IsValid() )
		{
			strReturn = date.Format( POSTGRESQL_FORMAT_STMT_WDATE );
		}
		else
		{
			strReturn += POSTGRESQL_SQL_WNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionPostgreSql::WriteStmtTimeW( const CTime & time ) const
	{
		std::wstring strReturn;

		if ( time.IsValid() )
		{
			strReturn = time.Format( POSTGRESQL_FORMAT_STMT_WTIME );
		}
		else
		{
			strReturn += POSTGRESQL_SQL_WNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionPostgreSql::WriteStmtDateTimeW( const CDateTime & dateTime ) const
	{
		std::wstring strReturn;

		if ( dateTime.GetYear() > 0 )
		{
			strReturn = dateTime.Format( POSTGRESQL_FORMAT_STMT_WDATETIME );
		}
		else
		{
			strReturn += POSTGRESQL_SQL_WNULL;
		}

		return strReturn;
	}

	String CDatabaseConnectionPostgreSql::WriteBool( bool value ) const
	{
		return ( value ? STR( "1" ) : STR( "0" ) );
	}

	String CDatabaseConnectionPostgreSql::WriteBool( const String & value ) const
	{
		const String lowerCaseValue = CStrUtils::LowerCase( value );
		return ( lowerCaseValue == STR( "x" ) || lowerCaseValue == STR( "oui" ) || lowerCaseValue == STR( "yes" ) || lowerCaseValue == STR( "y" ) || value == STR( "1" ) ? STR( "1" ) : STR( "0" ) );
	}

	CDate CDatabaseConnectionPostgreSql::ParseDate( const std::string & date ) const
	{
		CDate dateObj;

		if ( !CDate::IsDate( date, POSTGRESQL_FORMAT_SDATE, dateObj )
		&& !CDate::IsDate( date, POSTGRESQL_FORMAT_STMT_SDATE, dateObj ) )
		{
			dateObj = CDate( 0, EDateMonth_UNDEF, 0 );
		}

		return dateObj;
	}

	CTime CDatabaseConnectionPostgreSql::ParseTime( const std::string & time ) const
	{
		CTime timeObj;

		if ( !CTime::IsTime( time, POSTGRESQL_FORMAT_STIME, timeObj )
		&& !CTime::IsTime( time, POSTGRESQL_FORMAT_STMT_STIME, timeObj ) )
		{
			timeObj = CTime();
		}

		return timeObj;
	}

	CDateTime CDatabaseConnectionPostgreSql::ParseDateTime( const std::string & dateTime ) const
	{
		CDateTime dateTimeObj;

		if ( !CDateTime::IsDateTime( dateTime, POSTGRESQL_FORMAT_SDATETIME, dateTimeObj )
		&& !CDateTime::IsDateTime( dateTime, POSTGRESQL_FORMAT_STMT_SDATETIME, dateTimeObj )
		&& !CDateTime::IsDateTime( dateTime, POSTGRESQL_FORMAT_SDATE, dateTimeObj )
		&& !CDateTime::IsDateTime( dateTime, POSTGRESQL_FORMAT_STMT_SDATE, dateTimeObj ) )
		{
			dateTimeObj = CDateTime();
		}

		return dateTimeObj;
	}

	CDate CDatabaseConnectionPostgreSql::ParseDate( const std::wstring & date ) const
	{
		CDate dateObj;

		if ( !CDate::IsDate( date, POSTGRESQL_FORMAT_WDATE, dateObj )
		&& !CDate::IsDate( date, POSTGRESQL_FORMAT_STMT_WDATE, dateObj ) )
		{
			dateObj = CDate( 0, EDateMonth_UNDEF, 0 );
		}

		return dateObj;
	}

	CTime CDatabaseConnectionPostgreSql::ParseTime( const std::wstring & time ) const
	{
		CTime timeObj;

		if ( !CTime::IsTime( time, POSTGRESQL_FORMAT_WTIME, timeObj )
		&& !CTime::IsTime( time, POSTGRESQL_FORMAT_STMT_WTIME, timeObj ) )
		{
			timeObj = CTime();
		}

		return timeObj;
	}

	CDateTime CDatabaseConnectionPostgreSql::ParseDateTime( const std::wstring & dateTime ) const
	{
		CDateTime dateTimeObj;

		if ( !CDateTime::IsDateTime( dateTime, POSTGRESQL_FORMAT_WDATETIME, dateTimeObj )
		&& !CDateTime::IsDateTime( dateTime, POSTGRESQL_FORMAT_STMT_WDATETIME, dateTimeObj )
		&& !CDateTime::IsDateTime( dateTime, POSTGRESQL_FORMAT_WDATE, dateTimeObj )
		&& !CDateTime::IsDateTime( dateTime, POSTGRESQL_FORMAT_STMT_WDATE, dateTimeObj ) )
		{
			dateTimeObj = CDateTime();
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

		DoExecuteUpdate( POSTGRESQL_SQL_CREATE_DATABASE + database + POSTGRESQL_SQL_CHARSET );
	}

	void CDatabaseConnectionPostgreSql::SelectDatabase( const String & database )
	{
		if ( !IsConnected() )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_POSTGRESQL_NOT_CONNECTED );
		}

		if ( _connection )
		{
			if ( ExecuteUpdate( POSTGRESQL_SQL_USE + database ) )
			{
				_database = database;
			}
		}
	}

	void CDatabaseConnectionPostgreSql::DestroyDatabase( const String & database )
	{
		if ( !IsConnected() )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_POSTGRESQL_NOT_CONNECTED );
		}

		DoExecuteUpdate( POSTGRESQL_SQL_DROP_DATABASE + database );
	}

	EErrorType CDatabaseConnectionPostgreSql::DoConnect( String & connectionString )
	{
		EErrorType ret = EErrorType_NONE;

		try
		{
			std::stringstream stream;
			stream << "host=" << _server;
			stream << " hostaddr=" << _server;

			if ( !_userName.empty() )
			{
				stream << " user=" << _userName;
			}

			if ( !_password.empty() )
			{
				stream << " password=" << _password;
			}

			_connection = PQconnectdb( stream.str().c_str() );
			connectionString = CStrUtils::ToStr( stream.str() );

			if ( !_connection )
			{
				DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_POSTGRESQL_CONNECTION );
			}

			DoSetConnected( true );
		}
		catch ( CExceptionDatabase & exc )
		{
			CLogger::LogError( StringStream() << ERROR_POSTGRESQL_CONNECTION << STR( " - " ) << exc.GetFullDescription() );
			ret = EErrorType_ERROR;
		}
		catch ( std::exception & exc )
		{
			CLogger::LogError( StringStream() << ERROR_POSTGRESQL_CONNECTION << STR( " - " ) << exc.what() );
			ret = EErrorType_ERROR;
		}
		catch ( ... )
		{
			CLogger::LogError( ERROR_POSTGRESQL_CONNECTION );
			ret = EErrorType_ERROR;
		}

		return ret;
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
		std::string strQuery = CStrUtils::ToStr( query );
		PGresult * result = PQexec( _connection, strQuery.c_str() );
		PostgreSQLCheck( result, INFO_POSTGRESQL_STATEMENT_PREPARATION, EDatabaseExceptionCodes_StatementError, _connection );
		PQclear( result );
		return true;
	}

	DatabaseResultPtr CDatabaseConnectionPostgreSql::DoExecuteSelect( const String & query )
	{
		DatabaseResultPtr ret;
		std::string strQuery = CStrUtils::ToStr( query );
		PGresult * result = PQexec( _connection, strQuery.c_str() );
		PostgreSQLCheck( result, INFO_POSTGRESQL_STATEMENT_PREPARATION, EDatabaseExceptionCodes_StatementError, _connection );
		return DoRetrieveResults( result );
	}

	DatabaseStatementPtr CDatabaseConnectionPostgreSql::DoCreateStatement( const String & request )
	{
		return std::make_shared< CDatabaseStatementPostgreSql >( std::static_pointer_cast< CDatabaseConnectionPostgreSql >( shared_from_this() ), request );
	}

	DatabaseResultPtr CDatabaseConnectionPostgreSql::DoRetrieveResults( PGresult * result )
	{
		DatabaseResultPtr pResult;

		try
		{
			DatabaseConnectionPostgreSqlPtr connection = std::static_pointer_cast< CDatabaseConnectionPostgreSql >( shared_from_this() );
			std::vector< PGbind > binds;
			std::vector< std::unique_ptr< SInPostgreSqlBindBase > > inbinds;
			DatabaseFieldInfosPtrArray columns = PostgreSqlGetColumns( result, connection, inbinds, binds );
			pResult = PostgreSqlFetchResult( result, columns, connection, inbinds, binds );
		}
		catch ( CExceptionDatabase & exc )
		{
			StringStream stream;
			stream << ERROR_POSTGRESQL_EXECUTION << exc.GetFullDescription();
			CLogger::LogError( stream.str() );
		}
		catch ( std::exception & exc )
		{
			StringStream stream;
			stream << ERROR_POSTGRESQL_EXECUTION << exc.what();
			CLogger::LogError( stream.str() );
		}

		PQclear( result );
		return pResult;
	}
}
END_NAMESPACE_DATABASE_POSTGRESQL
