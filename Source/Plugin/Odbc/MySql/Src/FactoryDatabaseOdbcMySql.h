/************************************************************************//**
 * @file FactoryDatabaseOdbcMySql.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 4/08/2014 1:20:50 PM
 * 
 *
 * @brief ODBC database factory class.
 *
 * @details CObject factory used to manage ODBC database creation.
 *
 ***************************************************************************/

#ifndef ___FACTORY_DATABASE_ODBC_MYSQL_H___
#define ___FACTORY_DATABASE_ODBC_MYSQL_H___

#include "DatabaseOdbcMySqlPrerequisites.h"

#include <DatabaseFactory.h>

BEGIN_NAMESPACE_DATABASE_ODBC_MYSQL
{
    /** ODBC database factory class.
        CObject factory used to manage ODBC database creation.
    */
    class CFactoryDatabaseOdbcMySql
        : public CFactoryDatabase
    {
    public:
        /** Default constructor.
        */
        DatabaseOdbcMySqlExport CFactoryDatabaseOdbcMySql();
        
        /** Destructor.
        */
        DatabaseOdbcMySqlExport virtual ~CFactoryDatabaseOdbcMySql();
        
        /** Get the type of the factory.
        @returns
            String describing the factory type.
        */
        const String GetType() const;
        
    protected:
        /** Register creation functions to create new ODBC database objects.
        */
        DatabaseOdbcMySqlExport void RegisterServices();
        
        /** Registred the ODBC database object types that the factory can create.
        */
        DatabaseOdbcMySqlExport void RegisterObjectTypes();
    };
    
    //
    // Inlines
    //
    inline const String CFactoryDatabaseOdbcMySql::GetType() const
    {
        return MySql::FACTORY_DATABASE_ODBC_MYSQL;
    }
    
}
END_NAMESPACE_DATABASE_ODBC_MYSQL

#endif // ___FACTORY_DATABASE_ODBC_H___
