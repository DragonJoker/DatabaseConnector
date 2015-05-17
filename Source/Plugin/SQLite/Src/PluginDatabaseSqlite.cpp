/************************************************************************//**
 * @file PluginDatabaseSqlite.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 4/8/2014 4:03:23 PM
 *
 *
 * @brief Class defining a generic SQLITE database plugin.
 *
 * @details Class defining a generic SQLITE database plugin.
 *
 ***************************************************************************/

#include "DatabaseSqlitePch.h"

#include "FactoryDatabaseSqlite.h"
#include "PluginDatabaseSqlite.h"

BEGIN_NAMESPACE_DATABASE_SQLITE
{
	CPluginDatabaseSqlite::CPluginDatabaseSqlite()
		: CPluginDatabase( new CFactoryDatabaseSqlite() )
	{
		// Empty
	}

	CPluginDatabaseSqlite::~CPluginDatabaseSqlite()
	{
		// Empty
	}

	const String CPluginDatabaseSqlite::GetName() const
	{
		return Sqlite::PLUGIN_NAME_DATABASE_SQLITE;
	}

	void CPluginDatabaseSqlite::Initialize()
	{
	}

	void CPluginDatabaseSqlite::Shutdown()
	{
	}

	void CPluginDatabaseSqlite::AddBaseParameters()
	{
		// Empty
	}

} END_NAMESPACE_DATABASE_SQLITE
