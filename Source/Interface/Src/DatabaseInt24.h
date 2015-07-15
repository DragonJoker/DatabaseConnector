/************************************************************************//**
* @file DatabaseInt24.h
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

#ifndef ___DATABASE_INT24_H___
#define ___DATABASE_INT24_H___

#include "DatabasePrerequisites.h"

BEGIN_NAMESPACE_DATABASE
{
	/** Signed 24 bits integer
	*/
	class int24_t
	{
	public:
		/** Constructor
		*/
		DatabaseExport int24_t();

		/** Constructor from int8_t
		@param rhs
			The value
		*/
		DatabaseExport explicit int24_t( const int8_t rhs );

		/** Constructor from uint8_t
		@param rhs
			The value
		*/
		DatabaseExport explicit int24_t( const uint8_t rhs );

		/** Constructor from int16_t
		@param rhs
			The value
		*/
		DatabaseExport explicit int24_t( const int16_t rhs );

		/** Constructor from uint16_t
		@param rhs
			The value
		*/
		DatabaseExport explicit int24_t( const uint16_t rhs );

		/** Constructor from uint24_t
		@param rhs
			The value
		*/
		DatabaseExport explicit int24_t( const uint24_t rhs );

		/** Constructor from int32_t
		@param rhs
			The value
		*/
		DatabaseExport explicit int24_t( const int32_t rhs );

		/** Constructor from uint32_t
		@param rhs
			The value
		*/
		DatabaseExport explicit int24_t( const uint32_t rhs );

		/** Constructor from int64_t
		@param rhs
			The value
		*/
		DatabaseExport explicit int24_t( const int64_t rhs );

		/** Constructor from uint64_t
		@param rhs
			The value
		*/
		DatabaseExport explicit int24_t( const uint64_t rhs );

		/** Constructor from float
		@param rhs
			The value
		*/
		DatabaseExport explicit int24_t( const float rhs );

		/** Constructor from double
		@param rhs
			The value
		*/
		DatabaseExport explicit int24_t( const double rhs );

		/** Constructor from long double
		@param rhs
			The value
		*/
		DatabaseExport explicit int24_t( const long double rhs );

		/** uint24_t conversion operator
		@return
			This value as uint24_t
		*/
		inline explicit operator uint24_t() const;

		/** uint32_t conversion operator
		@return
			This value as uint32_t
		*/
		inline explicit operator int32_t() const;

		/** uint32_t conversion operator
		@return
			This value as uint32_t
		*/
		inline explicit operator int64_t() const;

		/** float conversion operator
		@return
			This value as float
		*/
		inline explicit operator float() const;

		/** double conversion operator
		@return
			This value as double
		*/
		inline explicit operator double() const;

		/** long double conversion operator
		@return
			This value as long double
		*/
		inline explicit operator long double() const;

		/** Implicit conversion to bool
		*/
		inline operator bool() const;

		/** Assignment operator from an int32_t
		@param rhs
			The value
		@return
			A reference to this value
		*/
		inline int24_t & operator=( const int32_t rhs );

		/** Addition assignment operator
		@param rhs
			The right value
		@return
			A reference to this value
		*/
		inline int24_t & operator+=( const int24_t & rhs );

		/** Subtraction assignment operator
		@param rhs
			The right value
		@return
			A reference to this value
		*/
		inline int24_t & operator-=( const int24_t & rhs );

		/** Multiplication assignment operator
		@param rhs
			The right value
		@return
			A reference to this value
		*/
		inline int24_t & operator*=( const int24_t & rhs );

		/** Division assignment operator
		@param rhs
			The right value
		@return
			A reference to this value
		*/
		inline int24_t & operator/=( const int24_t & rhs );

		/** Right shift assignment operator
		@param rhs
			The right value
		@return
			A reference to this value
		*/
		inline int24_t & operator>>=( const int rhs );

		/** Left shift assignment operator
		@param rhs
			The right value
		@return
			A reference to this value
		*/
		inline int24_t & operator<<=( const int rhs );

	private:
		//! The value
		uint8_t _value[3];
	};

	/** Addition operator
	@param lhs, rhs
		The values
	@return
		The result
	*/
	inline int24_t operator+( const int24_t & lhs, const int24_t & rhs );

	/** Subtraction operator
	@param lhs, rhs
		The values
	@return
		The result
	*/
	inline int24_t operator-( const int24_t & lhs, const int24_t & rhs );

	/** Multiplication operator
	@param lhs, rhs
		The values
	@return
		The result
	*/
	inline int24_t operator*( const int24_t & lhs, const int24_t & rhs );

	/** Division operator
	@param lhs, rhs
		The values
	@return
		The result
	*/
	inline int24_t operator/( const int24_t & lhs, const int24_t & rhs );

	/** Right shift operator
	@param lhs, rhs
		The values
	@return
		The result
	*/
	inline int24_t operator>>( const int24_t & lhs, const int rhs );

	/** Left shift operator
	@param lhs, rhs
		The values
	@return
		The result
	*/
	inline int24_t operator<<( const int24_t & lhs, const int rhs );

	/** Comparison operator
	@param lhs, rhs
		The values
	@return
		The result
	*/
	inline bool operator==( const int24_t & lhs, const int24_t & rhs );

	/** Addition operator
	@param lhs, rhs
		The values
	@return
		The result
	*/
	inline bool operator!=( const int24_t & lhs, const int24_t & rhs );

	/** Comparison operator
	@param lhs, rhs
		The values
	@return
		The result
	*/
	inline bool operator>=( const int24_t & lhs, const int24_t & rhs );

	/** Comparison operator
	@param lhs, rhs
		The values
	@return
		The result
	*/
	inline bool operator<=( const int24_t & lhs, const int24_t & rhs );

	/** Test operator
	@param rhs
		The value to test
	@return
		The result
	*/
	inline bool operator!( const int24_t & rhs );

	/** Negation operator
	@param rhs
		The value to negate
	@return
		The result
	*/
	inline int24_t operator-( const int24_t & rhs );

	static const int24_t INT24_MAX = int24_t( 0x007FFFFF );
	static const int24_t INT24_MIN = int24_t( 0x00800000 );
	static const int24_t INT24_LOWEST = INT24_MIN;

	/** Unsigned 24 bits integer
	*/
	class uint24_t
	{
	public:
		/** Constructor
		*/
		DatabaseExport uint24_t();

		/** Constructor from int8_t
		@param rhs
			The value
		*/
		DatabaseExport explicit uint24_t( const int8_t rhs );

		/** Constructor from uint8_t
		@param rhs
			The value
		*/
		DatabaseExport explicit uint24_t( const uint8_t rhs );

		/** Constructor from int16_t
		@param rhs
			The value
		*/
		DatabaseExport explicit uint24_t( const int16_t rhs );

		/** Constructor from uint16_t
		@param rhs
			The value
		*/
		DatabaseExport explicit uint24_t( const uint16_t rhs );

		/** Constructor from uint24_t
		@param rhs
			The value
		*/
		DatabaseExport explicit uint24_t( const int24_t rhs );

		/** Constructor from int32_t
		@param rhs
			The value
		*/
		DatabaseExport explicit uint24_t( const int32_t rhs );

		/** Constructor from uint32_t
		@param rhs
			The value
		*/
		DatabaseExport explicit uint24_t( const uint32_t rhs );

		/** Constructor from int64_t
		@param rhs
			The value
		*/
		DatabaseExport explicit uint24_t( const int64_t rhs );

		/** Constructor from uint64_t
		@param rhs
			The value
		*/
		DatabaseExport explicit uint24_t( const uint64_t rhs );

		/** Constructor from float
		@param rhs
			The value
		*/
		DatabaseExport explicit uint24_t( const float rhs );

		/** Constructor from double
		@param rhs
			The value
		*/
		DatabaseExport explicit uint24_t( const double rhs );

		/** Constructor from long double
		@param rhs
			The value
		*/
		DatabaseExport explicit uint24_t( const long double rhs );

		/** int24_t Implicit conversion operator
		@return
			This value as int24_t
		*/
		inline explicit operator int24_t() const;

		/** uint32_t Implicit conversion operator
		@return
			This value as uint32_t
		*/
		inline explicit operator uint32_t() const;

		/** uint64_t Implicit conversion operator
		@return
			This value as uint64_t
		*/
		inline explicit operator uint64_t() const;

		/** float conversion operator
		@return
			This value as float
		*/
		inline explicit operator float() const;

		/** double conversion operator
		@return
			This value as double
		*/
		inline explicit operator double() const;

		/** long double conversion operator
		@return
			This value as long double
		*/
		inline explicit operator long double() const;

		/** Implicit conversion to bool
		*/
		inline operator bool() const;

		/** Assignment operator from an uint32_t
		@param rhs
			The value
		@return
			A reference to this value
		*/
		inline uint24_t & operator=( const uint32_t rhs );

		/** Addition assignment operator
		@param rhs
			The right value
		@return
			A reference to this value
		*/
		inline uint24_t & operator+=( const uint24_t & rhs );

		/** Subtraction assignment operator
		@param rhs
			The right value
		@return
			A reference to this value
		*/
		inline uint24_t & operator-=( const uint24_t & rhs );

		/** Multiplication assignment operator
		@param rhs
			The right value
		@return
			A reference to this value
		*/
		inline uint24_t & operator*=( const uint24_t & rhs );

		/** Division assignment operator
		@param rhs
			The right value
		@return
			A reference to this value
		*/
		inline uint24_t & operator/=( const uint24_t & rhs );

		/** Right shift assignment operator
		@param rhs
			The right value
		@return
			A reference to this value
		*/
		inline uint24_t & operator>>=( const int rhs );

		/** Left shift assignment operator
		@param rhs
			The right value
		@return
			A reference to this value
		*/
		inline uint24_t & operator<<=( const int rhs );

	private:
		//! The value
		uint8_t _value[3];
	};

	static const uint24_t UINT24_MAX = uint24_t( 0x00FFFFFF );
	static const uint24_t UINT24_MIN = uint24_t( 0 );
	static const uint24_t UINT24_LOWEST = UINT24_MIN;

	/** Addition operator
	@param lhs, rhs
		The values
	@return
		The result
	*/
	inline uint24_t operator+( const uint24_t & lhs, const uint24_t & rhs );

	/** Subtraction operator
	@param lhs, rhs
		The values
	@return
		The result
	*/
	inline uint24_t operator-( const uint24_t & lhs, const uint24_t & rhs );

	/** Multiplication operator
	@param lhs, rhs
		The values
	@return
		The result
	*/
	inline uint24_t operator*( const uint24_t & lhs, const uint24_t & rhs );

	/** Division operator
	@param lhs, rhs
		The values
	@return
		The result
	*/
	inline uint24_t operator/( const uint24_t & lhs, const uint24_t & rhs );

	/** Right shift operator
	@param lhs, rhs
		The values
	@return
		The result
	*/
	inline uint24_t operator>>( const uint24_t & lhs, const int rhs );

	/** Left shift operator
	@param lhs, rhs
		The values
	@return
		The result
	*/
	inline uint24_t operator<<( const uint24_t & lhs, const int rhs );

	/** Comparison operator
	@param lhs, rhs
		The values
	@return
		The result
	*/
	inline bool operator==( const uint24_t & lhs, const uint24_t & rhs );

	/** Addition operator
	@param lhs, rhs
		The values
	@return
		The result
	*/
	inline bool operator!=( const uint24_t & lhs, const uint24_t & rhs );

	/** Comparison operator
	@param lhs, rhs
		The values
	@return
		The result
	*/
	inline bool operator>=( const uint24_t & lhs, const uint24_t & rhs );

	/** Comparison operator
	@param lhs, rhs
		The values
	@return
		The result
	*/
	inline bool operator<=( const uint24_t & lhs, const uint24_t & rhs );

	/** Test operator
	@param rhs
		The value to test
	@return
		The result
	*/
	inline bool operator!( const uint24_t & rhs );
}
END_NAMESPACE_DATABASE

