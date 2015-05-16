#include "DatabasePch.h"

#include "DatabaseStringUtils.h"

BEGIN_NAMESPACE_DATABASE
{
    namespace
    {
	    template< typename InChar, typename OutChar > struct str_converter;
#if !defined( __GNUG__ )
	    template< typename InChar, typename OutChar > struct str_converter
	    {
		    void operator ()( std::basic_string< InChar > const & p_strIn, std::basic_string< OutChar > & p_strOut, std::locale const & p_locale = std::locale() )
		    {
			    if( !p_strIn.empty() )
			    {
				    typedef typename std::codecvt< OutChar, InChar, std::mbstate_t > facet_type;
				    typedef typename facet_type::result result_type;

				    std::mbstate_t l_state = std::mbstate_t();
				    result_type l_result;
				    std::vector< OutChar > l_buffer( p_strIn.size() + 1 );
				    InChar const * l_pEndIn = NULL;
				    OutChar * l_pEndOut = NULL;

				    l_result = std::use_facet< facet_type >( p_locale ).in( l_state,
								    p_strIn.data(),		p_strIn.data() + p_strIn.size(),	l_pEndIn,
								    &l_buffer.front(),	&l_buffer.front() + p_strIn.size(),	l_pEndOut
							    );

				    p_strOut = std::basic_string< OutChar >( &l_buffer.front(), l_pEndOut );
			    }
			    else
			    {
				    p_strOut.clear();
			    }
		    }
	    };
#else
	    template<> struct str_converter< char, wchar_t >
	    {
		    void operator ()( std::basic_string< char > const & p_strIn, std::basic_string< wchar_t > & p_strOut, std::locale const & p_locale = std::locale() )
		    {
			    if( !p_strIn.empty() )
			    {
				    const std::size_t l_size = p_strIn.size();
				    p_strOut.resize( l_size, L'#' );
				    std::use_facet< std::ctype< wchar_t > >( p_locale ).widen( p_strIn.data(), p_strIn.data() + p_strIn.size(), &p_strOut[0] );
//				    mbstowcs( &p_strOut[0], p_strIn.c_str(), l_size );
			    }
			    else
			    {
				    p_strOut.clear();
			    }
		    }
	    };
	    template<> struct str_converter< wchar_t, char >
	    {
		    void operator ()( std::basic_string< wchar_t > const & p_strIn, std::basic_string< char > & p_strOut, std::locale const & p_locale = std::locale() )
		    {
			    if( !p_strIn.empty() )
			    {
				    const std::size_t l_size = p_strIn.size();
				    p_strOut.resize( l_size, '#' );
				    std::use_facet< std::ctype< wchar_t > >( p_locale ).narrow( p_strIn.data(), p_strIn.data() + p_strIn.size(), '#', &p_strOut[0] );
//				    wcstombs( &p_strOut[0], p_strIn.c_str(), l_size );
			    }
			    else
			    {
				    p_strOut.clear();
			    }
		    }
	    };
    #endif
	    template< typename InChar >
	    struct str_converter< InChar, InChar >
	    {
		    void operator ()( std::basic_string< InChar > const & p_strIn, std::basic_string< InChar > & p_strOut, std::locale const & )
		    {
			    p_strOut = p_strIn;
		    }
	    };
    }

    //*************************************************************************************************

    bool CStrUtils::IsInteger( String const & p_strToTest, std::locale const & CU_PARAM_UNUSED( p_locale ) )
    {
	    bool l_bReturn = true;

	    if( !p_strToTest.empty() )
	    {
		    l_bReturn = (p_strToTest[0] >= '0' && p_strToTest[0] <= '9') || p_strToTest[0] == '-';

		    for( std::size_t i = 1; i < p_strToTest.size() && l_bReturn; i++ )
		    {
			    l_bReturn = p_strToTest[i] >= '0' && p_strToTest[i] <= '9';
		    }
	    }

	    return l_bReturn;
    }

    bool CStrUtils::IsFloating( String const & p_strToTest, std::locale const & CU_PARAM_UNUSED( p_locale ) )
    {
	    bool		l_bReturn = false;
	    StringArray	l_arrayParts;
	    String		l_strText( p_strToTest );
	    std::size_t	l_nSize;

	    Replace( l_strText, STR( "," ), STR( "." ) );
	    l_arrayParts = Split( l_strText, STR( "." ) );
	    l_nSize = l_arrayParts.size();

	    if( l_nSize > 0 && l_nSize < 3 )
	    {
		    l_bReturn = IsInteger( l_arrayParts[0] );

		    if (l_bReturn && l_nSize > 1)
		    {
			    l_bReturn = IsInteger( l_arrayParts[1] );
		    }
	    }

	    return l_bReturn;
    }

    bool CStrUtils::IsDate( String const & p_strToTest, std::locale const & p_locale )
    {
	    bool							l_bReturn = false;
	    String							l_strText( p_strToTest );
	    String							l_strYear;
	    String							l_strMonth;
	    String							l_strDay;
	    std::tm							l_tmbuf = { 0 };
	    std::time_get<TChar> const &	l_timeFacet =  std::use_facet< std::time_get< TChar > >( p_locale );
	    std::time_get<TChar>::dateorder	l_order = l_timeFacet.date_order();
	    StringArray						l_arraySplitted;

	    Replace( l_strText, STR( "/"), STR( " "));
	    Replace( l_strText, STR( "-"), STR( " "));

	    while( l_strText.find( STR( "  " ) ) != String::npos )
	    {
		    Replace( l_strText, STR( "  " ), STR( " " ) );
	    }

	    l_arraySplitted = Split( l_strText, STR( " " ), 4, false );

	    if( l_arraySplitted.size() == 3 && IsInteger( l_arraySplitted[0] ) && IsInteger( l_arraySplitted[2] ) )
	    {
		    l_bReturn = true;

		    switch( l_order )
		    {
		    case std::time_base::dmy:
			    l_strYear = l_arraySplitted[2];
			    l_tmbuf.tm_mday = ToInt( l_arraySplitted[0] );
			    l_tmbuf.tm_mon = DoGetMonthIndex( l_arraySplitted[1] );
			    l_tmbuf.tm_year = ToInt( l_arraySplitted[2] );
			    break;

		    case std::time_base::mdy:
			    l_strYear = l_arraySplitted[2];
			    l_tmbuf.tm_mon = DoGetMonthIndex( l_arraySplitted[0] );
			    l_tmbuf.tm_mday = ToInt( l_arraySplitted[1] );
			    l_tmbuf.tm_year = ToInt( l_arraySplitted[2] );
			    break;

		    case std::time_base::ydm:
			    l_strYear = l_arraySplitted[0];
			    l_tmbuf.tm_year = ToInt( l_arraySplitted[0] );
			    l_tmbuf.tm_mday = ToInt( l_arraySplitted[1] );
			    l_tmbuf.tm_mon = DoGetMonthIndex( l_arraySplitted[2] );
			    break;

		    case std::time_base::ymd:
			    l_strYear = l_arraySplitted[0];
			    l_tmbuf.tm_year = ToInt( l_arraySplitted[0] );
			    l_tmbuf.tm_mon = DoGetMonthIndex( l_arraySplitted[1]);
			    l_tmbuf.tm_mday = ToInt( l_arraySplitted[2] );
			    break;

		    default:
			    l_bReturn = false;
		    }

		    l_bReturn &= l_tmbuf.tm_mon > 0 && l_tmbuf.tm_mon <= 12;

		    if( l_bReturn )
		    {
			    if( l_strYear.size() > 2 )
			    {
				    l_bReturn = l_tmbuf.tm_year > 1900 && l_tmbuf.tm_year < 2500;
			    }
			    else if( l_strYear.size() == 2 )
			    {
				    l_bReturn = true;
			    }
		    }

		    if( l_bReturn )
		    {
			    l_bReturn = DoIsValidDay( l_tmbuf.tm_mday, l_tmbuf.tm_mon, l_tmbuf.tm_year );
		    }
	    }

	    return l_bReturn;
    }

    bool CStrUtils::IsUpperCase( std::string const & p_strToTest )
    {
	    return p_strToTest == UpperCase( p_strToTest );
    }

    bool CStrUtils::IsLowerCase( std::string const & p_strToTest )
    {
	    return p_strToTest == LowerCase( p_strToTest );
    }

    bool CStrUtils::IsUpperCase( std::wstring const & p_strToTest )
    {
	    return p_strToTest == UpperCase( p_strToTest );
    }

    bool CStrUtils::IsLowerCase( std::wstring const & p_strToTest )
    {
	    return p_strToTest == LowerCase( p_strToTest );
    }

    short CStrUtils::ToShort( String const & p_strToTest, std::locale const & p_locale )
    {
	    short l_sReturn = 0;

	    if( ! p_strToTest.empty() )
	    {
		    Parse( p_strToTest, p_locale, l_sReturn );
	    }

	    return l_sReturn;
    }

    int CStrUtils::ToInt( String const & p_strToTest, std::locale const & p_locale )
    {
	    int l_iReturn = 0;

	    if( ! p_strToTest.empty() )
	    {
		    Parse( p_strToTest, p_locale, l_iReturn );
	    }

	    return l_iReturn;
    }

    long CStrUtils::ToLong( String const & p_strToTest, std::locale const & p_locale )
    {
	    long l_lReturn = 0;

	    if( ! p_strToTest.empty() )
	    {
		    Parse( p_strToTest, p_locale, l_lReturn );
	    }

	    return l_lReturn;
    }

    long long CStrUtils::ToLongLong( String const & p_strToTest, std::locale const & p_locale )
    {
	    long long l_llReturn = 0;

	    if( ! p_strToTest.empty() )
	    {
		    Parse( p_strToTest, p_locale, l_llReturn );
	    }

	    return l_llReturn;
    }

    float CStrUtils::ToFloat( String const & p_strToTest, std::locale const & p_locale )
    {
	    float l_fReturn = 0;

	    if( ! p_strToTest.empty() )
	    {
		    Parse( p_strToTest, p_locale, l_fReturn );
	    }

	    return l_fReturn;
    }

    double CStrUtils::ToDouble( String const & p_strToTest, std::locale const & p_locale )
    {
	    double l_dReturn = 0;

	    if( ! p_strToTest.empty() )
	    {
		    Parse( p_strToTest, p_locale, l_dReturn );
	    }

	    return l_dReturn;
    }

    long double CStrUtils::ToLongDouble( String const & p_strToTest, std::locale const & p_locale )
    {
	    long double l_ldReturn = 0;

	    if( !p_strToTest.empty() )
	    {
		    Parse( p_strToTest, p_locale, l_ldReturn );
	    }

	    return l_ldReturn;
    }

    std::string CStrUtils::UpperCase( std::string const & p_str )
    {
	    std::string l_strReturn;
	    std::locale loc;

	    for( std::size_t i = 0; i < p_str.size(); i++ )
	    {
		    l_strReturn += std::toupper( p_str[i], loc );
	    }

	    return l_strReturn;
    }

    std::string CStrUtils::LowerCase( std::string const & p_str )
    {
	    std::string l_strReturn;
	    std::locale loc;

	    for( std::size_t i = 0; i < p_str.size(); i++ )
	    {
		    l_strReturn += std::tolower( p_str[i], loc );
	    }

	    return l_strReturn;
    }

    std::wstring CStrUtils::UpperCase( std::wstring const & p_str )
    {
	    std::wstring l_strReturn;
	    std::locale loc;

	    for( std::size_t i = 0; i < p_str.size(); i++ )
	    {
		    l_strReturn += std::toupper( p_str[i], loc );
	    }

	    return l_strReturn;
    }

    std::wstring CStrUtils::LowerCase( std::wstring const & p_str )
    {
	    std::wstring l_strReturn;
	    std::locale loc;

	    for( std::size_t i = 0; i < p_str.size(); i++ )
	    {
		    l_strReturn += std::tolower( p_str[i], loc );
	    }

	    return l_strReturn;
    }

    std::string & CStrUtils::ToUpperCase( std::string & p_str )
    {
	    p_str = UpperCase( p_str );
	    return p_str;
    }

    std::string & CStrUtils::ToLowerCase( std::string & p_str )
    {
	    p_str = LowerCase( p_str );
	    return p_str;
    }

    std::wstring & CStrUtils::ToUpperCase( std::wstring & p_str )
    {
	    p_str = UpperCase( p_str );
	    return p_str;
    }

    std::wstring & CStrUtils::ToLowerCase( std::wstring & p_str )
    {
	    p_str = LowerCase( p_str );
	    return p_str;
    }

    StringArray CStrUtils::Split( String const & p_str, String const & p_delims, uint32_t p_maxSplits, bool p_bKeepVoid )
    {
	    StringArray	l_arrayReturn;

	    if( ! p_str.empty() && ! p_delims.empty() && p_maxSplits > 0 )
	    {
		    l_arrayReturn.reserve( p_maxSplits + 1 );
		    std::size_t l_numSplits = 0;
		    std::size_t	l_pos		= 0;
		    std::size_t	l_start		= 0;

		    do
		    {
			    l_pos = p_str.find_first_of( p_delims, l_start );

			    if( l_pos == l_start )
			    {
				    l_start = l_pos + 1;

				    if( p_bKeepVoid )
				    {
					    l_arrayReturn.push_back( STR( "" ) );
				    }
			    }
			    else if( l_pos == std::string::npos || l_numSplits == p_maxSplits )
			    {
				    l_arrayReturn.push_back( p_str.substr( l_start ) );
				    return l_arrayReturn;
			    }
			    else
			    {
				    l_arrayReturn.push_back( p_str.substr( l_start, l_pos - l_start ) );
				    l_start = l_pos + 1;
			    }

			    l_start = p_str.find_first_not_of( p_delims, l_start );
			    ++ l_numSplits;
		    }
		    while( l_pos != std::string::npos );
	    }

	    return l_arrayReturn;
    }

    String & CStrUtils::Trim( String & p_str, bool p_bLeft, bool p_bRight )
    {
	    if( p_str.size() > 0 )
	    {
		    std::size_t l_uiIndex;

		    if( p_bLeft )
		    {
			    l_uiIndex= p_str.find_first_not_of( STR( ' ' ) );

			    if( l_uiIndex > 0 )
			    {
				    if( l_uiIndex != String::npos )
				    {
					    p_str = p_str.substr( l_uiIndex, String::npos );
				    }
				    else
				    {
					    p_str.clear();
				    }
			    }

			    l_uiIndex = p_str.find_first_not_of( STR( '\t' ) );

			    if( l_uiIndex > 0 )
			    {
				    if( l_uiIndex != String::npos )
				    {
					    p_str = p_str.substr( l_uiIndex, String::npos );
				    }
				    else
				    {
					    p_str.clear();
				    }
			    }
		    }

		    if( p_bRight && p_str.size() > 0 )
		    {
			    l_uiIndex = p_str.find_last_not_of( STR( ' ' ) );

			    if( l_uiIndex < p_str.size() - 1 )
			    {
				    if( l_uiIndex != String::npos )
				    {
					    p_str = p_str.substr( 0, l_uiIndex + 1 );
				    }
				    else
				    {
					    p_str.clear();
				    }
			    }

			    if( p_str.size() > 0 )
			    {
				    l_uiIndex = p_str.find_last_not_of( STR( '\t' ) );

				    if( l_uiIndex != String::npos )
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

    String & CStrUtils::Replace( String & p_str, TChar p_find, TChar p_replaced )
    {
	    TChar l_szFind[2] = { p_find, STR( '\0' ) };
	    TChar l_szReplaced[2] = { p_replaced, STR( '\0' ) };
	    return Replace( p_str, l_szFind, l_szReplaced );
    }

    String & CStrUtils::Replace( String & p_str, String const & p_find, TChar p_replaced )
    {
	    TChar l_szReplaced[2] = { p_replaced, STR( '\0' ) };
	    return Replace( p_str, p_find, l_szReplaced );
    }

    String & CStrUtils::Replace( String & p_str, TChar p_find, String const & p_replaced )
    {
	    TChar l_szFind[2] = { p_find, STR( '\0' ) };
	    return Replace( p_str, l_szFind, p_replaced );
    }

    String & CStrUtils::Replace( String & p_str, String const & p_find, String const & p_replaced)
    {
	    String		l_temp;
	    String		l_return;
	    std::size_t	l_currentPos	= 0;
	    std::size_t	l_pos			= 0;

	    while( (l_pos = p_str.find( p_find, l_currentPos ) ) != String::npos )
	    {
		    l_return.append( p_str.substr( l_currentPos, l_pos - l_currentPos ) );
		    l_return.append( p_replaced );
		    l_currentPos = l_pos + p_find.size();
	    }
	    if( l_currentPos != p_str.size() )
	    {
		    l_return.append( p_str.substr( l_currentPos, l_pos - l_currentPos ) );
	    }

	    p_str = l_return;
	    return p_str;
    }

    std::string CStrUtils::ToStr( std::wstring const & p_str )
    {
	    std::string l_strReturn;
	    std::wostringstream l_wstream;
	    str_converter< wchar_t, char >()( p_str, l_strReturn, l_wstream.getloc() );
	    return l_strReturn;
    }

    std::wstring CStrUtils::ToWStr( std::string const & p_str )
    {
	    std::wstring l_strReturn;
	    std::wostringstream l_wstream;
	    str_converter< char, wchar_t >()( p_str, l_strReturn, l_wstream.getloc() );
	    return l_strReturn;
    }

    String CStrUtils::ToString( std::string const & p_strString )
    {
	    String l_strReturn;
	    std::wostringstream l_wstream;
	    str_converter< char, TChar >()( p_strString, l_strReturn, l_wstream.getloc() );
	    return l_strReturn;
    }

    String CStrUtils::ToString( std::wstring const & p_strString )
    {
	    String l_strReturn;
	    std::wostringstream l_wstream;
	    str_converter< wchar_t, TChar >()( p_strString, l_strReturn, l_wstream.getloc() );
	    return l_strReturn;
    }

    String CStrUtils::ToString( char p_char )
    {
	    char l_szTmp[2] = { p_char, '\0' };
	    return ToString( l_szTmp );
    }

    String CStrUtils::ToString( wchar_t p_wchar )
    {
	    wchar_t l_wszTmp[2] = { p_wchar, L'\0' };
	    return ToString( l_wszTmp );
    }

    int CStrUtils::DoGetMonthIndex( String const & p_strMonth )
    {
	    int l_iReturn = 0;
	    static std::map< String, int > l_mapMonths;

	    if( l_mapMonths.empty() )
	    {
		    l_mapMonths.insert( std::make_pair( STR( "janvier"		),	 1 ) );
		    l_mapMonths.insert( std::make_pair( STR( "février"		),	 2 ) );
		    l_mapMonths.insert( std::make_pair( STR( "fevrier"		),	 2 ) );
		    l_mapMonths.insert( std::make_pair( STR( "mars"			),	 3 ) );
		    l_mapMonths.insert( std::make_pair( STR( "avril"		),	 4 ) );
		    l_mapMonths.insert( std::make_pair( STR( "mai"			),	 5 ) );
		    l_mapMonths.insert( std::make_pair( STR( "juin"			),	 6 ) );
		    l_mapMonths.insert( std::make_pair( STR( "juillet"		),	 7 ) );
		    l_mapMonths.insert( std::make_pair( STR( "août"			),	 8 ) );
		    l_mapMonths.insert( std::make_pair( STR( "aout"			),	 8 ) );
		    l_mapMonths.insert( std::make_pair( STR( "septembre"	),	 9 ) );
		    l_mapMonths.insert( std::make_pair( STR( "octobre"		),	10 ) );
		    l_mapMonths.insert( std::make_pair( STR( "novembre"		),	11 ) );
		    l_mapMonths.insert( std::make_pair( STR( "décembre"		),	12 ) );
		    l_mapMonths.insert( std::make_pair( STR( "decembre"		),	12 ) );
		    l_mapMonths.insert( std::make_pair( STR( "jan"			),	 1 ) );
		    l_mapMonths.insert( std::make_pair( STR( "fév"			),	 2 ) );
		    l_mapMonths.insert( std::make_pair( STR( "fev"			),	 2 ) );
		    l_mapMonths.insert( std::make_pair( STR( "mar"			),	 3 ) );
		    l_mapMonths.insert( std::make_pair( STR( "avr"			),	 4 ) );
		    l_mapMonths.insert( std::make_pair( STR( "mai"			),	 5 ) );
		    l_mapMonths.insert( std::make_pair( STR( "jun"			),	 6 ) );
		    l_mapMonths.insert( std::make_pair( STR( "jul"			),	 7 ) );
		    l_mapMonths.insert( std::make_pair( STR( "aoû"			),	 8 ) );
		    l_mapMonths.insert( std::make_pair( STR( "aou"			),	 8 ) );
		    l_mapMonths.insert( std::make_pair( STR( "sep"			),	 9 ) );
		    l_mapMonths.insert( std::make_pair( STR( "oct"			),	10 ) );
		    l_mapMonths.insert( std::make_pair( STR( "nov"			),	11 ) );
		    l_mapMonths.insert( std::make_pair( STR( "déc"			),	12 ) );
		    l_mapMonths.insert( std::make_pair( STR( "dec"			),	12 ) );
		    l_mapMonths.insert( std::make_pair( STR( "january"		),	 1 ) );
		    l_mapMonths.insert( std::make_pair( STR( "february"		),	 2 ) );
		    l_mapMonths.insert( std::make_pair( STR( "march"		),	 3 ) );
		    l_mapMonths.insert( std::make_pair( STR( "april"		),	 4 ) );
		    l_mapMonths.insert( std::make_pair( STR( "may"			),	 5 ) );
		    l_mapMonths.insert( std::make_pair( STR( "june"			),	 6 ) );
		    l_mapMonths.insert( std::make_pair( STR( "july"			),	 7 ) );
		    l_mapMonths.insert( std::make_pair( STR( "august"		),	 8 ) );
		    l_mapMonths.insert( std::make_pair( STR( "september"	),	 9 ) );
		    l_mapMonths.insert( std::make_pair( STR( "october"		),	10 ) );
		    l_mapMonths.insert( std::make_pair( STR( "november"		),	11 ) );
		    l_mapMonths.insert( std::make_pair( STR( "december"		),	12 ) );
		    l_mapMonths.insert( std::make_pair( STR( "feb"			),	 2 ) );
		    l_mapMonths.insert( std::make_pair( STR( "apr"			),	 4 ) );
		    l_mapMonths.insert( std::make_pair( STR( "may"			),	 5 ) );
		    l_mapMonths.insert( std::make_pair( STR( "aug"			),	 8 ) );
	    }

	    if( IsInteger( p_strMonth ) )
	    {
		    l_iReturn = ToInt( p_strMonth );
	    }
	    else
	    {
		    String l_strMonthLC = p_strMonth;
            std::map< String, int >::const_iterator l_it = l_mapMonths.find( ToLowerCase( l_strMonthLC ) );

		    if( l_it != l_mapMonths.end() )
		    {
			    l_iReturn = l_it->second;
		    }
	    }

	    return l_iReturn;
    }

    bool CStrUtils::DoIsValidDay( int p_iDay, int p_iMonth, int p_iYear)
    {
	    bool l_bReturn = false;

	    if (p_iDay > 0)
	    {
		    if (p_iMonth == 1 || p_iMonth == 3 || p_iMonth == 5 || p_iMonth == 7 || p_iMonth == 8 || p_iMonth == 10 || p_iMonth == 12)
		    {
			    l_bReturn = p_iDay <= 31;
		    }
		    else if (p_iMonth != 2)
		    {
			    l_bReturn = p_iDay <= 30;
		    }
		    else if (p_iYear % 400 == 0 || (p_iYear % 4 == 0 && p_iYear % 100 != 0))
		    {
			    l_bReturn = p_iDay <= 29;
		    }
		    else
		    {
			    l_bReturn = p_iDay <= 28;
		    }
	    }

	    return l_bReturn;
    }
}
END_NAMESPACE_DATABASE

