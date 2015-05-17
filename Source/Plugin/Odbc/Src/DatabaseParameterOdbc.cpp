/************************************************************************//**
 * @file DatabaseParameterOdbc.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 *
 *
 * @brief CDatabaseParameterOdbc class declaration.
 *
 * @details Describes the base class for a statement or query parameter for ODBC database.
 *
 ***************************************************************************/

#include "DatabaseOdbcPch.h"

#include "DatabaseParameterOdbc.h"

#include "DatabaseOdbcHelper.h"

BEGIN_NAMESPACE_DATABASE_ODBC
{
	SQLSMALLINT CDatabaseParameterOdbc::CTypes[EFieldType_COUNT] =
	{
		0,               ///< EFieldType_NULL
		SQL_C_BIT,       ///< EFieldType_BOOL
		SQL_C_SSHORT,    ///< EFieldType_SMALL_INTEGER
		SQL_C_SLONG,     ///< EFieldType_INTEGER
		SQL_C_SBIGINT,   ///< EFieldType_LONG_INTEGER
		SQL_C_FLOAT,     ///< EFieldType_FLOAT
		SQL_C_DOUBLE,    ///< EFieldType_DOUBLE
		SQL_C_CHAR,      ///< EFieldType_VARCHAR
		SQL_C_CHAR,      ///< EFieldType_TEXT
		SQL_C_WCHAR,     ///< EFieldType_NVARCHAR
		SQL_C_WCHAR,     ///< EFieldType_NTEXT
		SQL_C_DATE,      ///< EFieldType_DATE
		SQL_C_TIMESTAMP, ///< EFieldType_DATETIME
		SQL_C_TIME,      ///< EFieldType_TIME
		SQL_C_BINARY,    ///< EFieldType_BINARY
		SQL_C_BINARY,    ///< EFieldType_VARBINARY
		SQL_C_BINARY,    ///< EFieldType_LONG_VARBINARY
	};

	SQLSMALLINT CDatabaseParameterOdbc::Types[EFieldType_COUNT] =
	{
		0,                   ///< EFieldType_NULL
		SQL_BIT,             ///< EFieldType_BOOL
		SQL_SMALLINT,        ///< EFieldType_SMALL_INTEGER
		SQL_INTEGER,         ///< EFieldType_INTEGER
		SQL_BIGINT,          ///< EFieldType_LONG_INTEGER
		SQL_FLOAT,           ///< EFieldType_FLOAT
		SQL_DOUBLE,          ///< EFieldType_DOUBLE
		SQL_VARCHAR,         ///< EFieldType_VARCHAR
		SQL_LONGVARCHAR,     ///< EFieldType_TEXT
		SQL_VARCHAR,         ///< EFieldType_NVARCHAR
		SQL_LONGVARCHAR,     ///< EFieldType_NTEXT
		SQL_DATE,            ///< EFieldType_DATE
		SQL_DATETIME,        ///< EFieldType_DATETIME
		SQL_TIME,            ///< EFieldType_TIME
		SQL_BINARY,          ///< EFieldType_BINARY
		SQL_VARBINARY,       ///< EFieldType_VARBINARY
		SQL_LONGVARBINARY,   ///< EFieldType_LONG_VARBINARY
	};

	SQLULEN CDatabaseParameterOdbc::Sizes[EFieldType_COUNT] =
	{
		0,   ///< EFieldType_NULL
		1,   ///< EFieldType_BOOL
		5,   ///< EFieldType_SMALL_INTEGER
		10,  ///< EFieldType_INTEGER
		19,  ///< EFieldType_LONG_INTEGER
		15,  ///< EFieldType_FLOAT
		15,  ///< EFieldType_DOUBLE
		0,   ///< EFieldType_VARCHAR
		0,   ///< EFieldType_TEXT
		0,   ///< EFieldType_NVARCHAR
		0,   ///< EFieldType_NTEXT
		16,  ///< EFieldType_DATE
		25,  ///< EFieldType_DATETIME
		14,  ///< EFieldType_TIME
		0,   ///< EFieldType_BINARY
		0,   ///< EFieldType_VARBINARY
		0,   ///< EFieldType_LONG_VARBINARY
	};

	SQLSMALLINT CDatabaseParameterOdbc::InOutTypes[EParameterType_COUNT] =
	{
		SQL_PARAM_INPUT,
		SQL_PARAM_OUTPUT,
		SQL_PARAM_INPUT_OUTPUT
	};

	CDatabaseParameterOdbc::CDatabaseParameterOdbc( unsigned short index, EFieldType fieldType, EParameterType parameterType )
		: _inputOutputType( InOutTypes[parameterType] )
		, _valueType( CTypes[fieldType] )
		, _columnIndex( 0 )
		, _parameterType( Types[fieldType] )
		, _columnSize( 0 )
		, _decimalDigits( 0 )
		, _isNullable( true )
	{
		memset( &_date, 0, sizeof( _date ) );
		memset( &_time, 0, sizeof( _time ) );
		memset( &_dateTime, 0, sizeof( _dateTime ) );
	}

	CDatabaseParameterOdbc::~CDatabaseParameterOdbc()
	{
		// Empty
	}

	const SQLSMALLINT & CDatabaseParameterOdbc::GetValueType() const
	{
		return _valueType;
	}

	const SQLSMALLINT & CDatabaseParameterOdbc::GetParameterType() const
	{
		return _parameterType;
	}

	const SQLSMALLINT & CDatabaseParameterOdbc::GetInputOutputType() const
	{
		return _inputOutputType;
	}

	const SQLULEN & CDatabaseParameterOdbc::GetColumnSize() const
	{
		return _columnSize;
	}

	const SQLSMALLINT & CDatabaseParameterOdbc::GetDecimalDigits() const
	{
		return _decimalDigits;
	}

	SQLLEN & CDatabaseParameterOdbc::GetColumnIndex()
	{
		return _columnIndex;
	}

	void CDatabaseParameterOdbc::SetColumnIndex( SQLINTEGER value )
	{
		_columnIndex = value;
	}

	const SQL_DATE_STRUCT & CDatabaseParameterOdbc::GetDate()const
	{
		return _date;
	}

	SQL_DATE_STRUCT & CDatabaseParameterOdbc::GetDate()
	{
		return _date;
	}

	const SQL_TIME_STRUCT & CDatabaseParameterOdbc::GetTime()const
	{
		return _time;
	}

	SQL_TIME_STRUCT & CDatabaseParameterOdbc::GetTime()
	{
		return _time;
	}

	const SQL_TIMESTAMP_STRUCT & CDatabaseParameterOdbc::GetDateTime()const
	{
		return _dateTime;
	}

	SQL_TIMESTAMP_STRUCT & CDatabaseParameterOdbc::GetDateTime()
	{
		return _dateTime;
	}
}
END_NAMESPACE_DATABASE_ODBC
