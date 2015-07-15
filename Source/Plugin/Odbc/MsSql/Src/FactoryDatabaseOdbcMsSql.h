/************************************************************************//**
* @file FactoryDatabaseOdbcMsSql.h
* @author Sylvain Doremus
* @version 1.0
* @date 4/08/2014 1:20:50 PM
*
*
* @brief ODBC database factory class.
*
* @details CObject factory used to manage ODBC database creation.
*
***************************************************************************/

#ifndef ___FACTORY_DATABASE_ODBC_MSSQL_H___
#define ___FACTORY_DATABASE_ODBC_MSSQL_H___

#include "DatabaseOdbcMsSqlPrerequisites.h"

#include <DatabaseFactory.h>

BEGIN_NAMESPACE_DATABASE_ODBC_MSSQL
{
	/** ODBC database factory class.
		CObject factory used to manage ODBC database creation.
	*/
	class CFactoryDatabaseOdbcMsSql
		: public CFactoryDatabase
	{
	public:
		/** Default constructor.
		*/
		DatabaseOdbcMsSqlExport CFactoryDatabaseOdbcMsSql();

		/** Destructor.
		*/
		DatabaseOdbcMsSqlExport virtual ~CFactoryDatabaseOdbcMsSql();

		/** Get the type of the factory.
		@returns
			String describing the factory type.
		*/
		const String GetType() const
		{
			return MsSql::FACTORY_DATABASE_ODBC_MSSQL;
		}

	protected:
		/** Register creation functions to create new ODBC database objects.
		*/
		DatabaseOdbcMsSqlExport void RegisterServices();

		/** Registred the ODBC database object types that the factory can create.
		*/
		DatabaseOdbcMsSqlExport void RegisterObjectTypes();
	};
}
END_NAMESPACE_DATABASE_ODBC_MSSQL

#endif // ___FACTORY_DATABASE_ODBC_H___
