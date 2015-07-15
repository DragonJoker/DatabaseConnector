/************************************************************************//**
* @file PluginDatabase.h
* @author Sylvain Doremus
* @version 1.0
* @date 4/08/2014 10:01:21 AM
*
*
* @brief Class defining a generic database plugin.
*
* @details Class defining a generic database plugin.
*
***************************************************************************/

#ifndef ___DATABASE_PLUGIN_DATABASE_H___
#define ___DATABASE_PLUGIN_DATABASE_H___

#include "DatabasePrerequisites.h"

BEGIN_NAMESPACE_DATABASE
{
	/** Class defining a generic database plugin.
	*/
	class CPluginDatabase
	{
	public:
		/** Constructor.
		@param factory
			The factory used to manage CDatabase object creation.
		*/
		DatabaseExport CPluginDatabase( CFactoryDatabase * factory );

		/** Destructor.
		*/
		DatabaseExport virtual ~CPluginDatabase();

		/** Perform the plugin initial installation sequence.
		@remarks
			An implementation must be supplied for this method. It must perform
			the startup tasks necessary to install anything else that is not dependent
			on system initialization, ie only dependent on the core. It must
			not perform any operations that would create specific objects at this stage,
			that should be done in Initialize ().
		*/
		DatabaseExport virtual void Install();

		/** Perform the final plugin uninstallation sequence.
		@remarks
			An implementation must be supplied for this method. It must perform
			the cleanup tasks which haven't already been performed in Shutdown ()
			(e.g. final deletion of custom instances, if you kept them around incase
			the system was reinitialized). At this stage you cannot be sure what other
			plugins are still loaded or active. It must therefore not perform any
			operations that would reference any specific objects - those
			should have been sorted out in the 'Shutdown' method.
		*/
		DatabaseExport virtual void Uninstall();

		/** Get plugin mame.
		@return
			Return the plugin name.
		@remarks
			An implementation must be supplied for this method to uniquely
			identify the plugin.
		*/
		DatabaseExport virtual const String GetName() const = 0;

		/** Perform any tasks the plugin needs to perform on full system initialization.
		@remarks
			An implementation must be supplied for this method. It is called
			just after the system is fully initialized. You can use this hook to create
			any resources which are have specific implementations.
		*/
		DatabaseExport virtual void Initialize() = 0;

		/** Perform any tasks the plugin needs to perform when the system is shut down.
		@remarks
			An implementation must be supplied for this method.
			This method is called just before key parts of the system are unloaded.
			You should use this hook to free up resources and decouple custom objects
			from the system, whilst all the instances of other plugins still exist.
		*/
		DatabaseExport virtual void Shutdown() = 0;

	protected:
		//! Database object factory.
		CFactoryDatabase * _factory;
		//! Plugin install state.
		bool _installed;
	};

}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_PLUGIN_DATABASE_H___
