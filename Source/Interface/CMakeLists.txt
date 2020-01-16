project( Database )

set( PROJECT_DESC "DatabaseConnector Interface" )
set( ${PROJECT_NAME}_VERSION_MAJOR	0 )
set( ${PROJECT_NAME}_VERSION_MINOR	1 )
set( ${PROJECT_NAME}_VERSION_BUILD	0 )

set( BinsDependencies
	${BinsDependencies}
	${PROJECT_NAME}
)

if ( MSVC )
	set( DBG_LIBS DbgHelp )
endif ()

add_target(
	${PROJECT_NAME}
	api_dll
	""
	""
	"DatabasePch.h"
	"DatabasePch.cpp"
	""	""	""
)
add_library( dbConnector::${PROJECT_NAME}
	ALIAS
	${PROJECT_NAME}
)
target_include_directories( ${PROJECT_NAME}
	PUBLIC
		${Boost_INCLUDE_DIR}
		${CMAKE_SOURCE_DIR}/Interface/Src
		${CMAKE_BINARY_DIR}/Interface
)
target_link_libraries( ${PROJECT_NAME}
	PUBLIC
		${Boost_LIBRARIES}
		${CMAKE_DL_LIBS}
		${DBG_LIBS}
)

if ( WIN32 )
	include( InstallRequiredSystemLibraries )
endif ()

set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Interface" )
add_target_astyle( ${PROJECT_NAME} ".h;.hpp;.inl;.cpp" )
