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
	CDatabaseValueBase::CDatabaseValueBase( DatabaseConnectionPtr connection )
		: _isValueAsStringSet( false )
		, _isValueSet( false )
		, _connection( connection )
	{
		// Empty
	}

	CDatabaseValueBase::~CDatabaseValueBase()
	{
		// Empty
	}

	void CDatabaseValueBase::SetStrValue( const String & value )
	{
		_isValueAsStringSet = true;
		_isValueSet = _value == value;
		_value = value;
	}

	const String & CDatabaseValueBase::GetStrValue()
	{
		DoUpdateStrValue();
		return _value;
	}

	const long & CDatabaseValueBase::GetPtrSize()
	{
		DoUpdateTValue();
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

	void CDatabaseValueBase::DoReset()
	{
		_value.clear();
		_isValueAsStringSet = false;
		_isValueSet = false;
	}
}
END_NAMESPACE_DATABASE
