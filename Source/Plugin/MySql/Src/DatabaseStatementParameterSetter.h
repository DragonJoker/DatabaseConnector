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

#include <cppconn/prepared_statement.h>
#include <cppconn/exception.h>

BEGIN_NAMESPACE_DATABASE_MYSQL
{
	static const String ERROR_MYSQL_PARAMETER_VALUE = STR( "Can't set parameter value" );

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
		void operator()( sql::PreparedStatement * statement, void * value, CDatabaseStatementParameterMySql * parameter )
		{
			MySQLTry( DoSetValue( statement, value, parameter ), STR( "Parameter set value" ) );
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
		virtual void DoSetValue( sql::PreparedStatement * statement, void * value, CDatabaseStatementParameterMySql * parameter ) = 0;
	};
	/** Generic template class to set the parameter value
	*/
	template< EFieldType Type >
	struct SParameterValueSetter
			: public SParameterValueSetterBase
	{
private:
		//!@copydoc SParameterValueSetterBase::DoSetValue
		virtual void DoSetValue( sql::PreparedStatement * statement, void * value, CDatabaseStatementParameterMySql * parameter )
		{
			throw std::runtime_error( "DoSetValue not implemented for this data type" );
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
		virtual void DoSetValue( sql::PreparedStatement * statement, void * value, CDatabaseStatementParameterMySql * parameter )
		{
			statement->setBoolean( parameter->GetIndex(), *static_cast< bool * >( value ) );
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
		virtual void DoSetValue( sql::PreparedStatement * statement, void * value, CDatabaseStatementParameterMySql * parameter )
		{
			statement->setInt( parameter->GetIndex(), *static_cast< short * >( value ) );
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
		virtual void DoSetValue( sql::PreparedStatement * statement, void * value, CDatabaseStatementParameterMySql * parameter )
		{
			statement->setInt( parameter->GetIndex(), *static_cast< int * >( value ) );
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
		virtual void DoSetValue( sql::PreparedStatement * statement, void * value, CDatabaseStatementParameterMySql * parameter )
		{
			statement->setInt64( parameter->GetIndex(), *static_cast< long long * >( value ) );
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
		virtual void DoSetValue( sql::PreparedStatement * statement, void * value, CDatabaseStatementParameterMySql * parameter )
		{
			statement->setDouble( parameter->GetIndex(), *static_cast< float * >( value ) );
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
		virtual void DoSetValue( sql::PreparedStatement * statement, void * value, CDatabaseStatementParameterMySql * parameter )
		{
			statement->setDouble( parameter->GetIndex(), *static_cast< double * >( value ) );
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
		virtual void DoSetValue( sql::PreparedStatement * statement, void * value, CDatabaseStatementParameterMySql * parameter )
		{
			statement->setString( parameter->GetIndex(), static_cast< char * >( value ) );
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
		virtual void DoSetValue( sql::PreparedStatement * statement, void * value, CDatabaseStatementParameterMySql * parameter )
		{
			statement->setString( parameter->GetIndex(), static_cast< char * >( value ) );
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
		virtual void DoSetValue( sql::PreparedStatement * statement, void * value, CDatabaseStatementParameterMySql * parameter )
		{
			statement->setString( parameter->GetIndex(), CStrUtils::ToStr( static_cast< wchar_t * >( value ) ) );
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
		virtual void DoSetValue( sql::PreparedStatement * statement, void * value, CDatabaseStatementParameterMySql * parameter )
		{
			statement->setString( parameter->GetIndex(), CStrUtils::ToStr( static_cast< wchar_t * >( value ) ) );
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
		virtual void DoSetValue( sql::PreparedStatement * statement, void * value, CDatabaseStatementParameterMySql * parameter )
		{
			statement->setDateTime( parameter->GetIndex(), static_cast< char * >( value ) );
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
		virtual void DoSetValue( sql::PreparedStatement * statement, void * value, CDatabaseStatementParameterMySql * parameter )
		{
			statement->setDateTime( parameter->GetIndex(), static_cast< char * >( value ) );
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
		virtual void DoSetValue( sql::PreparedStatement * statement, void * value, CDatabaseStatementParameterMySql * parameter )
		{
			statement->setDateTime( parameter->GetIndex(), static_cast< char * >( value ) );
		}
	};
	/** Specialization for EFieldType_BINARY
	*/
	template<>
	struct SParameterValueSetter< EFieldType_BINARY >
			: public SParameterValueSetterBase
	{
private:
		//!@copydoc SParameterValueSetterBase::DoSetValue
		virtual void DoSetValue( sql::PreparedStatement * statement, void * value, CDatabaseStatementParameterMySql * parameter )
		{
			statement->setBlob( parameter->GetIndex(), static_cast< std::istream * >( value ) );
		}
	};
	/** Specialization for EFieldType_VARBINARY
	*/
	template<>
	struct SParameterValueSetter< EFieldType_VARBINARY >
			: public SParameterValueSetterBase
	{
private:
		//!@copydoc SParameterValueSetterBase::DoSetValue
		virtual void DoSetValue( sql::PreparedStatement * statement, void * value, CDatabaseStatementParameterMySql * parameter )
		{
			statement->setBlob( parameter->GetIndex(), static_cast< std::istream * >( value ) );
		}
	};
	/** Specialization for EFieldType_LONG_VARBINARY
	*/
	template<>
	struct SParameterValueSetter< EFieldType_LONG_VARBINARY >
			: public SParameterValueSetterBase
	{
private:
		//!@copydoc SParameterValueSetterBase::DoSetValue
		virtual void DoSetValue( sql::PreparedStatement * statement, void * value, CDatabaseStatementParameterMySql * parameter )
		{
			statement->setBlob( parameter->GetIndex(), static_cast< std::istream * >( value ) );
		}
	};
}
END_NAMESPACE_DATABASE_MYSQL

#endif // ___DATABASE_STATEMENTPARAMETER_SETTER_H___
