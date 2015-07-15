/************************************************************************//**
* @file DateTime.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDateTime class declaration.
*
* @details Describes a date/time.
*
***************************************************************************/

#ifndef ___DATABASE_DATE_TIME_H___
#define ___DATABASE_DATE_TIME_H___

#include "DatabasePrerequisites.h"

#include "EDateDay.h"
#include "EDateMonth.h"

BEGIN_NAMESPACE_DATABASE
{
	/** Describes a date/time.
	*/
	class DatabaseExport CDateTime
	{
	public:
		/** Default constructor.
		*/
		CDateTime();

		/** Copy constructor.
		@param[in] dateTime
			Date/time.
		*/
		CDateTime( const CDateTime & dateTime );

		/** Constructor from CDate and a CTime.
		@param[in] date
			Date.
		*/
		CDateTime( const CDate & date, const CTime & time );

		/** Constructor from struct tm.
		@param[in] dateTime
			Date/time.
		*/
		explicit CDateTime( const std::tm & dateTime );

		/** Constructor from CDate.
		@param[in] date
			Date.
		*/
		explicit CDateTime( const CDate & date );

		/** Constructor from CTime.
		@param[in] time
			Time.
		*/
		explicit CDateTime( const CTime & time );

		/** Destructor.
		*/
		~CDateTime();

		/** Get date/time formatted according to the format mask.
		@param[in] format
			Time format.
		@return
			Formatted string.
		*/
		std::string Format( const std::string & format ) const;

		/** Get date/time formatted according to the format mask.
		@param[in] format
			Time format.
		@return
			Formatted string.
		*/
		std::wstring Format( const std::wstring & format ) const;

		/** Set date/time.
		@param[in] year
			Year.
		@param[in] month
			Month.
		@param[in] day
			Day.
		@param[in] hour
			Hours.
		@param[in] minute
			Minutes.
		@param[in] second
			Seconds.
		*/
		void SetDateTime( int year, EDateMonth month, int day, int hour, int minute, int second );

		/** Get year.
		@return
			Year.
		*/
		int GetYear() const;

		/** Get month.
		@return
			Month.
		*/
		EDateMonth GetMonth() const;

		/** Get month day.
		@return
			Month day.
		*/
		int GetMonthDay() const;

		/** Get year day.
		@return
			Year day.
		*/
		int GetYearDay() const;

		/** Get week day.
		@return
			Week day.
		*/
		EDateDay GetWeekDay() const;

		/** Get hour.
		@return
			Hour.
		*/
		int GetHour() const;

		/** Get minute.
		@return
			Minute.
		*/
		int GetMinute() const;

		/** Get second.
		@return
			Second.
		*/
		int GetSecond() const;

		/** Set date.
		@param[in] date
			String containing time.
		@param[in] format
			Format of time in string.
		@return
			true if success.
		*/
		bool Parse( const std::string & date, const std::string & format );

		/** Set date.
		@param[in] date
			String containing time.
		@param[in] format
			Format of time in string.
		@return
			true if success.
		*/
		bool Parse( const std::wstring & date, const std::wstring & format );

		/** Check date consistency.
		@return
			true if valid date.
		*/
		bool IsValid() const;

		/** Convert this object to a std::tm.
		@return
			Converted date/time.
		*/
		std::tm ToTm() const;

		/** Get system current date/time.
		@return
			System current date/time.
		*/
		static CDateTime Now();

		/** Check date/time consistency.
		@param[in] dateTime
			String containing date/time.
		@param[in] format
			Format of date/time in string.
		@return
			true if valid date.
		*/
		static bool IsDateTime( const std::string & dateTime, const std::string & format );

		/** Check date/time consistency.
		@param[in] dateTime
			String containing date/time.
		@param[in] format
			Format of date/time in string.
		@return
			true if valid date.
		*/
		static bool IsDateTime( const std::wstring & dateTime, const std::wstring & format );

		/** Check date/time consistency.
		@param[in] dateTime
			String containing date/time.
		@param[in] format
			Format of date/time in string.
		@param[out] result
			Resulting object.
		@return
			true if valid date.
		*/
		static bool IsDateTime( const std::string & dateTime, const std::string & format, CDateTime & result );

		/** Check date/time consistency.
		@param[in] dateTime
			String containing date/time.
		@param[in] format
			Format of date/time in string.
		@param[out] result
			Resulting object.
		@return
			true if valid date.
		*/
		static bool IsDateTime( const std::wstring & dateTime, const std::wstring & format, CDateTime & result );

		/** Check date/time consistency.
		@param[in] dateTime
			String containing date/time.
		@param[out] result
			Resulting object.
		@return
			true if valid time.
		*/
		static bool IsDateTime( const std::string & dateTime, CDateTime & result );

		/** Check date/time consistency.
		@param[in] dateTime
			String containing date/time.
		@param[out] result
			Resulting object.
		@return
			true if valid time.
		*/
		static bool IsDateTime( const std::wstring & dateTime, CDateTime & result );

		/** Get number of days in a month.
		@param[in] month
			Month.
		@param[in] year
			Year.
		@return
			Number of days.
		*/
		static int GetMonthDays( int month, int year );

		/** Get number of days in a year.
		@param[in] year
			Year.
		@return
			Number of days.
		*/
		static int GetYearDays( int year );

	private:
		/** Check date validity.
		*/
		void DoCheckValidity();

		/** Compute week day.
		*/
		void DoComputeWeekDay();

		/** Compute year day.
		*/
		void DoComputeYearDay();

	protected:
		//! Date.
		std::tm _date;
	};

	/** Check equality of two date/time.
	@param lhs
		Left-hand side date/time.
	@param rhs
		Right-hand side date/time.
	@return
		true if objects are equal.
	*/
	DatabaseExport bool operator ==( const CDateTime & lhs, const CDateTime & rhs );

	/** Check inequality of two date/time.
	@param lhs
		Left-hand side date/time.
	@param rhs
		Right-hand side date/time.
	@return
		true if objects are different.
	*/
	DatabaseExport bool operator !=( const CDateTime & lhs, const CDateTime & rhs );

	/** Stream operator
	@param stream
		The stream.
	@param dateTime
		The date/time.
	@return
		The stream.
	*/
	DatabaseExport std::ostream & operator <<( std::ostream & stream, const CDateTime & dateTime );

	/** Stream operator
	@param stream
		The stream.
	@param dateTime
		The date/time.
	@return
		The stream.
	*/
	DatabaseExport std::wostream & operator <<( std::wostream & stream, const CDateTime & dateTime );
}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_DATE_TIME_H___
