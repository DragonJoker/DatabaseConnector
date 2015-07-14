/************************************************************************//**
* @file DynLibManager.cpp
* @author spastor
* @version 1.0
* @date 2/11/2013 4:12:55 PM
*
*
* @brief Manager for Dynamic-loading Libraries.
*
* @details Manager for Dynamic-loading Libraries.
*
***************************************************************************/

#include "DatabasePch.h"

#include "DatabaseDynLibManager.h"
#include "DatabaseDynLib.h"
#include "DatabaseException.h"

BEGIN_NAMESPACE_DATABASE
{
	CDynLibManager::CDynLibManager()
	{
	}

	CDynLibManager::~CDynLibManager()
	{
		//!@remarks Unload
		for ( auto && it : _libList )
		{
			it.second->Unload();
			delete it.second;
		}

		//!@remarks Clear the list
		_libList.clear();
	}

	CDynLib * CDynLibManager::Load( const String & filename )
	{
		auto && it = _libList.find( filename );
		CDynLib * lib = NULL;

		if ( it != _libList.end() )
		{
			lib = it->second;
		}
		else
		{
			lib = new CDynLib( filename );

			try
			{
				lib->Load();
				_libList.insert( std::make_pair( lib->GetName(), lib ) );
			}
			catch ( CExceptionDatabase & )
			{
				delete lib;
				throw;
			}
		}

		return lib;
	}

	void CDynLibManager::Unload( CDynLib * dynLib )
	{
		auto && it = _libList.find( dynLib->GetName() );
		CDynLib * lib = NULL;

		if ( it != _libList.end() )
		{
			lib = it->second;
			_libList.erase( it );
			lib->Unload();
		}

		delete lib;
	}

} END_NAMESPACE_DATABASE
