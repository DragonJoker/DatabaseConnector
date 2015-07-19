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

		/** Get the connection handle.
		@return
			The connection handle.
		*/
		HDBC GetHdbc() const;

	protected:
		/** Disconnect from the database.
		*/
		virtual void DoDisconnect();

		/** Initialize a named transaction.
		@param[in] name
			Transaction name.
		@return
			Error code, EErrorType_NONE if no problem.
		*/
		virtual bool DoBeginTransaction( const String & name );

		/** Validate a named transaction.
		@param[in] name
			Transaction name.
		@return
			Error code, EErrorType_NONE if no problem.
		*/
		virtual bool DoCommit( const String & name );

		/** Invalidate a named transaction.
		@param[in] name
			Transaction name.
		@return
			Error code, EErrorType_NONE if no problem.
		*/
		virtual bool DoRollBack( const String & name );

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
		virtual bool DoExecuteUpdate( const String & query);

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
		virtual DatabaseResultPtr DoExecuteSelect( const String & query);

	protected:
		//! Connection identifier.
		HDBC _connectionHandle;
		//! Connection environment.
		HENV _environmentHandle;
	};
}
END_NAMESPACE_DATABASE_ODBC

#endif // ___DATABASE_CONNECTION_ODBC_H___
