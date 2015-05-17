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

#include "DatabaseFactory.h"
#include "DatabasePlugin.h"
#include "DatabaseFactoryManager.h"

#include "DatabaseStringUtils.h"

#include <boost/locale.hpp>

BEGIN_NAMESPACE_DATABASE
{
	const String ERROR_DATABASE_FACTORY_IS_NULL = STR( "Database factory is null" );

	CPluginDatabase::CPluginDatabase( CFactoryDatabase * factory )
		: _factory( factory )
		, _installed( false )
	{
		if ( _factory == NULL )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_NullPointer, ERROR_DATABASE_FACTORY_IS_NULL );
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
			///@remarks Initialize the factory.
			_factory->Initialize();

			///@remarks Register factory.
			CFactoryManager::Instance().AddFactory( _factory );

			_installed = true;
		}
	}

	void CPluginDatabase::Uninstall()
	{
		if ( _installed )
		{
			///@remarks Unregister factory.
			CFactoryManager::Instance().RemoveFactory( _factory );

			_installed = false;
		}
	}

	std::string ToUtf8( const std::string & src, const std::string & charset )
	{
		std::wstring strUtf = boost::locale::conv::to_utf< wchar_t >( src, charset );
		return boost::locale::conv::from_utf( strUtf, "UTF-8" );
	}

	std::string ToUtf8( const std::wstring & src, const std::wstring & charset )
	{
		return ToUtf8( CStrUtils::ToStr( src ), CStrUtils::ToStr( charset ) );
	}
}
END_NAMESPACE_DATABASE
