/************************************************************************//**
* @file DatabaseValue.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 3/21/2014 10:51:06 AM
*
*
* @brief CDatabaseValueBase class definition.
*
* @details Describes the base class of inherited templated classes dealing with database fields.
*
***************************************************************************/

#include "DatabasePch.h"

#include "DatabaseValue.h"

BEGIN_NAMESPACE_DATABASE
{
	CDatabaseValueBase::CDatabaseValueBase( DatabaseConnectionSPtr connection )
		: _connection( connection )
		, _valueSize( 0 )
	{
		// Empty
	}

	CDatabaseValueBase::~CDatabaseValueBase()
	{
		// Empty
	}

	unsigned long & CDatabaseValueBase::GetPtrSize()
	{
		return _valueSize;
	}

	const unsigned long & CDatabaseValueBase::GetPtrSize()const
	{
		return _valueSize;
	}

	bool CDatabaseValueBase::IsNull() const
	{
		return _isNull;
	}

	void CDatabaseValueBase::SetNull( bool null )
	{
		_isNull = null;

		if ( null )
		{
			DoSetNull();
		}
	}
}
END_NAMESPACE_DATABASE
