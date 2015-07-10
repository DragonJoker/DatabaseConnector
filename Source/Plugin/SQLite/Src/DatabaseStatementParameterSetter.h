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
			throw std::runtime_error( "SSqliteBinding::DoSetValue not implemented for this data type" );
		}

		//! The parameter value
		CDatabaseValue< Type > _value;
	};

	/** Specialization for EFieldType_BOOL
	*/
	template<>
	struct SSqliteBinding< EFieldType_BOOL >
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
		SSqliteBinding( SQLite::Statement * statement, SQLite::Database * connection, uint16_t index, CDatabaseValue< EFieldType_BOOL > const & value )
			: SSqliteBindingBase( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::DoSetValue
		virtual void UpdateValue()
		{
			SQLiteTry( SQLite::BindInt( _statement, _index, _value.GetValue() ? 1 : 0 ), StringStream() << STR( "Parameter set value: " ) << _value.GetValue(), EDatabaseExceptionCodes_StatementError, _connection );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_BOOL > const & _value;
	};

	/** Specialization for EFieldType_SMALL_INTEGER
	*/
	template<>
	struct SSqliteBinding< EFieldType_SMALL_INTEGER >
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
		SSqliteBinding( SQLite::Statement * statement, SQLite::Database * connection, uint16_t index, CDatabaseValue< EFieldType_SMALL_INTEGER > const & value )
			: SSqliteBindingBase( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::DoSetValue
		virtual void UpdateValue()
		{
			SQLiteTry( SQLite::BindInt( _statement, _index, _value.GetValue() ), StringStream() << STR( "Parameter set value: " ) << _value.GetValue(), EDatabaseExceptionCodes_StatementError, _connection );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_SMALL_INTEGER > const & _value;
	};

	/** Specialization for EFieldType_INTEGER
	*/
	template<>
	struct SSqliteBinding< EFieldType_INTEGER >
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
		SSqliteBinding( SQLite::Statement * statement, SQLite::Database * connection, uint16_t index, CDatabaseValue< EFieldType_INTEGER > const & value )
			: SSqliteBindingBase( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::DoSetValue
		virtual void UpdateValue()
		{
			SQLiteTry( SQLite::BindInt( _statement, _index, _value.GetValue() ), StringStream() << STR( "Parameter set value: " ) << _value.GetValue(), EDatabaseExceptionCodes_StatementError, _connection );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_INTEGER > const & _value;
	};

	/** Specialization for EFieldType_LONG_INTEGER
	*/
	template<>
	struct SSqliteBinding< EFieldType_LONG_INTEGER >
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
		SSqliteBinding( SQLite::Statement * statement, SQLite::Database * connection, uint16_t index, CDatabaseValue< EFieldType_LONG_INTEGER > const & value )
			: SSqliteBindingBase( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::DoSetValue
		virtual void UpdateValue()
		{
			SQLiteTry( SQLite::BindInt64( _statement, _index, _value.GetValue() ), StringStream() << STR( "Parameter set value: " ) << _value.GetValue(), EDatabaseExceptionCodes_StatementError, _connection );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_LONG_INTEGER > const & _value;
	};

	/** Specialization for EFieldType_FLOAT
	*/
	template<>
	struct SSqliteBinding< EFieldType_FLOAT >
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
		SSqliteBinding( SQLite::Statement * statement, SQLite::Database * connection, uint16_t index, CDatabaseValue< EFieldType_FLOAT > const & value )
			: SSqliteBindingBase( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::DoSetValue
		virtual void UpdateValue()
		{
			SQLiteTry( SQLite::BindDouble( _statement, _index, _value.GetValue() ), StringStream() << STR( "Parameter set value: " ) << _value.GetValue(), EDatabaseExceptionCodes_StatementError, _connection );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_FLOAT > const & _value;
	};

	/** Specialization for EFieldType_DOUBLE
	*/
	template<>
	struct SSqliteBinding< EFieldType_DOUBLE >
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
		SSqliteBinding( SQLite::Statement * statement, SQLite::Database * connection, uint16_t index, CDatabaseValue< EFieldType_DOUBLE > const & value )
			: SSqliteBindingBase( statement, connection, index )
			, _value( value )
		{
		}

		//!@copydoc SSqliteBindingBase::DoSetValue
		virtual void UpdateValue()
		{
			SQLiteTry( SQLite::BindDouble( _statement, _index, _value.GetValue() ), StringStream() << STR( "Parameter set value: " ) << _value.GetValue(), EDatabaseExceptionCodes_StatementError, _connection );
		}

		//! The parameter value
		CDatabaseValue< EFieldType_DOUBLE > const & _value;
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
			SQLiteTry( SQLite::BindText( _statement, _index, ( const char * )_value.GetPtrValue(), _value.GetPtrSize(), SQLite::NULL_DESTRUCTOR ), StringStream() << STR( "Parameter set value: " ) << _value.GetValue(), EDatabaseExceptionCodes_StatementError, _connection );
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
			SQLiteTry( SQLite::BindText( _statement, _index, ( const char * )_value.GetPtrValue(), _value.GetPtrSize(), SQLite::NULL_DESTRUCTOR ), StringStream() << STR( "Parameter set value: " ) << _value.GetValue(), EDatabaseExceptionCodes_StatementError, _connection );
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
			SQLiteTry( SQLite::BindText16( _statement, _index, _value.GetPtrValue(), _value.GetPtrSize(), SQLite::NULL_DESTRUCTOR ), StringStream() << STR( "Parameter set value: " ) << _value.GetPtrValue(), EDatabaseExceptionCodes_StatementError, _connection );
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
			SQLiteTry( SQLite::BindText16( _statement, _index, _value.GetPtrValue(), _value.GetPtrSize(), SQLite::NULL_DESTRUCTOR ), StringStream() << STR( "Parameter set value: " ) << _value.GetPtrValue(), EDatabaseExceptionCodes_StatementError, _connection );
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
			SQLiteTry( SQLite::BindText( _statement, _index, ( const char * )_value.GetPtrValue(), _value.GetPtrSize(), SQLite::NULL_DESTRUCTOR ), StringStream() << STR( "Parameter set value: " ) << _value.GetValue(), EDatabaseExceptionCodes_StatementError, _connection );
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
			SQLiteTry( SQLite::BindText( _statement, _index, ( const char * )_value.GetPtrValue(), _value.GetPtrSize(), SQLite::NULL_DESTRUCTOR ), StringStream() << STR( "Parameter set value: " ) << _value.GetValue(), EDatabaseExceptionCodes_StatementError, _connection );
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
			SQLiteTry( SQLite::BindText( _statement, _index, ( const char * )_value.GetPtrValue(), _value.GetPtrSize(), SQLite::NULL_DESTRUCTOR ), StringStream() << STR( "Parameter set value: " ) << _value.GetValue(), EDatabaseExceptionCodes_StatementError, _connection );
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
			SQLiteTry( SQLite::BindBlob( _statement, _index, _value.GetPtrValue(), _value.GetPtrSize(), SQLite::NULL_DESTRUCTOR ), StringStream() << STR( "Parameter set value: " ) << _value.GetPtrValue(), EDatabaseExceptionCodes_StatementError, _connection );
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
			SQLiteTry( SQLite::BindBlob( _statement, _index, _value.GetPtrValue(), _value.GetPtrSize(), SQLite::NULL_DESTRUCTOR ), StringStream() << STR( "Parameter set value: " ) << _value.GetPtrValue(), EDatabaseExceptionCodes_StatementError, _connection );
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
			SQLiteTry( SQLite::BindBlob( _statement, _index, _value.GetPtrValue(), _value.GetPtrSize(), SQLite::NULL_DESTRUCTOR ), StringStream() << STR( "Parameter set value: " ) << _value.GetPtrValue(), EDatabaseExceptionCodes_StatementError, _connection );
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
