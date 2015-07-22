/************************************************************************//**
* @file DatabaseMySqlPrerequisites.h
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

#ifndef ___DATABASE_MYSQL_PREREQUISITES_H___
#define ___DATABASE_MYSQL_PREREQUISITES_H___

#include <DatabasePrerequisites.h>

#define BEGIN_NAMESPACE_DATABASE_MYSQL BEGIN_NAMESPACE_DATABASE { namespace MySql
#define NAMESPACE_DATABASE_MYSQL NAMESPACE_DATABASE::MySql
#define END_NAMESPACE_DATABASE_MYSQL } END_NAMESPACE_DATABASE

BEGIN_NAMESPACE_DATABASE
{
	namespace MySql
	{
		// Pre-declare classes
		// Allows use of pointers in header files without including individual .h
		// so decreases dependencies between files
		class CDatabaseConnectionMySql;
		class CDatabaseMySql;
		class CDatabaseStatementMySql;
		class CDatabaseParameterMySql;
		class CExceptionDatabaseMySql;
		class CPluginDatabaseMySql;
		class CFactoryDatabaseMySql;
		struct SOutMySqlBindBase;

		// Pointers
		DECLARE_SMART_PTR( DatabaseConnectionMySql );
		DECLARE_SMART_PTR( DatabaseStatementMySql );
		DECLARE_SMART_PTR( DatabaseParameterMySql );

		typedef std::vector< DatabaseParameterMySqlWPtr > DatabaseParameterMySqlPtrArray;

		// Factory constants
		const String FACTORY_DATABASE_MYSQL = STR( "Factory Database MySql" );

		// Plugin constants
		const String DATABASE_MYSQL_TYPE = STR( "Database.MySql" );
		const String PLUGIN_NAME_DATABASE_MYSQL = STR( "Plugin Database MySql" );
	}
}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_MYSQL_PREREQUISITES_H___
