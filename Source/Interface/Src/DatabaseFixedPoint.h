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

BEGIN_NAMESPACE_DATABASE
{
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
			The precision
		*/
		DatabaseExport CFixedPoint( int32_t value, uint8_t precision );

		/** Constructor from uint32_t
		@param[in] value
			The value
		@param[in] precision
			The precision
		*/
		DatabaseExport CFixedPoint( uint32_t value, uint8_t precision );

		/** Constructor from int64_t
		@param[in] value
			The value
		@param[in] precision
			The precision
		*/
		DatabaseExport CFixedPoint( int64_t value, uint8_t precision );

		/** Constructor from uint64_t
		@param[in] value
			The value
		@param[in] precision
			The precision
		*/
		DatabaseExport CFixedPoint( uint64_t value, uint8_t precision );

		/** Constructor from float
		@param[in] value
			The value
		@param[in] precision
			The precision
		*/
		DatabaseExport CFixedPoint( float value, uint8_t precision );

		/** Constructor from double
		@param[in] value
			The value
		@param[in] precision
			The precision
		*/
		DatabaseExport CFixedPoint( double value, uint8_t precision );

		/** Constructor from double
		@param[in] value
			The value
		@param[in] precision
			The precision
		*/
		DatabaseExport CFixedPoint( long double value, uint8_t precision );

		/** Constructor from String
		@param[in] value
			The value
		@param[in] precision
			The precision
		*/
		DatabaseExport CFixedPoint( String const & value, uint8_t precision );

		/** Destructor
		*/
		DatabaseExport ~CFixedPoint();

		/** Retrieves the number as a string
		@return
			The value as a string
		*/
		DatabaseExport String ToString()const;

		/** Retrieves the value decimals
		*/
		DatabaseExport int64_t GetDecimals()const;

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

		/** Retrieves the value as a long double precision floating point
		*/
		inline long double ToLongDouble()const;

		/** Retrieves the precision
		@return
			The precision
		*/
		inline uint8_t GetPrecision()const;

		/** Tells if the value is signed
		@return
			The status
		*/
		inline bool IsSigned()const;

		/** Retrieves the raw value
		*/
		inline int64_t GetRawValue()const;
		
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

	private:
		uint8_t _precision;
		int64_t _value;
		bool _signed;
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