namespace std
{
	/** numeric_limits< int24_t >
	*/
	template<>
	class numeric_limits< NAMESPACE_DATABASE::int24_t >
	{
		// limits for type int
	public:
		typedef NAMESPACE_DATABASE::int24_t my_type;

		// return minimum value
		static my_type min() throw()
		{
			return NAMESPACE_DATABASE::INT24_MIN;
		}

		// return maximum value
		static my_type max() throw()
		{
			return NAMESPACE_DATABASE::INT24_MAX;
		}

		// return most negative value
		static my_type lowest() throw()
		{
			return min();
		}

		// return smallest effective increment from 1.0
		static my_type epsilon() throw()
		{
			return my_type( 0 );
		}

		// return largest rounding error
		static my_type round_error() throw()
		{
			return my_type( 0 );
		}

		// return minimum denormalized value
		static my_type denorm_min() throw()
		{
			return my_type( 0 );
		}

		// return positive infinity
		static my_type infinity() throw()
		{
			return my_type( 0 );
		}

		// return non-signaling NaN
		static my_type quiet_NaN() throw()
		{
			return my_type( 0 );
		}

		// return signaling NaN
		static my_type signaling_NaN() throw()
		{
			return my_type( 0 );
		}

		static const float_denorm_style has_denorm = denorm_absent;
		static const bool has_denorm_loss = false;
		static const bool has_infinity = false;
		static const bool has_quiet_NaN = false;
		static const bool has_signaling_NaN = false;
		static const bool is_iec559 = false;
		static const bool tinyness_before = false;
		static const bool traps = false;
		static const float_round_style round_style = round_toward_zero;

