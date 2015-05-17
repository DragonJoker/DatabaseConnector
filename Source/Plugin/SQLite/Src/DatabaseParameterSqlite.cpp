/************************************************************************//**
 * @file DatabaseParameterSqlite.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 *
 *
 * @brief CDatabaseParameterSqlite class declaration.
 *
 * @details Describes the base class for a statement or query parameter for SQLITE database.
 *
 ***************************************************************************/

#include "DatabaseSqlitePch.h"

#include "DatabaseParameterSqlite.h"

BEGIN_NAMESPACE_DATABASE_SQLITE
{
	int CDatabaseParameterSqlite::SqliteDataTypes[EFieldType_COUNT] =
	{
		SQLITE_NULL,     ///< EFieldType_NULL
		SQLITE_INTEGER,  ///< EFieldType_BOOL
		SQLITE_INTEGER,  ///< EFieldType_SMALL_INTEGER
		SQLITE_INTEGER,  ///< EFieldType_INTEGER
		SQLITE_INTEGER,  ///< EFieldType_LONG_INTEGER
		SQLITE_FLOAT,    ///< EFieldType_FLOAT
		SQLITE_FLOAT,    ///< EFieldType_DOUBLE
		SQLITE_FLOAT,    ///< EFieldType_VARCHAR
		SQLITE3_TEXT,    ///< EFieldType_TEXT
		SQLITE3_TEXT,    ///< EFieldType_NVARCHAR
		SQLITE3_TEXT,    ///< EFieldType_NTEXT
		SQLITE_INTEGER,  ///< EFieldType_DATE
		SQLITE_INTEGER,  ///< EFieldType_DATETIME
		SQLITE_INTEGER,  ///< EFieldType_TIME
		SQLITE_BLOB,     ///< EFieldType_BINARY
		SQLITE_BLOB,     ///< EFieldType_VARBINARY
		SQLITE_BLOB,     ///< EFieldType_LONG_VARBINARY
	};

	CDatabaseParameterSqlite::CDatabaseParameterSqlite( EFieldType fieldType )
		: _dataType( SqliteDataTypes[fieldType] )
	{
	}

	CDatabaseParameterSqlite::~CDatabaseParameterSqlite()
	{
		// Empty
	}

	const int & CDatabaseParameterSqlite::GetDataType() const
	{
		return _dataType;
	}
}
END_NAMESPACE_DATABASE_SQLITE
