project( DatabasePluginSqlite )

set( PROJECT_DESC "DatabaseConnector SQLite plug-in" )
set( ${PROJECT_NAME}_VERSION_MAJOR	0 )
set( ${PROJECT_NAME}_VERSION_MINOR	1 )
set( ${PROJECT_NAME}_VERSION_BUILD	0 )

set( DefaultTarget DatabasePluginSqlite )

add_target(
	DatabasePluginSqlite
	plugin
	"${CMAKE_CURRENT_SOURCE_DIR}/Src"
	"${CMAKE_CURRENT_SOURCE_DIR}/Src"
	""
	""
	"Src/DatabaseSqlitePch.h"
	"Src/DatabaseSqlitePch.cpp"
	"" "" ""
)
add_library( dbConnector::SQLite
	ALIAS
	${PROJECT_NAME}
)
target_include_directories( ${PROJECT_NAME}
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/Src
)
target_link_libraries( ${PROJECT_NAME}
	PRIVATE
		Database
)

set_property( TARGET DatabasePluginSqlite PROPERTY FOLDER "Plugin/SQLite" )
add_target_astyle( DatabasePluginSqlite ".h;.hpp;.inl;.cpp" )
set( Build "yes" PARENT_SCOPE )