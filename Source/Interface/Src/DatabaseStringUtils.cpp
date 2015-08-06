/************************************************************************//**
* @file DatabaseStringUtils.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief String helper functions
*
***************************************************************************/

#include "DatabasePch.h"

#include "DatabaseStringUtils.h"

#include "DatabaseBlockGuard.h"
#include "DatabaseException.h"
#include "DatabaseLogger.h"

#include <boost/locale.hpp>

#include <clocale>

BEGIN_NAMESPACE_DATABASE
{
	namespace StringUtils
	{
		namespace detail
		{
			static const String ERROR_DB_FORMALIZE = STR( "Error while formatting: " );
			static const String ERROR_DB_NOT_IMPLEMENTED = STR( "Not implemented" );
			static const String ERROR_DB_CONVERSION = STR( "Error while performing a strings conversion: " );

			static const String INFO_UNKNOWN = STR( "UNKNOWN" );
			std::mutex g_conversionMutex;

#if defined( _MSC_VER )

			//template< typename OutChar, typename InChar >
			//std::basic_string< OutChar > str_convert( std::basic_string< InChar > const & src, const std::string & charset )
			//{
			//	std::basic_string< OutChar > dst;

			//	if ( !src.empty() )
			//	{
			//		typedef typename std::codecvt< OutChar, InChar, std::mbstate_t > facet_type;
			//		typedef typename facet_type::result result_type;

			//		std::mbstate_t state = std::mbstate_t();
			//		result_type result;
			//		std::vector< OutChar > buffer( src.size() + 1 );
			//		InChar const * endIn = NULL;
			//		OutChar * endOut = NULL;

			//		result = std::use_facet< facet_type >( boost::locale::generator().generate( charset ) ).in( state,
			//		src.data(), src.data() + src.size(), endIn,
			//		&buffer.front(), &buffer.front() + src.size(), endOut );

			//		dst = std::basic_string< OutChar >( &buffer.front(), endOut );
			//	}

			//	return dst;
			//}

			template< typename OutChar, typename InChar >
			std::basic_string< OutChar > str_convert( std::basic_string< InChar > const & )
			{
				static_assert( false, ERROR_DB_NOT_IMPLEMENTED );
			}

			template<>
			std::basic_string< wchar_t > str_convert< wchar_t, char >( std::basic_string< char > const & src )
			{
				std::basic_string< wchar_t > dst;

				if ( !src.empty() )
				{
					try
					{
						std::unique_lock< std::mutex > lock( g_conversionMutex );
						char * szloc = setlocale( LC_CTYPE, "" );
						mbtowc( NULL, NULL, 0 );
						size_t max = src.size();
						int length = 1;
						const char * in = src.c_str();
						dst.reserve( src.size() );

						while ( max > 0 && length >= 1 )
						{
							wchar_t wc;
							length = mbtowc( &wc, in, max );

							if ( length >= 1 )
							{
								dst += wc;
								max -= length;
								in += length;
							}
						}

						setlocale( LC_CTYPE, szloc );
					}
					catch ( std::exception & exc )
					{
						CLogger::LogError( StringStream() << ERROR_DB_CONVERSION << exc.what() );
					}
					catch ( ... )
					{
						CLogger::LogError( StringStream() << ERROR_DB_CONVERSION << INFO_UNKNOWN );
					}
				}

				return std::move( dst );
			}

			template<>
			std::basic_string< char > str_convert< char, wchar_t >( std::basic_string< wchar_t > const & src )
			{
				std::basic_string< char > dst;

				if ( !src.empty() )
				{
					try
					{
						std::unique_lock< std::mutex > lock( g_conversionMutex );
						std::string szloc = setlocale( LC_CTYPE, "" );
						size_t max = src.size();
						int length = 1;
						const wchar_t * in = src.c_str();
						char buffer[32] = { 0 };
						dst.reserve( 1 + src.size() * 2 );

						while ( *in && length >= 1 )
						{
							length = wctomb( buffer, *in );

							if ( length >= 1 )
							{
								dst += buffer;
								in++;
							}
						}

						setlocale( LC_CTYPE, szloc.c_str() );
					}
					catch ( std::exception & exc )
					{
						CLogger::LogError( StringStream() << ERROR_DB_CONVERSION << exc.what() );
					}
					catch ( ... )
					{
						CLogger::LogError( StringStream() << ERROR_DB_CONVERSION << INFO_UNKNOWN );
					}
				}

				return std::move( dst );
			}

#else

			template< typename OutChar, typename InChar >
			std::basic_string< OutChar > str_convert( std::basic_string< InChar > const & )
			{
				static_assert( false, "Not implemented" );
			}

			template<>
			std::basic_string< wchar_t > str_convert< wchar_t, char >( std::basic_string< char > const & src )
			{
				std::basic_string< wchar_t > dst;

				if ( !src.empty() )
				{
					std::unique_lock< std::mutex > lock( g_conversionMutex );
					char * szloc = setlocale( LC_CTYPE, "" );
					size_t size = mbstowcs( NULL, src.c_str(), 0 ) + 1;
					wchar_t * buffer = NULL;
					{
						auto guard = make_block_guard( [&buffer, &size]()
						{
							buffer = new wchar_t[size + 1];
						}, [&buffer]()
						{
							delete [] buffer;
						} );

						size = std::min( size, mbstowcs( buffer, src.c_str(), size ) );
						setlocale( LC_CTYPE, szloc );
						dst.assign( buffer, buffer + size );
					}
				}

				return dst;
			}

			template<>
			std::basic_string< char > str_convert< char, wchar_t >( std::basic_string< wchar_t > const & src )
			{
				std::basic_string< char > dst;

				if ( !src.empty() )
				{
					std::unique_lock< std::mutex > lock( g_conversionMutex );
					char * szloc = setlocale( LC_CTYPE, "" );
					size_t size = wcstombs( NULL, src.c_str(), 0 ) + 1;
					char * buffer = NULL;
					{
						auto guard = make_block_guard( [&buffer, &size]()
						{
							buffer = new char[size + 1];
						}, [&buffer]()
						{
							delete [] buffer;
						} );

						size = std::min( size, wcstombs( buffer, src.c_str(), size ) );
						setlocale( LC_CTYPE, szloc );
						dst.assign( buffer, buffer + size );
					}
				}

				return dst;
			}

#endif

			template< typename CharType >
			std::basic_string< CharType > & str_replace( std::basic_string< CharType > & p_str, std::basic_string< CharType > const & p_find, std::basic_string< CharType > const & p_replaced )
			{
				typedef std::basic_string< CharType > string_t;
				string_t l_return;
				std::size_t	l_currentPos = 0;
				std::size_t	l_pos = 0;

				while ( ( l_pos = p_str.find( p_find, l_currentPos ) ) != string_t::npos )
				{
					l_return.append( p_str.substr( l_currentPos, l_pos - l_currentPos ) );
					l_return.append( p_replaced );
					l_currentPos = l_pos + p_find.size();
				}

				if ( l_currentPos != p_str.size() )
				{
					l_return.append( p_str.substr( l_currentPos, l_pos - l_currentPos ) );
				}

				p_str = l_return;
				return p_str;
			}

			template< typename CharType >
			std::vector< std::basic_string< CharType > > str_split( std::basic_string< CharType > const & p_str, std::basic_string< CharType > const & p_delims, uint32_t p_maxSplits, bool p_bKeepVoid )
			{
				typedef std::basic_string< CharType > string_t;
				std::vector< string_t > l_arrayReturn;

				if ( ! p_str.empty() && ! p_delims.empty() && p_maxSplits > 0 )
				{
					l_arrayReturn.reserve( p_maxSplits + 1 );
					std::size_t l_numSplits = 0;
					std::size_t	l_pos = 0;
					std::size_t	l_start = 0;

					do
					{
						l_pos = p_str.find_first_of( p_delims, l_start );

						if ( l_pos == l_start )
						{
							l_start = l_pos + 1;

							if ( p_bKeepVoid )
							{
								l_arrayReturn.push_back( string_t() );
							}
						}
						else if ( l_pos == string_t::npos || l_numSplits == p_maxSplits )
						{
							string_t remnants = p_str.substr( l_start );

							if ( !remnants.empty() || p_bKeepVoid )
							{
								l_arrayReturn.push_back( remnants );
							}

							return l_arrayReturn;
						}
						else
						{
							l_arrayReturn.push_back( p_str.substr( l_start, l_pos - l_start ) );
							l_start = l_pos + 1;
						}

						//l_start = p_str.find_first_not_of( p_delims, l_start );
						++ l_numSplits;
					}
					while ( l_pos != string_t::npos );
				}

				return l_arrayReturn;
			}

			template< typename CharType >
			std::basic_string< CharType > & str_trim( std::basic_string< CharType > & p_str, bool p_bLeft, bool p_bRight )
			{
				typedef std::basic_string< CharType > string_t;

				if ( p_str.size() > 0 )
				{
					if ( p_bLeft )
					{
						std::size_t l_uiIndex = p_str.find_first_not_of( ' ' );

						if ( l_uiIndex > 0 )
						{
							if ( l_uiIndex != string_t::npos )
							{
								p_str = p_str.substr( l_uiIndex, string_t::npos );
							}
							else
							{
								p_str.clear();
							}
						}

						l_uiIndex = p_str.find_first_not_of( '\t' );

						if ( l_uiIndex > 0 )
						{
							if ( l_uiIndex != string_t::npos )
							{
								p_str = p_str.substr( l_uiIndex, string_t::npos );
							}
							else
							{
								p_str.clear();
							}
						}
					}

					if ( p_bRight && p_str.size() > 0 )
					{
						std::size_t l_uiIndex = p_str.find_last_not_of( ' ' );

						if ( l_uiIndex < p_str.size() - 1 )
						{
							if ( l_uiIndex != string_t::npos )
							{
								p_str = p_str.substr( 0, l_uiIndex + 1 );
							}
							else
							{
								p_str.clear();
							}
						}

						if ( p_str.size() > 0 )
						{
							l_uiIndex = p_str.find_last_not_of( '\t' );

							if ( l_uiIndex != string_t::npos )
							{
								p_str = p_str.substr( 0, l_uiIndex + 1 );
							}
							else
							{
								p_str.clear();
							}
						}
					}
				}

				return p_str;
			}

			template< typename CharType >
			std::basic_string< CharType > str_upper( std::basic_string< CharType > const & p_str )
			{
				typedef std::basic_string< CharType > string_t;
				string_t l_strReturn;
				std::locale loc;

				for ( std::size_t i = 0; i < p_str.size(); i++ )
				{
					l_strReturn += std::toupper( p_str[i], loc );
				}

				return l_strReturn;
			}

			template< typename CharType >
			std::basic_string< CharType > str_lower( std::basic_string< CharType > const & p_str )
			{
				typedef std::basic_string< CharType > string_t;
				string_t l_strReturn;
				std::locale loc;

				for ( std::size_t i = 0; i < p_str.size(); i++ )
				{
					l_strReturn += std::tolower( p_str[i], loc );
				}

				return l_strReturn;
			}

			template< typename CharType > size_t str_vprintf( CharType * out, size_t max, const CharType * format, va_list vaList );

			template<>
			size_t str_vprintf< char >( char * out, size_t max, const char * format, va_list vaList )
			{
				return vsnprintf( out, max, format, vaList );
			}

			template<>
			size_t str_vprintf< wchar_t >( wchar_t * out, size_t max, const wchar_t * format, va_list vaList )
			{
				return vswprintf( out, max, format, vaList );
			}

			template< typename CharType >
			void str_formalize( std::basic_string< CharType > & formattedString, int maxSize, const CharType * format, va_list vaList )
			{
				std::vector< CharType > strText( maxSize + 1, 0 );

				try
				{

					if ( format )
					{
						size_t written = str_vprintf( strText.data(), maxSize + 1, format, vaList );
						formattedString.assign( strText.data(), strText.data() + std::min( written, size_t( maxSize ) ) );
					}
				}
				catch ( ... )
				{
					StringStream message;
					message << ERROR_DB_FORMALIZE << formattedString.c_str();
					DB_EXCEPT( EDatabaseExceptionCodes_DateTimeError, message.str() );
				}
			}
		}

		//*************************************************************************************************

		bool IsUpperCase( const std::string & p_strToTest )
		{
			return p_strToTest == detail::str_upper( p_strToTest );
		}

		bool IsLowerCase( const std::string & p_strToTest )
		{
			return p_strToTest == detail::str_lower( p_strToTest );
		}

		bool IsUpperCase( const std::wstring & p_strToTest )
		{
			return p_strToTest == detail::str_upper( p_strToTest );
		}

		bool IsLowerCase( const std::wstring & p_strToTest )
		{
			return p_strToTest == detail::str_lower( p_strToTest );
		}

		std::string UpperCase( const std::string & p_str )
		{
			return detail::str_upper( p_str );
		}

		std::string LowerCase( const std::string & p_str )
		{
			return detail::str_lower( p_str );
		}

		std::wstring UpperCase( const std::wstring & p_str )
		{
			return detail::str_upper( p_str );
		}

		std::wstring LowerCase( const std::wstring & p_str )
		{
			return detail::str_lower( p_str );
		}

		std::string & ToUpperCase( std::string & p_str )
		{
			p_str = detail::str_upper( p_str );
			return p_str;
		}

		std::string & ToLowerCase( std::string & p_str )
		{
			p_str = detail::str_lower( p_str );
			return p_str;
		}

		std::wstring & ToUpperCase( std::wstring & p_str )
		{
			p_str = detail::str_upper( p_str );
			return p_str;
		}

		std::wstring & ToLowerCase( std::wstring & p_str )
		{
			p_str = detail::str_lower( p_str );
			return p_str;
		}

		std::vector< std::string > Split( const std::string & p_str, const std::string & p_delims, uint32_t p_maxSplits, bool p_bKeepVoid )
		{
			return detail::str_split( p_str, p_delims, p_maxSplits, p_bKeepVoid );
		}

		std::vector< std::wstring > Split( const std::wstring & p_str, const std::wstring & p_delims, uint32_t p_maxSplits, bool p_bKeepVoid )
		{
			return detail::str_split( p_str, p_delims, p_maxSplits, p_bKeepVoid );
		}

		std::string & Trim( std::string & p_str, bool p_bLeft, bool p_bRight )
		{
			return detail::str_trim( p_str, p_bLeft, p_bRight );
		}

		std::wstring & Trim( std::wstring & p_str, bool p_bLeft, bool p_bRight )
		{
			return detail::str_trim( p_str, p_bLeft, p_bRight );
		}

		std::string & Replace( std::string & p_str, char p_find, char p_replaced )
		{
			char l_szFind[2] = { p_find, STR( '\0' ) };
			char l_szReplaced[2] = { p_replaced, STR( '\0' ) };
			return detail::str_replace( p_str, std::string( l_szFind ), std::string( l_szReplaced ) );
		}

		std::string & Replace( std::string & p_str, const std::string & p_find, char p_replaced )
		{
			char l_szReplaced[2] = { p_replaced, STR( '\0' ) };
			return detail::str_replace( p_str, p_find, std::string( l_szReplaced ) );
		}

		std::string & Replace( std::string & p_str, char p_find, const std::string & p_replaced )
		{
			char l_szFind[2] = { p_find, STR( '\0' ) };
			return detail::str_replace( p_str, std::string( l_szFind ), p_replaced );
		}

		std::string & Replace( std::string & p_str, const std::string & p_find, const std::string & p_replaced )
		{
			return detail::str_replace( p_str, p_find, p_replaced );
		}

		std::wstring & Replace( std::wstring & p_str, wchar_t p_find, wchar_t p_replaced )
		{
			wchar_t l_szFind[2] = { p_find, STR( '\0' ) };
			wchar_t l_szReplaced[2] = { p_replaced, STR( '\0' ) };
			return detail::str_replace( p_str, std::wstring( l_szFind ), std::wstring( l_szReplaced ) );
		}

		std::wstring & Replace( std::wstring & p_str, const std::wstring & p_find, wchar_t p_replaced )
		{
			wchar_t l_szReplaced[2] = { p_replaced, STR( '\0' ) };
			return detail::str_replace( p_str, p_find, std::wstring( l_szReplaced ) );
		}

		std::wstring & Replace( std::wstring & p_str, wchar_t p_find, const std::wstring & p_replaced )
		{
			wchar_t l_szFind[2] = { p_find, STR( '\0' ) };
			return detail::str_replace( p_str, std::wstring( l_szFind ), p_replaced );
		}

		std::wstring & Replace( std::wstring & p_str, const std::wstring & p_find, const std::wstring & p_replaced )
		{
			return detail::str_replace( p_str, p_find, p_replaced );
		}

		void Formalize( std::string & formattedString, int maxSize, const char * format, ... )
		{
			formattedString.clear();

			if ( format )
			{
				va_list vaList;
				va_start( vaList, format );
				detail::str_formalize( formattedString, maxSize, format, vaList );
				va_end( vaList );
			}
		}

		void Formalize( std::wstring & formattedString, int maxSize, const wchar_t * format, ... )
		{
			formattedString.clear();

			if ( format )
			{
				va_list vaList;
				va_start( vaList, format );
				detail::str_formalize( formattedString, maxSize, format, vaList );
				va_end( vaList );
			}
		}

		std::string ToStr( const std::wstring & src, const std::string & charset )
		{
			//std::stringstream stream;
			//std::locale loc( boost::locale::generator().generate( charset ) );
			//for ( auto c : src )
			//{
			//	stream << std::use_facet< std::ctype< wchar_t > >( loc ).narrow( c );
			//}
			//return stream.str();

			//return detail::str_convert< char >( src, charset );

			return detail::str_convert< char >( src );
		}

		std::wstring ToWStr( const std::string & src, const std::string & charset )
		{
			//std::wstringstream stream;
			//std::locale loc( boost::locale::generator().generate( charset ) );
			//for ( auto c : src )
			//{
			//	stream << std::use_facet< std::ctype< wchar_t > >( loc ).widen( c );
			//}
			//return stream.str();

			//return detail::str_convert< wchar_t >( src, charset );

			return detail::str_convert< wchar_t >( src );
		}

		String ToString( char p_char )
		{
			char l_szTmp[2] = { p_char, '\0' };
			return String( l_szTmp );
		}

		String ToString( wchar_t p_wchar )
		{
			wchar_t l_wszTmp[2] = { p_wchar, L'\0' };
			return ToStr( l_wszTmp );
		}

		std::string ToUtf8( const std::string & src, const std::string & charset )
		{
			std::wstring strUtf = boost::locale::conv::to_utf< wchar_t >( src, charset );
			return boost::locale::conv::from_utf( strUtf, "UTF-8" );
		}

		std::string ToUtf8( const std::wstring & src, const std::string & charset )
		{
			return ToUtf8( StringUtils::ToStr( src, charset ), charset );
		}
	}

	std::ostream & operator << ( std::ostream & stream, NAMESPACE_DATABASE::ByteArray const & vector )
	{
		auto flags = stream.setf( std::ios::hex, std::ios::basefield );

		for ( auto && it : vector )
		{
			stream.width( 2 );
			stream.fill( '0' );
			stream << int( it );
		}

		stream.setf( flags );
		return stream;
	}

	std::wostream & operator << ( std::wostream & stream, NAMESPACE_DATABASE::ByteArray const & vector )
	{
		auto flags = stream.setf( std::ios::hex, std::ios::basefield );

		for ( auto && it : vector )
		{
			stream.width( 2 );
			stream.fill( L'0' );
			stream << int( it );
		}

		stream.setf( flags );
		return stream;
	}

	std::ostream & operator <<( std::ostream & stream, const std::wstring & string )
	{
		stream << NAMESPACE_DATABASE::StringUtils::ToStr( string );
		return stream;
	}

	std::ostream & operator <<( std::ostream & stream, const wchar_t * string )
	{
		stream << NAMESPACE_DATABASE::StringUtils::ToStr( string );
		return stream;
	}
}
END_NAMESPACE_DATABASE
