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
	static const String DATABASE_INCOMPATIBLE_TYPES = STR( "Incompatible types between values, parameter: " );
	static const String DATABASE_PARAMETER_SETVALUE_TYPE_ERROR = STR( "Type error while setting value for the parameter: " );

	bool AreTypesCompatible( EFieldType typeA, EFieldType typeB )
	{
		if ( typeA == typeB )
		{
			return true;
		}

		if ( ( typeA == EFieldType_SMALL_INTEGER || typeA == EFieldType_INTEGER || typeA == EFieldType_LONG_INTEGER )
				&& ( typeB == EFieldType_SMALL_INTEGER || typeB == EFieldType_INTEGER || typeB == EFieldType_LONG_INTEGER ) )
		{
			return true;
		}

		if ( typeA == EFieldType_DATETIME && typeB == EFieldType_TEXT )
		{
			return true;
		}

		return false;
	}

	CDatabaseParameter::CDatabaseParameter( DatabaseConnectionPtr connection, const String & name, unsigned short index, EFieldType fieldType, EParameterType parameterType, SValueUpdater * updater )
		: CDatabaseValuedObject( connection )
		, _name( name )
		, _fieldType( fieldType )
		, _limits( -1 )
		, _index( index )
		, _parameterType( parameterType )
		, _updater( updater )
	{
		DoCreateValue();
	}

	CDatabaseParameter::CDatabaseParameter( DatabaseConnectionPtr connection, const String & name, unsigned short index, EFieldType fieldType, uint32_t limits, EParameterType parameterType, SValueUpdater * updater )
		: CDatabaseValuedObject( connection )
		, _name( name )
		, _fieldType( fieldType )
		, _limits( limits )
		, _index( index )
		, _parameterType( parameterType )
		, _updater( updater )
	{
		DoCreateValue();
	}

	CDatabaseParameter::~CDatabaseParameter()
	{
		delete _updater;
	}

	const unsigned short & CDatabaseParameter::GetIndex() const
	{
		return _index;
	}

	EFieldType CDatabaseParameter::GetType() const
	{
		return _fieldType;
	}

	const String & CDatabaseParameter::GetName() const
	{
		return _name;
	}

	const uint32_t & CDatabaseParameter::GetLimits() const
	{
		return _limits;
	}

	void CDatabaseParameter::SetNull()
	{
		GetObjectValue().SetNull();
		_updater->Update( shared_from_this() );
	}

	const EParameterType & CDatabaseParameter::GetParamType() const
	{
		return _parameterType;
	}

	void CDatabaseParameter::SetValue( DatabaseFieldPtr field )
	{
		if ( !AreTypesCompatible( GetType(), field->GetType() ) )
		{
			String errMsg = DATABASE_INCOMPATIBLE_TYPES + this->GetName();
			CLogger::LogError( errMsg );
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, errMsg );
		}

		SetValue( field->GetType(), field->GetObjectValue() );
	}

	void CDatabaseParameter::SetValue( DatabaseParameterPtr parameter )
	{
		if ( !AreTypesCompatible( GetType(), parameter->GetType() ) )
		{
			String errMsg = DATABASE_INCOMPATIBLE_TYPES + this->GetName();
			CLogger::LogError( errMsg );
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, errMsg );
		}

		SetValue( parameter->GetType(), parameter->GetObjectValue() );
	}

	void CDatabaseParameter::SetValue( EFieldType type, CDatabaseValueBase const & value )
	{
		switch ( type )
		{
		case EFieldType_BOOL:
			DoSetValue( static_cast< CDatabaseValue< EFieldType_BOOL > const & >( value ).GetValue() );
			break;

		case EFieldType_SMALL_INTEGER:
			DoSetValue( static_cast< CDatabaseValue< EFieldType_SMALL_INTEGER > const & >( value ).GetValue() );
			break;

		case EFieldType_INTEGER:
			DoSetValue( static_cast< CDatabaseValue< EFieldType_INTEGER > const & >( value ).GetValue() );
			break;

		case EFieldType_LONG_INTEGER:
			DoSetValue( static_cast< CDatabaseValue< EFieldType_LONG_INTEGER > const & >( value ).GetValue() );
			break;

		case EFieldType_FLOAT:
			DoSetValue( static_cast< CDatabaseValue< EFieldType_FLOAT > const & >( value ).GetValue() );
			break;

		case EFieldType_DOUBLE:
			DoSetValue( static_cast< CDatabaseValue< EFieldType_DOUBLE > const & >( value ).GetValue() );
			break;

		case EFieldType_VARCHAR:
			DoSetValue( static_cast< CDatabaseValue< EFieldType_VARCHAR > const & >( value ).GetValue() );
			break;

		case EFieldType_TEXT:
			DoSetValue( static_cast< CDatabaseValue< EFieldType_TEXT > const & >( value ).GetValue() );
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

		case EFieldType_LONG_VARBINARY:
			DoSetValue( static_cast< CDatabaseValue< EFieldType_LONG_VARBINARY > const & >( value ).GetValue() );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_SETVALUE_TYPE_ERROR + this->GetName() );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_SETVALUE_TYPE_ERROR + this->GetName() );
			break;
		}

		_updater->Update( shared_from_this() );
	}
}
END_NAMESPACE_DATABASE
