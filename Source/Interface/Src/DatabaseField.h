/************************************************************************//**
 * @file DatabaseField.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 *
 *
 * @brief CDatabaseField class declaration.
 *
 * @details Describes a database field.
 *
 ***************************************************************************/

#ifndef ___DATABASE_FIELD_H___
#define ___DATABASE_FIELD_H___

#include "DatabasePrerequisites.h"

#include "DatabaseException.h"
#include "DatabaseFieldValueInitializer.h"

#include "EFieldType.h"
#include "DatabaseLogger.h"

#include <boost/optional.hpp>

BEGIN_NAMESPACE_DATABASE
{
	/** Describes a database field.
	*/
	class CDatabaseField
	{
	public:
		/** Constructor.
		@param[in] infos
		    Field informations.
		@param[in] isNull
		    If true, the field is considered null.
		@param[in] value
		    Field value.
		 */
		DatabaseExport CDatabaseField( DatabaseFieldInfosPtr infos, bool isNull, const String & value );
		/** Constructor.
		@param[in] infos
		    Field informations.
		@param[in] begin, end
		    Iterators to the field value.
		 */
		DatabaseExport CDatabaseField( DatabaseFieldInfosPtr infos, uint8_t * begin, uint8_t * end );

		/** Constructor.
		@param[in] infos
		    Field informations.
		@param[in] value
		    Filed value.
		*/
		template< typename T >
		inline CDatabaseField( DatabaseFieldInfosPtr infos, const T & value );

		/** Destructor.
		*/
		DatabaseExport virtual ~CDatabaseField();

		/** Get field value.
		@return
		    Field value.
		*/
		inline CDatabaseValueBase * GetFieldValue() const;

		/** Get field value.
		@return
		    Field value.
		*/
		template <typename T> inline T GetValue() const;

		/** Get field value.
		@param[out] value
		    Field value.
		*/
		template <typename T> inline void GetValue( T & value ) const;

		/** Get field value as a std::optional.
		@return
		    Field value, std::none if the field is null.
		*/
		template <typename T> inline boost::optional< T > GetValueOpt() const;

		/** Get field value as a std::optional.
		@param[out] value
		    Field value, std::none if the field is null.
		*/
		template <typename T> inline void GetValueOpt( boost::optional< T > & value ) const;

		/** Get field value.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@return
		    Field value.
		*/
		template <typename T> inline T GetValueFast() const;

		/** Get field value.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[out] value
		    Field value.
		*/
		template <typename T> inline void GetValueFast( T & value ) const;

		/** Get field value as a std::optional.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@return
		    Field value, std::none if the field is null.
		*/
		template <typename T> inline boost::optional< T > GetValueOptFast() const;

		/** Get field value as a std::optional.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[out] value
		    Field value, std::none if the field is null.
		*/
		template <typename T> inline void GetValueOptFast( boost::optional< T > & value ) const;

		/** Set field value as a string.
		@param value
		    New field value.
		*/
		DatabaseExport void SetStrValue( const String & value );

		/** Get field value.
		@param[out] value
		    Field value.
		*/
		template <typename T> inline void SetValue( const T & value );

		/** Get field value.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[out] value
		    Field value.
		*/
		template <typename T> inline void SetValueFast( const T & value );

		/** Sets the field value to null
		*/
		DatabaseExport void SetNull();

		/** Get field type.
		@return
		    Field type.
		*/
		DatabaseExport EFieldType GetType() const;

		/** Get field name.
		@return
		    Field name.
		*/
		DatabaseExport const String & GetName() const;

		/** Get field limits.
		@return
		    Field limits.
		*/
		DatabaseExport const uint32_t & GetLimits() const;

		/** Get field value as a string.
		@return
		    Field value.
		*/
		DatabaseExport String GetStrValue() const;

		/** Get nullity status.
		@return
		    true if field is NULL, false if otherwise.
		*/
		DatabaseExport bool IsNull() const;

	protected:

		/** Set field value from another one.
		@param[in] field
		    The field.
		*/
		DatabaseExport void SetValue( DatabaseFieldPtr field );

		/** Get value as boolean.
		@param[out] value
		    Value as boolean.
		*/
		DatabaseExport virtual void DoGetValue( bool & value ) const;

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
		DatabaseExport virtual void DoGetValue( CDateTime & value ) const;

		/** Get value as date.
		@param[out] value
		    Value as date.
		*/
		DatabaseExport virtual void DoGetValue( CDate & value ) const;

		/** Get value as time.
		@param[out] value
		    Value as time.
		*/
		DatabaseExport virtual void DoGetValue( CTime & value ) const;

		/** Get value as byte array.
		@param[out] value
		    Value as byte array.
		*/
		DatabaseExport virtual void DoGetValue( std::vector< uint8_t > & value ) const;

		/** Get value as boolean.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[out] value
		    Value as boolean.
		*/
		DatabaseExport virtual void DoGetValueFast( bool & value ) const;

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
		DatabaseExport virtual void DoGetValueFast( CDateTime & value ) const;

		/** Get value as date.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[out] value
		    Value as date.
		*/
		DatabaseExport virtual void DoGetValueFast( CDate & value ) const;

		/** Get value as time.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[out] value
		    Value as time.
		*/
		DatabaseExport virtual void DoGetValueFast( CTime & value ) const;

		/** Get value as byte array.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[out] value
		    Value as byte array.
		*/
		DatabaseExport virtual void DoGetValueFast( std::vector< uint8_t > & value ) const;

		/** Set boolean value.
		@param[in] value
		    Value as boolean.
		*/
		DatabaseExport virtual void DoSetValue( const bool & value );

		/** Set short value.
		@param[in] value
		    Value as int.
		*/
		DatabaseExport virtual void DoSetValue( const int16_t & value );

		/** Set short value.
		@param[in] value
		    Value as int.
		*/
		DatabaseExport virtual void DoSetValue( const uint16_t & value );

		/** Set int value.
		@param[in] value
		    Value as int.
		*/
		DatabaseExport virtual void DoSetValue( const int32_t & value );

		/** Set unsigned int value.
		@param[in] value
		    Value as unsigned int.
		*/
		DatabaseExport virtual void DoSetValue( const uint32_t & value );

		/** Set long long value.
		@param[in] value
		    Value as long long.
		*/
		DatabaseExport virtual void DoSetValue( const int64_t & value );

		/** Set unsigned long long value.
		@param[in] value
		    Value as unsigned long long.
		*/
		DatabaseExport virtual void DoSetValue( const uint64_t & value );

		/** Set float value.
		@param[in] value
		    Value as float.
		*/
		DatabaseExport virtual void DoSetValue( const float & value );

		/** Set double value.
		@param[in] value
		    Value as double.
		*/
		DatabaseExport virtual void DoSetValue( const double & value );

		/** Set std::string value.
		@param[in] value
		    Value as std::string.
		*/
		DatabaseExport virtual void DoSetValue( const std::string & value );

		/** Set std::wstring value.
		@param[in] value
		    Value as std::wstring.
		*/
		DatabaseExport virtual void DoSetValue( const std::wstring & value );

		/** Set date/time value.
		@param[in] value
		    Value as date/time.
		*/
		DatabaseExport virtual void DoSetValue( const CDateTime & value );

		/** Set date value.
		@param[in] value
		    Value as date.
		*/
		DatabaseExport virtual void DoSetValue( const CDate & value );

		/** Set time value.
		@param[in] value
		    Value as time.
		*/
		DatabaseExport virtual void DoSetValue( const CTime & value );

		/** Set byte array value.
		@param[in] value
		    Value as byte array.
		*/
		DatabaseExport virtual void DoSetValue( const std::vector< uint8_t > & value );

		/** Set boolean value.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[in] value
		    Value as boolean.
		*/
		DatabaseExport virtual void DoSetValueFast( const bool & value );

		/** Set short value.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[in] value
		    Value as int.
		*/
		DatabaseExport virtual void DoSetValueFast( const int16_t & value );

		/** Set short value.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[in] value
		    Value as int.
		*/
		DatabaseExport virtual void DoSetValueFast( const uint16_t & value );

		/** Set int value.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[in] value
		    Value as int.
		*/
		DatabaseExport virtual void DoSetValueFast( const int32_t & value );

		/** Set unsigned int value.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[in] value
		    Value as unsigned int.
		*/
		DatabaseExport virtual void DoSetValueFast( const uint32_t & value );

		/** Set long long value.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[in] value
		    Value as long long.
		*/
		DatabaseExport virtual void DoSetValueFast( const int64_t & value );

		/** Set unsigned long long value.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[in] value
		    Value as unsigned long long.
		*/
		DatabaseExport virtual void DoSetValueFast( const uint64_t & value );

		/** Set float value.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[in] value
		    Value as float.
		*/
		DatabaseExport virtual void DoSetValueFast( const float & value );

		/** Set double value.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[in] value
		    Value as double.
		*/
		DatabaseExport virtual void DoSetValueFast( const double & value );

		/** Set char * value.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[in] value
		    Value as char *.
		*/
		DatabaseExport virtual void DoSetValueFast( const char * value );

		/** Set std::string value.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[in] value
		    Value as std::string.
		*/
		DatabaseExport virtual void DoSetValueFast( const std::string & value );

		/** Set wchar_t * value.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[in] value
		    Value as wchar_t *.
		*/
		DatabaseExport virtual void DoSetValueFast( const wchar_t * value );

		/** Set std::wstring value.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[in] value
		    Value as std::wstring.
		*/
		DatabaseExport virtual void DoSetValueFast( const std::wstring & value );

		/** Set date/time value.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[in] value
		    Value as date/time.
		*/
		DatabaseExport virtual void DoSetValueFast( const CDateTime & value );

		/** Set date value.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[in] value
		    Value as date.
		*/
		DatabaseExport virtual void DoSetValueFast( const CDate & value );

		/** Set time value.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[in] value
		    Value as time.
		*/
		DatabaseExport virtual void DoSetValueFast( const CTime & value );

		/** Set byte array value.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[in] value
		    Value as byte array.
		*/
		DatabaseExport virtual void DoSetValueFast( const std::vector< uint8_t > & value );

	protected:
		DatabaseFieldInfosPtr       _infos;     ///< Field information.
		CDatabaseValueBase  *  _value;     ///< Field value.
		bool                        _isNull;    ///< Field considered as NULL or not.
	};
}
END_NAMESPACE_DATABASE

#include "DatabaseField.inl"

#endif //__DATABASE_FIELD_H___
