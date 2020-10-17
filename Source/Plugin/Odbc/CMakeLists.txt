project( DatabasePluginOdbc )

set( PROJECT_DESC "DatabaseConnector ODBC Plugins Base" )
set( ${PROJECT_NAME}_VERSION_MAJOR	0 )
set( ${PROJECT_NAME}_VERSION_MINOR	1 )
set( ${PROJECT_NAME}_VERSION_BUILD	0 )

set( ${PROJECT_NAME}_HDR_FILES
	Src/DatabaseConnectionOdbc.h
	Src/DatabaseOdbcHelper.h
	Src/DatabaseOdbcParameterBinding.h
	Src/DatabaseOdbcPrerequisites.h
	Src/DatabaseStatementOdbc.h
	Src/DatabaseStatementParameterOdbc.h
	Src/ExceptionDatabaseOdbc.h
)
source_group( "Header Files"
	FILES
		${${PROJECT_NAME}_HDR_FILES}
)
set( ${PROJECT_NAME}_SRC_FILES
	Src/DatabaseConnectionOdbc.cpp
	Src/DatabaseOdbcHelper.cpp
	Src/DatabaseOdbcParameterBinding.cpp
	Src/DatabaseStatementOdbc.cpp
	Src/DatabaseStatementParameterOdbc.cpp
)
source_group( "Source Files"
	FILES
		${${PROJECT_NAME}_SRC_FILES}
)
if ( WIN32 )
	find_rsc_file(
		${PROJECT_NAME}
		lib
	)
	set( ${PROJECT_NAME}_SRC_FILES
		${${PROJECT_NAME}_SRC_FILES}
		${${PROJECT_NAME}_RSC_FILES}
	)
endif ()

add_target_min(
	${PROJECT_NAME}
	lib
	"Src/DatabaseOdbcPch.h"
	"Src/DatabaseOdbcPch.cpp"
)
add_library( dbConnector::ODBC
	ALIAS
	${PROJECT_NAME}
)
target_include_directories( ${PROJECT_NAME}
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/Src
		${ODBC_INCLUDE_DIR}
)
target_link_libraries( ${PROJECT_NAME}
	PUBLIC
		Database
		${ODBC_LIBRARIES}
)
set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Plugin" )
add_target_astyle( ${PROJECT_NAME} ".h;.hpp;.inl;.cpp" )

install_target( ${PROJECT_NAME} lib Src )

add_subdirectory( MsSql )
add_subdirectory( MySql )

set( DATABASE_INSTEXP_LIST
	${DATABASE_INSTEXP_LIST}
	PARENT_SCOPE
)
set( Build "yes" PARENT_SCOPE )
