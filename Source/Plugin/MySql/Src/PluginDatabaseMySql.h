/************************************************************************//**
 * @file PluginDatabaseMySql.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 4/8/2014 4:03:17 PM
 *
 *
 * @brief Class defining a generic MYSQL database plugin.
 *
 * @details Class defining a generic MYSQL database plugin.
 *
 ***************************************************************************/

#ifndef ___PLUGIN_DATABASE_MYSQL_H___
#define ___PLUGIN_DATABASE_MYSQL_H___

#include "DatabaseMySqlPrerequisites.h"

#include <DatabasePlugin.h>

BEGIN_NAMESPACE_DATABASE_MYSQL
{
	/** Class defining a generic MYSQL database plugin.
	*/
	class CPluginDatabaseMySql
		: public CPluginDatabase
	{
	public:
		/** Default constructor.
		*/
		DatabaseMySqlExport CPluginDatabaseMySql();

		/** Destructor.
		*/
		DatabaseMySqlExport virtual ~CPluginDatabaseMySql();

		///@copydoc Database::CPlugin::GetName
		DatabaseMySqlExport virtual const String GetName() const;

		///@copydoc Database::CPlugin::Initialize
		DatabaseMySqlExport virtual void Initialize();

		///@copydoc Database::CPlugin::Shutdown
		DatabaseMySqlExport virtual void Shutdown();

	protected:
		///@copydoc Database::CParameterListInterface::AddBaseParameters
		DatabaseMySqlExport virtual void AddBaseParameters();
	};

} END_NAMESPACE_DATABASE_MYSQL

#endif // ___PLUGIN_DATABASE_MYSQL_H___
