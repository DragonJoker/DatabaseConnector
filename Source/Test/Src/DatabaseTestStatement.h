/************************************************************************//**
* @file DatabaseTestStatement.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDatabaseTestStatement class declaration.
*
* @details Describes a statement for tests
*
***************************************************************************/

#ifndef ___DATABASE_TEST_STATEMENT_H___
#define ___DATABASE_TEST_STATEMENT_H___

#include "DatabaseTestPrerequisites.h"

#include <DatabaseStatement.h>

BEGIN_NAMESPACE_DATABASE_TEST
{
	/** Describes a statement for Test
	*/
	class CDatabaseTestStatement
		: public CDatabaseStatement
	{
	public:
		/** Constructor.
		@param[in] connection
			Database connection.
		@param[in] query
			Request text.
		*/
		CDatabaseTestStatement( DatabaseTestConnectionSPtr connection, const String & query );

		/** Destructor.
			*/
		virtual ~CDatabaseTestStatement();

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
		inline DatabaseTestConnectionSPtr DoGetTestConnection()const
		{
			return _connectionTest.lock();
		}

	private:
		/// Database connection.
		DatabaseTestConnectionWPtr _connectionTest;
		/// Tokenized string (delimiter is "?").
		StringArray _arrayQueries;
		/// Number of parameters (i.e. number of "?").
		uint32_t _paramsCount;
	};
}
END_NAMESPACE_DATABASE_TEST

#endif // ___DATABASE_TEST_STATEMENT_H___
