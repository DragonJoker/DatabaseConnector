/************************************************************************//**
* @file DateTime.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief DateTime helper functions.
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
	namespace DateTime
	{
		namespace Utils
		{
			static const size_t DATE_TIME_MAX_SIZE = 100;

			template< typename Char >
			bool IsDateTime( const  std::basic_string< Char > & date, const  std::basic_string< Char > & format, int & year, int & month, int & monthDay, int & hours, int & minutes, int & seconds )
			{
				typedef std::basic_string< Char > String;
				bool bReturn = !format.empty();

				monthDay = 0;
				month = 0;
				year = -1;
				hours = 0;
				minutes = 0;
				seconds = 0;

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
									hours = stoi( dc, 2 );
									break;

								case 'M':
									minutes = stoi( dc, 2 );
									break;

								case 'S':
									seconds = stoi( dc, 2 );
									break;

								case 'Y':
									year = stoi( dc, 4 );
									break;

								case 'd':
									monthDay = stoi( dc, 2 );
									break;

								case 'm':
									month = stoi( dc, 2 );
									break;

								case 'y':
									year = stoi( dc, 2 ) + 1900;
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

				if ( bReturn )
				{
					bReturn = year >= boost::gregorian::date( boost::gregorian::min_date_time ).year() && year <= boost::gregorian::date( boost::gregorian::max_date_time ).year();
				}

				if ( bReturn )
				{
					bReturn = month >= boost::gregorian::Jan && month <= boost::gregorian::Dec;
				}

				if ( bReturn )
				{
					if ( month != boost::gregorian::Feb )
					{
						bReturn = monthDay <= MonthMaxDays[month];
					}
					else
					{
						bReturn = monthDay <= ( MonthMaxDays[month] + IsLeap( year ) );
					}
				}

				if ( bReturn )
				{
					bReturn = hours >= 0 && hours <= 23 && minutes >= 0 && minutes <= 59 && seconds >= 0 && seconds <= 59;
				}

				return bReturn;
			}
		}

		std::string Format( const DateTimeType & dateTime, const std::string & format )
		{
			char buffer[Utils::DATE_TIME_MAX_SIZE + 1] = { 0 };
			std::tm tm = boost::posix_time::to_tm( dateTime );
			size_t length = strftime( buffer, Utils::DATE_TIME_MAX_SIZE, format.c_str(), &tm );
			assert( length < Utils::DATE_TIME_MAX_SIZE );
			return std::string( buffer, buffer + length );
		}

		std::wstring Format( const DateTimeType & dateTime, const std::wstring & format )
		{
			wchar_t buffer[Utils::DATE_TIME_MAX_SIZE + 1] = { 0 };
			std::tm tm = boost::posix_time::to_tm( dateTime );
			size_t length = wcsftime( buffer, Utils::DATE_TIME_MAX_SIZE, format.c_str(), &tm );
			assert( length < Utils::DATE_TIME_MAX_SIZE );
			return std::wstring( buffer, buffer + length );
		}

		std::string Print( const DateTimeType & dateTime, const std::string & format )
		{
			auto year = dateTime.date().year();
			auto month = dateTime.date().month();
			auto day = dateTime.date().day();
			auto hours = dateTime.time_of_day().hours();
			auto minutes = dateTime.time_of_day().minutes();
			auto seconds = dateTime.time_of_day().seconds();
			std::string result;
			StringUtils::Formalize( result, 1024, format.c_str(), year, month, day, hours, minutes, seconds );
			return result;
		}

		std::wstring Print( const DateTimeType & dateTime, const std::wstring & format )
		{
			auto year = dateTime.date().year();
			auto month = dateTime.date().month();
			auto day = dateTime.date().day();
			auto hours = dateTime.time_of_day().hours();
			auto minutes = dateTime.time_of_day().minutes();
			auto seconds = dateTime.time_of_day().seconds();
			std::wstring result;
			StringUtils::Formalize( result, 1024, format.c_str(), year, month, day, hours, minutes, seconds );
			return result;
		}

		bool IsDateTime( const std::string & date, const std::string & format )
		{
			int iMonthDay = 0;
			int iMonth = 0;
			int iYear = -1;
			int hour = 0;
			int min = 0;
			int sec = 0;

			return Utils::IsDateTime( date, format, iYear, iMonth, iMonthDay, hour, min, sec );
		}

		bool IsDateTime( const std::wstring & date, const std::wstring & format )
		{
			int iMonthDay = 0;
			int iMonth = 0;
			int iYear = -1;
			int hour = 0;
			int min = 0;
			int sec = 0;

			return Utils::IsDateTime( date, format, iYear, iMonth, iMonthDay, hour, min, sec );
		}

		bool IsDateTime( const std::string & date, const std::string & format, DateTimeType & result )
		{
			int iMonthDay = 0;
			int iMonth = 0;
			int iYear = -1;
			int hour = 0;
			int min = 0;
			int sec = 0;

			bool bReturn = Utils::IsDateTime( date, format, iYear, iMonth, iMonthDay, hour, min, sec );

			if ( bReturn )
			{
				result = DateTimeType( DateType( iYear, iMonth, iMonthDay ), TimeType( hour, min, sec ) );
			}
			else
			{
				result = DateTimeType( boost::posix_time::not_a_date_time );
			}

			return bReturn;
		}

		bool IsDateTime( const std::wstring & date, const std::wstring & format, DateTimeType & result )
		{
			int iMonthDay = 0;
			int iMonth = 0;
			int iYear = -1;
			int hour = 0;
			int min = 0;
			int sec = 0;

			bool bReturn = Utils::IsDateTime( date, format, iYear, iMonth, iMonthDay, hour, min, sec );

			if ( bReturn )
			{
				result = DateTimeType( DateType( iYear, iMonth, iMonthDay ), TimeType( hour, min, sec ) );
			}
			else
			{
				result = DateTimeType( boost::posix_time::not_a_date_time );
			}

			return bReturn;
		}

		bool IsValid( const DateTimeType & dateTime )
		{
			return !dateTime.date().is_not_a_date() && !dateTime.time_of_day().is_negative() && !dateTime.time_of_day().is_not_a_date_time();
		}
	}
}
END_NAMESPACE_DATABASE
