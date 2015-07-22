/************************************************************************//**
* @file DatabaseQuery.inl
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDatabaseQuery class definition.
*
* @details Describes CDatabaseQuery template methods.
*
***************************************************************************/

#include "DatabaseLogger.h"

BEGIN_NAMESPACE_DATABASE
{
	template< typename T >
	void CDatabaseQuery::SetParameterValue( uint32_t index, const T & value )
	{
		GetParameter( index )->SetValue( value );
	}

	template< typename T >
	void CDatabaseQuery::SetParameterValue( const String & name, const T & value )
	{
		GetParameter( index )->SetValue( value );
	}

	template< typename T >
	void CDatabaseQuery::SetParameterValueFast( uint32_t index, const T & value )
	{
		GetParameter( index )->SetValueFast( value );
	}

	template< typename T >
	void CDatabaseQuery::SetParameterValueFast( const String & name, const T & value )
	{
		GetParameter( index )->SetValueFast( value );
	}

	template <typename T>
	const T & CDatabaseQuery::GetOutputValue( uint32_t index )
	{
		return GetParameter( index )->GetValue< T >();
	}

	template <typename T>
	const T & CDatabaseQuery::GetOutputValue( const String & name )
	{
		return GetParameter( name )->GetValue< T >();
	}

	template <typename T>
	void CDatabaseQuery::GetOutputValue( uint32_t index, T & value )
	{
		GetParameter( index )->GetValue( value );
	}

	template <typename T>
	void CDatabaseQuery::GetOutputValue( const String & name, T & value )
	{
		GetParameter( name )->GetValue( value );
	}
}
END_NAMESPACE_DATABASE
