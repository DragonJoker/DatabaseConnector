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
	static const String NULL_STDSTRING = STR( "NULL" );

	CDatabaseConnection::CDatabaseConnection( const String & server, const String & userName, const String & password )
		:   _connected( false )
		,   _inTransaction( false )
		,   _server( server )
		,   _userName( userName )
		,   _password( password )
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
			strReturn = WriteDate( dateObj );
		}
		else
		{
			strReturn += NULL_STDSTRING;
		}

		return strReturn;
	}

	std::string CDatabaseConnection::WriteTime( const std::string & time, const std::string & format ) const
	{
		std::string strReturn;
		CTime timeObj;

		if ( CTime::IsTime( time, format, timeObj ) )
		{
			strReturn = WriteTime( timeObj );
		}
		else
		{
			strReturn += NULL_STDSTRING;
		}

		return strReturn;
	}

	std::string CDatabaseConnection::WriteDateTime( const std::string & dateTime, const std::string & format ) const
	{
		std::string strReturn;
		CDateTime dateTimeObj;

		if ( CDateTime::IsDateTime( dateTime, format, dateTimeObj ) )
		{
			strReturn = WriteDateTime( dateTimeObj );
		}
		else
		{
			strReturn += NULL_STDSTRING;
		}

		return strReturn;
	}

	std::string CDatabaseConnection::WriteStmtDate( const std::string & date, const std::string & format ) const
	{
		std::string strReturn;
		CDate dateObj;

		if ( CDate::IsDate( date, format, dateObj ) )
		{
			strReturn = WriteStmtDate( dateObj );
		}
		else
		{
			strReturn += NULL_STDSTRING;
		}

		return strReturn;
	}

	std::string CDatabaseConnection::WriteStmtTime( const std::string & time, const std::string & format ) const
	{
		std::string strReturn;
		CTime timeObj;

		if ( CTime::IsTime( time, format, timeObj ) )
		{
			strReturn = WriteStmtTime( timeObj );
		}
		else
		{
			strReturn += NULL_STDSTRING;
		}

		return strReturn;
	}

	std::string CDatabaseConnection::WriteStmtDateTime( const std::string & dateTime, const std::string & format ) const
	{
		std::string strReturn;
		CDateTime dateTimeObj;

		if ( CDateTime::IsDateTime( dateTime, format, dateTimeObj ) )
		{
			strReturn = WriteStmtDateTime( dateTimeObj );
		}
		else
		{
			strReturn += NULL_STDSTRING;
		}

		return strReturn;
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