		static const int max_digits10 = 0;

		static const int max_exponent = 0;
		static const int max_exponent10 = 0;
		static const int min_exponent = 0;
		static const int min_exponent10 = 0;

		static const bool is_bounded = true;
		static const bool is_exact = true;
		static const bool is_integer = true;
		static const bool is_modulo = true;
		static const bool is_specialized = true;
		static const int radix = 2;

		static const bool is_signed = true;
		static const int digits = CHAR_BIT * sizeof( my_type ) - 1;
		static const int digits10 = ( CHAR_BIT * sizeof( my_type ) - 1 ) * 301L / 1000;
	};

	/** numeric_limits< uint24_t >
	*/
	template<>
	class numeric_limits< NAMESPACE_DATABASE::uint24_t >
	{
		// limits for type int
	public:
		typedef NAMESPACE_DATABASE::uint24_t my_type;

		// return minimum value
		static my_type min() throw()
		{
			return NAMESPACE_DATABASE::UINT24_MIN;
		}

		// return maximum value
		static my_type max() throw()
		{
			return NAMESPACE_DATABASE::UINT24_MAX;
		}

		// return most negative value
		static my_type lowest() throw()
		{
			return min();
		}

		// return smallest effective increment from 1.0
		static my_type epsilon() throw()
		{
			return my_type( 0 );
		}

