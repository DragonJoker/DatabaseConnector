/************************************************************************//**
* @file Date.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief Date helper functions.
*
***************************************************************************/

#ifndef ___DATABASE_DATE_H___
#define ___DATABASE_DATE_H___

#include "DatabasePrerequisites.h"

BEGIN_NAMESPACE_DATABASE
{
	/** Describes a date.
	*/
	namespace Date
	{
		/** Get date formatted according to the format mask.
		@remarks
			Uses strftime
		@param[in] date
		    Time date.
		@param[in] format
		    Time format.
		@return
		    Formatted string.
		*/
		DatabaseExport std::string Format( const DateType & date, const std::string & format );

		/** Get date formatted according to the format mask.
		@remarks
			Uses wcsftime
		@param[in] date
		    Time date.
		@param[in] format
		    Time format.
		@return
		    Formatted string.
		*/
		DatabaseExport std::wstring Format( const DateType & date, const std::wstring & format );

		/** Get date formatted according to the format mask.
		@remarks
			Uses vsnprintf, the date will be printed in year, month, then day order
		@param[in] date
		    Time date.
		@param[in] format
		    Time format.
		@return
		    Formatted string.
		*/
		DatabaseExport std::string Print( const DateType & date, const std::string & format );

		/** Get date formatted according to the format mask.
		@remarks
			Uses vsnprintf, the date will be printed in year, month, then day order
		@param[in] date
		    Time date.
		@param[in] format
		    Time format.
		@return
		    Formatted string.
		*/
		DatabaseExport std::wstring Print( const DateType & date, const std::wstring & format );

		/** Check date consistency.
		@param[in] date
		    String containing date.
		@param[in] format
		    Format of date in string.
		@return
		    true if valid date.
		*/
		DatabaseExport bool IsDate( const std::string & date, const std::string & format );

		/** Check date consistency.
		@param[in] date
		    String containing date.
		@param[in] format
		    Format of date in string.
		@return
		    true if valid date.
		*/
		DatabaseExport bool IsDate( const std::wstring & date, const std::wstring & format );

		/** Check date consistency.
		@param[in] date
		    String containing date.
		@param[in] format
		    Format of date in string.
		@param[out] result
		    Resulting object.
		@return
		    true if valid date.
		*/
		DatabaseExport bool IsDate( const std::string & date, const std::string & format, DateType & result );

		/** Check date consistency.
		@param[in] date
		    String containing date.
		@param[in] format
		    Format of date in string.
		@param[out] result
		    Resulting object.
		@return
		    true if valid date.
		*/
		DatabaseExport bool IsDate( const std::wstring & date, const std::wstring & format, DateType & result );

		/** Tells if the given date is valid or not
		@param[in] date
			The date to test
		@return
			The validity
		*/
		DatabaseExport bool IsValid( const DateType & date );
	};
}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_DATE_H___
