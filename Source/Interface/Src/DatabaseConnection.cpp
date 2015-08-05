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
#include "DatabaseException.h"
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
	static const String ERROR_DB_NO_DATABASE_SELECTED = STR( "No database selected" );
	static const String ERROR_DB_EXECUTION_ERROR = STR( "Query execution error: " );
	static const String ERROR_DB_NOT_IN_TRANSACTION = STR( "Not in a transaction" );
	static const String ERROR_DB_ALREADY_IN_TRANSACTION = STR( "Already in a transaction" );
	static const String ERROR_DB_CANT_BEGIN_TRANSACTION = STR( "Can't begint the transaction" );
	static const String ERROR_DB_CANT_COMMIT_TRANSACTION = STR( "Can't commit the transaction" );
	static const String ERROR_DB_CANT_ROLLBACK_TRANSACTION = STR( "Can't rollback the transaction" );

	static const String INFO_DB_EXECUTING_UPDATE = STR( "Executing No Result query" );
	static const String INFO_DB_EXECUTING_SELECT = STR( "Executing Result query" );

	CDatabaseConnection::CDatabaseConnection( const String & server, const String & userName, const String & password )
		: _connected( false )
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

	bool CDatabaseConnection::IsConnected() const
	{
		return _connected;
	}

	void CDatabaseConnection::BeginTransaction( const String & name )
	{
		DoCheckConnected();
		DoCheckDatabaseSelected();

		if ( IsInTransaction( name ) )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_DB_ALREADY_IN_TRANSACTION );
		}

		if ( !DoBeginTransaction( name ) )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_DB_CANT_BEGIN_TRANSACTION );
		}

		DoStartTransaction( name );
	}

	void CDatabaseConnection::Commit( const String & name )
	{
		DoCheckConnected();
		DoCheckDatabaseSelected();

		if ( !IsInTransaction( name ) )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_DB_NOT_IN_TRANSACTION );
		}

		if ( !DoCommit( name ) )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_DB_CANT_COMMIT_TRANSACTION );
		}

		DoFinishTransaction( name );
	}

	void CDatabaseConnection::RollBack( const String & name )
	{
		DoCheckConnected();
		DoCheckDatabaseSelected();

		if ( !IsInTransaction( name ) )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_DB_NOT_IN_TRANSACTION );
		}

		if ( !DoRollBack( name ) )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_DB_CANT_ROLLBACK_TRANSACTION );
		}

		DoFinishTransaction( name );
	}

	bool CDatabaseConnection::IsInTransaction() const
	{
		return !_startedTransactions.empty();
	}

	bool CDatabaseConnection::IsInTransaction( const String & name ) const
	{
		return _startedTransactions.find( name ) != _startedTransactions.end();
	}

	DatabaseStatementSPtr CDatabaseConnection::CreateStatement( const String & query )
	{
		DoCheckConnected();
		DoCheckDatabaseSelected();
		return DoCreateStatement( query );
	}

	DatabaseQuerySPtr CDatabaseConnection::CreateQuery( const String & query )
	{
		DoCheckConnected();
		DoCheckDatabaseSelected();
		return std::make_shared< CDatabaseQuery >( shared_from_this(), query );
	}

	bool CDatabaseConnection::ExecuteUpdate( const String & query )
	{
		DoCheckConnected();
		DoCheckDatabaseSelected();
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
		DoCheckConnected();
		DoCheckDatabaseSelected();
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

	bool CDatabaseConnection::CreateDatabase( const String & database )
	{
		DoCheckConnected();
		bool ret = false;

		try
		{
			DoCreateDatabase( database );
			ret = true;
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

	bool CDatabaseConnection::SelectDatabase( const String & database )
	{
		DoCheckConnected();
		bool ret = false;

		try
		{
			DoSelectDatabase( database );
			ret = true;
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

	bool CDatabaseConnection::DestroyDatabase( const String & database )
	{
		DoCheckConnected();
		bool ret = false;

		try
		{
			DoDestroyDatabase( database );
			ret = true;
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

	String CDatabaseConnection::WriteDate( const DateType & date ) const
	{
		DoCheckConnected();
		return DoWriteDate( date );
	}

	String CDatabaseConnection::WriteStmtDate( const DateType & date ) const
	{
		DoCheckConnected();
		return DoWriteStmtDate( date );
	}

	String CDatabaseConnection::WriteTime( const TimeType & time ) const
	{
		DoCheckConnected();
		return DoWriteTime( time );
	}

	String CDatabaseConnection::WriteStmtTime( const TimeType & time ) const
	{
		DoCheckConnected();
		return DoWriteStmtTime( time );
	}

	String CDatabaseConnection::WriteDateTime( const DateTimeType & dateTime ) const
	{
		DoCheckConnected();
		return DoWriteDateTime( dateTime );
	}

	String CDatabaseConnection::WriteDateTime( const DateType & date ) const
	{
		DoCheckConnected();
		return DoWriteDateTime( date );
	}

	String CDatabaseConnection::WriteDateTime( const TimeType & time ) const
	{
		DoCheckConnected();
		return DoWriteDateTime( time );
	}

	String CDatabaseConnection::WriteStmtDateTime( const DateTimeType & dateTime ) const
	{
		DoCheckConnected();
		return DoWriteStmtDateTime( dateTime );
	}

	std::string CDatabaseConnection::WriteText( const std::string & text ) const
	{
		DoCheckConnected();
		return DoWriteText( text );
	}

	std::wstring CDatabaseConnection::WriteNText( const std::wstring & text ) const
	{
		DoCheckConnected();
		return DoWriteNText( text );
	}

	String CDatabaseConnection::WriteBinary( const ByteArray & array ) const
	{
		DoCheckConnected();
		return DoWriteBinary( array );
	}

	String CDatabaseConnection::WriteName( const String & name ) const
	{
		DoCheckConnected();
		return DoWriteName( name );
	}

	String CDatabaseConnection::WriteBool( bool value ) const
	{
		DoCheckConnected();
		return DoWriteBool( value );
	}

	String CDatabaseConnection::WriteFloat( float value ) const
	{
		DoCheckConnected();
		return DoWriteFloat( value );
	}

	String CDatabaseConnection::WriteDouble( double value ) const
	{
		DoCheckConnected();
		return DoWriteDouble( value );
	}

	DateType CDatabaseConnection::ParseDate( const String & date ) const
	{
		DoCheckConnected();
		return DoParseDate( date );
	}

	TimeType CDatabaseConnection::ParseTime( const String & time ) const
	{
		DoCheckConnected();
		return DoParseTime( time );
	}

	DateTimeType CDatabaseConnection::ParseDateTime( const String & dateTime ) const
	{
		DoCheckConnected();
		return DoParseDateTime( dateTime );
	}

	void CDatabaseConnection::DoCheckConnected() const
	{
		if ( !IsConnected() )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_DB_NOT_CONNECTED );
		}
	}

	void CDatabaseConnection::DoCheckDatabaseSelected() const
	{
		if ( _database.empty() )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_DB_NO_DATABASE_SELECTED );
		}
	}

	void CDatabaseConnection::DoSetConnected( bool value )
	{
		_connected = value;
	}

	void CDatabaseConnection::DoStartTransaction( const String & name )
	{
		_startedTransactions.insert( name );
	}

	void CDatabaseConnection::DoFinishTransaction( const String & name )
	{
		_startedTransactions.erase( name );
	}

	String CDatabaseConnection::DoWriteFloat( float value ) const
	{
		StringStream stream;
		stream.precision( GetPrecision( EFieldType_FLOAT32 ) );
		stream << value;
		return stream.str();
	}

	String CDatabaseConnection::DoWriteDouble( double value ) const
	{
		StringStream stream;
		stream.precision( GetPrecision( EFieldType_FLOAT64 ) );
		stream << value;
		return stream.str();
	}
}
END_NAMESPACE_DATABASE
