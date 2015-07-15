/************************************************************************//**
* @file DateTimeSpan.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDateTimeSpan class declaration.
*
* @details Describes a date and/or time difference.
*
***************************************************************************/

#ifndef ___DATABASE_DATE_TIME_SPAN_H___
#define ___DATABASE_DATE_TIME_SPAN_H___

#include "DatabasePrerequisites.h"

BEGIN_NAMESPACE_DATABASE
{
	/** Describes a date and/or time difference.
	*/
	class DatabaseExport CDateTimeSpan
	{
	public:
		/** Default constructor.
		*/
		CDateTimeSpan();

		/** Constructor.
		@param  span
			Number of days.
		*/
		CDateTimeSpan( double span );

		/** Constructeur
		@param  days
			Number of days.
		@param hours
			Hours.
		@param minutes
			Minutes.
		@param seconds
			Seconds.
		*/
		CDateTimeSpan( int days, int hours, int minutes, int seconds );

		/** Destructor.
		*/
		~CDateTimeSpan();

	private:
		/** Add a span to a date/time
		@param dateTime
			Date/time.
		@param dateTimeSpan
			Time interval.
		@return
			Date/time
		*/
		friend CDateTime operator +( const CDateTime & dateTime, const CDateTimeSpan & dateTimeSpan );

		/** Substract a span to a date/time
		@param dateTime
			Date/time.
		@param dateTimeSpan
			Time interval.
		@return
			Date/time
		*/
		friend CDateTime operator -( const CDateTime & dateTime, const CDateTimeSpan & dateTimeSpan );

	protected:
		//!< Difference (in days).
		double _span;
	};
}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_DATE_TIME_SPAN_H___
