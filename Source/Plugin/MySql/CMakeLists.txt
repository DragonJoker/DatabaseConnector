project( DatabasePluginMySql )

set( PROJECT_DESC "DatabaseConnector MySQL plug-in" )
set( ${PROJECT_NAME}_VERSION_MAJOR	0 )
set( ${PROJECT_NAME}_VERSION_MINOR	1 )
set( ${PROJECT_NAME}_VERSION_BUILD	0 )

# Include directories
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/Src 
	${DATABASE_SOURCE_DIR}/Interface/Src
	${MySQL_INCLUDE_DIRS}
)

set( MySqlLibraries "")
FOREACH(Lib ${MySQL_LIBRARIES})
	if (MySqlLibraries)
		set(MySqlLibraries "${MySqlLibraries}|${Lib}")
	else()
		set(MySqlLibraries "${Lib}")
	endif()
ENDFOREACH()

set( DefaultTarget DatabasePluginMySql )

add_target(
	DatabasePluginMySql
	plugin
	"Database"
	"Database;${MySqlLibraries};${MinLibraries}"
	"DatabaseMySqlPch.h"
	"DatabaseMySqlPch.cpp"
	""
	""
	"${MySQL_LINK_FLAGS}"
)

copy_dll( DatabasePluginMySql ${MySQL_client_LIBRARY_DEBUG} Debug )
copy_dll( DatabasePluginMySql ${MySQL_client_LIBRARY_RELEASE} Release )
copy_dll( DatabasePluginMySql ${MySQL_client_LIBRARY_RELEASE} RelWithDebInfo )

set_property( TARGET DatabasePluginMySql PROPERTY FOLDER "Plugin/MySQL" )
add_target_astyle( DatabasePluginMySql ".h;.hpp;.inl;.cpp" )
set( Build "yes" PARENT_SCOPE )
