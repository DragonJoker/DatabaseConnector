/************************************************************************//**
 * @file DatabaseResultSqlite.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 * 
 *
 * @brief CDatabaseResultSqlite class declaration.
 *
 * @details Describes a result for SQLite database.
 *
 ***************************************************************************/

#ifndef ___DATABASE_RESULT_SQLITE_H___
#define ___DATABASE_RESULT_SQLITE_H___

#include "DatabaseSqlitePrerequisites.h"

#include <DatabaseResult.h>

BEGIN_NAMESPACE_DATABASE_SQLITE
{
    /** Describes a result for SQLite database.
    */
    class CDatabaseResultSqlite
        : public CDatabaseResult
    {
    public:
        /** Constructor.
        @param[in] connection
            Database connection.
        @param[in] statement
            The SQLite statement.
        */
        DatabaseSqliteExport CDatabaseResultSqlite( DatabaseConnectionPtr connection, std::shared_ptr< SQLite::Statement > statement );
        
        /** Destructor.
         */
        DatabaseSqliteExport virtual ~CDatabaseResultSqlite();
        
        /** Initialize the result set.
        */
        void Initialize();
        
        /** Get the first row of the result set.
        @return
            The first row or NULL if no result.
        */
        DatabaseSqliteExport virtual DatabaseRowPtr GetFirstRow();
        
        /** Get the next row of the result set.
        @return
            The next row or NULL if no more results.
        */
        DatabaseSqliteExport virtual DatabaseRowPtr GetNextRow();
        
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
        
        //! The SQLite statement.
        std::shared_ptr< SQLite::Statement > _statement;
        //! The row that receives the results
        DatabaseRowPtr _row;
    };
}
END_NAMESPACE_DATABASE_SQLITE

#endif // ___DATABASE_RESULT_SQLITE_H___
