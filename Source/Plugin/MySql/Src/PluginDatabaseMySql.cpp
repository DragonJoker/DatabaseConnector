/************************************************************************//**
* @file PluginDatabaseMySql.cpp
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

#include "DatabaseMySqlPch.h"

#include "FactoryDatabaseMySql.h"
#include "PluginDatabaseMySql.h"

BEGIN_NAMESPACE_DATABASE_MYSQL
{
	CPluginDatabaseMySql::CPluginDatabaseMySql()
		: CPluginDatabase( new CFactoryDatabaseMySql() )
	{
		// Empty
	}

	CPluginDatabaseMySql::~CPluginDatabaseMySql()
	{
		// Empty
	}

	const String CPluginDatabaseMySql::GetName() const
	{
		return MySql::PLUGIN_NAME_DATABASE_MYSQL;
	}

	void CPluginDatabaseMySql::Initialize()
	{
	}

	void CPluginDatabaseMySql::Shutdown()
	{
	}

	void CPluginDatabaseMySql::AddBaseParameters()
	{
		// Empty
	}
}
END_NAMESPACE_DATABASE_MYSQL
