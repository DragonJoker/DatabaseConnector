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
		CDatabaseStatementMySql( DatabaseConnectionMySqlSPtr connection, const String & query );

		/** Destructor.
			*/
		virtual ~CDatabaseStatementMySql();

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
		virtual DatabaseParameterSPtr CreateParameter( const String & name, EFieldType fieldType, EParameterType parameterType );

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
		virtual DatabaseParameterSPtr CreateParameter( const String & name, EFieldType fieldType, uint32_t limits, EParameterType parameterType );

		/** Create a parameter which has limits (strings, etc.).
		@param[in] name
			Parameter name.
		@param[in] fieldType
			Date type.
		@param[in] precision
			Field precision.
		@param[in] parameterType
			Parameter type.
		@return
			Created parameter.
		*/
		virtual DatabaseParameterSPtr CreateParameter( const String & name, EFieldType fieldType, const std::pair< uint32_t, uint32_t > & precision, EParameterType parameterType );

	private:
		/** Initialize this statement.
		@return
			Error code.
		*/
		virtual EErrorType DoInitialize();

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

		/** Pre-execution action
		@remarks
			Computes the final query from parameters values
		@return
			The full query
		*/
		void DoPreExecute();

		/** Pre-execution action
		@remarks
			Computes the final query from parameters values
		*/
		void DoPostExecute();

		/** Retrieves the MySQL connection
		@return
			The connection.
		*/
		inline DatabaseConnectionMySqlSPtr DoGetMySqlConnection()const
		{
			return _connectionMySql.lock();
		}

	private:
		/// Prepared statement.
		MYSQL_STMT * _statement;
		/// Database connection.
		DatabaseConnectionMySqlWPtr _connectionMySql;
		/// Tokenized string (delimiter is "?").
		StringArray _arrayQueries;
		/// Number of parameters (i.e. number of "?").
		uint32_t _paramsCount;
		/// Array of in parameters
		DatabaseParameterMySqlPtrArray _arrayInParams;
		/// The data bindings
		std::vector< MYSQL_BIND > _bindings;
		/// Array of out parameters
		DatabaseParameterMySqlPtrArray _arrayOutParams;
		/// Array of in/out parameter initializer queries
		std::vector< std::pair< DatabaseStatementSPtr, DatabaseParameterWPtr > > _inOutInitializers;
		/// Array of out parameter initializer queries
		std::vector< DatabaseStatementSPtr > _outInitializers;
		/// The statement used to execute the out parameters retrieval query
		DatabaseStatementSPtr _stmtOutParams;
	};
}
END_NAMESPACE_DATABASE_MYSQL

#endif // ___DATABASE_STATEMENT_MYSQL_H___
