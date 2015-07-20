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
		BYTEAOID,		// EFieldType_LONG_VARBINARY,
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
		typedef CDate FieldDataType;
	};

	/** Specialization for EFieldType_DATETIME
	*/
	template<> struct SFieldTypePostgreSqlDataTyper< EFieldType_DATETIME >
	{
		typedef CDateTime FieldDataType;
	};

	/** Specialization for EFieldType_TIME
	*/
	template<> struct SFieldTypePostgreSqlDataTyper< EFieldType_TIME >
	{
		typedef CTime FieldDataType;
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

	/** Specialization for EFieldType_LONG_VARBINARY
	*/
	template<> struct SFieldTypePostgreSqlDataTyper< EFieldType_LONG_VARBINARY >
	{
		typedef uint8_t * FieldDataType;
	};

	/** Generic template class to update the binding from the parameter value
	*/
	template< typename T >
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
		SOutPostgreSqlBind( PGbind & bind, Oid type, CDatabaseValueBase & value, CDatabaseParameterPostgreSql & parameter )
			: SOutPostgreSqlBindBase( bind, type, parameter )
			, _value( value )
		{
		}

		//!@copydoc SOutPostgreSqlBindBase::DoUpdateValue
		virtual void DoUpdateValue()
		{
			_bind.length = sizeof( T );
			_bind.value = static_cast< char * >( _value.GetPtrValue() );
		}

		//! The parameter value
		CDatabaseValueBase & _value;
	};

	/** SOutPostgreSqlBind specialization for pointer types
	*/
	template< typename T >
	struct SOutPostgreSqlBind< T * >
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
		SOutPostgreSqlBind( PGbind & bind, Oid type, CDatabaseValueBase & value, CDatabaseParameterPostgreSql & parameter )
			: SOutPostgreSqlBindBase( bind, type, parameter )
			, _value( value )
		{
		}

		/** Destructor
		*/
		~SOutPostgreSqlBind()
		{
		}

		//!@copydoc SOutPostgreSqlBindBase::DoUpdateValue
		virtual void DoUpdateValue()
		{
			_bind.length = int( _value.GetPtrSize() );
			memcpy( _holder.data(), _value.GetPtrValue(), _bind.length );
			_bind.value = reinterpret_cast< char * >( _holder.data() );
		}

		//! The parameter value
		CDatabaseValueBase & _value;
		//! The value holder
		std::vector< T > _holder;
		//! The method used to send the value to the server
		void ( SOutPostgreSqlBind< T * >::*_updateFunc )();
	};

	/** SOutPostgreSqlBind specialization for wchar_t pointers, since PostgreSQL only support char pointers
	*/
	template<>
	struct SOutPostgreSqlBind< wchar_t * >
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
		SOutPostgreSqlBind( PGbind & bind, Oid type, CDatabaseValueBase & value, CDatabaseParameterPostgreSql & parameter )
			: SOutPostgreSqlBindBase( bind, type, parameter )
			, _value( value )
		{
		}

		/** Destructor
		*/
		~SOutPostgreSqlBind()
		{
		}

		//!@copydoc SOutPostgreSqlBindBase::DoUpdateValue
		virtual void DoUpdateValue()
		{
			std::string str = CStrUtils::ToStr( reinterpret_cast< const wchar_t * >( _value.GetPtrValue() ) );
			_bind.length = int( str.size() );
			_holder.resize( _bind.length );
			memcpy( _holder.data(), str.data(), _bind.length );
			_bind.value = _holder.data();
		}

		CDatabaseValueBase & _value;
		//! The value holder
		std::vector< char > _holder;
	};

	/** SOutPostgreSqlBind specialization for bool
	*/
	template<>
	struct SOutPostgreSqlBind< bool >
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
			, _holder( 0 )
		{
		}

		//!@copydoc SOutPostgreSqlBindBase::DoUpdateValue
		virtual void DoUpdateValue()
		{
			_holder = _value.GetValue() ? 1 : 0;
			_bind.length = sizeof( _holder );
			_bind.value = reinterpret_cast< char * >( &_holder );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_BIT > & _value;
		//! The value holder
		int8_t _holder;
	};

	/** SOutPostgreSqlBind specialization for int24_t
	*/
	template<>
	struct SOutPostgreSqlBind< int24_t >
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
		SOutPostgreSqlBind( PGbind & bind, Oid type, CDatabaseValue< EFieldType_SINT24 > & value, CDatabaseParameterPostgreSql & parameter )
			: SOutPostgreSqlBindBase( bind, type, parameter )
			, _value( value )
		{
		}

		//!@copydoc SOutPostgreSqlBindBase::DoUpdateValue
		virtual void DoUpdateValue()
		{
			_holder = int32_t( _value.GetValue() );
			_bind.length = sizeof( _holder );
			_bind.value = reinterpret_cast< char * >( &_holder );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_SINT24 > & _value;
		//! The value holder
		int32_t _holder;
	};

	/** SOutPostgreSqlBind specialization for uint24_t
	*/
	template<>
	struct SOutPostgreSqlBind< uint24_t >
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
		SOutPostgreSqlBind( PGbind & bind, Oid type, CDatabaseValue< EFieldType_UINT24 > & value, CDatabaseParameterPostgreSql & parameter )
			: SOutPostgreSqlBindBase( bind, type, parameter )
			, _value( value )
		{
		}

		//!@copydoc SOutPostgreSqlBindBase::DoUpdateValue
		virtual void DoUpdateValue()
		{
			_holder = uint32_t( _value.GetValue() );
			_bind.length = sizeof( _holder );
			_bind.value = reinterpret_cast< char * >( &_holder );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_UINT24 > & _value;
		//! The value holder
		uint32_t _holder;
	};

	/** SOutPostgreSqlBind specialization for CFixedPoint
	*/
	template<>
	struct SOutPostgreSqlBind< CFixedPoint >
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
			, _length( 0 )
			, _holder( { 0 } )
		{
		}

		//!@copydoc SOutPostgreSqlBindBase::DoUpdateValue
		virtual void DoUpdateValue()
		{
			String value = _value.GetValue().ToString();
			_length = static_cast< unsigned long >( value.size() );
			assert( _length < 32 );
			strcpy( _holder.data(), value.data() );
			_bind.length = _holder.size();
			_bind.value = _holder.data();
		}

		//! The parameter value
		CDatabaseValue< EFieldType_FIXED_POINT > & _value;
		//! The value holder
		std::array< char, 32 > _holder;
		//! The binding length
		unsigned long _length;
	};

	/** SOutPostgreSqlBind specialization for CDate
	*/
	template<>
	struct SOutPostgreSqlBind< CDate >
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
			, _holder( { 0 } )
		{
		}

		//!@copydoc SOutPostgreSqlBindBase::DoUpdateValue
		virtual void DoUpdateValue()
		{
			//_holder.year = _value.GetValue().GetYear();
			//_holder.month = _value.GetValue().GetMonth() + 1;
			//_holder.day = _value.GetValue().GetMonthDay();
			_bind.length = sizeof( _holder );
			_bind.value = reinterpret_cast< char * >( &_holder );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_DATE > & _value;
		//! The value holder
		date _holder;
	};

	/** SOutPostgreSqlBind specialization for CDateTime
	*/
	template<>
	struct SOutPostgreSqlBind< CDateTime >
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
			//_holder.year = _value.GetValue().GetYear();
			//_holder.month = _value.GetValue().GetMonth() + 1;
			//_holder.day = _value.GetValue().GetMonthDay();
			//_holder.hour = _value.GetValue().GetHour();
			//_holder.minute = _value.GetValue().GetMinute();
			//_holder.second = _value.GetValue().GetSecond();
			_bind.length = sizeof( _holder );
			_bind.value = reinterpret_cast< char * >( &_holder );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_DATETIME > & _value;
		//! The value holder
		timestamp _holder;
	};

	/** SOutPostgreSqlBind specialization for CTime
	*/
	template<>
	struct SOutPostgreSqlBind< CTime >
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
			, _holder( { 0 } )
		{
		}

		//!@copydoc SOutPostgreSqlBindBase::DoUpdateValue
		virtual void DoUpdateValue()
		{
			//_holder.hour = _value.GetValue().GetHour();
			//_holder.minute = _value.GetValue().GetMinute();
			//_holder.second = _value.GetValue().GetSecond();
			_bind.length = sizeof( _holder );
			_bind.value = reinterpret_cast< char * >( &_holder );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_TIME > & _value;
		//! The value holder
		time_t _holder;
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
		return std::make_unique< SOutPostgreSqlBind< typename typer_type::FieldDataType > >( bind, FieldTypes[Type], static_cast< CDatabaseValue< Type > & >( value ), parameter );
	}
}
END_NAMESPACE_DATABASE_POSTGRESQL

#endif // ___DATABASE_STATEMENTPARAMETER_SETTER_H___
