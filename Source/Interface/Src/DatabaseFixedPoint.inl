/************************************************************************//**
 * @file DatabaseFixedPoint.inl
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

#include "DatabaseException.h"

#include <boost/multiprecision/cpp_int.hpp>

BEGIN_NAMESPACE_DATABASE
{
	namespace
	{
		static const String ERROR_DIVIDE_BY_ZERO = STR( "Division by zero" );
		static const String ERROR_INTEGER_OVERFLOW = STR( "Integer underflow" );
		static const String ERROR_INTEGER_UNDERFLOW = STR( "Integer underflow" );

		template< typename T > struct SFixedPointOperations;
		
		using boost::multiprecision::int512_t;
		using boost::multiprecision::uint512_t;

		static uint64_t GetDecimalMult( uint8_t precision )
		{
			return uint64_t( pow( 10, precision ) );
		}

		template<>
		struct SFixedPointOperations< CFixedPoint >
		{
			static int64_t Add( CFixedPoint const & lhs, CFixedPoint const & rhs )
			{
				int64_t rhr = AdjustPrecision( lhs, rhs );

				if ( lhs.IsSigned() )
				{
					if ( std::numeric_limits< int64_t >::max() - rhr >= lhs.GetRawValue() )
					{
						DB_EXCEPT( EDatabaseExceptionCodes_InternalError, ERROR_INTEGER_OVERFLOW );
					}
				}
				else
				{
					if ( std::numeric_limits< uint64_t >::max() - uint64_t( rhr ) >= uint64_t( lhs.GetRawValue() ) )
					{
						DB_EXCEPT( EDatabaseExceptionCodes_InternalError, ERROR_INTEGER_OVERFLOW );
					}
				}

				return lhs.GetRawValue() + rhr;
			}

			static int64_t Subtract( CFixedPoint const & lhs, CFixedPoint const & rhs )
			{
				int64_t rhr = AdjustPrecision( lhs, rhs );

				if ( lhs.IsSigned() )
				{
					if ( std::numeric_limits< int64_t >::lowest() + rhr <= lhs.GetRawValue() )
					{
						DB_EXCEPT( EDatabaseExceptionCodes_InternalError, ERROR_INTEGER_UNDERFLOW );
					}
				}
				else
				{
					if ( std::numeric_limits< uint64_t >::lowest() + uint64_t( rhr ) <= uint64_t( lhs.GetRawValue() ) )
					{
						DB_EXCEPT( EDatabaseExceptionCodes_InternalError, ERROR_INTEGER_UNDERFLOW );
					}
				}

				return lhs.GetRawValue() - rhr;
			}

			static int64_t Multiply( CFixedPoint const & lhs, CFixedPoint const & rhs )
			{
				int64_t rhr = AdjustPrecision( lhs, rhs );
				int64_t result;

				if ( lhs.IsSigned() )
				{
					uint512_t mult = ( uint64_t( lhs.GetRawValue() ) * uint64_t( rhr ) ) / GetDecimalMult( lhs.GetPrecision() );

					if ( uint64_t( mult ) != mult )
					{
						DB_EXCEPT( EDatabaseExceptionCodes_InternalError, ERROR_INTEGER_OVERFLOW );
					}

					result = int64_t( mult );
				}
				else
				{
					int512_t mult = ( lhs.GetRawValue() * rhr ) / GetDecimalMult( lhs.GetPrecision() );
					result = int64_t( mult );

					if ( result != mult )
					{
						DB_EXCEPT( EDatabaseExceptionCodes_InternalError, ERROR_INTEGER_OVERFLOW );
					}
				}

				return result;
			}

			static int64_t Divide( CFixedPoint const & lhs, CFixedPoint const & rhs )
			{
				int64_t rhr = AdjustPrecision( lhs, rhs );
				int64_t result;

				if ( rhs.ToInt64() == 0 )
				{
					DB_EXCEPT( EDatabaseExceptionCodes_InternalError, ERROR_DIVIDE_BY_ZERO );
				}

				if ( lhs.IsSigned() )
				{
					result = int64_t( int512_t( lhs.GetRawValue() * GetDecimalMult( lhs.GetPrecision() ) ) / rhr );
				}
				else
				{
					result = int64_t( uint512_t( lhs.GetRawValue() * GetDecimalMult( lhs.GetPrecision() ) ) / uint64_t( rhr ) );
				}

				return result;
			}

		private:
			static int64_t AdjustPrecision( CFixedPoint const & lhs, CFixedPoint const & rhs )
			{
				int64_t rhr = rhs.GetRawValue();

				if ( lhs.GetPrecision() < rhs.GetPrecision() )
				{
					rhr /= GetDecimalMult( lhs.GetPrecision() * rhs.GetPrecision() );
				}
				else if ( lhs.GetPrecision() > rhs.GetPrecision() )
				{
					rhr *= GetDecimalMult( rhs.GetPrecision() - lhs.GetPrecision() );
				}

				return rhr;
			}
		};

		template< typename T >
		struct SFixedPointOperations
		{
			static int64_t Add( CFixedPoint const & lhs, T const & rhs )
			{
				return SFixedPointOperations< CFixedPoint >::Add( lhs, CFixedPoint( rhs, lhs.GetPrecision() ) );
			}

			static int64_t Subtract( CFixedPoint const & lhs, T const & rhs )
			{
				return SFixedPointOperations< CFixedPoint >::Subtract( lhs, CFixedPoint( rhs, lhs.GetPrecision() ) );
			}

			static int64_t Multiply( CFixedPoint const & lhs, T const & rhs )
			{
				return SFixedPointOperations< CFixedPoint >::Multiply( lhs, CFixedPoint( rhs, lhs.GetPrecision() ) );
			}

			static int64_t Divide( CFixedPoint const & lhs, T const & rhs )
			{
				return SFixedPointOperations< CFixedPoint >::Divide( lhs, CFixedPoint( rhs, lhs.GetPrecision() ) );
			}
		};
	}

	inline int32_t CFixedPoint::ToInt32()const
	{
		return int32_t( ToInt64() );
	}

	inline uint32_t CFixedPoint::ToUInt32()const
	{
		return uint32_t( ToUInt64() );
	}

	inline int64_t CFixedPoint::ToInt64()const
	{
		return _value / GetDecimalMult( _precision );
	}

	inline uint64_t CFixedPoint::ToUInt64()const
	{
		return _value / GetDecimalMult( _precision );
	}

	inline float CFixedPoint::ToFloat()const
	{
		return float( _value ) / GetDecimalMult( _precision );
	}

	inline double CFixedPoint::ToDouble()const
	{
		return double( _value ) / GetDecimalMult( _precision );
	}

	inline long double CFixedPoint::ToLongDouble()const
	{
		return ( ( long double )_value ) / GetDecimalMult( _precision );
	}

	inline uint8_t CFixedPoint::GetPrecision()const
	{
		return _precision;
	}

	inline bool CFixedPoint::IsSigned()const
	{
		return _signed;
	}

	inline int64_t CFixedPoint::GetRawValue()const
	{
		return _value;
	}

	template< typename T >
	inline CFixedPoint & CFixedPoint::operator *=( const T & rhs )
	{
		_value = SFixedPointOperations< T >::Multiply( *this, rhs );
		return *this;
	}

	template< typename T >
	inline CFixedPoint & CFixedPoint::operator /=( const T & rhs )
	{
		_value = SFixedPointOperations< T >::Divide( *this, rhs );
		return *this;
	}

	template< typename T >
	inline CFixedPoint & CFixedPoint::operator +=( const T & rhs )
	{
		_value = SFixedPointOperations< T >::Add( *this, rhs );
		return *this;
	}

	template< typename T >
	inline CFixedPoint & CFixedPoint::operator -=( const T & rhs )
	{
		_value = SFixedPointOperations< T >::Subtract( *this, rhs );
		return *this;
	}

	template< typename T >
	inline CFixedPoint operator *( const CFixedPoint & lhs, const T & rhs )
	{
		CFixedPoint result( lhs );
		result *= rhs;
		return result;
	}

	template< typename T >
	inline CFixedPoint operator /( const CFixedPoint & lhs, const T & rhs )
	{
		CFixedPoint result( lhs );
		result /= rhs;
		return result;
	}

	template< typename T >
	inline CFixedPoint operator +( const CFixedPoint & lhs, const T & rhs )
	{
		CFixedPoint result( lhs );
		result += rhs;
		return result;
	}

	template< typename T >
	inline CFixedPoint operator -( const CFixedPoint & lhs, const T & rhs )
	{
		CFixedPoint result( lhs );
		result -= rhs;
		return result;
	}
}
