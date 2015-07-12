/************************************************************************//**
 * @file DatabaseParameterOdbc.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 *
 * @brief COutOdbcBindBase and COutOdbcBind structures.
 *
 * @details Describes the base class for a statement or query parameter binding for ODBC.
 *
 ***************************************************************************/

#include "DatabaseOdbcPch.h"

#include "DatabaseParameterOdbc.h"

#include "DatabaseOdbcHelper.h"

BEGIN_NAMESPACE_DATABASE_ODBC
{
	namespace
	{
		SQLSMALLINT CTypes[EFieldType_COUNT] =
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

		SQLSMALLINT Types[EFieldType_COUNT] =
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
			SQL_TIMESTAMP,       ///< EFieldType_DATETIME
			SQL_TIME,            ///< EFieldType_TIME
			SQL_BINARY,          ///< EFieldType_BINARY
			SQL_VARBINARY,       ///< EFieldType_VARBINARY
			SQL_LONGVARBINARY,   ///< EFieldType_LONG_VARBINARY
		};

		SQLULEN Sizes[EFieldType_COUNT] =
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

		SQLSMALLINT InOutTypes[EParameterType_COUNT] =
		{
			SQL_PARAM_INPUT,
			SQL_PARAM_OUTPUT,
			SQL_PARAM_INPUT_OUTPUT
		};
	}

	static const String ODBC_DescribeParam_MSG = STR( "SQLDescribeParam" );
	static const String ODBC_PutData_MSG = STR( "SQLPutData" );
	static const String ODBC_GetData_MSG = STR( "SQLGetData" );

	static const String DATABASE_PARAMETER_TYPE_ERROR = STR( "Wrong parameter type when trying to set its binding." );

	COutOdbcBindBase::COutOdbcBindBase( HSTMT statement, uint16_t index, EFieldType fieldType, EParameterType parameterType, const String & name, CDatabaseValueBase & value )
		: _inputOutputType( InOutTypes[parameterType] )
		, _valueType( CTypes[fieldType] )
		, _columnLenOrInd( 0 )
		, _parameterType( Types[fieldType] )
		, _columnSize( 0 )
		, _decimalDigits( 0 )
		, _nullable( 0 )
		, _index( index )
		, _dataType( 0 )
		, _statement( statement )
		, _name( name )
		, _value( value )
	{
	}

	COutOdbcBindBase::~COutOdbcBindBase()
	{
		// Empty
	}

	EErrorType COutOdbcBindBase::Initialize()
	{
		EErrorType errorType = EErrorType_NONE;
		SqlTry( SQLDescribeParam( _statement, _index, &_dataType, &_columnSize, &_decimalDigits, &_nullable ), SQL_HANDLE_STMT, _statement, ODBC_DescribeParam_MSG );
		return errorType;
	}

	EErrorType COutOdbcBindBase::PutData()const
	{
		uint8_t * buffer = static_cast< uint8_t * >( _value.GetPtrValue() );
		long remaining = _value.GetPtrSize();
		long batch = 5000;
		EErrorType errorType = EErrorType_NONE;

		while ( remaining > batch && errorType == EErrorType_NONE )
		{
			SqlTry( SQLPutData( _statement, buffer, batch ), SQL_HANDLE_STMT, _statement, ODBC_PutData_MSG );
			buffer += batch;
			remaining -= batch;
		}

		if ( errorType == EErrorType_NONE && remaining > 0 )
		{
			SqlTry( SQLPutData( _statement, buffer, remaining ), SQL_HANDLE_STMT, _statement, ODBC_PutData_MSG );
		}

		return errorType;
	}

	EErrorType COutOdbcBindBase::GetData()
	{
		uint8_t * buffer = static_cast< uint8_t * >( _value.GetPtrValue() );
		long remaining = _value.GetPtrSize();
		long batch = 5000;
		SQLLEN retrieved;
		EErrorType errorType = EErrorType_NONE;

		while ( remaining > batch && errorType == EErrorType_NONE )
		{
			SqlTry( SQLGetData( _statement, _index, _valueType, buffer, batch, &retrieved ), SQL_HANDLE_STMT, _statement, ODBC_GetData_MSG );
			buffer += batch;
			remaining -= batch;
		}

		if ( errorType == EErrorType_NONE && remaining > 0 )
		{
			SqlTry( SQLGetData( _statement, _index, _valueType, buffer, remaining, &retrieved ), SQL_HANDLE_STMT, _statement, ODBC_GetData_MSG );
		}

		return errorType;
	}

	CDatabaseParameterOdbc::CDatabaseParameterOdbc()
	{
		// Empty
	}

	CDatabaseParameterOdbc::~CDatabaseParameterOdbc()
	{
		// Empty
	}

	void CDatabaseParameterOdbc::Initialize( SQLHSTMT statementHandle, CDatabaseParameter & parameter )
	{
		switch ( parameter.GetType() )
		{
		case EFieldType_BIT:
			_binding = MakeOutBind< EFieldType_BIT >( statementHandle, parameter, parameter.GetObjectValue() );
			break;

		case EFieldType_TINY_INTEGER:
			_binding = MakeOutBind< EFieldType_TINY_INTEGER >( statementHandle, parameter, parameter.GetObjectValue() );
			break;

		case EFieldType_SMALL_INTEGER:
			_binding = MakeOutBind< EFieldType_SMALL_INTEGER >( statementHandle, parameter, parameter.GetObjectValue() );
			break;

		case EFieldType_INTEGER:
			_binding = MakeOutBind< EFieldType_INTEGER >( statementHandle, parameter, parameter.GetObjectValue() );
			break;

		case EFieldType_LONG_INTEGER:
			_binding = MakeOutBind< EFieldType_LONG_INTEGER >( statementHandle, parameter, parameter.GetObjectValue() );
			break;

		case EFieldType_FLOATING_POINT_SIMPLE:
			_binding = MakeOutBind< EFieldType_FLOATING_POINT_SIMPLE >( statementHandle, parameter, parameter.GetObjectValue() );
			break;

		case EFieldType_FLOATING_POINT_DOUBLE:
			_binding = MakeOutBind< EFieldType_FLOATING_POINT_DOUBLE >( statementHandle, parameter, parameter.GetObjectValue() );
			break;

		case EFieldType_FIXED_POINT:
			_binding = MakeOutBind< EFieldType_FIXED_POINT >( statementHandle, parameter, parameter.GetObjectValue() );
			break;

		case EFieldType_VARCHAR:
			_binding = MakeOutBind< EFieldType_VARCHAR >( statementHandle, parameter, parameter.GetObjectValue() );
			break;

		case EFieldType_TEXT:
			_binding = MakeOutBind< EFieldType_TEXT >( statementHandle, parameter, parameter.GetObjectValue() );
			break;

		case EFieldType_NVARCHAR:
			_binding = MakeOutBind< EFieldType_NVARCHAR >( statementHandle, parameter, parameter.GetObjectValue() );
			break;

		case EFieldType_NTEXT:
			_binding = MakeOutBind< EFieldType_NTEXT >( statementHandle, parameter, parameter.GetObjectValue() );
			break;

		case EFieldType_DATE:
			_binding = MakeOutBind< EFieldType_DATE >( statementHandle, parameter, parameter.GetObjectValue() );
			break;

		case EFieldType_DATETIME:
			_binding = MakeOutBind< EFieldType_DATETIME >( statementHandle, parameter, parameter.GetObjectValue() );
			break;

		case EFieldType_TIME:
			_binding = MakeOutBind< EFieldType_TIME >( statementHandle, parameter, parameter.GetObjectValue() );
			break;

		case EFieldType_BINARY:
			_binding = MakeOutBind< EFieldType_BINARY >( statementHandle, parameter, parameter.GetObjectValue() );
			break;

		case EFieldType_VARBINARY:
			_binding = MakeOutBind< EFieldType_VARBINARY >( statementHandle, parameter, parameter.GetObjectValue() );
			break;

		case EFieldType_LONG_VARBINARY:
			_binding = MakeOutBind< EFieldType_LONG_VARBINARY >( statementHandle, parameter, parameter.GetObjectValue() );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}
	}
}
END_NAMESPACE_DATABASE_ODBC
