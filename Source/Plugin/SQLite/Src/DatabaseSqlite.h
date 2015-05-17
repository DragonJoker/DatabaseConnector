/************************************************************************//**
 * @file DatabaseSqlite.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 *
 *
 * @brief CDatabase class declaration.
 *
 * @details Describes an SQLite database.
 *
 ***************************************************************************/

#ifndef ___DATABASE_SQLITE_H___
#define ___DATABASE_SQLITE_H___

#include "DatabaseSqlitePrerequisites.h"

#include <Database.h>

BEGIN_NAMESPACE_DATABASE_SQLITE
{
	/** Describes an SQLite database.
	*/
	class CDatabaseSqlite
		: public CDatabase
	{
	public:

		/** Default constructor.
		 */
		DatabaseSqliteExport CDatabaseSqlite();

		/** Destructor.
		 */
		DatabaseSqliteExport virtual ~CDatabaseSqlite();

		///@copydoc Areva::ARIA::Native::CObject::GetType
		DatabaseSqliteExport virtual const String & GetType()const;

		/** Creator function
		 */
		DatabaseSqliteExport static CDatabase * Create();

	protected:
		/** Create a connection to the database.
		@param[out] connectionString
		    Created connection string.
		@return
		    Database connection.
		*/
		DatabaseSqliteExport virtual DatabaseConnectionPtr DoCreateConnection( String & connectionString ) const;
	};
}
END_NAMESPACE_DATABASE_SQLITE

#endif // ___DATABASE_SQLITE_H___
