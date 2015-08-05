/************************************************************************//**
* @file DatabaseTestConnection.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/14/2014 5:03:05 PM
*
*
* @brief CDatabaseConnectionTest class declaration.
*
* @details Describes a connection used to run the tests.
*
***************************************************************************/

#ifndef ___DATABASE_TEST_CONNECTION_H___
#define ___DATABASE_TEST_CONNECTION_H___

#include "DatabaseTestPrerequisites.h"

#include <DatabaseConnection.h>

BEGIN_NAMESPACE_DATABASE_TEST
{
	static const String TEST_SQL_NULL = STR( "NULL" );

	static const String TEST_FORMAT_DATE = STR( "CAST('%Y-%m-%d' AS DATE)" );
	static const String TEST_FORMAT_TIME = STR( "CAST('%H:%M:%S' AS TIME)" );
	static const String TEST_FORMAT_DATETIME = STR( "CAST('%Y-%m-%d %H:%M:%S' AS DATETIME)" );
	static const String TEST_FORMAT_DATETIME_DATE = STR( "CAST('%Y-%m-%d 00:00:00' AS DATETIME)" );
	static const String TEST_FORMAT_DATETIME_TIME = STR( "CAST('0000-00-00 %H:%M:%S' AS DATETIME)" );
	static const String TEST_FORMAT_STMT_DATE = STR( "{-d %Y-%m-%d}" );
	static const String TEST_FORMAT_STMT_TIME = STR( "{-t %H:%M:%S}" );
	static const String TEST_FORMAT_STMT_DATETIME = STR( "{-ts %Y-%m-%d %H:%M:%S}" );

	static const String TEST_GOOD_SERVER = STR( "TestServer" );
	static const String TEST_GOOD_USER = STR( "TestUser" );
	static const String TEST_GOOD_PASSWORD = STR( "TestPassword" );
	static const String TEST_GOOD_DATABASE = STR( "TestDatabase" );
	static const String TEST_GOOD_SELECT = STR( "TestSelect" );
	static const String TEST_GOOD_UPDATE = STR( "TestUpdate" );

	/** Describes a connection to a database via an MYSQL driver.
	*/
	class CDatabaseTestConnection
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
		CDatabaseTestConnection( const String & server, const String & userName, const String & password, String & connectionString );

		/** Destructor.
		*/
		virtual ~CDatabaseTestConnection();

		/** Retrieves the precision for given field type.
		@return
			The precision.
		*/
		virtual uint32_t GetPrecision( EFieldType type ) const;

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

		/** Creates a database.
		@param[in] database
			Database identifier (name or DSN (ODBC)).
		*/
		virtual void DoCreateDatabase( const String & database );

		/** Selects a database.
		@param[in] database
			Database identifier (name or DSN (ODBC)).
		*/
		virtual void DoSelectDatabase( const String & database );

		/** Destroys a database.
		@param[in] database
			Database identifier (name or DSN (ODBC)).
		*/
		virtual void DoDestroyDatabase( const String & database );

		/** Format a string to be supported by the DBMS.
		@param[in] text
			Text to format.
		@return
			The formatted text.
		*/
		virtual std::string DoWriteText( const std::string & text ) const;

		/** Format a string to be supported by the DBMS.
		@param[in] text
			Text to format.
		@return
			The formatted text.
		*/
		virtual std::wstring DoWriteNText( const std::wstring & text ) const;

		/** Format a byte array to insert into a request.
		@param[in] array
			Value to format.
		@return
			Formatted value.
		*/
		virtual String DoWriteBinary( const ByteArray & array ) const;

		/** Format a string to be supported by the DBMS.
		@param[in] text
			Text to format.
		@return
			The formatted text.
		*/
		virtual String DoWriteName( const String & text ) const;

		/** Format a date into a string to be supported by the DBMS.
		@param[in] date
			Date to format.
		@return
			The formatted date.
		*/
		virtual String DoWriteDate( const DateType & date ) const;

		/** Format a date into a string for a statement to be supported by the DBMS.
		@param[in] date
			Date to format.
		@return
			The formatted date.
		*/
		virtual String DoWriteStmtDate( const DateType & date ) const;

		/** Format a time into a string to be supported by the DBMS.
		@param[in] time
			Time to format.
		@return
			The formatted time.
		*/
		virtual String DoWriteTime( const TimeType & time ) const;

		/** Format a time into a string for a statement to be supported by the DBMS.
		@param[in] time
			Time to format.
		@return
			The formatted time.
		*/
		virtual String DoWriteStmtTime( const TimeType & time ) const;

		/** Format a date/time into a string to be supported by the DBMS.
		@param[in] dateTime
			Date/time to format.
		@return
			The formatted date/time.
		*/
		virtual String DoWriteDateTime( const DateTimeType & dateTime ) const;

		/** Format a date/time to insert into a request.
		@param[in] date
			Value to format.
		@return
			Formatted value.
		*/
		virtual String DoWriteDateTime( const DateType & date ) const;

		/** Format a date/time to insert into a request.
		@param[in] time
			Value to format.
		@return
			Formatted value.
		*/
		virtual String DoWriteDateTime( const TimeType & time ) const;

		/** Format a date/time into a string for a statement to be supported by the DBMS.
		@param[in] dateTime
			Date/time to format.
		@return
			The formatted date/time.
		*/
		virtual String DoWriteStmtDateTime( const DateTimeType & dateTime ) const;

		/** Format a boolean into a string to be supported by the DBMS.
		@param[in] value
			Boolean to format.
		@return
			The formatted boolean.
		*/
		virtual String DoWriteBool( bool value ) const;

		/** Convert a string from the database to a date.
		@param[in] date
			String representing a date.
		@return
			Created date with the string.
		*/
		virtual DateType DoParseDate( const String & date ) const;

		/** Convert a string from the database to a time.
		@param[in] time
			String representing a time.
		@return
			Created time with the string.
		*/
		virtual TimeType DoParseTime( const String & time ) const;

		/** Convert a string from the database to a date/time.
		@param[in] dateTime
			String representing a date/time.
		@return
			Created date/time with the string.
		*/
		virtual DateTimeType DoParseDateTime( const String & dateTime ) const;

		/** Connect to the database.
		@param[out] connectionString
			Connection string in case of error.
		@return
			Error code, EErrorType_NONE if no problem.
		*/
		virtual EErrorType DoConnect( String & connectionString );

		/** Disconnect from the database.
		*/
		virtual void DoDisconnect();

		/** Initialise a named transaction.
		@param[in] name
			Transaction name.
		@return
			true if no problem
		*/
		virtual bool DoBeginTransaction( const String & name );

		/** Validate a named transaction.
		@param[in] name
			Transaction name.
		@return
			true if no problem
		*/
		virtual bool DoCommit( const String & name );

		/** Invalidate a named transaction.
		@param[in] name
			Transaction name.
		@return
			true if no problem
		*/
		virtual bool DoRollBack( const String & name );

		/** Execute directly a request.
		@param[in]  query
			Request text.
		@param[out] result
			Error code if the returned value is NULL.
		@return
			The result.
		*/
		virtual bool DoExecuteUpdate( const String & query );

		/** Execute directly a request with a result set.
		@param[in] query
			Request text.
		@param[out] infos
			The valued object infos array, if called from a CDatabaseQuery.
		@return
			The result.
		*/
		virtual DatabaseResultSPtr DoExecuteSelect( const String & query, DatabaseValuedObjectInfosPtrArray & infos );

		/** Create a statement from a request.
		@param[in]  query
			Request text.
		@param[out] result
			Error code if the returned value is NULL.
		@return
			The created statement.
		*/
		virtual DatabaseStatementSPtr DoCreateStatement( const String & query );
	};
}
END_NAMESPACE_DATABASE_TEST

#endif // ___DATABASE_TEST_CONNECTION_H___
