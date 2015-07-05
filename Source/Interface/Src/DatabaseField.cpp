/************************************************************************//**
 * @file DatabaseField.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 *
 *
 * @brief CDatabaseField class definition.
 *
 * @details Describes a database field.
 *
 ***************************************************************************/

#include "DatabasePch.h"

#include "DatabaseField.h"

#include "Database.h"
#include "DatabaseConnection.h"
#include "DatabaseField.h"
#include "DatabaseFieldInfos.h"
#include "DatabaseValue.h"
#include "DatabaseException.h"
#include "EDateMonth.h"

BEGIN_NAMESPACE_DATABASE
{
	static const String DATABASE_FIELD_CREATION_TYPE_ERROR = STR( "Type error while creating the field : " );
	static const String DATABASE_FIELD_GETVALUE_TYPE_ERROR = STR( "Type error while getting value from the field : " );
	static const String DATABASE_FIELD_SETVALUE_TYPE_ERROR = STR( "Type error while setting value to the field : " );

	CDatabaseField::CDatabaseField( DatabaseFieldInfosPtr infos, bool isNull, const String & value )
		:   _infos( infos )
		,   _isNull( isNull )
	{
		switch ( _infos->GetType() )
		{
		case EFieldType_BOOL:
			_value = new CDatabaseValue< EFieldType_BOOL >( _infos->GetConnection() );
			break;

		case EFieldType_SMALL_INTEGER:
			_value = new CDatabaseValue< EFieldType_SMALL_INTEGER >( _infos->GetConnection() );
			break;

		case EFieldType_INTEGER:
			_value = new CDatabaseValue< EFieldType_INTEGER >( _infos->GetConnection() );
			break;

		case EFieldType_LONG_INTEGER:
			_value = new CDatabaseValue< EFieldType_LONG_INTEGER >( _infos->GetConnection() );
			break;

		case EFieldType_FLOAT:
			_value = new CDatabaseValue< EFieldType_FLOAT >( _infos->GetConnection() );
			break;

		case EFieldType_DOUBLE:
			_value = new CDatabaseValue< EFieldType_DOUBLE >( _infos->GetConnection() );
			break;

		case EFieldType_VARCHAR:
			_value = new CDatabaseValue< EFieldType_VARCHAR >( _infos->GetConnection() );
			break;

		case EFieldType_TEXT:
			_value = new CDatabaseValue< EFieldType_TEXT >( _infos->GetConnection() );
			break;

		case EFieldType_NVARCHAR:
			_value = new CDatabaseValue< EFieldType_NVARCHAR >( _infos->GetConnection() );
			break;

		case EFieldType_NTEXT:
			_value = new CDatabaseValue< EFieldType_NTEXT >( _infos->GetConnection() );
			break;

		case EFieldType_DATE:
			_value = new CDatabaseValue< EFieldType_DATE >( _infos->GetConnection() );
			break;

		case EFieldType_DATETIME:
			_value = new CDatabaseValue< EFieldType_DATETIME >( _infos->GetConnection() );
			break;

		case EFieldType_TIME:
			_value = new CDatabaseValue< EFieldType_TIME >( _infos->GetConnection() );
			break;

		case EFieldType_BINARY:
			_value = new CDatabaseValue< EFieldType_BINARY >( _infos->GetConnection() );
			break;

		case EFieldType_VARBINARY:
			_value = new CDatabaseValue< EFieldType_VARBINARY >( _infos->GetConnection() );
			break;

		case EFieldType_LONG_VARBINARY:
			_value = new CDatabaseValue< EFieldType_LONG_VARBINARY >( _infos->GetConnection() );
			break;

		default:
			String errMsg = DATABASE_FIELD_CREATION_TYPE_ERROR + this->GetName();
			CLogger::LogError( errMsg );
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, DATABASE_FIELD_CREATION_TYPE_ERROR );
			break;
		}

		if ( !_isNull )
		{
			SetStrValue( value );
		}
	}

	CDatabaseField::CDatabaseField( DatabaseFieldInfosPtr infos, uint8_t * begin, uint8_t * end )
		:   _infos( infos )
		,   _isNull( begin == end )
	{
		switch ( _infos->GetType() )
		{
		case EFieldType_BINARY:
			_value = new CDatabaseValue< EFieldType_BINARY >( _infos->GetConnection() );
			break;

		case EFieldType_VARBINARY:
			_value = new CDatabaseValue< EFieldType_VARBINARY >( _infos->GetConnection() );
			break;

		case EFieldType_LONG_VARBINARY:
			_value = new CDatabaseValue< EFieldType_LONG_VARBINARY >( _infos->GetConnection() );
			break;

		default:
			String errMsg = DATABASE_FIELD_CREATION_TYPE_ERROR + this->GetName();
			CLogger::LogError( errMsg );
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, DATABASE_FIELD_CREATION_TYPE_ERROR );
			break;
		}

		if ( !_isNull )
		{
			std::vector< uint8_t > value;
			value.insert( value.end(), begin, end );
			SetValue( value );
		}
	}

	CDatabaseField::~CDatabaseField()
	{
		delete _value;
	}

	void CDatabaseField::SetStrValue( const String & value )
	{
		if ( value.empty() )
		{
			switch ( _infos->GetType() )
			{
			case EFieldType_BOOL:
			case EFieldType_SMALL_INTEGER:
			case EFieldType_INTEGER:
			case EFieldType_LONG_INTEGER:
			case EFieldType_FLOAT:
			case EFieldType_DOUBLE:
			case EFieldType_DATE:
			case EFieldType_DATETIME:
			case EFieldType_TIME:
			case EFieldType_BINARY:
			case EFieldType_VARBINARY:
			case EFieldType_LONG_VARBINARY:
				_isNull = true;
				_value->Reset();
				break;

			case EFieldType_VARCHAR:
			case EFieldType_TEXT:
			case EFieldType_NVARCHAR:
			case EFieldType_NTEXT:
				_isNull = false;
				_value->Reset();
				break;

			default:
				String errMsg = DATABASE_FIELD_SETVALUE_TYPE_ERROR + this->GetName();
				CLogger::LogError( errMsg );
				DB_EXCEPT( EDatabaseExceptionCodes_FieldError, DATABASE_FIELD_SETVALUE_TYPE_ERROR );
				break;
			}
		}
		else
		{
			_isNull = false;
			_value->SetStrValue( value );
		}
	}

	void CDatabaseField::SetValue( DatabaseFieldPtr field )
	{
		if ( !AreTypesCompatible( GetType(), field->GetType() ) )
		{
			String errMsg = DATABASE_FIELD_SETVALUE_TYPE_ERROR + this->GetName();
			CLogger::LogError( errMsg );
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, DATABASE_FIELD_SETVALUE_TYPE_ERROR );
		}

		_value->SetValue( field->GetFieldValue() );
	}

	void CDatabaseField::SetNull()
	{
		_value->Reset();
	}

	EFieldType CDatabaseField::GetType() const
	{
		return _infos->GetType();
	}

	const String & CDatabaseField::GetName() const
	{
		return _infos->GetName();
	}

	const uint32_t & CDatabaseField::GetLimits() const
	{
		return _infos->GetLimits();
	}

	String CDatabaseField::GetStrValue() const
	{
		return _value->GetStrValue();
	}

	bool CDatabaseField::IsNull() const
	{
		return _isNull;
	}

	void CDatabaseField::DoGetValue( bool & value ) const
	{
		switch ( GetType() )
		{
		case EFieldType_BOOL:
			value = *reinterpret_cast< bool * >( _value->GetPtrValue() );
			break;

		case EFieldType_SMALL_INTEGER:
			value = ( *reinterpret_cast< short * >( _value->GetPtrValue() ) ) != 0;
			break;

		case EFieldType_INTEGER:
			value = ( *reinterpret_cast< int * >( _value->GetPtrValue() ) ) != 0;
			break;

		case EFieldType_VARCHAR:
		{
			char * val = reinterpret_cast< char * >( _value->GetPtrValue() );

			if ( ( val != NULL ) && ( ( CStrUtils::UpperCase( std::string( val ) ) == "TRUE" ) || ( strcmp( val, "1" ) == 0 ) ) )
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
			wchar_t * val = reinterpret_cast< wchar_t * >( _value->GetPtrValue() );

			if ( ( val != NULL ) && ( ( CStrUtils::UpperCase( std::wstring( val ) ) == L"TRUE" ) || ( wcscmp( val, L"1" ) == 0 ) ) )
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
			String errMsg = DATABASE_FIELD_GETVALUE_TYPE_ERROR + this->GetName();
			CLogger::LogError( errMsg );
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, DATABASE_FIELD_GETVALUE_TYPE_ERROR );
			break;
		}
	}

	void CDatabaseField::DoGetValue( int16_t & value ) const
	{
		switch ( GetType() )
		{
		case EFieldType_BOOL:
			value = ( *reinterpret_cast< bool * >( _value->GetPtrValue() ) ? 1 : 0 );
			break;

		case EFieldType_SMALL_INTEGER:
			value = *reinterpret_cast< int16_t * >( _value->GetPtrValue() );
			break;

		default:
			String errMsg = DATABASE_FIELD_GETVALUE_TYPE_ERROR + this->GetName();
			CLogger::LogError( errMsg );
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, DATABASE_FIELD_GETVALUE_TYPE_ERROR );
			break;
		}
	}

	void CDatabaseField::DoGetValue( uint16_t & value ) const
	{
		switch ( GetType() )
		{
		case EFieldType_BOOL:
			value = ( *reinterpret_cast< bool * >( _value->GetPtrValue() ) ? 1 : 0 );
			break;

		case EFieldType_SMALL_INTEGER:
			value = *reinterpret_cast< uint16_t * >( _value->GetPtrValue() );
			break;

		default:
			String errMsg = DATABASE_FIELD_GETVALUE_TYPE_ERROR + this->GetName();
			CLogger::LogError( errMsg );
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, DATABASE_FIELD_GETVALUE_TYPE_ERROR );
			break;
		}
	}

	void CDatabaseField::DoGetValue( int32_t & value ) const
	{
		switch ( GetType() )
		{
		case EFieldType_BOOL:
			value = ( *reinterpret_cast< bool * >( _value->GetPtrValue() ) ? 1 : 0 );
			break;

		case EFieldType_SMALL_INTEGER:
			value = *reinterpret_cast< int16_t * >( _value->GetPtrValue() );
			break;

		case EFieldType_INTEGER:
			value = *reinterpret_cast< int32_t * >( _value->GetPtrValue() );
			break;

		default:
			String errMsg = DATABASE_FIELD_GETVALUE_TYPE_ERROR + this->GetName();
			CLogger::LogError( errMsg );
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, DATABASE_FIELD_GETVALUE_TYPE_ERROR );
			break;
		}
	}

	void CDatabaseField::DoGetValue( uint32_t & value ) const
	{
		switch ( GetType() )
		{
		case EFieldType_BOOL:
			value = ( *reinterpret_cast< bool * >( _value->GetPtrValue() ) ? 1 : 0 );
			break;

		case EFieldType_SMALL_INTEGER:
			value = *reinterpret_cast< uint16_t * >( _value->GetPtrValue() );
			break;

		case EFieldType_INTEGER:
			value = *reinterpret_cast< uint32_t * >( _value->GetPtrValue() );
			break;

		default:
			String errMsg = DATABASE_FIELD_GETVALUE_TYPE_ERROR + this->GetName();
			CLogger::LogError( errMsg );
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, DATABASE_FIELD_GETVALUE_TYPE_ERROR );
			break;
		}
	}

	void CDatabaseField::DoGetValue( int64_t & value ) const
	{
		switch ( GetType() )
		{
		case EFieldType_SMALL_INTEGER:
			value = *reinterpret_cast< int16_t * >( _value->GetPtrValue() );
			break;

		case EFieldType_INTEGER:
			value = *reinterpret_cast< int32_t * >( _value->GetPtrValue() );
			break;

		case EFieldType_LONG_INTEGER:
			value = *reinterpret_cast< int64_t * >( _value->GetPtrValue() );
			break;

		default:
			String errMsg = DATABASE_FIELD_GETVALUE_TYPE_ERROR + this->GetName();
			CLogger::LogError( errMsg );
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, DATABASE_FIELD_GETVALUE_TYPE_ERROR );
			break;
		}
	}

	void CDatabaseField::DoGetValue( uint64_t & value ) const
	{
		switch ( GetType() )
		{
		case EFieldType_SMALL_INTEGER:
			value = *reinterpret_cast< uint16_t * >( _value->GetPtrValue() );
			break;

		case EFieldType_INTEGER:
			value = *reinterpret_cast< uint32_t * >( _value->GetPtrValue() );
			break;

		case EFieldType_LONG_INTEGER:
			value = *reinterpret_cast< uint64_t * >( _value->GetPtrValue() );
			break;

		default:
			String errMsg = DATABASE_FIELD_GETVALUE_TYPE_ERROR + this->GetName();
			CLogger::LogError( errMsg );
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, DATABASE_FIELD_GETVALUE_TYPE_ERROR );
			break;
		}
	}

	void CDatabaseField::DoGetValue( float & value ) const
	{
		switch ( GetType() )
		{
		case EFieldType_SMALL_INTEGER:
			value = float( *reinterpret_cast< short * >( _value->GetPtrValue() ) );
			break;

		case EFieldType_INTEGER:
			value = float( *reinterpret_cast< int * >( _value->GetPtrValue() ) );
			break;

		case EFieldType_LONG_INTEGER:
			value = float( *reinterpret_cast< long long * >( _value->GetPtrValue() ) );
			break;

		case EFieldType_FLOAT:
			value = ( *reinterpret_cast< float * >( _value->GetPtrValue() ) );
			break;

		case EFieldType_DOUBLE:
			value = float( *reinterpret_cast< double * >( _value->GetPtrValue() ) );
			break;

		default:
			String errMsg = DATABASE_FIELD_GETVALUE_TYPE_ERROR + this->GetName();
			CLogger::LogError( errMsg );
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, DATABASE_FIELD_GETVALUE_TYPE_ERROR );
			break;
		}
	}

	void CDatabaseField::DoGetValue( double & value ) const
	{
		switch ( GetType() )
		{
		case EFieldType_SMALL_INTEGER:
			value = double( *reinterpret_cast< short * >( _value->GetPtrValue() ) );
			break;

		case EFieldType_INTEGER:
			value = double( *reinterpret_cast< int * >( _value->GetPtrValue() ) );
			break;

		case EFieldType_LONG_INTEGER:
			value = double( *reinterpret_cast< long long * >( _value->GetPtrValue() ) );
			break;

		case EFieldType_FLOAT:
			value = double( *reinterpret_cast< float * >( _value->GetPtrValue() ) );
			break;

		case EFieldType_DOUBLE:
			value = ( *reinterpret_cast< double * >( _value->GetPtrValue() ) );
			break;

		default:
			String errMsg = DATABASE_FIELD_GETVALUE_TYPE_ERROR + this->GetName();
			CLogger::LogError( errMsg );
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, DATABASE_FIELD_GETVALUE_TYPE_ERROR );
			break;
		}
	}

	void CDatabaseField::DoGetValue( std::string & value ) const
	{
		value.clear();

		switch ( GetType() )
		{
		case EFieldType_TEXT:

			if ( _value->GetPtrValue() )
			{
				value = reinterpret_cast< char * >( _value->GetPtrValue() );
			}

			break;

		case EFieldType_VARCHAR:

			if ( _value->GetPtrValue() )
			{
				value = reinterpret_cast< char * >( _value->GetPtrValue() );
			}

			break;

		case EFieldType_NTEXT:

			if ( _value->GetPtrValue() )
			{
				value = CStrUtils::ToStr( reinterpret_cast< wchar_t * >( _value->GetPtrValue() ) );
			}

			break;

		case EFieldType_NVARCHAR:

			if ( _value->GetPtrValue() )
			{
				value = CStrUtils::ToStr( reinterpret_cast< wchar_t * >( _value->GetPtrValue() ) );
			}

			break;

		default:
			String errMsg = DATABASE_FIELD_GETVALUE_TYPE_ERROR + this->GetName();
			CLogger::LogError( errMsg );
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, DATABASE_FIELD_GETVALUE_TYPE_ERROR );
			break;
		}
	}

	void CDatabaseField::DoGetValue( std::wstring & value ) const
	{
		switch ( GetType() )
		{
		case EFieldType_TEXT:

			if ( _value->GetPtrValue() )
			{
				value = CStrUtils::ToWStr( *reinterpret_cast< std::string * >( _value->GetPtrValue() ) );
			}

			break;

		case EFieldType_VARCHAR:

			if ( _value->GetPtrValue() )
			{
				value = CStrUtils::ToWStr( reinterpret_cast< char * >( _value->GetPtrValue() ) );
			}

			break;

		case EFieldType_NTEXT:

			if ( _value->GetPtrValue() )
			{
				value = *reinterpret_cast< std::wstring * >( _value->GetPtrValue() );
			}

			break;

		case EFieldType_NVARCHAR:

			if ( _value->GetPtrValue() )
			{
				value = reinterpret_cast< wchar_t * >( _value->GetPtrValue() );
			}

			break;

		default:
			String errMsg = DATABASE_FIELD_GETVALUE_TYPE_ERROR + this->GetName();
			CLogger::LogError( errMsg );
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, DATABASE_FIELD_GETVALUE_TYPE_ERROR );
			break;
		}
	}

	void CDatabaseField::DoGetValue( CDate & value ) const
	{
		switch ( GetType() )
		{
		case EFieldType_DATE:
		case EFieldType_DATETIME:
		case EFieldType_NVARCHAR:// ODBC stores date as NVARCHAR
			value = _infos->GetConnection()->ParseDate( _value->GetStrValue() );
			break;

		default:
			String errMsg = DATABASE_FIELD_GETVALUE_TYPE_ERROR + this->GetName();
			CLogger::LogError( errMsg );
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, DATABASE_FIELD_GETVALUE_TYPE_ERROR );
			break;
		}
	}

	void CDatabaseField::DoGetValue( CDateTime & value ) const
	{
		switch ( GetType() )
		{
		case EFieldType_DATE:
		case EFieldType_DATETIME:
			value = _infos->GetConnection()->ParseDateTime( _value->GetStrValue() );
			break;

		case EFieldType_TIME:
		{
			std::tm val = { 0 };
			_infos->GetConnection()->ParseTime( _value->GetStrValue() ).ToCLibTm( val );
			value = CDateTime( val );
			break;
		}

		default:
			String errMsg = DATABASE_FIELD_GETVALUE_TYPE_ERROR + this->GetName();
			CLogger::LogError( errMsg );
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, DATABASE_FIELD_GETVALUE_TYPE_ERROR );
			break;
		}
	}

	void CDatabaseField::DoGetValue( CTime & value ) const
	{
		switch ( GetType() )
		{
		case EFieldType_DATETIME:
		case EFieldType_TIME:
			value = _infos->GetConnection()->ParseTime( _value->GetStrValue() );
			break;

		default:
			String errMsg = DATABASE_FIELD_GETVALUE_TYPE_ERROR + this->GetName();
			CLogger::LogError( errMsg );
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, DATABASE_FIELD_GETVALUE_TYPE_ERROR );
			break;
		}
	}

	void CDatabaseField::DoGetValue( std::vector< uint8_t > & value ) const
	{
		value.clear();

		switch ( GetType() )
		{
		case EFieldType_BINARY:
		case EFieldType_VARBINARY:
		case EFieldType_LONG_VARBINARY:

			if ( _value->GetPtrValue() )
			{
				uint8_t * ptr = reinterpret_cast< uint8_t * >( _value->GetPtrValue() );
				std::swap( value, std::vector< uint8_t >( ptr, ptr + _value->GetPtrSize() ) );
			}

			break;

		default:
			String errMsg = DATABASE_FIELD_GETVALUE_TYPE_ERROR + this->GetName();
			CLogger::LogError( errMsg );
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, DATABASE_FIELD_GETVALUE_TYPE_ERROR );
			break;
		}
	}

	void CDatabaseField::DoGetValueFast( bool & value ) const
	{
		assert( GetType() == EFieldType_BOOL );
		value = static_cast< CDatabaseValue< EFieldType_BOOL > * >( _value )->GetValue();
	}

	void CDatabaseField::DoGetValueFast( int16_t & value ) const
	{
		assert( GetType() == EFieldType_SMALL_INTEGER );
		value = static_cast< CDatabaseValue< EFieldType_SMALL_INTEGER > * >( _value )->GetValue();
	}

	void CDatabaseField::DoGetValueFast( uint16_t & value ) const
	{
		assert( GetType() == EFieldType_SMALL_INTEGER );
		value = static_cast< CDatabaseValue< EFieldType_SMALL_INTEGER > * >( _value )->GetValue();
	}

	void CDatabaseField::DoGetValueFast( int32_t & value ) const
	{
		assert( GetType() == EFieldType_INTEGER );
		value = static_cast< CDatabaseValue< EFieldType_INTEGER > * >( _value )->GetValue();
	}

	void CDatabaseField::DoGetValueFast( uint32_t & value ) const
	{
		assert( GetType() == EFieldType_INTEGER );
		value = static_cast< CDatabaseValue< EFieldType_INTEGER > * >( _value )->GetValue();
	}

	void CDatabaseField::DoGetValueFast( int64_t & value ) const
	{
		assert( GetType() == EFieldType_LONG_INTEGER );
		value = static_cast< CDatabaseValue< EFieldType_LONG_INTEGER > * >( _value )->GetValue();
	}

	void CDatabaseField::DoGetValueFast( uint64_t & value ) const
	{
		assert( GetType() == EFieldType_LONG_INTEGER );
		value = static_cast< CDatabaseValue< EFieldType_LONG_INTEGER > * >( _value )->GetValue();
	}

	void CDatabaseField::DoGetValueFast( float & value ) const
	{
		assert( GetType() == EFieldType_FLOAT );
		value = static_cast< CDatabaseValue< EFieldType_FLOAT > * >( _value )->GetValue();
	}

	void CDatabaseField::DoGetValueFast( double & value ) const
	{
		assert( GetType() == EFieldType_DOUBLE );
		value = static_cast< CDatabaseValue< EFieldType_DOUBLE > * >( _value )->GetValue();
	}

	void CDatabaseField::DoGetValueFast( std::string & value ) const
	{
		assert( GetType() == EFieldType_TEXT || GetType() == EFieldType_VARCHAR );

		if ( _value->GetPtrValue() )
		{
			if ( GetType() == EFieldType_TEXT )
			{
				value = static_cast< CDatabaseValue< EFieldType_TEXT > * >( _value )->GetValue();
			}
			else
			{
				value = static_cast< CDatabaseValue< EFieldType_VARCHAR > * >( _value )->GetValue();
			}
		}
		else
		{
			value.clear();
		}
	}

	void CDatabaseField::DoGetValueFast( std::wstring & value ) const
	{
		assert( GetType() == EFieldType_NTEXT || GetType() == EFieldType_NVARCHAR );

		if ( _value->GetPtrValue() )
		{
			if ( GetType() == EFieldType_TEXT )
			{
				value = static_cast< CDatabaseValue< EFieldType_NTEXT > * >( _value )->GetValue();
			}
			else
			{
				value = static_cast< CDatabaseValue< EFieldType_NVARCHAR > * >( _value )->GetValue();
			}
		}
		else
		{
			value.clear();
		}
	}

	void CDatabaseField::DoGetValueFast( CDate & value ) const
	{
		assert( GetType() == EFieldType_DATE );
		value = static_cast< CDatabaseValue< EFieldType_DATE > * >( _value )->GetValue();
	}

	void CDatabaseField::DoGetValueFast( CDateTime & value ) const
	{
		assert( GetType() == EFieldType_DATETIME );
		value = static_cast< CDatabaseValue< EFieldType_DATETIME > * >( _value )->GetValue();
	}

	void CDatabaseField::DoGetValueFast( CTime & value ) const
	{
		assert( GetType() == EFieldType_TIME );
		value = static_cast< CDatabaseValue< EFieldType_TIME > * >( _value )->GetValue();
	}

	void CDatabaseField::DoGetValueFast( std::vector< uint8_t > & value ) const
	{
		assert( GetType() == EFieldType_BINARY || GetType() == EFieldType_VARBINARY || GetType() == EFieldType_LONG_VARBINARY );
		value = static_cast< CDatabaseValue< EFieldType_BINARY > * >( _value )->GetValue();
	}

	void CDatabaseField::DoSetValue( const bool & value )
	{
		switch ( GetType() )
		{
		case EFieldType_BOOL:
			static_cast< CDatabaseValue< EFieldType_BOOL > * >( _value )->SetValue( value );
			break;

		case EFieldType_SMALL_INTEGER:
			static_cast< CDatabaseValue< EFieldType_SMALL_INTEGER > * >( _value )->SetValue( value ? 1 : 0 );
			break;

		case EFieldType_INTEGER:
			static_cast< CDatabaseValue< EFieldType_INTEGER > * >( _value )->SetValue( value ? 1 : 0 );
			break;

		case EFieldType_LONG_INTEGER:
			static_cast< CDatabaseValue< EFieldType_LONG_INTEGER > * >( _value )->SetValue( value ? 1 : 0 );
			break;

		case EFieldType_VARCHAR:
			static_cast< CDatabaseValue< EFieldType_VARCHAR > * >( _value )->SetValue( value ? "1" : "0", 1 );
			break;

		case EFieldType_TEXT:
			static_cast< CDatabaseValue< EFieldType_TEXT > * >( _value )->SetValue( value ? "1" : "0" );
			break;

		case EFieldType_NVARCHAR:
			static_cast< CDatabaseValue< EFieldType_NVARCHAR > * >( _value )->SetValue( value ? L"1" : L"0", 1 );
			break;

		case EFieldType_NTEXT:
			static_cast< CDatabaseValue< EFieldType_NTEXT > * >( _value )->SetValue( value ? L"1" : L"0" );
			break;

		default:
			String errMsg = DATABASE_FIELD_SETVALUE_TYPE_ERROR + this->GetName();
			CLogger::LogError( errMsg );
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, DATABASE_FIELD_SETVALUE_TYPE_ERROR );
			break;
		}

		_isNull = false;
	}

	void CDatabaseField::DoSetValue( const int16_t & value )
	{
		switch ( GetType() )
		{
		case EFieldType_BOOL:
			static_cast< CDatabaseValue< EFieldType_BOOL > * >( _value )->SetValue( value != 0 );
			break;

		case EFieldType_SMALL_INTEGER:
			static_cast< CDatabaseValue< EFieldType_SMALL_INTEGER > * >( _value )->SetValue( value );
			break;

		case EFieldType_INTEGER:
			static_cast< CDatabaseValue< EFieldType_INTEGER > * >( _value )->SetValue( value );
			break;

		case EFieldType_LONG_INTEGER:
			static_cast< CDatabaseValue< EFieldType_LONG_INTEGER > * >( _value )->SetValue( value );
			break;

		case EFieldType_FLOAT:
			static_cast< CDatabaseValue< EFieldType_FLOAT > * >( _value )->SetValue( value );
			break;

		case EFieldType_DOUBLE:
			static_cast< CDatabaseValue< EFieldType_DOUBLE > * >( _value )->SetValue( value );
			break;

		default:
			String errMsg = DATABASE_FIELD_SETVALUE_TYPE_ERROR + this->GetName();
			CLogger::LogError( errMsg );
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, DATABASE_FIELD_SETVALUE_TYPE_ERROR );
			break;
		}

		_isNull = false;
	}

	void CDatabaseField::DoSetValue( const uint16_t & value )
	{
		switch ( GetType() )
		{
		case EFieldType_BOOL:
			static_cast< CDatabaseValue< EFieldType_BOOL > * >( _value )->SetValue( value != 0 );
			break;

		case EFieldType_SMALL_INTEGER:
			static_cast< CDatabaseValue< EFieldType_SMALL_INTEGER > * >( _value )->SetValue( value );
			break;

		case EFieldType_INTEGER:
			static_cast< CDatabaseValue< EFieldType_INTEGER > * >( _value )->SetValue( value );
			break;

		case EFieldType_LONG_INTEGER:
			static_cast< CDatabaseValue< EFieldType_LONG_INTEGER > * >( _value )->SetValue( value );
			break;

		case EFieldType_FLOAT:
			static_cast< CDatabaseValue< EFieldType_FLOAT > * >( _value )->SetValue( value );
			break;

		case EFieldType_DOUBLE:
			static_cast< CDatabaseValue< EFieldType_DOUBLE > * >( _value )->SetValue( value );
			break;

		default:
			String errMsg = DATABASE_FIELD_SETVALUE_TYPE_ERROR + this->GetName();
			CLogger::LogError( errMsg );
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, DATABASE_FIELD_SETVALUE_TYPE_ERROR );
			break;
		}

		_isNull = false;
	}

	void CDatabaseField::DoSetValue( const int32_t & value )
	{
		switch ( GetType() )
		{
		case EFieldType_BOOL:
			static_cast< CDatabaseValue< EFieldType_BOOL > * >( _value )->SetValue( value != 0 );
			break;

		case EFieldType_INTEGER:
			static_cast< CDatabaseValue< EFieldType_INTEGER > * >( _value )->SetValue( value );
			break;

		case EFieldType_LONG_INTEGER:
			static_cast< CDatabaseValue< EFieldType_LONG_INTEGER > * >( _value )->SetValue( value );
			break;

		case EFieldType_FLOAT:
			static_cast< CDatabaseValue< EFieldType_FLOAT > * >( _value )->SetValue( float( value ) );
			break;

		case EFieldType_DOUBLE:
			static_cast< CDatabaseValue< EFieldType_DOUBLE > * >( _value )->SetValue( double( value ) );
			break;

		default:
			String errMsg = DATABASE_FIELD_SETVALUE_TYPE_ERROR + this->GetName();
			CLogger::LogError( errMsg );
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, DATABASE_FIELD_SETVALUE_TYPE_ERROR );
			break;
		}

		_isNull = false;
	}

	void CDatabaseField::DoSetValue( const uint32_t & value )
	{
		switch ( GetType() )
		{
		case EFieldType_BOOL:
			static_cast< CDatabaseValue< EFieldType_BOOL > * >( _value )->SetValue( value != 0 );
			break;

		case EFieldType_INTEGER:
			static_cast< CDatabaseValue< EFieldType_INTEGER > * >( _value )->SetValue( value );
			break;

		case EFieldType_LONG_INTEGER:
			static_cast< CDatabaseValue< EFieldType_LONG_INTEGER > * >( _value )->SetValue( value );
			break;

		case EFieldType_FLOAT:
			static_cast< CDatabaseValue< EFieldType_FLOAT > * >( _value )->SetValue( float( value ) );
			break;

		case EFieldType_DOUBLE:
			static_cast< CDatabaseValue< EFieldType_DOUBLE > * >( _value )->SetValue( double( value ) );
			break;

		default:
			String errMsg = DATABASE_FIELD_SETVALUE_TYPE_ERROR + this->GetName();
			CLogger::LogError( errMsg );
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, DATABASE_FIELD_SETVALUE_TYPE_ERROR );
			break;
		}

		_isNull = false;
	}

	void CDatabaseField::DoSetValue( const int64_t & value )
	{
		switch ( GetType() )
		{
		case EFieldType_BOOL:
			static_cast< CDatabaseValue< EFieldType_BOOL > * >( _value )->SetValue( value != 0 );
			break;

		case EFieldType_LONG_INTEGER:
			static_cast< CDatabaseValue< EFieldType_LONG_INTEGER > * >( _value )->SetValue( value );
			break;

		default:
			String errMsg = DATABASE_FIELD_SETVALUE_TYPE_ERROR + this->GetName();
			CLogger::LogError( errMsg );
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, DATABASE_FIELD_SETVALUE_TYPE_ERROR );
			break;
		}

		_isNull = false;
	}

	void CDatabaseField::DoSetValue( const uint64_t & value )
	{
		switch ( GetType() )
		{
		case EFieldType_BOOL:
			static_cast< CDatabaseValue< EFieldType_BOOL > * >( _value )->SetValue( value != 0 );
			break;

		case EFieldType_LONG_INTEGER:
			static_cast< CDatabaseValue< EFieldType_LONG_INTEGER > * >( _value )->SetValue( value );
			break;

		default:
			String errMsg = DATABASE_FIELD_SETVALUE_TYPE_ERROR + this->GetName();
			CLogger::LogError( errMsg );
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, DATABASE_FIELD_SETVALUE_TYPE_ERROR );
			break;
		}

		_isNull = false;
	}

	void CDatabaseField::DoSetValue( const float & value )
	{
		switch ( GetType() )
		{
		case EFieldType_SMALL_INTEGER:
			static_cast< CDatabaseValue< EFieldType_SMALL_INTEGER > * >( _value )->SetValue( short( value ) );
			break;

		case EFieldType_INTEGER:
			static_cast< CDatabaseValue< EFieldType_INTEGER > * >( _value )->SetValue( int( value ) );
			break;

		case EFieldType_LONG_INTEGER:
			static_cast< CDatabaseValue< EFieldType_LONG_INTEGER > * >( _value )->SetValue( ( long long )( value ) );
			break;

		case EFieldType_FLOAT:
			static_cast< CDatabaseValue< EFieldType_FLOAT > * >( _value )->SetValue( value );
			break;

		case EFieldType_DOUBLE:
			static_cast< CDatabaseValue< EFieldType_DOUBLE > * >( _value )->SetValue( value );
			break;

		default:
			String errMsg = DATABASE_FIELD_SETVALUE_TYPE_ERROR + this->GetName();
			CLogger::LogError( errMsg );
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, DATABASE_FIELD_SETVALUE_TYPE_ERROR );
			break;
		}

		_isNull = false;
	}

	void CDatabaseField::DoSetValue( const double & value )
	{
		switch ( GetType() )
		{
		case EFieldType_SMALL_INTEGER:
			static_cast< CDatabaseValue< EFieldType_SMALL_INTEGER > * >( _value )->SetValue( short( value ) );
			break;

		case EFieldType_INTEGER:
			static_cast< CDatabaseValue< EFieldType_INTEGER > * >( _value )->SetValue( int( value ) );
			break;

		case EFieldType_LONG_INTEGER:
			static_cast< CDatabaseValue< EFieldType_LONG_INTEGER > * >( _value )->SetValue( ( long long )( value ) );
			break;

		case EFieldType_FLOAT:
			static_cast< CDatabaseValue< EFieldType_FLOAT > * >( _value )->SetValue( float( value ) );
			break;

		case EFieldType_DOUBLE:
			static_cast< CDatabaseValue< EFieldType_DOUBLE > * >( _value )->SetValue( value );
			break;

		default:
			String errMsg = DATABASE_FIELD_SETVALUE_TYPE_ERROR + this->GetName();
			CLogger::LogError( errMsg );
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, DATABASE_FIELD_SETVALUE_TYPE_ERROR );
			break;
		}

		_isNull = false;
	}

	void CDatabaseField::DoSetValue( const std::string & value )
	{
		switch ( GetType() )
		{
		case EFieldType_VARCHAR:
			static_cast< CDatabaseValue< EFieldType_VARCHAR > * >( _value )->SetValue( value.c_str(), std::min( GetLimits(), uint32_t( value.size() ) ) );
			break;

		case EFieldType_TEXT:
			static_cast< CDatabaseValue< EFieldType_TEXT > * >( _value )->SetValue( value );
			break;

		case EFieldType_NVARCHAR:
			static_cast< CDatabaseValue< EFieldType_NVARCHAR > * >( _value )->SetValue( CStrUtils::ToWStr( value ).c_str(), std::min( GetLimits(), uint32_t( value.size() ) ) );
			break;

		case EFieldType_NTEXT:
			static_cast< CDatabaseValue< EFieldType_NTEXT > * >( _value )->SetValue( CStrUtils::ToWStr( value ) );
			break;

		case EFieldType_DATE:
			static_cast< CDatabaseValue< EFieldType_DATE > * >( _value )->SetValue( value );
			break;

		case EFieldType_DATETIME:
			static_cast< CDatabaseValue< EFieldType_DATETIME > * >( _value )->SetValue( value );
			break;

		case EFieldType_TIME:
			static_cast< CDatabaseValue< EFieldType_TIME > * >( _value )->SetValue( value );
			break;

		default:
			String errMsg = DATABASE_FIELD_SETVALUE_TYPE_ERROR + this->GetName();
			CLogger::LogError( errMsg );
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, DATABASE_FIELD_SETVALUE_TYPE_ERROR );
			break;
		}

		_isNull = false;
	}

	void CDatabaseField::DoSetValue( const std::wstring & value )
	{
		switch ( GetType() )
		{
		case EFieldType_TEXT:
			static_cast< CDatabaseValue< EFieldType_TEXT > * >( _value )->SetValue( CStrUtils::ToStr( value ) );
			break;

		case EFieldType_VARCHAR:
			static_cast< CDatabaseValue< EFieldType_VARCHAR > * >( _value )->SetValue( CStrUtils::ToStr( value ).c_str(), std::min( GetLimits(), uint32_t( value.size() ) ) );
			break;

		case EFieldType_NTEXT:
			static_cast< CDatabaseValue< EFieldType_NTEXT > * >( _value )->SetValue( value );
			break;

		case EFieldType_NVARCHAR:
			static_cast< CDatabaseValue< EFieldType_NVARCHAR > * >( _value )->SetValue( value.c_str(), std::min( GetLimits(), uint32_t( value.size() ) ) );
			break;

		case EFieldType_DATE:
			static_cast< CDatabaseValue< EFieldType_DATE > * >( _value )->SetValue( CStrUtils::ToStr( value ) );
			break;

		case EFieldType_DATETIME:
			static_cast< CDatabaseValue< EFieldType_DATETIME > * >( _value )->SetValue( CStrUtils::ToStr( value ) );
			break;

		case EFieldType_TIME:
			static_cast< CDatabaseValue< EFieldType_TIME > * >( _value )->SetValue( CStrUtils::ToStr( value ) );
			break;

		default:
			String errMsg = DATABASE_FIELD_SETVALUE_TYPE_ERROR + this->GetName();
			CLogger::LogError( errMsg );
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, DATABASE_FIELD_SETVALUE_TYPE_ERROR );
			break;
		}

		_isNull = false;
	}

	void CDatabaseField::DoSetValue( const CDate & value )
	{
		switch ( GetType() )
		{
		case EFieldType_DATE:
			static_cast< CDatabaseValue< EFieldType_DATE > * >( _value )->SetValue( value.Format( SDATE_FORMAT_EXP ) );
			break;

		case EFieldType_DATETIME:
			static_cast< CDatabaseValue< EFieldType_DATETIME > * >( _value )->SetValue( value.Format( SDATETIME_DATE_FORMAT_EXP ) );
			break;

		default:
			String errMsg = DATABASE_FIELD_SETVALUE_TYPE_ERROR + this->GetName();
			CLogger::LogError( errMsg );
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, DATABASE_FIELD_SETVALUE_TYPE_ERROR );
			break;
		}

		_isNull = false;
	}

	void CDatabaseField::DoSetValue( const CDateTime & value )
	{
		switch ( GetType() )
		{
		case EFieldType_DATE:
			static_cast< CDatabaseValue< EFieldType_DATE > * >( _value )->SetValue( value.ToStdString() );
			break;

		case EFieldType_DATETIME:
			static_cast< CDatabaseValue< EFieldType_DATETIME > * >( _value )->SetValue( value.ToStdString() );
			break;

		case EFieldType_TIME:
			static_cast< CDatabaseValue< EFieldType_DATE > * >( _value )->SetValue( value.ToStdString() );

		default:
			String errMsg = DATABASE_FIELD_SETVALUE_TYPE_ERROR + this->GetName();
			CLogger::LogError( errMsg );
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, DATABASE_FIELD_SETVALUE_TYPE_ERROR );
			break;
		}

		_isNull = false;
	}

	void CDatabaseField::DoSetValue( const CTime & value )
	{
		switch ( GetType() )
		{
		case EFieldType_DATETIME:
			static_cast< CDatabaseValue< EFieldType_DATETIME > * >( _value )->SetValue( value.Format( SDATETIME_TIME_FORMAT_EXP ) );
			break;

		case EFieldType_TIME:
			static_cast< CDatabaseValue< EFieldType_TIME > * >( _value )->SetValue( value.Format( STIME_FORMAT_EXP ) );
			break;

		default:
			String errMsg = DATABASE_FIELD_SETVALUE_TYPE_ERROR + this->GetName();
			CLogger::LogError( errMsg );
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, DATABASE_FIELD_SETVALUE_TYPE_ERROR );
			break;
		}

		_isNull = false;
	}

	void CDatabaseField::DoSetValue( const std::vector< uint8_t > & value )
	{
		switch ( GetType() )
		{
		case EFieldType_BINARY:
			static_cast< CDatabaseValue< EFieldType_BINARY > * >( _value )->SetValue( value.data(), std::min( GetLimits(), uint32_t( value.size() ) ) );
			break;

		case EFieldType_VARBINARY:
			static_cast< CDatabaseValue< EFieldType_VARBINARY > * >( _value )->SetValue( value.data(), std::min( GetLimits(), uint32_t( value.size() ) ) );
			break;

		case EFieldType_LONG_VARBINARY:
			static_cast< CDatabaseValue< EFieldType_LONG_VARBINARY > * >( _value )->SetValue( value.data(), std::min( GetLimits(), uint32_t( value.size() ) ) );
			break;

		default:
			String errMsg = DATABASE_FIELD_SETVALUE_TYPE_ERROR + this->GetName();
			CLogger::LogError( errMsg );
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, DATABASE_FIELD_SETVALUE_TYPE_ERROR );
			break;
		}

		_isNull = false;
	}

	void CDatabaseField::DoSetValueFast( const bool & value )
	{
		assert( GetType() == EFieldType_BOOL );
		static_cast< CDatabaseValue< EFieldType_BOOL > * >( _value )->SetValue( value );
		_isNull = false;
	}

	void CDatabaseField::DoSetValueFast( const int16_t & value )
	{
		assert( GetType() == EFieldType_SMALL_INTEGER );
		static_cast< CDatabaseValue< EFieldType_SMALL_INTEGER > * >( _value )->SetValue( value );
		_isNull = false;
	}

	void CDatabaseField::DoSetValueFast( const uint16_t & value )
	{
		assert( GetType() == EFieldType_SMALL_INTEGER );
		static_cast< CDatabaseValue< EFieldType_SMALL_INTEGER > * >( _value )->SetValue( value );
		_isNull = false;
	}

	void CDatabaseField::DoSetValueFast( const int32_t & value )
	{
		assert( GetType() == EFieldType_INTEGER );
		static_cast< CDatabaseValue< EFieldType_INTEGER > * >( _value )->SetValue( value );
		_isNull = false;
	}

	void CDatabaseField::DoSetValueFast( const uint32_t & value )
	{
		assert( GetType() == EFieldType_INTEGER );
		static_cast< CDatabaseValue< EFieldType_INTEGER > * >( _value )->SetValue( value );
		_isNull = false;
	}

	void CDatabaseField::DoSetValueFast( const int64_t & value )
	{
		assert( GetType() == EFieldType_LONG_INTEGER );
		static_cast< CDatabaseValue< EFieldType_LONG_INTEGER > * >( _value )->SetValue( value );
		_isNull = false;
	}

	void CDatabaseField::DoSetValueFast( const uint64_t & value )
	{
		assert( GetType() == EFieldType_LONG_INTEGER );
		static_cast< CDatabaseValue< EFieldType_LONG_INTEGER > * >( _value )->SetValue( value );
		_isNull = false;
	}

	void CDatabaseField::DoSetValueFast( const float & value )
	{
		assert( GetType() == EFieldType_FLOAT );
		static_cast< CDatabaseValue< EFieldType_FLOAT > * >( _value )->SetValue( value );
		_isNull = false;
	}

	void CDatabaseField::DoSetValueFast( const double & value )
	{
		assert( GetType() == EFieldType_DOUBLE );
		static_cast< CDatabaseValue< EFieldType_DOUBLE > * >( _value )->SetValue( value );
		_isNull = false;
	}

	void CDatabaseField::DoSetValueFast( const char * value )
	{
		assert( GetType() == EFieldType_VARCHAR );
		static_cast< CDatabaseValue< EFieldType_VARCHAR > * >( _value )->SetValue( value, std::min( GetLimits(), uint32_t( strlen( value ) ) ) );
		_isNull = false;
	}

	void CDatabaseField::DoSetValueFast( const std::string & value )
	{
		assert( GetType() == EFieldType_TEXT );
		static_cast< CDatabaseValue< EFieldType_TEXT > * >( _value )->SetValue( value );
		_isNull = false;
	}

	void CDatabaseField::DoSetValueFast( const wchar_t * value )
	{
		assert( GetType() == EFieldType_NVARCHAR );
		static_cast< CDatabaseValue< EFieldType_NVARCHAR > * >( _value )->SetValue( value, std::min( GetLimits(), uint32_t( wcslen( value ) ) ) );
		_isNull = false;
	}

	void CDatabaseField::DoSetValueFast( const std::wstring & value )
	{
		assert( GetType() == EFieldType_NTEXT );
		static_cast< CDatabaseValue< EFieldType_NTEXT > * >( _value )->SetValue( value );
		_isNull = false;
	}

	void CDatabaseField::DoSetValueFast( const CDate & value )
	{
		assert( GetType() == EFieldType_DATE );
		static_cast< CDatabaseValue< EFieldType_DATE > * >( _value )->SetValue( value.Format( SDATE_FORMAT_EXP ) );
		_isNull = false;
	}

	void CDatabaseField::DoSetValueFast( const CDateTime & value )
	{
		assert( GetType() == EFieldType_DATETIME );
		static_cast< CDatabaseValue< EFieldType_DATETIME > * >( _value )->SetValue( value.ToStdString() );
		_isNull = false;
	}

	void CDatabaseField::DoSetValueFast( const CTime & value )
	{
		assert( GetType() == EFieldType_TIME );
		static_cast< CDatabaseValue< EFieldType_TIME > * >( _value )->SetValue( value.Format( STIME_FORMAT_EXP ) );
		_isNull = false;
	}

	void CDatabaseField::DoSetValueFast( const std::vector< uint8_t > & value )
	{
		assert( GetType() == EFieldType_BINARY || GetType() == EFieldType_VARBINARY || GetType() == EFieldType_LONG_VARBINARY );
		static_cast< CDatabaseValue< EFieldType_BINARY > * >( _value )->SetValue( value.data(), std::min( GetLimits(), uint32_t( value.size() ) ) );
		_isNull = false;
	}
}
END_NAMESPACE_DATABASE
