/************************************************************************//**
* @file DatabaseFixedPoint.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 7/12/2015 7:51 PM
*
*
* @brief CFixedPoint class declaration.
*
* @details Describes a fixed point numeric value with precision and scale.
*
***************************************************************************/
#include "DatabasePch.h"

#include "DatabaseFixedPoint.h"
#include "DatabaseStringUtils.h"

BEGIN_NAMESPACE_DATABASE
{
	CFixedPoint::CFixedPoint()
		: _value( 0 )
		, _precision( 8 )
		, _signed( true )
	{
	}

	CFixedPoint::CFixedPoint( int32_t value, uint8_t precision )
		: _value( value )
		, _precision( precision )
		, _signed( true )
	{
	}

	CFixedPoint::CFixedPoint( uint32_t value, uint8_t precision )
		: _value( value )
		, _precision( precision )
		, _signed( false )
	{
	}

	CFixedPoint::CFixedPoint( int64_t value, uint8_t precision )
		: _value( value )
		, _precision( precision )
		, _signed( true )
	{
	}

	CFixedPoint::CFixedPoint( uint64_t value, uint8_t precision )
		: _value( value )
		, _precision( precision )
		, _signed( false )
	{
	}

	CFixedPoint::CFixedPoint( float value, uint8_t precision )
		: _precision( precision )
		, _signed( false )
	{
		_value = int64_t( value * GetDecimalMult( _precision ) );
	}

	CFixedPoint::CFixedPoint( double value, uint8_t precision )
		: _precision( precision )
		, _signed( false )
	{
		_value = int64_t( value * GetDecimalMult( _precision ) );
	}

	CFixedPoint::CFixedPoint( long double value, uint8_t precision )
		: _precision( precision )
		, _signed( false )
	{
		_value = int64_t( value * GetDecimalMult( _precision ) );
	}

	CFixedPoint::CFixedPoint( String const & value, uint8_t precision )
		: _precision( precision )
	{
		size_t index = value.find( STR( '.' ) );

		if ( index == String::npos )
		{
			_value = CStrUtils::ToLongLong( value ) * GetDecimalMult( _precision );
		}
		else if ( value.size() - ( index + 1 ) < _precision )
		{
			StringStream adjusted;
			adjusted.width( index + precision );
			adjusted.fill( '0' );
			adjusted << std::left << String( value.substr( 0, index ) + value.substr( index + 1 ) );
			_value = CStrUtils::ToLongLong( value );
		}
		else
		{
			_value = CStrUtils::ToLongLong( value.substr( 0, index ) + value.substr( index + 1, _precision ) );
		}
	}

	CFixedPoint::~CFixedPoint()
	{
	}

	String CFixedPoint::ToString()const
	{
		String result;

		if ( IsSigned() )
		{
			result = CStrUtils::ToString( uint64_t( _value ) );
		}
		else
		{
			result = CStrUtils::ToString( _value );
		}

		if ( _value )
		{
			result.insert( ( result.rbegin() + _precision ).base(), '.' );
		}

		return result;
	}

	int64_t CFixedPoint::GetDecimals()const
	{
		return _value - ( ToInt64() * GetDecimalMult( _precision ) );
	}

	bool operator ==( const CFixedPoint & lhs, const CFixedPoint & rhs )
	{
		return lhs.ToInt64() == rhs.ToInt64()
			&& lhs.GetDecimals() == rhs.GetDecimals();
	}

	bool operator !=( const CFixedPoint & lhs, const CFixedPoint & rhs )
	{
		return lhs.ToInt64() != rhs.ToInt64()
			|| lhs.GetDecimals() != rhs.GetDecimals();
	}

	std::ostream & operator <<( std::ostream & stream, const CFixedPoint & value )
	{
		stream << "[" << int16_t( value.GetPrecision() ) << "] " << CStrUtils::ToStr( value.ToString() );
		return stream;
	}

	std::wostream & operator <<( std::wostream & stream, const CFixedPoint & value )
	{
		stream << L"[" << int16_t( value.GetPrecision() ) << L"] " << CStrUtils::ToWStr( value.ToString() );
		return stream;
	}
}
