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
		/** Tests if the given String is upper case
		@param[in] p_strToTest
			The String to test
		@return
			\p true if p_strToTest is upper case
		*/
		DatabaseExport bool IsUpperCase( std::string const & p_strToTest );

		/** Tests if the given String is lower case
		@param[in] p_strToTest
			The String to test
		@return
			\p true if p_strToTest is lower case
		*/
		DatabaseExport bool IsLowerCase( std::string const & p_strToTest );

		/** Tests if the given String is upper case
		@param[in] p_strToTest
			The String to test
		@return
			\p true if p_strToTest is upper case
		*/
		DatabaseExport bool IsUpperCase( std::wstring const & p_strToTest );

		/** Tests if the given String is lower case
		@param[in] p_strToTest
			The String to test
		@return
			\p true if p_strToTest is lower case
		*/
		DatabaseExport bool IsLowerCase( std::wstring const & p_strToTest );

		/** Sets all characters in a String to upper case
		@param[in] p_str
			The String to modify
		@return
			The modified String
		*/
		DatabaseExport std::string UpperCase( std::string const & p_str );

		/** Sets all characters in a String to lower case
		@param[in] p_str
			The String to modify
		@return
			The modified String
		*/
		DatabaseExport std::string LowerCase( std::string const & p_str );

		/** Sets all characters in a String to upper case
		@param[in] p_str
			The String to modify
		@return
			The modified String
		*/
		DatabaseExport std::wstring UpperCase( std::wstring const & p_str );

		/** Sets all characters in a String to lower case
		@param[in] p_str
			The String to modify
		@return
			The modified String
		*/
		DatabaseExport std::wstring LowerCase( std::wstring const & p_str );

		/** Sets all characters in a String to upper case
		@param[in,out] p_str
			The String to modify, receives the modified String
		@return
			The modified String
		*/
		DatabaseExport std::string & ToUpperCase( std::string & p_str );

		/** Sets all characters in a String to lower case
		@param[in,out] p_str
			The String to modify, receives the modified String
		@return
			The modified String
		*/
		DatabaseExport std::string & ToLowerCase( std::string & p_str );

		/** Sets all characters in a String to upper case
		@param[in,out] p_str
			The String to modify, receives the modified String
		@return
			The modified String
		*/
		DatabaseExport std::wstring & ToUpperCase( std::wstring & p_str );

		/** Sets all characters in a String to lower case
		@param[in,out] p_str
			The String to modify, receives the modified String
		@return
			The modified String
		*/
		DatabaseExport std::wstring & ToLowerCase( std::wstring & p_str );

		/** Retrieves a std::wstring from a String
		@param[in] p_str
			The String
		@return
			The std::string
		*/
		DatabaseExport std::wstring ToWStr( std::string const & p_str );

		/** Retrieves a std::string from a String
		@param[in] p_str
			The String
		@return
			The std::string
		*/
		DatabaseExport std::string ToStr( std::wstring const & p_str );

		/** Cuts a String into substrings, using delimiter(s)
		@param[in] p_str
			The String to cut
		@param[in] p_delims
			The delimiter(s)
		@param[in] p_maxSplits
			The max splits count (the return will contain 0 < x < p_maxSplits substrings)
		@param[in] p_bKeepVoid
			Tells if the function keeps void substrings or not
		@return
			The array containing the substrings
		*/
		DatabaseExport std::vector< std::string > Split( std::string const & p_str, std::string const & p_delims, uint32_t p_maxSplits = 10, bool p_bKeepVoid = true );

		/** Cuts a String into substrings, using delimiter(s)
		@param[in] p_str
			The String to cut
		@param[in] p_delims
			The delimiter(s)
		@param[in] p_maxSplits
			The max splits count (the return will contain 0 < x < p_maxSplits substrings)
		@param[in] p_bKeepVoid
			Tells if the function keeps void substrings or not
		@return
			The array containing the substrings
		*/
		DatabaseExport std::vector< std::wstring > Split( std::wstring const & p_str, std::wstring const & p_delims, uint32_t p_maxSplits = 10, bool p_bKeepVoid = true );

		/** Removes spaces on the left and/or on the right of the given String
		@param[in,out] p_str
		 	The String to trim, receives the trimmed string
		@param[in] p_bLeft
		 	Tells if we remove the left spaces
		@param[in] p_bRight
		 	Tells if we remove the right spaces
		@return
			The trimmed String
		*/
		DatabaseExport std::string & Trim( std::string & p_str, bool p_bLeft = true, bool p_bRight = true );

		/** Removes spaces on the left and/or on the right of the given String
		@param[in,out] p_str
		 	The String to trim, receives the trimmed string
		@param[in] p_bLeft
		 	Tells if we remove the left spaces
		@param[in] p_bRight
		 	Tells if we remove the right spaces
		@return
			The trimmed String
		*/
		DatabaseExport std::wstring & Trim( std::wstring & p_str, bool p_bLeft = true, bool p_bRight = true );

		/** Replaces all occurences of a TChar by another one in a String
		@param[in,out] p_str
		 	The String to modify
		@param[in] p_find
		 	The TChar to replace
		@param[in] p_replacement
		 	The replacement TChar
		@return
			A reference on the modified string
		*/
		DatabaseExport std::string & Replace( std::string & p_str, char p_find, char p_replacement );

		/** Replaces all occurences of a String by a TChar in a String
		@param[in,out] p_str
		 	The String to modify
		@param[in] p_find
		 	The String to replace
		@param[in] p_replacement
		 	The replacement TChar
		@return
			A reference on the modified string
		*/
		DatabaseExport std::string & Replace( std::string & p_str, std::string const & p_find, char p_replacement );

		/** Replaces all occurences of a TChar by a String in a String
		@param[in,out] p_str
		 	The String to modify
		@param[in] p_find
		 	The TChar to replace
		@param[in] p_replacement
		 	The replacement String
		@return
			A reference on the modified string
		*/
		DatabaseExport std::string & Replace( std::string & p_str, char p_find, std::string const & p_replacement );

		/** Replaces all occurences of a String in another one by a third one
		@param[in,out] p_str
		 	The String to modify
		@param[in] p_find
		 	The String to replace
		@param[in] p_replacement
		 	The replacement String
		@return
			A reference on the modified string
		*/
		DatabaseExport std::string & Replace( std::string & p_str, std::string const & p_find, std::string const & p_replacement );

		/** Replaces all occurences of a TChar by another one in a String
		@param[in,out] p_str
		 	The String to modify
		@param[in] p_find
		 	The TChar to replace
		@param[in] p_replacement
		 	The replacement TChar
		@return
			A reference on the modified string
		*/
		DatabaseExport std::wstring & Replace( std::wstring & p_str, wchar_t p_find, wchar_t p_replacement );

		/** Replaces all occurences of a String by a TChar in a String
		@param[in,out] p_str
		 	The String to modify
		@param[in] p_find
		 	The String to replace
		@param[in] p_replacement
		 	The replacement TChar
		@return
			A reference on the modified string
		*/
		DatabaseExport std::wstring & Replace( std::wstring & p_str, std::wstring const & p_find, wchar_t p_replacement );

		/** Replaces all occurences of a TChar by a String in a String
		@param[in,out] p_str
		 	The String to modify
		@param[in] p_find
		 	The TChar to replace
		@param[in] p_replacement
		 	The replacement String
		@return
			A reference on the modified string
		*/
		DatabaseExport std::wstring & Replace( std::wstring & p_str, wchar_t p_find, std::wstring const & p_replacement );

		/** Replaces all occurences of a String in another one by a third one
		@param[in,out] p_str
		 	The String to modify
		@param[in] p_find
		 	The String to replace
		@param[in] p_replacement
		 	The replacement String
		@return
			A reference on the modified string
		*/
		DatabaseExport std::wstring & Replace( std::wstring & p_str, std::wstring const & p_find, std::wstring const & p_replacement );

		/** Retrieves a std::wstring from a String
		@param[in] p_str
			The String
		@return
			The std::string
		*/
		inline std::wstring ToWStr( std::wstring const & p_str )
		{
			return p_str;
		}

		/** Retrieves a std::string from a String
		@param[in] p_str
			The String
		@return
			The std::string
		*/
		inline std::string ToStr( std::string const & p_str )
		{
			return p_str;
		}

		/** Puts a value into a String
		@param[in] p_tValue
		 	The value
		@return
			The String containing the value
		*/
		template< typename T >
		inline String ToString( T const & p_tValue )
		{
			StringStream l_streamReturn;
			l_streamReturn <<  p_tValue;
			return l_streamReturn.str();
		}

		/** Sets the String value to the given std::string
		@param[in] p_strText
		 	The std::string
		@return
			The String
		*/
		DatabaseExport String ToString( std::string const & p_strText );

		/** Sets the String value to the given std::wstring
		@param[in] p_strText
		 	The std::wstring
		@return
			The String
		*/
		DatabaseExport String ToString( std::wstring const & p_strText );

		/** Sets the String value to the given char
		@param[in] p_char
		 	The char
		@return
			The String
		*/
		DatabaseExport String ToString( char p_char );

		/** Sets the String value to the given wchar_t
		@param[in] p_wchar
		 	The wchar_t
		@return
			The String
		*/
		DatabaseExport String ToString( wchar_t p_wchar );

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

		/** Converts a string in a given charset into an UTF-8 string
		@param src
			The original string
		@param charset
			The original string charset
		*/
		DatabaseExport std::string ToUtf8( const std::string & src, const std::string & charset );

		/** Converts a string in a given charset into an UTF-8 string
		@param src
			The original string
		@param charset
			The original string charset
		*/
		DatabaseExport std::string ToUtf8( const std::wstring & src, const std::wstring & charset );
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
	DatabaseExport std::ostream & operator <<( std::ostream & stream, std::wstring const & string );

	/** Stream operator
	@param stream
		The stream.
	@param string
		The string.
	@return
		The stream.
	*/
	DatabaseExport std::ostream & operator <<( std::ostream & stream, wchar_t const * string );

	namespace
	{
		/** Helper structure used to cast a string for one char type to another char type
		*/
		template< typename CharOut, typename CharIn > struct StringCaster;

		/** @copydoc Database::StringCaster
		@remarks Specialisation for no convertion
		*/
		template< typename Char >
		struct StringCaster< Char, Char >
		{
			static inline std::basic_string< Char > Cast( const Char * in )
			{
				return std::basic_string< Char >( in );
			}
		};

		/** @copydoc Database::StringCaster
		@remarks Specialisation for convertion to wchar_t
		*/
		template<>
		struct StringCaster< wchar_t, char >
		{
			static inline std::wstring Cast( const char * in )
			{
				return StringUtils::ToWStr( std::string( in, in + strlen( in ) ) );
			}
		};

		/** @copydoc Database::StringCaster
		@remarks Specialisation for convertion to char
		*/
		template<>
		struct StringCaster< char, wchar_t >
		{
			static inline std::string Cast( const wchar_t * in )
			{
				return StringUtils::ToStr( std::wstring( in, in + wcslen( in ) ) );
			}
		};
	}

	/** Helper function used to convert an input char type to an output char type
	@param[in] in
		The input string
	@return
		The output string
	*/
	template< typename CharOut, typename CharIn > std::basic_string< CharOut > StringCast( const CharIn * in )
	{
		return StringCaster< CharOut, CharIn >::Cast( in );
	}

	/** Helper function used to convert an input char type to an output char type
	@param[in] in
		The input string
	@return
		The output string
	*/
	template< typename CharOut, typename CharIn >
	inline std::basic_string< CharOut > StringCast( const std::basic_string< CharIn > & in )
	{
		return StringCast< CharOut >( in.c_str() );
	}
}
END_NAMESPACE_DATABASE

#endif
