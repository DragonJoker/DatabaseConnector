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
#include "DatabaseStringUtils.h"
#include "DatabaseDate.h"
#include "DatabaseDateTime.h"
#include "DatabaseTime.h"

BEGIN_NAMESPACE_DATABASE
{
	static const std::string DB_SQL_SNULL = "NULL";
	static const std::wstring DB_SQL_WNULL = L"NULL";

	static const String ERROR_DB_NOT_CONNECTED = STR( "Not connected" );
	static const String ERROR_DB_EXECUTION_ERROR = STR( "Query execution error: " );
	static const String ERROR_DB_NOT_IN_TRANSACTION = STR( "Not in a transaction" );
	static const String ERROR_DB_ALREADY_IN_TRANSACTION = STR( "Already in a transaction" );
	static const String ERROR_DB_CANT_BEGIN_TRANSACTION = STR( "Can't begint the transaction" );
	static const String ERROR_DB_CANT_COMMIT_TRANSACTION = STR( "Can't commit the transaction" );
	static const String ERROR_DB_CANT_ROLLBACK_TRANSACTION = STR( "Can't rollback the transaction" );

	static const String INFO_DB_EXECUTING_SELECT = STR( "Executing query: " );

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

	void CDatabaseConnection::BeginTransaction( const String & name )
	{
		if ( !IsConnected() )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_DB_NOT_CONNECTED );
		}

		if ( IsInTransaction() )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_DB_ALREADY_IN_TRANSACTION );
		}

		if ( !DoBeginTransaction( name ) )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_DB_CANT_BEGIN_TRANSACTION );
		}

		DoSetInTransaction( true );
	}

	void CDatabaseConnection::Commit( const String & name )
	{
		if ( !IsConnected() )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_DB_NOT_CONNECTED );
		}

		if ( !IsInTransaction() )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_DB_NOT_IN_TRANSACTION );
		}

		if ( !DoCommit( name ) )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_DB_CANT_COMMIT_TRANSACTION );
		}

		DoSetInTransaction( false );
	}

	void CDatabaseConnection::RollBack( const String & name )
	{
		if ( !IsConnected() )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_DB_NOT_CONNECTED );
		}

		if ( !IsInTransaction() )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_DB_NOT_IN_TRANSACTION );
		}

		if ( !DoRollBack( name ) )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_DB_CANT_ROLLBACK_TRANSACTION );
		}

		DoSetInTransaction( false );
	}

	DatabaseStatementSPtr CDatabaseConnection::CreateStatement( const String & query )
	{
		if ( !IsConnected() )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_DB_NOT_CONNECTED );
		}

		return DoCreateStatement( query );
	}

	DatabaseQuerySPtr CDatabaseConnection::CreateQuery( const String & query )
	{
		return std::make_shared< CDatabaseQuery >( shared_from_this(), query );
	}

	bool CDatabaseConnection::ExecuteUpdate( const String & query )
	{
		if ( !IsConnected() )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_DB_NOT_CONNECTED );
		}

		CLogger::LogInfo( INFO_DB_EXECUTING_SELECT + query );
		bool ret = false;

		try
		{
			ret = DoExecuteUpdate( query );
		}
		catch ( CDatabaseException & exc )
		{
			StringStream stream;
			stream << ERROR_DB_EXECUTION_ERROR << exc.GetFullDescription();
			CLogger::LogError( stream );
		}
		catch ( std::exception & exc )
		{
			StringStream stream;
			stream << ERROR_DB_EXECUTION_ERROR << exc.what();
			CLogger::LogError( stream );
		}
		catch ( ... )
		{
			StringStream stream;
			stream << ERROR_DB_EXECUTION_ERROR << STR( "UNKNOWN" );
			CLogger::LogError( stream );
		}

		return ret;
	}

	DatabaseResultSPtr CDatabaseConnection::ExecuteSelect( const String & query )
	{
		if ( !IsConnected() )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_DB_NOT_CONNECTED );
		}
		
		CLogger::LogInfo( INFO_DB_EXECUTING_SELECT + query );
		DatabaseResultSPtr ret;

		try
		{
			ret = DoExecuteSelect( query );
		}
		catch ( CDatabaseException & exc )
		{
			StringStream stream;
			stream << ERROR_DB_EXECUTION_ERROR << exc.GetFullDescription();
			CLogger::LogError( stream );
		}
		catch ( std::exception & exc )
		{
			StringStream stream;
			stream << ERROR_DB_EXECUTION_ERROR << STR( " - " ) << exc.what();
			CLogger::LogError( stream );
		}
		catch ( ... )
		{
			StringStream stream;
			stream << ERROR_DB_EXECUTION_ERROR << STR( " - UNKNOWN" );
			CLogger::LogError( stream );
		}

		return ret;
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
		DateType dateObj;

		if ( Date::IsDate( date, format, dateObj ) )
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
		TimeType timeObj;

		if ( Time::IsTime( time, format, timeObj ) )
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
		DateTimeType dateTimeObj;

		if ( DateTime::IsDateTime( dateTime, format, dateTimeObj ) )
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
		DateType dateObj;

		if ( Date::IsDate( date, format, dateObj ) )
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
		TimeType timeObj;

		if ( Time::IsTime( time, format, timeObj ) )
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
		DateTimeType dateTimeObj;

		if ( DateTime::IsDateTime( dateTime, format, dateTimeObj ) )
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
		DateType dateObj;

		if ( Date::IsDate( date, format, dateObj ) )
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
		TimeType timeObj;

		if ( Time::IsTime( time, format, timeObj ) )
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
		DateTimeType dateTimeObj;

		if ( DateTime::IsDateTime( dateTime, format, dateTimeObj ) )
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
		DateType dateObj;

		if ( Date::IsDate( date, format, dateObj ) )
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
		TimeType timeObj;

		if ( Time::IsTime( time, format, timeObj ) )
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
		DateTimeType dateTimeObj;

		if ( DateTime::IsDateTime( dateTime, format, dateTimeObj ) )
		{
			strReturn = WriteStmtDateTimeW( dateTimeObj );
		}
		else
		{
			strReturn = DB_SQL_WNULL;
		}

		return strReturn;
	}

	String CDatabaseConnection::WriteDate( const DateType & date ) const
	{
		return WriteDateS( date );
	}

	String CDatabaseConnection::WriteStmtDate( const DateType & date ) const
	{
		return WriteStmtDateS( date );
	}

	String CDatabaseConnection::WriteTime( const TimeType & time ) const
	{
		return WriteTimeS( time );
	}

	String CDatabaseConnection::WriteStmtTime( const TimeType & time ) const
	{
		return WriteStmtTimeS( time );
	}

	String CDatabaseConnection::WriteDateTime( const DateTimeType & dateTime ) const
	{
		return WriteDateTimeS( dateTime );
	}

	String CDatabaseConnection::WriteDateTime( const DateType & date ) const
	{
		return WriteDateTimeS( date );
	}

	String CDatabaseConnection::WriteDateTime( const TimeType & time ) const
	{
		return WriteDateTimeS( time );
	}

	String CDatabaseConnection::WriteStmtDateTime( const DateTimeType & dateTime ) const
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
