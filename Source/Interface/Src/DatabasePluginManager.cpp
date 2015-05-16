/************************************************************************//**
 * @file PluginManager.cpp
 * @author spastor
 * @version 1.0
 * @date 2/11/2013 11:38:19 AM
 * 
 *
 * @brief Manager for plugins.
 *
 * @details This manager is used to load and unload plugins.
 *
 ***************************************************************************/

#include "DatabasePch.h"

#include "DatabaseDynLib.h"
#include "DatabaseDynLibManager.h"
#include "DatabasePlugin.h"
#include "DatabasePluginManager.h"
#include "DatabaseException.h"

#include "DatabaseLogger.h"

BEGIN_NAMESPACE_DATABASE
{
    const String SYMBOL_DLL_STOP                     = STR( "DllStopPlugin" );
    const String SYMBOL_DLL_START                    = STR( "DllStartPlugin" );
    
    const String MSG_INSTALLING_PLUGIN               = STR( "Installing plugin \"%s\"" );
    const String MSG_PLUGIN_SUCCESSFULLY_INSTALLED   = STR( "CPlugin \"%s\" successfully installed" );
    const String MSG_UNINSTALLING_PLUGIN             = STR( "Uninstalling plugin \"%s\"" );
    const String MSG_PLUGIN_SUCCESSFULLY_UNINSTALLED = STR( "CPlugin \"%s\" successfully uninstalled" );
    
    const String ERROR_SYMBOL_DLL_STOP_NOT_FOUND     = STR( "Cannot find symbol DllStopPlugin in library " );
    const String ERROR_SYMBOL_DLL_START_NOT_FOUND    = STR( "Cannot find symbol DllStartPlugin in library " );
    
    const String LOGGER_NAME_CPLUGINMANAGER          = STR( "Areva.ARIA.Native.Core.CPluginManager" );
    
    typedef void ( *DLL_START_PLUGIN )();
    typedef void ( *DLL_STOP_PLUGIN )();
    
    CPluginManager::CPluginManager()
    {
    }
    
    CPluginManager::~CPluginManager()
    {
        ///@remarks Unload plugins.
        UnloadPlugins();
    }
    
    void CPluginManager::ShutdownPlugins()
    {
        ///@remarks Shutdown plugins in reverse order to enforce dependencies.
        for( std::vector< CPluginDatabase * >::reverse_iterator rit = _plugins.rbegin(); rit != _plugins.rend(); ++rit )
        {
            ( *rit )->Shutdown();
        }
    }
    
    void CPluginManager::InitializePlugins()
    {
        for( std::vector< CPluginDatabase * >::iterator it = _plugins.begin(); it != _plugins.end(); ++it )
        {
            ( *it )->Initialize();
        }
    }
    
    void CPluginManager::UnloadPlugins()
    {
        ///@remarks Unload all dynamic libs first.
        for( std::vector< CDynLib * >::reverse_iterator rit = _pluginLibs.rbegin(); rit != _pluginLibs.rend(); ++rit )
        {
            CDynLib * lib = ( *rit );
            
            ///@remarks Call plugin shutdown.
            DLL_STOP_PLUGIN func = ( DLL_STOP_PLUGIN )lib->GetSymbol( SYMBOL_DLL_STOP );
            if ( func )
            {
                ///@remarks This will call UninstallPlugin
                func();
            }
            
            ///@remarks Unload the library (destroyed by CDynLibManager).
            CDynLibManager::Instance().Unload( lib );
        }
        
        _pluginLibs.clear();
        
        ///@remarks Now deal with any remaining plugins that were registered through other means
        for( std::vector< CPluginDatabase * >::reverse_iterator rit = _plugins.rbegin(); rit != _plugins.rend(); ++rit )
        {
            ///@remarks Note this does NOT call uninstallPlugin - this shutdown is for the detail objects
            CPluginDatabase * plugin = ( *rit );
            plugin->Uninstall();
            delete plugin;
        }
        
        _plugins.clear();
    }
    
    void CPluginManager::LoadPlugin( const String & pluginName )
    {
        ///@remarks Load the plugin library.
        CDynLib * lib = CDynLibManager::Instance().Load( pluginName );
        
        ///@remarks Check for existence, because if it's called more than 2 times CDynLibManager returns the existing entry.
        std::vector< CDynLib * >::iterator it = std::find( _pluginLibs.begin(), _pluginLibs.end(), lib );
        if ( it == _pluginLibs.end() )
        {
            ///@remarks Store for later unload.
            _pluginLibs.push_back( lib );
            
            ///@remarks Call the startup function.
            DLL_START_PLUGIN func = ( DLL_START_PLUGIN ) lib->GetSymbol( SYMBOL_DLL_START );
            if ( !func )
            {
                StringStream ss;
                ss << ERROR_SYMBOL_DLL_START_NOT_FOUND << pluginName;
                DB_EXCEPT( EDatabaseExceptionCodes_ItemNotFound, ss.str() );
            }
            
            ///@remarks This must call InstallPlugin
            func();
        }
    }
    
    void CPluginManager::InstallPlugin( CPluginDatabase * plugin )
    {
        CLogger::LogDebug( ( Format( MSG_INSTALLING_PLUGIN ) % plugin->GetName() ).str() );
        
        ///@remarks Add the plugin.
        _plugins.push_back( plugin );
        
        ///@remarks Install the plugin.
        plugin->Install();
        
        ///@remarks Initialize the plugin.
        plugin->Initialize();
        
        CLogger::LogDebug( ( Format( MSG_PLUGIN_SUCCESSFULLY_INSTALLED ) % plugin->GetName() ).str() );
    }
    
    void CPluginManager::UninstallPlugin( CPluginDatabase * plugin )
    {
        CLogger::LogDebug( ( Format( MSG_UNINSTALLING_PLUGIN ) % plugin->GetName() ).str() );
        
        ///@remarks Find the plugin.
        std::vector< CPluginDatabase * >::iterator it = std::find( _plugins.begin(), _plugins.end(), plugin );
        if ( it != _plugins.end() )
        {
            ///@remarks Shutdown the plugin.
            plugin->Shutdown();
            
            ///@remarks Uninstall the plugin.
            plugin->Uninstall();
            
            ///@remarks Remove the plugin.
            _plugins.erase( it );
        }
        
        CLogger::LogDebug( ( Format( MSG_PLUGIN_SUCCESSFULLY_UNINSTALLED ) % plugin->GetName() ).str() );
    }
    
    void CPluginManager::UnloadPlugin( const String & pluginName )
    {
        ///@remarks Searches the lib by its name.
        std::vector< CDynLib * >::iterator it = std::find_if( _pluginLibs.begin(), _pluginLibs.end(), [&]( CDynLib * lib )
        {
            return lib->GetName() == pluginName;
        } );

        if( it != _pluginLibs.end() )
        {
            CDynLib * lib = ( *it );
            
            ///@remarks Call the plugin shutdown.
            DLL_STOP_PLUGIN func = ( DLL_STOP_PLUGIN )lib->GetSymbol( SYMBOL_DLL_STOP );
            if ( !func )
            {
                StringStream ss;
                ss << ERROR_SYMBOL_DLL_STOP_NOT_FOUND << pluginName;
                DB_EXCEPT( EDatabaseExceptionCodes_ItemNotFound, ss.str() );
            }
                
            ///@remarks This must call UninstallPlugin
            func();
                
            ///@remarks Unload the library (destroyed by CDynLibManager).
            CDynLibManager::Instance().Unload( lib );
                
            ///@remarks Remove the lib in the list.
            _pluginLibs.erase( it );
        }
    }
}
END_NAMESPACE_DATABASE
