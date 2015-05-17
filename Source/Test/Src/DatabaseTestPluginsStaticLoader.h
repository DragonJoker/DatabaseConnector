/************************************************************************//**
 * @file DatabaseTestPluginsStaticLoader.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 16/06/2014 12:04:36
 *
 *
 * @brief Specialization of CStaticLibLoader for Database needed plugins
 *
 * @details Loads the static library of the Database needed plugins
 *
 ***************************************************************************/

#ifndef ___DATABASE_TEST_STATIC_LOADER_H___
#define ___DATABASE_TEST_STATIC_LOADER_H___

#include "DatabaseTestPrerequisites.h"

#if defined( STATIC_LIB )
#   include <PluginDatabaseMySql.h>
#   include <PluginDatabaseOdbcMySql.h>
#   include <PluginDatabaseOdbcMsSql.h>
#endif

BEGIN_NAMESPACE_DATABASE_TEST
{
	/** Specialization of CStaticLibLoader for Database plugins
	*/
	class CTestPluginsStaticLoader
	{
	public:
		/** Default constructor
		*/
		CTestPluginsStaticLoader();

		/** Destructor
		*/
		virtual ~CTestPluginsStaticLoader();

		void Load( bool mySql, bool odbcMySql, bool odbcMsSql );
		void Unload();

	private:
		/** Load all the needed plugins.
		*/
		void OnLoad();

		/** Unload all the needed plugins.
		*/
		void OnUnload();

#ifdef STATIC_LIB
		//! ODBC MySSQL database plugin
		Odbc::MySql::CPluginDatabaseOdbcMySql * _pluginOdbcMySql;
		//! ODBC MSSQL database plugin
		Odbc::MsSql::CPluginDatabaseOdbcMsSql * _pluginOdbcMsSql;
		//! MySQL database plugin
		MySql::CPluginDatabaseMySql * _pluginMySql;
#endif
		bool _mySql;
		bool _odbcMySql;
		bool _odbcMsSql;
	};
}
END_NAMESPACE_DATABASE_TEST

#endif //___DATABASE_TEST_STATIC_LOADER_H___

