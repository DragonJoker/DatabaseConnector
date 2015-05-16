/************************************************************************//**
 * @file DatabaseFieldInfos.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 * 
 *
 * @brief CDatabaseFieldInfos class declaration.
 *
 * @details Describes database field information.
 *
 ***************************************************************************/

#ifndef ___DATABASE_FIELD_INFOS_H___
#define ___DATABASE_FIELD_INFOS_H___

#include "DatabasePrerequisites.h"

#include "EFieldType.h"

BEGIN_NAMESPACE_DATABASE
{

    /** Describes database field information.
    */
    class DatabaseExport CDatabaseFieldInfos
    {
    
    public:
        /** Constructor.
        @param[in] connection
            Database connection.
        @param[in] name
            Field name.
        @param[in] type
            Field type
        @param[in] limits
            Field limits
         */
        CDatabaseFieldInfos( DatabaseConnectionPtr connection, const String & name, EFieldType type = EFieldType_NULL, uint32_t limits = 0 );
        
        /** Constructor.
        @param[in] connection
            Database connection.
        @param[in] name
            Field name.
        @param[in] type
            Field type as string
        @param[in] length
            Field length
        */
        CDatabaseFieldInfos( DatabaseConnectionPtr connection, const String & name, const String & type, int length = -1 );
        
        /** Copy constructor.
        @param[in] fieldInfos
            Field information to copy.
        */
        CDatabaseFieldInfos( const CDatabaseFieldInfos & fieldInfos );
        
        /** Destructor.
        */
        virtual ~CDatabaseFieldInfos();
        
        /** Get field type.
        @return
            Field type.
        */
        EFieldType GetType() const;
        
        /** Get field name.
        @return
            Field name.
        */
        const String & GetName() const;
        
        /** Get field limits.
        @return
            Field limits.
        */
        const uint32_t & GetLimits() const;
        
        /** Get connection.
        @return
            Database connexion.
        */
        DatabaseConnectionPtr GetConnection() const;
        
    protected:
        String            _name;          ///< Field name.
        EFieldType              _type;          ///< Field type.
        uint32_t                _limits;        ///< Limits (char, varchar, etc.).
        DatabaseConnectionPtr   _connection;    ///< Database connection.
        
    }; // class CDatabaseFieldInfos
    
}
END_NAMESPACE_DATABASE

#endif //__DATABASE_FIELD_INFOS_H___
