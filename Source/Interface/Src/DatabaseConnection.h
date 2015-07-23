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
	class CDatabaseConnection
		: public std::enable_shared_from_this< CDatabaseConnection >
	{
	public:
		/** Constructor.
		@param[in] server
			Server identifier (name or address).
		@param[in] userName
			User name.
		@param[in] password
			User password.
		*/
		DatabaseExport CDatabaseConnection( const String & server, const String & userName, const String & password );

		/** Destructor.
			*/
		DatabaseExport virtual ~CDatabaseConnection();

		/** Initialize the connection to the database.
		@param[out] connectionString
			Connection string in case of error.
		@return
			Error code.
		*/
		DatabaseExport EErrorType Reconnect( String & connectionString );

		/** Initialize a named transaction.
		@param[in] name
			Transaction name.
		*/
		DatabaseExport void BeginTransaction( const String & name );

		/** Validate a named transaction.
		@param[in] name
			Transaction name.
		*/
		DatabaseExport void Commit( const String & name );

		/** Invalidate a named transaction.
		@param[in] name
			Transaction name.
		*/
		DatabaseExport void RollBack( const String & name );

		/** Create a statement based on a request.
		@param[in] query
			Request text.
		@param[out] result
			Error code.
		@return
			Created statement.
		*/
		DatabaseExport DatabaseStatementSPtr CreateStatement( const String & query );

		/** Create a query based on a request.
		@param[in] query
			Request text.
		@param[out] result
			Error code.
		@return
			Created query.
		*/
		DatabaseExport DatabaseQuerySPtr CreateQuery( const String & query );

		/** Execute directly a request without result set.
		@param[in] query
			Request text.
		@param[out] result
			Error code.
		@return
			Results.
		*/
		DatabaseExport bool ExecuteUpdate( const String & query );

		/** Execute directly a request with a result set.
		@param[in] query
			Request text.
		@param[out] result
			Error code.
		@return
			Results.
		*/
		DatabaseExport DatabaseResultSPtr ExecuteSelect( const String & query );

		/** Get the connection status.
		@return
			true if connected, false otherwise.
		*/
		DatabaseExport virtual bool IsConnected() const;

		/** Get the transaction status.
		@return
			true if a transaction is started, false otherwise.
		*/
		DatabaseExport virtual bool IsInTransaction() const;

		/** Format a date to insert into a request.
		@param[in] date
			Value to format.
		@param[in] format
			String describing the format.
		@return
			Formatted value.
		*/
		DatabaseExport std::string WriteDate( const std::string & date, const std::string & format ) const;

		/** Format a time to insert into a request.
		@param[in] time
			Value to format.
		@param[in] format
			String describing the format.
		@return
			Formatted value.
		*/
		DatabaseExport std::string WriteTime( const std::string & time, const std::string & format ) const;

		/** Format a date/time to insert into a request.
		@param[in] dateTime
			Value to format.
		@param[in] format
			String describing the format.
		@return
			Formatted value.
		*/
		DatabaseExport std::string WriteDateTime( const std::string & dateTime, const std::string & format ) const;

		/** Format a date to insert into a statement.
		@param[in] date
			Value to format.
		@param[in] format
			String describing the format.
		@return
			Formatted value.
		*/
		DatabaseExport std::string WriteStmtDate( const std::string & date, const std::string & format ) const;

		/** Format a time to insert into a statement.
		@param[in] time
			Value to format.
		@param[in] format
			String describing the format.
		@return
			Formatted value.
		*/
		DatabaseExport std::string WriteStmtTime( const std::string & time, const std::string & format ) const;

		/** Format a date/time to insert into a statement.
		@param[in] dateTime
			Value to format.
		@param[in] format
			String describing the format.
		@return
			Formatted value.
		*/
		DatabaseExport std::string WriteStmtDateTime( const std::string & dateTime, const std::string & format ) const;

		/** Format a date to insert into a request.
		@param[in] date
			Value to format.
		@param[in] format
			String describing the format.
		@return
			Formatted value.
		*/
		DatabaseExport std::wstring WriteDate( const std::wstring & date, const std::wstring & format ) const;

		/** Format a time to insert into a request.
		@param[in] time
			Value to format.
		@param[in] format
			String describing the format.
		@return
			Formatted value.
		*/
		DatabaseExport std::wstring WriteTime( const std::wstring & time, const std::wstring & format ) const;

		/** Format a date/time to insert into a request.
		@param[in] dateTime
			Value to format.
		@param[in] format
			String describing the format.
		@return
			Formatted value.
		*/
		DatabaseExport std::wstring WriteDateTime( const std::wstring & dateTime, const std::wstring & format ) const;

		/** Format a date to insert into a statement.
		@param[in] date
			Value to format.
		@param[in] format
			String describing the format.
		@return
			Formatted value.
		*/
		DatabaseExport std::wstring WriteStmtDate( const std::wstring & date, const std::wstring & format ) const;

		/** Format a time to insert into a statement.
		@param[in] time
			Value to format.
		@param[in] format
			String describing the format.
		@return
			Formatted value.
		*/
		DatabaseExport std::wstring WriteStmtTime( const std::wstring & time, const std::wstring & format ) const;

		/** Format a date/time to insert into a statement.
		@param[in] dateTime
			Value to format.
		@param[in] format
			String describing the format.
		@return
			Formatted value.
		*/
		DatabaseExport std::wstring WriteStmtDateTime( const std::wstring & dateTime, const std::wstring & format ) const;

		/** Format a date to insert into a request.
		@param[in] date
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport String WriteDate( const DateType & date ) const;

		/** Format a date to insert into a statement.
		@param[in] date
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport String WriteStmtDate( const DateType & date ) const;

		/** Format a time to insert into a request.
		@param[in] time
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport String WriteTime( const TimeType & time ) const;

		/** Format a time to insert into a statement.
		@param[in] time
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport String WriteStmtTime( const TimeType & time ) const;

		/** Format a date/time to insert into a request.
		@param[in] dateTime
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport String WriteDateTime( const DateTimeType & dateTime ) const;

		/** Format a date/time to insert into a request.
		@param[in] date
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport String WriteDateTime( const DateType & date ) const;

		/** Format a date/time to insert into a request.
		@param[in] time
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport String WriteDateTime( const TimeType & time ) const;

		/** Format a date/time to insert into a statement.
		@param[in] dateTime
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport String WriteStmtDateTime( const DateTimeType & dateTime ) const;

		/** Retrieves the precision for given field type.
		@return
			The precision.
		*/
		DatabaseExport virtual uint32_t GetPrecision( EFieldType type ) const = 0;

		/** Creates a database.
		@param[in] database
			Database identifier (name or DSN (ODBC)).
		*/
		DatabaseExport virtual void CreateDatabase( const String & database ) = 0;

		/** Selects a database.
		@param[in] database
			Database identifier (name or DSN (ODBC)).
		*/
		DatabaseExport virtual void SelectDatabase( const String & database ) = 0;

		/** Destroys a database.
		@param[in] database
			Database identifier (name or DSN (ODBC)).
		*/
		DatabaseExport virtual void DestroyDatabase( const String & database ) = 0;

		/** Format a string to insert into a request.
		@param[in] text
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport virtual std::string WriteText( const std::string & text ) const = 0;

		/** Format a string to insert into a request.
		@param[in] text
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport virtual std::wstring WriteNText( const std::wstring & text ) const = 0;

		/** Format a byte array to insert into a request.
		@param[in] array
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport virtual String WriteBinary( const ByteArray & array ) const = 0;

		/** Format a string to insert into a request.
		@param[in] name
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport virtual String WriteName( const String & name ) const = 0;

		/** Format a date to insert into a request.
		@param[in] date
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport virtual std::string WriteDateS( const DateType & date ) const = 0;

		/** Format a date to insert into a statement.
		@param[in] date
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport virtual std::string WriteStmtDateS( const DateType & date ) const = 0;

		/** Format a time to insert into a request.
		@param[in] time
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport virtual std::string WriteTimeS( const TimeType & time ) const = 0;

		/** Format a time to insert into a statement.
		@param[in] time
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport virtual std::string WriteStmtTimeS( const TimeType & time ) const = 0;

		/** Format a date/time to insert into a request.
		@param[in] dateTime
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport virtual std::string WriteDateTimeS( const DateTimeType & dateTime ) const = 0;

		/** Format a date/time to insert into a request.
		@param[in] date
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport virtual std::string WriteDateTimeS( const DateType & date ) const = 0;

		/** Format a date/time to insert into a request.
		@param[in] time
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport virtual std::string WriteDateTimeS( const TimeType & time ) const = 0;

		/** Format a date/time to insert into a statement.
		@param[in] dateTime
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport virtual std::string WriteStmtDateTimeS( const DateTimeType & dateTime ) const = 0;

		/** Format a date to insert into a request.
		@param[in] date
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport virtual std::wstring WriteDateW( const DateType & date ) const = 0;

		/** Format a date to insert into a statement.
		@param[in] date
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport virtual std::wstring WriteStmtDateW( const DateType & date ) const = 0;

		/** Format a time to insert into a request.
		@param[in] time
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport virtual std::wstring WriteTimeW( const TimeType & time ) const = 0;

		/** Format a time to insert into a statement.
		@param[in] time
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport virtual std::wstring WriteStmtTimeW( const TimeType & time ) const = 0;

		/** Format a date/time to insert into a request.
		@param[in] dateTime
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport virtual std::wstring WriteDateTimeW( const DateTimeType & dateTime ) const = 0;

		/** Format a date/time to insert into a request.
		@param[in] date
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport virtual std::wstring WriteDateTimeW( const DateType & date ) const = 0;

		/** Format a date/time to insert into a request.
		@param[in] time
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport virtual std::wstring WriteDateTimeW( const TimeType & time ) const = 0;

		/** Format a date/time to insert into a statement.
		@param[in] dateTime
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport virtual std::wstring WriteStmtDateTimeW( const DateTimeType & dateTime ) const = 0;

		/** Format a boolean to insert into a request or statement.
		@param[in] value
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport virtual String WriteBool( bool value ) const = 0;

		/** Format a boolean to insert into a request or statement.
		@param[in] value
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport virtual String WriteBool( const String & value ) const = 0;

		/** Convert a string from the database to a date.
		@param[in] date
			String representing a date.
		@return
			Created date with the string.
		*/
		DatabaseExport virtual DateType ParseDate( const std::string & date ) const = 0;

		/** Convert a string from the database to a time.
		@param[in] time
			String representing a time.
		@return
			Created time with the string.
		*/
		DatabaseExport virtual TimeType ParseTime( const std::string & time ) const = 0;

		/** Convert a string from the database to a date/time.
		@param[in] dateTime
			String representing a date/time.
		@return
			Created date/time with the string.
		*/
		DatabaseExport virtual DateTimeType ParseDateTime( const std::string & dateTime ) const = 0;

		/** Convert a string from the database to a date.
		@param[in] date
			String representing a date.
		@return
			Created date with the string.
		*/
		DatabaseExport virtual DateType ParseDate( const std::wstring & date ) const = 0;

		/** Convert a string from the database to a time.
		@param[in] time
			String representing a time.
		@return
			Created time with the string.
		*/
		DatabaseExport virtual TimeType ParseTime( const std::wstring & time ) const = 0;

		/** Convert a string from the database to a date/time.
		@param[in] dateTime
			String representing a date/time.
		@return
			Created date/time with the string.
		*/
		DatabaseExport virtual DateTimeType ParseDateTime( const std::wstring & dateTime ) const = 0;

		/** Retrieves the statement date type size
		@return
			The size
		*/
		DatabaseExport virtual unsigned long GetStmtDateSize()const = 0;

		/** Retrieves the statement date/time type size
		@return
			The size
		*/
		DatabaseExport virtual unsigned long GetStmtDateTimeSize()const = 0;

		/** Retrieves the statement time type size
		@return
			The size
		*/
		DatabaseExport virtual unsigned long GetStmtTimeSize()const = 0;

	protected:
		/** Disconnect from the database.
		*/
		DatabaseExport virtual void DoDisconnect() = 0;

		/** Connect to the database.
		@param[out] connectionString
			Connection string in case of error.
		@return
			Error code, EErrorType_NONE if no problem.
		*/
		DatabaseExport virtual EErrorType DoConnect( String & connectionString ) = 0;

		/** Initialize a named transaction.
		@param[in] name
			Transaction name.
		@return
			Error code, EErrorType_NONE if no problem.
		*/
		DatabaseExport virtual bool DoBeginTransaction( const String & name ) = 0;

		/** Validate a named transaction.
		@param[in] name
			Transaction name.
		@return
			Error code, EErrorType_NONE if no problem.
		*/
		DatabaseExport virtual bool DoCommit( const String & name ) = 0;

		/** Invalidate a named transaction.
		@param[in] name
			Transaction name.
		@return
			Error code, EErrorType_NONE if no problem.
		*/
		DatabaseExport virtual bool DoRollBack( const String & name ) = 0;

		/** Create a statement from a request.
		@param[in]  query
			Request text.
		@param[out] result
			Error code if the returned value is NULL.
		@return
			The created statement.
		*/
		DatabaseExport virtual DatabaseStatementSPtr DoCreateStatement( const String & query ) = 0;

		/** Execute directly a request without result set.
		@param[in]  query
			Request text.
		@param[out] result
			Error code if the returned value is NULL.
		@return
			The result.
		*/
		DatabaseExport virtual bool DoExecuteUpdate( const String & query ) = 0;

		/** Execute directly a request with a result set.
		@param[in]  query
			Request text.
		@param[out] result
			Error code if the returned value is NULL.
		@return
			The result.
		*/
		DatabaseExport virtual DatabaseResultSPtr DoExecuteSelect( const String & query ) = 0;

		/** Update the connection status.
		@param value
			New status.
		*/
		DatabaseExport void DoSetConnected( bool value );

		/** Update the transaction status.
		@param value
			New status.
		*/
		DatabaseExport void DoSetInTransaction( bool value );

	protected:
		//! Server identifier (name or address).
		String _server;
		//! Database identifier (name or DSN (ODBC)).
		String _database;
		//! User name.
		String _userName;
		//! User password.
		String _password;

	private:
		//! Connection status
		bool _connected;
		//! Transaction status.
		bool _inTransaction;
	};
}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_CONNECTION_H___
