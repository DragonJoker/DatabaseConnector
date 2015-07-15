/************************************************************************//**
* @file DatabaseConnection.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDatabaseConnection class definition.
*
* @details Describes a database connection.
*
***************************************************************************/

#include "DatabasePch.h"

#include "DatabaseConnection.h"
#include "DatabaseQuery.h"

BEGIN_NAMESPACE_DATABASE
{
	static const std::string DB_SQL_SNULL = "NULL";
	static const std::wstring DB_SQL_WNULL = L"NULL";

	CDatabaseConnection::CDatabaseConnection( const String & server, const String & userName, const String & password )
		: _connected( false )
		, _inTransaction( false )
		, _server( server )
		, _userName( userName )
		, _password( password )
	{
		// Empty
	}

	CDatabaseConnection::~CDatabaseConnection()
	{
		// Empty
	}

	EErrorType CDatabaseConnection::Reconnect( String & connectionString )
	{
		DoDisconnect();
		return DoConnect( connectionString );
	}

	DatabaseStatementPtr CDatabaseConnection::CreateStatement( const String & query, EErrorType * result )
	{
		return DoCreateStatement( query, result );
	}

	DatabaseQueryPtr CDatabaseConnection::CreateQuery( const String & query, EErrorType * result )
	{
		return DoCreateQuery( query, result );
	}

	bool CDatabaseConnection::ExecuteUpdate( const String & query, EErrorType * result )
	{
		return DoExecuteUpdate( query, result );
	}

	DatabaseResultPtr CDatabaseConnection::ExecuteSelect( const String & query, EErrorType * result )
	{
		return DoExecuteSelect( query, result );
	}

	bool CDatabaseConnection::IsConnected() const
	{
		return _connected;
	}

	bool CDatabaseConnection::IsInTransaction() const
	{
		return _inTransaction;
	}

	std::string CDatabaseConnection::WriteDate( const std::string & date, const std::string & format ) const
	{
		std::string strReturn;
		CDate dateObj;

		if ( CDate::IsDate( date, format, dateObj ) )
		{
			strReturn = WriteDateS( dateObj );
		}
		else
		{
			strReturn += DB_SQL_SNULL;
		}

		return strReturn;
	}

	std::string CDatabaseConnection::WriteTime( const std::string & time, const std::string & format ) const
	{
		std::string strReturn;
		CTime timeObj;

		if ( CTime::IsTime( time, format, timeObj ) )
		{
			strReturn = WriteTimeS( timeObj );
		}
		else
		{
			strReturn += DB_SQL_SNULL;
		}

		return strReturn;
	}

	std::string CDatabaseConnection::WriteDateTime( const std::string & dateTime, const std::string & format ) const
	{
		std::string strReturn;
		CDateTime dateTimeObj;

		if ( CDateTime::IsDateTime( dateTime, format, dateTimeObj ) )
		{
			strReturn = WriteDateTimeS( dateTimeObj );
		}
		else
		{
			strReturn += DB_SQL_SNULL;
		}

		return strReturn;
	}

	std::string CDatabaseConnection::WriteStmtDate( const std::string & date, const std::string & format ) const
	{
		std::string strReturn;
		CDate dateObj;

		if ( CDate::IsDate( date, format, dateObj ) )
		{
			strReturn = WriteStmtDateS( dateObj );
		}
		else
		{
			strReturn += DB_SQL_SNULL;
		}

		return strReturn;
	}

	std::string CDatabaseConnection::WriteStmtTime( const std::string & time, const std::string & format ) const
	{
		std::string strReturn;
		CTime timeObj;

		if ( CTime::IsTime( time, format, timeObj ) )
		{
			strReturn = WriteStmtTimeS( timeObj );
		}
		else
		{
			strReturn += DB_SQL_SNULL;
		}

		return strReturn;
	}

	std::string CDatabaseConnection::WriteStmtDateTime( const std::string & dateTime, const std::string & format ) const
	{
		std::string strReturn;
		CDateTime dateTimeObj;

		if ( CDateTime::IsDateTime( dateTime, format, dateTimeObj ) )
		{
			strReturn = WriteStmtDateTimeS( dateTimeObj );
		}
		else
		{
			strReturn += DB_SQL_SNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnection::WriteDate( const std::wstring & date, const std::wstring & format ) const
	{
		std::wstring strReturn;
		CDate dateObj;

		if ( CDate::IsDate( date, format, dateObj ) )
		{
			strReturn = WriteDateW( dateObj );
		}
		else
		{
			strReturn = DB_SQL_WNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnection::WriteTime( const std::wstring & time, const std::wstring & format ) const
	{
		std::wstring strReturn;
		CTime timeObj;

		if ( CTime::IsTime( time, format, timeObj ) )
		{
			strReturn = WriteTimeW( timeObj );
		}
		else
		{
			strReturn = DB_SQL_WNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnection::WriteDateTime( const std::wstring & dateTime, const std::wstring & format ) const
	{
		std::wstring strReturn;
		CDateTime dateTimeObj;

		if ( CDateTime::IsDateTime( dateTime, format, dateTimeObj ) )
		{
			strReturn = WriteDateTimeW( dateTimeObj );
		}
		else
		{
			strReturn = DB_SQL_WNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnection::WriteStmtDate( const std::wstring & date, const std::wstring & format ) const
	{
		std::wstring strReturn;
		CDate dateObj;

		if ( CDate::IsDate( date, format, dateObj ) )
		{
			strReturn = WriteStmtDateW( dateObj );
		}
		else
		{
			strReturn = DB_SQL_WNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnection::WriteStmtTime( const std::wstring & time, const std::wstring & format ) const
	{
		std::wstring strReturn;
		CTime timeObj;

		if ( CTime::IsTime( time, format, timeObj ) )
		{
			strReturn = WriteStmtTimeW( timeObj );
		}
		else
		{
			strReturn = DB_SQL_WNULL;
		}

		return strReturn;
	}

	std::wstring CDatabaseConnection::WriteStmtDateTime( const std::wstring & dateTime, const std::wstring & format ) const
	{
		std::wstring strReturn;
		CDateTime dateTimeObj;

		if ( CDateTime::IsDateTime( dateTime, format, dateTimeObj ) )
		{
			strReturn = WriteStmtDateTimeW( dateTimeObj );
		}
		else
		{
			strReturn = DB_SQL_WNULL;
		}

		return strReturn;
	}

	String CDatabaseConnection::WriteDate( const CDate & date ) const
	{
		return WriteDateS( date );
	}

	String CDatabaseConnection::WriteStmtDate( const CDate & date ) const
	{
		return WriteStmtDateS( date );
	}

	String CDatabaseConnection::WriteTime( const CTime & time ) const
	{
		return WriteTimeS( time );
	}

	String CDatabaseConnection::WriteStmtTime( const CTime & time ) const
	{
		return WriteStmtTimeS( time );
	}

	String CDatabaseConnection::WriteDateTime( const CDateTime & dateTime ) const
	{
		return WriteDateTimeS( dateTime );
	}

	String CDatabaseConnection::WriteDateTime( const CDate & date ) const
	{
		return WriteDateTimeS( date );
	}

	String CDatabaseConnection::WriteDateTime( const CTime & time ) const
	{
		return WriteDateTimeS( time );
	}

	String CDatabaseConnection::WriteStmtDateTime( const CDateTime & dateTime ) const
	{
		return WriteStmtDateTimeS( dateTime );
	}

	void CDatabaseConnection::DoSetConnected( bool value )
	{
		_connected = value;
	}

	void CDatabaseConnection::DoSetInTransaction( bool value )
	{
		_inTransaction = value;
	}
}
END_NAMESPACE_DATABASE
