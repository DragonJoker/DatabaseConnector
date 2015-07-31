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
	
	bool NeedsLimits( EFieldType type )
	{
		bool result = false;

		switch ( type )
		{
		case EFieldType_NULL:
			result = SFieldTypeNeedsLimits< EFieldType_NULL >::value;
			break;
		case EFieldType_BIT:
			result = SFieldTypeNeedsLimits< EFieldType_BIT >::value;
			break;
		case EFieldType_SINT8:
			result = SFieldTypeNeedsLimits< EFieldType_SINT8 >::value;
			break;
		case EFieldType_SINT16:
			result = SFieldTypeNeedsLimits< EFieldType_SINT16 >::value;
			break;
		case EFieldType_SINT24:
			result = SFieldTypeNeedsLimits< EFieldType_SINT24 >::value;
			break;
		case EFieldType_SINT32:
			result = SFieldTypeNeedsLimits< EFieldType_SINT32 >::value;
			break;
		case EFieldType_SINT64:
			result = SFieldTypeNeedsLimits< EFieldType_SINT64 >::value;
			break;
		case EFieldType_UINT8:
			result = SFieldTypeNeedsLimits< EFieldType_UINT8 >::value;
			break;
		case EFieldType_UINT16:
			result = SFieldTypeNeedsLimits< EFieldType_UINT16 >::value;
			break;
		case EFieldType_UINT24:
			result = SFieldTypeNeedsLimits< EFieldType_UINT24 >::value;
			break;
		case EFieldType_UINT32:
			result = SFieldTypeNeedsLimits< EFieldType_UINT32 >::value;
			break;
		case EFieldType_UINT64:
			result = SFieldTypeNeedsLimits< EFieldType_UINT64 >::value;
			break;
		case EFieldType_FLOAT32:
			result = SFieldTypeNeedsLimits< EFieldType_FLOAT32 >::value;
			break;
		case EFieldType_FLOAT64:
			result = SFieldTypeNeedsLimits< EFieldType_FLOAT64 >::value;
			break;
		case EFieldType_FIXED_POINT:
			result = SFieldTypeNeedsLimits< EFieldType_FIXED_POINT >::value;
			break;
		case EFieldType_CHAR:
			result = SFieldTypeNeedsLimits< EFieldType_CHAR >::value;
			break;
		case EFieldType_VARCHAR:
			result = SFieldTypeNeedsLimits< EFieldType_VARCHAR >::value;
			break;
		case EFieldType_TEXT:
			result = SFieldTypeNeedsLimits< EFieldType_TEXT >::value;
			break;
		case EFieldType_NCHAR:
			result = SFieldTypeNeedsLimits< EFieldType_NCHAR >::value;
			break;
		case EFieldType_NVARCHAR:
			result = SFieldTypeNeedsLimits< EFieldType_NVARCHAR >::value;
			break;
		case EFieldType_NTEXT:
			result = SFieldTypeNeedsLimits< EFieldType_NTEXT >::value;
			break;
		case EFieldType_DATE:
			result = SFieldTypeNeedsLimits< EFieldType_DATE >::value;
			break;
		case EFieldType_DATETIME:
			result = SFieldTypeNeedsLimits< EFieldType_DATETIME >::value;
			break;
		case EFieldType_TIME:
			result = SFieldTypeNeedsLimits< EFieldType_TIME >::value;
			break;
		case EFieldType_BINARY:
			result = SFieldTypeNeedsLimits< EFieldType_BINARY >::value;
			break;
		case EFieldType_VARBINARY:
			result = SFieldTypeNeedsLimits< EFieldType_VARBINARY >::value;
			break;
		case EFieldType_BLOB:
			result = SFieldTypeNeedsLimits< EFieldType_BLOB >::value;
			break;
		default:
			result = false;
			break;
		}

		return result;
	}
	
	bool NeedsPrecision( EFieldType type )
	{
		bool result = false;

		switch ( type )
		{
		case EFieldType_NULL:
			result = SFieldTypeNeedsPrecision< EFieldType_NULL >::value;
			break;
		case EFieldType_BIT:
			result = SFieldTypeNeedsPrecision< EFieldType_BIT >::value;
			break;
		case EFieldType_SINT8:
			result = SFieldTypeNeedsPrecision< EFieldType_SINT8 >::value;
			break;
		case EFieldType_SINT16:
			result = SFieldTypeNeedsPrecision< EFieldType_SINT16 >::value;
			break;
		case EFieldType_SINT24:
			result = SFieldTypeNeedsPrecision< EFieldType_SINT24 >::value;
			break;
		case EFieldType_SINT32:
			result = SFieldTypeNeedsPrecision< EFieldType_SINT32 >::value;
			break;
		case EFieldType_SINT64:
			result = SFieldTypeNeedsPrecision< EFieldType_SINT64 >::value;
			break;
		case EFieldType_UINT8:
			result = SFieldTypeNeedsPrecision< EFieldType_UINT8 >::value;
			break;
		case EFieldType_UINT16:
			result = SFieldTypeNeedsPrecision< EFieldType_UINT16 >::value;
			break;
		case EFieldType_UINT24:
			result = SFieldTypeNeedsPrecision< EFieldType_UINT24 >::value;
			break;
		case EFieldType_UINT32:
			result = SFieldTypeNeedsPrecision< EFieldType_UINT32 >::value;
			break;
		case EFieldType_UINT64:
			result = SFieldTypeNeedsPrecision< EFieldType_UINT64 >::value;
			break;
		case EFieldType_FLOAT32:
			result = SFieldTypeNeedsPrecision< EFieldType_FLOAT32 >::value;
			break;
		case EFieldType_FLOAT64:
			result = SFieldTypeNeedsPrecision< EFieldType_FLOAT64 >::value;
			break;
		case EFieldType_FIXED_POINT:
			result = SFieldTypeNeedsPrecision< EFieldType_FIXED_POINT >::value;
			break;
		case EFieldType_CHAR:
			result = SFieldTypeNeedsPrecision< EFieldType_CHAR >::value;
			break;
		case EFieldType_VARCHAR:
			result = SFieldTypeNeedsPrecision< EFieldType_VARCHAR >::value;
			break;
		case EFieldType_TEXT:
			result = SFieldTypeNeedsPrecision< EFieldType_TEXT >::value;
			break;
		case EFieldType_NCHAR:
			result = SFieldTypeNeedsPrecision< EFieldType_NCHAR >::value;
			break;
		case EFieldType_NVARCHAR:
			result = SFieldTypeNeedsPrecision< EFieldType_NVARCHAR >::value;
			break;
		case EFieldType_NTEXT:
			result = SFieldTypeNeedsPrecision< EFieldType_NTEXT >::value;
			break;
		case EFieldType_DATE:
			result = SFieldTypeNeedsPrecision< EFieldType_DATE >::value;
			break;
		case EFieldType_DATETIME:
			result = SFieldTypeNeedsPrecision< EFieldType_DATETIME >::value;
			break;
		case EFieldType_TIME:
			result = SFieldTypeNeedsPrecision< EFieldType_TIME >::value;
			break;
		case EFieldType_BINARY:
			result = SFieldTypeNeedsPrecision< EFieldType_BINARY >::value;
			break;
		case EFieldType_VARBINARY:
			result = SFieldTypeNeedsPrecision< EFieldType_VARBINARY >::value;
			break;
		case EFieldType_BLOB:
			result = SFieldTypeNeedsPrecision< EFieldType_BLOB >::value;
			break;
		default:
			result = false;
			break;
		}

		return result;
	}

	CDatabaseParameter::CDatabaseParameter( DatabaseConnectionSPtr connection, const String & name, unsigned short index, EFieldType fieldType, EParameterType parameterType, std::unique_ptr< SValueUpdater > updater )
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

	CDatabaseParameter::CDatabaseParameter( DatabaseConnectionSPtr connection, const String & name, unsigned short index, EFieldType fieldType, uint32_t limits, EParameterType parameterType, std::unique_ptr< SValueUpdater > updater )
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

	CDatabaseParameter::CDatabaseParameter( DatabaseConnectionSPtr connection, const String & name, unsigned short index, EFieldType fieldType, const std::pair< uint32_t, uint32_t > & precision, EParameterType parameterType, std::unique_ptr< SValueUpdater > updater )
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
		_updater->Update( *this );
	}

	const EParameterType & CDatabaseParameter::GetParamType() const
	{
		return _parameterType;
	}

	void CDatabaseParameter::SetValue( const CDatabaseField & field )
	{
		if ( !AreTypesCompatible( GetType(), field.GetType() ) )
		{
			String errMsg = ERROR_DB_INCOMPATIBLE_TYPES + this->GetName();
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, errMsg );
		}

		SetValue( field.GetType(), field.GetObjectValue() );
	}

	void CDatabaseParameter::SetValue( const CDatabaseParameter & parameter )
	{
		if ( !AreTypesCompatible( GetType(), parameter.GetType() ) )
		{
			String errMsg = ERROR_DB_INCOMPATIBLE_TYPES + this->GetName();
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, errMsg );
		}

		SetValue( parameter.GetType(), parameter.GetObjectValue() );
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
