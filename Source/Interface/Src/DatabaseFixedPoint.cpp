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
	static const String ERROR_INVALID_DECIMALS = STR( "Invalid decimals (should be: 0 <= decimals <= precision)" );
	static const String ERROR_PRECISION_OVERFLOW = STR( "Precision overflow" );

	CFixedPoint::CFixedPoint()
		: _value( 0 )
		, _decimals( 8 )
		, _signed( true )
	{
	}

	CFixedPoint::CFixedPoint( int32_t value, uint8_t precision, uint8_t decimals )
		: _value( value )
		, _decimals( decimals )
		, _signed( true )
	{
		DoAdjustValue( precision );
	}

	CFixedPoint::CFixedPoint( uint32_t value, uint8_t precision, uint8_t decimals )
		: _value( value )
		, _decimals( decimals )
		, _signed( false )
	{
		DoAdjustValue( precision );
	}

	CFixedPoint::CFixedPoint( int64_t value, uint8_t precision, uint8_t decimals )
		: _value( value )
		, _decimals( decimals )
		, _signed( true )
	{
		DoAdjustValue( precision );
	}

	CFixedPoint::CFixedPoint( uint64_t value, uint8_t precision, uint8_t decimals )
		: _value( value )
		, _decimals( decimals )
		, _signed( false )
	{
		DoAdjustValue( precision );
	}

	CFixedPoint::CFixedPoint( float value, uint8_t precision, uint8_t decimals )
		: _decimals( decimals )
		, _signed( false )
	{
		_value = int64_t( value * GetDecimalMult( decimals ) );
		DoAdjustValue( precision );
	}

	CFixedPoint::CFixedPoint( double value, uint8_t precision, uint8_t decimals )
		: _decimals( decimals )
		, _signed( false )
	{
		_value = int64_t( value * GetDecimalMult( decimals ) );
		DoAdjustValue( precision );
	}

	CFixedPoint::CFixedPoint( long double value, uint8_t precision, uint8_t decimals )
		: _decimals( decimals )
		, _signed( false )
	{
		_value = int64_t( value * GetDecimalMult( decimals ) );
		DoAdjustValue( precision );
	}

	CFixedPoint::CFixedPoint( String const & value, uint8_t precision, uint8_t decimals )
		: _decimals( decimals )
	{
		size_t index = value.find( STR( '.' ) );

		if ( index == String::npos )
		{
			_value = std::stoll( value ) * GetDecimalMult( _decimals );
		}
		else if ( value.size() - ( index + 1 ) < _decimals )
		{
			StringStream adjusted;
			adjusted.width( index + decimals );
			adjusted.fill( '0' );
			adjusted << std::left << String( value.substr( 0, index ) + value.substr( index + 1 ) );
			_value = std::stoll( adjusted.str() );
		}
		else
		{
			_value = std::stoll( value.substr( 0, index ) + value.substr( index + 1, _decimals ) );
		}

		DoAdjustValue( precision );
	}

	CFixedPoint::~CFixedPoint()
	{
	}

	String CFixedPoint::ToString()const
	{
		String result;

		if ( IsSigned() )
		{
			result = StringUtils::ToString( _value );
		}
		else
		{
			result = StringUtils::ToString( _value );
		}

		if ( _value && _decimals )
		{
			if ( result.size() < _decimals )
			{
				while ( result.size() < _decimals )
				{
					result = "0" + result;
				}

				result = "0" + result;
			}

			result.insert( ( result.rbegin() + _decimals ).base(), '.' );
		}

		return result;
	}

	int64_t CFixedPoint::GetDecimals()const
	{
		return _value - ( ToInt64() * GetDecimalMult( _decimals ) );
	}

	int64_t CFixedPoint::GetDecimals( uint8_t precision )const
	{
		int64_t decimals = GetDecimals();

		if ( precision > _decimals )
		{
			decimals *= GetDecimalMult( precision - _decimals );
		}
		else if ( precision < _decimals )
		{
			decimals /= GetDecimalMult( _decimals - precision );
		}

		return decimals;
	}

	void CFixedPoint::DoAdjustValue( uint8_t precision )
	{
		if ( precision )
		{
			if ( _decimals > precision )
			{
				DB_EXCEPT( EDatabaseExceptionCodes_InternalError, ERROR_INVALID_DECIMALS );
			}

			if ( abs( _value ) > int64_t( GetDecimalMult( precision ) ) )
			{
				DB_EXCEPT( EDatabaseExceptionCodes_InternalError, ERROR_PRECISION_OVERFLOW );
			}
		}
	}

	bool operator ==( const CFixedPoint & lhs, const CFixedPoint & rhs )
	{
		bool ret = lhs.ToInt64() == rhs.ToInt64();

		if ( ret )
		{
			if ( lhs.GetPrecision() == rhs.GetPrecision() )
			{
				ret = lhs.GetDecimals() == rhs.GetDecimals();
			}
			else if ( lhs.GetPrecision() < rhs.GetPrecision() )
			{
				ret = lhs.GetDecimals( rhs.GetPrecision() ) == rhs.GetDecimals();
			}
			else
			{
				ret = lhs.GetDecimals() == rhs.GetDecimals( lhs.GetPrecision() );
			}
		}

		return ret;
	}

	bool operator !=( const CFixedPoint & lhs, const CFixedPoint & rhs )
	{
		return !( lhs == rhs );
	}

	std::ostream & operator <<( std::ostream & stream, const CFixedPoint & value )
	{
		stream << "[" << int16_t( value.GetPrecision() ) << "] " << StringUtils::ToStr( value.ToString() );
		return stream;
	}

	std::wostream & operator <<( std::wostream & stream, const CFixedPoint & value )
	{
		stream << L"[" << int16_t( value.GetPrecision() ) << L"] " << StringUtils::ToWStr( value.ToString() );
		return stream;
	}
}
