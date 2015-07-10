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
		static const enum_field_types Value = MYSQL_TYPE_TINY;
		typedef bool Type;
	};

	/** Specialization for EFieldType_SMALL_INTEGER
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_SMALL_INTEGER >
	{
		static const enum_field_types Value = MYSQL_TYPE_SHORT;
		typedef int16_t Type;
	};

	/** Specialization for EFieldType_INTEGER
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_INTEGER >
	{
		static const enum_field_types Value = MYSQL_TYPE_LONG;
		typedef int32_t Type;
	};

	/** Specialization for EFieldType_LONG_INTEGER
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_LONG_INTEGER >
	{
		static const enum_field_types Value = MYSQL_TYPE_LONGLONG;
		typedef int64_t Type;
	};

	/** Specialization for EFieldType_FLOAT
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_FLOAT >
	{
		static const enum_field_types Value = MYSQL_TYPE_FLOAT;
		typedef float Type;
	};

	/** Specialization for EFieldType_DOUBLE
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_DOUBLE >
	{
		static const enum_field_types Value = MYSQL_TYPE_DOUBLE;
		typedef double Type;
	};

	/** Specialization for EFieldType_DATE
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_DATE >
	{
		static const enum_field_types Value = MYSQL_TYPE_DATE;
		typedef MYSQL_TIME Type;
	};

	/** Specialization for EFieldType_DATETIME
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_DATETIME >
	{
		static const enum_field_types Value = MYSQL_TYPE_DATETIME;
		typedef MYSQL_TIME Type;
	};

	/** Specialization for EFieldType_TIME
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_TIME >
	{
		static const enum_field_types Value = MYSQL_TYPE_TIME;
		typedef MYSQL_TIME Type;
	};

	/** Specialization for EFieldType_VARCHAR
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_VARCHAR >
	{
		static const enum_field_types Value = MYSQL_TYPE_STRING;
		typedef std::string Type;
	};

	/** Specialization for EFieldType_TEXT
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_TEXT >
	{
		static const enum_field_types Value = MYSQL_TYPE_STRING;
		typedef std::string Type;
	};

	/** Specialization for EFieldType_NVARCHAR
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_NVARCHAR >
	{
		static const enum_field_types Value = MYSQL_TYPE_STRING;
		typedef std::string Type;
	};

	/** Specialization for EFieldType_NTEXT
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_NTEXT >
	{
		static const enum_field_types Value = MYSQL_TYPE_STRING;
		typedef std::string Type;
	};

	/** Specialization for EFieldType_BINARY
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_BINARY >
	{
		static const enum_field_types Value = MYSQL_TYPE_BLOB;
		typedef std::vector< uint8_t > Type;
	};

	/** Specialization for EFieldType_VARBINARY
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_VARBINARY >
	{
		static const enum_field_types Value = MYSQL_TYPE_BLOB;
		typedef std::vector< uint8_t > Type;
	};

	/** Specialization for EFieldType_LONG_VARBINARY
	*/
	template<> struct SFieldTypeMySqlDataTyper< EFieldType_LONG_VARBINARY >
	{
		static const enum_field_types Value = MYSQL_TYPE_BLOB;
		typedef std::vector< uint8_t > Type;
	};

	template< typename T >
	struct COutMySqlBind
		: public COutMySqlBindBase
	{
		COutMySqlBind( MYSQL_BIND & bind, enum_field_types type, const T * value )
			: COutMySqlBindBase( bind, type )
		{
			bind.buffer = const_cast< T * >( value );
		}

		void SetValue( T const & value )
		{
			// Empty
		}
	};

	template< typename T >
	struct COutMySqlBind< T * >
		: public COutMySqlBindBase
	{
		COutMySqlBind( MYSQL_BIND & bind, enum_field_types type, const T * value )
			: COutMySqlBindBase( bind, type )
		{
			bind.buffer = const_cast< T * >( value );
		}

		void SetValue( T const * value )
		{
			// Empty
		}
	};

	template<>
	struct COutMySqlBind< bool >
		: public COutMySqlBindBase
	{
		COutMySqlBind( MYSQL_BIND & bind, enum_field_types type, const bool * value )
			: COutMySqlBindBase( bind, type )
		{
			bind.buffer = &_value;
		}
			
		void SetValue( bool const & value )
		{
			_value = ( value ) ? 1 : 0;
		}

		int8_t _value;
	};

	template<>
	struct COutMySqlBind< CDate >
		: public COutMySqlBindBase
	{
		COutMySqlBind( MYSQL_BIND & bind, enum_field_types type, const CDate * value )
			: COutMySqlBindBase( bind, type )
		{
			bind.buffer = &_value;
		}
			
		void SetValue( CDate const & value )
		{
			_value = MySqlTimeFromCDate( value );
		}

		MYSQL_TIME _value;
	};

	template<>
	struct COutMySqlBind< CDateTime >
		: public COutMySqlBindBase
	{
		COutMySqlBind( MYSQL_BIND & bind, enum_field_types type, const CDateTime * value )
			: COutMySqlBindBase( bind, type )
		{
			bind.buffer = &_value;
		}
			
		void SetValue( CDateTime const & value )
		{
			_value = MySqlTimeFromCDateTime( value );
		}

		MYSQL_TIME _value;
	};

	template<>
	struct COutMySqlBind< CTime >
		: public COutMySqlBindBase
	{
		COutMySqlBind( MYSQL_BIND & bind, enum_field_types type, const CTime * value )
			: COutMySqlBindBase( bind, type )
		{
			bind.buffer = &_value;
		}
			
		void SetValue( CTime const & value )
		{
			_value = MySqlTimeFromCTime( value );
		}

		MYSQL_TIME _value;
	};

	/** Base setter class
	*/
	struct SMySqlParameterValueSetterBase
	{
		/** Setter function
		@param statement
		    The statement
		@param value
		    The value pointer
		@param parameter
		    The parameter
		*/
		void operator()( MYSQL_STMT * statement, MYSQL_BIND * bind, const void * value, CDatabaseStatementParameterMySql * parameter )
		{
			bind->buffer_length = parameter->GetObjectValue().GetPtrSize();
			SetValue( statement, bind, value, parameter );
		}

		/** Setter function, to implement in child classes
		@param statement
		    The statement
		@param value
		    The value pointer
		@param parameter
		    The parameter
		*/
		virtual void SetValue( MYSQL_STMT * statement, MYSQL_BIND * bind, const void * value, CDatabaseStatementParameterMySql * parameter ) = 0;
	};
	/** Generic template class to set the parameter value
	*/
	template< EFieldType Type >
	struct SMySqlParameterValueSetter
			: public SMySqlParameterValueSetterBase
	{
		static const EFieldType _type = Type;
		//!@copydoc SMySqlParameterValueSetterBase::SetValue
		virtual void SetValue( MYSQL_STMT * statement, MYSQL_BIND * bind, const void * value, CDatabaseStatementParameterMySql * parameter )
		{
		}
	};
	/** Specialization for EFieldType_VARCHAR
	*/
	template<>
	struct SMySqlParameterValueSetter< EFieldType_VARCHAR >
			: public SMySqlParameterValueSetterBase
	{
		//!@copydoc SMySqlParameterValueSetterBase::SetValue
		virtual void SetValue( MYSQL_STMT * statement, MYSQL_BIND * bind, const void * value, CDatabaseStatementParameterMySql * parameter )
		{
		}

		static const EFieldType _type = EFieldType_VARCHAR;
	};
	/** Specialization for EFieldType_TEXT
	*/
	template<>
	struct SMySqlParameterValueSetter< EFieldType_TEXT >
			: public SMySqlParameterValueSetterBase
	{
		//!@copydoc SMySqlParameterValueSetterBase::SetValue
		virtual void SetValue( MYSQL_STMT * statement, MYSQL_BIND * bind, const void * value, CDatabaseStatementParameterMySql * parameter )
		{
		}

		static const EFieldType _type = EFieldType_TEXT;
	};
	/** Specialization for EFieldType_NVARCHAR
	*/
	template<>
	struct SMySqlParameterValueSetter< EFieldType_NVARCHAR >
			: public SMySqlParameterValueSetterBase
	{
		//!@copydoc SMySqlParameterValueSetterBase::SetValue
		virtual void SetValue( MYSQL_STMT * statement, MYSQL_BIND * bind, const void * value, CDatabaseStatementParameterMySql * parameter )
		{
			_value = CStrUtils::ToStr( static_cast< const wchar_t * >( value ) );
			bind->buffer = &_value[0];
		}

		static const EFieldType _type = EFieldType_NVARCHAR;
		SFieldTypeMySqlDataTyper< _type >::Type _value;
	};
	/** Specialization for EFieldType_NTEXT
	*/
	template<>
	struct SMySqlParameterValueSetter< EFieldType_NTEXT >
			: public SMySqlParameterValueSetterBase
	{
		//!@copydoc SMySqlParameterValueSetterBase::SetValue
		virtual void SetValue( MYSQL_STMT * statement, MYSQL_BIND * bind, const void * value, CDatabaseStatementParameterMySql * parameter )
		{
			_value = CStrUtils::ToStr( static_cast< const wchar_t * >( value ) );
			bind->buffer = &_value[0];
		}

		static const EFieldType _type = EFieldType_NTEXT;
		SFieldTypeMySqlDataTyper< _type >::Type _value;
	};
	/** Specialization for EFieldType_DATE
	*/
	template<>
	struct SMySqlParameterValueSetter< EFieldType_DATE >
			: public SMySqlParameterValueSetterBase
	{
		//!@copydoc SMySqlParameterValueSetterBase::SetValue
		virtual void SetValue( MYSQL_STMT * statement, MYSQL_BIND * bind, const void * value, CDatabaseStatementParameterMySql * parameter )
		{
		}

		static const EFieldType _type = EFieldType_DATE;
		SFieldTypeMySqlDataTyper< _type >::Type _value;
	};
	/** Specialization for EFieldType_DATETIME
	*/
	template<>
	struct SMySqlParameterValueSetter< EFieldType_DATETIME >
			: public SMySqlParameterValueSetterBase
	{
		//!@copydoc SMySqlParameterValueSetterBase::SetValue
		virtual void SetValue( MYSQL_STMT * statement, MYSQL_BIND * bind, const void * value, CDatabaseStatementParameterMySql * parameter )
		{
		}

		static const EFieldType _type = EFieldType_DATETIME;
		SFieldTypeMySqlDataTyper< _type >::Type _value;
	};
	/** Specialization for EFieldType_TIME
	*/
	template<>
	struct SMySqlParameterValueSetter< EFieldType_TIME >
			: public SMySqlParameterValueSetterBase
	{
		//!@copydoc SMySqlParameterValueSetterBase::SetValue
		virtual void SetValue( MYSQL_STMT * statement, MYSQL_BIND * bind, const void * value, CDatabaseStatementParameterMySql * parameter )
		{
		}

		static const EFieldType _type = EFieldType_TIME;
		SFieldTypeMySqlDataTyper< _type >::Type _value;
	};
	/** Specialization for EFieldType_BINARY
	*/
	template<>
	struct SMySqlParameterValueSetter< EFieldType_BINARY >
			: public SMySqlParameterValueSetterBase
	{
		//!@copydoc SMySqlParameterValueSetterBase::SetValue
		virtual void SetValue( MYSQL_STMT * statement, MYSQL_BIND * bind, const void * value, CDatabaseStatementParameterMySql * parameter )
		{
		}

		static const EFieldType _type = EFieldType_BINARY;
	};
	/** Specialization for EFieldType_VARBINARY
	*/
	template<>
	struct SMySqlParameterValueSetter< EFieldType_VARBINARY >
			: public SMySqlParameterValueSetterBase
	{
		//!@copydoc SMySqlParameterValueSetterBase::SetValue
		virtual void SetValue( MYSQL_STMT * statement, MYSQL_BIND * bind, const void * value, CDatabaseStatementParameterMySql * parameter )
		{
		}

		static const EFieldType _type = EFieldType_VARBINARY;
	};
	/** Specialization for EFieldType_LONG_VARBINARY
	*/
	template<>
	struct SMySqlParameterValueSetter< EFieldType_LONG_VARBINARY >
			: public SMySqlParameterValueSetterBase
	{
		//!@copydoc SMySqlParameterValueSetterBase::SetValue
		virtual void SetValue( MYSQL_STMT * statement, MYSQL_BIND * bind, const void * value, CDatabaseStatementParameterMySql * parameter )
		{
		}

		static const EFieldType _type = EFieldType_LONG_VARBINARY;
	};
}
END_NAMESPACE_DATABASE_MYSQL

#endif // ___DATABASE_STATEMENTPARAMETER_SETTER_H___
