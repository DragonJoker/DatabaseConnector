project( DatabasePluginSqlite )

set( PROJECT_DESC "DatabaseConnector SQLite plug-in" )
set( ${PROJECT_NAME}_VERSION_MAJOR	0 )
set( ${PROJECT_NAME}_VERSION_MINOR	1 )
set( ${PROJECT_NAME}_VERSION_BUILD	0 )

set( DefaultTarget ${PROJECT_NAME} )

set( ${PROJECT_NAME}_HDR_FILES
	Src/DatabaseConnectionSqlite.h
	Src/DatabaseSqlite.h
	Src/DatabaseSqliteHelper.h
	Src/DatabaseSqliteParameterBinding.h
	Src/DatabaseSqlitePrerequisites.h
	Src/DatabaseStatementParameterSqlite.h
	Src/DatabaseStatementSqlite.h
	Src/ExceptionDatabaseSqlite.h
	Src/FactoryDatabaseSqlite.h
	Src/PluginDatabaseSqlite.h
	Src/sqlite3.h
	Src/sqlite3ext.h
)
source_group( "Header Files"
	FILES
		${${PROJECT_NAME}_HDR_FILES}
)
set( ${PROJECT_NAME}_SRC_FILES
	Src/DatabaseConnectionSqlite.cpp
	Src/DatabaseSqlite.cpp
	Src/DatabaseSqliteHelper.cpp
	Src/DatabaseStatementParameterSqlite.cpp
	Src/DatabaseStatementSqlite.cpp
	Src/DllPluginDatabaseSqlite.cpp
	Src/FactoryDatabaseSqlite.cpp
	Src/PluginDatabaseSqlite.cpp
	Src/sqlite3.c
)
source_group( "Source Files"
	FILES
		${${PROJECT_NAME}_SRC_FILES}
)
if ( WIN32 )
	find_rsc_file(
		${PROJECT_NAME}
		plugin
	)
	set( ${PROJECT_NAME}_SRC_FILES
		${${PROJECT_NAME}_SRC_FILES}
		${${PROJECT_NAME}_RSC_FILES}
	)
endif ()

add_target_min(
	${PROJECT_NAME}
	plugin
	"Src/DatabaseSqlitePch.h"
	"Src/DatabaseSqlitePch.cpp"
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

set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Plugin/SQLite" )
add_target_astyle( ${PROJECT_NAME} ".h;.hpp;.inl;.cpp" )

install_target( ${PROJECT_NAME} plugin Src )

set( Build "yes" PARENT_SCOPE )