/************************************************************************//**
 * @file FactoryDatabaseMySql.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 4/08/2014 1:21:00 PM
 * 
 *
 * @brief MYSQL database factory class.
 *
 * @details CObject factory used to manage MYSQL database creation.
 *
 ***************************************************************************/

#include "DatabaseMySqlPch.h"

#include "FactoryDatabaseMySql.h"
#include "DatabaseMySql.h"

BEGIN_NAMESPACE_DATABASE_MYSQL
{
    CFactoryDatabaseMySql::CFactoryDatabaseMySql()
        : CFactoryDatabase()
    {
        // Empty
    }
    
    CFactoryDatabaseMySql::~CFactoryDatabaseMySql()
    {
        // Empty
    }
    
    void CFactoryDatabaseMySql::RegisterServices()
    {
        ///@remarks Register function for creation of a DatabaseMySql object.
        RegisterCreatorFunction( DATABASE_MYSQL_TYPE, CDatabaseMySql::Create );
    }
    
    void CFactoryDatabaseMySql::RegisterObjectTypes()
    {
        _objTypes.push_back( DATABASE_MYSQL_TYPE );
    }
    
}
END_NAMESPACE_DATABASE_MYSQL
