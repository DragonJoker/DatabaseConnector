/************************************************************************//**
* @file Time.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief Time helper functions
*
***************************************************************************/

#ifndef ___DATABASE_TIME_H___
#define ___DATABASE_TIME_H___

#include "DatabasePrerequisites.h"

BEGIN_NAMESPACE_DATABASE
{
	namespace Time
	{

		/** Get time formatted according to the format mask.
		@remarks
			Uses strftime
		@param[in] format
			Time format.
		@return
			Formatted string.
		*/
		DatabaseExport std::string Format( const TimeType & time, const std::string & format );

		/** Get time formatted according to the format mask.
		@remarks
			Uses wcsftime
		@param[in] format
			Time format.
		@return
			Formatted string.
		*/
		DatabaseExport std::wstring Format( const TimeType & time, const std::wstring & format );

		/** Get time formatted according to the format mask.
		@remarks
			Uses vsnprintf, the time, in hours, minutes, then seconds order
		@param[in] time
		    Time time.
		@param[in] format
		    Time format.
		@return
		    Formatted string.
		*/
		DatabaseExport std::string Print( const TimeType & time, const std::string & format );

		/** Get time formatted according to the format mask.
		@remarks
			Uses vsnprintf, the time, in hours, minutes, then seconds order
		@param[in] time
		    Time time.
		@param[in] format
		    Time format.
		@return
		    Formatted string.
		*/
		DatabaseExport std::wstring Print( const TimeType & time, const std::wstring & format );

		/** Check time consistency.
		@param[in] time
			String containing time.
		@param[in] format
			Format of time in string.
		@return
			true if valid time.
		*/
		DatabaseExport bool IsTime( const std::string & time, const std::string & format );

		/** Check time consistency.
		@param[in] time
			String containing time.
		@param[in] format
			Format of time in string.
		@return
			true if valid time.
		*/
		DatabaseExport bool IsTime( const std::wstring & time, const std::wstring & format );

		/** Check time consistency.
		@param[in] time
			String containing time.
		@param[in] format
			Format of time in string.
		@param[out] result
			Resulting object.
		@return
			true if valid time.
		*/
		DatabaseExport bool IsTime( const std::string & time, const std::string & format, TimeType & result );

		/** Check time consistency.
		@param[in] time
			String containing time.
		@param[in] format
			Format of time in string.
		@param[out] result
			Resulting object.
		@return
			true if valid time.
		*/
		DatabaseExport bool IsTime( const std::wstring & time, const std::wstring & format, TimeType & result );

		/** Tells if the given time is valid or not
		@param[in] time
			The time to test
		@return
			The validity
		*/
		DatabaseExport bool IsValid( const TimeType & time );
	};
}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_TIME_H___
