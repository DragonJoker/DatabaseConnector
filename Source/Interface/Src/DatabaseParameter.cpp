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
	static const String DATABASE_PARAMETER_ERROR = STR( "Error while treating a parameter." );
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
		, _limits( 0 )
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
		if ( !AreTypesCompatible( _fieldType, field->GetType() ) )
		{
			String errMsg = DATABASE_PARAMETER_ERROR + this->GetName();
			CLogger::LogError( errMsg );
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, DATABASE_PARAMETER_ERROR );
		}

		GetObjectValue().SetValue( field->GetObjectValue() );
		_updater->Update( shared_from_this() );
	}

	void CDatabaseParameter::SetValue( DatabaseParameterPtr parameter )
	{
		if ( !AreTypesCompatible( GetType(), parameter->GetType() ) )
		{
			String errMsg = DATABASE_PARAMETER_ERROR + this->GetName();
			CLogger::LogError( errMsg );
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, DATABASE_PARAMETER_ERROR );
		}

		GetObjectValue().SetValue( parameter->GetObjectValue() );
		_updater->Update( shared_from_this() );
	}

	void CDatabaseParameter::DoSetBlob( uint8_t * value, uint32_t length )
	{
		switch ( GetType() )
		{
		case EFieldType_BINARY:
			static_cast< CDatabaseValue< EFieldType_BINARY > & >( GetObjectValue() ).SetValue( value, std::min( GetLimits(), length ) );
			break;

		case EFieldType_VARBINARY:
			static_cast< CDatabaseValue< EFieldType_VARBINARY > & >( GetObjectValue() ).SetValue( value, std::min( GetLimits(), length ) );
			break;

		case EFieldType_LONG_VARBINARY:
			static_cast< CDatabaseValue< EFieldType_LONG_VARBINARY > & >( GetObjectValue() ).SetValue( value, std::min( GetLimits(), length ) );
			break;

		default:
			String errMsg = DATABASE_PARAMETER_SETVALUE_TYPE_ERROR + this->GetName();
			CLogger::LogError( errMsg );
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, DATABASE_PARAMETER_SETVALUE_TYPE_ERROR );
			break;
		}
	}
}
END_NAMESPACE_DATABASE
