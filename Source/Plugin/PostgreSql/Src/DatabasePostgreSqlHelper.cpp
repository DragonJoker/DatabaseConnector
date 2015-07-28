/************************************************************************//**
* @file DatabaseConnectionPostgreSql.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 3/14/2014 5:03:15 PM
*
*
* @brief CConnectionPostgreSql class definition.
*
* @details Describes a connection to a database via an MYSQL driver.
*
***************************************************************************/

#include "DatabasePostgreSqlPch.h"

#include "DatabasePostgreSqlHelper.h"
#include "DatabaseConnectionPostgreSql.h"
#include "ExceptionDatabasePostgreSql.h"
#include "DatabaseParameterPostgreSql.h"

#include <DatabaseFieldInfos.h>

#if defined( _WIN32 )
#	include <Winsock2.h>
#else
#	include <netinet/in.h>
#	include <arpa/inet.h>
#endif

#include <postgres.h>
#include <libpq-fe.h>
#include <pgtypes_date.h>
#include <pgtypes_timestamp.h>

BEGIN_NAMESPACE_DATABASE_POSTGRESQL
{
	static const TChar * ERROR_POSTGRESQL_DRIVER = STR( "PostgreSQL Driver error : " );
	static const TChar * ERROR_POSTGRESQL_UNKNOWN = STR( "Unknown error encountered while executing query" );

	//************************************************************************************************
	
	SInPostgreSqlBindBase::SInPostgreSqlBindBase( int index, PGresult * result )
		: _result( result )
		, _index( index )
	{
	}

	bool SInPostgreSqlBindBase::IsNull( int row )const
	{
		return PQgetisnull( _result, row, _index ) == 1;
	}

	//************************************************************************************************

	SOutPostgreSqlBindBase::SOutPostgreSqlBindBase( PGbind & bind, Oid const & type, CDatabaseParameterPostgreSql & parameter )
		: _bind( bind )
		, _statement( parameter.GetStatement() )
		, _connection( std::static_pointer_cast< CDatabaseConnectionPostgreSql >( parameter.GetConnection() )->GetConnection() )
		, _valueBase( parameter.GetObjectValue() )
	{
		_bind.format = 0;
	}

	void SOutPostgreSqlBindBase::UpdateValue()
	{
		if ( _valueBase.IsNull() )
		{
			_bind.value = NULL;
			_bind.length = 0;
		}
		else
		{
			DoUpdateValue();
		}
	}

	//************************************************************************************************

	namespace
	{
		/** Template class used to hold the binding for a specific parameter type
		*/
		template< EFieldType FieldType >
		struct SInPostgreSqlBind
			: public SInPostgreSqlBindBase
		{
			typedef typename SFieldTypeDataTyper< FieldType >::value_type value_type;

			/** Constructor
			@param[in] bind
				The PostgreSQL bind
			*/
			SInPostgreSqlBind( int index, PGresult * result )
				: SInPostgreSqlBindBase( index, result )
			{
			}

			/** Retrieves the binding's value
			@return
				The value
			*/
			value_type GetValue( int row )const
			{
				std::stringstream stream( PQgetvalue( _result, row, _index ) );
				value_type value = value_type();
				stream >> value;
				return value;
			}
		};

		/** SInPostgreSqlBind specialisation for EFieldType_BIT
		*/
		template<>
		struct SInPostgreSqlBind< EFieldType_BIT >
			: public SInPostgreSqlBindBase
		{
			/** Constructor
			@param[in] bind
				The PostgreSQL bind
			*/
			SInPostgreSqlBind( int index, PGresult * result )
				: SInPostgreSqlBindBase( index, result )
			{
			}

			/** Retrieves the binding's value
			@return
				The value
			*/
			bool GetValue( int row )const
			{
				char * value = PQgetvalue( _result, row, _index );
				return *value == 't';
			}
		};

		/** SInPostgreSqlBind specialisation for EFieldType_SINT24
		@remarks
			Holds the value as an int32_t.
			The conversion is done when the call is made to GetValue
		*/
		template<>
		struct SInPostgreSqlBind< EFieldType_SINT24 >
			: public SInPostgreSqlBindBase
		{
			/** Constructor
			@param[in] bind
				The PostgreSQL bind
			*/
			SInPostgreSqlBind( int index, PGresult * result )
				: SInPostgreSqlBindBase( index, result )
			{
			}

			/** Retrieves the binding's value
			@return
				The value
			*/
			int24_t GetValue( int row )const
			{
				std::stringstream stream( PQgetvalue( _result, row, _index ) );
				int32_t value = int32_t();
				stream >> value;
				return int24_t( value );
			}
		};

		/** SInPostgreSqlBind specialisation for EFieldType_UINT24
		@remarks
			Holds the value as an uint32_t.
			The conversion is done when the call is made to GetValue
		*/
		template<>
		struct SInPostgreSqlBind< EFieldType_UINT24 >
			: public SInPostgreSqlBindBase
		{
			/** Constructor
			@param[in] bind
				The PostgreSQL bind
			*/
			SInPostgreSqlBind( int index, PGresult * result )
				: SInPostgreSqlBindBase( index, result )
			{
			}

			/** Retrieves the binding's value
			@return
				The value
			*/
			uint24_t GetValue( int row )const
			{
				std::stringstream stream( PQgetvalue( _result, row, _index ) );
				uint32_t value = uint32_t();
				stream >> value;
				return uint24_t( value );
			}
		};

		/** Common holder for char buffers (VARCHAR and TEXT)
		*/
		struct SCharBufferInPostgreSqlBind
			: public SInPostgreSqlBindBase
		{
			/** Constructor
			@param[in] bind
				The PostgreSQL bind
			@param[in] length
				The buffer length
			*/
			SCharBufferInPostgreSqlBind( int index, PGresult * result )
				: SInPostgreSqlBindBase( index, result )
			{
			}

			/** Retrieves the binding's value
			@return
				The value
			*/
			std::string GetValue( int row )const
			{
				return std::string( PQgetvalue( _result, row, _index ) );
			}
		};

		/** SInPostgreSqlBind specialisation for EFieldType_CHAR
		*/
		template<>
		struct SInPostgreSqlBind< EFieldType_CHAR >
			: public SCharBufferInPostgreSqlBind
		{
			/** Constructor
			@param[in] bind
				The PostgreSQL bind
			@param[in] length
				The buffer length
			*/
			SInPostgreSqlBind( int index, PGresult * result )
				: SCharBufferInPostgreSqlBind( index, result )
			{
			}
		};

		/** SInPostgreSqlBind specialisation for EFieldType_VARCHAR
		*/
		template<>
		struct SInPostgreSqlBind< EFieldType_VARCHAR >
			: public SCharBufferInPostgreSqlBind
		{
			/** Constructor
			@param[in] bind
				The PostgreSQL bind
			@param[in] length
				The buffer length
			*/
			SInPostgreSqlBind( int index, PGresult * result )
				: SCharBufferInPostgreSqlBind( index, result )
			{
			}
		};

		/** SInPostgreSqlBind specialisation for EFieldType_TEXT
		*/
		template<>
		struct SInPostgreSqlBind< EFieldType_TEXT >
			: public SCharBufferInPostgreSqlBind
		{
			/** Constructor
			@param[in] bind
				The PostgreSQL bind
			@param[in] length
				The buffer length
			*/
			SInPostgreSqlBind( int index, PGresult * result )
				: SCharBufferInPostgreSqlBind( index, result )
			{
			}
		};

		/** Common holder for wchar_t buffers (NVARCHAR, and NTEXT)
		@remarks
			As PostgreSQL doesn't support NVARCHAR and NTEXT, the value is stored as a char array.
			The conversion is done when the call is made to GetValue
		*/
		struct SWCharBufferInPostgreSqlBind
			: public SInPostgreSqlBindBase
		{
			/** Constructor
			@param[in] bind
				The PostgreSQL bind
			@param[in] length
				The buffer length
			*/
			SWCharBufferInPostgreSqlBind( int index, PGresult * result )
				: SInPostgreSqlBindBase( index, result )
			{
			}

			/** Retrieves the binding's value
			@return
				The value
			*/
			std::wstring GetValue( int row )const
			{
				return StringUtils::ToWStr( PQgetvalue( _result, row, _index ) );
			}
		};

		/** SInPostgreSqlBind specialisation for EFieldType_NCHAR
		*/
		template<>
		struct SInPostgreSqlBind< EFieldType_NCHAR >
			: public SWCharBufferInPostgreSqlBind
		{
			/** Constructor
			@param[in] bind
				The PostgreSQL bind
			@param[in] length
				The buffer length
			*/
			SInPostgreSqlBind( int index, PGresult * result )
				: SWCharBufferInPostgreSqlBind( index, result )
			{
			}
		};

		/** SInPostgreSqlBind specialisation for EFieldType_NVARCHAR
		*/
		template<>
		struct SInPostgreSqlBind< EFieldType_NVARCHAR >
			: public SWCharBufferInPostgreSqlBind
		{
			/** Constructor
			@param[in] bind
				The PostgreSQL bind
			@param[in] length
				The buffer length
			*/
			SInPostgreSqlBind( int index, PGresult * result )
				: SWCharBufferInPostgreSqlBind( index, result )
			{
			}
		};

		/** SInPostgreSqlBind specialisation for EFieldType_NTEXT
		*/
		template<>
		struct SInPostgreSqlBind< EFieldType_NTEXT >
			: public SWCharBufferInPostgreSqlBind
		{
			/** Constructor
			@param[in] bind
				The PostgreSQL bind
			@param[in] length
				The buffer length
			*/
			SInPostgreSqlBind( int index, PGresult * result )
				: SWCharBufferInPostgreSqlBind( index, result )
			{
			}
		};

		struct SByteBufferInPostgreSqlBind
			: public SInPostgreSqlBindBase
		{
			/** Constructor
			@param[in] bind
				The PostgreSQL bind
			@param[in] length
				The buffer length
			*/
			SByteBufferInPostgreSqlBind( int index, PGresult * result )
				: SInPostgreSqlBindBase( index, result )
			{
			}

			/** Retrieves the binding's value
			@return
				The value
			*/
			std::vector< uint8_t > GetValue( int row )const
			{
				char * escaped = PQgetvalue( _result, row, _index );
				size_t length = 0;
				uint8_t * value = PQunescapeBytea( reinterpret_cast< uint8_t * >( escaped ), &length );
				
				return std::vector< uint8_t >( value, value + length );
			}
		};

		/** SInPostgreSqlBind specialisation for EFieldType_BINARY
		*/
		template<>
		struct SInPostgreSqlBind< EFieldType_BINARY >
			: public SByteBufferInPostgreSqlBind
		{
			/** Constructor
			@param[in] bind
				The PostgreSQL bind
			@param[in] length
				The buffer length
			*/
			SInPostgreSqlBind( int index, PGresult * result )
				: SByteBufferInPostgreSqlBind( index, result )
			{
			}
		};

		/** SInPostgreSqlBind specialisation for EFieldType_VARBINARY
		*/
		template<>
		struct SInPostgreSqlBind< EFieldType_VARBINARY >
			: public SByteBufferInPostgreSqlBind
		{
			/** Constructor
			@param[in] bind
				The PostgreSQL bind
			@param[in] length
				The buffer length
			*/
			SInPostgreSqlBind( int index, PGresult * result )
				: SByteBufferInPostgreSqlBind( index, result )
			{
			}
		};

		/** SInPostgreSqlBind specialisation for EFieldType_LONG_VARBINARY
		*/
		template<>
		struct SInPostgreSqlBind< EFieldType_LONG_VARBINARY >
			: public SByteBufferInPostgreSqlBind
		{
			/** Constructor
			@param[in] bind
				The PostgreSQL bind
			@param[in] length
				The buffer length
			*/
			SInPostgreSqlBind( int index, PGresult * result )
				: SByteBufferInPostgreSqlBind( index, result )
			{
			}
		};

		/** SInPostgreSqlBind specialisation for EFieldType_FIXED_POINT
		@remarks
			Holds value as a char array
		*/
		template<>
		struct SInPostgreSqlBind< EFieldType_FIXED_POINT >
			: public SInPostgreSqlBindBase
		{
			/** Constructor
			@param[in] bind
				The PostgreSQL bind
			@param[in] precision
				The value precision
			*/
			SInPostgreSqlBind( int index, PGresult * result )
				: SInPostgreSqlBindBase( index, result )
				, _precision( 10 )
				, _decimals( 0 )
			{
				int value = PQfmod( result, index );

				if ( value != -1 )
				{
					value -= VARHDRSZ;
					_decimals = 0x0000FFFF & value;
					_precision = ( ( value - _decimals ) >> 16 );
				}
			}

			/** Retrieves the binding's value
			@return
				The value
			*/
			CFixedPoint GetValue( int row )const
			{
				char * value = PQgetvalue( _result, row, _index );
				return CFixedPoint( value, _precision, _decimals );
			}

			//! The value precision
			uint8_t _precision;
			//! The value decimals
			uint8_t _decimals;
		};

		/** SInPostgreSqlBind specialisation for EFieldType_DATE
		*/
		template<>
		struct SInPostgreSqlBind< EFieldType_DATE >
			: public SInPostgreSqlBindBase
		{
			/** Constructor
			@param[in] bind
				The PostgreSQL bind
			*/
			SInPostgreSqlBind( int index, PGresult * result )
				: SInPostgreSqlBindBase( index, result )
			{
			}

			/** Retrieves the binding's value
			@return
				The value
			*/
			DateType GetValue( int row )const
			{
				char * value = PQgetvalue( _result, row, _index );
				DateType date;
				Date::IsDate( value, POSTGRE_FORMAT_DATE, date );
				return date;
			}
		};

		/** SInPostgreSqlBind specialisation for EFieldType_DATETIME
		*/
		template<>
		struct SInPostgreSqlBind< EFieldType_DATETIME >
			: public SInPostgreSqlBindBase
		{
			/** Constructor
			@param[in] bind
				The PostgreSQL bind
			*/
			SInPostgreSqlBind( int index, PGresult * result )
				: SInPostgreSqlBindBase( index, result )
			{
			}

			/** Retrieves the binding's value
			@return
				The value
			*/
			DateTimeType GetValue( int row )const
			{
				char * value = PQgetvalue( _result, row, _index );
				DateTimeType date;
				DateTime::IsDateTime( value, POSTGRE_FORMAT_DATETIME, date );
				return date;
			}
		};

		/** SInPostgreSqlBind specialisation for EFieldType_TIME
		*/
		template<>
		struct SInPostgreSqlBind< EFieldType_TIME >
			: public SInPostgreSqlBindBase
		{
			SInPostgreSqlBind( int index, PGresult * result )
				: SInPostgreSqlBindBase( index, result )
			{
			}

			/** Retrieves the binding's value
			@return
				The value
			*/
			TimeType GetValue( int row )const
			{
				char * value = PQgetvalue( _result, row, _index );
				TimeType date;
				Time::IsTime( value, POSTGRE_FORMAT_TIME, date );
				return date;
			}
		};

		typedef enum EOid
		{
			EOid_BOOL = 16,			// BOOLOID
			EOid_BYTEA = 17,		// BYTEAOID
			EOid_CHAR = 18,			// CHAROID
			EOid_INT8 = 20,			// INT8OID
			EOid_INT2 = 21,			// INT2OID
			EOid_INT4 = 23,			// INT4OID
			EOid_TEXT = 25,			// TEXTOID
			EOid_FLOAT4 = 700,		// FLOAT4OID
			EOid_FLOAT8 = 701,		// FLOAT8OID
			EOid_BPCHAR = 1042,		// BPCHAROID
			EOid_VARCHAR = 1043,	// VARCHAROID
			EOid_DATE = 1082,		// DATEOID
			EOid_TIME = 1083,		// TIMEOID
			EOid_TIMESTAMP = 1114,	// TIMESTAMPOID
			EOid_NUMERIC = 1700,	// NUMERICOID
		}	EOid;

		EFieldType GetFieldTypeFromOid( Oid pgType )
		{
			EFieldType result = EFieldType_NULL;

			switch ( EOid( pgType ) )
			{
			case EOid_BOOL:
				result = EFieldType_BIT;
				break;

			case EOid_CHAR:
				result = EFieldType_SINT8;
				break;

			case EOid_INT2:
				result = EFieldType_SINT16;
				break;

			case EOid_INT4:
				result = EFieldType_SINT32;
				break;

			case EOid_INT8:
				result = EFieldType_SINT64;
				break;

			case EOid_FLOAT4:
				result = EFieldType_FLOAT32;
				break;

			case EOid_FLOAT8:
				result = EFieldType_FLOAT64;
				break;

			case EOid_NUMERIC:
				result = EFieldType_FIXED_POINT;
				break;

			case EOid_BPCHAR:
				result = EFieldType_CHAR;
				break;

			case EOid_VARCHAR:
				result = EFieldType_VARCHAR;
				break;

			case EOid_TEXT:
				result = EFieldType_TEXT;
				break;

			case EOid_DATE:
				result = EFieldType_DATE;
				break;

			case EOid_TIMESTAMP:
				result = EFieldType_DATETIME;
				break;

			case EOid_TIME:
				result = EFieldType_TIME;
				break;

			case EOid_BYTEA:
				result = EFieldType_VARBINARY;
				break;
			}

			return result;
		}

		std::unique_ptr< SInPostgreSqlBindBase > GetInBind( EFieldType type, int index, PGresult * result )
		{
			std::unique_ptr< SInPostgreSqlBindBase > ret;

			switch ( type )
			{
			case EFieldType_BIT:
				ret = std::make_unique< SInPostgreSqlBind< EFieldType_BIT > >( index, result );
				break;

			case EFieldType_SINT8:
				ret = std::make_unique< SInPostgreSqlBind< EFieldType_SINT8 > >( index, result );
				break;

			case EFieldType_SINT16:
				ret = std::make_unique< SInPostgreSqlBind< EFieldType_SINT16 > >( index, result );
				break;

			case EFieldType_SINT24:
				ret = std::make_unique< SInPostgreSqlBind< EFieldType_SINT24 > >( index, result );
				break;

			case EFieldType_SINT32:
				ret = std::make_unique< SInPostgreSqlBind< EFieldType_SINT32 > >( index, result );
				break;

			case EFieldType_SINT64:
				ret = std::make_unique< SInPostgreSqlBind< EFieldType_SINT64 > >( index, result );
				break;

			case EFieldType_UINT8:
				ret = std::make_unique< SInPostgreSqlBind< EFieldType_UINT8 > >( index, result );
				break;

			case EFieldType_UINT16:
				ret = std::make_unique< SInPostgreSqlBind< EFieldType_UINT16 > >( index, result );
				break;

			case EFieldType_UINT24:
				ret = std::make_unique< SInPostgreSqlBind< EFieldType_UINT24 > >( index, result );
				break;

			case EFieldType_UINT32:
				ret = std::make_unique< SInPostgreSqlBind< EFieldType_UINT32 > >( index, result );
				break;

			case EFieldType_UINT64:
				ret = std::make_unique< SInPostgreSqlBind< EFieldType_UINT64 > >( index, result );
				break;

			case EFieldType_FLOAT32:
				ret = std::make_unique< SInPostgreSqlBind< EFieldType_FLOAT32 > >( index, result );
				break;

			case EFieldType_FLOAT64:
				ret = std::make_unique< SInPostgreSqlBind< EFieldType_FLOAT64 > >( index, result );
				break;

			case EFieldType_FIXED_POINT:
				ret = std::make_unique< SInPostgreSqlBind< EFieldType_FIXED_POINT > >( index, result );
				break;

			case EFieldType_DATE:
				ret = std::make_unique< SInPostgreSqlBind< EFieldType_DATE > >( index, result );
				break;

			case EFieldType_DATETIME:
				ret = std::make_unique< SInPostgreSqlBind< EFieldType_DATETIME > >( index, result );
				break;

			case EFieldType_TIME:
				ret = std::make_unique< SInPostgreSqlBind< EFieldType_TIME > >( index, result );
				break;

			case EFieldType_CHAR:
				ret = std::make_unique< SInPostgreSqlBind< EFieldType_CHAR > >( index, result );
				break;

			case EFieldType_VARCHAR:
				ret = std::make_unique< SInPostgreSqlBind< EFieldType_VARCHAR > >( index, result );
				break;

			case EFieldType_TEXT:
				ret = std::make_unique< SInPostgreSqlBind< EFieldType_TEXT > >( index, result );
				break;

			case EFieldType_NCHAR:
				ret = std::make_unique< SInPostgreSqlBind< EFieldType_NCHAR > >( index, result );
				break;

			case EFieldType_NVARCHAR:
				ret = std::make_unique< SInPostgreSqlBind< EFieldType_NVARCHAR > >( index, result );
				break;

			case EFieldType_NTEXT:
				ret = std::make_unique< SInPostgreSqlBind< EFieldType_NTEXT > >( index, result );
				break;

			case EFieldType_BINARY:
				ret = std::make_unique< SInPostgreSqlBind< EFieldType_BINARY > >( index, result );
				break;

			case EFieldType_VARBINARY:
				ret = std::make_unique< SInPostgreSqlBind< EFieldType_VARBINARY > >( index, result );
				break;

			case EFieldType_LONG_VARBINARY:
				ret = std::make_unique< SInPostgreSqlBind< EFieldType_LONG_VARBINARY > >( index, result );
				break;
			}

			return ret;
		}

		void SetFieldValue( DatabaseFieldInfosSPtr infos, DatabaseFieldSPtr field, SInPostgreSqlBindBase const & bind, int row )
		{
			if ( !bind.IsNull( row ) )
			{
				switch ( infos->GetType() )
				{
				case EFieldType_BIT:
					static_cast< CDatabaseValue< EFieldType_BIT > & >( field->GetObjectValue() ).SetValue( static_cast< SInPostgreSqlBind< EFieldType_BIT > const & >( bind ).GetValue( row ) != 0 );
					break;

				case EFieldType_SINT8:
					static_cast< CDatabaseValue< EFieldType_SINT8 > & >( field->GetObjectValue() ).SetValue( static_cast< SInPostgreSqlBind< EFieldType_SINT8 > const & >( bind ).GetValue( row ) );
					break;

				case EFieldType_SINT16:
					static_cast< CDatabaseValue< EFieldType_SINT16 > & >( field->GetObjectValue() ).SetValue( static_cast< SInPostgreSqlBind< EFieldType_SINT16 > const & >( bind ).GetValue( row ) );
					break;

				case EFieldType_SINT24:
					static_cast< CDatabaseValue< EFieldType_SINT24 > & >( field->GetObjectValue() ).SetValue( static_cast< SInPostgreSqlBind< EFieldType_SINT24 > const & >( bind ).GetValue( row ) );
					break;

				case EFieldType_SINT32:
					static_cast< CDatabaseValue< EFieldType_SINT32 > & >( field->GetObjectValue() ).SetValue( static_cast< SInPostgreSqlBind< EFieldType_SINT32 > const & >( bind ).GetValue( row ) );
					break;

				case EFieldType_SINT64:
					static_cast< CDatabaseValue< EFieldType_SINT64 > & >( field->GetObjectValue() ).SetValue( static_cast< SInPostgreSqlBind< EFieldType_SINT64 > const & >( bind ).GetValue( row ) );
					break;

				case EFieldType_UINT8:
					static_cast< CDatabaseValue< EFieldType_UINT8 > & >( field->GetObjectValue() ).SetValue( static_cast< SInPostgreSqlBind< EFieldType_UINT8 > const & >( bind ).GetValue( row ) );
					break;

				case EFieldType_UINT16:
					static_cast< CDatabaseValue< EFieldType_UINT16 > & >( field->GetObjectValue() ).SetValue( static_cast< SInPostgreSqlBind< EFieldType_UINT16 > const & >( bind ).GetValue( row ) );
					break;

				case EFieldType_UINT24:
					static_cast< CDatabaseValue< EFieldType_UINT24 > & >( field->GetObjectValue() ).SetValue( static_cast< SInPostgreSqlBind< EFieldType_UINT24 > const & >( bind ).GetValue( row ) );
					break;

				case EFieldType_UINT32:
					static_cast< CDatabaseValue< EFieldType_UINT32 > & >( field->GetObjectValue() ).SetValue( static_cast< SInPostgreSqlBind< EFieldType_UINT32 > const & >( bind ).GetValue( row ) );
					break;

				case EFieldType_UINT64:
					static_cast< CDatabaseValue< EFieldType_UINT64 > & >( field->GetObjectValue() ).SetValue( static_cast< SInPostgreSqlBind< EFieldType_UINT64 > const & >( bind ).GetValue( row ) );
					break;

				case EFieldType_FLOAT32:
					static_cast< CDatabaseValue< EFieldType_FLOAT32 > & >( field->GetObjectValue() ).SetValue( static_cast< SInPostgreSqlBind< EFieldType_FLOAT32 > const & >( bind ).GetValue( row ) );
					break;

				case EFieldType_FLOAT64:
					static_cast< CDatabaseValue< EFieldType_FLOAT64 > & >( field->GetObjectValue() ).SetValue( static_cast< SInPostgreSqlBind< EFieldType_FLOAT64 > const & >( bind ).GetValue( row ) );
					break;

				case EFieldType_FIXED_POINT:
					static_cast< CDatabaseValue< EFieldType_FIXED_POINT > & >( field->GetObjectValue() ).SetValue( static_cast< SInPostgreSqlBind< EFieldType_FIXED_POINT > const & >( bind ).GetValue( row ) );
					break;

				case EFieldType_CHAR:
					static_cast< CDatabaseValue< EFieldType_CHAR > & >( field->GetObjectValue() ).SetValue( static_cast< SInPostgreSqlBind< EFieldType_CHAR > const & >( bind ).GetValue( row ).c_str() );
					break;

				case EFieldType_VARCHAR:
					static_cast< CDatabaseValue< EFieldType_VARCHAR > & >( field->GetObjectValue() ).SetValue( static_cast< SInPostgreSqlBind< EFieldType_VARCHAR > const & >( bind ).GetValue( row ).c_str() );
					break;

				case EFieldType_TEXT:
					static_cast< CDatabaseValue< EFieldType_TEXT > & >( field->GetObjectValue() ).SetValue( static_cast< SInPostgreSqlBind< EFieldType_TEXT > const & >( bind ).GetValue( row ) );
					break;

				case EFieldType_NCHAR:
					static_cast< CDatabaseValue< EFieldType_NCHAR > & >( field->GetObjectValue() ).SetValue( static_cast< SInPostgreSqlBind< EFieldType_NCHAR > const & >( bind ).GetValue( row ).c_str() );
					break;

				case EFieldType_NVARCHAR:
					static_cast< CDatabaseValue< EFieldType_NVARCHAR > & >( field->GetObjectValue() ).SetValue( static_cast< SInPostgreSqlBind< EFieldType_NVARCHAR > const & >( bind ).GetValue( row ).c_str() );
					break;

				case EFieldType_NTEXT:
					static_cast< CDatabaseValue< EFieldType_NTEXT > & >( field->GetObjectValue() ).SetValue( static_cast< SInPostgreSqlBind< EFieldType_NTEXT > const & >( bind ).GetValue( row ) );
					break;

				case EFieldType_DATE:
					static_cast< CDatabaseValue< EFieldType_DATE > & >( field->GetObjectValue() ).SetValue( static_cast< SInPostgreSqlBind< EFieldType_DATE > const & >( bind ).GetValue( row ) );
					break;

				case EFieldType_DATETIME:
					static_cast< CDatabaseValue< EFieldType_DATETIME > & >( field->GetObjectValue() ).SetValue( static_cast< SInPostgreSqlBind< EFieldType_DATETIME > const & >( bind ).GetValue( row ) );
					break;

				case EFieldType_TIME:
					static_cast< CDatabaseValue< EFieldType_TIME > & >( field->GetObjectValue() ).SetValue( static_cast< SInPostgreSqlBind< EFieldType_TIME > const & >( bind ).GetValue( row ) );
					break;

				case EFieldType_BINARY:
					static_cast< CDatabaseValue< EFieldType_BINARY > & >( field->GetObjectValue() ).SetValue( static_cast< SInPostgreSqlBind< EFieldType_BINARY > const & >( bind ).GetValue( row ) );
					break;

				case EFieldType_VARBINARY:
					static_cast< CDatabaseValue< EFieldType_VARBINARY > & >( field->GetObjectValue() ).SetValue( static_cast< SInPostgreSqlBind< EFieldType_VARBINARY > const & >( bind ).GetValue( row ) );
					break;

				case EFieldType_LONG_VARBINARY:
					static_cast< CDatabaseValue< EFieldType_LONG_VARBINARY > & >( field->GetObjectValue() ).SetValue( static_cast< SInPostgreSqlBind< EFieldType_LONG_VARBINARY > const & >( bind ).GetValue( row ) );
					break;

				default:
					break;
				}
			}
		}
	}

	//************************************************************************************************

	DatabaseFieldInfosPtrArray PostgreSqlGetColumns( PGresult * result, std::vector< std::unique_ptr< SInPostgreSqlBindBase > > & binds )
	{
		DatabaseFieldInfosPtrArray arrayReturn;
		int columnCount = PQnfields( result );
		binds.resize( columnCount );
		int index = 0;

		for ( auto && bind : binds )
		{
			char * name = PQfname( result, index );
			Oid oid = PQftype( result, index );
			int size = PQfsize( result, index );
			EFieldType type = GetFieldTypeFromOid( oid );
			arrayReturn.push_back( std::make_shared< CDatabaseFieldInfos >( StringUtils::ToString( name ), type, size ) );
			bind = std::move( GetInBind( type, index, result ) );
			++index;
		}

		return arrayReturn;
	}

	DatabaseResultSPtr PostgreSqlFetchResult( PGresult * result, DatabaseFieldInfosPtrArray const & columns, DatabaseConnectionPostgreSqlSPtr connection, std::vector< std::unique_ptr< SInPostgreSqlBindBase > > const & binds )
	{
		DatabaseResultSPtr pReturn;

		try
		{
			pReturn = std::make_unique< CDatabaseResult >( columns );
			int iNbColumns = int( columns.size() );
			int rowCount = PQntuples( result );

			for ( int i = 0; i < rowCount; ++i )
			{
				DatabaseRowSPtr pRow = std::make_shared< CDatabaseRow >();
				int index = 0;

				for ( auto && bind : binds )
				{
					DatabaseFieldInfosSPtr infos;

					try
					{
						infos = pReturn->GetFieldInfos( index++ );
					}
					catch ( CDatabaseException & )
					{
						throw;
					}

					DatabaseFieldSPtr field = std::make_shared< CDatabaseField >( connection, infos );
					SetFieldValue( infos, field, *bind, i );
					pRow->AddField( field );
				}

				pReturn->AddRow( pRow );
			}

			PQclear( result );
		}
		catch ( const CDatabaseException & e )
		{
			PQclear( result );
			StringStream message;
			message << ERROR_POSTGRESQL_DRIVER << STR( " - " )
					<< e.what();
			CLogger::LogError( message );
			POSTGRESQL_EXCEPT( EDatabaseExceptionCodes_GenericError, message.str() );
		}
		catch ( const std::exception & e )
		{
			PQclear( result );
			StringStream message;
			message << ERROR_POSTGRESQL_DRIVER << STR( " - " )
					<< e.what();
			CLogger::LogError( message );
			POSTGRESQL_EXCEPT( EDatabaseExceptionCodes_GenericError, message.str() );
		}
		catch ( ... )
		{
			PQclear( result );
			StringStream message;
			message << ERROR_POSTGRESQL_DRIVER << STR( " - " )
					<< ERROR_POSTGRESQL_UNKNOWN;
			CLogger::LogError( message );
			POSTGRESQL_EXCEPT( EDatabaseExceptionCodes_UnknownError, message.str() );
		}

		return pReturn;
	}

	String GetStatusName( ExecStatusType status )
	{
		String result;

		switch ( status )
		{
		case PGRES_EMPTY_QUERY:
			result = STR( "Empty query string was executed" );
			break;

		case PGRES_COMMAND_OK:
			result = STR( "A query command that doesn't return anything was executed properly by the backend" );
			break;

		case PGRES_TUPLES_OK:
			result = STR( "A query command that returns tuples was executed properly by the backend, PGresult contains the result tuples" );
			break;

		case PGRES_COPY_OUT:
			result = STR( "Copy Out data transfer in progress" );
			break;

		case PGRES_COPY_IN:
			result = STR( "Copy In data transfer in progress" );
			break;

		case PGRES_BAD_RESPONSE:
			result = STR( "an unexpected response was recv'd from the backend" );
			break;

		case PGRES_NONFATAL_ERROR:
			result = STR( "Notice or warning message" );
			break;

		case PGRES_FATAL_ERROR:
			result = STR( "Query failed" );
			break;

		case PGRES_COPY_BOTH:
			result = STR( "Copy In/Out data transfer in progress" );
			break;

		case PGRES_SINGLE_TUPLE:
			result = STR( "Single tuple from larger resultset" );
			break;
		}

		return result;
	}

	void PostgreSQLCheck( PGresult * result, TChar const * msg, EDatabaseExceptionCodes code, PGconn * connection )
	{
		if ( !result )
		{
			StringStream error;
			error << STR( "Failure: " ) << msg << std::endl;
			String postgresql = StringUtils::ToString( PQerrorMessage( connection ) );
			error << postgresql;
			PQclear( result );
			DB_EXCEPT( code, error.str() );
		}

		ExecStatusType status = PQresultStatus( result );

		if ( status == PGRES_NONFATAL_ERROR || status == PGRES_FATAL_ERROR || status == PGRES_BAD_RESPONSE )
		{
			StringStream error;
			error << msg << std::endl;
			String postgresql = StringUtils::ToString( PQerrorMessage( connection ) );
			error << STR( "(" ) << GetStatusName( status ) << STR( ") " ) << postgresql;

			if ( status == PGRES_NONFATAL_ERROR )
			{
				CLogger::LogWarning( error );
			}
			else
			{
				PQclear( result );
				DB_EXCEPT( code, STR( "Failure: " ) + error.str() );
			}
		}

#if !defined( NDEBUG )
		CLogger::LogDebug( StringStream() << STR( "Success : " ) << msg );
#endif
	}

	Oid GetOidFromFieldType( EFieldType type )
	{
		Oid result = 0;

		switch ( type )
		{
		case EFieldType_BIT:
			result = EOid_BOOL;
			break;

		case EFieldType_SINT8:
			result = EOid_INT2;
			break;

		case EFieldType_SINT16:
			result = EOid_INT2;
			break;

		case EFieldType_SINT32:
			result = EOid_INT4;
			break;

		case EFieldType_SINT64:
			result = EOid_INT8;
			break;

		case EFieldType_UINT8:
			result = EOid_INT2;
			break;

		case EFieldType_UINT16:
			result = EOid_INT2;
			break;

		case EFieldType_UINT32:
			result = EOid_INT4;
			break;

		case EFieldType_UINT64:
			result = EOid_INT8;
			break;

		case EFieldType_FLOAT32:
			result = EOid_FLOAT4;
			break;

		case EFieldType_FLOAT64:
			result = EOid_FLOAT8;
			break;

		case EFieldType_FIXED_POINT:
			result = EOid_NUMERIC;
			break;

		case EFieldType_CHAR:
			result = EOid_BPCHAR;
			break;

		case EFieldType_VARCHAR:
			result = EOid_VARCHAR;
			break;

		case EFieldType_TEXT:
			result = EOid_TEXT;
			break;

		case EFieldType_NCHAR:
			result = EOid_BPCHAR;
			break;

		case EFieldType_NVARCHAR:
			result = EOid_VARCHAR;
			break;

		case EFieldType_NTEXT:
			result = EOid_TEXT;
			break;

		case EFieldType_DATE:
			result = EOid_DATE;
			break;

		case EFieldType_DATETIME:
			result = EOid_TIMESTAMP;
			break;

		case EFieldType_TIME:
			result = EOid_TIME;
			break;

		case EFieldType_BINARY:
			result = EOid_BYTEA;
			break;

		case EFieldType_VARBINARY:
			result = EOid_BYTEA;
			break;

		case EFieldType_LONG_VARBINARY:
			result = EOid_BYTEA;
			break;
		}

		return result;
	}

	String GetStatusName( ConnStatusType status )
	{
		String result;

		switch ( status )
		{
		case CONNECTION_OK:
			result = STR( "OK" );
			break;

		case CONNECTION_BAD:
			result = STR( "Bad" );
			break;

		case CONNECTION_STARTED:
			result = STR( "Waiting for connection to be made." );
			break;

		case CONNECTION_MADE:
			result = STR( "Connection OK; waiting to send." );
			break;

		case CONNECTION_AWAITING_RESPONSE:
			result = STR( "Waiting for a response from the postmaster." );
			break;

		case CONNECTION_AUTH_OK:
			result = STR( "Received authentication; waiting for backend startup." );
			break;

		case CONNECTION_SETENV:
			result = STR( "Negotiating environment." );
			break;

		case CONNECTION_SSL_STARTUP:
			result = STR( "Negotiating SSL." );
			break;

		case CONNECTION_NEEDED:
			result = STR( "Internal state: connect() needed." );
			break;
		}

		return result;
	}
}
END_NAMESPACE_DATABASE_POSTGRESQL
