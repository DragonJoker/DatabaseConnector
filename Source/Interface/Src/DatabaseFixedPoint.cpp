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
#include "DatabaseLogger.h"

BEGIN_NAMESPACE_DATABASE
{
	static const String ERROR_DB_INVALID_PRECISION = STR( "Invalid precision (should be: CFixedPoint::GetMinPrecision() <= precision <= CFixedPoint::GetMaxPrecision())" );
	static const String ERROR_DB_INVALID_DECIMALS = STR( "Invalid decimals (should be: 0 <= decimals < precision)" );

	CFixedPoint::CFixedPoint()
		: _value( 0 )
		, _decimals( 5 )
		, _precision( 10 )
		, _signed( true )
	{
	}

	CFixedPoint::CFixedPoint( int32_t value, uint8_t precision, uint8_t decimals )
		: _value( value )
		, _decimals( decimals )
		, _precision( precision )
		, _signed( true )
	{
		if ( ValuePrecision< int32_t >::Get( value ) > precision )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ArithmeticError, ERROR_DB_PRECISION_OVERFLOW );
		}

		DoAdjustValue();
	}

	CFixedPoint::CFixedPoint( uint32_t value, uint8_t precision, uint8_t decimals )
		: _value( value )
		, _decimals( decimals )
		, _precision( precision )
		, _signed( false )
	{
		if ( ValuePrecision< uint32_t >::Get( value ) > precision )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ArithmeticError, ERROR_DB_PRECISION_OVERFLOW );
		}

		DoAdjustValue();
	}

	CFixedPoint::CFixedPoint( int64_t value, uint8_t precision, uint8_t decimals )
		: _value( value )
		, _decimals( decimals )
		, _precision( precision )
		, _signed( true )
	{
		if ( ValuePrecision< int64_t >::Get( value ) > precision )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ArithmeticError, ERROR_DB_PRECISION_OVERFLOW );
		}

		DoAdjustValue();
	}

	CFixedPoint::CFixedPoint( uint64_t value, uint8_t precision, uint8_t decimals )
		: _value( value )
		, _decimals( decimals )
		, _precision( precision )
		, _signed( false )
	{
		if ( ValuePrecision< uint64_t >::Get( value ) > precision )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ArithmeticError, ERROR_DB_PRECISION_OVERFLOW );
		}

		DoAdjustValue();
	}

	CFixedPoint::CFixedPoint( float value, uint8_t precision, uint8_t decimals )
		: _decimals( decimals )
		, _precision( precision )
		, _signed( true )
	{
		if ( ValuePrecision< float >::Get( value ) > precision - decimals )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ArithmeticError, ERROR_DB_PRECISION_OVERFLOW );
		}

		_value = int64_t( value * GetDecimalMult( decimals ) );
		DoAdjustValue();
	}

	CFixedPoint::CFixedPoint( double value, uint8_t precision, uint8_t decimals )
		: _decimals( decimals )
		, _precision( precision )
		, _signed( true )
	{
		if ( ValuePrecision< double >::Get( value ) > precision - decimals )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ArithmeticError, ERROR_DB_PRECISION_OVERFLOW );
		}

		_value = int64_t( value * GetDecimalMult( decimals ) );
		DoAdjustValue();
	}

	CFixedPoint::CFixedPoint( String const & value, uint8_t precision, uint8_t decimals )
		: _decimals( decimals )
		, _precision( precision )
		, _signed( false )
	{
		String absval = value;
		int256_t val;

		if ( value.find( STR( '-' ) ) == 0 )
		{
			absval = value.substr( 1 );
			_signed = true;
		}

		size_t index = absval.find( STR( '.' ) );

		if ( index == String::npos )
		{
			if ( absval.size() > precision - decimals )
			{
				DB_EXCEPT( EDatabaseExceptionCodes_ArithmeticError, ERROR_DB_PRECISION_OVERFLOW );
			}

			val = int256_t( std::stoll( absval ) * Get10Pow256( _decimals ) );
		}
		else if ( index > precision )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ArithmeticError, ERROR_DB_PRECISION_OVERFLOW );
		}
		else if ( absval.size() - ( index + 1 ) < _decimals )
		{
			StringStream adjusted;
			adjusted.width( index + decimals );
			adjusted.fill( '0' );
			adjusted << std::left << String( absval.substr( 0, index ) + absval.substr( index + 1 ) );
			val = std::stoll( adjusted.str() );
		}
		else
		{
			val = std::stoll( absval.substr( 0, index ) + absval.substr( index + 1, _decimals ) );
		}

		if ( _signed )
		{
			val = -val;
		}

		_value = int64_t( val );
		DoAdjustValue();
	}

	CFixedPoint::~CFixedPoint()
	{
	}

	String CFixedPoint::ToString()const
	{
		String result;

		if ( IsSigned() && _value < 0 )
		{
			result = StringUtils::ToString( -int256_t( _value ) );
		}
		else
		{
			result = StringUtils::ToString( uint64_t( _value ) );
		}

		if ( _value && _decimals )
		{
			if ( result.size() <= _decimals )
			{
				while ( result.size() < _decimals )
				{
					result = "0" + result;
				}

				result = "0" + result;
			}

			result.insert( ( result.rbegin() + _decimals ).base(), '.' );
		}

		if ( IsSigned() && _value < 0 )
		{
			result = STR( "-" ) + result;
		}

		return result;
	}

	void Get10Pow256Rec( int32_t pow, int256_t & left, int256_t & right )
	{
		if ( pow > 1 )
		{
			Get10Pow256Rec( pow >> 1, left, left );
			Get10Pow256Rec( pow >> 1, right, right );
		}

		right *= ( pow & 0x1 ? 10 : 1 );
	}

	int256_t CFixedPoint::Get10Pow256( int32_t pow )
	{
		int256_t left = 1;
		int256_t right = 1;
		Get10Pow256Rec( pow, left, right );
		return left * right;
	}

	void CFixedPoint::DoAdjustValue()
	{
		if ( _precision )
		{
			if ( _precision > GetMaxPrecision() || _precision < GetMinPrecision() )
			{
				DB_EXCEPT( EDatabaseExceptionCodes_ArithmeticError, ERROR_DB_INVALID_PRECISION );
			}

			if ( _decimals >= _precision )
			{
				DB_EXCEPT( EDatabaseExceptionCodes_ArithmeticError, ERROR_DB_INVALID_DECIMALS );
			}
		}
		else if ( _value )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ArithmeticError, ERROR_DB_PRECISION_OVERFLOW );
		}
	}

	int64_t CFixedPoint::DoGetValueDecimals()const
	{
		int64_t remaining = _value;
		int64_t result = 0;
		uint8_t decimals = _decimals;

		while ( decimals-- )
		{
			result = ( result * 10 ) + ( remaining % 10 );
		}

		return result;
	}

	int64_t CFixedPoint::DoGetValueDecimals( uint8_t precision )const
	{
		int64_t decimals = DoGetValueDecimals();

		if ( precision > _decimals )
		{
			decimals *= int64_t( GetDecimalMult( precision - _decimals ) );
		}
		else if ( precision < _decimals )
		{
			decimals /= int64_t( GetDecimalMult( _decimals - precision ) );
		}

		return decimals;
	}

	bool operator ==( const CFixedPoint & lhs, const CFixedPoint & rhs )
	{
		bool ret = lhs.GetRawValue() == rhs.GetRawValue() && lhs.GetDecimals() == rhs.GetDecimals();

		if ( !ret )
		{
			CLogger::LogDebug( std::stringstream() << "lhs: " << lhs.ToInt64() << ", rhs: " << rhs.ToInt64() );
			ret = lhs.ToInt64() == rhs.ToInt64();

			if ( ret )
			{
				if ( lhs.GetDecimals() < rhs.GetDecimals() )
				{
					ret = lhs.DoGetValueDecimals( rhs.GetDecimals() ) == rhs.DoGetValueDecimals();
				}
				else if ( lhs.GetDecimals() > rhs.GetDecimals() )
				{
					ret = lhs.DoGetValueDecimals() == rhs.DoGetValueDecimals( lhs.GetDecimals() );
				}
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
		stream << "[" << int16_t( value.GetPrecision() ) << ", " << int16_t( value.GetDecimals() ) << ", " << value.GetRawValue() << "] " << value.ToString();
		return stream;
	}

	std::wostream & operator <<( std::wostream & stream, const CFixedPoint & value )
	{
		stream << L"[" << int16_t( value.GetPrecision() ) << L", " << int16_t( value.GetDecimals() ) << L", " << value.GetRawValue() << L"] " << StringUtils::ToWStr( value.ToString(), "ASCII" );
		return stream;
	}
}
