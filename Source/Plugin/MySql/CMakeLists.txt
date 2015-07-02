project( DatabasePluginMySql )

set( DBMYSQL_VERSION_MAJOR	0 )
set( DBMYSQL_VERSION_MINOR	1 )
set( DBMYSQL_VERSION_BUILD	0 )

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/Src 
    ${DATABASE_SOURCE_DIR}/Interface/Src
    ${MYSQL_CPP_INCLUDE_DIR}
)

set( MySqlLibraries "")
FOREACH(Lib ${MYSQL_LIBRARIES_DYNAMIC})
    if (MySqlLibraries)
        set(MySqlLibraries "${MySqlLibraries}|${Lib}")
    else()
        set(MySqlLibraries "${Lib}")
    endif()
ENDFOREACH()

set( DefaultTarget DatabasePluginMySql )

add_target(
    DatabasePluginMySql
    dll
    "Database"
    "Database;${MySqlLibraries};${MinLibraries}"
    "DatabaseMySqlPch.h"
    "DatabaseMySqlPch.cpp"
    "-Dmysqlcppconn_EXPORTS"
	"-Dmysqlcppconn_EXPORTS"
	""
)

copy_dll( DatabasePluginMySql ${MYSQL_CPP_LIBRARY_DYNAMIC_DEBUG} Debug )
copy_dll( DatabasePluginMySql ${MYSQL_CPP_LIBRARY_DYNAMIC_RELEASE} Release )

set_property( TARGET DatabasePluginMySql PROPERTY FOLDER "Plugin/MySQL" )
add_target_astyle( DatabasePluginMySql ".h;.hpp;.inl;.cpp" )
set( Build "yes" PARENT_SCOPE )