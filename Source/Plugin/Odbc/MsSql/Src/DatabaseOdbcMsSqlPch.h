/************************************************************************//**
 * @file DatabaseOdbcMsSqlPch.h
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

#ifndef ___DATABASE_ODBC_MSSQL_PCH_H___
#define ___DATABASE_ODBC_MSSQL_PCH_H___

#include <DatabasePluginManager.h>

#include <Database.h>
#include <DatabaseFactory.h>
#include <DatabasePlugin.h>

#include <DatabaseConnectionOdbc.h>
#include <DatabaseStatementOdbc.h>
#include <DatabaseQueryOdbc.h>
#include <DatabaseOdbcHelper.h>

#if defined( _WIN32 )
#    include <windows.h>
#    include <sql.h>
#    include <sqlext.h>
#endif

#endif // ___DATABASE_ODBC_MSSQL_PCH_H___
