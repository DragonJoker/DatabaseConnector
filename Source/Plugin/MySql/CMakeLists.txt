project( DatabasePluginMySql )

set( PROJECT_DESC "DatabaseConnector MySQL plug-in" )
set( ${PROJECT_NAME}_VERSION_MAJOR	0 )
set( ${PROJECT_NAME}_VERSION_MINOR	1 )
set( ${PROJECT_NAME}_VERSION_BUILD	0 )

set( DefaultTarget DatabasePluginMySql )

add_target(
	DatabasePluginMySql
	plugin
	"${CMAKE_CURRENT_SOURCE_DIR}/Src"
	"${CMAKE_CURRENT_SOURCE_DIR}/Src"
	""
	""
	"DatabaseMySqlPch.h"
	"DatabaseMySqlPch.cpp"
	""
	""
	"${MySQL_LINK_FLAGS}"
)
add_library( dbConnector::MySQL
	ALIAS
	${PROJECT_NAME}
)
target_include_directories( ${PROJECT_NAME}
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/Src 
		${MySQL_INCLUDE_DIRS}
)
target_link_libraries( ${PROJECT_NAME}
	PRIVATE
		Database
		${MySQL_LIBRARIES}
)

if ( NOT VCPKG_TARGET_TRIPLET )
	copy_dll( DatabasePluginMySql "${MySQL_client_LIBRARY_DEBUG}" Debug )
	copy_dll( DatabasePluginMySql "${MySQL_client_LIBRARY_RELEASE}" Release )
	copy_dll( DatabasePluginMySql "${MySQL_client_LIBRARY_RELEASE}" RelWithDebInfo )
endif ()

set_property( TARGET DatabasePluginMySql PROPERTY FOLDER "Plugin/MySQL" )
add_target_astyle( DatabasePluginMySql ".h;.hpp;.inl;.cpp" )
set( Build "yes" PARENT_SCOPE )
