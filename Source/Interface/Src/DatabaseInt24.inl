/************************************************************************//**
* @file DatabaseInt24.inl
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

#include "DatabasePrerequisites.h"

BEGIN_NAMESPACE_DATABASE
{
	inline int24_t::operator uint24_t() const
	{
		return uint24_t( *this );
	}

	inline int24_t::operator int32_t() const
	{
		// Sign extend negative quantities
		if ( _value[2] & 0x80 )
		{
			return ( 0xff << 24 )
			| ( _value[2] << 16 )
			| ( _value[1] <<  8 )
			| ( _value[0] );
		}
		else
		{
			return ( _value[2] << 16 )
			| ( _value[1] <<  8 )
			| ( _value[0] );
		}
	}

	inline int24_t::operator int64_t() const
	{
		return int64_t( int32_t( *this ) );
	}

	inline int24_t::operator float() const
	{
		return float( int32_t( *this ) );
	}

	inline int24_t::operator double() const
	{
		return double( int32_t( *this ) );
	}

	inline int24_t::operator long double() const
	{
		return long double( int32_t( *this ) );
	}

	inline int24_t & int24_t::operator=( const int32_t input )
	{
		_value[0] = ( ( uint8_t * )&input )[0];
		_value[1] = ( ( uint8_t * )&input )[1];
		_value[2] = ( ( uint8_t * )&input )[2];

		return *this;
	}

	inline int24_t & int24_t::operator+=( const int24_t & val )
	{
		*this = int24_t( int32_t( *this ) + int32_t( val ) );
		return *this;
	}

	inline int24_t & int24_t::operator-=( const int24_t & val )
	{
		*this = int24_t( int32_t( *this ) - int32_t( val ) );
		return *this;
	}

	inline int24_t & int24_t::operator*=( const int24_t & val )
	{
		*this = int24_t( int32_t( *this ) * int32_t( val ) );
		return *this;
	}

	inline int24_t & int24_t::operator/=( const int24_t & val )
	{
		*this = int24_t( int32_t( *this ) / int32_t( val ) );
		return *this;
	}

	inline int24_t & int24_t::operator>>=( const int val )
	{
		*this = int24_t( int32_t( *this ) >> val );
		return *this;
	}

	inline int24_t & int24_t::operator<<=( const int val )
	{
		*this = int24_t( int32_t( *this ) << val );
		return *this;
	}

	inline int24_t::operator bool() const
	{
		return int32_t( *this ) != 0;
	}

	inline int24_t operator+( const int24_t & a, const int24_t & b )
	{
		int24_t res( a );
		res += b;
		return res;
	}

	inline int24_t operator-( const int24_t & a, const int24_t & b )
	{
		int24_t res( a );
		res -= b;
		return res;
	}

	inline int24_t operator*( const int24_t & a, const int24_t & b )
	{
		int24_t res( a );
		res *= b;
		return res;
	}

	inline int24_t operator/( const int24_t & a, const int24_t & b )
	{
		int24_t res( a );
		res /= b;
		return res;
	}

	inline int24_t operator>>( const int24_t & a, const int val )
	{
		int24_t res( a );
		res >>= val;
		return res;
	}

	inline int24_t operator<<( const int24_t & a, const int val )
	{
		int24_t res( a );
		res <<= val;
		return res;
	}

	inline bool operator==( const int24_t & a, const int24_t & b )
	{
		return int32_t( a ) == int32_t( b );
	}

	inline bool operator!=( const int24_t & a, const int24_t & b )
	{
		return int32_t( a ) != int32_t( b );
	}

	inline bool operator>=( const int24_t & a, const int24_t & b )
	{
		return int32_t( a ) >= int32_t( b );
	}

	inline bool operator<=( const int24_t & a, const int24_t & b )
	{
		return int32_t( a ) <= int32_t( b );
	}

	inline bool operator!( const int24_t & value )
	{
		return !int32_t( value );
	}

	inline int24_t operator-( const int24_t & value )
	{
		return int24_t( -int32_t( value ) );
	}

	inline uint24_t::operator int24_t() const
	{
		return int24_t( *this );
	}

	inline uint24_t::operator uint32_t() const
	{
		return ( _value[2] << 16 )
		| ( _value[1] <<  8 )
		| ( _value[0] );
	}

	inline uint24_t::operator uint64_t() const
	{
		return uint64_t( int32_t( *this ) );
	}

	inline uint24_t::operator float() const
	{
		return float( int32_t( *this ) );
	}

	inline uint24_t::operator double() const
	{
		return double( int32_t( *this ) );
	}

	inline uint24_t::operator long double() const
	{
		return long double( int32_t( *this ) );
	}

	inline uint24_t & uint24_t::operator=( const uint32_t input )
	{
		_value[0] = ( ( uint8_t * )&input )[0];
		_value[1] = ( ( uint8_t * )&input )[1];
		_value[2] = ( ( uint8_t * )&input )[2];

		return *this;
	}

	inline uint24_t & uint24_t::operator+=( const uint24_t & val )
	{
		*this = uint24_t( uint32_t( *this ) + uint32_t( val ) );
		return *this;
	}

	inline uint24_t & uint24_t::operator-=( const uint24_t & val )
	{
		*this = uint24_t( uint32_t( *this ) - uint32_t( val ) );
		return *this;
	}

	inline uint24_t & uint24_t::operator*=( const uint24_t & val )
	{
		*this = uint24_t( uint32_t( *this ) * uint32_t( val ) );
		return *this;
	}

	inline uint24_t & uint24_t::operator/=( const uint24_t & val )
	{
		*this = uint24_t( uint32_t( *this ) / uint32_t( val ) );
		return *this;
	}

	inline uint24_t & uint24_t::operator>>=( const int val )
	{
		*this = uint24_t( uint32_t( *this ) >> val );
		return *this;
	}

	inline uint24_t & uint24_t::operator<<=( const int val )
	{
		*this = uint24_t( uint32_t( *this ) << val );
		return *this;
	}

	inline uint24_t::operator bool() const
	{
		return uint32_t( *this ) != 0;
	}

	inline uint24_t operator+( const uint24_t & a, const uint24_t & b )
	{
		uint24_t res( a );
		res += b;
		return res;
	}

	inline uint24_t operator-( const uint24_t & a, const uint24_t & b )
	{
		uint24_t res( a );
		res -= b;
		return res;
	}

	inline uint24_t operator*( const uint24_t & a, const uint24_t & b )
	{
		uint24_t res( a );
		res *= b;
		return res;
	}

	inline uint24_t operator/( const uint24_t & a, const uint24_t & b )
	{
		uint24_t res( a );
		res /= b;
		return res;
	}

	inline uint24_t operator>>( const uint24_t & a, const int val )
	{
		uint24_t res( a );
		res >>= val;
		return res;
	}

	inline uint24_t operator<<( const uint24_t & a, const int val )
	{
		uint24_t res( a );
		res <<= val;
		return res;
	}

	inline bool operator==( const uint24_t & a, const uint24_t & b )
	{
		return uint32_t( a ) == uint32_t( b );
	}

	inline bool operator!=( const uint24_t & a, const uint24_t & b )
	{
		return uint32_t( a ) != uint32_t( b );
	}

	inline bool operator>=( const uint24_t & a, const uint24_t & b )
	{
		return uint32_t( a ) >= uint32_t( b );
	}

	inline bool operator<=( const uint24_t & a, const uint24_t & b )
	{
		return uint32_t( a ) <= uint32_t( b );
	}

	inline bool operator!( const uint24_t & value )
	{
		return !uint32_t( value );
	}
}
END_NAMESPACE_DATABASE
