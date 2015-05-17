/************************************************************************//**
 * @file DllPluginDatabaseSqlite.cpp
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

#include "DatabaseSqlitePch.h"

#include "PluginDatabaseSqlite.h"

#include <DatabasePluginManager.h>

BEGIN_NAMESPACE_DATABASE_SQLITE
{
	CPluginDatabaseSqlite * plugin = NULL;

#ifndef STATIC_LIB
	extern "C" void DatabaseSqliteExport DllStartPlugin()
	{
		///@remarks Create plugin.
		plugin = new CPluginDatabaseSqlite();

		///@remarks Register plugin.
		CPluginManager::Instance().InstallPlugin( plugin );
	}

	extern "C" void DatabaseSqliteExport DllStopPlugin()
	{
		///@remarks Unregister plugin.
		CPluginManager::Instance().UninstallPlugin( plugin );

		///@remarks Delete plugin.
		delete plugin;
	}
#endif

}
END_NAMESPACE_DATABASE_SQLITE
