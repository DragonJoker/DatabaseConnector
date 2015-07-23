/************************************************************************//**
* @file DateTimeHelper.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 3/19/2014 3:03:29 PM
*
*
* @brief Helping functions for date/time management.
*
* @details Helping functions for date/time management.
*
***************************************************************************/

#include "DatabasePch.h"

#include "DatabaseDateTimeHelper.h"

#include "Database.h"
#include "DatabaseDateTime.h"
#include "DatabaseException.h"
#include "DatabaseLogger.h"

#include <cstdarg>

#pragma warning( push )
#pragma warning( disable:4996 )

BEGIN_NAMESPACE_DATABASE
{
	bool IsLeap( int year )
	{
		return ( year % 4 == 0 ) && ( year % 100 != 0 || year % 400 == 0 );
	}

	DateType DateFromDateTime( DateTimeType const & dateTime )
	{
		return DateType( dateTime.date().year(), dateTime.date().month(), dateTime.date().day() );
	}

	TimeType TimeFromDateTime( DateTimeType const & dateTime )
	{
		return TimeType( dateTime.time_of_day().hours(), dateTime.time_of_day().minutes(), dateTime.time_of_day().seconds() );
	}
}
END_NAMESPACE_DATABASE

#pragma warning( pop )
