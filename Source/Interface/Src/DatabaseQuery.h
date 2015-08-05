/************************************************************************//**
* @file DatabaseQuery.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDatabaseQuery class declaration.
*
* @details Describes a database query.
*
***************************************************************************/

#ifndef ___DATABASE_QUERY_H___
#define ___DATABASE_QUERY_H___

#include "DatabaseParameteredObject.h"

#include "EErrorType.h"

BEGIN_NAMESPACE_DATABASE
{
	/** Describes a database query.
	@remarks
		It can't be used to execute stored procedures with output parameters.
		For this purpose, use CDatabaseStatement
	*/
	class CDatabaseQuery
		: public CDatabaseParameteredObject
	{
	public:
		/** Constructor.
		@param[in] connection
			Database connection.
		@param[in] query
			Request text.
		*/
		DatabaseExport CDatabaseQuery( DatabaseConnectionSPtr connection, const String & query );

		/** Destructor.
		*/
		DatabaseExport virtual ~CDatabaseQuery();

		/** Execute a query that has no result set.
		@return
			Results.
		*/
		DatabaseExport bool ExecuteUpdate();

		/** Execute query that has a result set.
		@return
			Results.
		*/
		DatabaseExport DatabaseResultSPtr ExecuteSelect();

		/** Create a query parameter.
		@param[in] name
			Parameter name.
		@param[in] fieldType
			Field type.
		@return
			Created query parameter.
		*/
		DatabaseExport DatabaseParameterSPtr CreateParameter( const String & name, EFieldType fieldType );

		/** Create a query parameter for variable-sized parameter (with limits)
		@param[in] name
			Parameter name.
		@param[in] fieldType
			Field type.
		@param[in] limits
			Field limits.
		@return
			Created query parameter.
		*/
		DatabaseExport DatabaseParameterSPtr CreateParameter( const String & name, EFieldType fieldType, uint32_t limits );

		/** Create a query parameter for variable-sized parameter (with precision)
		@param[in] name
			Parameter name.
		@param[in] fieldType
			Field type.
		@param[in] limits
			Field precision and scale.
		@return
			Created query parameter.
		*/
		DatabaseExport DatabaseParameterSPtr CreateParameter( const String & name, EFieldType fieldType, const std::pair< uint32_t, uint32_t > & precision );

	protected:
		/** Initialise query.
		@remarks
			The query *MUST* be initialised, *AFTER* all parameters have been created.
		@return
			Error code.
		*/
		virtual EErrorType DoInitialise();

		/** Clean query.
		*/
		virtual void DoCleanup();

		/** Creates a parameter, given it's valued object informations
		@param[in] infos
			Parameter informations.
		@return
			The created parameter
		*/
		virtual DatabaseParameterSPtr DoCreateParameter( DatabaseValuedObjectInfosSPtr infos, EParameterType parameterType = EParameterType_IN );

		/** Pre-execution action
		@remarks
			Computes the final query from parameters values
		@return
			The full query
		*/
		String DoPreExecute();

		/** Execute query.
		@return
			Results.
		*/
		DatabaseResultSPtr DoExecute();

		/** Retrieves the connection
		@return
			The connection.
		*/
		inline DatabaseConnectionSPtr DoGetConnection()
		{
			return _connection.lock();
		}

	protected:
		//! Request text.
		String _query;
		//! Tokenized string (delimiter is "?").
		StringArray _arrayQueries;
		//! Database connection.
		DatabaseConnectionWPtr _connection;
		//! Number of parameters (i.e. number of "?").
		uint32_t _paramsCount;
	};
}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_QUERY_H___
