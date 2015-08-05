/************************************************************************//**
* @file DateTimeHelper.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/19/2014 3:03:13 PM
*
*
* @brief Helping functions for date/time management.
*
* @details Helping functions for date/time management.
*
***************************************************************************/

#ifndef ___DATABASE_DATE_TIME_HELPER_H___
#define ___DATABASE_DATE_TIME_HELPER_H___

#include "DatabasePrerequisites.h"

BEGIN_NAMESPACE_DATABASE
{
	static int MonthMaxDays[13] = { -1, 31, 28, 31, 30, 31 , 30 , 31 , 31 , 30 , 31 , 30 , 31 };

	/** Check if parameter number is a leap year.
	@param year
		Year to test.
	@return
		true if leap year.
	*/
	DatabaseExport bool IsLeap( int year );

	/** Retrieves count characters from the given string buffer and converts them to an integer
	@remarks
		The buffer is then offseted of count characters
	@param[in, out] in
		The buffer
	@param[in] count
		The number of characters to retrieve
	*/
	template< typename CharType >
	int stoi( CharType const *& in, size_t count )
	{
		if ( *in == '-' )
		{
			++count;
		}

		int result = std::stoi( std::basic_string< CharType >( in, in + count ) );
		in += count;
		return result;
	}

	/** Retrieves the date from a dCDateTime
	@param[i] dateTime
		The DateTimeType
	@return
		The date
	*/
	DatabaseExport DateType DateFromDateTime( DateTimeType const & dateTime );

	/** Retrieves the time from a dCDateTime
	@param[i] dateTime
		The DateTimeType
	@return
		The time
	*/
	DatabaseExport TimeType TimeFromDateTime( DateTimeType const & dateTime );
}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_DATE_TIME_HELPER_H___
