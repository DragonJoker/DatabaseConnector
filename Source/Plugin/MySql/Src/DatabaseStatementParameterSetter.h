/************************************************************************//**
* @file DatabaseStatementParameterSetter.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 11:56:56 AM
*
*
* @brief SParameterValueSetterBase and SParameterValueSetter classes.
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

	/** Base setter class
	*/
	struct SParameterValueSetterBase
	{
		/** Constructor
		*/
		SParameterValueSetterBase( MYSQL_BIND * bind, enum_field_types type )
			: _bind( bind )
		{
			_bind->length = &_length;
			_bind->error = &_error;
			_bind->is_null = &_null;
			_bind->buffer_type = type;
		}
		/** Destructor
		*/
		virtual ~SParameterValueSetterBase()
		{
		}
		/** Setter function
		@param statement
		    The statement
		@param value
		    The value pointer
		@param parameter
		    The parameter
		*/
		void operator()( MYSQL_STMT * statement, void * value, CDatabaseStatementParameterMySql * parameter )
		{
			SetValue( statement, value, parameter );
		}

		/** Setter function, to implement in child classes
		@param statement
		    The statement
		@param value
		    The value pointer
		@param parameter
		    The parameter
		*/
		virtual void SetValue( MYSQL_STMT * statement, void * value, CDatabaseStatementParameterMySql * parameter ) = 0;

		//! Tells if the parameter is null
		my_bool _null = 0;
		//! The parameter length
		unsigned long _length = 0;
		//! Tells if there is an error
		my_bool _error = 0;
		//! The data binding
		MYSQL_BIND * _bind;
	};
	/** Generic template class to set the parameter value
	*/
	template< EFieldType Type >
	struct SParameterValueSetter
			: public SParameterValueSetterBase
	{
		static const EFieldType _type = Type;
		/** Constructor
		*/
		SParameterValueSetter( MYSQL_BIND * bind )
			: SParameterValueSetterBase( bind, SFieldTypeMySqlDataTyper< _type >::Value )
		{
		}

		//!@copydoc SParameterValueSetterBase::SetValue
		virtual void SetValue( MYSQL_STMT * statement, void * value, CDatabaseStatementParameterMySql * parameter )
		{
			_bind->buffer = value;
		}
	};
	/** Specialization for EFieldType_VARCHAR
	*/
	template<>
	struct SParameterValueSetter< EFieldType_VARCHAR >
			: public SParameterValueSetterBase
	{
		/** Constructor
		*/
		SParameterValueSetter( MYSQL_BIND * bind )
			: SParameterValueSetterBase( bind, SFieldTypeMySqlDataTyper< _type >::Value )
		{
		}

		//!@copydoc SParameterValueSetterBase::SetValue
		virtual void SetValue( MYSQL_STMT * statement, void * value, CDatabaseStatementParameterMySql * parameter )
		{
			_length = parameter->GetPtrSize();
			_bind->buffer = value;
			_bind->buffer_length = _length;
		}

		static const EFieldType _type = EFieldType_VARCHAR;
	};
	/** Specialization for EFieldType_TEXT
	*/
	template<>
	struct SParameterValueSetter< EFieldType_TEXT >
			: public SParameterValueSetterBase
	{
		/** Constructor
		*/
		SParameterValueSetter( MYSQL_BIND * bind )
			: SParameterValueSetterBase( bind, SFieldTypeMySqlDataTyper< _type >::Value )
		{
		}

		//!@copydoc SParameterValueSetterBase::SetValue
		virtual void SetValue( MYSQL_STMT * statement, void * value, CDatabaseStatementParameterMySql * parameter )
		{
			_length = parameter->GetPtrSize();
			_bind->buffer = value;
			_bind->buffer_length = _length;
		}

		static const EFieldType _type = EFieldType_TEXT;
	};
	/** Specialization for EFieldType_NVARCHAR
	*/
	template<>
	struct SParameterValueSetter< EFieldType_NVARCHAR >
			: public SParameterValueSetterBase
	{
		/** Constructor
		*/
		SParameterValueSetter( MYSQL_BIND * bind )
			: SParameterValueSetterBase( bind, SFieldTypeMySqlDataTyper< _type >::Value )
		{
		}

		//!@copydoc SParameterValueSetterBase::SetValue
		virtual void SetValue( MYSQL_STMT * statement, void * value, CDatabaseStatementParameterMySql * parameter )
		{
			_value = CStrUtils::ToStr( static_cast< wchar_t * >( value ) );
			_length = _value.size();
			_bind->buffer = &_value[0];
			_bind->buffer_length = _length;
		}

		static const EFieldType _type = EFieldType_NVARCHAR;
		SFieldTypeMySqlDataTyper< _type >::Type _value;
	};
	/** Specialization for EFieldType_NTEXT
	*/
	template<>
	struct SParameterValueSetter< EFieldType_NTEXT >
			: public SParameterValueSetterBase
	{
		/** Constructor
		*/
		SParameterValueSetter( MYSQL_BIND * bind )
			: SParameterValueSetterBase( bind, SFieldTypeMySqlDataTyper< _type >::Value )
		{
		}

		//!@copydoc SParameterValueSetterBase::SetValue
		virtual void SetValue( MYSQL_STMT * statement, void * value, CDatabaseStatementParameterMySql * parameter )
		{
			_value = CStrUtils::ToStr( static_cast< wchar_t * >( value ) );
			_length = _value.size();
			_bind->buffer = &_value[0];
			_bind->buffer_length = _length;
		}

		static const EFieldType _type = EFieldType_NTEXT;
		SFieldTypeMySqlDataTyper< _type >::Type _value;
	};
	/** Specialization for EFieldType_DATE
	*/
	template<>
	struct SParameterValueSetter< EFieldType_DATE >
			: public SParameterValueSetterBase
	{
		/** Constructor
		*/
		SParameterValueSetter( MYSQL_BIND * bind )
			: SParameterValueSetterBase( bind, SFieldTypeMySqlDataTyper< _type >::Value )
		{
		}

		//!@copydoc SParameterValueSetterBase::SetValue
		virtual void SetValue( MYSQL_STMT * statement, void * value, CDatabaseStatementParameterMySql * parameter )
		{
			_bind->buffer = &_value;
		}

		static const EFieldType _type = EFieldType_DATE;
		SFieldTypeMySqlDataTyper< _type >::Type _value;
	};
	/** Specialization for EFieldType_DATETIME
	*/
	template<>
	struct SParameterValueSetter< EFieldType_DATETIME >
			: public SParameterValueSetterBase
	{
		/** Constructor
		*/
		SParameterValueSetter( MYSQL_BIND * bind )
			: SParameterValueSetterBase( bind, SFieldTypeMySqlDataTyper< _type >::Value )
		{
		}

		//!@copydoc SParameterValueSetterBase::SetValue
		virtual void SetValue( MYSQL_STMT * statement, void * value, CDatabaseStatementParameterMySql * parameter )
		{
			_bind->buffer = &_value;
		}

		static const EFieldType _type = EFieldType_DATETIME;
		SFieldTypeMySqlDataTyper< _type >::Type _value;
	};
	/** Specialization for EFieldType_TIME
	*/
	template<>
	struct SParameterValueSetter< EFieldType_TIME >
			: public SParameterValueSetterBase
	{
		/** Constructor
		*/
		SParameterValueSetter( MYSQL_BIND * bind )
			: SParameterValueSetterBase( bind, SFieldTypeMySqlDataTyper< _type >::Value )
		{
		}

		//!@copydoc SParameterValueSetterBase::SetValue
		virtual void SetValue( MYSQL_STMT * statement, void * value, CDatabaseStatementParameterMySql * parameter )
		{
			_bind->buffer = &_value;
		}

		static const EFieldType _type = EFieldType_TIME;
		SFieldTypeMySqlDataTyper< _type >::Type _value;
	};
	/** Specialization for EFieldType_BINARY
	*/
	template<>
	struct SParameterValueSetter< EFieldType_BINARY >
			: public SParameterValueSetterBase
	{
		/** Constructor
		*/
		SParameterValueSetter( MYSQL_BIND * bind )
			: SParameterValueSetterBase( bind, SFieldTypeMySqlDataTyper< _type >::Value )
		{
		}

		//!@copydoc SParameterValueSetterBase::SetValue
		virtual void SetValue( MYSQL_STMT * statement, void * value, CDatabaseStatementParameterMySql * parameter )
		{
			_length = parameter->GetPtrSize();
			_bind->buffer = value;
			_bind->buffer_length = _length;
		}

		static const EFieldType _type = EFieldType_BINARY;
	};
	/** Specialization for EFieldType_VARBINARY
	*/
	template<>
	struct SParameterValueSetter< EFieldType_VARBINARY >
			: public SParameterValueSetterBase
	{
		/** Constructor
		*/
		SParameterValueSetter( MYSQL_BIND * bind )
			: SParameterValueSetterBase( bind, SFieldTypeMySqlDataTyper< _type >::Value )
		{
		}

		//!@copydoc SParameterValueSetterBase::SetValue
		virtual void SetValue( MYSQL_STMT * statement, void * value, CDatabaseStatementParameterMySql * parameter )
		{
			_length = parameter->GetPtrSize();
			_bind->buffer = value;
			_bind->buffer_length = _length;
		}

		static const EFieldType _type = EFieldType_VARBINARY;
	};
	/** Specialization for EFieldType_LONG_VARBINARY
	*/
	template<>
	struct SParameterValueSetter< EFieldType_LONG_VARBINARY >
			: public SParameterValueSetterBase
	{
		/** Constructor
		*/
		SParameterValueSetter( MYSQL_BIND * bind )
			: SParameterValueSetterBase( bind, SFieldTypeMySqlDataTyper< _type >::Value )
		{
		}

		//!@copydoc SParameterValueSetterBase::SetValue
		virtual void SetValue( MYSQL_STMT * statement, void * value, CDatabaseStatementParameterMySql * parameter )
		{
			_length = parameter->GetPtrSize();
			_bind->buffer = value;
			_bind->buffer_length = _length;
		}

		static const EFieldType _type = EFieldType_LONG_VARBINARY;
	};
}
END_NAMESPACE_DATABASE_MYSQL

#endif // ___DATABASE_STATEMENTPARAMETER_SETTER_H___
