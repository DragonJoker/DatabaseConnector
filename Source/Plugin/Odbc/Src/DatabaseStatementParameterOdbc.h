/************************************************************************//**
* @file DatabaseStatementParameterOdbc.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDatabaseStatementParameterOdbc class declaration.
*
* @details Describes a statement parameter for ODBC database.
*
***************************************************************************/

#ifndef ___DATABASE_STATEMENT_PARAMETER_ODBC_H___
#define ___DATABASE_STATEMENT_PARAMETER_ODBC_H___

#include "DatabaseOdbcPrerequisites.h"
#include "DatabaseOdbcParameterBinding.h"

#include <DatabaseParameter.h>

BEGIN_NAMESPACE_DATABASE_ODBC
{
	/** Describes a statement parameter for ODBC database.
	*/
	class CDatabaseStatementParameterOdbc
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
		CDatabaseStatementParameterOdbc( DatabaseConnectionOdbcSPtr connection, DatabaseValuedObjectInfosSPtr infos, unsigned short index, EParameterType parameterType, std::unique_ptr< SValueUpdater > updater );

		/** Destructor.
		*/
		virtual ~CDatabaseStatementParameterOdbc();

		/** Initialises parameter members from the given statement handle
		@param statementHandle
			The statement handle
		*/
		void Initialise( SQLHSTMT statementHandle );

		/** Retrieves the parameter binding
		@return
			The binding
		*/
		const COutOdbcBindBase & GetBinding()const
		{
			return *_binding;
		}

		/** Retrieves the parameter binding
		@return
			The binding
		*/
		COutOdbcBindBase & GetBinding()
		{
			return *_binding;
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
			GetBinding().UpdateValue();
		}

		/** Set parameter value
		@param value
			New parameter value.
		*/
		template< typename T >
		inline void DoSetAndUpdateValue( const T * value )
		{
			CDatabaseValuedObject::DoSetValue( value );
			GetBinding().UpdateValue();
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
			GetBinding().UpdateValue();
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
			GetBinding().UpdateValue();
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
		//! The binding to ODBC parameter
		std::unique_ptr< COutOdbcBindBase > _binding;
	};
}
END_NAMESPACE_DATABASE_ODBC

#endif // ___DATABASE_STATEMENT_PARAMETER_ODBC_H___
