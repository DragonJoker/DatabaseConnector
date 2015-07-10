/************************************************************************//**
 * @file Date.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 *
 *
 * @brief CDate class definition.
 *
 * @details Describes a date.
 *
 ***************************************************************************/

#include "DatabasePch.h"

#include "DatabaseDate.h"

#include "DatabaseDateTime.h"
#include "DatabaseDateTimeHelper.h"
#include "DatabaseException.h"

#include "DatabaseStringUtils.h"

BEGIN_NAMESPACE_DATABASE
{
	namespace DateUtils
	{
		static const size_t DATE_MAX_SIZE = 100;

		template< typename CharOut, typename CharIn > std::basic_string< CharOut > Str( const CharIn * in );

		template<> std::basic_string< char > Str< char, char >( const char * in )
		{
			return std::basic_string< char >( in, in + strlen( in ) );
		}

		template<> std::basic_string< wchar_t > Str< wchar_t, wchar_t >( const wchar_t * in )
		{
			return std::basic_string< wchar_t >( in, in + wcslen( in ) );
		}

		template<> std::basic_string< wchar_t > Str< wchar_t, char >( const char * in )
		{
			return CStrUtils::ToWStr( std::basic_string< char >( in, in + strlen( in ) ) );
		}

		template<> std::basic_string< char > Str< char, wchar_t >( const wchar_t * in )
		{
			return CStrUtils::ToStr( std::basic_string< wchar_t >( in, in + wcslen( in ) ) );
		}

		template< typename CharOut, typename CharIn > std::basic_string< CharOut > Str( const std::basic_string< CharIn > & in );

		template<> std::basic_string< char > Str< char, char >( const std::basic_string< char > & in )
		{
			return in;
		}

		template<> std::basic_string< wchar_t > Str< wchar_t, wchar_t >( const std::basic_string< wchar_t > & in )
		{
			return in;
		}

		template<> std::basic_string< wchar_t > Str< wchar_t, char >( const std::basic_string< char > & in )
		{
			return CStrUtils::ToWStr( in );
		}

		template<> std::basic_string< char > Str< char, wchar_t >( const std::basic_string< wchar_t > & in )
		{
			return CStrUtils::ToStr( in );
		}

		template< typename CharType >
		int ttoi( const std::basic_string< CharType > & in )
		{
			std::basic_istringstream< CharType > stream( in );
			int l_return = 0;
			stream >> l_return;
			return l_return;
		}

		template< typename CharType >
		int ttoi( CharType const *& in, size_t count )
		{
			int result = ttoi( std::basic_string< CharType >( in, in + count ) );
			in += count;
			return result;
		}

		template< typename Char >
		std::basic_string< Char > FormatDate( const std::basic_string< Char > & format, int year, EDateMonth month, int yearDay, int monthDay, EDateDay weekDay )
		{
			typedef std::basic_string< Char > String;
			typedef std::basic_stringstream< Char > StringStream;
			String strReturn( format );
			String strTmp;

			if ( weekDay != EDateDay_UNDEF )
			{
				Replace( strReturn, Str< Char >( "%a" ), Str< Char >( CDate::ShortDay[weekDay] ) );
				Replace( strReturn, Str< Char >( "%A" ), Str< Char >( CDate::LongDay[weekDay] ) );
				Replace( strReturn, Str< Char >( "%b" ), Str< Char >( CDate::ShortMonth[weekDay] ) );
				Replace( strReturn, Str< Char >( "%B" ), Str< Char >( CDate::LongMonth[weekDay] ) );
				StringStream strTmp2( Str< Char >( CDate::ShortDay[weekDay] ) + Str< Char >( " " ) + Str< Char >( CDate::ShortMonth[month] ) );
				strTmp2 << " " << year;
				Replace( strReturn, Str< Char >( "%c" ), strTmp2.str() );
			}

			Formalize( strTmp, DATE_MAX_SIZE, Str< Char >( "%02i" ).c_str(), monthDay );
			Replace( strReturn, Str< Char >( "%d" ), strTmp );
			Formalize( strTmp, DATE_MAX_SIZE, Str< Char >( "%02i" ).c_str(), monthDay );
			Replace( strReturn, Str< Char >( "%D" ), strTmp );
			Formalize( strTmp, DATE_MAX_SIZE, Str< Char >( "%03i" ).c_str(), yearDay );
			Replace( strReturn, Str< Char >( "%J" ), strTmp );
			Formalize( strTmp, DATE_MAX_SIZE, Str< Char >( "%02i" ).c_str(), month + 1 );
			Replace( strReturn, Str< Char >( "%m" ), strTmp );
			Formalize( strTmp, DATE_MAX_SIZE, Str< Char >( "%02i" ).c_str(), month + 1 );
			Replace( strReturn, Str< Char >( "%M" ), strTmp );
			Formalize( strTmp, DATE_MAX_SIZE, Str< Char >( "%02i" ).c_str(), weekDay - 1 );
			Replace( strReturn, Str< Char >( "%w" ), strTmp );
			Formalize( strTmp, DATE_MAX_SIZE, Str< Char >( "%02i/%02i/%04i" ).c_str(), monthDay, month, year );
			Replace( strReturn, Str< Char >( "%x" ), strTmp );
			Formalize( strTmp, DATE_MAX_SIZE, Str< Char >( "%04i" ).c_str(), year );
			Replace( strReturn, Str< Char >( "%y" ), strTmp.substr( 2, 2 ) );
			Formalize( strTmp, DATE_MAX_SIZE, Str< Char >( "%04i" ).c_str(), year );
			Replace( strReturn, Str< Char >( "%Y" ), strTmp );
			strTmp.clear();
			Replace( strReturn, Str< Char >( "%%" ), Str< Char >( "%" ) );

			return strReturn;
		}

		template< typename Char >
		int FindDateDay( const std::basic_string< Char > & date, const std::basic_string< Char > & format )
		{
			typedef std::basic_string< Char > String;
			int iDay = 0;
			size_t uiIndex;

			if ( ( uiIndex = format.find( Str< Char >( "%a" ) ) ) != String::npos )
			{
				for ( int i = 0 ; i < 7 && iDay == 0 ; i++ )
				{
					if ( date.find( Str< Char >( CDate::ShortDay[i] ) ) != String::npos )
					{
						iDay = i + 1;
					}
				}
			}
			else if ( ( uiIndex = format.find( Str< Char >( "%A" ) ) ) != String::npos )
			{
				for ( int i = 0 ; i < 7 && iDay == 0 ; i++ )
				{
					if ( date.find( Str< Char >( CDate::LongDay[i] ) ) != String::npos )
					{
						iDay = i + 1;
					}
				}
			}
			else if ( ( uiIndex = format.find( Str< Char >( "%c" ) ) ) != String::npos )
			{
				for ( int i = 0 ; i < 7 && iDay == 0 ; i++ )
				{
					if ( date.find( Str< Char >( CDate::ShortDay[i] ) ) != String::npos )
					{
						iDay = i + 1;
					}
				}
			}
			else if ( ( uiIndex = format.find( Str< Char >( "%d" ) ) ) != String::npos )
			{
			}
			else if ( ( uiIndex = format.find( Str< Char >( "%D" ) ) ) != String::npos )
			{
			}

			return iDay;
		}

		template< typename Char >
		int FindDateYear( const std::basic_string< Char > & date, const std::basic_string< Char > & format )
		{
			typedef std::basic_string< Char > String;
			int iYear = -1;
			size_t uiIndex;

			if ( ( uiIndex = format.find( Str< Char >( "%y" ) ) ) != String::npos )
			{
				size_t uiNextIndex = uiIndex + 2;
				size_t uiStartIndex, uiEndIndex;

				if ( uiNextIndex < format.size() )
				{
					uiEndIndex = date.find( format[uiIndex + 2] );

					if ( uiEndIndex == String::npos )
					{
						uiEndIndex = date.size();
					}
				}
				else
				{
					uiEndIndex = date.size();
				}

				uiStartIndex = uiEndIndex - 4;

				if ( uiStartIndex >= 0 && uiStartIndex < date.size() )
				{
					iYear = ttoi( date.substr( uiStartIndex, uiEndIndex - uiStartIndex ) );
				}
			}
			else if ( ( uiIndex = format.find( Str< Char >( "%Y" ) ) ) != String::npos )
			{
				size_t uiNextIndex = uiIndex + 2;
				size_t uiStartIndex, uiEndIndex;

				if ( uiNextIndex < format.size() )
				{
					uiEndIndex = date.find( format[uiIndex + 2] );

					if ( uiEndIndex == String::npos )
					{
						uiEndIndex = date.size();
					}
				}
				else
				{
					uiEndIndex = date.size();
				}

				uiStartIndex = uiEndIndex - 4;

				if ( uiStartIndex >= 0 && uiStartIndex < date.size() )
				{
					iYear = ttoi( date.substr( uiStartIndex, uiEndIndex - uiStartIndex ) );
				}
			}

			return iYear;
		}

		template< typename Char >
		bool IsDate( const  std::basic_string< Char > & date, const  std::basic_string< Char > & format, int & year, EDateMonth & month, int & monthDay )
		{
			typedef std::basic_string< Char > String;
			bool bReturn = !format.empty();

			monthDay = 0;
			month = EDateMonth_UNDEF;
			year = -1;

			if ( bReturn )
			{
				Char const * fc = format.data();
				Char const * dc = date.data();

				while ( bReturn && *fc )
				{
					if ( *fc == '%' )
					{
						bReturn = ++fc != NULL;

						if ( bReturn )
						{
							switch ( *fc++ )
							{
							case 'Y':
								year = ttoi( dc, 4 );
								break;

							case 'd':
								monthDay = ttoi( dc, 2 );
								break;

							case 'm':
								month = EDateMonth( ttoi( dc, 2 ) - 1 );
								break;

							case 'y':
								year = ttoi( dc, 2 ) + 1900;
								break;

							case '%':
								++dc;
								break;

							default:
								bReturn = false;
								break;
							}
						}
					}
					else if ( *fc == *dc )
					{
						++fc;
						++dc;
					}
					else
					{
						bReturn = false;
					}
				}
			}

			if ( bReturn )
			{
				if ( month != EDateMonth_FEBRUARY )
				{
					bReturn = monthDay <= CDate::MonthMaxDays[month];
				}
				else
				{
					bReturn = monthDay <= ( CDate::MonthMaxDays[month] + IsLeap( year ) );
				}
			}

			return bReturn;
		}

		template< typename Char >
		bool IsDate( const std::basic_string< Char > & time, CDate & result )
		{
			typedef std::basic_string< Char > String;
			bool bReturn = false;

			if ( !time.empty() )
			{
				try
				{
					boost::posix_time::ptime t( boost::posix_time::time_from_string( Str< char >( time ) ) );

					if ( !t.is_not_a_date_time() )
					{
						std::tm dt = to_tm( t );
						result.SetDate( dt.tm_year, EDateMonth( dt.tm_mon ), dt.tm_mday );
						bReturn = true;
					}
				}
				catch ( ... )
				{
				}
			}

			return bReturn;
		}
	}

	static const int DATE_MAX_SIZE = 1024;

	String CDate::ShortDay[7] = { STR( "Mon" ), STR( "Tue." ), STR( "Wed." ), STR( "Thu" ), STR( "Fri" ), STR( "Sat" ), STR( "Sun" ) };
	String CDate::LongDay[7] = { STR( "Monday" ), STR( "Tuesday" ), STR( "Wednesday" ), STR( "Thursday" ), STR( "Friday" ), STR( "Saturday" ), STR( "Sunday" ) };
	String CDate::ShortMonth[12] = { STR( "Jan" ), STR( "Feb" ), STR( "Mar" ), STR( "Apr" ), STR( "May" ), STR( "Jun" ), STR( "Jul" ), STR( "Aug" ), STR( "Sep" ), STR( "Oct" ), STR( "Nov" ), STR( "Dec" ) };
	String CDate::LongMonth[12] = { STR( "January" ), STR( "February" ), STR( "March" ), STR( "April" ), STR( "May" ), STR( "June" ), STR( "July" ), STR( "August" ), STR( "September" ), STR( "October" ), STR( "November" ), STR( "December" ) };
	int CDate::MonthMaxDays[12] = { 31, 28, 31, 30, 31 , 30 , 31 , 31 , 30 , 31 , 30 , 31 };
	CDateTime CDate::Today = CDateTime::Now();

	CDate::CDate()
		:   _year( -1 )
		,   _month( EDateMonth_UNDEF )
		,   _monthDay( 0 )
		,   _yearDay( 0 )
		,   _weekDay( EDateDay_UNDEF )
	{
	}

	CDate::CDate( const CDateTime & dateTime )
		:   _year( dateTime.GetYear() )
		,   _month( EDateMonth( dateTime.GetMonth() ) )
		,   _monthDay( dateTime.GetMonthDay() )
		,   _yearDay( 0 )
		,   _weekDay( EDateDay_UNDEF )
	{
		DoCheckValidity();
		DoComputeWeekDay();
		DoComputeYearDay();
	}

	CDate::CDate( int year, EDateMonth month, int day )
		:   _year( year )
		,   _month( month )
		,   _monthDay( day )
		,   _yearDay( 0 )
		,   _weekDay( EDateDay_UNDEF )
	{
		DoCheckValidity();
		DoComputeWeekDay();
		DoComputeYearDay();
	}

	CDate::~CDate()
	{
	}

	std::string CDate::Format( const std::string & format ) const
	{
		return DateUtils::FormatDate( format, _year, _month, _yearDay, _monthDay, _weekDay );
	}

	std::wstring CDate::Format( const std::wstring & format ) const
	{
		return DateUtils::FormatDate( format, _year, _month, _yearDay, _monthDay, _weekDay );
	}

	void CDate::SetDate( int year, EDateMonth month, int day )
	{
		_year      = year;
		_month     = month;
		_monthDay  = day;
		DoCheckValidity();
		DoComputeWeekDay();
		DoComputeYearDay();
	}

	int CDate::GetYear() const
	{
		return _year;
	}

	EDateMonth CDate::GetMonth() const
	{
		return _month;
	}

	int CDate::GetMonthDay() const
	{
		return _monthDay;
	}

	int CDate::GetYearDay() const
	{
		return _yearDay;
	}

	EDateDay CDate::GetWeekDay() const
	{
		return _weekDay;
	}

	bool CDate::Parse( const std::string & date, const std::string & format )
	{
		bool bReturn = false;
		int iMonthDay = 0;
		EDateMonth eMonth = EDateMonth_UNDEF;
		int iYear = -1;

		bReturn = DateUtils::IsDate( date, format, iYear, eMonth, iMonthDay );
		SetDate( iYear, eMonth, iMonthDay );

		return bReturn;
	}

	bool CDate::Parse( const std::wstring & date, const std::wstring & format )
	{
		bool bReturn = false;
		int iMonthDay = 0;
		EDateMonth eMonth = EDateMonth_UNDEF;
		int iYear = -1;

		bReturn = DateUtils::IsDate( date, format, iYear, eMonth, iMonthDay );
		SetDate( iYear, eMonth, iMonthDay );

		return bReturn;
	}

	bool CDate::IsValid() const
	{
		bool bReturn = false;
		int iMonthDay = GetMonthDay();
		int iMonth = GetMonth();
		int iYear = GetYear();

		if ( iMonth >= EDateMonth_JANUARY && iMonthDay > 0 && iYear != -1 )
		{
			if ( iMonth != EDateMonth_FEBRUARY )
			{
				if ( iMonthDay <= MonthMaxDays[iMonth - 1] )
				{
					bReturn = true;
				}
			}
			else
			{
				int leap = IsLeap( iYear );

				if ( iMonthDay <= ( MonthMaxDays[iMonth - 1] + leap ) )
				{
					bReturn = true;
				}
			}
		}

		return bReturn;
	}

	std::tm CDate::ToTm() const
	{
		std::tm ret = { 0 };
		ret.tm_mday = GetMonthDay();
		ret.tm_mon = GetMonth();
		ret.tm_wday = GetWeekDay();
		ret.tm_yday = GetYearDay();
		ret.tm_year = GetYear() - 1900;
		return ret;
	}

	CDate CDate::Now()
	{
		time_t last_time = time( NULL );
		std::tm tm;
#if defined( _WIN32 )
		localtime_s( &tm, &last_time );
#else
		tm = *std::localtime( &last_time );
#endif

		return CDate( tm.tm_year + 1900, EDateMonth( tm.tm_mon ), tm.tm_mday );
	}

	bool CDate::IsDate( const std::string & date, const std::string & format )
	{
		int iMonthDay = 0;
		EDateMonth eMonth = EDateMonth_UNDEF;
		int iYear = -1;

		return DateUtils::IsDate( date, format, iYear, eMonth, iMonthDay );
	}

	bool CDate::IsDate( const std::wstring & date, const std::wstring & format )
	{
		int iMonthDay = 0;
		EDateMonth eMonth = EDateMonth_UNDEF;
		int iYear = -1;

		return DateUtils::IsDate( date, format, iYear, eMonth, iMonthDay );
	}

	bool CDate::IsDate( const std::string & date, const std::string & format, CDate & result )
	{
		return result.Parse( date, format );
	}

	bool CDate::IsDate( const std::wstring & date, const std::wstring & format, CDate & result )
	{
		return result.Parse( date, format );
	}

	bool CDate::IsDate( const std::string & date, CDate & result )
	{
		return DateUtils::IsDate( date, result );
	}

	bool CDate::IsDate( const std::wstring & date, CDate & result )
	{
		return DateUtils::IsDate( date, result );
	}

	int CDate::GetMonthDays( int month, int year )
	{
		int iReturn = 0;

		switch ( month )
		{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			iReturn = 31;
			break;

		case 4:
		case 6:
		case 9:
		case 11:
			iReturn = 30;
			break;

		default:

			if ( IsLeap( year ) )
			{
				iReturn = 29;
			}
			else
			{
				iReturn = 28;
			}

			break;
		}

		return iReturn;
	}

	int CDate::GetYearDays( int year )
	{
		return IsLeap( year ) ? 366 : 365;
	}

	void CDate::DoCheckValidity()
	{
		if ( GetMonth() < EDateMonth_JANUARY )
		{
			_month = EDateMonth_JANUARY;
		}
		else if ( GetMonth() > EDateMonth_DECEMBER )
		{
			_month = EDateMonth_DECEMBER;
		}

		if ( GetMonth() != EDateMonth_FEBRUARY )
		{
			if ( GetMonthDay() > MonthMaxDays[GetMonth() - 1] )
			{
				_month = EDateMonth_JANUARY;
				_monthDay = 0;
				_year = -1;
			}
		}
		else
		{
			int leap = IsLeap( GetYear() );

			if ( GetMonthDay() > ( MonthMaxDays[GetMonth() - 1] + leap ) )
			{
				_month = EDateMonth_JANUARY;
				_monthDay = 0;
				_year = -1;
			}
		}
	}

	void CDate::DoComputeWeekDay()
	{
		int iMonthOffset = 0;
		int iYearOffset = 0;
		int iCenturyOffset = 0;
		int iDayOffset = 0;

		int iCentury = GetYear() / 100;
		iCenturyOffset = ( ( 39 - iCentury ) % 4 ) * 2;

		int iYear = GetYear() - ( iCentury * 100 );
		iYearOffset = ( ( iYear / 4 ) + iYear ) % 7;

		switch ( GetMonth() )
		{
		case EDateMonth_JANUARY:
			iMonthOffset = 0;
			break;

		case EDateMonth_FEBRUARY:
			iMonthOffset = 3;
			break;

		case EDateMonth_MARCH:
			iMonthOffset = 3;
			break;

		case EDateMonth_APRIL:
			iMonthOffset = 6;
			break;

		case EDateMonth_MAY:
			iMonthOffset = 1;
			break;

		case EDateMonth_JUNE:
			iMonthOffset = 4;
			break;

		case EDateMonth_JULY:
			iMonthOffset = 6;
			break;

		case EDateMonth_AUGUST:
			iMonthOffset = 2;
			break;

		case EDateMonth_SEPTEMBER:
			iMonthOffset = 5;
			break;

		case EDateMonth_OCTOBER:
			iMonthOffset = 0;
			break;

		case EDateMonth_NOVEMBER:
			iMonthOffset = 3;
			break;

		case EDateMonth_DECEMBER:
			iMonthOffset = 5;
			break;
		}

		iDayOffset = GetMonthDay() % 7;

		int iTotalOffset = ( ( iCenturyOffset + iYearOffset + iMonthOffset + iDayOffset ) - 1 ) % 7;
		_weekDay = EDateDay( EDateDay_MONDAY + iTotalOffset );
	}

	void CDate::DoComputeYearDay()
	{
		int leap = IsLeap( _year );
		_yearDay = 0;

		for ( int i = EDateMonth_JANUARY ; i < _month ; i++ )
		{
			switch ( i )
			{
			case EDateMonth_JANUARY:
			case EDateMonth_MARCH:
			case EDateMonth_MAY:
			case EDateMonth_JULY:
			case EDateMonth_AUGUST:
			case EDateMonth_OCTOBER:
				_yearDay += 31;
				break;

			case EDateMonth_FEBRUARY:
				_yearDay += 28 + leap;
				break;

			case EDateMonth_APRIL:
			case EDateMonth_JUNE:
			case EDateMonth_SEPTEMBER:
			case EDateMonth_NOVEMBER:
				_yearDay += 30;
				break;
			}
		}

		_yearDay += _monthDay;
	}

	int CDate::DoFindDay( const std::string & date, const std::string & format )
	{
		return DateUtils::FindDateDay( date, format );
	}

	int CDate::DoFindDay( const std::wstring & date, const std::wstring & format )
	{
		return DateUtils::FindDateDay( date, format );
	}

	int CDate::DoFindMonth( const std::string & /*date*/, const std::string & /*format*/ )
	{
		DB_EXCEPT( EDatabaseExceptionCodes_Unimplemented, STR( "Not implemented" ) );
		return -1;
	}

	int CDate::DoFindMonth( const std::wstring & /*date*/, const std::wstring & /*format*/ )
	{
		DB_EXCEPT( EDatabaseExceptionCodes_Unimplemented, STR( "Not implemented" ) );
		return -1;
	}

	int CDate::DoFindYear( const std::string & date, const std::string & format )
	{
		return DateUtils::FindDateYear( date, format );
	}

	int CDate::DoFindYear( const std::wstring & date, const std::wstring & format )
	{
		return DateUtils::FindDateYear( date, format );
	}

	bool CDate::DoIsDate( const std::string & date, const std::string & format, int & year, EDateMonth & month, int & monthDay )
	{
		return DateUtils::IsDate( date, format, year, month, monthDay );
	}

	bool CDate::DoIsDate( const std::wstring & date, const std::wstring & format, int & year, EDateMonth & month, int & monthDay )
	{
		return DateUtils::IsDate( date, format, year, month, monthDay );
	}

	bool operator ==( const CDate & lhs, const CDate & rhs )
	{
		bool bReturn = false;

		if ( lhs.IsValid() && rhs.IsValid() )
		{
			bReturn = ( lhs._month == rhs._month && lhs._monthDay == rhs._monthDay && lhs._year == rhs._year );
		}
		else
		{
			bReturn = ( lhs.IsValid() == rhs.IsValid() );
		}

		return bReturn;
	}

	bool operator !=( const CDate & lhs, const CDate & rhs )
	{
		return !( lhs == rhs );
	}

	std::ostream & operator <<( std::ostream & stream, const CDate & date )
	{
		stream << date.Format( "%Y-%m-%d" );
		return stream;
	}

	std::wostream & operator <<( std::wostream & stream, const CDate & date )
	{
		stream << date.Format( L"%Y-%m-%d" );
		return stream;
	}

}
END_NAMESPACE_DATABASE
