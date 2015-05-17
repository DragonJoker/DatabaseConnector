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
	namespace
	{
		struct FieldFindCondition
		{
			const String & _name;

			FieldFindCondition( const String & name )
				: _name( name )
			{
			}

			bool operator()( DatabaseFieldPtr field )
			{
				return field->GetName() == _name;
			}
		};
	}

	static const String DATABASE_ROW_MISSING_FIELD_NAME_ERROR = STR( "Row misses field named: " );
	static const String DATABASE_ROW_MISSING_FIELD_INDEX_ERROR = STR( "Row misses field at index: " );

	CDatabaseRow::CDatabaseRow( DatabaseConnectionPtr connection )
		:   _connection( connection )
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

	bool CDatabaseRow::HasField( const String & name )
	{
		return std::find_if( _arrayFields.begin(), _arrayFields.end(), FieldFindCondition( name ) ) != _arrayFields.end();
	}

	DatabaseFieldPtr CDatabaseRow::GetField( const String & name )
	{
		DatabaseFieldPtr pReturn;
		DatabaseFieldPtrArray::iterator it = std::find_if( _arrayFields.begin(), _arrayFields.end(), FieldFindCondition( name ) );

		if ( it != _arrayFields.end() )
		{
			pReturn = ( *it );
		}
		else
		{
			StringStream message;
			message << DATABASE_ROW_MISSING_FIELD_NAME_ERROR << name;
			CLogger::LogError( message );
			DB_EXCEPT( EDatabaseExceptionCodes_RowError, message.str() );
		}

		return pReturn;
	}

	DatabaseFieldPtr CDatabaseRow::GetField( uint32_t index )
	{
		DatabaseFieldPtr pReturn;

		if ( index < _arrayFields.size() )
		{
			pReturn = _arrayFields[index];
		}
		else
		{
			StringStream message;
			message << DATABASE_ROW_MISSING_FIELD_INDEX_ERROR << index;
			CLogger::LogError( message );
			DB_EXCEPT( EDatabaseExceptionCodes_RowError, message.str() );
		}

		return pReturn;
	}

	String CDatabaseRow::GetStr( uint32_t index )
	{
		return GetField( index )->GetStrValue();
	}

	String CDatabaseRow::GetStr( const String & name )
	{
		return GetField( name )->GetStrValue();
	}

}
END_NAMESPACE_DATABASE
