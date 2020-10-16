project( DatabasePluginPostgreSql )

set( PROJECT_DESC "DatabaseConnector PostgreSQL plug-in" )
set( ${PROJECT_NAME}_VERSION_MAJOR	0 )
set( ${PROJECT_NAME}_VERSION_MINOR	1 )
set( ${PROJECT_NAME}_VERSION_BUILD	0 )

set( ${PROJECT_NAME}_HDR_FILES
	Src/DatabaseConnectionPostgreSql.h
	Src/DatabaseParameterPostgreSql.h
	Src/DatabasePostgreSql.h
	Src/DatabasePostgreSqlHelper.h
	Src/DatabasePostgreSqlParameterBinding.h
	Src/DatabasePostgreSqlPrerequisites.h
	Src/DatabaseStatementPostgreSql.h
	Src/ExceptionDatabasePostgreSql.h
	Src/FactoryDatabasePostgreSql.h
	Src/PluginDatabasePostgreSql.h
)
source_group( "Header Files"
	FILES
		${${PROJECT_NAME}_HDR_FILES}
)
set( ${PROJECT_NAME}_SRC_FILES
	Src/DatabaseConnectionPostgreSql.cpp
	Src/DatabaseParameterPostgreSql.cpp
	Src/DatabasePostgreSql.cpp
	Src/DatabasePostgreSqlHelper.cpp
	Src/DatabaseStatementPostgreSql.cpp
	Src/DllPluginDatabasePostgreSql.cpp
	Src/FactoryDatabasePostgreSql.cpp
	Src/PluginDatabasePostgreSql.cpp
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
if ( WIN32 )
	target_link_libraries( ${PROJECT_NAME}
		PRIVATE
			Bcrypt
	)
endif ()

if ( NOT VCPKG_TARGET_TRIPLET )
	copy_dll( ${PROJECT_NAME} ${PostgreSQL_LIBRARY} Debug )
	copy_dll( ${PROJECT_NAME} ${PostgreSQL_LIBRARY} Release )
	copy_dll( ${PROJECT_NAME} ${PostgreSQL_LIBRARY} RelWithDebInfo )
endif ()

set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Plugin/PostgreSQL" )
add_target_astyle( ${PROJECT_NAME} ".h;.hpp;.inl;.cpp" )

install_target( ${PROJECT_NAME} plugin Src )

set( Build "yes" PARENT_SCOPE )
