project( DatabasePluginSqlite )

set( DBSQLITE_VERSION_MAJOR	0 )
set( DBSQLITE_VERSION_MINOR	1 )
set( DBSQLITE_VERSION_BUILD	0 )

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/Src 
    ${DATABASE_SOURCE_DIR}/Interface/Src
)

set( DefaultTarget DatabasePluginSqlite )

add_target(
    DatabasePluginSqlite
    dll
    "Database"
    "Database;${MinLibraries}"
    "DatabaseSqlitePch.h"
    "DatabaseSqlitePch.cpp" "" "" ""
)

set_property( TARGET DatabasePluginSqlite PROPERTY FOLDER "Plugin/SQLite" )
set( Build "yes" PARENT_SCOPE )