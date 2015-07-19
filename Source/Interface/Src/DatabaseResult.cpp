/************************************************************************//**
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
#include "DatabaseFieldInfos.h"
#include "DatabaseException.h"
#include "DatabaseStringUtils.h"
#include "DatabaseLogger.h"

BEGIN_NAMESPACE_DATABASE
{
	static const String ERROR_DB_NO_FIELD = STR( "No field at index: " );

	CDatabaseResult::CDatabaseResult( DatabaseConnectionPtr connection )
		: _connection( connection )
		, _rowCount( 0 )
	{
		// Empty
	}

	CDatabaseResult::CDatabaseResult( DatabaseConnectionPtr connection, const DatabaseFieldInfosPtrArray & arrayFieldInfos )
		: _connection( connection )
		, _rowCount( 0 )
		, _arrayFieldInfos( arrayFieldInfos )
	{
	}

	CDatabaseResult::~CDatabaseResult()
	{
		_listRows.clear();
		_arrayFieldInfos.clear();
	}

	void CDatabaseResult::AddRow( DatabaseRowPtr row )
	{
		_listRows.push_back( row );
		_iterator = _listRows.begin();
		_rowCount++;
	}

	uint32_t CDatabaseResult::GetRowCount()
	{
		return _rowCount;
	}

	DatabaseRowPtr CDatabaseResult::GetNextRow()
	{
		DatabaseRowPtr pReturn;

		if ( _iterator != _listRows.end() )
		{
			pReturn = ( *_iterator );
			++_iterator;
		}

		return pReturn;
	}

	DatabaseRowPtr CDatabaseResult::GetFirstRow()
	{
		_iterator = _listRows.begin();
		return GetNextRow();
	}

	uint32_t CDatabaseResult::GetFieldCount() const
	{
		return ( uint32_t )_arrayFieldInfos.size();
	}

	DatabaseFieldInfosPtr CDatabaseResult::GetFieldInfos( uint32_t index ) const
	{
		if ( index >= _arrayFieldInfos.size() )
		{
			StringStream message;
			message << ERROR_DB_NO_FIELD << CStrUtils::ToString( index );
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, message.str() );
		}

		return _arrayFieldInfos[index];
	}
}
END_NAMESPACE_DATABASE
