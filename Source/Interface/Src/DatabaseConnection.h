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

#include <unordered_set>

BEGIN_NAMESPACE_DATABASE
{
	/** Describes a database connection.
	*/
	class CDatabaseConnection
		: public std::enable_shared_from_this< CDatabaseConnection >
	{
	public:
		/**@name Construction */
		//@{

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

		//@}
		/**@name Connection */
		//@{

		/** Initialize the connection to the database.
		@param[out] connectionString
			Connection string in case of error.
		@return
			Error code.
		*/
		DatabaseExport EErrorType Reconnect( String & connectionString );

		/** Get the connection status.
		@return
			true if connected, false otherwise.
		*/
		DatabaseExport virtual bool IsConnected() const;

		//@}
		/**@name Transactions */
		//@{

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

		/** Get the transaction status.
		@return
			true if any transaction is started, false otherwise.
		*/
		DatabaseExport bool IsInTransaction() const;

		/** Get a given transaction status.
		@param[in] name
			The transaction name
		@return
			true if the given transaction is started, false otherwise.
		*/
		DatabaseExport bool IsInTransaction( const String & name ) const;

		//@}
		/**@name Query/Statement */
		//@{

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

		/** Creates a database.
		@param[in] database
			Database identifier (name or DSN (ODBC)).
		*/
		DatabaseExport void CreateDatabase( const String & database );

		/** Selects a database.
		@param[in] database
			Database identifier (name or DSN (ODBC)).
		*/
		DatabaseExport void SelectDatabase( const String & database );

		/** Destroys a database.
		@param[in] database
			Database identifier (name or DSN (ODBC)).
		*/
		DatabaseExport void DestroyDatabase( const String & database );

		//@}
		/**@name Textual data writing */
		//@{

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

		/** Format a string to insert into a request.
		@param[in] text
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport std::string WriteText( const std::string & text ) const;

		/** Format a string to insert into a request.
		@param[in] text
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport std::wstring WriteNText( const std::wstring & text ) const;

		/** Format a byte array to insert into a request.
		@param[in] array
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport String WriteBinary( const ByteArray & array ) const;

		/** Format a string to insert into a request.
		@param[in] name
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport String WriteName( const String & name ) const;

		/** Format a boolean to insert into a request or statement.
		@param[in] value
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport String WriteBool( bool value ) const;

		//@}
		/**@name Textual data retrieval */
		//@{

		/** Convert a string from the database to a date.
		@param[in] date
			String representing a date.
		@return
			Created date with the string.
		*/
		DatabaseExport DateType ParseDate( const String & date ) const;

		/** Convert a string from the database to a time.
		@param[in] time
			String representing a time.
		@return
			Created time with the string.
		*/
		DatabaseExport TimeType ParseTime( const String & time ) const;

		/** Convert a string from the database to a date/time.
		@param[in] dateTime
			String representing a date/time.
		@return
			Created date/time with the string.
		*/
		DatabaseExport DateTimeType ParseDateTime( const String & dateTime ) const;

		//@}
		/**@name Size/Precision informations */
		//@{

		/** Retrieves the precision for given field type.
		@return
			The precision.
		*/
		DatabaseExport virtual uint32_t GetPrecision( EFieldType type ) const = 0;

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

		//@}

	protected:
		/** Checks if the connection is established, throws an exception if not
		*/
		DatabaseExport void DoCheckConnected() const;

		/** Update the connection status.
		@param value
			New status.
		*/
		DatabaseExport void DoSetConnected( bool value );

		/** Sets the given transaction to be started.
		@param name
			The transaction name.
		*/
		DatabaseExport void DoStartTransaction( const String & name );

		/** Sets the given transaction to be finished.
		@param name
			The transaction name.
		*/
		DatabaseExport void DoFinishTransaction( const String & name );

		/** Creates a database.
		@param[in] database
			Database identifier (name or DSN (ODBC)).
		*/
		DatabaseExport virtual void DoCreateDatabase( const String & database ) = 0;

		/** Selects a database.
		@param[in] database
			Database identifier (name or DSN (ODBC)).
		*/
		DatabaseExport virtual void DoSelectDatabase( const String & database ) = 0;

		/** Destroys a database.
		@param[in] database
			Database identifier (name or DSN (ODBC)).
		*/
		DatabaseExport virtual void DoDestroyDatabase( const String & database ) = 0;

		/** Format a string to insert into a request.
		@param[in] text
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport virtual std::string DoWriteText( const std::string & text ) const = 0;

		/** Format a string to insert into a request.
		@param[in] text
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport virtual std::wstring DoWriteNText( const std::wstring & text ) const = 0;

		/** Format a byte array to insert into a request.
		@param[in] array
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport virtual String DoWriteBinary( const ByteArray & array ) const = 0;

		/** Format a string to insert into a request.
		@param[in] name
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport virtual String DoWriteName( const String & name ) const = 0;

		/** Format a date to insert into a request.
		@param[in] date
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport virtual String DoWriteDate( const DateType & date ) const = 0;

		/** Format a date to insert into a statement.
		@param[in] date
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport virtual String DoWriteStmtDate( const DateType & date ) const = 0;

		/** Format a time to insert into a request.
		@param[in] time
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport virtual String DoWriteTime( const TimeType & time ) const = 0;

		/** Format a time to insert into a statement.
		@param[in] time
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport virtual String DoWriteStmtTime( const TimeType & time ) const = 0;

		/** Format a date/time to insert into a request.
		@param[in] dateTime
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport virtual String DoWriteDateTime( const DateTimeType & dateTime ) const = 0;

		/** Format a date/time to insert into a request.
		@param[in] date
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport virtual String DoWriteDateTime( const DateType & date ) const = 0;

		/** Format a date/time to insert into a request.
		@param[in] time
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport virtual String DoWriteDateTime( const TimeType & time ) const = 0;

		/** Format a date/time to insert into a statement.
		@param[in] dateTime
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport virtual String DoWriteStmtDateTime( const DateTimeType & dateTime ) const = 0;

		/** Format a boolean to insert into a request or statement.
		@param[in] value
			Value to format.
		@return
			Formatted value.
		*/
		DatabaseExport virtual String DoWriteBool( bool value ) const = 0;

		/** Convert a string from the database to a date.
		@param[in] date
			String representing a date.
		@return
			Created date with the string.
		*/
		DatabaseExport virtual DateType DoParseDate( const String & date ) const = 0;

		/** Convert a string from the database to a time.
		@param[in] time
			String representing a time.
		@return
			Created time with the string.
		*/
		DatabaseExport virtual TimeType DoParseTime( const String & time ) const = 0;

		/** Convert a string from the database to a date/time.
		@param[in] dateTime
			String representing a date/time.
		@return
			Created date/time with the string.
		*/
		DatabaseExport virtual DateTimeType DoParseDateTime( const String & dateTime ) const = 0;

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
		std::unordered_set< String > _startedTransactions;
	};
}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_CONNECTION_H___
