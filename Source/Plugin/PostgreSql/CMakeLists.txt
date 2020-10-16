project( DatabasePluginPostgreSql )

set( PROJECT_DESC "DatabaseConnector PostgreSQL plug-in" )
set( ${PROJECT_NAME}_VERSION_MAJOR	0 )
set( ${PROJECT_NAME}_VERSION_MINOR	1 )
set( ${PROJECT_NAME}_VERSION_BUILD	0 )

add_target(
	${PROJECT_NAME}
	plugin
	"${CMAKE_CURRENT_SOURCE_DIR}/Src"
	"${CMAKE_CURRENT_SOURCE_DIR}/Src"
	""
	""
	"Src/DatabasePostgreSqlPch.h"
	"Src/DatabasePostgreSqlPch.cpp"
)
add_library( dbConnector::PostgreSQL
	ALIAS
	${PROJECT_NAME}
)
target_include_directories( ${PROJECT_NAME}
	PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Src
    ${PostgreSQL_INCLUDE_DIRS}
)
target_link_libraries( ${PROJECT_NAME}
	PRIVATE
		Database
		${PostgreSQL_LIBRARY}
)

if ( NOT VCPKG_TARGET_TRIPLET )
	copy_dll( ${PROJECT_NAME} ${PostgreSQL_LIBRARY} Debug )
	copy_dll( ${PROJECT_NAME} ${PostgreSQL_LIBRARY} Release )
	copy_dll( ${PROJECT_NAME} ${PostgreSQL_LIBRARY} RelWithDebInfo )
endif ()

set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Plugin/PostgreSQL" )
add_target_astyle( ${PROJECT_NAME} ".h;.hpp;.inl;.cpp" )
set( Build "yes" PARENT_SCOPE )
