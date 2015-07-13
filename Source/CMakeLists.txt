cmake_minimum_required( VERSION 2.8.10 )
#--------------------------------------------------------------------------------------------------
#	Initial configurations
#--------------------------------------------------------------------------------------------------
# Set project name, used in folders and in workspace creation
set( MAIN_PROJECT_NAME "DatabaseConnector" )

# Set project version numbers
set( VERSION_MAJOR	0		)
set( VERSION_MINOR	1		)
set( VERSION_BUILD	0		)
set( VERSION_YEAR	2014	)

# Used to look for external modules
set( CMAKE_MODULE_PATH
	${CMAKE_SOURCE_DIR}/CMake
	${CMAKE_SOURCE_DIR}/CMake/Modules
)
set( CMAKE_TEMPLATES_DIR ${CMAKE_SOURCE_DIR}/Templates )
# Experimental Precompiled headers support for GCC
include( PCHSupport )

set( CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Set only Debug and Release configuration types" FORCE )

# Enable CTest
enable_testing()
    
# Declare the project
project( ${MAIN_PROJECT_NAME} )

include( Setup )
include( Project )
include( CompilerVersion )
include( UnitTest )
include( CompilationFlags )
include( AStyleUtils )
include( ExternalDependencies )

option( BUILD_STATIC "Build Static" FALSE )

# Organize projects into folders
set_property( GLOBAL PROPERTY USE_FOLDERS ON )

#--------------------------------------------------------------------------------------------------
#	Looking for needed libraries
#--------------------------------------------------------------------------------------------------
set( Boost_USE_STATIC_LIBS OFF )
set( Boost_USE_STATIC_RUNTIME OFF )
set( Boost_USE_MULTITHREADED ON )

set( Boost_NEEDED TRUE )

if (CMAKE_COMPILER_IS_GNUCXX)
	DumpCompilerVersion( COMPILER_VERSION )
	msg_debug( "GCC version : ${COMPILER_VERSION}" )
	if (${COMPILER_VERSION} LESS "46" OR MINGW)
		find_package( Boost COMPONENTS filesystem thread system chrono date_time locale )
	else()
		find_package( Boost COMPONENTS filesystem system date_time locale )
	endif ()
elseif ( CMAKE_CXX_COMPILER_ID MATCHES "Clang" )
	COMPILER_DUMPVERSION( COMPILER_VERSION )
	msg_debug( "clang version : ${COMPILER_VERSION}" )
    find_package( Boost COMPONENTS filesystem system date_time locale )
elseif (BORLAND)
	find_package( Boost COMPONENTS thread filesystem system date_time locale )
elseif (NOT MSVC11 AND NOT MSVC12)
	find_package( Boost COMPONENTS thread filesystem system chrono date_time locale )
else()
		find_package( Boost COMPONENTS filesystem system date_time locale )
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

	PluginInit()

	PluginBuild()

	add_subdirectory( Test )

	SET( msg "Database following plugins will be built" )
	PluginSummary( ${msg} )
	MESSAGE( STATUS ${msg} )
	
	if ( ( "${PROJECTS_PACKAGE_WIX}" STREQUAL "ON" ) OR ( "${PROJECTS_PACKAGE_DEB}" STREQUAL "ON" ) OR ( "${PROJECTS_PACKAGE_RPM}" STREQUAL "ON" ) )
		get_filename_component( PARENT_DIR ${CMAKE_SOURCE_DIR} PATH )
		file( MAKE_DIRECTORY "${PROJECTS_SETUP_OUTPUT_DIR}/${CMAKE_BUILD_TYPE}" )
		set( CPACK_PACKAGE_NAME "${MAIN_PROJECT_NAME}" )
		set( CPACK_PACKAGE_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_BUILD}" )
		set( CPACK_PACKAGE_DESCRIPTION_SUMMARY "${MAIN_PROJECT_NAME}" )
		set( CPACK_PACKAGE_DESCRIPTION "Allows ${MAIN_PROJECT_NAME} developments" )
		set( CPACK_PACKAGE_RELEASE 1 )
		set( CPACK_PACKAGE_CONTACT "dragonjoker59@hotmail.com" )
		set( CPACK_PACKAGE_VENDOR "DragonJoker" )
		set( CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_PACKAGE_RELEASE}.${PROJECTS_PLATFORM}" )
		set( CPACK_OUTPUT_FILE_PREFIX "${PROJECTS_SETUP_OUTPUT_DIR}/${CMAKE_BUILD_TYPE}" )
		set( CPACK_RESOURCE_FILE_LICENSE "${PARENT_DIR}/LGPL.txt" )

		if ( "${PROJECTS_PACKAGE_WIX}" STREQUAL "ON" )
			set( CPACK_GENERATOR "WIX" )
			set( CPACK_PACKAGE_INSTALL_DIRECTORY "${MAIN_PROJECT_NAME}" )
			set( CPACK_WIX_UPGRADE_GUID "3493BBBD-2073-4B4C-A7D6-8B5F8E0E55C4" )
			set( CPACK_WIX_PRODUCT_GUID "8508AB62-560C-46CA-9EBC-DD48CCECCC36" )
			set( CPACK_WIX_PRODUCT_ICON "${PARENT_DIR}/install/${MAIN_PROJECT_NAME}.ico" )
			set( CPACK_WIX_UI_DIALOG "${PARENT_DIR}/install/dialog.bmp" )
			set( CPACK_WIX_UI_BANNER "${PARENT_DIR}/install/banner.bmp" )
			set( CPACK_WIX_PROGRAM_MENU_FOLDER "${MAIN_PROJECT_NAME}" )
			set( CPACK_WIX_CULTURES "fr-FR;en-EN" )
			include(CPack)
		elseif ( "${PROJECTS_PACKAGE_DEB}" STREQUAL "ON" )
			set( CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX} )
			set( CPACK_DEB_COMPONENT_INSTALL ON )
			set( CPACK_GENERATOR "DEB" )
			set( CPACK_DEBIAN_PACKAGE_MAINTAINER "Sylvain Doremus" )
			set( CPACK_DEBIAN_PACKAGE_DEPENDS "${CPACK_DEBIAN_PACKAGE_DEPENDS}libogre (>= 2.4.2)" )
			set( CPACK_DEBIAN_PACKAGE_ARCHITECTURE "${CMAKE_SYSTEM_ARCHITECTURE}" )
			set( CPACK_DEBIAN_PACKAGE_SECTION "devel" )
			set( CPACK_DEBIAN_PACKAGE_PRIORITY "optional" )
			include(CPack)
		elseif ( "${PROJECTS_PACKAGE_RPM}" STREQUAL "ON" )
			set( ${PROJECT_PACKAGE_DIR} ${CMAKE_BINARY_DIR}/_CPack_Packages/Linux/RPM )
			set( CPACK_RPM_COMPONENT_INSTALL ON )
			set( CPACK_RPM_PACKAGE_SUMMARY "${MAIN_PROJECT_NAME}" )
			set( CPACK_RPM_PACKAGE_ARCHITECTURE "${CMAKE_SYSTEM_ARCHITECTURE}" )
			set( CPACK_RPM_PACKAGE_GROUP "Development/Tools/Other" )
			set( CPACK_RPM_PACKAGE_LICENSE "LGPL" )
			set( CPACK_RPM_SPEC_MORE_DEFINE "%define _rpmdir ${PROJECT_PACKAGE_DIR}" )
			set( CPACK_RPM_SPEC_MORE_DEFINE "%define _rpmfilename ${CPACK_PACKAGE_FILE_NAME}.rpm" )
			set( CPACK_RPM_SPEC_MORE_DEFINE "%define _unpackaged_files_terminate_build 0" )
			set( CPACK_RPM_SPEC_MORE_DEFINE "%define _topdir ${PROJECT_PACKAGE_DIR}" )
			include(CPack)
		endif()
	endif ()

	cpack_add_install_type( Minimal )
	cpack_add_install_type( Developer )
	cpack_add_install_type( Full )
	
	cpack_add_component_group( Core
		EXPANDED
		DISPLAY_NAME "Core libraries"
		DESCRIPTION "Base libraries needed to use ${MAIN_PROJECT_NAME}"
	)
	cpack_add_component_group( DatabasePlugins
		DISPLAY_NAME "Plugins"
		DESCRIPTION "Database plugins to use with ${MAIN_PROJECT_NAME}"
	)
	cpack_add_component_group( Development
		DISPLAY_NAME "Development libraries and headers"
		DESCRIPTION "All the tools needed to develop a project using ${MAIN_PROJECT_NAME}"
	)
	cpack_add_component_group( Documentation
		DISPLAY_NAME "Documentation"
		DESCRIPTION "Projects documentations"
	)

	cpack_add_component( Database
		DISPLAY_NAME "${MAIN_PROJECT_NAME} interface"
		DESCRIPTION "Base ${MAIN_PROJECT_NAME} interface"
		REQUIRED
		GROUP Core
		INSTALL_TYPES Minimal Full
	)
	cpack_add_component( Database_dev
		DISPLAY_NAME "${MAIN_PROJECT_NAME} SDK"
		DESCRIPTION "Base ${MAIN_PROJECT_NAME} interface development libraries and includes"
		GROUP Development
		INSTALL_TYPES Developer Full
	)

	cpack_add_component( DatabaseTest
		DISPLAY_NAME "${MAIN_PROJECT_NAME} test application"
		DESCRIPTION "Base ${MAIN_PROJECT_NAME} test application"
		REQUIRED
		GROUP Development
		INSTALL_TYPES Developer Full
	)
	
	PluginSetup()
else()
	SET( msg "Database projects can't be built because of following missing libraries :" )
	if( Boost_NEEDED AND NOT Boost_FOUND )
		SET( msg "${msg}\n\tBoost" )
	endif()
	MESSAGE( SEND_ERROR ${msg} )
endif()
