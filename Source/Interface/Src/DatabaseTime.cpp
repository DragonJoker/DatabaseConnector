/************************************************************************//**
* @file Time.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief Time helper functions
*
***************************************************************************/

#include "DatabasePch.h"

#include "DatabaseTime.h"

#include "DatabaseStringUtils.h"
#include "DatabaseDateTimeHelper.h"

#include <boost/date_time/posix_time/posix_time.hpp>

BEGIN_NAMESPACE_DATABASE
{
	namespace Time
	{
		namespace Utils
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

				if ( bReturn )
				{
					bReturn = hours >= 0 && hours <= 23 && minutes >= 0 && minutes <= 59 && seconds >= 0 && seconds <= 59;
				}

				return bReturn;
			}
		}

		std::string Format( const TimeType & time, const std::string & format )
		{
			if ( time.is_not_a_date_time() )
			{
				throw std::out_of_range( "Time::Format - Not a date time" );
			}

			int hours = time.hours();
			int minutes = time.minutes();
			int seconds = time.seconds();

			if ( hours < 0 || hours > 23 )
			{
				throw std::out_of_range( "Time::Format - Invalid hours value" );
			}

			if ( minutes < 0 || minutes > 59 )
			{
				throw std::out_of_range( "Time::Format - Invalid minutes value" );
			}

			if ( seconds < 0 || seconds > 59 )
			{
				throw std::out_of_range( "Time::Format - Invalid seconds value" );
			}

			std::tm tmbuf = boost::posix_time::to_tm( time );
			char buffer[Utils::TIME_MAX_SIZE];
			size_t length = strftime( buffer, Utils::TIME_MAX_SIZE, format.c_str(), &tmbuf );
			assert( length < Utils::TIME_MAX_SIZE );
			return std::string( buffer, buffer + length );
		}

		std::wstring Format( const TimeType & time, const std::wstring & format )
		{
			if ( time.is_not_a_date_time() )
			{
				throw std::out_of_range( "Time::Format - Not a date time" );
			}

			int hours = time.hours();
			int minutes = time.minutes();
			int seconds = time.seconds();

			if ( hours < 0 || hours > 23 )
			{
				throw std::out_of_range( "Time::Format - Invalid hours value" );
			}

			if ( minutes < 0 || minutes > 59 )
			{
				throw std::out_of_range( "Time::Format - Invalid minutes value" );
			}

			if ( seconds < 0 || seconds > 59 )
			{
				throw std::out_of_range( "Time::Format - Invalid seconds value" );
			}

			std::tm tmbuf = boost::posix_time::to_tm( time );
			wchar_t buffer[Utils::TIME_MAX_SIZE];
			size_t length = wcsftime( buffer, Utils::TIME_MAX_SIZE, format.c_str(), &tmbuf );
			assert( length < Utils::TIME_MAX_SIZE );
			return std::wstring( buffer, buffer + length );
		}

		std::string Print( const TimeType & time, const std::string & format )
		{
			if ( time.is_not_a_date_time() )
			{
				throw std::out_of_range( "Time::Print - Not a date time" );
			}

			int hours = time.hours();
			int minutes = time.minutes();
			int seconds = time.seconds();

			if ( hours < 0 || hours > 23 )
			{
				throw std::out_of_range( "Time::Print - Invalid hours value" );
			}

			if ( minutes < 0 || minutes > 59 )
			{
				throw std::out_of_range( "Time::Print - Invalid minutes value" );
			}

			if ( seconds < 0 || seconds > 59 )
			{
				throw std::out_of_range( "Time::Print - Invalid seconds value" );
			}

			std::string result;
			StringUtils::Formalize( result, 1024, format.c_str(), hours, minutes, seconds );
			return result;
		}

		std::wstring Print( const TimeType & time, const std::wstring & format )
		{
			if ( time.is_not_a_date_time() )
			{
				throw std::out_of_range( "Time::Print - Not a date time" );
			}

			int hours = time.hours();
			int minutes = time.minutes();
			int seconds = time.seconds();

			if ( hours < 0 || hours > 23 )
			{
				throw std::out_of_range( "Time::Print - Invalid hours value" );
			}

			if ( minutes < 0 || minutes > 59 )
			{
				throw std::out_of_range( "Time::Print - Invalid minutes value" );
			}

			if ( seconds < 0 || seconds > 59 )
			{
				throw std::out_of_range( "Time::Print - Invalid seconds value" );
			}

			std::wstring result;
			StringUtils::Formalize( result, 1024, format.c_str(), hours, minutes, seconds );
			return result;
		}

		bool IsTime( const std::string & time, const std::string & format )
		{
			int hours = 0;
			int minutes = 0;
			int seconds = 0;

			return Utils::IsTime( time, format, hours, minutes, seconds );
		}

		bool IsTime( const std::wstring & time, const std::wstring & format )
		{
			int hours = 0;
			int minutes = 0;
			int seconds = 0;

			return Utils::IsTime( time, format, hours, minutes, seconds );
		}

		bool IsTime( const std::string & time, const std::string & format, TimeType & result )
		{
			int hours = 0;
			int minutes = 0;
			int seconds = 0;

			bool bReturn = Utils::IsTime( time, format, hours, minutes, seconds );

			if ( bReturn )
			{
				result = TimeType( hours, minutes, seconds );
			}
			else
			{
				result = TimeType( boost::posix_time::not_a_date_time );
			}

			return bReturn;
		}

		bool IsTime( const std::wstring & time, const std::wstring & format, TimeType & result )
		{
			int hours = 0;
			int minutes = 0;
			int seconds = 0;

			bool bReturn = Utils::IsTime( time, format, hours, minutes, seconds );

			if ( bReturn )
			{
				result = TimeType( hours, minutes, seconds );
			}
			else
			{
				result = TimeType( boost::posix_time::not_a_date_time );
			}

			return bReturn;
		}

		bool IsValid( const TimeType & time )
		{
			return !time.is_negative() && !time.is_not_a_date_time();
		}
	}
}
END_NAMESPACE_DATABASE
