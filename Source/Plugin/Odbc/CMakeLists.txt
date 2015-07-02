project( DatabasePluginOdbc )

set( DBODBC_VERSION_MAJOR	0 )
set( DBODBC_VERSION_MINOR	1 )
set( DBODBC_VERSION_BUILD	0 )

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/Src 
    ${DATABASE_SOURCE_DIR}/Interface/Src
    ${ODBC_INCLUDE_DIR}
)

add_target(
	DatabasePluginOdbc
	lib
	"Database"
	"Database;${ODBC_LIBRARIES};${MinLibraries}"
	"DatabaseOdbcPch.h"
	"DatabaseOdbcPch.cpp"
	"" "" ""
)

set_property( TARGET DatabasePluginOdbc PROPERTY FOLDER "Plugin/Odbc" )
add_target_astyle( DatabasePluginOdbc ".h;.hpp;.inl;.cpp" )

add_subdirectory( MsSql )
add_subdirectory( MySql )
set( Build "yes" PARENT_SCOPE )
