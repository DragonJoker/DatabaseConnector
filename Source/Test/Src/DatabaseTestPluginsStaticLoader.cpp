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
    CTestPluginsStaticLoader::CTestPluginsStaticLoader()
        : _odbcMySql( false )
        , _odbcMsSql( false )
        , _mySql( false )
#if defined( STATIC_LIB )
        , _pluginOdbcMySql( NULL )
        , _pluginOdbcMsSql( NULL )
        , _pluginMySql( NULL )
#endif
        
    {
    }
    
    CTestPluginsStaticLoader::~CTestPluginsStaticLoader()
    {
    }
    
    void CTestPluginsStaticLoader::Load( bool mySql, bool odbcMySql, bool odbcMsSql )
    {
        _mySql = mySql;
        _odbcMySql = odbcMySql;
        _odbcMsSql = odbcMsSql;
        OnLoad();
    }
    
    void CTestPluginsStaticLoader::Unload()
    {
        OnUnload();
    }
    
    void CTestPluginsStaticLoader::OnLoad()
    {
#if defined( STATIC_LIB )
        if ( !_pluginOdbcMySql && _odbcMySql )
        {
            ///@remarks Create the plugin.
            _pluginOdbcMySql = new Odbc::MySql::CPluginDatabaseOdbcMySql;
            
            ///@remarks Register the plugin.
            CPluginManager::Instance().InstallPlugin( _pluginOdbcMySql );
        }
        
        if ( !_pluginOdbcMsSql && _odbcMsSql )
        {
            ///@remarks Create the plugin.
            _pluginOdbcMsSql = new Odbc::MsSql::CPluginDatabaseOdbcMsSql;
            
            ///@remarks Register the plugin.
            CPluginManager::Instance().InstallPlugin( _pluginOdbcMsSql );
        }
        
        if ( !_pluginMySql && _mySql )
        {
            ///@remarks Create the plugin.
            _pluginMySql = new MySql::CPluginDatabaseMySql;
            
            ///@remarks Register the plugin.
            CPluginManager::Instance().InstallPlugin( _pluginMySql );
        }
#endif
    }
    
    void CTestPluginsStaticLoader::OnUnload()
    {
#if defined( STATIC_LIB )
        if ( _pluginOdbcMySql )
        {
            ///@remarks Unregister the plugin.
            CPluginManager::Instance().UninstallPlugin( _pluginOdbcMySql );
            
            ///@remarks Delete the plugin.
            delete _pluginOdbcMySql;
            _pluginOdbcMySql = NULL;
        }
        
        if ( _pluginOdbcMsSql )
        {
            ///@remarks Unregister the plugin.
            CPluginManager::Instance().UninstallPlugin( _pluginOdbcMsSql );
            
            ///@remarks Delete the plugin.
            delete _pluginOdbcMsSql;
            _pluginOdbcMsSql = NULL;
        }
        
        if ( _pluginMySql )
        {
            ///@remarks Unregister the plugin.
            CPluginManager::Instance().UninstallPlugin( _pluginMySql );
            
            ///@remarks Delete the plugin.
            delete _pluginMySql;
            _pluginMySql = NULL;
        }
#endif
    }
}
END_NAMESPACE_DATABASE_TEST

