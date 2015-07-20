/************************************************************************//**
* @file PluginDatabasePostgreSql.h
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

#ifndef ___PLUGIN_DATABASE_POSTGRESQL_H___
#define ___PLUGIN_DATABASE_POSTGRESQL_H___

#include "DatabasePostgreSqlPrerequisites.h"

#include <DatabasePlugin.h>

BEGIN_NAMESPACE_DATABASE_POSTGRESQL
{
	/** Class defining a generic MYSQL database plugin.
	*/
	class CPluginDatabasePostgreSql
		: public CPluginDatabase
	{
	public:
		/** Default constructor.
		*/
		DatabasePostgreSqlExport CPluginDatabasePostgreSql();

		/** Destructor.
		*/
		DatabasePostgreSqlExport virtual ~CPluginDatabasePostgreSql();

		///@copydoc Database::CPlugin::GetName
		DatabasePostgreSqlExport virtual const String GetName() const;

		///@copydoc Database::CPlugin::Initialize
		DatabasePostgreSqlExport virtual void Initialize();

		///@copydoc Database::CPlugin::Shutdown
		DatabasePostgreSqlExport virtual void Shutdown();

	protected:
		///@copydoc Database::CParameterListInterface::AddBaseParameters
		DatabasePostgreSqlExport virtual void AddBaseParameters();
	};

} END_NAMESPACE_DATABASE_POSTGRESQL

#endif // ___PLUGIN_DATABASE_POSTGRESQL_H___
