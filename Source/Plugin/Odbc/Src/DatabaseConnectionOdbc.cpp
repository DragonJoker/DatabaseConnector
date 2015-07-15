/************************************************************************//**
* @file DatabaseConnectionOdbc.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 3/14/2014 5:03:15 PM
*
*
* @brief CConnectionOdbc class definition.
*
* @details Describes a connection to a database via an ODBC driver.
*
***************************************************************************/

#include "DatabaseOdbcPch.h"

#include "DatabaseConnectionOdbc.h"

#include "DatabaseOdbcHelper.h"
#include "DatabaseStatementOdbc.h"
#include "DatabaseQueryOdbc.h"
#include "ExceptionDatabaseOdbc.h"

#include <DatabaseQuery.h>
#include <DatabaseDate.h>
#include <DatabaseDateTime.h>
#include <DatabaseDateTimeHelper.h>
#include <DatabaseTime.h>

#include <DatabaseStringUtils.h>

BEGIN_NAMESPACE_DATABASE_ODBC
{
	static const String ODBC_SQL_TRANSACTION_BEGIN = STR( "BEGIN TRANSACTION " );
	static const String ODBC_SQL_TRANSACTION_COMMIT = STR( "COMMIT TRANSACTION " );
	static const String ODBC_SQL_TRANSACTION_ROLLBACK = STR( "ROLLBACK TRANSACTION " );
	static const String ODBC_SQL_NULL = STR( "NULL" );

	static const std::string ODBC_SQL_SNULL = "NULL";
	static const std::wstring ODBC_SQL_WNULL = L"NULL";

	static const std::string ODBC_FORMAT_SDATE = "CAST( '%04i%02i%02i' AS DATE)";
	static const std::string ODBC_FORMAT_STIME = "CONVERT(TIME, '%02i:%02i%:02i', 108)";
	static const std::string ODBC_FORMAT_SDATETIME = "CAST('%04i-%02i-%02i %02i:%02i:%02i' as DATETIME)";
	static const std::string ODBC_FORMAT_SDATETIME_DATE = "CAST('%04i-%02i-%02i 00:00:00' as DATETIME)";
	static const std::string ODBC_FORMAT_SDATETIME_TIME = "CAST('0000-00-00 %02i:%02i:%02i' as DATETIME)";
	static const std::string ODBC_FORMAT_STMT_SDATE = "{-d %04i-%02i-%02i}";
	static const std::string ODBC_FORMAT_STMT_STIME = "{-t %02i:%02i:%02i}";
	static const std::string ODBC_FORMAT_STMT_SDATETIME = "{-ts %04i-%02i-%02i %02i:%02i:%02i}";

	static const std::wstring ODBC_FORMAT_WDATE = L"CAST( '%04i%02i%02i' AS DATE)";
	static const std::wstring ODBC_FORMAT_WTIME = L"CONVERT(TIME, '%02i:%02i%:02i', 108)";
	static const std::wstring ODBC_FORMAT_WDATETIME = L"CAST('%04i-%02i-%02i %02i:%02i:%02i' as DATETIME)";
	static const std::wstring ODBC_FORMAT_WDATETIME_DATE = L"CAST('%04i-%02i-%02i 00:00:00' as DATETIME)";
	static const std::wstring ODBC_FORMAT_WDATETIME_TIME = L"CAST('0000-00-00 %02i:%02i:%02i' as DATETIME)";
	static const std::wstring ODBC_FORMAT_STMT_WDATE = L"{-d %04i-%02i-%02i}";
	static const std::wstring ODBC_FORMAT_STMT_WTIME = L"{-t %02i:%02i:%02i}";
	static const std::wstring ODBC_FORMAT_STMT_WDATETIME = L"{-ts %04i-%02i-%02i %02i:%02i:%02i}";

	CDatabaseConnectionOdbc::CDatabaseConnectionOdbc( SQLHENV sqlEnvironmentHandle, const String & server, const String & userName, const String & password, String & connectionString )
		:   CDatabaseConnection( server, userName, password )
		,   _connectionHandle( NULL )
		,   _environmentHandle( sqlEnvironmentHandle )
	{
	}

	CDatabaseConnectionOdbc::~CDatabaseConnectionOdbc()
	{
	}

	EErrorType CDatabaseConnectionOdbc::BeginTransaction( const String & name )
	{
		EErrorType eReturn = EErrorType_ERROR;

		if ( IsConnected() && !IsInTransaction() && DoExecuteUpdate( ODBC_SQL_TRANSACTION_BEGIN + name, &eReturn ) )
		{
			DoSetInTransaction( true );
		}

		return eReturn;
	}

	EErrorType CDatabaseConnectionOdbc::Commit( const String & name )
	{
		EErrorType eReturn = EErrorType_ERROR;

		if ( IsConnected() && IsInTransaction() && DoExecuteUpdate( ODBC_SQL_TRANSACTION_COMMIT + name, &eReturn ) )
		{
			DoSetInTransaction( false );
		}

		return eReturn;
	}

	EErrorType CDatabaseConnectionOdbc::RollBack( const String & name )
	{
		EErrorType eReturn = EErrorType_ERROR;

		if ( IsConnected() && IsInTransaction() && DoExecuteUpdate( ODBC_SQL_TRANSACTION_ROLLBACK + name, &eReturn ) )
		{
			DoSetInTransaction( false );
		}

		return eReturn;
	}

	std::string CDatabaseConnectionOdbc::WriteText( const std::string & text ) const
	{
		std::string strReturn( text );

		if ( strReturn != ODBC_SQL_SNULL )
		{
			Replace( strReturn, "'", "''" );
			Replace( strReturn, "\\", "\\\\" );
			strReturn = "'" + strReturn + "'";
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionOdbc::WriteNText( const std::wstring & text ) const
	{
		String strReturn( CStrUtils::ToString( text ) );

		if ( strReturn != ODBC_SQL_NULL )
		{
			Replace( strReturn, STR( "'" ), STR( "''" ) );
			Replace( strReturn, STR( "\\" ), STR( "\\\\" ) );
			strReturn = STR( "N'" ) + strReturn + STR( "'" );
		}

		return CStrUtils::ToWStr( strReturn );
	}

	String CDatabaseConnectionOdbc::WriteName( const String & text ) const
	{
		return STR( "[" ) + text + STR( "]" );
	}

	std::string CDatabaseConnectionOdbc::WriteDateS( const CDate & date ) const
	{
		std::string strReturn;

		if ( date.IsValid() )
		{
			Formalize( strReturn, 1024, ODBC_FORMAT_SDATE.c_str(), date.GetYear(), date.GetMonth(), date.GetMonthDay() );
		}
		else
		{
			strReturn += ODBC_SQL_SNULL;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionOdbc::WriteTimeS( const CTime & time ) const
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

	std::string CDatabaseConnectionOdbc::WriteDateTimeS( const CDateTime & dateTime ) const
	{
		std::string strReturn;

		if ( dateTime.GetYear() <= 0 )
		{
			strReturn += ODBC_SQL_SNULL;
		}
		else
		{
			Formalize( strReturn, 1024, ODBC_FORMAT_SDATETIME.c_str(), dateTime.GetYear(), dateTime.GetMonth(), dateTime.GetMonthDay(), dateTime.GetHour(), dateTime.GetMinute(), dateTime.GetSecond() );
		}

		return strReturn;
	}

	std::string CDatabaseConnectionOdbc::WriteDateTimeS( const CDate & date ) const
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

	std::string CDatabaseConnectionOdbc::WriteDateTimeS( const CTime & time ) const
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

	std::string CDatabaseConnectionOdbc::WriteStmtDateS( const CDate & date ) const
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

	std::string CDatabaseConnectionOdbc::WriteStmtTimeS( const CTime & time ) const
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

	std::string CDatabaseConnectionOdbc::WriteStmtDateTimeS( const CDateTime & dateTime ) const
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

	std::wstring CDatabaseConnectionOdbc::WriteDateW( const CDate & date ) const
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

	std::wstring CDatabaseConnectionOdbc::WriteTimeW( const CTime & time ) const
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

	std::wstring CDatabaseConnectionOdbc::WriteDateTimeW( const CDateTime & dateTime ) const
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

	std::wstring CDatabaseConnectionOdbc::WriteDateTimeW( const CDate & date ) const
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

	std::wstring CDatabaseConnectionOdbc::WriteDateTimeW( const CTime & time ) const
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

	std::wstring CDatabaseConnectionOdbc::WriteStmtDateW( const CDate & date ) const
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

	std::wstring CDatabaseConnectionOdbc::WriteStmtTimeW( const CTime & time ) const
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

	std::wstring CDatabaseConnectionOdbc::WriteStmtDateTimeW( const CDateTime & dateTime ) const
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

	String CDatabaseConnectionOdbc::WriteBool( bool value ) const
	{
		return ( value ? STR( "1" ) : STR( "0" ) );
	}

	String CDatabaseConnectionOdbc::WriteBool( const String & value ) const
	{
		const String lowerCaseValue = CStrUtils::LowerCase( value );
		return ( lowerCaseValue == STR( "x" ) || lowerCaseValue == STR( "oui" ) || lowerCaseValue == STR( "yes" ) || lowerCaseValue == STR( "y" ) || value == STR( "1" ) ? STR( "1" ) : STR( "0" ) );
	}

	CDate CDatabaseConnectionOdbc::ParseDate( const std::string & date ) const
	{
		CDate dateObj;
		CDate::IsDate( date, ODBC_FORMAT_STMT_SDATE, dateObj );
		return dateObj;
	}

	CTime CDatabaseConnectionOdbc::ParseTime( const std::string & time ) const
	{
		CTime timeObj;
		CTime::IsTime( time, ODBC_FORMAT_STMT_STIME, timeObj );
		return timeObj;
	}

	CDateTime CDatabaseConnectionOdbc::ParseDateTime( const std::string & dateTime ) const
	{
		CDateTime dateTimeObj;
		CDateTime::IsDateTime( dateTime, ODBC_FORMAT_STMT_SDATETIME, dateTimeObj );
		return dateTimeObj;
	}

	CDate CDatabaseConnectionOdbc::ParseDate( const std::wstring & date ) const
	{
		CDate dateObj;
		CDate::IsDate( date, ODBC_FORMAT_STMT_WDATE, dateObj );
		return dateObj;
	}

	CTime CDatabaseConnectionOdbc::ParseTime( const std::wstring & time ) const
	{
		CTime timeObj;
		CTime::IsTime( time, ODBC_FORMAT_STMT_WTIME, timeObj );
		return timeObj;
	}

	CDateTime CDatabaseConnectionOdbc::ParseDateTime( const std::wstring & dateTime ) const
	{
		CDateTime dateTimeObj;
		CDateTime::IsDateTime( dateTime, ODBC_FORMAT_STMT_WDATETIME, dateTimeObj );
		return dateTimeObj;
	}

	unsigned long CDatabaseConnectionOdbc::GetStmtDateSize()const
	{
		//"{-d YYYY-MM-DD}"
		return ( unsigned long )15;
	}

	unsigned long CDatabaseConnectionOdbc::GetStmtDateTimeSize()const
	{
		//"{-ts YYYY-MM-DD HH:MM:SS}"
		return ( unsigned long )25;
	}

	unsigned long CDatabaseConnectionOdbc::GetStmtTimeSize()const
	{
		//"{-t HH:MM:SS}"
		return ( unsigned long )13;
	}

	HDBC CDatabaseConnectionOdbc::GetHdbc() const
	{
		return _connectionHandle;
	}

	void CDatabaseConnectionOdbc::DoDisconnect()
	{
		if ( _connectionHandle != SQL_NULL_HDBC )
		{
			SQLDisconnect( _connectionHandle );
			SQLFreeHandle( SQL_HANDLE_DBC, _connectionHandle );
			_connectionHandle = NULL;
		}
	}

	bool CDatabaseConnectionOdbc::DoExecuteUpdate( const String & request, EErrorType * result )
	{
		bool bReturn;

		if ( IsConnected() )
		{
			DatabaseQueryPtr query = CreateQuery( request );
			bReturn = query->ExecuteUpdate( result );
		}

		return bReturn;
	}

	DatabaseResultPtr CDatabaseConnectionOdbc::DoExecuteSelect( const String & request, EErrorType * result )
	{
		DatabaseResultPtr pReturn;

		if ( IsConnected() )
		{
			DatabaseQueryPtr query = CreateQuery( request );
			pReturn = query->ExecuteSelect( result );
		}

		return pReturn;
	}
}
END_NAMESPACE_DATABASE_ODBC
