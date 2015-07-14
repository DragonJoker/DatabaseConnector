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
	static const double GREGORIAN_MEAN_YEAR = 365.2425;

	/** Check if parameter number is a leap year.
	@param year
		Year to test.
	@return
		true if leap year.
	*/
	DatabaseExport bool IsLeap( int year );

	/** Check if a string is an integer.
	@param value
		String to check.
	@return
		true if value represents a string.
	*/
	DatabaseExport bool IsInteger( const std::string & value );

	/** Replace a sub-string by another sub-string into a string.
	@param[in,out] originalString
		String where the replacement is made.
	@param[in] searchedSubString
		String to replace.
	@param[in] replacementSubString
		Replacement string.
	*/
	DatabaseExport int Replace( std::string & originalString, const std::string & searchedSubString, const std::string & replacementSubString );

	/** Format a string.
	@param[out] formattedString
		Formatted string.
	@param[in] maxSize
		Maximum length.
	@param[in] format
		Format to use.
	*/
	DatabaseExport void Formalize( std::string & formattedString, int maxSize, const char * format, ... );

	/** Check if a string is an integer.
	@param value
		String to check.
	@return
		true if value represents a string.
	*/
	DatabaseExport bool IsInteger( const std::wstring & value );

	/** Replace a sub-string by another sub-string into a string.
	@param[in,out] originalString
		String where the replacement is made.
	@param[in] searchedSubString
		String to replace.
	@param[in] replacementSubString
		Replacement string.
	*/
	DatabaseExport int Replace( std::wstring & originalString, const std::wstring & searchedSubString, const std::wstring & replacementSubString );

	/** Format a string.
	@param[out] formattedString
		Formatted string.
	@param[in] maxSize
		Maximum length.
	@param[in] format
		Format to use.
	*/
	DatabaseExport void Formalize( std::wstring & formattedString, int maxSize, const wchar_t * format, ... );
}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_DATE_TIME_HELPER_H___
