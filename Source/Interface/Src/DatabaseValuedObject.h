/************************************************************************//**
* @file DatabaseValuedObject.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDatabaseValuedObject class declaration.
*
* @details Describes a an object (field or parameter) with a value.
*
***************************************************************************/

#ifndef ___DATABASE_VALUED_OBJECT_H___
#define ___DATABASE_VALUED_OBJECT_H___

#include "DatabasePrerequisites.h"

#include <boost/optional.hpp>

BEGIN_NAMESPACE_DATABASE
{
	/** Describes a an object (field or parameter) with a value.
	*/
	class CDatabaseValuedObject
	{
	public:
		/** Constructor.
		@param[in] connection
			The database connection.
		@param[in] infos
			Valued object informations.
		*/
		DatabaseExport CDatabaseValuedObject( DatabaseConnectionSPtr connection, DatabaseValuedObjectInfosSPtr infos );

		/** Destructor.
		*/
		DatabaseExport virtual ~CDatabaseValuedObject();

		/** Get field value.
		@return
			Field value.
		*/
		inline CDatabaseValueBase const & GetObjectValue()const;

		/** Get field value.
		@return
			Field value.
		*/
		inline CDatabaseValueBase & GetObjectValue();

		/** Get field value.
		@return
			Field value.
		*/
		inline DatabaseConnectionSPtr GetConnection()const;

		/** Get field type.
		@return
			Field type.
		*/
		DatabaseExport EFieldType GetType() const;

		/** Get field type.
		@return
			Field type.
		*/
		DatabaseExport const String & GetName() const;

		/** Get field limits.
		@return
			Field limits.
		*/
		DatabaseExport const uint32_t & GetLimits() const;

		/** Get field precision.
		@return
			Field precision.
		*/
		DatabaseExport const std::pair< uint32_t, uint32_t > & GetPrecision() const;

	protected:
		/** Get value as boolean.
		@param[out] value
			Value as boolean.
		*/
		DatabaseExport virtual void DoGetValue( bool & value ) const;

		/** Get value as char.
		@param[out] value
			Value as int.
		*/
		DatabaseExport virtual void DoGetValue( int8_t & value ) const;

		/** Get value as unsigned char.
		@param[out] value
			Value as int.
		*/
		DatabaseExport virtual void DoGetValue( uint8_t & value ) const;

		/** Get value as short.
		@param[out] value
			Value as int.
		*/
		DatabaseExport virtual void DoGetValue( int16_t & value ) const;

		/** Get value as unsigned short.
		@param[out] value
			Value as int.
		*/
		DatabaseExport virtual void DoGetValue( uint16_t & value ) const;

		/** Get value as int24_t.
		@param[out] value
			Value as int24_t.
		*/
		DatabaseExport virtual void DoGetValue( int24_t & value ) const;

		/** Get value as uint24_t.
		@param[out] value
			Value as uint24_t.
		*/
		DatabaseExport virtual void DoGetValue( uint24_t & value ) const;

		/** Get value as int.
		@param[out] value
			Value as int.
		*/
		DatabaseExport virtual void DoGetValue( int32_t & value ) const;

		/** Get value as unsigned int.
		@param[out] value
			Value as unsigned int.
		*/
		DatabaseExport virtual void DoGetValue( uint32_t & value ) const;

		/** Get value as long long.
		@param[out] value
			Value as long long.
		*/
		DatabaseExport virtual void DoGetValue( int64_t & value ) const;

		/** Get value as unsigned long long.
		@param[out] value
			Value as unsigned long long.
		*/
		DatabaseExport virtual void DoGetValue( uint64_t & value ) const;

		/** Get value as float.
		@param[out] value
			Value as float.
		*/
		DatabaseExport virtual void DoGetValue( float & value ) const;

		/** Get value as double.
		@param[out] value
			Value as double.
		*/
		DatabaseExport virtual void DoGetValue( double & value ) const;

		/** Get value as fixed point numeric.
		@param[out] value
			Value as long double.
		*/
		DatabaseExport virtual void DoGetValue( CFixedPoint & value ) const;

		/** Get value as std::string.
		@param[out] value
			Value as std::string.
		*/
		DatabaseExport virtual void DoGetValue( std::string & value ) const;

		/** Get value as std::wstring.
		@param[out] value
			Value as std::wstring.
		*/
		DatabaseExport virtual void DoGetValue( std::wstring & value ) const;

		/** Get value as date/time.
		@param[out] value
			Value as date/time.
		*/
		DatabaseExport virtual void DoGetValue( DateTimeType & value ) const;

		/** Get value as date.
		@param[out] value
			Value as date.
		*/
		DatabaseExport virtual void DoGetValue( DateType & value ) const;

		/** Get value as time.
		@param[out] value
			Value as time.
		*/
		DatabaseExport virtual void DoGetValue( TimeType & value ) const;

		/** Get value as byte array.
		@param[out] value
			Value as byte array.
		*/
		DatabaseExport virtual void DoGetValue( ByteArray & value ) const;

		/** Get value as boolean.
		@remarks
			No check is made in this function, so the value type must match the field type
		@param[out] value
			Value as boolean.
		*/
		DatabaseExport virtual void DoGetValueFast( bool & value ) const;

		/** Get value as char.
		@remarks
			No check is made in this function, so the value type must match the field type
		@param[out] value
			Value as int.
		*/
		DatabaseExport virtual void DoGetValueFast( int8_t & value ) const;

		/** Get value as unsigned char.
		@remarks
			No check is made in this function, so the value type must match the field type
		@param[out] value
			Value as int.
		*/
		DatabaseExport virtual void DoGetValueFast( uint8_t & value ) const;

		/** Get value as short.
		@remarks
			No check is made in this function, so the value type must match the field type
		@param[out] value
			Value as int.
		*/
		DatabaseExport virtual void DoGetValueFast( int16_t & value ) const;

		/** Get value as unsigned short.
		@remarks
			No check is made in this function, so the value type must match the field type
		@param[out] value
			Value as int.
		*/
		DatabaseExport virtual void DoGetValueFast( uint16_t & value ) const;

		/** Get value as int24_t.
		@remarks
			No check is made in this function, so the value type must match the field type
		@param[out] value
			Value as int.
		*/
		DatabaseExport virtual void DoGetValueFast( int24_t & value ) const;

		/** Get value as uint24_t.
		@remarks
			No check is made in this function, so the value type must match the field type
		@param[out] value
			Value as int.
		*/
		DatabaseExport virtual void DoGetValueFast( uint24_t & value ) const;

		/** Get value as int.
		@remarks
			No check is made in this function, so the value type must match the field type
		@param[out] value
			Value as int.
		*/
		DatabaseExport virtual void DoGetValueFast( int32_t & value ) const;

		/** Get value as unsigned int.
		@remarks
			No check is made in this function, so the value type must match the field type
		@param[out] value
			Value as unsigned int.
		*/
		DatabaseExport virtual void DoGetValueFast( uint32_t & value ) const;

		/** Get value as long long.
		@remarks
			No check is made in this function, so the value type must match the field type
		@param[out] value
			Value as long long.
		*/
		DatabaseExport virtual void DoGetValueFast( int64_t & value ) const;

		/** Get value as unsigned long long.
		@remarks
			No check is made in this function, so the value type must match the field type
		@param[out] value
			Value as unsigned long long.
		*/
		DatabaseExport virtual void DoGetValueFast( uint64_t & value ) const;

		/** Get value as float.
		@remarks
			No check is made in this function, so the value type must match the field type
		@param[out] value
			Value as float.
		*/
		DatabaseExport virtual void DoGetValueFast( float & value ) const;

		/** Get value as double.
		@remarks
			No check is made in this function, so the value type must match the field type
		@param[out] value
			Value as double.
		*/
		DatabaseExport virtual void DoGetValueFast( double & value ) const;

		/** Get value as fixed point numeric.
		@remarks
			No check is made in this function, so the value type must match the field type
		@param[out] value
			Value as long double.
		*/
		DatabaseExport virtual void DoGetValueFast( CFixedPoint & value ) const;

		/** Get value as std::string.
		@remarks
			No check is made in this function, so the value type must match the field type
		@param[out] value
			Value as std::string.
		*/
		DatabaseExport virtual void DoGetValueFast( std::string & value ) const;

		/** Get value as std::wstring.
		@remarks
			No check is made in this function, so the value type must match the field type
		@param[out] value
			Value as std::wstring.
		*/
		DatabaseExport virtual void DoGetValueFast( std::wstring & value ) const;

		/** Get value as date/time.
		@remarks
			No check is made in this function, so the value type must match the field type
		@param[out] value
			Value as date/time.
		*/
		DatabaseExport virtual void DoGetValueFast( DateTimeType & value ) const;

		/** Get value as date.
		@remarks
			No check is made in this function, so the value type must match the field type
		@param[out] value
			Value as date.
		*/
		DatabaseExport virtual void DoGetValueFast( DateType & value ) const;

		/** Get value as time.
		@remarks
			No check is made in this function, so the value type must match the field type
		@param[out] value
			Value as time.
		*/
		DatabaseExport virtual void DoGetValueFast( TimeType & value ) const;

		/** Get value as byte array.
		@remarks
			No check is made in this function, so the value type must match the field type
		@param[out] value
			Value as byte array.
		*/
		DatabaseExport virtual void DoGetValueFast( ByteArray & value ) const;

		/** Set parameter value as a boolean.
		@param value
			New parameter value.
		*/
		DatabaseExport virtual void DoSetValue( const bool & value );

		/** Set parameter value as a char.
		@param value
			New parameter value.
		*/
		DatabaseExport virtual void DoSetValue( const int8_t & value );

		/** Set parameter value as a unsigned char.
		@param value
			New parameter value.
		*/
		DatabaseExport virtual void DoSetValue( const uint8_t & value );

		/** Set parameter value as a short.
		@param value
			New parameter value.
		*/
		DatabaseExport virtual void DoSetValue( const int16_t & value );

		/** Set parameter value as an unsigned short.
		@param value
			New parameter value.
		*/
		DatabaseExport virtual void DoSetValue( const uint16_t & value );

		/** Set parameter value as a long.
		@param value
			New parameter value.
		*/
		DatabaseExport virtual void DoSetValue( const int32_t & value );

		/** Set parameter value as an unsigned long.
		@param value
			New parameter value.
		*/
		DatabaseExport virtual void DoSetValue( const uint32_t & value );

		/** Set parameter value as an int24_t.
		@param value
			New parameter value.
		*/
		DatabaseExport virtual void DoSetValue( const int24_t & value );

		/** Set parameter value as an uint24_t.
		@param value
			New parameter value.
		*/
		DatabaseExport virtual void DoSetValue( const uint24_t & value );

		/** Set parameter value as a long long.
		@param value
			New parameter value.
		*/
		DatabaseExport virtual void DoSetValue( const int64_t & value );

		/** Set parameter value as an unsigned long long.
		@param value
			New parameter value.
		*/
		DatabaseExport virtual void DoSetValue( const uint64_t & value );

		/** Set parameter value as a float.
		@param value
			New parameter value.
		*/
		DatabaseExport virtual void DoSetValue( const float & value );

		/** Set parameter value as a double.
		@param value
			New parameter value.
		*/
		DatabaseExport virtual void DoSetValue( const double & value );

		/** Set parameter value as a fixed point numeric.
		@param value
			New parameter value.
		*/
		DatabaseExport virtual void DoSetValue( const CFixedPoint & value );

		/** Set parameter value as a std::string.
		@param value
			New parameter value.
		*/
		DatabaseExport virtual void DoSetValue( const std::string & value );

		/** Set parameter value as a std::wstring.
		@param value
			New parameter value.
		*/
		DatabaseExport virtual void DoSetValue( const std::wstring & value );

		/** Set parameter value as a date/time.
		@param value
			New parameter value.
		*/
		DatabaseExport virtual void DoSetValue( const DateTimeType & value );

		/** Set parameter value as a date.
		@param value
			New parameter value.
		*/
		DatabaseExport virtual void DoSetValue( const DateType & value );

		/** Set parameter value as a time.
		@param value
			New parameter value.
		*/
		DatabaseExport virtual void DoSetValue( const TimeType & value );

		/** Set parameter value as a byte array.
		@param value
			New parameter value.
		*/
		DatabaseExport virtual void DoSetValue( const ByteArray & value );

		/** Set parameter value as a boolean.
		@remarks
			Don't perform type checks
		@param value
			New parameter value.
		*/
		DatabaseExport virtual void DoSetValueFast( const bool & value );

		/** Set parameter value as a char.
		@remarks
			Don't perform type checks
		@param value
			New parameter value.
		*/
		DatabaseExport virtual void DoSetValueFast( const int8_t & value );

		/** Set parameter value as a unsigned char.
		@remarks
			Don't perform type checks
		@param value
			New parameter value.
		*/
		DatabaseExport virtual void DoSetValueFast( const uint8_t & value );

		/** Set parameter value as a short.
		@remarks
			Don't perform type checks
		@param value
			New parameter value.
		*/
		DatabaseExport virtual void DoSetValueFast( const int16_t & value );

		/** Set parameter value as an unsigned short.
		@remarks
			Don't perform type checks
		@param value
			New parameter value.
		*/
		DatabaseExport virtual void DoSetValueFast( const uint16_t & value );

		/** Set parameter value as an int24_t.
		@remarks
			Don't perform type checks
		@param value
			New parameter value.
		*/
		DatabaseExport virtual void DoSetValueFast( const int24_t & value );

		/** Set parameter value as an uint24_t.
		@remarks
			Don't perform type checks
		@param value
			New parameter value.
		*/
		DatabaseExport virtual void DoSetValueFast( const uint24_t & value );

		/** Set parameter value as a long.
		@remarks
			Don't perform type checks
		@param value
			New parameter value.
		*/
		DatabaseExport virtual void DoSetValueFast( const int32_t & value );

		/** Set parameter value as an unsigned long.
		@remarks
			Don't perform type checks
		@param value
			New parameter value.
		*/
		DatabaseExport virtual void DoSetValueFast( const uint32_t & value );

		/** Set parameter value as a long long.
		@remarks
			Don't perform type checks
		@param value
			New parameter value.
		*/
		DatabaseExport virtual void DoSetValueFast( const int64_t & value );

		/** Set parameter value as an unsigned long long.
		@remarks
			Don't perform type checks
		@param value
			New parameter value.
		*/
		DatabaseExport virtual void DoSetValueFast( const uint64_t & value );

		/** Set parameter value as a float.
		@remarks
			Don't perform type checks
		@param value
			New parameter value.
		*/
		DatabaseExport virtual void DoSetValueFast( const float & value );

		/** Set parameter value as a double.
		@remarks
			Don't perform type checks
		@param value
			New parameter value.
		*/
		DatabaseExport virtual void DoSetValueFast( const double & value );

		/** Set parameter value as a fixed point numeric.
		@remarks
			Don't perform type checks
		@param value
			New parameter value.
		*/
		DatabaseExport virtual void DoSetValueFast( const CFixedPoint & value );

		/** Set parameter value as a std::string.
		@remarks
			Don't perform type checks
		@param value
			New parameter value.
		*/
		DatabaseExport virtual void DoSetValueFast( const std::string & value );

		/** Set parameter value as a std::wstring.
		@remarks
			Don't perform type checks
		@param value
			New parameter value.
		*/
		DatabaseExport virtual void DoSetValueFast( const std::wstring & value );

		/** Set parameter value as a date.
		@remarks
			Don't perform type checks
		@param value
			New parameter value.
		*/
		DatabaseExport virtual void DoSetValueFast( const DateType & value );

		/** Set parameter value as a date/time.
		@remarks
			Don't perform type checks
		@param value
			New parameter value.
		*/
		DatabaseExport virtual void DoSetValueFast( const DateTimeType & value );

		/** Set parameter value as a time.
		@remarks
			Don't perform type checks
		@param value
			New parameter value.
		*/
		DatabaseExport virtual void DoSetValueFast( const TimeType & value );

		/** Set parameter value as a byte array.
		@remarks
			Don't perform type checks
		@remarks
			Don't perform type checks
		@param value
			New parameter value.
		*/
		DatabaseExport virtual void DoSetValueFast( const ByteArray & value );

		/** Creates the field value
		*/
		DatabaseExport virtual void DoCreateValue();

	private:
		//! The value.
		mutable DatabaseValueBaseSPtr _value;
		//! The value informations
		DatabaseValuedObjectInfosSPtr _infos;
		//! Database connection.
		DatabaseConnectionWPtr _connection;
	};
}
END_NAMESPACE_DATABASE

#include "DatabaseValuedObject.inl"

#endif //__DATABASE_FIELD_H___
