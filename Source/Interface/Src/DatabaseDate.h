/************************************************************************//**
* @file Date.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDate class declaration.
*
* @details Describes a date.
*
***************************************************************************/

#ifndef ___DATABASE_DATE_H___
#define ___DATABASE_DATE_H___

#include "DatabasePrerequisites.h"

#include "EDateDay.h"
#include "EDateMonth.h"

BEGIN_NAMESPACE_DATABASE
{
	/** Describes a date.
	*/
	class DatabaseExport CDate
	{
	public:
		/** Default constructor.
		*/
		CDate();

		/** Constructor.
		@param[in] dateTime
		    Date/time.
		*/
		explicit CDate( const CDateTime & dateTime );

		/** Constructor
		@param[in] year
		    Year.
		@param[in] month
		    Month.
		@param[in] day
		    Day.
		*/
		CDate( int year, EDateMonth month, int day );

		/** Destructor.
		*/
		~CDate();

		/** Get date formatted according to the format mask.
		@param[in] format
		    Time format.
		@return
		    Formatted string.
		*/
		std::string Format( const std::string & format ) const;

		/** Get date formatted according to the format mask.
		@param[in] format
		    Time format.
		@return
		    Formatted string.
		*/
		std::wstring Format( const std::wstring & format ) const;

		/** Set date.
		@param[in] year
		    Year.
		@param[in] month
		    Month.
		@param[in] day
		    Day.
		*/
		void SetDate( int year, EDateMonth month, int day );

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
		static CDate Now();

		/** Check date consistency.
		@param[in] date
		    String containing date.
		@param[in] format
		    Format of date in string.
		@return
		    true if valid date.
		*/
		static bool IsDate( const std::string & date, const std::string & format );

		/** Check date consistency.
		@param[in] date
		    String containing date.
		@param[in] format
		    Format of date in string.
		@return
		    true if valid date.
		*/
		static bool IsDate( const std::wstring & date, const std::wstring & format );

		/** Check date consistency.
		@param[in] date
		    String containing date.
		@param[in] format
		    Format of date in string.
		@param[out] result
		    Resulting object.
		@return
		    true if valid date.
		*/
		static bool IsDate( const std::string & date, const std::string & format, CDate & result );

		/** Check date consistency.
		@param[in] date
		    String containing date.
		@param[in] format
		    Format of date in string.
		@param[out] result
		    Resulting object.
		@return
		    true if valid date.
		*/
		static bool IsDate( const std::wstring & date, const std::wstring & format, CDate & result );

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

		friend DatabaseExport bool operator ==( const CDate & lhs, const CDate & rhs );

	protected:
		//! Year.
		int _year;
		//! Month.
		EDateMonth _month;
		//! Month day.
		int _monthDay;
		//! Year day.
		int _yearDay;
		//! Week day.
		EDateDay _weekDay;
	};

	/** Check equality of two dates.
	@param lhs
	    Left-hand side date.
	@param rhs
	    Right-hand side date.
	@return
	    true if objects are equal.
	*/
	DatabaseExport bool operator ==( const CDate & lhs, const CDate & rhs );

	/** Check inequality of two dates.
	@param lhs
	    Left-hand side date.
	@param rhs
	    Right-hand side date.
	@return
	    true if objects are different.
	*/
	DatabaseExport bool operator !=( const CDate & lhs, const CDate & rhs );

	/** Stream operator
	@param stream
	    The stream.
	@param date
	    The date.
	@return
	    The stream.
	*/
	DatabaseExport std::ostream & operator <<( std::ostream & stream, const CDate & date );

	/** Stream operator
	@param stream
	    The stream.
	@param date
	    The date.
	@return
	    The stream.
	*/
	DatabaseExport std::wostream & operator <<( std::wostream & stream, const CDate & date );
}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_DATE_H___
