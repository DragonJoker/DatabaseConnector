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
	static const String ERROR_SQLITE_PARAMETER_TYPE = STR( "Undefined parameter type when trying to set its binding." );

	CDatabaseStatementParameterSqlite::CDatabaseStatementParameterSqlite( DatabaseConnectionSqlitePtr connection, const String & name, unsigned short index, EFieldType fieldType, EParameterType parameterType, std::unique_ptr< SValueUpdater > updater )
		: CDatabaseParameter( connection, name, index, fieldType, parameterType, std::move( updater ) )
		, CDatabaseParameterSqlite( fieldType )
		, _statement( NULL )
	{
	}

	CDatabaseStatementParameterSqlite::CDatabaseStatementParameterSqlite( DatabaseConnectionSqlitePtr connection, const String & name, unsigned short index, EFieldType fieldType, uint32_t limits, EParameterType parameterType, std::unique_ptr< SValueUpdater > updater )
		: CDatabaseParameter( connection, name, index, fieldType, limits, parameterType, std::move( updater ) )
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

		case EFieldType_INT8:
			_binding = MakeSqliteBind< EFieldType_INT8 >( _statement, GetConnection(), GetIndex(), GetObjectValue() );
			break;

		case EFieldType_INT16:
			_binding = MakeSqliteBind< EFieldType_INT16 >( _statement, GetConnection(), GetIndex(), GetObjectValue() );
			break;

		case EFieldType_INT24:
			_binding = MakeSqliteBind< EFieldType_INT24 >( _statement, GetConnection(), GetIndex(), GetObjectValue() );
			break;

		case EFieldType_INT32:
			_binding = MakeSqliteBind< EFieldType_INT32 >( _statement, GetConnection(), GetIndex(), GetObjectValue() );
			break;

		case EFieldType_INT64:
			_binding = MakeSqliteBind< EFieldType_INT64 >( _statement, GetConnection(), GetIndex(), GetObjectValue() );
			break;

		case EFieldType_FLOAT32:
			_binding = MakeSqliteBind< EFieldType_FLOAT32 >( _statement, GetConnection(), GetIndex(), GetObjectValue() );
			break;

		case EFieldType_FLOAT64:
			_binding = MakeSqliteBind< EFieldType_FLOAT64 >( _statement, GetConnection(), GetIndex(), GetObjectValue() );
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
			CLogger::LogError( ERROR_SQLITE_PARAMETER_TYPE );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, ERROR_SQLITE_PARAMETER_TYPE );
			break;
		}
	}
}
END_NAMESPACE_DATABASE_SQLITE
