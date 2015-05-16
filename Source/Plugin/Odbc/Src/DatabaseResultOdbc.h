/************************************************************************//**
 * @file DatabaseResultOdbc.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 * 
 *
 * @brief CDatabaseResultOdbc class declaration.
 *
 * @details Describes a result for ODBC database.
 *
 ***************************************************************************/

#ifndef ___DATABASE_RESULT_ODBC_H___
#define ___DATABASE_RESULT_ODBC_H___

#include "DatabaseOdbcPrerequisites.h"

#include <DatabaseResult.h>

#include <boost/signals2.hpp>

BEGIN_NAMESPACE_DATABASE_ODBC
{
    /** Links the database data during the execution of a SELECT request.
    */
    struct DatabaseOdbcExport SDataBinding
    {
        SQLSMALLINT TargetType;         ///< Data type.
        SQLPOINTER  TargetValuePtr;     ///< Pointer to the value.
        SQLINTEGER  BufferLength;       ///< Size of the buffer containing the value.
        SQLLEN      StrLen_or_Ind;      ///< String length or indicator.
    }; // struct SDataBinding
    
    /** Describes a result for ODBC database.
    */
    class DatabaseOdbcExport CDatabaseResultOdbc
        : public CDatabaseResult
    {
        typedef void ( FuncResultSetFullyFetched )( SQLHSTMT statementHandle, SQLRETURN info );
    
    public:
        /** Constructor.
        @param[in] connection
            Database connection.
        */
        CDatabaseResultOdbc( DatabaseConnectionPtr connection );
        
        /** Destructor.
         */
        virtual ~CDatabaseResultOdbc();
        
        /** Initialize the result set.
        @param[in] statementHandle
            Statement handle.
        */
        void Initialize( HSTMT statementHandle );
        
        /** Get number of resulting rows.
        @return
            Number of rows.
        */
        virtual uint32_t GetRowCount();
        
        /** Get the first row of the result set.
        @return
            The first row or NULL if no result.
        */
        virtual DatabaseRowPtr GetFirstRow();
        
        /** Get the next row of the result set.
        @return
            The next row or NULL if no more results.
        */
        virtual DatabaseRowPtr GetNextRow();
        
        /// The signal launched when the result set is fully fetched
        boost::signals2::signal< FuncResultSetFullyFetched > sResultSetFullyFetched;
        
    private:
    
        /** Get the row.
        @param orientation
            Parsing direction.
        @return
            Row.
        */
        DatabaseRowPtr DoGetRow( SQLSMALLINT orientation );
        
    private:
        typedef std::vector< SDataBinding >         DataBindingArray;
        typedef DataBindingArray::iterator          DataBindingArrayIt;
        typedef DataBindingArray::const_iterator    DataBindingArrayConstIt;
        
        DataBindingArray    _arrayColumnData;       ///< List of data bindings.
        HSTMT               _statementHandle;       ///< Statement handle.
        String        _query;                 ///< Request text.
        uint32_t            _rowCount;              ///< Number of rows.
        DatabaseRowPtr      _row;                   ///< The row that receives the results
        
    }; // class CDatabaseResultOdbc
    
}
END_NAMESPACE_DATABASE_ODBC

#endif // ___DATABASE_RESULT_ODBC_H___
