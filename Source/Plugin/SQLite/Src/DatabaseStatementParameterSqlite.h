/************************************************************************//**
 * @file DatabaseStatementParameterSqlite.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 * 
 *
 * @brief CDatabaseStatementParameterSqlite class declaration.
 *
 * @details Describes a statement parameter for SQLite database.
 *
 ***************************************************************************/

#ifndef ___DATABASE_STATEMENT_PARAMETER_SQLITE_H___
#define ___DATABASE_STATEMENT_PARAMETER_SQLITE_H___

#include "DatabaseSqlitePrerequisites.h"

#include <DatabaseParameter.h>
#include "DatabaseParameterSqlite.h"

#include <cppconn/prepared_statement.h>

BEGIN_NAMESPACE_DATABASE_SQLITE
{
    /** Used to stream a byte array into an std::istream
    */
    struct membuf
            : std::streambuf
    {
        /** Consctructor
        */
        membuf( char * begin, char * end )
        {
            this->setg( begin, begin, end );
        }
    };
    /** Describes a statement parameter for SQLite database.
    */
    class CDatabaseStatementParameterSqlite
        : public CDatabaseParameter
        , public CDatabaseParameterSqlite
    {
    
    public:
        /** Constructor.
        @param[in] connection
            Connection to database.
        @param[in] name
            Parameter name.
        @param[in] index
            Internal index.
        @param[in] fieldType
            Field type.
        @param[in] parameterType
            Parameter type.
        @param[in] updater
            The parent updater
         */
        DatabaseSqliteExport CDatabaseStatementParameterSqlite( DatabaseConnectionSqlitePtr connection, const String & name, unsigned short index, EFieldType fieldType, EParameterType parameterType, SValueUpdater * updater );
        
        /** Constructor.
        @param[in] connection
            Connection to database.
        @param[in] name
            Parameter name.
        @param[in] index
            Internal index.
        @param[in] fieldType
            Field type.
        @param[in] limits
            Field limits.
        @param[in] parameterType
            Parameter type.
        @param[in] updater
            The parent updater
         */
        DatabaseSqliteExport CDatabaseStatementParameterSqlite( DatabaseConnectionSqlitePtr connection, const String & name, unsigned short index, EFieldType fieldType, uint32_t limits, EParameterType parameterType, SValueUpdater * updater );
        
        /** Destructor.
         */
        DatabaseSqliteExport virtual ~CDatabaseStatementParameterSqlite();
        
        //!@copydoc Areva::ARIA::Native::Database::CDatabaseParameter::SetNull
        virtual void SetNull();
        
        //!@copydoc Areva::ARIA::Native::Database::CDatabaseParameter::SetValue
        void SetValue( DatabaseParameterPtr value );
        
        /** Defines the prepared statement
        @param statement
            The statement
        */
        void SetStatement( sql::PreparedStatement * statement )
        {
            _statement = statement;
        }
        
    private:
        //!@copydoc Areva::ARIA::Native::Database::CDatabaseParameter::DoSetValue
        virtual void DoSetValue( bool value );
        
        //!@copydoc Areva::ARIA::Native::Database::CDatabaseParameter::DoSetValue
        virtual void DoSetValue( int16_t value );
        
        //!@copydoc Areva::ARIA::Native::Database::CDatabaseParameter::DoSetValue
        virtual void DoSetValue( uint16_t value );
        
        //!@copydoc Areva::ARIA::Native::Database::CDatabaseParameter::DoSetValue
        virtual void DoSetValue( int value );
        
        //!@copydoc Areva::ARIA::Native::Database::CDatabaseParameter::DoSetValue
        virtual void DoSetValue( unsigned int value );
        
        //!@copydoc Areva::ARIA::Native::Database::CDatabaseParameter::DoSetValue
        virtual void DoSetValue( long value );
        
        //!@copydoc Areva::ARIA::Native::Database::CDatabaseParameter::DoSetValue
        virtual void DoSetValue( unsigned long value );
        
        //!@copydoc Areva::ARIA::Native::Database::CDatabaseParameter::DoSetValue
        virtual void DoSetValue( int64_t value );
        
        //!@copydoc Areva::ARIA::Native::Database::CDatabaseParameter::DoSetValue
        virtual void DoSetValue( uint64_t value );
        
        //!@copydoc Areva::ARIA::Native::Database::CDatabaseParameter::DoSetValue
        virtual void DoSetValue( float value );
        
        //!@copydoc Areva::ARIA::Native::Database::CDatabaseParameter::DoSetValue
        virtual void DoSetValue( double value );
        
        //!@copydoc Areva::ARIA::Native::Database::CDatabaseParameter::DoSetValue
        virtual void DoSetValue( long double value );
        
        //!@copydoc Areva::ARIA::Native::Database::CDatabaseParameter::DoSetValue
        virtual void DoSetValue( const char * value );
        
        //!@copydoc Areva::ARIA::Native::Database::CDatabaseParameter::DoSetValue
        virtual void DoSetValue( const wchar_t * value );
        
        //!@copydoc Areva::ARIA::Native::Database::CDatabaseParameter::DoSetValue
        virtual void DoSetValue( std::vector< uint8_t > & value );
        
        //!@copydoc Areva::ARIA::Native::Database::CDatabaseParameter::DoSetValue
        virtual void DoSetValue( const CDateTime & value );
        
        //!@copydoc Areva::ARIA::Native::Database::CDatabaseParameter::DoSetValue
        virtual void DoSetValue( const CDate & value );
        
        //!@copydoc Areva::ARIA::Native::Database::CDatabaseParameter::DoSetValue
        virtual void DoSetValue( const CTime & value );
        
        //!@copydoc Areva::ARIA::Native::Database::CDatabaseParameter::DoSetValue
        virtual void DoSetValue( std::istream * value );
        
        /** Initializes the parameter value setter
        @remarks
            Called from constructor, takes account of the fact that CDatabaseStatementParameter constructor throws an exception
        */
        void DoInitializeParamSetter();
        
        //! The parameter value setter
        SParameterValueSetterBase * _paramSetter;
        //! The stream value, for binary fields
        std::istream * _streamValue;
        //! The stream buffer
        std::streambuf * _streamBuffer;
        //! The prepared statement
        sql::PreparedStatement * _statement;
    };
}
END_NAMESPACE_DATABASE_SQLITE

#endif // ___DATABASE_STATEMENT_PARAMETER_SQLITE_H___
