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

#include "DatabaseSqlitePrerequisites.h"

#include "DatabaseStatementParameterSqlite.h"

#include <DatabaseStringUtils.h>
#include <DatabaseLogger.h>

BEGIN_NAMESPACE_DATABASE_SQLITE
{
	static const String ERROR_SQLITE_PARAMETER_VALUE = STR( "Can't set parameter value" );

	/** Base setter class
	*/
	struct SParameterValueSetterBase
	{
public:
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
		void operator()( SQLite::Statement * statement, const void * value, CDatabaseStatementParameterSqlite * parameter )
		{
			SQLiteTry( DoSetValue( statement, value, parameter ), STR( "Parameter set value" ) );
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
		virtual SQLite::eCODE DoSetValue( SQLite::Statement * statement, const void * value, CDatabaseStatementParameterSqlite * parameter ) = 0;
	};
	/** Generic template class to set the parameter value
	*/
	template< EFieldType Type >
	struct SParameterValueSetter
			: public SParameterValueSetterBase
	{
private:
		//!@copydoc SParameterValueSetterBase::DoSetValue
		virtual SQLite::eCODE DoSetValue( SQLite::Statement * statement, const void * value, CDatabaseStatementParameterSqlite * parameter )
		{
			throw std::runtime_error( "SParameterValueSetter::DoSetValue not implemented for this data type" );
		}
	};
	/** Specialization for EFieldType_BOOL
	*/
	template<>
	struct SParameterValueSetter< EFieldType_BOOL >
			: public SParameterValueSetterBase
	{
private:
		//!@copydoc SParameterValueSetterBase::DoSetValue
		virtual SQLite::eCODE DoSetValue( SQLite::Statement * statement, const void * value, CDatabaseStatementParameterSqlite * parameter )
		{
			return SQLite::BindInt( statement, parameter->GetIndex(), ( *static_cast< const bool * >( value ) ) ? 1 : 0 );
		}
	};
	/** Specialization for EFieldType_SMALL_INTEGER
	*/
	template<>
	struct SParameterValueSetter< EFieldType_SMALL_INTEGER >
			: public SParameterValueSetterBase
	{
private:
		//!@copydoc SParameterValueSetterBase::DoSetValue
		virtual SQLite::eCODE DoSetValue( SQLite::Statement * statement, const void * value, CDatabaseStatementParameterSqlite * parameter )
		{
			return SQLite::BindInt( statement, parameter->GetIndex(), *static_cast< const int16_t * >( value ) );
		}
	};
	/** Specialization for EFieldType_INTEGER
	*/
	template<>
	struct SParameterValueSetter< EFieldType_INTEGER >
			: public SParameterValueSetterBase
	{
private:
		//!@copydoc SParameterValueSetterBase::DoSetValue
		virtual SQLite::eCODE DoSetValue( SQLite::Statement * statement, const void * value, CDatabaseStatementParameterSqlite * parameter )
		{
			return SQLite::BindInt( statement, parameter->GetIndex(), *static_cast< const int32_t * >( value ) );
		}
	};
	/** Specialization for EFieldType_LONG_INTEGER
	*/
	template<>
	struct SParameterValueSetter< EFieldType_LONG_INTEGER >
			: public SParameterValueSetterBase
	{
private:
		//!@copydoc SParameterValueSetterBase::DoSetValue
		virtual SQLite::eCODE DoSetValue( SQLite::Statement * statement, const void * value, CDatabaseStatementParameterSqlite * parameter )
		{
			return SQLite::BindInt64( statement, parameter->GetIndex(), *static_cast< const SQLite::Int64 * >( value ) );
		}
	};
	/** Specialization for EFieldType_FLOAT
	*/
	template<>
	struct SParameterValueSetter< EFieldType_FLOAT >
			: public SParameterValueSetterBase
	{
private:
		//!@copydoc SParameterValueSetterBase::DoSetValue
		virtual SQLite::eCODE DoSetValue( SQLite::Statement * statement, const void * value, CDatabaseStatementParameterSqlite * parameter )
		{
			return SQLite::BindDouble( statement, parameter->GetIndex(), *static_cast< const float * >( value ) );
		}
	};
	/** Specialization for EFieldType_DOUBLE
	*/
	template<>
	struct SParameterValueSetter< EFieldType_DOUBLE >
			: public SParameterValueSetterBase
	{
private:
		//!@copydoc SParameterValueSetterBase::DoSetValue
		virtual SQLite::eCODE DoSetValue( SQLite::Statement * statement, const void * value, CDatabaseStatementParameterSqlite * parameter )
		{
			return SQLite::BindDouble( statement, parameter->GetIndex(), *static_cast< const double * >( value ) );
		}
	};
	/** Specialization for EFieldType_VARCHAR
	*/
	template<>
	struct SParameterValueSetter< EFieldType_VARCHAR >
			: public SParameterValueSetterBase
	{
private:
		//!@copydoc SParameterValueSetterBase::DoSetValue
		virtual SQLite::eCODE DoSetValue( SQLite::Statement * statement, const void * value, CDatabaseStatementParameterSqlite * parameter )
		{
			return SQLite::BindText( statement, parameter->GetIndex(), static_cast< const char * >( value ), parameter->GetObjectValue().GetPtrSize(), SQLite::NULL_DESTRUCTOR );
		}
	};
	/** Specialization for EFieldType_TEXT
	*/
	template<>
	struct SParameterValueSetter< EFieldType_TEXT >
			: public SParameterValueSetterBase
	{
private:
		//!@copydoc SParameterValueSetterBase::DoSetValue
		virtual SQLite::eCODE DoSetValue( SQLite::Statement * statement, const void * value, CDatabaseStatementParameterSqlite * parameter )
		{
			return SQLite::BindText( statement, parameter->GetIndex(), static_cast< const char * >( value ), parameter->GetObjectValue().GetPtrSize(), SQLite::NULL_DESTRUCTOR );
		}
	};
	/** Specialization for EFieldType_NVARCHAR
	*/
	template<>
	struct SParameterValueSetter< EFieldType_NVARCHAR >
			: public SParameterValueSetterBase
	{
private:
		//!@copydoc SParameterValueSetterBase::DoSetValue
		virtual SQLite::eCODE DoSetValue( SQLite::Statement * statement, const void * value, CDatabaseStatementParameterSqlite * parameter )
		{
			return SQLite::BindText16( statement, parameter->GetIndex(), value, parameter->GetObjectValue().GetPtrSize(), SQLite::NULL_DESTRUCTOR );
		}
	};
	/** Specialization for EFieldType_NTEXT
	*/
	template<>
	struct SParameterValueSetter< EFieldType_NTEXT >
			: public SParameterValueSetterBase
	{
private:
		//!@copydoc SParameterValueSetterBase::DoSetValue
		virtual SQLite::eCODE DoSetValue( SQLite::Statement * statement, const void * value, CDatabaseStatementParameterSqlite * parameter )
		{
			return SQLite::BindText16( statement, parameter->GetIndex(), value, parameter->GetObjectValue().GetPtrSize(), SQLite::NULL_DESTRUCTOR );
		}
	};
	/** Specialization for EFieldType_DATE
	*/
	template<>
	struct SParameterValueSetter< EFieldType_DATE >
			: public SParameterValueSetterBase
	{
private:
		//!@copydoc SParameterValueSetterBase::DoSetValue
		virtual SQLite::eCODE DoSetValue( SQLite::Statement * statement, const void * value, CDatabaseStatementParameterSqlite * parameter )
		{
			return SQLite::BindText( statement, parameter->GetIndex(), static_cast< const char * >( value ), parameter->GetObjectValue().GetPtrSize(), SQLite::NULL_DESTRUCTOR );
		}
	};
	/** Specialization for EFieldType_DATETIME
	*/
	template<>
	struct SParameterValueSetter< EFieldType_DATETIME >
			: public SParameterValueSetterBase
	{
private:
		//!@copydoc SParameterValueSetterBase::DoSetValue
		virtual SQLite::eCODE DoSetValue( SQLite::Statement * statement, const void * value, CDatabaseStatementParameterSqlite * parameter )
		{
			return SQLite::BindText( statement, parameter->GetIndex(), static_cast< const char * >( value ), parameter->GetObjectValue().GetPtrSize(), SQLite::NULL_DESTRUCTOR );
		}
	};
	/** Specialization for EFieldType_TIME
	*/
	template<>
	struct SParameterValueSetter< EFieldType_TIME >
			: public SParameterValueSetterBase
	{
private:
		//!@copydoc SParameterValueSetterBase::DoSetValue
		virtual SQLite::eCODE DoSetValue( SQLite::Statement * statement, const void * value, CDatabaseStatementParameterSqlite * parameter )
		{
			return SQLite::BindText( statement, parameter->GetIndex(), static_cast< const char * >( value ), parameter->GetObjectValue().GetPtrSize(), SQLite::NULL_DESTRUCTOR );
		}
	};
}
END_NAMESPACE_DATABASE_SQLITE

#endif // ___DATABASE_STATEMENTPARAMETER_SETTER_H___
