project( Database )

set( PROJECT_DESC "DatabaseConnector Interface" )
set( ${PROJECT_NAME}_VERSION_MAJOR	0 )
set( ${PROJECT_NAME}_VERSION_MINOR	1 )
set( ${PROJECT_NAME}_VERSION_BUILD	0 )

set( BinsDependencies
	${BinsDependencies}
	${PROJECT_NAME}
)

include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

add_target(
	${PROJECT_NAME}
	api_dll
	""
	"${MinLibraries}"
	"DatabasePch.h"
	"DatabasePch.cpp"
	""	""	""
)

if ( WIN32 )
	include( InstallRequiredSystemLibraries )
	target_link_libraries( ${PROJECT_NAME} Dbghelp )
else ()
	target_link_libraries( ${PROJECT_NAME} dl )
endif ()

copy_dll( ${PROJECT_NAME} "${Boost_DATE_TIME_LIBRARY_DEBUG}" Debug )
copy_dll( ${PROJECT_NAME} "${Boost_DATE_TIME_LIBRARY_RELEASE}" Release )
copy_dll( ${PROJECT_NAME} "${Boost_FILESYSTEM_LIBRARY_DEBUG}" Debug )
copy_dll( ${PROJECT_NAME} "${Boost_FILESYSTEM_LIBRARY_RELEASE}" Release )
copy_dll( ${PROJECT_NAME} "${Boost_LOCALE_LIBRARY_DEBUG}" Debug )
copy_dll( ${PROJECT_NAME} "${Boost_LOCALE_LIBRARY_RELEASE}" Release )
copy_dll( ${PROJECT_NAME} "${Boost_SYSTEM_LIBRARY_DEBUG}" Debug )
copy_dll( ${PROJECT_NAME} "${Boost_SYSTEM_LIBRARY_RELEASE}" Release )

set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Interface" )
add_target_astyle( ${PROJECT_NAME} ".h;.hpp;.inl;.cpp" )
