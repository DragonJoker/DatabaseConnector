/************************************************************************//**
* @file DatabaseStatementPostgreSql.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDatabaseStatementPostgreSql class declaration.
*
* @details Describes a statement for MYSQL database.
*
***************************************************************************/

#ifndef ___DATABASE_STATEMENT_POSTGRESQL_H___
#define ___DATABASE_STATEMENT_POSTGRESQL_H___

#include "DatabasePostgreSqlPrerequisites.h"
#include "DatabasePostgreSqlHelper.h"

#include <DatabaseStatement.h>

BEGIN_NAMESPACE_DATABASE_POSTGRESQL
{
	/** Describes a statement for MYSQL database.
	*/
	class CDatabaseStatementPostgreSql
		: public CDatabaseStatement
	{
	public:
		/** Constructor.
		@param[in] connection
			Database connection.
		@param[in] query
			Request text.
		*/
		CDatabaseStatementPostgreSql( DatabaseConnectionPostgreSqlSPtr connection, const String & query );

		/** Destructor.
			*/
		virtual ~CDatabaseStatementPostgreSql();

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

		/** Retrieves the PostgreSQL connection
		@return
			The connection.
		*/
		inline DatabaseConnectionPostgreSqlSPtr DoGetPostgreSqlConnection()const
		{
		return _connectionPostgreSql.lock();
		}

	private:
		/// Prepared statement.
		std::string _statement;
		/// Database connection.
		DatabaseConnectionPostgreSqlWPtr _connectionPostgreSql;
		/// Tokenized string (delimiter is "?").
		StringArray _arrayQueries;
		/// Number of parameters (i.e. number of "?").
		uint32_t _paramsCount;
		/// Array of out parameters
		DatabaseParameterPostgreSqlPtrArray _arrayOutParams;
		/// The data bindings
		std::vector< PGbind > _bindings;
		//! The parameters values
		std::vector< char * > _arrayValues;
		//! The parameters lengths
		std::vector< int > _arrayLengths;
		//! The parameters formats
		std::vector< int > _arrayFormats;
		//! The parameters Oids
		std::vector< Oid > _arrayOids;
	};
}
END_NAMESPACE_DATABASE_POSTGRESQL

#endif // ___DATABASE_STATEMENT_POSTGRESQL_H___
