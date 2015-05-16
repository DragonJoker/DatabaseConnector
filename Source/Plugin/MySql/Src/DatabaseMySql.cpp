/************************************************************************//**
 * @file DatabaseMySql.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 * 
 *
 * @brief CDatabaseMySql class declaration.
 *
 * @details Describes an MYSQL database.
 *
 ***************************************************************************/

#include "DatabaseMySqlPch.h"

#include "DatabaseMySql.h"
#include "DatabaseConnectionMySql.h"

#include <cppconn/driver.h>

BEGIN_NAMESPACE_DATABASE_MYSQL
{
    CDatabaseMySql::CDatabaseMySql()
        : CDatabase()
        , _driver( get_driver_instance() )
    {
    }
    
    CDatabaseMySql::~CDatabaseMySql()
    {
    }
    
    const String & CDatabaseMySql::GetType()const
    {
        return DATABASE_MYSQL_TYPE;
    }
    
    CDatabase * CDatabaseMySql::Create()
    {
        return new CDatabaseMySql;
    }
    
    DatabaseConnectionPtr CDatabaseMySql::DoCreateConnection( String & connectionString ) const
    {
        DatabaseConnectionPtr pReturn;
        
        if ( _driver )
        {
            // std::make_shared limited to 5 parameters with VS2012
            pReturn.reset( new CDatabaseConnectionMySql( _driver, _server, _database, _userName, _password, connectionString ) );
        }
        
        return pReturn;
    }
}
END_NAMESPACE_DATABASE_MYSQL
