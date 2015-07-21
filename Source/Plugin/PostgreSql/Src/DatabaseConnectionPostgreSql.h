/************************************************************************//**
* @file DatabaseConnectionPostgreSql.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/14/2014 5:03:05 PM
*
*
* @brief CDatabaseConnectionPostgreSql class declaration.
*
* @details Describes a connection to a database via an MYSQL driver.
*
***************************************************************************/

#ifndef ___DATABASE_CONNECTION_POSTGRESQL_H___
#define ___DATABASE_CONNECTION_POSTGRESQL_H___

#include "DatabasePostgreSqlPrerequisites.h"

#include <DatabaseConnection.h>

#include <libpq-fe.h>

BEGIN_NAMESPACE_DATABASE_POSTGRESQL
{
	/** Describes a connection to a database via an MYSQL driver.
	*/
	class CDatabaseConnectionPostgreSql
		: public CDatabaseConnection
	{
	public:
		/** Constructor.
		@param[in] server
			Address or name of the server.
		@param[in] database
			Database name or DSN.
		@param[in] userName
			User name.
		@param[in] password
			User password.
		@param[out] connectionString
			Created connection string.
		*/
		DatabasePostgreSqlExport CDatabaseConnectionPostgreSql( const String & server, const String & userName, const String & password, String & connectionString );

		/** Destructor.
		*/
		DatabasePostgreSqlExport virtual ~CDatabaseConnectionPostgreSql();

		/** Retrieves the precision for given field type.
		@return
			The precision.
		*/
		DatabasePostgreSqlExport virtual uint32_t GetPrecision( EFieldType type ) const;

		/** Creates a database.
		@param[in] database
			Database identifier (name or DSN (ODBC)).
		*/
		DatabasePostgreSqlExport virtual void CreateDatabase( const String & database );

		/** Selects a database.
		@param[in] database
			Database identifier (name or DSN (ODBC)).
		*/
		DatabasePostgreSqlExport virtual void SelectDatabase( const String & database );

		/** Destroys a database.
		@param[in] database
			Database identifier (name or DSN (ODBC)).
		*/
		DatabasePostgreSqlExport virtual void DestroyDatabase( const String & database );

		/** Format a string to be supported by the DBMS.
		@param[in] text
			Text to format.
		@return
			The formatted text.
		*/
		DatabasePostgreSqlExport virtual std::string WriteText( const std::string & text ) const;

		/** Format a string to be supported by the DBMS.
		@param[in] text
			Text to format.
		@return
			The formatted text.
		*/
		DatabasePostgreSqlExport virtual std::wstring WriteNText( const std::wstring & text ) const;

		/** Format a byte array to insert into a request.
		@param[in] array
			Value to format.
		@return
			Formatted value.
		*/
		DatabasePostgreSqlExport virtual String WriteBinary( const ByteArray & array ) const;

		/** Format a string to be supported by the DBMS.
		@param[in] text
			Text to format.
		@return
			The formatted text.
		*/
		DatabasePostgreSqlExport virtual String WriteName( const String & text ) const;

		/** Format a date into a string to be supported by the DBMS.
		@param[in] date
			Date to format.
		@return
			The formatted date.
		*/
		DatabasePostgreSqlExport virtual std::string WriteDateS( const CDate & date ) const;

		/** Format a date into a string for a statement to be supported by the DBMS.
		@param[in] date
			Date to format.
		@return
			The formatted date.
		*/
		DatabasePostgreSqlExport virtual std::string WriteStmtDateS( const CDate & date ) const;

		/** Format a time into a string to be supported by the DBMS.
		@param[in] time
			Time to format.
		@return
			The formatted time.
		*/
		DatabasePostgreSqlExport virtual std::string WriteTimeS( const CTime & time ) const;

		/** Format a time into a string for a statement to be supported by the DBMS.
		@param[in] time
			Time to format.
		@return
			The formatted time.
		*/
		DatabasePostgreSqlExport virtual std::string WriteStmtTimeS( const CTime & time ) const;

		/** Format a date/time into a string to be supported by the DBMS.
		@param[in] dateTime
			Date/time to format.
		@return
			The formatted date/time.
		*/
		DatabasePostgreSqlExport virtual std::string WriteDateTimeS( const CDateTime & dateTime ) const;

		/** Format a date/time to insert into a request.
		@param[in] date
			Value to format.
		@return
			Formatted value.
		*/
		DatabasePostgreSqlExport virtual std::string WriteDateTimeS( const CDate & date ) const;

		/** Format a date/time to insert into a request.
		@param[in] time
			Value to format.
		@return
			Formatted value.
		*/
		DatabasePostgreSqlExport virtual std::string WriteDateTimeS( const CTime & time ) const;

		/** Format a date/time into a string for a statement to be supported by the DBMS.
		@param[in] dateTime
			Date/time to format.
		@return
			The formatted date/time.
		*/
		DatabasePostgreSqlExport virtual std::string WriteStmtDateTimeS( const CDateTime & dateTime ) const;

		/** Format a date into a string to be supported by the DBMS.
		@param[in] date
			Date to format.
		@return
			The formatted date.
		*/
		DatabasePostgreSqlExport virtual std::wstring WriteDateW( const CDate & date ) const;

		/** Format a date into a string for a statement to be supported by the DBMS.
		@param[in] date
			Date to format.
		@return
			The formatted date.
		*/
		DatabasePostgreSqlExport virtual std::wstring WriteStmtDateW( const CDate & date ) const;

		/** Format a time into a string to be supported by the DBMS.
		@param[in] time
			Time to format.
		@return
			The formatted time.
		*/
		DatabasePostgreSqlExport virtual std::wstring WriteTimeW( const CTime & time ) const;

		/** Format a time into a string for a statement to be supported by the DBMS.
		@param[in] time
			Time to format.
		@return
			The formatted time.
		*/
		DatabasePostgreSqlExport virtual std::wstring WriteStmtTimeW( const CTime & time ) const;

		/** Format a date/time into a string to be supported by the DBMS.
		@param[in] dateTime
			Date/time to format.
		@return
			The formatted date/time.
		*/
		DatabasePostgreSqlExport virtual std::wstring WriteDateTimeW( const CDateTime & dateTime ) const;

		/** Format a date/time to insert into a request.
		@param[in] date
			Value to format.
		@return
			Formatted value.
		*/
		DatabasePostgreSqlExport virtual std::wstring WriteDateTimeW( const CDate & date ) const;

		/** Format a date/time to insert into a request.
		@param[in] time
			Value to format.
		@return
			Formatted value.
		*/
		DatabasePostgreSqlExport virtual std::wstring WriteDateTimeW( const CTime & time ) const;

		/** Format a date/time into a string for a statement to be supported by the DBMS.
		@param[in] dateTime
			Date/time to format.
		@return
			The formatted date/time.
		*/
		DatabasePostgreSqlExport virtual std::wstring WriteStmtDateTimeW( const CDateTime & dateTime ) const;

		/** Format a boolean into a string to be supported by the DBMS.
		@param[in] value
			Boolean to format.
		@return
			The formatted boolean.
		*/
		DatabasePostgreSqlExport virtual String WriteBool( bool value ) const;

		/** Format a string representing a boolean into a string to be supported by the DBMS.
		@param[in] value
			Boolean to format.
		@return
			The formatted boolean.
		*/
		DatabasePostgreSqlExport virtual String WriteBool( const String & value ) const;

		/** Convert a string from the database to a date.
		@param[in] date
			String representing a date.
		@return
			Created date with the string.
		*/
		DatabasePostgreSqlExport virtual CDate ParseDate( const std::string & date ) const;

		/** Convert a string from the database to a time.
		@param[in] time
			String representing a time.
		@return
			Created time with the string.
		*/
		DatabasePostgreSqlExport virtual CTime ParseTime( const std::string & time ) const;

		/** Convert a string from the database to a date/time.
		@param[in] dateTime
			String representing a date/time.
		@return
			Created date/time with the string.
		*/
		DatabasePostgreSqlExport virtual CDateTime ParseDateTime( const std::string & dateTime ) const;

		/** Convert a string from the database to a date.
		@param[in] date
			String representing a date.
		@return
			Created date with the string.
		*/
		DatabasePostgreSqlExport virtual CDate ParseDate( const std::wstring & date ) const;

		/** Convert a string from the database to a time.
		@param[in] time
			String representing a time.
		@return
			Created time with the string.
		*/
		DatabasePostgreSqlExport virtual CTime ParseTime( const std::wstring & time ) const;

		/** Convert a string from the database to a date/time.
		@param[in] dateTime
			String representing a date/time.
		@return
			Created date/time with the string.
		*/
		DatabasePostgreSqlExport virtual CDateTime ParseDateTime( const std::wstring & dateTime ) const;

		/** Retrieves the statement date type size
		@return
			The size
		*/
		DatabasePostgreSqlExport virtual unsigned long GetStmtDateSize()const;

		/** Retrieves the statement date/time type size
		@return
			The size
		*/
		DatabasePostgreSqlExport virtual unsigned long GetStmtDateTimeSize()const;

		/** Retrieves the statement time type size
		@return
			The size
		*/
		DatabasePostgreSqlExport virtual unsigned long GetStmtTimeSize()const;

		/** Get the connection handle.
		@return
			The connection handle.
		*/
		DatabasePostgreSqlExport PGconn * GetConnection() const;

	protected:
		/** Connect to a database.
		@param[in] database
			The database name, can be empty
		@param[out] connectionString
			Connection string in case of error.
		@return
			Error code, true if no problem.
		*/
		DatabasePostgreSqlExport virtual bool DoConnect( String const & database, String & connectionString );

		/** Connect to the database.
		@param[out] connectionString
			Connection string in case of error.
		@return
			Error code, EErrorType_NONE if no problem.
		*/
		DatabasePostgreSqlExport virtual EErrorType DoConnect( String & connectionString );

		/** Disconnect from the database.
		*/
		DatabasePostgreSqlExport virtual void DoDisconnect();

		/** Initialize a named transaction.
		@param[in] name
			Transaction name.
		@return
			true if no problem
		*/
		DatabasePostgreSqlExport virtual bool DoBeginTransaction( const String & name );

		/** Validate a named transaction.
		@param[in] name
			Transaction name.
		@return
			true if no problem
		*/
		DatabasePostgreSqlExport virtual bool DoCommit( const String & name );

		/** Invalidate a named transaction.
		@param[in] name
			Transaction name.
		@return
			true if no problem
		*/
		DatabasePostgreSqlExport virtual bool DoRollBack( const String & name );

		/** Execute directly a request.
		@param[in]  query
			Request text.
		@param[out] result
			Error code if the returned value is NULL.
		@return
			The result.
		*/
		DatabasePostgreSqlExport virtual bool DoExecuteUpdate( const String & query );

		/** Execute directly a request.
		@param[in]  query
			Request text.
		@param[out] result
			Error code if the returned value is NULL.
		@return
			The result.
		*/
		DatabasePostgreSqlExport virtual DatabaseResultPtr DoExecuteSelect( const String & query );

		/** Create a statement from a request.
		@param[in]  query
			Request text.
		@param[out] result
			Error code if the returned value is NULL.
		@return
			The created statement.
		*/
		DatabasePostgreSqlExport virtual DatabaseStatementPtr DoCreateStatement( const String & query );

	protected:
		//! The connection
		PGconn * _connection;
	};
}
END_NAMESPACE_DATABASE_POSTGRESQL

#endif // ___DATABASE_CONNECTION_POSTGRESQL_H___
