project( Database )

set( DBI_VERSION_MAJOR	0 )
set( DBI_VERSION_MINOR	1 )
set( DBI_VERSION_BUILD	0 )

set( BinsDependencies
    ${BinsDependencies}
    Database
)

include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

add_target(
    Database
    api_dll
    ""
    "${MinLibraries}"
    "DatabasePch.h"
    "DatabasePch.cpp"
    ""	""	""
)

if ( WIN32 )
	include( InstallRequiredSystemLibraries )
endif ()

copy_dll( ${PROJECT_NAME} "${Boost_DATE_TIME_LIBRARY_DEBUG}" Debug )
copy_dll( ${PROJECT_NAME} "${Boost_DATE_TIME_LIBRARY_RELEASE}" Release )
copy_dll( ${PROJECT_NAME} "${Boost_FILESYSTEM_LIBRARY_DEBUG}" Debug )
copy_dll( ${PROJECT_NAME} "${Boost_FILESYSTEM_LIBRARY_RELEASE}" Release )
copy_dll( ${PROJECT_NAME} "${Boost_LOCALE_LIBRARY_DEBUG}" Debug )
copy_dll( ${PROJECT_NAME} "${Boost_LOCALE_LIBRARY_RELEASE}" Release )
copy_dll( ${PROJECT_NAME} "${Boost_SYSTEM_LIBRARY_DEBUG}" Debug )
copy_dll( ${PROJECT_NAME} "${Boost_SYSTEM_LIBRARY_RELEASE}" Release )

set_property( TARGET Database PROPERTY FOLDER "Interface" )
add_target_astyle( Database ".h;.hpp;.inl;.cpp" )