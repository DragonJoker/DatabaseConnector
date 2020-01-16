/************************************************************************//**
* @file DatabaseStatementParameterMySql.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDatabaseParameterMySql class declaration.
*
* @details Describes a statement parameter for MYSQL database.
*
***************************************************************************/

#ifndef ___DATABASE_STATEMENT_PARAMETER_MYSQL_H___
#define ___DATABASE_STATEMENT_PARAMETER_MYSQL_H___

#include "DatabaseMySqlPrerequisites.h"

#include "DatabaseMySqlHelper.h"

#include <DatabaseParameter.h>

#include <mysql/mysql.h>

BEGIN_NAMESPACE_DATABASE_MYSQL
{
	/** Describes a statement parameter for MYSQL database.
	*/
	class CDatabaseParameterMySql
		: public CDatabaseParameter
	{
	public:
		/** Constructor.
		@param[in] connection
			Connection to database.
		@param[in] infos
			Parameter informations.
		@param[in] index
			Internal index.
		@param[in] parameterType
			Parameter type.
		@param[in] updater
			The parent updater
		*/
		CDatabaseParameterMySql( DatabaseConnectionMySqlSPtr connection, DatabaseValuedObjectInfosSPtr infos, unsigned short index, EParameterType parameterType, std::unique_ptr< SValueUpdater > updater );

		/** Destructor.
		*/
		virtual ~CDatabaseParameterMySql();

		//!@copydoc Database::CDatabaseParameter::SetNull
		virtual void SetNull();

		/** Defines the data binding
		@param bind
			The binding
		*/
		void SetBinding( MYSQL_BIND * bind );

		/** Retrieves the data binding
		@return
			The binding
		*/
		MYSQL_BIND * GetBinding()const
		{
			return &_binding->_bind;
		}

		/** Defines the prepared statement
		@param statement
			The statement
		*/
		inline void SetStatement( MYSQL_STMT * statement )
		{
			_statement = statement;
		}

		/** Retrieves the statement
		@return
			The statement
		*/
		inline MYSQL_STMT * GetStatement()const
		{
			return _statement;
		}

	private:
		/** Set parameter value
		@param value
			New parameter value.
		*/
		template< typename T >
		inline void DoSetAndUpdateValue( const T & value )
		{
			CDatabaseValuedObject::DoSetValue( value );
			_binding->UpdateValue();
		}

		/** Set parameter value
		@param value
			New parameter value.
		*/
		template< typename T >
		inline void DoSetAndUpdateValue( const T * value )
		{
			CDatabaseValuedObject::DoSetValue( value );
			_binding->UpdateValue();
		}

		/** Set parameter value
		@remarks
			Don't perform type checks
		@param value
			New parameter value.
		*/
		template< typename T >
		inline void DoSetAndUpdateValueFast( const T & value )
		{
			CDatabaseValuedObject::DoSetValueFast( value );
			_binding->UpdateValue();
		}

		/** Set parameter value
		@remarks
			Don't perform type checks
		@param value
			New parameter value.
		*/
		template< typename T >
		inline void DoSetAndUpdateValueFast( const T * value )
		{
			CDatabaseValuedObject::DoSetValueFast( value );
			_binding->UpdateValue();
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValue
		virtual void DoSetValue( const bool & value )
		{
			DoSetAndUpdateValue( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValue
		virtual void DoSetValue( const int8_t & value )
		{
			DoSetAndUpdateValue( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValue
		virtual void DoSetValue( const uint8_t & value )
		{
			DoSetAndUpdateValue( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValue
		virtual void DoSetValue( const int16_t & value )
		{
			DoSetAndUpdateValue( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValue
		virtual void DoSetValue( const uint16_t & value )
		{
			DoSetAndUpdateValue( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValue
		virtual void DoSetValue( const int24_t & value )
		{
			DoSetAndUpdateValue( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValue
		virtual void DoSetValue( const uint24_t & value )
		{
			DoSetAndUpdateValue( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValue
		virtual void DoSetValue( const int32_t & value )
		{
			DoSetAndUpdateValue( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValue
		virtual void DoSetValue( const uint32_t & value )
		{
			DoSetAndUpdateValue( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValue
		virtual void DoSetValue( const int64_t & value )
		{
			DoSetAndUpdateValue( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValue
		virtual void DoSetValue( const uint64_t & value )
		{
			DoSetAndUpdateValue( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValue
		virtual void DoSetValue( const float & value )
		{
			DoSetAndUpdateValue( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValue
		virtual void DoSetValue( const double & value )
		{
			DoSetAndUpdateValue( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValue
		virtual void DoSetValue( const CFixedPoint & value )
		{
			DoSetAndUpdateValue( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValue
		virtual void DoSetValue( const std::string & value )
		{
			DoSetAndUpdateValue( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValue
		virtual void DoSetValue( const std::wstring & value )
		{
			DoSetAndUpdateValue( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValue
		virtual void DoSetValue( const DateTimeType & value )
		{
			DoSetAndUpdateValue( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValue
		virtual void DoSetValue( const DateType & value )
		{
			DoSetAndUpdateValue( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValue
		virtual void DoSetValue( const TimeType & value )
		{
			DoSetAndUpdateValue( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValue
		virtual void DoSetValue( const ByteArray & value )
		{
			DoSetAndUpdateValue( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValueFast
		virtual void DoSetValueFast( const bool & value )
		{
			DoSetAndUpdateValueFast( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValueFast
		virtual void DoSetValueFast( const int8_t & value )
		{
			DoSetAndUpdateValueFast( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValueFast
		virtual void DoSetValueFast( const uint8_t & value )
		{
			DoSetAndUpdateValueFast( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValueFast
		virtual void DoSetValueFast( const int16_t & value )
		{
			DoSetAndUpdateValueFast( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValueFast
		virtual void DoSetValueFast( const uint16_t & value )
		{
			DoSetAndUpdateValueFast( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValueFast
		virtual void DoSetValueFast( const int24_t & value )
		{
			DoSetAndUpdateValueFast( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValueFast
		virtual void DoSetValueFast( const uint24_t & value )
		{
			DoSetAndUpdateValueFast( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValueFast
		virtual void DoSetValueFast( const int32_t & value )
		{
			DoSetAndUpdateValueFast( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValueFast
		virtual void DoSetValueFast( const uint32_t & value )
		{
			DoSetAndUpdateValueFast( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValueFast
		virtual void DoSetValueFast( const int64_t & value )
		{
			DoSetAndUpdateValueFast( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValueFast
		virtual void DoSetValueFast( const uint64_t & value )
		{
			DoSetAndUpdateValueFast( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValueFast
		virtual void DoSetValueFast( const float & value )
		{
			DoSetAndUpdateValueFast( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValueFast
		virtual void DoSetValueFast( const double & value )
		{
			DoSetAndUpdateValueFast( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValueFast
		virtual void DoSetValueFast( const CFixedPoint & value )
		{
			DoSetAndUpdateValueFast( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValueFast
		virtual void DoSetValueFast( const std::string & value )
		{
			DoSetAndUpdateValueFast( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValueFast
		virtual void DoSetValueFast( const std::wstring & value )
		{
			DoSetAndUpdateValueFast( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValueFast
		virtual void DoSetValueFast( const DateType & value )
		{
			DoSetAndUpdateValueFast( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValueFast
		virtual void DoSetValueFast( const DateTimeType & value )
		{
			DoSetAndUpdateValueFast( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValueFast
		virtual void DoSetValueFast( const TimeType & value )
		{
			DoSetAndUpdateValueFast( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValueFast
		virtual void DoSetValueFast( const ByteArray & value )
		{
			DoSetAndUpdateValueFast( value );
		}

	private:
		//! The data binding
		std::unique_ptr< SOutMySqlBindBase > _binding;
		//! The prepared statement
		MYSQL_STMT * _statement;
	};
}
END_NAMESPACE_DATABASE_MYSQL

#endif // ___DATABASE_STATEMENT_PARAMETER_MYSQL_H___
