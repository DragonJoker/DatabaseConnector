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
			void operator()( SSqliteParameterValueSetterBase * paramSetter, const U & value, SQLite::Statement * statement, CDatabaseStatementParameterSqlite * parameter, CDatabaseValueBase & paramValue )
			{
				T val = T( value );

				if ( statement )
				{
					( *paramSetter )( statement, &val, parameter );
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
			void operator()( SSqliteParameterValueSetterBase * paramSetter, const T & value, SQLite::Statement * statement, CDatabaseStatementParameterSqlite * parameter, CDatabaseValueBase & paramValue )
			{
				if ( statement )
				{
					( *paramSetter )( statement, &value, parameter );
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
		struct SValueSetter< T, bool >
		{
			void operator()( SSqliteParameterValueSetterBase * paramSetter, const bool & value, SQLite::Statement * statement, CDatabaseStatementParameterSqlite * parameter, CDatabaseValueBase & paramValue )
			{
				T val = value ? 1 : 0;

				if ( statement )
				{
					( *paramSetter )( statement, &value, parameter );
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
			void operator()( SSqliteParameterValueSetterBase * paramSetter, const T & value, SQLite::Statement * statement, CDatabaseStatementParameterSqlite * parameter, CDatabaseValueBase & paramValue )
			{
				bool val = value != 0;

				if ( statement )
				{
					( *paramSetter )( statement, &value, parameter );
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
			void operator()( SSqliteParameterValueSetterBase * paramSetter, const bool & value, SQLite::Statement * statement, CDatabaseStatementParameterSqlite * parameter, CDatabaseValueBase & paramValue )
			{
				if ( statement )
				{
					( *paramSetter )( statement, &value, parameter );
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
			void operator()( SSqliteParameterValueSetterBase * paramSetter, const T & value, SQLite::Statement * statement, CDatabaseStatementParameterSqlite * parameter, CDatabaseValueBase & paramValue )
			{
				std::string val = std::to_string( value );

				if ( statement )
				{
					( *paramSetter )( statement, ( void * )value, parameter );
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
			void operator()( SSqliteParameterValueSetterBase * paramSetter, const T & value, SQLite::Statement * statement, CDatabaseStatementParameterSqlite * parameter, CDatabaseValueBase & paramValue )
			{
				std::wstring val = CStrUtils::ToWStr( std::to_string( value ) );

				if ( statement )
				{
					( *paramSetter )( statement, ( void * )value, parameter );
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
			void operator()( SSqliteParameterValueSetterBase * paramSetter, const char * value, SQLite::Statement * statement, CDatabaseStatementParameterSqlite * parameter, CDatabaseValueBase & paramValue )
			{
				if ( statement )
				{
					( *paramSetter )( statement, ( void * )value, parameter );
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
			void operator()( SSqliteParameterValueSetterBase * paramSetter, const wchar_t * value, SQLite::Statement * statement, CDatabaseStatementParameterSqlite * parameter, CDatabaseValueBase & paramValue )
			{
				std::string val = CStrUtils::ToStr( value );

				if ( statement )
				{
					( *paramSetter )( statement, value, parameter );
					paramValue.SetNull( false );

					if ( parameter->GetParamType() != EParameterType_IN )
					{
						if ( parameter->GetType() == EFieldType_VARCHAR )
						{
							static_cast< CDatabaseValue< SDataTypeFieldTyper< char * >::Value > & >( paramValue ).SetValue( val.c_str(), parameter->GetLimits() );
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
						static_cast< CDatabaseValue< SDataTypeFieldTyper< char * >::Value > & >( paramValue ).SetValue( val.c_str(), parameter->GetLimits() );
					}
					else
					{
						static_cast< CDatabaseValue< SDataTypeFieldTyper< std::string >::Value > & >( paramValue ).SetValue( val );
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
			std::vector< uint8_t > data;
			data.insert( data.end(), ( uint8_t * )parameter->GetObjectValue().GetPtrValue(), ( uint8_t * )parameter->GetObjectValue().GetPtrValue() + parameter->GetObjectValue().GetPtrSize() );
			DoSetValue( data );

			if ( !data.empty() )
			{
				SQLite::BindBlob( _statement, GetIndex(), data.data(), int( data.size() ), SQLite::NULL_DESTRUCTOR );
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

	void CDatabaseStatementParameterSqlite::DoSetValue( const bool & value )
	{
		switch ( GetType() )
		{
		case EFieldType_BOOL:
			SValueSetter< bool, bool >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_SMALL_INTEGER:
			SValueSetter< int16_t, bool >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_INTEGER:
			SValueSetter< int32_t, bool >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_LONG_INTEGER:
			SValueSetter< int64_t, bool >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValue( const int16_t & value )
	{
		switch ( GetType() )
		{
		case EFieldType_BOOL:
			SValueSetter< bool, int16_t >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_SMALL_INTEGER:
			SValueSetter< int16_t, int16_t >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_INTEGER:
			SValueSetter< int32_t, int16_t >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_LONG_INTEGER:
			SValueSetter< int64_t, int16_t >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_FLOAT:
			SValueSetter< float, int16_t >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_DOUBLE:
			SValueSetter< double, int16_t >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValue( const uint16_t & value )
	{
		switch ( GetType() )
		{
		case EFieldType_BOOL:
			SValueSetter< bool, uint16_t >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_SMALL_INTEGER:
			SValueSetter< int16_t, uint16_t >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_INTEGER:
			SValueSetter< int32_t, uint16_t >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_LONG_INTEGER:
			SValueSetter< int64_t, uint16_t >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_FLOAT:
			SValueSetter< float, uint16_t >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_DOUBLE:
			SValueSetter< double, uint16_t >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValue( const int32_t & value )
	{
		switch ( GetType() )
		{
		case EFieldType_BOOL:
			SValueSetter< bool, int >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_INTEGER:
			SValueSetter< int32_t, int >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_LONG_INTEGER:
			SValueSetter< int64_t, int >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_FLOAT:
			SValueSetter< float, int >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_DOUBLE:
			SValueSetter< double, int >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValue( const uint32_t & value )
	{
		switch ( GetType() )
		{
		case EFieldType_BOOL:
			SValueSetter< bool, unsigned int >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_INTEGER:
			SValueSetter< int32_t, unsigned int >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_LONG_INTEGER:
			SValueSetter< int64_t, unsigned int >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_FLOAT:
			SValueSetter< float, unsigned int >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_DOUBLE:
			SValueSetter< double, unsigned int >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValue( const int64_t & value )
	{
		switch ( GetType() )
		{
		case EFieldType_BOOL:
			SValueSetter< bool, int64_t >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_LONG_INTEGER:
			SValueSetter< int64_t, int64_t >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_DOUBLE:
			SValueSetter< double, int64_t >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValue( const uint64_t & value )
	{
		switch ( GetType() )
		{
		case EFieldType_BOOL:
			SValueSetter< bool, uint64_t >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_LONG_INTEGER:
			SValueSetter< int64_t, uint64_t >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_DOUBLE:
			SValueSetter< double, uint64_t >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValue( const float & value )
	{
		switch ( GetType() )
		{
		case EFieldType_SMALL_INTEGER:
			SValueSetter< int16_t, float >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_INTEGER:
			SValueSetter< int32_t, float >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_LONG_INTEGER:
			SValueSetter< int64_t, float >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_FLOAT:
			SValueSetter< float, float >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_DOUBLE:
			SValueSetter< double, float >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValue( const double & value )
	{
		switch ( GetType() )
		{
		case EFieldType_SMALL_INTEGER:
			SValueSetter< int16_t, double >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_INTEGER:
			SValueSetter< int32_t, double >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_LONG_INTEGER:
			SValueSetter< int64_t, double >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_FLOAT:
			SValueSetter< float, double >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_DOUBLE:
			SValueSetter< double, double >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValue( const long double & value )
	{
		switch ( GetType() )
		{
		case EFieldType_SMALL_INTEGER:
			SValueSetter< int16_t, long double >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_INTEGER:
			SValueSetter< int32_t, long double >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_LONG_INTEGER:
			SValueSetter< int64_t, long double >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_FLOAT:
			SValueSetter< float, long double >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		case EFieldType_DOUBLE:
			SValueSetter< double, long double >()( _paramSetter, value, _statement, this, GetObjectValue() );
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
			SValueSetter< const char *, const char * >()( _paramSetter, value, _statement, this, GetObjectValue() );
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
			SValueSetter< const char *, const wchar_t * >()( _paramSetter, value, _statement, this, GetObjectValue() );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValue( const std::string & value )
	{
		switch ( GetType() )
		{
		case EFieldType_VARCHAR:
		case EFieldType_TEXT:
		case EFieldType_NVARCHAR:
		case EFieldType_NTEXT:
			SValueSetter< const char *, const char * >()( _paramSetter, value.c_str(), _statement, this, GetObjectValue() );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValue( const std::wstring & value )
	{
		switch ( GetType() )
		{
		case EFieldType_VARCHAR:
		case EFieldType_TEXT:
		case EFieldType_NVARCHAR:
		case EFieldType_NTEXT:
			SValueSetter< const char *, const wchar_t * >()( _paramSetter, value.c_str(), _statement, this, GetObjectValue() );
			break;

		default:
			CLogger::LogError( DATABASE_PARAMETER_TYPE_ERROR );
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, DATABASE_PARAMETER_TYPE_ERROR );
			break;
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValue( const std::vector< uint8_t > & value )
	{
		CDatabaseValuedObject::DoSetValue( value );

		if ( !value.empty() )
		{
			SQLite::BindBlob( _statement, GetIndex(), value.data(), int( value.size() ), SQLite::NULL_DESTRUCTOR );
		}
		else
		{
			GetObjectValue().SetNull();
			SQLite::BindNull( _statement, GetIndex() );
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValue( const CDateTime & value )
	{
		CDatabaseValuedObject::DoSetValue( value );
		( *_paramSetter )( _statement, GetObjectValue().GetPtrValue(), this );
		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValue( const CDate & value )
	{
		CDatabaseValuedObject::DoSetValue( value );
		( *_paramSetter )( _statement, GetObjectValue().GetPtrValue(), this );
		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValue( const CTime & value )
	{
		CDatabaseValuedObject::DoSetValue( value );
		( *_paramSetter )( _statement, GetObjectValue().GetPtrValue(), this );
		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValueFast( const bool & value )
	{
		SValueSetter< bool, bool >()( _paramSetter, value, _statement, this, GetObjectValue() );
		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValueFast( const int16_t & value )
	{
		SValueSetter< int16_t, int16_t >()( _paramSetter, value, _statement, this, GetObjectValue() );
		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValueFast( const uint16_t & value )
	{
		SValueSetter< int16_t, uint16_t >()( _paramSetter, value, _statement, this, GetObjectValue() );
		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValueFast( const int32_t & value )
	{
		SValueSetter< int32_t, int >()( _paramSetter, value, _statement, this, GetObjectValue() );
		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValueFast( const uint32_t & value )
	{
		SValueSetter< int32_t, unsigned int >()( _paramSetter, value, _statement, this, GetObjectValue() );
		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValueFast( const int64_t & value )
	{
		SValueSetter< int64_t, int64_t >()( _paramSetter, value, _statement, this, GetObjectValue() );
		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValueFast( const uint64_t & value )
	{
		SValueSetter< int64_t, uint64_t >()( _paramSetter, value, _statement, this, GetObjectValue() );
		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValueFast( const float & value )
	{
		SValueSetter< float, float >()( _paramSetter, value, _statement, this, GetObjectValue() );
		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValueFast( const double & value )
	{
		SValueSetter< double, double >()( _paramSetter, value, _statement, this, GetObjectValue() );
		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValueFast( const long double & value )
	{
		SValueSetter< double, long double >()( _paramSetter, value, _statement, this, GetObjectValue() );
		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValueFast( const char * value )
	{
		SValueSetter< const char *, const char * >()( _paramSetter, value, _statement, this, GetObjectValue() );
		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValueFast( const wchar_t * value )
	{
		SValueSetter< const char *, const wchar_t * >()( _paramSetter, value, _statement, this, GetObjectValue() );
		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValueFast( const std::string & value )
	{
		SValueSetter< const char *, const char * >()( _paramSetter, value.c_str(), _statement, this, GetObjectValue() );
		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValueFast( const std::wstring & value )
	{
		SValueSetter< const char *, const wchar_t * >()( _paramSetter, value.c_str(), _statement, this, GetObjectValue() );
		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValueFast( const std::vector< uint8_t > & value )
	{
		CDatabaseValuedObject::DoSetValueFast( value );

		if ( GetType() == EFieldType_BINARY || GetType() == EFieldType_VARBINARY || GetType() == EFieldType_LONG_VARBINARY )
		{
			if ( !value.empty() )
			{
				SQLite::BindBlob( _statement, GetIndex(), value.data(), int( value.size() ), SQLite::NULL_DESTRUCTOR );
			}
			else
			{
				GetObjectValue().SetNull();
				SQLite::BindNull( _statement, GetIndex() );
			}
		}

		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValueFast( const CDateTime & value )
	{
		CDatabaseValuedObject::DoSetValueFast( value );
		( *_paramSetter )( _statement, GetObjectValue().GetPtrValue(), this );
		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValueFast( const CDate & value )
	{
		CDatabaseValuedObject::DoSetValueFast( value );
		( *_paramSetter )( _statement, GetObjectValue().GetPtrValue(), this );
		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoSetValueFast( const CTime & value )
	{
		CDatabaseValuedObject::DoSetValueFast( value );
		( *_paramSetter )( _statement, GetObjectValue().GetPtrValue(), this );
		_updater->Update( shared_from_this() );
	}

	void CDatabaseStatementParameterSqlite::DoInitializeParamSetter()
	{
		switch ( GetType() )
		{
		case EFieldType_BOOL:
			_paramSetter = new SSqliteParameterValueSetter< EFieldType_BOOL >;
			break;

		case EFieldType_SMALL_INTEGER:
			_paramSetter = new SSqliteParameterValueSetter< EFieldType_SMALL_INTEGER >;
			break;

		case EFieldType_INTEGER:
			_paramSetter = new SSqliteParameterValueSetter< EFieldType_INTEGER >;
			break;

		case EFieldType_LONG_INTEGER:
			_paramSetter = new SSqliteParameterValueSetter< EFieldType_LONG_INTEGER >;
			break;

		case EFieldType_FLOAT:
			_paramSetter = new SSqliteParameterValueSetter< EFieldType_FLOAT >;
			break;

		case EFieldType_DOUBLE:
			_paramSetter = new SSqliteParameterValueSetter< EFieldType_DOUBLE >;
			break;

		case EFieldType_VARCHAR:
			_paramSetter = new SSqliteParameterValueSetter< EFieldType_VARCHAR >;
			break;

		case EFieldType_TEXT:
			_paramSetter = new SSqliteParameterValueSetter< EFieldType_TEXT >;
			break;

		case EFieldType_NVARCHAR:
			_paramSetter = new SSqliteParameterValueSetter< EFieldType_NVARCHAR >;
			break;

		case EFieldType_NTEXT:
			_paramSetter = new SSqliteParameterValueSetter< EFieldType_NTEXT >;
			break;

		case EFieldType_DATE:
			_paramSetter = new SSqliteParameterValueSetter< EFieldType_DATE >;
			break;

		case EFieldType_DATETIME:
			_paramSetter = new SSqliteParameterValueSetter< EFieldType_DATETIME >;
			break;

		case EFieldType_TIME:
			_paramSetter = new SSqliteParameterValueSetter< EFieldType_TIME >;
			break;
		}
	}
}
END_NAMESPACE_DATABASE_SQLITE
