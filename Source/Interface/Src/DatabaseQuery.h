/************************************************************************//**
 * @file DatabaseQuery.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 * 
 *
 * @brief CDatabaseQuery class declaration.
 *
 * @details Describes a database query.
 *
 ***************************************************************************/

#ifndef ___DATABASE_QUERY_H___
#define ___DATABASE_QUERY_H___

#include "DatabasePrerequisites.h"

#include "Database.h"
#include "DatabaseParameter.h"
#include "DatabaseException.h"

#include "EErrorType.h"
#include "EFieldType.h"
#include "EParameterType.h"

BEGIN_NAMESPACE_DATABASE
{
    /** Describes a database query.
    */
    class DatabaseExport CDatabaseQuery
    {
    
    public:
        /** Constructor.
        @param[in] connection
            Database connection.
        @param[in] query
            Request text.
        */
        CDatabaseQuery( DatabaseConnectionPtr connection, const String & query );
        
        /** Destructor.
        */
        virtual ~CDatabaseQuery();
        
        /** Initialize query.
        @remarks
            The query *MUST* be initialized, *AFTER* all parameters have been created.
        @return
            Error code.
        */
        virtual EErrorType Initialize();
        
        /** Execute a query that has no result set.
        @param[out] result
            Error code.
        @return
            Results.
        */
        virtual bool ExecuteUpdate( EErrorType * result = NULL );
        
        /** Execute query that has a result set.
        @param[out] result
            Error code.
        @return
            Results.
        */
        virtual DatabaseResultPtr ExecuteSelect( EErrorType * result = NULL );
        
        /** Clean query.
        */
        virtual void Cleanup();
        
        /** Create a query parameter.
        @param[in] name
            Parameter name.
        @param[in] fieldType
            Field type.
        @param[in] parameterType
            Parameter type.
        @return
            Created query parameter.
        */
        virtual DatabaseParameterPtr CreateParameter( const String & name, EFieldType fieldType, EParameterType parameterType = EParameterType_IN );
        
        /** Create a query parameter for variable-sized parameter (with limits)
        @param[in] name
            Parameter name.
        @param[in] fieldType
            Field type.
        @param[in] limits
            Filed limits.
        @param[in] parameterType
            Parameter type.
        @return
            Created query parameter.
        */
        virtual DatabaseParameterPtr CreateParameter( const String & name, EFieldType fieldType, uint32_t limits, EParameterType parameterType = EParameterType_IN );
        
        /** Set parameter value to NULL.
        @param[in] index
            Parameter index.
        */
        void SetParameterNull( uint32_t index );
        
        /** Set parameter value to NULL.
        @param[in] name
            Parameter name.
        */
        void SetParameterNull( const String & name );
        
        /** Set parameter value.
        @param[in] index
            Parameter index.
        @param[in] value
            Parameter value.
        */
        template <typename T> void SetParameterValue( uint32_t index, const T & value );
        
        /** Set parameter value.
        @param[in] name
            Parameter name.
        @param[in] value
            Parameter value.
        */
        template <typename T> void SetParameterValue( const String & name, const T & value );
        
        /** Get parameter value.
        @param[in] index
            Parameter index.
        @return
            Parameter value.
        */
        template <typename T> T const & GetParameterValue( uint32_t index );
        
        /** Get parameter value.
        @param[in] name
            Parameter name.
        @return
            Parameter value.
        */
        template <typename T> T const & GetParameterValue( const String & name );
        
        /** Get parameter value.
        @param[in] index
            Parameter index.
        @param[out] value
            Parameter value.
        */
        template <typename T> void GetParameterValue( uint32_t index, T & value );
        
        /** Get parameter value.
        @param[in] name
            Parameter name.
        @param[out] value
            Parameter value.
        */
        template <typename T> void GetParameterValue( const String & name, T & value );
        
    protected:
    
        /** Add parameter to query.
        @param[in] parameter
            Parameter to add.
        @return
            true if addition succeeds, false otherwise.
        */
        bool DoAddParameter( DatabaseParameterPtr parameter );
        
        /** Pre-execution action
        @remarks
            Computes the final query from parameters values
        @param[out] outParams
            Receives the out parameters
        @param[out] result
            Receives the error code
        @return
            The full query
        */
        String DoPreExecute( DatabaseParameterPtrArray & outParams, EErrorType * result );
        
        /** Execute query.
        @param[out] result
            Error code.
        @return
            Results.
        */
        DatabaseResultPtr DoExecute( EErrorType * result = NULL );
        
        /** Pre-execution action
        @remarks
            Computes the final query from parameters values
        @param[in] outParams
            The output parameters
        @param[out] result
            Receives the error code
        */
        void DoPostExecute( DatabaseParameterPtrArray const & outParams, EErrorType * result );
        
    protected:
        /// Array of parameters (addition order).
        DatabaseParameterPtrArray _arrayParams;
        /// Map of parameters by pointers.
        std::map< void *, DatabaseParameterPtr > _mapParamsByPointer;
        /// Request text.
        String _query;
        /// Tokenized string (delimiter is "?").
        StringArray _arrayQueries;
        /// Database connection.
        DatabaseConnectionPtr _connection;
        /// Number of parameters (i.e. number of "?").
        uint32_t _paramsCount;
        
        /** Inform parent query from the value changes
        */
        struct SValueUpdater
            : public CDatabaseParameter::SValueUpdater
        {
            /** Constructor
            @param query
                The parent query
            */
            DatabaseExport SValueUpdater( CDatabaseQuery * query );
            
            ///@copydoc CDatabaseParameter::SValueUpdater
            DatabaseExport virtual void Update( DatabaseParameterPtr value );
            
        private:
            //! The parent query
            CDatabaseQuery * _query;
        };
    };
}
END_NAMESPACE_DATABASE

#include "DatabaseQuery.inl"

#endif // ___DATABASE_QUERY_H___
