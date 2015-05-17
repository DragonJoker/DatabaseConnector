/************************************************************************//**
 * @file DllPluginDatabaseMySql.cpp
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

#include "DatabaseMySqlPch.h"

#include "PluginDatabaseMySql.h"

#include <DatabasePluginManager.h>

BEGIN_NAMESPACE_DATABASE_MYSQL
{
	CPluginDatabaseMySql * plugin = NULL;

#ifndef STATIC_LIB
	extern "C" void DatabaseMySqlExport DllStartPlugin()
	{
		///@remarks Create plugin.
		plugin = new CPluginDatabaseMySql();

		///@remarks Register plugin.
		CPluginManager::Instance().InstallPlugin( plugin );
	}

	extern "C" void DatabaseMySqlExport DllStopPlugin()
	{
		///@remarks Unregister plugin.
		CPluginManager::Instance().UninstallPlugin( plugin );

		///@remarks Delete plugin.
		delete plugin;
	}
#endif

}
END_NAMESPACE_DATABASE_MYSQL
