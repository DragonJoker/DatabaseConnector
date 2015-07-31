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
#include "DatabaseSqliteParameterBinding.h"

#include <DatabaseException.h>
#include <DatabaseLogger.h>
#include <DatabaseStringUtils.h>

BEGIN_NAMESPACE_DATABASE_SQLITE
{
	static const String ERROR_SQLITE_PARAMETER_TYPE = STR( "Undefined parameter type when trying to set its binding." );

	CDatabaseStatementParameterSqlite::CDatabaseStatementParameterSqlite( DatabaseConnectionSqliteSPtr connection, const String & name, unsigned short index, EFieldType fieldType, EParameterType parameterType, std::unique_ptr< SValueUpdater > updater )
		: CDatabaseParameter( connection, name, index, fieldType, parameterType, std::move( updater ) )
		, CDatabaseParameterSqlite( fieldType )
		, _statement( NULL )
	{
	}

	CDatabaseStatementParameterSqlite::CDatabaseStatementParameterSqlite( DatabaseConnectionSqliteSPtr connection, const String & name, unsigned short index, EFieldType fieldType, uint32_t limits, EParameterType parameterType, std::unique_ptr< SValueUpdater > updater )
		: CDatabaseParameter( connection, name, index, fieldType, limits, parameterType, std::move( updater ) )
		, CDatabaseParameterSqlite( fieldType )
		, _statement( NULL )
	{
	}

	CDatabaseStatementParameterSqlite::CDatabaseStatementParameterSqlite( DatabaseConnectionSqliteSPtr connection, const String & name, unsigned short index, EFieldType fieldType, const std::pair< uint32_t, uint32_t > & precision, EParameterType parameterType, std::unique_ptr< SValueUpdater > updater )
		: CDatabaseParameter( connection, name, index, fieldType, precision, parameterType, std::move( updater ) )
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
		CDatabaseParameter::SetNull();
		_binding->UpdateValue();
	}

	void CDatabaseStatementParameterSqlite::SetStatement( sqlite3_stmt * statement )
	{
		_statement = statement;

		switch ( GetType() )
		{
		case EFieldType_BIT:
			_binding = MakeSqliteBind< EFieldType_BIT >( _statement, GetConnection(), GetIndex(), GetObjectValue() );
			break;

		case EFieldType_SINT8:
			_binding = MakeSqliteBind< EFieldType_SINT8 >( _statement, GetConnection(), GetIndex(), GetObjectValue() );
			break;

		case EFieldType_SINT16:
			_binding = MakeSqliteBind< EFieldType_SINT16 >( _statement, GetConnection(), GetIndex(), GetObjectValue() );
			break;

		case EFieldType_SINT24:
			_binding = MakeSqliteBind< EFieldType_SINT24 >( _statement, GetConnection(), GetIndex(), GetObjectValue() );
			break;

		case EFieldType_SINT32:
			_binding = MakeSqliteBind< EFieldType_SINT32 >( _statement, GetConnection(), GetIndex(), GetObjectValue() );
			break;

		case EFieldType_SINT64:
			_binding = MakeSqliteBind< EFieldType_SINT64 >( _statement, GetConnection(), GetIndex(), GetObjectValue() );
			break;

		case EFieldType_UINT8:
			_binding = MakeSqliteBind< EFieldType_UINT8 >( _statement, GetConnection(), GetIndex(), GetObjectValue() );
			break;

		case EFieldType_UINT16:
			_binding = MakeSqliteBind< EFieldType_UINT16 >( _statement, GetConnection(), GetIndex(), GetObjectValue() );
			break;

		case EFieldType_UINT24:
			_binding = MakeSqliteBind< EFieldType_UINT24 >( _statement, GetConnection(), GetIndex(), GetObjectValue() );
			break;

		case EFieldType_UINT32:
			_binding = MakeSqliteBind< EFieldType_UINT32 >( _statement, GetConnection(), GetIndex(), GetObjectValue() );
			break;

		case EFieldType_UINT64:
			_binding = MakeSqliteBind< EFieldType_UINT64 >( _statement, GetConnection(), GetIndex(), GetObjectValue() );
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

		case EFieldType_CHAR:
			_binding = MakeSqliteBind< EFieldType_CHAR >( _statement, GetConnection(), GetIndex(), GetObjectValue() );
			break;

		case EFieldType_VARCHAR:
			_binding = MakeSqliteBind< EFieldType_VARCHAR >( _statement, GetConnection(), GetIndex(), GetObjectValue() );
			break;

		case EFieldType_TEXT:
			_binding = MakeSqliteBind< EFieldType_TEXT >( _statement, GetConnection(), GetIndex(), GetObjectValue() );
			break;

		case EFieldType_NCHAR:
			_binding = MakeSqliteBind< EFieldType_NCHAR >( _statement, GetConnection(), GetIndex(), GetObjectValue() );
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

		case EFieldType_BLOB:
			_binding = MakeSqliteBind< EFieldType_BLOB >( _statement, GetConnection(), GetIndex(), GetObjectValue() );
			break;

		default:
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, ERROR_SQLITE_PARAMETER_TYPE );
			break;
		}
	}
}
END_NAMESPACE_DATABASE_SQLITE
