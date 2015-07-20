/************************************************************************//**
* @file PluginDatabasePostgreSql.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 4/8/2014 4:03:23 PM
*
*
* @brief Class defining a generic MYSQL database plugin.
*
* @details Class defining a generic MYSQL database plugin.
*
***************************************************************************/

#include "DatabasePostgreSqlPch.h"

#include "FactoryDatabasePostgreSql.h"
#include "PluginDatabasePostgreSql.h"

BEGIN_NAMESPACE_DATABASE_POSTGRESQL
{
	CPluginDatabasePostgreSql::CPluginDatabasePostgreSql()
		: CPluginDatabase( new CFactoryDatabasePostgreSql() )
	{
		// Empty
	}

	CPluginDatabasePostgreSql::~CPluginDatabasePostgreSql()
	{
		// Empty
	}

	const String CPluginDatabasePostgreSql::GetName() const
	{
		return PostgreSql::PLUGIN_NAME_DATABASE_POSTGRESQL;
	}

	void CPluginDatabasePostgreSql::Initialize()
	{
	}

	void CPluginDatabasePostgreSql::Shutdown()
	{
	}

	void CPluginDatabasePostgreSql::AddBaseParameters()
	{
		// Empty
	}
}
END_NAMESPACE_DATABASE_POSTGRESQL
