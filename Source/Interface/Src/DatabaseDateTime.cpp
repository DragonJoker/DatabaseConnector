/************************************************************************//**
 * @file DateTime.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 *
 *
 * @brief CDateTime class definition.
 *
 * @details Describes a date/time.
 *
 ***************************************************************************/

#include "DatabasePch.h"

#include "DatabaseDateTime.h"

#include "DatabaseStringUtils.h"

BEGIN_NAMESPACE_DATABASE
{
#   define DATE_MAX_LENGTH 25
	const String DATE_FORMAT_EXP = STR( "%Y/%m/%d %H:%M:%S" );
	const std::string SDATE_FORMAT_EXP = "%Y/%m/%d %H:%M:%S";
	const std::wstring WDATE_FORMAT_EXP = L"%Y/%m/%d %H:%M:%S";

	CDateTime::CDateTime()
	{
		_date.tm_isdst = -1;
	}


	CDateTime::CDateTime( const std::tm & dateTime )
		:   _date( dateTime )
	{
	}

	CDateTime::CDateTime( const CDateTime & dateTime )
		:   _date( dateTime.ToTm() )
	{
	}

	CDateTime::~CDateTime()
	{
		// Empty
	}

	void CDateTime::SetDateTime( int year, int month, int day, int hour, int minute, int second )
	{
		if ( year - 1900 >= 0 )
		{
			_date.tm_year = year - 1900;
		}
		else
		{
			_date.tm_year = 0;
		}

		if ( month - 1 >= 0 )
		{
			_date.tm_mon = month - 1;
		}
		else
		{
			_date.tm_mon = 0;
		}

		_date.tm_mday = day;
		_date.tm_hour = hour;
		_date.tm_min = minute;
		_date.tm_sec = second;

	}

	String CDateTime::ToString() const
	{
		String strReturn = DATE_FORMAT_EXP;
		TChar pBuffer[DATE_MAX_LENGTH];
		strftime( pBuffer, DATE_MAX_LENGTH, strReturn.c_str(), &_date );
		strReturn = pBuffer;
		return strReturn;
	}

	std::string CDateTime::ToStdString() const
	{
		std::string strReturn = SDATE_FORMAT_EXP;
		char pBuffer[DATE_MAX_LENGTH];
		strftime( pBuffer, DATE_MAX_LENGTH, strReturn.c_str(), &_date );
		strReturn = pBuffer;
		return strReturn;
	}

	std::wstring CDateTime::ToStdWString() const
	{
		std::wstring strReturn = WDATE_FORMAT_EXP;
		wchar_t pBuffer[DATE_MAX_LENGTH];
		wcsftime( pBuffer, DATE_MAX_LENGTH, strReturn.c_str(), &_date );
		strReturn = pBuffer;
		return strReturn;
	}

	int CDateTime::GetWeekDay() const
	{
		return _date.tm_wday;
	}


	int CDateTime::GetYear() const
	{
		return 1900 + _date.tm_year;
	}

	int CDateTime::GetMonth() const
	{
		return  _date.tm_mon + 1;
	}

	int CDateTime::GetMonthDay() const
	{
		return _date.tm_mday;
	}

	int CDateTime::GetYearDay() const
	{
		return _date.tm_yday;
	}


	int CDateTime::GetHour() const
	{
		return _date.tm_hour;
	}

	int CDateTime::GetMinute() const
	{
		return _date.tm_min;
	}

	int CDateTime::GetSecond() const
	{
		return _date.tm_sec;
	}

	CDateTime CDateTime::GetCurrentTime()
	{
		time_t last_time = time( NULL );
		std::tm tm;
#if defined( _WIN32 )
		localtime_s( &tm, &last_time );
#else
		tm = *std::localtime( &last_time );
#endif

		return CDateTime( tm );
	}

	bool CDateTime::IsDateTime( const std::string & dateTime, CDateTime & result )
	{
		if ( dateTime.empty() )
		{
			return false;
		}
		else
		{
			try
			{
				boost::posix_time::ptime t( boost::posix_time::time_from_string( dateTime ) );

				if ( t.is_not_a_date_time() )
				{
					return false;
				}

				result = to_tm( t );
			}
			catch ( ... )
			{
				return false;
			}
		}

		return true;
	}

	bool CDateTime::IsDateTime( const std::wstring & dateTime, CDateTime & result )
	{
		if ( dateTime.empty() )
		{
			return false;
		}
		else
		{
			try
			{
				boost::posix_time::ptime t( boost::posix_time::time_from_string( CStrUtils::ToStr( dateTime ) ) );

				if ( t.is_not_a_date_time() )
				{
					return false;
				}

				result = to_tm( t );
			}
			catch ( ... )
			{
				return false;
			}
		}

		return true;
	}

	std::tm CDateTime::ToTm() const
	{
		return _date;
	}

	bool operator ==( const CDateTime & a, const CDateTime & b )
	{
		return a.GetYear() == b.GetYear()
		&& a.GetMonth() == b.GetMonth()
		&& a.GetMonthDay() == b.GetMonthDay()
		&& a.GetHour() == b.GetHour()
		&& a.GetMinute() == b.GetMinute()
		&& a.GetSecond() == b.GetSecond();
	}

	bool operator !=( const CDateTime & a, const CDateTime & b )
	{
		return a.GetYear() != b.GetYear()
		|| a.GetMonth() != b.GetMonth()
		|| a.GetMonthDay() != b.GetMonthDay()
		|| a.GetHour() != b.GetHour()
		|| a.GetMinute() != b.GetMinute()
		|| a.GetSecond() != b.GetSecond();
	}
}
END_NAMESPACE_DATABASE

