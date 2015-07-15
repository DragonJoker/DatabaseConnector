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
	class CDatabaseValueBase
	{
	public:
		/** Constructor.
		@param connection
			The database connection
		*/
		DatabaseExport CDatabaseValueBase( DatabaseConnectionPtr connection );

		/** Destructor.
		*/
		DatabaseExport virtual ~CDatabaseValueBase();

		/** Defines this value from the given one
		@param value
			The value
		@param limits
			The field size limit
		*/
		DatabaseExport virtual void SetValue( CDatabaseValueBase const & value ) = 0;

		/** Get the value.
		@return
			The value.
		*/
		DatabaseExport virtual String GetQueryValue() = 0;

		/** Get a pointer to the value.
		@return
			Pointer to the value or NULL.
		*/
		DatabaseExport virtual void * GetPtrValue() = 0;

		/** Get a pointer to the value.
		@return
			Pointer to the value or NULL.
		*/
		DatabaseExport virtual const void * GetPtrValue() const = 0;

		/** Re-initialize internal values.
		*/
		DatabaseExport virtual void Reset() = 0;

		/** Get the value pointer size
		@return
			The size.
		*/
		DatabaseExport unsigned long & GetPtrSize();

		/** Get the value pointer size
		@return
			The size.
		*/
		DatabaseExport const unsigned long & GetPtrSize()const;

		/** Check if value is NULL.
		@return
			true if value is NULL.
		*/
		DatabaseExport bool IsNull() const;

		/** Set parameter value to NULL.
		*/
		DatabaseExport void SetNull( bool null = true );

	protected:
		/** Set parameter value to NULL.
		*/
		DatabaseExport virtual void DoSetNull() = 0;

	protected:
		//! Value size
		unsigned long _valueSize;
		//! Value is NULL or not.
		bool _isNull;
		//! The database connection
		DatabaseConnectionPtr _connection;
	};
}
END_NAMESPACE_DATABASE

#include "DatabaseValue.inl"

#endif // ___DATABASE_FIELD_VALUE_H___
