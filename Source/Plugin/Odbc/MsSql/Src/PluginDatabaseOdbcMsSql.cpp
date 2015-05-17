/************************************************************************//**
 * @file PluginDatabaseOdbcMsSql.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 4/8/2014 4:03:23 PM
 *
 *
 * @brief Class defining a generic ODBC database plugin.
 *
 * @details Class defining a generic ODBC database plugin.
 *
 ***************************************************************************/

#include "DatabaseOdbcMsSqlPch.h"

#include "FactoryDatabaseOdbcMsSql.h"
#include "PluginDatabaseOdbcMsSql.h"

BEGIN_NAMESPACE_DATABASE_ODBC_MSSQL
{
	CPluginDatabaseOdbcMsSql::CPluginDatabaseOdbcMsSql()
		: CPluginDatabase( new CFactoryDatabaseOdbcMsSql() )
	{
		// Empty
	}

	CPluginDatabaseOdbcMsSql::~CPluginDatabaseOdbcMsSql()
	{
		// Empty
	}

	const String CPluginDatabaseOdbcMsSql::GetName() const
	{
		return MsSql::PLUGIN_NAME_DATABASE_ODBC_MSSQL;
	}

	void CPluginDatabaseOdbcMsSql::Initialize()
	{
	}

	void CPluginDatabaseOdbcMsSql::Shutdown()
	{
	}

	void CPluginDatabaseOdbcMsSql::AddBaseParameters()
	{
		// Empty
	}
}
END_NAMESPACE_DATABASE_ODBC_MSSQL
