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
#include <DatabaseValuedObjectInfos.h>

BEGIN_NAMESPACE_DATABASE_SQLITE
{
	int CDatabaseStatementParameterSqlite::SqliteDataTypes[EFieldType_COUNT] =
	{
		SQLITE_NULL,     //!< EFieldType_NULL
		SQLITE_INTEGER,  //!< EFieldType_BIT
		SQLITE_INTEGER,  //!< EFieldType_SINT8
		SQLITE_INTEGER,  //!< EFieldType_SINT16
		SQLITE_INTEGER,  //!< EFieldType_SINT24
		SQLITE_INTEGER,  //!< EFieldType_SINT32
		SQLITE_INTEGER,  //!< EFieldType_SINT64
		SQLITE_INTEGER,  //!< EFieldType_UINT8
		SQLITE_INTEGER,  //!< EFieldType_UINT16
		SQLITE_INTEGER,  //!< EFieldType_UINT24
		SQLITE_INTEGER,  //!< EFieldType_UINT32
		SQLITE_INTEGER,  //!< EFieldType_UINT64
		SQLITE_FLOAT,    //!< EFieldType_FLOAT32
		SQLITE_FLOAT,    //!< EFieldType_FLOAT64
		SQLITE_FLOAT,    //!< EFieldType_FIXED_POINT
		SQLITE3_TEXT,    //!< EFieldType_CHAR
		SQLITE3_TEXT,    //!< EFieldType_VARCHAR
		SQLITE3_TEXT,    //!< EFieldType_TEXT
		SQLITE3_TEXT,    //!< EFieldType_NCHAR
		SQLITE3_TEXT,    //!< EFieldType_NVARCHAR
		SQLITE3_TEXT,    //!< EFieldType_NTEXT
		SQLITE_INTEGER,  //!< EFieldType_DATE
		SQLITE_INTEGER,  //!< EFieldType_DATETIME
		SQLITE_INTEGER,  //!< EFieldType_TIME
		SQLITE_BLOB,     //!< EFieldType_BINARY
		SQLITE_BLOB,     //!< EFieldType_VARBINARY
		SQLITE_BLOB,     //!< EFieldType_BLOB
	};

	static const String ERROR_SQLITE_PARAMETER_TYPE = STR( "Undefined parameter type when trying to set its binding." );

	CDatabaseStatementParameterSqlite::CDatabaseStatementParameterSqlite( DatabaseConnectionSqliteSPtr connection, DatabaseValuedObjectInfosSPtr infos, unsigned short index, EParameterType parameterType, std::unique_ptr< SValueUpdater > updater )
		: CDatabaseParameter( connection, infos, index, parameterType, std::move( updater ) )
		, _dataType( SqliteDataTypes[infos->GetType()] )
		, _statement( NULL )
	{
	}

	CDatabaseStatementParameterSqlite::~CDatabaseStatementParameterSqlite()
	{
		_statement = NULL;
	}

	const int & CDatabaseStatementParameterSqlite::GetDataType() const
	{
		return _dataType;
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
