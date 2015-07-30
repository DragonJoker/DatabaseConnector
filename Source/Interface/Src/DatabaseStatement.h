/************************************************************************//**
* @file DatabaseStatement.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDatabaseStatement class declaration.
*
* @details Describes a database statement.
*
***************************************************************************/

#ifndef ___DATABASE_STATEMENT_H___
#define ___DATABASE_STATEMENT_H___

#include "DatabasePrerequisites.h"

#include "Database.h"
#include "DatabaseParameter.h"
#include "DatabaseException.h"

#include "EErrorType.h"
#include "EFieldType.h"
#include "EParameterType.h"

BEGIN_NAMESPACE_DATABASE
{
	/** Describes a database statement.
	*/
	class CDatabaseStatement
	{
	public:
		/** Constructor.
		@param[in] connection
			Database connection.
		@param[in] query
			Request text.
		*/
		DatabaseExport CDatabaseStatement( DatabaseConnectionSPtr connection, const String & query );

		/** Destructor.
		*/
		DatabaseExport virtual ~CDatabaseStatement();

		/** Initialize query.
		@remarks
			The statement *MUST* be initialized, *AFTER* all parameters have been created.
		@return
			Error code.
		*/
		DatabaseExport EErrorType Initialize();

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

		/** Clean query.
		*/
		DatabaseExport void Cleanup();

		/** Create a statement parameter.
		@param[in] name
			Parameter name.
		@param[in] fieldType
			Field type.
		@param[in] parameterType
			Parameter type.
		@return
			Created statement parameter.
		*/
		DatabaseExport virtual DatabaseParameterSPtr CreateParameter( const String & name, EFieldType fieldType, EParameterType parameterType = EParameterType_IN ) = 0;

		/** Create a query parameter for variable-sized parameter (with limits)
		@param[in] name
			Parameter name.
		@param[in] fieldType
			Field type.
		@param[in] precision
			Field precision and scale.
		@return
			Created query parameter.
		*/
		DatabaseExport virtual DatabaseParameterSPtr CreateParameter( const String & name, EFieldType fieldType, const std::pair< uint32_t, uint32_t > & precision, EParameterType parameterType = EParameterType_IN ) = 0;

		/** Create a statement parameter for variable-sized parameter (with limits)
		@param[in] name
			Parameter name.
		@param[in] fieldType
			Field type.
		@param[in] limits
			Field limits.
		@param[in] parameterType
			Parameter type.
		@return
			Created statement parameter.
		*/
		DatabaseExport virtual DatabaseParameterSPtr CreateParameter( const String & name, EFieldType fieldType, uint32_t limits, EParameterType parameterType = EParameterType_IN ) = 0;

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

		/** Get output value.
		@param[in] index
			Parameter index.
		@return
			Parameter value.
		*/
		template< typename T > T const & GetOutputValue( uint32_t index );

		/** Get output value.
		@param[in] name
			Parameter name.
		@return
			Parameter value.
		*/
		template< typename T > T const & GetOutputValue( const String & name );

		/** Get output value.
		@param[in] index
			Parameter index.
		@param[out] value
			Parameter value.
		*/
		template< typename T > void GetOutputValue( uint32_t index, T & value );

		/** Get output value.
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

		/** Initialize query.
		@remarks
			The statement *MUST* be initialized, *AFTER* all parameters have been created.
		@return
			Error code.
		*/
		DatabaseExport virtual EErrorType DoInitialize() = 0;

		/** Execute a query that has no result set.
		@return
			Results.
		*/
		DatabaseExport virtual bool DoExecuteUpdate() = 0;

		/** Execute query that has a result set.
		@return
			Results.
		*/
		DatabaseExport virtual DatabaseResultSPtr DoExecuteSelect() = 0;

		/** Clean query.
		*/
		DatabaseExport virtual void DoCleanup() = 0;

	protected:
		//! Array of parameters (addition order).
		DatabaseParameterPtrArray _arrayParams;
		//! Map of parameters by pointers.
		std::map< const void *, CDatabaseParameter const * > _mapParamsByPointer;
		//! Request text.
		String _query;
		//! Database connection.
		DatabaseConnectionWPtr _connection;
		//! Tells if the statement is initialised 
		bool _initialised = false;

		/** Inform parent statement from the value changes
		*/
		struct SValueUpdater
			: public CDatabaseParameter::SValueUpdater
		{
			/** Constructor
			@param query
				The parent statement
			*/
			DatabaseExport SValueUpdater( CDatabaseStatement * stmt );

			//!@copydoc CDatabaseParameter::SValueUpdater
			DatabaseExport virtual void Update( const CDatabaseParameter & value );

			//! The parent statement
			CDatabaseStatement * _stmt;
		};
	};
}
END_NAMESPACE_DATABASE

#include "DatabaseStatement.inl"

#endif // ___DATABASE_STATEMENT_H___
