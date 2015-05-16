/************************************************************************//**
 * @file DatabaseStatementParameterMySql.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 * 
 *
 * @brief CDatabaseStatementParameterMySql class declaration.
 *
 * @details Describes a statement parameter for MYSQL database.
 *
 ***************************************************************************/

#ifndef ___DATABASE_STATEMENT_PARAMETER_MYSQL_H___
#define ___DATABASE_STATEMENT_PARAMETER_MYSQL_H___

#include "DatabaseMySqlPrerequisites.h"

#include <DatabaseParameter.h>
#include "DatabaseParameterMySql.h"

#include <cppconn/prepared_statement.h>

BEGIN_NAMESPACE_DATABASE_MYSQL
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
    /** Describes a statement parameter for MYSQL database.
    */
    class CDatabaseStatementParameterMySql
        : public CDatabaseParameter
        , public CDatabaseParameterMySql
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
        DatabaseMySqlExport CDatabaseStatementParameterMySql( DatabaseConnectionMySqlPtr connection, const String & name, unsigned short index, EFieldType fieldType, EParameterType parameterType, SValueUpdater * updater );
        
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
        DatabaseMySqlExport CDatabaseStatementParameterMySql( DatabaseConnectionMySqlPtr connection, const String & name, unsigned short index, EFieldType fieldType, uint32_t limits, EParameterType parameterType, SValueUpdater * updater );
        
        /** Destructor.
         */
        DatabaseMySqlExport virtual ~CDatabaseStatementParameterMySql();
        
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
END_NAMESPACE_DATABASE_MYSQL

#endif // ___DATABASE_STATEMENT_PARAMETER_MYSQL_H___
