/************************************************************************//**
 * @file DatabasePrerequisites.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 03/14/2013 09:20:25 AM
 *
 *
 * @brief Database prerequisite header.
 *
 * @details This file contains all Database prerequisite instructions.
 *
 ***************************************************************************/

#ifndef ___DATABASE_PREREQUISITES_H___
#define ___DATABASE_PREREQUISITES_H___

#define BEGIN_NAMESPACE_DATABASE   namespace Database
#define NAMESPACE_DATABASE         Database
#define END_NAMESPACE_DATABASE

#include <string>
#include <vector>
#include <map>
#include <list>
#include <thread>
#include <mutex>
#include <memory>
#include <iostream>
#include <sstream>
#include <locale>

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdarg>

#include <wchar.h>
#include <sys/stat.h>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/optional.hpp>
#include <boost/locale.hpp>
#include <boost/format.hpp>

#include "DatabaseMacros.h"
#include "EFieldType.h"

#if defined( _WIN32 )
#   ifdef Database_EXPORTS
#        define DatabaseExport __declspec ( dllexport )
#   else
#       if defined ( __MINGW32__ )
#           define DatabaseExport
#       else
#           define DatabaseExport __declspec ( dllimport )
#       endif
#   endif
#else
#   define DatabaseExport
#endif

/** Database namespace
*/
namespace Database
{
	typedef char TChar;
#   define STR( x ) x
#   define tcout std::cout

#if defined( _WIN32 )
	static const TChar * PATH_SEP = STR( "\\" );
#else
	static const TChar * PATH_SEP = STR( "/" );
#endif

	typedef std::basic_string< TChar > String;
	typedef std::basic_stringstream< TChar > StringStream;
	typedef boost::basic_format< TChar > Format;

	const String DATETIME_FORMAT_EXP = STR( "%Y/%m/%d %H:%M:%S" );
	const std::string SDATETIME_FORMAT_EXP = "%Y/%m/%d %H:%M:%S";
	const std::wstring WDATETIME_FORMAT_EXP = L"%Y/%m/%d %H:%M:%S";

	const String DATETIME_TIME_FORMAT_EXP = STR( "0000/00/00 %H:%M:%S" );
	const std::string SDATETIME_TIME_FORMAT_EXP = "0000/00/00 %H:%M:%S";
	const std::wstring WDATETIME_TIME_FORMAT_EXP = L"0000/00/00 %H:%M:%S";

	const String DATETIME_DATE_FORMAT_EXP = STR( "%Y/%m/%d 00:00:00" );
	const std::string SDATETIME_DATE_FORMAT_EXP = "%Y/%m/%d 00:00:00";
	const std::wstring WDATETIME_DATE_FORMAT_EXP = L"%Y/%m/%d 00:00:00";

	const String TIME_FORMAT_EXP = STR( "%H:%M:%S" );
	const std::string STIME_FORMAT_EXP = "%H:%M:%S";
	const std::wstring WTIME_FORMAT_EXP = L"%H:%M:%S";

	const String DATE_FORMAT_EXP = STR( "%Y/%m/%d" );
	const std::string SDATE_FORMAT_EXP = "%Y/%m/%d";
	const std::wstring WDATE_FORMAT_EXP = L"%Y/%m/%d";

	// Pre-declare classes
	// Allows use of pointers in header files without including individual .h
	// so decreases dependencies between files
	class CDynLib;
	class CDynLibManager;
	class CPluginManager;
	class CDatabase;
	class CDatabaseConnection;
	class CDatabaseField;
	class CDatabaseFieldInfos;
	class CDatabaseValueBase;
	template< EFieldType > class CDatabaseValue;
	class CDatabaseParameter;
	class CDatabaseQuery;
	class CDatabaseResult;
	class CDatabaseRow;
	class CDatabaseStatement;
	class CDate;
	class CDateTime;
	class CDateTimeSpan;
	class CExceptionDatabase;
	class CTime;
	class CFactoryDatabase;
	class CPluginDatabase;

	// Pointers
	typedef std::shared_ptr< CDatabase >            DatabasePtr;
	typedef std::shared_ptr< CDatabaseConnection >  DatabaseConnectionPtr;
	typedef std::shared_ptr< CDatabaseField >       DatabaseFieldPtr;
	typedef std::shared_ptr< CDatabaseFieldInfos >  DatabaseFieldInfosPtr;
	typedef std::unique_ptr< CDatabaseValueBase >   DatabaseValueBasePtr;
	typedef std::shared_ptr< CDatabaseParameter >   DatabaseParameterPtr;
	typedef std::shared_ptr< CDatabaseQuery >       DatabaseQueryPtr;
	typedef std::shared_ptr< CDatabaseResult >      DatabaseResultPtr;
	typedef std::shared_ptr< CDatabaseRow >         DatabaseRowPtr;
	typedef std::shared_ptr< CDatabaseStatement >   DatabaseStatementPtr;

