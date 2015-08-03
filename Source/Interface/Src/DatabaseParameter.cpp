/************************************************************************//**
* @file DatabaseParameter.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDatabaseParameter class definition.
*
* @details Describes a parameter for a query.
*
***************************************************************************/

#include "DatabasePch.h"

#include "DatabaseParameter.h"

#include "Database.h"
#include "DatabaseField.h"
#include "DatabaseException.h"

BEGIN_NAMESPACE_DATABASE
{
	static const String ERROR_DB_INCOMPATIBLE_TYPES = STR( "Incompatible types between values, parameter: " );
	static const String ERROR_DB_PARAMETER_SETVALUE_TYPE = STR( "Type error while setting value for the parameter: " );

	CDatabaseParameter::CDatabaseParameter( DatabaseConnectionSPtr connection, DatabaseValuedObjectInfosSPtr infos, unsigned short index, EParameterType parameterType, std::unique_ptr< SValueUpdater > updater )
		: CDatabaseValuedObject( connection, infos )
		, _index( index )
		, _parameterType( parameterType )
		, _updater( std::move( updater ) )
	{
		DoCreateValue();
	}

	CDatabaseParameter::~CDatabaseParameter()
	{
		_updater.reset();
	}

	const unsigned short & CDatabaseParameter::GetIndex() const
	{
		return _index;
	}

	void CDatabaseParameter::SetNull()
	{
		GetObjectValue().SetNull();
		_updater->Update( *this );
	}

	const EParameterType & CDatabaseParameter::GetParamType() const
	{
		return _parameterType;
	}

	void CDatabaseParameter::SetValue( const CDatabaseValuedObject & object )
	{
		if ( !AreTypesCompatibleSet( object.GetType(), GetType() ) )
		{
			String errMsg = ERROR_DB_INCOMPATIBLE_TYPES + this->GetName();
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, errMsg );
		}

		SetValue( object.GetType(), object.GetObjectValue() );
	}

	void CDatabaseParameter::SetValue( EFieldType type, CDatabaseValueBase const & value )
	{
		switch ( type )
		{
		case EFieldType_BIT:
			DoSetValue( static_cast< CDatabaseValue< EFieldType_BIT > const & >( value ).GetValue() );
			break;

		case EFieldType_SINT8:
			DoSetValue( static_cast< CDatabaseValue< EFieldType_SINT8 > const & >( value ).GetValue() );
			break;

		case EFieldType_SINT16:
			DoSetValue( static_cast< CDatabaseValue< EFieldType_SINT16 > const & >( value ).GetValue() );
			break;

		case EFieldType_SINT24:
			DoSetValue( static_cast< CDatabaseValue< EFieldType_SINT24 > const & >( value ).GetValue() );
			break;

		case EFieldType_SINT32:
			DoSetValue( static_cast< CDatabaseValue< EFieldType_SINT32 > const & >( value ).GetValue() );
			break;

		case EFieldType_SINT64:
			DoSetValue( static_cast< CDatabaseValue< EFieldType_SINT64 > const & >( value ).GetValue() );
			break;

		case EFieldType_UINT8:
			DoSetValue( static_cast< CDatabaseValue< EFieldType_UINT8 > const & >( value ).GetValue() );
			break;

		case EFieldType_UINT16:
			DoSetValue( static_cast< CDatabaseValue< EFieldType_UINT16 > const & >( value ).GetValue() );
			break;

		case EFieldType_UINT24:
			DoSetValue( static_cast< CDatabaseValue< EFieldType_UINT24 > const & >( value ).GetValue() );
			break;

		case EFieldType_UINT32:
			DoSetValue( static_cast< CDatabaseValue< EFieldType_UINT32 > const & >( value ).GetValue() );
			break;

		case EFieldType_UINT64:
			DoSetValue( static_cast< CDatabaseValue< EFieldType_UINT64 > const & >( value ).GetValue() );
			break;

		case EFieldType_FLOAT32:
			DoSetValue( static_cast< CDatabaseValue< EFieldType_FLOAT32 > const & >( value ).GetValue() );
			break;

		case EFieldType_FLOAT64:
			DoSetValue( static_cast< CDatabaseValue< EFieldType_FLOAT64 > const & >( value ).GetValue() );
			break;

		case EFieldType_FIXED_POINT:
			DoSetValue( static_cast< CDatabaseValue< EFieldType_FIXED_POINT > const & >( value ).GetValue() );
			break;

		case EFieldType_CHAR:
			DoSetValue( static_cast< CDatabaseValue< EFieldType_CHAR > const & >( value ).GetValue() );
			break;

		case EFieldType_VARCHAR:
			DoSetValue( static_cast< CDatabaseValue< EFieldType_VARCHAR > const & >( value ).GetValue() );
			break;

		case EFieldType_TEXT:
			DoSetValue( static_cast< CDatabaseValue< EFieldType_TEXT > const & >( value ).GetValue() );
			break;

		case EFieldType_NCHAR:
			DoSetValue( static_cast< CDatabaseValue< EFieldType_NCHAR > const & >( value ).GetValue() );
			break;

		case EFieldType_NVARCHAR:
			DoSetValue( static_cast< CDatabaseValue< EFieldType_NVARCHAR > const & >( value ).GetValue() );
			break;

		case EFieldType_NTEXT:
			DoSetValue( static_cast< CDatabaseValue< EFieldType_NTEXT > const & >( value ).GetValue() );
			break;

		case EFieldType_DATE:
			DoSetValue( static_cast< CDatabaseValue< EFieldType_DATE > const & >( value ).GetValue() );
			break;

		case EFieldType_DATETIME:
			DoSetValue( static_cast< CDatabaseValue< EFieldType_DATETIME > const & >( value ).GetValue() );
			break;

		case EFieldType_TIME:
			DoSetValue( static_cast< CDatabaseValue< EFieldType_TIME > const & >( value ).GetValue() );
			break;

		case EFieldType_BINARY:
			DoSetValue( static_cast< CDatabaseValue< EFieldType_BINARY > const & >( value ).GetValue() );
			break;

		case EFieldType_VARBINARY:
			DoSetValue( static_cast< CDatabaseValue< EFieldType_VARBINARY > const & >( value ).GetValue() );
			break;

		case EFieldType_BLOB:
			DoSetValue( static_cast< CDatabaseValue< EFieldType_BLOB > const & >( value ).GetValue() );
			break;

		default:
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, ERROR_DB_PARAMETER_SETVALUE_TYPE + this->GetName() );
			break;
		}

		_updater->Update( *this );
	}
}
END_NAMESPACE_DATABASE
