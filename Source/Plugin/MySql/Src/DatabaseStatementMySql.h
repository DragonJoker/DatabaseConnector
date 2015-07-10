/************************************************************************//**
 * @file DatabaseStatementMySql.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 *
 *
 * @brief CDatabaseStatementMySql class declaration.
 *
 * @details Describes a statement for MYSQL database.
 *
 ***************************************************************************/

#ifndef ___DATABASE_STATEMENT_MYSQL_H___
#define ___DATABASE_STATEMENT_MYSQL_H___

#include "DatabaseMySqlPrerequisites.h"

#include <DatabaseStatement.h>

BEGIN_NAMESPACE_DATABASE_MYSQL
{
	/** Describes a statement for MYSQL database.
	*/
	class CDatabaseStatementMySql
		: public CDatabaseStatement
	{

	public:
		/** Constructor.
		@param[in] connection
		    Database connection.
		@param[in] query
		    Request text.
		*/
		DatabaseMySqlExport CDatabaseStatementMySql( DatabaseConnectionMySqlPtr connection, const String & query );

		/** Destructor.
		 */
		DatabaseMySqlExport virtual ~CDatabaseStatementMySql();

		/** Initialize this statement.
		@return
		    Error code.
		*/
		DatabaseMySqlExport virtual EErrorType Initialize();

		/** Execute this statement.
		@param[out] result
		    Error code.
		@return
		    The result.
		*/
		DatabaseMySqlExport virtual bool ExecuteUpdate( EErrorType * result = NULL );

		/** Execute this statement.
		@param[out] result
		    Error code.
		@return
		    The result.
		*/
		DatabaseMySqlExport virtual DatabaseResultPtr ExecuteSelect( EErrorType * result = NULL );

		/** Clean statement.
		*/
		DatabaseMySqlExport virtual void Cleanup();

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
		DatabaseMySqlExport virtual DatabaseParameterPtr CreateParameter( const String & name, EFieldType fieldType, EParameterType parameterType );

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
		DatabaseMySqlExport virtual DatabaseParameterPtr CreateParameter( const String & name, EFieldType fieldType, uint32_t limits, EParameterType parameterType );

	private:
		/** Effectively prepares the statement
		@remarks
		    Replace '?' delimiters by '@paramName' for out an in/out parameters, in order to be able to retrieve their modified value
		@param[out] result
		    Receives the error code
		*/
		void DoPrepareStatement( EErrorType * result );

		/** Pre-execution action
		@remarks
		    Computes the final query from parameters values
		@param[out] result
		    Receives the error code
		@return
		    The full query
		*/
		void DoPreExecute( EErrorType * result );

		/** Pre-execution action
		@remarks
		    Computes the final query from parameters values
		@param[out] result
		    Receives the error code
		*/
		void DoPostExecute( EErrorType * result );

		/// Prepared statement.
		MYSQL_STMT * _statement;
		/// Database connection.
		DatabaseConnectionMySqlPtr _connectionMySql;
		/// Tokenized string (delimiter is "?").
		StringArray _arrayQueries;
		/// Number of parameters (i.e. number of "?").
		uint32_t _paramsCount;
		/// Array of in parameters
		DatabaseStatementParameterMySqlPtrArray _arrayInParams;
		/// The data bindings
		std::vector< MYSQL_BIND > _bindings;
		/// Array of out parameters
		DatabaseStatementParameterMySqlPtrArray _arrayOutParams;
		/// Array of in/out parameter initializer queries
		std::vector< std::pair< DatabaseStatementPtr, DatabaseParameterPtr > > _inOutInitializers;
		/// Array of out parameter initializer queries
		std::vector< DatabaseStatementPtr > _outInitializers;
		/// The statement used to execute the out parameters retrieval query
		DatabaseStatementPtr _stmtOutParams;
	};
}
END_NAMESPACE_DATABASE_MYSQL

#endif // ___DATABASE_STATEMENT_MYSQL_H___
