/************************************************************************//**
* @file DatabaseStatementParameterSetter.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 11:56:56 AM
*
*
* @brief SSqliteBindingBase and SSqliteBinding classes.
*
* @details Functors used to set the parameter value in a statement.
*
***************************************************************************/

#ifndef ___DATABASE_STATEMENTPARAMETER_SETTER_H___
#define ___DATABASE_STATEMENTPARAMETER_SETTER_H___

#include "DatabaseSqlitePrerequisites.h"

#include "DatabaseStatementParameterSqlite.h"
#include "DatabaseSqliteHelper.h"

#include <DatabaseStringUtils.h>
#include <DatabaseLogger.h>

BEGIN_NAMESPACE_DATABASE_SQLITE
{
	static const String ERROR_SQLITE_PARAMETER_VALUE = STR( "Can't set parameter value" );
	static const String ERROR_SQLITE_UPDATE_UNIMPLEMENTED = STR( "UpdateValue not implemented for this data type" );

	static const String INFO_SQLITE_SET_PARAMETER_VALUE = STR( "Set parameter value: " );
	static const String INFO_SQLITE_SET_PARAMETER_NULL = STR( "Set parameter NULL" );

	/** Generic template class to set the parameter value
	*/
	template< EFieldType Type >
	struct SSqliteBinding
		: public SSqliteBindingBase
	{
		/** Constructor
		@param statement
			The statement
		@param connection
			The database connection
		@param index
			The parameter index
		@param value
			The parameter value
		*/
		SSqliteBinding( sqlite3_stmt * statement, sqlite3 * connection, uint16_t index, CDatabaseValue< Type > const & value )
			: SSqliteBindingBase( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::UpdateValue
		virtual void UpdateValue()
		{
			DB_EXCEPT( EDatabaseExceptionCodes_Unimplemented, ERROR_SQLITE_UPDATE_UNIMPLEMENTED );
		}

		//! The parameter value
		CDatabaseValue< Type > _value;
	};

	/** SSqliteBinding specialization for EFieldType_BIT
	*/
	template<>
	struct SSqliteBinding< EFieldType_BIT >
		: public SSqliteBindingBase
	{
		/** Constructor
		@param statement
			The statement
		@param connection
			The database connection
		@param index
			The parameter index
		@param value
			The parameter value
		*/
		SSqliteBinding( sqlite3_stmt * statement, sqlite3 * connection, uint16_t index, CDatabaseValue< EFieldType_BIT > const & value )
			: SSqliteBindingBase( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::UpdateValue
		virtual void UpdateValue()
		{
			if ( _value.IsNull() )
			{
				SQLiteCheck( sqlite3_bind_null( _statement, _index ), StringStream() << INFO_SQLITE_SET_PARAMETER_NULL, EDatabaseExceptionCodes_StatementError, _connection );
			}
			else
			{
				SQLiteCheck( sqlite3_bind_int( _statement, _index, _value.GetValue() ? 1 : 0 ), StringStream() << INFO_SQLITE_SET_PARAMETER_VALUE << _value.GetValue(), EDatabaseExceptionCodes_StatementError, _connection );
			}
		}

		//! The parameter value
		CDatabaseValue< EFieldType_BIT > const & _value;
	};

	/** base class for all integer type bindings
	*/
	struct SSqliteIntegerBinding
		: public SSqliteBindingBase
	{
		/** Constructor
		@param statement
			The statement
		@param connection
			The database connection
		@param index
			The parameter index
		@param value
			The parameter value
		*/
		SSqliteIntegerBinding( sqlite3_stmt * statement, sqlite3 * connection, uint16_t index )
			: SSqliteBindingBase( statement, connection, index )
		{
		}

		/** Updates the binding value
		@param[in] null
			Tells if the parameter is null
		@param[in] value
			The value
		*/
		void UpdateValue( bool null, int value )
		{
			if ( null )
			{
				SQLiteCheck( sqlite3_bind_null( _statement, _index ), StringStream() << INFO_SQLITE_SET_PARAMETER_NULL, EDatabaseExceptionCodes_StatementError, _connection );
			}
			else
			{
				SQLiteCheck( sqlite3_bind_int( _statement, _index, value ), StringStream() << INFO_SQLITE_SET_PARAMETER_VALUE << value, EDatabaseExceptionCodes_StatementError, _connection );
			}
		}
	};

	/** SSqliteBinding specialization for EFieldType_SINT8
	*/
	template<>
	struct SSqliteBinding< EFieldType_SINT8 >
		: public SSqliteIntegerBinding
	{
		/** Constructor
		@param statement
			The statement
		@param connection
			The database connection
		@param index
			The parameter index
		@param value
			The parameter value
		*/
		SSqliteBinding( sqlite3_stmt * statement, sqlite3 * connection, uint16_t index, CDatabaseValue< EFieldType_SINT8 > const & value )
			: SSqliteIntegerBinding( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::UpdateValue
		virtual void UpdateValue()
		{
			SSqliteIntegerBinding::UpdateValue( _value.IsNull(), int( _value.GetValue() ) );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_SINT8 > const & _value;
	};

	/** SSqliteBinding specialization for EFieldType_UINT8
	*/
	template<>
	struct SSqliteBinding< EFieldType_UINT8 >
		: public SSqliteIntegerBinding
	{
		/** Constructor
		@param statement
			The statement
		@param connection
			The database connection
		@param index
			The parameter index
		@param value
			The parameter value
		*/
		SSqliteBinding( sqlite3_stmt * statement, sqlite3 * connection, uint16_t index, CDatabaseValue< EFieldType_UINT8 > const & value )
			: SSqliteIntegerBinding( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::UpdateValue
		virtual void UpdateValue()
		{
			SSqliteIntegerBinding::UpdateValue( _value.IsNull(), int( _value.GetValue() ) );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_UINT8 > const & _value;
	};

	/** SSqliteBinding specialization for EFieldType_SINT16
	*/
	template<>
	struct SSqliteBinding< EFieldType_SINT16 >
		: public SSqliteIntegerBinding
	{
		/** Constructor
		@param statement
			The statement
		@param connection
			The database connection
		@param index
			The parameter index
		@param value
			The parameter value
		*/
		SSqliteBinding( sqlite3_stmt * statement, sqlite3 * connection, uint16_t index, CDatabaseValue< EFieldType_SINT16 > const & value )
			: SSqliteIntegerBinding( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::UpdateValue
		virtual void UpdateValue()
		{
			SSqliteIntegerBinding::UpdateValue( _value.IsNull(), int( _value.GetValue() ) );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_SINT16 > const & _value;
	};

	/** SSqliteBinding specialization for EFieldType_UINT16
	*/
	template<>
	struct SSqliteBinding< EFieldType_UINT16 >
		: public SSqliteIntegerBinding
	{
		/** Constructor
		@param statement
			The statement
		@param connection
			The database connection
		@param index
			The parameter index
		@param value
			The parameter value
		*/
		SSqliteBinding( sqlite3_stmt * statement, sqlite3 * connection, uint16_t index, CDatabaseValue< EFieldType_UINT16 > const & value )
			: SSqliteIntegerBinding( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::UpdateValue
		virtual void UpdateValue()
		{
			SSqliteIntegerBinding::UpdateValue( _value.IsNull(), int( _value.GetValue() ) );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_UINT16 > const & _value;
	};

	/** SSqliteBinding specialization for EFieldType_SINT24
	*/
	template<>
	struct SSqliteBinding< EFieldType_SINT24 >
		: public SSqliteIntegerBinding
	{
		/** Constructor
		@param statement
			The statement
		@param connection
			The database connection
		@param index
			The parameter index
		@param value
			The parameter value
		*/
		SSqliteBinding( sqlite3_stmt * statement, sqlite3 * connection, uint16_t index, CDatabaseValue< EFieldType_SINT24 > const & value )
			: SSqliteIntegerBinding( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::UpdateValue
		virtual void UpdateValue()
		{
			SSqliteIntegerBinding::UpdateValue( _value.IsNull(), int32_t( _value.GetValue() ) );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_SINT24 > const & _value;
	};

	/** SSqliteBinding specialization for EFieldType_UINT24
	*/
	template<>
	struct SSqliteBinding< EFieldType_UINT24 >
		: public SSqliteIntegerBinding
	{
		/** Constructor
		@param statement
			The statement
		@param connection
			The database connection
		@param index
			The parameter index
		@param value
			The parameter value
		*/
		SSqliteBinding( sqlite3_stmt * statement, sqlite3 * connection, uint16_t index, CDatabaseValue< EFieldType_UINT24 > const & value )
			: SSqliteIntegerBinding( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::UpdateValue
		virtual void UpdateValue()
		{
			SSqliteIntegerBinding::UpdateValue( _value.IsNull(), int32_t( _value.GetValue() ) );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_UINT24 > const & _value;
	};

	/** SSqliteBinding specialization for EFieldType_SINT32
	*/
	template<>
	struct SSqliteBinding< EFieldType_SINT32 >
		: public SSqliteIntegerBinding
	{
		/** Constructor
		@param statement
			The statement
		@param connection
			The database connection
		@param index
			The parameter index
		@param value
			The parameter value
		*/
		SSqliteBinding( sqlite3_stmt * statement, sqlite3 * connection, uint16_t index, CDatabaseValue< EFieldType_SINT32 > const & value )
			: SSqliteIntegerBinding( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::UpdateValue
		virtual void UpdateValue()
		{
			SSqliteIntegerBinding::UpdateValue( _value.IsNull(), int( _value.GetValue() ) );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_SINT32 > const & _value;
	};

	/** SSqliteBinding specialization for EFieldType_UINT32
	*/
	template<>
	struct SSqliteBinding< EFieldType_UINT32 >
		: public SSqliteIntegerBinding
	{
		/** Constructor
		@param statement
			The statement
		@param connection
			The database connection
		@param index
			The parameter index
		@param value
			The parameter value
		*/
		SSqliteBinding( sqlite3_stmt * statement, sqlite3 * connection, uint16_t index, CDatabaseValue< EFieldType_UINT32 > const & value )
			: SSqliteIntegerBinding( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::UpdateValue
		virtual void UpdateValue()
		{
			SSqliteIntegerBinding::UpdateValue( _value.IsNull(), int( _value.GetValue() ) );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_UINT32 > const & _value;
	};

	/** SSqliteBinding specialization for EFieldType_INT64
	*/
	template<>
	struct SSqliteBinding< EFieldType_SINT64 >
		: public SSqliteBindingBase
	{
		/** Constructor
		@param statement
			The statement
		@param connection
			The database connection
		@param index
			The parameter index
		@param value
			The parameter value
		*/
		SSqliteBinding( sqlite3_stmt * statement, sqlite3 * connection, uint16_t index, CDatabaseValue< EFieldType_SINT64 > const & value )
			: SSqliteBindingBase( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::UpdateValue
		virtual void UpdateValue()
		{
			if ( _value.IsNull() )
			{
				SQLiteCheck( sqlite3_bind_null( _statement, _index ), StringStream() << INFO_SQLITE_SET_PARAMETER_NULL, EDatabaseExceptionCodes_StatementError, _connection );
			}
			else
			{
				SQLiteCheck( sqlite3_bind_int64( _statement, _index, _value.GetValue() ), StringStream() << INFO_SQLITE_SET_PARAMETER_VALUE << _value.GetValue(), EDatabaseExceptionCodes_StatementError, _connection );
			}
		}

		//! The parameter value
		CDatabaseValue< EFieldType_SINT64 > const & _value;
	};

	/** SSqliteBinding specialization for EFieldType_UINT64
	*/
	template<>
	struct SSqliteBinding< EFieldType_UINT64 >
		: public SSqliteBindingBase
	{
		/** Constructor
		@param statement
			The statement
		@param connection
			The database connection
		@param index
			The parameter index
		@param value
			The parameter value
		*/
		SSqliteBinding( sqlite3_stmt * statement, sqlite3 * connection, uint16_t index, CDatabaseValue< EFieldType_UINT64 > const & value )
			: SSqliteBindingBase( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::UpdateValue
		virtual void UpdateValue()
		{
			if ( _value.IsNull() )
			{
				SQLiteCheck( sqlite3_bind_null( _statement, _index ), StringStream() << INFO_SQLITE_SET_PARAMETER_NULL, EDatabaseExceptionCodes_StatementError, _connection );
			}
			else
			{
				SQLiteCheck( sqlite3_bind_int64( _statement, _index, _value.GetValue() ), StringStream() << INFO_SQLITE_SET_PARAMETER_VALUE << _value.GetValue(), EDatabaseExceptionCodes_StatementError, _connection );
			}
		}

		//! The parameter value
		CDatabaseValue< EFieldType_UINT64 > const & _value;
	};

	/** SSqliteBinding specialization for EFieldType_FLOAT32
	*/
	template<>
	struct SSqliteBinding< EFieldType_FLOAT32 >
		: public SSqliteBindingBase
	{
		/** Constructor
		@param statement
			The statement
		@param connection
			The database connection
		@param index
			The parameter index
		@param value
			The parameter value
		*/
		SSqliteBinding( sqlite3_stmt * statement, sqlite3 * connection, uint16_t index, CDatabaseValue< EFieldType_FLOAT32 > const & value )
			: SSqliteBindingBase( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::UpdateValue
		virtual void UpdateValue()
		{
			if ( _value.IsNull() )
			{
				SQLiteCheck( sqlite3_bind_null( _statement, _index ), StringStream() << INFO_SQLITE_SET_PARAMETER_NULL, EDatabaseExceptionCodes_StatementError, _connection );
			}
			else
			{
				SQLiteCheck( sqlite3_bind_double( _statement, _index, _value.GetValue() ), StringStream() << INFO_SQLITE_SET_PARAMETER_VALUE << _value.GetValue(), EDatabaseExceptionCodes_StatementError, _connection );
			}
		}

		//! The parameter value
		CDatabaseValue< EFieldType_FLOAT32 > const & _value;
	};

	/** SSqliteBinding specialization for EFieldType_FLOAT64
	*/
	template<>
	struct SSqliteBinding< EFieldType_FLOAT64 >
		: public SSqliteBindingBase
	{
		/** Constructor
		@param statement
			The statement
		@param connection
			The database connection
		@param index
			The parameter index
		@param value
			The parameter value
		*/
		SSqliteBinding( sqlite3_stmt * statement, sqlite3 * connection, uint16_t index, CDatabaseValue< EFieldType_FLOAT64 > const & value )
			: SSqliteBindingBase( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::UpdateValue
		virtual void UpdateValue()
		{
			if ( _value.IsNull() )
			{
				SQLiteCheck( sqlite3_bind_null( _statement, _index ), StringStream() << INFO_SQLITE_SET_PARAMETER_NULL, EDatabaseExceptionCodes_StatementError, _connection );
			}
			else
			{
				SQLiteCheck( sqlite3_bind_double( _statement, _index, _value.GetValue() ), StringStream() << INFO_SQLITE_SET_PARAMETER_VALUE << _value.GetValue(), EDatabaseExceptionCodes_StatementError, _connection );
			}
		}

		//! The parameter value
		CDatabaseValue< EFieldType_FLOAT64 > const & _value;
	};

	/** SSqliteBinding specialization for EFieldType_FIXED_POINT
	*/
	template<>
	struct SSqliteBinding< EFieldType_FIXED_POINT >
		: public SSqliteBindingBase
	{
		/** Constructor
		@param statement
			The statement
		@param connection
			The database connection
		@param index
			The parameter index
		@param value
			The parameter value
		*/
		SSqliteBinding( sqlite3_stmt * statement, sqlite3 * connection, uint16_t index, CDatabaseValue< EFieldType_FIXED_POINT > const & value )
			: SSqliteBindingBase( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::UpdateValue
		virtual void UpdateValue()
		{
			if ( _value.IsNull() )
			{
				SQLiteCheck( sqlite3_bind_null( _statement, _index ), StringStream() << INFO_SQLITE_SET_PARAMETER_NULL, EDatabaseExceptionCodes_StatementError, _connection );
			}
			else
			{
				SQLiteCheck( sqlite3_bind_double( _statement, _index, _value.GetValue().ToDouble() ), StringStream() << INFO_SQLITE_SET_PARAMETER_VALUE << _value.GetValue().ToString(), EDatabaseExceptionCodes_StatementError, _connection );
			}
		}

		//! The parameter value
		CDatabaseValue< EFieldType_FIXED_POINT > const & _value;
	};

	/** SSqliteBinding specialization for EFieldType_CHAR
	*/
	template<>
	struct SSqliteBinding< EFieldType_CHAR >
		: public SSqliteBindingBase
	{
		/** Constructor
		@param statement
			The statement
		@param connection
			The database connection
		@param index
			The parameter index
		@param value
			The parameter value
		*/
		SSqliteBinding( sqlite3_stmt * statement, sqlite3 * connection, uint16_t index, CDatabaseValue< EFieldType_CHAR > const & value )
			: SSqliteBindingBase( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::UpdateValue
		virtual void UpdateValue()
		{
			if ( _value.IsNull() )
			{
				SQLiteCheck( sqlite3_bind_null( _statement, _index ), StringStream() << INFO_SQLITE_SET_PARAMETER_NULL, EDatabaseExceptionCodes_StatementError, _connection );
			}
			else
			{
				SQLiteCheck( sqlite3_bind_text64( _statement, _index, ( const char * )_value.GetPtrValue(), _value.GetPtrSize(), SQLITE_STATIC, SQLITE_UTF8 ), StringStream() << INFO_SQLITE_SET_PARAMETER_VALUE << STR( "[" ) <<  _value.GetValue() << STR( "]" ), EDatabaseExceptionCodes_StatementError, _connection );
			}
		}

		//! The parameter value
		CDatabaseValue< EFieldType_CHAR > const & _value;
	};

	/** SSqliteBinding specialization for EFieldType_VARCHAR
	*/
	template<>
	struct SSqliteBinding< EFieldType_VARCHAR >
		: public SSqliteBindingBase
	{
		/** Constructor
		@param statement
			The statement
		@param connection
			The database connection
		@param index
			The parameter index
		@param value
			The parameter value
		*/
		SSqliteBinding( sqlite3_stmt * statement, sqlite3 * connection, uint16_t index, CDatabaseValue< EFieldType_VARCHAR > const & value )
			: SSqliteBindingBase( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::UpdateValue
		virtual void UpdateValue()
		{
			if ( _value.IsNull() )
			{
				SQLiteCheck( sqlite3_bind_null( _statement, _index ), StringStream() << INFO_SQLITE_SET_PARAMETER_NULL, EDatabaseExceptionCodes_StatementError, _connection );
			}
			else
			{
				SQLiteCheck( sqlite3_bind_text64( _statement, _index, ( const char * )_value.GetPtrValue(), _value.GetPtrSize(), SQLITE_STATIC, SQLITE_UTF8 ), StringStream() << INFO_SQLITE_SET_PARAMETER_VALUE << STR( "[" ) <<  _value.GetValue() << STR( "]" ), EDatabaseExceptionCodes_StatementError, _connection );
			}
		}

		//! The parameter value
		CDatabaseValue< EFieldType_VARCHAR > const & _value;
	};

	/** SSqliteBinding specialization for EFieldType_TEXT
	*/
	template<>
	struct SSqliteBinding< EFieldType_TEXT >
		: public SSqliteBindingBase
	{
		/** Constructor
		@param statement
			The statement
		@param connection
			The database connection
		@param index
			The parameter index
		@param value
			The parameter value
		*/
		SSqliteBinding( sqlite3_stmt * statement, sqlite3 * connection, uint16_t index, CDatabaseValue< EFieldType_TEXT > const & value )
			: SSqliteBindingBase( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::UpdateValue
		virtual void UpdateValue()
		{
			if ( _value.IsNull() )
			{
				SQLiteCheck( sqlite3_bind_null( _statement, _index ), StringStream() << INFO_SQLITE_SET_PARAMETER_NULL, EDatabaseExceptionCodes_StatementError, _connection );
			}
			else
			{
				SQLiteCheck( sqlite3_bind_text64( _statement, _index, ( const char * )_value.GetPtrValue(), _value.GetPtrSize(), SQLITE_STATIC, SQLITE_UTF8 ), StringStream() << INFO_SQLITE_SET_PARAMETER_VALUE << STR( "[" ) <<  _value.GetValue() << STR( "]" ), EDatabaseExceptionCodes_StatementError, _connection );
			}
		}

		//! The parameter value
		CDatabaseValue< EFieldType_TEXT > const & _value;
	};

	/** SSqliteBinding specialization for EFieldType_NVARCHAR
	*/
	template<>
	struct SSqliteBinding< EFieldType_NCHAR >
		: public SSqliteBindingBase
	{
		/** Constructor
		@param statement
			The statement
		@param connection
			The database connection
		@param index
			The parameter index
		@param value
			The parameter value
		*/
		SSqliteBinding( sqlite3_stmt * statement, sqlite3 * connection, uint16_t index, CDatabaseValue< EFieldType_NCHAR > const & value )
			: SSqliteBindingBase( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::UpdateValue
		virtual void UpdateValue()
		{
			if ( _value.IsNull() )
			{
				SQLiteCheck( sqlite3_bind_null( _statement, _index ), StringStream() << INFO_SQLITE_SET_PARAMETER_NULL, EDatabaseExceptionCodes_StatementError, _connection );
			}
			else
			{
				SQLiteCheck( sqlite3_bind_text16( _statement, _index, _value.GetPtrValue(), _value.GetPtrSize(), SQLITE_STATIC ), StringStream() << INFO_SQLITE_SET_PARAMETER_VALUE << _value.GetPtrValue(), EDatabaseExceptionCodes_StatementError, _connection );
			}
		}

		//! The parameter value
		CDatabaseValue< EFieldType_NCHAR > const & _value;
	};

	/** SSqliteBinding specialization for EFieldType_NVARCHAR
	*/
	template<>
	struct SSqliteBinding< EFieldType_NVARCHAR >
		: public SSqliteBindingBase
	{
		/** Constructor
		@param statement
			The statement
		@param connection
			The database connection
		@param index
			The parameter index
		@param value
			The parameter value
		*/
		SSqliteBinding( sqlite3_stmt * statement, sqlite3 * connection, uint16_t index, CDatabaseValue< EFieldType_NVARCHAR > const & value )
			: SSqliteBindingBase( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::UpdateValue
		virtual void UpdateValue()
		{
			if ( _value.IsNull() )
			{
				SQLiteCheck( sqlite3_bind_null( _statement, _index ), StringStream() << INFO_SQLITE_SET_PARAMETER_NULL, EDatabaseExceptionCodes_StatementError, _connection );
			}
			else
			{
				SQLiteCheck( sqlite3_bind_text16( _statement, _index, _value.GetPtrValue(), _value.GetPtrSize(), SQLITE_STATIC ), StringStream() << INFO_SQLITE_SET_PARAMETER_VALUE << _value.GetPtrValue(), EDatabaseExceptionCodes_StatementError, _connection );
			}
		}

		//! The parameter value
		CDatabaseValue< EFieldType_NVARCHAR > const & _value;
	};

	/** SSqliteBinding specialization for EFieldType_NTEXT
	*/
	template<>
	struct SSqliteBinding< EFieldType_NTEXT >
		: public SSqliteBindingBase
	{
		/** Constructor
		@param statement
			The statement
		@param connection
			The database connection
		@param index
			The parameter index
		@param value
			The parameter value
		*/
		SSqliteBinding( sqlite3_stmt * statement, sqlite3 * connection, uint16_t index, CDatabaseValue< EFieldType_NTEXT > const & value )
			: SSqliteBindingBase( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::UpdateValue
		virtual void UpdateValue()
		{
			if ( _value.IsNull() )
			{
				SQLiteCheck( sqlite3_bind_null( _statement, _index ), StringStream() << INFO_SQLITE_SET_PARAMETER_NULL, EDatabaseExceptionCodes_StatementError, _connection );
			}
			else
			{
				SQLiteCheck( sqlite3_bind_text16( _statement, _index, _value.GetPtrValue(), _value.GetPtrSize(), SQLITE_STATIC ), StringStream() << INFO_SQLITE_SET_PARAMETER_VALUE << _value.GetPtrValue(), EDatabaseExceptionCodes_StatementError, _connection );
			}
		}

		//! The parameter value
		CDatabaseValue< EFieldType_NTEXT > const & _value;
	};

	/** SSqliteBinding specialization for EFieldType_DATE
	*/
	template<>
	struct SSqliteBinding< EFieldType_DATE >
		: public SSqliteBindingBase
	{
		/** Constructor
		@param statement
			The statement
		@param connection
			The database connection
		@param index
			The parameter index
		@param value
			The parameter value
		*/
		SSqliteBinding( sqlite3_stmt * statement, sqlite3 * connection, uint16_t index, CDatabaseValue< EFieldType_DATE > const & value )
			: SSqliteBindingBase( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::UpdateValue
		virtual void UpdateValue()
		{
			if ( _value.IsNull() )
			{
				SQLiteCheck( sqlite3_bind_null( _statement, _index ), StringStream() << INFO_SQLITE_SET_PARAMETER_NULL, EDatabaseExceptionCodes_StatementError, _connection );
			}
			else
			{
				SQLiteCheck( sqlite3_bind_text( _statement, _index, ( const char * )_value.GetPtrValue(), _value.GetPtrSize(), SQLITE_STATIC ), StringStream() << INFO_SQLITE_SET_PARAMETER_VALUE << _value.GetValue(), EDatabaseExceptionCodes_StatementError, _connection );
			}
		}

		//! The parameter value
		CDatabaseValue< EFieldType_DATE > const & _value;
	};

	/** SSqliteBinding specialization for EFieldType_DATETIME
	*/
	template<>
	struct SSqliteBinding< EFieldType_DATETIME >
		: public SSqliteBindingBase
	{
		/** Constructor
		@param statement
			The statement
		@param connection
			The database connection
		@param index
			The parameter index
		@param value
			The parameter value
		*/
		SSqliteBinding( sqlite3_stmt * statement, sqlite3 * connection, uint16_t index, CDatabaseValue< EFieldType_DATETIME > const & value )
			: SSqliteBindingBase( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::UpdateValue
		virtual void UpdateValue()
		{
			if ( _value.IsNull() )
			{
				SQLiteCheck( sqlite3_bind_null( _statement, _index ), StringStream() << INFO_SQLITE_SET_PARAMETER_NULL, EDatabaseExceptionCodes_StatementError, _connection );
			}
			else
			{
				SQLiteCheck( sqlite3_bind_text( _statement, _index, ( const char * )_value.GetPtrValue(), _value.GetPtrSize(), SQLITE_STATIC ), StringStream() << INFO_SQLITE_SET_PARAMETER_VALUE << _value.GetValue(), EDatabaseExceptionCodes_StatementError, _connection );
			}
		}

		//! The parameter value
		CDatabaseValue< EFieldType_DATETIME > const & _value;
	};

	/** SSqliteBinding specialization for EFieldType_TIME
	*/
	template<>
	struct SSqliteBinding< EFieldType_TIME >
		: public SSqliteBindingBase
	{
		/** Constructor
		@param statement
			The statement
		@param connection
			The database connection
		@param index
			The parameter index
		@param value
			The parameter value
		*/
		SSqliteBinding( sqlite3_stmt * statement, sqlite3 * connection, uint16_t index, CDatabaseValue< EFieldType_TIME > const & value )
			: SSqliteBindingBase( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::UpdateValue
		virtual void UpdateValue()
		{
			if ( _value.IsNull() )
			{
				SQLiteCheck( sqlite3_bind_null( _statement, _index ), StringStream() << INFO_SQLITE_SET_PARAMETER_NULL, EDatabaseExceptionCodes_StatementError, _connection );
			}
			else
			{
				SQLiteCheck( sqlite3_bind_text( _statement, _index, ( const char * )_value.GetPtrValue(), _value.GetPtrSize(), SQLITE_STATIC ), StringStream() << INFO_SQLITE_SET_PARAMETER_VALUE << _value.GetValue(), EDatabaseExceptionCodes_StatementError, _connection );
			}
		}

		//! The parameter value
		CDatabaseValue< EFieldType_TIME > const & _value;
	};

	/** SSqliteBinding specialization for EFieldType_BINARY
	*/
	template<>
	struct SSqliteBinding< EFieldType_BINARY >
		: public SSqliteBindingBase
	{
		/** Constructor
		@param statement
			The statement
		@param connection
			The database connection
		@param index
			The parameter index
		@param value
			The parameter value
		*/
		SSqliteBinding( sqlite3_stmt * statement, sqlite3 * connection, uint16_t index, CDatabaseValue< EFieldType_BINARY > const & value )
			: SSqliteBindingBase( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::UpdateValue
		virtual void UpdateValue()
		{
			if ( _value.IsNull() )
			{
				SQLiteCheck( sqlite3_bind_null( _statement, _index ), StringStream() << INFO_SQLITE_SET_PARAMETER_NULL, EDatabaseExceptionCodes_StatementError, _connection );
			}
			else
			{
				SQLiteCheck( sqlite3_bind_blob( _statement, _index, _value.GetPtrValue(), _value.GetPtrSize(), SQLITE_STATIC ), StringStream() << INFO_SQLITE_SET_PARAMETER_VALUE << _value.GetPtrValue(), EDatabaseExceptionCodes_StatementError, _connection );
			}
		}

		//! The parameter value
		CDatabaseValue< EFieldType_BINARY > const & _value;
	};

	/** SSqliteBinding specialization for EFieldType_VARBINARY
	*/
	template<>
	struct SSqliteBinding< EFieldType_VARBINARY >
		: public SSqliteBindingBase
	{
		/** Constructor
		@param statement
			The statement
		@param connection
			The database connection
		@param index
			The parameter index
		@param value
			The parameter value
		*/
		SSqliteBinding( sqlite3_stmt * statement, sqlite3 * connection, uint16_t index, CDatabaseValue< EFieldType_VARBINARY > const & value )
			: SSqliteBindingBase( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::UpdateValue
		virtual void UpdateValue()
		{
			if ( _value.IsNull() )
			{
				SQLiteCheck( sqlite3_bind_null( _statement, _index ), StringStream() << INFO_SQLITE_SET_PARAMETER_NULL, EDatabaseExceptionCodes_StatementError, _connection );
			}
			else
			{
				SQLiteCheck( sqlite3_bind_blob( _statement, _index, _value.GetPtrValue(), _value.GetPtrSize(), SQLITE_STATIC ), StringStream() << INFO_SQLITE_SET_PARAMETER_VALUE << _value.GetPtrValue(), EDatabaseExceptionCodes_StatementError, _connection );
			}
		}

		//! The parameter value
		CDatabaseValue< EFieldType_VARBINARY > const & _value;
	};

	/** SSqliteBinding specialization for EFieldType_LONG_VARBINARY
	*/
	template<>
	struct SSqliteBinding< EFieldType_LONG_VARBINARY >
		: public SSqliteBindingBase
	{
		/** Constructor
		@param statement
			The statement
		@param connection
			The database connection
		@param index
			The parameter index
		@param value
			The parameter value
		*/
		SSqliteBinding( sqlite3_stmt * statement, sqlite3 * connection, uint16_t index, CDatabaseValue< EFieldType_LONG_VARBINARY > const & value )
			: SSqliteBindingBase( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::UpdateValue
		virtual void UpdateValue()
		{
			if ( _value.IsNull() )
			{
				SQLiteCheck( sqlite3_bind_null( _statement, _index ), StringStream() << INFO_SQLITE_SET_PARAMETER_NULL, EDatabaseExceptionCodes_StatementError, _connection );
			}
			else
			{
				SQLiteCheck( sqlite3_bind_blob( _statement, _index, _value.GetPtrValue(), _value.GetPtrSize(), SQLITE_STATIC ), StringStream() << INFO_SQLITE_SET_PARAMETER_VALUE << _value.GetPtrValue(), EDatabaseExceptionCodes_StatementError, _connection );
			}
		}

		//! The parameter value
		CDatabaseValue< EFieldType_LONG_VARBINARY > const & _value;
	};

	/** Function used to create a SSqliteBinding easily
	@param statement
		The statement
	@param connection
		The database connection
	@param index
		The parameter index
	@param value
		The parameter value
	*/
	template< EFieldType Type >
	std::unique_ptr< SSqliteBindingBase > MakeSqliteBind( sqlite3_stmt * statement, DatabaseConnectionPtr connection, uint16_t index, CDatabaseValueBase const & value )
	{
		return std::make_unique< SSqliteBinding< Type > >( statement, std::static_pointer_cast< CDatabaseConnectionSqlite >( connection )->GetConnection(), index, static_cast< CDatabaseValue< Type > const & >( value ) );
	}
}
END_NAMESPACE_DATABASE_SQLITE

#endif // ___DATABASE_STATEMENTPARAMETER_SETTER_H___
