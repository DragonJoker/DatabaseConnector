/************************************************************************//**
 * @file DatabaseValue.inl
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 *
 *
 * @brief CDatabaseValueBase derived classes.
 *
 * @details Template classes inherited CDatabaseValueBase.
 *
 ***************************************************************************/

#include "DatabasePrerequisites.h" // Help doxygen

#include "DatabaseStringUtils.h"
#include "DatabaseValuePolicy.h"

BEGIN_NAMESPACE_DATABASE
{
	//! The minimal date, time and date/time size. Needed because ODBC writes at least 16 bytes of data if the parameter is out or in/out
	static const size_t MIN_DATE_TIME_SIZE = 16;

	static const String DATABASE_VALUE_ERROR = STR( "Unsupported type." );

	/** Describes a value, used in fields and parameters.
	*/
	template< EFieldType Type > class CDatabaseValue
		: public CDatabaseValueBase
	{
	public:
		typedef typename SFieldTypeDataTyper< Type >::value_type value_type;

	public:

		/** Default constructor.
		*/
		CDatabaseValue( DatabaseConnectionPtr connection )
			: CDatabaseValueBase( connection )
			, _tValue( value_type() )
		{
			// Empty
		}

		/** Destructor.
		*/
		DatabaseExport virtual ~CDatabaseValue()
		{
			Reset();
		}

		/** Defines this value from the given one
		@param value
		    The value
		@param limits
		    The field size limit
		*/
		DatabaseExport virtual void SetValue( CDatabaseValueBase * value )
		{
			SetValue( static_cast< CDatabaseValue< Type > * >( value )->_tValue );
		}

		/** Set value.
		@param[in] tValue
		    New value.
		*/
		inline void SetValue( const value_type & tValue )
		{
			CDatabaseValuePolicy< value_type >::Set( tValue, _tValue, _isValueSet, _isValueAsStringSet, _valueSize );
			SetNull( !_isValueSet );
		}

		/** Get the value.
		@return
		    The value.
		*/
		DatabaseExport virtual void GetInsertValue( String & result )
		{
			DoUpdateTValue();
			CDatabaseValuePolicy< value_type >::ToInsertable( _tValue, _isValueSet, _connection, result );
		}

		/** Get a pointer to the value.
		@return
		    Pointer to the value or NULL.
		*/
		DatabaseExport virtual void * GetPtrValue()
		{
			DoUpdateTValue();
			return CDatabaseValuePolicy< value_type >::Ptr( _tValue );
		}

		/** Get the value.
		@return
		    The value.
		*/
		DatabaseExport virtual const value_type & GetValue()
		{
			DoUpdateTValue();
			return _tValue;
		}

		/** Re-initialize internal values.
		*/
		DatabaseExport virtual void Reset()
		{
			DoReset();
			CDatabaseValuePolicy< value_type >::Reset( _tValue );
		}

	private:

		/** Update value as string from the typed value.
		*/
		DatabaseExport virtual void DoUpdateStrValue()
		{
			CDatabaseValuePolicy< value_type >::ToStr( _tValue, _isValueSet, _value, _isValueAsStringSet );
		}

		/** Update typed value from the value as string.
		*/
		DatabaseExport virtual void DoUpdateTValue()
		{
			CDatabaseValuePolicy< value_type >::FromStr( _value, _isValueAsStringSet, _tValue, _isValueSet, _valueSize );
		}

		/** Set parameter value to NULL.
		*/
		DatabaseExport virtual void DoSetNull()
		{
			_tValue = value_type( 0 );
			_valueSize = 0;
		}

	private:
		value_type _tValue;
	};

	/** Describes a EFieldType_VARCHAR field value.
	*/
	template<> class CDatabaseValue< EFieldType_VARCHAR >
		: public CDatabaseValueBase
	{
	public:
		typedef SFieldTypeDataTyper< EFieldType_VARCHAR >::value_type value_type;

	private:
		value_type _tValue;

	public:

		/** Default constructor.
		*/
		CDatabaseValue( DatabaseConnectionPtr connection )
			: CDatabaseValueBase( connection )
			, _tValue( value_type() )
		{
			// Empty
		}

		/** Destructor.
		*/
		DatabaseExport virtual ~CDatabaseValue()
		{
			Reset();
		}

		/** Defines this value from the given one
		@param value
		    The value
		@param limits
		    The field size limit
		*/
		DatabaseExport virtual void SetValue( CDatabaseValueBase * value )
		{
			SetValue( static_cast< CDatabaseValue< EFieldType_VARCHAR > * >( value )->_tValue );
		}

		/** Set value.
		@param[in] tValue
		    New value.
		@param[in] limits
		    Parameter limits.
		*/
		inline void SetValue( const value_type & tValue )
		{
			CDatabaseValuePolicy< value_type >::Set( tValue, _tValue, _isValueSet, _isValueAsStringSet, _valueSize );
			SetNull( !_isValueSet );
		}

		/** Set value.
		@param[in] tValue
		    New value.
		@param[in] limits
		    Parameter limits.
		*/
		inline void SetValue( const char * tValue, uint32_t limits )
		{
			value_type value;

			if ( limits != 0 )
			{
				value.assign( tValue, tValue + std::min< uint32_t >( limits, uint32_t( strlen( tValue ) ) ) );
			}

			SetValue( value );
		}

		/** Get the value.
		@return
		    The value.
		*/
		DatabaseExport virtual void GetInsertValue( String & result )
		{
			DoUpdateTValue();
			CDatabaseValuePolicy< value_type >::ToInsertable( _tValue, _isValueSet, _connection, result );
		}

		/** Get a pointer to the value.
		@return
		    Pointer to the value or NULL.
		*/
		DatabaseExport virtual void * GetPtrValue()
		{
			DoUpdateTValue();
			return CDatabaseValuePolicy< value_type >::Ptr( _tValue );
		}

		/** Get the value.
		@return
		    The value.
		*/
		DatabaseExport virtual const value_type & GetValue()
		{
			DoUpdateTValue();
			return _tValue;
		}

		/** Re-initialize internal values.
		*/
		DatabaseExport virtual void Reset()
		{
			DoReset();
			CDatabaseValuePolicy< value_type >::Reset( _tValue );
		}

	private:

		/** Update value as string from the typed value.
		*/
		DatabaseExport virtual void DoUpdateStrValue()
		{
			CDatabaseValuePolicy< value_type >::ToStr( _tValue, _isValueSet, _value, _isValueAsStringSet );
		}

		/** Update typed value from the value as string.
		*/
		DatabaseExport virtual void DoUpdateTValue()
		{
			CDatabaseValuePolicy< value_type >::FromStr( _value, _isValueAsStringSet, _tValue, _isValueSet, _valueSize );
		}

		/** Set parameter value to NULL.
		*/
		DatabaseExport virtual void DoSetNull()
		{
			_tValue = value_type();
			_valueSize = 0;
		}
	};

	/** Describes a EFieldType_NVARCHAR field value.
	*/
	template<> class CDatabaseValue< EFieldType_NVARCHAR >
		: public CDatabaseValueBase
	{
	public:
		typedef SFieldTypeDataTyper< EFieldType_NVARCHAR >::value_type value_type;

	private:
		value_type _tValue;

	public:

		/** Default constructor.
		*/
		CDatabaseValue( DatabaseConnectionPtr connection )
			: CDatabaseValueBase( connection )
			, _tValue( value_type() )
		{
			// Empty
		}

		/** Destructor.
		*/
		DatabaseExport virtual ~CDatabaseValue()
		{
			Reset();
		}

		/** Defines this value from the given one
		@param value
		    The value
		@param limits
		    The field size limit
		*/
		DatabaseExport virtual void SetValue( CDatabaseValueBase * value )
		{
			SetValue( static_cast< CDatabaseValue< EFieldType_NVARCHAR > * >( value )->_tValue );
		}

		/** Set value.
		@param[in] tValue
		    New value.
		@param[in] limits
		    Parameter limits.
		*/
		inline void SetValue( const value_type & tValue )
		{
			CDatabaseValuePolicy< value_type >::Set( tValue, _tValue, _isValueSet, _isValueAsStringSet, _valueSize );
			SetNull( !_isValueSet );
		}

		/** Set value.
		@param[in] tValue
		    New value.
		@param[in] limits
		    Parameter limits.
		*/
		inline void SetValue( const wchar_t * tValue, uint32_t limits )
		{
			value_type value;

			if ( limits != 0 )
			{
				value.assign( tValue, tValue + std::min< uint32_t >( limits, uint32_t( wcslen( tValue ) ) ) );
			}

			SetValue( value );
		}

		/** Get the value.
		@return
		    The value.
		*/
		DatabaseExport virtual void GetInsertValue( String & result )
		{
			DoUpdateTValue();
			CDatabaseValuePolicy< value_type >::ToInsertable( _tValue, _isValueSet, _connection, result );
		}

		/** Get a pointer to the value.
		@return
		    Pointer to the value or NULL.
		*/
		DatabaseExport virtual void * GetPtrValue()
		{
			DoUpdateTValue();
			return CDatabaseValuePolicy< value_type >::Ptr( _tValue );
		}

		/** Get the value.
		@return
		    The value.
		*/
		DatabaseExport virtual const value_type & GetValue()
		{
			DoUpdateTValue();
			return _tValue;
		}

		/** Re-initialize internal values.
		*/
		DatabaseExport virtual void Reset()
		{
			DoReset();
			CDatabaseValuePolicy< value_type >::Reset( _tValue );
		}

	private:

		/** Update value as string from the typed value.
		*/
		DatabaseExport virtual void DoUpdateStrValue()
		{
			CDatabaseValuePolicy< value_type >::ToStr( _tValue, _isValueSet, _value, _isValueAsStringSet );
		}

		/** Update typed value from the value as string.
		*/
		DatabaseExport virtual void DoUpdateTValue()
		{
			CDatabaseValuePolicy< value_type >::FromStr( _value, _isValueAsStringSet, _tValue, _isValueSet, _valueSize );
		}

		/** Set parameter value to NULL.
		*/
		DatabaseExport virtual void DoSetNull()
		{
			_tValue = value_type();
			_valueSize = 0;
		}
	};

	/** Describes a EFieldType_DATE field value.
	*/
	template<> class CDatabaseValue< EFieldType_DATE >
		: public CDatabaseValueBase
	{
	public:
		typedef SFieldTypeDataTyper< EFieldType_DATE >::value_type value_type;

	private:
		value_type _tValue;

	public:

		/** Default constructor.
		*/
		CDatabaseValue( DatabaseConnectionPtr connection )
			: CDatabaseValueBase( connection )
			, _tValue( value_type() )
		{
			// Empty
		}

		/** Destructor.
		*/
		DatabaseExport virtual ~CDatabaseValue()
		{
			Reset();
		}

		/** Defines this value from the given one
		@param value
		    The value
		@param limits
		    The field size limit
		*/
		DatabaseExport virtual void SetValue( CDatabaseValueBase * value )
		{
			SetValue( static_cast< CDatabaseValue< EFieldType_DATE > * >( value )->_tValue );
		}

		/** Set value.
		@param[in] tValue
		    New value.
		*/
		inline void SetValue( const value_type & tValue )
		{
			CDatabaseValuePolicy< value_type >::Set( tValue, _tValue, _isValueSet, _isValueAsStringSet, _valueSize );
			SetNull( !_isValueSet );
		}

		/** Get the value.
		@return
		    The value.
		*/
		DatabaseExport virtual void GetInsertValue( String & result )
		{
			DoUpdateTValue();
			CDate date;
			CDate::IsDate( _tValue, DATE_FORMAT_EXP, date );
			result += CStrUtils::ToString( _connection->WriteDate( date ) );
		}

		/** Get a pointer to the value.
		@return
		    Pointer to the value or NULL.
		*/
		DatabaseExport virtual void * GetPtrValue()
		{
			DoUpdateTValue();
			return CDatabaseValuePolicy< value_type >::Ptr( _tValue );
		}

		/** Get the value.
		@return
		    The value.
		*/
		DatabaseExport virtual CDate GetValue()
		{
			DoUpdateTValue();
			return _connection->ParseDate( _tValue );
		}

		/** Re-initialize internal values.
		*/
		DatabaseExport virtual void Reset()
		{
			DoReset();
			CDatabaseValuePolicy< value_type >::Reset( _tValue );
		}

	private:

		/** Update value as string from the typed value.
		*/
		DatabaseExport virtual void DoUpdateStrValue()
		{
			CDatabaseValuePolicy< value_type >::ToStr( _tValue, _isValueSet, _value, _isValueAsStringSet );
		}

		/** Update typed value from the value as string.
		*/
		DatabaseExport virtual void DoUpdateTValue()
		{
			CDatabaseValuePolicy< value_type >::FromStr( _value, _isValueAsStringSet, _tValue, _isValueSet, _valueSize );
		}

		/** Set parameter value to NULL.
		*/
		DatabaseExport virtual void DoSetNull()
		{
			_tValue = value_type();
			_valueSize = 0;
		}
	};

	/** Describes a EFieldType_DATETME field value.
	*/
	template<> class CDatabaseValue< EFieldType_DATETIME >
		: public CDatabaseValueBase
	{
	public:
		typedef SFieldTypeDataTyper< EFieldType_DATETIME >::value_type value_type;

	private:
		value_type _tValue;

	public:

		/** Default constructor.
		*/
		CDatabaseValue( DatabaseConnectionPtr connection )
			: CDatabaseValueBase( connection )
			, _tValue( value_type() )
		{
			// Empty
		}

		/** Destructor.
		*/
		DatabaseExport virtual ~CDatabaseValue()
		{
			Reset();
		}

		/** Defines this value from the given one
		@param value
		    The value
		@param limits
		    The field size limit
		*/
		DatabaseExport virtual void SetValue( CDatabaseValueBase * value )
		{
			SetValue( static_cast< CDatabaseValue< EFieldType_DATETIME > * >( value )->_tValue );
		}

		/** Set value.
		@param[in] tValue
		    New value.
		*/
		inline void SetValue( const value_type & tValue )
		{
			CDatabaseValuePolicy< value_type >::Set( tValue, _tValue, _isValueSet, _isValueAsStringSet, _valueSize );
			SetNull( !_isValueSet );
		}

		/** Get the value.
		@return
		    The value.
		*/
		DatabaseExport virtual void GetInsertValue( String & result )
		{
			DoUpdateTValue();
			CDateTime dateTime;
			CDateTime::IsDateTime( _tValue, DATETIME_FORMAT_EXP, dateTime );
			result += CStrUtils::ToString( _connection->WriteDateTime( dateTime ) );
		}

		/** Get a pointer to the value.
		@return
		    Pointer to the value or NULL.
		*/
		DatabaseExport virtual void * GetPtrValue()
		{
			DoUpdateTValue();
			return CDatabaseValuePolicy< value_type >::Ptr( _tValue );
		}

		/** Get the value.
		@return
		    The value.
		*/
		DatabaseExport virtual CDateTime GetValue()
		{
			DoUpdateTValue();
			return _connection->ParseDateTime( _tValue );
		}

		/** Re-initialize internal values.
		*/
		DatabaseExport virtual void Reset()
		{
			DoReset();
			CDatabaseValuePolicy< value_type >::Reset( _tValue );
		}

	private:

		/** Update value as string from the typed value.
		*/
		DatabaseExport virtual void DoUpdateStrValue()
		{
			CDatabaseValuePolicy< value_type >::ToStr( _tValue, _isValueSet, _value, _isValueAsStringSet );
		}

		/** Update typed value from the value as string.
		*/
		DatabaseExport virtual void DoUpdateTValue()
		{
			CDatabaseValuePolicy< value_type >::FromStr( _value, _isValueAsStringSet, _tValue, _isValueSet, _valueSize );
		}

		/** Set parameter value to NULL.
		*/
		DatabaseExport virtual void DoSetNull()
		{
			_tValue = value_type();
			_valueSize = 0;
		}
	};

	/** Describes a EFieldType_TIME field value.
	*/
	template<> class CDatabaseValue< EFieldType_TIME >
		: public CDatabaseValueBase
	{
	public:
		typedef SFieldTypeDataTyper< EFieldType_TIME >::value_type value_type;

	private:
		value_type _tValue;

	public:

		/** Default constructor.
		*/
		CDatabaseValue( DatabaseConnectionPtr connection )
			: CDatabaseValueBase( connection )
			, _tValue( value_type() )
		{
			// Empty
		}

		/** Destructor.
		*/
		DatabaseExport virtual ~CDatabaseValue()
		{
			Reset();
		}

		/** Defines this value from the given one
		@param value
		    The value
		@param limits
		    The field size limit
		*/
		DatabaseExport virtual void SetValue( CDatabaseValueBase * value )
		{
			SetValue( static_cast< CDatabaseValue< EFieldType_TIME > * >( value )->_tValue );
		}

		/** Set value as string.
		@param[in] tValue
		    Field value as string.
		*/
		inline void SetValue( const value_type & tValue )
		{
			CDatabaseValuePolicy< value_type >::Set( tValue, _tValue, _isValueSet, _isValueAsStringSet, _valueSize );
			SetNull( !_isValueSet );
		}

		/** Get the value.
		@return
		    The value.
		*/
		DatabaseExport virtual void GetInsertValue( String & result )
		{
			DoUpdateTValue();
			CTime time;
			CTime::IsTime( _tValue, TIME_FORMAT_EXP, time );
			result += CStrUtils::ToString( _connection->WriteTime( time ) );
		}

		/** Get a pointer to the value.
		@return
		    Pointer to the value or NULL.
		*/
		DatabaseExport virtual void * GetPtrValue()
		{
			DoUpdateTValue();
			return CDatabaseValuePolicy< value_type >::Ptr( _tValue );
		}

		/** Get the value.
		@return
		    The value.
		*/
		DatabaseExport virtual CTime GetValue()
		{
			DoUpdateTValue();
			return _connection->ParseTime( _tValue );
		}

		/** Re-initialize internal values.
		*/
		DatabaseExport virtual void Reset()
		{
			DoReset();
			CDatabaseValuePolicy< value_type >::Reset( _tValue );
		}

	private:

		/** Update value as string from the typed value.
		*/
		DatabaseExport virtual void DoUpdateStrValue()
		{
			CDatabaseValuePolicy< value_type >::ToStr( _tValue, _isValueSet, _value, _isValueAsStringSet );
		}

		/** Update typed value from the value as string.
		*/
		DatabaseExport virtual void DoUpdateTValue()
		{
			CDatabaseValuePolicy< value_type >::FromStr( _value, _isValueAsStringSet, _tValue, _isValueSet, _valueSize );
		}

		/** Set parameter value to NULL.
		*/
		DatabaseExport virtual void DoSetNull()
		{
			_tValue = value_type();
			_valueSize = 0;
		}
	};

	/** Describes a EFieldType_BINARY field value.
	*/
	template<> class CDatabaseValue< EFieldType_BINARY >
		: public CDatabaseValueBase
	{
	public:
		typedef SFieldTypeDataTyper< EFieldType_BINARY >::value_type value_type;

	private:
		value_type _tValue;

	public:

		/** Default constructor.
		*/
		CDatabaseValue( DatabaseConnectionPtr connection )
			: CDatabaseValueBase( connection )
			, _tValue( value_type() )
		{
			// Empty
		}

		/** Destructor.
		*/
		DatabaseExport virtual ~CDatabaseValue()
		{
			Reset();
		}

		/** Defines this value from the given one
		@param value
		    The value
		@param limits
		    The field size limit
		*/
		DatabaseExport virtual void SetValue( CDatabaseValueBase * value )
		{
			SetValue( static_cast< CDatabaseValue< EFieldType_BINARY > * >( value )->_tValue );
		}

		/** Set value.
		@param[in] tValue
		    New value.
		@param[in] size
		    Array size.
		*/
		inline void SetValue( const value_type & tValue )
		{
			CDatabaseValuePolicy< value_type >::Set( tValue, _tValue, _isValueSet, _isValueAsStringSet, _valueSize );
			SetNull( !_isValueSet );
		}

		/** Set value.
		@param[in] tValue
		    New value.
		@param[in] size
		    Array size.
		*/
		inline void SetValue( const uint8_t * tValue, uint32_t size )
		{
			value_type value;

			if ( tValue && size )
			{
				value.insert( value.end(), tValue, tValue + size );
			}

			SetValue( value );
		}

		/** Get the value.
		@return
		    The value.
		*/
		DatabaseExport virtual void GetInsertValue( String & result )
		{
			DoUpdateTValue();
			CDatabaseValuePolicy< value_type >::ToInsertable( _tValue, _isValueSet, _connection, result );
		}

		/** Get a pointer to the value.
		@return
		    Pointer to the value or NULL.
		*/
		DatabaseExport virtual void * GetPtrValue()
		{
			DoUpdateTValue();
			return CDatabaseValuePolicy< value_type >::Ptr( _tValue );
		}

		/** Get the value.
		@return
		    The value.
		*/
		DatabaseExport virtual const value_type & GetValue()
		{
			DoUpdateTValue();
			return _tValue;
		}

		/** Re-initialize internal values.
		*/
		DatabaseExport virtual void Reset()
		{
			DoReset();
			CDatabaseValuePolicy< value_type >::Reset( _tValue );
		}

	private:

		/** Update value as string from the typed value.
		*/
		DatabaseExport virtual void DoUpdateStrValue()
		{
			CDatabaseValuePolicy< value_type >::ToStr( _tValue, _isValueSet, _value, _isValueAsStringSet );
		}

		/** Update typed value from the value as string.
		*/
		DatabaseExport virtual void DoUpdateTValue()
		{
			CDatabaseValuePolicy< value_type >::FromStr( _value, _isValueAsStringSet, _tValue, _isValueSet, _valueSize );
		}

		/** Set parameter value to NULL.
		*/
		DatabaseExport virtual void DoSetNull()
		{
			_tValue = value_type( 0 );
			_valueSize = 0;
		}
	};

	/** Describes a EFieldType_BINARY field value.
	*/
	template<> class CDatabaseValue< EFieldType_VARBINARY >
		: public CDatabaseValueBase
	{
	public:
		typedef SFieldTypeDataTyper< EFieldType_VARBINARY >::value_type value_type;

	private:
		value_type _tValue;

	public:

		/** Default constructor.
		*/
		CDatabaseValue( DatabaseConnectionPtr connection )
			: CDatabaseValueBase( connection )
			, _tValue( value_type() )
		{
			// Empty
		}

		/** Destructor.
		*/
		DatabaseExport virtual ~CDatabaseValue()
		{
			Reset();
		}

		/** Defines this value from the given one
		@param value
		    The value
		@param limits
		    The field size limit
		*/
		DatabaseExport virtual void SetValue( CDatabaseValueBase * value )
		{
			SetValue( static_cast< CDatabaseValue< EFieldType_VARBINARY > * >( value )->_tValue );
		}

		/** Set value.
		@param[in] tValue
		    New value.
		@param[in] size
		    Array size.
		*/
		inline void SetValue( const value_type & tValue )
		{
			CDatabaseValuePolicy< value_type >::Set( tValue, _tValue, _isValueSet, _isValueAsStringSet, _valueSize );
			SetNull( !_isValueSet );
		}

		/** Set value.
		@param[in] tValue
		    New value.
		@param[in] size
		    Array size.
		*/
		inline void SetValue( const uint8_t * tValue, uint32_t size )
		{
			value_type value;

			if ( tValue && size )
			{
				value.insert( value.end(), tValue, tValue + size );
			}

			SetValue( value );
		}

		/** Get the value.
		@return
		    The value.
		*/
		DatabaseExport virtual void GetInsertValue( String & result )
		{
			DoUpdateTValue();
			CDatabaseValuePolicy< value_type >::ToInsertable( _tValue, _isValueSet, _connection, result );
		}

		/** Get a pointer to the value.
		@return
		    Pointer to the value or NULL.
		*/
		DatabaseExport virtual void * GetPtrValue()
		{
			DoUpdateTValue();
			return CDatabaseValuePolicy< value_type >::Ptr( _tValue );
		}

		/** Get the value.
		@return
		    The value.
		*/
		DatabaseExport virtual const value_type & GetValue()
		{
			DoUpdateTValue();
			return _tValue;
		}

		/** Re-initialize internal values.
		*/
		DatabaseExport virtual void Reset()
		{
			DoReset();
			CDatabaseValuePolicy< value_type >::Reset( _tValue );
		}

	private:

		/** Update value as string from the typed value.
		*/
		DatabaseExport virtual void DoUpdateStrValue()
		{
			CDatabaseValuePolicy< value_type >::ToStr( _tValue, _isValueSet, _value, _isValueAsStringSet );
		}

		/** Update typed value from the value as string.
		*/
		DatabaseExport virtual void DoUpdateTValue()
		{
			CDatabaseValuePolicy< value_type >::FromStr( _value, _isValueAsStringSet, _tValue, _isValueSet, _valueSize );
		}

		/** Set parameter value to NULL.
		*/
		DatabaseExport virtual void DoSetNull()
		{
			_tValue = value_type();
			_valueSize = 0;
		}
	};

	/** Describes a EFieldType_BINARY field value.
	*/
	template<> class CDatabaseValue< EFieldType_LONG_VARBINARY >
		: public CDatabaseValueBase
	{
	public:
		typedef SFieldTypeDataTyper< EFieldType_LONG_VARBINARY >::value_type value_type;

	private:
		value_type _tValue;

	public:

		/** Default constructor.
		*/
		CDatabaseValue( DatabaseConnectionPtr connection )
			: CDatabaseValueBase( connection )
			, _tValue( value_type( 0 ) )
		{
			// Empty
		}

		/** Destructor.
		*/
		DatabaseExport virtual ~CDatabaseValue()
		{
			Reset();
		}

		/** Defines this value from the given one
		@param value
		    The value
		@param limits
		    The field size limit
		*/
		DatabaseExport virtual void SetValue( CDatabaseValueBase * value )
		{
			SetValue( static_cast< CDatabaseValue< EFieldType_LONG_VARBINARY > * >( value )->_tValue );
		}

		/** Set value.
		@param[in] tValue
		    New value.
		@param[in] size
		    Array size.
		*/
		inline void SetValue( const value_type & tValue )
		{
			CDatabaseValuePolicy< value_type >::Set( tValue, _tValue, _isValueSet, _isValueAsStringSet, _valueSize );
			SetNull( !_isValueSet );
		}

		/** Set value.
		@param[in] tValue
		    New value.
		@param[in] size
		    Array size.
		*/
		inline void SetValue( const uint8_t * tValue, uint32_t size )
		{
			value_type value;

			if ( tValue && size )
			{
				value.insert( value.end(), tValue, tValue + size );
			}

			SetValue( value );
		}

		/** Get the value.
		@return
		    The value.
		*/
		DatabaseExport virtual void GetInsertValue( String & result )
		{
			DoUpdateTValue();
			CDatabaseValuePolicy< value_type >::ToInsertable( _tValue, _isValueSet, _connection, result );
		}

		/** Get a pointer to the value.
		@return
		    Pointer to the value or NULL.
		*/
		DatabaseExport virtual void * GetPtrValue()
		{
			DoUpdateTValue();
			return CDatabaseValuePolicy< value_type >::Ptr( _tValue );
		}

		/** Get the value.
		@return
		    The value.
		*/
		DatabaseExport virtual const value_type & GetValue()
		{
			DoUpdateTValue();
			return _tValue;
		}

		/** Re-initialize internal values.
		*/
		DatabaseExport virtual void Reset()
		{
			DoReset();
			CDatabaseValuePolicy< value_type >::Reset( _tValue );
		}

	private:
		/** Update value as string from the typed value.
		*/
		DatabaseExport virtual void DoUpdateStrValue()
		{
			CDatabaseValuePolicy< value_type >::ToStr( _tValue, _isValueSet, _value, _isValueAsStringSet );
		}

		/** Update typed value from the value as string.
		*/
		DatabaseExport virtual void DoUpdateTValue()
		{
			CDatabaseValuePolicy< value_type >::FromStr( _value, _isValueAsStringSet, _tValue, _isValueSet, _valueSize );
		}

		/** Set parameter value to NULL.
		*/
		DatabaseExport virtual void DoSetNull()
		{
			_tValue = value_type();
			_valueSize = 0;
		}
	};
}
END_NAMESPACE_DATABASE
