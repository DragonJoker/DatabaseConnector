/************************************************************************//**
* @file ExceptionDatabasePostgreSql.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/18/2014 2:47:39 PM
*
*
* @brief CExceptionDatabasePostgreSql class declaration and definition.
*
* @details Should be thrown when a problem occured in the MYSQL database plugin.
*
***************************************************************************/

#ifndef ___EXCEPTION_DATABASE_POSTGRESQL_H___
#define ___EXCEPTION_DATABASE_POSTGRESQL_H___

#include "DatabasePostgreSqlPrerequisites.h"

#include <DatabaseException.h>

BEGIN_NAMESPACE_DATABASE_POSTGRESQL
{
	/** Static definitions of MYSQL database error codes.
	*/
	enum EDatabasePostgreSqlExceptionCodes
	{
		EDatabasePostgreSqlExceptionCodes_GenericError = 0,
		EDatabasePostgreSqlExceptionCodes_UnknownError,
		EDatabasePostgreSqlExceptionCodes_StatementError,
		EDatabasePostgreSqlExceptionCodes_QueryError,

		EDatabasePostgreSqlExceptionCodes_LastCode ///< Represent the maximum number of exception code. Must be always the last.
	};

	/** Should be thrown when a problem occured in the MYSQL database plugin.
	*/
	class CExceptionDatabasePostgreSql
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
		CExceptionDatabasePostgreSql( int number, const String & description, const std::string & source, const std::string & file, long line )
			: CExceptionDatabase( number, description, source, STR( "CExceptionDatabasePostgreSql" ), file, line )
		{
			// Empty
		}
	};

#	define POSTGRESQL_EXCEPT( number, description ) throw CExceptionDatabasePostgreSql( number, description, __FUNCTION__, __FILE__, __LINE__ )
}
END_NAMESPACE_DATABASE_POSTGRESQL

#endif // ___EXCEPTION_DATABASE_POSTGRESQL_H___