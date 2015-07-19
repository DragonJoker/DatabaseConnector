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

	static const std::string ODBC_FORMAT_SDATE = "CAST( '%04i-%02i-%02i' AS DATE)";
	static const std::string ODBC_FORMAT_STIME = "CAST( '%02i:%02i%:02i' AS TIME)";
	static const std::string ODBC_FORMAT_SDATETIME = "CAST('%04i-%02i-%02i %02i:%02i:%02i' as DATETIME)";
	static const std::string ODBC_FORMAT_SDATETIME_DATE = "CAST('%04i-%02i-%02i 00:00:00' as DATETIME)";
	static const std::string ODBC_FORMAT_SDATETIME_TIME = "CAST('0000-00-00 %02i:%02i:%02i' as DATETIME)";
	static const std::string ODBC_FORMAT_STMT_SDATE = "{-d %04i-%02i-%02i}";
	static const std::string ODBC_FORMAT_STMT_STIME = "{-t %02i:%02i:%02i}";
	static const std::string ODBC_FORMAT_STMT_SDATETIME = "{-ts %04i-%02i-%02i %02i:%02i:%02i}";

	static const std::wstring ODBC_FORMAT_WDATE = L"CAST( '%04i-%02i-%02i' AS DATE)";
	static const std::wstring ODBC_FORMAT_WTIME = L"CAST( '%02i:%02i%:02i' AS TIME)";
	static const std::wstring ODBC_FORMAT_WDATETIME = L"CAST('%04i-%02i-%02i %02i:%02i:%02i' as DATETIME)";
	static const std::wstring ODBC_FORMAT_WDATETIME_DATE = L"CAST('%04i-%02i-%02i 00:00:00' as DATETIME)";
	static const std::wstring ODBC_FORMAT_WDATETIME_TIME = L"CAST('0000-00-00 %02i:%02i:%02i' as DATETIME)";
	static const std::wstring ODBC_FORMAT_STMT_WDATE = L"{-d %04i-%02i-%02i}";
	static const std::wstring ODBC_FORMAT_STMT_WTIME = L"{-t %02i:%02i:%02i}";
	static const std::wstring ODBC_FORMAT_STMT_WDATETIME = L"{-ts %04i-%02i-%02i %02i:%02i:%02i}";

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

	std::string CDatabaseConnectionOdbcMsSql::WriteDateS( const CDate & date ) const
	{
		std::string strReturn;

		if ( date.IsValid() )
		{
			Formalize( strReturn, 1024, ODBC_FORMAT_SDATE.c_str(), date.GetYear(), date.GetMonth() + 1, date.GetMonthDay() );
		}
		else
		{
			strReturn += ODBC_SQL_SNULL;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionOdbcMsSql::WriteTimeS( const CTime & time ) const
	{
		std::string strReturn;

		if ( time.IsValid() )
		{
			Formalize( strReturn, 1024, ODBC_FORMAT_STIME.c_str(), time.GetHour(), time.GetMinute(), time.GetSecond() );
		}
		else
		{
			strReturn += ODBC_SQL_SNULL;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionOdbcMsSql::WriteDateTimeS( const CDateTime & dateTime ) const
	{
		std::string strReturn;

		if ( dateTime.GetYear() <= 0 )
		{
			strReturn += ODBC_SQL_SNULL;
		}
		else
		{
			Formalize( strReturn, 1024, ODBC_FORMAT_SDATETIME.c_str(), dateTime.GetYear(), dateTime.GetMonth() + 1, dateTime.GetMonthDay(), dateTime.GetHour(), dateTime.GetMinute(), dateTime.GetSecond() );
		}

		return strReturn;
	}

	std::string CDatabaseConnectionOdbcMsSql::WriteDateTimeS( const CDate & date ) const
	{
		std::string strReturn;

		if ( date.IsValid() )
		{
			if ( date.GetYear() <= 0 )
			{
				strReturn += ODBC_SQL_SNULL;
			}
			else
			{
				Formalize( strReturn, 1024, ODBC_FORMAT_SDATETIME_DATE.c_str(), date.GetYear(), date.GetMonth(), date.GetMonthDay() );
			}
		}
		else
		{
			strReturn += ODBC_SQL_SNULL;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionOdbcMsSql::WriteDateTimeS( const CTime & time ) const
	{
		std::string strReturn;

		if ( time.IsValid() )
		{
			Formalize( strReturn, 1024, ODBC_FORMAT_SDATETIME_TIME.c_str(), time.GetHour(), time.GetMinute(), time.GetSecond() );
		}
		else
		{
			strReturn += ODBC_SQL_SNULL;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionOdbcMsSql::WriteStmtDateS( const CDate & date ) const
	{
		std::string strReturn;

		if ( date.IsValid() )
		{
			Formalize( strReturn, 1024, ODBC_FORMAT_STMT_SDATE.c_str(), date.GetYear(), date.GetMonth(), date.GetMonthDay() );
		}
		else
		{
			strReturn += ODBC_SQL_SNULL;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionOdbcMsSql::WriteStmtTimeS( const CTime & time ) const
	{
		std::string strReturn;

		if ( time.IsValid() )
		{
			Formalize( strReturn, 1024, ODBC_FORMAT_STMT_STIME.c_str(), time.GetHour(), time.GetMinute(), time.GetSecond() );
		}
		else
		{
			strReturn += ODBC_SQL_SNULL;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionOdbcMsSql::WriteStmtDateTimeS( const CDateTime & dateTime ) const
	{
		std::string strReturn;

		if ( dateTime.GetYear() > 0 )
		{
			Formalize( strReturn, 1024, ODBC_FORMAT_STMT_SDATETIME.c_str(), dateTime.GetYear(), dateTime.GetMonth(), dateTime.GetMonthDay(), dateTime.GetHour(), dateTime.GetMinute(), dateTime.GetSecond() );
		}
		else
		{
			strReturn += ODBC_SQL_SNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionOdbcMsSql::WriteDateW( const CDate & date ) const
	{
		std::wstring strReturn;

		if ( date.IsValid() )
		{
			Formalize( strReturn, 1024, ODBC_FORMAT_WDATE.c_str(), date.GetYear(), date.GetMonth(), date.GetMonthDay() );
		}
		else
		{
			strReturn += ODBC_SQL_WNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionOdbcMsSql::WriteTimeW( const CTime & time ) const
	{
		std::wstring strReturn;

		if ( time.IsValid() )
		{
			Formalize( strReturn, 1024, ODBC_FORMAT_WTIME.c_str(), time.GetHour(), time.GetMinute(), time.GetSecond() );
		}
		else
		{
			strReturn += ODBC_SQL_WNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionOdbcMsSql::WriteDateTimeW( const CDateTime & dateTime ) const
	{
		std::wstring strReturn;

		if ( dateTime.GetYear() <= 0 )
		{
			strReturn += ODBC_SQL_WNULL;
		}
		else
		{
			Formalize( strReturn, 1024, ODBC_FORMAT_WDATETIME.c_str(), dateTime.GetYear(), dateTime.GetMonth(), dateTime.GetMonthDay(), dateTime.GetHour(), dateTime.GetMinute(), dateTime.GetSecond() );
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionOdbcMsSql::WriteDateTimeW( const CDate & date ) const
	{
		std::wstring strReturn;

		if ( date.IsValid() )
		{
			if ( date.GetYear() <= 0 )
			{
				strReturn += ODBC_SQL_WNULL;
			}
			else
			{
				Formalize( strReturn, 1024, ODBC_FORMAT_WDATETIME_DATE.c_str(), date.GetYear(), date.GetMonth(), date.GetMonthDay() );
			}
		}
		else
		{
			strReturn += ODBC_SQL_WNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionOdbcMsSql::WriteDateTimeW( const CTime & time ) const
	{
		std::wstring strReturn;

		if ( time.IsValid() )
		{
			Formalize( strReturn, 1024, ODBC_FORMAT_WDATETIME_TIME.c_str(), time.GetHour(), time.GetMinute(), time.GetSecond() );
		}
		else
		{
			strReturn += ODBC_SQL_WNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionOdbcMsSql::WriteStmtDateW( const CDate & date ) const
	{
		std::wstring strReturn;

		if ( date.IsValid() )
		{
			Formalize( strReturn, 1024, ODBC_FORMAT_STMT_WDATE.c_str(), date.GetYear(), date.GetMonth(), date.GetMonthDay() );
		}
		else
		{
			strReturn += ODBC_SQL_WNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionOdbcMsSql::WriteStmtTimeW( const CTime & time ) const
	{
		std::wstring strReturn;

		if ( time.IsValid() )
		{
			Formalize( strReturn, 1024, ODBC_FORMAT_STMT_WTIME.c_str(), time.GetHour(), time.GetMinute(), time.GetSecond() );
		}
		else
		{
			strReturn += ODBC_SQL_WNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionOdbcMsSql::WriteStmtDateTimeW( const CDateTime & dateTime ) const
	{
		std::wstring strReturn;

		if ( dateTime.GetYear() > 0 )
		{
			Formalize( strReturn, 1024, ODBC_FORMAT_STMT_WDATETIME.c_str(), dateTime.GetYear(), dateTime.GetMonth(), dateTime.GetMonthDay(), dateTime.GetHour(), dateTime.GetMinute(), dateTime.GetSecond() );
		}
		else
		{
			strReturn += ODBC_SQL_WNULL;
		}

		return strReturn;
	}

	CDate CDatabaseConnectionOdbcMsSql::ParseDate( const std::string & date ) const
	{
		CDate dateObj;
		CDate::IsDate( date, ODBC_FORMAT_STMT_SDATE, dateObj );
		return dateObj;
	}

	CTime CDatabaseConnectionOdbcMsSql::ParseTime( const std::string & time ) const
	{
		CTime timeObj;
		CTime::IsTime( time, ODBC_FORMAT_STMT_STIME, timeObj );
		return timeObj;
	}

	CDateTime CDatabaseConnectionOdbcMsSql::ParseDateTime( const std::string & dateTime ) const
	{
		CDateTime dateTimeObj;
		CDateTime::IsDateTime( dateTime, ODBC_FORMAT_STMT_SDATETIME, dateTimeObj );
		return dateTimeObj;
	}

	CDate CDatabaseConnectionOdbcMsSql::ParseDate( const std::wstring & date ) const
	{
		CDate dateObj;
		CDate::IsDate( date, ODBC_FORMAT_STMT_WDATE, dateObj );
		return dateObj;
	}

	CTime CDatabaseConnectionOdbcMsSql::ParseTime( const std::wstring & time ) const
	{
		CTime timeObj;
		CTime::IsTime( time, ODBC_FORMAT_STMT_WTIME, timeObj );
		return timeObj;
	}

	CDateTime CDatabaseConnectionOdbcMsSql::ParseDateTime( const std::wstring & dateTime ) const
	{
		CDateTime dateTimeObj;
		CDateTime::IsDateTime( dateTime, ODBC_FORMAT_STMT_WDATETIME, dateTimeObj );
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

	DatabaseStatementPtr CDatabaseConnectionOdbcMsSql::DoCreateStatement( const String & query )
	{
		return std::make_shared< CDatabaseStatementOdbc >( std::static_pointer_cast< CDatabaseConnectionOdbc >( shared_from_this() ), query );
	}
}
END_NAMESPACE_DATABASE_ODBC_MSSQL
