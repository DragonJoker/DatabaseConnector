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
			return in;
		}

		template<> std::basic_string< wchar_t > Str< wchar_t, wchar_t >( const wchar_t * in )
		{
			return in;
		}

		template<> std::basic_string< wchar_t > Str< wchar_t, char >( const char * in )
		{
			return CStrUtils::ToWStr( in );
		}

		template<> std::basic_string< char > Str< char, wchar_t >( const wchar_t * in )
		{
			return CStrUtils::ToStr( in );
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
			Formalize( strTmp, DATE_MAX_SIZE, Str< Char >( "%02i" ).c_str(), month );
			Replace( strReturn, Str< Char >( "%m" ), strTmp );
			Formalize( strTmp, DATE_MAX_SIZE, Str< Char >( "%02i" ).c_str(), month );
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
		bool IsDate( const  std::basic_string< Char > & date, const  std::basic_string< Char > & format, int & year, int & month, int & monthDay )
		{
			typedef std::basic_string< Char > String;
			bool    bReturn = false;
			size_t  uiIndex;
			size_t  uiIndex2;
			String  strTmp;

			monthDay   = 0;
			month      = 0;
			year       = -1;

			if ( format == Str< Char >( "%Y%m%d" ) )
			{
				if ( date.size() >= 8 && IsInteger( date ) )
				{
					year       = ttoi( date.substr( 0, 4 ) );
					month      = ttoi( date.substr( 4, 2 ) );
					monthDay   = ttoi( date.substr( 6, 2 ) );
				}
			}
			else if ( format == Str< Char >( "CONVERT( DATE, '%Y%m%d' )" ) )
			{
				uiIndex = date.find( Str< Char >( "'" ) );

				if ( date.size() >= 26 && uiIndex != String::npos && IsInteger( date.substr( uiIndex + 1, 8 ) ) )
				{
					year       = ttoi( date.substr( uiIndex + 1, 4 ) );
					month      = ttoi( date.substr( uiIndex + 5, 2 ) );
					monthDay   = ttoi( date.substr( uiIndex + 7, 2 ) );
				}
			}
			else if ( format == Str< Char >( "{-d %Y-%m-%d}" ) )
			{
				if ( date.size() >= 15 )
				{
					year       = ttoi( date.substr( 4, 4 ) );
					month      = ttoi( date.substr( 9, 2 ) );
					monthDay   = ttoi( date.substr( 12, 2 ) );
				}
			}
			else if ( format == Str< Char >( "%Y-%m-%d" ) )
			{
				if ( date.size() >= 10 )
				{
					uiIndex = date.find( Str< Char >( "-" ), 0 );

					if ( uiIndex != String::npos )
					{
						if ( uiIndex == 0 )
						{
							year   = ttoi( date.substr( 0, 5 ) );
							uiIndex = date.find( Str< Char >( "-" ), 1 );
						}
						else
						{
							year = ttoi( date.substr( 0, 4 ) );
						}

						uiIndex2 = date.find( Str< Char >( "-" ), uiIndex + 1 );

						if ( uiIndex2 != String::npos )
						{
							month      = ttoi( date.substr( uiIndex + 1, 2 ) );
							monthDay   = ttoi( date.substr( uiIndex2 + 1, 2 ) );
						}
					}
				}
			}
			else if ( format == Str< Char >( "%d/%m/%Y" ) )
			{
				if ( date.size() >= 10 )
				{
					monthDay   = ttoi( date.substr( 0, 2 ) );
					month      = ttoi( date.substr( 3, 2 ) );
					year       = ttoi( date.substr( 6, 4 ) );
				}
			}
			else if ( format == Str< Char >( "%Y-%Y" ) )
			{
				if ( date.size() >= 4 )
				{
					year = ttoi( date.substr( 0, 4 ) );

					if ( year < 100 )
					{
						year += 1900;
					}

					month = 1;
					monthDay = 1;
				}
			}
			else if ( format == Str< Char >( "%Y" ) )
			{
				if ( date.size() >= 4 )
				{
					year = ttoi( date.substr( 0, 4 ) );
					Formalize( strTmp, DATE_MAX_SIZE, Str< Char >( "%04i" ).c_str(), year );

					if ( year < 100 && date.substr( 0, 4 ) != strTmp )
					{
						year += 1900;
					}

					month = 1;
					monthDay = 1;
				}
			}

			if ( month != 0 && monthDay != 0 && year != -1 )
			{
				if ( month != EDateMonth_FEBRUARY )
				{
					if ( monthDay <= CDate::MonthMaxDays[month - 1] )
					{
						bReturn = true;
					}
				}
				else
				{
					int leap = IsLeap( year );

					if ( monthDay <= ( CDate::MonthMaxDays[month - 1] + leap ) )
					{
						bReturn = true;
					}
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
	CDateTime CDate::Today = CDateTime::GetCurrentTime();

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

	void CDate::SetDate( int year, EDateMonth month, int day )
	{
		_year      = year;
		_month     = month;
		_monthDay  = day;
		DoCheckValidity();
		DoComputeWeekDay();
		DoComputeYearDay();
	}

	std::string CDate::Format( const std::string & format ) const
	{
		return DateUtils::FormatDate( format, _year, _month, _yearDay, _monthDay, _weekDay );
	}

	std::wstring CDate::Format( const std::wstring & format ) const
	{
		return DateUtils::FormatDate( format, _year, _month, _yearDay, _monthDay, _weekDay );
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

	bool CDate::DoIsDate( const std::string & date, const std::string & format, int & year, int & month, int & monthDay )
	{
		return DateUtils::IsDate( date, format, year, month, monthDay );
	}

	bool CDate::DoIsDate( const std::wstring & date, const std::wstring & format, int & year, int & month, int & monthDay )
	{
		return DateUtils::IsDate( date, format, year, month, monthDay );
	}

	bool CDate::Parse( const std::string & date, const std::string & format )
	{
		bool bReturn = false;
		int iMonthDay = 0;
		int iMonth = 0;
		int iYear = -1;

		bReturn = DateUtils::IsDate( date, format, iYear, iMonth, iMonthDay );
		SetDate( iYear, EDateMonth( iMonth ), iMonthDay );

		return bReturn;
	}

	bool CDate::Parse( const std::wstring & date, const std::wstring & format )
	{
		bool bReturn = false;
		int iMonthDay = 0;
		int iMonth = 0;
		int iYear = -1;

		bReturn = DateUtils::IsDate( date, format, iYear, iMonth, iMonthDay );
		SetDate( iYear, EDateMonth( iMonth ), iMonthDay );

		return bReturn;
	}

	bool CDate::IsDate( const std::string & date, const std::string & format )
	{
		int iMonthDay = 0;
		int iMonth = 0;
		int iYear = -1;

		return DateUtils::IsDate( date, format, iYear, iMonth, iMonthDay );
	}

	bool CDate::IsDate( const std::wstring & date, const std::wstring & format )
	{
		int iMonthDay = 0;
		int iMonth = 0;
		int iYear = -1;

		return DateUtils::IsDate( date, format, iYear, iMonth, iMonthDay );
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

	bool CDate::IsValid() const
	{
		bool bReturn = false;
		int iMonthDay = _monthDay;
		int iMonth = _month;
		int iYear = _year;

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

	void CDate::DoCheckValidity()
	{
		if ( _month < EDateMonth_JANUARY )
		{
			_month = EDateMonth_JANUARY;
		}
		else if ( _month > EDateMonth_DECEMBER )
		{
			_month = EDateMonth_DECEMBER;
		}

		if ( _month != EDateMonth_FEBRUARY )
		{
			if ( _monthDay > MonthMaxDays[_month - 1] )
			{
				_month = EDateMonth_JANUARY;
				_monthDay = 0;
				_year = -1;
			}
		}
		else
		{
			int leap = IsLeap( _year );

			if ( _monthDay > ( MonthMaxDays[_month - 1] + leap ) )
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

		int iCentury = _year / 100;
		iCenturyOffset = ( ( 39 - iCentury ) % 4 ) * 2;

		int iYear = _year - ( iCentury * 100 );
		iYearOffset = ( ( iYear / 4 ) + iYear ) % 7;

		switch ( _month )
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

		iDayOffset = _monthDay % 7;

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

}
END_NAMESPACE_DATABASE
