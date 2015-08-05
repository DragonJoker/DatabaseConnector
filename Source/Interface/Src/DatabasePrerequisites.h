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
#include <functional>
#include <deque>

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdarg>

#include <wchar.h>
#include <sys/stat.h>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/locale.hpp>
#include <boost/format.hpp>

#include "DatabaseMacros.h"

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
#include "EFieldType.h"

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
	typedef boost::gregorian::date DateType;
	typedef boost::posix_time::time_duration TimeType;
	typedef boost::posix_time::ptime DateTimeType;

	// "Basic" Types
	class int24_t;
	class uint24_t;
	class CFixedPoint;
	template< typename T > class CDatabaseNullable;

	// Logging related classes
	struct SMessageBase;
	class CLogger;
	class CLoggerImpl;
	class CProgramConsole;

	// Plugin related stuff
	class CDynLib;
	class CDynLibManager;
	class CPluginManager;
	class CFactoryDatabase;
	class CPluginDatabase;

	/** Base class holding the system error handler
	*/
	class CSystemExceptionHandler {};

	//
	class CDatabase;
	class CDatabaseConnection;
	class CDatabaseField;
	class CDatabaseValuedObject;
	class CDatabaseValuedObjectInfos;
	class CDatabaseValueBase;
	template< EFieldType FieldType > struct SFieldTypeDataTyper;
	template< EFieldType FieldType > struct SDatabaseValuePolicy;
	template< EFieldType FieldType, typename ValuePolicy = SDatabaseValuePolicy< FieldType > > class CDatabaseValue;
	class CDatabaseParameter;
	class CDatabaseQuery;
	class CDatabaseResult;
	class CDatabaseRow;
	class CDatabaseStatement;
	class CDatabaseException;

	// Pointers
	DECLARE_SMART_PTR( Database );
	DECLARE_SMART_PTR( DatabaseConnection );
	DECLARE_SMART_PTR( DatabaseField );
	DECLARE_SMART_PTR( DatabaseValuedObjectInfos );
	DECLARE_SMART_PTR( DatabaseValueBase );
	DECLARE_SMART_PTR( DatabaseParameter );
	DECLARE_SMART_PTR( DatabaseQuery );
	DECLARE_SMART_PTR( DatabaseResult );
	DECLARE_SMART_PTR( DatabaseRow );
	DECLARE_SMART_PTR( DatabaseStatement );

	// Containers
	typedef std::vector< String > StringArray;
	typedef std::vector< uint8_t > ByteArray;
	typedef std::vector< DatabaseFieldSPtr > DatabaseFieldPtrArray;
	typedef std::map< String, DatabaseFieldWPtr > DatabaseFieldPtrStrMap;
	typedef std::list< DatabaseRowSPtr > DatabaseRowPtrList;
	typedef std::vector< DatabaseValuedObjectInfosSPtr > DatabaseValuedObjectInfosPtrArray;
	typedef std::vector< DatabaseParameterSPtr > DatabaseParameterPtrArray;
	typedef std::map< String, DatabaseParameterWPtr > DatabaseParameterPtrStrMap;
	typedef std::map< std::thread::id, DatabaseConnectionSPtr > DatabaseConnectionPtrIdMap;
	typedef std::deque< std::unique_ptr< SMessageBase > > MessageQueue;

	// Factory type constants
	const String FACTORY_DATABASE_TYPE = STR( "Factory database" );

}
#if !DB_HAS_MAKE_UNIQUE
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
		return unique_ptr< T >( new T( std::forward< Arg1 >( arg1 ) ) );
	}
	template< class T, typename Arg1, typename Arg2 >
	unique_ptr< T > make_unique( Arg1 && arg1, Arg2 && arg2 )
	{
		return unique_ptr< T >( new T( std::forward< Arg1 >( arg1 ), std::forward< Arg2 >( arg2 ) ) );
	}
	template< class T, typename Arg1, typename Arg2, typename Arg3 >
	unique_ptr< T > make_unique( Arg1 && arg1, Arg2 && arg2, Arg3 && arg3 )
	{
		return unique_ptr< T >( new T( std::forward< Arg1 >( arg1 ), std::forward< Arg2 >( arg2 ), std::forward< Arg3 >( arg3 ) ) );
	}
	template< class T, typename Arg1, typename Arg2, typename Arg3, typename Arg4 >
	unique_ptr< T > make_unique( Arg1 && arg1, Arg2 && arg2, Arg3 && arg3, Arg4 && arg4 )
	{
		return unique_ptr< T >( new T( std::forward< Arg1 >( arg1 ), std::forward< Arg2 >( arg2 ), std::forward< Arg3 >( arg3 ), std::forward< Arg4 >( arg4 ) ) );
	}
	template< class T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5 >
	unique_ptr< T > make_unique( Arg1 && arg1, Arg2 && arg2, Arg3 && arg3, Arg4 && arg4, Arg5 && arg5 )
	{
		return unique_ptr< T >( new T( std::forward< Arg1 >( arg1 ), std::forward< Arg2 >( arg2 ), std::forward< Arg3 >( arg3 ), std::forward< Arg4 >( arg4 ), std::forward< Arg5 >( arg5 ) ) );
	}
	template< class T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6 >
	unique_ptr< T > make_unique( Arg1 && arg1, Arg2 && arg2, Arg3 && arg3, Arg4 && arg4, Arg5 && arg5, Arg6 && arg6 )
	{
		return unique_ptr< T >( new T( std::forward< Arg1 >( arg1 ), std::forward< Arg2 >( arg2 ), std::forward< Arg3 >( arg3 ), std::forward< Arg4 >( arg4 ), std::forward< Arg5 >( arg5 ), std::forward< Arg6 >( arg6 ) ) );
	}
	template< class T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7 >
	unique_ptr< T > make_unique( Arg1 && arg1, Arg2 && arg2, Arg3 && arg3, Arg4 && arg4, Arg5 && arg5, Arg6 && arg6, Arg7 && arg7 )
	{
		return unique_ptr< T >( new T( std::forward< Arg1 >( arg1 ), std::forward< Arg2 >( arg2 ), std::forward< Arg3 >( arg3 ), std::forward< Arg4 >( arg4 ), std::forward< Arg5 >( arg5 ), std::forward< Arg6 >( arg6 ), std::forward< Arg7 >( arg7 ) ) );
	}
	template< class T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8 >
	unique_ptr< T > make_unique( Arg1 && arg1, Arg2 && arg2, Arg3 && arg3, Arg4 && arg4, Arg5 && arg5, Arg6 && arg6, Arg7 && arg7, Arg8 && arg8 )
	{
		return unique_ptr< T >( new T( std::forward< Arg1 >( arg1 ), std::forward< Arg2 >( arg2 ), std::forward< Arg3 >( arg3 ), std::forward< Arg4 >( arg4 ), std::forward< Arg5 >( arg5 ), std::forward< Arg6 >( arg6 ), std::forward< Arg7 >( arg7 ), std::forward< Arg8 >( arg8 ) ) );
	}
}
#endif

#endif // ___DATABASE_PREREQUISITES_H___
