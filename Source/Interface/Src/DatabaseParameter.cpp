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
#include "DatabaseParameterValueSetter.h"

BEGIN_NAMESPACE_DATABASE
{
	static const String DATABASE_PARAMETER_ERROR = STR( "Error while treating a parameter." );

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
		:   _name( name )
		,   _fieldType( fieldType )
		,   _limits( 0 )
		,   _index( index )
		,   _parameterType( parameterType )
		,   _value( NULL )
		,   _connection( connection )
		,   _updater( updater )
	{
		switch ( fieldType )
		{
		case EFieldType_BOOL:
			_value = new CDatabaseValue< EFieldType_BOOL >( connection );
			break;

		case EFieldType_SMALL_INTEGER:
			_value = new CDatabaseValue< EFieldType_SMALL_INTEGER >( connection );
			break;

		case EFieldType_INTEGER:
			_value = new CDatabaseValue< EFieldType_INTEGER >( connection );
			break;

		case EFieldType_LONG_INTEGER:
			_value = new CDatabaseValue< EFieldType_LONG_INTEGER >( connection );
			break;

		case EFieldType_FLOAT:
			_value = new CDatabaseValue< EFieldType_FLOAT >( connection );
			break;

		case EFieldType_DOUBLE:
			_value = new CDatabaseValue< EFieldType_DOUBLE >( connection );
			break;

		case EFieldType_VARCHAR:
			_value = new CDatabaseValue< EFieldType_VARCHAR >( connection );
			break;

		case EFieldType_TEXT:
			_value = new CDatabaseValue< EFieldType_TEXT>( connection );
			break;

		case EFieldType_NVARCHAR:
			_value = new CDatabaseValue< EFieldType_NVARCHAR >( connection );
			break;

		case EFieldType_NTEXT:
			_value = new CDatabaseValue< EFieldType_NTEXT >( connection );
			break;

		case EFieldType_DATE:
			_value = new CDatabaseValue< EFieldType_DATE >( connection );
			break;

		case EFieldType_DATETIME:
			_value = new CDatabaseValue< EFieldType_DATETIME >( connection );
			break;

		case EFieldType_TIME:
			_value = new CDatabaseValue< EFieldType_TIME >( connection );
			break;

		case EFieldType_BINARY:
			_value = new CDatabaseValue< EFieldType_BINARY >( connection );
			break;

		case EFieldType_VARBINARY:
			_value = new CDatabaseValue< EFieldType_VARBINARY >( connection );
			break;

		case EFieldType_LONG_VARBINARY:
			_value = new CDatabaseValue< EFieldType_LONG_VARBINARY >( connection );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_ERROR );
			break;
		}
	}

	CDatabaseParameter::CDatabaseParameter( DatabaseConnectionPtr connection, const String & name, unsigned short index, EFieldType fieldType, uint32_t limits, EParameterType parameterType, SValueUpdater * updater )
		:   _name( name )
		,   _fieldType( fieldType )
		,   _limits( limits )
		,   _index( index )
		,   _parameterType( parameterType )
		,   _value( NULL )
		,   _connection( connection )
		,   _updater( updater )
	{
		switch ( fieldType )
		{
		case EFieldType_BOOL:
			_value = new CDatabaseValue< EFieldType_BOOL >( connection );
			break;

		case EFieldType_SMALL_INTEGER:
			_value = new CDatabaseValue< EFieldType_SMALL_INTEGER >( connection );
			break;

		case EFieldType_INTEGER:
			_value = new CDatabaseValue< EFieldType_INTEGER >( connection );
			break;

		case EFieldType_LONG_INTEGER:
			_value = new CDatabaseValue< EFieldType_LONG_INTEGER >( connection );
			break;

		case EFieldType_FLOAT:
			_value = new CDatabaseValue< EFieldType_FLOAT >( connection );
			break;

		case EFieldType_DOUBLE:
			_value = new CDatabaseValue< EFieldType_DOUBLE >( connection );
			break;

		case EFieldType_VARCHAR:
			_value = new CDatabaseValue< EFieldType_VARCHAR >( connection );
			break;

		case EFieldType_TEXT:
			_value = new CDatabaseValue< EFieldType_TEXT >( connection );
			break;

		case EFieldType_NVARCHAR:
			_value = new CDatabaseValue< EFieldType_NVARCHAR >( connection );
			break;

		case EFieldType_NTEXT:
			_value = new CDatabaseValue< EFieldType_NTEXT >( connection );
			break;

		case EFieldType_DATE:
			_value = new CDatabaseValue< EFieldType_DATE >( connection );
			break;

		case EFieldType_DATETIME:
			_value = new CDatabaseValue< EFieldType_DATETIME >( connection );
			break;

		case EFieldType_TIME:
			_value = new CDatabaseValue< EFieldType_TIME >( connection );
			break;

		case EFieldType_BINARY:
			_value = new CDatabaseValue< EFieldType_BINARY >( connection );
			break;

		case EFieldType_VARBINARY:
			_value = new CDatabaseValue< EFieldType_VARBINARY >( connection );
			break;

		case EFieldType_LONG_VARBINARY:
			_value = new CDatabaseValue< EFieldType_LONG_VARBINARY >( connection );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_ERROR );
			break;
		}
	}

	CDatabaseParameter::~CDatabaseParameter()
	{
		delete _updater;
		delete _value;
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
		_value->SetNull();
		_updater->Update( shared_from_this() );
	}

	bool CDatabaseParameter::IsNull() const
	{
		return _value->IsNull();
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

		_value->SetValue( field->GetFieldValue() );
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

		_value->SetValue( parameter->GetParameterValue() );
		_updater->Update( shared_from_this() );
	}

	String CDatabaseParameter::GetInsertValue() const
	{
		String strInsertValue;

		if ( IsNull() )
		{
			strInsertValue = STR( "NULL" );
		}
		else
		{
			_value->GetInsertValue( strInsertValue );
		}

		return strInsertValue;
	}

	String CDatabaseParameter::GetStrValue() const
	{
		return _value->GetStrValue();
	}

	void * CDatabaseParameter::GetPtrValue()
	{
		return _value->GetPtrValue();
	}

	const long & CDatabaseParameter::GetPtrSize() const
	{
		return _value->GetPtrSize();
	}

	CDatabaseValueBase * CDatabaseParameter::GetParameterValue() const
	{
		return _value;
	}

	void CDatabaseParameter::DoSetValue( bool value )
	{
		switch ( GetType() )
		{
		case EFieldType_BOOL:
			SDatabaseParameterValueSetter< bool >()( _value, value );
			break;

		case EFieldType_SMALL_INTEGER:
			SDatabaseParameterValueSetter< int16_t >()( _value, value ? 1 : 0 );
			break;

		case EFieldType_INTEGER:
			SDatabaseParameterValueSetter< int32_t >()( _value, value ? 1 : 0 );
			break;

		case EFieldType_LONG_INTEGER:
			SDatabaseParameterValueSetter< int64_t >()( _value, value ? 1 : 0 );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseParameter::DoSetValue( int16_t value )
	{
		switch ( _fieldType )
		{
		case EFieldType_BOOL:
			SDatabaseParameterValueSetter< bool >()( _value, value != 0 );
			break;

		case EFieldType_SMALL_INTEGER:
			SDatabaseParameterValueSetter< int16_t >()( _value, value );
			break;

		case EFieldType_INTEGER:
			SDatabaseParameterValueSetter< int32_t >()( _value, value );
			break;

		case EFieldType_LONG_INTEGER:
			SDatabaseParameterValueSetter< int64_t >()( _value, value );
			break;

		case EFieldType_FLOAT:
			SDatabaseParameterValueSetter< float >()( _value, float( value ) );
			break;

		case EFieldType_DOUBLE:
			SDatabaseParameterValueSetter< double >()( _value, double( value ) );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseParameter::DoSetValue( uint16_t value )
	{
		switch ( _fieldType )
		{
		case EFieldType_BOOL:
			SDatabaseParameterValueSetter< bool >()( _value, value != 0 );
			break;

		case EFieldType_SMALL_INTEGER:
			SDatabaseParameterValueSetter< int16_t >()( _value, value );
			break;

		case EFieldType_INTEGER:
			SDatabaseParameterValueSetter< int32_t >()( _value, value );
			break;

		case EFieldType_LONG_INTEGER:
			SDatabaseParameterValueSetter< int64_t >()( _value, value );
			break;

		case EFieldType_FLOAT:
			SDatabaseParameterValueSetter< float >()( _value, float( value ) );
			break;

		case EFieldType_DOUBLE:
			SDatabaseParameterValueSetter< double >()( _value, double( value ) );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseParameter::DoSetValue( int32_t value )
	{
		switch ( GetType() )
		{
		case EFieldType_BOOL:
			SDatabaseParameterValueSetter< bool >()( _value, value != 0 );
			break;

		case EFieldType_INTEGER:
			SDatabaseParameterValueSetter< int32_t >()( _value, value );
			break;

		case EFieldType_LONG_INTEGER:
			SDatabaseParameterValueSetter< int64_t >()( _value, value );
			break;

		case EFieldType_FLOAT:
			SDatabaseParameterValueSetter< float >()( _value, float( value ) );
			break;

		case EFieldType_DOUBLE:
			SDatabaseParameterValueSetter< double >()( _value, double( value ) );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseParameter::DoSetValue( uint32_t value )
	{
		switch ( GetType() )
		{
		case EFieldType_BOOL:
			SDatabaseParameterValueSetter< bool >()( _value, value != 0 );
			break;

		case EFieldType_INTEGER:
			SDatabaseParameterValueSetter< int32_t >()( _value, value );
			break;

		case EFieldType_LONG_INTEGER:
			SDatabaseParameterValueSetter< int64_t >()( _value, value );
			break;

		case EFieldType_FLOAT:
			SDatabaseParameterValueSetter< float >()( _value, float( value ) );
			break;

		case EFieldType_DOUBLE:
			SDatabaseParameterValueSetter< double >()( _value, double( value ) );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseParameter::DoSetValue( int64_t value )
	{
		switch ( GetType() )
		{
		case EFieldType_BOOL:
			SDatabaseParameterValueSetter< bool >()( _value, value != 0 );
			break;

		case EFieldType_LONG_INTEGER:
			SDatabaseParameterValueSetter< int64_t >()( _value, value );
			break;

		case EFieldType_DOUBLE:
			SDatabaseParameterValueSetter< double >()( _value, double( value ) );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseParameter::DoSetValue( uint64_t value )
	{
		switch ( GetType() )
		{
		case EFieldType_BOOL:
			SDatabaseParameterValueSetter< bool >()( _value, value != 0 );
			break;

		case EFieldType_LONG_INTEGER:
			SDatabaseParameterValueSetter< int64_t >()( _value, value );
			break;

		case EFieldType_DOUBLE:
			SDatabaseParameterValueSetter< double >()( _value, double( value ) );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseParameter::DoSetValue( float value )
	{
		switch ( GetType() )
		{
		case EFieldType_SMALL_INTEGER:
			SDatabaseParameterValueSetter< int16_t >()( _value, int16_t( value ) );
			break;

		case EFieldType_INTEGER:
			SDatabaseParameterValueSetter< int32_t >()( _value, int32_t( value ) );
			break;

		case EFieldType_LONG_INTEGER:
			SDatabaseParameterValueSetter< int64_t >()( _value, int64_t( value ) );
			break;

		case EFieldType_FLOAT:
			SDatabaseParameterValueSetter< float >()( _value, float( value ) );
			break;

		case EFieldType_DOUBLE:
			SDatabaseParameterValueSetter< double >()( _value, double( value ) );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseParameter::DoSetValue( double value )
	{
		switch ( GetType() )
		{
		case EFieldType_SMALL_INTEGER:
			SDatabaseParameterValueSetter< int16_t >()( _value, int16_t( value ) );
			break;

		case EFieldType_INTEGER:
			SDatabaseParameterValueSetter< int32_t >()( _value, int32_t( value ) );
			break;

		case EFieldType_LONG_INTEGER:
			SDatabaseParameterValueSetter< int64_t >()( _value, int64_t( value ) );
			break;

		case EFieldType_FLOAT:
			SDatabaseParameterValueSetter< float >()( _value, float( value ) );
			break;

		case EFieldType_DOUBLE:
			SDatabaseParameterValueSetter< double >()( _value, double( value ) );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseParameter::DoSetValue( long double value )
	{
		switch ( GetType() )
		{
		case EFieldType_SMALL_INTEGER:
			SDatabaseParameterValueSetter< int16_t >()( _value, int16_t( value ) );
			break;

		case EFieldType_INTEGER:
			SDatabaseParameterValueSetter< int32_t >()( _value, int32_t( value ) );
			break;

		case EFieldType_LONG_INTEGER:
			SDatabaseParameterValueSetter< int64_t >()( _value, int64_t( value ) );
			break;

		case EFieldType_FLOAT:
			SDatabaseParameterValueSetter< float >()( _value, float( value ) );
			break;

		case EFieldType_DOUBLE:
			SDatabaseParameterValueSetter< double >()( _value, double( value ) );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseParameter::DoSetValue( const std::string & value )
	{
		DoSetValue( value.c_str() );
	}

	void CDatabaseParameter::DoSetValue( const char * value )
	{
		switch ( GetType() )
		{
		case EFieldType_VARCHAR:
			SDatabaseParameterValueSetter< char * >()( _value, value );
			break;

		case EFieldType_TEXT:
			SDatabaseParameterValueSetter< std::string >()( _value, value );
			break;

		case EFieldType_NVARCHAR:
			SDatabaseParameterValueSetter< wchar_t * >()( _value, CStrUtils::ToWStr( value ) );
			break;

		case EFieldType_NTEXT:
			SDatabaseParameterValueSetter< std::wstring >()( _value, CStrUtils::ToWStr( value ) );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseParameter::DoSetValue( const std::wstring & value )
	{
		DoSetValue( value.c_str() );
	}

	void CDatabaseParameter::DoSetValue( const wchar_t * value )
	{
		switch ( GetType() )
		{
		case EFieldType_VARCHAR:
			SDatabaseParameterValueSetter< char * >()( _value, CStrUtils::ToStr( value ) );
			break;

		case EFieldType_TEXT:
			SDatabaseParameterValueSetter< std::string >()( _value, CStrUtils::ToStr( value ) );
			break;

		case EFieldType_NVARCHAR:
			SDatabaseParameterValueSetter< wchar_t * >()( _value, value );
			break;

		case EFieldType_NTEXT:
			SDatabaseParameterValueSetter< std::wstring >()( _value, value );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseParameter::DoSetValue( std::vector< uint8_t > & value )
	{
		switch ( GetType() )
		{
		case EFieldType_BINARY:
			SDatabaseParameterValueSetter< uint8_t >()( _value, value.data(), std::min( _limits, uint32_t( value.size() ) ) );
			break;

		case EFieldType_VARBINARY:
			SDatabaseParameterValueSetter< uint8_t >()( _value, value.data(), std::min( _limits, uint32_t( value.size() ) ) );
			break;

		case EFieldType_LONG_VARBINARY:
			SDatabaseParameterValueSetter< uint8_t >()( _value, value.data(), std::min( _limits, uint32_t( value.size() ) ) );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseParameter::DoSetValue( std::istream * value )
	{
		std::noskipws( *value );
		std::vector< uint8_t > in;
		std::copy(
			std::istream_iterator< uint8_t >( *value ),
			std::istream_iterator< uint8_t >(),
			std::back_inserter( in ) );

		switch ( GetType() )
		{
		case EFieldType_BINARY:
			SDatabaseParameterValueSetter< uint8_t >()( _value, in.data(), std::min( _limits, uint32_t( in.size() ) ) );
			break;

		case EFieldType_VARBINARY:
			SDatabaseParameterValueSetter< uint8_t >()( _value, in.data(), std::min( _limits, uint32_t( in.size() ) ) );
			break;

		case EFieldType_LONG_VARBINARY:
			SDatabaseParameterValueSetter< uint8_t >()( _value, in.data(), std::min( _limits, uint32_t( in.size() ) ) );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseParameter::DoSetValue( const CDate & value )
	{
		std::string strValue;

		switch ( GetType() )
		{
		case EFieldType_DATETIME:
		case EFieldType_DATE:
			strValue = _connection->WriteStmtDate( value );
			SDatabaseParameterValueSetter< CDate >()( _value, strValue );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseParameter::DoSetValue( const CDateTime & value )
	{
		std::string strValue;

		switch ( _fieldType )
		{
		case EFieldType_DATE:
			strValue = _connection->WriteStmtDate( value );
			SDatabaseParameterValueSetter< CDate >()( _value, strValue );
			break;

		case EFieldType_DATETIME:
			strValue = _connection->WriteStmtDateTime( value );
			SDatabaseParameterValueSetter< CDateTime >()( _value, strValue );
			break;

		case EFieldType_TIME:
			strValue = _connection->WriteStmtTime( value );
			SDatabaseParameterValueSetter< CTime >()( _value, strValue );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseParameter::DoSetValue( const CTime & value )
	{
		std::string strValue;

		switch ( GetType() )
		{
		case EFieldType_DATETIME:
		case EFieldType_TIME:
			strValue = _connection->WriteStmtTime( value );
			SDatabaseParameterValueSetter< CTime >()( _value, strValue );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}
}
END_NAMESPACE_DATABASE
