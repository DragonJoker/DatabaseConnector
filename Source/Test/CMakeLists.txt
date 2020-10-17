option( BUILD_DATABASE_TESTS "Build tests" OFF )
option( BUILD_DATABASE_TESTS_SQLite "Build SQLite tests" ${BUILD_SQLite_PLUGIN} )
option( BUILD_DATABASE_TESTS_MySQL "Build MySQL tests" ${BUILD_MySQL_PLUGIN} )
option( BUILD_DATABASE_TESTS_PostgreSql "Build PostgreSQL tests" ${BUILD_PostgreSql_PLUGIN} )
option( BUILD_DATABASE_TESTS_ODBC "Build ODBC tests" ${BUILD_ODBC_PLUGIN} )

if ( BUILD_DATABASE_TESTS )
	project( DatabaseTest )

	set( PROJECT_DESC "DatabaseConnector Test" )
	set( ${PROJECT_NAME}_VERSION_MAJOR	0 )
	set( ${PROJECT_NAME}_VERSION_MINOR	1 )
	set( ${PROJECT_NAME}_VERSION_BUILD	0 )

	if( WIN32 )
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

	find_package( Boost COMPONENTS unit_test_framework )

	if( Boost_UNIT_TEST_FRAMEWORK_FOUND )
		if ( MSVC )
			set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj" )
		endif ()

		set( ${PROJECT_NAME}_HDR_FILES
			${CMAKE_CURRENT_BINARY_DIR}/DatabaseTestUtilities.h
			Src/DatabaseConnectionTest.h
			Src/DatabaseDateTest.h
			Src/DatabaseDateTimeTest.h
			Src/DatabaseFieldTest.h
			Src/DatabaseFixedPointTest.h
			Src/DatabaseInt24Test.h
			Src/DatabaseNullableTest.h
			Src/DatabaseParameterTest.h
			Src/DatabaseQueryTest.h
			Src/DatabaseResultTest.h
			Src/DatabaseRowTest.h
			Src/DatabaseStatementTest.h
			Src/DatabaseStringUtilsTest.h
			Src/DatabaseTest.h
			Src/DatabaseTestBase.h
			Src/DatabaseTestBase.inl
			Src/DatabaseTestConnection.h
			Src/DatabaseTestHelpers.h
			Src/DatabaseTestPluginsStaticLoader.h
			Src/DatabaseTestPrerequisites.h
			Src/DatabaseTestStatement.h
			Src/DatabaseTestUtils.h
			Src/DatabaseTestUtilsUtf8.h
			Src/DatabaseTestValuedObject.h
			Src/DatabaseTimeTest.h
			Src/DatabaseUInt24Test.h
			Src/DatabaseValuedObjectInfosTest.h
			Src/DatabaseValuedObjectTest.h
			Src/DatabaseValueTest.h
		)
		set( ${PROJECT_NAME}_SRC_FILES
			Src/DatabaseConnectionTest.cpp
			Src/DatabaseDateTest.cpp
			Src/DatabaseDateTimeTest.cpp
			Src/DatabaseFieldTest.cpp
			Src/DatabaseFixedPointTest.cpp
			Src/DatabaseInt24Test.cpp
			Src/DatabaseNullableTest.cpp
			Src/DatabaseParameterTest.cpp
			Src/DatabaseQueryTest.cpp
			Src/DatabaseResultTest.cpp
			Src/DatabaseRowTest.cpp
			Src/DatabaseStatementTest.cpp
			Src/DatabaseStringUtilsTest.cpp
			Src/DatabaseTest.cpp
			Src/DatabaseTestBase.cpp
			Src/DatabaseTestConnection.cpp
			Src/DatabaseTestPluginsStaticLoader.cpp
			Src/DatabaseTestStatement.cpp
			Src/DatabaseTestUtils.cpp
			Src/DatabaseTimeTest.cpp
			Src/DatabaseUInt24Test.cpp
			Src/DatabaseValuedObjectInfosTest.cpp
			Src/DatabaseValuedObjectTest.cpp
			Src/DatabaseValueTest.cpp
		)
		if( ${BUILD_DATABASE_TESTS_MySQL} )
			set( ${PROJECT_NAME}_HDR_FILES
				${${PROJECT_NAME}_HDR_FILES}
				Src/DatabaseMySqlTest.h
			)
			set( ${PROJECT_NAME}_SRC_FILES
				${${PROJECT_NAME}_SRC_FILES}
				Src/DatabaseMySqlTest.cpp
			)
		endif ()
		if( ${BUILD_DATABASE_TESTS_ODBC} )
			set( ${PROJECT_NAME}_HDR_FILES
				${${PROJECT_NAME}_HDR_FILES}
				Src/DatabaseOdbcMsSqlTest.h
				Src/DatabaseOdbcMySqlTest.h
			)
			set( ${PROJECT_NAME}_SRC_FILES
				${${PROJECT_NAME}_SRC_FILES}
				Src/DatabaseOdbcMsSqlTest.cpp
				Src/DatabaseOdbcMySqlTest.cpp
			)
		endif()
		if( ${BUILD_DATABASE_TESTS_SQLite} )
			set( ${PROJECT_NAME}_HDR_FILES
				${${PROJECT_NAME}_HDR_FILES}
				Src/DatabaseSqliteTest.h
			)
			set( ${PROJECT_NAME}_SRC_FILES
				${${PROJECT_NAME}_SRC_FILES}
				Src/DatabaseSqliteTest.cpp
			)
		endif ()
		if( ${BUILD_DATABASE_TESTS_PostgreSql} )
			set( ${PROJECT_NAME}_HDR_FILES
				${${PROJECT_NAME}_HDR_FILES}
				Src/DatabasePostgreSqlTest.h
			)
			set( ${PROJECT_NAME}_SRC_FILES
				${${PROJECT_NAME}_SRC_FILES}
				Src/DatabasePostgreSqlTest.cpp
			)
		endif ()
		source_group( "Header Files"
			FILES
				${${PROJECT_NAME}_HDR_FILES}
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
			bin_dos
			"Src/DatabaseTestPch.h"
			"Src/DatabaseTestPch.cpp"
		)
		target_link_directories( ${PROJECT_NAME}
			PRIVATE
				${Boost_LIBRARY_DIR}
		)
		target_link_libraries( ${PROJECT_NAME}
			PRIVATE
				Database
				${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
		)
		if( WIN32 )
			target_include_directories( ${PROJECT_NAME}
				PRIVATE
					${CMAKE_CURRENT_SOURCE_DIR}/Src/Win32
			)
		endif ()
		target_include_directories( ${PROJECT_NAME}
			PRIVATE
				${CMAKE_CURRENT_SOURCE_DIR}/Src
				${CMAKE_CURRENT_BINARY_DIR}
				${CMAKE_CURRENT_SOURCE_DIR}/../Plugin/MySql/Src
				${CMAKE_CURRENT_SOURCE_DIR}/../Plugin/Odbc/Src
				${CMAKE_CURRENT_SOURCE_DIR}/../Plugin/Odbc/MySql/Src
				${CMAKE_CURRENT_SOURCE_DIR}/../Plugin/Odbc/MsSql/Src
				${CMAKE_CURRENT_SOURCE_DIR}/../Plugin/Sqlite/Src
				${CMAKE_CURRENT_SOURCE_DIR}/../Plugin/PostgreSql/Src
		)
		if( ${BUILD_DATABASE_TESTS_MySQL} )
			add_dependencies( ${PROJECT_NAME}
				DatabasePluginMySql
			)
			target_compile_definitions( ${PROJECT_NAME}
				PRIVATE
					TESTING_PLUGIN_MYSQL
			)
		endif()
		if( ${BUILD_DATABASE_TESTS_ODBC} )
			add_dependencies( ${PROJECT_NAME}
				DatabasePluginOdbcMySql
				DatabasePluginOdbcMsSql
			)
			target_compile_definitions( ${PROJECT_NAME}
				PRIVATE
					TESTING_PLUGIN_ODBC
			)
		endif()
		if( ${BUILD_DATABASE_TESTS_SQLite} )
			add_dependencies( ${PROJECT_NAME}
				DatabasePluginSqlite
			)
			target_compile_definitions( ${PROJECT_NAME}
				PRIVATE
					TESTING_PLUGIN_SQLITE
			)
		endif()
		if( ${BUILD_DATABASE_TESTS_PostgreSql} )
			add_dependencies( ${PROJECT_NAME}
				DatabasePluginPostgreSql
			)
			target_compile_definitions( ${PROJECT_NAME}
				PRIVATE
					TESTING_PLUGIN_POSTGRE
			)
		endif()
		target_compile_definitions( ${PROJECT_NAME}
			PRIVATE
				BOOST_TEST_DYN_LINK
		)
		set_property( TARGET ${PROJECT_NAME}
			PROPERTY
				FOLDER "Test"
		)
		add_target_astyle( ${PROJECT_NAME}
			".h;.hpp;.inl;.cpp"
		)

		message( STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}" )
		# Adding debug test
		add_test( NAME ${PROJECT_NAME}_boost_test_debug
			CONFIGURATIONS Debug
			COMMAND "${PROJECT_NAME}"
			WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}"
		)

		# Adding release test
		add_test( NAME ${PROJECT_NAME}_boost_test_release
			CONFIGURATIONS Release
			COMMAND "${PROJECT_NAME}"
			WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}"
		)

		if ( WIN32 )
			copy_dll( ${PROJECT_NAME} "${Boost_UNIT_TEST_FRAMEWORK_LIBRARY_DEBUG}" Debug )
			copy_dll( ${PROJECT_NAME} "${Boost_UNIT_TEST_FRAMEWORK_LIBRARY_RELEASE}" Release )
			copy_dll( ${PROJECT_NAME} "${Boost_UNIT_TEST_FRAMEWORK_LIBRARY_RELEASE}" RelWithDebInfo )
		endif ()
	else()
		message( STATUS "Test can't be built (missing Boost)" )
	endif()
endif()
