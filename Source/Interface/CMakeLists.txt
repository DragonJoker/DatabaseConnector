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

set( ${PROJECT_NAME}_HDR_FILES
	Src/Database.h
	Src/DatabaseBlockGuard.h
	Src/DatabaseConnection.h
	Src/DatabaseDate.h
	Src/DatabaseDateTime.h
	Src/DatabaseDateTimeHelper.h
	Src/DatabaseDynLib.h
	Src/DatabaseDynLibManager.h
	Src/DatabaseException.h
	Src/DatabaseFactory.h
	Src/DatabaseFactoryManager.h
	Src/DatabaseField.h
	Src/DatabaseFileUtils.h
	Src/DatabaseFixedPoint.h
	Src/DatabaseInt24.h
	Src/DatabaseLogger.h
	Src/DatabaseLoggerConsole.h
	Src/DatabaseLoggerImpl.h
	Src/DatabaseMacros.h
	Src/DatabaseNullable.h
	Src/DatabaseParameter.h
	Src/DatabaseParameteredObject.h
	Src/DatabasePlugin.h
	Src/DatabasePluginManager.h
	Src/DatabasePrerequisites.h
	Src/DatabaseQuery.h
	Src/DatabaseResult.h
	Src/DatabaseRow.h
	Src/DatabaseStatement.h
	Src/DatabaseStringUtils.h
	Src/DatabaseTime.h
	Src/DatabaseValue.h
	Src/DatabaseValuedObject.h
	Src/DatabaseValuedObjectInfos.h
	Src/DatabaseValuePolicy.h
	Src/EErrorType.h
	Src/EFieldType.h
	Src/ELogType.h
	Src/EParameterType.h
	Src/DatabaseField.inl
	Src/DatabaseFixedPoint.inl
	Src/DatabaseInt24.inl
	Src/DatabaseParameteredObject.inl
	Src/DatabaseRow.inl
	Src/DatabaseStatement.inl
	Src/DatabaseValue.inl
	Src/DatabaseValuedObject.inl
)
source_group( "Header Files"
	FILES
		${${PROJECT_NAME}_HDR_FILES}
)
set( ${PROJECT_NAME}_SRC_FILES
	Src/Database.cpp
	Src/DatabaseConnection.cpp
	Src/DatabaseDate.cpp
	Src/DatabaseDateTime.cpp
	Src/DatabaseDateTimeHelper.cpp
	Src/DatabaseDynLib.cpp
	Src/DatabaseDynLibManager.cpp
	Src/DatabaseException.cpp
	Src/DatabaseFactory.cpp
	Src/DatabaseFactoryManager.cpp
	Src/DatabaseField.cpp
	Src/DatabaseFileUtils.cpp
	Src/DatabaseFixedPoint.cpp
	Src/DatabaseInt24.cpp
	Src/DatabaseLogger.cpp
	Src/DatabaseLoggerConsole.cpp
	Src/DatabaseLoggerImpl.cpp
	Src/DatabaseParameter.cpp
	Src/DatabaseParameteredObject.cpp
	Src/DatabasePlugin.cpp
	Src/DatabasePluginManager.cpp
	Src/DatabaseQuery.cpp
	Src/DatabaseResult.cpp
	Src/DatabaseRow.cpp
	Src/DatabaseStatement.cpp
	Src/DatabaseStringUtils.cpp
	Src/DatabaseTime.cpp
	Src/DatabaseValue.cpp
	Src/DatabaseValuedObject.cpp
	Src/DatabaseValuedObjectInfos.cpp
)
source_group( "Source Files"
	FILES
		${${PROJECT_NAME}_SRC_FILES}
)
if ( WIN32 )
	find_rsc_file(
		${PROJECT_NAME}
		api_dll
	)
	set( ${PROJECT_NAME}_SRC_FILES
		${${PROJECT_NAME}_SRC_FILES}
		${${PROJECT_NAME}_RSC_FILES}
	)
endif ()
add_target_min(
	${PROJECT_NAME}
	api_dll
	"Src/DatabasePch.h"
	"Src/DatabasePch.cpp"
)
add_library( dbConnector::${PROJECT_NAME}
	ALIAS
	${PROJECT_NAME}
)
target_include_directories( ${PROJECT_NAME}
	PUBLIC
		${Boost_INCLUDE_DIR}
		$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Interface/Src>
		$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/Interface>
		$<INSTALL_INTERFACE:include>
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

install_target( ${PROJECT_NAME} api_dll Src )