project( DatabasePluginOdbcMsSql )

set( PROJECT_DESC "DatabaseConnector ODBC MSSQL plug-in" )
set( ${PROJECT_NAME}_VERSION_MAJOR	0 )
set( ${PROJECT_NAME}_VERSION_MINOR	1 )
set( ${PROJECT_NAME}_VERSION_BUILD	0 )

set( ${PROJECT_NAME}_HDR_FILES
	Src/DatabaseConnectionOdbcMsSql.h
	Src/DatabaseOdbcMsSql.h
	Src/DatabaseOdbcMsSqlPrerequisites.h
	Src/FactoryDatabaseOdbcMsSql.h
	Src/PluginDatabaseOdbcMsSql.h
)
source_group( "Header Files"
	FILES
		${${PROJECT_NAME}_HDR_FILES}
)
set( ${PROJECT_NAME}_SRC_FILES
	Src/DatabaseConnectionOdbcMsSql.cpp
	Src/DatabaseOdbcMsSql.cpp
	Src/DllPluginDatabaseOdbcMsSql.cpp
	Src/FactoryDatabaseOdbcMsSql.cpp
	Src/PluginDatabaseOdbcMsSql.cpp
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
	"Src/DatabaseOdbcMsSqlPch.h"
	"Src/DatabaseOdbcMsSqlPch.cpp"
)
target_include_directories( ${PROJECT_NAME}
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/Src
)
target_link_libraries( ${PROJECT_NAME}
	PRIVATE
		DatabasePluginOdbc
		${ODBC_LIBRARIES_DYNAMIC}
)
set( DATABASE_INSTEXP_LIST
	${DATABASE_INSTEXP_LIST}
	${PROJECT_NAME}
	PARENT_SCOPE
)
set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Plugin" )
add_target_astyle( ${PROJECT_NAME} ".h;.hpp;.inl;.cpp" )
