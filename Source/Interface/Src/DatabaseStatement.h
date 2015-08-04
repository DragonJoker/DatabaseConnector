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

#include "DatabaseParameteredObject.h"

#include "DatabaseParameter.h"
#include "EErrorType.h"

BEGIN_NAMESPACE_DATABASE
{
	/** Describes a database statement.
	*/
	class CDatabaseStatement
		: public CDatabaseParameteredObject
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
		DatabaseExport DatabaseParameterSPtr CreateParameter( const String & name, EFieldType fieldType, EParameterType parameterType = EParameterType_IN );

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
		DatabaseExport DatabaseParameterSPtr CreateParameter( const String & name, EFieldType fieldType, uint32_t limits, EParameterType parameterType = EParameterType_IN );

		/** Create a query parameter for variable-sized parameter (with limits)
		@param[in] name
			Parameter name.
		@param[in] fieldType
			Field type.
		@param[in] precision
			Field precision and scale.
		@param[in] parameterType
			Parameter type.
		@return
			Created query parameter.
		*/
		DatabaseExport DatabaseParameterSPtr CreateParameter( const String & name, EFieldType fieldType, const std::pair< uint32_t, uint32_t > & precision, EParameterType parameterType = EParameterType_IN );

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

	protected:
		//! Map of parameters by pointers.
		std::map< const void *, CDatabaseParameter const * > _mapParamsByPointer;
		//! Request text.
		String _query;
		//! Database connection.
		DatabaseConnectionWPtr _connection;

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
