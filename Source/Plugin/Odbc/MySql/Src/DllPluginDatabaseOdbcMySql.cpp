/************************************************************************//**
* @file DllPluginDatabaseOdbcMySql.cpp
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

#include "DatabaseOdbcMySqlPch.h"

#include "PluginDatabaseOdbcMySql.h"

#include <DatabasePluginManager.h>

#if defined( _WIN32 ) && !defined ( __MINGW32__ ) && !defined ( STATIC_LIB )
#	ifdef DatabasePluginOdbcMySql_EXPORTS
#	    define DatabaseOdbcMySqlExport __declspec ( dllexport )
#	else
#		if defined ( __MINGW32__ )
#			define DatabaseOdbcMySqlExport
#		else
#			define DatabaseOdbcMySqlExport __declspec ( dllimport )
#		endif
#	endif
#else
#    define DatabaseOdbcMySqlExport
#endif

BEGIN_NAMESPACE_DATABASE_ODBC_MYSQL
{
	CPluginDatabaseOdbcMySql * plugin = NULL;

#ifndef STATIC_LIB
	extern "C" void DatabaseOdbcMySqlExport DllStartPlugin()
	{
		//!@remarks Create plugin.
		plugin = new CPluginDatabaseOdbcMySql();

		//!@remarks Register plugin.
		CPluginManager::Instance().InstallPlugin( plugin );
	}

	extern "C" void DatabaseOdbcMySqlExport DllStopPlugin()
	{
		//!@remarks Unregister plugin.
		CPluginManager::Instance().UninstallPlugin( plugin );

		//!@remarks Delete plugin.
		delete plugin;
	}
#endif
}
END_NAMESPACE_DATABASE_ODBC_MYSQL
