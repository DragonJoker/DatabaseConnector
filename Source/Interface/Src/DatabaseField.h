/************************************************************************//**
* @file DatabaseField.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDatabaseField class declaration.
*
* @details Describes a database field.
*
***************************************************************************/

#ifndef ___DATABASE_FIELD_H___
#define ___DATABASE_FIELD_H___

#include "DatabaseValuedObject.h"

#include "DatabaseException.h"
#include "EFieldType.h"
#include "DatabaseLogger.h"

#include <boost/optional.hpp>

BEGIN_NAMESPACE_DATABASE
{
	/** Describes a database field.
	*/
	class CDatabaseField
		: public CDatabaseValuedObject
	{
	public:
		/** Constructor.
		@param[in] infos
			Field informations.
			*/
		DatabaseExport CDatabaseField( DatabaseFieldInfosPtr infos );

		/** Destructor.
		*/
		DatabaseExport virtual ~CDatabaseField();

		/** Get field type.
		@return
			Field type.
		*/
		DatabaseExport virtual EFieldType GetType() const;

		/** Get field name.
		@return
			Field name.
		*/
		DatabaseExport virtual const String & GetName() const;

		/** Get field limits.
		@return
			Field limits.
		*/
		DatabaseExport virtual const uint32_t & GetLimits() const;

		/** Get field limits.
		@return
			Field limits.
		*/
		DatabaseExport virtual const std::pair< uint32_t, uint32_t > & GetPrecision() const;

		/** Get field value.
		@return
			Field value.
		*/
		template< typename T > inline T GetValue() const;

		/** Get field value.
		@param[out] value
			Field value.
		*/
		template< typename T > inline void GetValue( T & value ) const;

		/** Get field value as a std::optional.
		@return
			Field value, std::none if the field is null.
		*/
		template< typename T > inline CDatabaseNullable< T > GetValueOpt() const;

		/** Get field value as a std::optional.
		@param[out] value
			Field value, std::none if the field is null.
		*/
		template< typename T > inline void GetValueOpt( CDatabaseNullable< T > & value ) const;

		/** Get field value.
		@remarks
			No check is made in this function, so the value type must match the field type
		@return
			Field value.
		*/
		template< typename T > inline T GetValueFast() const;

		/** Get field value.
		@remarks
			No check is made in this function, so the value type must match the field type
		@param[out] value
			Field value.
		*/
		template< typename T > inline void GetValueFast( T & value ) const;

		/** Get field value as a std::optional.
		@remarks
			No check is made in this function, so the value type must match the field type
		@return
			Field value, std::none if the field is null.
		*/
		template< typename T > inline CDatabaseNullable< T > GetValueOptFast() const;

		/** Get field value as a std::optional.
		@remarks
			No check is made in this function, so the value type must match the field type
		@param[out] value
			Field value, std::none if the field is null.
		*/
		template< typename T > inline void GetValueOptFast( CDatabaseNullable< T > & value ) const;

	protected:
		//! Field information.
		DatabaseFieldInfosPtr _infos;
	};
}
END_NAMESPACE_DATABASE

#include "DatabaseField.inl"

#endif //__DATABASE_FIELD_H___
