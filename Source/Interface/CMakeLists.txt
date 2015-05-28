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

set_property( TARGET Database PROPERTY FOLDER "Interface" )
add_target_astyle( Database ".h;.hpp;.inl;.cpp" )