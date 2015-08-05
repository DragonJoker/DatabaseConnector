/************************************************************************//**
* @file DatabaseStatementParameterMySql.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDatabaseParameterMySql class declaration.
*
* @details Describes a statement parameter for MYSQL database.
*
***************************************************************************/

#include "DatabaseMySqlPch.h"

#include "DatabaseParameterMySql.h"

#include "DatabaseConnectionMySql.h"
#include "DatabaseMySqlParameterBinding.h"

#include <DatabaseException.h>
#include <DatabaseLogger.h>
#include <DatabaseStringUtils.h>

BEGIN_NAMESPACE_DATABASE_MYSQL
{
	static const String ERROR_MYSQL_PARAMETER_TYPE = STR( "Undefined parameter type when trying to set its binding." );

	CDatabaseParameterMySql::CDatabaseParameterMySql( DatabaseConnectionMySqlSPtr connection, DatabaseValuedObjectInfosSPtr infos, unsigned short index, EParameterType parameterType, std::unique_ptr< SValueUpdater > updater )
		: CDatabaseParameter( connection, infos, index, parameterType, std::move( updater ) )
		, _statement( NULL )
	{
	}

	CDatabaseParameterMySql::~CDatabaseParameterMySql()
	{
		_statement = NULL;
	}

	void CDatabaseParameterMySql::SetNull()
	{
		if ( _binding )
		{
			_binding->_null = true;
		}

		CDatabaseParameter::SetNull();
	}

	void CDatabaseParameterMySql::SetBinding( MYSQL_BIND * bind )
	{
		bind->length = const_cast< unsigned long * >( &GetObjectValue().GetPtrSize() );
		bind->param_number = GetIndex() - 1;

		switch ( GetType() )
		{
		case EFieldType_BIT:
			_binding = MakeOutBind< EFieldType_BIT >( *bind, GetObjectValue(), *this );
			break;

		case EFieldType_SINT8:
			_binding = MakeOutBind< EFieldType_SINT8 >( *bind, GetObjectValue(), *this );
			break;

		case EFieldType_SINT16:
			_binding = MakeOutBind< EFieldType_SINT16 >( *bind, GetObjectValue(), *this );
			break;

		case EFieldType_SINT24:
			_binding = MakeOutBind< EFieldType_SINT24 >( *bind, GetObjectValue(), *this );
			break;

		case EFieldType_SINT32:
			_binding = MakeOutBind< EFieldType_SINT32 >( *bind, GetObjectValue(), *this );
			break;

		case EFieldType_SINT64:
			_binding = MakeOutBind< EFieldType_SINT64 >( *bind, GetObjectValue(), *this );
			break;

		case EFieldType_UINT8:
			_binding = MakeOutBind< EFieldType_UINT8 >( *bind, GetObjectValue(), *this );
			break;

		case EFieldType_UINT16:
			_binding = MakeOutBind< EFieldType_UINT16 >( *bind, GetObjectValue(), *this );
			break;

		case EFieldType_UINT24:
			_binding = MakeOutBind< EFieldType_UINT24 >( *bind, GetObjectValue(), *this );
			break;

		case EFieldType_UINT32:
			_binding = MakeOutBind< EFieldType_UINT32 >( *bind, GetObjectValue(), *this );
			break;

		case EFieldType_UINT64:
			_binding = MakeOutBind< EFieldType_UINT64 >( *bind, GetObjectValue(), *this );
			break;

		case EFieldType_FLOAT32:
			_binding = MakeOutBind< EFieldType_FLOAT32 >( *bind, GetObjectValue(), *this );
			break;

		case EFieldType_FLOAT64:
			_binding = MakeOutBind< EFieldType_FLOAT64 >( *bind, GetObjectValue(), *this );
			break;

		case EFieldType_FIXED_POINT:
			_binding = MakeOutBind< EFieldType_FIXED_POINT >( *bind, GetObjectValue(), *this );
			break;

		case EFieldType_CHAR:
			_binding = MakeOutBind< EFieldType_CHAR >( *bind, GetObjectValue(), *this );
			break;

		case EFieldType_VARCHAR:
			_binding = MakeOutBind< EFieldType_VARCHAR >( *bind, GetObjectValue(), *this );
			break;

		case EFieldType_TEXT:
			_binding = MakeOutBind< EFieldType_TEXT >( *bind, GetObjectValue(), *this );
			break;

		case EFieldType_NCHAR:
			_binding = MakeOutBind< EFieldType_NCHAR >( *bind, GetObjectValue(), *this );
			break;

		case EFieldType_NVARCHAR:
			_binding = MakeOutBind< EFieldType_NVARCHAR >( *bind, GetObjectValue(), *this );
			break;

		case EFieldType_NTEXT:
			_binding = MakeOutBind< EFieldType_NTEXT >( *bind, GetObjectValue(), *this );
			break;

		case EFieldType_DATE:
			_binding = MakeOutBind< EFieldType_DATE >( *bind, GetObjectValue(), *this );
			break;

		case EFieldType_DATETIME:
			_binding = MakeOutBind< EFieldType_DATETIME >( *bind, GetObjectValue(), *this );
			break;

		case EFieldType_TIME:
			_binding = MakeOutBind< EFieldType_TIME >( *bind, GetObjectValue(), *this );
			break;

		case EFieldType_BINARY:
			_binding = MakeOutBind< EFieldType_BINARY >( *bind, GetObjectValue(), *this );
			break;

		case EFieldType_VARBINARY:
			_binding = MakeOutBind< EFieldType_VARBINARY >( *bind, GetObjectValue(), *this );
			break;

		case EFieldType_BLOB:
			_binding = MakeOutBind< EFieldType_BLOB >( *bind, GetObjectValue(), *this );
			break;

		default:
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, ERROR_MYSQL_PARAMETER_TYPE );
			break;
		}
	}
}
END_NAMESPACE_DATABASE_MYSQL
