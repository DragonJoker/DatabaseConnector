/************************************************************************//**
* @file DatabaseRow.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 3/24/2014 8:37:01 AM
*
*
* @brief CDatabaseRow class definition.
*
* @details Describes a row of the result set.
*
***************************************************************************/

#include "DatabasePch.h"

#include "DatabaseRow.h"

#include "Database.h"
#include "DatabaseException.h"

BEGIN_NAMESPACE_DATABASE
{
	static const String ERROR_DB_ROW_MISSING_FIELD_NAME = STR( "Row misses field named: " );
	static const String ERROR_DB_ROW_MISSING_FIELD_INDEX = STR( "Row misses field at index: " );

	CDatabaseRow::CDatabaseRow( DatabaseConnectionPtr connection )
		: _connection( connection )
	{
		// Empty
	}

	CDatabaseRow::~CDatabaseRow()
	{
		_arrayFields.clear();
	}

	void CDatabaseRow::AddField( DatabaseFieldPtr field )
	{
		if ( field )
		{
			_arrayFields.push_back( field );
		}
	}

	DatabaseFieldPtr CDatabaseRow::GetField( const String & name )
	{
		auto && it = std::find_if( _arrayFields.begin(), _arrayFields.end(), [&name]( DatabaseFieldPtr parameter )
		{
			return parameter->GetName() == name;
		} );

		if ( it == _arrayFields.end() )
		{
			StringStream message;
			message << ERROR_DB_ROW_MISSING_FIELD_NAME << name;
			CLogger::LogError( message );
			DB_EXCEPT( EDatabaseExceptionCodes_RowError, message.str() );
		}

		return *it;
	}

	DatabaseFieldPtr CDatabaseRow::GetField( uint32_t index )
	{
		if ( index >= _arrayFields.size() )
		{
			StringStream message;
			message << ERROR_DB_ROW_MISSING_FIELD_INDEX << index;
			CLogger::LogError( message );
			DB_EXCEPT( EDatabaseExceptionCodes_RowError, message.str() );
		}

		return _arrayFields[index];
	}
}
END_NAMESPACE_DATABASE
