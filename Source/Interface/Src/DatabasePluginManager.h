/************************************************************************//**
* @file PluginManager.h
* @author spastor
* @version 1.0
* @date 2/11/2013 11:38:04 AM
*
*
* @brief Manager for plugins.
*
* @details This manager is used to load and unload plugins.
*
***************************************************************************/

#ifndef ___DATABASE_PLUGIN_MANAGER_H___
#define ___DATABASE_PLUGIN_MANAGER_H___

#include "DatabasePrerequisites.h"

BEGIN_NAMESPACE_DATABASE
{
	/** Manager for plugins.
	@remarks
		This manager is used to load and unload plugins.
		Plugins can be manually loaded or automatically from a config file ( i.e plugins.cfg ).
	*/
	class CPluginManager
	{
	public:
		/** Initialise all loaded plugins - allows plugins to perform
			actions once the renderer is initialised.
		*/
		DatabaseExport void InitialisePlugins();

		/** Shuts down all loaded plugins - allows things to be tidied up whilst
			all plugins are still loaded.
		*/
		DatabaseExport void ShutdownPlugins();

		/** Unload all loaded plugins.
		*/
		DatabaseExport void UnloadPlugins();

		/** Manually load a CPlugin contained in a DLL / DSO.
			@remarks
			Embedded plugins in DLLs can be loaded at startup using the plugin
			configuration specified file (default: plugins.cfg).
			This method allows you to load plugin DLLs directly in code.
			The DLL in question is expected to implement a DllStartPlugin method
			which instantiates a CPlugin subclass and calls CPluginManager::InstallPlugin.
			It should also implement DllStopPlugin (see CPluginManager::UnloadPlugin).
		@param pluginName
			Name of the plugin library to load.
		*/
		DatabaseExport void LoadPlugin( const String & pluginName );

		/** Install a new plugin.
		@remarks
			This function installs a new extension.
			The plugin itself may be loaded from a DLL / DSO, or it might be
			statically linked into your own  application.
			Either way, you need to call this method to get it registered
			and functioning. You should only call this method directly
			if your plugin is not in a DLL that could otherwise be loaded with
			LoadPlugin, since the DLL function DllStartPlugin should call this
			method when the DLL is loaded.
		*/
		DatabaseExport void InstallPlugin( CPluginDatabase * plugin );

		/** Uninstall an existing plugin.
		@remarks
			This function uninstalls an extension.
			Plugins are automatically uninstalled at shutdown but this lets you
			remove them early. If the plugin was loaded from a DLL / DSO
			you should call UnloadPlugin which should result in this method
			getting called anyway (if the DLL is well behaved).
		*/
		DatabaseExport void UninstallPlugin( CPluginDatabase * plugin );

		/** Manually unload a CPlugin contained in a DLL / DSO.
			@remarks
			CPlugin DLLs are automatically unloaded at shutdown.
			This method allows you to unload plugins in code,
			but make sure their dependencies are decoupled first.
			This method will call the DllStopPlugin method defined in the DLL,
			which in turn should call CPluginManager::UninstallPlugin.
		@param pluginName
			Name of the plugin library to unload
		*/
		DatabaseExport void UnloadPlugin( const String & pluginName );

		/** Sets the application (main execution) path
		@param path
			The path
		*/
		inline void SetApplicationPath( const String & path )
		{
			_applicationPath = path;
		}

		/** Sets the plugins path
		@param path
			The path
		*/
		inline void SetPluginsPath( const String & path )
		{
			_pluginsPath = path;
		}

		/** Sets the translations path
		@param path
			The path
		*/
		inline void SetTranslationsPath( const String & path )
		{
			_translationsPath = path;
		}

		/** Retrieves the application (main execution) path
		@return
			The path
		*/
		inline const String & GetApplicationPath()const { return _applicationPath; }

		/** Retrieves the plugins path
		@return
			The path
		*/
		inline const String & GetPluginsPath()const { return _pluginsPath; }

		/** Retrieves the translations path
		@return
			The path
		*/
		inline const String & GetTranslationsPath()const { return _translationsPath; }

		/** Retrieves the unique instance
		*/
		DatabaseExport static CPluginManager & Instance()
		{
			static CPluginManager singleton;
			return singleton;
		}

	protected:
		/** Default constructor.
		@note
			Should never be called as the singleton is automatically
			created during the creation of application.
		*/
		DatabaseExport CPluginManager();

		/** Destructor.
		*/
		DatabaseExport virtual ~CPluginManager();

	protected:
		//! List of plugin DLLs loaded.
		std::vector< CDynLib * > _pluginLibs;
		//! List of CPlugin instances registered.
		std::vector< CPluginDatabase * > _plugins;
		//! Application (main execution) path
		String _applicationPath;
		//! Plugins path, by default equal to application path
		String _pluginsPath;
		//! Translations path, by default equal to application path
		String _translationsPath;
	};
}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_PLUGIN_MANAGER_H___
