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
								year = stoi( dc, 4 );
								break;

							case 'd':
								monthDay = stoi( dc, 2 );
								break;

							case 'm':
								month = EDateMonth( stoi( dc, 2 ) - 1 );
								break;

							case 'y':
								year = stoi( dc, 2 ) + 1900;
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
					bReturn = monthDay <= MonthMaxDays[month];
				}
				else
				{
					bReturn = monthDay <= ( MonthMaxDays[month] + IsLeap( year ) );
				}
			}

			return bReturn;
		}
	}

	CDate::CDate()
		: _year( 1 )
		, _month( EDateMonth_JANUARY )
		, _monthDay( 1 )
		, _yearDay( 0 )
		, _weekDay( EDateDay_UNDEF )
	{
		DoCheckValidity();
		DoComputeWeekDay();
		DoComputeYearDay();
	}

	CDate::CDate( const CDateTime & dateTime )
		: _year( dateTime.GetYear() )
		, _month( EDateMonth( dateTime.GetMonth() ) )
		, _monthDay( dateTime.GetMonthDay() )
		, _yearDay( 0 )
		, _weekDay( EDateDay_UNDEF )
	{
		DoCheckValidity();
		DoComputeWeekDay();
		DoComputeYearDay();
	}

	CDate::CDate( int year, EDateMonth month, int day )
		: _year( year )
		, _month( month )
		, _monthDay( day )
		, _yearDay( 0 )
		, _weekDay( EDateDay_UNDEF )
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
		std::string result;

		if ( !IsValid() )
		{
			std::stringstream stream;
			stream << "invalid: (" << GetYear() << "-" << ( GetMonth() + 1 ) << "-" << GetMonthDay() << ")";
			result = stream.str();
		}
		else
		{
			char buffer[DateUtils::DATE_MAX_SIZE + 1] = { 0 };
			std::tm tm = ToTm();
			size_t length = strftime( buffer, DateUtils::DATE_MAX_SIZE, format.c_str(), &tm );
			assert( length < DateUtils::DATE_MAX_SIZE );
			result = std::string( buffer, buffer + length );
		}

		return result;
	}

	std::wstring CDate::Format( const std::wstring & format ) const
	{
		std::wstring result;

		if ( !IsValid() )
		{
			std::wstringstream stream;
			stream << L"invalid: (" << GetYear() << L"-" << ( GetMonth() + 1 ) << L"-" << GetMonthDay() << L")";
			result = stream.str();
		}
		else
		{
			wchar_t buffer[DateUtils::DATE_MAX_SIZE + 1] = { 0 };
			std::tm tm = ToTm();
			size_t length = wcsftime( buffer, DateUtils::DATE_MAX_SIZE, format.c_str(), &tm );
			assert( length < DateUtils::DATE_MAX_SIZE );
			result = std::wstring( buffer, buffer + length );
		}

		return result;
	}

	void CDate::SetDate( int year, EDateMonth month, int day )
	{
		_year = year;
		_month = month;
		_monthDay = day;
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

		if ( iMonth >= EDateMonth_JANUARY && iMonthDay > 0 && iYear >= 0 )
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
