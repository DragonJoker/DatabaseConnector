/************************************************************************//**
* @file DatabaseConnectionOdbcMsSql.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 3/14/2014 5:03:15 PM
*
*
* @brief CConnectionOdbcMsSql class definition.
*
* @details Describes a connection to a database via an ODBC driver.
*
***************************************************************************/

#include "DatabaseOdbcMsSqlPch.h"

#include "DatabaseConnectionOdbcMsSql.h"

#include <DatabaseOdbcHelper.h>
#include <DatabaseStatementOdbc.h>
#include <DatabaseDateTimeHelper.h>

BEGIN_NAMESPACE_DATABASE_ODBC_MSSQL
{
	static const String ERROR_ODBC_NOT_CONNECTED = STR( "Not connected" );

	static const String ODBC_SQL_CREATE_DATABASE = STR( "CREATE DATABASE " );
	static const String ODBC_SQL_COLLATE = STR( " COLLATE utf8_BIN" );
	static const String ODBC_SQL_USE_DATABASE = STR( "USE " );
	static const String ODBC_SQL_DROP_DATABASE = STR( "DROP DATABASE " );

	static const String ODBC_DSN_DRIVER = STR( "DRIVER={SQL Server};SERVER=" );
	static const String ODBC_DSN_DSN = STR( ";DSN=" );
	static const String ODBC_DSN_DATABASE = STR( ";DATABASE=" );
	static const String ODBC_DSN_UID = STR( ";UID=" );
	static const String ODBC_DSN_PWD = STR( ";PWD=" );
	static const String ODBC_DSN_INTEGRATED = STR( ";INTEGRATED SECURITY=true;Trusted_Connection=yes" );

	static const String INFO_ODBC_AllocHandle = STR( "SQLAllocHandle" );
	static const String INFO_ODBC_DriverConnect = STR( "SQLDriverConnect" );

	static const std::string ODBC_FORMAT_SDATE = "CAST('%Y-%m-%d' AS DATE)";
	static const std::string ODBC_FORMAT_STIME = "CAST('%H:%M:%S' AS TIME)";
	static const std::string ODBC_FORMAT_SDATETIME = "CAST('%Y-%m-%d %H:%M:%S' AS TIMESTAMP)";
	static const std::string ODBC_FORMAT_SDATETIME_DATE = "CAST('%Y-%m-%d 00:00:00' AS TIMESTAMP)";
	static const std::string ODBC_FORMAT_SDATETIME_TIME = "CAST('0000-00-00 %H:%M:%S' AS TIMESTAMP)";
	static const std::string ODBC_FORMAT_STMT_SDATE = "{-d %Y-%m-%d}";
	static const std::string ODBC_FORMAT_STMT_STIME = "{-t %H:%M:%S}";
	static const std::string ODBC_FORMAT_STMT_SDATETIME = "{-ts %Y-%m-%d %H:%M:%S}";

	static const std::wstring ODBC_FORMAT_WDATE = L"CAST('%Y-%m-%d' AS DATE)";
	static const std::wstring ODBC_FORMAT_WTIME = L"CAST('%H:%M:%S' AS TIME)";
	static const std::wstring ODBC_FORMAT_WDATETIME = L"CAST('%Y-%m-%d %H:%M:%S' AS TIMESTAMP)";
	static const std::wstring ODBC_FORMAT_WDATETIME_DATE = L"CAST('%Y-%m-%d 00:00:00' AS TIMESTAMP)";
	static const std::wstring ODBC_FORMAT_WDATETIME_TIME = L"CAST('0000-00-00 %H:%M:%S' AS TIMESTAMP)";
	static const std::wstring ODBC_FORMAT_STMT_WDATE = L"{-d %Y-%m-%d}";
	static const std::wstring ODBC_FORMAT_STMT_WTIME = L"{-t %H:%M:%S}";
	static const std::wstring ODBC_FORMAT_STMT_WDATETIME = L"{-ts %Y-%m-%d %H:%M:%S}";

	static const std::string ODBC_SQL_SNULL = "NULL";
	static const std::wstring ODBC_SQL_WNULL = L"NULL";

	CDatabaseConnectionOdbcMsSql::CDatabaseConnectionOdbcMsSql( SQLHENV sqlEnvironmentHandle, const String & server, const String & userName, const String & password, String & connectionString )
		:   CDatabaseConnectionOdbc( sqlEnvironmentHandle, server, userName, password, connectionString )
	{
		DoConnect( connectionString );
	}

	CDatabaseConnectionOdbcMsSql::~CDatabaseConnectionOdbcMsSql()
	{
		DoDisconnect();
	}

	uint32_t CDatabaseConnectionOdbcMsSql::GetPrecision( EFieldType type ) const
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

	void CDatabaseConnectionOdbcMsSql::CreateDatabase( const String & database )
	{
		if ( !IsConnected() )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_ODBC_NOT_CONNECTED );
		}

		DoExecuteUpdate( ODBC_SQL_CREATE_DATABASE + database + ODBC_SQL_COLLATE );
	}

	void CDatabaseConnectionOdbcMsSql::SelectDatabase( const String & database )
	{
		if ( !IsConnected() )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_ODBC_NOT_CONNECTED );
		}

		if ( !_database.empty() )
		{
			SQLDisconnect( _connectionHandle );
		}
		
		StringStream connectionString;
		connectionString << ODBC_DSN_DRIVER + _server;
		connectionString << ODBC_DSN_DATABASE << database;

		if ( _userName.size() > 0 )
		{
			connectionString << ODBC_DSN_UID << _userName << ODBC_DSN_PWD << _password;
		}
		else
		{
			connectionString << ODBC_DSN_INTEGRATED;
		}

