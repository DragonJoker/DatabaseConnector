/************************************************************************//**
 * @file DatabaseParameter.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 * 
 *
 * @brief CDatabaseParameter class declaration.
 *
 * @details Describes a parameter for a query or a statement.
 *
 ***************************************************************************/

#ifndef ___DATABASE_PARAMETER_H___
#define ___DATABASE_PARAMETER_H___

#include "DatabasePrerequisites.h"

#include "EFieldType.h"
#include "EParameterType.h"
#include "DatabaseValue.h"

BEGIN_NAMESPACE_DATABASE
{
    /** Describes a parameter for a query or a statement.
    */
    class DatabaseExport CDatabaseParameter
        : public std::enable_shared_from_this< CDatabaseParameter >
    {
    public:
        /** Structure used to inform parent that the value changed
        */
        struct SValueUpdater
        {
            /** Updates the parent
            @param parameter
                The parameter
            */
            virtual void Update( DatabaseParameterPtr parameter ) = 0;
        };
        
    public:
        /** Constructor.
        @param[in] connection
            Database connection.
        @param[in] name
            Parameter name.
        @param[in] index
            Parameter index.
        @param[in] fieldType
            Field type.
        @param[in] parameterType
            Parameter type (in, inout, out).
        @param[in] updater
            The parent updater
        */
        CDatabaseParameter( DatabaseConnectionPtr connection, const String & name, unsigned short index, EFieldType fieldType, EParameterType parameterType, SValueUpdater * updater );
        
        /** Constructor.
        @param[in] connection
            Database connection.
        @param[in] name
            Parameter name.
        @param[in] index
            Parameter index.
        @param[in] fieldType
            Field type.
        @param[in] limits
            Filed limits (for VARCHAR, etc.)
        @param[in] parameterType
            Parameter type (in, inout, out).
        @param[in] updater
            The parent updater
        */
        CDatabaseParameter( DatabaseConnectionPtr connection, const String & name, unsigned short index, EFieldType fieldType, uint32_t limits, EParameterType parameterType, SValueUpdater * updater );
        
        /** Desctructor.
        */
        virtual ~CDatabaseParameter();
        
        /** Get parameter index.
        @return
            Parameter index.
        @remarks
            The reference is here to be able to pass this method to function wanting pointer.
        */
        const unsigned short & GetIndex() const;
        
        /** Get field type.
        @return
            Field type.
        */
        EFieldType GetType() const;
        
        /** Get parameter type.
        @return
            Parameter type.
        @remarks
            The reference is here to be able to pass this method to function wanting pointer.
         */
        const EParameterType & GetParamType() const;
        
        /** Get parameter name.
        @return
            Parameter name.
        @remarks
            The reference is here to be able to pass this method to function wanting pointer.
        */
        const String & GetName() const;
        
        /** Get parameter limits.
        @return
            Parameter limits.
        @remarks
            The reference is here to be able to pass this method to function wanting pointer.
        */
        const uint32_t & GetLimits() const;
        
        /** Set parameter value to NULL.
        */
        virtual void SetNull();
        
        /** Check if value is NULL.
        @return
            true if value is NULL.
        */
        virtual bool IsNull() const;
        
        /** Set parameter value from a field.
        @param[in] field
            The field.
        @remarks
            If field type is different than the value type, the value is ignored.
        */
        virtual void SetValue( DatabaseFieldPtr field );
        
        /** Set parameter value from another parameter.
        @param[in] field
            The field.
        @remarks
            If field type is different than the value type, the value is ignored.
        */
        virtual void SetValue( DatabaseParameterPtr field );
        
        /** Set parameter value.
        @remarks
            If value is a std::string or std::wstring, the user *MUST* make sure it is encoded in UTF-8
        @param[in] value
            New parameter value.
        @remarks
            If field type is different than the value type, the value is ignored.
        */
        template <typename T> inline void SetValue( T value )
        {
            DoSetValue( value );
        }
        
        /** Get parameter value.
        @remarks
            If value is a std::string or std::wstring, it will be encoded in UTF-8
        @return
            Parameter value.
        */
        template <typename T> const T & GetValue()
        {
            return static_cast< CDatabaseValue< SDataTypeFieldTyper< T >::Value > *>( _value )->GetValue();
        }
        
        /** Get parameter value.
        @remarks
            If value is a std::string or std::wstring, it will be encoded in UTF-8
        @param[out] value
            Parameter value.
        */
        template <typename T> void GetValue( T & value )
        {
            value = static_cast< CDatabaseValue< T > *>( _value )->GetValue();
        }
        
        /** Use database connection to format the parameter value.
        @return
            String insertable in an SQL request.
        */
        String GetInsertValue() const;
        
        /** Format the parameter value.
        @return
            Value as string.
        */
        String GetStrValue() const;
        
        /** Get pointer to the parameter value
        @return
            Pointer to the value.
        */
        void * GetPtrValue();
        
        /** Get the parameter value size.
        @return
            Parameter value size.
        */
        const long & GetPtrSize() const;
        
        /** Get the parameter value.
        @return
            Parameter value.
        */
        CDatabaseValueBase * GetParameterValue() const;
        
    protected:
        /** Set parameter value as a boolean.
        @param value
            New parameter value.
        */
        virtual void DoSetValue( bool value );
        
        /** Set parameter value as an int.
        @param value
            New parameter value.
        */
        virtual void DoSetValue( int16_t value );
        
        /** Set parameter value as an unsigned int.
        @param value
            New parameter value.
        */
        virtual void DoSetValue( uint16_t value );
        
        /** Set parameter value as an int.
        @param value
            New parameter value.
        */
        virtual void DoSetValue( int value );
        
        /** Set parameter value as an unsigned int.
        @param value
            New parameter value.
        */
        virtual void DoSetValue( unsigned int value );
        
        /** Set parameter value as an int.
        @param value
            New parameter value.
        */
        virtual void DoSetValue( long value );
        
        /** Set parameter value as an unsigned int.
        @param value
            New parameter value.
        */
        virtual void DoSetValue( unsigned long value );
        
        /** Set parameter value as a long long.
        @param value
            New parameter value.
        */
        virtual void DoSetValue( int64_t value );
        
        /** Set parameter value as an unsigned long long.
        @param value
            New parameter value.
        */
        virtual void DoSetValue( uint64_t value );
        
        /** Set parameter value as a float.
        @param value
            New parameter value.
        */
        virtual void DoSetValue( float value );
        
        /** Set parameter value as a double.
        @param value
            New parameter value.
        */
        virtual void DoSetValue( double value );
        
        /** Set parameter value as a double.
        @param value
            New parameter value.
        */
        virtual void DoSetValue( long double value );
        
        /** Set parameter value as a std::string.
        @param value
            New parameter value.
        */
        virtual void DoSetValue( const char * value );
        
        /** Set parameter value as a std::wstring.
        @param value
            New parameter value.
        */
        virtual void DoSetValue( const wchar_t * value );
        
        /** Set parameter value as a std::string.
        @param value
            New parameter value.
        */
        void DoSetValue( const std::string & value );
        
        /** Set parameter value as a std::wstring.
        @param value
            New parameter value.
        */
        void DoSetValue( const std::wstring & value );
        
        /** Set parameter value as a byte array.
        @param value
            New parameter value.
        */
        virtual void DoSetValue( std::vector< uint8_t > & value );
        
        /** Set parameter value as a date/time.
        @param value
            New parameter value.
        */
        virtual void DoSetValue( const CDateTime & value );
        
        /** Set parameter value as a date.
        @param value
            New parameter value.
        */
        virtual void DoSetValue( const CDate & value );
        
        /** Set parameter value as a time.
        @param value
            New parameter value.
        */
        virtual void DoSetValue( const CTime & value );
        
        /** Set parameter value as a byte array.
        @param value
            New parameter value.
        */
        virtual void DoSetValue( std::istream * value );
        
    protected:
        //! Parameter name.
        String _name;
        //! Field type.
        EFieldType _fieldType;
        //! Limits (VARCHAR, etc.).
        uint32_t _limits;
        //! Parameter type (in, out, inout).
        EParameterType _parameterType;
        //! Parameter index.
        unsigned short _index;
        //! Parameter value.
        CDatabaseValueBase * _value;
        //! Database connection.
        DatabaseConnectionPtr _connection;
        //! Functor used to inform parent that the value was modified
        SValueUpdater * _updater;
    };
}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_PARAMETER_H___
