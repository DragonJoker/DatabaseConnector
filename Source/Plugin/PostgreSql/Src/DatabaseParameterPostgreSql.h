/************************************************************************//**
* @file DatabaseStatementParameterPostgreSql.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDatabaseParameterPostgreSql class declaration.
*
* @details Describes a statement parameter for MYSQL database.
*
***************************************************************************/

#ifndef ___DATABASE_STATEMENT_PARAMETER_POSTGRESQL_H___
#define ___DATABASE_STATEMENT_PARAMETER_POSTGRESQL_H___

#include "DatabasePostgreSqlPrerequisites.h"

#include "DatabasePostgreSqlHelper.h"

#include <DatabaseParameter.h>

#include <libpq-fe.h>

BEGIN_NAMESPACE_DATABASE_POSTGRESQL
{
	/** Describes a statement parameter for MYSQL database.
	*/
	class CDatabaseParameterPostgreSql
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
		DatabasePostgreSqlExport CDatabaseParameterPostgreSql( DatabaseConnectionPostgreSqlPtr connection, const String & name, unsigned short index, EFieldType fieldType, EParameterType parameterType, std::unique_ptr< SValueUpdater > updater );

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
		DatabasePostgreSqlExport CDatabaseParameterPostgreSql( DatabaseConnectionPostgreSqlPtr connection, const String & name, unsigned short index, EFieldType fieldType, uint32_t limits, EParameterType parameterType, std::unique_ptr< SValueUpdater > updater );

		/** Constructor.
		@param[in] connection
			Connection to database.
		@param[in] name
			Parameter name.
		@param[in] index
			Internal index.
		@param[in] fieldType
			Field type.
		@param[in] precision
			Field precision.
		@param[in] parameterType
			Parameter type.
		@param[in] updater
			The parent updater
		*/
		DatabasePostgreSqlExport CDatabaseParameterPostgreSql( DatabaseConnectionPostgreSqlPtr connection, const String & name, unsigned short index, EFieldType fieldType, const std::pair< uint32_t, uint32_t > & precision, EParameterType parameterType, std::unique_ptr< SValueUpdater > updater );

		/** Destructor.
		*/
		DatabasePostgreSqlExport virtual ~CDatabaseParameterPostgreSql();

		//!@copydoc Database::CDatabaseParameter::SetNull
		DatabasePostgreSqlExport virtual void SetNull();

		/** Defines the data binding
		@param[out] param
			The field that will receive the value
		@param[out] length
			The field that will receive the value length
		@param[out[ format
			The field that will receive the value format (text=0, binary=1)
		*/
		DatabasePostgreSqlExport void SetBinding( PGbind * bind );

		/** Retrieves the data binding
		@return
			The binding
		*/
		PGbind * GetBinding()const
		{
			return &_binding->_bind;
		}

		/** Defines the prepared statement
		@param statement
			The statement
		*/
		inline void SetStatement( std::string const & statement )
		{
			_statement = statement;
		}

		/** Retrieves the statement
		@return
			The statement
		*/
		inline std::string const & GetStatement()const
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
		DatabasePostgreSqlExport virtual void DoSetValue( const bool & value )
		{
			DoSetAndUpdateValue( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValue
		DatabasePostgreSqlExport virtual void DoSetValue( const int8_t & value )
		{
			DoSetAndUpdateValue( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValue
		DatabasePostgreSqlExport virtual void DoSetValue( const uint8_t & value )
		{
			DoSetAndUpdateValue( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValue
		DatabasePostgreSqlExport virtual void DoSetValue( const int16_t & value )
		{
			DoSetAndUpdateValue( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValue
		DatabasePostgreSqlExport virtual void DoSetValue( const uint16_t & value )
		{
			DoSetAndUpdateValue( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValue
		DatabasePostgreSqlExport virtual void DoSetValue( const int24_t & value )
		{
			DoSetAndUpdateValue( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValue
		DatabasePostgreSqlExport virtual void DoSetValue( const uint24_t & value )
		{
			DoSetAndUpdateValue( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValue
		DatabasePostgreSqlExport virtual void DoSetValue( const int32_t & value )
		{
			DoSetAndUpdateValue( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValue
		DatabasePostgreSqlExport virtual void DoSetValue( const uint32_t & value )
		{
			DoSetAndUpdateValue( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValue
		DatabasePostgreSqlExport virtual void DoSetValue( const int64_t & value )
		{
			DoSetAndUpdateValue( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValue
		DatabasePostgreSqlExport virtual void DoSetValue( const uint64_t & value )
		{
			DoSetAndUpdateValue( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValue
		DatabasePostgreSqlExport virtual void DoSetValue( const float & value )
		{
			DoSetAndUpdateValue( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValue
		DatabasePostgreSqlExport virtual void DoSetValue( const double & value )
		{
			DoSetAndUpdateValue( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValue
		DatabasePostgreSqlExport virtual void DoSetValue( const CFixedPoint & value )
		{
			DoSetAndUpdateValue( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValue
		DatabasePostgreSqlExport virtual void DoSetValue( const std::string & value )
		{
			DoSetAndUpdateValue( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValue
		DatabasePostgreSqlExport virtual void DoSetValue( const std::wstring & value )
		{
			DoSetAndUpdateValue( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValue
		DatabasePostgreSqlExport virtual void DoSetValue( const CDateTime & value )
		{
			DoSetAndUpdateValue( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValue
		DatabasePostgreSqlExport virtual void DoSetValue( const CDate & value )
		{
			DoSetAndUpdateValue( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValue
		DatabasePostgreSqlExport virtual void DoSetValue( const CTime & value )
		{
			DoSetAndUpdateValue( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValue
		DatabasePostgreSqlExport virtual void DoSetValue( const ByteArray & value )
		{
			DoSetAndUpdateValue( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValueFast
		DatabasePostgreSqlExport virtual void DoSetValueFast( const bool & value )
		{
			DoSetAndUpdateValueFast( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValueFast
		DatabasePostgreSqlExport virtual void DoSetValueFast( const int8_t & value )
		{
			DoSetAndUpdateValueFast( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValueFast
		DatabasePostgreSqlExport virtual void DoSetValueFast( const uint8_t & value )
		{
			DoSetAndUpdateValueFast( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValueFast
		DatabasePostgreSqlExport virtual void DoSetValueFast( const int16_t & value )
		{
			DoSetAndUpdateValueFast( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValueFast
		DatabasePostgreSqlExport virtual void DoSetValueFast( const uint16_t & value )
		{
			DoSetAndUpdateValueFast( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValueFast
		DatabasePostgreSqlExport virtual void DoSetValueFast( const int24_t & value )
		{
			DoSetAndUpdateValueFast( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValueFast
		DatabasePostgreSqlExport virtual void DoSetValueFast( const uint24_t & value )
		{
			DoSetAndUpdateValueFast( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValueFast
		DatabasePostgreSqlExport virtual void DoSetValueFast( const int32_t & value )
		{
			DoSetAndUpdateValueFast( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValueFast
		DatabasePostgreSqlExport virtual void DoSetValueFast( const uint32_t & value )
		{
			DoSetAndUpdateValueFast( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValueFast
		DatabasePostgreSqlExport virtual void DoSetValueFast( const int64_t & value )
		{
			DoSetAndUpdateValueFast( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValueFast
		DatabasePostgreSqlExport virtual void DoSetValueFast( const uint64_t & value )
		{
			DoSetAndUpdateValueFast( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValueFast
		DatabasePostgreSqlExport virtual void DoSetValueFast( const float & value )
		{
			DoSetAndUpdateValueFast( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValueFast
		DatabasePostgreSqlExport virtual void DoSetValueFast( const double & value )
		{
			DoSetAndUpdateValueFast( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValueFast
		DatabasePostgreSqlExport virtual void DoSetValueFast( const CFixedPoint & value )
		{
			DoSetAndUpdateValueFast( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValueFast
		DatabasePostgreSqlExport virtual void DoSetValueFast( const std::string & value )
		{
			DoSetAndUpdateValueFast( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValueFast
		DatabasePostgreSqlExport virtual void DoSetValueFast( const std::wstring & value )
		{
			DoSetAndUpdateValueFast( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValueFast
		DatabasePostgreSqlExport virtual void DoSetValueFast( const CDate & value )
		{
			DoSetAndUpdateValueFast( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValueFast
		DatabasePostgreSqlExport virtual void DoSetValueFast( const CDateTime & value )
		{
			DoSetAndUpdateValueFast( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValueFast
		DatabasePostgreSqlExport virtual void DoSetValueFast( const CTime & value )
		{
			DoSetAndUpdateValueFast( value );
		}

		//!@copydoc Database::CDatabaseValuedObject::DoSetValueFast
		DatabasePostgreSqlExport virtual void DoSetValueFast( const ByteArray & value )
		{
			DoSetAndUpdateValueFast( value );
		}

	private:
		//! The data binding
		std::unique_ptr< SOutPostgreSqlBindBase > _binding;
		//! The prepared statement
		std::string _statement;
	};
}
END_NAMESPACE_DATABASE_POSTGRESQL

#endif // ___DATABASE_STATEMENT_PARAMETER_POSTGRESQL_H___
