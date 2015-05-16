/************************************************************************//**
 * @file DatabaseStatement.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 * 
 *
 * @brief CDatabaseStatement class definition.
 *
 * @details Describes a database statement.
 *
 ***************************************************************************/

#include "DatabasePch.h"

#include "DatabaseStatement.h"

#include "Database.h"
#include "DatabaseParameter.h"
#include "DatabaseException.h"

#include "DatabaseStringUtils.h"
#include "DatabaseLogger.h"

BEGIN_NAMESPACE_DATABASE
{
    static const String DATABASE_STATEMENT_ALREADY_ADDED_PARAMETER = STR( "Parameter with name [%1%] already exists." );
    static const String DATABASE_STATEMENT_NULL_PARAMETER = STR( "Trying to add a null parameter." );
    
    CDatabaseStatement::SValueUpdater::SValueUpdater( CDatabaseStatement * stmt )
        : _stmt( stmt )
    {
    }
    
    void CDatabaseStatement::SValueUpdater::Update( DatabaseParameterPtr value )
    {
        _stmt->_mapParamsByPointer[value->GetPtrValue()] = value;
    }
    
    CDatabaseStatement::CDatabaseStatement( DatabaseConnectionPtr connection, const String & query )
        :   _connection( connection )
        ,   _query( query )
    {
        // Empty
    }
    
    CDatabaseStatement::~CDatabaseStatement()
    {
        // Empty
    }
    
    void CDatabaseStatement::SetParameterNull( uint32_t index )
    {
        try
        {
            _arrayParams[index]->SetNull();
        }
        catch ( ... )
        {
            StringStream message;
            message << DATABASE_STATEMENT_INDEX_ERROR << index;
            CLogger::LogError( message );
            DB_EXCEPT( EDatabaseExceptionCodes_StatementError, message.str() );
        }
    }
    
    void CDatabaseStatement::SetParameterNull( const String & name )
    {
        DatabaseParameterPtrArray::iterator it = std::find_if( _arrayParams.begin(), _arrayParams.end(), StatementParameterFindCondition( name ) );
        
        if ( it != _arrayParams.end() )
        {
            ( *it )->SetNull();
        }
        else
        {
            StringStream message;
            message << DATABASE_STATEMENT_NAME_ERROR << name;
            CLogger::LogError( message );
            DB_EXCEPT( EDatabaseExceptionCodes_StatementError, message.str() );
        }
    }
    
    void CDatabaseStatement::SetParameterValue( uint32_t index, DatabaseParameterPtr parameter )
    {
        try
        {
            _arrayParams[index]->SetValue( parameter );
        }
        catch ( ... )
        {
            StringStream message;
            message << DATABASE_STATEMENT_INDEX_ERROR << index;
            CLogger::LogError( message );
            DB_EXCEPT( EDatabaseExceptionCodes_StatementError, message.str() );
        }
    }
    
    void CDatabaseStatement::SetParameterValue( const String & name, DatabaseParameterPtr parameter )
    {
        DatabaseParameterPtrArray::iterator it = std::find_if( _arrayParams.begin(), _arrayParams.end(), StatementParameterFindCondition( name ) );
        
        if ( it != _arrayParams.end() )
        {
            ( *it )->SetValue( parameter );
        }
        else
        {
            StringStream message;
            message << DATABASE_STATEMENT_NAME_ERROR << name;
            CLogger::LogError( message );
            DB_EXCEPT( EDatabaseExceptionCodes_StatementError, message.str() );
        }
    }
    
    EFieldType CDatabaseStatement::GetParameterType( uint32_t index )
    {
        try
        {
            return _arrayParams[index]->GetType();
        }
        catch ( ... )
        {
            StringStream message;
            message << DATABASE_STATEMENT_INDEX_ERROR << index;
            CLogger::LogError( message );
            DB_EXCEPT( EDatabaseExceptionCodes_StatementError, message.str() );
        }
    }
    
    bool CDatabaseStatement::DoAddParameter( DatabaseParameterPtr parameter )
    {
        bool bReturn = false;
        
        if ( parameter )
        {
            if ( std::find_if( _arrayParams.begin(), _arrayParams.end(), StatementParameterFindCondition( parameter->GetName() ) ) == _arrayParams.end() )
            {
                _arrayParams.push_back( parameter );
                bReturn = true;
            }
            else
            {
                Format format( DATABASE_STATEMENT_ALREADY_ADDED_PARAMETER );
                format % parameter->GetName();
                CLogger::LogError( format.str() );
            }
        }
        else
        {
            CLogger::LogError( DATABASE_STATEMENT_NULL_PARAMETER );
        }
        
        return bReturn;
    }
}
END_NAMESPACE_DATABASE