		// return largest rounding error
		static my_type round_error() throw()
		{
			return my_type( 0 );
		}

		// return minimum denormalized value
		static my_type denorm_min() throw()
		{
			return my_type( 0 );
		}

		// return positive infinity
		static my_type infinity() throw()
		{
			return my_type( 0 );
		}

		// return non-signaling NaN
		static my_type quiet_NaN() throw()
		{
			return my_type( 0 );
		}

		// return signaling NaN
		static my_type signaling_NaN() throw()
		{
			return my_type( 0 );
		}

		static const float_denorm_style has_denorm = denorm_absent;
		static const bool has_denorm_loss = false;
		static const bool has_infinity = false;
		static const bool has_quiet_NaN = false;
		static const bool has_signaling_NaN = false;
		static const bool is_iec559 = false;
		static const bool tinyness_before = false;
		static const bool traps = false;
		static const float_round_style round_style = round_toward_zero;

		static const int max_digits10 = 0;

		static const int max_exponent = 0;
		static const int max_exponent10 = 0;
		static const int min_exponent = 0;
		static const int min_exponent10 = 0;

		static const bool is_bounded = true;
		static const bool is_exact = true;
		static const bool is_integer = true;
		static const bool is_modulo = true;
		static const bool is_specialized = true;
		static const int radix = 2;

		static const bool is_signed = false;
		static const int digits = CHAR_BIT * sizeof( my_type ) - 1;
		static const int digits10 = ( CHAR_BIT * sizeof( my_type ) - 1 ) * 301L / 1000;
	};
}

#include "DatabaseInt24.inl"

#endif
