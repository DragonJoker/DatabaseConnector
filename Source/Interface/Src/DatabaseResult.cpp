/************************************************************************//**
 * @file DatabaseResult.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/24/2014 8:37:01 AM
 * 
 *
 * @brief CDatabaseResult class definition.
 *
 * @details Describes the result of a SELECT request.
 *
 ***************************************************************************/

#include "DatabasePch.h"

#include "DatabaseResult.h"

#include "Database.h"
#include "DatabaseConnection.h"
#include "DatabaseFieldInfos.h"
#include "DatabaseException.h"
#include "DatabaseStringUtils.h"
#include "DatabaseLogger.h"

BEGIN_NAMESPACE_DATABASE
{
    static const String DATABASE_NO_FIELD_ERROR = STR( "No field at index: " );
    
    CDatabaseResult::CDatabaseResult( DatabaseConnectionPtr connection )
        : _connection( connection )
        , _rowCount( 0 )
    {
        // Empty
    }
    
    CDatabaseResult::CDatabaseResult( DatabaseConnectionPtr connection, const DatabaseFieldInfosPtrArray & arrayFieldInfos )
        : _connection( connection )
        , _rowCount( 0 )
    {
        for ( DatabaseFieldInfosPtrArray::const_iterator it = arrayFieldInfos.begin() ; it != arrayFieldInfos.end() ; ++it )
        {
            _arrayFieldInfos.push_back( std::make_shared< CDatabaseFieldInfos >( *( *it ) ) );
        }
    }
    
    CDatabaseResult::~CDatabaseResult()
    {
        _listRows.clear();
        _arrayFieldInfos.clear();
    }
    
    void CDatabaseResult::AddField( const String & name, EFieldType type, uint32_t limits )
    {
        _arrayFieldInfos.push_back( std::make_shared< CDatabaseFieldInfos >( _connection, name, type, limits ) );
    }
    
    void CDatabaseResult::AddField( const String & name, const String & type, int length )
    {
        _arrayFieldInfos.push_back( std::make_shared< CDatabaseFieldInfos >( _connection, name, type, length ) );
    }
    
    void CDatabaseResult::AddRow( DatabaseRowPtr row )
    {
        _listRows.push_back( row );
        _rowCount++;
    }
    
    uint32_t CDatabaseResult::GetRowCount()
    {
        return _rowCount;
    }
    
    DatabaseRowPtr CDatabaseResult::GetNextRow()
    {
        DatabaseRowPtr pReturn;
        
        if ( _iterator != _listRows.end() )
        {
            pReturn = ( *_iterator );
            ++_iterator;
        }
        
        return pReturn;
    }
    
    DatabaseRowPtr CDatabaseResult::GetFirstRow()
    {
        _rowCount = 0;
        _iterator = _listRows.begin();
        return GetNextRow();
    }
    
    uint32_t CDatabaseResult::GetFieldCount() const
    {
        return ( uint32_t )_arrayFieldInfos.size();
    }
    
    DatabaseFieldInfosPtr CDatabaseResult::GetFieldInfos( uint32_t index ) const
    {
        DatabaseFieldInfosPtr pReturn;
        
        if ( index < _arrayFieldInfos.size() )
        {
            pReturn = _arrayFieldInfos[index];
        }
        else
        {
            StringStream message;
            message << DATABASE_NO_FIELD_ERROR << CStrUtils::ToString( index );
            CLogger::LogError( message );
            DB_EXCEPT( EDatabaseExceptionCodes_FieldError, message.str() );
        }
        
        return pReturn;
    }
    
}
END_NAMESPACE_DATABASE
