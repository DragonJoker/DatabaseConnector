/************************************************************************//**
* @file DatabaseRow.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/24/2014 8:37:01 AM
*
*
* @brief CDatabaseRow class declaration.
*
* @details Describes a row of the result set.
*
***************************************************************************/

#ifndef ___DATABASE_ROW_H___
#define ___DATABASE_ROW_H___

#include "DatabasePrerequisites.h"

#include "DatabaseField.h"

BEGIN_NAMESPACE_DATABASE
{
	/** Describes a row of the result set.
	*/
	class CDatabaseRow
	{

	public:
		/** Constructor.
		*/
		DatabaseExport CDatabaseRow();

		/** Destructor.
			*/
		DatabaseExport virtual ~CDatabaseRow();

		/** Add a field to the row.
		@param[in] field
			New field.
			*/
		DatabaseExport void AddField( DatabaseFieldSPtr field );

		/** Get field by name.
		@param[in] name
			Field name.
		@return
			Field.
		*/
		DatabaseExport DatabaseFieldSPtr GetField( const String & name );

		/** Get field by index.
		@param[in] index
			Field index.
		@return
			Field.
		*/
		DatabaseExport DatabaseFieldSPtr GetField( uint32_t index );

		/** Get value
		@param[in] index
			Field index.
		@return
			Value.
		*/
		template< typename T > inline T Get( uint32_t index );

		/** Get value
		@param[in] name
			Field name.
		@return
			Value.
		*/
		template< typename T > inline T Get( const String & name );

		/** Get value
		@param[in] index
			Field index.
		@param[out] value
			Value.
		*/
		template< typename T > inline void Get( uint32_t index, T & value );

		/** Get value
		@param[in] name
			Field name.
		@param[out] value
			Value.
		*/
		template< typename T > inline void Get( const String & name, T & value );

		/** Get value
		@param[in] index
			Field index.
		@return
			Value.
		*/
		template< typename T > inline CDatabaseNullable< T > GetOpt( uint32_t index );

		/** Get value
		@param[in] name
			Field name.
		@return
			Value.
		*/
		template< typename T > inline CDatabaseNullable< T > GetOpt( const String & name );

		/** Get value
		@param[in] index
			Field index.
		@param[out] value
			Value.
		*/
		template< typename T > inline void GetOpt( uint32_t index, CDatabaseNullable< T > & value );

		/** Get value
		@param[in] name
			Field name.
		@param[out] value
			Value.
		*/
		template< typename T > inline void GetOpt( const String & name, CDatabaseNullable< T > & value );

		/** Get value
		@param[in] index
			Field index.
		@param[out] value
			Value.
		*/
		template< typename T > inline void GetFast( uint32_t index, T & value );

		/** Get value
		@param[in] name
			Field name.
		@param[out] value
			Value.
		*/
		template< typename T > inline void GetFast( const String & name, T & value );

		/** Get value
		@param[in] index
			Field index.
		@param[out] value
			Value.
		*/
		template< typename T > inline void GetOptFast( uint32_t index, CDatabaseNullable< T > & value );

		/** Get value
		@param[in] name
			Field name.
		@param[out] value
			Value.
		*/
		template< typename T > inline void GetOptFast( const String & name, CDatabaseNullable< T > & value );

		/** Get default value.
		@return
			Default value.
		*/
		template< typename T > inline T GetDefault();

	protected:
		//! Array of fields.
		DatabaseFieldPtrArray _arrayFields;
	};
}
END_NAMESPACE_DATABASE

#include "DatabaseRow.inl"

#endif // ___DATABASE_ROW_H___
