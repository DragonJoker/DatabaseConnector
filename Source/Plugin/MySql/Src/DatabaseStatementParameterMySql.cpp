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
	namespace
	{
		static const String ERROR_PARAMETER_NOT_INITIALIZED = STR( "Parameter is not initialized" );

		template< typename T, typename U >
		struct SValueSetter
		{
			void operator()( SMySqlParameterValueSetterBase & paramSetter, const U & value, MYSQL_STMT * statement, CDatabaseStatementParameterMySql * parameter, CDatabaseValueBase & paramValue )
			{
				T val = T( value );

				if ( statement )
				{
					paramSetter( statement, parameter->GetBinding(), &val, parameter );
					paramValue.SetNull( false );

					if ( parameter->GetParamType() != EParameterType_IN )
					{
						static_cast< CDatabaseValue< SDataTypeFieldTyper< T >::Value > & >( paramValue ).SetValue( val );
					}
				}
				else if ( parameter->GetParamType() == EParameterType_IN )
				{
					DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, ERROR_PARAMETER_NOT_INITIALIZED );
				}
				else
				{
					static_cast< CDatabaseValue< SDataTypeFieldTyper< T >::Value > & >( paramValue ).SetValue( val );
				}
			}
		};

		template< typename T >
		struct SValueSetter< T, T >
		{
			void operator()( SMySqlParameterValueSetterBase & paramSetter, const T & value, MYSQL_STMT * statement, CDatabaseStatementParameterMySql * parameter, CDatabaseValueBase & paramValue )
			{
				if ( statement )
				{
					paramSetter( statement, parameter->GetBinding(), &value, parameter );
					paramValue.SetNull( false );

					if ( parameter->GetParamType() != EParameterType_IN )
					{
						static_cast< CDatabaseValue< SDataTypeFieldTyper< T >::Value > & >( paramValue ).SetValue( value );
					}
				}
				else if ( parameter->GetParamType() == EParameterType_IN )
				{
					DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, ERROR_PARAMETER_NOT_INITIALIZED );
				}
				else
				{
					static_cast< CDatabaseValue< SDataTypeFieldTyper< T >::Value > & >( paramValue ).SetValue( value );
				}
			}
		};

		template< typename T >
		struct SValueSetter< T *, T * >
		{
			void operator()( SMySqlParameterValueSetterBase & paramSetter, const T * value, MYSQL_STMT * statement, CDatabaseStatementParameterMySql * parameter, CDatabaseValueBase & paramValue )
			{
				if ( statement )
				{
					paramSetter( statement, parameter->GetBinding(), reinterpret_cast< const void * >( value ), parameter );
					paramValue.SetNull( false );

					if ( parameter->GetParamType() != EParameterType_IN )
					{
						static_cast< CDatabaseValue< SDataTypeFieldTyper< T * >::Value > & >( paramValue ).SetValue( value, parameter->GetObjectValue().GetPtrSize() );
					}
				}
				else if ( parameter->GetParamType() == EParameterType_IN )
				{
					DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, ERROR_PARAMETER_NOT_INITIALIZED );
				}
				else
				{
					static_cast< CDatabaseValue< SDataTypeFieldTyper< T * >::Value > & >( paramValue ).SetValue( value, parameter->GetObjectValue().GetPtrSize() );
				}
			}
		};

		template< typename T >
		struct SValueSetter< T, bool >
		{
			void operator()( SMySqlParameterValueSetterBase & paramSetter, const bool & value, MYSQL_STMT * statement, CDatabaseStatementParameterMySql * parameter, CDatabaseValueBase & paramValue )
			{
				T val = value ? 1 : 0;

				if ( statement )
				{
					paramSetter( statement, parameter->GetBinding(), &value, parameter );
					paramValue.SetNull( false );

					if ( parameter->GetParamType() != EParameterType_IN )
					{
						static_cast< CDatabaseValue< SDataTypeFieldTyper< T >::Value > & >( paramValue ).SetValue( val );
					}
				}
				else if ( parameter->GetParamType() == EParameterType_IN )
				{
					DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, ERROR_PARAMETER_NOT_INITIALIZED );
				}
				else
				{
					static_cast< CDatabaseValue< SDataTypeFieldTyper< T >::Value > & >( paramValue ).SetValue( val );
				}
			}
		};

		template< typename T >
		struct SValueSetter< bool, T >
		{
			void operator()( SMySqlParameterValueSetterBase & paramSetter, const T & value, MYSQL_STMT * statement, CDatabaseStatementParameterMySql * parameter, CDatabaseValueBase & paramValue )
			{
				bool val = value != 0;

				if ( statement )
				{
					paramSetter( statement, parameter->GetBinding(), &value, parameter );
					paramValue.SetNull( false );

					if ( parameter->GetParamType() != EParameterType_IN )
					{
						static_cast< CDatabaseValue< SDataTypeFieldTyper< bool >::Value > & >( paramValue ).SetValue( val );
					}
				}
				else if ( parameter->GetParamType() == EParameterType_IN )
				{
					DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, ERROR_PARAMETER_NOT_INITIALIZED );
				}
				else
				{
					static_cast< CDatabaseValue< SDataTypeFieldTyper< bool >::Value > & >( paramValue ).SetValue( val );
				}
			}
		};

		template<>
		struct SValueSetter< bool, bool >
		{
			void operator()( SMySqlParameterValueSetterBase & paramSetter, const bool & value, MYSQL_STMT * statement, CDatabaseStatementParameterMySql * parameter, CDatabaseValueBase & paramValue )
			{
				if ( statement )
				{
					paramSetter( statement, parameter->GetBinding(), &value, parameter );
					paramValue.SetNull( false );

					if ( parameter->GetParamType() != EParameterType_IN )
					{
						static_cast< CDatabaseValue< SDataTypeFieldTyper< bool >::Value > & >( paramValue ).SetValue( value );
					}
				}
				else if ( parameter->GetParamType() == EParameterType_IN )
				{
					DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, ERROR_PARAMETER_NOT_INITIALIZED );
				}
				else
				{
					static_cast< CDatabaseValue< SDataTypeFieldTyper< bool >::Value > & >( paramValue ).SetValue( value );
				}
			}
		};

		template< typename T >
		struct SValueSetter< const char *, T >
		{
			void operator()( SMySqlParameterValueSetterBase & paramSetter, const T & value, MYSQL_STMT * statement, CDatabaseStatementParameterMySql * parameter, CDatabaseValueBase & paramValue )
			{
				std::string val = std::to_string( value );

				if ( statement )
				{
					paramSetter( statement, parameter->GetBinding(), reinterpret_cast< const void * >( value ), parameter );
					paramValue.SetNull( false );

					if ( parameter->GetParamType() != EParameterType_IN )
					{
						if ( parameter->GetType() == EFieldType_VARCHAR )
						{
							static_cast< CDatabaseValue< SDataTypeFieldTyper< char * >::Value > & >( paramValue ).SetValue( val.data(), uint32_t( val.size() ) );
						}
						else
						{
							static_cast< CDatabaseValue< SDataTypeFieldTyper< std::string >::Value > & >( paramValue ).SetValue( val );
						}
					}
				}
				else if ( parameter->GetParamType() == EParameterType_IN )
				{
					DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, ERROR_PARAMETER_NOT_INITIALIZED );
				}
				else
				{
					if ( parameter->GetType() == EFieldType_VARCHAR )
					{
						static_cast< CDatabaseValue< SDataTypeFieldTyper< char * >::Value > & >( paramValue ).SetValue( val.data(), uint32_t( val.size() ) );
					}
					else
					{
						static_cast< CDatabaseValue< SDataTypeFieldTyper< std::string >::Value > & >( paramValue ).SetValue( val );
					}
				}
			}
		};

		template< typename T >
		struct SValueSetter< const wchar_t *, T >
		{
			void operator()( SMySqlParameterValueSetterBase & paramSetter, const T & value, MYSQL_STMT * statement, CDatabaseStatementParameterMySql * parameter, CDatabaseValueBase & paramValue )
			{
				std::wstring val = CStrUtils::ToWStr( std::to_string( value ) );

				if ( statement )
				{
					paramSetter( statement, parameter->GetBinding(), reinterpret_cast< const void * >( value ), parameter );
					paramValue.SetNull( false );

					if ( parameter->GetParamType() != EParameterType_IN )
					{
						if ( parameter->GetType() == EFieldType_NVARCHAR )
						{
							static_cast< CDatabaseValue< SDataTypeFieldTyper< wchar_t * >::Value > & >( paramValue ).SetValue( val.data(), uint32_t( val.size() ) );
						}
						else
						{
							static_cast< CDatabaseValue< SDataTypeFieldTyper< std::wstring >::Value > & >( paramValue ).SetValue( val );
						}
					}
				}
				else if ( parameter->GetParamType() == EParameterType_IN )
				{
					DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, ERROR_PARAMETER_NOT_INITIALIZED );
				}
				else
				{
					if ( parameter->GetType() == EFieldType_NVARCHAR )
					{
						static_cast< CDatabaseValue< SDataTypeFieldTyper< wchar_t * >::Value > & >( paramValue ).SetValue( val.data(), uint32_t( val.size() ) );
					}
					else
					{
						static_cast< CDatabaseValue< SDataTypeFieldTyper< std::wstring >::Value > & >( paramValue ).SetValue( val );
					}
				}
			}
		};

		template<>
		struct SValueSetter< const char *, const char * >
		{
			void operator()( SMySqlParameterValueSetterBase & paramSetter, const char * value, MYSQL_STMT * statement, CDatabaseStatementParameterMySql * parameter, CDatabaseValueBase & paramValue )
			{
				if ( statement )
				{
					paramSetter( statement, parameter->GetBinding(), reinterpret_cast< const void * >( value ), parameter );
					paramValue.SetNull( false );

					if ( parameter->GetParamType() != EParameterType_IN )
					{
						if ( parameter->GetType() == EFieldType_VARCHAR )
						{
							static_cast< CDatabaseValue< SDataTypeFieldTyper< char * >::Value > & >( paramValue ).SetValue( value );
						}
						else
						{
							static_cast< CDatabaseValue< SDataTypeFieldTyper< std::string >::Value > & >( paramValue ).SetValue( value );
						}
					}
				}
				else if ( parameter->GetParamType() == EParameterType_IN )
				{
					DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, ERROR_PARAMETER_NOT_INITIALIZED );
				}
				else
				{
					if ( parameter->GetType() == EFieldType_VARCHAR )
					{
						static_cast< CDatabaseValue< SDataTypeFieldTyper< char * >::Value > & >( paramValue ).SetValue( value );
					}
					else
					{
						static_cast< CDatabaseValue< SDataTypeFieldTyper< std::string >::Value > & >( paramValue ).SetValue( value );
					}
				}
			}
		};

		template<>
		struct SValueSetter< const char *, const wchar_t * >
		{
			void operator()( SMySqlParameterValueSetterBase & paramSetter, const wchar_t * value, MYSQL_STMT * statement, CDatabaseStatementParameterMySql * parameter, CDatabaseValueBase & paramValue )
			{
				if ( statement )
				{
					paramSetter( statement, parameter->GetBinding(), reinterpret_cast< const void * >( value ), parameter );
					paramValue.SetNull( false );

					if ( parameter->GetParamType() != EParameterType_IN )
					{
						if ( parameter->GetType() == EFieldType_NVARCHAR )
						{
							static_cast< CDatabaseValue< SDataTypeFieldTyper< wchar_t * >::Value > & >( paramValue ).SetValue( value, parameter->GetLimits() );
						}
						else
						{
							static_cast< CDatabaseValue< SDataTypeFieldTyper< std::wstring >::Value > & >( paramValue ).SetValue( value );
						}
					}
				}
				else if ( parameter->GetParamType() == EParameterType_IN )
				{
					DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, ERROR_PARAMETER_NOT_INITIALIZED );
				}
				else
				{
					if ( parameter->GetType() == EFieldType_NVARCHAR )
					{
						static_cast< CDatabaseValue< SDataTypeFieldTyper< wchar_t * >::Value > & >( paramValue ).SetValue( value, parameter->GetLimits() );
					}
					else
					{
						static_cast< CDatabaseValue< SDataTypeFieldTyper< std::wstring >::Value > & >( paramValue ).SetValue( value );
					}
				}
			}
		};

		template< typename T > std::unique_ptr< CMySqlBindBase > GetOutBind( MYSQL_BIND & bind )
		{
			return std::make_unique< COutMySqlBind< T > >( bind );
		}
	}

	static const String MYSQL_DescribeParam_MSG = STR( "SQLDescribeParam" );
	static const String DATABASE_PARAMETER_TYPE_ERROR = STR( "Wrong parameter type when trying to set its value." );

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
		_setter.reset();
	}

	void CDatabaseStatementParameterMySql::SetNull()
	{
		if ( _statement )
		{
			_binding->_null = true;
		}

		CDatabaseParameter::SetNull();
		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterMySql::SetValue( DatabaseParameterPtr parameter )
	{
		if ( !AreTypesCompatible( GetType(), parameter->GetType() ) )
		{
			String errMsg = DATABASE_PARAMETER_TYPE_ERROR + this->GetName();
			CLogger::LogError( errMsg );
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, DATABASE_PARAMETER_TYPE_ERROR );
		}

		switch ( parameter->GetType() )
		{
		case EFieldType_BOOL:
			DoSetValue( *static_cast< bool * >( parameter->GetObjectValue().GetPtrValue() ) );
			break;

		case EFieldType_SMALL_INTEGER:
			DoSetValue( *static_cast< int16_t * >( parameter->GetObjectValue().GetPtrValue() ) );
			break;

		case EFieldType_INTEGER:
			DoSetValue( *static_cast< int32_t * >( parameter->GetObjectValue().GetPtrValue() ) );
			break;

		case EFieldType_LONG_INTEGER:
			DoSetValue( *static_cast< int64_t * >( parameter->GetObjectValue().GetPtrValue() ) );
			break;

		case EFieldType_FLOAT:
			DoSetValue( *static_cast< float * >( parameter->GetObjectValue().GetPtrValue() ) );
			break;

		case EFieldType_DOUBLE:
			DoSetValue( *static_cast< double * >( parameter->GetObjectValue().GetPtrValue() ) );
			break;

		case EFieldType_VARCHAR:
			DoSetValue( static_cast< char * >( parameter->GetObjectValue().GetPtrValue() ) );
			break;

		case EFieldType_TEXT:
			DoSetValue( static_cast< char * >( parameter->GetObjectValue().GetPtrValue() ) );
			break;

		case EFieldType_NVARCHAR:
			DoSetValue( static_cast< wchar_t * >( parameter->GetObjectValue().GetPtrValue() ) );
			break;

		case EFieldType_NTEXT:
			DoSetValue( static_cast< wchar_t * >( parameter->GetObjectValue().GetPtrValue() ) );
			break;

		case EFieldType_DATE:
			DoSetValue( GetConnection()->ParseDate( static_cast< char * >( parameter->GetObjectValue().GetPtrValue() ) ) );
			break;

		case EFieldType_DATETIME:
			DoSetValue( GetConnection()->ParseDateTime( static_cast< char * >( parameter->GetObjectValue().GetPtrValue() ) ) );
			break;

		case EFieldType_TIME:
			DoSetValue( GetConnection()->ParseTime( static_cast< char * >( parameter->GetObjectValue().GetPtrValue() ) ) );
			break;

		case EFieldType_BINARY:
		case EFieldType_VARBINARY:
		case EFieldType_LONG_VARBINARY:
		{
			DoSetBlob( static_cast< uint8_t * >( parameter->GetObjectValue().GetPtrValue() ), parameter->GetObjectValue().GetPtrSize() );
		}
		break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterMySql::SetBinding( MYSQL_BIND * bind )
	{
		switch ( GetType() )
		{
		case EFieldType_BOOL:
			_setter = std::make_unique< SMySqlParameterValueSetter< EFieldType_BOOL > >();
			_binding = std::make_unique< COutMySqlBind< bool > >( *bind, SFieldTypeMySqlDataTyper< EFieldType_BOOL >::Value, static_cast< CDatabaseValue< EFieldType_BOOL > & >( GetObjectValue() ).GetTypedPtrValue() );
			break;

		case EFieldType_SMALL_INTEGER:
			_setter = std::make_unique< SMySqlParameterValueSetter< EFieldType_SMALL_INTEGER > >();
			_binding = std::make_unique< COutMySqlBind< int16_t > >( *bind, SFieldTypeMySqlDataTyper< EFieldType_SMALL_INTEGER >::Value, static_cast< CDatabaseValue< EFieldType_SMALL_INTEGER > & >( GetObjectValue() ).GetTypedPtrValue() );
			break;

		case EFieldType_INTEGER:
			_setter = std::make_unique< SMySqlParameterValueSetter< EFieldType_INTEGER > >();
			_binding = std::make_unique< COutMySqlBind< int32_t > >( *bind, SFieldTypeMySqlDataTyper< EFieldType_INTEGER >::Value, static_cast< CDatabaseValue< EFieldType_INTEGER > & >( GetObjectValue() ).GetTypedPtrValue() );
			break;

		case EFieldType_LONG_INTEGER:
			_setter = std::make_unique< SMySqlParameterValueSetter< EFieldType_LONG_INTEGER > >();
			_binding = std::make_unique< COutMySqlBind< int64_t > >( *bind, SFieldTypeMySqlDataTyper< EFieldType_LONG_INTEGER >::Value, static_cast< CDatabaseValue< EFieldType_LONG_INTEGER > & >( GetObjectValue() ).GetTypedPtrValue() );
			break;

		case EFieldType_FLOAT:
			_setter = std::make_unique< SMySqlParameterValueSetter< EFieldType_FLOAT > >();
			_binding = std::make_unique< COutMySqlBind< float > >( *bind, SFieldTypeMySqlDataTyper< EFieldType_FLOAT >::Value, static_cast< CDatabaseValue< EFieldType_FLOAT > & >( GetObjectValue() ).GetTypedPtrValue() );
			break;

		case EFieldType_DOUBLE:
			_setter = std::make_unique< SMySqlParameterValueSetter< EFieldType_DOUBLE > >();
			_binding = std::make_unique< COutMySqlBind< double > >( *bind, SFieldTypeMySqlDataTyper< EFieldType_DOUBLE >::Value, static_cast< CDatabaseValue< EFieldType_DOUBLE > & >( GetObjectValue() ).GetTypedPtrValue() );
			break;

		case EFieldType_VARCHAR:
			_setter = std::make_unique< SMySqlParameterValueSetter< EFieldType_VARCHAR > >();
			_binding = std::make_unique< COutMySqlBind< char * > >( *bind, SFieldTypeMySqlDataTyper< EFieldType_VARCHAR >::Value, static_cast< CDatabaseValue< EFieldType_VARCHAR > & >( GetObjectValue() ).GetTypedPtrValue() );
			break;

		case EFieldType_TEXT:
			_setter = std::make_unique< SMySqlParameterValueSetter< EFieldType_TEXT > >();
			_binding = std::make_unique< COutMySqlBind< char * > >( *bind, SFieldTypeMySqlDataTyper< EFieldType_TEXT >::Value, static_cast< CDatabaseValue< EFieldType_TEXT > & >( GetObjectValue() ).GetTypedPtrValue() );
			break;

		case EFieldType_NVARCHAR:
			_setter = std::make_unique< SMySqlParameterValueSetter< EFieldType_NVARCHAR > >();
			_binding = std::make_unique< COutMySqlBind< wchar_t * > >( *bind, SFieldTypeMySqlDataTyper< EFieldType_NVARCHAR >::Value, static_cast< CDatabaseValue< EFieldType_NVARCHAR > & >( GetObjectValue() ).GetTypedPtrValue() );
			break;

		case EFieldType_NTEXT:
			_setter = std::make_unique< SMySqlParameterValueSetter< EFieldType_NTEXT > >();
			_binding = std::make_unique< COutMySqlBind< wchar_t * > >( *bind, SFieldTypeMySqlDataTyper< EFieldType_NTEXT >::Value, static_cast< CDatabaseValue< EFieldType_NTEXT > & >( GetObjectValue() ).GetTypedPtrValue() );
			break;

		case EFieldType_DATE:
			_setter = std::make_unique< SMySqlParameterValueSetter< EFieldType_DATE > >();
			_binding = std::make_unique< COutMySqlBind< CDate > >( *bind, SFieldTypeMySqlDataTyper< EFieldType_DATE >::Value, static_cast< CDatabaseValue< EFieldType_DATE > & >( GetObjectValue() ).GetTypedPtrValue() );
			break;

		case EFieldType_DATETIME:
			_setter = std::make_unique< SMySqlParameterValueSetter< EFieldType_DATETIME > >();
			_binding = std::make_unique< COutMySqlBind< CDateTime > >( *bind, SFieldTypeMySqlDataTyper< EFieldType_DATETIME >::Value, static_cast< CDatabaseValue< EFieldType_DATETIME > & >( GetObjectValue() ).GetTypedPtrValue() );
			break;

		case EFieldType_TIME:
			_setter = std::make_unique< SMySqlParameterValueSetter< EFieldType_TIME > >();
			_binding = std::make_unique< COutMySqlBind< CTime > >( *bind, SFieldTypeMySqlDataTyper< EFieldType_TIME >::Value, static_cast< CDatabaseValue< EFieldType_TIME > & >( GetObjectValue() ).GetTypedPtrValue() );
			break;

		case EFieldType_BINARY:
			_setter = std::make_unique< SMySqlParameterValueSetter< EFieldType_BINARY > >();
			_binding = std::make_unique< COutMySqlBind< uint8_t * > >( *bind, SFieldTypeMySqlDataTyper< EFieldType_BINARY >::Value, static_cast< CDatabaseValue< EFieldType_BINARY > & >( GetObjectValue() ).GetTypedPtrValue() );
			break;

		case EFieldType_VARBINARY:
			_setter = std::make_unique< SMySqlParameterValueSetter< EFieldType_VARBINARY > >();
			_binding = std::make_unique< COutMySqlBind< uint8_t * > >( *bind, SFieldTypeMySqlDataTyper< EFieldType_VARBINARY >::Value, static_cast< CDatabaseValue< EFieldType_VARBINARY > & >( GetObjectValue() ).GetTypedPtrValue() );
			break;

		case EFieldType_LONG_VARBINARY:
			_setter = std::make_unique< SMySqlParameterValueSetter< EFieldType_LONG_VARBINARY > >();
			_binding = std::make_unique< COutMySqlBind< uint8_t * > >( *bind, SFieldTypeMySqlDataTyper< EFieldType_LONG_VARBINARY >::Value, static_cast< CDatabaseValue< EFieldType_LONG_VARBINARY > & >( GetObjectValue() ).GetTypedPtrValue() );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}

		_binding->_bind.length = const_cast< unsigned long * >( &GetObjectValue().GetPtrSize() );
	}

	MYSQL_BIND * CDatabaseStatementParameterMySql::GetBinding()const
	{
		return &_binding->_bind;
	}

	void CDatabaseStatementParameterMySql::DoSetValue( const bool & value )
	{
		CDatabaseValuedObject::DoSetValue( value );

		switch ( GetType() )
		{
		case EFieldType_BOOL:
			SValueSetter< bool, bool >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_SMALL_INTEGER:
			SValueSetter< int16_t, bool >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_INTEGER:
			SValueSetter< int32_t, bool >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_LONG_INTEGER:
			SValueSetter< int64_t, bool >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterMySql::DoSetValue( const int16_t & value )
	{
		CDatabaseValuedObject::DoSetValue( value );

		switch ( GetType() )
		{
		case EFieldType_BOOL:
			SValueSetter< bool, int16_t >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_SMALL_INTEGER:
			SValueSetter< int16_t, int16_t >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_INTEGER:
			SValueSetter< int32_t, int16_t >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_LONG_INTEGER:
			SValueSetter< int64_t, int16_t >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_FLOAT:
			SValueSetter< float, int16_t >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_DOUBLE:
			SValueSetter< double, int16_t >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterMySql::DoSetValue( const uint16_t & value )
	{
		CDatabaseValuedObject::DoSetValue( value );

		switch ( GetType() )
		{
		case EFieldType_BOOL:
			SValueSetter< bool, uint16_t >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_SMALL_INTEGER:
			SValueSetter< int16_t, uint16_t >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_INTEGER:
			SValueSetter< int32_t, uint16_t >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_LONG_INTEGER:
			SValueSetter< int64_t, uint16_t >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_FLOAT:
			SValueSetter< float, uint16_t >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_DOUBLE:
			SValueSetter< double, uint16_t >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterMySql::DoSetValue( const int32_t & value )
	{
		CDatabaseValuedObject::DoSetValue( value );

		switch ( GetType() )
		{
		case EFieldType_BOOL:
			SValueSetter< bool, int >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_INTEGER:
			SValueSetter< int32_t, int >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_LONG_INTEGER:
			SValueSetter< int64_t, int >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_FLOAT:
			SValueSetter< float, int >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_DOUBLE:
			SValueSetter< double, int >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterMySql::DoSetValue( const uint32_t & value )
	{
		CDatabaseValuedObject::DoSetValue( value );

		switch ( GetType() )
		{
		case EFieldType_BOOL:
			SValueSetter< bool, unsigned int >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_INTEGER:
			SValueSetter< int32_t, unsigned int >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_LONG_INTEGER:
			SValueSetter< int64_t, unsigned int >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_FLOAT:
			SValueSetter< float, unsigned int >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_DOUBLE:
			SValueSetter< double, unsigned int >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterMySql::DoSetValue( const int64_t & value )
	{
		CDatabaseValuedObject::DoSetValue( value );

		switch ( GetType() )
		{
		case EFieldType_BOOL:
			SValueSetter< bool, int64_t >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_LONG_INTEGER:
			SValueSetter< int64_t, int64_t >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_DOUBLE:
			SValueSetter< double, int64_t >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterMySql::DoSetValue( const uint64_t & value )
	{
		CDatabaseValuedObject::DoSetValue( value );

		switch ( GetType() )
		{
		case EFieldType_BOOL:
			SValueSetter< bool, uint64_t >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_LONG_INTEGER:
			SValueSetter< int64_t, uint64_t >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_DOUBLE:
			SValueSetter< double, uint64_t >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterMySql::DoSetValue( const float & value )
	{
		CDatabaseValuedObject::DoSetValue( value );

		switch ( GetType() )
		{
		case EFieldType_SMALL_INTEGER:
			SValueSetter< int16_t, float >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_INTEGER:
			SValueSetter< int32_t, float >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_LONG_INTEGER:
			SValueSetter< int64_t, float >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_FLOAT:
			SValueSetter< float, float >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_DOUBLE:
			SValueSetter< double, float >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterMySql::DoSetValue( const double & value )
	{
		CDatabaseValuedObject::DoSetValue( value );

		switch ( GetType() )
		{
		case EFieldType_SMALL_INTEGER:
			SValueSetter< int16_t, double >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_INTEGER:
			SValueSetter< int32_t, double >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_LONG_INTEGER:
			SValueSetter< int64_t, double >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_FLOAT:
			SValueSetter< float, double >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_DOUBLE:
			SValueSetter< double, double >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterMySql::DoSetValue( const long double & value )
	{
		CDatabaseValuedObject::DoSetValue( value );

		switch ( GetType() )
		{
		case EFieldType_SMALL_INTEGER:
			SValueSetter< int16_t, long double >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_INTEGER:
			SValueSetter< int32_t, long double >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_LONG_INTEGER:
			SValueSetter< int64_t, long double >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_FLOAT:
			SValueSetter< float, long double >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_DOUBLE:
			SValueSetter< double, long double >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterMySql::DoSetValue( const char * value )
	{
		CDatabaseValuedObject::DoSetValue( value );

		switch ( GetType() )
		{
		case EFieldType_VARCHAR:
		case EFieldType_TEXT:
		case EFieldType_NVARCHAR:
		case EFieldType_NTEXT:
			SValueSetter< const char *, const char * >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterMySql::DoSetValue( const wchar_t * value )
	{
		CDatabaseValuedObject::DoSetValue( value );

		switch ( GetType() )
		{
		case EFieldType_VARCHAR:
		case EFieldType_TEXT:
		case EFieldType_NVARCHAR:
		case EFieldType_NTEXT:
			SValueSetter< const char *, const wchar_t * >()( *_setter, value, _statement, this, GetObjectValue() );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterMySql::DoSetValue( const std::string & value )
	{
		CDatabaseValuedObject::DoSetValue( value );

		switch ( GetType() )
		{
		case EFieldType_VARCHAR:
		case EFieldType_TEXT:
		case EFieldType_NVARCHAR:
		case EFieldType_NTEXT:
			SValueSetter< const char *, const char * >()( *_setter, value.c_str(), _statement, this, GetObjectValue() );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterMySql::DoSetValue( const std::wstring & value )
	{
		CDatabaseValuedObject::DoSetValue( value );

		switch ( GetType() )
		{
		case EFieldType_VARCHAR:
		case EFieldType_TEXT:
		case EFieldType_NVARCHAR:
		case EFieldType_NTEXT:
			SValueSetter< const char *, const wchar_t * >()( *_setter, value.c_str(), _statement, this, GetObjectValue() );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterMySql::DoSetValue( const std::vector< uint8_t > & value )
	{
		CDatabaseValuedObject::DoSetValue( value );

		if ( GetType() == EFieldType_BINARY || GetType() == EFieldType_VARBINARY || GetType() == EFieldType_LONG_VARBINARY )
		{
			if ( !value.empty() )
			{
				SValueSetter< uint8_t *, uint8_t * >()( *_setter, value.data(), _statement, this, GetObjectValue() );
			}
			else
			{
				SetNull();
			}
		}
		else
		{
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterMySql::DoSetValue( const CDateTime & value )
	{
		CDatabaseValuedObject::DoSetValue( value );
		( *_setter )( _statement, GetBinding(), GetObjectValue().GetPtrValue(), this );
		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterMySql::DoSetValue( const CDate & value )
	{
		CDatabaseValuedObject::DoSetValue( value );
		( *_setter )( _statement, GetBinding(), GetObjectValue().GetPtrValue(), this );
		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterMySql::DoSetValue( const CTime & value )
	{
		CDatabaseValuedObject::DoSetValue( value );
		( *_setter )( _statement, GetBinding(), GetObjectValue().GetPtrValue(), this );
		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterMySql::DoSetValueFast( const bool & value )
	{
		SValueSetter< bool, bool >()( *_setter, value, _statement, this, GetObjectValue() );
		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterMySql::DoSetValueFast( const int16_t & value )
	{
		SValueSetter< int16_t, int16_t >()( *_setter, value, _statement, this, GetObjectValue() );
		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterMySql::DoSetValueFast( const uint16_t & value )
	{
		SValueSetter< int16_t, uint16_t >()( *_setter, value, _statement, this, GetObjectValue() );
		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterMySql::DoSetValueFast( const int32_t & value )
	{
		SValueSetter< int32_t, int32_t >()( *_setter, value, _statement, this, GetObjectValue() );
		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterMySql::DoSetValueFast( const uint32_t & value )
	{
		SValueSetter< int32_t, uint32_t >()( *_setter, value, _statement, this, GetObjectValue() );
		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterMySql::DoSetValueFast( const int64_t & value )
	{
		SValueSetter< int64_t, int64_t >()( *_setter, value, _statement, this, GetObjectValue() );
		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterMySql::DoSetValueFast( const uint64_t & value )
	{
		SValueSetter< int64_t, uint64_t >()( *_setter, value, _statement, this, GetObjectValue() );
		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterMySql::DoSetValueFast( const float & value )
	{
		SValueSetter< float, float >()( *_setter, value, _statement, this, GetObjectValue() );
		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterMySql::DoSetValueFast( const double & value )
	{
		SValueSetter< double, double >()( *_setter, value, _statement, this, GetObjectValue() );
		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterMySql::DoSetValueFast( const long double & value )
	{
		SValueSetter< double, long double >()( *_setter, value, _statement, this, GetObjectValue() );
		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterMySql::DoSetValueFast( const char * value )
	{
		SValueSetter< const char *, const char * >()( *_setter, value, _statement, this, GetObjectValue() );
		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterMySql::DoSetValueFast( const wchar_t * value )
	{
		SValueSetter< const char *, const wchar_t * >()( *_setter, value, _statement, this, GetObjectValue() );
		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterMySql::DoSetValueFast( const std::string & value )
	{
		SValueSetter< const char *, const char * >()( *_setter, value.c_str(), _statement, this, GetObjectValue() );
		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterMySql::DoSetValueFast( const std::wstring & value )
	{
		SValueSetter< const char *, const wchar_t * >()( *_setter, value.c_str(), _statement, this, GetObjectValue() );
		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterMySql::DoSetValueFast( const std::vector< uint8_t > & value )
	{
		if ( GetType() == EFieldType_BINARY || GetType() == EFieldType_VARBINARY || GetType() == EFieldType_LONG_VARBINARY )
		{
			if ( !value.empty() )
			{
				SValueSetter< uint8_t *, uint8_t * >()( *_setter, value.data(), _statement, this, GetObjectValue() );
			}
			else
			{
				SetNull();
			}
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterMySql::DoSetValueFast( const CDateTime & value )
	{
		CDatabaseValuedObject::DoSetValueFast( value );
		( *_setter )( _statement, GetBinding(), GetObjectValue().GetPtrValue(), this );
		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterMySql::DoSetValueFast( const CDate & value )
	{
		CDatabaseValuedObject::DoSetValueFast( value );
		( *_setter )( _statement, GetBinding(), GetObjectValue().GetPtrValue(), this );
		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterMySql::DoSetValueFast( const CTime & value )
	{
		CDatabaseValuedObject::DoSetValueFast( value );
		( *_setter )( _statement, GetBinding(), GetObjectValue().GetPtrValue(), this );
		_updater->Update( shared_from_this() );
	}
}
END_NAMESPACE_DATABASE_MYSQL
