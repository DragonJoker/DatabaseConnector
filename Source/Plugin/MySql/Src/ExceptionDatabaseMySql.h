/************************************************************************//**
 * @file ExceptionDatabaseMySql.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/18/2014 2:47:39 PM
 *
 *
 * @brief CExceptionDatabaseMySql class declaration and definition.
 *
 * @details Should be thrown when a problem occured in the MYSQL database plugin.
 *
 ***************************************************************************/

#ifndef ___EXCEPTION_DATABASE_MYSQL_H___
#define ___EXCEPTION_DATABASE_MYSQL_H___

#include "DatabaseMySqlPrerequisites.h"

#include <DatabaseException.h>

BEGIN_NAMESPACE_DATABASE_MYSQL
{
	/** Static definitions of MYSQL database error codes.
	*/
	enum EDatabaseMySqlExceptionCodes
	{
		EDatabaseMySqlExceptionCodes_GenericError = 0,
		EDatabaseMySqlExceptionCodes_UnknownError,
		EDatabaseMySqlExceptionCodes_StatementError,
		EDatabaseMySqlExceptionCodes_QueryError,

		EDatabaseMySqlExceptionCodes_LastCode ///< Represent the maximum number of exception code. Must be always the last.
	};

	/** Should be thrown when a problem occured in the MYSQL database plugin.
	*/
	class CExceptionDatabaseMySql
		: public CExceptionDatabase
	{
	public:
		/** Create a exception for the MYSQL database.
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
		CExceptionDatabaseMySql( int number, const String & description, const std::string & source, const std::string & file, long line )
			: CExceptionDatabase( number, description, source, STR( "CExceptionDatabaseMySql" ), file, line )
		{
			// Empty
		}
	};

}
END_NAMESPACE_DATABASE_MYSQL

#endif // ___EXCEPTION_DATABASE_MYSQL_H___
