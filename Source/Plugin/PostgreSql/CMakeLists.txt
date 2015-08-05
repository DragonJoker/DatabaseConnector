project( DatabasePluginPostgreSql )

set( PROJECT_DESC "DatabaseConnector PostgreSQL plug-in" )
set( ${PROJECT_NAME}_VERSION_MAJOR	0 )
set( ${PROJECT_NAME}_VERSION_MINOR	1 )
set( ${PROJECT_NAME}_VERSION_BUILD	0 )

# Include directories
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/Src 
	${DATABASE_SOURCE_DIR}/Interface/Src
	${PostgreSQL_INCLUDE_DIRS}
)

set( PostgreSqlLibraries "")
FOREACH(Lib ${PostgreSQL_LIBRARY})
	if (PostgreSqlLibraries)
		set(PostgreSqlLibraries "${PostgreSqlLibraries}|${Lib}")
	else()
		set(PostgreSqlLibraries "${Lib}")
	endif()
ENDFOREACH()

add_target(
	${PROJECT_NAME}
	plugin
	"Database"
	"Database;${PostgreSqlLibraries};${MinLibraries}"
	"DatabasePostgreSqlPch.h"
	"DatabasePostgreSqlPch.cpp"
)

copy_dll( ${PROJECT_NAME} ${PostgreSQL_LIBRARY} Debug )
copy_dll( ${PROJECT_NAME} ${PostgreSQL_LIBRARY} Release )
copy_dll( ${PROJECT_NAME} ${PostgreSQL_LIBRARY} RelWithDebInfo )

set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Plugin/PostgreSQL" )
add_target_astyle( ${PROJECT_NAME} ".h;.hpp;.inl;.cpp" )
set( Build "yes" PARENT_SCOPE )
