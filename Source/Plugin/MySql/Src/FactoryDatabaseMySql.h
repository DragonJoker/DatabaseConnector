/************************************************************************//**
* @file FactoryDatabaseMySql.h
* @author Sylvain Doremus
* @version 1.0
* @date 4/08/2014 1:20:50 PM
*
*
* @brief MYSQL database factory class.
*
* @details CObject factory used to manage MYSQL database creation.
*
***************************************************************************/

#ifndef ___FACTORY_DATABASE_MYSQL_H___
#define ___FACTORY_DATABASE_MYSQL_H___

#include "DatabaseMySqlPrerequisites.h"

#include <DatabaseFactory.h>

BEGIN_NAMESPACE_DATABASE_MYSQL
{
	/** MYSQL database factory class.
		CObject factory used to manage MYSQL database creation.
	*/
	class CFactoryDatabaseMySql
		: public CFactoryDatabase
	{
	public:
		/** Default constructor.
		*/
		CFactoryDatabaseMySql();

		/** Destructor.
		*/
		virtual ~CFactoryDatabaseMySql();

		/** Get the type of the factory.
		@returns
			String describing the factory type.
		*/
		inline const String GetType() const
		{
			return MySql::FACTORY_DATABASE_MYSQL;
		}

	protected:
		/** Register creation functions to create new MYSQL database objects.
		*/
		void RegisterServices();

		/** Registred the MYSQL database object types that the factory can create.
		*/
		void RegisterObjectTypes();
	};
}
END_NAMESPACE_DATABASE_MYSQL

#endif // ___FACTORY_DATABASE_H___
