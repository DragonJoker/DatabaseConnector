option( BUILD_MySQLConnPP_PLUGIN "Build MySQL Connector C++ plugin" TRUE )
option( BUILD_ODBC_PLUGIN "Build ODBC plugin" TRUE )
option( BUILD_SQLite_PLUGIN "Build SQLite plugin" TRUE )

function( PluginInit )
	set( MySql "no (Not wanted)" PARENT_SCOPE )
	set( Odbc "no (Not wanted)" PARENT_SCOPE )
	set( Sqlite "no (Not wanted)" PARENT_SCOPE )
endfunction( PluginInit )

function( PluginBuild )
	if( ${BUILD_MySQLConnPP_PLUGIN} )
        find_package( MySQL )
        if( MYSQL_FOUND )
            set( Build ${MySql} )
            add_subdirectory( Plugin/MySql )
            set( MySql ${Build} PARENT_SCOPE )
        else()
			set( ${BUILD_MySQLConnPP_PLUGIN} FALSE PARENT_SCOPE )
            set( MySql "no (missing MySQL connector C++ library" PARENT_SCOPE )
        endif()
	endif()
    if( ${BUILD_ODBC_PLUGIN} )
        find_package( ODBC )
        if( ODBC_FOUND )
            set( Build ${Odbc} )
            add_subdirectory( Plugin/Odbc )
            set( Odbc ${Build} PARENT_SCOPE )
        else()
			set( ${BUILD_ODBC_PLUGIN} FALSE PARENT_SCOPE )
            set( Odbc "no (missing ODBC library" PARENT_SCOPE )
        endif()
    endif()
    if( ${BUILD_SQLite_PLUGIN} )
		set( Build ${Sqlite} )
		add_subdirectory( Plugin/SQLite )
		set( Sqlite ${Build} PARENT_SCOPE )
    endif()
endfunction( PluginBuild )

function( PluginSummary msg )
	set( msg_tmp "\n\nPlugins :" )
	set( msg_tmp "${msg_tmp}\n    MySQL Connector C++   ${MySql}" )
	set( msg_tmp "${msg_tmp}\n    ODBC                  ${Odbc}" )
	set( msg_tmp "${msg_tmp}\n    SQLite                ${Sqlite}" )
	SET( msg "${msg}${msg_tmp}" PARENT_SCOPE )
endfunction( PluginSummary )

function( PluginSetup )

	if( ${BUILD_SQLite_PLUGIN} )
		cpack_add_component( DatabasePluginSqlite
			DISPLAY_NAME "SQLite plugin"
			DESCRIPTION "SQLite plugin to use with ${MAIN_PROJECT_NAME}"
			REQUIRED
			GROUP DatabasePlugins
			INSTALL_TYPES Minimal Full
		)
	endif ()
		message( STATUS "BUILD_MySQLConnPP_PLUGIN ${BUILD_MySQLConnPP_PLUGIN}" )
		message( STATUS "MYSQL_FOUND ${MYSQL_FOUND}" )

	if( ${BUILD_MySQLConnPP_PLUGIN} )
		message( STATUS "BUILD_MySQLConnPP_PLUGIN" )
		cpack_add_component( DatabasePluginMySql
			DISPLAY_NAME "MySQL plugin"
			DESCRIPTION "MySQL plugin to use with ${MAIN_PROJECT_NAME}"
			DEPENDS Database
			GROUP DatabasePlugins
			INSTALL_TYPES Minimal Full
		)
	endif ()

	if( ${BUILD_ODBC_PLUGIN} )
		cpack_add_component( DatabasePluginOdbc
			DISPLAY_NAME "ODBC plugins library SDK"
			DESCRIPTION "ODBC base library for ODBC plugins to use with ${MAIN_PROJECT_NAME}"
			GROUP Development
			DEPENDS Database
			INSTALL_TYPES Developer Full
		)
		cpack_add_component( DatabasePluginOdbc_dev
			DISPLAY_NAME "ODBC plugins library SDK"
			DESCRIPTION "ODBC base library for ODBC plugins to use with ${MAIN_PROJECT_NAME}"
			GROUP Development
			DEPENDS DatabasePluginOdbc
			INSTALL_TYPES Developer Full
		)
		cpack_add_component( DatabasePluginOdbcMsSql
			DISPLAY_NAME "ODBC MSSQL plugin"
			DESCRIPTION "ODBC MSSQL plugin to use with ${MAIN_PROJECT_NAME}"
			DEPENDS Database
			GROUP DatabasePlugins
			INSTALL_TYPES Minimal Full
		)
		cpack_add_component( DatabasePluginOdbcMySql
			DISPLAY_NAME "ODBC MySQL plugin"
			DESCRIPTION "ODBC MySQL plugin to use with ${MAIN_PROJECT_NAME}"
			DEPENDS Database
			GROUP DatabasePlugins
			INSTALL_TYPES Minimal Full
		)
	endif ()
endfunction( PluginSetup )
