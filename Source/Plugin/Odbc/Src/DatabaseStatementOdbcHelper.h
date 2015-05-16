/************************************************************************//**
* @file DatabaseStatementOdbcHelper.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 11:56:56 AM
* 
*
* @brief SDatabaseStatementParamBinderDirectOdbc and SDatabaseStatementParamBinderExecOdbc.
*
* @details Useful functors.
*
***************************************************************************/

#ifndef ___DATABASE_STATEMENT_ODBC_HELPER_H___
#define ___DATABASE_STATEMENT_ODBC_HELPER_H___

#include "DatabaseOdbcPrerequisites.h"

#include "DatabaseStatementParameterOdbc.h"
#include "DatabaseQueryParameterOdbc.h"
#include <EErrorType.h>

BEGIN_NAMESPACE_DATABASE_ODBC
{
    static const String DATABASE_BINDER_DIRECT_NULL = STR( "SDatabaseParamBinderDirectOdbc(Null): " );
    static const String DATABASE_BINDER_DIRECT_STRING = STR( "SDatabaseParamBinderDirectOdbc(Void string): " );
    static const String DATABASE_BINDER_DIRECT_OTHER = STR( "SDatabaseParamBinderDirectOdbc(Void other): " );
    static const String DATABASE_BINDER_DIRECT_NOT_NULL = STR( "SDatabaseParamBinderDirectOdbc(Not null): " );
    
    static const String DATABASE_BINDER_EXEC_NULL = STR( "SDatabaseParamBinderExecOdbc(Null); " );
    static const String DATABASE_BINDER_EXEC_STRING = STR( "SDatabaseParamBinderExecOdbc(Void string): " );
    static const String DATABASE_BINDER_EXEC_OTHER = STR( "SDatabaseParamBinderExecOdbc(Void other): " );
    static const String DATABASE_BINDER_EXEC_NOT_NULL = STR( "SDatabaseParamBinderExecOdbc(Not null): " );
    
    /** Functor for binding fixed-sized parameter.
    */
    struct SDatabaseParamBinderDirectOdbc
    {
        /** () overload.
        @param[in] statementHandle
            Statement handle.
        @param[in] parameter
            Parameter.
        @param[in] nullData
            Data type.
        @param[in] message
            Message to display.
        @param[in] query
            Request text.
        @return
            Error code.
        */
        template< typename T >
        inline EErrorType operator()( SQLHSTMT statementHandle, std::shared_ptr< T > parameter, SQLLEN * nullData, const String & message, const String & query )
        {
            EErrorType errorType;
            int attemptCount;
            
            if ( parameter->IsNull() )
            {
                SqlTry( SQLBindParameter( statementHandle, parameter->GetIndex(), parameter->GetInputOutputType(), parameter->GetValueType(), parameter->GetParameterType(), parameter->GetColumnSize(), 0, parameter->GetPtrValue(), parameter->GetPtrSize(), nullData ), SQL_HANDLE_STMT, statementHandle, DATABASE_BINDER_DIRECT_NULL + message );
            }
            else if ( parameter->GetPtrSize() == 0 )
            {
                switch ( parameter->GetType() )
                {
                    case EFieldType_VARCHAR:
                    case EFieldType_TEXT:
                    case EFieldType_NVARCHAR:
                    case EFieldType_NTEXT:
                        SqlTry( SQLBindParameter( statementHandle, parameter->GetIndex(), parameter->GetInputOutputType(), parameter->GetValueType(), parameter->GetParameterType(), parameter->GetColumnSize(), 0, parameter->GetPtrValue(), parameter->GetPtrSize(), &parameter->GetColumnIndex() ), SQL_HANDLE_STMT, statementHandle, DATABASE_BINDER_DIRECT_STRING + message );
                        break;
                    default:
                        SqlTry( SQLBindParameter( statementHandle, parameter->GetIndex(), parameter->GetInputOutputType(), parameter->GetValueType(), parameter->GetParameterType(), parameter->GetColumnSize(), 0, parameter->GetPtrValue(), parameter->GetPtrSize(), nullData ), SQL_HANDLE_STMT, statementHandle, DATABASE_BINDER_DIRECT_OTHER + message );
                        break;
                }
            }
            else
            {
                SqlTry( SQLBindParameter( statementHandle, parameter->GetIndex(), parameter->GetInputOutputType(), parameter->GetValueType(), parameter->GetParameterType(), parameter->GetColumnSize(), 0, parameter->GetPtrValue(), parameter->GetPtrSize(), &parameter->GetColumnIndex() ), SQL_HANDLE_STMT, statementHandle, DATABASE_BINDER_DIRECT_NOT_NULL + message );
            }
            
            return errorType;
        }
        /** () overload.
        @param[in] statementHandle
            Statement handle.
        @param[in] parameter
            Parameter.
        @param[in] date
            Date value.
        @param[in] nullData
            Data type.
        @param[in] message
            Message to display.
        @param[in] query
            Request text.
        @return
            Error code.
        */
        template< typename T >
        inline EErrorType operator()( SQLHSTMT statementHandle, std::shared_ptr< T > parameter, SQL_DATE_STRUCT & date, SQLLEN * nullData, const String & message, const String & query )
        {
            EErrorType errorType;
            int attemptCount;
            
            if ( parameter->IsNull() )
            {
                *nullData = SQL_NULL_DATA;
                SqlTry( SQLBindParameter( statementHandle, parameter->GetIndex(), parameter->GetInputOutputType(), parameter->GetValueType(), parameter->GetParameterType(), parameter->GetColumnSize(), 0, &date, parameter->GetPtrSize(), nullData ), SQL_HANDLE_STMT, statementHandle, DATABASE_BINDER_DIRECT_NULL + message );
            }
            else
            {
                SqlTry( SQLBindParameter( statementHandle, parameter->GetIndex(), parameter->GetInputOutputType(), parameter->GetValueType(), parameter->GetParameterType(), parameter->GetColumnSize(), 0, &date, parameter->GetPtrSize(), nullData ), SQL_HANDLE_STMT, statementHandle, DATABASE_BINDER_DIRECT_OTHER + message );
            }
            
            return errorType;
        }
        /** () overload.
        @param[in] statementHandle
            Statement handle.
        @param[in] parameter
            Parameter.
        @param[in] time
            Time value.
        @param[in] nullData
            Data type.
        @param[in] message
            Message to display.
        @param[in] query
            Request text.
        @return
            Error code.
        */
        template< typename T >
        inline EErrorType operator()( SQLHSTMT statementHandle, std::shared_ptr< T > parameter, SQL_TIME_STRUCT & time, SQLLEN * nullData, const String & message, const String & query )
        {
            EErrorType errorType;
            int attemptCount;
            
            if ( parameter->IsNull() )
            {
                *nullData = SQL_NULL_DATA;
                SqlTry( SQLBindParameter( statementHandle, parameter->GetIndex(), parameter->GetInputOutputType(), parameter->GetValueType(), parameter->GetParameterType(), parameter->GetColumnSize(), 0, &time, parameter->GetPtrSize(), nullData ), SQL_HANDLE_STMT, statementHandle, DATABASE_BINDER_DIRECT_NULL + message );
            }
            else
            {
                SqlTry( SQLBindParameter( statementHandle, parameter->GetIndex(), parameter->GetInputOutputType(), parameter->GetValueType(), parameter->GetParameterType(), parameter->GetColumnSize(), 0, &time, parameter->GetPtrSize(), nullData ), SQL_HANDLE_STMT, statementHandle, DATABASE_BINDER_DIRECT_OTHER + message );
            }
            
            return errorType;
        }
        /** () overload.
        @param[in] statementHandle
            Statement handle.
        @param[in] parameter
            Parameter.
        @param[in] timestamp
            Date and time value.
        @param[in] nullData
            Data type.
        @param[in] message
            Message to display.
        @param[in] query
            Request text.
        @return
            Error code.
        */
        template< typename T >
        inline EErrorType operator()( SQLHSTMT statementHandle, std::shared_ptr< T > parameter, SQL_TIMESTAMP_STRUCT & timestamp, SQLLEN * nullData, const String & message, const String & query )
        {
            EErrorType errorType;
            int attemptCount;
            
            if ( parameter->IsNull() )
            {
                *nullData = SQL_NULL_DATA;
                SqlTry( SQLBindParameter( statementHandle, parameter->GetIndex(), parameter->GetInputOutputType(), parameter->GetValueType(), parameter->GetParameterType(), parameter->GetColumnSize(), 0, &timestamp, parameter->GetPtrSize(), nullData ), SQL_HANDLE_STMT, statementHandle, DATABASE_BINDER_DIRECT_NULL + message );
            }
            else
            {
                SqlTry( SQLBindParameter( statementHandle, parameter->GetIndex(), parameter->GetInputOutputType(), parameter->GetValueType(), parameter->GetParameterType(), parameter->GetColumnSize(), 0, &timestamp, parameter->GetPtrSize(), nullData ), SQL_HANDLE_STMT, statementHandle, DATABASE_BINDER_DIRECT_OTHER + message );
            }
            
            return errorType;
        }
    };
    
    /** Functor for binding variable-sized parameters (strings, etc.).
    */
    struct SDatabaseParamBinderExecOdbc
    {
        /** () overload.
        @param[in] statementHandle
            Statement handle.
        @param[in] parameter
            Parameter.
        @param[in] nullData
            Data type.
        @param[in] message
            Message to display.
        @param[in] query
            Request text.
        @return
            Error code.
        */
        template< typename T >
        inline EErrorType operator()( SQLHSTMT statementHandle, std::shared_ptr< T > parameter, SQLLEN * nullData, const String & message, const String & query )
        {
            EErrorType errorType;
            int attemptCount;
            
            if ( parameter->IsNull() )
            {
                SqlTry( SQLBindParameter( statementHandle, parameter->GetIndex(), parameter->GetInputOutputType(), parameter->GetValueType(), parameter->GetParameterType(), 1, 0, parameter->GetPtrValue(), parameter->GetPtrSize(), nullData ), SQL_HANDLE_STMT, statementHandle, DATABASE_BINDER_EXEC_NULL + message );
            }
            else if ( parameter->GetPtrSize() == 0 )
            {
                switch ( parameter->GetType() )
                {
                    case EFieldType_VARCHAR:
                    case EFieldType_TEXT:
                    case EFieldType_NVARCHAR:
                    case EFieldType_NTEXT:
                        parameter->SetColumnIndex( SQL_NTS );
                        SqlTry( SQLBindParameter( statementHandle, parameter->GetIndex(), parameter->GetInputOutputType(), parameter->GetValueType(), parameter->GetParameterType(), parameter->GetColumnSize(), 0, parameter->GetPtrValue(), parameter->GetPtrSize(), &parameter->GetColumnIndex() ), SQL_HANDLE_STMT, statementHandle, DATABASE_BINDER_EXEC_STRING + message );
                        break;
                    default:
                        SqlTry( SQLBindParameter( statementHandle, parameter->GetIndex(), parameter->GetInputOutputType(), parameter->GetValueType(), parameter->GetParameterType(), 1, 0, parameter->GetPtrValue(), parameter->GetPtrSize(), nullData ), SQL_HANDLE_STMT, statementHandle, DATABASE_BINDER_EXEC_OTHER + message );
                        break;
                }
            }
            else
            {
                parameter->SetColumnIndex( SQL_LEN_DATA_AT_EXEC( parameter->GetPtrSize() ) );
                SqlTry( SQLBindParameter( statementHandle, parameter->GetIndex(), parameter->GetInputOutputType(), parameter->GetValueType(), parameter->GetParameterType(), parameter->GetColumnSize(), 0, parameter->GetPtrValue(), parameter->GetPtrSize(), & parameter->GetColumnIndex() ), SQL_HANDLE_STMT, statementHandle, DATABASE_BINDER_EXEC_NOT_NULL + message );
            }
            
            return errorType;
        }
    };
}
END_NAMESPACE_DATABASE_ODBC

#endif // ___DATABASE_STATEMENT_ODBC_HELPER_H___
