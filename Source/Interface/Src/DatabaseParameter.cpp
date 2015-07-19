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

	bool AreTypesCompatible( EFieldType typeA, EFieldType typeB )
	{
		if ( typeA == typeB )
		{
			return true;
		}

		if ( ( typeA == EFieldType_SINT16 || typeA == EFieldType_SINT24 || typeA == EFieldType_SINT32 || typeA == EFieldType_SINT64 )
				&& ( typeB == EFieldType_SINT16 || typeB == EFieldType_SINT24 || typeB == EFieldType_SINT32 || typeB == EFieldType_SINT64 ) )
		{
			return true;
		}

		if ( ( typeA == EFieldType_UINT16 || typeA == EFieldType_UINT24 || typeA == EFieldType_UINT32 || typeA == EFieldType_UINT64 )
				&& ( typeB == EFieldType_UINT16 || typeB == EFieldType_UINT24 || typeB == EFieldType_UINT32 || typeB == EFieldType_UINT64 ) )
		{
			return true;
		}

		if ( typeA == EFieldType_DATETIME && typeB == EFieldType_TEXT )
		{
			return true;
		}

		return false;
	}

	CDatabaseParameter::CDatabaseParameter( DatabaseConnectionPtr connection, const String & name, unsigned short index, EFieldType fieldType, EParameterType parameterType, std::unique_ptr< SValueUpdater > updater )
		: CDatabaseValuedObject( connection )
		, _name( name )
		, _fieldType( fieldType )
		, _precision( std::make_pair( -1, -1 ) )
		, _index( index )
		, _parameterType( parameterType )
		, _updater( std::move( updater ) )
	{
		DoCreateValue();
	}

	CDatabaseParameter::CDatabaseParameter( DatabaseConnectionPtr connection, const String & name, unsigned short index, EFieldType fieldType, uint32_t limits, EParameterType parameterType, std::unique_ptr< SValueUpdater > updater )
		: CDatabaseValuedObject( connection )
		, _name( name )
		, _fieldType( fieldType )
		, _precision( std::make_pair( limits, 0 ) )
		, _index( index )
		, _parameterType( parameterType )
		, _updater( std::move( updater ) )
	{
		DoCreateValue();
	}

	CDatabaseParameter::CDatabaseParameter( DatabaseConnectionPtr connection, const String & name, unsigned short index, EFieldType fieldType, const std::pair< uint32_t, uint32_t > & precision, EParameterType parameterType, std::unique_ptr< SValueUpdater > updater )
		: CDatabaseValuedObject( connection )
		, _name( name )
		, _fieldType( fieldType )
		, _precision( precision )
		, _index( index )
		, _parameterType( parameterType )
		, _updater( std::move( updater ) )
	{
		DoCreateValue();
	}

	CDatabaseParameter::~CDatabaseParameter()
	{
		_updater.reset();;
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
		return _precision.first;
	}

	const std::pair< uint32_t, uint32_t > & CDatabaseParameter::GetPrecision() const
	{
		return _precision;
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
			String errMsg = ERROR_DB_INCOMPATIBLE_TYPES + this->GetName();
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, errMsg );
		}

		SetValue( field->GetType(), field->GetObjectValue() );
	}

	void CDatabaseParameter::SetValue( DatabaseParameterPtr parameter )
	{
		if ( !AreTypesCompatible( GetType(), parameter->GetType() ) )
		{
			String errMsg = ERROR_DB_INCOMPATIBLE_TYPES + this->GetName();
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, errMsg );
		}

		SetValue( parameter->GetType(), parameter->GetObjectValue() );
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
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, ERROR_DB_PARAMETER_SETVALUE_TYPE + this->GetName() );
			break;
		}

		_updater->Update( shared_from_this() );
	}
}
END_NAMESPACE_DATABASE
