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
	CDatabaseConnection::CDatabaseConnection( const String & server, const String & database, const String & userName, const String & password )
		:   _connected( false )
		,   _inTransaction( false )
		,   _server( server )
		,   _database( database )
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
