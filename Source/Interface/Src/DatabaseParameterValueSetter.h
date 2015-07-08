/************************************************************************//**
 * @file DatabaseParameterValue.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/24/2014 8:37:01 AM
 *
 *
 * @brief CDatabaseQueryParameterValueBase class declaration.
 *
 * @details Describes the base class of inherited templated classes dealing with query parameters.
 *
 ***************************************************************************/

#ifndef ___DATABASE_PARAMETER_VALUE_SETTER_H___
#define ___DATABASE_PARAMETER_VALUE_SETTER_H___

#include "DatabasePrerequisites.h"

#include "DatabaseValue.h"

BEGIN_NAMESPACE_DATABASE
{
	/** Generic functor for setting the value.
	*/
	template< typename T > struct SDatabaseParameterValueSetter
	{
		typedef T value_type;
		/** Operator () for functor.
		@param[in] parameterValue
		    Parameter value.
		@param[in] value
		    New value.
		*/
		inline void operator()( CDatabaseValueBase & parameterValue, const T & value )
		{
			static_cast< CDatabaseValue< SDataTypeFieldTyper< value_type >::Value > & >( parameterValue ).SetValue( value );
		}
	};

	/** Functor for setting the value of std::string type.
	*/
	template<> struct SDatabaseParameterValueSetter< std::string >
	{
		typedef std::string value_type;
		/** Operator () for functor.
		@param[in] parameterValue
		    Parameter value.
		@param[in] value
		    New value.
		@param[in] limits
		    Parameter limits.
		*/
		inline void operator()( CDatabaseValueBase & parameterValue, const std::string & value )
		{
			static_cast< CDatabaseValue< SDataTypeFieldTyper< value_type >::Value > & >( parameterValue ).SetValue( value );
		}
	};

	/** Functor for setting the value of char* type.
	*/
	template<> struct SDatabaseParameterValueSetter< char * >
	{
		typedef char * value_type;
		/** Operator () for functor.
		@param[in] parameterValue
		    Parameter value.
		@param[in] value
		    New value.
		@param[in] limits
		    Parameter limits.
		*/
		inline void operator()( CDatabaseValueBase & parameterValue, const std::string & value )
		{
			static_cast< CDatabaseValue< SDataTypeFieldTyper< value_type >::Value > & >( parameterValue ).SetValue( value );
		}
	};

	/** Functor for setting the value of std::wstring type.
	*/
	template<> struct SDatabaseParameterValueSetter< std::wstring >
	{
		typedef std::wstring value_type;
		/** Operator () for functor.
		@param[in] parameterValue
		    Parameter value.
		@param[in] value
		    New value.
		@param[in] limits
		    Parameter limits.
		*/
		inline void operator()( CDatabaseValueBase & parameterValue, const std::wstring & value )
		{
			static_cast< CDatabaseValue< SDataTypeFieldTyper< value_type >::Value > & >( parameterValue ).SetValue( value );
		}
	};

	/** Functor for setting the value of wchar_t* type.
	*/
	template<> struct SDatabaseParameterValueSetter< wchar_t * >
	{
		typedef wchar_t * value_type;
		/** Operator () for functor.
		@param[in] parameterValue
		    Parameter value.
		@param[in] value
		    New value.
		@param[in] limits
		    Parameter limits.
		*/
		inline void operator()( CDatabaseValueBase & parameterValue, const std::wstring & value )
		{
			static_cast< CDatabaseValue< SDataTypeFieldTyper< value_type >::Value > & >( parameterValue ).SetValue( value );
		}
	};

	/** Functor for setting the value of date type.
	*/
	template<> struct SDatabaseParameterValueSetter< CDate >
	{
		typedef CDate value_type;
		/** Operator () for functor.
		@param[in] parameterValue
		    Parameter value.
		@param[in] value
		    New value.
		*/
		inline void operator()( CDatabaseValueBase & parameterValue, const CDate & value )
		{
			static_cast< CDatabaseValue< SDataTypeFieldTyper< value_type >::Value > & >( parameterValue ).SetValue( value );
		}
	};

	/** Functor for setting the value of date/time type.
	*/
	template<> struct SDatabaseParameterValueSetter< CDateTime >
	{
		typedef CDateTime value_type;
		/** Operator () for functor.
		@param[in] parameterValue
		    Parameter value.
		@param[in] value
		    New value.
		*/
		inline void operator()( CDatabaseValueBase & parameterValue, const CDateTime & value )
		{
			static_cast< CDatabaseValue< SDataTypeFieldTyper< value_type >::Value > & >( parameterValue ).SetValue( value );
		}
	};

	/** Functor for setting the value of time type.
	*/
	template<> struct SDatabaseParameterValueSetter< CTime >
	{
		typedef CTime value_type;
		/** Operator () for functor.
		@param[in] parameterValue
		    Parameter value.
		@param[in] value
		    New value.
		*/
		inline void operator()( CDatabaseValueBase & parameterValue, const CTime & value )
		{
			static_cast< CDatabaseValue< SDataTypeFieldTyper< value_type >::Value > & >( parameterValue ).SetValue( value );
		}
	};

	/** Functor for setting the value of byte type.
	*/
	template<> struct SDatabaseParameterValueSetter< uint8_t >
	{
		typedef uint8_t * value_type;
		/** Operator () for functor.
		@param[in] parameterValue
		    Parameter value.
		@param[in] value
		    New value.
		@param[in] size
		    Number of bytes
		*/
		inline void operator()( CDatabaseValueBase & parameterValue, const uint8_t * value, uint32_t size )
		{
			static_cast< CDatabaseValue< SDataTypeFieldTyper< value_type >::Value > & >( parameterValue ).SetValue( value, size );
		}
	};
}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_PARAMETER_VALUE_SETTER_H___
