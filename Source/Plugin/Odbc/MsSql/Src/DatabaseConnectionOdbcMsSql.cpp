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
	static const String ODBC_SQL_NULL = STR( "NULL" );
	static const std::string ODBC_SQL_SNULL = "NULL" ;
	static const std::wstring ODBC_SQL_WNULL = L"NULL";

	static const String ODBC_DSN_DRIVER = STR( "DRIVER={SQL Server};SERVER=" );
	static const String ODBC_DSN_DSN = STR( ";DSN=" );
	static const String ODBC_DSN_DATABASE = STR( ";DATABASE=" );
	static const String ODBC_DSN_UID = STR( ";UID=" );
	static const String ODBC_DSN_PWD = STR( ";PWD=" );
	static const String ODBC_DSN_INTEGRATED = STR( ";INTEGRATED SECURITY=true;Trusted_Connection=yes" );

	static const String INFO_ODBC_AllocHandle = STR( "SQLAllocHandle" );
	static const String INFO_ODBC_DriverConnect = STR( "SQLDriverConnect" );

	static const String ODBC_FORMAT_DATE = STR( "CAST('%Y-%m-%d' AS DATE)" );
	static const String ODBC_FORMAT_TIME = STR( "CAST('%H:%M:%S' AS TIME)" );
	static const String ODBC_FORMAT_DATETIME = STR( "CAST('%Y-%m-%d %H:%M:%S' AS TIMESTAMP)" );
	static const String ODBC_FORMAT_DATETIME_DATE = STR( "CAST('%Y-%m-%d 00:00:00' AS TIMESTAMP)" );
	static const String ODBC_FORMAT_DATETIME_TIME = STR( "CAST('0000-00-00 %H:%M:%S' AS TIMESTAMP)" );
	static const String ODBC_FORMAT_STMT_DATE = STR( "{-d %Y-%m-%d}" );
	static const String ODBC_FORMAT_STMT_TIME = STR( "{-t %H:%M:%S}" );
	static const String ODBC_FORMAT_STMT_DATETIME = STR( "{-ts %Y-%m-%d %H:%M:%S}" );


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

	void CDatabaseConnectionOdbcMsSql::DoCreateDatabase( const String & database )
	{
		DoExecuteUpdate( ODBC_SQL_CREATE_DATABASE + database + ODBC_SQL_COLLATE );
	}

	void CDatabaseConnectionOdbcMsSql::DoSelectDatabase( const String & database )
	{
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

	void CDatabaseConnectionOdbcMsSql::DoDestroyDatabase( const String & database )
	{
		DoExecuteUpdate( ODBC_SQL_DROP_DATABASE + database );
	}

	std::string CDatabaseConnectionOdbcMsSql::DoWriteText( const std::string & text ) const
	{
		std::string strReturn( text );

		if ( strReturn != ODBC_SQL_SNULL )
		{
			StringUtils::Replace( strReturn, "'", "''" );
			StringUtils::Replace( strReturn, "\\", "\\\\" );
			strReturn = "'" + strReturn + "'";
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionOdbcMsSql::DoWriteNText( const std::wstring & text ) const
	{
		std::wstring strReturn( text );

		if ( strReturn != ODBC_SQL_WNULL )
		{
			StringUtils::Replace( strReturn, L"'", L"''" );
			StringUtils::Replace( strReturn, L"\\", L"\\\\" );
			strReturn = L"N'" + strReturn + L"'";
		}

		return strReturn;
	}

	String CDatabaseConnectionOdbcMsSql::DoWriteName( const String & text ) const
	{
		return STR( "[" ) + text + STR( "]" );
	}

	String CDatabaseConnectionOdbcMsSql::DoWriteBinary( const ByteArray & array ) const
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

	String CDatabaseConnectionOdbcMsSql::DoWriteDate( const DateType & date ) const
	{
		String strReturn;

		if ( Date::IsValid( date ) )
		{
			strReturn = Date::Format( date, ODBC_FORMAT_DATE );
		}
		else
		{
			strReturn += ODBC_SQL_NULL;
		}

		return strReturn;
	}

	String CDatabaseConnectionOdbcMsSql::DoWriteTime( const TimeType & time ) const
	{
		String strReturn;

		if ( Time::IsValid( time ) )
		{
			strReturn = Time::Format( time, ODBC_FORMAT_TIME );
		}
		else
		{
			strReturn += ODBC_SQL_NULL;
		}

		return strReturn;
	}

	String CDatabaseConnectionOdbcMsSql::DoWriteDateTime( const DateTimeType & dateTime ) const
	{
		String strReturn;

		if ( DateTime::IsValid( dateTime ) )
		{
			strReturn = DateTime::Format( dateTime, ODBC_FORMAT_DATETIME );
		}
		else
		{
			strReturn += ODBC_SQL_NULL;
		}

		return strReturn;
	}

	String CDatabaseConnectionOdbcMsSql::DoWriteDateTime( const DateType & date ) const
	{
		String strReturn;

		if ( Date::IsValid( date ) )
		{
			strReturn = Date::Format( date, ODBC_FORMAT_DATETIME_DATE );
		}
		else
		{
			strReturn += ODBC_SQL_NULL;
		}

		return strReturn;
	}

	String CDatabaseConnectionOdbcMsSql::DoWriteDateTime( const TimeType & time ) const
	{
		String strReturn;

		if ( Time::IsValid( time ) )
		{
			strReturn = Time::Format( time, ODBC_FORMAT_DATETIME_TIME );
		}
		else
		{
			strReturn += ODBC_SQL_NULL;
		}

		return strReturn;
	}

	String CDatabaseConnectionOdbcMsSql::DoWriteStmtDate( const DateType & date ) const
	{
		String strReturn;

		if ( Date::IsValid( date ) )
		{
			strReturn = Date::Format( date, ODBC_FORMAT_STMT_DATE );
		}
		else
		{
			strReturn += ODBC_SQL_NULL;
		}

		return strReturn;
	}

	String CDatabaseConnectionOdbcMsSql::DoWriteStmtTime( const TimeType & time ) const
	{
		String strReturn;

		if ( Time::IsValid( time ) )
		{
			strReturn = Time::Format( time, ODBC_FORMAT_STMT_TIME );
		}
		else
		{
			strReturn += ODBC_SQL_NULL;
		}

		return strReturn;
	}

	String CDatabaseConnectionOdbcMsSql::DoWriteStmtDateTime( const DateTimeType & dateTime ) const
	{
		String strReturn;

		if ( DateTime::IsValid( dateTime ) )
		{
			strReturn = DateTime::Format( dateTime, ODBC_FORMAT_STMT_DATETIME );
		}
		else
		{
			strReturn += ODBC_SQL_NULL;
		}

		return strReturn;
	}

	String CDatabaseConnectionOdbcMsSql::DoWriteBool( bool value ) const
	{
		return ( value ? STR( "1" ) : STR( "0" ) );
	}

	DateType CDatabaseConnectionOdbcMsSql::DoParseDate( const String & date ) const
	{
		DateType dateObj;
		Date::IsDate( date, ODBC_FORMAT_STMT_DATE, dateObj );
		return dateObj;
	}

	TimeType CDatabaseConnectionOdbcMsSql::DoParseTime( const String & time ) const
	{
		TimeType timeObj;
		Time::IsTime( time, ODBC_FORMAT_STMT_TIME, timeObj );
		return timeObj;
	}

	DateTimeType CDatabaseConnectionOdbcMsSql::DoParseDateTime( const String & dateTime ) const
	{
		DateTimeType dateTimeObj;
		DateTime::IsDateTime( dateTime, ODBC_FORMAT_STMT_DATETIME, dateTimeObj );
		return dateTimeObj;
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
