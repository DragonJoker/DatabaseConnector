﻿/************************************************************************//**
* @file DatabaseResult.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 3/24/2014 8:37:01 AM
*
*
* @brief CDatabaseResult class definition.
*
* @details Describes the result of a SELECT request.
*
***************************************************************************/

#include "DatabasePch.h"

#include "DatabaseResult.h"

#include "Database.h"
#include "DatabaseConnection.h"
#include "DatabaseValuedObjectInfos.h"
#include "DatabaseException.h"
#include "DatabaseStringUtils.h"
#include "DatabaseLogger.h"

BEGIN_NAMESPACE_DATABASE
{
	static const String ERROR_DB_NO_FIELD = STR( "No field at index: " );

	CDatabaseResult::CDatabaseResult( const DatabaseValuedObjectInfosPtrArray & arrayFieldInfos )
		: _rowCount( 0 )
		, _arrayFieldInfos( arrayFieldInfos )
	{
		// Empty
	}

	CDatabaseResult::~CDatabaseResult()
	{
		_listRows.clear();
		_arrayFieldInfos.clear();
	}

	void CDatabaseResult::AddRow( DatabaseRowSPtr row )
	{
		_listRows.push_back( row );
		_iterator = _listRows.begin();
		_rowCount++;
	}

	uint32_t CDatabaseResult::GetRowCount()
	{
		return _rowCount;
	}

	DatabaseRowSPtr CDatabaseResult::GetNextRow()
	{
		DatabaseRowSPtr pReturn;

		if ( _iterator != _listRows.end() )
		{
			pReturn = ( *_iterator );
			++_iterator;
		}

		return pReturn;
	}

	DatabaseRowSPtr CDatabaseResult::GetFirstRow()
	{
		_iterator = _listRows.begin();
		return GetNextRow();
	}

	uint32_t CDatabaseResult::GetFieldCount() const
	{
		return ( uint32_t )_arrayFieldInfos.size();
	}

	DatabaseValuedObjectInfosSPtr CDatabaseResult::GetFieldInfos( uint32_t index ) const
	{
		if ( index >= _arrayFieldInfos.size() )
		{
			StringStream message;
			message << ERROR_DB_NO_FIELD << StringUtils::ToString( index );
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, message.str() );
		}

		return _arrayFieldInfos[index];
	}
}
END_NAMESPACE_DATABASE
