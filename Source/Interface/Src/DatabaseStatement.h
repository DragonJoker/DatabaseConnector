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
	class DatabaseExport CDatabaseStatement
	{
	public:
		/** Constructor.
		@param[in] connection
		    Database connection.
		@param[in] query
		    Request text.
		*/
		CDatabaseStatement( DatabaseConnectionPtr connection, const String & query );

		/** Destructor.
		*/
		virtual ~CDatabaseStatement();

		/** Initialize query.
		@remarks
		    The statement *MUST* be initialized, *AFTER* all parameters have been created.
		@return
		    Error code.
		*/
		virtual EErrorType Initialize() = 0;

		/** Execute a query that has no result set.
		@param[out] result
		    Error code.
		@return
		    Results.
		*/
		virtual bool ExecuteUpdate( EErrorType * result = NULL ) = 0;

		/** Execute query that has a result set.
		@param[out] result
		    Error code.
		@return
		    Results.
		*/
		virtual DatabaseResultPtr ExecuteSelect( EErrorType * result = NULL ) = 0;

		/** Clean query.
		*/
		virtual void Cleanup() = 0;

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
		virtual DatabaseParameterPtr CreateParameter( const String & name, EFieldType fieldType, EParameterType parameterType ) = 0;

		/** Create a statement parameter for variable-sized parameter (with limits)
		@param[in] name
		    Parameter name.
		@param[in] fieldType
		    Field type.
		@param[in] limits
		    Filed limits.
		@param[in] parameterType
		    Parameter type.
		@return
		    Created statement parameter.
		*/
		virtual DatabaseParameterPtr CreateParameter( const String & name, EFieldType fieldType, uint32_t limits, EParameterType parameterType ) = 0;

		/** Get parameter type.
		@param[in] index
		    Parameter index.
		@return
		    Parameter type.
		*/
		EFieldType GetParameterType( uint32_t index );

		/** Set parameter value to NULL.
		@param[in] index
		    Parameter index.
		*/
		void SetParameterNull( uint32_t index );

		/** Set parameter value to NULL.
		@param[in] name
		    Parameter name.
		*/
		void SetParameterNull( const String & name );

		/** Set parameter value from another parameter.
		@param[in] index
		    Parameter index.
		@param[in] parameter
		    The parameter.
		*/
		void SetParameterValue( uint32_t index, DatabaseParameterPtr parameter );

		/** Set parameter value from another parameter.
		@param[in] name
		    Parameter name.
		@param[in] parameter
		    The parameter.
		*/
		void SetParameterValue( const String & name, DatabaseParameterPtr parameter );

		/** Set parameter value.
		@param[in] index
		    Parameter index.
		@param[in] value
		    Parameter value.
		*/
		template <typename T> void SetParameterValue( uint32_t index, const T & value );

		/** Set parameter value.
		@param[in] name
		    Parameter name.
		@param[in] value
		    Parameter value.
		*/
		template <typename T> void SetParameterValue( const String & name, const T & value );

		/** Get parameter value.
		@param[in] index
		    Parameter index.
		@return
		    Parameter value.
		*/
		template <typename T> T const & GetParameterValue( uint32_t index );

		/** Get parameter value.
		@param[in] name
		    Parameter name.
		@return
		    Parameter value.
		*/
		template <typename T> T const & GetParameterValue( const String & name );

		/** Get parameter value.
		@param[in] index
		    Parameter index.
		@param[out] value
		    Parameter value.
		*/
		template <typename T> void GetParameterValue( uint32_t index, T & value );

		/** Get parameter value.
		@param[in] name
		    Parameter name.
		@param[out] value
		    Parameter value.
		*/
		template <typename T> void GetParameterValue( const String & name, T & value );

	protected:

		/** Add parameter to query.
		@param[in] parameter
		    Parameter to add.
		@return
		    true if addition succeeds, false otherwise.
		*/
		bool DoAddParameter( DatabaseParameterPtr parameter );

	protected:
		/// Array of parameters (addition order).
		DatabaseParameterPtrArray _arrayParams;
		/// Map of parameters by pointers.
		std::map< void *, DatabaseParameterPtr > _mapParamsByPointer;
		/// Request text.
		String _query;
		/// Database connection.
		DatabaseConnectionPtr _connection;

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

			///@copydoc CDatabaseParameter::SValueUpdater
			DatabaseExport virtual void Update( DatabaseParameterPtr value );

private:
			//! The parent statement
			CDatabaseStatement * _stmt;
		};
	};
}
END_NAMESPACE_DATABASE

#include "DatabaseStatement.inl"

#endif // ___DATABASE_STATEMENT_H___
