/************************************************************************//**
* @file ExceptionDatabaseSqlite.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/18/2014 2:47:39 PM
*
*
* @brief CExceptionDatabaseSqlite class declaration and definition.
*
* @details Should be thrown when a problem occured in the SQLite database plugin.
*
***************************************************************************/

#ifndef ___EXCEPTION_DATABASE_SQLITE_H___
#define ___EXCEPTION_DATABASE_SQLITE_H___

#include "DatabaseSqlitePrerequisites.h"

#include <DatabaseException.h>

BEGIN_NAMESPACE_DATABASE_SQLITE
{
	/** Static definitions of SQLite database error codes.
	*/
	enum EDatabaseSqliteExceptionCodes
	{
		EDatabaseSqliteExceptionCodes_GenericError = 0,
		EDatabaseSqliteExceptionCodes_UnknownError,
		EDatabaseSqliteExceptionCodes_StatementError,
		EDatabaseSqliteExceptionCodes_QueryError,

		EDatabaseSqliteExceptionCodes_LastCode //!< Represent the maximum number of exception code. Must be always the last.
	};

	/** Should be thrown when a problem occured in the SQLite database plugin.
	*/
	class CExceptionDatabaseSqlite
		: public CExceptionDatabase
	{
	public:
		/** Create a exception for the SQLite database.
		@param number
			Error code.
		@param description
			Error description.
		@param source
			Error source function.
		@param file
			Source file name.
		@param line
			Source file line number.
		*/
		CExceptionDatabaseSqlite( int number, const String & description, const std::string & source, const std::string & file, long line )
			: CExceptionDatabase( number, description, source, STR( "CExceptionDatabaseSqlite" ), file, line )
		{
			// Empty
		}
	};

#define SQLITE_EXCEPT( code, message ) throw CExceptionDatabaseSqlite( code, message, __FUNCTION__, __FILE__, __LINE__ )
}
END_NAMESPACE_DATABASE_SQLITE

#endif // ___EXCEPTION_DATABASE_SQLITE_H___
