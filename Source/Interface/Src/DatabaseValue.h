/************************************************************************//**
 * @file DatabaseValue.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 *
 *
 * @brief CDatabaseValueBase class declaration.
 *
 * @details Describes the base class of inherited templated classes dealing with database fields.
 *
 ***************************************************************************/

#ifndef ___DATABASE_FIELD_VALUE_H___
#define ___DATABASE_FIELD_VALUE_H___

#include "DatabasePrerequisites.h"

#include "Database.h"
#include "DatabaseConnection.h"
#include "DatabaseDate.h"
#include "DatabaseDateTime.h"
#include "DatabaseTime.h"
#include "EFieldType.h"

BEGIN_NAMESPACE_DATABASE
{
	/** Describes the base class of inherited templated classes dealing with database fields.
	*/
	class DatabaseExport CDatabaseValueBase
	{

	public:
		/** Constructor.
		@param connection
		    The database connection
		*/
		CDatabaseValueBase( DatabaseConnectionPtr connection );

		/** Destructor.
		*/
		virtual ~CDatabaseValueBase();

		/** Set value as string.
		@param[in] value
		    Field value as string.
		*/
		void SetStrValue( const String & value );

		/** Get value as string.
		@return
		    Field value as string.
		*/
		const String & GetStrValue();

		/** Defines this value from the given one
		@param value
		    The value
		@param limits
		    The field size limit
		*/
		virtual void SetValue( CDatabaseValueBase * value ) = 0;

		/** Get parameter value as a string.
		@param[out] result
		    Parameter value as a string.
		*/
		virtual void GetInsertValue( String & result ) = 0;

		/** Get a pointer to the value.
		@return
		    Pointer to the value or NULL.
		*/
		virtual void * GetPtrValue() = 0;

		/** Re-initialize internal values.
		*/
		virtual void Reset() = 0;

		/** Get the value pointer size
		@return
		    The size.
		*/
		const long & GetPtrSize();

		/** Check if value is NULL.
		@return
		    true if value is NULL.
		*/
		bool IsNull() const;

		/** Set parameter value to NULL.
		*/
		void SetNull( bool null = true );

	protected:
		/** Update value as string from the typed value.
		*/
		virtual void DoUpdateStrValue() = 0;

		/** Update typed value from the value as string.
		*/
		virtual void DoUpdateTValue() = 0;

		/** Set parameter value to NULL.
		*/
		virtual void DoSetNull() = 0;

		/** Re-initialize internal values: indeterminate states.
		 */
		void DoReset();

	protected:
		//! Value as string.
		String _value;
		//! Value size
		long _valueSize;
		/// Value is NULL or not.
		bool _isNull;
		//! Internal state.
		bool _isValueAsStringSet;
		//! Internal state.
		bool _isValueSet;
		//! The database connection
		DatabaseConnectionPtr _connection;
	};
}
END_NAMESPACE_DATABASE

#include "DatabaseValue.inl"

#endif // ___DATABASE_FIELD_VALUE_H___
