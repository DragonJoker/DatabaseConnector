/************************************************************************//**
 * @file DatabaseParameterMySql.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 * 
 *
 * @brief CDatabaseParameterMySql class declaration.
 *
 * @details Describes the base class for a statement or query parameter for MYSQL database.
 *
 ***************************************************************************/

#ifndef ___DATABASE_PARAMETER_MYSQL_H___
#define ___DATABASE_PARAMETER_MYSQL_H___

#include "DatabaseMySqlPrerequisites.h"

#include <EFieldType.h>
#include <EParameterType.h>

BEGIN_NAMESPACE_DATABASE_MYSQL
{
    /** Describes a statement parameter for MYSQL database.
    */
    class CDatabaseParameterMySql
    {
    public:
        //! The MySQL data types associated to Database::EFieldType
        static int MySQLDataTypes[EFieldType_COUNT];
        //! The MySQL parameter types associated to Database::EParameterType
        static int MySqlParameterTypes[EParameterType_COUNT];
        
    public:
        /** Constructor.
        @param[in] fieldType
            Field type.
        @param[in] parameterType
            Parameter type.
         */
        DatabaseMySqlExport CDatabaseParameterMySql( EFieldType fieldType, EParameterType parameterType );
        
        /** Destructor.
         */
        DatabaseMySqlExport virtual ~CDatabaseParameterMySql();
        
        /** Retrieve the data type
        @return
            The type
        */
        DatabaseMySqlExport const int & GetDataType()const;
        
        /** Retrieve the parameter type
        @return
            The type
        */
        DatabaseMySqlExport const int & GetParameterType()const;
        
    protected:
        //! The MySQL data type
        int _dataType;
        //! The MySQL parameter type
        int _parameterType;
    };
}
END_NAMESPACE_DATABASE_MYSQL

#endif // ___DATABASE_PARAMETER_MYSQL_H___