#if defined( _WIN32 )
		SQLHWND sqlHwnd = GetDesktopWindow();
#else
		SQLHWND sqlHwnd = NULL;
#endif
		SQLRETURN sqlReturn = SQLDriverConnectA( _connectionHandle, sqlHwnd, ( SqlChar * )connectionString.str().c_str(), SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE_REQUIRED );

		if ( SqlSuccess( sqlReturn, SQL_HANDLE_DBC, _connectionHandle, INFO_ODBC_DriverConnect ) == EErrorType_NONE )
		{
			_database = database;
		}
	}

	void CDatabaseConnectionOdbcMsSql::DestroyDatabase( const String & database )
	{
		if ( !IsConnected() )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_ODBC_NOT_CONNECTED );
		}

		DoExecuteUpdate( ODBC_SQL_DROP_DATABASE + database );
	}

	String CDatabaseConnectionOdbcMsSql::WriteBinary( const ByteArray & array ) const
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

	std::string CDatabaseConnectionOdbcMsSql::WriteDateS( const DateType & date ) const
	{
		std::string strReturn;

		if ( Date::IsValid( date ) )
		{
			strReturn = Date::Format( date, ODBC_FORMAT_SDATE );
		}
		else
		{
			strReturn += ODBC_SQL_SNULL;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionOdbcMsSql::WriteTimeS( const TimeType & time ) const
	{
		std::string strReturn;

		if ( Time::IsValid( time ) )
		{
			strReturn = Time::Format( time, ODBC_FORMAT_STIME );
		}
		else
		{
			strReturn += ODBC_SQL_SNULL;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionOdbcMsSql::WriteDateTimeS( const DateTimeType & dateTime ) const
	{
		std::string strReturn;

		if ( DateTime::IsValid( dateTime ) )
		{
			strReturn = DateTime::Format( dateTime, ODBC_FORMAT_SDATETIME );
		}
		else
		{
			strReturn += ODBC_SQL_SNULL;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionOdbcMsSql::WriteDateTimeS( const DateType & date ) const
	{
		std::string strReturn;

		if ( Date::IsValid( date ) )
		{
			strReturn = Date::Format( date, ODBC_FORMAT_SDATETIME_DATE );
		}
		else
		{
			strReturn += ODBC_SQL_SNULL;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionOdbcMsSql::WriteDateTimeS( const TimeType & time ) const
	{
		std::string strReturn;

		if ( Time::IsValid( time ) )
		{
			strReturn = Time::Format( time, ODBC_FORMAT_SDATETIME_TIME );
		}
		else
		{
			strReturn += ODBC_SQL_SNULL;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionOdbcMsSql::WriteStmtDateS( const DateType & date ) const
	{
		std::string strReturn;

		if ( Date::IsValid( date ) )
		{
			strReturn = Date::Format( date, ODBC_FORMAT_STMT_SDATE );
		}
		else
		{
			strReturn += ODBC_SQL_SNULL;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionOdbcMsSql::WriteStmtTimeS( const TimeType & time ) const
	{
		std::string strReturn;

		if ( Time::IsValid( time ) )
		{
			strReturn = Time::Format( time, ODBC_FORMAT_STMT_STIME );
		}
		else
		{
			strReturn += ODBC_SQL_SNULL;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionOdbcMsSql::WriteStmtDateTimeS( const DateTimeType & dateTime ) const
	{
		std::string strReturn;

		if ( DateTime::IsValid( dateTime ) )
		{
			strReturn = DateTime::Format( dateTime, ODBC_FORMAT_STMT_SDATETIME );
		}
		else
		{
			strReturn += ODBC_SQL_SNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionOdbcMsSql::WriteDateW( const DateType & date ) const
	{
		std::wstring strReturn;

		if ( Date::IsValid( date ) )
		{
			strReturn = Date::Format( date, ODBC_FORMAT_WDATE );
		}
		else
		{
			strReturn += ODBC_SQL_WNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionOdbcMsSql::WriteTimeW( const TimeType & time ) const
	{
		std::wstring strReturn;

		if ( Time::IsValid( time ) )
		{
			strReturn = Time::Format( time, ODBC_FORMAT_WTIME );
		}
		else
		{
			strReturn += ODBC_SQL_WNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionOdbcMsSql::WriteDateTimeW( const DateTimeType & dateTime ) const
	{
		std::wstring strReturn;

		if ( DateTime::IsValid( dateTime ) )
		{
			strReturn = DateTime::Format( dateTime, ODBC_FORMAT_WDATETIME );
		}
		else
		{
			strReturn += ODBC_SQL_WNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionOdbcMsSql::WriteDateTimeW( const DateType & date ) const
	{
		std::wstring strReturn;

		if ( Date::IsValid( date ) )
		{
			strReturn = Date::Format( date, ODBC_FORMAT_WDATETIME_DATE );
		}
		else
		{
			strReturn += ODBC_SQL_WNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionOdbcMsSql::WriteDateTimeW( const TimeType & time ) const
	{
		std::wstring strReturn;

		if ( Time::IsValid( time ) )
		{
			strReturn = Time::Format( time, ODBC_FORMAT_WDATETIME_TIME );
		}
		else
		{
			strReturn += ODBC_SQL_WNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionOdbcMsSql::WriteStmtDateW( const DateType & date ) const
	{
		std::wstring strReturn;

		if ( Date::IsValid( date ) )
		{
			strReturn = Date::Format( date, ODBC_FORMAT_STMT_WDATE );
		}
		else
		{
			strReturn += ODBC_SQL_WNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionOdbcMsSql::WriteStmtTimeW( const TimeType & time ) const
	{
		std::wstring strReturn;

		if ( Time::IsValid( time ) )
		{
			strReturn = Time::Format( time, ODBC_FORMAT_STMT_WTIME );
		}
		else
		{
			strReturn += ODBC_SQL_WNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionOdbcMsSql::WriteStmtDateTimeW( const DateTimeType & dateTime ) const
	{
		std::wstring strReturn;

		if ( DateTime::IsValid( dateTime ) )
		{
			strReturn = DateTime::Format( dateTime, ODBC_FORMAT_STMT_WDATETIME );
		}
		else
		{
			strReturn += ODBC_SQL_WNULL;
		}

		return strReturn;
	}

	DateType CDatabaseConnectionOdbcMsSql::ParseDate( const std::string & date ) const
	{
		DateType dateObj;
		Date::IsDate( date, ODBC_FORMAT_STMT_SDATE, dateObj );
		return dateObj;
	}

	TimeType CDatabaseConnectionOdbcMsSql::ParseTime( const std::string & time ) const
	{
		TimeType timeObj;
		Time::IsTime( time, ODBC_FORMAT_STMT_STIME, timeObj );
		return timeObj;
	}

	DateTimeType CDatabaseConnectionOdbcMsSql::ParseDateTime( const std::string & dateTime ) const
	{
		DateTimeType dateTimeObj;
		DateTime::IsDateTime( dateTime, ODBC_FORMAT_STMT_SDATETIME, dateTimeObj );
		return dateTimeObj;
	}

	DateType CDatabaseConnectionOdbcMsSql::ParseDate( const std::wstring & date ) const
	{
		DateType dateObj;
		Date::IsDate( date, ODBC_FORMAT_STMT_WDATE, dateObj );
		return dateObj;
	}

	TimeType CDatabaseConnectionOdbcMsSql::ParseTime( const std::wstring & time ) const
	{
		TimeType timeObj;
		Time::IsTime( time, ODBC_FORMAT_STMT_WTIME, timeObj );
		return timeObj;
	}

	DateTimeType CDatabaseConnectionOdbcMsSql::ParseDateTime( const std::wstring & dateTime ) const
	{
		DateTimeType dateTimeObj;
		DateTime::IsDateTime( dateTime, ODBC_FORMAT_STMT_WDATETIME, dateTimeObj );
		return dateTimeObj;
	}

	unsigned long CDatabaseConnectionOdbcMsSql::GetStmtDateSize()const
	{
		//"{-d YYYY-MM-DD}"
		return ( unsigned long )15;
	}

	unsigned long CDatabaseConnectionOdbcMsSql::GetStmtDateTimeSize()const
	{
		//"{-ts YYYY-MM-DD HH:MM:SS}"
		return ( unsigned long )25;
	}

	unsigned long CDatabaseConnectionOdbcMsSql::GetStmtTimeSize()const
	{
		//"{-t HH:MM:SS}"
		return ( unsigned long )13;
	}

	EErrorType CDatabaseConnectionOdbcMsSql::DoConnect( String & connectionString )
	{
		EErrorType eReturn = EErrorType_ERROR;

		if ( SqlSuccess( SQLAllocHandle( SQL_HANDLE_DBC, _environmentHandle, &_connectionHandle ), SQL_HANDLE_ENV, _environmentHandle, INFO_ODBC_AllocHandle ) == EErrorType_NONE )
		{
			DoSetConnected( true );
			eReturn = EErrorType_NONE;
		}

		return eReturn;
	}

	DatabaseStatementSPtr CDatabaseConnectionOdbcMsSql::DoCreateStatement( const String & query )
	{
		return std::make_shared< CDatabaseStatementOdbc >( std::static_pointer_cast< CDatabaseConnectionOdbc >( shared_from_this() ), query );
	}
}
END_NAMESPACE_DATABASE_ODBC_MSSQL
