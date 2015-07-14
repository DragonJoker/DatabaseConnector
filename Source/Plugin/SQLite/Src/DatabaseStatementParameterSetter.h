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

#include <DatabaseStringUtils.h>
#include <DatabaseLogger.h>

BEGIN_NAMESPACE_DATABASE_SQLITE
{
	static const String ERROR_SQLITE_PARAMETER_VALUE = STR( "Can't set parameter value" );
	static const String ERROR_SQLITE_UPDATE_UNIMPLEMENTED = STR( "UpdateValue not implemented for this data type" );

	static const String INFO_SQLITE_SET_PARAMETER_VALUE = STR( "Set parameter value: " );

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
		SSqliteBinding( SQLite::Statement * statement, SQLite::Database * connection, uint16_t index, CDatabaseValue< Type > const & value )
			: SSqliteBindingBase( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::DoSetValue
		virtual void UpdateValue()
		{
			throw DB_EXCEPT( EDatabaseExceptionCodes_Unimplemented, ERROR_SQLITE_UPDATE_UNIMPLEMENTED );
		}

		//! The parameter value
		CDatabaseValue< Type > _value;
	};

	/** Specialization for EFieldType_BIT
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
		SSqliteBinding( SQLite::Statement * statement, SQLite::Database * connection, uint16_t index, CDatabaseValue< EFieldType_BIT > const & value )
			: SSqliteBindingBase( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::DoSetValue
		virtual void UpdateValue()
		{
			SQLiteTry( SQLite::BindInt( _statement, _index, _value.GetValue() ? 1 : 0 ), StringStream() << INFO_SQLITE_SET_PARAMETER_VALUE << _value.GetValue(), EDatabaseExceptionCodes_StatementError, _connection );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_BIT > const & _value;
	};

	/** Specialization for EFieldType_INT8
	*/
	template<>
	struct SSqliteBinding< EFieldType_INT8 >
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
		SSqliteBinding( SQLite::Statement * statement, SQLite::Database * connection, uint16_t index, CDatabaseValue< EFieldType_INT8 > const & value )
			: SSqliteBindingBase( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::DoSetValue
		virtual void UpdateValue()
		{
			SQLiteTry( SQLite::BindInt( _statement, _index, _value.GetValue() ), StringStream() << INFO_SQLITE_SET_PARAMETER_VALUE << int16_t( _value.GetValue() ), EDatabaseExceptionCodes_StatementError, _connection );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_INT8 > const & _value;
	};

	/** Specialization for EFieldType_INT16
	*/
	template<>
	struct SSqliteBinding< EFieldType_INT16 >
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
		SSqliteBinding( SQLite::Statement * statement, SQLite::Database * connection, uint16_t index, CDatabaseValue< EFieldType_INT16 > const & value )
			: SSqliteBindingBase( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::DoSetValue
		virtual void UpdateValue()
		{
			SQLiteTry( SQLite::BindInt( _statement, _index, _value.GetValue() ), StringStream() << INFO_SQLITE_SET_PARAMETER_VALUE << _value.GetValue(), EDatabaseExceptionCodes_StatementError, _connection );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_INT16 > const & _value;
	};

	/** Specialization for EFieldType_INT32
	*/
	template<>
	struct SSqliteBinding< EFieldType_INT24 >
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
		SSqliteBinding( SQLite::Statement * statement, SQLite::Database * connection, uint16_t index, CDatabaseValue< EFieldType_INT24 > const & value )
			: SSqliteBindingBase( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::DoSetValue
		virtual void UpdateValue()
		{
			SQLiteTry( SQLite::BindInt( _statement, _index, _value.GetValue() ), StringStream() << INFO_SQLITE_SET_PARAMETER_VALUE << _value.GetValue(), EDatabaseExceptionCodes_StatementError, _connection );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_INT24 > const & _value;
	};

	/** Specialization for EFieldType_INT32
	*/
	template<>
	struct SSqliteBinding< EFieldType_INT32 >
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
		SSqliteBinding( SQLite::Statement * statement, SQLite::Database * connection, uint16_t index, CDatabaseValue< EFieldType_INT32 > const & value )
			: SSqliteBindingBase( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::DoSetValue
		virtual void UpdateValue()
		{
			SQLiteTry( SQLite::BindInt( _statement, _index, _value.GetValue() ), StringStream() << INFO_SQLITE_SET_PARAMETER_VALUE << _value.GetValue(), EDatabaseExceptionCodes_StatementError, _connection );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_INT32 > const & _value;
	};

	/** Specialization for EFieldType_INT64
	*/
	template<>
	struct SSqliteBinding< EFieldType_INT64 >
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
		SSqliteBinding( SQLite::Statement * statement, SQLite::Database * connection, uint16_t index, CDatabaseValue< EFieldType_INT64 > const & value )
			: SSqliteBindingBase( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::DoSetValue
		virtual void UpdateValue()
		{
			SQLiteTry( SQLite::BindInt64( _statement, _index, _value.GetValue() ), StringStream() << INFO_SQLITE_SET_PARAMETER_VALUE << _value.GetValue(), EDatabaseExceptionCodes_StatementError, _connection );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_INT64 > const & _value;
	};

	/** Specialization for EFieldType_FLOAT32
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
		SSqliteBinding( SQLite::Statement * statement, SQLite::Database * connection, uint16_t index, CDatabaseValue< EFieldType_FLOAT32 > const & value )
			: SSqliteBindingBase( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::DoSetValue
		virtual void UpdateValue()
		{
			SQLiteTry( SQLite::BindDouble( _statement, _index, _value.GetValue() ), StringStream() << INFO_SQLITE_SET_PARAMETER_VALUE << _value.GetValue(), EDatabaseExceptionCodes_StatementError, _connection );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_FLOAT32 > const & _value;
	};

	/** Specialization for EFieldType_FLOAT64
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
		SSqliteBinding( SQLite::Statement * statement, SQLite::Database * connection, uint16_t index, CDatabaseValue< EFieldType_FLOAT64 > const & value )
			: SSqliteBindingBase( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::DoSetValue
		virtual void UpdateValue()
		{
			SQLiteTry( SQLite::BindDouble( _statement, _index, _value.GetValue() ), StringStream() << INFO_SQLITE_SET_PARAMETER_VALUE << _value.GetValue(), EDatabaseExceptionCodes_StatementError, _connection );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_FLOAT64 > const & _value;
	};

	/** Specialization for EFieldType_FIXED_POINT
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
		SSqliteBinding( SQLite::Statement * statement, SQLite::Database * connection, uint16_t index, CDatabaseValue< EFieldType_FIXED_POINT > const & value )
			: SSqliteBindingBase( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::DoSetValue
		virtual void UpdateValue()
		{
			SQLiteTry( SQLite::BindDouble( _statement, _index, _value.GetValue().ToDouble() ), StringStream() << INFO_SQLITE_SET_PARAMETER_VALUE << _value.GetValue().ToString(), EDatabaseExceptionCodes_StatementError, _connection );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_FIXED_POINT > const & _value;
	};

	/** Specialization for EFieldType_VARCHAR
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
		SSqliteBinding( SQLite::Statement * statement, SQLite::Database * connection, uint16_t index, CDatabaseValue< EFieldType_VARCHAR > const & value )
			: SSqliteBindingBase( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::DoSetValue
		virtual void UpdateValue()
		{
			SQLiteTry( SQLite::BindText( _statement, _index, ( const char * )_value.GetPtrValue(), _value.GetPtrSize(), SQLite::NULL_DESTRUCTOR ), StringStream() << INFO_SQLITE_SET_PARAMETER_VALUE << _value.GetValue(), EDatabaseExceptionCodes_StatementError, _connection );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_VARCHAR > const & _value;
	};

	/** Specialization for EFieldType_TEXT
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
		SSqliteBinding( SQLite::Statement * statement, SQLite::Database * connection, uint16_t index, CDatabaseValue< EFieldType_TEXT > const & value )
			: SSqliteBindingBase( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::DoSetValue
		virtual void UpdateValue()
		{
			SQLiteTry( SQLite::BindText( _statement, _index, ( const char * )_value.GetPtrValue(), _value.GetPtrSize(), SQLite::NULL_DESTRUCTOR ), StringStream() << INFO_SQLITE_SET_PARAMETER_VALUE << _value.GetValue(), EDatabaseExceptionCodes_StatementError, _connection );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_TEXT > const & _value;
	};

	/** Specialization for EFieldType_NVARCHAR
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
		SSqliteBinding( SQLite::Statement * statement, SQLite::Database * connection, uint16_t index, CDatabaseValue< EFieldType_NVARCHAR > const & value )
			: SSqliteBindingBase( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::DoSetValue
		virtual void UpdateValue()
		{
			SQLiteTry( SQLite::BindText16( _statement, _index, _value.GetPtrValue(), _value.GetPtrSize(), SQLite::NULL_DESTRUCTOR ), StringStream() << INFO_SQLITE_SET_PARAMETER_VALUE << _value.GetPtrValue(), EDatabaseExceptionCodes_StatementError, _connection );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_NVARCHAR > const & _value;
	};

	/** Specialization for EFieldType_NTEXT
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
		SSqliteBinding( SQLite::Statement * statement, SQLite::Database * connection, uint16_t index, CDatabaseValue< EFieldType_NTEXT > const & value )
			: SSqliteBindingBase( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::DoSetValue
		virtual void UpdateValue()
		{
			SQLiteTry( SQLite::BindText16( _statement, _index, _value.GetPtrValue(), _value.GetPtrSize(), SQLite::NULL_DESTRUCTOR ), StringStream() << INFO_SQLITE_SET_PARAMETER_VALUE << _value.GetPtrValue(), EDatabaseExceptionCodes_StatementError, _connection );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_NTEXT > const & _value;
	};

	/** Specialization for EFieldType_DATE
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
		SSqliteBinding( SQLite::Statement * statement, SQLite::Database * connection, uint16_t index, CDatabaseValue< EFieldType_DATE > const & value )
			: SSqliteBindingBase( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::DoSetValue
		virtual void UpdateValue()
		{
			SQLiteTry( SQLite::BindText( _statement, _index, ( const char * )_value.GetPtrValue(), _value.GetPtrSize(), SQLite::NULL_DESTRUCTOR ), StringStream() << INFO_SQLITE_SET_PARAMETER_VALUE << _value.GetValue(), EDatabaseExceptionCodes_StatementError, _connection );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_DATE > const & _value;
	};

	/** Specialization for EFieldType_DATETIME
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
		SSqliteBinding( SQLite::Statement * statement, SQLite::Database * connection, uint16_t index, CDatabaseValue< EFieldType_DATETIME > const & value )
			: SSqliteBindingBase( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::DoSetValue
		virtual void UpdateValue()
		{
			SQLiteTry( SQLite::BindText( _statement, _index, ( const char * )_value.GetPtrValue(), _value.GetPtrSize(), SQLite::NULL_DESTRUCTOR ), StringStream() << INFO_SQLITE_SET_PARAMETER_VALUE << _value.GetValue(), EDatabaseExceptionCodes_StatementError, _connection );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_DATETIME > const & _value;
	};

	/** Specialization for EFieldType_TIME
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
		SSqliteBinding( SQLite::Statement * statement, SQLite::Database * connection, uint16_t index, CDatabaseValue< EFieldType_TIME > const & value )
			: SSqliteBindingBase( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::DoSetValue
		virtual void UpdateValue()
		{
			SQLiteTry( SQLite::BindText( _statement, _index, ( const char * )_value.GetPtrValue(), _value.GetPtrSize(), SQLite::NULL_DESTRUCTOR ), StringStream() << INFO_SQLITE_SET_PARAMETER_VALUE << _value.GetValue(), EDatabaseExceptionCodes_StatementError, _connection );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_TIME > const & _value;
	};

	/** Specialization for EFieldType_BINARY
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
		SSqliteBinding( SQLite::Statement * statement, SQLite::Database * connection, uint16_t index, CDatabaseValue< EFieldType_BINARY > const & value )
			: SSqliteBindingBase( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::DoSetValue
		virtual void UpdateValue()
		{
			SQLiteTry( SQLite::BindBlob( _statement, _index, _value.GetPtrValue(), _value.GetPtrSize(), SQLite::NULL_DESTRUCTOR ), StringStream() << INFO_SQLITE_SET_PARAMETER_VALUE << _value.GetPtrValue(), EDatabaseExceptionCodes_StatementError, _connection );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_BINARY > const & _value;
	};

	/** Specialization for EFieldType_BINARY
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
		SSqliteBinding( SQLite::Statement * statement, SQLite::Database * connection, uint16_t index, CDatabaseValue< EFieldType_VARBINARY > const & value )
			: SSqliteBindingBase( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::DoSetValue
		virtual void UpdateValue()
		{
			SQLiteTry( SQLite::BindBlob( _statement, _index, _value.GetPtrValue(), _value.GetPtrSize(), SQLite::NULL_DESTRUCTOR ), StringStream() << INFO_SQLITE_SET_PARAMETER_VALUE << _value.GetPtrValue(), EDatabaseExceptionCodes_StatementError, _connection );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_VARBINARY > const & _value;
	};

	/** Specialization for EFieldType_BINARY
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
		SSqliteBinding( SQLite::Statement * statement, SQLite::Database * connection, uint16_t index, CDatabaseValue< EFieldType_LONG_VARBINARY > const & value )
			: SSqliteBindingBase( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::DoSetValue
		virtual void UpdateValue()
		{
			SQLiteTry( SQLite::BindBlob( _statement, _index, _value.GetPtrValue(), _value.GetPtrSize(), SQLite::NULL_DESTRUCTOR ), StringStream() << INFO_SQLITE_SET_PARAMETER_VALUE << _value.GetPtrValue(), EDatabaseExceptionCodes_StatementError, _connection );
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
	std::unique_ptr< SSqliteBindingBase > MakeSqliteBind( SQLite::Statement * statement, DatabaseConnectionPtr connection, uint16_t index, CDatabaseValueBase const & value )
	{
		return std::make_unique< SSqliteBinding< Type > >( statement, std::static_pointer_cast< CDatabaseConnectionSqlite >( connection )->GetConnection(), index, static_cast< CDatabaseValue< Type > const & >( value ) );
	}
}
END_NAMESPACE_DATABASE_SQLITE

#endif // ___DATABASE_STATEMENTPARAMETER_SETTER_H___
