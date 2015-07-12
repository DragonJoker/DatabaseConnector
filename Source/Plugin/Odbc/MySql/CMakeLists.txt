project( DatabasePluginOdbcMySql )

set( DBODBCMYSQL_VERSION_MAJOR	0 )
set( DBODBCMYSQL_VERSION_MINOR	1 )
set( DBODBCMYSQL_VERSION_BUILD	0 )

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/Src 
    ${DATABASE_SOURCE_DIR}/Interface/Src
    ${DATABASE_SOURCE_DIR}/Plugin/Odbc/Src
    ${ODBC_INCLUDE_DIR}
)

add_target(
    DatabasePluginOdbcMySql
    plugin
    "Database"
    "Database;DatabasePluginOdbc;${ODBC_LIBRARIES_DYNAMIC};${MinLibraries}"
    "DatabaseOdbcMySqlPch.h"
    "DatabaseOdbcMySqlPch.cpp"
    "" "" ""
)

set_property( TARGET DatabasePluginOdbcMySql PROPERTY FOLDER "Plugin/Odbc/MySql" )
add_target_astyle( DatabasePluginOdbcMySql ".h;.hpp;.inl;.cpp" )