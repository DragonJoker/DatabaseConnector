/************************************************************************//**
* @file FactoryDatabaseSqlite.h
* @author Sylvain Doremus
* @version 1.0
* @date 4/08/2014 1:20:50 PM
*
*
* @brief SQLite database factory class.
*
* @details CObject factory used to manage SQLite database creation.
*
***************************************************************************/

#ifndef ___FACTORY_DATABASE_SQLITE_H___
#define ___FACTORY_DATABASE_SQLITE_H___

#include "DatabaseSqlitePrerequisites.h"

#include <DatabaseFactory.h>

BEGIN_NAMESPACE_DATABASE_SQLITE
{
	/** SQLITE database factory class.
		CObject factory used to manage SQLite database creation.
	*/
	class CFactoryDatabaseSqlite
		: public CFactoryDatabase
	{
	public:
		/** Default constructor.
		*/
		DatabaseSqliteExport CFactoryDatabaseSqlite();

		/** Destructor.
		*/
		DatabaseSqliteExport virtual ~CFactoryDatabaseSqlite();

		/** Get the type of the factory.
		@returns
			String describing the factory type.
		*/
		inline const String GetType() const
		{
			return Sqlite::FACTORY_DATABASE_SQLITE;
		}

	protected:
		/** Register creation functions to create new SQLite database objects.
		*/
		DatabaseSqliteExport void RegisterServices();

		/** Registers the SQLite database object types that the factory can create.
		*/
		DatabaseSqliteExport void RegisterObjectTypes();
	};
}
END_NAMESPACE_DATABASE_SQLITE

#endif // ___FACTORY_DATABASE_H___
