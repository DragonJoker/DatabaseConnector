/************************************************************************//**
 * @file ExceptionDatabaseOdbc.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/18/2014 2:47:39 PM
 *
 *
 * @brief CDatabaseExceptionOdbc class declaration and definition.
 *
 * @details Should be thrown when a problem occured in the ODBC database plugin.
 *
 ***************************************************************************/

#ifndef ___EXCEPTION_DATABASE_ODBC_H___
#define ___EXCEPTION_DATABASE_ODBC_H___

#include "DatabaseOdbcPrerequisites.h"

#include <DatabaseException.h>

BEGIN_NAMESPACE_DATABASE_ODBC
{
	/** Should be thrown when a problem occured in the ODBC database plugin.
	*/
	class CDatabaseExceptionOdbc
		: public CDatabaseException
	{
	public:
		/** Create a exception for the ODBC database.
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
		CDatabaseExceptionOdbc( int number, const String & description, const std::string & source, const std::string & file, long line )
			: CDatabaseException( STR( "CDatabaseExceptionOdbc" ), number, description, source, file, line )
		{
			// Empty
		}
	};

#	define ODBC_EXCEPT( number, description ) throw CDatabaseExceptionOdbc( number, description, __FUNCTION__, __FILE__, __LINE__ )
}
END_NAMESPACE_DATABASE_ODBC

#endif // ___EXCEPTION_DATABASE_ODBC_H___
