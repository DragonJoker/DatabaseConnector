/************************************************************************//**
 * @file DatabaseOdbcPch.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 03/14/2014 11:00:50 AM
 *
 *
 * @brief Database ODBC precompiled header.
 *
 * @details This file generates the Database ODBC precompiled headers.
 *
 ***************************************************************************/

#ifndef ___DATABASE_ODBC_PCH_H___
#define ___DATABASE_ODBC_PCH_H___

// Stable headers wich will be used for precompilation if the compiler
// supports it. Add entries here when headers are unlikely to change.
// NB: a change to any of these headers will result in a full rebuild,
// so don't add things to this lightly.

#include <DatabaseException.h>
#include <DatabaseLogger.h>
#include <DatabasePluginManager.h>
#include <DatabaseStringUtils.h>

#include <Database.h>
#include <DatabaseConnection.h>
#include <DatabaseField.h>
#include <DatabaseResult.h>
#include <DatabaseRow.h>
#include <DatabaseStatement.h>
#include <DatabaseParameter.h>
#include <EErrorType.h>
#include <EFieldType.h>
#include <DatabaseException.h>
#include <DatabaseFactory.h>
#include <DatabasePlugin.h>
#include <DatabaseDate.h>
#include <DatabaseDateTime.h>
#include <DatabaseDateTimeHelper.h>
#include <DatabaseTime.h>

#include <functional>

#include <boost/signals2.hpp>

#include "DatabaseOdbcPrerequisites.h"

#include <sql.h>

#if defined( _WIN32 )
#	include <odbcss.h>
#endif

#endif // ___DATABASE_ODBC_PCH_H___
