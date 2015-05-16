/************************************************************************//**
 * @file ExceptionDatabaseOdbc.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/18/2014 2:47:39 PM
 * 
 *
 * @brief CExceptionDatabaseOdbc class declaration and definition.
 *
 * @details Should be thrown when a problem occured in the ODBC database plugin.
 *
 ***************************************************************************/

#ifndef ___EXCEPTION_DATABASE_ODBC_H___
#define ___EXCEPTION_DATABASE_ODBC_H___

#include "DatabaseOdbcPrerequisites.h"

#include <DatabaseException.h>

BEGIN_NAMESPACE_DATABASE_ODBC
{
    /** Static definitions of ODBC database error codes.
    */
    enum EDatabaseOdbcExceptionCodes
    {
        EDatabaseOdbcExceptionCodes_GenericError = 0,
        EDatabaseOdbcExceptionCodes_UnknownError,
        EDatabaseOdbcExceptionCodes_StatementError,
        EDatabaseOdbcExceptionCodes_QueryError,
        
        EDatabaseOdbcExceptionCodes_LastCode ///< Represent the maximum number of exception code. Must be always the last.
    };
    
    /** Should be thrown when a problem occured in the ODBC database plugin.
    */
    class DatabaseOdbcExport CExceptionDatabaseOdbc
        : public CExceptionDatabase
    {
    public:
        /** Create a exception for the ODBC database.
        @param number
            Error code.
        @param description
            Error description.
        @param source
            Error source function.
        @param file
            Source file name.
        @param line
            Source file line number.
        */
        CExceptionDatabaseOdbc( int number, const String & description, const std::string & source, const std::string & file, long line )
            : CExceptionDatabase( number, description, source, STR( "CExceptionDatabaseOdbc" ) /*typename*/, file, line )
        {
            // Empty
        }
    };
    
}
END_NAMESPACE_DATABASE_ODBC

#endif // ___EXCEPTION_DATABASE_ODBC_H___
