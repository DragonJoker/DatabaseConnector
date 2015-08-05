/************************************************************************//**
* @file DatabaseParameter.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDatabaseParameter class declaration.
*
* @details Describes a parameter for a query or a statement.
*
***************************************************************************/

#ifndef ___DATABASE_PARAMETER_H___
#define ___DATABASE_PARAMETER_H___

#include "DatabaseValuedObject.h"

#include "EFieldType.h"
#include "EParameterType.h"
#include "DatabaseValue.h"

BEGIN_NAMESPACE_DATABASE
{
	/** Describes a parameter for a query or a statement.
	*/
	class CDatabaseParameter
		: public std::enable_shared_from_this< CDatabaseParameter >
		, public CDatabaseValuedObject
	{
	public:
		/** Structure used to inform parent that the value changed
		*/
		struct SValueUpdater
		{
			/** Updates the parent
			@param parameter
				The parameter
			*/
			DatabaseExport virtual void Update( const CDatabaseParameter & parameter ) = 0;
		};

	public:
		/** Constructor.
		@param[in] connection
			Database connection.
		@param[in] infos
			Valued object informations.
		@param[in] index
			Parameter index.
		@param[in] parameterType
			Parameter type (in, inout, out).
		@param[in] updater
			The parent updater
		*/
		DatabaseExport CDatabaseParameter( DatabaseConnectionSPtr connection, DatabaseValuedObjectInfosSPtr infos, unsigned short index, EParameterType parameterType, std::unique_ptr< SValueUpdater > updater );

		/** Desctructor.
		*/
		DatabaseExport virtual ~CDatabaseParameter();

		/** Get parameter index.
		@return
			Parameter index.
		@remarks
			The reference is here to be able to pass this method to function wanting pointer.
		*/
		DatabaseExport const unsigned short & GetIndex() const;

		/** Get parameter type.
		@return
			Parameter type.
		@remarks
			The reference is here to be able to pass this method to function wanting pointer.
			*/
		DatabaseExport const EParameterType & GetParamType() const;

		/** Set parameter value to NULL.
		*/
		DatabaseExport virtual void SetNull();

		/** Set parameter value from another valued object.
		@param[in] object
			The valued object.
		@remarks
			If field type is incompatible with the value type, an exception will be thrown.
		*/
		DatabaseExport void SetValue( const CDatabaseValuedObject & object );

		/** Set parameter value from another value.
		@param[in] type
			The value type
		@param[in] value
			The value.
		*/
		DatabaseExport void SetValue( EFieldType type, CDatabaseValueBase const & value );

		/** Set parameter value.
		@remarks
			If value is a std::string or std::wstring, the user *MUST* make sure it is encoded in UTF-8
		@param[in] value
			New parameter value.
		@remarks
			If field type is incompatible with the value type, an exception will be thrown.
		*/
		template< typename T > inline void SetValue( const T & value )
		{
			DoSetValue( value );
			_updater->Update( *this );
		}

		/** Set parameter value.
		@remarks
			If value is a std::string or std::wstring, the user *MUST* make sure it is encoded in UTF-8
		@param[in] value
			New parameter value.
		@remarks
			If field type is incompatible with the value type, The behaviour is undefined.
		*/
		template< typename T > inline void SetValueFast( const T & value )
		{
			DoSetValueFast( value );
			_updater->Update( *this );
		}

		/** Get parameter value.
		@return
			Field value.
		*/
		template< typename T > inline T GetValue() const
		{
			T val;
			DoGetValue( val );
			return val;
		}

		/** Get parameter value.
		@param[out] value
			Field value.
		*/
		template< typename T > inline void GetValue( T & value ) const
		{
			DoGetValue( value );
		}

	protected:
		//! Parameter name.
		String _name;
		//! The value type.
		EFieldType _fieldType;
		//! Precision (floating and fixed points), _precision.first is also used as field limit (char, varchar, ...)
		std::pair< uint32_t, uint32_t > _precision;
		//! Parameter type (in, out, inout).
		EParameterType _parameterType;
		//! Parameter index.
		unsigned short _index;
		//! Functor used to inform parent that the value was modified
		std::unique_ptr< SValueUpdater > _updater;
	};
}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_PARAMETER_H___
