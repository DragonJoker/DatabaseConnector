/************************************************************************//**
* @file DatabaseParameteredObject.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDatabaseParameteredObject class declaration.
*
* @details Describes the base class for queries and statements.
*
***************************************************************************/

#ifndef ___DATABASE_PARAMETERED_OBJECT_H___
#define ___DATABASE_PARAMETERED_OBJECT_H___

#include "DatabasePrerequisites.h"

#include "EErrorType.h"
#include "EFieldType.h"
#include "EParameterType.h"

BEGIN_NAMESPACE_DATABASE
{
	/** Describes the base class for queries and statements.
	*/
	class CDatabaseParameteredObject
	{
	public:
		/** Constructor.
		@param[in] connection
			Database connection.
		@param[in] query
			Request text.
		*/
		DatabaseExport CDatabaseParameteredObject();

		/** Destructor.
		*/
		DatabaseExport virtual ~CDatabaseParameteredObject();

		/** Initialise the object.
		@remarks
			The object *MUST* be initialised, *AFTER* all parameters have been created.
		@return
			Error code.
		*/
		DatabaseExport EErrorType Initialise();

		/** Clean the object.
		*/
		DatabaseExport void Cleanup();

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

		/** Retrieves parameter type.
		@param[in] index
			Parameter index.
		@return
			Parameter type.
		*/
		DatabaseExport EFieldType GetParameterType( uint32_t index );

		/** Retrieves parameter type.
		@param[in] name
			Parameter name.
		@return
			Parameter type.
		*/
		DatabaseExport EFieldType GetParameterType( const String & name );

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

		/** Set parameter value from another valued object.
		@param[in] index
			Parameter index.
		@param[in] object
			The object.
		*/
		DatabaseExport void SetParameterValue( uint32_t index, const CDatabaseValuedObject & object );

		/** Set parameter value from another valued object.
		@param[in] name
			Parameter name.
		@param[in] object
			The object.
		*/
		DatabaseExport void SetParameterValue( const String & name, const CDatabaseValuedObject & object );

		/** Retrieves the paramters count
		@return
			The count.
		*/
		inline uint16_t GetParametersCount() const;

		/** Set parameter value.
		@param[in] index
			Parameter index.
		@param[in] value
			Parameter value.
		*/
		template< typename T > inline void SetParameterValue( uint32_t index, const T & value );

		/** Set parameter value.
		@param[in] name
			Parameter name.
		@param[in] value
			Parameter value.
		*/
		template< typename T > inline void SetParameterValue( const String & name, const T & value );

		/** Set parameter value.
		@param[in] index
			Parameter index.
		@param[in] value
			Parameter value.
		*/
		template< typename T > inline void SetParameterValueFast( uint32_t index, const T & value );

		/** Set parameter value.
		@param[in] name
			Parameter name.
		@param[in] value
			Parameter value.
		*/
		template< typename T > inline void SetParameterValueFast( const String & name, const T & value );

	protected:
		/** Initialise the object.
		@return
			Error code.
		*/
		DatabaseExport virtual EErrorType DoInitialise() = 0;

		/** Clean the object.
		*/
		DatabaseExport virtual void DoCleanup() = 0;

		/** Creates a parameter, given it's valued object informations
		@param[in] infos
			Parameter informations.
		@param[in] parameterType
			Parameter in/out type.
		@return
			The created parameter
		*/
		DatabaseExport virtual DatabaseParameterSPtr DoCreateParameter( DatabaseValuedObjectInfosSPtr infos, EParameterType parameterType ) = 0;

		/** Add parameter to query.
		@param[in] parameter
			Parameter to add.
		@return
			The parameter if addition succeeds, nullptr otherwise.
		*/
		DatabaseExport DatabaseParameterSPtr DoAddParameter( DatabaseParameterSPtr parameter );

		/** Retrieves the parameters
		@return
			The parameters.
		*/
		inline const DatabaseParameterPtrArray & DoGetParameters() const;

	protected:
		//! Array of parameters (addition order).
		DatabaseParameterPtrArray _parameters;
		//! Tells if the object is initialised
		bool _initialised;
	};
}
END_NAMESPACE_DATABASE

#include "DatabaseParameteredObject.inl"

#endif // ___DATABASE_PARAMETERED_OBJECT_H___
