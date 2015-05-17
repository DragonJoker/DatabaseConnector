/************************************************************************//**
 * @file DatabaseResultMySql.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 *
 *
 * @brief CDatabaseResultMySql class declaration.
 *
 * @details Describes a result for MYSQL database.
 *
 ***************************************************************************/

#ifndef ___DATABASE_RESULT_MYSQL_H___
#define ___DATABASE_RESULT_MYSQL_H___

#include "DatabaseMySqlPrerequisites.h"

#include <DatabaseResult.h>

#include <cppconn/statement.h>

BEGIN_NAMESPACE_DATABASE_MYSQL
{
	/** Describes a result for MYSQL database.
	*/
	class CDatabaseResultMySql
		: public CDatabaseResult
	{
	public:
		/** Constructor.
		@param[in] connection
		    Database connection.
		@param[in] statement
		    The MySQL statement.
		*/
		DatabaseMySqlExport CDatabaseResultMySql( DatabaseConnectionPtr connection, std::shared_ptr< sql::Statement > statement );

		/** Destructor.
		 */
		DatabaseMySqlExport virtual ~CDatabaseResultMySql();

		/** Initialize the result set.
		@param[in] result
		    The MySQL result set.
		*/
		void Initialize( std::shared_ptr< sql::ResultSet > result );

		/** Get the first row of the result set.
		@return
		    The first row or NULL if no result.
		*/
		DatabaseMySqlExport virtual DatabaseRowPtr GetFirstRow();

		/** Get the next row of the result set.
		@return
		    The next row or NULL if no more results.
		*/
		DatabaseMySqlExport virtual DatabaseRowPtr GetNextRow();

	private:
		/** Get the current row fields values.
		*/
		void DoGetRow();

		/** Get the row.
		@return
		    Row.
		*/
		DatabaseRowPtr DoFillRow();

		/** Initializes a field with the given index
		@remarks
		    Defines its value.
		    Index goes into [1,ColumnCount]
		@return
		    The created and filled field
		*/
		DatabaseFieldPtr DoSetFieldValue( uint32_t index );

		//! The MySQL result set.
		std::shared_ptr< sql::ResultSet > _result;
		//! The MySQL statement.
		std::shared_ptr< sql::Statement > _statement;
		//! The row that receives the results
		DatabaseRowPtr _row;
	};
}
END_NAMESPACE_DATABASE_MYSQL

#endif // ___DATABASE_RESULT_MYSQL_H___
