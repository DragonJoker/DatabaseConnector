/************************************************************************//**
* @file DatabaseConnectionOdbcMySql.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 3/14/2014 5:03:15 PM
* 
*
* @brief CConnectionOdbcMySql class definition.
*
* @details Describes a connection to a database via an ODBC driver.
*
***************************************************************************/

#include "DatabaseOdbcMySqlPch.h"

#include "DatabaseConnectionOdbcMySql.h"

#include <DatabaseOdbcHelper.h>
#include <DatabaseStatementOdbc.h>
#include <DatabaseQueryOdbc.h>

#include <DatabaseStringUtils.h>

/** Return matched rows instead of affected rows
@remarks
     The client cannot handle when MySQL returns the true value of affected rows.
     If this flag is set, MySQL returns "found rows" instead.
     You must have MySQL 3.21.14 or newer for this to work.
*/
#define SQL_MYSQL_OPT_FOUND_ROWS 0x00000002

/** Allow big result set
@remarks
    Do not set any packet limit for results and bind parameters.
    Without this option, parameter binding will be truncated to 255 characters.
*/
#define SQL_MYSQL_OPT_BIG_PACKETS 0x00000008

/** Don't prompt when connecting
@remarks
    Do not prompt for questions even if driver would like to prompt.
*/
#define SQL_MYSQL_OPT_NO_PROMPT 0x00000010

/** Enable Dynamic Cursors
@remarks
    Enable or disable the dynamic cursor support.
*/
#define SQL_MYSQL_OPT_DYNAMIC_CURSOR 0x00000020

/** Ignore schema in column specifications
@remarks
    Ignore use of database name in db_name.tbl_name.col_name.
*/
#define SQL_MYSQL_OPT_NO_SCHEMA 0x00000040

/** Disable driver-provided cursor support
@remarks
    Force use of ODBC manager cursors (experimental).
*/
#define SQL_MYSQL_OPT_NO_DEFAULT_CURSOR 0x00000080

/** Don't use setlocale()
@remarks
    Disable the use of extended fetch (experimental).
*/
#define SQL_MYSQL_OPT_NO_LOCALE 0x00000100

/** Pad CHAR to full length with space
@remarks
    Pad CHAR columns to full column length.
*/
#define SQL_MYSQL_OPT_PAD_SPACE 0x00000200

/** Include table name in SQLDescribeCol()
@remarks
    SQLDescribeCol() returns fully-qualified column names.
*/
#define SQL_MYSQL_OPT_FULL_COLUMN_NAMES 0x00000400

/** Use compression
@remarks
    Use the compressed client/server protocol.
*/
#define SQL_MYSQL_OPT_COMPRESSED_PROTO 0x00000800

/** Ignore space after function names
@remarks
    Tell server to ignore space after function name and before "(" (needed by PowerBuilder).
    This makes all function names keywords.
*/
#define SQL_MYSQL_OPT_IGNORE_SPACE 0x00001000

/** Named Pipe
@remarks
    Connect with named pipes to a mysqld server running on NT.
*/
#define SQL_MYSQL_OPT_NAMED_PIPE 0x00002000

/** Treat BIGINT columns as INT columns
@remarks
    Change BIGINT columns to INT columns (some applications cannot handle BIGINT).
*/
#define SQL_MYSQL_OPT_NO_BIGINT 0x00004000

/** Disable catalog support
@remarks
    Forces results from the catalog functions, such as SQLTables, to always return NULL and the driver to report that catalogs are not supported.
*/
#define SQL_MYSQL_OPT_NO_CATALOG 0x00008000

/** Read options from my.cnf
@remarks
    Read parameters from the [client] and [odbc] groups from my.cnf.
*/
#define SQL_MYSQL_OPT_USE_MYCNF 0x00010000

/** Enable safe options
@remarks
    Add some extra safety checks.
*/
#define SQL_MYSQL_OPT_SAFE 0x00020000

/** Disable transaction support
@remarks
    Disable transactions.
*/
#define SQL_MYSQL_OPT_NO_TRANSACTIONS 0x00040000

/** Log queries to %TEMP%/myodbc.sql
@remarks
    Enable query logging to c:/myodbc.sql(/tmp/myodbc.sql) file.
    (Enabled only in debug mode.)
*/
#define SQL_MYSQL_OPT_LOG_QUERY 0x00080000

/** Don't cache results of forward-only cursors
@remarks
    Do not cache the results locally in the driver, instead read from server (mysql_use_result()).
    This works only for forward-only cursors.
    This option is very important in dealing with large tables when you do not want the driver to cache the entire result set.
*/
#define SQL_MYSQL_OPT_NO_CACHE 0x00100000

/** Force use of forward-only cursors
@remarks
    Force the use of Forward-only cursor type.
    In cases of applications setting the default static/dynamic cursor type and one wants the driver to use noncache result sets, this option ensures the forward-only cursor behavior.
*/
#define SQL_MYSQL_OPT_FORWARD_CURSOR 0x00200000

/** Enable automatic reconnect
@remarks
    Enables auto-reconnection functionality.
    Do not use this option with transactions, since an auto-reconnection during a incomplete transaction may cause corruption.
    Note that an auto-reconnected connection will not inherit the same settings and environment as the original connection.
    Added in 3.51.13.
*/
#define SQL_MYSQL_OPT_AUTO_RECONNECT 0x00400000

