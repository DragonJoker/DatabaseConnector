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

#include "DatabasePrerequisites.h"

#include "Database.h"
#include "DatabaseParameter.h"
#include "DatabaseException.h"

#include "EErrorType.h"
#include "EFieldType.h"
#include "EParameterType.h"

BEGIN_NAMESPACE_DATABASE
{
	/** Describes a database query.
	*/
	class CDatabaseQuery
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

		/** Initialize query.
		@remarks
			The query *MUST* be initialized, *AFTER* all parameters have been created.
		@return
			Error code.
		*/
		DatabaseExport EErrorType Initialize();

		/** Execute a query that has no result set.
		@param[out] result
			Error code.
		@return
			Results.
		*/
		DatabaseExport bool ExecuteUpdate( EErrorType * result = NULL );

		/** Execute query that has a result set.
		@param[out] result
			Error code.
		@return
			Results.
		*/
		DatabaseExport DatabaseResultSPtr ExecuteSelect( EErrorType * result = NULL );

		/** Clean query.
		*/
		DatabaseExport void Cleanup();

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

		/** Create a query parameter for variable-sized parameter (with limits)
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

		/** Retrieves a parameter, by index
		@param[in] index
			Parameter index.
		*/
		DatabaseExport DatabaseParameterSPtr GetParameter( uint32_t index )const;

		/** Retrieves a parameter, by name
		@param[in] name
			Parameter name.
		*/
		DatabaseExport DatabaseParameterSPtr GetParameter( const String & name )const;

		/** Get parameter type.
		@param[in] index
			Parameter index.
		@return
			Parameter type.
		*/
		DatabaseExport EFieldType GetParameterType( uint32_t index );

		/** Set parameter value to NULL.
		@param[in] index
			Parameter index.
		*/
		DatabaseExport void SetParameterNull( uint32_t index );

		/** Set parameter value to NULL.
		@param[in] name
			Parameter name.
		*/
		DatabaseExport void SetParameterNull( const String & name );

		/** Set parameter value from another parameter.
		@param[in] index
			Parameter index.
		@param[in] parameter
			The parameter.
		*/
		DatabaseExport void SetParameterValue( uint32_t index, const CDatabaseParameter & parameter );

		/** Set parameter value from another parameter.
		@param[in] name
			Parameter name.
		@param[in] parameter
			The parameter.
		*/
		DatabaseExport void SetParameterValue( const String & name, const CDatabaseParameter & parameter );

		/** Set parameter value.
		@param[in] index
			Parameter index.
		@param[in] value
			Parameter value.
		*/
		template< typename T > void SetParameterValue( uint32_t index, const T & value );

		/** Set parameter value.
		@param[in] name
			Parameter name.
		@param[in] value
			Parameter value.
		*/
		template< typename T > void SetParameterValue( const String & name, const T & value );

		/** Set parameter value.
		@param[in] index
			Parameter index.
		@param[in] value
			Parameter value.
		*/
		template< typename T > void SetParameterValueFast( uint32_t index, const T & value );

		/** Set parameter value.
		@param[in] name
			Parameter name.
		@param[in] value
			Parameter value.
		*/
		template< typename T > void SetParameterValueFast( const String & name, const T & value );

		/** Get parameter value.
		@param[in] index
			Parameter index.
		@return
			Parameter value.
		*/
		template< typename T > T const & GetOutputValue( uint32_t index );

		/** Get parameter value.
		@param[in] name
			Parameter name.
		@return
			Parameter value.
		*/
		template< typename T > T const & GetOutputValue( const String & name );

		/** Get parameter value.
		@param[in] index
			Parameter index.
		@param[out] value
			Parameter value.
		*/
		template< typename T > void GetOutputValue( uint32_t index, T & value );

		/** Get parameter value.
		@param[in] name
			Parameter name.
		@param[out] value
			Parameter value.
		*/
		template< typename T > void GetOutputValue( const String & name, T & value );

	protected:
		/** Add parameter to query.
		@param[in] parameter
			Parameter to add.
		@return
			true if addition succeeds, false otherwise.
		*/
		DatabaseExport bool DoAddParameter( DatabaseParameterSPtr parameter );

		/** Pre-execution action
		@remarks
			Computes the final query from parameters values
		@return
			The full query
		*/
		DatabaseExport String DoPreExecute();

		/** Execute query.
		@param[out] result
			Error code.
		@return
			Results.
		*/
		DatabaseExport DatabaseResultSPtr DoExecute( EErrorType * result = NULL );

		/** Retrieves the connection
		@return
			The connection.
		*/
		inline DatabaseConnectionSPtr DoGetConnection()
		{
			return _connection.lock();
		}

	protected:
		//! Array of parameters (addition order).
		DatabaseParameterPtrArray _arrayParams;
		//! Request text.
		String _query;
		//! Tokenized string (delimiter is "?").
		StringArray _arrayQueries;
		//! Database connection.
		DatabaseConnectionWPtr _connection;
		//! Number of parameters (i.e. number of "?").
		uint32_t _paramsCount;

		/** Does nothing
		*/
		struct SDummyValueUpdater
			: public CDatabaseParameter::SValueUpdater
		{
			/** Constructor
			*/
			DatabaseExport SDummyValueUpdater(){}

			//!@copydoc CDatabaseParameter::SValueUpdater
			DatabaseExport virtual void Update( const CDatabaseParameter & value ){}
		};
	};
}
END_NAMESPACE_DATABASE

#include "DatabaseQuery.inl"

#endif // ___DATABASE_QUERY_H___
