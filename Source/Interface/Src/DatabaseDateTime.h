/************************************************************************//**
* @file DateTime.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief DateTime helper functions.
*
***************************************************************************/

#ifndef ___DATABASE_DATE_TIME_H___
#define ___DATABASE_DATE_TIME_H___

#include "DatabasePrerequisites.h"

BEGIN_NAMESPACE_DATABASE
{
	namespace DateTime
	{
		/** Get date/time formatted according to the format mask.
		@remarks
			Uses strftime
		@param[in] dateTime
			Time date/time.
		@param[in] format
			Time format.
		@return
			Formatted string.
		*/
		DatabaseExport std::string Format( const DateTimeType & dateTime, const std::string & format );

		/** Get date/time formatted according to the format mask.
		@remarks
			Uses wcsftime
		@param[in] dateTime
			Time date/time.
		@param[in] format
			Time format.
		@return
			Formatted string.
		*/
		DatabaseExport std::wstring Format( const DateTimeType & dateTime, const std::wstring & format );

		/** Get date/time formatted according to the format mask.
		@remarks
			Uses vsnprintf, the date will be printed in year, month, then day order, then the time, in hours, minutes, then seconds order
		@param[in] dateTime
		    Time date/time.
		@param[in] format
		    Time format.
		@return
		    Formatted string.
		*/
		DatabaseExport std::string Print( const DateTimeType & dateTime, const std::string & format );

		/** Get date/time formatted according to the format mask.
		@remarks
			Uses vsnprintf, the date will be printed in year, month, then day order, then the time, in hours, minutes, then seconds order
		@param[in] dateTime
		    Time date/time.
		@param[in] format
		    Time format.
		@return
		    Formatted string.
		*/
		DatabaseExport std::wstring Print( const DateTimeType & dateTime, const std::wstring & format );

		/** Check date/time consistency.
		@param[in] dateTime
			String containing date/time.
		@param[in] format
			Format of date/time in string.
		@return
			true if valid date.
		*/
		DatabaseExport bool IsDateTime( const std::string & dateTime, const std::string & format );

		/** Check date/time consistency.
		@param[in] dateTime
			String containing date/time.
		@param[in] format
			Format of date/time in string.
		@return
			true if valid date.
		*/
		DatabaseExport bool IsDateTime( const std::wstring & dateTime, const std::wstring & format );

		/** Check date/time consistency.
		@param[in] dateTime
			String containing date/time.
		@param[in] format
			Format of date/time in string.
		@param[out] result
			Resulting object.
		@return
			true if valid date.
		*/
		DatabaseExport bool IsDateTime( const std::string & dateTime, const std::string & format, DateTimeType & result );

		/** Check date/time consistency.
		@param[in] dateTime
			String containing date/time.
		@param[in] format
			Format of date/time in string.
		@param[out] result
			Resulting object.
		@return
			true if valid date.
		*/
		DatabaseExport bool IsDateTime( const std::wstring & dateTime, const std::wstring & format, DateTimeType & result );

		/** Tells if the given date/time is valid or not
		@param[in] dateTime
			The date/time to test
		@return
			The validity
		*/
		DatabaseExport bool IsValid( const DateTimeType & dateTime );
	}
}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_DATE_TIME_H___
