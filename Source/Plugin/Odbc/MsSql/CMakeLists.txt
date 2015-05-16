project( DatabasePluginOdbcMsSql )

set( DBODBCMSSQL_VERSION_MAJOR	0 )
set( DBODBCMSSQL_VERSION_MINOR	1 )
set( DBODBCMSSQL_VERSION_BUILD	0 )

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/Src 
    ${DATABASE_SOURCE_DIR}/Interface/Src
    ${DATABASE_SOURCE_DIR}/Plugin/Odbc/Src
    ${ODBC_INCLUDE_DIR}
)

add_target(
    DatabasePluginOdbcMsSql
    dll
    "Database"
    "Database;DatabasePluginOdbc;${ODBC_LIBRARIES_DYNAMIC};${MinLibraries}"
    "DatabaseOdbcMsSqlPch.h"
    "DatabaseOdbcMsSqlPch.cpp"
    "" "" ""
)

set_property( TARGET DatabasePluginOdbcMsSql PROPERTY FOLDER "Plugin/Odbc/MsSql" )