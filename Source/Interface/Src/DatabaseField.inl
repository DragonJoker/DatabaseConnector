/************************************************************************//**
 * @file DatabaseField.inl
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 *
 *
 * @brief CDatabaseField class template functionalities.
 *
 * @details Describes a database field.
 *
 ***************************************************************************/

#include "DatabasePrerequisites.h" // Help doxygen

#include "DatabaseValue.h"
#include "DatabaseNullable.h"

BEGIN_NAMESPACE_DATABASE
{
	static const String DATABASE_FIELD_TYPE_ERROR = STR( "Unknown field type" );
	static const String DATABASE_FIELD_NULL_VALUE_ERROR = STR( "Field value is null: " );

	template< typename T >
	inline T CDatabaseField::GetValue() const
	{
		T tReturn;
		GetValue( tReturn );
		return tReturn;
	}

	template< typename T >
	inline void CDatabaseField::GetValue( T & value ) const
	{
		DoGetValue( value );
	}

	template< typename T >
	inline CDatabaseNullable< T > CDatabaseField::GetValueOpt() const
	{
		CDatabaseNullable< T > tReturn;
		GetValueOpt( tReturn );
		return tReturn;
	}

	template< typename T >
	inline void CDatabaseField::GetValueOpt( CDatabaseNullable< T > & value ) const
	{
		if ( GetObjectValue().IsNull() )
		{
			value = None;
		}
		else
		{
			T val;
			DoGetValue( val );
			value = val;
		}
	}

	template< typename T >
	inline T CDatabaseField::GetValueFast() const
	{
		T tReturn;
		GetValueFast( tReturn );
		return tReturn;
	}

	template< typename T >
	inline void CDatabaseField::GetValueFast( T & value ) const
	{
		DoGetValueFast( value );
	}

	template< typename T >
	inline CDatabaseNullable< T > CDatabaseField::GetValueOptFast() const
	{
		CDatabaseNullable< T > tReturn;
		GetValueOptFast( tReturn );
		return tReturn;
	}

	template< typename T >
	inline void CDatabaseField::GetValueOptFast( CDatabaseNullable< T > & value ) const
	{
		if ( GetObjectValue().IsNull() )
		{
			value = None;
		}
		else
		{
			T val;
			DoGetValueFast( val );
			value = val;
		}
	}
}
END_NAMESPACE_DATABASE
