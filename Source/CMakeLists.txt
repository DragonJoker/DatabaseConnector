cmake_minimum_required( VERSION 2.8 )
#--------------------------------------------------------------------------------------------------
#	Initial configurations
#--------------------------------------------------------------------------------------------------
# Set project name, used in folders and in workspace creation
set( ProjectName "DatabaseEngine" )

# Set project version numbers
set( VERSION_MAJOR	0		)
set( VERSION_MINOR	1		)
set( VERSION_BUILD	0		)
set( VERSION_YEAR	2014	)

# Used to look for external modules
set( DATABASE_CMAKE_PATH "${CMAKE_SOURCE_DIR}/CMake" )
# Used to look for external modules
set( CMAKE_MODULE_PATH "${DATABASE_CMAKE_PATH}/Modules" )
# Experimental Precompiled headers support for GCC
include( ${DATABASE_CMAKE_PATH}/PCHSupport.cmake )
# Force Unicode projects (MBCS is past :P)
add_definitions( -DUNICODE -D_UNICODE )

# Enable CTest
enable_testing()
    
# Declare the project
project( ${ProjectName} )

include( ${DATABASE_CMAKE_PATH}/Project.cmake )
include( ${DATABASE_CMAKE_PATH}/UnitTest.cmake )

option( BUILD_STATIC "Build Static" FALSE )

# Organize projects into folders
set_property( GLOBAL PROPERTY USE_FOLDERS ON )

set( CMAKE_TEMPLATES_DIR "${DATABASE_CMAKE_PATH}/Templates" )

#--------------------------------------------------------------------------------------------------
#	Looking for needed libraries
#--------------------------------------------------------------------------------------------------
set( Boost_USE_STATIC_LIBS OFF )
set( Boost_USE_STATIC_RUNTIME OFF )
set( Boost_USE_MULTITHREADED ON )

set( Boost_NEEDED TRUE )

if (CMAKE_COMPILER_IS_GNUCXX)
	COMPILER_DUMPVERSION( COMPILER_VERSION )
	msg_debug( "GCC version : ${COMPILER_VERSION}" )
	if (${COMPILER_VERSION} LESS "46" OR MINGW)
		find_package( Boost COMPONENTS thread system chrono date_time locale )
	else()
		find_package( Boost COMPONENTS date_time locale )
	endif ()
elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
	COMPILER_DUMPVERSION( COMPILER_VERSION )
	msg_debug( "clang version : ${COMPILER_VERSION}" )
    find_package( Boost COMPONENTS date_time locale )
elseif (BORLAND)
	find_package( Boost COMPONENTS thread system date_time locale )
elseif (NOT MSVC11 AND NOT MSVC12)
	find_package( Boost COMPONENTS thread system chrono date_time locale )
else()
		find_package( Boost COMPONENTS date_time locale )
endif ()

#--------------------------------------------------------------------------------------------------
#	Adding include dirs to include search path
#--------------------------------------------------------------------------------------------------
include_directories( Interface/Src )
include_directories( ${CMAKE_CURRENT_BINARY_DIR}/Interface/Src )

if( Boost_FOUND AND Boost_NEEDED )
	include_directories( ${Boost_INCLUDE_DIRS} )
    link_directories( ${Boost_LIBRARY_DIRS} )
endif()

#Boost Libs
set( BoostLibraries "")
FOREACH(Lib ${Boost_LIBRARIES})
	if (BoostLibraries)
		set(BoostLibraries "${BoostLibraries}|${Lib}")
	else()
		set(BoostLibraries "${Lib}")
	endif()
ENDFOREACH()

if( MSVC )
	# We remove the existing /Zm flag to replace it by /Zm300
	string( REGEX REPLACE ".*(/Zm[0-9]+).*" "\\1" ZmFlag ${CMAKE_CXX_FLAGS} )
	if( (NOT "${ZmFlag}" STREQUAL "") AND (NOT "${CMAKE_CXX_FLAGS}" STREQUAL "${ZmFlag}") )
		string( REPLACE ${ZmFlag} "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} )
	endif()
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zm300" )
endif()

msg_debug( "CMAKE_BUILD_TYPE : ${CMAKE_BUILD_TYPE}")
msg_debug( "CMAKE_C_FLAGS    : ${CMAKE_C_FLAGS}" )
msg_debug( "CMAKE_CXX_FLAGS  : ${CMAKE_CXX_FLAGS}" )

SET( MinLibraries
	${BoostLibraries}
)

SET( BinsDependencies "" )

set( DATABASE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} )

if( Boost_FOUND )
	add_subdirectory( Interface )
	add_subdirectory( Plugin )
	add_subdirectory( Test )

	PluginInit()

	PluginBuild()

	SET( msg "Database following plugins will be built" )
	PluginSummary( ${msg} )
	MESSAGE( STATUS ${msg} )
else()
	SET( msg "Database projects can't be built because of following missing libraries :" )
	if( Boost_NEEDED AND NOT Boost_FOUND )
		SET( msg "${msg}\n\tBoost" )
	endif()
	MESSAGE( SEND_ERROR ${msg} )
endif()
