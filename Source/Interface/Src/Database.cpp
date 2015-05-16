/************************************************************************//**
 * @file Database/Interface/Src/Database.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 * 
 *
 * @brief CDatabase class definition.
 *
 * @details Describes a database.
 *
 ***************************************************************************/

#include "DatabasePch.h"

#include "Database.h"

#include "DatabaseConnection.h"
#include "DatabaseDate.h"
#include "DatabaseDateTime.h"
#include "DatabaseTime.h"

BEGIN_NAMESPACE_DATABASE
{
    const String LOGGER_NAME_CDATABASE = STR( "Areva.ARIA.Native.Database.CDatabase" );
    
    CDatabase::CDatabase()
    {
        // Empty
    }
    
    CDatabase::~CDatabase()
    {
        // Empty
    }
    
    void CDatabase::Connect( const String & server, const String & database, const String & userName, const String & password, String & connectionString )
    {
        std::unique_lock< std::recursive_mutex > lock( _mutex );
        _server = server;
        _database = database;
        _userName = userName;
        _password = password;
        _connection = DoCreateConnection( connectionString );
    }
    
    void CDatabase::Disconnect()
    {
        if ( _connection )
        {
            _connection.reset();
        }
    }
    
    void CDatabase::Initialize( const String & server, const String & database, const String & userName, const String & password )
    {
        std::unique_lock< std::recursive_mutex > lock( _mutex );
        _server = server;
        _database = database;
        _userName = userName;
        _password = password;
    }
    
    void CDatabase::CreateConnection( String & connectionString )
    {
        std::unique_lock< std::recursive_mutex > lock( _mutex );
        std::thread::id dwThreadId = std::this_thread::get_id();
        DatabaseConnectionPtrIdMap::const_iterator it = _mapConnections.find( dwThreadId );
        
        if ( it == _mapConnections.end() )
        {
            _mapConnections.insert( std::make_pair( dwThreadId, DoCreateConnection( connectionString ) ) );
        }
    }
    
    DatabaseConnectionPtr CDatabase::RetrieveConnection()
    {
        DatabaseConnectionPtr pReturn = DoGetConnection();
        
        if ( ! pReturn )
        {
            String strResult;
            CreateConnection( strResult );
            pReturn = DoGetConnection();
        }
        
        return pReturn;
    }
    
    void CDatabase::RemoveConnection()
    {
        std::unique_lock< std::recursive_mutex > lock( _mutex );
        std::thread::id dwThreadId = std::this_thread::get_id();
        DatabaseConnectionPtrIdMap::iterator it = _mapConnections.find( dwThreadId );
        
        if ( it != _mapConnections.end() )
        {
            _mapConnections.erase( it );
        }
    }
    
    EErrorType CDatabase::BeginTransaction( const String & name )
    {
        EErrorType eResult = EErrorType_NONE;
        DatabaseConnectionPtr pConnexion = DoGetConnection();
        
        if ( pConnexion )
        {
            eResult = pConnexion->BeginTransaction( name );
        }
        
        return eResult;
    }
    
    EErrorType CDatabase::Commit( const String & name )
    {
        EErrorType eResult = EErrorType_NONE;
        DatabaseConnectionPtr pConnexion = DoGetConnection();
        
        if ( pConnexion )
        {
            eResult = pConnexion->Commit( name );
        }
        
        return eResult;
    }
    
    EErrorType CDatabase::RollBack( const String & name )
    {
        EErrorType eResult = EErrorType_NONE;
        DatabaseConnectionPtr pConnexion = DoGetConnection();
        
        if ( pConnexion )
        {
            eResult = pConnexion->RollBack( name );
        }
        
        return eResult;
    }
    
    DatabaseStatementPtr CDatabase::CreateStatement( const String & query, EErrorType * result )
    {
        DatabaseStatementPtr pReturn;
        DatabaseConnectionPtr pConnexion = DoGetConnection();
        
        if ( pConnexion )
        {
            pReturn = pConnexion->CreateStatement( query, result );
        }
        
        return pReturn;
    }
    
    DatabaseQueryPtr CDatabase::CreateQuery( const String & query, EErrorType * result )
    {
        DatabaseQueryPtr pReturn;
        DatabaseConnectionPtr pConnexion = DoGetConnection();
        
        if ( pConnexion )
        {
            pReturn = pConnexion->CreateQuery( query, result );
        }
        
        return pReturn;
    }
    
    bool CDatabase::ExecuteUpdate( const String & query, EErrorType * result )
    {
        bool bReturn;
        DatabaseConnectionPtr pConnexion = DoGetConnection();
        
        if ( pConnexion )
        {
            bReturn = pConnexion->ExecuteUpdate( query, result );
        }
        
        return bReturn;
    }
    
    DatabaseResultPtr CDatabase::ExecuteSelect( const String & query, EErrorType * result )
    {
        DatabaseResultPtr pReturn;
        DatabaseConnectionPtr pConnexion = DoGetConnection();
        
        if ( pConnexion )
        {
            pReturn = pConnexion->ExecuteSelect( query, result );
        }
        
        return pReturn;
    }
    
    std::string CDatabase::WriteText( const std::string & text ) const
    {
        std::string strReturn;
        DatabaseConnectionPtr pConnexion = DoGetConnection();
        
        if ( pConnexion )
        {
            strReturn = pConnexion->WriteText( text );
        }
        
        return strReturn;
    }
    
    std::wstring CDatabase::WriteNText( const std::wstring & text ) const
    {
        std::wstring strReturn;
        DatabaseConnectionPtr pConnexion = DoGetConnection();
        
        if ( pConnexion )
        {
            strReturn = pConnexion->WriteNText( text );
        }
        
        return strReturn;
    }
    
    String CDatabase::WriteName( const String & name ) const
    {
        String strReturn;
        DatabaseConnectionPtr pConnexion = DoGetConnection();
        
        if ( pConnexion )
        {
            strReturn = pConnexion->WriteName( name );
        }
        
        return strReturn;
    }
    
    std::string CDatabase::WriteDate( const CDate & date ) const
    {
        std::string strReturn;
        DatabaseConnectionPtr pConnexion = DoGetConnection();
        
        if ( pConnexion )
        {
            strReturn = pConnexion->WriteDate( date );
        }
        
        return strReturn;
    }
    
    std::string CDatabase::WriteDate( const std::string & date, const std::string & format ) const
    {
        std::string strReturn;
        DatabaseConnectionPtr pConnexion = DoGetConnection();
        
        if ( pConnexion )
        {
            strReturn = pConnexion->WriteDate( date, format );
        }
        
        return strReturn;
    }
    
    std::string CDatabase::WriteTime( const CTime & time ) const
    {
        std::string strReturn;
        DatabaseConnectionPtr pConnexion = DoGetConnection();
        
        if ( pConnexion )
        {
            strReturn = pConnexion->WriteTime( time );
        }
        
        return strReturn;
    }
    
    std::string CDatabase::WriteTime( const std::string & time, const std::string & format ) const
    {
        std::string strReturn;
        DatabaseConnectionPtr pConnexion = DoGetConnection();
        
        if ( pConnexion )
        {
            strReturn = pConnexion->WriteTime( time, format );
        }
        
        return strReturn;
    }
    
    std::string CDatabase::WriteDateTime( const CDateTime & dateTime ) const
    {
        std::string strReturn;
        DatabaseConnectionPtr pConnexion = DoGetConnection();
        
        if ( pConnexion )
        {
            strReturn = pConnexion->WriteDateTime( dateTime );
        }
        
        return strReturn;
    }
    
    std::string CDatabase::WriteDateTime( const std::string & dateTime, const std::string & format ) const
    {
        std::string strReturn;
        DatabaseConnectionPtr pConnexion = DoGetConnection();
        
        if ( pConnexion )
        {
            strReturn = pConnexion->WriteDateTime( dateTime, format );
        }
        
        return strReturn;
    }
    
    std::string CDatabase::WriteStmtDate( const CDate & date ) const
    {
        std::string strReturn;
        DatabaseConnectionPtr pConnexion = DoGetConnection();
        
        if ( pConnexion )
        {
            strReturn = pConnexion->WriteStmtDate( date );
        }
        
        return strReturn;
    }
    
    std::string CDatabase::WriteStmtDate( const std::string & date, const std::string & format ) const
    {
        std::string strReturn;
        DatabaseConnectionPtr pConnexion = DoGetConnection();
        
        if ( pConnexion )
        {
            strReturn = pConnexion->WriteStmtDate( date, format );
        }
        
        return strReturn;
    }
    
    std::string CDatabase::WriteStmtTime( const CTime & time ) const
    {
        std::string strReturn;
        DatabaseConnectionPtr pConnexion = DoGetConnection();
        
        if ( pConnexion )
        {
            strReturn = pConnexion->WriteStmtTime( time );
        }
        
        return strReturn;
    }
    
    std::string CDatabase::WriteStmtTime( const std::string & time, const std::string & format ) const
    {
        std::string strReturn;
        DatabaseConnectionPtr pConnexion = DoGetConnection();
        
        if ( pConnexion )
        {
            strReturn = pConnexion->WriteStmtTime( time, format );
        }
        
        return strReturn;
    }
    
    std::string CDatabase::WriteStmtDateTime( const CDateTime & dateTime ) const
    {
        std::string strReturn;
        DatabaseConnectionPtr pConnexion = DoGetConnection();
        
        if ( pConnexion )
        {
            strReturn = pConnexion->WriteStmtDateTime( dateTime );
        }
        
        return strReturn;
    }
    
    std::string CDatabase::WriteStmtDateTime( const std::string & dateTime, const std::string & format ) const
    {
        std::string strReturn;
        DatabaseConnectionPtr pConnexion = DoGetConnection();
        
        if ( pConnexion )
        {
            strReturn = pConnexion->WriteStmtDateTime( dateTime, format );
        }
        
        return strReturn;
    }
    
    String CDatabase::WriteBool( bool value ) const
    {
        String strReturn;
        DatabaseConnectionPtr pConnexion = DoGetConnection();
        
        if ( pConnexion )
        {
            strReturn = pConnexion->WriteBool( value );
        }
        
        return strReturn;
    }
    
    String CDatabase::WriteBool( const String & value ) const
    {
        String strReturn;
        DatabaseConnectionPtr pConnexion = DoGetConnection();
        
        if ( pConnexion )
        {
            strReturn = pConnexion->WriteBool( value );
        }
        
        return strReturn;
    }
    
    CDate CDatabase::ParseDate( const String & date ) const
    {
        CDate dtReturn;
        DatabaseConnectionPtr pConnexion = DoGetConnection();
        
        if ( pConnexion )
        {
            dtReturn = pConnexion->ParseDate( date );
        }
        
        return dtReturn;
    }
    
    CTime CDatabase::ParseTime( const String & time ) const
    {
        CTime dtReturn;
        DatabaseConnectionPtr pConnexion = DoGetConnection();
        
        if ( pConnexion )
        {
            dtReturn = pConnexion->ParseTime( time );
        }
        
        return dtReturn;
    }
    
    CDateTime CDatabase::ParseDateTime( const String & dateTime ) const
    {
        CDateTime dtReturn;
        DatabaseConnectionPtr pConnexion = DoGetConnection();
        
        if ( pConnexion )
        {
            dtReturn = pConnexion->ParseDateTime( dateTime );
        }
        
        return dtReturn;
    }
    
    bool CDatabase::IsConnected() const
    {
        bool bReturn = false;
        DatabaseConnectionPtr pConnexion = DoGetConnection();
        
        if ( pConnexion )
        {
            bReturn = pConnexion->IsConnected();
        }
        
        return bReturn;
    }
    
    bool CDatabase::IsInTransaction() const
    {
        bool bReturn = false;
        DatabaseConnectionPtr pConnexion = DoGetConnection();
        
        if ( pConnexion )
        {
            bReturn = pConnexion->IsInTransaction();
        }
        
        return bReturn;
    }
    
    DatabaseConnectionPtr CDatabase::DoGetConnection() const
    {
        DatabaseConnectionPtr pReturn;
        std::unique_lock< std::recursive_mutex > lock( _mutex );
        pReturn = _connection;
        
        if ( ! pReturn )
        {
            std::thread::id threadId = std::this_thread::get_id();
            DatabaseConnectionPtrIdMap::const_iterator it = _mapConnections.find( threadId );
            
            if ( it != _mapConnections.end() )
            {
                pReturn = it->second;
            }
        }
        
        return pReturn;
    }
}
END_NAMESPACE_DATABASE
