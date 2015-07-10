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
#    ifdef DatabasePluginSqlite_EXPORTS
#        define DatabaseSqliteExport __declspec ( dllexport )
#    else
#       if defined ( __MINGW32__ )
#           define DatabaseSqliteExport
#       else
#           define DatabaseSqliteExport __declspec ( dllimport )
#       endif
#   endif
#else
#    define DatabaseSqliteExport
#endif

#if defined( STATIC_LIB )
#   define CPPCONN_PUBLIC_FUNC
#endif

#define BEGIN_NAMESPACE_DATABASE_SQLITE     BEGIN_NAMESPACE_DATABASE { namespace Sqlite
#define NAMESPACE_DATABASE_SQLITE           NAMESPACE_DATABASE::Sqlite
#define END_NAMESPACE_DATABASE_SQLITE       END_NAMESPACE_DATABASE }

BEGIN_NAMESPACE_DATABASE
{
	/** Sqlite namespace
	*/
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
#include "sqlite3.h"

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
			}   eCODE;

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
			}   eEXTENDED_CODE;

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
			}   eOPEN_FLAG;

			typedef sqlite3_destructor_type DestructorType;
			static const SQLite::DestructorType NULL_DESTRUCTOR = 0;

			typedef sqlite3_stmt Statement;
			typedef sqlite3 Database;
			typedef sqlite3_int64 Int64;

			class Value
			{
			public:
				Value( sqlite3_value * value )
					: _value( value )
				{
				}

				int GetType()
				{
					return sqlite3_value_type( _value );
				}
				int GetNumericType()
				{
					return sqlite3_value_numeric_type( _value );
				}
				int GetBytes()
				{
					return sqlite3_value_bytes( _value );
				}
				int GetBytes16()
				{
					return  sqlite3_value_bytes16( _value );
				}

				const void * AsBlob()
				{
					return sqlite3_value_blob( _value );
				}
				double AsDouble()
				{
					return  sqlite3_value_double( _value );
				}
				int AsInt()
				{
					return  sqlite3_value_int( _value );
				}
				sqlite3_int64 AsInt64()
				{
					return  sqlite3_value_int64( _value );
				}
				const unsigned char * AsText()
				{
					return sqlite3_value_text( _value );
				}
				const void * AsText16()
				{
					return sqlite3_value_text16( _value );
				}
				const void * AsText16le()
				{
					return sqlite3_value_text16le( _value );
				}
				const void * AsText16be()
				{
					return sqlite3_value_text16be( _value );
				}

				operator sqlite3_value const * ()const
				{
					return _value;
				}

			private:
				sqlite3_value * _value;
			};

			inline eCODE Step( Statement * pStatement )
			{
				return eCODE( sqlite3_step( pStatement ) );
			}
			inline eCODE Finalize( Statement * pStatement )
			{
				return eCODE( sqlite3_finalize( pStatement ) );
			}
			inline eCODE Reset( Statement * pStatement )
			{
				return eCODE( sqlite3_reset( pStatement ) );
			}
			inline eCODE ClearBindings( Statement * pStatement )
			{
				return eCODE( sqlite3_clear_bindings( pStatement ) );
			}
			inline eCODE ColumnCount( Statement * pStatement )
			{
				return eCODE( sqlite3_column_count( pStatement ) );
			}
			inline char const * ColumnName( Statement * pStatement, int iCol )
			{
				return sqlite3_column_name( pStatement, iCol );
			}
			inline void const * ColumnName16( Statement * pStatement, int iCol )
			{
				return sqlite3_column_name16( pStatement, iCol );
			}
			inline char const * ColumnDecltype( Statement * pStatement, int iCol )
			{
				return sqlite3_column_decltype( pStatement, iCol );
			}
			inline void const * ColumnDecltype16( Statement * pStatement, int iCol )
			{
				return sqlite3_column_decltype16( pStatement, iCol );
			}
			inline void const * ColumnBlob( Statement * pStatement, int iCol )
			{
				return sqlite3_column_blob( pStatement, iCol );
			}
			inline int ColumnBytes( Statement * pStatement, int iCol )
			{
				return sqlite3_column_bytes( pStatement, iCol );
			}
			inline int ColumnBytes16( Statement * pStatement, int iCol )
			{
				return sqlite3_column_bytes16( pStatement, iCol );
			}
			inline double ColumnDouble( Statement * pStatement, int iCol )
			{
				return sqlite3_column_double( pStatement, iCol );
			}
			inline int ColumnInt( Statement * pStatement, int iCol )
			{
				return sqlite3_column_int( pStatement, iCol );
			}
			inline Int64 ColumnInt64( Statement * pStatement, int iCol )
			{
				return sqlite3_column_int64( pStatement, iCol );
			}
			inline const unsigned char * ColumnText( Statement * pStatement, int iCol )
			{
				return sqlite3_column_text( pStatement, iCol );
			}
			inline void const * ColumnText16( Statement * pStatement, int iCol )
			{
				return sqlite3_column_text16( pStatement, iCol );
			}
			inline int ColumnType( Statement * pStatement, int iCol )
			{
				return sqlite3_column_type( pStatement, iCol );
			}
			inline Value ColumnValue( Statement * pStatement, int iCol )
			{
				return Value( sqlite3_column_value( pStatement, iCol ) );
			}

			inline eCODE BindBlob( Statement * pStatement, int iCol, const void * pValue, int iSizeInBytes, DestructorType pfnDestructor )
			{
				return eCODE( sqlite3_bind_blob( pStatement, iCol, pValue, iSizeInBytes, pfnDestructor ) );
			}
			inline eCODE BindDouble( Statement * pStatement, int iCol, double dValue )
			{
				return eCODE( sqlite3_bind_double( pStatement, iCol, dValue ) );
			}
			inline eCODE BindInt( Statement * pStatement, int iCol, int iValue )
			{
				return eCODE( sqlite3_bind_int( pStatement, iCol, iValue ) );
			}
			inline eCODE BindInt64( Statement * pStatement, int iCol, Int64 i64value )
			{
				return eCODE( sqlite3_bind_int64( pStatement, iCol, i64value ) );
			}
			inline eCODE BindNull( Statement * pStatement, int iCol )
			{
				return eCODE( sqlite3_bind_null( pStatement, iCol ) );
			}
			inline eCODE BindText( Statement * pStatement, int iCol, const char * szValue, int iSizeInBytes, DestructorType pfnDestructor )
			{
				return eCODE( sqlite3_bind_text( pStatement, iCol, szValue, iSizeInBytes, pfnDestructor ) );
			}
			inline eCODE BindText16( Statement * pStatement, int iCol, const void * wszValue, int iSizeInBytes, DestructorType pfnDestructor )
			{
				return eCODE( sqlite3_bind_text16( pStatement, iCol, wszValue, iSizeInBytes, pfnDestructor ) );
			}
			inline eCODE BindValue( Statement * pStatement, int iCol, const Value & pValue )
			{
				return eCODE( sqlite3_bind_value( pStatement, iCol, pValue ) );
			}
			inline eCODE BindZeroblob( Statement * pStatement, int iCol, int iSizeInBytes )
			{
				return eCODE( sqlite3_bind_zeroblob( pStatement, iCol, iSizeInBytes ) );
			}
			inline int BindParameterCount( Statement * pStatement )
			{
				return sqlite3_bind_parameter_count( pStatement );
			}

			inline eCODE Errcode( Database * pDb )
			{
				return eCODE( sqlite3_errcode( pDb ) );
			}
			inline eEXTENDED_CODE ExtendedErrcode( Database * pDb )
			{
				return eEXTENDED_CODE( sqlite3_extended_errcode( pDb ) );
			}
			inline char const * Errmsg( Database * pDb )
			{
				return sqlite3_errmsg( pDb );
			}
			inline void const * Errmsg16( Database * pDb )
			{
				return sqlite3_errmsg16( pDb );
			}

			inline eCODE Open( char const * szFilename, Database ** ppDb )
			{
				return eCODE( sqlite3_open( szFilename, ppDb ) );
			}
			inline eCODE Open16( void const * wszfilename, Database ** ppDb )
			{
				return eCODE( sqlite3_open16( wszfilename, ppDb ) );
			}
			inline eCODE OpenV2( char const * szFilename, Database ** ppDb, int iFlags, char const * szVfs )
			{
				return eCODE( sqlite3_open_v2( szFilename, ppDb, iFlags, szVfs ) );
			}
			inline eCODE Close( Database * pDb )
			{
				return eCODE( sqlite3_close( pDb ) );
			}
			inline eCODE DbStatus( Database * pDb, int op, int * pCur, int * pHiwtr, int resetFlg )
			{
				return eCODE( sqlite3_db_status( pDb, op, pCur, pHiwtr, resetFlg ) );
			}

			inline eCODE Prepare( Database * pDb, char const * szSql, int iByte, Statement ** ppStmt, char const ** pszTail )
			{
				return eCODE( sqlite3_prepare( pDb, szSql, iByte, ppStmt, pszTail ) );
			}
			inline eCODE PrepareV2( Database * pDb, char const * szSql, int iByte, Statement ** ppStmt, char const ** pszTail )
			{
				return eCODE( sqlite3_prepare_v2( pDb, szSql, iByte, ppStmt, pszTail ) );
			}
			inline eCODE Prepare16( Database * pDb, char const * szSql, int iByte, Statement ** ppStmt, void const ** pszTail )
			{
				return eCODE( sqlite3_prepare16( pDb, szSql, iByte, ppStmt, pszTail ) );
			}
			inline eCODE Prepare16V2( Database * pDb, char const * szSql, int iByte, Statement ** ppStmt, void const ** pszTail )
			{
				return eCODE( sqlite3_prepare16_v2( pDb, szSql, iByte, ppStmt, pszTail ) );
			}
		}

		DatabaseFieldInfosPtrArray SqliteGetColumns( SQLite::Statement * statement, DatabaseConnectionPtr connection );
		DatabaseResultPtr SqliteExecute( SQLite::Statement * statement, SQLite::eCODE & code, DatabaseFieldInfosPtrArray const & columns, DatabaseConnectionPtr connection );

		void SQLiteTry( SQLite::eCODE code, TChar const * msg, EDatabaseExceptionCodes exc, SQLite::Database * database );
		void SQLiteTry( SQLite::eCODE code, std::ostream const & stream, EDatabaseExceptionCodes exc, SQLite::Database * database );

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
			SSqliteBindingBase( SQLite::Statement * statement, SQLite::Database * connection, uint16_t index )
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
			SQLite::Statement * _statement;
			//! The database connection
			SQLite::Database * _connection;
			// The parameter index
			uint16_t _index;
		};
	}
}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_SQLITE_PREREQUISITES_H___
