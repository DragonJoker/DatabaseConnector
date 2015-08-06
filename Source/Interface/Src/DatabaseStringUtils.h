/************************************************************************//**
* @file DatabaseStringUtils.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief String helper functions
*
***************************************************************************/

#ifndef ___DATABASE_STRING_UTILS_H___
#define ___DATABASE_STRING_UTILS_H___

#include "DatabasePrerequisites.h"

BEGIN_NAMESPACE_DATABASE
{
	/** String functions class
	*/
	namespace StringUtils
	{
		/** Tests if the given string is upper case
		@param[in] toTest
			The string to test
		@return
			\p true if p_strToTest is upper case
		*/
		DatabaseExport bool IsUpperCase( const std::string & toTest );

		/** Tests if the given string is lower case
		@param[in] toTest
			The string to test
		@return
			\p true if p_strToTest is lower case
		*/
		DatabaseExport bool IsLowerCase( const std::string & toTest );

		/** Tests if the given string is upper case
		@param[in] toTest
			The string to test
		@return
			\p true if p_strToTest is upper case
		*/
		DatabaseExport bool IsUpperCase( const std::wstring & toTest );

		/** Tests if the given string is lower case
		@param[in] toTest
			The string to test
		@return
			\p true if p_strToTest is lower case
		*/
		DatabaseExport bool IsLowerCase( const std::wstring & toTest );

		/** Sets all characters in a string to upper case
		@param[in] str
			The string to modify
		@return
			The modified String
		*/
		DatabaseExport std::string UpperCase( const std::string & str );

		/** Sets all characters in a string to lower case
		@param[in] str
			The string to modify
		@return
			The modified String
		*/
		DatabaseExport std::string LowerCase( const std::string & str );

		/** Sets all characters in a string to upper case
		@param[in] str
			The string to modify
		@return
			The modified String
		*/
		DatabaseExport std::wstring UpperCase( const std::wstring & str );

		/** Sets all characters in a string to lower case
		@param[in] str
			The string to modify
		@return
			The modified String
		*/
		DatabaseExport std::wstring LowerCase( const std::wstring & str );

		/** Sets all characters in a string to upper case
		@param[in,out] str
			The string to modify, receives the modified string
		@return
			The modified String
		*/
		DatabaseExport std::string & ToUpperCase( std::string & str );

		/** Sets all characters in a string to lower case
		@param[in,out] str
			The string to modify, receives the modified string
		@return
			The modified String
		*/
		DatabaseExport std::string & ToLowerCase( std::string & str );

		/** Sets all characters in a string to upper case
		@param[in,out] str
			The string to modify, receives the modified string
		@return
			The modified String
		*/
		DatabaseExport std::wstring & ToUpperCase( std::wstring & str );

		/** Sets all characters in a string to lower case
		@param[in,out] str
			The string to modify, receives the modified string
		@return
			The modified String
		*/
		DatabaseExport std::wstring & ToLowerCase( std::wstring & str );

		/** Cuts a string into substrings, using delimiter(s)
		@param[in] str
			The string to cut
		@param[in] p_delims
			The delimiter(s)
		@param[in] p_maxSplits
			The max splits count (the return will contain 0 < x < p_maxSplits substrings)
		@param[in] p_bKeepVoid
			Tells if the function keeps void substrings or not
		@return
			The array containing the substrings
		*/
		DatabaseExport std::vector< std::string > Split( const std::string & str, const std::string & p_delims, uint32_t p_maxSplits = 10, bool p_bKeepVoid = true );

		/** Cuts a string into substrings, using delimiter(s)
		@param[in] str
			The string to cut
		@param[in] p_delims
			The delimiter(s)
		@param[in] p_maxSplits
			The max splits count (the return will contain 0 < x < p_maxSplits substrings)
		@param[in] p_bKeepVoid
			Tells if the function keeps void substrings or not
		@return
			The array containing the substrings
		*/
		DatabaseExport std::vector< std::wstring > Split( const std::wstring & str, const std::wstring & p_delims, uint32_t p_maxSplits = 10, bool p_bKeepVoid = true );

		/** Removes spaces on the left and/or on the right of the given string
		@param[in,out] str
		 	The string to trim, receives the trimmed string
		@param[in] p_bLeft
		 	Tells if we remove the left spaces
		@param[in] p_bRight
		 	Tells if we remove the right spaces
		@return
			The trimmed String
		*/
		DatabaseExport std::string & Trim( std::string & str, bool p_bLeft = true, bool p_bRight = true );

		/** Removes spaces on the left and/or on the right of the given string
		@param[in,out] str
		 	The string to trim, receives the trimmed string
		@param[in] p_bLeft
		 	Tells if we remove the left spaces
		@param[in] p_bRight
		 	Tells if we remove the right spaces
		@return
			The trimmed String
		*/
		DatabaseExport std::wstring & Trim( std::wstring & str, bool p_bLeft = true, bool p_bRight = true );

		/** Replaces all occurences of a TChar by another one in a string
		@param[in,out] str
		 	The string to modify
		@param[in] find
		 	The value to replace
		@param[in] replacement
		 	The replacement value
		@return
			A reference on the modified string
		*/
		DatabaseExport std::string & Replace( std::string & str, char find, char replacement );

		/** Replaces all occurences of a string by a TChar in a string
		@param[in,out] str
		 	The string to modify
		@param[in] find
		 	The value to replace
		@param[in] replacement
		 	The replacement value
		@return
			A reference on the modified string
		*/
		DatabaseExport std::string & Replace( std::string & str, const std::string & find, char replacement );

		/** Replaces all occurences of a TChar by a string in a string
		@param[in,out] str
		 	The string to modify
		@param[in] find
		 	The value to replace
		@param[in] replacement
		 	The replacement value
		@return
			A reference on the modified string
		*/
		DatabaseExport std::string & Replace( std::string & str, char find, const std::string & replacement );

		/** Replaces all occurences of a string in another one by a third one
		@param[in,out] str
		 	The string to modify
		@param[in] find
		 	The value to replace
		@param[in] replacement
		 	The replacement value
		@return
			A reference on the modified string
		*/
		DatabaseExport std::string & Replace( std::string & str, const std::string & find, const std::string & replacement );

		/** Replaces all occurences of a TChar by another one in a string
		@param[in,out] str
		 	The string to modify
		@param[in] find
		 	The value to replace
		@param[in] replacement
		 	The replacement value
		@return
			A reference on the modified string
		*/
		DatabaseExport std::wstring & Replace( std::wstring & str, wchar_t find, wchar_t replacement );

		/** Replaces all occurences of a string by a TChar in a string
		@param[in,out] str
		 	The String to modify
		@param[in] find
		 	The value to replace
		@param[in] replacement
		 	The replacement value
		@return
			A reference on the modified string
		*/
		DatabaseExport std::wstring & Replace( std::wstring & str, const std::wstring & find, wchar_t replacement );

		/** Replaces all occurences of a TChar by a string in a string
		@param[in,out] str
		 	The string to modify
		@param[in] find
		 	The value to replace
		@param[in] replacement
		 	The replacement value
		@return
			A reference on the modified string
		*/
		DatabaseExport std::wstring & Replace( std::wstring & str, wchar_t find, const std::wstring & replacement );

		/** Replaces all occurences of a string in another one by a third one
		@param[in,out] str
		 	The String to modify
		@param[in] find
		 	The value to replace
		@param[in] replacement
		 	The replacement value
		@return
			A reference on the modified string
		*/
		DatabaseExport std::wstring & Replace( std::wstring & str, const std::wstring & find, const std::wstring & replacement );

		/** Format a string.
		@param[out] formattedString
			Formatted string.
		@param[in] maxSize
			Maximum length.
		@param[in] format
			Format to use.
		*/
		DatabaseExport void Formalize( std::string & formattedString, int maxSize, const char * format, ... );

		/** Format a string.
		@param[out] formattedString
			Formatted string.
		@param[in] maxSize
			Maximum length.
		@param[in] format
			Format to use.
		*/
		DatabaseExport void Formalize( std::wstring & formattedString, int maxSize, const wchar_t * format, ... );

		/** Puts a value into a String
		@param[in] value
		 	The value
		@return
			The String containing the value
		*/
		template< typename T >
		inline String ToString( T const & value )
		{
			StringStream l_streamReturn;
			l_streamReturn <<  value;
			return l_streamReturn.str();
		}

		/** Sets the String value to the given char
		@param[in] src
		 	The char
		@return
			The String
		*/
		DatabaseExport String ToString( char src );

		/** Sets the String value to the given wchar_t
		@param[in] src
		 	The wchar_t
		@return
			The String
		*/
		DatabaseExport String ToString( wchar_t src );

		/** Converts a std::string in a given charset to a std::wstring in the same charset
		@param[in] src
			The string
		@param[in] charset
			The original string charset
		@return
			The std::string
		*/
		DatabaseExport std::wstring ToWStr( const std::string & src, const std::string & charset = "UTF-8" );

		/** Converts a std::wstring in a given charset to a std::string in the same charset
		@param[in] src
			The string
		@param[in] charset
			The original string charset
		@return
			The std::string
		*/
		DatabaseExport std::string ToStr( const std::wstring & src, const std::string & charset = "UTF-8" );

		/** Converts a std::string in a given charset into an UTF-8 string
		@param[in] src
			The original string
		@param[in] charset
			The original string charset
		*/
		DatabaseExport std::string ToUtf8( const std::string & src, const std::string & charset );

		/** Converts a std::wstring in a given charset into an UTF-8 string
		@param[in] src
			The original string
		@param[in] charset
			The original string charset
		*/
		DatabaseExport std::string ToUtf8( const std::wstring & src, const std::string & charset );
	}

	/** An empty String
	*/
	static const String cuEmptyString;

	/** Stream operator
	@param stream
		The stream.
	@param vector
		The vector.
	@return
		The stream.
	*/
	DatabaseExport std::ostream & operator <<( std::ostream & stream, const NAMESPACE_DATABASE::ByteArray & vector );

	/** Stream operator
	@param stream
		The stream.
	@param vector
		The vector.
	@return
		The stream.
	*/
	DatabaseExport std::wostream & operator <<( std::wostream & stream, const NAMESPACE_DATABASE::ByteArray & vector );

	/** Stream operator
	@param stream
		The stream.
	@param string
		The string.
	@return
		The stream.
	*/
	DatabaseExport std::ostream & operator <<( std::ostream & stream, const std::wstring & string );

	/** Stream operator
	@param stream
		The stream.
	@param string
		The string.
	@return
		The stream.
	*/
	DatabaseExport std::ostream & operator <<( std::ostream & stream, wchar_t const * string );
}
END_NAMESPACE_DATABASE

#endif
