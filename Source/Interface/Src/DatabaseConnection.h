/************************************************************************//**
 * @file DatabaseConnection.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 *
 *
 * @brief CDatabaseConnection class declaration.
 *
 * @details Describes a database connection.
 *
 ***************************************************************************/

#ifndef ___DATABASE_CONNECTION_H___
#define ___DATABASE_CONNECTION_H___

#include "DatabasePrerequisites.h"

#include "EErrorType.h"

BEGIN_NAMESPACE_DATABASE
{
	/** Describes a database connection.
	*/
	class DatabaseExport CDatabaseConnection
	{

	public:

		/** Constructor.
		@param[in] server
		    Server identifier (name or address).
		@param[in] database
		    Database identifier (name or DSN (ODBC)).
		@param[in] userName
		    User name.
		@param[in] password
		    User password.
		*/
		CDatabaseConnection( const String & server, const String & database, const String & userName, const String & password );

		/** Destructor.
		 */
		virtual ~CDatabaseConnection();

		/** Initialize the connection to the database.
		@param[out] connectionString
		    Connection string in case of error.
		@return
		    Error code.
		*/
		EErrorType Reconnect( String & connectionString );

		/** Create a statement based on a request.
		@param[in] query
		    Request text.
		@param[out] result
		    Error code.
		@return
		    Created statement.
		*/
		DatabaseStatementPtr CreateStatement( const String & query, EErrorType * result = NULL );

		/** Create a query based on a request.
		@param[in] query
		    Request text.
		@param[out] result
		    Error code.
		@return
		    Created query.
		*/
		DatabaseQueryPtr CreateQuery( const String & query, EErrorType * result = NULL );

		/** Execute directly a request without result set.
		@param[in] query
		    Request text.
		@param[out] result
		    Error code.
		@return
		    Results.
		*/
		bool ExecuteUpdate( const String & query, EErrorType * result = NULL );

		/** Execute directly a request with a result set.
		@param[in] query
		    Request text.
		@param[out] result
		    Error code.
		@return
		    Results.
		*/
		DatabaseResultPtr ExecuteSelect( const String & query, EErrorType * result = NULL );

		/** Initialize a named transaction.
		@param[in] name
		    Transaction name.
		@return
		    Error code, EErrorType_NONE if no problem.
		*/
		virtual EErrorType BeginTransaction( const String & name ) = 0;

		/** Validate a named transaction.
		@param[in] name
		    Transaction name.
		@return
		    Error code, EErrorType_NONE if no problem.
		*/
		virtual EErrorType Commit( const String & name ) = 0;

		/** Invalidate a named transaction.
		@param[in] name
		    Transaction name.
		@return
		    Error code, EErrorType_NONE if no problem.
		*/
		virtual EErrorType RollBack( const String & name ) = 0;

		/** Format a string to insert into a request.
		@param[in] text
		    Value to format.
		@return
		    Formatted value.
		*/
		virtual std::string WriteText( const std::string & text ) const = 0;

		/** Format a string to insert into a request.
		@param[in] text
		    Value to format.
		@return
		    Formatted value.
		*/
		virtual std::wstring WriteNText( const std::wstring & text ) const = 0;

		/** Format a string to insert into a request.
		@param[in] name
		    Value to format.
		@return
		    Formatted value.
		*/
		virtual String WriteName( const String & name ) const = 0;

		/** Format a date to insert into a request.
		@param[in] date
		    Value to format.
		@return
		    Formatted value.
		*/
		virtual std::string WriteDate( const CDate & date ) const = 0;

		/** Format a date to insert into a request.
		@param[in] date
		    Value to format.
		@param[in] format
		    String describing the format.
		@return
		    Formatted value.
		*/
		virtual std::string WriteDate( const std::string & date, const std::string & format ) const = 0;

		/** Format a date to insert into a statement.
		@param[in] date
		    Value to format.
		@return
		    Formatted value.
		*/
		virtual std::string WriteStmtDate( const CDate & date ) const = 0;

		/** Format a date to insert into a statement.
		@param[in] date
		    Value to format.
		@param[in] format
		    String describing the format.
		@return
		    Formatted value.
		*/
		virtual std::string WriteStmtDate( const std::string & date, const std::string & format ) const = 0;

		/** Format a time to insert into a request.
		@param[in] time
		    Value to format.
		@return
		    Formatted value.
		*/
		virtual std::string WriteTime( const CTime & time ) const = 0;

		/** Format a time to insert into a request.
		@param[in] time
		    Value to format.
		@param[in] format
		    String describing the format.
		@return
		    Formatted value.
		*/
		virtual std::string WriteTime( const std::string & time, const std::string & format ) const = 0;

		/** Format a time to insert into a statement.
		@param[in] time
		    Value to format.
		@return
		    Formatted value.
		*/
		virtual std::string WriteStmtTime( const CTime & time ) const = 0;

		/** Format a time to insert into a statement.
		@param[in] time
		    Value to format.
		@param[in] format
		    String describing the format.
		@return
		    Formatted value.
		*/
		virtual std::string WriteStmtTime( const std::string & time, const std::string & format ) const = 0;

		/** Format a date/time to insert into a request.
		@param[in] dateTime
		    Value to format.
		@return
		    Formatted value.
		*/
		virtual std::string WriteDateTime( const CDateTime & dateTime ) const = 0;

		/** Format a date/time to insert into a request.
		@param[in] date
		    Value to format.
		@return
		    Formatted value.
		*/
		virtual std::string WriteDateTime( const CDate & date ) const = 0;

		/** Format a date/time to insert into a request.
		@param[in] time
		    Value to format.
		@return
		    Formatted value.
		*/
		virtual std::string WriteDateTime( const CTime & time ) const = 0;

		/** Format a date/time to insert into a request.
		@param[in] dateTime
		    Value to format.
		@param[in] format
		    String describing the format.
		@return
		    Formatted value.
		*/
		virtual std::string WriteDateTime( const std::string & dateTime, const std::string & format ) const = 0;

		/** Format a date/time to insert into a statement.
		@param[in] dateTime
		    Value to format.
		@return
		    Formatted value.
		*/
		virtual std::string WriteStmtDateTime( const CDateTime & dateTime ) const = 0;

		/** Format a date/time to insert into a statement.
		@param[in] dateTime
		    Value to format.
		@param[in] format
		    String describing the format.
		@return
		    Formatted value.
		*/
		virtual std::string WriteStmtDateTime( const std::string & dateTime, const std::string & format ) const = 0;

		/** Format a boolean to insert into a request or statement.
		@param[in] value
		    Value to format.
		@return
		    Formatted value.
		*/
		virtual String WriteBool( bool value ) const = 0;

		/** Format a boolean to insert into a request or statement.
		@param[in] value
		    Value to format.
		@return
		    Formatted value.
		*/
		virtual String WriteBool( const String & value ) const = 0;

		/** Convert a string from the database to a date.
		@param[in] date
		    String representing a date.
		@return
		    Created date with the string.
		*/
		virtual CDate ParseDate( const std::string & date ) const = 0;

		/** Convert a string from the database to a time.
		@param[in] time
		    String representing a time.
		@return
		    Created time with the string.
		*/
		virtual CTime ParseTime( const std::string & time ) const = 0;

		/** Convert a string from the database to a date/time.
		@param[in] dateTime
		    String representing a date/time.
		@return
		    Created date/time with the string.
		*/
		virtual CDateTime ParseDateTime( const std::string & dateTime ) const = 0;

		/** Convert a string from the database to a date.
		@param[in] date
		    String representing a date.
		@return
		    Created date with the string.
		*/
		virtual CDate ParseDate( const std::wstring & date ) const = 0;

		/** Convert a string from the database to a time.
		@param[in] time
		    String representing a time.
		@return
		    Created time with the string.
		*/
		virtual CTime ParseTime( const std::wstring & time ) const = 0;

		/** Convert a string from the database to a date/time.
		@param[in] dateTime
		    String representing a date/time.
		@return
		    Created date/time with the string.
		*/
		virtual CDateTime ParseDateTime( const std::wstring & dateTime ) const = 0;

		/** Get the connection status.
		@return
		    true if connected, false otherwise.
		*/
		virtual bool IsConnected() const;

		/** Get the transaction status.
		@return
		    true if a transaction is started, false otherwise.
		*/
		virtual bool IsInTransaction() const;

	protected:

		/** Disconnect from the database.
		*/
		virtual void DoDisconnect() = 0;

		/** Connect to the database.
		@param[out] connectionString
		    Connection string in case of error.
		@return
		    Error code, EErrorType_NONE if no problem.
		*/
		virtual EErrorType DoConnect( String & connectionString ) = 0;

		/** Create a statement from a request.
		@param[in]  query
		    Request text.
		@param[out] result
		    Error code if the returned value is NULL.
		@return
		    The created statement.
		*/
		virtual DatabaseStatementPtr DoCreateStatement( const String & query, EErrorType * result ) = 0;

		/** Create a query from a request.
		@param[in]  query
		    Request text.
		@param[out] result
		    Error code if the returned value is NULL.
		@return
		    The created query.
		*/
		virtual DatabaseQueryPtr DoCreateQuery( const String & query, EErrorType * result ) = 0;

		/** Execute directly a request without result set.
		@param[in]  query
		    Request text.
		@param[out] result
		    Error code if the returned value is NULL.
		@return
		    The result.
		*/
		virtual bool DoExecuteUpdate( const String & query, EErrorType * result ) = 0;

		/** Execute directly a request with a result set.
		@param[in]  query
		    Request text.
		@param[out] result
		    Error code if the returned value is NULL.
		@return
		    The result.
		*/
		virtual DatabaseResultPtr DoExecuteSelect( const String & query, EErrorType * result ) = 0;

		/** Update the connection status.
		@param value
		    New status.
		*/
		void DoSetConnected( bool value );

		/** Update the transaction status.
		@param value
		    New status.
		*/
		void DoSetInTransaction( bool value );

	protected:
		String    _server;        ///< Server identifier (name or address).
		String    _database;      ///< Database identifier (name or DSN (ODBC)).
		String    _userName;      ///< User name.
		String    _password;      ///< User password.

	private:
		bool    _connected;         ///< Connection status
		bool    _inTransaction;     ///< Transaction status.

	};
}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_CONNECTION_H___
