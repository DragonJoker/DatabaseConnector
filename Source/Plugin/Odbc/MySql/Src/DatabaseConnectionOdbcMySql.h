/************************************************************************//**
* @file DatabaseConnectionOdbcMySql.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/14/2014 5:03:05 PM
*
*
* @brief CDatabaseConnectionOdbcMySql class declaration.
*
* @details Describes a connection to a database via an ODBC driver.
*
***************************************************************************/

#ifndef ___DATABASE_CONNECTION_ODBC_MYSQL_H___
#define ___DATABASE_CONNECTION_ODBC_MYSQL_H___

#include "DatabaseOdbcMySqlPrerequisites.h"

#include <DatabaseConnectionOdbc.h>

BEGIN_NAMESPACE_DATABASE_ODBC_MYSQL
{
	/** Describes a connection to a database via an ODBC driver.
	*/
	class CDatabaseConnectionOdbcMySql
		: public CDatabaseConnectionOdbc
	{
	public:
		/** Constructor.
		@param[in] sqlEnvironmentHandle
			The handle to the SQL environment.
		@param[in] server
			Address or name of the server.
		@param[in] userName
			User name.
		@param[in] password
			User password.
		@param[out] connectionString
			Created connection string.
			*/
		CDatabaseConnectionOdbcMySql( SQLHENV sqlEnvironmentHandle, const String & server, const String & userName, const String & password, String & connectionString );

		/** Destructor.
			*/
		virtual ~CDatabaseConnectionOdbcMySql();

		/** Retrieves the precision for given field type.
		@return
			The precision.
		*/
		virtual uint32_t GetPrecision( EFieldType type ) const;

		/** Creates a database.
		@param[in] database
			Database identifier (name or DSN (ODBC)).
		*/
		virtual void CreateDatabase( const String & database );

		/** Selects a database.
		@param[in] database
			Database identifier (name or DSN (ODBC)).
		*/
		virtual void SelectDatabase( const String & database );

		/** Destroys a database.
		@param[in] database
			Database identifier (name or DSN (ODBC)).
		*/
		virtual void DestroyDatabase( const String & database );

		/** Format a byte array to insert into a request.
		@param[in] array
			Value to format.
		@return
			Formatted value.
		*/
		virtual String WriteBinary( const ByteArray & array ) const;

		/** Format a date into a string to be supported by the DBMS.
		@param[in] date
			Date to format.
		@return
			The formatted date.
		*/
		virtual std::string WriteDateS( const DateType & date ) const;

		/** Format a date into a string for a statement to be supported by the DBMS.
		@param[in] date
			Date to format.
		@return
			The formatted date.
		*/
		virtual std::string WriteStmtDateS( const DateType & date ) const;

		/** Format a time into a string to be supported by the DBMS.
		@param[in] time
			Time to format.
		@return
			The formatted time.
		*/
		virtual std::string WriteTimeS( const TimeType & time ) const;

		/** Format a time into a string for a statement to be supported by the DBMS.
		@param[in] time
			Time to format.
		@return
			The formatted time.
		*/
		virtual std::string WriteStmtTimeS( const TimeType & time ) const;

		/** Format a date/time into a string to be supported by the DBMS.
		@param[in] dateTime
			Date/time to format.
		@return
			The formatted date/time.
		*/
		virtual std::string WriteDateTimeS( const DateTimeType & dateTime ) const;

		/** Format a date/time to insert into a request.
		@param[in] date
			Value to format.
		@return
			Formatted value.
		*/
		virtual std::string WriteDateTimeS( const DateType & date ) const;

		/** Format a date/time to insert into a request.
		@param[in] time
			Value to format.
		@return
			Formatted value.
		*/
		virtual std::string WriteDateTimeS( const TimeType & time ) const;

		/** Format a date/time into a string for a statement to be supported by the DBMS.
		@param[in] dateTime
			Date/time to format.
		@return
			The formatted date/time.
		*/
		virtual std::string WriteStmtDateTimeS( const DateTimeType & dateTime ) const;

		/** Format a date into a string to be supported by the DBMS.
		@param[in] date
			Date to format.
		@return
			The formatted date.
		*/
		virtual std::wstring WriteDateW( const DateType & date ) const;

		/** Format a date into a string for a statement to be supported by the DBMS.
		@param[in] date
			Date to format.
		@return
			The formatted date.
		*/
		virtual std::wstring WriteStmtDateW( const DateType & date ) const;

		/** Format a time into a string to be supported by the DBMS.
		@param[in] time
			Time to format.
		@return
			The formatted time.
		*/
		virtual std::wstring WriteTimeW( const TimeType & time ) const;

		/** Format a time into a string for a statement to be supported by the DBMS.
		@param[in] time
			Time to format.
		@return
			The formatted time.
		*/
		virtual std::wstring WriteStmtTimeW( const TimeType & time ) const;

		/** Format a date/time into a string to be supported by the DBMS.
		@param[in] dateTime
			Date/time to format.
		@return
			The formatted date/time.
		*/
		virtual std::wstring WriteDateTimeW( const DateTimeType & dateTime ) const;

		/** Format a date/time to insert into a request.
		@param[in] date
			Value to format.
		@return
			Formatted value.
		*/
		virtual std::wstring WriteDateTimeW( const DateType & date ) const;

		/** Format a date/time to insert into a request.
		@param[in] time
			Value to format.
		@return
			Formatted value.
		*/
		virtual std::wstring WriteDateTimeW( const TimeType & time ) const;

		/** Format a date/time into a string for a statement to be supported by the DBMS.
		@param[in] dateTime
			Date/time to format.
		@return
			The formatted date/time.
		*/
		virtual std::wstring WriteStmtDateTimeW( const DateTimeType & dateTime ) const;

		/** Convert a string from the database to a date.
		@param[in] date
			String representing a date.
		@return
			Created date with the string.
		*/
		virtual DateType ParseDate( const std::string & date ) const;

		/** Convert a string from the database to a time.
		@param[in] time
			String representing a time.
		@return
			Created time with the string.
		*/
		virtual TimeType ParseTime( const std::string & time ) const;

		/** Convert a string from the database to a date/time.
		@param[in] dateTime
			String representing a date/time.
		@return
			Created date/time with the string.
		*/
		virtual DateTimeType ParseDateTime( const std::string & dateTime ) const;

		/** Convert a string from the database to a date.
		@param[in] date
			String representing a date.
		@return
			Created date with the string.
		*/
		virtual DateType ParseDate( const std::wstring & date ) const;

		/** Convert a string from the database to a time.
		@param[in] time
			String representing a time.
		@return
			Created time with the string.
		*/
		virtual TimeType ParseTime( const std::wstring & time ) const;

		/** Convert a string from the database to a date/time.
		@param[in] dateTime
			String representing a date/time.
		@return
			Created date/time with the string.
		*/
		virtual DateTimeType ParseDateTime( const std::wstring & dateTime ) const;

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

	protected:
		/** Connect to the database.
		@param[out] connectionString
			Connection string in case of error.
		@return
			Error code, EErrorType_NONE if no problem.
		*/
		virtual EErrorType DoConnect( String & connectionString );

		/** Create a statement from a request.
		@param[in]  query
			Request text.
		@param[out] result
			Error code if the returned value is NULL.
		@return
			The created statement.
		*/
		virtual DatabaseStatementSPtr DoCreateStatement( const String & query);
	};
}
END_NAMESPACE_DATABASE_ODBC_MYSQL

#endif // ___DATABASE_CONNECTION_ODBC_MYSQL_H___
