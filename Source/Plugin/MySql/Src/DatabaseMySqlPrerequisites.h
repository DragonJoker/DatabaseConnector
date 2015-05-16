/************************************************************************//**
 * @file DatabaseMySqlPrerequisites.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 03/14/2014 11:48:00 AM
 * 
 *
 * @brief Database MYSQL prerequisite header.
 *
 * @details This file contains all Database MYSQL prerequisite instructions.
 *
 ***************************************************************************/

#ifndef ___DATABASE_MYSQL_PREREQUISITES_H___
#define ___DATABASE_MYSQL_PREREQUISITES_H___

#include <DatabasePrerequisites.h>

#if defined( STATIC_LIB )
#   define CPPCONN_PUBLIC_FUNC
#endif

#define BEGIN_NAMESPACE_DATABASE_MYSQL     BEGIN_NAMESPACE_DATABASE { namespace MySql
#define NAMESPACE_DATABASE_MYSQL           NAMESPACE_DATABASE::MySql
#define END_NAMESPACE_DATABASE_MYSQL       END_NAMESPACE_DATABASE }

BEGIN_NAMESPACE_DATABASE
{
    /** MYSQL namespace
    */
    namespace MySql
    {
#if ( PLATFORM == PLATFORM_WIN32 ) && !defined ( __MINGW32__ ) && !defined ( STATIC_LIB )
#    ifdef DatabasePluginMySql_EXPORTS
#        define DatabaseMySqlExport __declspec ( dllexport )
#    else
#       if defined ( __MINGW32__ )
#           define DatabaseMySqlExport
#       else
#           define DatabaseMySqlExport __declspec ( dllimport )
#       endif
#   endif
#else
#    define DatabaseMySqlExport
#endif
        
        // Pre-declare classes
        // Allows use of pointers in header files without including individual .h
        // so decreases dependencies between files
        class CDatabaseConnectionMySql;
        class CDatabaseMySql;
        class CDatabaseResultMySql;
        class CDatabaseStatementMySql;
        class CDatabaseStatementParameterMySql;
        class CExceptionDatabaseMySql;
        class CPluginDatabaseMySql;
        class CFactoryDatabaseMySql;
        struct SParameterValueSetterBase;
        
        // Pointers
        typedef std::shared_ptr< CDatabaseConnectionMySql > DatabaseConnectionMySqlPtr;
        typedef std::shared_ptr< CDatabaseStatementMySql > DatabaseStatementMySqlPtr;
        typedef std::shared_ptr< CDatabaseStatementParameterMySql > DatabaseStatementParameterMySqlPtr;
        typedef std::shared_ptr< CDatabaseResultMySql > DatabaseResultMySqlPtr;
        
        // Factory constants
        const String FACTORY_DATABASE_MYSQL = STR( "Factory Database MySql" );
        
        // Plugin constants
        const String DATABASE_MYSQL_TYPE = STR( "Aria.Database.MySql" );
        const String PLUGIN_NAME_DATABASE_MYSQL = STR( "Plugin Database MySql" );
    }
}
END_NAMESPACE_DATABASE

#if !defined( NDEBUG )
#   define MySQLTry( x, msg )\
    try\
    {\
        x;\
        CLogger::LogDebug( StringStream() << STR( "Success : " ) << msg );\
    }\
    catch( sql::SQLException & e )\
    {\
        CLogger::LogError( StringStream() << STR( "Failure : " ) << msg << STR( "\n" ) << e.what() << STR( "\nMySQL State : " ) << e.getSQLState().c_str() << STR( "\nError code : " ) << e.getErrorCode() );\
    }
#else
#   define MySQLTry( x, msg ) x;
#endif

#endif // ___DATABASE_MYSQL_PREREQUISITES_H___
