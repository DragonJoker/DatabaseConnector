/************************************************************************//**
 * @file DatabaseMySqlPch.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 03/14/2014 11:00:50 AM
 * 
 *
 * @brief Database MYSQL precompiled header.
 *
 * @details This file generates the Database MYSQL precompiled headers.
 *
 ***************************************************************************/

#ifndef ___DATABASE_MYSQL_PCH_H___
#define ___DATABASE_MYSQL_PCH_H___

// Stable headers wich will be used for precompilation if the compiler
// supports it. Add entries here when headers are unlikely to change.
// NB: a change to any of these headers will result in a full rebuild,
// so don't add things to this lightly.
#include <DatabasePrerequisites.h>
#include <Database.h>
#include <DatabaseConnection.h>
#include <DatabaseField.h>
#include <DatabaseParameter.h>
#include <DatabaseParameterValueSetter.h>
#include <DatabaseQuery.h>
#include <DatabaseResult.h>
#include <DatabaseRow.h>
#include <DatabaseDate.h>
#include <DatabaseDateTime.h>
#include <DatabaseDateTimeHelper.h>
#include <EFieldType.h>
#include <EParameterType.h>
#include <DatabaseException.h>
#include <DatabaseFactory.h>
#include <DatabasePlugin.h>
#include <DatabasePluginManager.h>
#include <DatabaseTime.h>
#include <DatabaseLogger.h>
#include <DatabaseStringUtils.h>

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/datatype.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/metadata.h>
#include <cppconn/parameter_metadata.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include "DatabaseMySqlPrerequisites.h"

#endif // ___DATABASE_MYSQL_PCH_H___
