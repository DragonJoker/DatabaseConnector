/************************************************************************//**
* @file DatabaseStatementParameterSetter.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 11:56:56 AM
*
*
* @brief SSqliteParameterValueSetterBase and SSqliteParameterValueSetter classes.
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

	/** Base setter class
	*/
	struct SSqliteParameterValueSetterBase
	{
public:
		/** Destructor
		*/
		virtual ~SSqliteParameterValueSetterBase()
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
		void operator()( SQLite::Statement * statement, const void * value, CDatabaseStatementParameterSqlite * parameter )
		{
			DoSetValue( statement, value, parameter, std::static_pointer_cast< CDatabaseConnectionSqlite >( parameter->GetConnection() )->GetConnection() );
		}

protected:
		/** Setter function, to implement in child classes
		@param statement
		    The statement
		@param value
		    The value pointer
		@param parameter
		    The parameter
		*/
		virtual void DoSetValue( SQLite::Statement * statement, const void * value, CDatabaseStatementParameterSqlite * parameter, SQLite::Database * connection ) = 0;
	};
	/** Generic template class to set the parameter value
	*/
	template< EFieldType Type >
	struct SSqliteParameterValueSetter
			: public SSqliteParameterValueSetterBase
	{
private:
		//!@copydoc SSqliteParameterValueSetterBase::DoSetValue
		virtual void DoSetValue( SQLite::Statement * statement, const void * value, CDatabaseStatementParameterSqlite * parameter, SQLite::Database * connection )
		{
			throw std::runtime_error( "SSqliteParameterValueSetter::DoSetValue not implemented for this data type" );
		}
	};
	/** Specialization for EFieldType_BOOL
	*/
	template<>
	struct SSqliteParameterValueSetter< EFieldType_BOOL >
			: public SSqliteParameterValueSetterBase
	{
private:
		//!@copydoc SSqliteParameterValueSetterBase::DoSetValue
		virtual void DoSetValue( SQLite::Statement * statement, const void * value, CDatabaseStatementParameterSqlite * parameter, SQLite::Database * connection )
		{
			const bool & val = *static_cast< const bool * >( value );
			SQLiteTry( SQLite::BindInt( statement, parameter->GetIndex(), val ? 1 : 0 ), StringStream() << STR( "Parameter set value: " ) << val, EDatabaseExceptionCodes_StatementError, connection );
		}
	};
	/** Specialization for EFieldType_SMALL_INTEGER
	*/
	template<>
	struct SSqliteParameterValueSetter< EFieldType_SMALL_INTEGER >
			: public SSqliteParameterValueSetterBase
	{
private:
		//!@copydoc SSqliteParameterValueSetterBase::DoSetValue
		virtual void DoSetValue( SQLite::Statement * statement, const void * value, CDatabaseStatementParameterSqlite * parameter, SQLite::Database * connection )
		{
			const int32_t & val = *static_cast< const int16_t * >( value );
			SQLiteTry( SQLite::BindInt( statement, parameter->GetIndex(), val ), StringStream() << STR( "Parameter set value: " ) << val, EDatabaseExceptionCodes_StatementError, connection );
		}
	};
	/** Specialization for EFieldType_INTEGER
	*/
	template<>
	struct SSqliteParameterValueSetter< EFieldType_INTEGER >
			: public SSqliteParameterValueSetterBase
	{
private:
		//!@copydoc SSqliteParameterValueSetterBase::DoSetValue
		virtual void DoSetValue( SQLite::Statement * statement, const void * value, CDatabaseStatementParameterSqlite * parameter, SQLite::Database * connection )
		{
			const int32_t & val = *static_cast< const int32_t * >( value );
			SQLiteTry( SQLite::BindInt( statement, parameter->GetIndex(), val ), StringStream() << STR( "Parameter set value: " ) << val, EDatabaseExceptionCodes_StatementError, connection );
		}
	};
	/** Specialization for EFieldType_LONG_INTEGER
	*/
	template<>
	struct SSqliteParameterValueSetter< EFieldType_LONG_INTEGER >
			: public SSqliteParameterValueSetterBase
	{
private:
		//!@copydoc SSqliteParameterValueSetterBase::DoSetValue
		virtual void DoSetValue( SQLite::Statement * statement, const void * value, CDatabaseStatementParameterSqlite * parameter, SQLite::Database * connection )
		{
			const SQLite::Int64 & val = *static_cast< const SQLite::Int64 * >( value );
			SQLiteTry( SQLite::BindInt64( statement, parameter->GetIndex(), val ), StringStream() << STR( "Parameter set value: " ) << val, EDatabaseExceptionCodes_StatementError, connection );
		}
	};
	/** Specialization for EFieldType_FLOAT
	*/
	template<>
	struct SSqliteParameterValueSetter< EFieldType_FLOAT >
			: public SSqliteParameterValueSetterBase
	{
private:
		//!@copydoc SSqliteParameterValueSetterBase::DoSetValue
		virtual void DoSetValue( SQLite::Statement * statement, const void * value, CDatabaseStatementParameterSqlite * parameter, SQLite::Database * connection )
		{
			const float & val = *static_cast< const float * >( value );
			SQLiteTry( SQLite::BindDouble( statement, parameter->GetIndex(), val ), StringStream() << STR( "Parameter set value: " ) << val, EDatabaseExceptionCodes_StatementError, connection );
		}
	};
	/** Specialization for EFieldType_DOUBLE
	*/
	template<>
	struct SSqliteParameterValueSetter< EFieldType_DOUBLE >
			: public SSqliteParameterValueSetterBase
	{
private:
		//!@copydoc SSqliteParameterValueSetterBase::DoSetValue
		virtual void DoSetValue( SQLite::Statement * statement, const void * value, CDatabaseStatementParameterSqlite * parameter, SQLite::Database * connection )
		{
			const double & val = *static_cast< const double * >( value );
			SQLiteTry( SQLite::BindDouble( statement, parameter->GetIndex(), val ), StringStream() << STR( "Parameter set value: " ) << val, EDatabaseExceptionCodes_StatementError, connection );
		}
	};
	/** Specialization for EFieldType_VARCHAR
	*/
	template<>
	struct SSqliteParameterValueSetter< EFieldType_VARCHAR >
			: public SSqliteParameterValueSetterBase
	{
private:
		//!@copydoc SSqliteParameterValueSetterBase::DoSetValue
		virtual void DoSetValue( SQLite::Statement * statement, const void * value, CDatabaseStatementParameterSqlite * parameter, SQLite::Database * connection )
		{
			char const * val = static_cast< const char * >( value );
			SQLiteTry( SQLite::BindText( statement, parameter->GetIndex(), val, parameter->GetObjectValue().GetPtrSize(), SQLite::NULL_DESTRUCTOR ), StringStream() << STR( "Parameter set value: " ) << val, EDatabaseExceptionCodes_StatementError, connection );
		}
	};
	/** Specialization for EFieldType_TEXT
	*/
	template<>
	struct SSqliteParameterValueSetter< EFieldType_TEXT >
			: public SSqliteParameterValueSetterBase
	{
private:
		//!@copydoc SSqliteParameterValueSetterBase::DoSetValue
		virtual void DoSetValue( SQLite::Statement * statement, const void * value, CDatabaseStatementParameterSqlite * parameter, SQLite::Database * connection )
		{
			char const * val = static_cast< const char * >( value );
			SQLiteTry( SQLite::BindText( statement, parameter->GetIndex(), val, parameter->GetObjectValue().GetPtrSize(), SQLite::NULL_DESTRUCTOR ), StringStream() << STR( "Parameter set value: " ) << val, EDatabaseExceptionCodes_StatementError, connection );
		}
	};
	/** Specialization for EFieldType_NVARCHAR
	*/
	template<>
	struct SSqliteParameterValueSetter< EFieldType_NVARCHAR >
			: public SSqliteParameterValueSetterBase
	{
private:
		//!@copydoc SSqliteParameterValueSetterBase::DoSetValue
		virtual void DoSetValue( SQLite::Statement * statement, const void * value, CDatabaseStatementParameterSqlite * parameter, SQLite::Database * connection )
		{
			SQLiteTry( SQLite::BindText16( statement, parameter->GetIndex(), value, parameter->GetObjectValue().GetPtrSize(), SQLite::NULL_DESTRUCTOR ), StringStream() << STR( "Parameter set value: " ) << value, EDatabaseExceptionCodes_StatementError, connection );
		}
	};
	/** Specialization for EFieldType_NTEXT
	*/
	template<>
	struct SSqliteParameterValueSetter< EFieldType_NTEXT >
			: public SSqliteParameterValueSetterBase
	{
private:
		//!@copydoc SSqliteParameterValueSetterBase::DoSetValue
		virtual void DoSetValue( SQLite::Statement * statement, const void * value, CDatabaseStatementParameterSqlite * parameter, SQLite::Database * connection )
		{
			SQLiteTry( SQLite::BindText16( statement, parameter->GetIndex(), value, parameter->GetObjectValue().GetPtrSize(), SQLite::NULL_DESTRUCTOR ), StringStream() << STR( "Parameter set value: " ) << value, EDatabaseExceptionCodes_StatementError, connection );
		}
	};
	/** Specialization for EFieldType_DATE
	*/
	template<>
	struct SSqliteParameterValueSetter< EFieldType_DATE >
			: public SSqliteParameterValueSetterBase
	{
private:
		//!@copydoc SSqliteParameterValueSetterBase::DoSetValue
		virtual void DoSetValue( SQLite::Statement * statement, const void * value, CDatabaseStatementParameterSqlite * parameter, SQLite::Database * connection )
		{
			char const * val = static_cast< const char * >( value );
			SQLiteTry( SQLite::BindText( statement, parameter->GetIndex(), val, parameter->GetObjectValue().GetPtrSize(), SQLite::NULL_DESTRUCTOR ), StringStream() << STR( "Parameter set value: " ) << val, EDatabaseExceptionCodes_StatementError, connection );
		}
	};
	/** Specialization for EFieldType_DATETIME
	*/
	template<>
	struct SSqliteParameterValueSetter< EFieldType_DATETIME >
			: public SSqliteParameterValueSetterBase
	{
private:
		//!@copydoc SSqliteParameterValueSetterBase::DoSetValue
		virtual void DoSetValue( SQLite::Statement * statement, const void * value, CDatabaseStatementParameterSqlite * parameter, SQLite::Database * connection )
		{
			char const * val = static_cast< const char * >( value );
			SQLiteTry( SQLite::BindText( statement, parameter->GetIndex(), val, parameter->GetObjectValue().GetPtrSize(), SQLite::NULL_DESTRUCTOR ), StringStream() << STR( "Parameter set value: " ) << val, EDatabaseExceptionCodes_StatementError, connection );
		}
	};
	/** Specialization for EFieldType_TIME
	*/
	template<>
	struct SSqliteParameterValueSetter< EFieldType_TIME >
			: public SSqliteParameterValueSetterBase
	{
private:
		//!@copydoc SSqliteParameterValueSetterBase::DoSetValue
		virtual void DoSetValue( SQLite::Statement * statement, const void * value, CDatabaseStatementParameterSqlite * parameter, SQLite::Database * connection )
		{
			char const * val = static_cast< const char * >( value );
			SQLiteTry( SQLite::BindText( statement, parameter->GetIndex(), val, parameter->GetObjectValue().GetPtrSize(), SQLite::NULL_DESTRUCTOR ), StringStream() << STR( "Parameter set value: " ) << val, EDatabaseExceptionCodes_StatementError, connection );
		}
	};
}
END_NAMESPACE_DATABASE_SQLITE

#endif // ___DATABASE_STATEMENTPARAMETER_SETTER_H___
