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
	namespace Date
	{
		namespace Utils
		{
			static const size_t DATE_MAX_SIZE = 100;

			template< typename Char >
			bool IsDate( const  std::basic_string< Char > & date, const  std::basic_string< Char > & format, int & year, int & month, int & monthDay )
			{
				typedef std::basic_string< Char > String;
				bool bReturn = !format.empty();

				monthDay = 0;
				month = 0;
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
									month = stoi( dc, 2 );
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

				return bReturn;
			}
		}

		std::string Format( const DateType & date, const std::string & format )
		{
			std::string result;

			if ( !Date::IsValid( date ) )
			{
				std::stringstream stream;
				stream << "invalid: (" << date.year() << "-" << ( date.month() ) << "-" << date.day() << ")";
				result = stream.str();
			}
			else
			{
				char buffer[Utils::DATE_MAX_SIZE + 1] = { 0 };
				std::tm tm = boost::gregorian::to_tm( date );
				size_t length = strftime( buffer, Utils::DATE_MAX_SIZE, format.c_str(), &tm );
				assert( length < Utils::DATE_MAX_SIZE );
				result = std::string( buffer, buffer + length );
			}

			return result;
		}

		std::wstring Format( const DateType & date, const std::wstring & format )
		{
			std::wstring result;

			if ( !Date::IsValid( date ) )
			{
				std::wstringstream stream;
				stream << L"invalid: (" << date.year() << L"-" << ( date.month() ) << L"-" << date.day() << L")";
				result = stream.str();
			}
			else
			{
				wchar_t buffer[Utils::DATE_MAX_SIZE + 1] = { 0 };
				std::tm tm = boost::gregorian::to_tm( date );
				size_t length = wcsftime( buffer, Utils::DATE_MAX_SIZE, format.c_str(), &tm );
				assert( length < Utils::DATE_MAX_SIZE );
				result = std::wstring( buffer, buffer + length );
			}

			return result;
		}

		std::string Print( const DateType & date, const std::string & format )
		{
			int year = date.year();
			int month = date.month();
			int day = date.day();
			std::string result;
			StringUtils::Formalize( result, 1024, format.c_str(), year, month, day );
			return result;
		}

		std::wstring Print( const DateType & date, const std::wstring & format )
		{
			int year = date.year();
			int month = date.month();
			int day = date.day();
			std::wstring result;
			StringUtils::Formalize( result, 1024, format.c_str(), year, month, day );
			return result;
		}

		bool IsDate( const std::string & date, const std::string & format )
		{
			int iMonthDay = 0;
			int iMonth = 0;
			int iYear = -1;

			return Utils::IsDate( date, format, iYear, iMonth, iMonthDay );
		}

		bool IsDate( const std::wstring & date, const std::wstring & format )
		{
			int iMonthDay = 0;
			int iMonth = 0;
			int iYear = -1;

			return Utils::IsDate( date, format, iYear, iMonth, iMonthDay );
		}

		bool IsDate( const std::string & date, const std::string & format, DateType & result )
		{
			int iMonthDay = 0;
			int iMonth = 0;
			int iYear = -1;

			bool bReturn = Utils::IsDate( date, format, iYear, iMonth, iMonthDay );

			if ( bReturn )
			{
				result = DateType( iYear, iMonth, iMonthDay );
			}

			return bReturn;
		}

		bool IsDate( const std::wstring & date, const std::wstring & format, DateType & result )
		{
			int iMonthDay = 0;
			int iMonth = 0;
			int iYear = -1;

			bool bReturn = Utils::IsDate( date, format, iYear, iMonth, iMonthDay );

			if ( bReturn )
			{
				result = DateType( iYear, iMonth, iMonthDay );
			}

			return bReturn;
		}

		bool IsValid( const DateType & date )
		{
			return !date.is_not_a_date();
		}
	}
}
END_NAMESPACE_DATABASE
