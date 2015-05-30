project( DatabaseTest )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/Src )

if( WIN32 )
    include_directories( 
        "Src/Win32"
        ${CMAKE_CURRENT_BINARY_DIR}
    )
    
    source_group( "Resource Files" FILES ${DatabaseTest_PLATFORM_RESOURCE_FILES} )

    set( DatabaseTest_PRE_COMMAND
    )

    set( DatabaseTest_POST_COMMAND
    )
        
    # Get path to utilities for (un)installing the ODBC sources
    find_path( ODBC_BATCH_FILES_DIR
        NAMES   Install_odbc.bat
                Install_odbc_aria.bat
                Install_odbc_fdms.bat
                Uninstall_odbc.bat
                Uninstall_odbc_aria.bat
                Uninstall_odbc_fdms.bat
        PATHS   ${CMAKE_SOURCE_DIR}/../Scripts/Batch
    )
    
    # Get path to utilities for database creation
    find_path( DATABASE_BATCH_FILES_DIR
        NAMES   CreateDatabase.bat
                DeleteDatabase.bat
        PATHS   ${CMAKE_SOURCE_DIR}/../Scripts/Batch
    )
    
    configure_file( ${CMAKE_TEMPLATES_DIR}/DatabaseTestUtilities.h.in ${CMAKE_CURRENT_BINARY_DIR}/DatabaseTestUtilities.h NEWLINE_STYLE LF )
    set( DatabaseTest_SOURCE_FILES
        ${CMAKE_BINARY_DIR}/Test/Src/DatabaseTestUtilities.h )
else()
    set( DatabaseTest_PRE_COMMAND
        mkdir -p "${CMAKE_BINARY_DIR}/Database/Test/Src"
    )

    set( DatabaseTest_POST_COMMAND
    )
        
    # Get path to utilities for (un)installing the ODBC sources
    find_path( ODBC_BATCH_FILES_DIR
        NAMES   Install_odbc.sh
                Install_odbc_aria.sh
                Install_odbc_fdms.sh
                Uninstall_odbc.sh
                Uninstall_odbc_aria.sh
                Uninstall_odbc_fdms.sh
        PATHS   ${CMAKE_SOURCE_DIR}/../Scripts/Shell
    )
    
    # Get path to utilities for database creation
    find_path( DATABASE_BATCH_FILES_DIR
        NAMES   CreateDatabase.sh
                DeleteDatabase.sh
        PATHS   ${CMAKE_SOURCE_DIR}/../Scripts/Shell
    )
endif()
    
configure_file( ${CMAKE_TEMPLATES_DIR}/DatabaseTestUtilities.h.in ${CMAKE_CURRENT_BINARY_DIR}/DatabaseTestUtilities.h NEWLINE_STYLE LF )
set( DatabaseTest_SOURCE_FILES ${CMAKE_BINARY_DIR}/Test/Src/DatabaseTestUtilities.h )

set( DBT_VERSION_MAJOR	0 )
set( DBT_VERSION_MINOR	1 )
set( DBT_VERSION_BUILD	0 )

set( BinsDependencies
    ${BinsDependencies}
    Database
)
if( BUILD_MySQLConnPP_PLUGIN )
    set( BinsDependencies
        ${BinsDependencies}
        DatabasePluginMySql
    )
endif()
if( BUILD_ODBC_PLUGIN )
    set( BinsDependencies
        ${BinsDependencies}
        DatabasePluginOdbcMySql
        DatabasePluginOdbcMsSql
    )
endif()
if( BUILD_SQLite_PLUGIN )
    set( BinsDependencies
        ${BinsDependencies}
        DatabasePluginSqlite
    )
endif()

include_directories( ${CMAKE_CURRENT_BINARY_DIR} )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../Plugin/MySql/Src )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../Plugin/Odbc/Src )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../Plugin/Odbc/MySql/Src )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../Plugin/Odbc/MsSql/Src )

find_package( Boost COMPONENTS unit_test_framework )

if( Boost_UNIT_TEST_FRAMEWORK_FOUND )
    link_directories( ${Boost_LIBRARY_DIRS} )
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
        DatabaseTest
        bin_dos
        "${BinsDependencies}"
        "${BinsDependencies};${BoostUnitTestLibraries};${MinLibraries};"
        "DatabaseTestPch.h"
        "DatabaseTestPch.cpp"
        ""	""	""
    )

    set_target_properties( DatabaseTest PROPERTIES COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK )
    set_property( TARGET DatabaseTest PROPERTY FOLDER "Test" )
    add_target_astyle( DatabaseTest ".h;.hpp;.inl;.cpp" )
    
    # Adding debug test
    add_test( NAME DatabaseTest_boost_test_debug
        CONFIGURATIONS Debug
        COMMAND "DatabaseTest"
        WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug"
    )

    # Adding release test
    add_test( NAME DatabaseTest_boost_test_release
        CONFIGURATIONS Release
        COMMAND "DatabaseTest"
        WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release"
    )

    # Adding release with debug info test
    add_test( NAME DatabaseTest_boost_test_relwithdebinfo
        CONFIGURATIONS RelWithDebInfo
        COMMAND "DatabaseTest"
        WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/RelWithDebInfo"
    )
else()
    message( STATUS "Test can't be built (missing Boost)" )
endif()
