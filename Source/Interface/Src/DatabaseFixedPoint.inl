/************************************************************************//**
* @file DatabaseFixedPoint.inl
* @author Sylvain Doremus
* @version 1.0
* @date 7/12/2015 7:51 PM
*
*
* @brief CFixedPoint class declaration.
*
* @details Describes a fixed point numeric value with decimals and scale.
*
 ***************************************************************************/

#include "DatabaseException.h"

BEGIN_NAMESPACE_DATABASE
{
	namespace
	{
		static const String ERROR_DB_DIVIDE_BY_ZERO = STR( "Division by zero" );
		static const String ERROR_DB_PRECISION_OVERFLOW = STR( "Precision overflow" );
		static const String ERROR_DB_INTEGER_OVERFLOW = STR( "Integer overflow" );
		static const String ERROR_DB_INTEGER_UNDERFLOW = STR( "Integer underflow" );

		template< typename T > class SFixedPointOperations;

		static double GetDecimalMult( uint8_t decimals )
		{
			return double( pow( 10, decimals ) );
		}

		template< typename T, typename Enable = void > struct ValuePrecision;

		template< typename T >
		struct ValuePrecision< T, typename std::enable_if< std::is_signed< T >::value >::type >
		{
			static int16_t Get( T value )
			{
				int16_t precision = 0;
				value = abs( value );

				while ( value >= 1 )
				{
					++precision;
					value /= 10;
				}

				return precision;
			}
		};

		template< typename T >
		struct ValuePrecision< T, typename std::enable_if< std::is_unsigned< T >::value >::type >
		{
			static int16_t Get( T value )
			{
				int16_t precision = 0;

				while ( value >= 1 )
				{
					++precision;
					value /= 10;
				}

				return precision;
			}
		};

		template<>
		struct ValuePrecision< int256_t, void >
		{
			static int16_t Get( int256_t value )
			{
				int16_t precision = 0;
				value = abs( value );

				while ( value >= 1 )
				{
					++precision;
					value /= 10;
				}

				return precision;
			}
		};

		template<>
		struct ValuePrecision< uint256_t, void >
		{
			static int16_t Get( uint256_t value )
			{
				int16_t precision = 0;

				while ( value >= 1 )
				{
					++precision;
					value /= 10;
				}

				return precision;
			}
		};

		template<>
		class SFixedPointOperations< CFixedPoint >
		{
		public:
			static int64_t Add( CFixedPoint const & lhs, CFixedPoint const & rhs )
			{
				int256_t lhr = DoExpandValue( lhs, rhs );
				int256_t rhr = DoExpandValue( rhs, lhs );
				int64_t result;

				if ( lhs.IsSigned() )
				{
					int256_t add = lhr + rhr;

					if ( rhs.GetDecimals() > lhs.GetDecimals() )
					{
						if ( ValuePrecision< int256_t >::Get( add ) - ( rhs.GetDecimals() - lhs.GetDecimals() ) > lhs.GetPrecision() )
						{
							DB_EXCEPT( EDatabaseExceptionCodes_ArithmeticError, ERROR_DB_PRECISION_OVERFLOW );
						}
					}
					else if ( ValuePrecision< int256_t >::Get( add ) > lhs.GetPrecision() )
					{
						DB_EXCEPT( EDatabaseExceptionCodes_ArithmeticError, ERROR_DB_PRECISION_OVERFLOW );
					}

					result = DoShrinkValue( add, lhs, rhs );
				}
				else
				{
					uint256_t add = uint256_t( lhr ) + uint256_t( rhr );

					if ( rhs.GetDecimals() > lhs.GetDecimals() )
					{
						if ( ValuePrecision< uint256_t >::Get( add ) - ( rhs.GetDecimals() - lhs.GetDecimals() ) > lhs.GetPrecision() )
						{
							DB_EXCEPT( EDatabaseExceptionCodes_ArithmeticError, ERROR_DB_PRECISION_OVERFLOW );
						}
					}
					else if ( ValuePrecision< uint256_t >::Get( add ) > lhs.GetPrecision() )
					{
						DB_EXCEPT( EDatabaseExceptionCodes_ArithmeticError, ERROR_DB_PRECISION_OVERFLOW );
					}

					result = DoShrinkValue( add, lhs, rhs );
				}

				return result;
			}

			static int64_t Subtract( CFixedPoint const & lhs, CFixedPoint const & rhs )
			{
				int256_t lhr = DoExpandValue( lhs, rhs );
				int256_t rhr = DoExpandValue( rhs, lhs );
				int64_t result;

				if ( lhs.IsSigned() )
				{
					int256_t sub = lhr - rhr;

					if ( rhs.GetDecimals() > lhs.GetDecimals() )
					{
						if ( ValuePrecision< int256_t >::Get( sub ) - ( rhs.GetDecimals() - lhs.GetDecimals() ) > lhs.GetPrecision() )
						{
							DB_EXCEPT( EDatabaseExceptionCodes_ArithmeticError, ERROR_DB_PRECISION_OVERFLOW );
						}
					}
					else if ( ValuePrecision< int256_t >::Get( sub ) > lhs.GetPrecision() )
					{
						DB_EXCEPT( EDatabaseExceptionCodes_ArithmeticError, ERROR_DB_PRECISION_OVERFLOW );
					}

					result = DoShrinkValue( sub, lhs, rhs );
				}
				else
				{
					uint256_t sub = uint256_t( lhr ) - uint256_t( rhr );

					if ( rhs.GetDecimals() > lhs.GetDecimals() )
					{
						if ( ValuePrecision< uint256_t >::Get( sub ) - ( rhs.GetDecimals() - lhs.GetDecimals() ) > lhs.GetPrecision() )
						{
							DB_EXCEPT( EDatabaseExceptionCodes_ArithmeticError, ERROR_DB_PRECISION_OVERFLOW );
						}
					}
					else if ( ValuePrecision< uint256_t >::Get( sub ) > lhs.GetPrecision() )
					{
						DB_EXCEPT( EDatabaseExceptionCodes_ArithmeticError, ERROR_DB_PRECISION_OVERFLOW );
					}

					result = DoShrinkValue( sub, lhs, rhs );
				}

				return result;
			}

			static int64_t Multiply( CFixedPoint const & lhs, CFixedPoint const & rhs )
			{
				int256_t lhr = DoExpandValue( lhs, rhs );
				int256_t rhr = DoExpandValue( rhs, lhs );
				int64_t result;

				if ( lhs.IsSigned() )
				{
					int256_t mul = ( lhs.GetRawValue() * rhr ) / CFixedPoint::Get10Pow256( lhs.GetDecimals() );

					if ( rhs.GetDecimals() > lhs.GetDecimals() )
					{
						if ( ValuePrecision< int256_t >::Get( mul ) - ( rhs.GetDecimals() - lhs.GetDecimals() ) > lhs.GetPrecision() )
						{
							DB_EXCEPT( EDatabaseExceptionCodes_ArithmeticError, ERROR_DB_PRECISION_OVERFLOW );
						}
					}
					else if ( ValuePrecision< int256_t >::Get( mul ) > lhs.GetPrecision() )
					{
						DB_EXCEPT( EDatabaseExceptionCodes_ArithmeticError, ERROR_DB_PRECISION_OVERFLOW );
					}

					result = DoShrinkValue( mul, lhs, rhs );
				}
				else
				{
					uint256_t mul = ( uint64_t( lhs.GetRawValue() ) * uint256_t( rhr ) ) / uint256_t( CFixedPoint::Get10Pow256( lhs.GetDecimals() ) );

					if ( rhs.GetDecimals() > lhs.GetDecimals() )
					{
						if ( ValuePrecision< uint256_t >::Get( mul ) - ( rhs.GetDecimals() - lhs.GetDecimals() ) > lhs.GetPrecision() )
						{
							DB_EXCEPT( EDatabaseExceptionCodes_ArithmeticError, ERROR_DB_PRECISION_OVERFLOW );
						}
					}
					else if ( ValuePrecision< uint256_t >::Get( mul ) > lhs.GetPrecision() )
					{
						DB_EXCEPT( EDatabaseExceptionCodes_ArithmeticError, ERROR_DB_PRECISION_OVERFLOW );
					}

					result = DoShrinkValue( mul, lhs, rhs );
				}

				return result;
			}

			static int64_t Divide( CFixedPoint const & lhs, CFixedPoint const & rhs )
			{
				int256_t lhr = DoExpandValue( lhs, rhs );
				int256_t rhr = DoExpandValue( rhs, lhs );

				if ( rhr == 0 )
				{
					DB_EXCEPT( EDatabaseExceptionCodes_ArithmeticError, ERROR_DB_DIVIDE_BY_ZERO );
				}

				if ( lhs.IsSigned() )
				{
					lhr = int256_t( lhr * CFixedPoint::Get10Pow256( lhs.GetDecimals() ) ) / rhr;
				}
				else
				{
					lhr = uint256_t( lhr * uint256_t( CFixedPoint::Get10Pow256( lhs.GetDecimals() ) ) ) / uint256_t( rhr );
				}

				return DoShrinkValue( lhr, lhs, rhs );
			}

		private:
			static int256_t DoExpandValue( CFixedPoint const & lhs, CFixedPoint const & rhs )
			{
				int256_t result = lhs.GetRawValue();

				if ( rhs.GetDecimals() > lhs.GetDecimals() )
				{
					result *= CFixedPoint::Get10Pow256( rhs.GetDecimals() - lhs.GetDecimals() );
				}

				return result;
			}

			static int64_t DoShrinkValue( int256_t value, CFixedPoint const & lhs, CFixedPoint const & rhs )
			{
				int64_t result;

				if ( rhs.GetDecimals() > lhs.GetDecimals() )
				{
					value /= CFixedPoint::Get10Pow256( rhs.GetDecimals() - lhs.GetDecimals() );
				}

				result = int64_t( value );

				if ( result != value )
				{
					DB_EXCEPT( EDatabaseExceptionCodes_ArithmeticError, ERROR_DB_INTEGER_OVERFLOW );
				}

				return result;
			}

			static int64_t DoShrinkValue( uint256_t value, CFixedPoint const & lhs, CFixedPoint const & rhs )
			{
				if ( rhs.GetDecimals() > lhs.GetDecimals() )
				{
					value /= uint256_t( CFixedPoint::Get10Pow256( rhs.GetDecimals() - lhs.GetDecimals() ) );
				}

				uint64_t tmp = uint64_t( value );

				if ( tmp != value )
				{
					DB_EXCEPT( EDatabaseExceptionCodes_ArithmeticError, ERROR_DB_INTEGER_OVERFLOW );
				}

				return int64_t( tmp );
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
		return int64_t( _value / CFixedPoint::Get10Pow256( _decimals ) );
	}

	inline uint64_t CFixedPoint::ToUInt64()const
	{
		return uint64_t( ToInt64() );
	}

	inline float CFixedPoint::ToFloat()const
	{
		return float( ToDouble() );
	}

	inline double CFixedPoint::ToDouble()const
	{
		return _value / GetDecimalMult( _decimals );
	}

	inline uint8_t CFixedPoint::GetDecimals()const
	{
		return _decimals;
	}

	inline uint8_t CFixedPoint::GetPrecision()const
	{
		return _precision;
	}

	inline bool CFixedPoint::IsSigned()const
	{
		return _signed;
	}

	inline const int64_t & CFixedPoint::GetRawValue()const
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

	inline int8_t CFixedPoint::GetMinPrecision()
	{
		return int8_t( 0 );
	}

	inline int8_t CFixedPoint::GetMaxPrecision()
	{
		return int8_t( 20 );
	}

	inline uint8_t CFixedPoint::GetValuePrecision( float value )
	{
		return uint8_t( ValuePrecision< float >::Get( value ) );
	}

	inline uint16_t CFixedPoint::GetValuePrecision( double value )
	{
		return uint16_t( ValuePrecision< double >::Get( value ) );
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
