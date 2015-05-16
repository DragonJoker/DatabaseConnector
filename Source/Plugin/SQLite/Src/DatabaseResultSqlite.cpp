/************************************************************************//**
 * @file DatabaseResultSqlite.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 * 
 *
 * @brief CDatabaseResultSqlite class definition.
 *
 * @details Describes a result for SQLITE database.
 *
 ***************************************************************************/

#include "DatabaseSqlitePch.h"

#include "DatabaseResultSqlite.h"
#include "ExceptionDatabaseSqlite.h"
#include "DatabaseSqlite.h"
#include "DatabaseStatementSqlite.h"

#include <DatabaseField.h>
#include <DatabaseRow.h>

#include <DatabaseStringUtils.h>

#include <cppconn/exception.h>

BEGIN_NAMESPACE_DATABASE_SQLITE
{
    static const String   SQLITE_DRIVER_ERROR = STR( "SQLITE Driver error : " );
    static const String   SQLITE_UNKNOWN_ERROR = STR( "Unknown error encountered while executing query" );
    static const String   SQLITE_INCONSISTENCY_ERROR = STR( "Number of columns is less than retrieved data." );
    
    CDatabaseResultSqlite::CDatabaseResultSqlite( DatabaseConnectionPtr connection, std::shared_ptr< sql::Statement > statement )
        : CDatabaseResult( connection )
        , _statement( statement )
    {
        // Empty
    }
    
    CDatabaseResultSqlite::~CDatabaseResultSqlite()
    {
        ///@remarks Consume the result set
        if ( _result )
        {
            while ( _result->next() );
            _result.reset();
        }
        
        ///@remarks Flush the statement
        try
        {
            bool hasResult = false;
            
            while ( hasResult = _statement->getMoreResults() )
            {
                _result.reset( _statement->getResultSet() );
                CLogger::LogDebug( "Success : Result set retrieval" );
            }
        }
        catch ( sql::SQLException & e )
        {
            StringStream stream;
            stream << "Failure : " << "Get more results" << std::endl;
            stream << "    SQLite Error code : " << e.getErrorCode() << std::endl;
            stream << "    SQL State : " << e.getSQLState().c_str() << std::endl;
            stream << "    Why : " << e.what() << std::endl;
            CLogger::LogDebug( stream.str() );
        }
        
        _statement.reset();
    }
    
    EFieldType GetFieldTypeFromSqlType( int sqlType )
    {
        static EFieldType types[] =
        {
            EFieldType_NULL,            //!< UNKNOWN = 0
            EFieldType_BOOL,            //!< BIT
            EFieldType_SMALL_INTEGER,   //!< TINYINT
            EFieldType_SMALL_INTEGER,   //!< SMALLINT
            EFieldType_INTEGER,         //!< MEDIUMINT
            EFieldType_INTEGER,         //!< INTEGER
            EFieldType_LONG_INTEGER,    //!< BIGINT
            EFieldType_FLOAT,           //!< REAL
            EFieldType_DOUBLE,          //!< DOUBLE
            EFieldType_DOUBLE,          //!< DECIMAL
            EFieldType_DOUBLE,          //!< NUMERIC
            EFieldType_VARCHAR,         //!< CHAR
            EFieldType_BINARY,          //!< BINARY
            EFieldType_VARCHAR,         //!< VARCHAR
            EFieldType_VARBINARY,       //!< VARBINARY
            EFieldType_TEXT,            //!< LONGVARCHAR
            EFieldType_LONG_VARBINARY,  //!< LONGVARBINARY
            EFieldType_DATETIME,        //!< TIMESTAMP
            EFieldType_DATE,            //!< DATE
            EFieldType_TIME,            //!< TIME
            EFieldType_INTEGER,         //!< YEAR
            EFieldType_NULL,            //!< GEOMETRY
            EFieldType_NULL,            //!< ENUM
            EFieldType_NULL,            //!< SET
            EFieldType_NULL,            //!< SQLNULL
        };

        return types[sqlType];
    }
    
    void CDatabaseResultSqlite::Initialize( std::shared_ptr< sql::ResultSet > result )
    {
        _result = result;
        sql::ResultSetMetaData * data = _result->getMetaData();
        _rowCount = uint32_t( result->rowsCount() );
        unsigned int count = data->getColumnCount();
        _row = std::make_shared< CDatabaseRow >( _connection );
        DatabaseFieldPtr field;
        
        for ( unsigned int i = 1; i <= count; ++i )
        {
            AddField( CStrUtils::ToString( data->getColumnName( i ).asStdString() ), GetFieldTypeFromSqlType( data->getColumnType( i ) ), data->getColumnDisplaySize( i ) );
            DatabaseFieldInfosPtr infos = GetFieldInfos( i - 1 );
            field = std::make_shared< CDatabaseField >( infos, true, STR( "" ) );
            _row->AddField( field );
        }
    }
    
    DatabaseRowPtr CDatabaseResultSqlite::GetFirstRow()
    {
        DatabaseRowPtr result;
        
        if ( _result->first() )
        {
            DoGetRow();
            result = _row;
        }
        
        return result;
    }
    
    DatabaseRowPtr CDatabaseResultSqlite::GetNextRow()
    {
        DatabaseRowPtr result;
        
        if ( _result->next() )
        {
            DoGetRow();
            result = _row;
        }
        
        return result;
    }
    
    void CDatabaseResultSqlite::DoGetRow()
    {
        bool null = true;
        sql::SQLString fieldValue;
        DatabaseFieldPtr field;
        
        try
        {
            for ( uint32_t i = 1; i <= _arrayFieldInfos.size() ; ++i )
            {
                sql::SQLString fieldValue;
                SQLiteTry( null = _result->isNull( i ), STR( "Is parameter null retrieval" ) );
                field = _row->GetField( i - 1 );
                
                if ( !null )
                {
                    switch ( field->GetType() )
                    {
                        case EFieldType_BOOL:
                            field->SetValueFast( _result->getBoolean( i ) );
                            break;
                        case EFieldType_SMALL_INTEGER:
                            field->SetValueFast( short( _result->getInt( i ) ) );
                            break;
                        case EFieldType_INTEGER:
                            field->SetValueFast( _result->getInt( i ) );
                            break;
                        case EFieldType_LONG_INTEGER:
                            field->SetValueFast( _result->getInt64( i ) );
                            break;
                        case EFieldType_FLOAT:
                            field->SetValueFast( float( _result->getDouble( i ) ) );
                            break;
                        case EFieldType_DOUBLE:
                            field->SetValueFast( double( _result->getDouble( i ) ) );
                            break;
                        case EFieldType_VARCHAR:
                            field->SetValueFast( std::string( _result->getString( i ) ).c_str() );
                            break;
                        case EFieldType_TEXT:
                            field->SetValueFast( std::string( _result->getString( i ) ) );
                            break;
                        case EFieldType_NVARCHAR:
                            field->SetValueFast( CStrUtils::ToWStr( _result->getString( i ) ).c_str() );
                            break;
                        case EFieldType_NTEXT:
                            field->SetValueFast( CStrUtils::ToWStr( _result->getString( i ) ) );
                            break;
                        case EFieldType_DATE:
                            field->SetValue( _result->getString( i ).asStdString() );
                            break;
                        case EFieldType_DATETIME:
                            field->SetValue( _result->getString( i ).asStdString() );
                            break;
                        case EFieldType_TIME:
                            field->SetValue( _result->getString( i ).asStdString() );
                            break;
                        case EFieldType_BINARY:
                        case EFieldType_VARBINARY:
                        case EFieldType_LONG_VARBINARY:
                        {
                            std::istream * blob = _result->getBlob( i );
                            std::vector< uint8_t > in;
                            std::copy(
                                std::istream_iterator< uint8_t >( *blob ),
                                std::istream_iterator< uint8_t >(),
                                std::back_inserter( in ) );
                            field->SetValueFast( in );
                            delete blob;
                        }
                        break;
                    }
                }
                else
                {
                    field->SetNull();
                }
            }
        }
        catch ( const CExceptionDatabase & e )
        {
            StringStream message;
            message << SQLITE_DRIVER_ERROR << STR( " - " )
                    << e.what();
            CLogger::LogError( message );
            throw CExceptionDatabaseSqlite( EDatabaseSqliteExceptionCodes_GenericError, message.str(), __FUNCTION__, __FILE__, __LINE__ );
        }
        catch ( const std::exception & e )
        {
            StringStream message;
            message << SQLITE_DRIVER_ERROR << STR( " - " )
                    << e.what();
            CLogger::LogError( message );
            throw CExceptionDatabaseSqlite( EDatabaseSqliteExceptionCodes_GenericError, message.str(), __FUNCTION__, __FILE__, __LINE__ );
        }
        catch ( ... )
        {
            StringStream message;
            message << SQLITE_DRIVER_ERROR << STR( " - " )
                    << SQLITE_UNKNOWN_ERROR;
            CLogger::LogError( message );
            throw CExceptionDatabaseSqlite( EDatabaseSqliteExceptionCodes_UnknownError, message.str(), __FUNCTION__, __FILE__, __LINE__ );
        }
    }
    
}
END_NAMESPACE_DATABASE_SQLITE
