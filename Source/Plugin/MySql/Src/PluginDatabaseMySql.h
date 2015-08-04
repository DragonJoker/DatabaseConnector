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
		CPluginDatabaseMySql();

		/** Destructor.
		*/
		virtual ~CPluginDatabaseMySql();

		///@copydoc Database::CPlugin::GetName
		virtual const String GetName() const;

		///@copydoc Database::CPlugin::Initialise
		virtual void Initialise();

		///@copydoc Database::CPlugin::Shutdown
		virtual void Shutdown();

	protected:
		///@copydoc Database::CParameterListInterface::AddBaseParameters
		virtual void AddBaseParameters();
	};

} END_NAMESPACE_DATABASE_MYSQL

#endif // ___PLUGIN_DATABASE_MYSQL_H___
