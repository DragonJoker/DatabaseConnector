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
		@param[in] connection
			The database connection.
		@param[in] infos
			Valued object informations.
		*/
		DatabaseExport CDatabaseField( DatabaseConnectionSPtr connection, DatabaseValuedObjectInfosSPtr infos );

		/** Destructor.
		*/
		DatabaseExport virtual ~CDatabaseField();

		/** Tells if the field is null.
		@return
			The null status.
		*/
		inline bool IsNull() const;

		/** Get field value.
		@remarks
			If field type is incompatible with the value type, an exception will be thrown.
		@return
			Field value.
		*/
		template< typename T > inline T GetValue() const;

		/** Get field value.
		@remarks
			If field type is incompatible with the value type, an exception will be thrown.
		@param[out] value
			Field value.
		*/
		template< typename T > inline void GetValue( T & value ) const;

		/** Get field value as a std::optional.
		@remarks
			If field type is incompatible with the value type, an exception will be thrown.
		@return
			Field value, std::none if the field is null.
		*/
		template< typename T > inline CDatabaseNullable< T > GetValueOpt() const;

		/** Get field value as a std::optional.
		@remarks
			If field type is incompatible with the value type, an exception will be thrown.
		@param[out] value
			Field value, std::none if the field is null.
		*/
		template< typename T > inline void GetValueOpt( CDatabaseNullable< T > & value ) const;

		/** Get field value.
		@remarks
			If field type is incompatible with the value type, The behaviour is undefined.
		@return
			Field value.
		*/
		template< typename T > inline T GetValueFast() const;

		/** Get field value.
		@remarks
			If field type is incompatible with the value type, The behaviour is undefined.
		@param[out] value
			Field value.
		*/
		template< typename T > inline void GetValueFast( T & value ) const;

		/** Get field value as a std::optional.
		@remarks
			If field type is incompatible with the value type, The behaviour is undefined.
		@return
			Field value, std::none if the field is null.
		*/
		template< typename T > inline CDatabaseNullable< T > GetValueOptFast() const;

		/** Get field value as a std::optional.
		@remarks
			If field type is incompatible with the value type, The behaviour is undefined.
		@param[out] value
			Field value, std::none if the field is null.
		*/
		template< typename T > inline void GetValueOptFast( CDatabaseNullable< T > & value ) const;
	};
}
END_NAMESPACE_DATABASE

#include "DatabaseField.inl"

#endif //__DATABASE_FIELD_H___
