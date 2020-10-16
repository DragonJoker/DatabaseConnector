project( DatabasePluginOdbcMsSql )

set( PROJECT_DESC "DatabaseConnector ODBC MSSQL plug-in" )
set( ${PROJECT_NAME}_VERSION_MAJOR	0 )
set( ${PROJECT_NAME}_VERSION_MINOR	1 )
set( ${PROJECT_NAME}_VERSION_BUILD	0 )

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/Src 
    ${DATABASE_SOURCE_DIR}/Interface/Src
    ${DATABASE_SOURCE_DIR}/Plugin/Odbc/Src
    ${ODBC_INCLUDE_DIR}
)

add_target(
    DatabasePluginOdbcMsSql
    plugin
	"${CMAKE_CURRENT_SOURCE_DIR}/Src"
	"${CMAKE_CURRENT_SOURCE_DIR}/Src"
    "Database"
    "Database;DatabasePluginOdbc;${ODBC_LIBRARIES_DYNAMIC};${MinLibraries}"
    "Src/DatabaseOdbcMsSqlPch.h"
    "Src/DatabaseOdbcMsSqlPch.cpp"
    "" "" ""
)

set_property( TARGET DatabasePluginOdbcMsSql PROPERTY FOLDER "Plugin/Odbc/MsSql" )
add_target_astyle( DatabasePluginOdbcMsSql ".h;.hpp;.inl;.cpp" )