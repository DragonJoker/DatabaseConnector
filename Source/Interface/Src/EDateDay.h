/************************************************************************//**
* @file DateDay.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/17/2014 1:26:02 PM
*
*
* @brief EDateDay enumeration declaration.
*
* @details Enumeration of days.
*
***************************************************************************/

#ifndef ___DATABASE_DATE_DAY_H___
#define ___DATABASE_DATE_DAY_H___

#include "DatabasePrerequisites.h"

BEGIN_NAMESPACE_DATABASE
{
	/** Enumeration of days.
	*/
	typedef enum EDateDay
	{
		EDateDay_UNDEF,
		EDateDay_MONDAY,
		EDateDay_TUESDAY,
		EDateDay_WEDNESDAY,
		EDateDay_THURSDAY,
		EDateDay_FRIDAY,
		EDateDay_SATURDAY,
		EDateDay_SUNDAY
	}	EDateDay;
}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_DATE_DAY_H___
