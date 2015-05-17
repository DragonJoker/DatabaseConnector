/************************************************************************//**
 * @file DatabaseRow.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/24/2014 8:37:01 AM
 *
 *
 * @brief CDatabaseRow class declaration.
 *
 * @details Describes a row of the result set.
 *
 ***************************************************************************/

#ifndef ___DATABASE_ROW_H___
#define ___DATABASE_ROW_H___

#include "DatabasePrerequisites.h"

#include "DatabaseField.h"

BEGIN_NAMESPACE_DATABASE
{
	/** Describes a row of the result set.
	*/
	class DatabaseExport CDatabaseRow
	{

	public:
		/** Constructor.
		@param[in] connection
		    Database connection.
		*/
		CDatabaseRow( DatabaseConnectionPtr connection );

		/** Destructor.
		 */
		virtual ~CDatabaseRow();

		/** Add a field to the row.
		@param[in] field
		    New field.
		 */
		void AddField( DatabaseFieldPtr field );

		/** Check field existence.
		@param[in] name
		    Name of the field to check.
		@return
		    true if found.
		 */
		bool HasField( const String & name );

		/** Get field by name.
		@param[in] name
		    Field name.
		@return
		    Field.
		*/
		DatabaseFieldPtr GetField( const String & name );

		/** Get field by index.
		@param[in] index
		    Field index.
		@return
		    Field.
		*/
		DatabaseFieldPtr GetField( uint32_t index );

		/** Get value as string of a field.
		@param[in] index
		    Field index.
		@return
		    Field value as string.
		*/
		String GetStr( uint32_t index );

		/** Get value as string of a field.
		@param[in] name
		    Field name.
		@return
		    Field value as string.
		*/
		String GetStr( const String & name );

		/** Get value
		@param[in] index
		    Field index.
		@return
		    Value.
		*/
		template <typename T> inline T Get( uint32_t index );

		/** Get value
		@param[in] name
		    Field name.
		@return
		    Value.
		*/
		template <typename T> inline T Get( const String & name );

		/** Get value
		@param[in] index
		    Field index.
		@param[out] value
		    Value.
		*/
		template <typename T> inline void Get( uint32_t index, T & value );

		/** Get value
		@param[in] name
		    Field name.
		@param[out] value
		    Value.
		*/
		template <typename T> inline void Get( const String & name, T & value );

		/** Get value
		@param[in] index
		    Field index.
		@return
		    Value.
		*/
		template <typename T> inline boost::optional< T > GetOpt( uint32_t index );

		/** Get value
		@param[in] name
		    Field name.
		@return
		    Value.
		*/
		template <typename T> inline boost::optional< T > GetOpt( const String & name );

		/** Get value
		@param[in] index
		    Field index.
		@param[out] value
		    Value.
		*/
		template <typename T> inline void GetOpt( uint32_t index, boost::optional< T > & value );

		/** Get value
		@param[in] name
		    Field name.
		@param[out] value
		    Value.
		*/
		template <typename T> inline void GetOpt( const String & name, boost::optional< T > & value );

		/** Get default value.
		@return
		    Default value.
		*/
		template <typename T> inline T GetDefault();

	protected:
		DatabaseFieldPtrArray   _arrayFields;       ///< Array of fields.
		DatabaseConnectionPtr   _connection;        ///< Database connection.

	}; // class CDatabaseRow

}
END_NAMESPACE_DATABASE

#include "DatabaseRow.inl"

#endif // ___DATABASE_ROW_H___
