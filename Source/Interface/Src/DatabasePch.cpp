/************************************************************************//**
 * @file DatabasePch.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 03/14/2014 09:21:05 AM
 *
 *
 * @brief Database precompiled header
 *
 * @details This file generates the Database precompiled headers
 *
 ***************************************************************************/

#include "DatabasePch.h"

#include "DatabasePrerequisites.h"
#include "DatabaseStringUtils.h"

#if defined( _WIN32 )
#   include <direct.h>
#endif

#include <boost/filesystem.hpp>

using namespace boost::filesystem;

BEGIN_NAMESPACE_DATABASE
{
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
			return exists( CStrUtils::ToStr( p_filename ).c_str() ) && is_directory( CStrUtils::ToStr( p_filename ).c_str() );
		}
		catch ( std::exception & )
		{
			return false;
		}
	}
}
END_NAMESPACE_DATABASE
