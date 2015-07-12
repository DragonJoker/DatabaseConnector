/************************************************************************//**
 * @file DatabaseStatementParameterSqlite.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 *
 *
 * @brief CDatabaseStatementParameterSqlite class declaration.
 *
 * @details Describes a statement parameter for SQLITE database.
 *
 ***************************************************************************/

#include "DatabaseSqlitePch.h"

#include "DatabaseStatementParameterSqlite.h"

#include "DatabaseConnectionSqlite.h"
#include "DatabaseStatementParameterSetter.h"

#include <DatabaseException.h>
#include <DatabaseLogger.h>
#include <DatabaseStringUtils.h>

BEGIN_NAMESPACE_DATABASE_SQLITE
{
	static const String DATABASE_PARAMETER_TYPE_ERROR = STR( "Wrong parameter type when trying to set its binding." );

	CDatabaseStatementParameterSqlite::CDatabaseStatementParameterSqlite( DatabaseConnectionSqlitePtr connection, const String & name, unsigned short index, EFieldType fieldType, EParameterType parameterType, SValueUpdater * updater )
		: CDatabaseParameter( connection, name, index, fieldType, parameterType, updater )
		, CDatabaseParameterSqlite( fieldType )
		, _statement( NULL )
	{
	}

	CDatabaseStatementParameterSqlite::CDatabaseStatementParameterSqlite( DatabaseConnectionSqlitePtr connection, const String & name, unsigned short index, EFieldType fieldType, uint32_t limits, EParameterType parameterType, SValueUpdater * updater )
		: CDatabaseParameter( connection, name, index, fieldType, limits, parameterType, updater )
		, CDatabaseParameterSqlite( fieldType )
		, _statement( NULL )
	{
	}

	CDatabaseStatementParameterSqlite::~CDatabaseStatementParameterSqlite()
	{
		_statement = NULL;
	}

	void CDatabaseStatementParameterSqlite::SetNull()
	{
		if ( _statement )
		{
			SQLite::BindNull( _statement, GetIndex() );
		}

		CDatabaseParameter::SetNull();
	}

	void CDatabaseStatementParameterSqlite::SetStatement( SQLite::Statement * statement )
	{
		_statement = statement;

		switch ( GetType() )
		{
		case EFieldType_BIT:
			_binding = MakeSqliteBind< EFieldType_BIT >( _statement, GetConnection(), GetIndex(), GetObjectValue() );
			break;

		case EFieldType_TINY_INTEGER:
			_binding = MakeSqliteBind< EFieldType_TINY_INTEGER >( _statement, GetConnection(), GetIndex(), GetObjectValue() );
			break;

		case EFieldType_SMALL_INTEGER:
			_binding = MakeSqliteBind< EFieldType_SMALL_INTEGER >( _statement, GetConnection(), GetIndex(), GetObjectValue() );
			break;

		case EFieldType_INTEGER:
			_binding = MakeSqliteBind< EFieldType_INTEGER >( _statement, GetConnection(), GetIndex(), GetObjectValue() );
			break;

		case EFieldType_LONG_INTEGER:
			_binding = MakeSqliteBind< EFieldType_LONG_INTEGER >( _statement, GetConnection(), GetIndex(), GetObjectValue() );
			break;

		case EFieldType_FLOATING_POINT_SIMPLE:
			_binding = MakeSqliteBind< EFieldType_FLOATING_POINT_SIMPLE >( _statement, GetConnection(), GetIndex(), GetObjectValue() );
			break;

		case EFieldType_FLOATING_POINT_DOUBLE:
			_binding = MakeSqliteBind< EFieldType_FLOATING_POINT_DOUBLE >( _statement, GetConnection(), GetIndex(), GetObjectValue() );
			break;

		case EFieldType_FIXED_POINT:
			_binding = MakeSqliteBind< EFieldType_FIXED_POINT >( _statement, GetConnection(), GetIndex(), GetObjectValue() );
			break;

		case EFieldType_VARCHAR:
			_binding = MakeSqliteBind< EFieldType_VARCHAR >( _statement, GetConnection(), GetIndex(), GetObjectValue() );
			break;

		case EFieldType_TEXT:
			_binding = MakeSqliteBind< EFieldType_TEXT >( _statement, GetConnection(), GetIndex(), GetObjectValue() );
			break;

		case EFieldType_NVARCHAR:
			_binding = MakeSqliteBind< EFieldType_NVARCHAR >( _statement, GetConnection(), GetIndex(), GetObjectValue() );
			break;

		case EFieldType_NTEXT:
			_binding = MakeSqliteBind< EFieldType_NTEXT >( _statement, GetConnection(), GetIndex(), GetObjectValue() );
			break;

		case EFieldType_DATE:
			_binding = MakeSqliteBind< EFieldType_DATE >( _statement, GetConnection(), GetIndex(), GetObjectValue() );
			break;

		case EFieldType_DATETIME:
			_binding = MakeSqliteBind< EFieldType_DATETIME >( _statement, GetConnection(), GetIndex(), GetObjectValue() );
			break;

		case EFieldType_TIME:
			_binding = MakeSqliteBind< EFieldType_TIME >( _statement, GetConnection(), GetIndex(), GetObjectValue() );
			break;

		case EFieldType_BINARY:
			_binding = MakeSqliteBind< EFieldType_BINARY >( _statement, GetConnection(), GetIndex(), GetObjectValue() );
			break;

		case EFieldType_VARBINARY:
			_binding = MakeSqliteBind< EFieldType_VARBINARY >( _statement, GetConnection(), GetIndex(), GetObjectValue() );
			break;

		case EFieldType_LONG_VARBINARY:
			_binding = MakeSqliteBind< EFieldType_LONG_VARBINARY >( _statement, GetConnection(), GetIndex(), GetObjectValue() );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}
	}
}
END_NAMESPACE_DATABASE_SQLITE
