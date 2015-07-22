/************************************************************************//**
* @file DatabasePostgreSqlPrerequisites.h
* @author Sylvain Doremus
* @version 1.0
* @date 03/14/2014 11:48:00 AM
*
*
* @brief Database MYSQL prerequisite header.
*
* @details This file contains all Database MYSQL prerequisite instructions.
*
***************************************************************************/

#ifndef ___DATABASE_POSTGRESQL_PREREQUISITES_H___
#define ___DATABASE_POSTGRESQL_PREREQUISITES_H___

#include <DatabasePrerequisites.h>

#define BEGIN_NAMESPACE_DATABASE_POSTGRESQL BEGIN_NAMESPACE_DATABASE { namespace PostgreSql
#define NAMESPACE_DATABASE_POSTGRESQL NAMESPACE_DATABASE::PostgreSql
#define END_NAMESPACE_DATABASE_POSTGRESQL } END_NAMESPACE_DATABASE

BEGIN_NAMESPACE_DATABASE
{
	namespace PostgreSql
	{
		// Pre-declare classes
		// Allows use of pointers in header files without including individual .h
		// so decreases dependencies between files
		class CDatabaseConnectionPostgreSql;
		class CDatabasePostgreSql;
		class CDatabaseStatementPostgreSql;
		class CDatabaseParameterPostgreSql;
		class CExceptionDatabasePostgreSql;
		class CPluginDatabasePostgreSql;
		class CFactoryDatabasePostgreSql;
		struct SOutPostgreSqlBindBase;

		// Pointers
		DECLARE_SMART_PTR( DatabaseConnectionPostgreSql );
		DECLARE_SMART_PTR( DatabaseStatementPostgreSql );
		DECLARE_SMART_PTR( DatabaseParameterPostgreSql );

		typedef std::vector< DatabaseParameterPostgreSqlWPtr > DatabaseParameterPostgreSqlPtrArray;

		// Factory constants
		const String FACTORY_DATABASE_POSTGRESQL = STR( "Factory Database PostgreSql" );

		// Plugin constants
		const String DATABASE_POSTGRESQL_TYPE = STR( "Database.PostgreSql" );
		const String PLUGIN_NAME_DATABASE_POSTGRESQL = STR( "Plugin Database PostgreSql" );
	}
}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_POSTGRESQL_PREREQUISITES_H___
