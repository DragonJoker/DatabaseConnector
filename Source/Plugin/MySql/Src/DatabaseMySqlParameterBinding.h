/************************************************************************//**
* @file DatabaseStatementParameterSetter.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 11:56:56 AM
*
*
* @brief SMySqlParameterValueSetterBase and SMySqlParameterValueSetter classes.
*
* @details Functors used to set the parameter value in a statement.
*
***************************************************************************/

#ifndef ___DATABASE_STATEMENTPARAMETER_SETTER_H___
#define ___DATABASE_STATEMENTPARAMETER_SETTER_H___

#include "DatabaseMySqlPrerequisites.h"

#include "DatabaseMySqlHelper.h"
#include "DatabaseParameterMySql.h"

#include <DatabaseStringUtils.h>
#include <DatabaseLogger.h>

#include <mysql.h>
#include <mysql_time.h>

#undef min
#undef max
#undef abs

BEGIN_NAMESPACE_DATABASE_MYSQL
{
	static const String ERROR_MYSQL_PARAMETER_VALUE = STR( "Can't set parameter value" );

	/** Tables of MySQL types association to EFieldTypes
	*/
	static const enum_field_types FieldTypes[EFieldType_COUNT] =
	{
		MYSQL_TYPE_NULL,		// EFieldType_NULL,
		MYSQL_TYPE_TINY,		// EFieldType_BIT,
		MYSQL_TYPE_TINY,		// EFieldType_SINT8,
		MYSQL_TYPE_SHORT,		// EFieldType_SINT16,
		MYSQL_TYPE_LONG,		// EFieldType_SINT24,
		MYSQL_TYPE_LONG,		// EFieldType_SINT32,
		MYSQL_TYPE_LONGLONG,	// EFieldType_SINT64,
		MYSQL_TYPE_TINY,		// EFieldType_UINT8,
		MYSQL_TYPE_SHORT,		// EFieldType_UINT16,
		MYSQL_TYPE_LONG,		// EFieldType_UINT24,
		MYSQL_TYPE_LONG,		// EFieldType_UINT32,
		MYSQL_TYPE_LONGLONG,	// EFieldType_UINT64,
		MYSQL_TYPE_FLOAT,		// EFieldType_FLOAT32,
		MYSQL_TYPE_DOUBLE,		// EFieldType_FLOAT64,
		MYSQL_TYPE_NEWDECIMAL,	// EFieldType_FIXED_POINT,
		MYSQL_TYPE_VARCHAR,		// EFieldType_CHAR,
		MYSQL_TYPE_VARCHAR,		// EFieldType_VARCHAR,
		MYSQL_TYPE_STRING,		// EFieldType_TEXT,
		MYSQL_TYPE_VARCHAR,		// EFieldType_NCHAR,
		MYSQL_TYPE_VARCHAR,		// EFieldType_NVARCHAR,
		MYSQL_TYPE_STRING,		// EFieldType_NTEXT,
		MYSQL_TYPE_DATE,		// EFieldType_DATE,
		MYSQL_TYPE_DATETIME,	// EFieldType_DATETIME,
		MYSQL_TYPE_TIME,		// EFieldType_TIME,
		MYSQL_TYPE_BLOB,		// EFieldType_BINARY,
		MYSQL_TYPE_BLOB,		// EFieldType_VARBINARY,
		MYSQL_TYPE_BLOB,		// EFieldType_LONG_VARBINARY,
	};

	/** Structure used to retrieve the MySQL data types from the field type
	*/
	template< EFieldType Type > class SFieldTypeMySqlDataTyper;

	/** Specialization for EFieldType_BOOL
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_BIT >
	{
		typedef bool FieldDataType;
	};

	/** Specialization for EFieldType_SINT8
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_SINT8 >
	{
		typedef int8_t FieldDataType;
	};

	/** Specialization for EFieldType_SINT16
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_SINT16 >
	{
		typedef int16_t FieldDataType;
	};

	/** Specialization for EFieldType_SINT24
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_SINT24 >
	{
		typedef int24_t FieldDataType;
	};

	/** Specialization for EFieldType_SINT32
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_SINT32 >
	{
		typedef int32_t FieldDataType;
	};

	/** Specialization for EFieldType_SINT64
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_SINT64 >
	{
		typedef int64_t FieldDataType;
	};

	/** Specialization for EFieldType_UINT8
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_UINT8 >
	{
		typedef uint8_t FieldDataType;
	};

	/** Specialization for EFieldType_UINT16
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_UINT16 >
	{
		typedef uint16_t FieldDataType;
	};

	/** Specialization for EFieldType_UINT24
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_UINT24 >
	{
		typedef uint24_t FieldDataType;
	};

	/** Specialization for EFieldType_UINT32
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_UINT32 >
	{
		typedef uint32_t FieldDataType;
	};

	/** Specialization for EFieldType_UINT64
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_UINT64 >
	{
		typedef uint64_t FieldDataType;
	};

	/** Specialization for EFieldType_FLOAT32
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_FLOAT32 >
	{
		typedef float FieldDataType;
	};

	/** Specialization for EFieldType_FLOAT64
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_FLOAT64 >
	{
		typedef double FieldDataType;
	};

	/** Specialization for EFieldType_FIXED_POINT
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_FIXED_POINT >
	{
		typedef CFixedPoint FieldDataType;
	};

	/** Specialization for EFieldType_DATE
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_DATE >
	{
		typedef DateType FieldDataType;
	};

	/** Specialization for EFieldType_DATETIME
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_DATETIME >
	{
		typedef DateTimeType FieldDataType;
	};

	/** Specialization for EFieldType_TIME
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_TIME >
	{
		typedef TimeType FieldDataType;
	};

	/** Specialization for EFieldType_CHAR
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_CHAR >
	{
		typedef char * FieldDataType;
	};

	/** Specialization for EFieldType_VARCHAR
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_VARCHAR >
	{
		typedef char * FieldDataType;
	};

	/** Specialization for EFieldType_TEXT
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_TEXT >
	{
		typedef char * FieldDataType;
	};

	/** Specialization for EFieldType_NCHAR
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_NCHAR >
	{
		typedef wchar_t * FieldDataType;
	};

	/** Specialization for EFieldType_NVARCHAR
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_NVARCHAR >
	{
		typedef wchar_t * FieldDataType;
	};

	/** Specialization for EFieldType_NTEXT
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_NTEXT >
	{
		typedef wchar_t * FieldDataType;
	};

	/** Specialization for EFieldType_BINARY
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_BINARY >
	{
		typedef uint8_t * FieldDataType;
	};

	/** Specialization for EFieldType_VARBINARY
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_VARBINARY >
	{
		typedef uint8_t * FieldDataType;
	};

	/** Specialization for EFieldType_LONG_VARBINARY
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_LONG_VARBINARY >
	{
		typedef uint8_t * FieldDataType;
	};

	SOutMySqlBindBase::SOutMySqlBindBase( MYSQL_BIND & bind, enum_field_types type, CDatabaseParameterMySql & parameter )
		: SMySqlBindBase( bind )
		, _statement( parameter.GetStatement() )
		, _connection( std::static_pointer_cast< CDatabaseConnectionMySql >( parameter.GetConnection() )->GetConnection() )
	{
		_bind.buffer_type = type;
	}

	/** Generic template class to update the binding from the parameter value
	*/
	template< typename T >
	struct SOutMySqlBind
		: public SOutMySqlBindBase
	{
		/** Constructor
		@param bind
			The binding
		@param type
			The MySQL data type
		@param value
			The parameter value
		@param parameter
			The parameter
		*/
		SOutMySqlBind( MYSQL_BIND & bind, enum_field_types type, CDatabaseValueBase & value, CDatabaseParameterMySql & parameter )
			: SOutMySqlBindBase( bind, type, parameter )
			, _value( value )
		{
			_bind.buffer_length = sizeof( T );
			_bind.buffer = value.GetPtrValue();
			_bind.is_unsigned = !std::numeric_limits< T >::is_signed;
		}

		//!@copydoc SOutMySqlBindBase::UpdateValue
		virtual void UpdateValue()
		{
			// Empty
		}

		//! The parameter value
		CDatabaseValueBase & _value;
	};

	/** SOutMySqlBind specialization for pointer types
	*/
	template< typename T >
	struct SOutMySqlBind< T * >
		: public SOutMySqlBindBase
	{
		/** Constructor
		@param bind
			The binding
		@param type
			The MySQL data type
		@param value
			The parameter value
		@param parameter
			The parameter
		*/
		SOutMySqlBind( MYSQL_BIND & bind, enum_field_types type, CDatabaseValueBase & value, CDatabaseParameterMySql & parameter )
			: SOutMySqlBindBase( bind, type, parameter )
			, _value( value )
		{
			if ( type == MYSQL_TYPE_STRING || type == MYSQL_TYPE_BLOB || parameter.GetLimits() == -1 || parameter.GetLimits() == 0 )
			{
				_updateFunc = &SOutMySqlBind< T * >::DoUpdateUnlimited;
			}
			else
			{
				_bind.buffer_length = parameter.GetLimits() * sizeof( T );
				_holder.resize( _bind.buffer_length );
				_bind.buffer = _holder.data();
				_updateFunc = &SOutMySqlBind< T * >::DoUpdateLimited;
			}
		}

		/** Destructor
		*/
		~SOutMySqlBind()
		{
		}

		//!@copydoc SOutMySqlBindBase::UpdateValue
		virtual void UpdateValue()
		{
			( this->*_updateFunc )();
		}

		/** Sends the value in traditional way, using the binding
		*/
		void DoUpdateLimited()
		{
			unsigned long length = std::min( _bind.buffer_length, _value.GetPtrSize() );
			memcpy( _holder.data(), _value.GetPtrValue(), length );
		}

		/** Sends the value using the mysql_send_long_data method
		*/
		void DoUpdateUnlimited()
		{
			MySqlSendLongData( _value, _bind.param_number, _statement, _connection );
		}

		//! The parameter value
		CDatabaseValueBase & _value;
		//! The value holder
		std::vector< T > _holder;
		//! The method used to send the value to the server
		void ( SOutMySqlBind< T * >::*_updateFunc )();
	};

	/** SOutMySqlBind specialization for wchar_t pointers, since MySQL only support char pointers
	*/
	template<>
	struct SOutMySqlBind< wchar_t * >
		: public SOutMySqlBindBase
	{
		/** Constructor
		@param bind
			The binding
		@param type
			The MySQL data type
		@param value
			The parameter value
		@param parameter
			The parameter
		*/
		SOutMySqlBind( MYSQL_BIND & bind, enum_field_types type, CDatabaseValueBase & value, CDatabaseParameterMySql & parameter )
			: SOutMySqlBindBase( bind, type, parameter )
			, _value( value )
			, _length( 0 )
		{
			_bind.length = &_length;

			if ( type == MYSQL_TYPE_STRING || parameter.GetLimits() == -1 || parameter.GetLimits() == 0 )
			{
				_updateFunc = &SOutMySqlBind< wchar_t * >::DoUpdateUnlimited;
			}
			else
			{
				_bind.buffer_length = parameter.GetLimits();
				_holder.resize( _bind.buffer_length );
				_bind.buffer = _holder.data();
				_updateFunc = &SOutMySqlBind< wchar_t * >::DoUpdateLimited;
			}
		}

		/** Destructor
		*/
		~SOutMySqlBind()
		{
		}

		//!@copydoc SOutMySqlBindBase::UpdateValue
		virtual void UpdateValue()
		{
			std::string str = StringUtils::ToStr( reinterpret_cast< const wchar_t * >( _value.GetPtrValue() ) );
			( this->*_updateFunc )( str );
		}

		/** Sends the value in traditional way, using the binding
		*/
		void DoUpdateLimited( const std::string & str )
		{
			_length = std::min( _bind.buffer_length, ( unsigned long )str.size() );
			memcpy( _holder.data(), str.data(), _length );
		}

		/** Sends the value using the mysql_send_long_data method
		*/
		void DoUpdateUnlimited( const std::string & str )
		{
			MySqlSendLongData( _value, _bind.param_number, _statement, _connection );
		}

		CDatabaseValueBase & _value;
		//! The value holder
		std::vector< char > _holder;
		//! The binding length
		unsigned long _length;
		//! The method used to send the value to the server
		void ( SOutMySqlBind< wchar_t * >::*_updateFunc )( const std::string & );
	};

	/** SOutMySqlBind specialization for bool
	*/
	template<>
	struct SOutMySqlBind< bool >
		: public SOutMySqlBindBase
	{
		/** Constructor
		@param bind
			The binding
		@param type
			The MySQL data type
		@param value
			The parameter value
		@param parameter
			The parameter
		*/
		SOutMySqlBind( MYSQL_BIND & bind, enum_field_types type, CDatabaseValue< EFieldType_BIT > & value, CDatabaseParameterMySql & parameter )
			: SOutMySqlBindBase( bind, type, parameter )
			, _value( value )
			, _holder( 0 )
		{
			_bind.buffer_length = sizeof( _holder );
			*_bind.length = _bind.buffer_length;
			_bind.buffer = &_holder;
		}

		//!@copydoc SOutMySqlBindBase::UpdateValue
		virtual void UpdateValue()
		{
			_holder = _value.GetValue() ? 1 : 0;
		}

		//! The parameter value
		CDatabaseValue< EFieldType_BIT > & _value;
		//! The value holder
		int8_t _holder;
	};

	/** SOutMySqlBind specialization for int24_t
	*/
	template<>
	struct SOutMySqlBind< int24_t >
		: public SOutMySqlBindBase
	{
		/** Constructor
		@param bind
			The binding
		@param type
			The MySQL data type
		@param value
			The parameter value
		@param parameter
			The parameter
		*/
		SOutMySqlBind( MYSQL_BIND & bind, enum_field_types type, CDatabaseValue< EFieldType_SINT24 > & value, CDatabaseParameterMySql & parameter )
			: SOutMySqlBindBase( bind, type, parameter )
			, _value( value )
		{
			_bind.buffer_length = sizeof( int32_t );
			_bind.buffer = &_holder;
			_bind.is_unsigned = false;
		}

		//!@copydoc SOutMySqlBindBase::UpdateValue
		virtual void UpdateValue()
		{
			_holder = int32_t( _value.GetValue() );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_SINT24 > & _value;
		//! The value holder
		int32_t _holder;
	};

	/** SOutMySqlBind specialization for uint24_t
	*/
	template<>
	struct SOutMySqlBind< uint24_t >
		: public SOutMySqlBindBase
	{
		/** Constructor
		@param bind
			The binding
		@param type
			The MySQL data type
		@param value
			The parameter value
		@param parameter
			The parameter
		*/
		SOutMySqlBind( MYSQL_BIND & bind, enum_field_types type, CDatabaseValue< EFieldType_UINT24 > & value, CDatabaseParameterMySql & parameter )
			: SOutMySqlBindBase( bind, type, parameter )
			, _value( value )
		{
			_bind.buffer_length = sizeof( uint32_t );
			_bind.buffer = &_holder;
			_bind.is_unsigned = true;
		}

		//!@copydoc SOutMySqlBindBase::UpdateValue
		virtual void UpdateValue()
		{
			_holder = uint32_t( _value.GetValue() );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_UINT24 > & _value;
		//! The value holder
		uint32_t _holder;
	};

	/** SOutMySqlBind specialization for CFixedPoint
	*/
	template<>
	struct SOutMySqlBind< CFixedPoint >
		: public SOutMySqlBindBase
	{
		/** Constructor
		@param bind
			The binding
		@param type
			The MySQL data type
		@param value
			The parameter value
		@param parameter
			The parameter
		*/
		SOutMySqlBind( MYSQL_BIND & bind, enum_field_types type, CDatabaseValue< EFieldType_FIXED_POINT > & value, CDatabaseParameterMySql & parameter )
			: SOutMySqlBindBase( bind, type, parameter )
			, _value( value )
			, _length( 0 )
			,_holder( { 0 } )
		{
			_bind.length = &_length;
			_bind.buffer_length = static_cast< unsigned long >( _holder.size() );
			bind.buffer = _holder.data();
		}

		//!@copydoc SOutMySqlBindBase::UpdateValue
		virtual void UpdateValue()
		{
			String value = _value.GetValue().ToString();
			_length = static_cast< unsigned long >( value.size() );
			assert( _length < 32 );
			strcpy( _holder.data(), value.data() );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_FIXED_POINT > & _value;
		//! The value holder
		std::array< char, 32 > _holder;
		//! The binding length
		unsigned long _length;
	};

	/** SOutMySqlBind specialization for CDate
	*/
	template<>
	struct SOutMySqlBind< DateType >
		: public SOutMySqlBindBase
	{
		/** Constructor
		@param bind
			The binding
		@param type
			The MySQL data type
		@param value
			The parameter value
		@param parameter
			The parameter
		*/
		SOutMySqlBind( MYSQL_BIND & bind, enum_field_types type, CDatabaseValue< EFieldType_DATE > & value, CDatabaseParameterMySql & parameter )
			: SOutMySqlBindBase( bind, type, parameter )
			, _value( value )
			, _holder( { 0 } )
		{
			_holder.time_type = MYSQL_TIMESTAMP_DATE;
			_bind.buffer_length = sizeof( _holder );
			bind.buffer = &_holder;
		}

		//!@copydoc SOutMySqlBindBase::UpdateValue
		virtual void UpdateValue()
		{
			_holder.year = _value.GetValue().year();
			_holder.month = _value.GetValue().month();
			_holder.day = _value.GetValue().day();
		}

		//! The parameter value
		CDatabaseValue< EFieldType_DATE > & _value;
		//! The value holder
		MYSQL_TIME _holder;
	};

	/** SOutMySqlBind specialization for DateTimeType
	*/
	template<>
	struct SOutMySqlBind< DateTimeType >
		: public SOutMySqlBindBase
	{
		/** Constructor
		@param bind
			The binding
		@param type
			The MySQL data type
		@param value
			The parameter value
		@param parameter
			The parameter
		*/
		SOutMySqlBind( MYSQL_BIND & bind, enum_field_types type, CDatabaseValue< EFieldType_DATETIME > & value, CDatabaseParameterMySql & parameter )
			: SOutMySqlBindBase( bind, type, parameter )
			, _value( value )
			, _holder( { 0 } )
		{
			_holder.time_type = MYSQL_TIMESTAMP_DATETIME;
			_bind.buffer_length = sizeof( _holder );
			bind.buffer = &_holder;
		}

		//!@copydoc SOutMySqlBindBase::UpdateValue
		virtual void UpdateValue()
		{
			_holder.year = _value.GetValue().date().year();
			_holder.month = _value.GetValue().date().month();
			_holder.day = _value.GetValue().date().day();
			_holder.hour = _value.GetValue().time_of_day().hours();
			_holder.minute = _value.GetValue().time_of_day().minutes();
			_holder.second = _value.GetValue().time_of_day().seconds();
		}

		//! The parameter value
		CDatabaseValue< EFieldType_DATETIME > & _value;
		//! The value holder
		MYSQL_TIME _holder;
	};

	/** SOutMySqlBind specialization for TimeType
	*/
	template<>
	struct SOutMySqlBind< TimeType >
		: public SOutMySqlBindBase
	{
		/** Constructor
		@param bind
			The binding
		@param type
			The MySQL data type
		@param value
			The parameter value
		@param parameter
			The parameter
		*/
		SOutMySqlBind( MYSQL_BIND & bind, enum_field_types type, CDatabaseValue< EFieldType_TIME > & value, CDatabaseParameterMySql & parameter )
			: SOutMySqlBindBase( bind, type, parameter )
			, _value( value )
			, _holder( { 0 } )
		{
			_holder.time_type = MYSQL_TIMESTAMP_TIME;
			_bind.buffer_length = sizeof( _holder );
			bind.buffer = &_holder;
		}

		//!@copydoc SOutMySqlBindBase::UpdateValue
		virtual void UpdateValue()
		{
			_holder.hour = _value.GetValue().hours();
			_holder.minute = _value.GetValue().minutes();
			_holder.second = _value.GetValue().seconds();
		}

		//! The parameter value
		CDatabaseValue< EFieldType_TIME > & _value;
		//! The value holder
		MYSQL_TIME _holder;
	};

	/** Function used to create a SOutMySqlBind easily
	@param bind
		The binding
	@param value
		The parameter value
	@param parameter
		The parameter
	*/
	template< EFieldType Type > std::unique_ptr< SOutMySqlBindBase > MakeOutBind( MYSQL_BIND & bind, CDatabaseValueBase & value, CDatabaseParameterMySql & parameter )
	{
		typedef SFieldTypeMySqlDataTyper< Type > typer_type;
		return std::make_unique< SOutMySqlBind< typename typer_type::FieldDataType > >( bind, FieldTypes[Type], static_cast< CDatabaseValue< Type > & >( value ), parameter );
	}
}
END_NAMESPACE_DATABASE_MYSQL

#endif // ___DATABASE_STATEMENTPARAMETER_SETTER_H___
