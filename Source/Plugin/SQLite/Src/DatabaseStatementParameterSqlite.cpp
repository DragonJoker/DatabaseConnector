/************************************************************************//**
 * @file DatabaseStatementParameterSqlite.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 *
 *
 * @brief CDatabaseStatementParameterSqlite class declaration.
 *
 * @details Describes a statement parameter for SQLITE database.
 *
 ***************************************************************************/

#include "DatabaseSqlitePch.h"

#include "DatabaseStatementParameterSqlite.h"

#include "DatabaseConnectionSqlite.h"
#include "DatabaseStatementParameterSetter.h"

#include <DatabaseException.h>
#include <DatabaseParameterValueSetter.h>

#include <DatabaseLogger.h>
#include <DatabaseStringUtils.h>

BEGIN_NAMESPACE_DATABASE_SQLITE
{
	namespace
	{
		static const String ERROR_PARAMETER_NOT_INITIALIZED = STR( "Parameter is not initialized" );

		template< typename T, typename U >
		struct SValueSetter
		{
			void operator()( SParameterValueSetterBase * paramSetter, U & value, SQLite::Statement * statement, CDatabaseStatementParameterSqlite * parameter, CDatabaseValueBase * paramValue )
			{
				T val = T( value );

				if ( statement )
				{
					( *paramSetter )( statement, &val, parameter );
					paramValue->SetNull( false );

					if ( parameter->GetParamType() != EParameterType_IN )
					{
						static_cast< CDatabaseValue< SDataTypeFieldTyper< T >::Value > * >( paramValue )->SetValue( val );
					}
				}
				else if ( parameter->GetParamType() == EParameterType_IN )
				{
					DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, ERROR_PARAMETER_NOT_INITIALIZED );
				}
				else
				{
					static_cast< CDatabaseValue< SDataTypeFieldTyper< T >::Value > * >( paramValue )->SetValue( val );
				}
			}
		};

		template< typename T >
		struct SValueSetter< T, T >
		{
			void operator()( SParameterValueSetterBase * paramSetter, T & value, SQLite::Statement * statement, CDatabaseStatementParameterSqlite * parameter, CDatabaseValueBase * paramValue )
			{
				if ( statement )
				{
					( *paramSetter )( statement, &value, parameter );
					paramValue->SetNull( false );

					if ( parameter->GetParamType() != EParameterType_IN )
					{
						static_cast< CDatabaseValue< SDataTypeFieldTyper< T >::Value > * >( paramValue )->SetValue( value );
					}
				}
				else if ( parameter->GetParamType() == EParameterType_IN )
				{
					DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, ERROR_PARAMETER_NOT_INITIALIZED );
				}
				else
				{
					static_cast< CDatabaseValue< SDataTypeFieldTyper< T >::Value > * >( paramValue )->SetValue( value );
				}
			}
		};

		template< typename T >
		struct SValueSetter< T, bool >
		{
			void operator()( SParameterValueSetterBase * paramSetter, bool & value, SQLite::Statement * statement, CDatabaseStatementParameterSqlite * parameter, CDatabaseValueBase * paramValue )
			{
				T val = value ? 1 : 0;

				if ( statement )
				{
					( *paramSetter )( statement, &value, parameter );
					paramValue->SetNull( false );

					if ( parameter->GetParamType() != EParameterType_IN )
					{
						static_cast< CDatabaseValue< SDataTypeFieldTyper< T >::Value > * >( paramValue )->SetValue( val );
					}
				}
				else if ( parameter->GetParamType() == EParameterType_IN )
				{
					DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, ERROR_PARAMETER_NOT_INITIALIZED );
				}
				else
				{
					static_cast< CDatabaseValue< SDataTypeFieldTyper< T >::Value > * >( paramValue )->SetValue( val );
				}
			}
		};

		template< typename T >
		struct SValueSetter< bool, T >
		{
			void operator()( SParameterValueSetterBase * paramSetter, T & value, SQLite::Statement * statement, CDatabaseStatementParameterSqlite * parameter, CDatabaseValueBase * paramValue )
			{
				bool val = value != 0;

				if ( statement )
				{
					( *paramSetter )( statement, &value, parameter );
					paramValue->SetNull( false );

					if ( parameter->GetParamType() != EParameterType_IN )
					{
						static_cast< CDatabaseValue< SDataTypeFieldTyper< bool >::Value > * >( paramValue )->SetValue( val );
					}
				}
				else if ( parameter->GetParamType() == EParameterType_IN )
				{
					DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, ERROR_PARAMETER_NOT_INITIALIZED );
				}
				else
				{
					static_cast< CDatabaseValue< SDataTypeFieldTyper< bool >::Value > * >( paramValue )->SetValue( val );
				}
			}
		};

		template<>
		struct SValueSetter< bool, bool >
		{
			void operator()( SParameterValueSetterBase * paramSetter, bool & value, SQLite::Statement * statement, CDatabaseStatementParameterSqlite * parameter, CDatabaseValueBase * paramValue )
			{
				if ( statement )
				{
					( *paramSetter )( statement, &value, parameter );
					paramValue->SetNull( false );

					if ( parameter->GetParamType() != EParameterType_IN )
					{
						static_cast< CDatabaseValue< SDataTypeFieldTyper< bool >::Value > * >( paramValue )->SetValue( value );
					}
				}
				else if ( parameter->GetParamType() == EParameterType_IN )
				{
					DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, ERROR_PARAMETER_NOT_INITIALIZED );
				}
				else
				{
					static_cast< CDatabaseValue< SDataTypeFieldTyper< bool >::Value > * >( paramValue )->SetValue( value );
				}
			}
		};

		template< typename T >
		struct SValueSetter< const char *, T >
		{
			void operator()( SParameterValueSetterBase * paramSetter, T & value, SQLite::Statement * statement, CDatabaseStatementParameterSqlite * parameter, CDatabaseValueBase * paramValue )
			{
				std::string val = std::to_string( value );

				if ( statement )
				{
					( *paramSetter )( statement, ( void * )value, parameter );
					paramValue->SetNull( false );

					if ( parameter->GetParamType() != EParameterType_IN )
					{
						if ( parameter->GetType() == EFieldType_VARCHAR )
						{
							static_cast< CDatabaseValue< SDataTypeFieldTyper< char * >::Value > * >( paramValue )->SetValue( val.data(), uint32_t( val.size() ) );
						}
						else
						{
							static_cast< CDatabaseValue< SDataTypeFieldTyper< std::string >::Value > * >( paramValue )->SetValue( val );
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
						static_cast< CDatabaseValue< SDataTypeFieldTyper< char * >::Value > * >( paramValue )->SetValue( val.data(), uint32_t( val.size() ) );
					}
					else
					{
						static_cast< CDatabaseValue< SDataTypeFieldTyper< std::string >::Value > * >( paramValue )->SetValue( val );
					}
				}
			}
		};

		template< typename T >
		struct SValueSetter< const wchar_t *, T >
		{
			void operator()( SParameterValueSetterBase * paramSetter, T & value, SQLite::Statement * statement, CDatabaseStatementParameterSqlite * parameter, CDatabaseValueBase * paramValue )
			{
				std::wstring val = CStrUtils::ToWStr( std::to_string( value ) );

				if ( statement )
				{
					( *paramSetter )( statement, ( void * )value, parameter );
					paramValue->SetNull( false );

					if ( parameter->GetParamType() != EParameterType_IN )
					{
						if ( parameter->GetType() == EFieldType_NVARCHAR )
						{
							static_cast< CDatabaseValue< SDataTypeFieldTyper< wchar_t * >::Value > * >( paramValue )->SetValue( val.data(), uint32_t( val.size() ) );
						}
						else
						{
							static_cast< CDatabaseValue< SDataTypeFieldTyper< std::wstring >::Value > * >( paramValue )->SetValue( val );
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
						static_cast< CDatabaseValue< SDataTypeFieldTyper< wchar_t * >::Value > * >( paramValue )->SetValue( val.data(), uint32_t( val.size() ) );
					}
					else
					{
						static_cast< CDatabaseValue< SDataTypeFieldTyper< std::wstring >::Value > * >( paramValue )->SetValue( val );
					}
				}
			}
		};

		template<>
		struct SValueSetter< const char *, const char * >
		{
			void operator()( SParameterValueSetterBase * paramSetter, const char * value, SQLite::Statement * statement, CDatabaseStatementParameterSqlite * parameter, CDatabaseValueBase * paramValue )
			{
				if ( statement )
				{
					( *paramSetter )( statement, ( void * )value, parameter );
					paramValue->SetNull( false );

					if ( parameter->GetParamType() != EParameterType_IN )
					{
						if ( parameter->GetType() == EFieldType_VARCHAR )
						{
							static_cast< CDatabaseValue< SDataTypeFieldTyper< char * >::Value > * >( paramValue )->SetValue( value );
						}
						else
						{
							static_cast< CDatabaseValue< SDataTypeFieldTyper< std::string >::Value > * >( paramValue )->SetValue( value );
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
						static_cast< CDatabaseValue< SDataTypeFieldTyper< char * >::Value > * >( paramValue )->SetValue( value );
					}
					else
					{
						static_cast< CDatabaseValue< SDataTypeFieldTyper< std::string >::Value > * >( paramValue )->SetValue( value );
					}
				}
			}
		};

		template<>
		struct SValueSetter< const char *, const wchar_t * >
		{
			void operator()( SParameterValueSetterBase * paramSetter, const wchar_t * value, SQLite::Statement * statement, CDatabaseStatementParameterSqlite * parameter, CDatabaseValueBase * paramValue )
			{
				std::string val = CStrUtils::ToStr( value );

				if ( statement )
				{
					( *paramSetter )( statement, &val[0], parameter );
					paramValue->SetNull( false );

					if ( parameter->GetParamType() != EParameterType_IN )
					{
						if ( parameter->GetType() == EFieldType_VARCHAR )
						{
							static_cast< CDatabaseValue< SDataTypeFieldTyper< char * >::Value > * >( paramValue )->SetValue( val.c_str(), parameter->GetLimits() );
						}
						else
						{
							static_cast< CDatabaseValue< SDataTypeFieldTyper< std::string >::Value > * >( paramValue )->SetValue( val );
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
						static_cast< CDatabaseValue< SDataTypeFieldTyper< char * >::Value > * >( paramValue )->SetValue( val.c_str(), parameter->GetLimits() );
					}
					else
					{
						static_cast< CDatabaseValue< SDataTypeFieldTyper< std::string >::Value > * >( paramValue )->SetValue( val );
					}
				}
			}
		};
	}

	static const String SQLITE_DescribeParam_MSG = STR( "SQLDescribeParam" );
	static const String DATABASE_PARAMETER_TYPE_ERROR = STR( "Wrong parameter type when trying to set its value." );

	CDatabaseStatementParameterSqlite::CDatabaseStatementParameterSqlite( DatabaseConnectionSqlitePtr connection, const String & name, unsigned short index, EFieldType fieldType, EParameterType parameterType, SValueUpdater * updater )
		: CDatabaseParameter( connection, name, index, fieldType, parameterType, updater )
		, CDatabaseParameterSqlite( fieldType )
		, _paramSetter( NULL )
		, _statement( NULL )
	{
		DoInitializeParamSetter();
	}

	CDatabaseStatementParameterSqlite::CDatabaseStatementParameterSqlite( DatabaseConnectionSqlitePtr connection, const String & name, unsigned short index, EFieldType fieldType, uint32_t limits, EParameterType parameterType, SValueUpdater * updater )
		: CDatabaseParameter( connection, name, index, fieldType, limits, parameterType, updater )
		, CDatabaseParameterSqlite( fieldType )
		, _paramSetter( NULL )
		, _statement( NULL )
	{
		DoInitializeParamSetter();
	}

	CDatabaseStatementParameterSqlite::~CDatabaseStatementParameterSqlite()
	{
		_statement = NULL;
		delete _paramSetter;
	}

	void CDatabaseStatementParameterSqlite::SetNull()
	{
		if ( _statement )
		{
			SQLite::BindNull( _statement, GetIndex() );
		}

		CDatabaseParameter::SetNull();
		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::SetValue( DatabaseParameterPtr parameter )
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
			DoSetValue( *static_cast< bool * >( parameter->GetPtrValue() ) );
			break;

		case EFieldType_SMALL_INTEGER:
			DoSetValue( *static_cast< int16_t * >( parameter->GetPtrValue() ) );
			break;

		case EFieldType_INTEGER:
			DoSetValue( *static_cast< int32_t * >( parameter->GetPtrValue() ) );
			break;

		case EFieldType_LONG_INTEGER:
			DoSetValue( *static_cast< int64_t * >( parameter->GetPtrValue() ) );
			break;

		case EFieldType_FLOAT:
			DoSetValue( *static_cast< float * >( parameter->GetPtrValue() ) );
			break;

		case EFieldType_DOUBLE:
			DoSetValue( *static_cast< double * >( parameter->GetPtrValue() ) );
			break;

		case EFieldType_VARCHAR:
			DoSetValue( static_cast< char * >( parameter->GetPtrValue() ) );
			break;

		case EFieldType_TEXT:
			DoSetValue( static_cast< char * >( parameter->GetPtrValue() ) );
			break;

		case EFieldType_NVARCHAR:
			DoSetValue( static_cast< wchar_t * >( parameter->GetPtrValue() ) );
			break;

		case EFieldType_NTEXT:
			DoSetValue( static_cast< wchar_t * >( parameter->GetPtrValue() ) );
			break;

		case EFieldType_DATE:
			DoSetValue( _connection->ParseDate( static_cast< char * >( parameter->GetPtrValue() ) ) );
			break;

		case EFieldType_DATETIME:
			DoSetValue( _connection->ParseDateTime( static_cast< char * >( parameter->GetPtrValue() ) ) );
			break;

		case EFieldType_TIME:
			DoSetValue( _connection->ParseTime( static_cast< char * >( parameter->GetPtrValue() ) ) );
			break;

		case EFieldType_BINARY:
		case EFieldType_VARBINARY:
		case EFieldType_LONG_VARBINARY:
		{
			std::vector< uint8_t > data;
			data.insert( data.end(), ( uint8_t * )parameter->GetPtrValue(), ( uint8_t * )parameter->GetPtrValue() + parameter->GetPtrSize() );
			DoSetValue( data );

			if ( !data.empty() )
			{
				SQLite::BindBlob( _statement, GetIndex(), data.data(), data.size(), SQLite::NULL_DESTRUCTOR );
			}
			else
			{
				SQLite::BindNull( _statement, GetIndex() );
			}
		}
		break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValue( bool value )
	{
		switch ( GetType() )
		{
		case EFieldType_BOOL:
			SValueSetter< bool, bool >()( _paramSetter, value, _statement, this, _value );
			break;

		case EFieldType_SMALL_INTEGER:
			SValueSetter< int16_t, bool >()( _paramSetter, value, _statement, this, _value );
			break;

		case EFieldType_INTEGER:
			SValueSetter< int32_t, bool >()( _paramSetter, value, _statement, this, _value );
			break;

		case EFieldType_LONG_INTEGER:
			SValueSetter< int64_t, bool >()( _paramSetter, value, _statement, this, _value );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValue( int16_t value )
	{
		switch ( GetType() )
		{
		case EFieldType_BOOL:
			SValueSetter< bool, int16_t >()( _paramSetter, value, _statement, this, _value );
			break;

		case EFieldType_SMALL_INTEGER:
			SValueSetter< int16_t, int16_t >()( _paramSetter, value, _statement, this, _value );
			break;

		case EFieldType_INTEGER:
			SValueSetter< int32_t, int16_t >()( _paramSetter, value, _statement, this, _value );
			break;

		case EFieldType_LONG_INTEGER:
			SValueSetter< int64_t, int16_t >()( _paramSetter, value, _statement, this, _value );
			break;

		case EFieldType_FLOAT:
			SValueSetter< float, int16_t >()( _paramSetter, value, _statement, this, _value );
			break;

		case EFieldType_DOUBLE:
			SValueSetter< double, int16_t >()( _paramSetter, value, _statement, this, _value );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValue( uint16_t value )
	{
		switch ( GetType() )
		{
		case EFieldType_BOOL:
			SValueSetter< bool, uint16_t >()( _paramSetter, value, _statement, this, _value );
			break;

		case EFieldType_SMALL_INTEGER:
			SValueSetter< int16_t, uint16_t >()( _paramSetter, value, _statement, this, _value );
			break;

		case EFieldType_INTEGER:
			SValueSetter< int32_t, uint16_t >()( _paramSetter, value, _statement, this, _value );
			break;

		case EFieldType_LONG_INTEGER:
			SValueSetter< int64_t, uint16_t >()( _paramSetter, value, _statement, this, _value );
			break;

		case EFieldType_FLOAT:
			SValueSetter< float, uint16_t >()( _paramSetter, value, _statement, this, _value );
			break;

		case EFieldType_DOUBLE:
			SValueSetter< double, uint16_t >()( _paramSetter, value, _statement, this, _value );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValue( int32_t value )
	{
		switch ( GetType() )
		{
		case EFieldType_BOOL:
			SValueSetter< bool, int >()( _paramSetter, value, _statement, this, _value );
			break;

		case EFieldType_INTEGER:
			SValueSetter< int32_t, int >()( _paramSetter, value, _statement, this, _value );
			break;

		case EFieldType_LONG_INTEGER:
			SValueSetter< int64_t, int >()( _paramSetter, value, _statement, this, _value );
			break;

		case EFieldType_FLOAT:
			SValueSetter< float, int >()( _paramSetter, value, _statement, this, _value );
			break;

		case EFieldType_DOUBLE:
			SValueSetter< double, int >()( _paramSetter, value, _statement, this, _value );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValue( uint32_t value )
	{
		switch ( GetType() )
		{
		case EFieldType_BOOL:
			SValueSetter< bool, unsigned int >()( _paramSetter, value, _statement, this, _value );
			break;

		case EFieldType_INTEGER:
			SValueSetter< int32_t, unsigned int >()( _paramSetter, value, _statement, this, _value );
			break;

		case EFieldType_LONG_INTEGER:
			SValueSetter< int64_t, unsigned int >()( _paramSetter, value, _statement, this, _value );
			break;

		case EFieldType_FLOAT:
			SValueSetter< float, unsigned int >()( _paramSetter, value, _statement, this, _value );
			break;

		case EFieldType_DOUBLE:
			SValueSetter< double, unsigned int >()( _paramSetter, value, _statement, this, _value );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValue( int64_t value )
	{
		switch ( GetType() )
		{
		case EFieldType_BOOL:
			SValueSetter< bool, int64_t >()( _paramSetter, value, _statement, this, _value );
			break;

		case EFieldType_LONG_INTEGER:
			SValueSetter< int64_t, int64_t >()( _paramSetter, value, _statement, this, _value );
			break;

		case EFieldType_DOUBLE:
			SValueSetter< double, int64_t >()( _paramSetter, value, _statement, this, _value );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValue( uint64_t value )
	{
		switch ( GetType() )
		{
		case EFieldType_BOOL:
			SValueSetter< bool, uint64_t >()( _paramSetter, value, _statement, this, _value );
			break;

		case EFieldType_LONG_INTEGER:
			SValueSetter< int64_t, uint64_t >()( _paramSetter, value, _statement, this, _value );
			break;

		case EFieldType_DOUBLE:
			SValueSetter< double, uint64_t >()( _paramSetter, value, _statement, this, _value );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValue( float value )
	{
		switch ( GetType() )
		{
		case EFieldType_SMALL_INTEGER:
			SValueSetter< int16_t, float >()( _paramSetter, value, _statement, this, _value );
			break;

		case EFieldType_INTEGER:
			SValueSetter< int32_t, float >()( _paramSetter, value, _statement, this, _value );
			break;

		case EFieldType_LONG_INTEGER:
			SValueSetter< int64_t, float >()( _paramSetter, value, _statement, this, _value );
			break;

		case EFieldType_FLOAT:
			SValueSetter< float, float >()( _paramSetter, value, _statement, this, _value );
			break;

		case EFieldType_DOUBLE:
			SValueSetter< double, float >()( _paramSetter, value, _statement, this, _value );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValue( double value )
	{
		switch ( GetType() )
		{
		case EFieldType_SMALL_INTEGER:
			SValueSetter< int16_t, double >()( _paramSetter, value, _statement, this, _value );
			break;

		case EFieldType_INTEGER:
			SValueSetter< int32_t, double >()( _paramSetter, value, _statement, this, _value );
			break;

		case EFieldType_LONG_INTEGER:
			SValueSetter< int64_t, double >()( _paramSetter, value, _statement, this, _value );
			break;

		case EFieldType_FLOAT:
			SValueSetter< float, double >()( _paramSetter, value, _statement, this, _value );
			break;

		case EFieldType_DOUBLE:
			SValueSetter< double, double >()( _paramSetter, value, _statement, this, _value );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValue( long double value )
	{
		switch ( GetType() )
		{
		case EFieldType_SMALL_INTEGER:
			SValueSetter< int16_t, long double >()( _paramSetter, value, _statement, this, _value );
			break;

		case EFieldType_INTEGER:
			SValueSetter< int32_t, long double >()( _paramSetter, value, _statement, this, _value );
			break;

		case EFieldType_LONG_INTEGER:
			SValueSetter< int64_t, long double >()( _paramSetter, value, _statement, this, _value );
			break;

		case EFieldType_FLOAT:
			SValueSetter< float, long double >()( _paramSetter, value, _statement, this, _value );
			break;

		case EFieldType_DOUBLE:
			SValueSetter< double, long double >()( _paramSetter, value, _statement, this, _value );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValue( const char * value )
	{
		switch ( GetType() )
		{
		case EFieldType_VARCHAR:
		case EFieldType_TEXT:
		case EFieldType_NVARCHAR:
		case EFieldType_NTEXT:
			SValueSetter< const char *, const char * >()( _paramSetter, value, _statement, this, _value );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValue( const wchar_t * value )
	{
		switch ( GetType() )
		{
		case EFieldType_VARCHAR:
		case EFieldType_TEXT:
		case EFieldType_NVARCHAR:
		case EFieldType_NTEXT:
			SValueSetter< const char *, const wchar_t * >()( _paramSetter, value, _statement, this, _value );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValue( std::vector< uint8_t > & value )
	{
		if ( GetType() == EFieldType_BINARY || GetType() == EFieldType_VARBINARY || GetType() == EFieldType_LONG_VARBINARY )
		{
			if ( value.empty() )
			{
				SDatabaseParameterValueSetter< uint8_t >()( _value, value.data(), value.size() );
				SQLite::BindBlob( _statement, GetIndex(), value.data(), value.size(), SQLite::NULL_DESTRUCTOR );
			}
			else
			{
				_value->SetNull();
				SQLite::BindNull( _statement, GetIndex() );
			}
		}
		else
		{
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValue( std::istream * value )
	{
		if ( value && GetType() == EFieldType_BINARY || GetType() == EFieldType_VARBINARY || GetType() == EFieldType_LONG_VARBINARY )
		{
			std::noskipws( *value );
			std::vector< uint8_t > in;
			std::copy(
				std::istream_iterator< uint8_t >( *value ),
				std::istream_iterator< uint8_t >(),
				std::back_inserter( in ) );

			if ( in.empty() )
			{
				SDatabaseParameterValueSetter< uint8_t >()( _value, in.data(), in.size() );
				SQLite::BindBlob( _statement, GetIndex(), in.data(), in.size(), SQLite::NULL_DESTRUCTOR );
			}
			else
			{
				_value->SetNull();
				SQLite::BindNull( _statement, GetIndex() );
			}
		}
		else
		{
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValue( const CDateTime & value )
	{
		std::string strValue;

		switch ( _fieldType )
		{
		case EFieldType_DATE:
			strValue = _connection->WriteStmtDate( value );
			SDatabaseParameterValueSetter< CDate >()( _value, strValue );
			( *_paramSetter )( _statement, GetPtrValue(), this );
			break;

		case EFieldType_DATETIME:
			strValue = _connection->WriteStmtDateTime( value );
			SDatabaseParameterValueSetter< CDateTime >()( _value, strValue );
			( *_paramSetter )( _statement, GetPtrValue(), this );
			break;

		case EFieldType_TIME:
			strValue = _connection->WriteStmtTime( value );
			SDatabaseParameterValueSetter< CTime >()( _value, strValue );
			( *_paramSetter )( _statement, GetPtrValue(), this );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValue( const CDate & value )
	{
		std::string strValue;

		switch ( GetType() )
		{
		case EFieldType_DATETIME:
		case EFieldType_DATE:
			strValue = _connection->WriteStmtDate( value );
			SDatabaseParameterValueSetter< CDate >()( _value, strValue );
			( *_paramSetter )( _statement, GetPtrValue(), this );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValue( const CTime & value )
	{
		std::string strValue;

		switch ( GetType() )
		{
		case EFieldType_DATETIME:
		case EFieldType_TIME:
			strValue = _connection->WriteStmtTime( value );
			SDatabaseParameterValueSetter< CTime >()( _value, strValue );
			( *_paramSetter )( _statement, GetPtrValue(), this );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoInitializeParamSetter()
	{
		switch ( GetType() )
		{
		case EFieldType_BOOL:
			_paramSetter = new SParameterValueSetter< EFieldType_BOOL >;
			break;

		case EFieldType_SMALL_INTEGER:
			_paramSetter = new SParameterValueSetter< EFieldType_SMALL_INTEGER >;
			break;

		case EFieldType_INTEGER:
			_paramSetter = new SParameterValueSetter< EFieldType_INTEGER >;
			break;

		case EFieldType_LONG_INTEGER:
			_paramSetter = new SParameterValueSetter< EFieldType_LONG_INTEGER >;
			break;

		case EFieldType_FLOAT:
			_paramSetter = new SParameterValueSetter< EFieldType_FLOAT >;
			break;

		case EFieldType_DOUBLE:
			_paramSetter = new SParameterValueSetter< EFieldType_DOUBLE >;
			break;

		case EFieldType_VARCHAR:
			_paramSetter = new SParameterValueSetter< EFieldType_VARCHAR >;
			break;

		case EFieldType_TEXT:
			_paramSetter = new SParameterValueSetter< EFieldType_TEXT >;
			break;

		case EFieldType_NVARCHAR:
			_paramSetter = new SParameterValueSetter< EFieldType_NVARCHAR >;
			break;

		case EFieldType_NTEXT:
			_paramSetter = new SParameterValueSetter< EFieldType_NTEXT >;
			break;

		case EFieldType_DATE:
			_paramSetter = new SParameterValueSetter< EFieldType_DATE >;
			break;

		case EFieldType_DATETIME:
			_paramSetter = new SParameterValueSetter< EFieldType_DATETIME >;
			break;

		case EFieldType_TIME:
			_paramSetter = new SParameterValueSetter< EFieldType_TIME >;
			break;
		}
	}
}
END_NAMESPACE_DATABASE_SQLITE
