/************************************************************************//**
 * @file DynLibManager.h
 * @author spastor
 * @version 1.0
 * @date 2/11/2013 3:42:08 PM
 * 
 *
 * @brief Manager for Dynamic-loading Libraries.
 *
 * @details Manager for Dynamic-loading Libraries.
 *
 ***************************************************************************/

#ifndef ___DATABASE_DYNAMIC_LIBRARY_MANAGER_H___
#define ___DATABASE_DYNAMIC_LIBRARY_MANAGER_H___

#include "DatabasePrerequisites.h"

BEGIN_NAMESPACE_DATABASE
{
    /** Manager for Dynamic-loading Libraries.
    @remarks
        This manager keeps a track of all the open dynamic-loading libraries,
        opens them and returns references to already-open libraries.
    */
    class CDynLibManager
    {
    public:
        /** Load the passed library.
        @param filename
            The name of the library.
            The extension can be omitted.
        */
        DatabaseExport CDynLib * Load( const String & filename );
        
        /** Unload the passed library.
        @param dynLib
            Library to unload.
        */
        DatabaseExport void Unload( CDynLib * dynLib );
        
        /** Retrieves the unique instance
        */
        DatabaseExport static CDynLibManager & Instance()
        {
            static CDynLibManager singleton;
            return singleton;
        }
        
    protected:
        /** Default constructor.
        @note
            Should never be called as the singleton is automatically
            created during the creation of application.
        */
        DatabaseExport CDynLibManager();
        
        /** Destructor.
        */
        DatabaseExport virtual ~CDynLibManager();
        
    protected:
        std::map< String, CDynLib * > _libList; //!< Map of CDynLib instances registered.
    };
    
} END_NAMESPACE_DATABASE

#endif // ___DATABASE_DYNAMIC_LIBRARY_MANAGER_H___
