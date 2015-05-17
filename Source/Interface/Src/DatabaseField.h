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
	class DatabaseExport CDatabaseField
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
		CDatabaseField( DatabaseFieldInfosPtr infos, bool isNull, const String & value );
		/** Constructor.
		@param[in] infos
		    Field informations.
		@param[in] begin, end
		    Iterators to the field value.
		 */
		CDatabaseField( DatabaseFieldInfosPtr infos, uint8_t * begin, uint8_t * end );

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
		virtual ~CDatabaseField();

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
		void SetStrValue( const String & value );

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
		void SetNull();

		/** Get field type.
		@return
		    Field type.
		*/
		EFieldType GetType() const;

		/** Get field name.
		@return
		    Field name.
		*/
		const String & GetName() const;

		/** Get field limits.
		@return
		    Field limits.
		*/
		const uint32_t & GetLimits() const;

		/** Get field value as a string.
		@return
		    Field value.
		*/
		String GetStrValue() const;

		/** Get nullity status.
		@return
		    true if field is NULL, false if otherwise.
		*/
		bool IsNull() const;

	protected:

		/** Set field value from another one.
		@param[in] field
		    The field.
		*/
		void SetValue( DatabaseFieldPtr field );

		/** Get value as boolean.
		@param[out] value
		    Value as boolean.
		*/
		virtual void DoGetValue( bool & value ) const;

		/** Get value as short.
		@param[out] value
		    Value as int.
		*/
		virtual void DoGetValue( short & value ) const;

		/** Get value as unsigned short.
		@param[out] value
		    Value as int.
		*/
		virtual void DoGetValue( unsigned short & value ) const;

		/** Get value as int.
		@param[out] value
		    Value as int.
		*/
		virtual void DoGetValue( int & value ) const;

		/** Get value as unsigned int.
		@param[out] value
		    Value as unsigned int.
		*/
		virtual void DoGetValue( unsigned int & value ) const;

		/** Get value as long.
		@param[out] value
		    Value as long.
		*/
		virtual void DoGetValue( long & value ) const;

		/** Get value as unsigned long.
		@param[out] value
		    Value as unsigned long.
		*/
		virtual void DoGetValue( unsigned long & value ) const;

		/** Get value as long long.
		@param[out] value
		    Value as long long.
		*/
		virtual void DoGetValue( long long & value ) const;

		/** Get value as unsigned long long.
		@param[out] value
		    Value as unsigned long long.
		*/
		virtual void DoGetValue( unsigned long long & value ) const;

		/** Get value as float.
		@param[out] value
		    Value as float.
		*/
		virtual void DoGetValue( float & value ) const;

		/** Get value as double.
		@param[out] value
		    Value as double.
		*/
		virtual void DoGetValue( double & value ) const;

		/** Get value as std::string.
		@param[out] value
		    Value as std::string.
		*/
		virtual void DoGetValue( std::string & value ) const;

		/** Get value as std::wstring.
		@param[out] value
		    Value as std::wstring.
		*/
		virtual void DoGetValue( std::wstring & value ) const;

		/** Get value as date/time.
		@param[out] value
		    Value as date/time.
		*/
		virtual void DoGetValue( CDateTime & value ) const;

		/** Get value as date.
		@param[out] value
		    Value as date.
		*/
		virtual void DoGetValue( CDate & value ) const;

		/** Get value as time.
		@param[out] value
		    Value as time.
		*/
		virtual void DoGetValue( CTime & value ) const;

		/** Get value as byte array.
		@param[out] value
		    Value as byte array.
		*/
		virtual void DoGetValue( std::vector< uint8_t > & value ) const;

		/** Get value as boolean.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[out] value
		    Value as boolean.
		*/
		virtual void DoGetValueFast( bool & value ) const;

		/** Get value as short.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[out] value
		    Value as int.
		*/
		virtual void DoGetValueFast( short & value ) const;

		/** Get value as unsigned short.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[out] value
		    Value as int.
		*/
		virtual void DoGetValueFast( unsigned short & value ) const;

		/** Get value as int.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[out] value
		    Value as int.
		*/
		virtual void DoGetValueFast( int & value ) const;

		/** Get value as unsigned int.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[out] value
		    Value as unsigned int.
		*/
		virtual void DoGetValueFast( unsigned int & value ) const;

		/** Get value as long.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[out] value
		    Value as long.
		*/
		virtual void DoGetValueFast( long & value ) const;

		/** Get value as unsigned long.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[out] value
		    Value as unsigned long.
		*/
		virtual void DoGetValueFast( unsigned long & value ) const;

		/** Get value as long long.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[out] value
		    Value as long long.
		*/
		virtual void DoGetValueFast( long long & value ) const;

		/** Get value as unsigned long long.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[out] value
		    Value as unsigned long long.
		*/
		virtual void DoGetValueFast( unsigned long long & value ) const;

		/** Get value as float.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[out] value
		    Value as float.
		*/
		virtual void DoGetValueFast( float & value ) const;

		/** Get value as double.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[out] value
		    Value as double.
		*/
		virtual void DoGetValueFast( double & value ) const;

		/** Get value as std::string.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[out] value
		    Value as std::string.
		*/
		virtual void DoGetValueFast( std::string & value ) const;

		/** Get value as std::wstring.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[out] value
		    Value as std::wstring.
		*/
		virtual void DoGetValueFast( std::wstring & value ) const;

		/** Get value as date/time.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[out] value
		    Value as date/time.
		*/
		virtual void DoGetValueFast( CDateTime & value ) const;

		/** Get value as date.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[out] value
		    Value as date.
		*/
		virtual void DoGetValueFast( CDate & value ) const;

		/** Get value as time.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[out] value
		    Value as time.
		*/
		virtual void DoGetValueFast( CTime & value ) const;

		/** Get value as byte array.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[out] value
		    Value as byte array.
		*/
		virtual void DoGetValueFast( std::vector< uint8_t > & value ) const;

		/** Set boolean value.
		@param[in] value
		    Value as boolean.
		*/
		virtual void DoSetValue( const bool & value );

		/** Set short value.
		@param[in] value
		    Value as int.
		*/
		virtual void DoSetValue( const short & value );

		/** Set short value.
		@param[in] value
		    Value as int.
		*/
		virtual void DoSetValue( const unsigned short & value );

		/** Set int value.
		@param[in] value
		    Value as int.
		*/
		virtual void DoSetValue( const int & value );

		/** Set unsigned int value.
		@param[in] value
		    Value as unsigned int.
		*/
		virtual void DoSetValue( const unsigned int & value );

		/** Set long value.
		@param[in] value
		    Value as long.
		*/
		virtual void DoSetValue( const long & value );

		/** Set unsigned long value.
		@param[in] value
		    Value as unsigned long.
		*/
		virtual void DoSetValue( const unsigned long & value );

		/** Set long long value.
		@param[in] value
		    Value as long long.
		*/
		virtual void DoSetValue( const long long & value );

		/** Set unsigned long long value.
		@param[in] value
		    Value as unsigned long long.
		*/
		virtual void DoSetValue( const unsigned long long & value );

		/** Set float value.
		@param[in] value
		    Value as float.
		*/
		virtual void DoSetValue( const float & value );

		/** Set double value.
		@param[in] value
		    Value as double.
		*/
		virtual void DoSetValue( const double & value );

		/** Set std::string value.
		@param[in] value
		    Value as std::string.
		*/
		virtual void DoSetValue( const std::string & value );

		/** Set std::wstring value.
		@param[in] value
		    Value as std::wstring.
		*/
		virtual void DoSetValue( const std::wstring & value );

		/** Set date/time value.
		@param[in] value
		    Value as date/time.
		*/
		virtual void DoSetValue( const CDateTime & value );

		/** Set date value.
		@param[in] value
		    Value as date.
		*/
		virtual void DoSetValue( const CDate & value );

		/** Set time value.
		@param[in] value
		    Value as time.
		*/
		virtual void DoSetValue( const CTime & value );

		/** Set byte array value.
		@param[in] value
		    Value as byte array.
		*/
		virtual void DoSetValue( const std::vector< uint8_t > & value );

		/** Set boolean value.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[in] value
		    Value as boolean.
		*/
		virtual void DoSetValueFast( const bool & value );

		/** Set short value.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[in] value
		    Value as int.
		*/
		virtual void DoSetValueFast( const short & value );

		/** Set short value.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[in] value
		    Value as int.
		*/
		virtual void DoSetValueFast( const unsigned short & value );

		/** Set int value.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[in] value
		    Value as int.
		*/
		virtual void DoSetValueFast( const int & value );

		/** Set unsigned int value.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[in] value
		    Value as unsigned int.
		*/
		virtual void DoSetValueFast( const unsigned int & value );

		/** Set long value.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[in] value
		    Value as long.
		*/
		virtual void DoSetValueFast( const long & value );

		/** Set unsigned long value.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[in] value
		    Value as unsigned long.
		*/
		virtual void DoSetValueFast( const unsigned long & value );

		/** Set long long value.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[in] value
		    Value as long long.
		*/
		virtual void DoSetValueFast( const long long & value );

		/** Set unsigned long long value.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[in] value
		    Value as unsigned long long.
		*/
		virtual void DoSetValueFast( const unsigned long long & value );

		/** Set float value.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[in] value
		    Value as float.
		*/
		virtual void DoSetValueFast( const float & value );

		/** Set double value.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[in] value
		    Value as double.
		*/
		virtual void DoSetValueFast( const double & value );

		/** Set char * value.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[in] value
		    Value as char *.
		*/
		virtual void DoSetValueFast( const char * value );

		/** Set std::string value.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[in] value
		    Value as std::string.
		*/
		virtual void DoSetValueFast( const std::string & value );

		/** Set wchar_t * value.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[in] value
		    Value as wchar_t *.
		*/
		virtual void DoSetValueFast( const wchar_t * value );

		/** Set std::wstring value.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[in] value
		    Value as std::wstring.
		*/
		virtual void DoSetValueFast( const std::wstring & value );

		/** Set date/time value.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[in] value
		    Value as date/time.
		*/
		virtual void DoSetValueFast( const CDateTime & value );

		/** Set date value.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[in] value
		    Value as date.
		*/
		virtual void DoSetValueFast( const CDate & value );

		/** Set time value.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[in] value
		    Value as time.
		*/
		virtual void DoSetValueFast( const CTime & value );

		/** Set byte array value.
		@remarks
		    No check is made in this function, so the value type must match the field type
		@param[in] value
		    Value as byte array.
		*/
		virtual void DoSetValueFast( const std::vector< uint8_t > & value );

	protected:
		DatabaseFieldInfosPtr       _infos;     ///< Field information.
		CDatabaseValueBase  *  _value;     ///< Field value.
		bool                        _isNull;    ///< Field considered as NULL or not.
	};
}
END_NAMESPACE_DATABASE

#include "DatabaseField.inl"

#endif //__DATABASE_FIELD_H___
