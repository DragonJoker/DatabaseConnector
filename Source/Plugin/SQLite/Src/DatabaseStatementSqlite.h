/************************************************************************//**
* @file DatabaseStatementSqlite.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDatabaseStatementSqlite class declaration.
*
* @details Describes a statement for SQLite database.
*
***************************************************************************/

#ifndef ___DATABASE_STATEMENT_SQLITE_H___
#define ___DATABASE_STATEMENT_SQLITE_H___

#include "DatabaseSqlitePrerequisites.h"

#include <DatabaseStatement.h>

BEGIN_NAMESPACE_DATABASE_SQLITE
{
	/** Describes a statement for SQLite database.
	*/
	class CDatabaseStatementSqlite
		: public CDatabaseStatement
	{

	public:
		/** Constructor.
		@param[in] connection
			Database connection.
		@param[in] query
			Request text.
		*/
		CDatabaseStatementSqlite( DatabaseConnectionSqliteSPtr connection, const String & query );

		/** Destructor.
		*/
		virtual ~CDatabaseStatementSqlite();

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
		@param[out] result
			Error code.
		@return
			The result.
		*/
		virtual bool DoExecuteUpdate( EErrorType * result = NULL );

		/** Execute this statement.
		@param[out] result
			Error code.
		@return
			The result.
		*/
		virtual DatabaseResultSPtr DoExecuteSelect( EErrorType * result = NULL );

		/** Clean statement.
		*/
		virtual void DoCleanup();

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

		/** Retrieves the SQLite connection
		@return
			The connection.
		*/
		inline DatabaseConnectionSqliteSPtr DoGetSqliteConnection()const
		{
			return _connectionSqlite.lock();
		}

	private:
		//! Prepared statement.
		sqlite3_stmt * _statement;
		//! Database connection.
		DatabaseConnectionSqliteWPtr _connectionSqlite;
		//! Tokenized string (delimiter is "?").
		StringArray _arrayQueries;
		//! Number of parameters (i.e. number of "?").
		uint32_t _paramsCount;
		//! Array of in parameters
		DatabaseParameterPtrArray _arrayInParams;
		//! Array of out parameters
		DatabaseParameterPtrArray _arrayOutParams;
		//! Array of in/out parameter initializer queries
		std::vector< std::pair< DatabaseStatementSPtr, DatabaseParameterWPtr > > _inOutInitializers;
		//! Array of out parameter initializer queries
		std::vector< DatabaseStatementSPtr > _outInitializers;
		//! The statement used to execute the out parameters retrieval query
		DatabaseStatementSPtr _stmtOutParams;
	};
}
END_NAMESPACE_DATABASE_SQLITE

#endif // ___DATABASE_STATEMENT_SQLITE_H___
