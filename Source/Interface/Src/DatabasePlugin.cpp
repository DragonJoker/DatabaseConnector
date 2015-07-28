/************************************************************************//**
* @file PluginDatabase.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 4/08/2014 10:01:21 AM
*
*
* @brief Class defining a generic database plugin.
*
* @details Class defining a generic database plugin.
*
***************************************************************************/

#include "DatabasePch.h"

#include "DatabasePlugin.h"

#include "DatabaseFactory.h"
#include "DatabaseFactoryManager.h"
#include "DatabaseFileUtils.h"
#include "DatabaseStringUtils.h"

BEGIN_NAMESPACE_DATABASE
{
	const String ERROR_DB_FACTORY_IS_NULL = STR( "Database factory is null" );

	CPluginDatabase::CPluginDatabase( CFactoryDatabase * factory )
		: _factory( factory )
		, _installed( false )
	{
		if ( _factory == NULL )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_NullPointer, ERROR_DB_FACTORY_IS_NULL );
		}
	}

	CPluginDatabase::~CPluginDatabase()
	{
		delete _factory;
		_factory = NULL;
	}

	void CPluginDatabase::Install()
	{
		if ( !_installed )
		{
			//!@remarks Initialize the factory.
			_factory->Initialize();

			//!@remarks Register factory.
			CFactoryManager::Instance().AddFactory( _factory );

			_installed = true;
		}
	}

	void CPluginDatabase::Uninstall()
	{
		if ( _installed )
		{
			//!@remarks Unregister factory.
			CFactoryManager::Instance().RemoveFactory( _factory );

			_installed = false;
		}
	}
}
END_NAMESPACE_DATABASE
