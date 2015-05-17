/************************************************************************//**
 * @file DatabaseConnectionOdbcMsSql.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/14/2014 5:03:05 PM
 *
 *
 * @brief CDatabaseConnectionOdbcMsSql class declaration.
 *
 * @details Describes a connection to a database via an ODBC driver.
 *
 ***************************************************************************/

#ifndef ___DATABASE_CONNECTION_ODBC_MSSQL_H___
#define ___DATABASE_CONNECTION_ODBC_MSSQL_H___

#include "DatabaseOdbcMsSqlPrerequisites.h"

#include <DatabaseConnectionOdbc.h>

BEGIN_NAMESPACE_DATABASE_ODBC_MSSQL
{
	/** Describes a connection to a database via an ODBC driver.
	*/
	class DatabaseOdbcMsSqlExport CDatabaseConnectionOdbcMsSql
		: public CDatabaseConnectionOdbc
		, public std::enable_shared_from_this<CDatabaseConnectionOdbcMsSql>
	{

	public:

		/** Constructor.
		@param[in] sqlEnvironmentHandle
		    The handle to the SQL environment.
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
		CDatabaseConnectionOdbcMsSql( SQLHENV sqlEnvironmentHandle, const String & server, const String & database, const String & userName, const String & password, String & connectionString );

		/** Destructor.
		 */
		virtual ~CDatabaseConnectionOdbcMsSql();

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
		virtual DatabaseStatementPtr DoCreateStatement( const String & query, EErrorType * result );

		/** Create a query from a request.
		@param[in]  query
		    Request text.
		@param[out] result
		    Error code if the returned value is NULL.
		@return
		    The created query.
		*/
		virtual DatabaseQueryPtr DoCreateQuery( const String & query, EErrorType * result );
	};
}
END_NAMESPACE_DATABASE_ODBC_MSSQL

#endif // ___DATABASE_CONNECTION_ODBC_MSSQL_H___
