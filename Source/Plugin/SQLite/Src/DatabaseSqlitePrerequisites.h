/************************************************************************//**
* @file DatabaseSqlitePrerequisites.h
* @author Sylvain Doremus
* @version 1.0
* @date 03/14/2014 11:48:00 AM
*
*
* @brief Database SQLite prerequisite header.
*
* @details This file contains all Database SQLite prerequisite instructions.
*
***************************************************************************/

#ifndef ___DATABASE_SQLITE_PREREQUISITES_H___
#define ___DATABASE_SQLITE_PREREQUISITES_H___

#include <DatabasePrerequisites.h>

#if defined( _WIN32 ) && !defined ( __MINGW32__ ) && !defined ( STATIC_LIB )
#	ifdef DatabasePluginSqlite_EXPORTS
#	    define DatabaseSqliteExport __declspec ( dllexport )
#	else
#		if defined ( __MINGW32__ )
#			define DatabaseSqliteExport
#		else
#			define DatabaseSqliteExport __declspec ( dllimport )
#		endif
#	endif
#else
#    define DatabaseSqliteExport
#endif

#if defined( STATIC_LIB )
#	define CPPCONN_PUBLIC_FUNC
#endif

#define BEGIN_NAMESPACE_DATABASE_SQLITE BEGIN_NAMESPACE_DATABASE { namespace Sqlite
#define NAMESPACE_DATABASE_SQLITE NAMESPACE_DATABASE::Sqlite
#define END_NAMESPACE_DATABASE_SQLITE } END_NAMESPACE_DATABASE

