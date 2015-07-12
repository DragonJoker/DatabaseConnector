/************************************************************************//**
 * @file DatabaseStatementParameterMySql.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 *
 *
 * @brief CDatabaseStatementParameterMySql class declaration.
 *
 * @details Describes a statement parameter for MYSQL database.
 *
 ***************************************************************************/

#include "DatabaseMySqlPch.h"

#include "DatabaseStatementParameterMySql.h"

#include "DatabaseConnectionMySql.h"
#include "DatabaseStatementParameterSetter.h"

#include <DatabaseException.h>
#include <DatabaseLogger.h>
#include <DatabaseStringUtils.h>

BEGIN_NAMESPACE_DATABASE_MYSQL
{
	static const String DATABASE_PARAMETER_TYPE_ERROR = STR( "Wrong parameter type when trying to set its binding." );

	CDatabaseStatementParameterMySql::CDatabaseStatementParameterMySql( DatabaseConnectionMySqlPtr connection, const String & name, unsigned short index, EFieldType fieldType, EParameterType parameterType, SValueUpdater * updater )
		: CDatabaseParameter( connection, name, index, fieldType, parameterType, updater )
		, _statement( NULL )
	{
	}

	CDatabaseStatementParameterMySql::CDatabaseStatementParameterMySql( DatabaseConnectionMySqlPtr connection, const String & name, unsigned short index, EFieldType fieldType, uint32_t limits, EParameterType parameterType, SValueUpdater * updater )
		: CDatabaseParameter( connection, name, index, fieldType, limits, parameterType, updater )
		, _statement( NULL )
	{
	}

	CDatabaseStatementParameterMySql::~CDatabaseStatementParameterMySql()
	{
		_statement = NULL;
	}

	void CDatabaseStatementParameterMySql::SetNull()
	{
		if ( _binding )
		{
			_binding->_null = true;
		}

		CDatabaseParameter::SetNull();
	}

	void CDatabaseStatementParameterMySql::SetBinding( MYSQL_BIND * bind )
	{
		bind->length = const_cast< unsigned long * >( &GetObjectValue().GetPtrSize() );
		bind->param_number = GetIndex() - 1;

		switch ( GetType() )
		{
		case EFieldType_BIT:
			_binding = MakeOutBind< EFieldType_BIT >( *bind, GetObjectValue(), *this );
			break;

		case EFieldType_TINY_INTEGER:
			_binding = MakeOutBind< EFieldType_TINY_INTEGER >( *bind, GetObjectValue(), *this );
			break;

		case EFieldType_SMALL_INTEGER:
			_binding = MakeOutBind< EFieldType_SMALL_INTEGER >( *bind, GetObjectValue(), *this );
			break;

		case EFieldType_INTEGER:
			_binding = MakeOutBind< EFieldType_INTEGER >( *bind, GetObjectValue(), *this );
			break;

		case EFieldType_LONG_INTEGER:
			_binding = MakeOutBind< EFieldType_LONG_INTEGER >( *bind, GetObjectValue(), *this );
			break;

		case EFieldType_FLOATING_POINT_SIMPLE:
			_binding = MakeOutBind< EFieldType_FLOATING_POINT_SIMPLE >( *bind, GetObjectValue(), *this );
			break;

		case EFieldType_FLOATING_POINT_DOUBLE:
			_binding = MakeOutBind< EFieldType_FLOATING_POINT_DOUBLE >( *bind, GetObjectValue(), *this );
			break;

		case EFieldType_FIXED_POINT:
			_binding = MakeOutBind< EFieldType_FIXED_POINT >( *bind, GetObjectValue(), *this );
			break;

		case EFieldType_VARCHAR:
			_binding = MakeOutBind< EFieldType_VARCHAR >( *bind, GetObjectValue(), *this );
			break;

		case EFieldType_TEXT:
			_binding = MakeOutBind< EFieldType_TEXT >( *bind, GetObjectValue(), *this );
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

		case EFieldType_LONG_VARBINARY:
			_binding = MakeOutBind< EFieldType_LONG_VARBINARY >( *bind, GetObjectValue(), *this );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}
	}
}
END_NAMESPACE_DATABASE_MYSQL
