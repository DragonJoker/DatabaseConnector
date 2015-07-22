/************************************************************************//**
* @file DllPluginDatabaseOdbcMsSql.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 4/8/2014 4:01:00 PM
*
*
* @brief DLL Interface.
*
* @details DLL Interface.
*
***************************************************************************/

#include "DatabaseOdbcMsSqlPch.h"

#include "PluginDatabaseOdbcMsSql.h"

#include <DatabasePluginManager.h>

#if defined( _WIN32 ) && !defined ( __MINGW32__ ) && !defined ( STATIC_LIB )
#	ifdef DatabasePluginOdbcMsSql_EXPORTS
#	    define DatabaseOdbcMsSqlExport __declspec ( dllexport )
#	else
#		if defined ( __MINGW32__ )
#			define DatabaseOdbcMsSqlExport
#		else
#			define DatabaseOdbcMsSqlExport __declspec ( dllimport )
#		endif
#	endif
#else
#    define DatabaseOdbcMsSqlExport
#endif

BEGIN_NAMESPACE_DATABASE_ODBC_MSSQL
{
	CPluginDatabaseOdbcMsSql * plugin = NULL;

#ifndef STATIC_LIB
	extern "C" void DatabaseOdbcMsSqlExport DllStartPlugin()
	{
		//!@remarks Create plugin.
		plugin = new CPluginDatabaseOdbcMsSql();

		//!@remarks Register plugin.
		CPluginManager::Instance().InstallPlugin( plugin );
	}

	extern "C" void DatabaseOdbcMsSqlExport DllStopPlugin()
	{
		//!@remarks Unregister plugin.
		CPluginManager::Instance().UninstallPlugin( plugin );

		//!@remarks Delete plugin.
		delete plugin;
	}
#endif
}
END_NAMESPACE_DATABASE_ODBC_MSSQL
