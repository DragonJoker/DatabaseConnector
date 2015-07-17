/************************************************************************//**
* @file DatabaseStringUtils.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief String functions class
*
***************************************************************************/

#ifndef ___DATABASE_STRING_UTILS_H___
#define ___DATABASE_STRING_UTILS_H___

#include "DatabasePrerequisites.h"

BEGIN_NAMESPACE_DATABASE
{
	/** String functions class
	*/
	class CStrUtils
	{
	public:
		/** Tests if the given String represents an integer
		@param[in] p_strToTest
			The String to test
		@param[in] p_locale
			The locale (unused)
		@return
			\p false if p_strToTest contains any character other than '0123456789'
		*/
		DatabaseExport static bool IsInteger( String const & p_strToTest, std::locale const & p_locale = std::locale() );

		/** Tests if the given String represents a floating number
		@param[in] p_strToTest
			The String to test
		@param[in] p_locale
			The locale used to determine the dot format
		@return
			\p false if p_strToTest contains any character other than '0123456789.,' or more than one '.'/','
		*/
		DatabaseExport static bool IsFloating( String const & p_strToTest, std::locale const & p_locale = std::locale() );

		/** Retrieves a short from a given String
		@param[in] p_str
			The String
		@param[in] p_locale
			The locale used in the conversion
		@return
			The short contained in the String
		*/
		DatabaseExport static short ToShort( String const & p_str, std::locale const & p_locale = std::locale() );

		/** Retrieves an int from a given String
		@param[in] p_str
			The String
		@param[in] p_locale
			The locale used in the conversion
		@return
			The int contained in the String
		*/
		DatabaseExport static int ToInt( String const & p_str, std::locale const & p_locale = std::locale() );

		/** Retrieves a long from a given String
		@param[in] p_str
			The String
		@param[in] p_locale
			The locale used in the conversion
		@return
			The long contained in the String
		*/
		DatabaseExport static long ToLong( String const & p_str, std::locale const & p_locale = std::locale() );

		/** Retrieves a long long from a given String
		@param[in] p_str
			The String
		@param[in] p_locale
			The locale used in the conversion
		@return
			The long long contained in the String
		*/
		DatabaseExport static long long ToLongLong( String const & p_str, std::locale const & p_locale = std::locale() );

		/** Retrieves a float from a given String
		@param[in] p_str
			The String
		@param[in] p_locale
			The locale used in the conversion
		@return
			The float contained in the String
		*/
		DatabaseExport static float ToFloat( String const & p_str, std::locale const & p_locale = std::locale() );

		/** Retrieves a double from a given String
		@param[in] p_str
			The String
		@param[in] p_locale
			The locale used in the conversion
		@return
			The double contained in the String
		*/
		DatabaseExport static double ToDouble( String const & p_str, std::locale const & p_locale = std::locale() );

		/** Retrieves a double from a given String
		@param[in] p_str
			The String
		@param[in] p_locale
			The locale used in the conversion
		@return
			The double contained in the String
		*/
		DatabaseExport static long double ToLongDouble( String const & p_str, std::locale const & p_locale = std::locale() );

		/** Tests if the given String is upper case
		@param[in] p_strToTest
			The String to test
		@return
			\p true if p_strToTest is upper case
		*/
		DatabaseExport static bool IsUpperCase( std::string const & p_strToTest );

		/** Tests if the given String is lower case
		@param[in] p_strToTest
			The String to test
		@return
			\p true if p_strToTest is lower case
		*/
		DatabaseExport static bool IsLowerCase( std::string const & p_strToTest );

		/** Tests if the given String is upper case
		@param[in] p_strToTest
			The String to test
		@return
			\p true if p_strToTest is upper case
		*/
		DatabaseExport static bool IsUpperCase( std::wstring const & p_strToTest );

		/** Tests if the given String is lower case
		@param[in] p_strToTest
			The String to test
		@return
			\p true if p_strToTest is lower case
		*/
		DatabaseExport static bool IsLowerCase( std::wstring const & p_strToTest );

		/** Sets all characters in a String to upper case
		@param[in] p_str
			The String to modify
		@return
			The modified String
		*/
		DatabaseExport static std::string UpperCase( std::string const & p_str );

		/** Sets all characters in a String to lower case
		@param[in] p_str
			The String to modify
		@return
			The modified String
		*/
		DatabaseExport static std::string LowerCase( std::string const & p_str );

		/** Sets all characters in a String to upper case
		@param[in] p_str
			The String to modify
		@return
			The modified String
		*/
		DatabaseExport static std::wstring UpperCase( std::wstring const & p_str );

		/** Sets all characters in a String to lower case
		@param[in] p_str
			The String to modify
		@return
			The modified String
		*/
		DatabaseExport static std::wstring LowerCase( std::wstring const & p_str );

		/** Sets all characters in a String to upper case
		@param[in,out] p_str
			The String to modify, receives the modified String
		@return
			The modified String
		*/
		DatabaseExport static std::string & ToUpperCase( std::string & p_str );

		/** Sets all characters in a String to lower case
		@param[in,out] p_str
			The String to modify, receives the modified String
		@return
			The modified String
		*/
		DatabaseExport static std::string & ToLowerCase( std::string & p_str );

		/** Sets all characters in a String to upper case
		@param[in,out] p_str
			The String to modify, receives the modified String
		@return
			The modified String
		*/
		DatabaseExport static std::wstring & ToUpperCase( std::wstring & p_str );

		/** Sets all characters in a String to lower case
		@param[in,out] p_str
			The String to modify, receives the modified String
		@return
			The modified String
		*/
		DatabaseExport static std::wstring & ToLowerCase( std::wstring & p_str );

		/** Retrieves a std::string from a String
		@param[in] p_str
			The String
		@return
			The std::string
		*/
		DatabaseExport static std::string ToStr( std::string const & p_str )
		{
			return p_str;
		}

		/** Retrieves a std::wstring from a String
		@param[in] p_str
			The String
		@return
			The std::string
		*/
		DatabaseExport static std::wstring ToWStr( std::string const & p_str );

		/** Retrieves a std::string from a String
		@param[in] p_str
			The String
		@return
			The std::string
		*/
		DatabaseExport static std::string ToStr( std::wstring const & p_str );

		/** Retrieves a std::wstring from a String
		@param[in] p_str
			The String
		@return
			The std::string
		*/
		DatabaseExport static std::wstring ToWStr( std::wstring const & p_str )
		{
			return p_str;
		}

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
		DatabaseExport static StringArray Split( String const & p_str, String const & p_delims, uint32_t p_maxSplits = 10, bool p_bKeepVoid = true );

		/** Retrieves a value from the given String
		@param[in] p_str
			The String supposedly containing the value
		@param[out] p_val
			Receives the retrieved value
		@param[in] p_locale
			The locale used in the conversion
		*/
		template< typename T >
		static inline void Parse( String const & p_str, T & p_val )
		{
			StringStream l_out( p_str );
			l_out >> p_val;
		}

		/** Retrieves a value from the given String
		@param[in] p_str
			The String supposedly containing the value
		@param[in] p_locale
			The locale used in the conversion
		@param[out] p_val
			Receives the retrieved value
		*/
		template< typename T >
		static inline void Parse( String const & p_str, std::locale const & p_locale, T & p_val )
		{
			StringStream l_out( p_str );
			l_out.imbue( p_locale );
			l_out >> p_val;
		}

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
		DatabaseExport static String & Replace( String & p_str, TChar p_find, TChar p_replacement );

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
		DatabaseExport static String & Replace( String & p_str, String const & p_find, TChar p_replacement );

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
		DatabaseExport static String & Replace( String & p_str, TChar p_find, String const & p_replacement );

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
		DatabaseExport static String & Replace( String & p_str, String const & p_find, String const & p_replacement );

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
		DatabaseExport static String & Trim( String & p_str, bool p_bLeft = true, bool p_bRight = true );

		/** Puts a value into a String
		@param[in] p_tValue
		 	The value
		@return
			The String containing the value
		*/
		template< typename T >
		static String ToString( T const & p_tValue )
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
		DatabaseExport static String ToString( std::string const & p_strText );

		/** Sets the String value to the given std::wstring
		@param[in] p_strText
		 	The std::wstring
		@return
			The String
		*/
		DatabaseExport static String ToString( std::wstring const & p_strText );

		/** Sets the String value to the given char
		@param[in] p_char
		 	The char
		@return
			The String
		*/
		DatabaseExport static String ToString( char p_char );

		/** Sets the String value to the given wchar_t
		@param[in] p_wchar
		 	The wchar_t
		@return
			The String
		*/
		DatabaseExport static String ToString( wchar_t p_wchar );
	};

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
				return CStrUtils::ToWStr( std::string( in, in + strlen( in ) ) );
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
				return CStrUtils::ToStr( std::wstring( in, in + wcslen( in ) ) );
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

/** Stream operator, using String
@param[in,out] p_strStream
 	The String stream
@param[in] p_tVal
 	The value to put in the stream
@return
	The String stream
*/
template< typename T >
inline NAMESPACE_DATABASE::String & operator <<( NAMESPACE_DATABASE::String & p_strStream, T const & p_tVal )
{
	NAMESPACE_DATABASE::StringStream l_out;
	l_out << p_tVal;
	p_strStream += l_out.str();
	return p_strStream;
}

/** Stream operator, using String
@param[in,out] p_strStream
 	The String stream
@param[in] p_strVal
 	The value to put in the stream
@return
	The String stream
*/
inline NAMESPACE_DATABASE::String & operator <<( NAMESPACE_DATABASE::String & p_strStream, NAMESPACE_DATABASE::String const & p_strVal )
{
	p_strStream += p_strVal;
	return p_strStream;
}

/** Stream operator, using String
@param[in,out] p_strStream
 	The String stream
@param[in] p_tVal
 	The value to get from the stream
@return
	The String stream
*/
template< typename T >
inline NAMESPACE_DATABASE::String & operator >>( NAMESPACE_DATABASE::String & p_strStream, T & p_tVal )
{
	NAMESPACE_DATABASE::StringStream l_out( p_strStream );
	l_out >> p_tVal;
	p_strStream = l_out.str();
	return p_strStream;
}

#endif
