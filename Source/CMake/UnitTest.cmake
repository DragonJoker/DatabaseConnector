macro( AddUnitTest ProjectName ) # inputArgs
	if( NOT TARGET ${ProjectName} )
	    message( FATAL_ERROR "AddUnitTest was given a target name that does not exist: '${ProjectName}'!" )
	endif()

    set( InputArgs ${ARGN} )

    # Adding debug test
    add_test( NAME ${ProjectName}_TestDebug
        CONFIGURATIONS Debug
        COMMAND "${ProjectName}" ${inputArgs}
        WORKING_DIRECTORY "${EXECUTABLE_OUTPUT_PATH}/Debug"
    )

    # Adding release test
    add_test( NAME ${ProjectName}_TestRelease
        CONFIGURATIONS Release
        COMMAND "${ProjectName}" ${inputArgs}
        WORKING_DIRECTORY "${EXECUTABLE_OUTPUT_PATH}/Release"
    )

    # Adding release with debug info test
    add_test( NAME ${ProjectName}_TestRelWithDebInfo
        CONFIGURATIONS RelWithDebInfo
        COMMAND "${ProjectName}" ${inputArgs}
        WORKING_DIRECTORY "${EXECUTABLE_OUTPUT_PATH}/RelWithDebInfo"
    )

    # Adding minimal size release test
    add_test( NAME ${ProjectName}_TestMinSizeRel
        CONFIGURATIONS MinSizeRel
        COMMAND "${ProjectName}" ${inputArgs}
        WORKING_DIRECTORY "${EXECUTABLE_OUTPUT_PATH}/MinSizeRel"
    )
endmacro( AddUnitTest )
