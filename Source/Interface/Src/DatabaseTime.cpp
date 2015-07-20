/************************************************************************//**
* @file Time.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CTime class definition.
*
* @details Describes a time.
*
***************************************************************************/

#include "DatabasePch.h"

#include "DatabaseTime.h"

#include "DatabaseDateTime.h"
#include "DatabaseDateTimeHelper.h"
#include "DatabaseStringUtils.h"

#include <boost/date_time/posix_time/posix_time.hpp>

BEGIN_NAMESPACE_DATABASE
{
	namespace TimeUtils
	{
		static const int TIME_MAX_SIZE = 100;

		template< typename Char >
		bool IsTime( const std::basic_string< Char > & time, const std::basic_string< Char > & format, int & hours, int & minutes, int & seconds )
		{
			typedef std::basic_string< Char > String;
			bool bReturn = !format.empty();

			hours = 0;
			minutes = 0;
			seconds = 0;

			if ( bReturn )
			{
				Char const * fc = format.data();
				Char const * dc = time.data();

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
								hours = stoi( dc, 2 );
								break;

							case 'M':
								minutes = stoi( dc, 2 );
								break;

							case 'S':
								seconds = stoi( dc, 2 );
								break;

							case '%':
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

			if ( hours >= 0 && hours <= 23 && minutes >= 0 && minutes <= 59 && seconds >= 0 && seconds <= 59 )
			{
				bReturn = true;
			}

			return bReturn;
		}
	}

	CTime::CTime()
		:   _time( 0 )
	{
		// Empty
	}

	CTime::CTime( const CDateTime & dateTime )
		:   _time( 0 )
	{
		FromCLibTm( dateTime.ToTm() );
	}

	CTime::CTime( int hours, int minutes, int seconds )
		:   _time( 0 )
	{
		time_t last_time = time( NULL );
		std::tm tmbuf = *localtime( &last_time );
		tmbuf.tm_hour = hours;
		tmbuf.tm_min = minutes;
		tmbuf.tm_sec = seconds;
		FromCLibTm( tmbuf );
	}

	void CTime::SetTime( int hours, int minutes, int seconds )
	{
		std::tm tmbuf = { 0 };
		tmbuf.tm_hour = hours;
		tmbuf.tm_min = minutes;
		tmbuf.tm_sec = seconds;
		FromCLibTm( tmbuf );
	}

	void CTime::ToCLibTm( std::tm & time ) const
	{
		time = *localtime( &_time );
	}

	void CTime::FromCLibTm( const std::tm & time )
	{
		_time = mktime( const_cast< std::tm * >( &time ) );
	}

	std::string CTime::Format( const std::string & format ) const
	{
		std::tm tmbuf = { 0 };
		ToCLibTm( tmbuf );
		char buffer[TimeUtils::TIME_MAX_SIZE];
		size_t length = strftime( buffer, TimeUtils::TIME_MAX_SIZE, format.c_str(), &tmbuf );
		assert( length < TimeUtils::TIME_MAX_SIZE );
		return std::string( buffer, buffer + length );
	}

	std::wstring CTime::Format( const std::wstring & format ) const
	{
		std::tm tmbuf = { 0 };
		ToCLibTm( tmbuf );
		wchar_t buffer[TimeUtils::TIME_MAX_SIZE];
		size_t length = wcsftime( buffer, TimeUtils::TIME_MAX_SIZE, format.c_str(), &tmbuf );
		assert( length < TimeUtils::TIME_MAX_SIZE );
		return std::wstring( buffer, buffer + length );
	}

	int CTime::GetHour() const
	{
		std::tm tmbuf = { 0 };
		ToCLibTm( tmbuf );
		return tmbuf.tm_hour;
	}

	int CTime::GetMinute() const
	{
		std::tm tmbuf = { 0 };
		ToCLibTm( tmbuf );
		return tmbuf.tm_min;
	}

	int CTime::GetSecond() const
	{
		std::tm tmbuf = { 0 };
		ToCLibTm( tmbuf );
		return tmbuf.tm_sec;
	}

	bool CTime::IsValid() const
	{
		std::tm tmbuf = { 0 };
		ToCLibTm( tmbuf );
		return tmbuf.tm_hour < 24 && tmbuf.tm_min < 60 && tmbuf.tm_sec < 60;
	}

	bool CTime::Parse( const std::string & time, const std::string & format )
	{
		bool bReturn = false;
		int hours = 0;
		int minutes = 0;
		int seconds = 0;

		bReturn = DoIsTime( time, format, hours, minutes, seconds );
		SetTime( hours, minutes, seconds );

		return bReturn;
	}

	bool CTime::Parse( const std::wstring & time, const std::wstring & format )
	{
		bool bReturn = false;
		int hours = 0;
		int minutes = 0;
		int seconds = 0;

		bReturn = DoIsTime( time, format, hours, minutes, seconds );
		SetTime( hours, minutes, seconds );

		return bReturn;
	}

	std::tm CTime::ToTm() const
	{
		std::tm ret = { 0 };
		ret.tm_hour = GetHour();
		ret.tm_min = GetMinute();
		ret.tm_sec = GetSecond();
		return ret;
	}

	CTime CTime::Now()
	{
		time_t last_time = time( NULL );
		std::tm tm;
#if defined( _WIN32 )
		localtime_s( &tm, &last_time );
#else
		tm = *std::localtime( &last_time );
#endif

		return CTime( tm.tm_hour, tm.tm_min, tm.tm_sec );
	}

	bool CTime::IsTime( const std::string & time, const std::string & format )
	{
		int hours = 0;
		int minutes = 0;
		int seconds = 0;

		return DoIsTime( time, format, hours, minutes, seconds );
	}

	bool CTime::IsTime( const std::wstring & time, const std::wstring & format )
	{
		int hours = 0;
		int minutes = 0;
		int seconds = 0;

		return DoIsTime( time, format, hours, minutes, seconds );
	}

	bool CTime::IsTime( const std::string & time, const std::string & format, CTime & result )
	{
		return result.Parse( time, format );
	}

	bool CTime::IsTime( const std::wstring & time, const std::wstring & format, CTime & result )
	{
		return result.Parse( time, format );
	}

	bool CTime::DoIsTime( const std::string & time, const std::string & format, int & hours, int & minutes, int & seconds )
	{
		return TimeUtils::IsTime( time, format, hours, minutes, seconds );
	}

	bool CTime::DoIsTime( const std::wstring & time, const std::wstring & format, int & hours, int & minutes, int & seconds )
	{
		return TimeUtils::IsTime( time, format, hours, minutes, seconds );
	}

	bool operator ==( const CTime & lhs, const CTime & rhs )
	{
		return lhs._time == rhs._time;
	}

	bool operator !=( const CTime & lhs, const CTime & rhs )
	{
		return !( lhs == rhs );
	}

	std::ostream & operator <<( std::ostream & stream, const CTime & time )
	{
		stream << time.Format( "%H:%M:%S" );
		return stream;
	}

	std::wostream & operator <<( std::wostream & stream, const CTime & time )
	{
		stream << time.Format( L"%H:%M:%S" );
		return stream;
	}
}
END_NAMESPACE_DATABASE
