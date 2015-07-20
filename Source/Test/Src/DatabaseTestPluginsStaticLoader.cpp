/************************************************************************//**
 * @file DatabaseTestPluginsStaticLoader.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 16/06/2014 12:04:46
 *
 *
 * @brief Specialization of CStaticLibLoader for Database needed plugin
 *
 * @details Loads the static library of the Database needed plugin
 *
 ***************************************************************************/

#include "DatabaseTestPch.h"

#include "DatabaseTestPluginsStaticLoader.h"

#include <DatabasePluginManager.h>

BEGIN_NAMESPACE_DATABASE_TEST
{
	CPluginConfigBase::CPluginConfigBase( bool load )
		: _load( load )
	{
	}

#if defined( STATIC_LIB )
	template< typename PluginType >
	CPluginConfig( bool load )
		: CPluginConfigBase( load )
	{
	}

	template< typename PluginType >
	void CPluginConfig< PluginType >::Load()
	{
		if ( _load && !_plugin )
		{
			//!@remarks Create the plugin.
			_plugin = new PluginType;

			//!@remarks Register the plugin.
			CPluginManager::Instance().InstallPlugin( _plugin );
		}
	}

	template< typename PluginType >
	void CPluginConfig< PluginType >::Unload()
	{
		if ( _plugin )
		{
			//!@remarks Unregister the plugin.
			CPluginManager::Instance().UninstallPlugin( _plugin );

			//!@remarks Delete the plugin.
			delete _plugin;
			_plugin = NULL;
		}
	}
#else
	namespace
	{
#if defined( _WIN32 )
		static const String LIB_PREFIX;
		static const String LIB_EXT = STR( ".dll" );
#else
		static const String LIB_PREFIX = STR( "lib" );
		static const String LIB_EXT = STR( ".so" );
#endif
#if defined( NDEBUG )
		static const String LIB_SUFFIX;
#else
		static const String LIB_SUFFIX = "d";
#endif

		String GetLibName( const String & name )
		{
			return LIB_PREFIX + name + LIB_SUFFIX + LIB_EXT;
		}
	}

	CPluginConfig::CPluginConfig( bool load, const String & path, const String & name )
		: CPluginConfigBase( load )
		, _path( path )
		, _plugin( name )
	{
	}

	void CPluginConfig::Load()
	{
		if ( _load )
		{
			CPluginManager::Instance().LoadPlugin( _path + GetLibName( _plugin ) );
		}
	}

	void CPluginConfig::Unload()
	{
		if ( _load )
		{
			CPluginManager::Instance().UnloadPlugin( _path + GetLibName( _plugin ) );
		}
	}
#endif

	CTestPluginsLoader::CTestPluginsLoader()
		: _config()
	{
	}

	CTestPluginsLoader::~CTestPluginsLoader()
	{
	}

	void CTestPluginsLoader::Load( SPluginsConfig && config )
	{
		_config._mySql = std::move( config._mySql );
		_config._sqlite = std::move( config._sqlite );
		_config._odbcMySql = std::move( config._odbcMySql );
		_config._odbcMsSql = std::move( config._odbcMsSql );
		OnLoad();
	}

	void CTestPluginsLoader::Unload()
	{
		OnUnload();
	}
	
	void CTestPluginsLoader::OnLoad()
	{
		_config._mySql->Load();
		_config._sqlite->Load();
		_config._odbcMySql->Load();
		_config._odbcMsSql->Load();
	}

	void CTestPluginsLoader::OnUnload()
	{
		_config._mySql->Unload();
		_config._sqlite->Unload();
		_config._odbcMySql->Unload();
		_config._odbcMsSql->Unload();
	}
}
END_NAMESPACE_DATABASE_TEST

