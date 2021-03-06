/************************************************************************//**
* @file DllPluginDatabasePostgreSql.cpp
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

#include "DatabasePostgreSqlPch.h"

#include "PluginDatabasePostgreSql.h"

#include <DatabasePluginManager.h>

#if defined( _WIN32 ) && !defined ( __MINGW32__ ) && !defined ( STATIC_LIB )
#	ifdef DatabasePluginPostgreSql_EXPORTS
#		define DatabasePostgreSqlExport __declspec ( dllexport )
#	else
#		if defined ( __MINGW32__ )
#			define DatabasePostgreSqlExport
#		else
#			define DatabasePostgreSqlExport __declspec ( dllimport )
#		endif
#	endif
#else
#    define DatabasePostgreSqlExport
#endif

BEGIN_NAMESPACE_DATABASE_POSTGRESQL
{
	CPluginDatabasePostgreSql * plugin = NULL;

#ifndef STATIC_LIB
	extern "C" void DatabasePostgreSqlExport DllStartPlugin()
	{
		///@remarks Create plugin.
		plugin = new CPluginDatabasePostgreSql();

		///@remarks Register plugin.
		CPluginManager::Instance().InstallPlugin( plugin );
	}

	extern "C" void DatabasePostgreSqlExport DllStopPlugin()
	{
		///@remarks Unregister plugin.
		CPluginManager::Instance().UninstallPlugin( plugin );

		///@remarks Delete plugin.
		delete plugin;
	}
#endif
}
END_NAMESPACE_DATABASE_POSTGRESQL
