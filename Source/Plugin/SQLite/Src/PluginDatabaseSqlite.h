/************************************************************************//**
 * @file PluginDatabaseSqlite.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 4/8/2014 4:03:17 PM
 *
 *
 * @brief Class defining a generic SQLite database plugin.
 *
 * @details Class defining a generic SQLite database plugin.
 *
 ***************************************************************************/

#ifndef ___PLUGIN_DATABASE_SQLITE_H___
#define ___PLUGIN_DATABASE_SQLITE_H___

#include "DatabaseSqlitePrerequisites.h"

#include <DatabasePlugin.h>

BEGIN_NAMESPACE_DATABASE_SQLITE
{
	/** Class defining a generic SQLite database plugin.
	*/
	class CPluginDatabaseSqlite
		: public CPluginDatabase
	{
	public:
		/** Default constructor.
		*/
		DatabaseSqliteExport CPluginDatabaseSqlite();

		/** Destructor.
		*/
		DatabaseSqliteExport virtual ~CPluginDatabaseSqlite();

		///@copydoc Areva::ARIA::Native::Core::CPlugin::GetName
		DatabaseSqliteExport virtual const String GetName() const;

		///@copydoc Areva::ARIA::Native::Core::CPlugin::Initialize
		DatabaseSqliteExport virtual void Initialize();

		///@copydoc Areva::ARIA::Native::Core::CPlugin::Shutdown
		DatabaseSqliteExport virtual void Shutdown();

	protected:
		///@copydoc Areva::ARIA::Native::Core::CParameterListInterface::AddBaseParameters
		DatabaseSqliteExport virtual void AddBaseParameters();
	};

} END_NAMESPACE_DATABASE_SQLITE

#endif // ___PLUGIN_DATABASE_SQLITE_H___
