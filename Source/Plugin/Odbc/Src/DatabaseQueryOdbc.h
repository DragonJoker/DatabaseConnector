/************************************************************************//**
* @file DatabaseQueryOdbc.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDatabaseQueryOdbc class declaration.
*
* @details Describes a query for ODBC database.
*
***************************************************************************/

#ifndef ___DATABASE_QUERY_ODBC_H___
#define ___DATABASE_QUERY_ODBC_H___

#include "DatabaseOdbcPrerequisites.h"

#include <DatabaseQuery.h>

BEGIN_NAMESPACE_DATABASE_ODBC
{
	/** Describes a statement for ODBC database.
	*/
	class CDatabaseQueryOdbc
		: public CDatabaseQuery
	{
	public:
		/** Constructor.
		@param[in] connection
			Database connection.
		@param[in] query
			Request text.
		*/
		CDatabaseQueryOdbc( DatabaseConnectionOdbcPtr connection, const String & query );

		/** Destructor.
			*/
		virtual ~CDatabaseQueryOdbc();

		/** Initialize this statement.
		@return
			Error code.
		*/
		virtual EErrorType Initialize();

		/** Execute this statement.
		@param[out] result
			Error code.
		@return
			The result.
		*/
		virtual bool ExecuteUpdate( EErrorType * result = NULL );

		/** Execute this statement.
		@param[out] result
			Error code.
		@return
			The result.
		*/
		virtual DatabaseResultPtr ExecuteSelect( EErrorType * result = NULL );

		/** Clean statement.
		*/
		virtual void Cleanup();

		/** Create a parameter.
		@param[in] name
			Parameter name.
		@param[in] fieldType
			Date type.
		@param[in] parameterType
			Parameter type.
		@return
			Created parameter.
		*/
		virtual DatabaseParameterPtr CreateParameter( const String & name, EFieldType fieldType, EParameterType parameterType );

		/** Create a parameter which has limits (strings, etc.).
		@param[in] name
			Parameter name.
		@param[in] fieldType
			Date type.
		@param[in] limits
			Size limits.
		@param[in] parameterType
			Parameter type.
		@return
			Created parameter.
		*/
		virtual DatabaseParameterPtr CreateParameter( const String & name, EFieldType fieldType, uint32_t limits, EParameterType parameterType );

	private:
		/** Pre-execute operations.
		@param statementHandle
			The statement handle
		@return
			Error code.
		*/
		EErrorType DoPreExecute( HSTMT statementHandle );

		/** Execute this statement.
		@param[out] ret
			Receives the result set, if any.
		@return
			The error code.
		*/
		virtual EErrorType DoExecute( DatabaseResultPtr & ret );

		/** Function called when the result set is fully fetched (to retrieve out parameters values)
		@param statementHandle
			The statement fetched
		@info
			The execution result
		*/
		void OnResultSetFullyFetched( HSTMT statementHandle, SQLRETURN info );

	private:
		//! Database connection.
		DatabaseConnectionOdbcPtr _connectionOdbc;
	};
}
END_NAMESPACE_DATABASE_ODBC

#endif // ___DATABASE_STATEMENT_ODBC_H___