	// Containers
	typedef std::vector< String >                               StringArray;
	typedef std::vector< DatabaseFieldPtr >                     DatabaseFieldPtrArray;
	typedef std::map< String, DatabaseFieldPtr >                DatabaseFieldPtrStrMap;
	typedef std::list< DatabaseRowPtr >                         DatabaseRowPtrList;
	typedef std::vector< DatabaseFieldInfosPtr >                DatabaseFieldInfosPtrArray;
	typedef std::vector< DatabaseParameterPtr >                 DatabaseParameterPtrArray;
	typedef std::map< String, DatabaseParameterPtr >            DatabaseParameterPtrStrMap;
	typedef std::map< std::thread::id, DatabaseConnectionPtr >  DatabaseConnectionPtrIdMap;

	// Factory type constants
	const String FACTORY_DATABASE_TYPE = STR( "Factory database" );

	/** Converts a string in a given charset into an UTF-8 string
	@param src
	    The original string
	@param charset
	    The original string charset
	*/
	DatabaseExport std::string ToUtf8( const std::string & src, const std::string & charset );

	/** Converts a string in a given charset into an UTF-8 string
	@param src
	    The original string
	@param charset
	    The original string charset
	*/
	DatabaseExport std::string ToUtf8( const std::wstring & src, const std::wstring & charset );

	/** Checks if the two given field types are compatible
	@param typeA, typeB
	    The field types
	@return
	    True if typeA and typeB are compatible
	*/
	DatabaseExport bool AreTypesCompatible( EFieldType typeA, EFieldType typeB );
	/**
	 *\~english
	 *\brief		Opens a file
	 *\param[out]	p_pFile		Receives the file descriptor
	 *\param[in]	p_pszPath	The file path
	 *\param[in]	p_pszMode	The opening mode
	 *\return		\p true on success
	 *\~french
	 *\brief		Ouvre un fichier
	 *\param[out]	p_pFile		Reçoit le descripteur du fichier
	 *\param[in]	p_pszPath	Le chemin d'acès au fichier
	 *\param[in]	p_pszMode	Le mode d'ouverture
	 *\return		\p true en cas de réussite
	 */
	DatabaseExport bool FOpen( FILE *& p_pFile, char const * p_pszPath, char const * p_pszMode );

	/** Checks the file existence
	@param name
	    The file name
	@return true if the file exists
	*/
	DatabaseExport bool FileExists( const String & name );

	/** Checks the folder existence
	@param name
	    The folder name
	@return true if the folder exists
	*/
	DatabaseExport bool FolderExists( String const & p_filename );

	/** Creates a folder
	@param name
	    The folder name
	@return true if the folder was correctly created
	*/
	DatabaseExport bool CreateFolder( String const & pathFolder );
}
#if !_HAS_MAKE_UNIQUE
namespace std
{
	template< class T >
	unique_ptr< T > make_unique()
	{
		return unique_ptr< T >( new T );
	}
	template< class T, typename Arg1 >
	unique_ptr< T > make_unique( Arg1 && arg1 )
	{
		return unique_ptr< T >( new T( arg1 ) );
	}
	template< class T, typename Arg1, typename Arg2 >
	unique_ptr< T > make_unique( Arg1 && arg1, Arg2 && arg2 )
	{
		return unique_ptr< T >( new T( arg1, arg2 ) );
	}
	template< class T, typename Arg1, typename Arg2, typename Arg3 >
	unique_ptr< T > make_unique( Arg1 && arg1, Arg2 && arg2, Arg3 && arg3 )
	{
		return unique_ptr< T >( new T( arg1, arg2, arg3 ) );
	}
	template< class T, typename Arg1, typename Arg2, typename Arg3, typename Arg4 >
	unique_ptr< T > make_unique( Arg1 && arg1, Arg2 && arg2, Arg3 && arg3, Arg4 && arg4 )
	{
		return unique_ptr< T >( new T( arg1, arg2, arg3, arg4 ) );
	}
	template< class T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5 >
	unique_ptr< T > make_unique( Arg1 && arg1, Arg2 && arg2, Arg3 && arg3, Arg4 && arg4, Arg5 && arg5 )
	{
		return unique_ptr< T >( new T( arg1, arg2, arg3, arg4, arg5 ) );
	}
	template< class T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6 >
	unique_ptr< T > make_unique( Arg1 && arg1, Arg2 && arg2, Arg3 && arg3, Arg4 && arg4, Arg5 && arg5, Arg6 && arg6 )
	{
		return unique_ptr< T >( new T( arg1, arg2, arg3, arg4, arg5, arg6 ) );
	}
	template< class T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7 >
	unique_ptr< T > make_unique( Arg1 && arg1, Arg2 && arg2, Arg3 && arg3, Arg4 && arg4, Arg5 && arg5, Arg6 && arg6, Arg7 && arg7 )
	{
		return unique_ptr< T >( new T( arg1, arg2, arg3, arg4, arg5, arg6, arg7 ) );
	}
	template< class T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8 >
	unique_ptr< T > make_unique( Arg1 && arg1, Arg2 && arg2, Arg3 && arg3, Arg4 && arg4, Arg5 && arg5, Arg6 && arg6, Arg7 && arg7, Arg8 && arg8 )
	{
		return unique_ptr< T >( new T( arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8 ) );
	}
}
#endif

#endif // ___DATABASE_PREREQUISITES_H___
