/************************************************************************//**
* @file DatabaseConnectionOdbcMsSql.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 3/14/2014 5:03:15 PM
* 
*
* @brief CConnectionOdbcMsSql class definition.
*
* @details Describes a connection to a database via an ODBC driver.
*
***************************************************************************/

#include "DatabaseOdbcMsSqlPch.h"

#include "DatabaseConnectionOdbcMsSql.h"

#include <DatabaseOdbcHelper.h>
#include <DatabaseStatementOdbc.h>
#include <DatabaseQueryOdbc.h>

BEGIN_NAMESPACE_DATABASE_ODBC_MSSQL
{
    CDatabaseConnectionOdbcMsSql::CDatabaseConnectionOdbcMsSql( SQLHENV sqlEnvironmentHandle, const String & server, const String & database, const String & userName, const String & password, String & connectionString )
        :   CDatabaseConnectionOdbc( sqlEnvironmentHandle, server, database, userName, password, connectionString )
    {
        DoConnect( connectionString );
    }
    
    CDatabaseConnectionOdbcMsSql::~CDatabaseConnectionOdbcMsSql()
    {
        DoDisconnect();
    }
    
    EErrorType CDatabaseConnectionOdbcMsSql::DoConnect( String & connectionString )
    {
        EErrorType  eReturn     = EErrorType_ERROR;
        SQLHWND     sqlHwnd     = NULL;
        SQLRETURN   sqlReturn;
        
        if ( _userName.size() > 0 )
        {
            connectionString = STR( "DSN=" ) + _database
            + STR( ";UID=" ) + _userName
            + STR( ";PWD=" ) + _password;
        }
        else
        {
            connectionString = STR( "DSN=" ) + _database
                               + STR( ";INTEGRATED SECURITY=true" )
                               + STR( ";Trusted_Connection=yes" );
        }
        
        if ( SqlSuccess( SQLAllocHandle( SQL_HANDLE_DBC, _environmentHandle, &_connectionHandle ), SQL_HANDLE_ENV, _environmentHandle, STR( "SQLAllocHandle" ) ) == EErrorType_NONE )
        {
            sqlReturn = SQLDriverConnect( _connectionHandle, sqlHwnd, ( SqlChar * )connectionString.c_str(), SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE_REQUIRED );
            
            if ( SqlSuccess( sqlReturn, SQL_HANDLE_DBC, _connectionHandle, STR( "SQLDriverConnect" ) ) == EErrorType_NONE )
            {
                DoSetConnected( true );
            }
        }
        
        return eReturn;
    }
    
    DatabaseStatementPtr CDatabaseConnectionOdbcMsSql::DoCreateStatement( const String & request, EErrorType * result )
    {
        DatabaseStatementPtr pReturn;
        EErrorType eResult = EErrorType_ERROR;
        
        if ( IsConnected() )
        {
            pReturn = std::make_shared< CDatabaseStatementOdbc >( shared_from_this(), request );
            eResult = EErrorType_NONE;
        }
        
        if ( result )
        {
            *result = eResult;
        }
        
        return pReturn;
    }
    
    DatabaseQueryPtr CDatabaseConnectionOdbcMsSql::DoCreateQuery( const String & request, EErrorType * result )
    {
        DatabaseQueryPtr l_pReturn;
        EErrorType eResult = EErrorType_ERROR;
        
        if ( IsConnected() )
        {
            l_pReturn = std::make_shared< CDatabaseQueryOdbc >( shared_from_this(), request );
            eResult = EErrorType_NONE;
        }
        
        if ( result )
        {
            *result = eResult;
        }
        
        return l_pReturn;
    }
}
END_NAMESPACE_DATABASE_ODBC_MSSQL
