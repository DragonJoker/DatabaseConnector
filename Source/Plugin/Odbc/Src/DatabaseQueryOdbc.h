/************************************************************************//**
 * @file DatabaseQueryOdbc.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 * 
 *
 * @brief CDatabaseQueryOdbc class declaration.
 *
 * @details Describes a query for ODBC database.
 *
 ***************************************************************************/

#ifndef ___DATABASE_QUERY_ODBC_H___
#define ___DATABASE_QUERY_ODBC_H___

#include "DatabaseOdbcPrerequisites.h"

#include <DatabaseQuery.h>

BEGIN_NAMESPACE_DATABASE_ODBC
{
    /** Describes a statement for ODBC database.
    */
    class DatabaseOdbcExport CDatabaseQueryOdbc
        : public CDatabaseQuery
    {
    
    public:
        /** Constructor.
        @param[in] connection
            Database connection.
        @param[in] query
            Request text.
        */
        CDatabaseQueryOdbc( DatabaseConnectionOdbcPtr connection, const String & query );
        
        /** Destructor.
         */
        virtual ~CDatabaseQueryOdbc();
        
        /** Initialize this statement.
        @return
            Error code.
        */
        virtual EErrorType Initialize();
        
        /** Execute this statement.
        @param[out] result
            Error code.
        @return
            The result.
        */
        virtual bool ExecuteUpdate( EErrorType * result = NULL );
        
        /** Execute this statement.
        @param[out] result
            Error code.
        @return
            The result.
        */
        virtual DatabaseResultPtr ExecuteSelect( EErrorType * result = NULL );
        
        /** Clean statement.
        */
        virtual void Cleanup();
        
        /** Create a parameter.
        @param[in] name
            Parameter name.
        @param[in] fieldType
            Date type.
        @param[in] parameterType
            Parameter type.
        @return
            Created parameter.
        */
        virtual DatabaseParameterPtr CreateParameter( const String & name, EFieldType fieldType, EParameterType parameterType );
        
        /** Create a parameter which has limits (strings, etc.).
        @param[in] name
            Parameter name.
        @param[in] fieldType
            Date type.
        @param[in] limits
            Size limits.
        @param[in] parameterType
            Parameter type.
        @return
            Created parameter.
        */
        virtual DatabaseParameterPtr CreateParameter( const String & name, EFieldType fieldType, uint32_t limits, EParameterType parameterType );
        
    private:
        /** Bind a parameter.
        @param parameter
            The parameter to bind.
        @return
            Error code.
        */
        EErrorType DoBindParameter( DatabaseQueryParameterOdbcPtr parameter );
        
        /** Put field data (for variable-sized data).
        @param parameter
            The parameter to use.
        @return
            Error code.
        */
        EErrorType DoPutParameterData( DatabaseQueryParameterOdbcPtr parameter );
        
        /** Get field data (for variable-sized data).
        @param statementHandle
            The statement handle
        @param parameter
            The parameter to use.
        @return
            Error code.
        */
        EErrorType DoGetParameterData( HSTMT statementHandle, DatabaseQueryParameterOdbcPtr parameter );
        
        /** Pre-execute operations.
        @param statementHandle
            The statement handle
        @return
            Error code.
        */
        EErrorType DoPreExecute();
        
        /** Execute this statement.
        @param[out] result
            Error code.
        @return
            The result (data for a SELECT or empty (but not null) for other operations).
        */
        virtual DatabaseResultPtr DoExecute( EErrorType * result = NULL );
        
        /** Function called when the result set is fully fetched (to retrieve out parameters values)
        @param statementHandle
            The statement fetched
        @info
            The execution result
        */
        void OnResultSetFullyFetched( HSTMT statementHandle, SQLRETURN info );
        
    private:
        /// Size or indicator.
        SQLLEN _strLenIndPtr;
        /// Database connection.
        DatabaseConnectionOdbcPtr _connectionOdbc;
        /// The statement for the currently executing query
        SQLHSTMT _statementHandle;
    };
    
}
END_NAMESPACE_DATABASE_ODBC

#endif // ___DATABASE_STATEMENT_ODBC_H___
