project( DatabasePluginSqlite )

set( PROJECT_DESC "DatabaseConnector SQLite plug-in" )
set( ${PROJECT_NAME}_VERSION_MAJOR	0 )
set( ${PROJECT_NAME}_VERSION_MINOR	1 )
set( ${PROJECT_NAME}_VERSION_BUILD	0 )

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/Src 
    ${DATABASE_SOURCE_DIR}/Interface/Src
)

set( DefaultTarget DatabasePluginSqlite )

add_target(
    DatabasePluginSqlite
    plugin
    "Database"
    "Database;${MinLibraries}"
    "DatabaseSqlitePch.h"
    "DatabaseSqlitePch.cpp" "" "" ""
)

set_property( TARGET DatabasePluginSqlite PROPERTY FOLDER "Plugin/SQLite" )
add_target_astyle( DatabasePluginSqlite ".h;.hpp;.inl;.cpp" )
set( Build "yes" PARENT_SCOPE )