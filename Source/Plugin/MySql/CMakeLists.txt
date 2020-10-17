project( DatabasePluginMySql )

set( PROJECT_DESC "DatabaseConnector MySQL plug-in" )
set( ${PROJECT_NAME}_VERSION_MAJOR	0 )
set( ${PROJECT_NAME}_VERSION_MINOR	1 )
set( ${PROJECT_NAME}_VERSION_BUILD	0 )

set( DefaultTarget ${PROJECT_NAME} )

set( ${PROJECT_NAME}_HDR_FILES
	Src/DatabaseConnectionMySql.h
	Src/DatabaseMySql.h
	Src/DatabaseMySqlHelper.h
	Src/DatabaseMySqlParameterBinding.h
	Src/DatabaseMySqlPrerequisites.h
	Src/DatabaseParameterMySql.h
	Src/DatabaseStatementMySql.h
	Src/ExceptionDatabaseMySql.h
	Src/FactoryDatabaseMySql.h
	Src/PluginDatabaseMySql.h
)
source_group( "Header Files"
	FILES
		${${PROJECT_NAME}_HDR_FILES}
)
set( ${PROJECT_NAME}_SRC_FILES
	Src/DatabaseConnectionMySql.cpp
	Src/DatabaseMySql.cpp
	Src/DatabaseMySqlHelper.cpp
	Src/DatabaseParameterMySql.cpp
	Src/DatabaseStatementMySql.cpp
	Src/DllPluginDatabaseMySql.cpp
	Src/FactoryDatabaseMySql.cpp
	Src/PluginDatabaseMySql.cpp
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
	"Src/DatabaseMySqlPch.h"
	"Src/DatabaseMySqlPch.cpp"
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
target_compile_options( ${PROJECT_NAME}
	PRIVATE
		${MySQL_LINK_FLAGS}
)
target_link_libraries( ${PROJECT_NAME}
	PRIVATE
		Database
		${MySQL_LIBRARIES}
)

if ( NOT VCPKG_TARGET_TRIPLET )
	copy_dll( ${PROJECT_NAME} "${MySQL_client_LIBRARY_DEBUG}" Debug )
	copy_dll( ${PROJECT_NAME} "${MySQL_client_LIBRARY_RELEASE}" Release )
	copy_dll( ${PROJECT_NAME} "${MySQL_client_LIBRARY_RELEASE}" RelWithDebInfo )
endif ()
set( DATABASE_INSTEXP_LIST
	${DATABASE_INSTEXP_LIST}
	${PROJECT_NAME}
	PARENT_SCOPE
)
set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Plugin" )
add_target_astyle( ${PROJECT_NAME} ".h;.hpp;.inl;.cpp" )

install_target( ${PROJECT_NAME} plugin Src )

set( Build "yes" PARENT_SCOPE )
