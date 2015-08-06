/************************************************************************//**
* @file DatabaseParameteredObject.inl
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

#include "DatabaseLogger.h"

#include "DatabaseParameter.h"

BEGIN_NAMESPACE_DATABASE
{
	uint16_t CDatabaseParameteredObject::GetParametersCount() const
	{
		return uint16_t( _parameters.size() );
	}

	template< typename T >
	void CDatabaseParameteredObject::SetParameterValue( uint32_t index, const T & value )
	{
		GetParameter( index )->SetValue( value );
	}

	template< typename T >
	void CDatabaseParameteredObject::SetParameterValue( const String & name, const T & value )
	{
		GetParameter( name )->SetValue( value );
	}

	template< typename T >
	void CDatabaseParameteredObject::SetParameterValueFast( uint32_t index, const T & value )
	{
		GetParameter( index )->SetValueFast( value );
	}

	template< typename T >
	void CDatabaseParameteredObject::SetParameterValueFast( const String & name, const T & value )
	{
		GetParameter( name )->SetValueFast( value );
	}

	const DatabaseParameterPtrArray & CDatabaseParameteredObject::DoGetParameters() const
	{
		return _parameters;
	}
}
END_NAMESPACE_DATABASE
