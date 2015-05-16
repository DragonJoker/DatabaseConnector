/************************************************************************//**
 * @file FactoryObjectManager.h
 * @author spastor
 * @version 1.0
 * @date 2/13/2013 4:12:20 PM
 * 
 *
 * @brief This manager manages factory objects.
 *
 * @details This class not only manages the object themselves, it also manages
 *          the factory classes which allow the creation of specific objects.
 *
 ***************************************************************************/

#ifndef ___DATABASE_FACTORY_MANAGER_H___
#define ___DATABASE_FACTORY_MANAGER_H___

#include "DatabasePrerequisites.h"

BEGIN_NAMESPACE_DATABASE
{
    /** This manager manages factory objects.
        This class is not only managing the objects themselves,
        it also manages the factory classes which allow the creation of specific objects.
    */
    class CFactoryManager
    {
    public:
        /** Add a new factory object.
        @param factory
            Object factory to add.
        */
        DatabaseExport virtual void AddFactory( CFactoryDatabase * factory );
        
        /** Remove a factory object.
        @param factory
            Object factory to remove.
        */
        DatabaseExport virtual void RemoveFactory( CFactoryDatabase * factory );
        
        /** Create a object instance with its given type.
        @param factoryType
            Type that identify wich factory is used to create the object.
        @param objectType
            Type that identify wich object has to create.
        @return
            Pointer on a new object of type \c type.
        @remarks
            If the factory type \c type doesn't exist, a NULL be returned.
        */
        DatabaseExport CDatabase * CreateInstance( const String & factoryType, const String & objectType );
        
        /** Create a object instance with its given type.
        @param objectType
            Type that identify wich object has to create.
        @return
            Pointer on a new object of type \c type.
        @remarks
            If the factory type \c type doesn't exist, a NULL be returned.
        */
        DatabaseExport CDatabase * CreateInstance( const String & objectType );
        
        /** Retrieves the unique instance
        */
        DatabaseExport static CFactoryManager & Instance()
        {
            static CFactoryManager singleton;
            return singleton;
        }
        
    protected:
        /** Default constructor.
        */
        DatabaseExport CFactoryManager();
        
        /** Destructor.
        */
        DatabaseExport virtual ~CFactoryManager();
        
    protected:
        /** Get the Map where the construction info is stored.
        */
        DatabaseExport std::map< String, CFactoryDatabase * > * GetMapFactories()
        {
            return &_factories;
        }
        
    protected:
        std::map< String, CFactoryDatabase * > _factories; //!< Factories managing the created instances.
        std::map< String, String > _objectFactories; //!< Manage the association between an object type and its factory.
    };
}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_FACTORY_MANAGER_H___
