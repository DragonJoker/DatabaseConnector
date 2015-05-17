/************************************************************************//**
 * @file FactoryDatabase.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 4/8/2013 10:11:31 AM
 *
 *
 * @brief Database factory class.
 *
 * @details CFactoryDatabase is used to manage CDatabase creation.
 *
 ***************************************************************************/

#include "DatabasePch.h"

#include "DatabaseFactory.h"

BEGIN_NAMESPACE_DATABASE
{
	CFactoryDatabase::CFactoryDatabase()
	{
		// Empty
	}

	CFactoryDatabase::~CFactoryDatabase()
	{
		// Empty
	}

	void CFactoryDatabase::Initialize()
	{
		RegisterServices();
		RegisterObjectTypes();
	}

	void CFactoryDatabase::RegisterCreatorFunction( const String & idKey, CreatorFunction classCreator )
	{
		iterator it = GetMapFactory()->find( idKey );

		if ( it != GetMapFactory()->end() )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_DuplicateItem, ERROR_DUPLICATE_FUNCTION );
		}

		GetMapFactory()->insert( std::pair< String, CreatorFunction >( idKey, classCreator ) );
	}

	CDatabase * CFactoryDatabase::CreateInstance( const String & idKey )
	{
		iterator it = GetMapFactory()->find( idKey );

		if ( it != GetMapFactory()->end() )
		{
			if ( it->second )
			{
				return it->second();
			}
		}

		return NULL;
	}
}
END_NAMESPACE_DATABASE
