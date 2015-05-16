/************************************************************************//**
 * @file PluginDatabaseOdbcMySql.cpp
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

#include "DatabaseOdbcMySqlPch.h"

#include "FactoryDatabaseOdbcMySql.h"
#include "PluginDatabaseOdbcMySql.h"

BEGIN_NAMESPACE_DATABASE_ODBC_MYSQL
{
    CPluginDatabaseOdbcMySql::CPluginDatabaseOdbcMySql()
        : CPluginDatabase( new CFactoryDatabaseOdbcMySql() )
    {
        // Empty
    }
    
    CPluginDatabaseOdbcMySql::~CPluginDatabaseOdbcMySql()
    {
        // Empty
    }
    
    const String CPluginDatabaseOdbcMySql::GetName() const
    {
        return MySql::PLUGIN_NAME_DATABASE_ODBC_MYSQL;
    }
    
    void CPluginDatabaseOdbcMySql::Initialize()
    {
    }
    
    void CPluginDatabaseOdbcMySql::Shutdown()
    {
    }
    
    void CPluginDatabaseOdbcMySql::AddBaseParameters()
    {
        // Empty
    }
    
} END_NAMESPACE_DATABASE_ODBC_MYSQL
