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
		_bind.format = 1;
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
			value_type const & GetValue( int row )const
			{
				char * value = PQgetvalue( _result, row, _index );
				uint32_t val( *reinterpret_cast< value_type * >( value ) );
				return value_type( ntohl( val ) );
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
				return int8_t( *value ) != 0;
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
				char * value = PQgetvalue( _result, row, _index );
				int32_t val = ntohl( *reinterpret_cast< int32_t * >( value ) );
				return int24_t( val );
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
				char * value = PQgetvalue( _result, row, _index );
				uint32_t val = ntohl( *reinterpret_cast< uint32_t * >( value ) );
				return uint24_t( val );
			}
		};

		/** SInPostgreSqlBind specialisation for EFieldType_SINT24
		@remarks
			Holds the value as an int32_t.
			The conversion is done when the call is made to GetValue
		*/
		template<>
		struct SInPostgreSqlBind< EFieldType_SINT64 >
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
			int64_t GetValue( int row )const
			{
				char * value = PQgetvalue( _result, row, _index );
				int32_t valLo( *reinterpret_cast< int32_t * >( value ) );
				int32_t valHi( *reinterpret_cast< int32_t * >( value + 4 ) );
				return int64_t( ntohl( valLo ) + ( ntohl( valHi ) << 32 ) );
			}
		};

		/** SInPostgreSqlBind specialisation for EFieldType_UINT24
		@remarks
			Holds the value as an uint32_t.
			The conversion is done when the call is made to GetValue
		*/
		template<>
		struct SInPostgreSqlBind< EFieldType_UINT64 >
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
			uint64_t GetValue( int row )const
			{
				char * value = PQgetvalue( _result, row, _index );
				uint32_t valLo( *reinterpret_cast< uint32_t * >( value ) );
				uint32_t valHi( *reinterpret_cast< uint32_t * >( value + 4 ) );
				return uint64_t( ntohl( valLo ) + ( ntohl( valHi ) << 32 ) );
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
				return CStrUtils::ToWStr( PQgetvalue( _result, row, _index ) );
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
				char * value = PQgetvalue( _result, row, _index );
				int length = PQgetlength( _result, row, _index );
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
				//TODO Retrieve precision and decimals
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
			CDate GetValue( int row )const
			{
				char * value = PQgetvalue( _result, row, _index );
				return CDate( _value.year, EDateMonth( _value.month - 1 ), _value.day );
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
			CDateTime GetValue( int row )const
			{
				char * value = PQgetvalue( _result, row, _index );
				return CDateTime( CDate( _value.year, EDateMonth( _value.month - 1 ), _value.day ), CTime( _value.hour, _value.minute, _value.second ) );
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
			CTime GetValue( int row )const
			
				char * value = PQgetvalue( _result, row, _index );
				return CTime( _value.hour, _value.minute, _value.second );
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

		std::unique_ptr< SInPostgreSqlBindBase > GetInBind( EFieldType type, int index, PGresult * result, std::uint32_t precision, std::uint32_t decimals, uint32_t length )
		{
			std::unique_ptr< SInPostgreSqlBindBase > result;

			switch ( type )
			{
			case EFieldType_BIT:
				result = std::make_unique< SInPostgreSqlBind< EFieldType_BIT > >( index, result );
				break;

			case EFieldType_SINT8:
				result = std::make_unique< SInPostgreSqlBind< EFieldType_SINT8 > >( index, result );
				break;

			case EFieldType_SINT16:
				result = std::make_unique< SInPostgreSqlBind< EFieldType_SINT16 > >( index, result );
				break;

			case EFieldType_SINT24:
				result = std::make_unique< SInPostgreSqlBind< EFieldType_SINT24 > >( index, result );
				break;

			case EFieldType_SINT32:
				result = std::make_unique< SInPostgreSqlBind< EFieldType_SINT32 > >( index, result );
				break;

			case EFieldType_SINT64:
				result = std::make_unique< SInPostgreSqlBind< EFieldType_SINT64 > >( index, result );
				break;

			case EFieldType_UINT8:
				result = std::make_unique< SInPostgreSqlBind< EFieldType_UINT8 > >( index, result );
				break;

			case EFieldType_UINT16:
				result = std::make_unique< SInPostgreSqlBind< EFieldType_UINT16 > >( index, result );
				break;

			case EFieldType_UINT24:
				result = std::make_unique< SInPostgreSqlBind< EFieldType_UINT24 > >( index, result );
				break;

			case EFieldType_UINT32:
				result = std::make_unique< SInPostgreSqlBind< EFieldType_UINT32 > >( index, result );
				break;

			case EFieldType_UINT64:
				result = std::make_unique< SInPostgreSqlBind< EFieldType_UINT64 > >( index, result );
				break;

			case EFieldType_FLOAT32:
				result = std::make_unique< SInPostgreSqlBind< EFieldType_FLOAT32 > >( index, result );
				break;

			case EFieldType_FLOAT64:
				result = std::make_unique< SInPostgreSqlBind< EFieldType_FLOAT64 > >( index, result );
				break;

			case EFieldType_FIXED_POINT:
				result = std::make_unique< SInPostgreSqlBind< EFieldType_FIXED_POINT > >( index, result, precision, decimals );
				break;

			case EFieldType_DATE:
				result = std::make_unique< SInPostgreSqlBind< EFieldType_DATE > >( index, result );
				break;

			case EFieldType_DATETIME:
				result = std::make_unique< SInPostgreSqlBind< EFieldType_DATETIME > >( index, result );
				break;

			case EFieldType_TIME:
				result = std::make_unique< SInPostgreSqlBind< EFieldType_TIME > >( index, result );
				break;

			case EFieldType_VARCHAR:
				result = std::make_unique< SInPostgreSqlBind< EFieldType_VARCHAR > >( index, result, length );
				break;

			case EFieldType_TEXT:
				result = std::make_unique< SInPostgreSqlBind< EFieldType_TEXT > >( index, result, length );
				break;

			case EFieldType_BINARY:
				result = std::make_unique< SInPostgreSqlBind< EFieldType_BINARY > >( index, result, length );
				break;

			case EFieldType_VARBINARY:
				result = std::make_unique< SInPostgreSqlBind< EFieldType_VARBINARY > >( index, result, length );
				break;

			case EFieldType_LONG_VARBINARY:
				result = std::make_unique< SInPostgreSqlBind< EFieldType_LONG_VARBINARY > >( index, result, length );
				break;
			}

			return result;
		}

		void SetFieldValue( DatabaseFieldInfosPtr infos, DatabaseFieldPtr field, SInPostgreSqlBindBase const & bind, int row )
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

				case EFieldType_VARCHAR:
					static_cast< CDatabaseValue< EFieldType_VARCHAR > & >( field->GetObjectValue() ).SetValue( static_cast< SInPostgreSqlBind< EFieldType_VARCHAR > const & >( bind ).GetValue( row ).c_str() );
					break;

				case EFieldType_TEXT:
					static_cast< CDatabaseValue< EFieldType_TEXT > & >( field->GetObjectValue() ).SetValue( static_cast< SInPostgreSqlBind< EFieldType_TEXT > const & >( bind ).GetValue( row ) );
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

	DatabaseFieldInfosPtrArray PostgreSqlGetColumns( PGresult * result, DatabaseConnectionPostgreSqlPtr connection, std::vector< std::unique_ptr< SInPostgreSqlBindBase > > & binds )
	{
		DatabaseFieldInfosPtrArray arrayReturn;
		int columnCount = PQnfields( result );
		binds.resize( columnCount, { 0 } );
		int index = 0;

		for ( auto && bind : binds )
		{
			bind = { 0 };
			char * name = PQfname( result, index );
			Oid oid = PQftype( result, index );
			int modif = PQfmod( result, index );
			int size = PQfsize( result, index );
			EFieldType type = GetFieldTypeFromOid( oid );
			arrayReturn.push_back( std::make_shared< CDatabaseFieldInfos >( connection, CStrUtils::ToString( name ), type, size ) );
			binds.emplace_back( GetInBind( type, index, result, size, modif, size ) );
		}

		return arrayReturn;
	}

	DatabaseResultPtr PostgreSqlFetchResult( PGresult * result, DatabaseFieldInfosPtrArray const & columns, DatabaseConnectionPostgreSqlPtr connection, std::vector< std::unique_ptr< SInPostgreSqlBindBase > > const & binds )
	{
		DatabaseResultPtr pReturn;

		try
		{
			if ( connection->IsConnected() )
			{
				pReturn = std::make_unique< CDatabaseResult >( connection, columns );
				int iNbColumns = int( columns.size() );
				int rowCount = PQntuples( result );

				for ( int i = 0; i < rowCount; ++i )
				{
					DatabaseRowPtr pRow = std::make_shared< CDatabaseRow >( connection );
					int index = 0;

					for ( auto && bind : binds )
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
							SetFieldValue( infos, field, *bind, i );
						}

						pRow->AddField( field );
					}

					pReturn->AddRow( pRow );
				}
			}
		}
		catch ( const CExceptionDatabase & e )
		{
			StringStream message;
			message << ERROR_POSTGRESQL_DRIVER << STR( " - " )
					<< e.what();
			CLogger::LogError( message );
			POSTGRESQL_EXCEPT( EDatabasePostgreSqlExceptionCodes_GenericError, message.str() );
		}
		catch ( const std::exception & e )
		{
			StringStream message;
			message << ERROR_POSTGRESQL_DRIVER << STR( " - " )
					<< e.what();
			CLogger::LogError( message );
			POSTGRESQL_EXCEPT( EDatabasePostgreSqlExceptionCodes_GenericError, message.str() );
		}
		catch ( ... )
		{
			StringStream message;
			message << ERROR_POSTGRESQL_DRIVER << STR( " - " )
					<< ERROR_POSTGRESQL_UNKNOWN;
			CLogger::LogError( message );
			POSTGRESQL_EXCEPT( EDatabasePostgreSqlExceptionCodes_UnknownError, message.str() );
		}

		return pReturn;
	}

	void PostgreSQLCheck( PGresult * result, TChar const * msg, EDatabaseExceptionCodes code, PGconn * connection )
	{
		if ( !result || PQresultStatus( result ) != PGRES_COMMAND_OK )
		{
			StringStream error;
			error << STR( "Failure: " ) << msg << std::endl;
			String postgresql = CStrUtils::ToString( PQerrorMessage( connection ) );
			error << STR( "(" ) << result << STR( ") " ) << postgresql;
			DB_EXCEPT( code, error.str() );
			PQclear( result );
		}

#if !defined( NDEBUG )

		else
		{
			CLogger::LogDebug( StringStream() << STR( "Success : " ) << msg );
		}

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
				result = EOid_CHAR;
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
				result = EOid_CHAR;
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

			case EFieldType_VARCHAR:
				result = EOid_VARCHAR;
				break;

			case EFieldType_TEXT:
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
	}
}
END_NAMESPACE_DATABASE_POSTGRESQL
