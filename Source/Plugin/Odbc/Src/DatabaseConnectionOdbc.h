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

		/** Get the connection handle.
		@return
			The connection handle.
		*/
		HDBC GetHdbc() const;

	protected:
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
		@return
			The result.
		*/
		virtual bool DoExecuteUpdate( const String & query );

		/** Execute directly a request.
		@param[in]  query
			Request text.
		@param[out] infos
			The valued object infos array, if called from a CDatabaseQuery.
		@return
			The result.
		*/
		virtual DatabaseResultSPtr DoExecuteSelect( const String & query, DatabaseValuedObjectInfosPtrArray & infos );

	protected:
		//! Connection identifier.
		HDBC _connectionHandle;
		//! Connection environment.
		HENV _environmentHandle;
	};
}
END_NAMESPACE_DATABASE_ODBC

#endif // ___DATABASE_CONNECTION_ODBC_H___
