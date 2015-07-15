/************************************************************************//**
* @file DatabaseConnectionOdbc.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/14/2014 5:03:05 PM
*
*
* @brief CDatabaseConnectionOdbc class declaration.
*
* @details Describes a connection to a database via an ODBC driver.
*
***************************************************************************/

#ifndef ___DATABASE_CONNECTION_ODBC_H___
#define ___DATABASE_CONNECTION_ODBC_H___

#include "DatabaseOdbcPrerequisites.h"

#include <DatabaseConnection.h>

BEGIN_NAMESPACE_DATABASE_ODBC
{
	/** Describes a connection to a database via an ODBC driver.
	*/
	class CDatabaseConnectionOdbc
		: public CDatabaseConnection
	{
	public:
		/** Constructor.
		@param[in] sqlEnvironmentHandle
			The handle to the SQL environment.
		@param[in] server
			Database name or DSN.
		@param[in] userName
			User name.
		@param[in] password
			User password.
		@param[out] connectionString
			Created connection string.
			*/
		CDatabaseConnectionOdbc( SQLHENV sqlEnvironmentHandle, const String & server, const String & userName, const String & password, String & connectionString );

		/** Destructor.
			*/
		virtual ~CDatabaseConnectionOdbc();

		/** Initialize a named transaction.
		@param[in] name
			Transaction name.
		@return
			Error code, EErrorType_NONE if no problem.
		*/
		virtual EErrorType BeginTransaction( const String & name );

		/** Validate a named transaction.
		@param[in] name
			Transaction name.
		@return
			Error code, EErrorType_NONE if no problem.
		*/
		virtual EErrorType Commit( const String & name );

		/** Invalidate a named transaction.
		@param[in] name
			Transaction name.
		@return
			Error code, EErrorType_NONE if no problem.
		*/
		virtual EErrorType RollBack( const String & name );

		/** Format a string to be supported by the DBMS.
		@param[in] text
			Text to format.
		@return
			The formatted text.
		*/
		virtual std::string WriteText( const std::string & text ) const;

		/** Format a string to be supported by the DBMS.
		@param[in] text
			Text to format.
		@return
			The formatted text.
		*/
		virtual std::wstring WriteNText( const std::wstring & text ) const;

		/** Format a string to be supported by the DBMS.
		@param[in] text
			Text to format.
		@return
			The formatted text.
		*/
		virtual String WriteName( const String & text ) const;

		/** Format a date into a string to be supported by the DBMS.
		@param[in] date
			Date to format.
		@return
			The formatted date.
		*/
		virtual std::string WriteDateS( const CDate & date ) const;

		/** Format a date into a string for a statement to be supported by the DBMS.
		@param[in] date
			Date to format.
		@return
			The formatted date.
		*/
		virtual std::string WriteStmtDateS( const CDate & date ) const;

		/** Format a time into a string to be supported by the DBMS.
		@param[in] time
			Time to format.
		@return
			The formatted time.
		*/
		virtual std::string WriteTimeS( const CTime & time ) const;

		/** Format a time into a string for a statement to be supported by the DBMS.
		@param[in] time
			Time to format.
		@return
			The formatted time.
		*/
		virtual std::string WriteStmtTimeS( const CTime & time ) const;

		/** Format a date/time into a string to be supported by the DBMS.
		@param[in] dateTime
			Date/time to format.
		@return
			The formatted date/time.
		*/
		virtual std::string WriteDateTimeS( const CDateTime & dateTime ) const;

		/** Format a date/time to insert into a request.
		@param[in] date
			Value to format.
		@return
			Formatted value.
		*/
		virtual std::string WriteDateTimeS( const CDate & date ) const;

		/** Format a date/time to insert into a request.
		@param[in] time
			Value to format.
		@return
			Formatted value.
		*/
		virtual std::string WriteDateTimeS( const CTime & time ) const;

		/** Format a date/time into a string for a statement to be supported by the DBMS.
		@param[in] dateTime
			Date/time to format.
		@return
			The formatted date/time.
		*/
		virtual std::string WriteStmtDateTimeS( const CDateTime & dateTime ) const;

		/** Format a date into a string to be supported by the DBMS.
		@param[in] date
			Date to format.
		@return
			The formatted date.
		*/
		virtual std::wstring WriteDateW( const CDate & date ) const;

		/** Format a date into a string for a statement to be supported by the DBMS.
		@param[in] date
			Date to format.
		@return
			The formatted date.
		*/
		virtual std::wstring WriteStmtDateW( const CDate & date ) const;

		/** Format a time into a string to be supported by the DBMS.
		@param[in] time
			Time to format.
		@return
			The formatted time.
		*/
		virtual std::wstring WriteTimeW( const CTime & time ) const;

		/** Format a time into a string for a statement to be supported by the DBMS.
		@param[in] time
			Time to format.
		@return
			The formatted time.
		*/
		virtual std::wstring WriteStmtTimeW( const CTime & time ) const;

		/** Format a date/time into a string to be supported by the DBMS.
		@param[in] dateTime
			Date/time to format.
		@return
			The formatted date/time.
		*/
		virtual std::wstring WriteDateTimeW( const CDateTime & dateTime ) const;

		/** Format a date/time to insert into a request.
		@param[in] date
			Value to format.
		@return
			Formatted value.
		*/
		virtual std::wstring WriteDateTimeW( const CDate & date ) const;

		/** Format a date/time to insert into a request.
		@param[in] time
			Value to format.
		@return
			Formatted value.
		*/
		virtual std::wstring WriteDateTimeW( const CTime & time ) const;

		/** Format a date/time into a string for a statement to be supported by the DBMS.
		@param[in] dateTime
			Date/time to format.
		@return
			The formatted date/time.
		*/
		virtual std::wstring WriteStmtDateTimeW( const CDateTime & dateTime ) const;

		/** Format a boolean into a string to be supported by the DBMS.
		@param[in] value
			Boolean to format.
		@return
			The formatted boolean.
		*/
		virtual String WriteBool( bool value ) const;

		/** Format a string representing a boolean into a string to be supported by the DBMS.
		@param[in] value
			Boolean to format.
		@return
			The formatted boolean.
		*/
		virtual String WriteBool( const String & value ) const;

		/** Convert a string from the database to a date.
		@param[in] date
			String representing a date.
		@return
			Created date with the string.
		*/
		virtual CDate ParseDate( const std::string & date ) const;

		/** Convert a string from the database to a time.
		@param[in] time
			String representing a time.
		@return
			Created time with the string.
		*/
		virtual CTime ParseTime( const std::string & time ) const;

		/** Convert a string from the database to a date/time.
		@param[in] dateTime
			String representing a date/time.
		@return
			Created date/time with the string.
		*/
		virtual CDateTime ParseDateTime( const std::string & dateTime ) const;

		/** Convert a string from the database to a date.
		@param[in] date
			String representing a date.
		@return
			Created date with the string.
		*/
		virtual CDate ParseDate( const std::wstring & date ) const;

		/** Convert a string from the database to a time.
		@param[in] time
			String representing a time.
		@return
			Created time with the string.
		*/
		virtual CTime ParseTime( const std::wstring & time ) const;

		/** Convert a string from the database to a date/time.
		@param[in] dateTime
			String representing a date/time.
		@return
			Created date/time with the string.
		*/
		virtual CDateTime ParseDateTime( const std::wstring & dateTime ) const;

		/** Retrieves the statement date type size
		@return
			The size
		*/
		virtual unsigned long GetStmtDateSize()const;

		/** Retrieves the statement date/time type size
		@return
			The size
		*/
		virtual unsigned long GetStmtDateTimeSize()const;

		/** Retrieves the statement time type size
		@return
			The size
		*/
		virtual unsigned long GetStmtTimeSize()const;

		/** Get the connection handle.
		@return
			The connection handle.
		*/
		HDBC GetHdbc() const;

	protected:
		/** Disconnect from the database.
		*/
		virtual void DoDisconnect();

		/** Execute directly a request.
		@param[in]  query
			Request text.
		@param[out] result
			Error code if the returned value is NULL.
		@param[in] transactional
			The request must execute inside a transaction or not.
		@return
			The result.
		*/
		virtual bool DoExecuteUpdate( const String & query, EErrorType * result );

		/** Execute directly a request.
		@param[in]  query
			Request text.
		@param[out] result
			Error code if the returned value is NULL.
		@param[in] transactional
			The request must execute inside a transaction or not.
		@return
			The result.
		*/
		virtual DatabaseResultPtr DoExecuteSelect( const String & query, EErrorType * result );

	protected:
		//! Connection identifier.
		HDBC _connectionHandle;
		//! Connection environment.
		HENV _environmentHandle;
	};
}
END_NAMESPACE_DATABASE_ODBC

#endif // ___DATABASE_CONNECTION_ODBC_H___
