/************************************************************************//**
 * @file DynLib.cpp
 * @author spastor
 * @version 1.0
 * @date 2/11/2013 11:54:36 AM
 *
 *
 * @brief Dynamic library class.
 *
 * @details Resource holding data about a dynamic library.
 *
 ***************************************************************************/

#include "DatabasePch.h"

#include "DatabaseDynLib.h"
#include "DatabaseStringUtils.h"
#include "DatabaseException.h"

#include <sys/stat.h>

#if defined( _WIN32 )
#   include <Windows.h>
#elif defined( __linux__ )
#	include <dlfcn.h>
#endif

BEGIN_NAMESPACE_DATABASE
{
	///@remarks Static members
#   if defined( _WIN32 )
	const String   CDynLib::LIB_EXTENSION( STR( ".dll" ) );
#   elif defined( __linux__ )
	const String   CDynLib::LIB_EXTENSION( STR( ".so" ) );
#   endif

	const String TEXT_DYNAMIC_LIB_SYSTEM_ERROR = STR( "System Error: " );

	const String MSG_LOADING_LIBRARY = STR( "Loading library %s" );
	const String MSG_UNLOADING_LIBRARY = STR( "Unloading library %s" );

	const String ERROR_LIB_FILE_NOT_FOUND = STR( "File %s not found!" );
	const String ERROR_LOAD_DYNAMIC_LIB = STR( "Could not load dynamic library %s." );
	const String ERROR_UNLOAD_DYNAMIC_LIB = STR( "Could not unload dynamic library %s." );

	namespace
	{
		bool FindFile( const String & name, const String & extension, String & file )
		{
			bool result = false;

			if ( FileExists( name ) )
			{
				result = true;
				file = name;
			}
			else if ( FileExists( name + extension ) )
			{
				result = true;
				file = name + extension;
			}

			return result;
		}
	}

	CDynLib::CDynLib( const String & name )
		: _name( name )
		, _handle( NULL )
	{
	}

	CDynLib::~CDynLib()
	{
	}

	void CDynLib::Load()
	{
		String name = _name;

		///@remarks Check if file exists.
		if ( FindFile( _name, LIB_EXTENSION, name ) == false )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ItemNotFound, ( Format( ERROR_LIB_FILE_NOT_FOUND ) % name ).str() );
		}

		///@remarks Update name with the complete library path.
		_name = name;

		///@remarks Load library.
		_handle = ( DYNLIB_HANDLE )DYNLIB_LOAD( name.c_str() );

		if ( !_handle )
		{
			StringStream ss;
			ss << ( Format( ERROR_LOAD_DYNAMIC_LIB ) % name ).str() << TEXT_DYNAMIC_LIB_SYSTEM_ERROR << DynlibError();
			DB_EXCEPT( EDatabaseExceptionCodes_InternalError, ss.str() );
		}
	}

	void CDynLib::Unload()
	{
		//CLogManager::Get().LogMessage( "Unloading library " + _name );

		if ( DYNLIB_UNLOAD( _handle ) )
		{
			StringStream ss;
			ss << ( Format( ERROR_UNLOAD_DYNAMIC_LIB ) % _name ).str() << TEXT_DYNAMIC_LIB_SYSTEM_ERROR << DynlibError();
			DB_EXCEPT( EDatabaseExceptionCodes_InternalError, ss.str() );
		}
	}

	void * CDynLib::GetSymbol( const String & strName ) const throw()
	{
		///@remarks GetProcAddress doesn't have unicode version. Always use ANSI string.
		return ( void * )DYNLIB_GETSYM( _handle, CStrUtils::ToStr( strName ).c_str() );
	}

	String CDynLib::DynlibError()
	{
		String result;
#   if defined( _WIN32 )
		LPVOID lpMsgBuf = NULL;
		String sErrorDescription;
		DWORD dwError = ::GetLastError();

		///@remarks Format a message string
		FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, // source and processing options
			NULL,                                                                                        // pointer to message source
			dwError,                                                                                     // requested message identifier
			MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),                                                 // Default language
			reinterpret_cast< LPSTR >( &lpMsgBuf ),                                                     // pointer to message buffer
			0,                                                                                           // maximum size of message buffer
			NULL );                                                                                      // address of array of message inserts

		if ( lpMsgBuf )
		{
			sErrorDescription = reinterpret_cast< LPSTR >( lpMsgBuf );
			LocalFree( lpMsgBuf );
		}

		result = sErrorDescription;
#   elif defined( __linux__ )
		result = CStrUtils::ToString( dlerror() );
#   endif
		return result;
	}

} END_NAMESPACE_DATABASE
