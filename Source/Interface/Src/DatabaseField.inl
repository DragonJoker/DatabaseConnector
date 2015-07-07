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

BEGIN_NAMESPACE_DATABASE
{
	static const String DATABASE_FIELD_TYPE_ERROR = STR( "Unknown field type" );
	static const String DATABASE_FIELD_NULL_VALUE_ERROR = STR( "Field value is null: " );

	template <typename T>
	inline CDatabaseField::CDatabaseField( DatabaseFieldInfosPtr infos, const T & value )
		:   _infos( infos )
		,   _isNull( false )
	{
		_value = SDatabaseFieldValueInitializer< T >()( value, _infos );
	}

	inline CDatabaseValueBase * CDatabaseField::GetFieldValue() const
	{
		return _value;
	}

	template <typename T>
	inline T CDatabaseField::GetValue() const
	{
		T tReturn;
		GetValue( tReturn );
		return tReturn;
	}

	template <typename T>
	inline void CDatabaseField::GetValue( T & value ) const
	{
		if ( !_value->GetPtrValue() )
		{
			String errMsg = DATABASE_FIELD_NULL_VALUE_ERROR + this->GetName();
			CLogger::LogError( errMsg );
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, DATABASE_FIELD_NULL_VALUE_ERROR );
		}

		DoGetValue( value );
	}

	template <typename T>
	inline boost::optional< T > CDatabaseField::GetValueOpt() const
	{
		boost::optional< T > tReturn;
		GetValueOpt( tReturn );
		return tReturn;
	}

	template <typename T>
	inline void CDatabaseField::GetValueOpt( boost::optional< T > & value ) const
	{
		if ( IsNull() )
		{
			value = boost::none;
		}
		else
		{
			if ( !_value->GetPtrValue() )
			{
				String errMsg = DATABASE_FIELD_NULL_VALUE_ERROR + this->GetName();
				CLogger::LogError( errMsg );
				DB_EXCEPT( EDatabaseExceptionCodes_FieldError, DATABASE_FIELD_NULL_VALUE_ERROR );
			}

			T val;
			DoGetValue( val );
			value = val;
		}
	}

	template <typename T>
	inline T CDatabaseField::GetValueFast() const
	{
		T tReturn;
		GetValueFast( tReturn );
		return tReturn;
	}

	template <typename T>
	inline void CDatabaseField::GetValueFast( T & value ) const
	{
		if ( !_value->GetPtrValue() )
		{
			String errMsg = DATABASE_FIELD_NULL_VALUE_ERROR + this->GetName();
			CLogger::LogError( errMsg );
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, DATABASE_FIELD_NULL_VALUE_ERROR );
		}

		DoGetValueFast( value );
	}

	template <typename T>
	inline boost::optional< T > CDatabaseField::GetValueOptFast() const
	{
		boost::optional< T > tReturn;
		GetValueOptFast( tReturn );
		return tReturn;
	}

	template <typename T>
	inline void CDatabaseField::GetValueOptFast( boost::optional< T > & value ) const
	{
		if ( IsNull() )
		{
			value = boost::none;
		}
		else
		{
			if ( !_value->GetPtrValue() )
			{
				String errMsg = DATABASE_FIELD_NULL_VALUE_ERROR + this->GetName();
				CLogger::LogError( errMsg );
				DB_EXCEPT( EDatabaseExceptionCodes_FieldError, DATABASE_FIELD_NULL_VALUE_ERROR );
			}

			T val;
			DoGetValueFast( val );
			value = val;
		}
	}

	template <typename T>
	inline void CDatabaseField::SetValue( const T & value )
	{
		DoSetValue( value );
	}

	template <typename T>
	inline void CDatabaseField::SetValueFast( const T & value )
	{
		DoSetValueFast( value );
	}
}
END_NAMESPACE_DATABASE
