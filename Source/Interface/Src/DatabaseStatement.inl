/************************************************************************//**
* @file DatabaseStatement.inl
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDatabaseStatement class definition.
*
* @details Describes a database statement.
*
***************************************************************************/

#include "DatabaseLogger.h"

BEGIN_NAMESPACE_DATABASE
{
	template< typename T >
	void CDatabaseStatement::SetParameterValue( uint32_t index, const T & value )
	{
		GetParameter( index )->SetValue( value );
	}

	template< typename T >
	void CDatabaseStatement::SetParameterValue( const String & name, const T & value )
	{
		GetParameter( name )->SetValue( value );
	}

	template< typename T >
	void CDatabaseStatement::SetParameterValueFast( uint32_t index, const T & value )
	{
		GetParameter( index )->SetValueFast( value );
	}

	template< typename T >
	void CDatabaseStatement::SetParameterValueFast( const String & name, const T & value )
	{
		GetParameter( name )->SetValueFast( value );
	}

	template <typename T>
	const T & CDatabaseStatement::GetOutputValue( uint32_t index )
	{
		return GetParameter( index )->GetValue< T >();
	}

	template <typename T>
	const T & CDatabaseStatement::GetOutputValue( const String & name )
	{
		return GetParameter( name )->GetValue< T >();
	}

	template <typename T>
	void CDatabaseStatement::GetOutputValue( uint32_t index, T & value )
	{
		GetParameter( index )->GetValue( value );
	}

	template <typename T>
	void CDatabaseStatement::GetOutputValue( const String & name, T & value )
	{
		GetParameter( name )->GetValue( value );
	}
}
END_NAMESPACE_DATABASE
