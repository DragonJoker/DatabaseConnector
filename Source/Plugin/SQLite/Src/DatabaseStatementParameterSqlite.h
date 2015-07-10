/************************************************************************//**
 * @file DatabaseStatementParameterSqlite.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 *
 *
 * @brief CDatabaseStatementParameterSqlite class declaration.
 *
 * @details Describes a statement parameter for SQLite database.
 *
 ***************************************************************************/

#ifndef ___DATABASE_STATEMENT_PARAMETER_SQLITE_H___
#define ___DATABASE_STATEMENT_PARAMETER_SQLITE_H___

#include "DatabaseSqlitePrerequisites.h"

#include <DatabaseParameter.h>
#include "DatabaseParameterSqlite.h"

BEGIN_NAMESPACE_DATABASE_SQLITE
{
	/** Used to stream a byte array into an std::istream
	*/
	struct membuf
			: std::streambuf
	{
		/** Consctructor
		*/
		membuf( char * begin, char * end )
		{
			this->setg( begin, begin, end );
		}
	};
	/** Describes a statement parameter for SQLite database.
	*/
	class CDatabaseStatementParameterSqlite
		: public CDatabaseParameter
		, public CDatabaseParameterSqlite
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
		DatabaseSqliteExport CDatabaseStatementParameterSqlite( DatabaseConnectionSqlitePtr connection, const String & name, unsigned short index, EFieldType fieldType, EParameterType parameterType, SValueUpdater * updater );

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
		DatabaseSqliteExport CDatabaseStatementParameterSqlite( DatabaseConnectionSqlitePtr connection, const String & name, unsigned short index, EFieldType fieldType, uint32_t limits, EParameterType parameterType, SValueUpdater * updater );

		/** Destructor.
		 */
		DatabaseSqliteExport virtual ~CDatabaseStatementParameterSqlite();

		//!@copydoc Database::CDatabaseParameter::SetNull
		virtual void SetNull();

		//!@copydoc Database::CDatabaseParameter::SetValue
		void SetValue( DatabaseParameterPtr value );

		/** Defines the prepared statement
		@param statement
		    The statement
		*/
		void SetStatement( SQLite::Statement * statement )
		{
			_statement = statement;
		}

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

		/** Initializes the parameter value setter
		@remarks
		    Called from constructor, takes account of the fact that CDatabaseStatementParameter constructor throws an exception
		*/
		void DoInitializeParamSetter();

		//! The parameter value setter
		SSqliteParameterValueSetterBase * _paramSetter;
		//! The prepared statement
		SQLite::Statement * _statement;
	};
}
END_NAMESPACE_DATABASE_SQLITE

#endif // ___DATABASE_STATEMENT_PARAMETER_SQLITE_H___
