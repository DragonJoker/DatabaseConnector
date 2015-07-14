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

#if defined( _WIN32 )
#   include <direct.h>
#endif

#include <boost/filesystem.hpp>
#include <boost/locale.hpp>

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

	std::string ToUtf8( const std::string & src, const std::string & charset )
	{
		std::wstring strUtf = boost::locale::conv::to_utf< wchar_t >( src, charset );
		return boost::locale::conv::from_utf( strUtf, "UTF-8" );
	}

	std::string ToUtf8( const std::wstring & src, const std::wstring & charset )
	{
		return ToUtf8( CStrUtils::ToStr( src ), CStrUtils::ToStr( charset ) );
	}

#if defined( _MSC_VER)
	bool FOpen( FILE *& p_pFile, char const * p_pszPath, char const * p_pszMode )
	{
		return fopen_s( &p_pFile, p_pszPath, p_pszMode ) == 0;
	}
#else
	bool FOpen( FILE *& p_pFile, char const * p_pszPath, char const * p_pszMode )
	{
		p_pFile = fopen( p_pszPath, p_pszMode );
		return p_pFile != NULL;
	}
#endif

	bool FileExists( const String & name )
	{
		bool result = false;
		struct stat buffer;
		std::string strname = CStrUtils::ToStr( name );

		if ( stat( strname.c_str(), &buffer ) == 0 )
		{
			result = true;
		}

		return result;
	}

	bool CreateFolder( String const & pathFolder )
	{
		bool bReturn = false;

		if ( !pathFolder.empty() )
		{
			String pathParentFolder = pathFolder.substr( 0, pathFolder.find_last_of( PATH_SEP[0] ) );
			bReturn = FolderExists( pathParentFolder );

			if ( !bReturn )
			{
				bReturn = CreateFolder( pathParentFolder );
			}

			if ( bReturn )
			{
#if defined( _MSC_VER )
				bReturn = _mkdir( CStrUtils::ToStr( pathFolder ).c_str() ) == 0;
#else
				bReturn = mkdir( CStrUtils::ToStr( pathFolder ).c_str(), 777 ) == 0;
#endif
			}
		}

		return bReturn;
	}

	bool FolderExists( String const & p_filename )
	{
		try
		{
			return boost::filesystem::exists( CStrUtils::ToStr( p_filename ).c_str() ) && boost::filesystem::is_directory( CStrUtils::ToStr( p_filename ).c_str() );
		}
		catch ( std::exception & )
		{
			return false;
		}
	}
}
END_NAMESPACE_DATABASE
