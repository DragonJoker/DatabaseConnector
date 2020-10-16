project( DatabasePluginOdbc )

set( PROJECT_DESC "DatabaseConnector ODBC Plugins Base" )
set( ${PROJECT_NAME}_VERSION_MAJOR	0 )
set( ${PROJECT_NAME}_VERSION_MINOR	1 )
set( ${PROJECT_NAME}_VERSION_BUILD	0 )

add_target(
	${PROJECT_NAME}
	lib
	"${CMAKE_CURRENT_SOURCE_DIR}/Src"
	"${CMAKE_CURRENT_SOURCE_DIR}/Src"
	""
	""
	"DatabaseOdbcPch.h"
	"DatabaseOdbcPch.cpp"
	"" "" ""
)
add_library( dbConnector::ODBC
	ALIAS
	${PROJECT_NAME}
)
target_include_directories( ${PROJECT_NAME}
	PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Src
    ${ODBC_INCLUDE_DIR}
)
target_link_libraries( ${PROJECT_NAME}
	PRIVATE
		Database
		${ODBC_LIBRARIES}
)

set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Plugin/Odbc" )
add_target_astyle( ${PROJECT_NAME} ".h;.hpp;.inl;.cpp" )

add_subdirectory( MsSql )
add_subdirectory( MySql )
set( Build "yes" PARENT_SCOPE )
