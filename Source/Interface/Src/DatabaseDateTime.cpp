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

#include "DatabaseDate.h"
#include "DatabaseTime.h"
#include "DatabaseDateTimeHelper.h"
#include "DatabaseException.h"

#include "DatabaseStringUtils.h"

BEGIN_NAMESPACE_DATABASE
{
#   define DATE_MAX_LENGTH 25

	namespace DateTimeUtils
	{
		static const size_t DATE_TIME_MAX_SIZE = 100;

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

		template< typename CharType >
		int ttoi( CharType const *& in, size_t count )
		{
			int result = ttoi( std::basic_string< CharType >( in, in + count ) );
			in += count;
			return result;
		}

		template< typename Char >
		std::basic_string< Char > FormatDateTime( const std::basic_string< Char > & format, int year, EDateMonth month, int yearDay, int monthDay, EDateDay weekDay, int hour, int min, int sec )
		{
			typedef std::basic_string< Char > String;
			typedef std::basic_stringstream< Char > StringStream;
			String strReturn( format );
			String strTmp;

			if ( weekDay > EDateDay_UNDEF && weekDay < EDateDay_SUNDAY )
			{
				Replace( strReturn, Str< Char >( "%a" ), Str< Char >( CDate::ShortDay[weekDay] ) );
				Replace( strReturn, Str< Char >( "%A" ), Str< Char >( CDate::LongDay[weekDay] ) );
				Replace( strReturn, Str< Char >( "%b" ), Str< Char >( CDate::ShortMonth[weekDay] ) );
				Replace( strReturn, Str< Char >( "%B" ), Str< Char >( CDate::LongMonth[weekDay] ) );
				StringStream strTmp2( Str< Char >( CDate::ShortDay[weekDay] ) + Str< Char >( " " ) + Str< Char >( CDate::ShortMonth[month] ) );
				strTmp2 << " " << year;
				Replace( strReturn, Str< Char >( "%c" ), strTmp2.str() );
			}

			Formalize( strTmp, DATE_TIME_MAX_SIZE, Str< Char >( "%02i" ).c_str(), monthDay );
			Replace( strReturn, Str< Char >( "%d" ), strTmp );
			Formalize( strTmp, DATE_TIME_MAX_SIZE, Str< Char >( "%02i" ).c_str(), monthDay );
			Replace( strReturn, Str< Char >( "%D" ), strTmp );
			Formalize( strTmp, DATE_TIME_MAX_SIZE, Str< Char >( "%03i" ).c_str(), yearDay );
			Replace( strReturn, Str< Char >( "%J" ), strTmp );
			Formalize( strTmp, DATE_TIME_MAX_SIZE, Str< Char >( "%02i" ).c_str(), month + 1 );
			Replace( strReturn, Str< Char >( "%m" ), strTmp );
			Formalize( strTmp, DATE_TIME_MAX_SIZE, Str< Char >( "%02i" ).c_str(), weekDay - 1 );
			Replace( strReturn, Str< Char >( "%w" ), strTmp );
			Formalize( strTmp, DATE_TIME_MAX_SIZE, Str< Char >( "%02i/%02i/%04i" ).c_str(), monthDay, month + 1, year );
			Replace( strReturn, Str< Char >( "%x" ), strTmp );
			Formalize( strTmp, DATE_TIME_MAX_SIZE, Str< Char >( "%04i" ).c_str(), year );
			Replace( strReturn, Str< Char >( "%y" ), strTmp.substr( 2, 2 ) );
			Formalize( strTmp, DATE_TIME_MAX_SIZE, Str< Char >( "%04i" ).c_str(), year );
			Replace( strReturn, Str< Char >( "%Y" ), strTmp );
			Formalize( strTmp, DATE_TIME_MAX_SIZE, Str< Char >( "%02i" ).c_str(), hour );
			Replace( strReturn, Str< Char >( "%H" ), strTmp );
			Formalize( strTmp, DATE_TIME_MAX_SIZE, Str< Char >( "%02i" ).c_str(), min );
			Replace( strReturn, Str< Char >( "%M" ), strTmp );
			Formalize( strTmp, DATE_TIME_MAX_SIZE, Str< Char >( "%02i" ).c_str(), sec );
			Replace( strReturn, Str< Char >( "%S" ), strTmp );
			strTmp.clear();
			Replace( strReturn, Str< Char >( "%%" ), Str< Char >( "%" ) );

			return strReturn;
		}

		template< typename Char >
		bool IsDateTime( const  std::basic_string< Char > & date, const  std::basic_string< Char > & format, int & year, EDateMonth & month, int & monthDay, int & hour, int & min, int & sec )
		{
			typedef std::basic_string< Char > String;
			bool bReturn = date.size() >= format.size() && !format.empty();

			monthDay = 0;
			month = EDateMonth_UNDEF;
			year = -1;
			hour = 0;
			min = 0;
			sec = 0;

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
							case 'H':
								hour = ttoi( dc, 2 );
								break;

							case 'M':
								min = ttoi( dc, 2 );
								break;

							case 'S':
								sec = ttoi( dc, 2 );
								break;

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
	}

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

	CDateTime::CDateTime( const CDate & date )
		:   _date( date.ToTm() )
	{
	}

	CDateTime::CDateTime( const CTime & time )
		:   _date( time.ToTm() )
	{
	}

	CDateTime::~CDateTime()
	{
		// Empty
	}

	void CDateTime::SetDateTime( int year, EDateMonth month, int day, int hour, int minute, int second )
	{
		_date = std::tm { 0 };

		if ( year - 1900 >= 0 )
		{
			_date.tm_year = year - 1900;
		}
		else
		{
			_date.tm_year = 0;
		}

		_date.tm_mon = month;
		_date.tm_mday = day;
		_date.tm_hour = hour;
		_date.tm_min = minute;
		_date.tm_sec = second;

	}

	String CDateTime::ToString() const
	{
		String strReturn = DATETIME_FORMAT_EXP;
		TChar pBuffer[DATE_MAX_LENGTH];
		strftime( pBuffer, DATE_MAX_LENGTH, strReturn.c_str(), &_date );
		strReturn = pBuffer;
		return strReturn;
	}

	std::string CDateTime::ToStdString() const
	{
		std::string strReturn = SDATETIME_FORMAT_EXP;
		char pBuffer[DATE_MAX_LENGTH];
		strftime( pBuffer, DATE_MAX_LENGTH, strReturn.c_str(), &_date );
		strReturn = pBuffer;
		return strReturn;
	}

	std::wstring CDateTime::ToStdWString() const
	{
		std::wstring strReturn = WDATETIME_FORMAT_EXP;
		wchar_t pBuffer[DATE_MAX_LENGTH];
		wcsftime( pBuffer, DATE_MAX_LENGTH, strReturn.c_str(), &_date );
		strReturn = pBuffer;
		return strReturn;
	}

	EDateDay CDateTime::GetWeekDay() const
	{
		return EDateDay( _date.tm_wday );
	}


	int CDateTime::GetYear() const
	{
		return 1900 + _date.tm_year;
	}

	EDateMonth CDateTime::GetMonth() const
	{
		return EDateMonth( _date.tm_mon );
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

	std::tm CDateTime::ToTm() const
	{
		return _date;
	}

	std::string CDateTime::Format( const std::string & format ) const
	{
		return DateTimeUtils::FormatDateTime( format, GetYear(), GetMonth(), GetYearDay(), GetMonthDay(), GetWeekDay(), GetHour(), GetMinute(), GetSecond() );
	}

	std::wstring CDateTime::Format( const std::wstring & format ) const
	{
		return DateTimeUtils::FormatDateTime( format, GetYear(), GetMonth(), GetYearDay(), GetMonthDay(), GetWeekDay(), GetHour(), GetMinute(), GetSecond() );
	}

	bool CDateTime::Parse( const std::string & date, const std::string & format )
	{
		bool bReturn = false;
		int iMonthDay = 0;
		EDateMonth eMonth = EDateMonth_UNDEF;
		int iYear = -1;
		int hour = 0;
		int min = 0;
		int sec = 0;

		bReturn = DateTimeUtils::IsDateTime( date, format, iYear, eMonth, iMonthDay, hour, min, sec );
		SetDateTime( iYear, eMonth, iMonthDay, hour, min, sec );

		return bReturn;
	}

	bool CDateTime::Parse( const std::wstring & date, const std::wstring & format )
	{
		bool bReturn = false;
		int iMonthDay = 0;
		EDateMonth eMonth = EDateMonth_UNDEF;
		int iYear = -1;
		int hour = 0;
		int min = 0;
		int sec = 0;

		bReturn = DateTimeUtils::IsDateTime( date, format, iYear, eMonth, iMonthDay, hour, min, sec );
		SetDateTime( iYear, eMonth, iMonthDay, hour, min, sec );

		return bReturn;
	}

	CDateTime CDateTime::Now()
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

	bool CDateTime::IsDateTime( const std::string & date, const std::string & format )
	{
		int iMonthDay = 0;
		EDateMonth eMonth = EDateMonth_UNDEF;
		int iYear = -1;
		int hour = 0;
		int min = 0;
		int sec = 0;

		return DateTimeUtils::IsDateTime( date, format, iYear, eMonth, iMonthDay, hour, min, sec );
	}

	bool CDateTime::IsDateTime( const std::wstring & date, const std::wstring & format )
	{
		int iMonthDay = 0;
		EDateMonth eMonth = EDateMonth_UNDEF;
		int iYear = -1;
		int hour = 0;
		int min = 0;
		int sec = 0;

		return DateTimeUtils::IsDateTime( date, format, iYear, eMonth, iMonthDay, hour, min, sec );
	}

	bool CDateTime::IsDateTime( const std::string & date, const std::string & format, CDateTime & result )
	{
		return result.Parse( date, format );
	}

	bool CDateTime::IsDateTime( const std::wstring & date, const std::wstring & format, CDateTime & result )
	{
		return result.Parse( date, format );
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

				result = CDateTime( to_tm( t ) );
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

				result = CDateTime( to_tm( t ) );
			}
			catch ( ... )
			{
				return false;
			}
		}

		return true;
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

	std::ostream & operator <<( std::ostream & stream, const CDateTime & dateTime )
	{
		stream << dateTime.Format( "%Y-%m-%d %H:%M:%S" );
		return stream;
	}

	std::wostream & operator <<( std::wostream & stream, const CDateTime & dateTime )
	{
		stream << dateTime.Format( L"%Y-%m-%d %H:%M:%S" );
		return stream;
	}
}
END_NAMESPACE_DATABASE
