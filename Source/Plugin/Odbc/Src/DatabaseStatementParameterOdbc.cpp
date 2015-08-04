/************************************************************************//**
 * @file DatabaseStatementParameterOdbc.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 *
 *
 * @brief CDatabaseStatementParameterOdbc class declaration.
 *
 * @details Describes a statement parameter for ODBC database.
 *
 ***************************************************************************/

#include "DatabaseOdbcPch.h"

#include "DatabaseStatementParameterOdbc.h"

#include "DatabaseConnectionOdbc.h"
#include "DatabaseOdbcHelper.h"

#include "DatabaseOdbcHelper.h"

BEGIN_NAMESPACE_DATABASE_ODBC
{
	static const String ERROR_ODBC_PARAMETER_TYPE = STR( "Undefined parameter type when trying to set its binding." );

	CDatabaseStatementParameterOdbc::CDatabaseStatementParameterOdbc( DatabaseConnectionOdbcSPtr connection, DatabaseValuedObjectInfosSPtr infos, unsigned short index, EParameterType parameterType, std::unique_ptr< SValueUpdater > updater )
		: CDatabaseParameter( connection, infos, index, parameterType, std::move( updater ) )
	{
		// Empty
	}

	CDatabaseStatementParameterOdbc::~CDatabaseStatementParameterOdbc()
	{
		// Empty
	}

	void CDatabaseStatementParameterOdbc::Initialise( SQLHSTMT statementHandle )
	{
		switch ( GetType() )
		{
		case EFieldType_BIT:
			_binding = MakeOutBind< EFieldType_BIT >( statementHandle, *this, GetObjectValue() );
			break;

		case EFieldType_SINT8:
			_binding = MakeOutBind< EFieldType_SINT8 >( statementHandle, *this, GetObjectValue() );
			break;

		case EFieldType_SINT16:
			_binding = MakeOutBind< EFieldType_SINT16 >( statementHandle, *this, GetObjectValue() );
			break;

		case EFieldType_SINT24:
			_binding = MakeOutBind< EFieldType_SINT24 >( statementHandle, *this, GetObjectValue() );
			break;

		case EFieldType_SINT32:
			_binding = MakeOutBind< EFieldType_SINT32 >( statementHandle, *this, GetObjectValue() );
			break;

		case EFieldType_SINT64:
			_binding = MakeOutBind< EFieldType_SINT64 >( statementHandle, *this, GetObjectValue() );
			break;

		case EFieldType_UINT8:
			_binding = MakeOutBind< EFieldType_UINT8 >( statementHandle, *this, GetObjectValue() );
			break;

		case EFieldType_UINT16:
			_binding = MakeOutBind< EFieldType_UINT16 >( statementHandle, *this, GetObjectValue() );
			break;

		case EFieldType_UINT24:
			_binding = MakeOutBind< EFieldType_UINT24 >( statementHandle, *this, GetObjectValue() );
			break;

		case EFieldType_UINT32:
			_binding = MakeOutBind< EFieldType_UINT32 >( statementHandle, *this, GetObjectValue() );
			break;

		case EFieldType_UINT64:
			_binding = MakeOutBind< EFieldType_UINT64 >( statementHandle, *this, GetObjectValue() );
			break;

		case EFieldType_FLOAT32:
			_binding = MakeOutBind< EFieldType_FLOAT32 >( statementHandle, *this, GetObjectValue() );
			break;

		case EFieldType_FLOAT64:
			_binding = MakeOutBind< EFieldType_FLOAT64 >( statementHandle, *this, GetObjectValue() );
			break;

		case EFieldType_FIXED_POINT:
			_binding = MakeOutBind< EFieldType_FIXED_POINT >( statementHandle, *this, GetObjectValue() );
			break;

		case EFieldType_CHAR:
			_binding = MakeOutBind< EFieldType_CHAR >( statementHandle, *this, GetObjectValue() );
			break;

		case EFieldType_VARCHAR:
			_binding = MakeOutBind< EFieldType_VARCHAR >( statementHandle, *this, GetObjectValue() );
			break;

		case EFieldType_TEXT:
			_binding = MakeOutBind< EFieldType_TEXT >( statementHandle, *this, GetObjectValue() );
			break;

		case EFieldType_NCHAR:
			_binding = MakeOutBind< EFieldType_NCHAR >( statementHandle, *this, GetObjectValue() );
			break;

		case EFieldType_NVARCHAR:
			_binding = MakeOutBind< EFieldType_NVARCHAR >( statementHandle, *this, GetObjectValue() );
			break;

		case EFieldType_NTEXT:
			_binding = MakeOutBind< EFieldType_NTEXT >( statementHandle, *this, GetObjectValue() );
			break;

		case EFieldType_DATE:
			_binding = MakeOutBind< EFieldType_DATE >( statementHandle, *this, GetObjectValue() );
			break;

		case EFieldType_DATETIME:
			_binding = MakeOutBind< EFieldType_DATETIME >( statementHandle, *this, GetObjectValue() );
			break;

		case EFieldType_TIME:
			_binding = MakeOutBind< EFieldType_TIME >( statementHandle, *this, GetObjectValue() );
			break;

		case EFieldType_BINARY:
			_binding = MakeOutBind< EFieldType_BINARY >( statementHandle, *this, GetObjectValue() );
			break;

		case EFieldType_VARBINARY:
			_binding = MakeOutBind< EFieldType_VARBINARY >( statementHandle, *this, GetObjectValue() );
			break;

		case EFieldType_BLOB:
			_binding = MakeOutBind< EFieldType_BLOB >( statementHandle, *this, GetObjectValue() );
			break;

		default:
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, ERROR_ODBC_PARAMETER_TYPE );
			break;
		}

		_binding->Initialise();
	}
}
END_NAMESPACE_DATABASE_ODBC
