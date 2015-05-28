/************************************************************************//**
 * @file DynLib.h
 * @author spastor
 * @version 1.0
 * @date 2/11/2013 11:47:46 AM
 *
 *
 * @brief Dynamic library class.
 *
 * @details Resource holding data about a dynamic library.
 *
 ***************************************************************************/

#ifndef ___DATABASE_DYNAMIC_LIBRARY_H___
#define ___DATABASE_DYNAMIC_LIBRARY_H___

#include "DatabasePrerequisites.h"

#if defined( _WIN32 )
#   define DYNLIB_HANDLE hInstance
#   define DYNLIB_LOAD( a ) LoadLibraryExA( a, NULL, LOAD_WITH_ALTERED_SEARCH_PATH )
#   define DYNLIB_GETSYM( a, b ) GetProcAddress( a, b )
#   define DYNLIB_UNLOAD( a ) !FreeLibrary( a )

struct HINSTANCE__;
typedef struct HINSTANCE__ * hInstance;

#elif defined( __linux__ )
#   define DYNLIB_HANDLE void *
#   if WCHAR_T_STRINGS
#       define DYNLIB_LOAD( a ) dlopen( Conv_wstr_to_str( a ).c_str(), RTLD_LAZY | RTLD_GLOBAL)
#   else
#       define DYNLIB_LOAD( a ) dlopen( a, RTLD_LAZY | RTLD_GLOBAL)
#   endif
#   define DYNLIB_GETSYM( a, b ) dlsym( a, b )
#   define DYNLIB_UNLOAD( a ) dlclose( a )
#endif

BEGIN_NAMESPACE_DATABASE
{
	/** Resource holding data about a dynamic library.
	@remarks
	    This class holds the data required to get symbols from
	    libraries loaded at run-time (i.e. from DLL's for so's).
	*/
	class CDynLib
	{
	public:
		/** Default constructor.
		*/
		DatabaseExport CDynLib( const String & name );

		/** Destructor.
		*/
		DatabaseExport virtual ~CDynLib();

		/** Load the library.
		*/
		DatabaseExport void Load();

		/** Unload the library.
		*/
		DatabaseExport void Unload();

		/** Get the name of the library.
		@return
		    Return the library name.
		*/
		inline const String & GetName() const { return _name; }

		/** Return the address of the given symbol from the loaded library.
		@param
		    strName The name of the symbol to search for.
		@return
		    If the function succeeds, the returned value is a handle to the symbol.
		@par
		    If the function fails, the returned value is <b>NULL</b>.
		*/
		DatabaseExport void * GetSymbol( const String & strName ) const throw();

		/** File extension used for ARIA plug-in files (platform specific)
		*/
		static const String LIB_EXTENSION;

	protected:
		/** Get the last loading error.
		*/
		String DynlibError();

		String _name; //!< Library name.
		DYNLIB_HANDLE _handle; //!< Handle to the loaded library.
	};

} END_NAMESPACE_DATABASE

#endif // ___DATABASE_DYNAMIC_LIBRARY_H___
