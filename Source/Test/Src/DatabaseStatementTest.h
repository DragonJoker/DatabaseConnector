/************************************************************************//**
* @file DatabaseStatementTest.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDatabaseStatementTest class declaration.
*
* @details Describes a statement for tests
*
***************************************************************************/

#ifndef ___DATABASE_STATEMENT_TEST_H___
#define ___DATABASE_STATEMENT_TEST_H___

#include "DatabaseTestPrerequisites.h"

#include <DatabaseStatement.h>

BEGIN_NAMESPACE_DATABASE_TEST
{
	/** Describes a statement for Test
	*/
	class CDatabaseStatementTest
		: public CDatabaseStatement
	{
	public:
		/** Constructor.
		@param[in] connection
			Database connection.
		@param[in] query
			Request text.
		*/
		CDatabaseStatementTest( DatabaseConnectionTestSPtr connection, const String & query );

		/** Destructor.
			*/
		virtual ~CDatabaseStatementTest();

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
		inline DatabaseConnectionTestSPtr DoGetTestConnection()const
		{
			return _connectionTest.lock();
		}

	private:
		/// Database connection.
		DatabaseConnectionTestWPtr _connectionTest;
		/// Tokenized string (delimiter is "?").
		StringArray _arrayQueries;
		/// Number of parameters (i.e. number of "?").
		uint32_t _paramsCount;
	};
}
END_NAMESPACE_DATABASE_TEST

#endif // ___DATABASE_STATEMENT_TEST_H___
