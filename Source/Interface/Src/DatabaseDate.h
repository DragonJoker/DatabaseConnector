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
    
        static String ShortDay[7];        ///< Array of short day names.
        static String LongDay[7];         ///< Array of long day names.
        static String ShortMonth[12];     ///< Array of short month names.
        static String LongMonth[12];      ///< Array of long month names.
        static int MonthMaxDays[12];            ///< Array of max number of days per month.
        static CDateTime Today;                 ///< Today date/time
        
    public:
        /** Default constructor.
        */
        CDate();
        
        /** Constructor.
        @param[in] dateTime
            Date/time.
        */
        CDate( const CDateTime & dateTime );
        
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
        static bool IsDate( const std::string & date, CDate & result );
        
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
        static bool IsDate( const std::wstring & date, CDate & result );
        
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
        
        /** Check date consistency.
        @param[in] date
            String containing date.
        @param[in] format
            Format of date in string.
        @param[in] year
            Year.
        @param[in] month
            Month.
        @param[in] monthDay
            Day.
        @return
            true if valid date.
        */
        static bool DoIsDate( const std::string & date, const std::string & format, int & year, int & month, int & monthDay );
        
        /** Find day.
        @param[in] date
            Date.
        @param[in] format
            Format.
        @return
            Day.
        */
        static int DoFindDay( const std::string & date, const std::string & format );
        
        /** Find month.
        @param[in] date
            Date.
        @param[in] format
            Format.
        @return
            Month.
        */
        static int DoFindMonth( const std::string & date, const std::string & format );
        
        /** Find year.
        @param[in] date
            Date.
        @param[in] format
            Format.
        @return
            Year.
        */
        static int DoFindYear( const std::string & date, const std::string & format );
        
        /** Check date consistency.
        @param[in] date
            String containing date.
        @param[in] format
            Format of date in string.
        @param[in] year
            Year.
        @param[in] month
            Month.
        @param[in] monthDay
            Day.
        @return
            true if valid date.
        */
        static bool DoIsDate( const std::wstring & date, const std::wstring & format, int & year, int & month, int & monthDay );
        
        /** Find day.
        @param[in] date
            Date.
        @param[in] format
            Format.
        @return
            Day.
        */
        static int DoFindDay( const std::wstring & date, const std::wstring & format );
        
        /** Find month.
        @param[in] date
            Date.
        @param[in] format
            Format.
        @return
            Month.
        */
        static int DoFindMonth( const std::wstring & date, const std::wstring & format );
        
        /** Find year.
        @param[in] date
            Date.
        @param[in] format
            Format.
        @return
            Year.
        */
        static int DoFindYear( const std::wstring & date, const std::wstring & format );
        
        friend bool operator ==( const CDate & lhs, const CDate & rhs );
        
    protected:
    
        int         _year;      ///< Year.
        EDateMonth  _month;     ///< Month.
        int         _monthDay;  ///< Month day.
        int         _yearDay;   ///< Year day.
        EDateDay    _weekDay;   ///< Week day.
        
    };
    
    /** Check equality of two dates.
    @param lhs
        Left-hand side date.
    @param rhs
        Right-hand side date.
    @return
        true if objects are equal.
    */
    inline bool operator ==( const CDate & lhs, const CDate & rhs );
    
    /** Check inequality of two dates.
    @param lhs
        Left-hand side date.
    @param rhs
        Right-hand side date.
    @return
        true if objects are different.
    */
    inline bool operator !=( const CDate & lhs, const CDate & rhs );
    
}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_DATE_H___
