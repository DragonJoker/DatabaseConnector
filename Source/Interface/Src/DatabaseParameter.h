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
		@param[in] name
			Parameter name.
		@param[in] index
			Parameter index.
		@param[in] fieldType
			Field type.
		@param[in] parameterType
			Parameter type (in, inout, out).
		@param[in] updater
			The parent updater
		*/
		DatabaseExport CDatabaseParameter( DatabaseConnectionSPtr connection, const String & name, unsigned short index, EFieldType fieldType, EParameterType parameterType, std::unique_ptr< SValueUpdater > updater );

		/** Constructor.
		@param[in] connection
			Database connection.
		@param[in] name
			Parameter name.
		@param[in] index
			Parameter index.
		@param[in] fieldType
			Field type.
		@param[in] limits
			Filed limits (for VARCHAR, etc.)
		@param[in] parameterType
			Parameter type (in, inout, out).
		@param[in] updater
			The parent updater
		*/
		DatabaseExport CDatabaseParameter( DatabaseConnectionSPtr connection, const String & name, unsigned short index, EFieldType fieldType, uint32_t limits, EParameterType parameterType, std::unique_ptr< SValueUpdater > updater );

		/** Constructor.
		@param[in] connection
			Database connection.
		@param[in] name
			Parameter name.
		@param[in] index
			Parameter index.
		@param[in] fieldType
			Field type.
		@param[in] precision
			Field precision and scale (for FIXED_POINT)
		@param[in] parameterType
			Parameter type (in, inout, out).
		@param[in] updater
			The parent updater
		*/
		DatabaseExport CDatabaseParameter( DatabaseConnectionSPtr connection, const String & name, unsigned short index, EFieldType fieldType, const std::pair< uint32_t, uint32_t > & precision, EParameterType parameterType, std::unique_ptr< SValueUpdater > updater );

		/** Desctructor.
		*/
		DatabaseExport virtual ~CDatabaseParameter();

		/** Get field type.
		@return
			Field type.
		*/
		DatabaseExport virtual EFieldType GetType() const;

		/** Get name.
		@return
			The name.
		*/
		DatabaseExport virtual const String & GetName() const;

		/** Get limits.
		@return
			The limits.
		@remarks
			The reference is here to be able to pass this method to function wanting pointer.
		*/
		DatabaseExport virtual const uint32_t & GetLimits() const;

		/** Get precision.
		@return
			The precision.
		*/
		DatabaseExport virtual const std::pair< uint32_t, uint32_t > & GetPrecision() const;

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

		/** Set parameter value from a field.
		@param[in] field
			The field.
		@remarks
			If field type is different than the value type, the value is ignored.
		*/
		DatabaseExport void SetValue( const CDatabaseField & field );

		/** Set parameter value from another parameter.
		@param[in] field
			The field.
		@remarks
			If field type is different than the value type, the value is ignored.
		*/
		DatabaseExport void SetValue( const CDatabaseParameter & field );

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
			If field type is different than the value type, the value is ignored.
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
			If field type is different than the value type, the value is ignored.
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
