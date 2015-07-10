/************************************************************************//**
 * @file DatabaseStatementParameterMySql.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 *
 *
 * @brief CDatabaseStatementParameterMySql class declaration.
 *
 * @details Describes a statement parameter for MYSQL database.
 *
 ***************************************************************************/

#ifndef ___DATABASE_STATEMENT_PARAMETER_MYSQL_H___
#define ___DATABASE_STATEMENT_PARAMETER_MYSQL_H___

#include "DatabaseMySqlPrerequisites.h"

#include <DatabaseParameter.h>

#include <mysql.h>

BEGIN_NAMESPACE_DATABASE_MYSQL
{
	/** Describes a statement parameter for MYSQL database.
	*/
	class CDatabaseStatementParameterMySql
		: public CDatabaseParameter
	{

	public:
		/** Constructor.
		@param[in] connection
		    Connection to database.
		@param[in] name
		    Parameter name.
		@param[in] index
		    Internal index.
		@param[in] fieldType
		    Field type.
		@param[in] parameterType
		    Parameter type.
		@param[in] updater
		    The parent updater
		 */
		DatabaseMySqlExport CDatabaseStatementParameterMySql( DatabaseConnectionMySqlPtr connection, const String & name, unsigned short index, EFieldType fieldType, EParameterType parameterType, SValueUpdater * updater );

		/** Constructor.
		@param[in] connection
		    Connection to database.
		@param[in] name
		    Parameter name.
		@param[in] index
		    Internal index.
		@param[in] fieldType
		    Field type.
		@param[in] limits
		    Field limits.
		@param[in] parameterType
		    Parameter type.
		@param[in] updater
		    The parent updater
		 */
		DatabaseMySqlExport CDatabaseStatementParameterMySql( DatabaseConnectionMySqlPtr connection, const String & name, unsigned short index, EFieldType fieldType, uint32_t limits, EParameterType parameterType, SValueUpdater * updater );

		/** Destructor.
		 */
		DatabaseMySqlExport virtual ~CDatabaseStatementParameterMySql();

		//!@copydoc Database::CDatabaseParameter::SetNull
		virtual void SetNull();

		//!@copydoc Database::CDatabaseParameter::SetValue
		void SetValue( DatabaseParameterPtr value );

		/** Set parameter value from a field.
		@param[in] field
		    The field.
		@remarks
		    If field type is different than the value type, the value is ignored.
		*/
		virtual void SetValue( DatabaseFieldPtr field )
		{
			CDatabaseParameter::SetValue( field );
		}

		/** Defines the prepared statement
		@param statement
		    The statement
		*/
		void SetStatement( MYSQL_STMT * statement )
		{
			_statement = statement;
		}

		/** Defines the data binding
		@param bind
		    The binding
		*/
		void SetBinding( MYSQL_BIND * bind );

		/** Retrieves the data binding
		@return
		    The binding
		*/
		MYSQL_BIND * GetBinding()const;

	private:
		//!@copydoc Database::CDatabaseParameter::DoSetValue
		virtual void DoSetValue( const bool & value );

		//!@copydoc Database::CDatabaseParameter::DoSetValue
		virtual void DoSetValue( const int16_t & value );

		//!@copydoc Database::CDatabaseParameter::DoSetValue
		virtual void DoSetValue( const uint16_t & value );

		//!@copydoc Database::CDatabaseParameter::DoSetValue
		virtual void DoSetValue( const int32_t & value );

		//!@copydoc Database::CDatabaseParameter::DoSetValue
		virtual void DoSetValue( const uint32_t & value );

		//!@copydoc Database::CDatabaseParameter::DoSetValue
		virtual void DoSetValue( const int64_t & value );

		//!@copydoc Database::CDatabaseParameter::DoSetValue
		virtual void DoSetValue( const uint64_t & value );

		//!@copydoc Database::CDatabaseParameter::DoSetValue
		virtual void DoSetValue( const float & value );

		//!@copydoc Database::CDatabaseParameter::DoSetValue
		virtual void DoSetValue( const double & value );

		//!@copydoc Database::CDatabaseParameter::DoSetValue
		virtual void DoSetValue( const long double & value );

		//!@copydoc Database::CDatabaseParameter::DoSetValue
		virtual void DoSetValue( const char * value );

		//!@copydoc Database::CDatabaseParameter::DoSetValue
		virtual void DoSetValue( const wchar_t * value );

		//!@copydoc Database::CDatabaseParameter::DoSetValue
		virtual void DoSetValue( const std::string & value );

		//!@copydoc Database::CDatabaseParameter::DoSetValue
		virtual void DoSetValue( const std::wstring & value );

		//!@copydoc Database::CDatabaseParameter::DoSetValue
		virtual void DoSetValue( const std::vector< uint8_t > & value );

		//!@copydoc Database::CDatabaseParameter::DoSetValue
		virtual void DoSetValue( const CDateTime & value );

		//!@copydoc Database::CDatabaseParameter::DoSetValue
		virtual void DoSetValue( const CDate & value );

		//!@copydoc Database::CDatabaseParameter::DoSetValue
		virtual void DoSetValue( const CTime & value );
		
		//!@copydoc Database::CDatabaseParameter::DoSetValueFast
		virtual void DoSetValueFast( const bool & value );

		//!@copydoc Database::CDatabaseParameter::DoSetValueFast
		virtual void DoSetValueFast( const int16_t & value );

		//!@copydoc Database::CDatabaseParameter::DoSetValueFast
		virtual void DoSetValueFast( const uint16_t & value );

		//!@copydoc Database::CDatabaseParameter::DoSetValueFast
		virtual void DoSetValueFast( const int32_t & value );

		//!@copydoc Database::CDatabaseParameter::DoSetValueFast
		virtual void DoSetValueFast( const uint32_t & value );

		//!@copydoc Database::CDatabaseParameter::DoSetValueFast
		virtual void DoSetValueFast( const int64_t & value );

		//!@copydoc Database::CDatabaseParameter::DoSetValueFast
		virtual void DoSetValueFast( const uint64_t & value );

		//!@copydoc Database::CDatabaseParameter::DoSetValueFast
		virtual void DoSetValueFast( const float & value );

		//!@copydoc Database::CDatabaseParameter::DoSetValueFast
		virtual void DoSetValueFast( const double & value );

		//!@copydoc Database::CDatabaseParameter::DoSetValueFast
		virtual void DoSetValueFast( const long double & value );

		//!@copydoc Database::CDatabaseParameter::DoSetValueFast
		virtual void DoSetValueFast( const char * value );

		//!@copydoc Database::CDatabaseParameter::DoSetValueFast
		virtual void DoSetValueFast( const wchar_t * value );

		//!@copydoc Database::CDatabaseParameter::DoSetValueFast
		virtual void DoSetValueFast( const std::string & value );

		//!@copydoc Database::CDatabaseParameter::DoSetValueFast
		virtual void DoSetValueFast( const std::wstring & value );

		//!@copydoc Database::CDatabaseParameter::DoSetValueFast
		virtual void DoSetValueFast( const std::vector< uint8_t > & value );

		//!@copydoc Database::CDatabaseParameter::DoSetValueFast
		virtual void DoSetValueFast( const CDateTime & value );

		//!@copydoc Database::CDatabaseParameter::DoSetValueFast
		virtual void DoSetValueFast( const CDate & value );

		//!@copydoc Database::CDatabaseParameter::DoSetValueFast
		virtual void DoSetValueFast( const CTime & value );

		//! The value setter
		std::unique_ptr< SMySqlParameterValueSetterBase > _setter;
		//! The data binding
		std::unique_ptr< COutMySqlBindBase > _binding;
		//! The prepared statement
		MYSQL_STMT * _statement;
	};
}
END_NAMESPACE_DATABASE_MYSQL

#endif // ___DATABASE_STATEMENT_PARAMETER_MYSQL_H___
