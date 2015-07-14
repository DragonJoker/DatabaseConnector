/************************************************************************//**
* @file DateMonth.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/17/2014 1:19:49 PM
*
*
* @brief EDateMonth enumeration declaration.
*
* @details Enumeration of monthes.
*
***************************************************************************/

#ifndef ___DATABASE_DATE_MONTH_H___
#define ___DATABASE_DATE_MONTH_H___

#include "DatabasePrerequisites.h"

BEGIN_NAMESPACE_DATABASE
{
	/** Enumeration of monthes.
	*/
	typedef enum EDateMonth
	{
		EDateMonth_UNDEF = -1,
		EDateMonth_JANUARY = 0,
		EDateMonth_FEBRUARY,
		EDateMonth_MARCH,
		EDateMonth_APRIL,
		EDateMonth_MAY,
		EDateMonth_JUNE,
		EDateMonth_JULY,
		EDateMonth_AUGUST,
		EDateMonth_SEPTEMBER,
		EDateMonth_OCTOBER,
		EDateMonth_NOVEMBER,
		EDateMonth_DECEMBER
	}	EDateMonth;
}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_DATE_MONTH_H___
