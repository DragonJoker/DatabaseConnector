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
	{
	}

	CFixedPoint::CFixedPoint( int8_t value )
	{
	}

	CFixedPoint::CFixedPoint( uint8_t value )
	{
	}

	CFixedPoint::CFixedPoint( int16_t value )
	{
	}

	CFixedPoint::CFixedPoint( uint16_t value )
	{
	}

	CFixedPoint::CFixedPoint( int32_t value )
	{
	}

	CFixedPoint::CFixedPoint( uint32_t value )
	{
	}

	CFixedPoint::CFixedPoint( int64_t value )
	{
	}

	CFixedPoint::CFixedPoint( uint64_t value )
	{
	}

	CFixedPoint::CFixedPoint( float value )
	{
	}

	CFixedPoint::CFixedPoint( double value )
	{
	}

	CFixedPoint::CFixedPoint( long double value )
	{
	}

	CFixedPoint::CFixedPoint( const String & value, uint8_t precision, uint8_t scale )
		: _value( value )
		, _precision( precision )
		, _scale( scale )
	{
		if ( _precision == -1 )
		{
			_precision = value.size();
			size_t index = value.find( STR( '.' ) );

			if ( index != String::npos )
			{
				_precision--;
			}

			_scale = _precision - ( index + 1 );
		}
	}

	CFixedPoint::~CFixedPoint()
	{
	}

	float CFixedPoint::ToFloat()const
	{
		return 0.0f;
	}

	double CFixedPoint::ToDouble()const
	{
		return 0.0;
	}

	long double CFixedPoint::ToLongDouble()const
	{
		return 0.0;
	}

	String const & CFixedPoint::ToString()const
	{
		return _value;
	}

	uint8_t CFixedPoint::GetPrecision()const
	{
		return _precision;
	}

	uint8_t CFixedPoint::GetScale()const
	{
		return _scale;
	}

	bool CFixedPoint::IsSigned()const
	{
		return _signed;
	}

	bool operator ==( const CFixedPoint & lhs, const CFixedPoint & rhs )
	{
		return lhs.GetPrecision() == rhs.GetPrecision()
			&& lhs.GetScale() == rhs.GetScale()
			&& lhs.IsSigned() == rhs.IsSigned()
			&& lhs.ToString() == rhs.ToString();
	}

	bool operator !=( const CFixedPoint & lhs, const CFixedPoint & rhs )
	{
		return lhs.GetPrecision() != rhs.GetPrecision()
			|| lhs.GetScale() != rhs.GetScale()
			|| lhs.IsSigned() != rhs.IsSigned()
			|| lhs.ToString() != rhs.ToString();
	}

	std::ostream & operator <<( std::ostream & stream, const CFixedPoint & value )
	{
		stream << "[" << value.GetPrecision() << "," << value.GetScale() << "] " << CStrUtils::ToStr( value.ToString() );
		return stream;
	}

	std::wostream & operator <<( std::wostream & stream, const CFixedPoint & value )
	{
		stream << L"[" << value.GetPrecision() << L"," << value.GetScale() << L"] " << CStrUtils::ToWStr( value.ToString() );
		return stream;
	}
}
