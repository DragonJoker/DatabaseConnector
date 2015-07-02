/************************************************************************//**
 * @file DatabaseParameterOdbc.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 *
 *
 * @brief CDatabaseParameterOdbc class declaration.
 *
 * @details Describes the base class for a statement or query parameter for ODBC database.
 *
 ***************************************************************************/

#ifndef ___DATABASE_PARAMETER_ODBC_H___
#define ___DATABASE_PARAMETER_ODBC_H___

#include "DatabaseOdbcPrerequisites.h"

#include <EFieldType.h>
#include <EParameterType.h>

BEGIN_NAMESPACE_DATABASE_ODBC
{

	/** Describes a statement parameter for ODBC database.
	*/
	class CDatabaseParameterOdbc
	{

	public:
		/** Constructor.
		@param[in] index
		    Internal index.
		@param[in] fieldType
		    Field type.
		@param[in] parameterType
		    Parameter type.
		 */
		CDatabaseParameterOdbc( unsigned short index, EFieldType fieldType, EParameterType parameterType );

		/** Destructor.
		 */
		virtual ~CDatabaseParameterOdbc();

		/** Get value type.
		@return
		    Current value type.
		@remarks
		    Returns reference to allow pointer for ODBC functions.
		*/
		const SQLSMALLINT & GetValueType() const;

		/** Get parameter type.
		@return
		    Current parameter type.
		@remarks
		    Passed by reference to allow pointer for ODBC functions.
		*/
		const SQLSMALLINT & GetParameterType() const;

		/** Get input/output type.
		@return
		    Current input/output type.
		@remarks
		    Passed by reference to allow pointer for ODBC functions.
		*/
		const SQLSMALLINT & GetInputOutputType() const;

		/** Get column size.
		@return
		    Current column size.
		@remarks
		    Passed by reference to allow pointer for ODBC functions.
		*/
		const SQLULEN & GetColumnSize() const;

		/** Get decimal number of digits.
		@return
		    Current decimal number of digits.
		@remarks
		    Passed by reference to allow pointer for ODBC functions.
		*/
		const SQLSMALLINT & GetDecimalDigits() const;

		/** Get column index
		@return
		    Current column index.
		@remarks
		    Return reference to allow update by ODBC functions.
		*/
		SQLLEN & GetColumnIndex();

		/** Tells the parameter is nullable
		@return
		    The value.
		*/
		bool IsNullable()const { return _isNullable; }

		/** Set column index
		@param[in] value
		     New column index.
		*/
		void SetColumnIndex( SQLINTEGER value );

		/** Retrieves the date value
		@return
		    The value
		 */
		const SQL_DATE_STRUCT & GetDate()const;

		/** Retrieves the date value
		@return
		    The value
		 */
		SQL_DATE_STRUCT & GetDate();

		/** Retrieves the time value
		@return
		    The value
		 */
		const SQL_TIME_STRUCT & GetTime()const;

		/** Retrieves the time value
		@return
		    The value
		 */
		SQL_TIME_STRUCT & GetTime();

		/** Retrieves the datetime value
		@return
		    The value
		 */
		const SQL_TIMESTAMP_STRUCT & GetDateTime()const;

		/** Retrieves the datetime value
		@return
		    The value
		 */
		SQL_TIMESTAMP_STRUCT & GetDateTime();

	protected:
		/// C data type.
		SQLSMALLINT _valueType;
		/// SQL data type.
		SQLSMALLINT _parameterType;
		/// I/O type (in, inout, out)
		SQLSMALLINT _inputOutputType;
		/// Column size.
		SQLULEN _columnSize;
		/// Decimal number of digits
		SQLSMALLINT _decimalDigits;
		/// Column index
		SQLLEN _columnIndex;
		/// Association between EFieldTypes and C data types.
		static SQLSMALLINT CTypes[EFieldType_COUNT];
		/// Association between EFieldTypes and SQL data types.
		static SQLSMALLINT Types[EFieldType_COUNT];
		/// Association between EParameterType and ODBC data types.
		static SQLSMALLINT InOutTypes[EParameterType_COUNT];
		/// Association between EFieldType and sizes (depending on the field type).
		static SQLULEN Sizes[EFieldType_COUNT];
		//! The date field, if needed
		SQL_DATE_STRUCT _date;
		//! The time field, if needed
		SQL_TIME_STRUCT _time;
		//! The datetime field, if needed
		SQL_TIMESTAMP_STRUCT _dateTime;
		//! Tells the parameter is nullable
		bool _isNullable;
	};
}
END_NAMESPACE_DATABASE_ODBC

#endif // ___DATABASE_PARAMETER_ODBC_H___
