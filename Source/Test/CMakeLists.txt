project( DatabaseTest )

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/Src
	${CMAKE_CURRENT_BINARY_DIR}
)

if( WIN32 )
	include_directories( 
		${CMAKE_CURRENT_SOURCE_DIR}/Src/Win32
	)
	
	# Get path to utilities for (un)installing the ODBC sources
	find_path( SCRIPT_FILES_DIR
		NAMES
			UninstallOdbc.bat
			InstallOdbcMsSql.bat
			CreateDatabaseMsSql.bat
			DeleteDatabaseMsSql.bat
		PATHS
			${CMAKE_CURRENT_SOURCE_DIR}/Scripts
	)
else()
	# Get path to utilities for database creation
	find_path( SCRIPT_FILES_DIR
		NAMES
			UninstallOdbc.sh
			InstallOdbcMySql.sh
			CreateDatabaseMySql.sh
			DeleteDatabaseMySql.sh
		PATHS
			${CMAKE_CURRENT_SOURCE_DIR}/Scripts
	)
endif()

if ( MySQL_COMMAND )
	file( TO_NATIVE_PATH ${MySQL_COMMAND} MySQL_COMMAND )
	string( REPLACE "\\" "\\\\" MySQL_COMMAND ${MySQL_COMMAND} )
endif ()

file( TO_NATIVE_PATH ${SCRIPT_FILES_DIR} SCRIPT_FILES_DIR )
string( REPLACE "\\" "\\\\" SCRIPT_FILES_DIR ${SCRIPT_FILES_DIR} )
configure_file( ${CMAKE_TEMPLATES_DIR}/DatabaseTestUtilities.h.in ${CMAKE_CURRENT_BINARY_DIR}/DatabaseTestUtilities.h NEWLINE_STYLE LF )

set( PROJECT_DESC "DatabaseConnector Test" )
set( ${PROJECT_NAME}_VERSION_MAJOR	0 )
set( ${PROJECT_NAME}_VERSION_MINOR	1 )
set( ${PROJECT_NAME}_VERSION_BUILD	0 )

set( BinsDependencies
    ${BinsDependencies}
    Database
)

include_directories( ${CMAKE_CURRENT_BINARY_DIR} )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../Plugin/MySql/Src )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../Plugin/Odbc/Src )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../Plugin/Odbc/MySql/Src )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../Plugin/Odbc/MsSql/Src )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../Plugin/Sqlite/Src )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../Plugin/PostgreSql/Src )

find_package( Boost COMPONENTS unit_test_framework )

if( Boost_UNIT_TEST_FRAMEWORK_FOUND )
	link_directories( ${Boost_LIBRARY_DIR} )
	#Boost unit test Libs
	set( BoostUnitTestLibraries "")
	FOREACH(Lib ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
		if (BoostUnitTestLibraries)
			set(BoostUnitTestLibraries "${BoostUnitTestLibraries}|${Lib}")
		else()
			set(BoostUnitTestLibraries "${Lib}")
		endif()
	ENDFOREACH()
	
	add_target(
		${PROJECT_NAME}
		bin_dos
		"${BinsDependencies}"
		"${BinsDependencies};${BoostUnitTestLibraries};${MinLibraries};"
		"DatabaseTestPch.h"
		"DatabaseTestPch.cpp"
		""
		""
		""
		"${CMAKE_CURRENT_BINARY_DIR}/DatabaseTestUtilities.h"
	)

	if( ${BUILD_MySQL_PLUGIN} )
		add_dependencies( ${PROJECT_NAME} DatabasePluginMySql )
		set( ADDITIONAL_FLAGS ${ADDITIONAL_FLAGS} TESTING_PLUGIN_MYSQL )
	endif()
	if( ${BUILD_ODBC_PLUGIN} )
		add_dependencies( ${PROJECT_NAME} DatabasePluginOdbcMySql DatabasePluginOdbcMsSql )
		set( ADDITIONAL_FLAGS ${ADDITIONAL_FLAGS} TESTING_PLUGIN_ODBC )
	endif()
	if( ${BUILD_SQLite_PLUGIN} )
		add_dependencies( ${PROJECT_NAME} DatabasePluginSqlite )
		set( ADDITIONAL_FLAGS ${ADDITIONAL_FLAGS} TESTING_PLUGIN_SQLITE )
	endif()
	if( ${BUILD_PostgreSql_PLUGIN} )
		add_dependencies( ${PROJECT_NAME} DatabasePluginPostgreSql )
		set( ADDITIONAL_FLAGS ${ADDITIONAL_FLAGS} TESTING_PLUGIN_POSTGRE )
	endif()

	set( ADDITIONAL_FLAGS ${ADDITIONAL_FLAGS} BOOST_TEST_DYN_LINK )

	if ( NOT "" STREQUAL "${ADDITIONAL_FLAGS}" )
	  set_target_properties( ${PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS "${ADDITIONAL_FLAGS}" )
	endif ()

	set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Test" )
	add_target_astyle( ${PROJECT_NAME} ".h;.hpp;.inl;.cpp" )
	
	# Adding debug test
	add_test( NAME DatabaseTest_boost_test_debug
		CONFIGURATIONS Debug
		COMMAND "${PROJECT_NAME}d"
		WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug"
	)

	# Adding release test
	add_test( NAME DatabaseTest_boost_test_release
		CONFIGURATIONS Release
		COMMAND "${PROJECT_NAME}"
		WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release"
	)

	copy_dll( ${PROJECT_NAME} "${Boost_UNIT_TEST_FRAMEWORK_LIBRARY_DEBUG}" Debug )
	copy_dll( ${PROJECT_NAME} "${Boost_UNIT_TEST_FRAMEWORK_LIBRARY_RELEASE}" Release )
else()
	message( STATUS "Test can't be built (missing Boost)" )
endif()
