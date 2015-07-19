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
			0,					//!< EFieldType_NULL
			SQL_C_BIT,			//!< EFieldType_BIT
			SQL_C_STINYINT,		//!< EFieldType_SINT8
			SQL_C_SSHORT,		//!< EFieldType_SINT16
			SQL_C_SLONG,		//!< EFieldType_SINT24
			SQL_C_SLONG,		//!< EFieldType_SINT32
			SQL_C_SBIGINT,		//!< EFieldType_SINT64
			SQL_C_UTINYINT,		//!< EFieldType_UINT8
			SQL_C_USHORT,		//!< EFieldType_UINT16
			SQL_C_ULONG,		//!< EFieldType_UINT24
			SQL_C_ULONG,		//!< EFieldType_UINT32
			SQL_C_UBIGINT,		//!< EFieldType_UINT64
			SQL_C_FLOAT,		//!< EFieldType_FLOAT32
			SQL_C_DOUBLE,		//!< EFieldType_FLOAT64
			SQL_C_NUMERIC,		//!< EFieldType_FIXED_POINT
			SQL_C_CHAR,			//!< EFieldType_VARCHAR
			SQL_C_CHAR,			//!< EFieldType_TEXT
			SQL_C_WCHAR,		//!< EFieldType_NVARCHAR
			SQL_C_WCHAR,		//!< EFieldType_NTEXT
			SQL_C_DATE,			//!< EFieldType_DATE
			SQL_C_TIMESTAMP,	//!< EFieldType_DATETIME
			SQL_C_TIME,			//!< EFieldType_TIME
			SQL_C_BINARY,		//!< EFieldType_BINARY
			SQL_C_BINARY,		//!< EFieldType_VARBINARY
			SQL_C_BINARY,		//!< EFieldType_LONG_VARBINARY
		};

		SQLSMALLINT Types[EFieldType_COUNT] =
		{
			0,					//!< EFieldType_NULL
			SQL_BIT,			//!< EFieldType_BOOL
			SQL_TINYINT,		//!< EFieldType_SINT8
			SQL_SMALLINT,		//!< EFieldType_SINT16
			SQL_INTEGER,		//!< EFieldType_SINT24
			SQL_INTEGER,		//!< EFieldType_SINT32
			SQL_BIGINT,			//!< EFieldType_SINT64
			SQL_TINYINT,		//!< EFieldType_UINT8
			SQL_SMALLINT,		//!< EFieldType_UINT16
			SQL_INTEGER,		//!< EFieldType_UINT24
			SQL_INTEGER,		//!< EFieldType_UINT32
			SQL_BIGINT,			//!< EFieldType_UINT64
			SQL_FLOAT,			//!< EFieldType_FLOAT32
			SQL_DOUBLE,			//!< EFieldType_FLOAT64
			SQL_NUMERIC,		//!< EFieldType_FIXED_POINT
			SQL_VARCHAR,		//!< EFieldType_VARCHAR
			SQL_LONGVARCHAR,	//!< EFieldType_TEXT
			SQL_VARCHAR,		//!< EFieldType_NVARCHAR
			SQL_LONGVARCHAR,	//!< EFieldType_NTEXT
			SQL_DATE,			//!< EFieldType_DATE
			SQL_TIMESTAMP,		//!< EFieldType_DATETIME
			SQL_TIME,			//!< EFieldType_TIME
			SQL_BINARY,			//!< EFieldType_BINARY
			SQL_VARBINARY,		//!< EFieldType_VARBINARY
			SQL_LONGVARBINARY,	//!< EFieldType_LONG_VARBINARY
		};

		SQLULEN Sizes[EFieldType_COUNT] =
		{
			0,	//!< EFieldType_NULL
			1,	//!< EFieldType_BOOL
			1,	//!< EFieldType_SINT8
			5,	//!< EFieldType_SINT16
			8,	//!< EFieldType_SINT24
			10,	//!< EFieldType_SINT32
			19,	//!< EFieldType_SINT64
			1,	//!< EFieldType_UINT8
			5,	//!< EFieldType_UINT16
			8,	//!< EFieldType_UINT24
			10,	//!< EFieldType_UINT32
			19,	//!< EFieldType_UINT64
			15,	//!< EFieldType_FLOAT32
			15,	//!< EFieldType_FLOAT64
			15,	//!< EFieldType_FIXED_POINT
			0,	//!< EFieldType_VARCHAR
			0,	//!< EFieldType_TEXT
			0,	//!< EFieldType_NVARCHAR
			0,	//!< EFieldType_NTEXT
			16,	//!< EFieldType_DATE
			25,	//!< EFieldType_DATETIME
			14,	//!< EFieldType_TIME
			0,	//!< EFieldType_BINARY
			0,	//!< EFieldType_VARBINARY
			0,	//!< EFieldType_LONG_VARBINARY
		};

		SQLSMALLINT InOutTypes[EParameterType_COUNT] =
		{
			SQL_PARAM_INPUT,
			SQL_PARAM_OUTPUT,
			SQL_PARAM_INPUT_OUTPUT
		};
	}

	static const String INFO_ODBC_DescribeParam = STR( "SQLDescribeParam" );
	static const String INFO_ODBC_GetData = STR( "SQLGetData" );
	static const String INFO_ODBC_PutData = STR( "SQLPutData: " );

	static const String ERROR_ODBC_PARAMETER_TYPE = STR( "Undefined parameter type when trying to set its binding." );

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
		OdbcCheck( SQLDescribeParam( _statement, _index, &_dataType, &_columnSize, &_decimalDigits, &_nullable ), SQL_HANDLE_STMT, _statement, INFO_ODBC_DescribeParam );
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
			OdbcCheck( SQLPutData( _statement, buffer, batch ), SQL_HANDLE_STMT, _statement, INFO_ODBC_PutData );
			buffer += batch;
			remaining -= batch;
		}

		if ( errorType == EErrorType_NONE && remaining > 0 )
		{
			OdbcCheck( SQLPutData( _statement, buffer, remaining ), SQL_HANDLE_STMT, _statement, INFO_ODBC_PutData );
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
			OdbcCheck( SQLGetData( _statement, _index, _valueType, buffer, batch, &retrieved ), SQL_HANDLE_STMT, _statement, INFO_ODBC_GetData );
			buffer += batch;
			remaining -= batch;
		}

		if ( errorType == EErrorType_NONE && remaining > 0 )
		{
			OdbcCheck( SQLGetData( _statement, _index, _valueType, buffer, remaining, &retrieved ), SQL_HANDLE_STMT, _statement, INFO_ODBC_GetData );
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

		case EFieldType_SINT8:
			_binding = MakeOutBind< EFieldType_SINT8 >( statementHandle, parameter, parameter.GetObjectValue() );
			break;

		case EFieldType_SINT16:
			_binding = MakeOutBind< EFieldType_SINT16 >( statementHandle, parameter, parameter.GetObjectValue() );
			break;

		case EFieldType_SINT24:
			_binding = MakeOutBind< EFieldType_SINT24 >( statementHandle, parameter, parameter.GetObjectValue() );
			break;

		case EFieldType_SINT32:
			_binding = MakeOutBind< EFieldType_SINT32 >( statementHandle, parameter, parameter.GetObjectValue() );
			break;

		case EFieldType_SINT64:
			_binding = MakeOutBind< EFieldType_SINT64 >( statementHandle, parameter, parameter.GetObjectValue() );
			break;

		case EFieldType_UINT8:
			_binding = MakeOutBind< EFieldType_UINT8 >( statementHandle, parameter, parameter.GetObjectValue() );
			break;

		case EFieldType_UINT16:
			_binding = MakeOutBind< EFieldType_UINT16 >( statementHandle, parameter, parameter.GetObjectValue() );
			break;

		case EFieldType_UINT24:
			_binding = MakeOutBind< EFieldType_UINT24 >( statementHandle, parameter, parameter.GetObjectValue() );
			break;

		case EFieldType_UINT32:
			_binding = MakeOutBind< EFieldType_UINT32 >( statementHandle, parameter, parameter.GetObjectValue() );
			break;

		case EFieldType_UINT64:
			_binding = MakeOutBind< EFieldType_UINT64 >( statementHandle, parameter, parameter.GetObjectValue() );
			break;

		case EFieldType_FLOAT32:
			_binding = MakeOutBind< EFieldType_FLOAT32 >( statementHandle, parameter, parameter.GetObjectValue() );
			break;

		case EFieldType_FLOAT64:
			_binding = MakeOutBind< EFieldType_FLOAT64 >( statementHandle, parameter, parameter.GetObjectValue() );
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
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, ERROR_ODBC_PARAMETER_TYPE );
			break;
		}
	}
}
END_NAMESPACE_DATABASE_ODBC
