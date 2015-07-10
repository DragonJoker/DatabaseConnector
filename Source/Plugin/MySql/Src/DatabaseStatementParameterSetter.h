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

#include "DatabaseStatementParameterMySql.h"

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

	/** Structure used to retrieve the MySQL data types from the field type
	*/
	template< EFieldType Type > class SFieldTypeMySqlDataTyper;

	/** Specialization for EFieldType_BOOL
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_BOOL >
	{
		static const enum_field_types MySqlFieldType = MYSQL_TYPE_TINY;
		typedef bool DataType;
	};

	/** Specialization for EFieldType_SMALL_INTEGER
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_SMALL_INTEGER >
	{
		static const enum_field_types MySqlFieldType = MYSQL_TYPE_SHORT;
		typedef int16_t DataType;
	};

	/** Specialization for EFieldType_INTEGER
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_INTEGER >
	{
		static const enum_field_types MySqlFieldType = MYSQL_TYPE_LONG;
		typedef int32_t DataType;
	};

	/** Specialization for EFieldType_LONG_INTEGER
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_LONG_INTEGER >
	{
		static const enum_field_types MySqlFieldType = MYSQL_TYPE_LONGLONG;
		typedef int64_t DataType;
	};

	/** Specialization for EFieldType_FLOAT
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_FLOAT >
	{
		static const enum_field_types MySqlFieldType = MYSQL_TYPE_FLOAT;
		typedef float DataType;
	};

	/** Specialization for EFieldType_DOUBLE
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_DOUBLE >
	{
		static const enum_field_types MySqlFieldType = MYSQL_TYPE_DOUBLE;
		typedef double DataType;
	};

	/** Specialization for EFieldType_DATE
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_DATE >
	{
		static const enum_field_types MySqlFieldType = MYSQL_TYPE_DATE;
		typedef CDate DataType;
	};

	/** Specialization for EFieldType_DATETIME
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_DATETIME >
	{
		static const enum_field_types MySqlFieldType = MYSQL_TYPE_DATETIME;
		typedef CDateTime DataType;
	};

	/** Specialization for EFieldType_TIME
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_TIME >
	{
		static const enum_field_types MySqlFieldType = MYSQL_TYPE_TIME;
		typedef CTime DataType;
	};

	/** Specialization for EFieldType_VARCHAR
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_VARCHAR >
	{
		static const enum_field_types MySqlFieldType = MYSQL_TYPE_VARCHAR;
		typedef char * DataType;
	};

	/** Specialization for EFieldType_TEXT
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_TEXT >
	{
		static const enum_field_types MySqlFieldType = MYSQL_TYPE_STRING;
		typedef char * DataType;
	};

	/** Specialization for EFieldType_NVARCHAR
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_NVARCHAR >
	{
		static const enum_field_types MySqlFieldType = MYSQL_TYPE_VARCHAR;
		typedef wchar_t * DataType;
	};

	/** Specialization for EFieldType_NTEXT
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_NTEXT >
	{
		static const enum_field_types MySqlFieldType = MYSQL_TYPE_STRING;
		typedef wchar_t * DataType;
	};

	/** Specialization for EFieldType_BINARY
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_BINARY >
	{
		static const enum_field_types MySqlFieldType = MYSQL_TYPE_BLOB;
		typedef uint8_t * DataType;
	};

	/** Specialization for EFieldType_VARBINARY
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_VARBINARY >
	{
		static const enum_field_types MySqlFieldType = MYSQL_TYPE_BLOB;
		typedef uint8_t * DataType;
	};

	/** Specialization for EFieldType_LONG_VARBINARY
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_LONG_VARBINARY >
	{
		static const enum_field_types MySqlFieldType = MYSQL_TYPE_BLOB;
		typedef uint8_t * DataType;
	};

	COutMySqlBindBase::COutMySqlBindBase( MYSQL_BIND & bind, enum_field_types type, CDatabaseStatementParameterMySql & parameter )
		: CMySqlBindBase( bind )
		, _statement( parameter.GetStatement() )
		, _connection( std::static_pointer_cast< CDatabaseConnectionMySql >( parameter.GetConnection() )->GetConnection() )
	{
		_bind.buffer_type = type;
	}

	/** Generic template class to update the binding from the parameter value
	*/
	template< typename T >
	struct COutMySqlBind
			: public COutMySqlBindBase
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
		COutMySqlBind( MYSQL_BIND & bind, enum_field_types type, CDatabaseValueBase & value, CDatabaseStatementParameterMySql & parameter )
			: COutMySqlBindBase( bind, type, parameter )
			, _value( value )
		{
			_bind.buffer_length = sizeof( T );
			bind.buffer = value.GetPtrValue();
		}

		//!@copydoc COutMySqlBindBase::DoSetValue
		virtual void UpdateValue()
		{
			// Empty
		}

		//! The parameter value
		CDatabaseValueBase & _value;
	};

	/** Specialization for pointer types
	*/
	template< typename T >
	struct COutMySqlBind< T * >
			: public COutMySqlBindBase
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
		COutMySqlBind( MYSQL_BIND & bind, enum_field_types type, CDatabaseValueBase & value, CDatabaseStatementParameterMySql & parameter )
			: COutMySqlBindBase( bind, type, parameter )
			, _value( value )
			, _holder( NULL )
		{
			if ( type == MYSQL_TYPE_STRING || type == MYSQL_TYPE_BLOB || parameter.GetLimits() == -1 || parameter.GetLimits() == 0 )
			{
				_updateFunc = &COutMySqlBind< T * >::DoUpdateUnlimited;
			}
			else
			{
				_bind.buffer_length = parameter.GetLimits() * sizeof( T );
				_holder = ( T * )malloc( _bind.buffer_length );
				_bind.buffer = _holder;
				_updateFunc = &COutMySqlBind< T * >::DoUpdateLimited;
			}
		}

		/** Destructor
		*/
		~COutMySqlBind()
		{
			free( _holder );
		}

		//!@copydoc COutMySqlBindBase::DoSetValue
		virtual void UpdateValue()
		{
			( this->*_updateFunc )();
		}

		/** Sends the value in traditional way, using the binding
		*/
		void DoUpdateLimited()
		{
			unsigned long length = std::min( _bind.buffer_length, _value.GetPtrSize() );
			memcpy( _holder, _value.GetPtrValue(), length );
		}

		/** Sends the value using the mysql_send_long_data method
		*/
		void DoUpdateUnlimited()
		{
			MySqlSendLongData( _value, _bind, _statement, _connection );
		}

		//! The parameter value
		CDatabaseValueBase & _value;
		//! The value holder
		T * _holder;
		//! The method used to send the value to the server
		void ( COutMySqlBind< T * >::*_updateFunc )();
	};

	/** Specialization for wchar_t pointers, since MySQL only support char pointers
	*/
	template<>
	struct COutMySqlBind< wchar_t * >
			: public COutMySqlBindBase
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
		COutMySqlBind( MYSQL_BIND & bind, enum_field_types type, CDatabaseValueBase & value, CDatabaseStatementParameterMySql & parameter )
			: COutMySqlBindBase( bind, type, parameter )
			, _value( value )
			, _holder( NULL )
			, _length( 0 )
		{
			_bind.length = &_length;

			if ( type == MYSQL_TYPE_STRING || parameter.GetLimits() == -1 || parameter.GetLimits() == 0 )
			{
				_updateFunc = &COutMySqlBind< wchar_t * >::DoUpdateUnlimited;
			}
			else
			{
				_bind.buffer_length = parameter.GetLimits();
				_holder = ( char * )malloc( _bind.buffer_length );
				_bind.buffer = _holder;
				_updateFunc = &COutMySqlBind< wchar_t * >::DoUpdateLimited;
			}
		}

		/** Destructor
		*/
		~COutMySqlBind()
		{
			free( _holder );
		}

		//!@copydoc COutMySqlBindBase::DoSetValue
		virtual void UpdateValue()
		{
			std::string str = CStrUtils::ToStr( reinterpret_cast< const wchar_t * >( _value.GetPtrValue() ) );
			( this->*_updateFunc )( str );
		}

		/** Sends the value in traditional way, using the binding
		*/
		void DoUpdateLimited( const std::string & str )
		{
			_length = std::min( _bind.buffer_length, ( unsigned long )str.size() );
			memcpy( _holder, str.data(), _length );
		}

		/** Sends the value using the mysql_send_long_data method
		*/
		void DoUpdateUnlimited( const std::string & str )
		{
			MySqlSendLongData( _value, _bind, _statement, _connection );
		}

		CDatabaseValueBase & _value;
		//! The value holder
		char * _holder;
		//! The binding length
		unsigned long _length;
		//! The method used to send the value to the server
		void ( COutMySqlBind< wchar_t * >::*_updateFunc )( const std::string & );
	};

	/** Specialization for bool
	*/
	template<>
	struct COutMySqlBind< bool >
			: public COutMySqlBindBase
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
		COutMySqlBind( MYSQL_BIND & bind, enum_field_types type, CDatabaseValue< EFieldType_BOOL > & value, CDatabaseStatementParameterMySql & parameter )
			: COutMySqlBindBase( bind, type, parameter )
			, _value( value )
		{
			_bind.buffer_length = sizeof( _holder );
			bind.buffer = &_holder;
		}

		//!@copydoc COutMySqlBindBase::DoSetValue
		virtual void UpdateValue()
		{
			_holder = _value.GetValue() ? 1 : 0;
		}

		//! The parameter value
		CDatabaseValue< EFieldType_BOOL > & _value;
		//! The value holder
		int8_t _holder;
	};

	/** Specialization for CDate
	*/
	template<>
	struct COutMySqlBind< CDate >
			: public COutMySqlBindBase
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
		COutMySqlBind( MYSQL_BIND & bind, enum_field_types type, CDatabaseValue< EFieldType_DATE > & value, CDatabaseStatementParameterMySql & parameter )
			: COutMySqlBindBase( bind, type, parameter )
			, _value( value )
		{
			_bind.buffer_length = sizeof( _holder );
			bind.buffer = &_holder;
		}

		//!@copydoc COutMySqlBindBase::DoSetValue
		virtual void UpdateValue()
		{
			_holder = MySqlTimeFromCDate( _value.GetValue() );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_DATE > & _value;
		//! The value holder
		MYSQL_TIME _holder;
	};

	/** Specialization for CDateTime
	*/
	template<>
	struct COutMySqlBind< CDateTime >
			: public COutMySqlBindBase
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
		COutMySqlBind( MYSQL_BIND & bind, enum_field_types type, CDatabaseValue< EFieldType_DATETIME > & value, CDatabaseStatementParameterMySql & parameter )
			: COutMySqlBindBase( bind, type, parameter )
			, _value( value )
		{
			_bind.buffer_length = sizeof( _holder );
			bind.buffer = &_holder;
		}

		//!@copydoc COutMySqlBindBase::DoSetValue
		virtual void UpdateValue()
		{
			_holder = MySqlTimeFromCDateTime( _value.GetValue() );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_DATETIME > & _value;
		//! The value holder
		MYSQL_TIME _holder;
	};

	/** Specialization for CTime
	*/
	template<>
	struct COutMySqlBind< CTime >
			: public COutMySqlBindBase
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
		COutMySqlBind( MYSQL_BIND & bind, enum_field_types type, CDatabaseValue< EFieldType_TIME > & value, CDatabaseStatementParameterMySql & parameter )
			: COutMySqlBindBase( bind, type, parameter )
			, _value( value )
		{
			_bind.buffer_length = sizeof( _holder );
			bind.buffer = &_holder;
		}

		//!@copydoc COutMySqlBindBase::DoSetValue
		virtual void UpdateValue()
		{
			_holder = MySqlTimeFromCTime( _value.GetValue() );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_TIME > & _value;
		//! The value holder
		MYSQL_TIME _holder;
	};

	/** Function used to create a COutMySqlBind easily
	@param bind
		The binding
	@param value
		The parameter value
	@param parameter
		The parameter
	*/
	template< EFieldType Type > std::unique_ptr< COutMySqlBindBase > MakeOutBind( MYSQL_BIND & bind, CDatabaseValueBase & value, CDatabaseStatementParameterMySql & parameter )
	{
		return std::make_unique< COutMySqlBind< SFieldTypeMySqlDataTyper< Type >::DataType > >( bind, SFieldTypeMySqlDataTyper< Type >::MySqlFieldType, static_cast< CDatabaseValue< Type > & >( value ), parameter );
	}
}
END_NAMESPACE_DATABASE_MYSQL

#endif // ___DATABASE_STATEMENTPARAMETER_SETTER_H___
