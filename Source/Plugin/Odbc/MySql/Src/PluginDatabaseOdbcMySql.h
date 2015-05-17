/************************************************************************//**
 * @file PluginDatabaseOdbcMySql.h
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

#ifndef ___PLUGIN_DATABASE_ODBC_MYSQL_H___
#define ___PLUGIN_DATABASE_ODBC_MYSQL_H___

#include "DatabaseOdbcMySqlPrerequisites.h"

#include <DatabasePlugin.h>

BEGIN_NAMESPACE_DATABASE_ODBC_MYSQL
{
	/** Class defining a generic ODBC database plugin.
	*/
	class CPluginDatabaseOdbcMySql
		: public CPluginDatabase
	{
	public:
		/** Default constructor.
		*/
		DatabaseOdbcMySqlExport CPluginDatabaseOdbcMySql();

		/** Destructor.
		*/
		DatabaseOdbcMySqlExport virtual ~CPluginDatabaseOdbcMySql();

		///@copydoc Areva::ARIA::Native::Core::CPlugin::GetName
		DatabaseOdbcMySqlExport virtual const String GetName() const;

		///@copydoc Areva::ARIA::Native::Core::CPlugin::Initialize
		DatabaseOdbcMySqlExport virtual void Initialize();

		///@copydoc Areva::ARIA::Native::Core::CPlugin::Shutdown
		DatabaseOdbcMySqlExport virtual void Shutdown();

	protected:
		///@copydoc Areva::ARIA::Native::Core::CParameterListInterface::AddBaseParameters
		DatabaseOdbcMySqlExport virtual void AddBaseParameters();
	};

} END_NAMESPACE_DATABASE_ODBC_MYSQL

#endif // ___PLUGIN_DATABASE_ODBC_MYSQL_H___
