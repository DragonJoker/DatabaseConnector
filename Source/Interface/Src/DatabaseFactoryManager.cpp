/************************************************************************//**
* @file FactoryObjectManager.cpp
* @author spastor
* @version 1.0
* @date 2/13/2013 4:27:50 PM
*
*
* @brief This manager manages factory objects.
*
* @details This class not only manages the object themselves, it also manages
*          the factory classes wich allow the creation of specific objects.
*
***************************************************************************/

#include "DatabasePch.h"

#include "DatabaseFactory.h"
#include "DatabaseFactoryManager.h"

BEGIN_NAMESPACE_DATABASE
{
	static const String ERROR_DB_FACTORY_TYPE_NOT_FOUND = STR( "Factory type %1% not found in registered factories" );
	static const String ERROR_DB_OBJECT_TYPE_NOT_FOUND = STR( "Object type %1% not found in registered objects" );

	CFactoryManager::CFactoryManager()
	{
	}

	CFactoryManager::~CFactoryManager()
	{
	}

	void CFactoryManager::AddFactory( CFactoryDatabase * factory )
	{
		_factories.insert( std::make_pair( factory->GetType(), factory ) );

		//!@remarks Create the association between an object type and its factory.
		auto && objTypes = factory->GetObjectTypes();

		for ( auto && name : objTypes )
		{
			_objectFactories.insert( std::make_pair( name, factory->GetType() ) );
		}
	}

	void CFactoryManager::RemoveFactory( CFactoryDatabase * factory )
	{
		auto && itFactory = _factories.find( factory->GetType() );

		if ( itFactory != _factories.end() )
		{
			_factories.erase( itFactory );

			//!@remarks Remove the association between an object type and its factory.
			auto && objTypes = factory->GetObjectTypes();

			for ( auto && name : objTypes )
			{
				auto && itObject = _objectFactories.find( name );

				if ( itObject != _objectFactories.end() )
				{
					_objectFactories.erase( itObject );
				}
			}
		}
	}

	CDatabase * CFactoryManager::CreateInstance( const String & factoryType, const String & objectType )
	{
		CDatabase * object = NULL;

		//!@remarks Find factory object.
		auto && itFactory = _factories.find( factoryType );

		if ( itFactory != _factories.end() )
		{
			CFactoryDatabase * factory = itFactory->second;

			if ( factory )
			{
				//!@remarks Create a new object instance.
				object = factory->CreateInstance( objectType );
			}
			else
			{
				Format fmt( ERROR_DB_FACTORY_TYPE_NOT_FOUND );
				fmt % factoryType;
				DB_EXCEPT( EDatabaseExceptionCodes_NullPointer, fmt.str() );
			}
		}
		else
		{
			Format fmt( ERROR_DB_FACTORY_TYPE_NOT_FOUND );
			fmt % factoryType;
			DB_EXCEPT( EDatabaseExceptionCodes_ItemNotFound, fmt.str() );
		}

		return object;
	}

	CDatabase * CFactoryManager::CreateInstance( const String & objectType )
	{
		//!@remarks Find factory type.
		auto && itObject = _objectFactories.find( objectType );

		if ( itObject != _objectFactories.end() )
		{
			//!@remarks Create a new object instance.
			return CreateInstance( itObject->second, objectType );
		}
		else
		{
			Format fmt( ERROR_DB_OBJECT_TYPE_NOT_FOUND );
			fmt % objectType;
			DB_EXCEPT( EDatabaseExceptionCodes_ItemNotFound, fmt.str() );
		}

		return NULL;
	}
}
END_NAMESPACE_DATABASE
