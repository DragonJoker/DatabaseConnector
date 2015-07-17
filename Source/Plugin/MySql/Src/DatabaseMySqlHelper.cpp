/************************************************************************//**
* @file DatabaseConnectionMySql.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 3/14/2014 5:03:15 PM
*
*
* @brief CConnectionMySql class definition.
*
* @details Describes a connection to a database via an MYSQL driver.
*
***************************************************************************/

#include "DatabaseMySqlPch.h"

#include "DatabaseMySqlHelper.h"
#include "DatabaseConnectionMySql.h"
#include "ExceptionDatabaseMySql.h"

#include <DatabaseFieldInfos.h>

BEGIN_NAMESPACE_DATABASE_MYSQL
{
	static const String ERROR_MYSQL_DRIVER = STR( "MYSQL Driver error : " );
	static const String ERROR_MYSQL_UNKNOWN = STR( "Unknown error encountered while executing query" );
	static const String ERROR_MYSQL_STMT_METADATA = STR( "Could not retrieve metadata from the statement" );

	static const TChar * INFO_MYSQL_SENDING_LONG_DATA = STR( "Long data sending" );
	static const String INFO_MYSQL_STMT_ATTR_SET = STR( "Setting statement attribute" );
	static const String INFO_MYSQL_STMT_STORE_RESULT = STR( "Storing statement results" );
	static const String INFO_MYSQL_STMT_BIND_RESULT = STR( "Binding statement results" );

	static const String MYSQL_ATTRIBUTE_UPDATE_MAX_LENGTH = STR( "STMT_ATTR_UPDATE_MAX_LENGTH" );

	// cf. https://dev.mysql.com/doc/refman/5.1/en/c-api-data-structures.html
	static const int MYSQL_BINARY_CHARSET = 63;

	namespace
	{
		/** Template class used to hold the binding for a specific parameter type
		*/
		template< EFieldType FieldType >
		struct SInMySqlBind
			: public SInMySqlBindBase
		{
			typedef typename SFieldTypeDataTyper< FieldType >::value_type value_type;

			/** Constructor
			@param[in] bind
				The MySQL bind
			*/
			SInMySqlBind( MYSQL_BIND & bind )
				: SInMySqlBindBase( bind )
			{
				bind.buffer = &_value;
			}

			/** Retrieves the binding's value
			@return
				The value
			*/
			value_type const & GetValue()const
			{
				return _value;
			}

			//! The value holder
			value_type _value;
		};

		/** SInMySqlBind specialisation for EFieldType_BIT
		*/
		template<>
		struct SInMySqlBind< EFieldType_BIT >
			: public SInMySqlBindBase
		{
			/** Constructor
			@param[in] bind
				The MySQL bind
			*/
			SInMySqlBind( MYSQL_BIND & bind )
				: SInMySqlBindBase( bind )
			{
				bind.buffer = &_value;
			}

			/** Retrieves the binding's value
			@return
				The value
			*/
			bool GetValue()const
			{
				return _value != 0;
			}

			//! The value holder
			int8_t _value;
		};

		/** SInMySqlBind specialisation for EFieldType_INT24
		@remarks
			Holds the value as an int32_t.
			The conversion is done when the call is made to GetValue
		*/
		template<>
		struct SInMySqlBind< EFieldType_INT24 >
			: public SInMySqlBindBase
		{
			/** Constructor
			@param[in] bind
				The MySQL bind
			*/
			SInMySqlBind( MYSQL_BIND & bind )
				: SInMySqlBindBase( bind )
			{
				bind.buffer = &_value;
				bind.buffer_length = sizeof( int32_t );
			}

			/** Retrieves the binding's value
			@return
				The value
			*/
			int24_t GetValue()const
			{
				return int24_t( _value );
			}

			//! The value holder
			int32_t _value;
		};

		/** Common holder for buffers
		@remarks
			Holds value as a vector of T
		*/
		template< typename T >
		struct SBufferInMySqlBind
			: public SInMySqlBindBase
		{
			/** Constructor
			@param[in] bind
				The MySQL bind
			@param[in] length
				The buffer length
			*/
			SBufferInMySqlBind( MYSQL_BIND & bind, uint32_t length )
				: SInMySqlBindBase( bind )
				, _value( length )
			{
				bind.buffer = _value.data();
				bind.buffer_length = length;
			}

			//! The value holder
			std::vector< T > _value;
		};

		/** Common holder for char buffers (VARCHAR and TEXT)
		*/
		struct SCharBufferInMySqlBind
			: public SBufferInMySqlBind< char >
		{
			/** Constructor
			@param[in] bind
				The MySQL bind
			@param[in] length
				The buffer length
			*/
			SCharBufferInMySqlBind( MYSQL_BIND & bind, uint32_t length )
				: SBufferInMySqlBind< char >( bind, length )
			{
			}

			/** Retrieves the binding's value
			@return
				The value
			*/
			std::string GetValue( bool truncated )const
			{
				char * value = reinterpret_cast< char * >( _bind.buffer );
				std::string result;

				if ( truncated )
				{
					if ( _bind.buffer_length )
					{
						result.assign( value, value + _bind.buffer_length );
					}
				}
				else
				{
					result.assign( value, value + *_bind.length );
				}

				return result;
			}
		};

		/** SInMySqlBind specialisation for EFieldType_VARCHAR
		*/
		template<>
		struct SInMySqlBind< EFieldType_VARCHAR >
			: public SCharBufferInMySqlBind
		{
			/** Constructor
			@param[in] bind
				The MySQL bind
			@param[in] length
				The buffer length
			*/
			SInMySqlBind( MYSQL_BIND & bind, uint32_t length )
				: SCharBufferInMySqlBind( bind, length + 1 )
			{
			}
		};

		/** SInMySqlBind specialisation for EFieldType_TEXT
		*/
		template<>
		struct SInMySqlBind< EFieldType_TEXT >
			: public SCharBufferInMySqlBind
		{
			/** Constructor
			@param[in] bind
				The MySQL bind
			@param[in] length
				The buffer length
			*/
			SInMySqlBind( MYSQL_BIND & bind, uint32_t length )
				: SCharBufferInMySqlBind( bind, length + 1 )
			{
			}
		};

		/** Common holder for wchar_t buffers (NVARCHAR, and NTEXT)
		@remarks
			As MySQL doesn't support NVARCHAR and NTEXT, the value is stored as a char array.
			The conversion is done when the call is made to GetValue
		*/
		struct SWCharBufferInMySqlBind
			: public SBufferInMySqlBind< char >
		{
			/** Constructor
			@param[in] bind
				The MySQL bind
			@param[in] length
				The buffer length
			*/
			SWCharBufferInMySqlBind( MYSQL_BIND & bind, uint32_t length )
				: SBufferInMySqlBind< char >( bind, length )
			{
			}

			/** Retrieves the binding's value
			@return
				The value
			*/
			std::wstring GetValue( bool truncated )const
			{
				char * value = reinterpret_cast< char * >( _bind.buffer );
				std::string result;

				if ( truncated )
				{
					if ( _bind.buffer_length )
					{
						result.assign( value, value + _bind.buffer_length );
					}
				}
				else
				{
					result.assign( value, value + *_bind.length );
				}

				return CStrUtils::ToWStr( result );
			}
		};

		/** SInMySqlBind specialisation for EFieldType_NVARCHAR
		*/
		template<>
		struct SInMySqlBind< EFieldType_NVARCHAR >
			: public SWCharBufferInMySqlBind
		{
			/** Constructor
			@param[in] bind
				The MySQL bind
			@param[in] length
				The buffer length
			*/
			SInMySqlBind( MYSQL_BIND & bind, uint32_t length )
				: SWCharBufferInMySqlBind( bind, length + 1 )
			{
			}
		};

		/** SInMySqlBind specialisation for EFieldType_NTEXT
		*/
		template<>
		struct SInMySqlBind< EFieldType_NTEXT >
			: public SWCharBufferInMySqlBind
		{
			/** Constructor
			@param[in] bind
				The MySQL bind
			@param[in] length
				The buffer length
			*/
			SInMySqlBind( MYSQL_BIND & bind, uint32_t length )
				: SWCharBufferInMySqlBind( bind, length + 1 )
			{
			}
		};

		struct SByteBufferInMySqlBind
			: public SBufferInMySqlBind< uint8_t >
		{
			/** Constructor
			@param[in] bind
				The MySQL bind
			@param[in] length
				The buffer length
			*/
			SByteBufferInMySqlBind( MYSQL_BIND & bind, uint32_t length )
				: SBufferInMySqlBind< uint8_t >( bind, length )
			{
			}

			/** Retrieves the binding's value
			@return
				The value
			*/
			std::vector< uint8_t > const & GetValue()const
			{
				return _value;
			}
		};

		/** SInMySqlBind specialisation for EFieldType_BINARY
		*/
		template<>
		struct SInMySqlBind< EFieldType_BINARY >
			: public SByteBufferInMySqlBind
		{
			/** Constructor
			@param[in] bind
				The MySQL bind
			@param[in] length
				The buffer length
			*/
			SInMySqlBind( MYSQL_BIND & bind, uint32_t length )
				: SByteBufferInMySqlBind( bind, length )
			{
			}
		};

		/** SInMySqlBind specialisation for EFieldType_VARBINARY
		*/
		template<>
		struct SInMySqlBind< EFieldType_VARBINARY >
			: public SByteBufferInMySqlBind
		{
			/** Constructor
			@param[in] bind
				The MySQL bind
			@param[in] length
				The buffer length
			*/
			SInMySqlBind( MYSQL_BIND & bind, uint32_t length )
				: SByteBufferInMySqlBind( bind, length )
			{
			}
		};

		/** SInMySqlBind specialisation for EFieldType_LONG_VARBINARY
		*/
		template<>
		struct SInMySqlBind< EFieldType_LONG_VARBINARY >
			: public SByteBufferInMySqlBind
		{
			/** Constructor
			@param[in] bind
				The MySQL bind
			@param[in] length
				The buffer length
			*/
			SInMySqlBind( MYSQL_BIND & bind, uint32_t length )
				: SByteBufferInMySqlBind( bind, length )
			{
			}
		};

		/** SInMySqlBind specialisation for EFieldType_FIXED_POINT
		@remarks
			Holds value as a char array
		*/
		template<>
		struct SInMySqlBind< EFieldType_FIXED_POINT >
			: public SInMySqlBindBase
		{
			/** Constructor
			@param[in] bind
				The MySQL bind
			@param[in] precision
				The value precision
			*/
			SInMySqlBind( MYSQL_BIND & bind, uint32_t precision )
				: SInMySqlBindBase( bind )
				, _precision( uint8_t( precision ) )
			{
				memset( _value, 0, sizeof( _value ) );
				bind.buffer = _value;
				bind.buffer_length = sizeof( _value ) / sizeof( *_value );
			}

			/** Retrieves the binding's value
			@return
				The value
			*/
			CFixedPoint GetValue()const
			{
				return CFixedPoint( _value, _precision );
			}

			//! The value holder
			char _value[1024];
			//! The value precision
			uint8_t _precision;
		};

		/** Common holder for EFieldType_DATE, EFieldType_DATETIME, and EFieldType_TIME
		@remarks
			Holds value as a MYSQL_TIME
		*/
		struct STimeInMySqlBind
			: public SInMySqlBindBase
		{
			/** Constructor
			@param[in] bind
				The MySQL bind
			*/
			STimeInMySqlBind( MYSQL_BIND & bind )
				: SInMySqlBindBase( bind )
			{
				bind.buffer = &_value;
				bind.buffer_length = sizeof( _value );
			}

			//! The value holder
			MYSQL_TIME _value;
		};

		/** SInMySqlBind specialisation for EFieldType_DATE
		*/
		template<>
		struct SInMySqlBind< EFieldType_DATE >
			: public STimeInMySqlBind
		{
			/** Constructor
			@param[in] bind
				The MySQL bind
			*/
			SInMySqlBind( MYSQL_BIND & bind )
				: STimeInMySqlBind( bind )
			{
			}

			/** Retrieves the binding's value
			@return
				The value
			*/
			CDate GetValue()const
			{
				assert( _value.time_type == MYSQL_TIMESTAMP_DATE );
				return CDate( _value.year, EDateMonth( _value.month - 1 ), _value.day );
			}
		};

		/** SInMySqlBind specialisation for EFieldType_DATETIME
		*/
		template<>
		struct SInMySqlBind< EFieldType_DATETIME >
			: public STimeInMySqlBind
		{
			/** Constructor
			@param[in] bind
				The MySQL bind
			*/
			SInMySqlBind( MYSQL_BIND & bind )
				: STimeInMySqlBind( bind )
			{
			}

			/** Retrieves the binding's value
			@return
				The value
			*/
			CDateTime GetValue()const
			{
				assert( _value.time_type == MYSQL_TIMESTAMP_DATETIME );
				return CDateTime( CDate( _value.year, EDateMonth( _value.month - 1 ), _value.day ), CTime( _value.hour, _value.minute, _value.second ) );
			}
		};

		/** SInMySqlBind specialisation for EFieldType_TIME
		*/
		template<>
		struct SInMySqlBind< EFieldType_TIME >
			: public STimeInMySqlBind
		{
			SInMySqlBind( MYSQL_BIND & bind )
				: STimeInMySqlBind( bind )
			{
			}

			/** Retrieves the binding's value
			@return
				The value
			*/
			CTime GetValue()const
			{
				assert( _value.time_type == MYSQL_TIMESTAMP_TIME );
				return CTime( _value.hour, _value.minute, _value.second );
			}
		};

		EFieldType GetFieldType( enum_field_types sqlType, int charset, int length )
		{
			EFieldType result = EFieldType_NULL;

			switch ( sqlType )
			{
			case MYSQL_TYPE_NULL:
				result = EFieldType_NULL;
				break;

			case MYSQL_TYPE_BIT:
				result = EFieldType_BIT;
				break;

			case MYSQL_TYPE_TINY:
				if ( length > 1 )
				{
					result = EFieldType_INT8;
				}
				else
				{
					result = EFieldType_BIT;
				}

				break;

			case MYSQL_TYPE_SHORT:
				result = EFieldType_INT16;
				break;

			case MYSQL_TYPE_INT24:
				result = EFieldType_INT24;
				break;

			case MYSQL_TYPE_LONG:
				result = EFieldType_INT32;
				break;

			case MYSQL_TYPE_LONGLONG:
				result = EFieldType_INT64;
				break;

			case MYSQL_TYPE_FLOAT:
				result = EFieldType_FLOAT32;
				break;

			case MYSQL_TYPE_DOUBLE:
				result = EFieldType_FLOAT64;
				break;

			case MYSQL_TYPE_NEWDECIMAL:
				result = EFieldType_FIXED_POINT;
				break;

			case MYSQL_TYPE_VAR_STRING:
			case MYSQL_TYPE_STRING:
				result = EFieldType_VARCHAR;
				break;

			case MYSQL_TYPE_DATE:
				result = EFieldType_DATE;
				break;

			case MYSQL_TYPE_DATETIME:
				result = EFieldType_DATETIME;
				break;

			case MYSQL_TYPE_TIME:
				result = EFieldType_TIME;
				break;

			case MYSQL_TYPE_BLOB:
				if ( charset == MYSQL_BINARY_CHARSET )
				{
					result = EFieldType_VARBINARY;
				}
				else
				{
					result = EFieldType_TEXT;
				}

				break;
			}

			return result;
		}

		std::unique_ptr< SInMySqlBindBase > GetInBind( EFieldType type, enum_field_types sqlType, MYSQL_BIND & bind, std::uint32_t precision, uint32_t length )
		{
			std::unique_ptr< SInMySqlBindBase > result;
			bind.buffer_type = sqlType;

			switch ( type )
			{
			case EFieldType_BIT:
				result = std::make_unique< SInMySqlBind< EFieldType_BIT > >( bind );
				bind.is_unsigned = false;
				break;

			case EFieldType_INT8:
				result = std::make_unique< SInMySqlBind< EFieldType_INT8 > >( bind );
				bind.is_unsigned = false;
				break;

			case EFieldType_INT16:
				result = std::make_unique< SInMySqlBind< EFieldType_INT16 > >( bind );
				bind.is_unsigned = false;
				break;

			case EFieldType_INT24:
				result = std::make_unique< SInMySqlBind< EFieldType_INT24 > >( bind );
				bind.is_unsigned = false;
				break;

			case EFieldType_INT32:
				result = std::make_unique< SInMySqlBind< EFieldType_INT32 > >( bind );
				bind.is_unsigned = false;
				break;

			case EFieldType_INT64:
				result = std::make_unique< SInMySqlBind< EFieldType_INT64 > >( bind );
				bind.is_unsigned = false;
				break;

			case EFieldType_FLOAT32:
				result = std::make_unique< SInMySqlBind< EFieldType_FLOAT32 > >( bind );
				bind.is_unsigned = false;
				break;

			case EFieldType_FLOAT64:
				result = std::make_unique< SInMySqlBind< EFieldType_FLOAT64 > >( bind );
				bind.is_unsigned = false;
				break;

			case EFieldType_FIXED_POINT:
				result = std::make_unique< SInMySqlBind< EFieldType_FIXED_POINT > >( bind, precision );
				bind.is_unsigned = false;
				break;

			case EFieldType_DATE:
				result = std::make_unique< SInMySqlBind< EFieldType_DATE > >( bind );
				bind.is_unsigned = false;
				break;

			case EFieldType_DATETIME:
				result = std::make_unique< SInMySqlBind< EFieldType_DATETIME > >( bind );
				bind.is_unsigned = false;
				break;

			case EFieldType_TIME:
				result = std::make_unique< SInMySqlBind< EFieldType_TIME > >( bind );
				bind.is_unsigned = false;
				break;

			case EFieldType_VARCHAR:
				result = std::make_unique< SInMySqlBind< EFieldType_VARCHAR > >( bind, length );
				bind.is_unsigned = false;
				break;

			case EFieldType_TEXT:
				result = std::make_unique< SInMySqlBind< EFieldType_TEXT > >( bind, length );
				bind.is_unsigned = false;
				break;

			case EFieldType_BINARY:
				result = std::make_unique< SInMySqlBind< EFieldType_BINARY > >( bind, length );
				bind.is_unsigned = false;
				break;

			case EFieldType_VARBINARY:
				result = std::make_unique< SInMySqlBind< EFieldType_VARBINARY > >( bind, length );
				bind.is_unsigned = false;
				break;

			case EFieldType_LONG_VARBINARY:
				result = std::make_unique< SInMySqlBind< EFieldType_LONG_VARBINARY > >( bind, length );
				bind.is_unsigned = false;
				break;
			}

			return result;
		}

		void SetFieldValue( DatabaseFieldInfosPtr infos, DatabaseFieldPtr field, SInMySqlBindBase const & bind, int result )
		{
			switch ( infos->GetType() )
			{
			case EFieldType_BIT:
				static_cast< CDatabaseValue< EFieldType_BIT > & >( field->GetObjectValue() ).SetValue( static_cast< SInMySqlBind< EFieldType_BIT > const & >( bind ).GetValue() != 0 );
				break;

			case EFieldType_INT8:
				static_cast< CDatabaseValue< EFieldType_INT8 > & >( field->GetObjectValue() ).SetValue( static_cast< SInMySqlBind< EFieldType_INT8 > const & >( bind ).GetValue() );
				break;

			case EFieldType_INT16:
				static_cast< CDatabaseValue< EFieldType_INT16 > & >( field->GetObjectValue() ).SetValue( static_cast< SInMySqlBind< EFieldType_INT16 > const & >( bind ).GetValue() );
				break;

			case EFieldType_INT24:
				static_cast< CDatabaseValue< EFieldType_INT24 > & >( field->GetObjectValue() ).SetValue( static_cast< SInMySqlBind< EFieldType_INT24 > const & >( bind ).GetValue() );
				break;

			case EFieldType_INT32:
				static_cast< CDatabaseValue< EFieldType_INT32 > & >( field->GetObjectValue() ).SetValue( static_cast< SInMySqlBind< EFieldType_INT32 > const & >( bind ).GetValue() );
				break;

			case EFieldType_INT64:
				static_cast< CDatabaseValue< EFieldType_INT64 > & >( field->GetObjectValue() ).SetValue( static_cast< SInMySqlBind< EFieldType_INT64 > const & >( bind ).GetValue() );
				break;

			case EFieldType_FLOAT32:
				static_cast< CDatabaseValue< EFieldType_FLOAT32 > & >( field->GetObjectValue() ).SetValue( static_cast< SInMySqlBind< EFieldType_FLOAT32 > const & >( bind ).GetValue() );
				break;

			case EFieldType_FLOAT64:
				static_cast< CDatabaseValue< EFieldType_FLOAT64 > & >( field->GetObjectValue() ).SetValue( static_cast< SInMySqlBind< EFieldType_FLOAT64 > const & >( bind ).GetValue() );
				break;

			case EFieldType_FIXED_POINT:
				static_cast< CDatabaseValue< EFieldType_FIXED_POINT > & >( field->GetObjectValue() ).SetValue( static_cast< SInMySqlBind< EFieldType_FIXED_POINT > const & >( bind ).GetValue() );
				break;

			case EFieldType_VARCHAR:
				static_cast< CDatabaseValue< EFieldType_VARCHAR > & >( field->GetObjectValue() ).SetValue( static_cast< SInMySqlBind< EFieldType_VARCHAR > const & >( bind ).GetValue( result != 0 ).c_str() );
				break;

			case EFieldType_TEXT:
				static_cast< CDatabaseValue< EFieldType_TEXT > & >( field->GetObjectValue() ).SetValue( static_cast< SInMySqlBind< EFieldType_TEXT > const & >( bind ).GetValue( result != 0 ) );
				break;

			case EFieldType_NVARCHAR:
				static_cast< CDatabaseValue< EFieldType_NVARCHAR > & >( field->GetObjectValue() ).SetValue( static_cast< SInMySqlBind< EFieldType_NVARCHAR > const & >( bind ).GetValue( result != 0 ).c_str() );
				break;

			case EFieldType_NTEXT:
				static_cast< CDatabaseValue< EFieldType_NTEXT > & >( field->GetObjectValue() ).SetValue( static_cast< SInMySqlBind< EFieldType_NTEXT > const & >( bind ).GetValue( result != 0 ) );
				break;

			case EFieldType_DATE:
				static_cast< CDatabaseValue< EFieldType_DATE > & >( field->GetObjectValue() ).SetValue( static_cast< SInMySqlBind< EFieldType_DATE > const & >( bind ).GetValue() );
				break;

			case EFieldType_DATETIME:
				static_cast< CDatabaseValue< EFieldType_DATETIME > & >( field->GetObjectValue() ).SetValue( static_cast< SInMySqlBind< EFieldType_DATETIME > const & >( bind ).GetValue() );
				break;

			case EFieldType_TIME:
				static_cast< CDatabaseValue< EFieldType_TIME > & >( field->GetObjectValue() ).SetValue( static_cast< SInMySqlBind< EFieldType_TIME > const & >( bind ).GetValue() );
				break;

			case EFieldType_BINARY:
				static_cast< CDatabaseValue< EFieldType_BINARY > & >( field->GetObjectValue() ).SetValue( static_cast< SInMySqlBind< EFieldType_BINARY > const & >( bind ).GetValue() );
				break;

			case EFieldType_VARBINARY:
				static_cast< CDatabaseValue< EFieldType_VARBINARY > & >( field->GetObjectValue() ).SetValue( static_cast< SInMySqlBind< EFieldType_VARBINARY > const & >( bind ).GetValue() );
				break;

			case EFieldType_LONG_VARBINARY:
				static_cast< CDatabaseValue< EFieldType_LONG_VARBINARY > & >( field->GetObjectValue() ).SetValue( static_cast< SInMySqlBind< EFieldType_LONG_VARBINARY > const & >( bind ).GetValue() );
				break;

			default:
				break;
			}
		}
	}

	DatabaseFieldInfosPtrArray MySqlGetColumns( MYSQL_STMT * stmt, DatabaseConnectionMySqlPtr connection, std::vector< std::unique_ptr< SInMySqlBindBase > > & inbinds, std::vector< MYSQL_BIND > & binds )
	{
		MySQLCheck( mysql_stmt_attr_set( stmt, STMT_ATTR_UPDATE_MAX_LENGTH, NULL ), ( INFO_MYSQL_STMT_ATTR_SET + MYSQL_ATTRIBUTE_UPDATE_MAX_LENGTH ).c_str(), EDatabaseExceptionCodes_StatementError, connection->GetConnection() );
		MySQLCheck( mysql_stmt_store_result( stmt ), INFO_MYSQL_STMT_STORE_RESULT.c_str(), EDatabaseExceptionCodes_StatementError, connection->GetConnection() );

		MYSQL_RES * data = mysql_stmt_result_metadata( stmt );

		if ( !data )
		{
			CLogger::LogError( ERROR_MYSQL_STMT_METADATA );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_MYSQL_STMT_METADATA, __FUNCTION__, __FILE__, __LINE__ );
		}

		DatabaseFieldInfosPtrArray arrayReturn;
		uint32_t columnCount = mysql_num_fields( data );
		binds.resize( columnCount, { 0 } );

		for ( auto && bind : binds )
		{
			bind = { 0 };
			MYSQL_FIELD * field = mysql_fetch_field( data );
			EFieldType type = GetFieldType( field->type, field->charsetnr, field->length );
			arrayReturn.push_back( std::make_shared< CDatabaseFieldInfos >( connection, CStrUtils::ToString( field->name ), type, field->length ) );
			inbinds.emplace_back( GetInBind( type, field->type, bind, field->decimals, std::max( field->length, field->max_length ) ) );
		}

		mysql_free_result( data );
		return arrayReturn;
	}

	DatabaseResultPtr MySqlFetchResult( MYSQL_STMT * statement, DatabaseFieldInfosPtrArray const & columns, DatabaseConnectionMySqlPtr connection, std::vector< std::unique_ptr< SInMySqlBindBase > > const & inbinds, std::vector< MYSQL_BIND > & binds )
	{
		DatabaseResultPtr pReturn;

		try
		{
			if ( connection->IsConnected() )
			{
				pReturn = std::make_unique< CDatabaseResult >( connection, columns );
				int iNbColumns = int( columns.size() );
				MySQLCheck( mysql_stmt_bind_result( statement, binds.data() ), INFO_MYSQL_STMT_BIND_RESULT.c_str(), EDatabaseExceptionCodes_StatementError, connection->GetConnection() );
				int result = 0;

				while ( ( result = mysql_stmt_fetch( statement ) ) == 0 || result == MYSQL_DATA_TRUNCATED )
				{
					DatabaseRowPtr pRow = std::make_shared< CDatabaseRow >( connection );
					int index = 0;

					for ( auto && bind : inbinds )
					{
						DatabaseFieldInfosPtr infos;

						try
						{
							infos = pReturn->GetFieldInfos( index++ );
						}
						catch ( CExceptionDatabase & )
						{
							throw;
						}

						DatabaseFieldPtr field = std::make_shared< CDatabaseField >( infos );

						if ( !bind->_null )
						{
							SetFieldValue( infos, field, *bind, result );
						}

						pRow->AddField( field );
					}

					pReturn->AddRow( pRow );
				}

				///@remarks Flush the statement, for the multiple results ones
				//try
				//{
				//	bool hasResult = false;

				//	while ( hasResult = statement->getMoreResults() )
				//	{
				//		rs.reset( statement->getResultSet() );
				//		CLogger::LogDebug( "Success : Result set retrieval" );
				//	}
				//}
				//catch ( sql::SQLException & e )
				//{
				//	StringStream stream;
				//	stream << "Failure : " << "Get more results" << std::endl;
				//	stream << "    MySQL Error code : " << e.getErrorCode() << std::endl;
				//	stream << "    SQL State : " << e.getSQLState().c_str() << std::endl;
				//	stream << "    Why : " << e.what() << std::endl;
				//	CLogger::LogDebug( stream.str() );
				//}
			}
		}
		catch ( const CExceptionDatabase & e )
		{
			StringStream message;
			message << ERROR_MYSQL_DRIVER << STR( " - " )
					<< e.what();
			CLogger::LogError( message );
			MYSQL_EXCEPT( EDatabaseMySqlExceptionCodes_GenericError, message.str() );
		}
		catch ( const std::exception & e )
		{
			StringStream message;
			message << ERROR_MYSQL_DRIVER << STR( " - " )
					<< e.what();
			CLogger::LogError( message );
			MYSQL_EXCEPT( EDatabaseMySqlExceptionCodes_GenericError, message.str() );
		}
		catch ( ... )
		{
			StringStream message;
			message << ERROR_MYSQL_DRIVER << STR( " - " )
					<< ERROR_MYSQL_UNKNOWN;
			CLogger::LogError( message );
			MYSQL_EXCEPT( EDatabaseMySqlExceptionCodes_UnknownError, message.str() );
		}

		return pReturn;
	}

	void MySQLCheck( int result, TChar const * msg, EDatabaseExceptionCodes code, MYSQL * connection )
	{
		if ( result )
		{
			StringStream error;
			error << STR( "Failure: " ) << msg << std::endl;
			error << STR( "(" ) << result << STR( ")" ) << mysql_error( connection );
			DB_EXCEPT( code, error.str() );
		}

#if !defined( NDEBUG )

		else
		{
			CLogger::LogDebug( StringStream() << STR( "Success : " ) << msg );
		}

#endif
	}

	MYSQL_TIME MySqlTimeFromCDate( CDate const & value )
	{
		MYSQL_TIME ts = { 0 };
		ts.time_type = MYSQL_TIMESTAMP_DATE;
		ts.year = value.GetYear();
		ts.month = value.GetMonth() + 1;
		ts.day = value.GetMonthDay();
		return ts;
	}

	MYSQL_TIME MySqlTimeFromCDateTime( CDateTime const & value )
	{
		MYSQL_TIME ts = { 0 };
		ts.time_type = MYSQL_TIMESTAMP_DATETIME;
		ts.year = value.GetYear();
		ts.month = value.GetMonth() + 1;
		ts.day = value.GetMonthDay();
		ts.hour = value.GetHour();
		ts.minute = value.GetMinute();
		ts.second = value.GetSecond();
		return ts;
	}

	MYSQL_TIME MySqlTimeFromCTime( CTime const & value )
	{
		MYSQL_TIME ts = { 0 };
		ts.time_type = MYSQL_TIMESTAMP_TIME;
		ts.hour = value.GetHour();
		ts.minute = value.GetMinute();
		ts.second = value.GetSecond();
		return ts;
	}

	void MySqlSendLongData( CDatabaseValueBase & value, unsigned int index, MYSQL_STMT * statement, MYSQL * connection )
	{
		unsigned long const chunk = 1024;
		unsigned long remaining = value.GetPtrSize();
		char * data = reinterpret_cast< char * >( value.GetPtrValue() );
		my_bool ret = 0;

		while ( remaining && !ret )
		{
			unsigned long length = std::min( chunk, remaining );
			ret = mysql_stmt_send_long_data( statement, index, data, length );
			data += length;
			remaining -= length;
		}

		MySQLCheck( ret, INFO_MYSQL_SENDING_LONG_DATA, EDatabaseExceptionCodes_StatementError, connection );
	}
}
END_NAMESPACE_DATABASE_MYSQL