BEGIN_NAMESPACE_DATABASE
{
#	include "sqlite3.h"
	namespace Sqlite
	{
		// Pre-declare classes
		// Allows use of pointers in header files without including individual .h
		// so decreases dependencies between files
		class CDatabaseConnectionSqlite;
		class CDatabaseSqlite;
		class CDatabaseStatementSqlite;
		class CDatabaseStatementParameterSqlite;
		class CExceptionDatabaseSqlite;
		class CPluginDatabaseSqlite;
		class CFactoryDatabaseSqlite;
		struct SSqliteBindingBase;

		// Pointers
		typedef std::shared_ptr< CDatabaseConnectionSqlite > DatabaseConnectionSqlitePtr;
		typedef std::shared_ptr< CDatabaseStatementSqlite > DatabaseStatementSqlitePtr;
		typedef std::shared_ptr< CDatabaseStatementParameterSqlite > DatabaseStatementParameterSqlitePtr;

		// Factory constants
		const String FACTORY_DATABASE_SQLITE = STR( "Factory Database SQLite" );

		// Plugin constants
		const String DATABASE_SQLITE_TYPE = STR( "Database.SQLite" );
		const String PLUGIN_NAME_DATABASE_SQLITE = STR( "Plugin Database SQLite" );

		namespace SQLite
		{

			typedef enum
			{
				eCODE_OK = SQLITE_OK,
				eCODE_ERROR = SQLITE_ERROR,
				eCODE_INTERNAL = SQLITE_INTERNAL,
				eCODE_PERM = SQLITE_PERM,
				eCODE_ABORT = SQLITE_ABORT,
				eCODE_BUSY = SQLITE_BUSY,
				eCODE_LOCKED = SQLITE_LOCKED,
				eCODE_NOMEM = SQLITE_NOMEM,
				eCODE_READONLY = SQLITE_READONLY,
				eCODE_INTERRUPT = SQLITE_INTERRUPT,
				eCODE_IOERR = SQLITE_IOERR,
				eCODE_CORRUPT = SQLITE_CORRUPT,
				eCODE_NOTFOUND = SQLITE_NOTFOUND,
				eCODE_FULL = SQLITE_FULL,
				eCODE_CANTOPEN = SQLITE_CANTOPEN,
				eCODE_PROTOCOL = SQLITE_PROTOCOL,
				eCODE_EMPTY = SQLITE_EMPTY,
				eCODE_SCHEMA = SQLITE_SCHEMA,
				eCODE_TOOBIG = SQLITE_TOOBIG,
				eCODE_CONSTRAINT = SQLITE_CONSTRAINT,
				eCODE_MISMATCH = SQLITE_MISMATCH,
				eCODE_MISUSE = SQLITE_MISUSE,
				eCODE_NOLFS = SQLITE_NOLFS,
				eCODE_AUTH = SQLITE_AUTH,
				eCODE_FORMAT = SQLITE_FORMAT,
				eCODE_RANGE = SQLITE_RANGE,
				eCODE_NOTADB = SQLITE_NOTADB,
				eCODE_ROW = SQLITE_ROW,
				eCODE_DONE = SQLITE_DONE,
			}	eCODE;

			typedef enum
			{
				eEXTENDED_CODE_READ = SQLITE_IOERR_READ,
				eEXTENDED_CODE_SHORT_READ = SQLITE_IOERR_SHORT_READ,
				eEXTENDED_CODE_WRITE = SQLITE_IOERR_WRITE,
				eEXTENDED_CODE_FSYNC = SQLITE_IOERR_FSYNC,
				eEXTENDED_CODE_DIR_FSYNC = SQLITE_IOERR_DIR_FSYNC,
				eEXTENDED_CODE_TRUNCATE = SQLITE_IOERR_TRUNCATE,
				eEXTENDED_CODE_FSTAT = SQLITE_IOERR_FSTAT,
				eEXTENDED_CODE_UNLOCK = SQLITE_IOERR_UNLOCK,
				eEXTENDED_CODE_RDLOCK = SQLITE_IOERR_RDLOCK,
				eEXTENDED_CODE_DELETE = SQLITE_IOERR_DELETE,
				eEXTENDED_CODE_BLOCKED = SQLITE_IOERR_BLOCKED,
				eEXTENDED_CODE_NOMEM = SQLITE_IOERR_NOMEM,
				eEXTENDED_CODE_ACCESS = SQLITE_IOERR_ACCESS,
				eEXTENDED_CODE_CHECKRESERVEDLOCK = SQLITE_IOERR_CHECKRESERVEDLOCK,
				eEXTENDED_CODE_LOCK = SQLITE_IOERR_LOCK,
				eEXTENDED_CODE_CLOSE = SQLITE_IOERR_CLOSE,
				eEXTENDED_CODE_DIR_CLOSE = SQLITE_IOERR_DIR_CLOSE,
				eEXTENDED_CODE_LOCKED_SHAREDCACHE = SQLITE_LOCKED_SHAREDCACHE,
			}	eEXTENDED_CODE;

			typedef enum
			{
				eOPEN_FLAG_READONLY = SQLITE_OPEN_READONLY,
				eOPEN_FLAG_READWRITE = SQLITE_OPEN_READWRITE,
				eOPEN_FLAG_CREATE = SQLITE_OPEN_CREATE,
				eOPEN_FLAG_DELETEONCLOSE = SQLITE_OPEN_DELETEONCLOSE,
				eOPEN_FLAG_EXCLUSIVE = SQLITE_OPEN_EXCLUSIVE,
				eOPEN_FLAG_AUTOPROXY = SQLITE_OPEN_AUTOPROXY,
				eOPEN_FLAG_MAIN_DB = SQLITE_OPEN_MAIN_DB,
				eOPEN_FLAG_TEMP_DB = SQLITE_OPEN_TEMP_DB,
				eOPEN_FLAG_TRANSIENT_DB = SQLITE_OPEN_TRANSIENT_DB,
				eOPEN_FLAG_MAIN_JOURNAL = SQLITE_OPEN_MAIN_JOURNAL,
				eOPEN_FLAG_TEMP_JOURNAL = SQLITE_OPEN_TEMP_JOURNAL,
				eOPEN_FLAG_SUBJOURNAL = SQLITE_OPEN_SUBJOURNAL,
				eOPEN_FLAG_MASTER_JOURNAL = SQLITE_OPEN_MASTER_JOURNAL,
				eOPEN_FLAG_NOMUTEX = SQLITE_OPEN_NOMUTEX,
				eOPEN_FLAG_FULLMUTEX = SQLITE_OPEN_FULLMUTEX,
				eOPEN_FLAG_SHAREDCACHE = SQLITE_OPEN_SHAREDCACHE,
				eOPEN_FLAG_PRIVATECACHE = SQLITE_OPEN_PRIVATECACHE,
			}	eOPEN_FLAG;

			typedef enum
				: uint8_t
			{
				eENCODING_UTF8			= 1,	// IMP: R-37514-35566
				eENCODING_UTF16LE		= 2,	// IMP: R-03371-37637
				eENCODING_UTF16BE		= 3,	// IMP: R-51971-34154
				eENCODING_UTF16			= 4,	// Use native byte order
				eENCODING_ANY			= 5,	// Deprecated
				eENCODING_UTF16_ALIGNED	= 8,	// sqlite3_create_collation only
			}	eENCODING;

			static const sqlite3_destructor_type NULL_DESTRUCTOR = 0;
		}

		DatabaseFieldInfosPtrArray SqliteGetColumns( sqlite3_stmt * statement, DatabaseConnectionPtr connection );
		DatabaseResultPtr SqliteExecute( sqlite3_stmt * statement, int & code, DatabaseFieldInfosPtrArray const & columns, DatabaseConnectionPtr connection );

		void SQLiteTry( int code, TChar const * msg, EDatabaseExceptionCodes exc, sqlite3 * database );
		void SQLiteTry( int code, std::ostream const & stream, EDatabaseExceptionCodes exc, sqlite3 * database );

		/** Base binding uupdater class
		*/
		struct SSqliteBindingBase
		{
			/** Constructor
			@param statement
				The statement
			@param connection
				The database connection
			@param index
				The parameter index
			*/
			SSqliteBindingBase( sqlite3_stmt * statement, sqlite3 * connection, uint16_t index )
				: _statement( statement )
				, _connection( connection )
				, _index( index )
			{
			}

			/** Destructor
			*/
			virtual ~SSqliteBindingBase()
			{
			}

			/** Updates the SQLite binding
			*/
			virtual void UpdateValue() = 0;

			//! The statement
			sqlite3_stmt * _statement;
			//! The database connection
			sqlite3 * _connection;
			// The parameter index
			uint16_t _index;
		};
	}
}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_SQLITE_PREREQUISITES_H___
