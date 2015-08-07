/************************************************************************//**
 * @file DatabaseTestPluginsStaticLoader.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 16/06/2014 12:04:36
 *
 *
 * @brief CTestPluginsLoader class
 *
 * @details Class used to load pluginsm dynamically or statically
 *
 ***************************************************************************/

#ifndef ___DATABASE_TEST_STATIC_LOADER_H___
#define ___DATABASE_TEST_STATIC_LOADER_H___

#include "DatabaseTestPrerequisites.h"

#if defined( STATIC_LIB )
#   include <PluginDatabaseMySql.h>
#   include <PluginDatabaseOdbcMySql.h>
#   include <PluginDatabaseOdbcMsSql.h>
#endif

BEGIN_NAMESPACE_DATABASE_TEST
{
	/** Structure holding the needed loading informations about one plugin
	*/
	class CPluginConfigBase
	{
	public:
		/** Constructor
		*/
		CPluginConfigBase( bool load );

		/** Destructor
		*/
		virtual ~CPluginConfigBase();

		/** Loads the plugin, if needed
		*/
		virtual void Load() = 0;

		/** Unloads the plugin, if needed
		*/
		virtual void Unload() = 0;

		//! Tells if plugin is to load/loaded
		bool _load = false;
	};

#if defined( STATIC_LIB )
	template< typename PluginType >
	class CPluginConfig
		: public CPluginConfigBase
	{
	public:
		/** Constructor
		*/
		CPluginConfig( bool load );

		/** Destructor
		*/
		virtual ~CPluginConfig();

		/** Loads the plugin, if needed
		*/
		void Load();

		/** Unloads the plugin, if needed
		*/
		void Unload();

		//! The plugin
		PluginType * _plugin = NULL;
	};
#else
	/** Structure holding the needed loading informations about one plugin
	*/
	class CPluginConfig
		: public CPluginConfigBase
	{
	public:
		/** Constructor
		*/
		CPluginConfig( bool load, const String & path, const String & name );

		/** Destructor
		*/
		virtual ~CPluginConfig();

		/** Loads the plugin, if needed
		*/
		void Load();

		/** Unloads the plugin, if needed
		*/
		void Unload();

		//! The plugins path
		String _path;
		//! The plugin name
		String _plugin;
	};
#endif

	/** Structure holding the needed informations about plugins to load, and plugins path
	*/
	struct SPluginsConfig
	{
		//! The MySQL plugin
		std::unique_ptr< CPluginConfigBase > _mySql;
		//! The SQLite plugin
		std::unique_ptr< CPluginConfigBase > _sqlite;
		//! The ODBC MySQL plugin
		std::unique_ptr< CPluginConfigBase > _odbcMySql;
		//! The ODBC MSSQL plugin
		std::unique_ptr< CPluginConfigBase > _odbcMsSql;
		//! The PostgreSQL plugin
		std::unique_ptr< CPluginConfigBase > _postgreSql;
	};

	/** Class used to load pluginsm dynamically or statically
	*/
	class CTestPluginsLoader
	{
	public:
		/** Default constructor
		*/
		CTestPluginsLoader();

		/** Destructor
		*/
		virtual ~CTestPluginsLoader();

		/** Load the plugins wanted in the given config
		@param[in] config
			The config
		*/
		void Load( SPluginsConfig && config );

		/** Unloads every loaded plugin
		*/
		void Unload();

	private:
		/** Load all the needed plugins.
		*/
		void OnLoad();

		/** Unload all the needed plugins.
		*/
		void OnUnload();

		//! The current configuration
		SPluginsConfig _config;
	};
}
END_NAMESPACE_DATABASE_TEST

#endif //___DATABASE_TEST_STATIC_LOADER_H___

