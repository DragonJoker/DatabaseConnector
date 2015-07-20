project( DatabasePluginOdbc )

set( PROJECT_DESC "DatabaseConnector ODBC Plugins Base" )
set( ${PROJECT_NAME}_VERSION_MAJOR	0 )
set( ${PROJECT_NAME}_VERSION_MINOR	1 )
set( ${PROJECT_NAME}_VERSION_BUILD	0 )

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/Src 
    ${DATABASE_SOURCE_DIR}/Interface/Src
    ${ODBC_INCLUDE_DIR}
)

add_target(
	${PROJECT_NAME}
	lib
	"Database"
	"Database;${ODBC_LIBRARIES};${MinLibraries}"
	"DatabaseOdbcPch.h"
	"DatabaseOdbcPch.cpp"
	"" "" ""
)

set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Plugin/Odbc" )
add_target_astyle( ${PROJECT_NAME} ".h;.hpp;.inl;.cpp" )

add_subdirectory( MsSql )
add_subdirectory( MySql )
set( Build "yes" PARENT_SCOPE )
