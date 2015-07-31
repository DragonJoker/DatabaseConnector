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

	DatabaseConnectionSPtr CDatabaseValueBase::DoGetConnection()
	{
		return _connection.lock();
	}

	/** Describes a value, used in fields and parameters.
	*/
	template< EFieldType FieldType, typename ValuePolicy >
	class CDatabaseValue
		: public CDatabaseValueBase
		, private ValuePolicy
	{
	public:
		typedef typename ValuePolicy::value_type value_type;

	public:
		/** Default constructor.
		*/
		CDatabaseValue( DatabaseConnectionSPtr connection )
			: CDatabaseValueBase( connection )
			, _tValue( value_type() )
		{
		}

		/** Destructor.
		*/
		inline ~CDatabaseValue()
		{
			Reset();
		}

		/** Defines this value from the given one
		@param value
			The value
		@param limits
			The field size limit
		*/
		inline void SetValue( CDatabaseValueBase const & value )
		{
			SetValue( static_cast< CDatabaseValue< FieldType > const & >( value )._tValue );
		}

		/** Set value.
		@param[in] tValue
			New value.
		*/
		inline void SetValue( const value_type & tValue )
		{
			SetNull( !ValuePolicy::Set( tValue, _tValue, _valueSize, *DoGetConnection() ) );
		}

		/** Get the value.
		@return
			The value.
		*/
		inline String GetQueryValue()
		{
			return ValuePolicy::ToQueryValue( _tValue, !IsNull(), *DoGetConnection() );
		}

		/** Get a pointer to the value.
		@return
			Pointer to the value or NULL.
		*/
		inline void * GetPtrValue()
		{
			return ValuePolicy::Ptr( _tValue );
		}

		/** Get a pointer to the value.
		@return
			Pointer to the value or NULL.
		*/
		inline const void * GetPtrValue()const
		{
			return ValuePolicy::Ptr( _tValue );
		}

		/** Get the value.
		@return
			The value.
		*/
		inline const value_type & GetValue()const
		{
			return _tValue;
		}

		/** Re-initialize internal values.
		*/
		inline void Reset()
		{
			ValuePolicy::Reset( _tValue );
		}

	private:
		/** Set parameter value to NULL.
		*/
		inline void DoSetNull()
		{
			_tValue = value_type();
			_valueSize = 0;
		}

	private:
		//! The value
		value_type _tValue;
	};

	/** Describes a EFieldType_CHAR field value.
	*/
	template< typename ValuePolicy >
	class CDatabaseValue< EFieldType_CHAR, ValuePolicy >
		: public CDatabaseValueBase
		, private ValuePolicy
	{
	public:
		typedef typename ValuePolicy::value_type value_type;

	public:
		/** Default constructor.
		*/
		CDatabaseValue( DatabaseConnectionSPtr connection )
			: CDatabaseValueBase( connection )
			, _tValue( value_type() )
		{
			// Empty
		}

		/** Destructor.
		*/
		inline ~CDatabaseValue()
		{
			Reset();
		}

		/** Defines this value from the given one
		@param value
			The value
		@param limits
			The field size limit
		*/
		inline void SetValue( CDatabaseValueBase const & value )
		{
			CDatabaseValue< EFieldType_CHAR > const & val = static_cast< CDatabaseValue< EFieldType_CHAR > const & >( value );
			SetValue( val._tValue.c_str(), val.GetPtrSize() );
		}

		/** Set value.
		@param[in] tValue
			New value.
		@param[in] limits
			Parameter limits.
		*/
		inline void SetValue( const value_type & tValue )
		{
			SetNull( !ValuePolicy::Set( tValue, _tValue, _valueSize, *DoGetConnection() ) );
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
				size_t length = strlen( tValue );

				if ( length < limits )
				{
					std::stringstream stream;
					stream.width( limits );
					stream.fill( ' ' );
					stream << std::left << tValue;
					value = stream.str();
				}
				else
				{
					value.assign( tValue, tValue + limits );
				}
			}

			SetValue( value );
		}

		/** Get the value.
		@return
			The value.
		*/
		inline String GetQueryValue()
		{
			return ValuePolicy::ToQueryValue( _tValue, !IsNull(), *DoGetConnection() );
		}

		/** Get a pointer to the value.
		@return
			Pointer to the value or NULL.
		*/
		inline void * GetPtrValue()
		{
			return ValuePolicy::Ptr( _tValue );
		}

		/** Get a pointer to the value.
		@return
			Pointer to the value or NULL.
		*/
		inline const void * GetPtrValue()const
		{
			return ValuePolicy::Ptr( _tValue );
		}

		/** Get the value.
		@return
			The value.
		*/
		inline const value_type & GetValue()const
		{
			return _tValue;
		}

		/** Re-initialize internal values.
		*/
		inline void Reset()
		{
			ValuePolicy::Reset( _tValue );
		}

	private:
		/** Set parameter value to NULL.
		*/
		inline void DoSetNull()
		{
			_tValue = value_type();
			_valueSize = 0;
		}

	private:
		//! The value
		value_type _tValue;
	};

	/** Describes a EFieldType_VARCHAR field value.
	*/
	template< typename ValuePolicy >
	class CDatabaseValue< EFieldType_VARCHAR, ValuePolicy >
		: public CDatabaseValueBase
		, private ValuePolicy
	{
	public:
		typedef typename ValuePolicy::value_type value_type;

	public:
		/** Default constructor.
		*/
		CDatabaseValue( DatabaseConnectionSPtr connection )
			: CDatabaseValueBase( connection )
			, _tValue( value_type() )
		{
			// Empty
		}

		/** Destructor.
		*/
		inline ~CDatabaseValue()
		{
			Reset();
		}

		/** Defines this value from the given one
		@param value
			The value
		@param limits
			The field size limit
		*/
		inline void SetValue( CDatabaseValueBase const & value )
		{
			SetValue( static_cast< CDatabaseValue< EFieldType_VARCHAR > const & >( value )._tValue );
		}

		/** Set value.
		@param[in] tValue
			New value.
		@param[in] limits
			Parameter limits.
		*/
		inline void SetValue( const value_type & tValue )
		{
			SetNull( !ValuePolicy::Set( tValue, _tValue, _valueSize, *DoGetConnection() ) );
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
		inline String GetQueryValue()
		{
			return ValuePolicy::ToQueryValue( _tValue, !IsNull(), *DoGetConnection() );
		}

		/** Get a pointer to the value.
		@return
			Pointer to the value or NULL.
		*/
		inline void * GetPtrValue()
		{
			return ValuePolicy::Ptr( _tValue );
		}

		/** Get a pointer to the value.
		@return
			Pointer to the value or NULL.
		*/
		inline const void * GetPtrValue()const
		{
			return ValuePolicy::Ptr( _tValue );
		}

		/** Get the value.
		@return
			The value.
		*/
		inline const value_type & GetValue()const
		{
			return _tValue;
		}

		/** Re-initialize internal values.
		*/
		inline void Reset()
		{
			ValuePolicy::Reset( _tValue );
		}

	private:
		/** Set parameter value to NULL.
		*/
		inline void DoSetNull()
		{
			_tValue = value_type();
			_valueSize = 0;
		}

	private:
		//! The value
		value_type _tValue;
	};

	/** Describes a EFieldType_TEXT field value.
	*/
	template< typename ValuePolicy >
	class CDatabaseValue< EFieldType_TEXT, ValuePolicy >
		: public CDatabaseValueBase
		, private ValuePolicy
	{
	public:
		typedef typename ValuePolicy::value_type value_type;

	public:
		/** Default constructor.
		*/
		CDatabaseValue( DatabaseConnectionSPtr connection )
			: CDatabaseValueBase( connection )
			, _tValue( value_type() )
		{
			// Empty
		}

		/** Destructor.
		*/
		inline ~CDatabaseValue()
		{
			Reset();
		}

		/** Defines this value from the given one
		@param value
			The value
		@param limits
			The field size limit
		*/
		inline void SetValue( CDatabaseValueBase const & value )
		{
			SetValue( static_cast< CDatabaseValue< EFieldType_TEXT > const & >( value )._tValue );
		}

		/** Set value.
		@param[in] tValue
			New value.
		@param[in] limits
			Parameter limits.
		*/
		inline void SetValue( const value_type & tValue )
		{
			SetNull( !ValuePolicy::Set( tValue, _tValue, _valueSize, *DoGetConnection() ) );
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
		inline String GetQueryValue()
		{
			return ValuePolicy::ToQueryValue( _tValue, !IsNull(), *DoGetConnection() );
		}

		/** Get a pointer to the value.
		@return
			Pointer to the value or NULL.
		*/
		inline void * GetPtrValue()
		{
			return ValuePolicy::Ptr( _tValue );
		}

		/** Get a pointer to the value.
		@return
			Pointer to the value or NULL.
		*/
		inline const void * GetPtrValue()const
		{
			return ValuePolicy::Ptr( _tValue );
		}

		/** Get the value.
		@return
			The value.
		*/
		inline const value_type & GetValue()const
		{
			return _tValue;
		}

		/** Re-initialize internal values.
		*/
		inline void Reset()
		{
			ValuePolicy::Reset( _tValue );
		}

	private:
		/** Set parameter value to NULL.
		*/
		inline void DoSetNull()
		{
			_tValue = value_type();
			_valueSize = 0;
		}

	private:
		//! The value
		value_type _tValue;
	};

	/** Describes a EFieldType_NCHAR field value.
	*/
	template< typename ValuePolicy >
	class CDatabaseValue< EFieldType_NCHAR, ValuePolicy >
		: public CDatabaseValueBase
		, private ValuePolicy
	{
	public:
		typedef typename ValuePolicy::value_type value_type;

	public:
		/** Default constructor.
		*/
		CDatabaseValue( DatabaseConnectionSPtr connection )
			: CDatabaseValueBase( connection )
			, _tValue( value_type() )
		{
			// Empty
		}

		/** Destructor.
		*/
		inline ~CDatabaseValue()
		{
			Reset();
		}

		/** Defines this value from the given one
		@param value
			The value
		@param limits
			The field size limit
		*/
		inline void SetValue( CDatabaseValueBase const & value )
		{
			CDatabaseValue< EFieldType_NCHAR > const & val = static_cast< CDatabaseValue< EFieldType_NCHAR > const & >( value );
			SetValue( val._tValue.c_str(), val.GetPtrSize() );
		}

		/** Set value.
		@param[in] tValue
			New value.
		@param[in] limits
			Parameter limits.
		*/
		inline void SetValue( const value_type & tValue )
		{
			SetNull( !ValuePolicy::Set( tValue, _tValue, _valueSize, *DoGetConnection() ) );
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
				size_t length = wcslen( tValue );

				if ( length < limits )
				{
					std::wstringstream stream;
					stream.width( limits );
					stream.fill( L' ' );
					stream << std::left << tValue;
					value = stream.str();
				}
				else
				{
					value.assign( tValue, tValue + limits );
				}
			}

			SetValue( value );
		}

		/** Get the value.
		@return
			The value.
		*/
		inline String GetQueryValue()
		{
			return ValuePolicy::ToQueryValue( _tValue, !IsNull(), *DoGetConnection() );
		}

		/** Get a pointer to the value.
		@return
			Pointer to the value or NULL.
		*/
		inline void * GetPtrValue()
		{
			return ValuePolicy::Ptr( _tValue );
		}

		/** Get a pointer to the value.
		@return
			Pointer to the value or NULL.
		*/
		inline const void * GetPtrValue()const
		{
			return ValuePolicy::Ptr( _tValue );
		}

		/** Get the value.
		@return
			The value.
		*/
		inline const value_type & GetValue()const
		{
			return _tValue;
		}

		/** Re-initialize internal values.
		*/
		inline void Reset()
		{
			ValuePolicy::Reset( _tValue );
		}

	private:
		/** Set parameter value to NULL.
		*/
		inline void DoSetNull()
		{
			_tValue = value_type();
			_valueSize = 0;
		}

	private:
		//! The value
		value_type _tValue;
	};

	/** Describes a EFieldType_NVARCHAR field value.
	*/
	template< typename ValuePolicy >
	class CDatabaseValue< EFieldType_NVARCHAR, ValuePolicy >
		: public CDatabaseValueBase
		, private ValuePolicy
	{
	public:
		typedef typename ValuePolicy::value_type value_type;

	public:
		/** Default constructor.
		*/
		CDatabaseValue( DatabaseConnectionSPtr connection )
			: CDatabaseValueBase( connection )
			, _tValue( value_type() )
		{
			// Empty
		}

		/** Destructor.
		*/
		inline ~CDatabaseValue()
		{
			Reset();
		}

		/** Defines this value from the given one
		@param value
			The value
		@param limits
			The field size limit
		*/
		inline void SetValue( CDatabaseValueBase const & value )
		{
			SetValue( static_cast< CDatabaseValue< EFieldType_NVARCHAR > const & >( value )._tValue );
		}

		/** Set value.
		@param[in] tValue
			New value.
		@param[in] limits
			Parameter limits.
		*/
		inline void SetValue( const value_type & tValue )
		{
			SetNull( !ValuePolicy::Set( tValue, _tValue, _valueSize, *DoGetConnection() ) );
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
		inline String GetQueryValue()
		{
			return ValuePolicy::ToQueryValue( _tValue, !IsNull(), *DoGetConnection() );
		}

		/** Get a pointer to the value.
		@return
			Pointer to the value or NULL.
		*/
		inline void * GetPtrValue()
		{
			return ValuePolicy::Ptr( _tValue );
		}

		/** Get a pointer to the value.
		@return
			Pointer to the value or NULL.
		*/
		inline const void * GetPtrValue()const
		{
			return ValuePolicy::Ptr( _tValue );
		}

		/** Get the value.
		@return
			The value.
		*/
		inline const value_type & GetValue()const
		{
			return _tValue;
		}

		/** Re-initialize internal values.
		*/
		inline void Reset()
		{
			ValuePolicy::Reset( _tValue );
		}

	private:
		/** Set parameter value to NULL.
		*/
		inline void DoSetNull()
		{
			_tValue = value_type();
			_valueSize = 0;
		}

	private:
		//! The value
		value_type _tValue;
	};

	/** Describes a EFieldType_NVARCHAR field value.
	*/
	template< typename ValuePolicy >
	class CDatabaseValue< EFieldType_NTEXT, ValuePolicy >
		: public CDatabaseValueBase
		, private ValuePolicy
	{
	public:
		typedef typename ValuePolicy::value_type value_type;

	public:
		/** Default constructor.
		*/
		CDatabaseValue( DatabaseConnectionSPtr connection )
			: CDatabaseValueBase( connection )
			, _tValue( value_type() )
		{
			// Empty
		}

		/** Destructor.
		*/
		inline ~CDatabaseValue()
		{
			Reset();
		}

		/** Defines this value from the given one
		@param value
			The value
		@param limits
			The field size limit
		*/
		inline void SetValue( CDatabaseValueBase const & value )
		{
			SetValue( static_cast< CDatabaseValue< EFieldType_NTEXT > const & >( value )._tValue );
		}

		/** Set value.
		@param[in] tValue
			New value.
		@param[in] limits
			Parameter limits.
		*/
		inline void SetValue( const value_type & tValue )
		{
			SetNull( !ValuePolicy::Set( tValue, _tValue, _valueSize, *DoGetConnection() ) );
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
		inline String GetQueryValue()
		{
			return ValuePolicy::ToQueryValue( _tValue, !IsNull(), *DoGetConnection() );
		}

		/** Get a pointer to the value.
		@return
			Pointer to the value or NULL.
		*/
		inline void * GetPtrValue()
		{
			return ValuePolicy::Ptr( _tValue );
		}

		/** Get a pointer to the value.
		@return
			Pointer to the value or NULL.
		*/
		inline const void * GetPtrValue()const
		{
			return ValuePolicy::Ptr( _tValue );
		}

		/** Get the value.
		@return
			The value.
		*/
		inline const value_type & GetValue()const
		{
			return _tValue;
		}

		/** Re-initialize internal values.
		*/
		inline void Reset()
		{
			ValuePolicy::Reset( _tValue );
		}

	private:
		/** Set parameter value to NULL.
		*/
		inline void DoSetNull()
		{
			_tValue = value_type();
			_valueSize = 0;
		}

	private:
		//! The value
		value_type _tValue;
	};

	/** Describes a EFieldType_BINARY field value.
	*/
	template< typename ValuePolicy >
	class CDatabaseValue< EFieldType_BINARY, ValuePolicy >
		: public CDatabaseValueBase
		, private ValuePolicy
	{
	public:
		typedef typename ValuePolicy::value_type value_type;

	public:
		/** Default constructor.
		*/
		CDatabaseValue( DatabaseConnectionSPtr connection )
			: CDatabaseValueBase( connection )
			, _tValue( value_type() )
		{
			// Empty
		}

		/** Destructor.
		*/
		inline ~CDatabaseValue()
		{
			Reset();
		}

		/** Defines this value from the given one
		@param value
			The value
		@param limits
			The field size limit
		*/
		inline void SetValue( CDatabaseValueBase const & value )
		{
			SetValue( static_cast< CDatabaseValue< EFieldType_BINARY > const & >( value )._tValue );
		}

		/** Set value.
		@param[in] tValue
			New value.
		@param[in] size
			Array size.
		*/
		inline void SetValue( const value_type & tValue )
		{
			SetNull( !ValuePolicy::Set( tValue, _tValue, _valueSize, *DoGetConnection() ) );
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
		inline String GetQueryValue()
		{
			return ValuePolicy::ToQueryValue( _tValue, !IsNull(), *DoGetConnection() );
		}

		/** Get a pointer to the value.
		@return
			Pointer to the value or NULL.
		*/
		inline void * GetPtrValue()
		{
			return ValuePolicy::Ptr( _tValue );
		}

		/** Get a pointer to the value.
		@return
			Pointer to the value or NULL.
		*/
		inline const void * GetPtrValue()const
		{
			return ValuePolicy::Ptr( _tValue );
		}

		/** Get the value.
		@return
			The value.
		*/
		inline const value_type & GetValue()const
		{
			return _tValue;
		}

		/** Re-initialize internal values.
		*/
		inline void Reset()
		{
			ValuePolicy::Reset( _tValue );
		}

	private:
		/** Set parameter value to NULL.
		*/
		inline void DoSetNull()
		{
			_tValue = value_type( 0 );
			_valueSize = 0;
		}

	private:
		//! The value
		value_type _tValue;
	};

	/** Describes a EFieldType_VARBINARY field value.
	*/
	template< typename ValuePolicy >
	class CDatabaseValue< EFieldType_VARBINARY, ValuePolicy >
		: public CDatabaseValueBase
		, private ValuePolicy
	{
	public:
		typedef typename ValuePolicy::value_type value_type;

	public:
		/** Default constructor.
		*/
		CDatabaseValue( DatabaseConnectionSPtr connection )
			: CDatabaseValueBase( connection )
			, _tValue( value_type() )
		{
			// Empty
		}

		/** Destructor.
		*/
		inline ~CDatabaseValue()
		{
			Reset();
		}

		/** Defines this value from the given one
		@param value
			The value
		@param limits
			The field size limit
		*/
		inline void SetValue( CDatabaseValueBase const & value )
		{
			SetValue( static_cast< CDatabaseValue< EFieldType_VARBINARY > const & >( value )._tValue );
		}

		/** Set value.
		@param[in] tValue
			New value.
		@param[in] size
			Array size.
		*/
		inline void SetValue( const value_type & tValue )
		{
			SetNull( !ValuePolicy::Set( tValue, _tValue, _valueSize, *DoGetConnection() ) );
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
		inline String GetQueryValue()
		{
			return ValuePolicy::ToQueryValue( _tValue, !IsNull(), *DoGetConnection() );
		}

		/** Get a pointer to the value.
		@return
			Pointer to the value or NULL.
		*/
		inline void * GetPtrValue()
		{
			return ValuePolicy::Ptr( _tValue );
		}

		/** Get a pointer to the value.
		@return
			Pointer to the value or NULL.
		*/
		inline const void * GetPtrValue()const
		{
			return ValuePolicy::Ptr( _tValue );
		}

		/** Get the value.
		@return
			The value.
		*/
		inline const value_type & GetValue()const
		{
			return _tValue;
		}

		/** Re-initialize internal values.
		*/
		inline void Reset()
		{
			ValuePolicy::Reset( _tValue );
		}

	private:
		/** Set parameter value to NULL.
		*/
		inline void DoSetNull()
		{
			_tValue = value_type();
			_valueSize = 0;
		}

	private:
		//! The value
		value_type _tValue;
	};

	/** Describes a EFieldType_LONG_VARBINARY field value.
	*/
	template< typename ValuePolicy >
	class CDatabaseValue< EFieldType_LONG_VARBINARY, ValuePolicy >
		: public CDatabaseValueBase
		, private ValuePolicy
	{
	public:
		typedef typename ValuePolicy::value_type value_type;

	public:
		/** Default constructor.
		*/
		CDatabaseValue( DatabaseConnectionSPtr connection )
			: CDatabaseValueBase( connection )
			, _tValue( value_type( 0 ) )
		{
			// Empty
		}

		/** Destructor.
		*/
		inline ~CDatabaseValue()
		{
			Reset();
		}

		/** Defines this value from the given one
		@param value
			The value
		@param limits
			The field size limit
		*/
		inline void SetValue( CDatabaseValueBase const & value )
		{
			SetValue( static_cast< CDatabaseValue< EFieldType_LONG_VARBINARY > const & >( value )._tValue );
		}

		/** Set value.
		@param[in] tValue
			New value.
		@param[in] size
			Array size.
		*/
		inline void SetValue( const value_type & tValue )
		{
			SetNull( !ValuePolicy::Set( tValue, _tValue, _valueSize, *DoGetConnection() ) );
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
		inline String GetQueryValue()
		{
			return ValuePolicy::ToQueryValue( _tValue, !IsNull(), *DoGetConnection() );
		}

		/** Get a pointer to the value.
		@return
			Pointer to the value or NULL.
		*/
		inline void * GetPtrValue()
		{
			return ValuePolicy::Ptr( _tValue );
		}

		/** Get a pointer to the value.
		@return
			Pointer to the value or NULL.
		*/
		inline const void * GetPtrValue()const
		{
			return ValuePolicy::Ptr( _tValue );
		}

		/** Get the value.
		@return
			The value.
		*/
		inline const value_type & GetValue()const
		{
			return _tValue;
		}

		/** Re-initialize internal values.
		*/
		inline void Reset()
		{
			ValuePolicy::Reset( _tValue );
		}

	private:
		/** Set parameter value to NULL.
		*/
		inline void DoSetNull()
		{
			_tValue = value_type();
			_valueSize = 0;
		}

	private:
		//! The value
		value_type _tValue;
	};
}
END_NAMESPACE_DATABASE