/** Enable SQL_AUTO_IS_NULL
@remarks
     When AUTO_IS_NULL is set, the driver does not change the default value of sql_auto_is_null, leaving it at 1, so you get the MySQL default, not the SQL standard behavior.
     When AUTO_IS_NULL is not set, the driver changes the default value of SQL_AUTO_IS_NULL to 0 after connecting, so you get the SQL standard, not the MySQL default behavior.
     Thus, omitting the flag disables the compatibility option and forces SQL standard behavior.
     Added in 3.51.13.
*/
#define SQL_MYSQL_OPT_AUTO_IS_NULL 0x00800000

/** Return SQL_NULL_DATA for zero date
@remarks
    Translates zero dates (XXXX-00-00) into the minimum date values supported by ODBC, XXXX-01-01.
    This resolves an issue where some statements will not work because the date returned and the minimum ODBC date value are incompatible.
    Added in 3.51.17.
*/
#define SQL_MYSQL_OPT_ZERO_DATE_TO_MIN 0x01000000

/** Bind minimal date as zero date
@remarks
    Translates the minimum ODBC date value (XXXX-01-01) to the zero date format supported by MySQL (XXXX-00-00).
    This resolves an issue where some statements will not work because the date returned and the minimum ODBC date value are incompatible.
    Added in 3.51.17.
*/
#define SQL_MYSQL_OPT_MIN_DATE_TO_ZERO 0x02000000

/** Allow multiple statements
@remarks
    Enables support for batched statements.
    Added in 3.51.18.
*/
#define SQL_MYSQL_OPT_MULTI_STATEMENTS 0x04000000

/** Limit column size to signed 32-bit range
@remarks
    Limits the column size to a signed 32-bit value to prevent problems with larger column sizes in applications that do not support them.
    This option is automatically enabled when working with ADO applications.
    Added in 3.51.22.
*/
#define SQL_MYSQL_OPT_COLUMN_SIZE_S32 0x08000000

/** Always handle binary function results as character data
@remarks
    When set, this option disables charset 63 for columns with an empty org_table.
    Added in 3.51.26.
*/
#define SQL_MYSQL_OPT_NO_BINARY_RESULT 0x10000000

/** [This option is not on the GUI dialog box]
@remarks
    Causes BIGINT parameters to be bound as strings.
    Microsoft Access treats BIGINT as a string on linked tables.
    The value is read correctly, but bound as a string.
    This option is used automatically if the driver is used by Microsoft Access.
    Added in 5.1.3.
*/
#define SQL_MYSQL_OPT_DFLT_BIGINT_BIND_STR 0x20000000

/** Don't use INFORMATION_SCHEMA for metadata
@remarks
    Tells catalog functions not to use INFORMATION_SCHEMA, but rather use legacy algorithms.
    The trade-off here is usually speed for information quality.
    Using INFORMATION_SCHEMA is often slow, but the information obtained is more complete.
    Added in 5.1.7.
*/
#define SQL_MYSQL_OPT_NO_INFORMATION_SCHEMA 0x40000000

BEGIN_NAMESPACE_DATABASE_ODBC_MYSQL
{
    CDatabaseConnectionOdbcMySql::CDatabaseConnectionOdbcMySql( SQLHENV sqlEnvironmentHandle, const String & server, const String & database, const String & userName, const String & password, String & connectionString )
        :   CDatabaseConnectionOdbc( sqlEnvironmentHandle, server, database, userName, password, connectionString )
    {
        DoConnect( connectionString );
    }
    
    CDatabaseConnectionOdbcMySql::~CDatabaseConnectionOdbcMySql()
    {
        DoDisconnect();
    }
    
    EErrorType CDatabaseConnectionOdbcMySql::DoConnect( String & connectionString )
    {
        EErrorType  eReturn     = EErrorType_ERROR;
        SQLHWND     sqlHwnd     = NULL;
        SQLRETURN   sqlReturn;
        
        if ( _userName.size() > 0 )
        {
            uint32_t flags = SQL_MYSQL_OPT_NO_CACHE | SQL_MYSQL_OPT_FORWARD_CURSOR | SQL_MYSQL_OPT_BIG_PACKETS | SQL_MYSQL_OPT_MULTI_STATEMENTS | SQL_MYSQL_OPT_NO_DEFAULT_CURSOR;
            //            connectionString = STR( "Driver={MySQL ODBC 5.3 UNICODE Driver};Server=" ) + _server + STR( ";Database=" ) + _database + STR( ";User=" ) + _userName + STR( ";Password=" ) + _password + STR( ";Option=" ) + CStrUtils::ToString( SQL_MYSQL_OPT_NO_CACHE | SQL_MYSQL_OPT_FORWARD_CURSOR | SQL_MYSQL_OPT_BIG_PACKETS | SQL_MYSQL_OPT_MULTI_STATEMENTS | SQL_MYSQL_OPT_NO_DEFAULT_CURSOR ) + STR( ";" );
            connectionString = STR( "DSN=" ) + _database
            + STR( ";UID=" ) + _userName
            + STR( ";PWD=" ) + _password
            + STR( ";Option=" ) + CStrUtils::ToString( flags )
            + STR( ";CharSet=utf8;" );
        }
        else
        {
            connectionString = STR( "DSN=" ) + _database + STR( ";INTEGRATED SECURITY=true;Trusted_Connection=yes" );
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
    
    DatabaseStatementPtr CDatabaseConnectionOdbcMySql::DoCreateStatement( const String & request, EErrorType * result )
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
    
    DatabaseQueryPtr CDatabaseConnectionOdbcMySql::DoCreateQuery( const String & request, EErrorType * result )
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
END_NAMESPACE_DATABASE_ODBC_MYSQL
