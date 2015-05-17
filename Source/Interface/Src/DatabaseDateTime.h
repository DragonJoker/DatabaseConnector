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

		/** Constructor from struct tm.
		@param[in] dateTime
		    Date/time.
		*/
		CDateTime( const std::tm & dateTime );

		/** Copy constructor.
		@param[in] dateTime
		    Date/time.
		*/
		CDateTime( const CDateTime & dateTime );

		/** Destructor.
		*/
		~CDateTime();

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
		void SetDateTime( int year, int month, int day, int hour, int minute, int second );

		/** Get date as a string with format "DD/MM/YYYY HH:mm:ss".
		@return
		    Date as string.
		*/
		String ToString() const;

		/** Get date as a string with format "DD/MM/YYYY HH:mm:ss".
		@return
		    Date as string.
		*/
		std::string ToStdString() const;

		/** Get date as a string with format "DD/MM/YYYY HH:mm:ss".
		@return
		    Date as string.
		*/
		std::wstring ToStdWString() const;

		/** Get day of week.
		@return
		    Day of week (1 to 7, 1 = lundi)
		*/
		int GetWeekDay() const;

		/** Get system current date/time.
		@return
		    System current date/time.
		*/
		static CDateTime GetCurrentTime();

		/** Get year.
		@return
		    Year.
		*/
		int GetYear() const;

		/** Get month.
		@return
		    Month.
		*/
		int GetMonth() const;

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

		/** Check time consistency.
		@param[in] dateTime
		    String containing date/time.
		@param[out] result
		    Resulting object.
		@return
		    true if valid time.
		*/
		static bool IsDateTime( const std::string & dateTime, CDateTime & result );

		/** Check time consistency.
		@param[in] dateTime
		    String containing date/time.
		@param[out] result
		    Resulting object.
		@return
		    true if valid time.
		*/
		static bool IsDateTime( const std::wstring & dateTime, CDateTime & result );

		/** Convert this object to a std::tm.
		@return
		    Converted date/time.
		*/
		std::tm ToTm() const;

	protected:
		std::tm _date;   /// Date.
	};

	/** Check equality of two dates.
	@param lhs
	    Left-hand side date.
	@param rhs
	    Right-hand side date.
	@return
	    true if objects are equal.
	*/
	DatabaseExport bool operator ==( const CDateTime & lhs, const CDateTime & rhs );

	/** Check inequality of two dates.
	@param lhs
	    Left-hand side date.
	@param rhs
	    Right-hand side date.
	@return
	    true if objects are different.
	*/
	DatabaseExport bool operator !=( const CDateTime & lhs, const CDateTime & rhs );
}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_DATE_TIME_H___
