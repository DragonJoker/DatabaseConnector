/************************************************************************//**
* @file DatabaseInt24.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 7/12/2015 7:51 PM
*
*
* @brief int24_t and uint24_t classes declaration.
*
* @details 24 bits integer classes
*
***************************************************************************/

#include "DatabasePch.h"

#include "DatabaseInt24.h"

#pragma warning( disable:4351 )

BEGIN_NAMESPACE_DATABASE
{
	int24_t::int24_t()
		: _value()
	{
	}

	int24_t::int24_t( const int8_t input )
		: _value()
	{
		_value[0] = ( ( uint8_t * )&input )[0];
	}

	int24_t::int24_t( const uint8_t input )
		: _value()
	{
		_value[0] = ( ( uint8_t * )&input )[0];
	}

	int24_t::int24_t( const int16_t input )
		: _value()
	{
		_value[0] = ( ( uint8_t * )&input )[0];
		_value[1] = ( ( uint8_t * )&input )[1];
		_value[2] = ( ( uint8_t * )&input )[2];
	}

	int24_t::int24_t( const uint16_t input )
		: _value()
	{
		_value[0] = ( ( uint8_t * )&input )[0];
		_value[1] = ( ( uint8_t * )&input )[1];
		_value[2] = ( ( uint8_t * )&input )[2];
	}

	int24_t::int24_t( const uint24_t input )
		: _value()
	{
		_value[0] = ( ( uint8_t * )&input )[0];
		_value[1] = ( ( uint8_t * )&input )[1];
		_value[2] = ( ( uint8_t * )&input )[2];
	}

	int24_t::int24_t( const int32_t input )
		: _value()
	{
		_value[0] = ( ( uint8_t * )&input )[0];
		_value[1] = ( ( uint8_t * )&input )[1];
		_value[2] = ( ( uint8_t * )&input )[2];
	}

	int24_t::int24_t( const uint32_t input )
		: _value()
	{
		_value[0] = ( ( uint8_t * )&input )[0];
		_value[1] = ( ( uint8_t * )&input )[1];
		_value[2] = ( ( uint8_t * )&input )[2];
	}

	int24_t::int24_t( const int64_t input )
		: _value()
	{
		_value[0] = ( ( uint8_t * )&input )[0];
		_value[1] = ( ( uint8_t * )&input )[1];
		_value[2] = ( ( uint8_t * )&input )[2];
	}

	int24_t::int24_t( const uint64_t input )
		: _value()
	{
		_value[0] = ( ( uint8_t * )&input )[0];
		_value[1] = ( ( uint8_t * )&input )[1];
		_value[2] = ( ( uint8_t * )&input )[2];
	}

	int24_t::int24_t( const float input )
		: _value()
	{
		int32_t in = int32_t( input );
		_value[0] = ( ( uint8_t * )&in )[0];
		_value[1] = ( ( uint8_t * )&in )[1];
		_value[2] = ( ( uint8_t * )&in )[2];
	}

	int24_t::int24_t( const double input )
		: _value()
	{
		int32_t in = int32_t( input );
		_value[0] = ( ( uint8_t * )&in )[0];
		_value[1] = ( ( uint8_t * )&in )[1];
		_value[2] = ( ( uint8_t * )&in )[2];
	}

	int24_t::int24_t( const long double input )
		: _value()
	{
		int32_t in = int32_t( input );
		_value[0] = ( ( uint8_t * )&in )[0];
		_value[1] = ( ( uint8_t * )&in )[1];
		_value[2] = ( ( uint8_t * )&in )[2];
	}
	
	uint24_t::uint24_t()
		: _value()
	{
	}

	uint24_t::uint24_t( const int8_t input )
		: _value()
	{
		_value[0] = ( ( uint8_t * )&input )[0];
	}

	uint24_t::uint24_t( const uint8_t input )
		: _value()
	{
		_value[0] = ( ( uint8_t * )&input )[0];
	}

	uint24_t::uint24_t( const int16_t input )
		: _value()
	{
		_value[0] = ( ( uint8_t * )&input )[0];
		_value[1] = ( ( uint8_t * )&input )[1];
		_value[2] = ( ( uint8_t * )&input )[2];
	}

	uint24_t::uint24_t( const uint16_t input )
		: _value()
	{
		_value[0] = ( ( uint8_t * )&input )[0];
		_value[1] = ( ( uint8_t * )&input )[1];
		_value[2] = ( ( uint8_t * )&input )[2];
	}

	uint24_t::uint24_t( const int24_t input )
		: _value()
	{
		_value[0] = ( ( uint8_t * )&input )[0];
		_value[1] = ( ( uint8_t * )&input )[1];
		_value[2] = ( ( uint8_t * )&input )[2];
	}

	uint24_t::uint24_t( const int32_t input )
		: _value()
	{
		_value[0] = ( ( uint8_t * )&input )[0];
		_value[1] = ( ( uint8_t * )&input )[1];
		_value[2] = ( ( uint8_t * )&input )[2];
	}

	uint24_t::uint24_t( const uint32_t input )
		: _value()
	{
		_value[0] = ( ( uint8_t * )&input )[0];
		_value[1] = ( ( uint8_t * )&input )[1];
		_value[2] = ( ( uint8_t * )&input )[2];
	}

	uint24_t::uint24_t( const int64_t input )
		: _value()
	{
		_value[0] = ( ( uint8_t * )&input )[0];
		_value[1] = ( ( uint8_t * )&input )[1];
		_value[2] = ( ( uint8_t * )&input )[2];
	}

	uint24_t::uint24_t( const uint64_t input )
		: _value()
	{
		_value[0] = ( ( uint8_t * )&input )[0];
		_value[1] = ( ( uint8_t * )&input )[1];
		_value[2] = ( ( uint8_t * )&input )[2];
	}

	uint24_t::uint24_t( const float input )
		: _value()
	{
		int32_t in = int32_t( input );
		_value[0] = ( ( uint8_t * )&in )[0];
		_value[1] = ( ( uint8_t * )&in )[1];
		_value[2] = ( ( uint8_t * )&in )[2];
	}

	uint24_t::uint24_t( const double input )
		: _value()
	{
		int32_t in = int32_t( input );
		_value[0] = ( ( uint8_t * )&in )[0];
		_value[1] = ( ( uint8_t * )&in )[1];
		_value[2] = ( ( uint8_t * )&in )[2];
	}

	uint24_t::uint24_t( const long double input )
		: _value()
	{
		int32_t in = int32_t( input );
		_value[0] = ( ( uint8_t * )&in )[0];
		_value[1] = ( ( uint8_t * )&in )[1];
		_value[2] = ( ( uint8_t * )&in )[2];
	}
}
END_NAMESPACE_DATABASE
