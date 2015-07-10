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
	static const String ODBC_TRANSACTION_BEGIN = STR( "BEGIN TRANSACTION " );
	static const String ODBC_TRANSACTION_COMMIT = STR( "COMMIT TRANSACTION " );
	static const String ODBC_TRANSACTION_ROLLBACK = STR( "ROLLBACK TRANSACTION " );

	static const String ODBC_DRIVER_ERROR = STR( "ODBC Driver error" );
	static const String ODBC_UNKNOWN_ERROR = STR( "Unknown error encountered while executing query: " );

	static const std::string ODBC_NULL_STDSTRING = "NULL";
	static const std::wstring ODBC_NULL_STDWSTRING = L"NULL";
	static const String ODBC_NULL_STRING = STR( "NULL" );

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

		if ( IsConnected() && !IsInTransaction() && DoExecuteUpdate( ODBC_TRANSACTION_BEGIN + name, &eReturn ) )
		{
			DoSetInTransaction( true );
		}

		return eReturn;
	}

	EErrorType CDatabaseConnectionOdbc::Commit( const String & name )
	{
		EErrorType eReturn = EErrorType_ERROR;

		if ( IsConnected() && IsInTransaction() && DoExecuteUpdate( ODBC_TRANSACTION_COMMIT + name, &eReturn ) )
		{
			DoSetInTransaction( false );
		}

		return eReturn;
	}

	EErrorType CDatabaseConnectionOdbc::RollBack( const String & name )
	{
		EErrorType eReturn = EErrorType_ERROR;

		if ( IsConnected() && IsInTransaction() && DoExecuteUpdate( ODBC_TRANSACTION_ROLLBACK + name, &eReturn ) )
		{
			DoSetInTransaction( false );
		}

		return eReturn;
	}

	std::string CDatabaseConnectionOdbc::WriteText( const std::string & text ) const
	{
		std::string strReturn( text );

		if ( strReturn != ODBC_NULL_STDSTRING )
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

		if ( strReturn != ODBC_NULL_STRING )
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

	std::string CDatabaseConnectionOdbc::WriteDate( const CDate & date ) const
	{
		std::string strReturn;

		if ( date.IsValid() )
		{
			if ( date.GetYear() < 0 )
			{
				Formalize( strReturn, 1024, "CAST('%05i%02i%02i' AS DATE)", date.GetYear(), date.GetMonth(), date.GetMonthDay() );
			}
			else
			{
				Formalize( strReturn, 1024, "CAST( '%04i%02i%02i' AS DATE)", date.GetYear(), date.GetMonth(), date.GetMonthDay() );
			}
		}
		else
		{
			strReturn += ODBC_NULL_STDSTRING;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionOdbc::WriteDate( const std::string & date, const std::string & format ) const
	{
		std::string strReturn;
		CDate dateObj;

		if ( CDate::IsDate( date, format, dateObj ) )
		{
			strReturn = WriteDate( dateObj );
		}
		else
		{
			strReturn += ODBC_NULL_STDSTRING;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionOdbc::WriteTime( const CTime & time ) const
	{
		std::string strReturn;

		if ( time.IsValid() )
		{
			Formalize( strReturn, 1024, "CONVERT(TIME, '%02i:%02i%:02i', 108)", time.GetHour(), time.GetMinute(), time.GetSecond() );
		}
		else
		{
			strReturn += ODBC_NULL_STDSTRING;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionOdbc::WriteTime( const std::string & time, const std::string & format ) const
	{
		std::string strReturn;
		CTime timeObj;

		if ( CTime::IsTime( time, format, timeObj ) )
		{
			strReturn = WriteTime( timeObj );
		}
		else
		{
			strReturn += ODBC_NULL_STDSTRING;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionOdbc::WriteDateTime( const CDateTime & dateTime ) const
	{
		std::string strReturn;

		if ( dateTime.GetYear() <= 0 )
		{
			strReturn += ODBC_NULL_STDSTRING;
		}
		else
		{
			Formalize( strReturn, 1024, "CAST('%04i-%02i-%02i %02i:%02i:%02i' as DATETIME)", dateTime.GetYear(), dateTime.GetMonth(), dateTime.GetMonthDay(), dateTime.GetHour(), dateTime.GetMinute(), dateTime.GetSecond() );
		}

		return strReturn;
	}

	std::string CDatabaseConnectionOdbc::WriteDateTime( const CDate & date ) const
	{
		std::string strReturn;

		if ( date.IsValid() )
		{
			if ( date.GetYear() <= 0 )
			{
				strReturn += ODBC_NULL_STDSTRING;
			}
			else
			{
				Formalize( strReturn, 1024, "CAST('%04i-%02i-%02i 00:00:00' as DATETIME)", date.GetYear(), date.GetMonth(), date.GetMonthDay() );
			}
		}
		else
		{
			strReturn += ODBC_NULL_STDSTRING;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionOdbc::WriteDateTime( const CTime & time ) const
	{
		std::string strReturn;

		if ( time.IsValid() )
		{
			Formalize( strReturn, 1024, "CAST('0000-00-00 %02i:%02i:%02i' as DATETIME)", time.GetHour(), time.GetMinute(), time.GetSecond() );
		}
		else
		{
			strReturn += ODBC_NULL_STDSTRING;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionOdbc::WriteDateTime( const std::string & dateTime, const std::string & format ) const
	{
		std::string strReturn;
		CDateTime dateTimeObj;

		if ( CDateTime::IsDateTime( dateTime,  dateTimeObj ) )
		{
			strReturn = WriteDateTime( dateTimeObj );
		}
		else
		{
			strReturn += ODBC_NULL_STDSTRING;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionOdbc::WriteStmtDate( const CDate & date ) const
	{
		std::string strReturn;

		if ( date.IsValid() )
		{
			Formalize( strReturn, 1024, "{-d %04i-%02i-%02i}", date.GetYear(), date.GetMonth(), date.GetMonthDay() );
		}
		else
		{
			strReturn += ODBC_NULL_STDSTRING;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionOdbc::WriteStmtDate( const std::string & date, const std::string & format ) const
	{
		std::string strReturn;
		CDate dateObj;

		if ( CDate::IsDate( date, format, dateObj ) )
		{
			strReturn = WriteStmtDate( dateObj );
		}
		else
		{
			strReturn += ODBC_NULL_STDSTRING;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionOdbc::WriteStmtTime( const CTime & time ) const
	{
		std::string strReturn;

		if ( time.IsValid() )
		{
			Formalize( strReturn, 1024, "{-t %02i:%02i:%02i}", time.GetHour(), time.GetMinute(), time.GetSecond() );
		}
		else
		{
			strReturn += ODBC_NULL_STDSTRING;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionOdbc::WriteStmtTime( const std::string & time, const std::string & format ) const
	{
		std::string strReturn;
		CTime timeObj;

		if ( CTime::IsTime( time, format, timeObj ) )
		{
			strReturn = WriteStmtTime( timeObj );
		}
		else
		{
			strReturn += ODBC_NULL_STDSTRING;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionOdbc::WriteStmtDateTime( const CDateTime & dateTime ) const
	{
		std::string strReturn;

		if ( dateTime.GetYear() > 0 )
		{
			Formalize( strReturn, 1024, "{-ts %04i-%02i-%02i %02i:%02i:%02i}", dateTime.GetYear(), dateTime.GetMonth(), dateTime.GetMonthDay(), dateTime.GetHour(), dateTime.GetMinute(), dateTime.GetSecond() );
		}
		else
		{
			strReturn += ODBC_NULL_STDSTRING;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionOdbc::WriteStmtDateTime( const std::string & dateTime, const std::string & format ) const
	{
		std::string strReturn;
		CDateTime dateTimeObj;

		if ( CDateTime::IsDateTime( dateTime, dateTimeObj ) )
		{
			strReturn = WriteStmtDateTime( dateTimeObj );
		}
		else
		{
			strReturn += ODBC_NULL_STDSTRING;
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

		if ( !CDate::IsDate( date, "CAST('%Y%M%D' AS DATE)", dateObj )
		&& !CDate::IsDate( date, "{-d %Y-%M-%D}", dateObj )
		&& !CDate::IsDate( date, "%Y%M%D", dateObj )
		&& !CDate::IsDate( date, "%Y-%M-%D", dateObj )
		&& !CDate::IsDate( date, "%D/%M/%Y", dateObj ) )
		{
			dateObj = CDate( 0, EDateMonth_UNDEF, 0 );
		}

		return dateObj;
	}

	CTime CDatabaseConnectionOdbc::ParseTime( const std::string & time ) const
	{
		CTime timeObj;
		CTime::IsTime( time, timeObj );
		return timeObj;
	}

	CDateTime CDatabaseConnectionOdbc::ParseDateTime( const std::string & dateTime ) const
	{
		CDateTime dateTimeObj;
		CDateTime::IsDateTime( dateTime,  dateTimeObj );
		return dateTimeObj;
	}

	CDate CDatabaseConnectionOdbc::ParseDate( const std::wstring & date ) const
	{
		CDate dateObj;

		if ( !CDate::IsDate( date, L"CAST('%Y%M%D' AS DATE)", dateObj )
		&& !CDate::IsDate( date, L"{-d %Y-%M-%D}", dateObj )
		&& !CDate::IsDate( date, L"%Y%M%D", dateObj )
		&& !CDate::IsDate( date, L"%Y-%M-%D", dateObj )
		&& !CDate::IsDate( date, L"%D/%M/%Y", dateObj ) )
		{
			dateObj = CDate( 0, EDateMonth_UNDEF, 0 );
		}

		return dateObj;
	}

	CTime CDatabaseConnectionOdbc::ParseTime( const std::wstring & time ) const
	{
		CTime timeObj;
		CTime::IsTime( time, timeObj );
		return timeObj;
	}

	CDateTime CDatabaseConnectionOdbc::ParseDateTime( const std::wstring & dateTime ) const
	{
		CDateTime dateTimeObj;
		CDateTime::IsDateTime( dateTime,  dateTimeObj );
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
