/************************************************************************//**
* @file PluginDatabaseOdbcMsSql.h
* @author Sylvain Doremus
* @version 1.0
* @date 4/8/2014 4:03:17 PM
*
*
* @brief Class defining a generic ODBC database plugin.
*
* @details Class defining a generic ODBC database plugin.
*
***************************************************************************/

#ifndef ___PLUGIN_DATABASE_ODBC_MSSQL_H___
#define ___PLUGIN_DATABASE_ODBC_MSSQL_H___

#include "DatabaseOdbcMsSqlPrerequisites.h"

#include <DatabasePlugin.h>

BEGIN_NAMESPACE_DATABASE_ODBC_MSSQL
{
	/** Class defining a generic ODBC database plugin.
	*/
	class CPluginDatabaseOdbcMsSql
		: public CPluginDatabase
	{
	public:
		/** Default constructor.
		*/
		DatabaseOdbcMsSqlExport CPluginDatabaseOdbcMsSql();

		/** Destructor.
		*/
		DatabaseOdbcMsSqlExport virtual ~CPluginDatabaseOdbcMsSql();

		//!@copydoc Database::CPlugin::GetName
		DatabaseOdbcMsSqlExport virtual const String GetName() const;

		//!@copydoc Database::CPlugin::Initialize
		DatabaseOdbcMsSqlExport virtual void Initialize();

		//!@copydoc Database::CPlugin::Shutdown
		DatabaseOdbcMsSqlExport virtual void Shutdown();

	protected:
		//!@copydoc Database::CParameterListInterface::AddBaseParameters
		DatabaseOdbcMsSqlExport virtual void AddBaseParameters();
	};

} END_NAMESPACE_DATABASE_ODBC_MSSQL

#endif // ___PLUGIN_DATABASE_ODBC_MSSQL_H___
