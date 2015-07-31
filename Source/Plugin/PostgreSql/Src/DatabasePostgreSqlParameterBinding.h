/************************************************************************//**
* @file DatabaseStatementParameterSetter.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 11:56:56 AM
*
*
* @brief SPostgreSqlParameterValueSetterBase and SPostgreSqlParameterValueSetter classes.
*
* @details Functors used to set the parameter value in a statement.
*
***************************************************************************/

#ifndef ___DATABASE_STATEMENTPARAMETER_SETTER_H___
#define ___DATABASE_STATEMENTPARAMETER_SETTER_H___

#include "DatabasePostgreSqlPrerequisites.h"

#include "DatabasePostgreSqlHelper.h"
#include "DatabaseParameterPostgreSql.h"
#include "DatabaseConnectionPostgreSql.h"

#include <DatabaseStringUtils.h>
#include <DatabaseLogger.h>

#include <postgres.h>
#include <libpq-fe.h>
#include <pgtypes_date.h>
#include <pgtypes_timestamp.h>
#include <catalog/pg_type.h>

#if defined( _WIN32 )
#	include <Winsock2.h>
#	undef min
#	undef max
#	undef abs
#else
#	include <netinet/in.h>
#	include <arpa/inet.h>
#endif

BEGIN_NAMESPACE_DATABASE_POSTGRESQL
{
	static const String ERROR_POSTGRESQL_PARAMETER_VALUE = STR( "Can't set parameter value" );
	static const TChar * INFO_ESCAPING_BINARY = STR( "Escaping byte array" );

	/** Tables of PostgreSQL types association to EFieldTypes
	*/
	static const Oid FieldTypes[EFieldType_COUNT] =
	{
		0,				// EFieldType_NULL,
		BOOLOID,		// EFieldType_BIT,
		CHAROID,		// EFieldType_SINT8,
		INT2OID,		// EFieldType_SINT16,
		INT4OID,		// EFieldType_SINT24,
		INT4OID,		// EFieldType_SINT32,
		INT8OID,		// EFieldType_SINT64,
		CHAROID,		// EFieldType_UINT8,
		INT2OID,		// EFieldType_UINT16,
		INT4OID,		// EFieldType_UINT24,
		INT4OID,		// EFieldType_UINT32,
		INT8OID,		// EFieldType_UINT64,
		FLOAT4OID,		// EFieldType_FLOAT32,
		FLOAT8OID,		// EFieldType_FLOAT64,
		NUMERICOID,		// EFieldType_FIXED_POINT,
		BPCHAROID,		// EFieldType_CHAR,
		VARCHAROID,		// EFieldType_VARCHAR,
		TEXTOID,		// EFieldType_TEXT,
		BPCHAROID,		// EFieldType_NCHAR,
		VARCHAROID,		// EFieldType_NVARCHAR,
		TEXTOID,		// EFieldType_NTEXT,
		DATEOID,		// EFieldType_DATE,
		TIMESTAMPOID,	// EFieldType_DATETIME,
		TIMEOID,		// EFieldType_TIME,
		BYTEAOID,		// EFieldType_BINARY,
		BYTEAOID,		// EFieldType_VARBINARY,
		BYTEAOID,		// EFieldType_BLOB,
	};

	/** Structure used to retrieve the PostgreSQL data types from the field type
	*/
	template< EFieldType Type > class SFieldTypePostgreSqlDataTyper;

	/** Specialization for EFieldType_BOOL
	*/
	template<> struct SFieldTypePostgreSqlDataTyper< EFieldType_BIT >
	{
		typedef bool FieldDataType;
	};

	/** Specialization for EFieldType_SINT8
	*/
	template<> struct SFieldTypePostgreSqlDataTyper< EFieldType_SINT8 >
	{
		typedef int8_t FieldDataType;
	};

	/** Specialization for EFieldType_SINT16
	*/
	template<> struct SFieldTypePostgreSqlDataTyper< EFieldType_SINT16 >
	{
		typedef int16_t FieldDataType;
	};

	/** Specialization for EFieldType_SINT24
	*/
	template<> struct SFieldTypePostgreSqlDataTyper< EFieldType_SINT24 >
	{
		typedef int24_t FieldDataType;
	};

	/** Specialization for EFieldType_SINT32
	*/
	template<> struct SFieldTypePostgreSqlDataTyper< EFieldType_SINT32 >
	{
		typedef int32_t FieldDataType;
	};

	/** Specialization for EFieldType_SINT64
	*/
	template<> struct SFieldTypePostgreSqlDataTyper< EFieldType_SINT64 >
	{
		typedef int64_t FieldDataType;
	};

	/** Specialization for EFieldType_UINT8
	*/
	template<> struct SFieldTypePostgreSqlDataTyper< EFieldType_UINT8 >
	{
		typedef uint8_t FieldDataType;
	};

	/** Specialization for EFieldType_UINT16
	*/
	template<> struct SFieldTypePostgreSqlDataTyper< EFieldType_UINT16 >
	{
		typedef uint16_t FieldDataType;
	};

	/** Specialization for EFieldType_UINT24
	*/
	template<> struct SFieldTypePostgreSqlDataTyper< EFieldType_UINT24 >
	{
		typedef uint24_t FieldDataType;
	};

	/** Specialization for EFieldType_UINT32
	*/
	template<> struct SFieldTypePostgreSqlDataTyper< EFieldType_UINT32 >
	{
		typedef uint32_t FieldDataType;
	};

	/** Specialization for EFieldType_UINT64
	*/
	template<> struct SFieldTypePostgreSqlDataTyper< EFieldType_UINT64 >
	{
		typedef uint64_t FieldDataType;
	};

	/** Specialization for EFieldType_FLOAT32
	*/
	template<> struct SFieldTypePostgreSqlDataTyper< EFieldType_FLOAT32 >
	{
		typedef float FieldDataType;
	};

	/** Specialization for EFieldType_FLOAT64
	*/
	template<> struct SFieldTypePostgreSqlDataTyper< EFieldType_FLOAT64 >
	{
		typedef double FieldDataType;
	};

	/** Specialization for EFieldType_FIXED_POINT
	*/
	template<> struct SFieldTypePostgreSqlDataTyper< EFieldType_FIXED_POINT >
	{
		typedef CFixedPoint FieldDataType;
	};

	/** Specialization for EFieldType_DATE
	*/
	template<> struct SFieldTypePostgreSqlDataTyper< EFieldType_DATE >
	{
		typedef DateType FieldDataType;
	};

	/** Specialization for EFieldType_DATETIME
	*/
	template<> struct SFieldTypePostgreSqlDataTyper< EFieldType_DATETIME >
	{
		typedef DateTimeType FieldDataType;
	};

	/** Specialization for EFieldType_TIME
	*/
	template<> struct SFieldTypePostgreSqlDataTyper< EFieldType_TIME >
	{
		typedef TimeType FieldDataType;
	};

	/** Specialization for EFieldType_CHAR
	*/
	template<> struct SFieldTypePostgreSqlDataTyper< EFieldType_CHAR >
	{
		typedef char * FieldDataType;
	};

	/** Specialization for EFieldType_VARCHAR
	*/
	template<> struct SFieldTypePostgreSqlDataTyper< EFieldType_VARCHAR >
	{
		typedef char * FieldDataType;
	};

	/** Specialization for EFieldType_TEXT
	*/
	template<> struct SFieldTypePostgreSqlDataTyper< EFieldType_TEXT >
	{
		typedef char * FieldDataType;
	};

	/** Specialization for EFieldType_NCHAR
	*/
	template<> struct SFieldTypePostgreSqlDataTyper< EFieldType_NCHAR >
	{
		typedef wchar_t * FieldDataType;
	};

	/** Specialization for EFieldType_NVARCHAR
	*/
	template<> struct SFieldTypePostgreSqlDataTyper< EFieldType_NVARCHAR >
	{
		typedef wchar_t * FieldDataType;
	};

	/** Specialization for EFieldType_NTEXT
	*/
	template<> struct SFieldTypePostgreSqlDataTyper< EFieldType_NTEXT >
	{
		typedef wchar_t * FieldDataType;
	};

	/** Specialization for EFieldType_BINARY
	*/
	template<> struct SFieldTypePostgreSqlDataTyper< EFieldType_BINARY >
	{
		typedef uint8_t * FieldDataType;
	};

	/** Specialization for EFieldType_VARBINARY
	*/
	template<> struct SFieldTypePostgreSqlDataTyper< EFieldType_VARBINARY >
	{
		typedef uint8_t * FieldDataType;
	};

	/** Specialization for EFieldType_BLOB
	*/
	template<> struct SFieldTypePostgreSqlDataTyper< EFieldType_BLOB >
	{
		typedef uint8_t * FieldDataType;
	};

	/** Generic template class to update the binding from the parameter value
	*/
	template< EFieldType FieldType >
	struct SOutPostgreSqlBind
		: public SOutPostgreSqlBindBase
	{
		/** Constructor
		@param bind
			The binding
		@param type
			The PostgreSQL data type
		@param value
			The parameter value
		@param parameter
			The parameter
		*/
		SOutPostgreSqlBind( PGbind & bind, Oid type, CDatabaseValue< FieldType > & value, CDatabaseParameterPostgreSql & parameter )
			: SOutPostgreSqlBindBase( bind, type, parameter )
			, _value( value )
		{
		}

		//!@copydoc SOutPostgreSqlBindBase::DoUpdateValue
		virtual void DoUpdateValue()
		{
			std::stringstream stream;
			stream << _value.GetValue();
			std::string value = stream.str();
			_bind.length = int( value.size() );
			assert( _bind.length < _holder.size() );
			strcpy( _holder.data(), value.data() );
			_bind.value = _holder.data();
		}

		//! The parameter value
		CDatabaseValue< FieldType > & _value;
		//! The parameter value holder
		std::array< char, 32 > _holder;
	};

	/** SOutPostgreSqlBind specialization for EFieldType_SINT8
	*/
	template<>
	struct SOutPostgreSqlBind< EFieldType_SINT8 >
		: public SOutPostgreSqlBindBase
	{
		/** Constructor
		@param bind
			The binding
		@param type
			The PostgreSQL data type
		@param value
			The parameter value
		@param parameter
			The parameter
		*/
		SOutPostgreSqlBind( PGbind & bind, Oid type, CDatabaseValue< EFieldType_SINT8 > & value, CDatabaseParameterPostgreSql & parameter )
			: SOutPostgreSqlBindBase( bind, type, parameter )
			, _value( value )
		{
		}

		//!@copydoc SOutPostgreSqlBindBase::DoUpdateValue
		virtual void DoUpdateValue()
		{
			std::stringstream stream;
			stream << int16_t( _value.GetValue() );
			std::string value = stream.str();
			_bind.length = int( value.size() );
			assert( _bind.length < _holder.size() );
			strcpy( _holder.data(), value.data() );
			_bind.value = _holder.data();
		}

		//! The parameter value
		CDatabaseValue< EFieldType_SINT8 > & _value;
		//! The value holder
		std::array< char, 5 > _holder;
	};

	/** SOutPostgreSqlBind specialization for EFieldType_SINT8
	*/
	template<>
	struct SOutPostgreSqlBind< EFieldType_UINT8 >
		: public SOutPostgreSqlBindBase
	{
		/** Constructor
		@param bind
			The binding
		@param type
			The PostgreSQL data type
		@param value
			The parameter value
		@param parameter
			The parameter
		*/
		SOutPostgreSqlBind( PGbind & bind, Oid type, CDatabaseValue< EFieldType_UINT8 > & value, CDatabaseParameterPostgreSql & parameter )
			: SOutPostgreSqlBindBase( bind, type, parameter )
			, _value( value )
		{
		}

		//!@copydoc SOutPostgreSqlBindBase::DoUpdateValue
		virtual void DoUpdateValue()
		{
			std::stringstream stream;
			stream << uint16_t( _value.GetValue() );
			std::string value = stream.str();
			_bind.length = int( value.size() );
			assert( _bind.length < _holder.size() );
			strcpy( _holder.data(), value.data() );
			_bind.value = _holder.data();
		}

		//! The parameter value
		CDatabaseValue< EFieldType_UINT8 > & _value;
		//! The value holder
		std::array< char, 4 > _holder;
	};

	/** Base binding class for string types
	*/
	struct SStringOutPostgreSqlBind
		: public SOutPostgreSqlBindBase
	{
		/** Constructor
		@param bind
			The binding
		@param type
			The PostgreSQL data type
		@param value
			The parameter value
		@param parameter
			The parameter
		*/
		SStringOutPostgreSqlBind( PGbind & bind, Oid type, CDatabaseValueBase & value, CDatabaseParameterPostgreSql & parameter )
			: SOutPostgreSqlBindBase( bind, type, parameter )
			, _value( value )
		{
		}

		//!@copydoc SOutPostgreSqlBindBase::DoUpdateValue
		virtual void DoUpdateValue()
		{
			_bind.length = int( _value.GetPtrSize() );
			_holder.resize( _bind.length + 1 );
			strcpy( _holder.data(), reinterpret_cast< const char * >( _value.GetPtrValue() ) );
			_bind.value = _holder.data();
		}

		//! The parameter value
		CDatabaseValueBase & _value;
		//! The value holder
		std::vector< char > _holder;
	};

	/** SOutPostgreSqlBind specialization for EFieldType_CHAR
	*/
	template<>
	struct SOutPostgreSqlBind< EFieldType_CHAR >
		: public SStringOutPostgreSqlBind
	{
		/** Constructor
		@param bind
			The binding
		@param type
			The PostgreSQL data type
		@param value
			The parameter value
		@param parameter
			The parameter
		*/
		SOutPostgreSqlBind( PGbind & bind, Oid type, CDatabaseValue< EFieldType_CHAR > & value, CDatabaseParameterPostgreSql & parameter )
			: SStringOutPostgreSqlBind( bind, type, value, parameter )
		{
		}
	};

	/** SOutPostgreSqlBind specialization for EFieldType_VARCHAR
	*/
	template<>
	struct SOutPostgreSqlBind< EFieldType_VARCHAR >
		: public SStringOutPostgreSqlBind
	{
		/** Constructor
		@param bind
			The binding
		@param type
			The PostgreSQL data type
		@param value
			The parameter value
		@param parameter
			The parameter
		*/
		SOutPostgreSqlBind( PGbind & bind, Oid type, CDatabaseValue< EFieldType_VARCHAR > & value, CDatabaseParameterPostgreSql & parameter )
			: SStringOutPostgreSqlBind( bind, type, value, parameter )
		{
		}
	};

	/** SOutPostgreSqlBind specialization for EFieldType_TEXT
	*/
	template<>
	struct SOutPostgreSqlBind< EFieldType_TEXT >
		: public SStringOutPostgreSqlBind
	{
		/** Constructor
		@param bind
			The binding
		@param type
			The PostgreSQL data type
		@param value
			The parameter value
		@param parameter
			The parameter
		*/
		SOutPostgreSqlBind( PGbind & bind, Oid type, CDatabaseValue< EFieldType_TEXT > & value, CDatabaseParameterPostgreSql & parameter )
			: SStringOutPostgreSqlBind( bind, type, value, parameter )
		{
		}
	};

	/** Base binding class for wide string types
	*/
	struct SWStringOutPostgreSqlBind
		: public SOutPostgreSqlBindBase
	{
		/** Constructor
		@param bind
			The binding
		@param type
			The PostgreSQL data type
		@param value
			The parameter value
		@param parameter
			The parameter
		*/
		SWStringOutPostgreSqlBind( PGbind & bind, Oid type, CDatabaseValueBase & value, CDatabaseParameterPostgreSql & parameter )
			: SOutPostgreSqlBindBase( bind, type, parameter )
			, _value( value )
		{
		}

		//!@copydoc SOutPostgreSqlBindBase::DoUpdateValue
		virtual void DoUpdateValue()
		{
			std::string value = StringUtils::ToStr( reinterpret_cast< const wchar_t * >( _value.GetPtrValue() ) );
			_bind.length = int( value.size() );
			_holder.resize( _bind.length + 1 );
			strcpy( _holder.data(), value.c_str() );
			_bind.value = _holder.data();
		}

		//! The parameter value
		CDatabaseValueBase & _value;
		//! The value holder
		std::vector< char > _holder;
	};

	/** SOutPostgreSqlBind specialization for EFieldType_NCHAR
	*/
	template<>
	struct SOutPostgreSqlBind< EFieldType_NCHAR >
		: public SWStringOutPostgreSqlBind
	{
		/** Constructor
		@param bind
			The binding
		@param type
			The PostgreSQL data type
		@param value
			The parameter value
		@param parameter
			The parameter
		*/
		SOutPostgreSqlBind( PGbind & bind, Oid type, CDatabaseValue< EFieldType_NCHAR > & value, CDatabaseParameterPostgreSql & parameter )
			: SWStringOutPostgreSqlBind( bind, type, value, parameter )
		{
		}
	};

	/** SOutPostgreSqlBind specialization for EFieldType_NVARCHAR
	*/
	template<>
	struct SOutPostgreSqlBind< EFieldType_NVARCHAR >
		: public SWStringOutPostgreSqlBind
	{
		/** Constructor
		@param bind
			The binding
		@param type
			The PostgreSQL data type
		@param value
			The parameter value
		@param parameter
			The parameter
		*/
		SOutPostgreSqlBind( PGbind & bind, Oid type, CDatabaseValue< EFieldType_NVARCHAR > & value, CDatabaseParameterPostgreSql & parameter )
			: SWStringOutPostgreSqlBind( bind, type, value, parameter )
		{
		}
	};

	/** SOutPostgreSqlBind specialization for EFieldType_NTEXT
	*/
	template<>
	struct SOutPostgreSqlBind< EFieldType_NTEXT >
		: public SWStringOutPostgreSqlBind
	{
		/** Constructor
		@param bind
			The binding
		@param type
			The PostgreSQL data type
		@param value
			The parameter value
		@param parameter
			The parameter
		*/
		SOutPostgreSqlBind( PGbind & bind, Oid type, CDatabaseValue< EFieldType_NTEXT > & value, CDatabaseParameterPostgreSql & parameter )
			: SWStringOutPostgreSqlBind( bind, type, value, parameter )
		{
		}
	};

	/** Base binding class for byte array types
	*/
	struct SBinaryOutPostgreSqlBind
		: public SOutPostgreSqlBindBase
	{
		/** Constructor
		@param bind
			The binding
		@param type
			The PostgreSQL data type
		@param value
			The parameter value
		@param parameter
			The parameter
		*/
		SBinaryOutPostgreSqlBind( PGbind & bind, Oid type, CDatabaseValueBase & value, CDatabaseParameterPostgreSql & parameter )
			: SOutPostgreSqlBindBase( bind, type, parameter )
			, _value( value )
		{
		}

		//!@copydoc SOutPostgreSqlBindBase::DoUpdateValue
		virtual void DoUpdateValue()
		{
			size_t length = 0;
			uint8_t * escaped = PQescapeByteaConn( _connection, reinterpret_cast< const uint8_t * >( _value.GetPtrValue() ), _value.GetPtrSize(), &length );

			if ( !escaped )
			{
				PostgreSQLCheck( NULL, INFO_ESCAPING_BINARY, EDatabaseExceptionCodes_ConnectionError, _connection );
			}
			
			std::string value = StringUtils::ToString( reinterpret_cast< char * >( escaped ) );
			PQfreemem( escaped );
			_bind.length = int( length );
			_holder.resize( length + 1 );
			strcpy( _holder.data(), value.c_str() );
			_bind.value = _holder.data();
		}

		//! The parameter value
		CDatabaseValueBase & _value;
		//! The value holder
		std::vector< char > _holder;
	};

	/** SOutPostgreSqlBind specialization for EFieldType_BINARY
	*/
	template<>
	struct SOutPostgreSqlBind< EFieldType_BINARY >
		: public SBinaryOutPostgreSqlBind
	{
		/** Constructor
		@param bind
			The binding
		@param type
			The PostgreSQL data type
		@param value
			The parameter value
		@param parameter
			The parameter
		*/
		SOutPostgreSqlBind( PGbind & bind, Oid type, CDatabaseValue< EFieldType_BINARY > & value, CDatabaseParameterPostgreSql & parameter )
			: SBinaryOutPostgreSqlBind( bind, type, value, parameter )
		{
		}
	};

	/** SOutPostgreSqlBind specialization for EFieldType_VARBINARY
	*/
	template<>
	struct SOutPostgreSqlBind< EFieldType_VARBINARY >
		: public SBinaryOutPostgreSqlBind
	{
		/** Constructor
		@param bind
			The binding
		@param type
			The PostgreSQL data type
		@param value
			The parameter value
		@param parameter
			The parameter
		*/
		SOutPostgreSqlBind( PGbind & bind, Oid type, CDatabaseValue< EFieldType_VARBINARY > & value, CDatabaseParameterPostgreSql & parameter )
			: SBinaryOutPostgreSqlBind( bind, type, value, parameter )
		{
		}
	};

	/** SOutPostgreSqlBind specialization for EFieldType_BLOB
	*/
	template<>
	struct SOutPostgreSqlBind< EFieldType_BLOB >
		: public SBinaryOutPostgreSqlBind
	{
		/** Constructor
		@param bind
			The binding
		@param type
			The PostgreSQL data type
		@param value
			The parameter value
		@param parameter
			The parameter
		*/
		SOutPostgreSqlBind( PGbind & bind, Oid type, CDatabaseValue< EFieldType_BLOB > & value, CDatabaseParameterPostgreSql & parameter )
			: SBinaryOutPostgreSqlBind( bind, type, value, parameter )
		{
		}
	};

	/** SOutPostgreSqlBind specialization for EFieldType_BIT
	*/
	template<>
	struct SOutPostgreSqlBind< EFieldType_BIT >
		: public SOutPostgreSqlBindBase
	{
		/** Constructor
		@param bind
			The binding
		@param type
			The PostgreSQL data type
		@param value
			The parameter value
		@param parameter
			The parameter
		*/
		SOutPostgreSqlBind( PGbind & bind, Oid type, CDatabaseValue< EFieldType_BIT > & value, CDatabaseParameterPostgreSql & parameter )
			: SOutPostgreSqlBindBase( bind, type, parameter )
			, _value( value )
		{
		}

		//!@copydoc SOutPostgreSqlBindBase::DoUpdateValue
		virtual void DoUpdateValue()
		{
			strcpy( _holder.data(), _value.GetValue() ? "true" : "false" );
			_bind.length = _value.GetValue() ? 4 : 5;
			_bind.value = _holder.data();
		}

		//! The parameter value
		CDatabaseValue< EFieldType_BIT > & _value;
		//! The value holder
		std::array< char, 6 > _holder;
	};

	/** SOutPostgreSqlBind specialization for EFieldType_FIXED_POINT
	*/
	template<>
	struct SOutPostgreSqlBind< EFieldType_FIXED_POINT >
		: public SOutPostgreSqlBindBase
	{
		/** Constructor
		@param bind
			The binding
		@param type
			The PostgreSQL data type
		@param value
			The parameter value
		@param parameter
			The parameter
		*/
		SOutPostgreSqlBind( PGbind & bind, Oid type, CDatabaseValue< EFieldType_FIXED_POINT > & value, CDatabaseParameterPostgreSql & parameter )
			: SOutPostgreSqlBindBase( bind, type, parameter )
			, _value( value )
		{
		}

		//!@copydoc SOutPostgreSqlBindBase::DoUpdateValue
		virtual void DoUpdateValue()
		{
			std::string value = StringUtils::ToStr( _value.GetValue().ToString() );
			_bind.length = int( value.size() );
			assert( _bind.length < _holder.size() );
			strcpy( _holder.data(), value.data() );
			_bind.value = _holder.data();
		}

		//! The parameter value
		CDatabaseValue< EFieldType_FIXED_POINT > & _value;
		//! The value holder
		std::array< char, 32 > _holder;
	};

	/** SOutPostgreSqlBind specialization for EFieldType_DATE
	*/
	template<>
	struct SOutPostgreSqlBind< EFieldType_DATE >
		: public SOutPostgreSqlBindBase
	{
		/** Constructor
		@param bind
			The binding
		@param type
			The PostgreSQL data type
		@param value
			The parameter value
		@param parameter
			The parameter
		*/
		SOutPostgreSqlBind( PGbind & bind, Oid type, CDatabaseValue< EFieldType_DATE > & value, CDatabaseParameterPostgreSql & parameter )
			: SOutPostgreSqlBindBase( bind, type, parameter )
			, _value( value )
		{
		}

		//!@copydoc SOutPostgreSqlBindBase::DoUpdateValue
		virtual void DoUpdateValue()
		{
			std::string value = Date::Format( _value.GetValue(), POSTGRE_FORMAT_DATE );
			_bind.length = int( value.size() );
			assert( _bind.length < _holder.size() );
			strcpy( _holder.data(), value.data() );
			_bind.value = reinterpret_cast< char * >( &_holder );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_DATE > & _value;
		//! The value holder
		std::array< char, 11 > _holder;
	};

	/** SOutPostgreSqlBind specialization for EFieldType_DATETIME
	*/
	template<>
	struct SOutPostgreSqlBind< EFieldType_DATETIME >
		: public SOutPostgreSqlBindBase
	{
		/** Constructor
		@param bind
			The binding
		@param type
			The PostgreSQL data type
		@param value
			The parameter value
		@param parameter
			The parameter
		*/
		SOutPostgreSqlBind( PGbind & bind, Oid type, CDatabaseValue< EFieldType_DATETIME > & value, CDatabaseParameterPostgreSql & parameter )
			: SOutPostgreSqlBindBase( bind, type, parameter )
			, _value( value )
			, _holder( { 0 } )
		{
		}

		//!@copydoc SOutPostgreSqlBindBase::DoUpdateValue
		virtual void DoUpdateValue()
		{
			std::string value = DateTime::Format( _value.GetValue(), POSTGRE_FORMAT_DATETIME );
			_bind.length = int( value.size() );
			assert( _bind.length < _holder.size() );
			strcpy( _holder.data(), value.data() );
			_bind.value = reinterpret_cast< char * >( &_holder );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_DATETIME > & _value;
		//! The value holder
		std::array< char, 20 > _holder;
	};

	/** SOutPostgreSqlBind specialization for EFieldType_TIME
	*/
	template<>
	struct SOutPostgreSqlBind< EFieldType_TIME >
		: public SOutPostgreSqlBindBase
	{
		/** Constructor
		@param bind
			The binding
		@param type
			The PostgreSQL data type
		@param value
			The parameter value
		@param parameter
			The parameter
		*/
		SOutPostgreSqlBind( PGbind & bind, Oid type, CDatabaseValue< EFieldType_TIME > & value, CDatabaseParameterPostgreSql & parameter )
			: SOutPostgreSqlBindBase( bind, type, parameter )
			, _value( value )
		{
		}

		//!@copydoc SOutPostgreSqlBindBase::DoUpdateValue
		virtual void DoUpdateValue()
		{
			std::string value = Time::Format( _value.GetValue(), POSTGRE_FORMAT_TIME );
			_bind.length = int( value.size() );
			assert( _bind.length < _holder.size() );
			strcpy( _holder.data(), value.data() );
			_bind.value = reinterpret_cast< char * >( &_holder );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_TIME > & _value;
		//! The value holder
		std::array< char, 7 > _holder;
	};

	/** Function used to create a SOutPostgreSqlBind easily
	@param bind
		The binding
	@param value
		The parameter value
	@param parameter
		The parameter
	*/
	template< EFieldType Type > std::unique_ptr< SOutPostgreSqlBindBase > MakeOutBind( PGbind & bind, CDatabaseValueBase & value, CDatabaseParameterPostgreSql & parameter )
	{
		typedef SFieldTypePostgreSqlDataTyper< Type > typer_type;
		return std::make_unique< SOutPostgreSqlBind< Type > >( bind, FieldTypes[Type], static_cast< CDatabaseValue< Type > & >( value ), parameter );
	}
}
END_NAMESPACE_DATABASE_POSTGRESQL

#endif // ___DATABASE_STATEMENTPARAMETER_SETTER_H___
