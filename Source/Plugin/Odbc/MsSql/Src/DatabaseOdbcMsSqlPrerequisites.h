/************************************************************************//**
* @file DatabaseOdbcMsSqlPrerequisites.h
* @author Sylvain Doremus
* @version 1.0
* @date 03/14/2014 11:48:00 AM
*
*
* @brief Database ODBC prerequisite header.
*
* @details This file contains all Database ODBC prerequisite instructions.
*
***************************************************************************/

#ifndef ___DATABASE_ODBC_MSSQL_PREREQUISITES_H___
#define ___DATABASE_ODBC_MSSQL_PREREQUISITES_H___

#include <DatabaseOdbcPrerequisites.h>

#define BEGIN_NAMESPACE_DATABASE_ODBC_MSSQL BEGIN_NAMESPACE_DATABASE_ODBC { namespace MsSql
#define NAMESPACE_DATABASE_ODBC_MSSQL NAMESPACE_DATABASE_ODBC::MsSql
#define END_NAMESPACE_DATABASE_ODBC_MSSQL } END_NAMESPACE_DATABASE_ODBC

BEGIN_NAMESPACE_DATABASE_ODBC
{
	namespace MsSql
	{

		// Pre-declare classes
		// Allows use of pointers in header files without including individual .h
		// so decreases dependencies between files
		class CDatabaseConnectionOdbcMsSql;
		class CPluginDatabaseOdbcMsSql;
		class CFactoryDatabaseOdbcMsSql;

		// Factory constants
		const String FACTORY_DATABASE_ODBC_MSSQL = STR( "Factory Database Odbc MsSql" );

		// Plugin constants
		const String DATABASE_ODBC_MSSQL_TYPE = STR( "Database.Odbc.MsSql" );
		const String PLUGIN_NAME_DATABASE_ODBC_MSSQL = STR( "Plugin Database Odbc MsSql" );
	}
}
END_NAMESPACE_DATABASE_ODBC

#endif // ___DATABASE_ODBC_MSSQL_PREREQUISITES_H___
