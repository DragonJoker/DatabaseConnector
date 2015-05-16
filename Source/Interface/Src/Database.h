/************************************************************************//**
 * @file Database.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 * 
 *
 * @brief CDatabase class declaration.
 *
 * @details Describes a database.
 *
 ***************************************************************************/

#ifndef ___DATABASE_H___
#define ___DATABASE_H___

#include "DatabasePrerequisites.h"

#include "EErrorType.h"

BEGIN_NAMESPACE_DATABASE
{
    /** Describes a database.
    */
    class DatabaseExport CDatabase
    {
    
    public:
        /** Constructor.
        @param[in] type
            Database type.
        */
        CDatabase();
        
        /** Destructor.
        */
        virtual ~CDatabase();
        
        /** Connect to the database.
        @param[in] server
            Server identifier (name or address).
        @param[in] database
            Database identifier (name or DSN (ODBC)).
        @param[in] userName
            User name.
        @param[in] password
            User password.
        @param[out] connectionString
            Created connection string.
        */
        void Connect( const String & server, const String & database, const String & userName, const String & password, String & connectionString );
        
        /** Close connection to the database.
        */
        void Disconnect();
        
        /** Initialize data.
        @param[in] server
            Server identifier (name or address).
        @param[in] database
            Database identifier (name or DSN (ODBC)).
        @param[in] userName
            User name.
        @param[in] password
            User password.
        */
        void Initialize( const String & server, const String & database, const String & userName, const String & password );
        
        /** Create a connection.
        @param[out] connectionString
            Created connection string.
        */
        void CreateConnection( String & connectionString );
        
        /** Retrieve connection.
        @return
            Found connection.
        */
        DatabaseConnectionPtr RetrieveConnection();
        
        /** Remove connection.
        */
        void RemoveConnection();
        
        /** Initialize a named transaction.
        @param[in] name
            Transaction name.
        @return
            Error code.
        */
        EErrorType BeginTransaction( const String & name = STR( "" ) );
        
        /** Validate the operations of the transaction.
        @param[in] name
            Transaction name.
        @return
            Error code.
        */
        EErrorType Commit( const String & name = STR( "" ) );
        
        /** Invalidate the operations of the transaction.
        @param[in] name
            Transaction name.
        @return
            Error code.
        */
        EErrorType RollBack( const String & name = STR( "" ) );
        
        /** Create a statement based on a request.
        @param[in] query
            Request text.
        @param[out] result
            Error code.
        @return
            Created statement.
        */
        DatabaseStatementPtr CreateStatement( const String & query, EErrorType * result );
        
        /** Create a query based on a request.
        @param[in] query
            Request text.
        @param[out] result
            Error code.
        @return
            Created query.
        */
        DatabaseQueryPtr CreateQuery( const String & query, EErrorType * result );
        
        /** Execute directly a request without result set.
        @param[in] query
            Request text.
        @param[out] result
            Error code.
        @return
            Results.
        */
        bool ExecuteUpdate( const String & query, EErrorType * result );
        
        /** Execute directly a request with a result set.
        @param[in] query
            Request text.
        @param[out] result
            Error code.
        @return
            Results.
        */
        DatabaseResultPtr ExecuteSelect( const String & query, EErrorType * result );
        
        /** Format a string to insert into a request.
        @param[in] text
            Value to format.
        @return
            Formatted value.
        */
        std::string WriteText( const std::string & text ) const;
        
        /** Format a string to insert into a request.
        @param[in] text
            Value to format.
        @return
            Formatted value.
        */
        std::wstring WriteNText( const std::wstring & text ) const;
        
        /** Format a string to insert into a request.
        @param[in] name
            Value to format.
        @return
            Formatted value.
        */
        String WriteName( const String & name ) const;
        
        /** Format a date to insert into a request.
        @param[in] date
            Value to format.
        @return
            Formatted value.
        */
        std::string WriteDate( const CDate & date ) const;
        
        /** Format a date to insert into a request.
        @param[in] date
            Value to format.
        @param[in] format
            String describing the format.
        @return
            Formatted value.
        */
        std::string WriteDate( const std::string & date, const std::string & format ) const;
        
        /** Format a date to insert into a statement.
        @param[in] date
            Value to format.
        @return
            Formatted value.
        */
        std::string WriteStmtDate( const CDate & date ) const;
        
        /** Format a date to insert into a statement.
        @param[in] date
            Value to format.
        @param[in] format
            String describing the format.
        @return
            Formatted value.
        */
        std::string WriteStmtDate( const std::string & date, const std::string & format ) const;
        
        /** Format a time to insert into a request.
        @param[in] time
            Value to format.
        @return
            Formatted value.
        */
        std::string WriteTime( const CTime & time ) const;
        
        /** Format a time to insert into a request.
        @param[in] time
            Value to format.
        @param[in] format
            String describing the format.
        @return
            Formatted value.
        */
        std::string WriteTime( const std::string & time, const std::string & format ) const;
        
        /** Format a time to insert into a statement.
        @param[in] time
            Value to format.
        @return
            Formatted value.
        */
        std::string WriteStmtTime( const CTime & time ) const;
        
        /** Format a time to insert into a statement.
        @param[in] time
            Value to format.
        @param[in] format
            String describing the format.
        @return
            Formatted value.
        */
        std::string WriteStmtTime( const std::string & time, const std::string & format ) const;
        
        /** Format a date/time to insert into a request.
        @param[in] dateTime
            Value to format.
        @return
            Formatted value.
        */
        std::string WriteDateTime( const CDateTime & dateTime ) const;
        
        /** Format a date/time to insert into a request.
        @param[in] dateTime
            Value to format.
        @param[in] format
            String describing the format.
        @return
            Formatted value.
        */
        std::string WriteDateTime( const std::string & dateTime, const std::string & format ) const;
        
        /** Format a date/time to insert into a statement.
        @param[in] dateTime
            Value to format.
        @return
            Formatted value.
        */
        std::string WriteStmtDateTime( const CDateTime & dateTime ) const;
        
        /** Format a date/time to insert into a statement.
        @param[in] dateTime
            Value to format.
        @param[in] format
            String describing the format.
        @return
            Formatted value.
        */
        std::string WriteStmtDateTime( const std::string & dateTime, const std::string & format ) const;
        
        /** Format a boolean to insert into a request or statement.
        @param[in] value
            Value to format.
        @return
            Formatted value.
        */
        String WriteBool( bool value ) const;
        
        /** Format a boolean to insert into a request or statement.
        @param[in] value
            Value to format.
        @return
            Formatted value.
        */
        String WriteBool( const String & value ) const;
        
        /** Convert a string from the database to a date.
        @param[in] date
            String representing a date.
        @return
            Created date with the string.
        */
        CDate ParseDate( const String & date ) const;
        
        /** Convert a string from the database to a time.
        @param[in] time
            String representing a time.
        @return
            Created time with the string.
        */
        CTime ParseTime( const String & time ) const;
        
        /** Convert a string from the database to a date/time.
        @param[in] dateTime
            String representing a date/time.
        @return
            Created date/time with the string.
        */
        CDateTime ParseDateTime( const String & dateTime ) const;
        
        /** Get the connection status.
        @return
            true if connected, false otherwise.
        */
        bool IsConnected() const;
        
        /** Get the transaction status.
        @return
            true if a transaction is started, false otherwise.
        */
        bool IsInTransaction() const;
        
    protected:
    
        /** Create a connection.
        @param[out] connectionString
            Created connection string.
        @return
            Database connection.
        */
        virtual DatabaseConnectionPtr DoCreateConnection( String & connectionString ) const = 0;
        
    protected:
    
        typedef std::map< unsigned long, DatabaseConnectionPtr > DatabaseConnectionPtrDwordMap;
        
        mutable std::recursive_mutex    _mutex;             ///< Mutex.
        DatabaseConnectionPtr           _connection;        ///< Connection (single thread).
        DatabaseConnectionPtrIdMap      _mapConnections;    ///< Map of connections (multiple threads).
        String                          _server;            ///< Server identifier (name or address).
        String                          _database;          ///< Database identifier (name or DSN (ODBC)).
        String                          _userName;          ///< User name.
        String                          _password;          ///< User password.
        
    private:
    
        /** Get connection.
        @return
            Database connection.
        */
        DatabaseConnectionPtr DoGetConnection() const;
        
    }; // class CDatabase
    
}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_H___
