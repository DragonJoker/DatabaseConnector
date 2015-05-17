/************************************************************************//**
 * @file DateTimeSpan.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 *
 *
 * @brief CDateTimeSpan class definition.
 *
 * @details Describes a date and/or time difference.
 *
 ***************************************************************************/

#include "DatabasePch.h"

#include "DatabaseDateTimeSpan.h"

BEGIN_NAMESPACE_DATABASE
{
	CDateTimeSpan::CDateTimeSpan()
		:   _span( 0 )
	{
		// Empty
	}

	CDateTimeSpan::CDateTimeSpan( double span )
		:   _span( span )
	{
		// Empty
	}

	CDateTimeSpan::CDateTimeSpan( int days, int hours, int minutes, int seconds )
	{
		_span = days;
		_span += hours / 24.0;
		_span += ( minutes / 24.0 ) / 60.0;
		_span += ( ( seconds / 24.0 ) / 60.0 ) / 60.0;
	}

	CDateTimeSpan::~CDateTimeSpan()
	{
		// Empty
	}

}
END_NAMESPACE_DATABASE
