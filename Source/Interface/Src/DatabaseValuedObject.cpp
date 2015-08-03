/************************************************************************//**
* @file DatabaseValuedObject.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDatabaseValuedObject class declaration.
*
* @details Describes a an object (field or parameter) with a value.
*
***************************************************************************/

#include "DatabasePch.h"

#include "DatabaseValuedObject.h"

#include "DatabaseValuedObjectInfos.h"
#include "DatabaseDateTimeHelper.h"
#include "DatabaseLogger.h"
#include "DatabaseValue.h"
#include "DatabaseException.h"
#include "DatabaseFixedPoint.h"

BEGIN_NAMESPACE_DATABASE
{
	static const String ERROR_DB_FIELD_CREATION_TYPE = STR( "Type error while creating the object: " );
	static const String ERROR_DB_FIELD_GETVALUE_TYPE = STR( "Type error while getting value from the object: " );
	static const String ERROR_DB_FIELD_SETVALUE_TYPE = STR( "Type error while setting value of the object: " );

	CDatabaseValuedObject::CDatabaseValuedObject( DatabaseConnectionSPtr connection, DatabaseValuedObjectInfosSPtr infos )
		: _connection( connection )
		, _infos( infos )
	{
	}

	CDatabaseValuedObject::~CDatabaseValuedObject()
	{
	}

	EFieldType CDatabaseValuedObject::GetType() const
	{
		return _infos->GetType();
	}

	const String & CDatabaseValuedObject::GetName() const
	{
		return _infos->GetName();
	}

	const uint32_t & CDatabaseValuedObject::GetLimits() const
	{
		return _infos->GetLimits();
	}

	const std::pair< uint32_t, uint32_t > & CDatabaseValuedObject::GetPrecision() const
	{
		return _infos->GetPrecision();
	}

	void CDatabaseValuedObject::DoGetValue( bool & value ) const
	{
		switch ( GetType() )
		{
		case EFieldType_BIT:
			value = static_cast< CDatabaseValue< EFieldType_BIT > & >( *_value ).GetValue();
			break;

		case EFieldType_SINT8:
			value = static_cast< CDatabaseValue< EFieldType_SINT8 > & >( *_value ).GetValue() != 0;
			break;

		case EFieldType_SINT16:
			value = static_cast< CDatabaseValue< EFieldType_SINT16 > & >( *_value ).GetValue() != 0;
			break;

		case EFieldType_SINT24:
			value = static_cast< CDatabaseValue< EFieldType_SINT24 > & >( *_value ).GetValue() != 0;
			break;

		case EFieldType_SINT32:
			value = static_cast< CDatabaseValue< EFieldType_SINT32 > & >( *_value ).GetValue() != 0;
			break;

		case EFieldType_UINT8:
			value = static_cast< CDatabaseValue< EFieldType_UINT8 > & >( *_value ).GetValue() != 0;
			break;

		case EFieldType_UINT16:
			value = static_cast< CDatabaseValue< EFieldType_UINT16 > & >( *_value ).GetValue() != 0;
			break;

		case EFieldType_UINT24:
			value = static_cast< CDatabaseValue< EFieldType_UINT24 > & >( *_value ).GetValue() != 0;
			break;

		case EFieldType_UINT32:
			value = static_cast< CDatabaseValue< EFieldType_UINT32 > & >( *_value ).GetValue() != 0;
			break;

		case EFieldType_VARCHAR:
		{
			std::string const & val = static_cast< CDatabaseValue< EFieldType_VARCHAR > & >( *_value ).GetValue();

			if ( ( !val.empty() ) && ( ( StringUtils::UpperCase( val ) == "TRUE" ) || ( val == "1" ) ) )
			{
				value = true;
			}
			else
			{
				value = false;
			}

			break;
		}

		case EFieldType_NVARCHAR:
		{
			std::wstring const & val = static_cast< CDatabaseValue< EFieldType_NVARCHAR > & >( *_value ).GetValue();

			if ( ( !val.empty() ) && ( ( StringUtils::UpperCase( val ) == L"TRUE" ) || ( val == L"1" ) ) )
			{
				value = true;
			}
			else
			{
				value = false;
			}

			break;
		}

		default:
			String errMsg = ERROR_DB_FIELD_GETVALUE_TYPE + this->GetName();
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_FIELD_GETVALUE_TYPE );
			break;
		}
	}

	void CDatabaseValuedObject::DoGetValue( int8_t & value ) const
	{
		switch ( GetType() )
		{
		case EFieldType_BIT:
			value = static_cast< CDatabaseValue< EFieldType_BIT > & >( *_value ).GetValue() ? 1 : 0;
			break;

		case EFieldType_SINT8:
			value = static_cast< CDatabaseValue< EFieldType_SINT8 > & >( *_value ).GetValue();
			break;

		case EFieldType_UINT8:
			value = static_cast< CDatabaseValue< EFieldType_UINT8 > & >( *_value ).GetValue();
			break;

		default:
			String errMsg = ERROR_DB_FIELD_GETVALUE_TYPE + this->GetName();
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_FIELD_GETVALUE_TYPE );
			break;
		}
	}

	void CDatabaseValuedObject::DoGetValue( uint8_t & value ) const
	{
		switch ( GetType() )
		{
		case EFieldType_BIT:
			value = static_cast< CDatabaseValue< EFieldType_BIT > & >( *_value ).GetValue() ? 1 : 0;
			break;

		case EFieldType_SINT8:
			value = static_cast< CDatabaseValue< EFieldType_SINT8 > & >( *_value ).GetValue();
			break;

		case EFieldType_UINT8:
			value = static_cast< CDatabaseValue< EFieldType_UINT8 > & >( *_value ).GetValue();
			break;

		default:
			String errMsg = ERROR_DB_FIELD_GETVALUE_TYPE + this->GetName();
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_FIELD_GETVALUE_TYPE );
			break;
		}
	}

	void CDatabaseValuedObject::DoGetValue( int16_t & value ) const
	{
		switch ( GetType() )
		{
		case EFieldType_BIT:
			value = static_cast< CDatabaseValue< EFieldType_BIT > & >( *_value ).GetValue() ? 1 : 0;
			break;

		case EFieldType_SINT8:
			value = static_cast< CDatabaseValue< EFieldType_SINT8 > & >( *_value ).GetValue();
			break;

		case EFieldType_SINT16:
			value = static_cast< CDatabaseValue< EFieldType_SINT16 > & >( *_value ).GetValue();
			break;

		case EFieldType_UINT8:
			value = static_cast< CDatabaseValue< EFieldType_UINT8 > & >( *_value ).GetValue();
			break;

		case EFieldType_UINT16:
			value = static_cast< CDatabaseValue< EFieldType_UINT16 > & >( *_value ).GetValue();
			break;

		default:
			String errMsg = ERROR_DB_FIELD_GETVALUE_TYPE + this->GetName();
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_FIELD_GETVALUE_TYPE );
			break;
		}
	}

	void CDatabaseValuedObject::DoGetValue( uint16_t & value ) const
	{
		switch ( GetType() )
		{
		case EFieldType_BIT:
			value = static_cast< CDatabaseValue< EFieldType_BIT > & >( *_value ).GetValue() ? 1 : 0;
			break;

		case EFieldType_SINT8:
			value = static_cast< CDatabaseValue< EFieldType_SINT8 > & >( *_value ).GetValue();
			break;

		case EFieldType_SINT16:
			value = static_cast< CDatabaseValue< EFieldType_SINT16 > & >( *_value ).GetValue();
			break;

		case EFieldType_UINT8:
			value = static_cast< CDatabaseValue< EFieldType_UINT8 > & >( *_value ).GetValue();
			break;

		case EFieldType_UINT16:
			value = static_cast< CDatabaseValue< EFieldType_UINT16 > & >( *_value ).GetValue();
			break;

		default:
			String errMsg = ERROR_DB_FIELD_GETVALUE_TYPE + this->GetName();
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_FIELD_GETVALUE_TYPE );
			break;
		}
	}

	void CDatabaseValuedObject::DoGetValue( int24_t & value ) const
	{
		switch ( GetType() )
		{
		case EFieldType_BIT:
			value = static_cast< CDatabaseValue< EFieldType_BIT > & >( *_value ).GetValue() ? 1 : 0;
			break;

		case EFieldType_SINT8:
			value = static_cast< CDatabaseValue< EFieldType_SINT8 > & >( *_value ).GetValue();
			break;

		case EFieldType_SINT16:
			value = static_cast< CDatabaseValue< EFieldType_SINT16 > & >( *_value ).GetValue();
			break;

		case EFieldType_SINT24:
			value = static_cast< CDatabaseValue< EFieldType_SINT24 > & >( *_value ).GetValue();
			break;

		case EFieldType_UINT8:
			value = static_cast< CDatabaseValue< EFieldType_UINT8 > & >( *_value ).GetValue();
			break;

		case EFieldType_UINT16:
			value = static_cast< CDatabaseValue< EFieldType_UINT16 > & >( *_value ).GetValue();
			break;

		case EFieldType_UINT24:
			value = static_cast< CDatabaseValue< EFieldType_UINT24 > & >( *_value ).GetValue();
			break;

		default:
			String errMsg = ERROR_DB_FIELD_GETVALUE_TYPE + this->GetName();
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_FIELD_GETVALUE_TYPE );
			break;
		}
	}

	void CDatabaseValuedObject::DoGetValue( uint24_t & value ) const
	{
		switch ( GetType() )
		{
		case EFieldType_BIT:
			value = static_cast< CDatabaseValue< EFieldType_BIT > & >( *_value ).GetValue() ? 1 : 0;
			break;

		case EFieldType_SINT8:
			value = static_cast< CDatabaseValue< EFieldType_SINT8 > & >( *_value ).GetValue();
			break;

		case EFieldType_SINT16:
			value = static_cast< CDatabaseValue< EFieldType_SINT16 > & >( *_value ).GetValue();
			break;

		case EFieldType_SINT24:
			value = static_cast< CDatabaseValue< EFieldType_SINT24 > & >( *_value ).GetValue();
			break;

		case EFieldType_UINT8:
			value = static_cast< CDatabaseValue< EFieldType_UINT8 > & >( *_value ).GetValue();
			break;

		case EFieldType_UINT16:
			value = static_cast< CDatabaseValue< EFieldType_UINT16 > & >( *_value ).GetValue();
			break;

		case EFieldType_UINT24:
			value = static_cast< CDatabaseValue< EFieldType_UINT24 > & >( *_value ).GetValue();
			break;

		default:
			String errMsg = ERROR_DB_FIELD_GETVALUE_TYPE + this->GetName();
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_FIELD_GETVALUE_TYPE );
			break;
		}
	}

	void CDatabaseValuedObject::DoGetValue( int32_t & value ) const
	{
		switch ( GetType() )
		{
		case EFieldType_BIT:
			value = static_cast< CDatabaseValue< EFieldType_BIT > & >( *_value ).GetValue() ? 1 : 0;
			break;

		case EFieldType_SINT8:
			value = static_cast< CDatabaseValue< EFieldType_SINT8 > & >( *_value ).GetValue();
			break;

		case EFieldType_SINT16:
			value = static_cast< CDatabaseValue< EFieldType_SINT16 > & >( *_value ).GetValue();
			break;

		case EFieldType_SINT24:
			value = static_cast< CDatabaseValue< EFieldType_SINT24 > & >( *_value ).GetValue();
			break;

		case EFieldType_SINT32:
			value = static_cast< CDatabaseValue< EFieldType_SINT32 > & >( *_value ).GetValue();
			break;

		case EFieldType_UINT8:
			value = static_cast< CDatabaseValue< EFieldType_UINT8 > & >( *_value ).GetValue();
			break;

		case EFieldType_UINT16:
			value = static_cast< CDatabaseValue< EFieldType_UINT16 > & >( *_value ).GetValue();
			break;

		case EFieldType_UINT24:
			value = static_cast< CDatabaseValue< EFieldType_UINT24 > & >( *_value ).GetValue();
			break;

		case EFieldType_UINT32:
			value = static_cast< CDatabaseValue< EFieldType_UINT32 > & >( *_value ).GetValue();
			break;

		case EFieldType_FLOAT32:
			value = uint32_t ( static_cast< CDatabaseValue< EFieldType_FLOAT32 > & >( *_value ).GetValue() );
			break;

		case EFieldType_FLOAT64:
			value = int32_t ( static_cast< CDatabaseValue< EFieldType_FLOAT64 > & >( *_value ).GetValue() );
			break;

		default:
			String errMsg = ERROR_DB_FIELD_GETVALUE_TYPE + this->GetName();
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_FIELD_GETVALUE_TYPE );
			break;
		}
	}

	void CDatabaseValuedObject::DoGetValue( uint32_t & value ) const
	{
		switch ( GetType() )
		{
		case EFieldType_BIT:
			value = static_cast< CDatabaseValue< EFieldType_BIT > & >( *_value ).GetValue() ? 1 : 0;
			break;

		case EFieldType_SINT8:
			value = static_cast< CDatabaseValue< EFieldType_SINT8 > & >( *_value ).GetValue();
			break;

		case EFieldType_SINT16:
			value = static_cast< CDatabaseValue< EFieldType_SINT16 > & >( *_value ).GetValue();
			break;

		case EFieldType_SINT24:
			value = static_cast< CDatabaseValue< EFieldType_SINT24 > & >( *_value ).GetValue();
			break;

		case EFieldType_SINT32:
			value = static_cast< CDatabaseValue< EFieldType_SINT32 > & >( *_value ).GetValue();
			break;

		case EFieldType_UINT8:
			value = static_cast< CDatabaseValue< EFieldType_UINT8 > & >( *_value ).GetValue();
			break;

		case EFieldType_UINT16:
			value = static_cast< CDatabaseValue< EFieldType_UINT16 > & >( *_value ).GetValue();
			break;

		case EFieldType_UINT24:
			value = static_cast< CDatabaseValue< EFieldType_UINT24 > & >( *_value ).GetValue();
			break;

		case EFieldType_UINT32:
			value = static_cast< CDatabaseValue< EFieldType_UINT32 > & >( *_value ).GetValue();
			break;

		case EFieldType_FLOAT32:
			value = uint32_t ( static_cast< CDatabaseValue< EFieldType_FLOAT32 > & >( *_value ).GetValue() );
			break;

		case EFieldType_FLOAT64:
			value = uint32_t ( static_cast< CDatabaseValue< EFieldType_FLOAT64 > & >( *_value ).GetValue() );
			break;

		default:
			String errMsg = ERROR_DB_FIELD_GETVALUE_TYPE + this->GetName();
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_FIELD_GETVALUE_TYPE );
			break;
		}
	}

	void CDatabaseValuedObject::DoGetValue( int64_t & value ) const
	{
		switch ( GetType() )
		{
		case EFieldType_SINT8:
			value = static_cast< CDatabaseValue< EFieldType_SINT8 > & >( *_value ).GetValue();
			break;

		case EFieldType_SINT16:
			value = static_cast< CDatabaseValue< EFieldType_SINT16 > & >( *_value ).GetValue();
			break;

		case EFieldType_SINT24:
			value = static_cast< CDatabaseValue< EFieldType_SINT24 > & >( *_value ).GetValue();
			break;

		case EFieldType_SINT32:
			value = static_cast< CDatabaseValue< EFieldType_SINT32 > & >( *_value ).GetValue();
			break;

		case EFieldType_SINT64:
			value = static_cast< CDatabaseValue< EFieldType_SINT64 > & >( *_value ).GetValue();
			break;

		case EFieldType_UINT8:
			value = static_cast< CDatabaseValue< EFieldType_UINT8 > & >( *_value ).GetValue();
			break;

		case EFieldType_UINT16:
			value = static_cast< CDatabaseValue< EFieldType_UINT16 > & >( *_value ).GetValue();
			break;

		case EFieldType_UINT24:
			value = static_cast< CDatabaseValue< EFieldType_UINT24 > & >( *_value ).GetValue();
			break;

		case EFieldType_UINT32:
			value = static_cast< CDatabaseValue< EFieldType_UINT32 > & >( *_value ).GetValue();
			break;

		case EFieldType_UINT64:
			value = static_cast< CDatabaseValue< EFieldType_UINT64 > & >( *_value ).GetValue();
			break;

		case EFieldType_FLOAT32:
			value = int64_t ( static_cast< CDatabaseValue< EFieldType_FLOAT32 > & >( *_value ).GetValue() );
			break;

		case EFieldType_FLOAT64:
			value = int64_t ( static_cast< CDatabaseValue< EFieldType_FLOAT64 > & >( *_value ).GetValue() );
			break;

		default:
			String errMsg = ERROR_DB_FIELD_GETVALUE_TYPE + this->GetName();
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_FIELD_GETVALUE_TYPE );
			break;
		}
	}

	void CDatabaseValuedObject::DoGetValue( uint64_t & value ) const
	{
		switch ( GetType() )
		{
		case EFieldType_SINT8:
			value = static_cast< CDatabaseValue< EFieldType_SINT8 > & >( *_value ).GetValue();
			break;

		case EFieldType_SINT16:
			value = static_cast< CDatabaseValue< EFieldType_SINT16 > & >( *_value ).GetValue();
			break;

		case EFieldType_SINT24:
			value = static_cast< CDatabaseValue< EFieldType_SINT24 > & >( *_value ).GetValue();
			break;

		case EFieldType_SINT32:
			value = static_cast< CDatabaseValue< EFieldType_SINT32 > & >( *_value ).GetValue();
			break;

		case EFieldType_SINT64:
			value = static_cast< CDatabaseValue< EFieldType_SINT64 > & >( *_value ).GetValue();
			break;

		case EFieldType_UINT8:
			value = static_cast< CDatabaseValue< EFieldType_UINT8 > & >( *_value ).GetValue();
			break;

		case EFieldType_UINT16:
			value = static_cast< CDatabaseValue< EFieldType_UINT16 > & >( *_value ).GetValue();
			break;

		case EFieldType_UINT24:
			value = static_cast< CDatabaseValue< EFieldType_UINT24 > & >( *_value ).GetValue();
			break;

		case EFieldType_UINT32:
			value = static_cast< CDatabaseValue< EFieldType_UINT32 > & >( *_value ).GetValue();
			break;

		case EFieldType_UINT64:
			value = static_cast< CDatabaseValue< EFieldType_UINT64 > & >( *_value ).GetValue();
			break;

		case EFieldType_FLOAT32:
			value = uint64_t ( static_cast< CDatabaseValue< EFieldType_FLOAT32 > & >( *_value ).GetValue() );
			break;

		case EFieldType_FLOAT64:
			value = uint64_t ( static_cast< CDatabaseValue< EFieldType_FLOAT64 > & >( *_value ).GetValue() );
			break;

		default:
			String errMsg = ERROR_DB_FIELD_GETVALUE_TYPE + this->GetName();
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_FIELD_GETVALUE_TYPE );
			break;
		}
	}

	void CDatabaseValuedObject::DoGetValue( float & value ) const
	{
		switch ( GetType() )
		{
		case EFieldType_SINT16:
			value = float( static_cast< CDatabaseValue< EFieldType_SINT16 > & >( *_value ).GetValue() );
			break;

		case EFieldType_SINT24:
			value = float( static_cast< CDatabaseValue< EFieldType_SINT24 > & >( *_value ).GetValue() );
			break;

		case EFieldType_SINT32:
			value = float( static_cast< CDatabaseValue< EFieldType_SINT32 > & >( *_value ).GetValue() );
			break;

		case EFieldType_SINT64:
			value = float( static_cast< CDatabaseValue< EFieldType_SINT64 > & >( *_value ).GetValue() );
			break;

		case EFieldType_UINT16:
			value = float( static_cast< CDatabaseValue< EFieldType_UINT16 > & >( *_value ).GetValue() );
			break;

		case EFieldType_UINT24:
			value = float( static_cast< CDatabaseValue< EFieldType_UINT24 > & >( *_value ).GetValue() );
			break;

		case EFieldType_UINT32:
			value = float( static_cast< CDatabaseValue< EFieldType_UINT32 > & >( *_value ).GetValue() );
			break;

		case EFieldType_UINT64:
			value = float( static_cast< CDatabaseValue< EFieldType_UINT64 > & >( *_value ).GetValue() );
			break;

		case EFieldType_FLOAT32:
			value = static_cast< CDatabaseValue< EFieldType_FLOAT32 > & >( *_value ).GetValue();
			break;

		case EFieldType_FLOAT64:
			value = float( static_cast< CDatabaseValue< EFieldType_FLOAT64 > & >( *_value ).GetValue() );
			break;

		case EFieldType_FIXED_POINT:
			value = static_cast< CDatabaseValue< EFieldType_FIXED_POINT > & >( *_value ).GetValue().ToFloat();
			break;

		default:
			String errMsg = ERROR_DB_FIELD_GETVALUE_TYPE + this->GetName();
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_FIELD_GETVALUE_TYPE );
			break;
		}
	}

	void CDatabaseValuedObject::DoGetValue( double & value ) const
	{
		switch ( GetType() )
		{
		case EFieldType_SINT16:
			value = double( static_cast< CDatabaseValue< EFieldType_SINT16 > & >( *_value ).GetValue() );
			break;

		case EFieldType_SINT24:
			value = double( static_cast< CDatabaseValue< EFieldType_SINT24 > & >( *_value ).GetValue() );
			break;

		case EFieldType_SINT32:
			value = double( static_cast< CDatabaseValue< EFieldType_SINT32 > & >( *_value ).GetValue() );
			break;

		case EFieldType_SINT64:
			value = double( static_cast< CDatabaseValue< EFieldType_SINT64 > & >( *_value ).GetValue() );
			break;

		case EFieldType_UINT16:
			value = double( static_cast< CDatabaseValue< EFieldType_UINT16 > & >( *_value ).GetValue() );
			break;

		case EFieldType_UINT24:
			value = double( static_cast< CDatabaseValue< EFieldType_UINT24 > & >( *_value ).GetValue() );
			break;

		case EFieldType_UINT32:
			value = double( static_cast< CDatabaseValue< EFieldType_UINT32 > & >( *_value ).GetValue() );
			break;

		case EFieldType_UINT64:
			value = double( static_cast< CDatabaseValue< EFieldType_UINT64 > & >( *_value ).GetValue() );
			break;

		case EFieldType_FLOAT32:
			value = double( static_cast< CDatabaseValue< EFieldType_FLOAT32 > & >( *_value ).GetValue() );
			break;

		case EFieldType_FLOAT64:
			value = static_cast< CDatabaseValue< EFieldType_FLOAT64 > & >( *_value ).GetValue();
			break;

		case EFieldType_FIXED_POINT:
			value = static_cast< CDatabaseValue< EFieldType_FIXED_POINT > & >( *_value ).GetValue().ToDouble();
			break;

		default:
			String errMsg = ERROR_DB_FIELD_GETVALUE_TYPE + this->GetName();
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_FIELD_GETVALUE_TYPE );
			break;
		}
	}

	void CDatabaseValuedObject::DoGetValue( CFixedPoint & value ) const
	{
		switch ( GetType() )
		{
		case EFieldType_SINT8:
			value = CFixedPoint( static_cast< CDatabaseValue< EFieldType_SINT8 > & >( *_value ).GetValue(), 4, 0 );
			break;

		case EFieldType_SINT16:
			value = CFixedPoint( static_cast< CDatabaseValue< EFieldType_SINT16 > & >( *_value ).GetValue(), 6, 0 );
			break;

		case EFieldType_SINT24:
			value = CFixedPoint( int32_t( static_cast< CDatabaseValue< EFieldType_SINT24 > & >( *_value ).GetValue() ), 10, 0 );
			break;

		case EFieldType_SINT32:
			value = CFixedPoint( static_cast< CDatabaseValue< EFieldType_SINT32 > & >( *_value ).GetValue(), 12, 0 );
			break;

		case EFieldType_SINT64:
			value = CFixedPoint( static_cast< CDatabaseValue< EFieldType_SINT64 > & >( *_value ).GetValue(), CFixedPoint::GetMaxPrecision(), 0 );
			break;

		case EFieldType_UINT8:
			value = CFixedPoint( static_cast< CDatabaseValue< EFieldType_UINT8 > & >( *_value ).GetValue(), 4, 0 );
			break;

		case EFieldType_UINT16:
			value = CFixedPoint( static_cast< CDatabaseValue< EFieldType_UINT16 > & >( *_value ).GetValue(), 6, 0 );
			break;

		case EFieldType_UINT24:
			value = CFixedPoint( uint32_t( static_cast< CDatabaseValue< EFieldType_UINT24 > & >( *_value ).GetValue() ), 10, 0 );
			break;

		case EFieldType_UINT32:
			value = CFixedPoint( static_cast< CDatabaseValue< EFieldType_UINT32 > & >( *_value ).GetValue(), 12, 0 );
			break;

		case EFieldType_UINT64:
			value = CFixedPoint( static_cast< CDatabaseValue< EFieldType_UINT32 > & >( *_value ).GetValue(), CFixedPoint::GetMaxPrecision(), 0 );
			break;

		case EFieldType_FLOAT32:
			value = CFixedPoint( static_cast< CDatabaseValue< EFieldType_FLOAT32 > & >( *_value ).GetValue(), CFixedPoint::GetMaxPrecision(), 7 );
			break;

		case EFieldType_FLOAT64:
			value = CFixedPoint( static_cast< CDatabaseValue< EFieldType_FLOAT64 > & >( *_value ).GetValue(), CFixedPoint::GetMaxPrecision(), 7 );
			break;

		case EFieldType_FIXED_POINT:
			value = static_cast< CDatabaseValue< EFieldType_FIXED_POINT > & >( *_value ).GetValue();
			break;

		default:
			String errMsg = ERROR_DB_FIELD_GETVALUE_TYPE + this->GetName();
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_FIELD_GETVALUE_TYPE );
			break;
		}
	}

	void CDatabaseValuedObject::DoGetValue( std::string & value ) const
	{
		value.clear();

		switch ( GetType() )
		{
		case EFieldType_TEXT:
			value = static_cast< CDatabaseValue< EFieldType_TEXT > & >( *_value ).GetValue();
			break;

		case EFieldType_CHAR:
			value = static_cast< CDatabaseValue< EFieldType_CHAR > & >( *_value ).GetValue();
			break;

		case EFieldType_VARCHAR:
			value = static_cast< CDatabaseValue< EFieldType_VARCHAR > & >( *_value ).GetValue();
			break;

		case EFieldType_NTEXT:
			value = StringUtils::ToStr( static_cast< CDatabaseValue< EFieldType_NTEXT > & >( *_value ).GetValue() );
			break;

		case EFieldType_NCHAR:
			value = StringUtils::ToStr( static_cast< CDatabaseValue< EFieldType_NCHAR > & >( *_value ).GetValue() );
			break;

		case EFieldType_NVARCHAR:
			value = StringUtils::ToStr( static_cast< CDatabaseValue< EFieldType_NVARCHAR > & >( *_value ).GetValue() );
			break;

		default:
			String errMsg = ERROR_DB_FIELD_GETVALUE_TYPE + this->GetName();
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_FIELD_GETVALUE_TYPE );
			break;
		}
	}

	void CDatabaseValuedObject::DoGetValue( std::wstring & value ) const
	{
		switch ( GetType() )
		{
		case EFieldType_TEXT:
			value = StringUtils::ToWStr( static_cast< CDatabaseValue< EFieldType_TEXT > & >( *_value ).GetValue() );
			break;

		case EFieldType_CHAR:
			value = StringUtils::ToWStr( static_cast< CDatabaseValue< EFieldType_CHAR > & >( *_value ).GetValue() );
			break;

		case EFieldType_VARCHAR:
			value = StringUtils::ToWStr( static_cast< CDatabaseValue< EFieldType_VARCHAR > & >( *_value ).GetValue() );
			break;

		case EFieldType_NTEXT:
			value = static_cast< CDatabaseValue< EFieldType_NTEXT > & >( *_value ).GetValue();
			break;

		case EFieldType_NCHAR:
			value = static_cast< CDatabaseValue< EFieldType_NCHAR > & >( *_value ).GetValue();
			break;

		case EFieldType_NVARCHAR:
			value = static_cast< CDatabaseValue< EFieldType_NVARCHAR > & >( *_value ).GetValue();
			break;

		default:
			String errMsg = ERROR_DB_FIELD_GETVALUE_TYPE + this->GetName();
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_FIELD_GETVALUE_TYPE );
			break;
		}
	}

	void CDatabaseValuedObject::DoGetValue( DateType & value ) const
	{
		switch ( GetType() )
		{
		case EFieldType_DATE:
			value = static_cast< CDatabaseValue< EFieldType_DATE > & >( *_value ).GetValue();
			break;

		case EFieldType_DATETIME:
			value = DateFromDateTime( static_cast< CDatabaseValue< EFieldType_DATETIME > & >( *_value ).GetValue() );
			break;

		case EFieldType_NVARCHAR:// ODBC stores date as NVARCHAR
			value = GetConnection()->ParseDate( StringUtils::ToString( static_cast< CDatabaseValue< EFieldType_NVARCHAR > & >( *_value ).GetValue() ) );
			break;

		default:
			String errMsg = ERROR_DB_FIELD_GETVALUE_TYPE + this->GetName();
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_FIELD_GETVALUE_TYPE );
			break;
		}
	}

	void CDatabaseValuedObject::DoGetValue( DateTimeType & value ) const
	{
		switch ( GetType() )
		{
		case EFieldType_DATE:
			value = DateTimeType( static_cast< CDatabaseValue< EFieldType_DATE > & >( *_value ).GetValue() );
			break;

		case EFieldType_DATETIME:
			value = static_cast< CDatabaseValue< EFieldType_DATETIME > & >( *_value ).GetValue();
			break;

		case EFieldType_TIME:
			value = DateTimeType( DateType( boost::gregorian::min_date_time ), static_cast< CDatabaseValue< EFieldType_TIME > & >( *_value ).GetValue() );
			break;

		default:
			String errMsg = ERROR_DB_FIELD_GETVALUE_TYPE + this->GetName();
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_FIELD_GETVALUE_TYPE );
			break;
		}
	}

	void CDatabaseValuedObject::DoGetValue( TimeType & value ) const
	{
		switch ( GetType() )
		{
		case EFieldType_DATETIME:
			value = TimeFromDateTime( static_cast< CDatabaseValue< EFieldType_DATETIME > & >( *_value ).GetValue() );
			break;

		case EFieldType_TIME:
			value = static_cast< CDatabaseValue< EFieldType_TIME > & >( *_value ).GetValue();
			break;

		default:
			String errMsg = ERROR_DB_FIELD_GETVALUE_TYPE + this->GetName();
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_FIELD_GETVALUE_TYPE );
			break;
		}
	}

	void CDatabaseValuedObject::DoGetValue( ByteArray & value ) const
	{
		value.clear();

		switch ( GetType() )
		{
		case EFieldType_BINARY:
			value = static_cast< CDatabaseValue< EFieldType_BINARY > & >( *_value ).GetValue();
			break;

		case EFieldType_VARBINARY:
			value = static_cast< CDatabaseValue< EFieldType_VARBINARY > & >( *_value ).GetValue();
			break;

		case EFieldType_BLOB:
			value = static_cast< CDatabaseValue< EFieldType_BLOB > & >( *_value ).GetValue();
			break;

		default:
			String errMsg = ERROR_DB_FIELD_GETVALUE_TYPE + this->GetName();
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_FIELD_GETVALUE_TYPE );
			break;
		}
	}

	void CDatabaseValuedObject::DoGetValueFast( bool & value ) const
	{
		assert( GetType() == EFieldType_BIT );
		value = static_cast< CDatabaseValue< EFieldType_BIT > & >( *_value ).GetValue();
	}

	void CDatabaseValuedObject::DoGetValueFast( int8_t & value ) const
	{
		assert( GetType() == EFieldType_SINT8 || GetType() == EFieldType_UINT8 );
		value = static_cast< CDatabaseValue< EFieldType_SINT8 > & >( *_value ).GetValue();
	}

	void CDatabaseValuedObject::DoGetValueFast( uint8_t & value ) const
	{
		assert( GetType() == EFieldType_SINT8 || GetType() == EFieldType_UINT8 );
		value = static_cast< CDatabaseValue< EFieldType_UINT8 > & >( *_value ).GetValue();
	}

	void CDatabaseValuedObject::DoGetValueFast( int16_t & value ) const
	{
		assert( GetType() == EFieldType_SINT16 || GetType() == EFieldType_UINT16 );
		value = static_cast< CDatabaseValue< EFieldType_SINT16 > & >( *_value ).GetValue();
	}

	void CDatabaseValuedObject::DoGetValueFast( uint16_t & value ) const
	{
		assert( GetType() == EFieldType_SINT16 || GetType() == EFieldType_UINT16 );
		value = static_cast< CDatabaseValue< EFieldType_UINT16 > & >( *_value ).GetValue();
	}

	void CDatabaseValuedObject::DoGetValueFast( int24_t & value ) const
	{
		assert( GetType() == EFieldType_SINT24 || GetType() == EFieldType_UINT24 );
		value = static_cast< CDatabaseValue< EFieldType_SINT24 > & >( *_value ).GetValue();
	}

	void CDatabaseValuedObject::DoGetValueFast( uint24_t & value ) const
	{
		assert( GetType() == EFieldType_SINT24 || GetType() == EFieldType_UINT24 );
		value = static_cast< CDatabaseValue< EFieldType_UINT24 > & >( *_value ).GetValue();
	}

	void CDatabaseValuedObject::DoGetValueFast( int32_t & value ) const
	{
		assert( GetType() == EFieldType_SINT32 || GetType() == EFieldType_UINT32 );
		value = static_cast< CDatabaseValue< EFieldType_SINT32 > & >( *_value ).GetValue();
	}

	void CDatabaseValuedObject::DoGetValueFast( uint32_t & value ) const
	{
		assert( GetType() == EFieldType_SINT32 || GetType() == EFieldType_UINT32 );
		value = static_cast< CDatabaseValue< EFieldType_UINT32 > & >( *_value ).GetValue();
	}

	void CDatabaseValuedObject::DoGetValueFast( int64_t & value ) const
	{
		assert( GetType() == EFieldType_SINT64 || GetType() == EFieldType_UINT64 );
		value = static_cast< CDatabaseValue< EFieldType_SINT64 > & >( *_value ).GetValue();
	}

	void CDatabaseValuedObject::DoGetValueFast( uint64_t & value ) const
	{
		assert( GetType() == EFieldType_SINT64 || GetType() == EFieldType_UINT64 );
		value = static_cast< CDatabaseValue< EFieldType_UINT64 > & >( *_value ).GetValue();
	}

	void CDatabaseValuedObject::DoGetValueFast( float & value ) const
	{
		assert( GetType() == EFieldType_FLOAT32 );
		value = static_cast< CDatabaseValue< EFieldType_FLOAT32 > & >( *_value ).GetValue();
	}

	void CDatabaseValuedObject::DoGetValueFast( double & value ) const
	{
		assert( GetType() == EFieldType_FLOAT64 );
		value = static_cast< CDatabaseValue< EFieldType_FLOAT64 > & >( *_value ).GetValue();
	}

	void CDatabaseValuedObject::DoGetValueFast( CFixedPoint & value ) const
	{
		assert( GetType() == EFieldType_FIXED_POINT );
		value = static_cast< CDatabaseValue< EFieldType_FIXED_POINT > & >( *_value ).GetValue();
	}

	void CDatabaseValuedObject::DoGetValueFast( std::string & value ) const
	{
		assert( GetType() == EFieldType_TEXT || GetType() == EFieldType_VARCHAR || GetType() == EFieldType_CHAR );

		if ( _value->GetPtrValue() )
		{
			if ( GetType() == EFieldType_TEXT )
			{
				value = static_cast< CDatabaseValue< EFieldType_TEXT > & >( *_value ).GetValue();
			}
			else if ( GetType() == EFieldType_VARCHAR )
			{
				value = static_cast< CDatabaseValue< EFieldType_VARCHAR > & >( *_value ).GetValue();
			}
			else
			{
				value = static_cast< CDatabaseValue< EFieldType_CHAR > & >( *_value ).GetValue();
			}
		}
		else
		{
			value.clear();
		}
	}

	void CDatabaseValuedObject::DoGetValueFast( std::wstring & value ) const
	{
		assert( GetType() == EFieldType_NTEXT || GetType() == EFieldType_NVARCHAR || GetType() == EFieldType_NCHAR );

		if ( _value->GetPtrValue() )
		{
			if ( GetType() == EFieldType_TEXT )
			{
				value = static_cast< CDatabaseValue< EFieldType_NTEXT > & >( *_value ).GetValue();
			}
			else if ( GetType() == EFieldType_NVARCHAR )
			{
				value = static_cast< CDatabaseValue< EFieldType_NVARCHAR > & >( *_value ).GetValue();
			}
			else
			{
				value = static_cast< CDatabaseValue< EFieldType_NCHAR > & >( *_value ).GetValue();
			}
		}
		else
		{
			value.clear();
		}
	}

	void CDatabaseValuedObject::DoGetValueFast( DateType & value ) const
	{
		assert( GetType() == EFieldType_DATE );
		value = static_cast< CDatabaseValue< EFieldType_DATE > & >( *_value ).GetValue();
	}

	void CDatabaseValuedObject::DoGetValueFast( DateTimeType & value ) const
	{
		assert( GetType() == EFieldType_DATETIME );
		value = static_cast< CDatabaseValue< EFieldType_DATETIME > & >( *_value ).GetValue();
	}

	void CDatabaseValuedObject::DoGetValueFast( TimeType & value ) const
	{
		assert( GetType() == EFieldType_TIME );
		value = static_cast< CDatabaseValue< EFieldType_TIME > & >( *_value ).GetValue();
	}

	void CDatabaseValuedObject::DoGetValueFast( ByteArray & value ) const
	{
		assert( GetType() == EFieldType_BINARY || GetType() == EFieldType_VARBINARY || GetType() == EFieldType_BLOB );
		value = static_cast< CDatabaseValue< EFieldType_BINARY > & >( *_value ).GetValue();
	}

	void CDatabaseValuedObject::DoSetValue( const bool & value )
	{
		switch ( GetType() )
		{
		case EFieldType_BIT:
			static_cast< CDatabaseValue< EFieldType_BIT > & >( *_value ).SetValue( value );
			break;

		case EFieldType_SINT8:
			static_cast< CDatabaseValue< EFieldType_SINT8 > & >( *_value ).SetValue( value ? 1 : 0 );
			break;

		case EFieldType_SINT16:
			static_cast< CDatabaseValue< EFieldType_SINT16 > & >( *_value ).SetValue( value ? 1 : 0 );
			break;

		case EFieldType_SINT24:
			static_cast< CDatabaseValue< EFieldType_SINT24 > & >( *_value ).SetValue( int24_t( value ? 1 : 0 ) );
			break;

		case EFieldType_SINT32:
			static_cast< CDatabaseValue< EFieldType_SINT32 > & >( *_value ).SetValue( value ? 1 : 0 );
			break;

		case EFieldType_SINT64:
			static_cast< CDatabaseValue< EFieldType_SINT64 > & >( *_value ).SetValue( value ? 1 : 0 );
			break;

		case EFieldType_UINT8:
			static_cast< CDatabaseValue< EFieldType_UINT8 > & >( *_value ).SetValue( value ? 1 : 0 );
			break;

		case EFieldType_UINT16:
			static_cast< CDatabaseValue< EFieldType_UINT16 > & >( *_value ).SetValue( value ? 1 : 0 );
			break;

		case EFieldType_UINT24:
			static_cast< CDatabaseValue< EFieldType_UINT24 > & >( *_value ).SetValue( uint24_t( value ? 1 : 0 ) );
			break;

		case EFieldType_UINT32:
			static_cast< CDatabaseValue< EFieldType_UINT32 > & >( *_value ).SetValue( value ? 1 : 0 );
			break;

		case EFieldType_UINT64:
			static_cast< CDatabaseValue< EFieldType_UINT64 > & >( *_value ).SetValue( value ? 1 : 0 );
			break;

		case EFieldType_VARCHAR:
			static_cast< CDatabaseValue< EFieldType_VARCHAR > & >( *_value ).SetValue( value ? "1" : "0", 1 );
			break;

		case EFieldType_TEXT:
			static_cast< CDatabaseValue< EFieldType_TEXT > & >( *_value ).SetValue( value ? "1" : "0" );
			break;

		case EFieldType_NVARCHAR:
			static_cast< CDatabaseValue< EFieldType_NVARCHAR > & >( *_value ).SetValue( value ? L"1" : L"0", 1 );
			break;

		case EFieldType_NTEXT:
			static_cast< CDatabaseValue< EFieldType_NTEXT > & >( *_value ).SetValue( value ? L"1" : L"0" );
			break;

		default:
			String errMsg = ERROR_DB_FIELD_SETVALUE_TYPE + this->GetName();
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_FIELD_SETVALUE_TYPE );
			break;
		}
	}

	void CDatabaseValuedObject::DoSetValue( const int8_t & value )
	{
		switch ( GetType() )
		{
		case EFieldType_BIT:
			static_cast< CDatabaseValue< EFieldType_BIT > & >( *_value ).SetValue( value != 0 );
			break;

		case EFieldType_SINT8:
			static_cast< CDatabaseValue< EFieldType_SINT8 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_SINT16:
			static_cast< CDatabaseValue< EFieldType_SINT16 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_SINT24:
			static_cast< CDatabaseValue< EFieldType_SINT24 > & >( *_value ).SetValue( int24_t( value ) );
			break;

		case EFieldType_SINT32:
			static_cast< CDatabaseValue< EFieldType_SINT32 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_SINT64:
			static_cast< CDatabaseValue< EFieldType_SINT64 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_UINT8:
			static_cast< CDatabaseValue< EFieldType_UINT8 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_UINT16:
			static_cast< CDatabaseValue< EFieldType_UINT16 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_UINT24:
			static_cast< CDatabaseValue< EFieldType_UINT24 > & >( *_value ).SetValue( uint24_t( value ) );
			break;

		case EFieldType_UINT32:
			static_cast< CDatabaseValue< EFieldType_UINT32 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_UINT64:
			static_cast< CDatabaseValue< EFieldType_UINT64 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_FLOAT32:
			static_cast< CDatabaseValue< EFieldType_FLOAT32 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_FLOAT64:
			static_cast< CDatabaseValue< EFieldType_FLOAT64 > & >( *_value ).SetValue( value );
			break;

		default:
			String errMsg = ERROR_DB_FIELD_SETVALUE_TYPE + this->GetName();
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_FIELD_SETVALUE_TYPE );
			break;
		}
	}

	void CDatabaseValuedObject::DoSetValue( const uint8_t & value )
	{
		switch ( GetType() )
		{
		case EFieldType_BIT:
			static_cast< CDatabaseValue< EFieldType_BIT > & >( *_value ).SetValue( value != 0 );
			break;
			
		case EFieldType_SINT8:
			static_cast< CDatabaseValue< EFieldType_SINT8 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_SINT16:
			static_cast< CDatabaseValue< EFieldType_SINT16 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_SINT24:
			static_cast< CDatabaseValue< EFieldType_SINT24 > & >( *_value ).SetValue( int24_t( value ) );
			break;

		case EFieldType_SINT32:
			static_cast< CDatabaseValue< EFieldType_SINT32 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_SINT64:
			static_cast< CDatabaseValue< EFieldType_SINT64 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_UINT8:
			static_cast< CDatabaseValue< EFieldType_UINT8 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_UINT16:
			static_cast< CDatabaseValue< EFieldType_UINT16 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_UINT24:
			static_cast< CDatabaseValue< EFieldType_UINT24 > & >( *_value ).SetValue( uint24_t( value ) );
			break;

		case EFieldType_UINT32:
			static_cast< CDatabaseValue< EFieldType_UINT32 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_UINT64:
			static_cast< CDatabaseValue< EFieldType_UINT64 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_FLOAT32:
			static_cast< CDatabaseValue< EFieldType_FLOAT32 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_FLOAT64:
			static_cast< CDatabaseValue< EFieldType_FLOAT64 > & >( *_value ).SetValue( value );
			break;

		default:
			String errMsg = ERROR_DB_FIELD_SETVALUE_TYPE + this->GetName();
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_FIELD_SETVALUE_TYPE );
			break;
		}
	}

	void CDatabaseValuedObject::DoSetValue( const int16_t & value )
	{
		switch ( GetType() )
		{
		case EFieldType_BIT:
			static_cast< CDatabaseValue< EFieldType_BIT > & >( *_value ).SetValue( value != 0 );
			break;

		case EFieldType_SINT16:
			static_cast< CDatabaseValue< EFieldType_SINT16 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_SINT24:
			static_cast< CDatabaseValue< EFieldType_SINT24 > & >( *_value ).SetValue( int24_t( value ) );
			break;

		case EFieldType_SINT32:
			static_cast< CDatabaseValue< EFieldType_SINT32 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_SINT64:
			static_cast< CDatabaseValue< EFieldType_SINT64 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_UINT16:
			static_cast< CDatabaseValue< EFieldType_UINT16 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_UINT24:
			static_cast< CDatabaseValue< EFieldType_UINT24 > & >( *_value ).SetValue( uint24_t( value ) );
			break;

		case EFieldType_UINT32:
			static_cast< CDatabaseValue< EFieldType_UINT32 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_UINT64:
			static_cast< CDatabaseValue< EFieldType_UINT64 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_FLOAT32:
			static_cast< CDatabaseValue< EFieldType_FLOAT32 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_FLOAT64:
			static_cast< CDatabaseValue< EFieldType_FLOAT64 > & >( *_value ).SetValue( value );
			break;

		default:
			String errMsg = ERROR_DB_FIELD_SETVALUE_TYPE + this->GetName();
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_FIELD_SETVALUE_TYPE );
			break;
		}
	}

	void CDatabaseValuedObject::DoSetValue( const uint16_t & value )
	{
		switch ( GetType() )
		{
		case EFieldType_BIT:
			static_cast< CDatabaseValue< EFieldType_BIT > & >( *_value ).SetValue( value != 0 );
			break;

		case EFieldType_SINT16:
			static_cast< CDatabaseValue< EFieldType_SINT16 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_SINT24:
			static_cast< CDatabaseValue< EFieldType_SINT24 > & >( *_value ).SetValue( int24_t( value ) );
			break;

		case EFieldType_SINT32:
			static_cast< CDatabaseValue< EFieldType_SINT32 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_SINT64:
			static_cast< CDatabaseValue< EFieldType_SINT64 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_UINT16:
			static_cast< CDatabaseValue< EFieldType_UINT16 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_UINT24:
			static_cast< CDatabaseValue< EFieldType_UINT24 > & >( *_value ).SetValue( uint24_t( value ) );
			break;

		case EFieldType_UINT32:
			static_cast< CDatabaseValue< EFieldType_UINT32 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_UINT64:
			static_cast< CDatabaseValue< EFieldType_UINT64 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_FLOAT32:
			static_cast< CDatabaseValue< EFieldType_FLOAT32 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_FLOAT64:
			static_cast< CDatabaseValue< EFieldType_FLOAT64 > & >( *_value ).SetValue( value );
			break;

		default:
			String errMsg = ERROR_DB_FIELD_SETVALUE_TYPE + this->GetName();
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_FIELD_SETVALUE_TYPE );
			break;
		}
	}

	void CDatabaseValuedObject::DoSetValue( const int24_t & value )
	{
		switch ( GetType() )
		{
		case EFieldType_BIT:
			static_cast< CDatabaseValue< EFieldType_BIT > & >( *_value ).SetValue( value != 0 );
			break;

		case EFieldType_SINT24:
			static_cast< CDatabaseValue< EFieldType_SINT24 > & >( *_value ).SetValue( int24_t( value ) );
			break;

		case EFieldType_SINT32:
			static_cast< CDatabaseValue< EFieldType_SINT32 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_SINT64:
			static_cast< CDatabaseValue< EFieldType_SINT64 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_UINT24:
			static_cast< CDatabaseValue< EFieldType_UINT24 > & >( *_value ).SetValue( uint24_t( value ) );
			break;

		case EFieldType_UINT32:
			static_cast< CDatabaseValue< EFieldType_UINT32 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_UINT64:
			static_cast< CDatabaseValue< EFieldType_UINT64 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_FLOAT32:
			static_cast< CDatabaseValue< EFieldType_FLOAT32 > & >( *_value ).SetValue( float( value ) );
			break;

		case EFieldType_FLOAT64:
			static_cast< CDatabaseValue< EFieldType_FLOAT64 > & >( *_value ).SetValue( double( value ) );
			break;

		default:
			String errMsg = ERROR_DB_FIELD_SETVALUE_TYPE + this->GetName();
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_FIELD_SETVALUE_TYPE );
			break;
		}
	}

	void CDatabaseValuedObject::DoSetValue( const uint24_t & value )
	{
		switch ( GetType() )
		{
		case EFieldType_BIT:
			static_cast< CDatabaseValue< EFieldType_BIT > & >( *_value ).SetValue( value != 0 );
			break;

		case EFieldType_SINT24:
			static_cast< CDatabaseValue< EFieldType_SINT24 > & >( *_value ).SetValue( int24_t( value ) );
			break;

		case EFieldType_SINT32:
			static_cast< CDatabaseValue< EFieldType_SINT32 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_SINT64:
			static_cast< CDatabaseValue< EFieldType_SINT64 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_UINT24:
			static_cast< CDatabaseValue< EFieldType_UINT24 > & >( *_value ).SetValue( uint24_t( value ) );
			break;

		case EFieldType_UINT32:
			static_cast< CDatabaseValue< EFieldType_UINT32 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_UINT64:
			static_cast< CDatabaseValue< EFieldType_UINT64 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_FLOAT32:
			static_cast< CDatabaseValue< EFieldType_FLOAT32 > & >( *_value ).SetValue( float( value ) );
			break;

		case EFieldType_FLOAT64:
			static_cast< CDatabaseValue< EFieldType_FLOAT64 > & >( *_value ).SetValue( double( value ) );
			break;

		default:
			String errMsg = ERROR_DB_FIELD_SETVALUE_TYPE + this->GetName();
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_FIELD_SETVALUE_TYPE );
			break;
		}
	}

	void CDatabaseValuedObject::DoSetValue( const int32_t & value )
	{
		switch ( GetType() )
		{
		case EFieldType_BIT:
			static_cast< CDatabaseValue< EFieldType_BIT > & >( *_value ).SetValue( value != 0 );
			break;

		case EFieldType_SINT32:
			static_cast< CDatabaseValue< EFieldType_SINT32 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_SINT64:
			static_cast< CDatabaseValue< EFieldType_SINT64 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_UINT32:
			static_cast< CDatabaseValue< EFieldType_UINT32 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_UINT64:
			static_cast< CDatabaseValue< EFieldType_UINT64 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_FLOAT32:
			static_cast< CDatabaseValue< EFieldType_FLOAT32 > & >( *_value ).SetValue( float( value ) );
			break;

		case EFieldType_FLOAT64:
			static_cast< CDatabaseValue< EFieldType_FLOAT64 > & >( *_value ).SetValue( double( value ) );
			break;

		default:
			String errMsg = ERROR_DB_FIELD_SETVALUE_TYPE + this->GetName();
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_FIELD_SETVALUE_TYPE );
			break;
		}
	}

	void CDatabaseValuedObject::DoSetValue( const uint32_t & value )
	{
		switch ( GetType() )
		{
		case EFieldType_BIT:
			static_cast< CDatabaseValue< EFieldType_BIT > & >( *_value ).SetValue( value != 0 );
			break;

		case EFieldType_SINT32:
			static_cast< CDatabaseValue< EFieldType_SINT32 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_SINT64:
			static_cast< CDatabaseValue< EFieldType_SINT64 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_UINT32:
			static_cast< CDatabaseValue< EFieldType_UINT32 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_UINT64:
			static_cast< CDatabaseValue< EFieldType_UINT64 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_FLOAT32:
			static_cast< CDatabaseValue< EFieldType_FLOAT32 > & >( *_value ).SetValue( float( value ) );
			break;

		case EFieldType_FLOAT64:
			static_cast< CDatabaseValue< EFieldType_FLOAT64 > & >( *_value ).SetValue( double( value ) );
			break;

		default:
			String errMsg = ERROR_DB_FIELD_SETVALUE_TYPE + this->GetName();
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_FIELD_SETVALUE_TYPE );
			break;
		}
	}

	void CDatabaseValuedObject::DoSetValue( const int64_t & value )
	{
		switch ( GetType() )
		{
		case EFieldType_BIT:
			static_cast< CDatabaseValue< EFieldType_BIT > & >( *_value ).SetValue( value != 0 );
			break;

		case EFieldType_SINT64:
			static_cast< CDatabaseValue< EFieldType_SINT64 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_UINT64:
			static_cast< CDatabaseValue< EFieldType_UINT64 > & >( *_value ).SetValue( value );
			break;

		default:
			String errMsg = ERROR_DB_FIELD_SETVALUE_TYPE + this->GetName();
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_FIELD_SETVALUE_TYPE );
			break;
		}
	}

	void CDatabaseValuedObject::DoSetValue( const uint64_t & value )
	{
		switch ( GetType() )
		{
		case EFieldType_BIT:
			static_cast< CDatabaseValue< EFieldType_BIT > & >( *_value ).SetValue( value != 0 );
			break;

		case EFieldType_SINT64:
			static_cast< CDatabaseValue< EFieldType_SINT64 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_UINT64:
			static_cast< CDatabaseValue< EFieldType_UINT64 > & >( *_value ).SetValue( value );
			break;

		default:
			String errMsg = ERROR_DB_FIELD_SETVALUE_TYPE + this->GetName();
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_FIELD_SETVALUE_TYPE );
			break;
		}
	}

	void CDatabaseValuedObject::DoSetValue( const float & value )
	{
		switch ( GetType() )
		{
		case EFieldType_SINT16:
			static_cast< CDatabaseValue< EFieldType_SINT16 > & >( *_value ).SetValue( int16_t( value ) );
			break;

		case EFieldType_SINT24:
			static_cast< CDatabaseValue< EFieldType_SINT24 > & >( *_value ).SetValue( int24_t( value ) );
			break;

		case EFieldType_SINT32:
			static_cast< CDatabaseValue< EFieldType_SINT32 > & >( *_value ).SetValue( int32_t( value ) );
			break;

		case EFieldType_SINT64:
			static_cast< CDatabaseValue< EFieldType_SINT64 > & >( *_value ).SetValue( int64_t( value ) );
			break;

		case EFieldType_UINT16:
			static_cast< CDatabaseValue< EFieldType_UINT16 > & >( *_value ).SetValue( uint16_t( value ) );
			break;

		case EFieldType_UINT24:
			static_cast< CDatabaseValue< EFieldType_UINT24 > & >( *_value ).SetValue( uint24_t( value ) );
			break;

		case EFieldType_UINT32:
			static_cast< CDatabaseValue< EFieldType_UINT32 > & >( *_value ).SetValue( uint32_t( value ) );
			break;

		case EFieldType_UINT64:
			static_cast< CDatabaseValue< EFieldType_UINT64 > & >( *_value ).SetValue( uint64_t( value ) );
			break;

		case EFieldType_FLOAT32:
			static_cast< CDatabaseValue< EFieldType_FLOAT32 > & >( *_value ).SetValue( value );
			break;

		case EFieldType_FLOAT64:
			static_cast< CDatabaseValue< EFieldType_FLOAT64 > & >( *_value ).SetValue( value );
			break;

		default:
			String errMsg = ERROR_DB_FIELD_SETVALUE_TYPE + this->GetName();
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_FIELD_SETVALUE_TYPE );
			break;
		}
	}

	void CDatabaseValuedObject::DoSetValue( const double & value )
	{
		switch ( GetType() )
		{
		case EFieldType_SINT16:
			static_cast< CDatabaseValue< EFieldType_SINT16 > & >( *_value ).SetValue( int16_t( value ) );
			break;

		case EFieldType_SINT24:
			static_cast< CDatabaseValue< EFieldType_SINT24 > & >( *_value ).SetValue( int24_t( value ) );
			break;

		case EFieldType_SINT32:
			static_cast< CDatabaseValue< EFieldType_SINT32 > & >( *_value ).SetValue( int32_t( value ) );
			break;

		case EFieldType_SINT64:
			static_cast< CDatabaseValue< EFieldType_SINT64 > & >( *_value ).SetValue( int64_t( value ) );
			break;

		case EFieldType_UINT16:
			static_cast< CDatabaseValue< EFieldType_UINT16 > & >( *_value ).SetValue( uint16_t( value ) );
			break;

		case EFieldType_UINT24:
			static_cast< CDatabaseValue< EFieldType_UINT24 > & >( *_value ).SetValue( uint24_t( value ) );
			break;

		case EFieldType_UINT32:
			static_cast< CDatabaseValue< EFieldType_UINT32 > & >( *_value ).SetValue( uint32_t( value ) );
			break;

		case EFieldType_UINT64:
			static_cast< CDatabaseValue< EFieldType_UINT64 > & >( *_value ).SetValue( uint64_t( value ) );
			break;

		case EFieldType_FLOAT32:
			static_cast< CDatabaseValue< EFieldType_FLOAT32 > & >( *_value ).SetValue( float( value ) );
			break;

		case EFieldType_FLOAT64:
			static_cast< CDatabaseValue< EFieldType_FLOAT64 > & >( *_value ).SetValue( value );
			break;

		default:
			String errMsg = ERROR_DB_FIELD_SETVALUE_TYPE + this->GetName();
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_FIELD_SETVALUE_TYPE );
			break;
		}
	}

	void CDatabaseValuedObject::DoSetValue( const CFixedPoint & value )
	{
		switch ( GetType() )
		{
		case EFieldType_FLOAT32:
			static_cast< CDatabaseValue< EFieldType_FLOAT32 > & >( *_value ).SetValue( value.ToFloat() );
			break;

		case EFieldType_FLOAT64:
			static_cast< CDatabaseValue< EFieldType_FLOAT64 > & >( *_value ).SetValue( value.ToDouble() );
			break;

		case EFieldType_FIXED_POINT:
			static_cast< CDatabaseValue< EFieldType_FIXED_POINT > & >( *_value ).SetValue( value );
			break;

		default:
			String errMsg = ERROR_DB_FIELD_SETVALUE_TYPE + this->GetName();
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_FIELD_SETVALUE_TYPE );
			break;
		}
	}

	void CDatabaseValuedObject::DoSetValue( const std::string & value )
	{
		switch ( GetType() )
		{
		case EFieldType_CHAR:
			static_cast< CDatabaseValue< EFieldType_CHAR > & >( *_value ).SetValue( value.c_str(), GetLimits() );
			break;

		case EFieldType_VARCHAR:
			static_cast< CDatabaseValue< EFieldType_VARCHAR > & >( *_value ).SetValue( value.c_str(), std::min( GetLimits(), uint32_t( value.size() ) ) );
			break;

		case EFieldType_TEXT:
			static_cast< CDatabaseValue< EFieldType_TEXT > & >( *_value ).SetValue( value );
			break;

		case EFieldType_NCHAR:
			static_cast< CDatabaseValue< EFieldType_NVARCHAR > & >( *_value ).SetValue( StringUtils::ToWStr( value ).c_str(), GetLimits() );
			break;

		case EFieldType_NVARCHAR:
			static_cast< CDatabaseValue< EFieldType_NVARCHAR > & >( *_value ).SetValue( StringUtils::ToWStr( value ).c_str(), std::min( GetLimits(), uint32_t( value.size() ) ) );
			break;

		case EFieldType_NTEXT:
			static_cast< CDatabaseValue< EFieldType_NTEXT > & >( *_value ).SetValue( StringUtils::ToWStr( value ) );
			break;

		default:
			String errMsg = ERROR_DB_FIELD_SETVALUE_TYPE + this->GetName();
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_FIELD_SETVALUE_TYPE );
			break;
		}
	}

	void CDatabaseValuedObject::DoSetValue( const std::wstring & value )
	{
		switch ( GetType() )
		{
		case EFieldType_CHAR:
			static_cast< CDatabaseValue< EFieldType_CHAR > & >( *_value ).SetValue( StringUtils::ToStr( value ).c_str(), GetLimits() );
			break;

		case EFieldType_VARCHAR:
			static_cast< CDatabaseValue< EFieldType_VARCHAR > & >( *_value ).SetValue( StringUtils::ToStr( value ).c_str(), std::min( GetLimits(), uint32_t( value.size() ) ) );
			break;

		case EFieldType_TEXT:
			static_cast< CDatabaseValue< EFieldType_TEXT > & >( *_value ).SetValue( StringUtils::ToStr( value ) );
			break;

		case EFieldType_NCHAR:
			static_cast< CDatabaseValue< EFieldType_NCHAR > & >( *_value ).SetValue( value.c_str(), GetLimits() );
			break;

		case EFieldType_NVARCHAR:
			static_cast< CDatabaseValue< EFieldType_NVARCHAR > & >( *_value ).SetValue( value.c_str(), std::min( GetLimits(), uint32_t( value.size() ) ) );
			break;

		case EFieldType_NTEXT:
			static_cast< CDatabaseValue< EFieldType_NTEXT > & >( *_value ).SetValue( value );
			break;

		default:
			String errMsg = ERROR_DB_FIELD_SETVALUE_TYPE + this->GetName();
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_FIELD_SETVALUE_TYPE );
			break;
		}
	}

	void CDatabaseValuedObject::DoSetValue( const DateType & value )
	{
		switch ( GetType() )
		{
		case EFieldType_DATE:
			static_cast< CDatabaseValue< EFieldType_DATE > & >( *_value ).SetValue( value );
			break;

		case EFieldType_DATETIME:
			static_cast< CDatabaseValue< EFieldType_DATETIME > & >( *_value ).SetValue( DateTimeType( value ) );
			break;

		default:
			String errMsg = ERROR_DB_FIELD_SETVALUE_TYPE + this->GetName();
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_FIELD_SETVALUE_TYPE );
			break;
		}
	}

	void CDatabaseValuedObject::DoSetValue( const DateTimeType & value )
	{
		switch ( GetType() )
		{
		case EFieldType_DATE:
			static_cast< CDatabaseValue< EFieldType_DATE > & >( *_value ).SetValue( DateFromDateTime( value ) );
			break;

		case EFieldType_DATETIME:
			static_cast< CDatabaseValue< EFieldType_DATETIME > & >( *_value ).SetValue( value );
			break;

		case EFieldType_TIME:
			static_cast< CDatabaseValue< EFieldType_TIME > & >( *_value ).SetValue( TimeFromDateTime( value ) );
			break;

		default:
			String errMsg = ERROR_DB_FIELD_SETVALUE_TYPE + this->GetName();
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_FIELD_SETVALUE_TYPE );
			break;
		}
	}

	void CDatabaseValuedObject::DoSetValue( const TimeType & value )
	{
		switch ( GetType() )
		{
		case EFieldType_DATETIME:
			static_cast< CDatabaseValue< EFieldType_DATETIME > & >( *_value ).SetValue( DateTimeType( DateType( boost::gregorian::min_date_time ), value ) );
			break;

		case EFieldType_TIME:
			static_cast< CDatabaseValue< EFieldType_TIME > & >( *_value ).SetValue( value );
			break;

		default:
			String errMsg = ERROR_DB_FIELD_SETVALUE_TYPE + this->GetName();
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_FIELD_SETVALUE_TYPE );
			break;
		}
	}

	void CDatabaseValuedObject::DoSetValue( const ByteArray & value )
	{
		switch ( GetType() )
		{
		case EFieldType_BINARY:
			static_cast< CDatabaseValue< EFieldType_BINARY > & >( *_value ).SetValue( value.data(), std::min( GetLimits(), uint32_t( value.size() ) ) );
			break;

		case EFieldType_VARBINARY:
			static_cast< CDatabaseValue< EFieldType_VARBINARY > & >( *_value ).SetValue( value.data(), std::min( GetLimits(), uint32_t( value.size() ) ) );
			break;

		case EFieldType_BLOB:
			static_cast< CDatabaseValue< EFieldType_BLOB > & >( *_value ).SetValue( value.data(), std::min( GetLimits(), uint32_t( value.size() ) ) );
			break;

		default:
			String errMsg = ERROR_DB_FIELD_SETVALUE_TYPE + this->GetName();
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_FIELD_SETVALUE_TYPE );
			break;
		}
	}

	void CDatabaseValuedObject::DoSetValueFast( const bool & value )
	{
		assert( GetType() == EFieldType_BIT );
		static_cast< CDatabaseValue< EFieldType_BIT > & >( *_value ).SetValue( value );
	}

	void CDatabaseValuedObject::DoSetValueFast( const int8_t & value )
	{
		assert( GetType() == EFieldType_SINT8 || GetType() == EFieldType_UINT8 );
		static_cast< CDatabaseValue< EFieldType_SINT8 > & >( *_value ).SetValue( value );
	}

	void CDatabaseValuedObject::DoSetValueFast( const uint8_t & value )
	{
		assert( GetType() == EFieldType_SINT8 || GetType() == EFieldType_UINT8 );
		static_cast< CDatabaseValue< EFieldType_UINT8 > & >( *_value ).SetValue( value );
	}

	void CDatabaseValuedObject::DoSetValueFast( const int16_t & value )
	{
		assert( GetType() == EFieldType_SINT16 || GetType() == EFieldType_UINT16 );
		static_cast< CDatabaseValue< EFieldType_SINT16 > & >( *_value ).SetValue( value );
	}

	void CDatabaseValuedObject::DoSetValueFast( const uint16_t & value )
	{
		assert( GetType() == EFieldType_SINT16 || GetType() == EFieldType_UINT16 );
		static_cast< CDatabaseValue< EFieldType_UINT16 > & >( *_value ).SetValue( value );
	}

	void CDatabaseValuedObject::DoSetValueFast( const int24_t & value )
	{
		assert( GetType() == EFieldType_SINT24 || GetType() == EFieldType_UINT24 );
		static_cast< CDatabaseValue< EFieldType_SINT24 > & >( *_value ).SetValue( value );
	}

	void CDatabaseValuedObject::DoSetValueFast( const uint24_t & value )
	{
		assert( GetType() == EFieldType_SINT24 || GetType() == EFieldType_UINT24 );
		static_cast< CDatabaseValue< EFieldType_UINT24 > & >( *_value ).SetValue( value );
	}

	void CDatabaseValuedObject::DoSetValueFast( const int32_t & value )
	{
		assert( GetType() == EFieldType_SINT32 || GetType() == EFieldType_UINT32 );
		static_cast< CDatabaseValue< EFieldType_SINT32 > & >( *_value ).SetValue( value );
	}

	void CDatabaseValuedObject::DoSetValueFast( const uint32_t & value )
	{
		assert( GetType() == EFieldType_SINT32 || GetType() == EFieldType_UINT32 );
		static_cast< CDatabaseValue< EFieldType_UINT32 > & >( *_value ).SetValue( value );
	}

	void CDatabaseValuedObject::DoSetValueFast( const int64_t & value )
	{
		assert( GetType() == EFieldType_SINT64 || GetType() == EFieldType_UINT64 );
		static_cast< CDatabaseValue< EFieldType_SINT64 > & >( *_value ).SetValue( value );
	}

	void CDatabaseValuedObject::DoSetValueFast( const uint64_t & value )
	{
		assert( GetType() == EFieldType_SINT64 || GetType() == EFieldType_UINT64 );
		static_cast< CDatabaseValue< EFieldType_UINT64 > & >( *_value ).SetValue( value );
	}

	void CDatabaseValuedObject::DoSetValueFast( const float & value )
	{
		assert( GetType() == EFieldType_FLOAT32 );
		static_cast< CDatabaseValue< EFieldType_FLOAT32 > & >( *_value ).SetValue( value );
	}

	void CDatabaseValuedObject::DoSetValueFast( const double & value )
	{
		assert( GetType() == EFieldType_FLOAT64 );
		static_cast< CDatabaseValue< EFieldType_FLOAT64 > & >( *_value ).SetValue( value );
	}

	void CDatabaseValuedObject::DoSetValueFast( const CFixedPoint & value )
	{
		assert( GetType() == EFieldType_FIXED_POINT );
		static_cast< CDatabaseValue< EFieldType_FIXED_POINT > & >( *_value ).SetValue( value );
	}

	void CDatabaseValuedObject::DoSetValueFast( const std::string & value )
	{
		assert( GetType() == EFieldType_TEXT || GetType() == EFieldType_VARCHAR || GetType() == EFieldType_CHAR );

		if ( GetType() == EFieldType_CHAR )
		{
			static_cast< CDatabaseValue< EFieldType_CHAR > & >( *_value ).SetValue( value.c_str(), GetLimits() );
		}
		else
		{
			static_cast< CDatabaseValue< EFieldType_TEXT > & >( *_value ).SetValue( value.c_str(), std::min( GetLimits(), uint32_t( value.size() ) ) );
		}
	}

	void CDatabaseValuedObject::DoSetValueFast( const std::wstring & value )
	{
		assert( GetType() == EFieldType_NTEXT || GetType() == EFieldType_NVARCHAR || GetType() == EFieldType_NCHAR );

		if ( GetType() == EFieldType_NCHAR )
		{
			static_cast< CDatabaseValue< EFieldType_NCHAR > & >( *_value ).SetValue( value.c_str(), GetLimits() );
		}
		else
		{
			static_cast< CDatabaseValue< EFieldType_NTEXT > & >( *_value ).SetValue( value.c_str(), std::min( GetLimits(), uint32_t( value.size() ) ) );
		}
	}

	void CDatabaseValuedObject::DoSetValueFast( const DateType & value )
	{
		assert( GetType() == EFieldType_DATE );
		static_cast< CDatabaseValue< EFieldType_DATE > & >( *_value ).SetValue( value );
	}

	void CDatabaseValuedObject::DoSetValueFast( const DateTimeType & value )
	{
		assert( GetType() == EFieldType_DATETIME );
		static_cast< CDatabaseValue< EFieldType_DATETIME > & >( *_value ).SetValue( value );
	}

	void CDatabaseValuedObject::DoSetValueFast( const TimeType & value )
	{
		assert( GetType() == EFieldType_TIME );
		static_cast< CDatabaseValue< EFieldType_TIME > & >( *_value ).SetValue( value );
	}

	void CDatabaseValuedObject::DoSetValueFast( const ByteArray & value )
	{
		assert( GetType() == EFieldType_BINARY || GetType() == EFieldType_VARBINARY || GetType() == EFieldType_BLOB );
		static_cast< CDatabaseValue< EFieldType_BINARY > & >( *_value ).SetValue( value.data(), std::min( GetLimits(), uint32_t( value.size() ) ) );
	}

	void CDatabaseValuedObject::DoCreateValue()
	{
		switch ( GetType() )
		{
		case EFieldType_BIT:
			_value = std::make_unique< CDatabaseValue< EFieldType_BIT > >( GetConnection() );
			break;

		case EFieldType_SINT8:
			_value = std::make_unique< CDatabaseValue< EFieldType_SINT8 > >( GetConnection() );
			break;

		case EFieldType_SINT16:
			_value = std::make_unique< CDatabaseValue< EFieldType_SINT16 > >( GetConnection() );
			break;

		case EFieldType_SINT24:
			_value = std::make_unique< CDatabaseValue< EFieldType_SINT24 > >( GetConnection() );
			break;

		case EFieldType_SINT32:
			_value = std::make_unique< CDatabaseValue< EFieldType_SINT32 > >( GetConnection() );
			break;

		case EFieldType_SINT64:
			_value = std::make_unique< CDatabaseValue< EFieldType_SINT64 > >( GetConnection() );
			break;

		case EFieldType_UINT8:
			_value = std::make_unique< CDatabaseValue< EFieldType_UINT8 > >( GetConnection() );
			break;

		case EFieldType_UINT16:
			_value = std::make_unique< CDatabaseValue< EFieldType_UINT16 > >( GetConnection() );
			break;

		case EFieldType_UINT24:
			_value = std::make_unique< CDatabaseValue< EFieldType_UINT24 > >( GetConnection() );
			break;

		case EFieldType_UINT32:
			_value = std::make_unique< CDatabaseValue< EFieldType_UINT32 > >( GetConnection() );
			break;

		case EFieldType_UINT64:
			_value = std::make_unique< CDatabaseValue< EFieldType_UINT64 > >( GetConnection() );
			break;

		case EFieldType_FLOAT32:
			_value = std::make_unique< CDatabaseValue< EFieldType_FLOAT32 > >( GetConnection() );
			break;

		case EFieldType_FLOAT64:
			_value = std::make_unique< CDatabaseValue< EFieldType_FLOAT64 > >( GetConnection() );
			break;

		case EFieldType_FIXED_POINT:
			_value = std::make_unique< CDatabaseValue< EFieldType_FIXED_POINT > >( GetConnection() );
			break;

		case EFieldType_CHAR:
			_value = std::make_unique< CDatabaseValue< EFieldType_CHAR > >( GetConnection() );
			break;

		case EFieldType_VARCHAR:
			_value = std::make_unique< CDatabaseValue< EFieldType_VARCHAR > >( GetConnection() );
			break;

		case EFieldType_TEXT:
			_value = std::make_unique< CDatabaseValue< EFieldType_TEXT > >( GetConnection() );
			break;

		case EFieldType_NCHAR:
			_value = std::make_unique< CDatabaseValue< EFieldType_NCHAR > >( GetConnection() );
			break;

		case EFieldType_NVARCHAR:
			_value = std::make_unique< CDatabaseValue< EFieldType_NVARCHAR > >( GetConnection() );
			break;

		case EFieldType_NTEXT:
			_value = std::make_unique< CDatabaseValue< EFieldType_NTEXT > >( GetConnection() );
			break;

		case EFieldType_DATE:
			_value = std::make_unique< CDatabaseValue< EFieldType_DATE > >( GetConnection() );
			break;

		case EFieldType_DATETIME:
			_value = std::make_unique< CDatabaseValue< EFieldType_DATETIME > >( GetConnection() );
			break;

		case EFieldType_TIME:
			_value = std::make_unique< CDatabaseValue< EFieldType_TIME > >( GetConnection() );
			break;

		case EFieldType_BINARY:
			_value = std::make_unique< CDatabaseValue< EFieldType_BINARY > >( GetConnection() );
			break;

		case EFieldType_VARBINARY:
			_value = std::make_unique< CDatabaseValue< EFieldType_VARBINARY > >( GetConnection() );
			break;

		case EFieldType_BLOB:
			_value = std::make_unique< CDatabaseValue< EFieldType_BLOB > >( GetConnection() );
			break;

		default:
			String errMsg = ERROR_DB_FIELD_CREATION_TYPE + this->GetName();
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_FIELD_CREATION_TYPE );
			break;
		}
	}
}
END_NAMESPACE_DATABASE
