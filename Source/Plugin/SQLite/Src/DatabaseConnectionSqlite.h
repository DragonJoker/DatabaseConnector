/************************************************************************//**
 * @file DatabaseConnectionSqlite.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/14/2014 5:03:05 PM
 * 
 *
 * @brief CDatabaseConnectionSqlite class declaration.
 *
 * @details Describes a connection to a database via an SQLite driver.
 *
 ***************************************************************************/

#ifndef ___DATABASE_CONNECTION_SQLITE_H___
#define ___DATABASE_CONNECTION_SQLITE_H___

#include "DatabaseSqlitePrerequisites.h"

#include <DatabaseConnection.h>

BEGIN_NAMESPACE_DATABASE_SQLITE
{
    /** Describes a connection to a database via an SQLite driver.
    */
    class CDatabaseConnectionSqlite
        : public CDatabaseConnection
        , public std::enable_shared_from_this< CDatabaseConnectionSqlite >
    {
    public:
        /** Constructor.
        @param[in] driver
            The SQLite driver.
        @param[in] server
            Address or name of the server.
        @param[in] database
            Database name or DSN.
        @param[in] userName
            User name.
        @param[in] password
            User password.
        @param[out] connectionString
            Created connection string.
         */
        DatabaseSqliteExport CDatabaseConnectionSqlite( const String & server, const String & database, const String & userName, const String & password, String & connectionString );
        
        /** Destructor.
         */
        DatabaseSqliteExport virtual ~CDatabaseConnectionSqlite();
        
        /** Initialize a named transaction.
        @param[in] name
            Transaction name.
        @return
            Error code, EErrorType_NONE if no problem.
        */
        DatabaseSqliteExport virtual EErrorType BeginTransaction( const String & name );
        
        /** Validate a named transaction.
        @param[in] name
            Transaction name.
        @return
            Error code, EErrorType_NONE if no problem.
        */
        DatabaseSqliteExport virtual EErrorType Commit( const String & name );
        
        /** Invalidate a named transaction.
        @param[in] name
            Transaction name.
        @return
            Error code, EErrorType_NONE if no problem.
        */
        DatabaseSqliteExport virtual EErrorType RollBack( const String & name );
        
        /** Format a string to be supported by the DBMS.
        @param[in] text
            Text to format.
        @return
            The formatted text.
        */
        DatabaseSqliteExport virtual std::string WriteText( const std::string & text ) const;
        
        /** Format a string to be supported by the DBMS.
        @param[in] text
            Text to format.
        @return
            The formatted text.
        */
        DatabaseSqliteExport virtual std::wstring WriteNText( const std::wstring & text ) const;
        
        /** Format a string to be supported by the DBMS.
        @param[in] text
            Text to format.
        @return
            The formatted text.
        */
        DatabaseSqliteExport virtual String WriteName( const String & text ) const;
        
        /** Format a date into a string to be supported by the DBMS.
        @param[in] date
            Date to format.
        @return
            The formatted date.
        */
        DatabaseSqliteExport virtual std::string WriteDate( const CDate & date ) const;
        
        /** Format a date into a string to be supported by the DBMS.
        @param[in] date
            Date to format.
        @param[in] format
            Format to use.
        @return
            The formatted date.
        */
        DatabaseSqliteExport virtual std::string WriteDate( const std::string & date, const std::string & format ) const;
        
        /** Format a date into a string for a statement to be supported by the DBMS.
        @param[in] date
            Date to format.
        @return
            The formatted date.
        */
        DatabaseSqliteExport virtual std::string WriteStmtDate( const CDate & date ) const;
        
        /** Format a date into a string for a statement to be supported by the DBMS.
        @param[in] date
            Date to format.
        @param[in] format
            Format to use.
        @return
            The formatted date.
        */
        DatabaseSqliteExport virtual std::string WriteStmtDate( const std::string & date, const std::string & format ) const;
        
        /** Format a time into a string to be supported by the DBMS.
        @param[in] time
            Time to format.
        @return
            The formatted time.
        */
        DatabaseSqliteExport virtual std::string WriteTime( const CTime & time ) const;
        
        /** Format a time into a string to be supported by the DBMS.
        @param[in] time
            Time to format.
        @param[in] format
            Format to use.
        @return
            The formatted time.
        */
        DatabaseSqliteExport virtual std::string WriteTime( const std::string & time, const std::string & format ) const;
        
        /** Format a time into a string for a statement to be supported by the DBMS.
        @param[in] time
            Time to format.
        @return
            The formatted time.
        */
        DatabaseSqliteExport virtual std::string WriteStmtTime( const CTime & time ) const;
        
        /** Format a time into a string for a statement to be supported by the DBMS.
        @param[in] time
            Time to format.
        @param[in] format
            Format to use.
        @return
            The formatted time.
        */
        DatabaseSqliteExport virtual std::string WriteStmtTime( const std::string & time, const std::string & format ) const;
        
        /** Format a date/time into a string to be supported by the DBMS.
        @param[in] dateTime
            Date/time to format.
        @return
            The formatted date/time.
        */
        DatabaseSqliteExport virtual std::string WriteDateTime( const CDateTime & dateTime ) const;
        
        /** Format a date/time to insert into a request.
        @param[in] date
            Value to format.
        @return
            Formatted value.
        */
        virtual std::string WriteDateTime( const CDate & date ) const;
        
        /** Format a date/time to insert into a request.
        @param[in] time
            Value to format.
        @return
            Formatted value.
        */
        virtual std::string WriteDateTime( const CTime & time ) const;
        
        /** Format a date/time into a string to be supported by the DBMS.
        @param[in] dateTime
            Date/time to format.
        @param[in] format
            Format to use.
        @return
            The formatted date/time.
        */
        DatabaseSqliteExport virtual std::string WriteDateTime( const std::string & dateTime, const std::string & format ) const;
        
        /** Format a date/time into a string for a statement to be supported by the DBMS.
        @param[in] dateTime
            Date/time to format.
        @return
            The formatted date/time.
        */
        DatabaseSqliteExport virtual std::string WriteStmtDateTime( const CDateTime & dateTime ) const;
        
        /** Format a date/time into a string for a statement to be supported by the DBMS.
        @param[in] dateTime
            Date/time to format.
        @param[in] format
            Format to use.
        @return
            The formatted date/time.
        */
        DatabaseSqliteExport virtual std::string WriteStmtDateTime( const std::string & dateTime, const std::string & format ) const;
        
        /** Format a boolean into a string to be supported by the DBMS.
        @param[in] value
            Boolean to format.
        @return
            The formatted boolean.
        */
        DatabaseSqliteExport virtual String WriteBool( bool value ) const;
        
        /** Format a string representing a boolean into a string to be supported by the DBMS.
        @param[in] value
            Boolean to format.
        @return
            The formatted boolean.
        */
        DatabaseSqliteExport virtual String WriteBool( const String & value ) const;
        
        /** Convert a string from the database to a date.
        @param[in] date
            String representing a date.
        @return
            Created date with the string.
        */
        DatabaseSqliteExport virtual CDate ParseDate( const std::string & date ) const;
        
        /** Convert a string from the database to a time.
        @param[in] time
            String representing a time.
        @return
            Created time with the string.
        */
        DatabaseSqliteExport virtual CTime ParseTime( const std::string & time ) const;
        
        /** Convert a string from the database to a date/time.
        @param[in] dateTime
            String representing a date/time.
        @return
            Created date/time with the string.
        */
        DatabaseSqliteExport virtual CDateTime ParseDateTime( const std::string & dateTime ) const;
        
        /** Convert a string from the database to a date.
        @param[in] date
            String representing a date.
        @return
            Created date with the string.
        */
        DatabaseSqliteExport virtual CDate ParseDate( const std::wstring & date ) const;
        
        /** Convert a string from the database to a time.
        @param[in] time
            String representing a time.
        @return
            Created time with the string.
        */
        DatabaseSqliteExport virtual CTime ParseTime( const std::wstring & time ) const;
        
        /** Convert a string from the database to a date/time.
        @param[in] dateTime
            String representing a date/time.
        @return
            Created date/time with the string.
        */
        DatabaseSqliteExport virtual CDateTime ParseDateTime( const std::wstring & dateTime ) const;
        
        /** Get the connection handle.
        @return
            The connection handle.
        */
        DatabaseSqliteExport SQLite::Database * GetConnection() const;
        
        /** Executes a statement and retrieves the result set if needed
        @param statement
            The statement
        @return
            The result
        */
        bool ExecuteUpdate( std::shared_ptr< SQLite::Statement > statement );
        
        /** Executes a statement and retrieves the result set if needed
        @param statement
            The statement
        @return
            The result
        */
        DatabaseResultSqlitePtr ExecuteSelect( std::shared_ptr< SQLite::Statement > statement );
        
        /** Executes a statement and retrieves the result set if needed
        @param statement
            The statement
        @param query
            The query, if needed
        @return
            The result
        */
        bool ExecuteUpdate( const String & query );
        
        /** Executes a statement and retrieves the result set if needed
        @param statement
            The statement
        @param query
            The query, if needed
        @return
            The result
        */
        DatabaseResultSqlitePtr ExecuteSelect( const String & query );
        
    protected:
        /** Connect to the database.
        @param[out] connectionString
            Connection string in case of error.
        @return
            Error code, EErrorType_NONE if no problem.
        */
        DatabaseSqliteExport virtual EErrorType DoConnect( String & connectionString );
        
        /** Disconnect from the database.
        */
        DatabaseSqliteExport virtual void DoDisconnect();
        
        /** Execute directly a request.
        @param[in]  query
            Request text.
        @param[out] result
            Error code if the returned value is NULL.
        @return
            The result.
        */
        DatabaseSqliteExport virtual bool DoExecuteUpdate( const String & query, EErrorType * result );
        
        /** Execute directly a request.
        @param[in]  query
            Request text.
        @param[out] result
            Error code if the returned value is NULL.
        @return
            The result.
        */
        DatabaseSqliteExport virtual DatabaseResultPtr DoExecuteSelect( const String & query, EErrorType * result );
        
        /** Create a statement from a request.
        @param[in]  query
            Request text.
        @param[out] result
            Error code if the returned value is NULL.
        @return
            The created statement.
        */
        DatabaseSqliteExport virtual DatabaseStatementPtr DoCreateStatement( const String & query, EErrorType * result );
        
        /** Create a query from a request.
        @param[in]  query
            Request text.
        @param[out] result
            Error code if the returned value is NULL.
        @return
            The created query.
        */
        DatabaseSqliteExport virtual DatabaseQueryPtr DoCreateQuery( const String & query, EErrorType * result );
        
        /** Retrieves the result for the statement
        @param statement
            The statement
        @param rs
            The result set
        */
        DatabaseResultSqlitePtr DoRetrieveResults( std::shared_ptr< SQLite::Statement > statement );
        
    protected:
        //! The connection
        SQLite::Database * _connection;
        //! The global statement used to execute direct queries
        std::shared_ptr< SQLite::Statement > _statement;
    };
}
END_NAMESPACE_DATABASE_SQLITE

#endif // ___DATABASE_CONNECTION_SQLITE_H___
