/************************************************************************//**
* @file DatabaseResult.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/24/2014 8:37:01 AM
*
*
* @brief CDatabaseResult class declaration.
*
* @details Describes the result of a SELECT request.
*
***************************************************************************/

#ifndef ___DATABASE_RESULT_H___
#define ___DATABASE_RESULT_H___

#include "DatabasePrerequisites.h"

#include "EFieldType.h"

BEGIN_NAMESPACE_DATABASE
{
	/** Describes the result of a SELECT request.
	*/
	class CDatabaseResult
	{
	public:
		/** Constructor.
		@param[in] connection
			Database connection.
		*/
		DatabaseExport CDatabaseResult( DatabaseConnectionPtr connection );

		/** Constructor.
		@param[in] connection
			Database connection.
		@param[in] arrayFieldInfos
			Array of field informations for the future results.
		*/
		DatabaseExport CDatabaseResult( DatabaseConnectionPtr connection, const DatabaseFieldInfosPtrArray & arrayFieldInfos );

		/** Destructor.
			*/
		DatabaseExport virtual ~CDatabaseResult();

		/** Add a field which information are known.
		@param[in] name
			Field name.
		@param[in] type
			Field type.
		@param[in] limits
			Field limits.
		*/
		DatabaseExport void AddField( const String & name, EFieldType type, uint32_t limits = 0 );

		/** Add a field which information are unknown.
		@param[in] name
			Field name.
		@param[in] type
			Field type.
		@param[in] length
			Field length.
		*/
		DatabaseExport void AddField( const String & name, const String & type, int length = -1 );

		/** Get number of fields in result.
			*/
		DatabaseExport uint32_t GetFieldCount() const;

		/** Get informations of the field at the specified index.
		@param index
			Field index.
		@return
			Field informations.
		*/
		DatabaseExport DatabaseFieldInfosPtr GetFieldInfos( uint32_t index ) const;

		/** Add a new row (result) to the result set.
		@param[in] row
			New row (result).
			*/
		DatabaseExport virtual void AddRow( DatabaseRowPtr row );

		/** Get number of rows (results).
		@return
			Number of results.
			*/
		DatabaseExport virtual uint32_t GetRowCount();

		/** Get first row (result).
		@return
			First row or NULL if no rows (another request than SELECT).
		*/
		DatabaseExport virtual DatabaseRowPtr GetFirstRow();

		/** Get next row (result).
		@return
			Next row or NULL if no more rows (results).
		*/
		DatabaseExport virtual DatabaseRowPtr GetNextRow();

	protected:
		//! List of resulting rows.
		DatabaseRowPtrList _listRows;
		//! Array of field informations.
		DatabaseFieldInfosPtrArray _arrayFieldInfos;
		//! Number of resulting rows.
		uint32_t _rowCount;
		//! Current row iterator.
		DatabaseRowPtrList::iterator _iterator;
		//! Database connection.
		DatabaseConnectionPtr _connection;
	};
}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_RESULT_H___
