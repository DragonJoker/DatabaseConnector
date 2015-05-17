/************************************************************************//**
 * @file Time.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 *
 *
 * @brief CTime class declaration.
 *
 * @details Describes a time.
 *
 ***************************************************************************/

#ifndef ___DATABASE_TIME_H___
#define ___DATABASE_TIME_H___

#include "DatabasePrerequisites.h"

BEGIN_NAMESPACE_DATABASE
{
	/** Describes a time.
	*/
	class DatabaseExport CTime
	{

	public:

		/** Default constructor.
		*/
		CTime();

		/** Constructor.
		@param time
		    Time.
		*/
		CTime( const std::tm & time );

		/** Constructor.
		@param dateTime
		    Date/time.
		*/
		CTime( const CDateTime & dateTime );

		/** Constructor.
		@param hours
		    Hours.
		@param minutes
		    Minutes.
		@param seconds
		    Seconds.
		*/
		CTime( int hours, int minutes, int seconds );

		/** Convert to a std::tm
		@param[out] time
		    std::tm filled with current CTime data.
		*/
		void ToCLibTm( std::tm & time ) const;

		/** Get time from a std::tm
		@param[in] time
		    std::tm to "copy".
		*/
		void FromCLibTm( const std::tm & time );

		/** Get time formatted according to the format mask.
		@param[in] format
		    Time format.
		@return
		    Formatted string.
		*/
		std::string Format( const std::string & format ) const;

		/** Get time formatted according to the format mask.
		@param[in] format
		    Time format.
		@return
		    Formatted string.
		*/
		std::wstring Format( const std::wstring & format ) const;

		/** Set values.
		@param[in] hours
		    Hours.
		@param[in] minutes
		    Minutes.
		@param[in] seconds
		    Seconds.
		*/
		void SetTime( int hours, int minutes, int seconds );

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

		/** Set time.
		@param[in] time
		    String containing time.
		@param[in] format
		    Format of time in string.
		@return
		    true if success.
		*/
		bool Parse( const std::string & time, const std::string & format );

		/** Set time.
		@param[in] time
		    String containing time.
		@param[in] format
		    Format of time in string.
		@return
		    true if success.
		*/
		bool Parse( const std::wstring & time, const std::wstring & format );

		/** Check time consistency.
		@return
		    true if valid time.
		*/
		bool IsValid() const;

		/** Check time consistency.
		@param[in] time
		    String containing time.
		@param[in] format
		    Format of time in string.
		@return
		    true if valid time.
		*/
		static bool IsTime( const std::string & time, const std::string & format );

		/** Check time consistency.
		@param[in] time
		    String containing time.
		@param[in] format
		    Format of time in string.
		@return
		    true if valid time.
		*/
		static bool IsTime( const std::wstring & time, const std::wstring & format );

		/** Check time consistency.
		@param[in] time
		    String containing time.
		@param[in] format
		    Format of time in string.
		@param[out] result
		    Resulting object.
		@return
		    true if valid time.
		*/
		static bool IsTime( const std::string & time, const std::string & format, CTime & result );

		/** Check time consistency.
		@param[in] time
		    String containing time.
		@param[in] format
		    Format of time in string.
		@param[out] result
		    Resulting object.
		@return
		    true if valid time.
		*/
		static bool IsTime( const std::wstring & time, const std::wstring & format, CTime & result );

		/** Check time consistency.
		@param[in] dateTime
		    String containing date/time.
		@param[out] result
		    Resulting object.
		@return
		    true if valid time.
		*/
		static bool IsTime( const std::string & dateTime, CTime & result );

		/** Check time consistency.
		@param[in] dateTime
		    String containing date/time.
		@param[out] result
		    Resulting object.
		@return
		    true if valid time.
		*/
		static bool IsTime( const std::wstring & dateTime, CTime & result );

	private:

		/** Check time consistency.
		@param[in] time
		    String containing time.
		@param[in] format
		    Format of time in string.
		@param[in] hours
		    Hours.
		@param[in] minutes
		    Minutes.
		@param[in] seconds
		    Seconds.
		@return
		    true if valid time.
		*/
		static bool DoIsTime( const std::string & time, const std::string & format, int & hours, int & minutes, int & seconds );

		/** Check time consistency.
		@param[in] time
		    String containing time.
		@param[in] format
		    Format of time in string.
		@param[in] hours
		    Hours.
		@param[in] minutes
		    Minutes.
		@param[in] seconds
		    Seconds.
		@return
		    true if valid time.
		*/
		static bool DoIsTime( const std::wstring & time, const std::wstring & format, int & hours, int & minutes, int & seconds );

		friend bool operator ==( const CTime & lhs, const CTime & rhs );

	protected:

		time_t _time;   ///< Time value.

	};

	/** Check equality of two times.
	@param lhs
	    Left-hand side time.
	@param rhs
	    Right-hand side time.
	@return
	    true if objects are equal.
	*/
	inline bool operator ==( const CTime & lhs, const CTime & rhs );

	/** Check inequality of two times.
	@param lhs
	    Left-hand side time.
	@param rhs
	    Right-hand side time.
	@return
	    true if objects are different.
	*/
	inline bool operator !=( const CTime & lhs, const CTime & rhs );

}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_TIME_H___
