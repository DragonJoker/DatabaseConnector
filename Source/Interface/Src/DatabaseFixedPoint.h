/************************************************************************//**
* @file DatabaseFixedPoint.h
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

#ifndef ___DATABASE_FIXED_POINT_H___
#define ___DATABASE_FIXED_POINT_H___

#include "DatabasePrerequisites.h"

#include <boost/multiprecision/cpp_int.hpp>

BEGIN_NAMESPACE_DATABASE
{
	using boost::multiprecision::int256_t;
	using boost::multiprecision::uint256_t;

	/** Describes a fixed point numeric value with precision and scale
	*/
	class CFixedPoint
	{
	public:
		/** Default constructor
		*/
		DatabaseExport CFixedPoint();

		/** Constructor from int32_t
		@param[in] value
			The value
		@param[in] precision
			The precision (total digits count)
		@param[in] decimals
			The decimals (digits afet decimals separator)
		*/
		DatabaseExport CFixedPoint( int32_t value, uint8_t precision, uint8_t decimals );

		/** Constructor from uint32_t
		@param[in] value
			The value
		@param[in] precision
			The precision (total digits count)
		@param[in] decimals
			The decimals (digits afet decimals separator)
		*/
		DatabaseExport CFixedPoint( uint32_t value, uint8_t precision, uint8_t decimals );

		/** Constructor from int64_t
		@param[in] value
			The value
		@param[in] precision
			The precision (total digits count)
		@param[in] decimals
			The decimals (digits afet decimals separator)
		*/
		DatabaseExport CFixedPoint( int64_t value, uint8_t precision, uint8_t decimals );

		/** Constructor from uint64_t
		@param[in] value
			The value
		@param[in] precision
			The precision (total digits count)
		@param[in] decimals
			The decimals (digits afet decimals separator)
		*/
		DatabaseExport CFixedPoint( uint64_t value, uint8_t precision, uint8_t decimals );

		/** Constructor from float
		@param[in] value
			The value
		@param[in] precision
			The precision (total digits count)
		@param[in] decimals
			The decimals (digits afet decimals separator)
		*/
		DatabaseExport CFixedPoint( float value, uint8_t precision, uint8_t decimals );

		/** Constructor from double
		@param[in] value
			The value
		@param[in] precision
			The precision (total digits count)
		@param[in] decimals
			The decimals (digits afet decimals separator)
		*/
		DatabaseExport CFixedPoint( double value, uint8_t precision, uint8_t decimals );

		/** Constructor from String
		@param[in] value
			The value
		@param[in] precision
			The precision (total digits count)
		@param[in] decimals
			The decimals (digits afet decimals separator)
		*/
		DatabaseExport CFixedPoint( String const & value, uint8_t precision, uint8_t decimals );

		/** Destructor
		*/
		DatabaseExport ~CFixedPoint();

		/** Retrieves the number as a string
		@return
			The value as a string
		*/
		DatabaseExport String ToString()const;

		/** Retrieves the value as a signed 32 bits integer
		*/
		inline int32_t ToInt32()const;

		/** Retrieves the value as an unsigned 32 bits integer
		*/
		inline uint32_t ToUInt32()const;

		/** Retrieves the value as a signed 64 bits integer
		*/
		inline int64_t ToInt64()const;

		/** Retrieves the value as an unsigned 64 bits integer
		*/
		inline uint64_t ToUInt64()const;

		/** Retrieves the value as a simple precision floating point
		*/
		inline float ToFloat()const;

		/** Retrieves the value as a double precision floating point
		*/
		inline double ToDouble()const;

		/** Retrieves the precision
		@return
			The precision
		*/
		inline uint8_t GetPrecision()const;

		/** Retrieves the decimals
		@return
			The decimals
		*/
		inline uint8_t GetDecimals()const;

		/** Tells if the value is signed
		@return
			The status
		*/
		inline bool IsSigned()const;

		/** Retrieves the raw value
		*/
		inline const int64_t & GetRawValue()const;

		/** Multiplication assignment operator
		@param[in] rhs
			The multiplicator
		*/
		template< typename T >
		inline CFixedPoint & operator *=( const T & rhs );

		/** Division assignment operator
		@param[in] rhs
			The divisor
		*/
		template< typename T >
		inline CFixedPoint & operator /=( const T & rhs );

		/** Addition assignment operator
		@param[in] rhs
			The added
		*/
		template< typename T >
		inline CFixedPoint & operator +=( const T & rhs );

		/** Subtraction assignment operator
		@param[in] rhs
			The subtracted
		*/
		template< typename T >
		inline CFixedPoint & operator -=( const T & rhs );

		/** Retrieves the minimum supported precision
		@return
			The value
		*/
		static inline int8_t GetMinPrecision();

		/** Retrieves the maximum supported precision
		@return
			The value
		*/
		static inline int8_t GetMaxPrecision();

		/** Retrieves the precision needed for the given value
		@param[in] value
			The value
		@return
			The precision
		*/
		static inline uint8_t GetValuePrecision( float value );

		/** Retrieves the precision needed for the given value
		@param[in] value
			The value
		@return
			The precision
		*/
		static inline uint16_t GetValuePrecision( double value );

		/** Retrieves the wanted power of 10
		@param[in] pow
			The power
		@return
			10 ^ pow
		*/
		DatabaseExport static int256_t Get10Pow256( int32_t pow );

	private:
		/** Adjusts the value to the decimals, checks the precision
		@remarks
			Throws an exception if the value is over 10 ^ precision
		*/
		void DoAdjustValue();

		/** Retrieves the value decimals
		*/
		int64_t DoGetValueDecimals()const;

		/** Retrieves the value decimals, in given precision
		@remarks
			Will add zeros or remove decimals, to obtain the wanted precision
		@param precision
			The precision
		*/
		int64_t DoGetValueDecimals( uint8_t precision )const;

	private:
		//! The value precision (total digits count)
		uint8_t _precision;
		//! The value decimals (digits after decimals separator)
		uint8_t _decimals;
		//! The raw value
		int64_t _value;
		//! Tells if the value is signed or not
		bool _signed;

		friend DatabaseExport bool operator ==( const CFixedPoint &, const CFixedPoint & );
	};

	/** Multiplication operator
	@param lhs
		Left-hand side value.
	@param rhs
		Right-hand side value.
	@return
		The result
	*/
	template< typename T >
	inline CFixedPoint operator *( const CFixedPoint & lhs, const T & rhs );

	/** Division operator
	@param lhs
		Left-hand side value.
	@param rhs
		Right-hand side value.
	@return
		The result
	*/
	template< typename T >
	inline CFixedPoint operator /( const CFixedPoint & lhs, const T & rhs );

	/** Addition operator
	@param lhs
		Left-hand side value.
	@param rhs
		Right-hand side value.
	@return
		The result
	*/
	template< typename T >
	inline CFixedPoint operator +( const CFixedPoint & lhs, const T & rhs );

	/** Subtraction operator
	@param lhs
		Left-hand side value.
	@param rhs
		Right-hand side value.
	@return
		The result
	*/
	template< typename T >
	inline CFixedPoint operator -( const CFixedPoint & lhs, const T & rhs );

	/** Check equality of two fixed point numeric values.
	@param lhs
		Left-hand side value.
	@param rhs
		Right-hand side value.
	@return
		true if objects are equal.
	*/
	DatabaseExport bool operator ==( const CFixedPoint & lhs, const CFixedPoint & rhs );

	/** Check inequality of two times.
	@param lhs
		Left-hand side value.
	@param rhs
		Right-hand side value.
	@return
		true if objects are different.
	*/
	DatabaseExport bool operator !=( const CFixedPoint & lhs, const CFixedPoint & rhs );

	/** Stream operator
	@param stream
		The stream.
	@param value
		The value.
	@return
		The stream.
	*/
	DatabaseExport std::ostream & operator <<( std::ostream & stream, const CFixedPoint & value );

	/** Stream operator
	@param stream
		The stream.
	@param value
		The value.
	@return
		The stream.
	*/
	DatabaseExport std::wostream & operator <<( std::wostream & stream, const CFixedPoint & value );
}

#include "DatabaseFixedPoint.inl"

#endif
