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
		CPluginDatabaseOdbcMySql();

		/** Destructor.
		*/
		virtual ~CPluginDatabaseOdbcMySql();

		//!@copydoc Database::CPlugin::GetName
		virtual const String GetName() const;

		//!@copydoc Database::CPlugin::Initialize
		virtual void Initialize();

		//!@copydoc Database::CPlugin::Shutdown
		virtual void Shutdown();

	protected:
		//!@copydoc Database::CParameterListInterface::AddBaseParameters
		virtual void AddBaseParameters();
	};
}
END_NAMESPACE_DATABASE_ODBC_MYSQL

#endif // ___PLUGIN_DATABASE_ODBC_MYSQL_H___
