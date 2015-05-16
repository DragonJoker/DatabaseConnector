/************************************************************************//**
 * @file DatabaseOdbcHelper.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/18/2014 2:47:39 PM
 * 
 *
 * @brief Helper functions for ODBC.
 *
 * @details Helper functions for ODBC.
 *
 ***************************************************************************/

#include "DatabaseOdbcPch.h"

#include "DatabaseOdbcHelper.h"

#include "DatabaseResultOdbc.h"
#include "ExceptionDatabaseOdbc.h"

#include <DatabaseField.h>
#include <DatabaseResult.h>
#include <DatabaseRow.h>
#include <DatabaseException.h>

#include <DatabaseStringUtils.h>

BEGIN_NAMESPACE_DATABASE_ODBC
{
    static const String ODBC_DRIVER_ERROR = STR( "ODBC Driver error" );
    static const String ODBC_QUERY_ERROR = STR( "Error encountered while executing query: " );
    static const String ODBC_UNKNOWN_ERROR = STR( "Unknown error encountered while executing query: " );
    static const String ODBC_INCONSISTENCY_ERROR = STR( "Number of columns is less than retrieved data." );
    static const String ODBC_QUERY_SUCCESS = STR( "Success executing action: " );
    
    static const SQLINTEGER   ODBC_RERUN_TRANSACTION_CODE = 1205;
    
    static const String ODBC_STATUS_MSG = STR( "Status: " );
    static const String ODBC_NATIVE_ERROR_MSG = STR( "Native error: " );
    static const String ODBC_REQUEST_MSG = STR( "Request: " );
    
    static const String ODBC_NumResultCols_MSG = STR( "SQLNumResultCols: " );
    static const String ODBC_ColAttributeLabel_MSG = STR( "SQLColAttribute - SQL_DESC_LABEL: " );
    static const String ODBC_ColAttributeDescType_MSG = STR( "SQLColAttribute - SQL_DESC_TYPE: " );
    static const String ODBC_ColAttributeTypeName_MSG = STR( "SQLColAttribute - SQL_TYPE_NAME: " );
    static const String ODBC_Fetch_MSG = STR( "SQLFetch: " );
    static const String ODBC_MoreResults_MSG = STR( "SQLMoreResults" );
    static const String ODBC_RowCount_MSG = STR( "SQLRowCount" );
    static const String ODBC_FreeStmt_MSG = STR( "SQLFreeStmt" );
    
    static const String ODBC_EXECUTE_REQUEST_MSG = STR( "Execute request: " );
    
    static const int COLUMN_BUFFER_SIZE = 1024 * 1024;
    
    namespace
    {
        EErrorType InitializeColumns( DatabaseResultPtr results, std::vector< SDataBinding > & columns, SQLHSTMT statementHandle )
        {
            static const SQLSMALLINT BUFFER_SIZE = 255;
            int attemptCount;
            EErrorType errorType = EErrorType_NONE;
            String name;
            std::vector< SDataBinding >::iterator it = columns.begin();
            uint32_t index = 0;
            SQLSMALLINT numResults;
            SQLLEN numeric;
            TCHAR buffer[BUFFER_SIZE];
            
            for ( SQLSMALLINT i = 1 ; i <= SQLSMALLINT( columns.size() ) ; i++ )
            {
                SqlTry( SQLColAttribute( statementHandle, i, SQL_DESC_LABEL, SQLPOINTER( buffer ), BUFFER_SIZE, &numResults, &numeric ), SQL_HANDLE_STMT, statementHandle, ODBC_ColAttributeLabel_MSG );
                
                if ( errorType == EErrorType_NONE )
                {
                    name = buffer;
                    numResults = 0;
                    memset( buffer, 0, sizeof( buffer ) );
                    SqlTry( SQLColAttribute( statementHandle, i, SQL_DESC_CONCISE_TYPE, SQLPOINTER( buffer ), BUFFER_SIZE, &numResults, &numeric ), SQL_HANDLE_STMT, statementHandle, ODBC_ColAttributeDescType_MSG );
                    
                    if ( errorType == EErrorType_NONE )
                    {
                        if ( numeric == SQL_NTS )
                        {
                            SqlTry( SQLColAttribute( statementHandle, i, SQL_DESC_TYPE_NAME, SQLPOINTER( buffer ), BUFFER_SIZE, &numResults, &numeric ), SQL_HANDLE_STMT, statementHandle, ODBC_ColAttributeTypeName_MSG );
                            
                            if ( errorType == EErrorType_NONE )
                            {
                                results->AddField( name, buffer );
                                memset( buffer, 0, sizeof( buffer ) );
                            }
                        }
                        else
                        {
                            results->AddField( name, GetFieldConciseType( numeric ) );
                        }
                        
                        // allocate memory for the binding, and set it up
                        it->TargetType = SQL_C_WCHAR;
                        it->BufferLength = COLUMN_BUFFER_SIZE + 1;
                        it->TargetValuePtr = new unsigned char[it->BufferLength];
                        it->StrLen_or_Ind = 0;
                        SqlTry( SQLBindCol( statementHandle, ( SQLUSMALLINT )++index, it->TargetType, it->TargetValuePtr, it->BufferLength, &( it->StrLen_or_Ind ) ), SQL_HANDLE_STMT, statementHandle, STR( "SQLBindCol\n" ) );
                        ++it;
                    }
                }
            }
            
            return errorType;
        }
        
        EErrorType FetchResultSet( DatabaseConnectionPtr connection, DatabaseResultPtr results, std::vector< SDataBinding > & columns, SQLHSTMT statementHandle )
        {
            int attemptCount;
            EErrorType errorType = EErrorType_NONE;
            SQLRETURN res;
            SQLLEN rowCount = 0;
            
            SqlTry( SQLRowCount( statementHandle, &rowCount ), SQL_HANDLE_STMT, statementHandle, ODBC_RowCount_MSG );
            
            if ( errorType == EErrorType_NONE && rowCount )
            {
                try
                {
                    res = SQLFetch( statementHandle );
                    
                    if ( res != SQL_NO_DATA && res != SQL_SUCCESS )
                    {
                        SqlTry( res, SQL_HANDLE_STMT, statementHandle, ODBC_Fetch_MSG );
                    }
                    else if ( res == SQL_NO_DATA )
                    {
                        CLogger::LogDebug( STR( "Void result" ) );
                    }
                    
                    std::wstring fieldValue;
                    bool isNull = false;
                    uint32_t index;
                    
                    while ( errorType == EErrorType_NONE && res != SQL_NO_DATA )
                    {
                        DatabaseRowPtr row = std::make_shared< CDatabaseRow >( connection );
                        DatabaseFieldPtr field;
                        index = 0;
                        
                        for ( std::vector< SDataBinding >::iterator it = columns.begin(); it != columns.end(); ++it )
                        {
                            if ( it->StrLen_or_Ind == SQL_NULL_DATA )
                            {
                                isNull = true;
                                fieldValue.clear();
                            }
                            else
                            {
                                assert( it->StrLen_or_Ind < COLUMN_BUFFER_SIZE );
                                isNull = false;
                                
                                if ( it->StrLen_or_Ind >= 0 && it->TargetValuePtr )
                                {
                                    fieldValue = ( wchar_t * )it->TargetValuePtr;
                                }
                                else
                                {
                                    fieldValue.clear();
                                }
                            }
                            
                            try
                            {
                                field = std::make_shared< CDatabaseField >( results->GetFieldInfos( index++ ), isNull, CStrUtils::ToString( fieldValue ) );
                            }
                            catch ( const CExceptionDatabase & e )
                            {
                                StringStream message;
                                message << ODBC_DRIVER_ERROR << std::endl
                                        << e.what();
                                CLogger::LogError( message );
                                throw CExceptionDatabaseOdbc( EDatabaseOdbcExceptionCodes_GenericError, message.str(), __FUNCTION__, __FILE__, __LINE__ );
                            }
                            
                            row->AddField( field );
                        }
                        
                        results->AddRow( row );
                        
                        res = SQLFetch( statementHandle );
                        
                        if ( res != SQL_NO_DATA && res != SQL_SUCCESS )
                        {
                            SqlTry( res, SQL_HANDLE_STMT, statementHandle, ODBC_Fetch_MSG );
                        }
                    }
                    
                    // free memory from the binding
                    SqlTry( SQLFreeStmt( statementHandle, SQL_UNBIND ), SQL_HANDLE_STMT, statementHandle, ODBC_FreeStmt_MSG );
                    for ( std::vector< SDataBinding >::iterator it = columns.begin(); it != columns.end(); ++it )
                    {
                        delete [] it->TargetValuePtr;
                    }
                }
                catch ( const std::exception & e )
                {
                    StringStream message;
                    message << ODBC_DRIVER_ERROR << std::endl
                            << e.what();
                    CLogger::LogError( message );
                    throw CExceptionDatabaseOdbc( EDatabaseOdbcExceptionCodes_GenericError, message.str(), __FUNCTION__, __FILE__, __LINE__ );
                }
                catch ( ... )
                {
                    StringStream message;
                    message << ODBC_DRIVER_ERROR << std::endl
                            << ODBC_UNKNOWN_ERROR;
                    CLogger::LogError( message );
                    throw CExceptionDatabaseOdbc( EDatabaseOdbcExceptionCodes_UnknownError, message.str(), __FUNCTION__, __FILE__, __LINE__ );
                }
            }
            
            return errorType;
        }
    }
    
    EErrorType SqlError( SQLSMALLINT typeHandle, SQLHANDLE handle, const String & query, bool error )
    {
        EErrorType errorType = EErrorType_ERROR;
        SqlChar sqlState[10];
        SqlChar sqlMessage[1024 + 1];
        sqlMessage[1024] = 0;
        
        SQLSMALLINT sqlMessageSize = 0;
        SQLINTEGER nativeError = 0;
        SQLSMALLINT errorCount = 1;
        SQLRETURN sqlReturn;
        
        while ( ( sqlReturn = SQLGetDiagRec( typeHandle, handle, errorCount, sqlState, &nativeError, sqlMessage, 1024, &sqlMessageSize ) ) == SQL_SUCCESS )
        {
            errorCount++;
            String message = ( TCHAR * )sqlMessage;
            String state = ( TCHAR * )sqlState;
            StringStream logMessage;
            logMessage << ODBC_STATUS_MSG << state
                       << STR( ", " )
                       << ODBC_NATIVE_ERROR_MSG << nativeError
                       << STR( ", " ) << sqlMessage
                       << STR( ", " )
                       << ODBC_REQUEST_MSG << query;
                       
            if ( nativeError == ODBC_RERUN_TRANSACTION_CODE )
            {
                errorType = EErrorType_RETRY;
                CLogger::LogWarning( logMessage.str() );
            }
            else
            {
                if ( error )
                {
                    CLogger::LogError( logMessage.str() );
                }
                else
                {
                    CLogger::LogWarning( logMessage.str() );
                }
            }
        }
        
        if ( ( errorCount == 2 && errorType != EErrorType_RETRY )
                || ( errorCount > 2 && errorCount == EErrorType_RETRY ) )
        {
            StringStream logMessage;
            logMessage << ODBC_QUERY_ERROR
                       << query;
            if ( error )
            {
                CLogger::LogError( logMessage.str() );
            }
            else
            {
                CLogger::LogWarning( logMessage.str() );
            }
        }
        
        return errorType;
    }
    
    EErrorType SqlSuccess( SQLRETURN rc, SQLSMALLINT typeHandle, SQLHANDLE handle, const String & query )
    {
        EErrorType errorType = EErrorType_ERROR;
        
        if ( rc == SQL_SUCCESS )
        {
            errorType = EErrorType_NONE;
            
            StringStream logMessage;
            logMessage << ODBC_QUERY_SUCCESS
                       << query;
            CLogger::LogDebug( logMessage.str() );
        }
        else if ( rc == SQL_SUCCESS_WITH_INFO )
        {
            SqlError( typeHandle, handle, query, false );
            errorType = EErrorType_NONE;
        }
        else if ( typeHandle && handle )
        {
            errorType = SqlError( typeHandle, handle, query, true );
        }
        
        return errorType;
    }
    
    EFieldType GetFieldConciseType( SQLLEN sqlType )
    {
        EFieldType fieldType = EFieldType_NULL;
        
        switch ( sqlType )
        {
            case SQL_CHAR:
            case SQL_VARCHAR:
                fieldType = EFieldType_VARCHAR;
                break;
                
            case SQL_WCHAR:
            case SQL_WVARCHAR:
                fieldType = EFieldType_NVARCHAR;
                break;
                
            case SQL_LONGVARCHAR:
                fieldType = EFieldType_TEXT;
                break;
                
            case SQL_WLONGVARCHAR:
                fieldType = EFieldType_NTEXT;
                break;
                
            case SQL_FLOAT:
            case SQL_REAL:
                fieldType = EFieldType_FLOAT;
                break;
                
            case SQL_DECIMAL:
            case SQL_DOUBLE:
                fieldType = EFieldType_DOUBLE;
                break;
                
            case SQL_INTEGER:
                fieldType = EFieldType_INTEGER;
                break;
                
            case SQL_TINYINT:
            case SQL_SMALLINT:
                fieldType = EFieldType_SMALL_INTEGER;
                break;
                
            case SQL_BIGINT:
                fieldType = EFieldType_LONG_INTEGER;
                break;
                
            case SQL_BIT:
                fieldType = EFieldType_BOOL;
                break;
                
            case SQL_BINARY:
                fieldType = EFieldType_BINARY;
                break;
                
            case SQL_VARBINARY:
                fieldType = EFieldType_VARBINARY;
                break;
                
            case SQL_LONGVARBINARY:
                fieldType = EFieldType_LONG_VARBINARY;
                break;
                
            case SQL_TYPE_DATE:
            case SQL_INTERVAL_MONTH:
            case SQL_INTERVAL_YEAR:
            case SQL_INTERVAL_YEAR_TO_MONTH:
            case SQL_INTERVAL_DAY:
                fieldType = EFieldType_DATE;
                break;
                
            case SQL_TYPE_TIME:
            case SQL_INTERVAL_HOUR:
            case SQL_INTERVAL_MINUTE:
            case SQL_INTERVAL_SECOND:
            case SQL_INTERVAL_HOUR_TO_MINUTE:
            case SQL_INTERVAL_HOUR_TO_SECOND:
                fieldType = EFieldType_TIME;
                break;
                
            case SQL_TYPE_TIMESTAMP:
            case SQL_INTERVAL_DAY_TO_HOUR:
            case SQL_INTERVAL_DAY_TO_MINUTE:
            case SQL_INTERVAL_DAY_TO_SECOND:
                fieldType = EFieldType_DATETIME;
                break;
            default:
                fieldType = EFieldType_NULL;
                break;
        }
        
        return fieldType;
    }
    
    SQLRETURN SqlExecute( DatabaseConnectionPtr connection, SQLHSTMT statementHandle, const String & query, DatabaseResultPtr & pReturn, EErrorType & result )
    {
        CLogger::LogMessage( ODBC_EXECUTE_REQUEST_MSG + query );
        
        pReturn = std::make_shared< CDatabaseResult >( connection );
        
        SQLRETURN res = SQL_SUCCESS;
        int attemptCount;
        EErrorType errorType;
        SQLSMALLINT numColumns;
        
        do
        {
            SqlTry( SQLNumResultCols( statementHandle, &numColumns ), SQL_HANDLE_STMT, statementHandle, ODBC_NumResultCols_MSG );
            
            if ( errorType == EErrorType_NONE )
            {
                if ( numColumns )
                {
                    std::vector< SDataBinding > arrayColumnData( numColumns );
                    InitializeColumns( pReturn, arrayColumnData, statementHandle );
                    FetchResultSet( connection, pReturn, arrayColumnData, statementHandle );
                }
                
                res = SQLMoreResults( statementHandle );
                SqlTry( res, SQL_HANDLE_STMT, statementHandle, ODBC_MoreResults_MSG );
                
                if ( res != SQL_NO_DATA && errorType == EErrorType_NONE )
                {
                    CLogger::LogMessage( STR( "Additional result detected" ) );
                }
            }
        }
        while ( errorType == EErrorType_NONE && res != SQL_NO_DATA && res != SQL_PARAM_DATA_AVAILABLE );
        
        result = errorType;
        return res;
    }
    
}
END_NAMESPACE_DATABASE_ODBC
