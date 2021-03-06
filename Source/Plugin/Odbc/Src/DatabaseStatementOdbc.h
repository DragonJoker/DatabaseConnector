/************************************************************************//**
* @file DatabaseStatementOdbc.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDatabaseStatementOdbc class declaration.
*
* @details Describes a statement for ODBC database.
*
***************************************************************************/

#ifndef ___DATABASE_STATEMENT_ODBC_H___
#define ___DATABASE_STATEMENT_ODBC_H___

#include "DatabaseOdbcPrerequisites.h"

#include <DatabaseStatement.h>

BEGIN_NAMESPACE_DATABASE_ODBC
{
	/** Describes a statement for ODBC database.
	*/
	class CDatabaseStatementOdbc
		: public CDatabaseStatement
	{
	public:
		/** Constructor.
		@param[in] connection
			Database connection.
		@param[in] query
			Request text.
		*/
		CDatabaseStatementOdbc( DatabaseConnectionOdbcSPtr connection, const String & query );

		/** Destructor.
			*/
		virtual ~CDatabaseStatementOdbc();

	private:
		/** Create a parameter.
		@param[in] infos
			Parameter informations.
		@param[in] parameterType
			Parameter type.
		@return
			Created parameter.
		*/
		virtual DatabaseParameterSPtr DoCreateParameter( DatabaseValuedObjectInfosSPtr infos, EParameterType parameterType );

		/** Initialise this statement.
		@remarks
			The statement *MUST* be initialised, *AFTER* all parameters have been created.
		@return
			Error code.
		*/
		virtual EErrorType DoInitialise();

		/** Execute this statement.
		@return
			The result.
		*/
		virtual bool DoExecuteUpdate();

		/** Execute this statement.
		@return
			The result.
		*/
		virtual DatabaseResultSPtr DoExecuteSelect();

		/** Clean statement.
		*/
		virtual void DoCleanup();

		/** Pre-execute operations.
		@return
			Error code.
		*/
		EErrorType DoPreExecute();

		/** Execute this statement.
		@param[out] ret
			Receives the result set, if any.
		@return
			The error code.
		*/
		virtual EErrorType DoExecute( DatabaseResultSPtr & ret );

		/** Function called when the result set is fully fetched (to retrieve out parameters values)
		@param statementHandle
			The statement fetched
		@info
			The execution result
		*/
		void OnResultSetFullyFetched( HSTMT statementHandle, SQLRETURN info );

		/** Function called when the result set is fully fetched (to retrieve out parameters values)
		@param statementHandle
			The statement fetched
		@info
			The execution result
		*/
		inline DatabaseConnectionOdbcSPtr DoGetConnectionOdbc()
		{
			return _connectionOdbc.lock();
		}

	private:
		//! Statement handle.
		SQLHSTMT _statementHandle;
		//! Database connection.
		DatabaseConnectionOdbcWPtr _connectionOdbc;
	};
}
END_NAMESPACE_DATABASE_ODBC

#endif // ___DATABASE_STATEMENT_ODBC_H___
