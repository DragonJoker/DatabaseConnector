/************************************************************************//**
* @file ExceptionDatabaseMySql.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/18/2014 2:47:39 PM
*
*
* @brief CDatabaseExceptionMySql class declaration and definition.
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
	/** Should be thrown when a problem occured in the MYSQL database plugin.
	*/
	class CDatabaseExceptionMySql
		: public CDatabaseException
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
		CDatabaseExceptionMySql( int number, const String & description, const std::string & source, const std::string & file, long line )
			: CDatabaseException( STR( "CDatabaseExceptionMySql" ), number, description, source, file, line )
		{
			// Empty
		}
	};

#	define MYSQL_EXCEPT( number, description ) throw CDatabaseExceptionMySql( number, description, __FUNCTION__, __FILE__, __LINE__ )
}
END_NAMESPACE_DATABASE_MYSQL

#endif // ___EXCEPTION_DATABASE_MYSQL_H___
