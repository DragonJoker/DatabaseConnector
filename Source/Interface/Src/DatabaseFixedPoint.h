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

		/** Constructor from int8_t
		@param[in] value
			The value
		*/
		DatabaseExport explicit CFixedPoint( int8_t value );

		/** Constructor from uint8_t
		@param[in] value
			The value
		*/
		DatabaseExport explicit CFixedPoint( uint8_t value );

		/** Constructor from int16_t
		@param[in] value
			The value
		*/
		DatabaseExport explicit CFixedPoint( int16_t value );

		/** Constructor from uint16_t
		@param[in] value
			The value
		*/
		DatabaseExport explicit CFixedPoint( uint16_t value );

		/** Constructor from int32_t
		@param[in] value
			The value
		*/
		DatabaseExport explicit CFixedPoint( int32_t value );

		/** Constructor from uint32_t
		@param[in] value
			The value
		*/
		DatabaseExport explicit CFixedPoint( uint32_t value );

		/** Constructor from int64_t
		@param[in] value
			The value
		*/
		DatabaseExport explicit CFixedPoint( int64_t value );

		/** Constructor from uint64_t
		@param[in] value
			The value
		*/
		DatabaseExport explicit CFixedPoint( uint64_t value );

		/** Constructor from float
		@param[in] value
			The value
		*/
		DatabaseExport explicit CFixedPoint( float value );

		/** Constructor from double
		@param[in] value
			The value
		*/
		DatabaseExport explicit CFixedPoint( double value );

		/** Constructor from double
		@param[in] value
			The value
		*/
		DatabaseExport explicit CFixedPoint( long double value );

		/** Constructor from string
		@param[in] value
			The value
		@param[in] precision
			The precision
		@param[in] scale
			The scale
		*/
		DatabaseExport CFixedPoint( const String & value, uint8_t precision = -1, uint8_t scale = -1 );

		/** Destructor
		*/
		DatabaseExport ~CFixedPoint();

		/** Retrieves the value as a simple precision floating point
		*/
		DatabaseExport float ToFloat()const;

		/** Retrieves the value as a double precision floating point
		*/
		DatabaseExport double ToDouble()const;

		/** Retrieves the value as a long double precision floating point
		*/
		DatabaseExport long double ToLongDouble()const;

		/** Retrieves the number as a string
		*/
		DatabaseExport String const & ToString()const;

		/** Retrieves the precision
		*/
		DatabaseExport uint8_t GetPrecision()const;

		/** Retrieves the scale
		*/
		DatabaseExport uint8_t GetScale()const;

		/** Tells if the number is signed
		*/
		DatabaseExport bool IsSigned()const;

	private:
		bool _signed = true;
		uint8_t _precision = 10;
		uint8_t _scale = 5;
		String _value;
	};

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

#endif
