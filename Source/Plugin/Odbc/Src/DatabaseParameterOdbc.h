/************************************************************************//**
* @file DatabaseParameterOdbc.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
* @brief COutOdbcBindBase and COutOdbcBind structures.
*
* @details Describes the base class for a statement or query parameter binding for ODBC.
*
***************************************************************************/

#ifndef ___DATABASE_PARAMETER_ODBC_H___
#define ___DATABASE_PARAMETER_ODBC_H___

#include "DatabaseOdbcPrerequisites.h"

#include "DatabaseOdbcHelper.h"
#include "DatabaseStringUtils.h"

#include <EFieldType.h>
#include <EParameterType.h>

BEGIN_NAMESPACE_DATABASE_ODBC
{
	static const String INFO_ODBC_BindParameter = STR( "SQLBindParameter: " );
	static const String INFO_ODBC_BIND_PARAMETER_NAME = STR( "Parameter: " );
	static const String INFO_ODBC_BIND_PARAMETER_VALUE = STR( "Value: " );

	/** Describes a parameter binding for ODBC.
	*/
	struct COutOdbcBindBase
	{
		/** Constructor.
		@param[in] statement
			The statement.
		@param[in] index
			Internal index.
		@param[in] fieldType
			Field type.
		@param[in] parameterType
			Parameter type.
		@param[in] name
			Parameter name.
		*/
		COutOdbcBindBase( HSTMT statement, uint16_t index, EFieldType fieldType, EParameterType parameterType, const String & name, CDatabaseValueBase & value );

		/** Destructor.
			*/
		virtual ~COutOdbcBindBase();

		/** Initialize this binding.
		@return
		    Error code.
		*/
		EErrorType Initialize();

		/** Binds the value to the statement
		@return
			The error code
		*/
		virtual EErrorType UpdateValue()const = 0;

		/** Sends data at execution of query
		@return
			The error code
		*/
		virtual EErrorType PutData()const;

		/** Retrieves data after execution of query
		@return
			The error code
		*/
		virtual EErrorType GetData()const;

		//! The parameter index
		SQLUSMALLINT _index;
		//! C data type.
		SQLSMALLINT _valueType;
		//! SQL data type.
		SQLSMALLINT _parameterType;
		//! I/O type (in, inout, out)
		SQLSMALLINT _inputOutputType;
		//! Column size.
		SQLULEN _columnSize;
		//! Decimal number of digits
		SQLSMALLINT _decimalDigits;
		//! Column length or indicator
		mutable SQLLEN _columnLenOrInd;
		//! Tells the parameter is nullable
		SQLSMALLINT _nullable;
		//! The parameter data type
		SQLSMALLINT _dataType;
		//! The statement handle
		HSTMT _statement;
		// The parameter name
		const String & _name;
		//! The parameter value
		CDatabaseValueBase & _value;
	};

	/** Template binding class, used to bind data
	*/
	template< EFieldType Type >
	struct COutOdbcBind
		: public COutOdbcBindBase
	{
		/** Constructor.
		@param[in] statement
			The statement.
		@param[in] index
			Internal index.
		@param[in] parameterType
			Parameter type.
		@param[in] value
			Parameter value.
		*/
		COutOdbcBind( HSTMT statement, uint16_t index, EParameterType parameterType, const String & name, CDatabaseValue< Type > & value )
			: COutOdbcBindBase( statement, index, Type, parameterType, name, value )
			, _value( value )
		{
		}

		/** @copydoc Database::Odbc::COutOdbcBindBase::UpdateValue
		*/
		virtual EErrorType UpdateValue()const
		{
			EErrorType errorType = EErrorType_NONE;
			StringStream message;
			message << INFO_ODBC_BIND_PARAMETER_NAME << _name << STR( ", " ) << INFO_ODBC_BIND_PARAMETER_VALUE << STR( "[" ) << _value.GetValue() << STR( "]" );

			if ( _value.IsNull() || _value.GetPtrSize() == 0 )
			{
				_columnLenOrInd = SQL_NULL_DATA;
			}
			else
			{
				_columnLenOrInd = _value.GetPtrSize();
			}

			OdbcCheck( SQLBindParameter( _statement, _index, _inputOutputType, _valueType, _parameterType, _columnSize, 0, _value.GetPtrValue(), _value.GetPtrSize(), &_columnLenOrInd ), SQL_HANDLE_STMT, _statement, INFO_ODBC_BindParameter + message.str() );
			return errorType;
		}

		//! The explicitly typed value
		CDatabaseValue< Type > & _value;
	};

	/** COutOdbcBind specialisation for EFieldType_SINT8
	*/
	template<>
	struct COutOdbcBind< EFieldType_SINT8 >
		: public COutOdbcBindBase
	{
		/** Constructor.
		@param[in] statement
			The statement.
		@param[in] index
			Internal index.
		@param[in] parameterType
			Parameter type.
		@param[in] value
			Parameter value.
		*/
		COutOdbcBind( HSTMT statement, uint16_t index, EParameterType parameterType, const String & name, CDatabaseValue< EFieldType_SINT8 > & value )
			: COutOdbcBindBase( statement, index, EFieldType_SINT8, parameterType, name, value )
			, _value( value )
		{
		}

		/** @copydoc Database::Odbc::COutOdbcBindBase::UpdateValue
		*/
		virtual EErrorType UpdateValue()const
		{
			EErrorType errorType = EErrorType_NONE;
			StringStream message;
			message << INFO_ODBC_BIND_PARAMETER_NAME << _name << STR( ", " ) << INFO_ODBC_BIND_PARAMETER_VALUE << STR( "[" ) << int16_t( _value.GetValue() ) << STR( "]" );

			if ( _value.IsNull() || _value.GetPtrSize() == 0 )
			{
				_columnLenOrInd = SQL_NULL_DATA;
			}
			else
			{
				_columnLenOrInd = _value.GetPtrSize();
			}

			OdbcCheck( SQLBindParameter( _statement, _index, _inputOutputType, _valueType, _parameterType, _columnSize, 0, (uint8_t*)_value.GetPtrValue(), _value.GetPtrSize(), &_columnLenOrInd ), SQL_HANDLE_STMT, _statement, INFO_ODBC_BindParameter + message.str() );
			return errorType;
		}

		//! The explicitly typed value
		CDatabaseValue< EFieldType_SINT8 > & _value;
	};

	/** COutOdbcBind specialisation for EFieldType_UINT8
	*/
	template<>
	struct COutOdbcBind< EFieldType_UINT8 >
		: public COutOdbcBindBase
	{
		/** Constructor.
		@param[in] statement
			The statement.
		@param[in] index
			Internal index.
		@param[in] parameterType
			Parameter type.
		@param[in] value
			Parameter value.
		*/
		COutOdbcBind( HSTMT statement, uint16_t index, EParameterType parameterType, const String & name, CDatabaseValue< EFieldType_UINT8 > & value )
			: COutOdbcBindBase( statement, index, EFieldType_UINT8, parameterType, name, value )
			, _value( value )
		{
		}

		/** @copydoc Database::Odbc::COutOdbcBindBase::UpdateValue
		*/
		virtual EErrorType UpdateValue()const
		{
			EErrorType errorType = EErrorType_NONE;
			StringStream message;
			message << INFO_ODBC_BIND_PARAMETER_NAME << _name << STR( ", " ) << INFO_ODBC_BIND_PARAMETER_VALUE << STR( "[" ) << int16_t( _value.GetValue() ) << STR( "]" );

			if ( _value.IsNull() || _value.GetPtrSize() == 0 )
			{
				_columnLenOrInd = SQL_NULL_DATA;
			}
			else
			{
				_columnLenOrInd = _value.GetPtrSize();
			}

			OdbcCheck( SQLBindParameter( _statement, _index, _inputOutputType, _valueType, _parameterType, _columnSize, 0, _value.GetPtrValue(), _value.GetPtrSize(), &_columnLenOrInd ), SQL_HANDLE_STMT, _statement, INFO_ODBC_BindParameter + message.str() );
			return errorType;
		}

		//! The explicitly typed value
		CDatabaseValue< EFieldType_UINT8 > & _value;
	};

	/** COutOdbcBind specialisation for EFieldType_SINT24
	*/
	template<>
	struct COutOdbcBind< EFieldType_SINT24 >
		: public COutOdbcBindBase
	{
		/** Constructor.
		@param[in] statement
			The statement.
		@param[in] index
			Internal index.
		@param[in] parameterType
			Parameter type.
		@param[in] value
			Parameter value.
		*/
		COutOdbcBind( HSTMT statement, uint16_t index, EParameterType parameterType, const String & name, CDatabaseValue< EFieldType_SINT24 > & value )
			: COutOdbcBindBase( statement, index, EFieldType_SINT24, parameterType, name, value )
			, _value( value )
		{
		}

		/** @copydoc Database::Odbc::COutOdbcBindBase::UpdateValue
		*/
		virtual EErrorType UpdateValue()const
		{
			EErrorType errorType = EErrorType_NONE;
			_holder = int32_t( _value.GetValue() );
			StringStream message;
			message << INFO_ODBC_BIND_PARAMETER_NAME << _name << STR( ", " ) << INFO_ODBC_BIND_PARAMETER_VALUE << STR( "[" ) << _holder << STR( "]" );

			if ( _value.IsNull() || _value.GetPtrSize() == 0 )
			{
				_columnLenOrInd = SQL_NULL_DATA;
			}
			else
			{
				_columnLenOrInd = _value.GetPtrSize();
			}

			OdbcCheck( SQLBindParameter( _statement, _index, _inputOutputType, _valueType, _parameterType, _columnSize, 0, SQLPOINTER( &_holder ), sizeof( int32_t ), &_columnLenOrInd ), SQL_HANDLE_STMT, _statement, INFO_ODBC_BindParameter + message.str() );
			return errorType;
		}

		//! The explicitly typed value
		CDatabaseValue< EFieldType_SINT24 > & _value;
		//! value holder
		mutable int32_t _holder;
	};

	/** COutOdbcBind specialisation for EFieldType_UINT24
	*/
	template<>
	struct COutOdbcBind< EFieldType_UINT24 >
		: public COutOdbcBindBase
	{
		/** Constructor.
		@param[in] statement
			The statement.
		@param[in] index
			Internal index.
		@param[in] parameterType
			Parameter type.
		@param[in] value
			Parameter value.
		*/
		COutOdbcBind( HSTMT statement, uint16_t index, EParameterType parameterType, const String & name, CDatabaseValue< EFieldType_UINT24 > & value )
			: COutOdbcBindBase( statement, index, EFieldType_UINT24, parameterType, name, value )
			, _value( value )
		{
		}

		/** @copydoc Database::Odbc::COutOdbcBindBase::UpdateValue
		*/
		virtual EErrorType UpdateValue()const
		{
			EErrorType errorType = EErrorType_NONE;
			_holder = int32_t( _value.GetValue() );
			StringStream message;
			message << INFO_ODBC_BIND_PARAMETER_NAME << _name << STR( ", " ) << INFO_ODBC_BIND_PARAMETER_VALUE << STR( "[" ) << _holder << STR( "]" );

			if ( _value.IsNull() || _value.GetPtrSize() == 0 )
			{
				_columnLenOrInd = SQL_NULL_DATA;
			}
			else
			{
				_columnLenOrInd = _value.GetPtrSize();
			}

			OdbcCheck( SQLBindParameter( _statement, _index, _inputOutputType, _valueType, _parameterType, _columnSize, 0, SQLPOINTER( &_holder ), sizeof( int32_t ), &_columnLenOrInd ), SQL_HANDLE_STMT, _statement, INFO_ODBC_BindParameter + message.str() );
			return errorType;
		}

		//! The explicitly typed value
		CDatabaseValue< EFieldType_UINT24 > & _value;
		//! value holder
		mutable int32_t _holder;
	};

	/** COutOdbcBind specialisation for EFieldType_FIXED_POINT
	*/
	template<>
	struct COutOdbcBind< EFieldType_FIXED_POINT >
		: public COutOdbcBindBase
	{
		/** Constructor.
		@param[in] statement
			The statement.
		@param[in] index
			Internal index.
		@param[in] parameterType
			Parameter type.
		@param[in] value
			Parameter value.
		*/
		COutOdbcBind( HSTMT statement, uint16_t index, EParameterType parameterType, const String & name, CDatabaseValue< EFieldType_FIXED_POINT > & value )
			: COutOdbcBindBase( statement, index, EFieldType_FIXED_POINT, parameterType, name, value )
			, _value( value )
			, _holder()
		{
		}

		/** @copydoc Database::Odbc::COutOdbcBindBase::UpdateValue
		*/
		virtual EErrorType UpdateValue()const
		{
			EErrorType errorType = EErrorType_NONE;
			StringStream message;
			message << INFO_ODBC_BIND_PARAMETER_NAME << _name << STR( ", " ) << INFO_ODBC_BIND_PARAMETER_VALUE << STR( "[" ) << _value.GetValue().ToString() << STR( "]" );
			auto columnSize = _columnSize;

			if ( _value.IsNull() )
			{
				_columnLenOrInd = SQL_NULL_DATA;
			}
			else
			{
				std::string value = StringUtils::ToStr( _value.GetValue().ToString() );
				strcpy( _holder, value.c_str() );
				_columnLenOrInd = value.size();
				columnSize = SQLULEN( value.size() );
			}

			OdbcCheck( SQLBindParameter( _statement, _index, _inputOutputType, SQL_C_CHAR, _parameterType, columnSize, 0, SQLPOINTER( &_holder ), sizeof( _holder ), &_columnLenOrInd ), SQL_HANDLE_STMT, _statement, INFO_ODBC_BindParameter + message.str() );
			return errorType;
		}

		CDatabaseValue< EFieldType_FIXED_POINT > & _value;
		mutable char _holder[32];
	};

	/** COutOdbcBind specialisation for EFieldType_DATE
	*/
	template<>
	struct COutOdbcBind< EFieldType_DATE >
		: public COutOdbcBindBase
	{
		/** Constructor.
		@param[in] statement
			The statement.
		@param[in] index
			Internal index.
		@param[in] parameterType
			Parameter type.
		@param[in] value
			Parameter value.
		*/
		COutOdbcBind( HSTMT statement, uint16_t index, EParameterType parameterType, const String & name, CDatabaseValue< EFieldType_DATE > & value )
			: COutOdbcBindBase( statement, index, EFieldType_DATE, parameterType, name, value )
			, _value( value )
			, _holder()
		{
		}

		/** @copydoc Database::Odbc::COutOdbcBindBase::UpdateValue
		*/
		virtual EErrorType UpdateValue()const
		{
			EErrorType errorType = EErrorType_NONE;
			StringStream message;
			message << INFO_ODBC_BIND_PARAMETER_NAME << _name << STR( ", " ) << INFO_ODBC_BIND_PARAMETER_VALUE << STR( "[" ) << _value.GetValue() << STR( "]" );

			DateType const & date = _value.GetValue();
			_holder.year = date.year();
			_holder.month = date.month();
			_holder.day = date.day();

			if ( _value.IsNull() || _value.GetPtrSize() == 0 )
			{
				_columnLenOrInd = SQL_NULL_DATA;
			}
			else
			{
				_columnLenOrInd = _value.GetPtrSize();
			}

			OdbcCheck( SQLBindParameter( _statement, _index, _inputOutputType, _valueType, _parameterType, _columnSize, 0, SQLPOINTER( &_holder ), sizeof( _holder ), &_columnLenOrInd ), SQL_HANDLE_STMT, _statement, INFO_ODBC_BindParameter + message.str() );
			return errorType;
		}

		//! The explicitly typed value
		CDatabaseValue< EFieldType_DATE > & _value;
		//! Holds the value sent to the server
		mutable SQL_DATE_STRUCT _holder;
	};

	/** COutOdbcBind specialisation for EFieldType_TIME
	*/
	template<>
	struct COutOdbcBind< EFieldType_TIME >
		: public COutOdbcBindBase
	{
		/** Constructor.
		@param[in] statement
			The statement.
		@param[in] index
			Internal index.
		@param[in] parameterType
			Parameter type.
		@param[in] value
			Parameter value.
		*/
		COutOdbcBind( HSTMT statement, uint16_t index, EParameterType parameterType, const String & name, CDatabaseValue< EFieldType_TIME > & value )
			: COutOdbcBindBase( statement, index, EFieldType_TIME, parameterType, name, value )
			, _value( value )
			, _holder()
		{
		}

		/** @copydoc Database::Odbc::COutOdbcBindBase::UpdateValue
		*/
		virtual EErrorType UpdateValue()const
		{
			EErrorType errorType = EErrorType_NONE;
			StringStream message;
			message << INFO_ODBC_BIND_PARAMETER_NAME << _name << STR( ", " ) << INFO_ODBC_BIND_PARAMETER_VALUE << STR( "[" ) << _value.GetValue() << STR( "]" );

			TimeType const & date = _value.GetValue();
			_holder.hour = date.hours();
			_holder.minute = date.minutes();
			_holder.second = date.seconds();

			if ( _value.IsNull() || _value.GetPtrSize() == 0 )
			{
				_columnLenOrInd = SQL_NULL_DATA;
			}
			else
			{
				_columnLenOrInd = _value.GetPtrSize();
			}

			OdbcCheck( SQLBindParameter( _statement, _index, _inputOutputType, _valueType, _parameterType, _columnSize, 0, SQLPOINTER( &_holder ), sizeof( _holder ), &_columnLenOrInd ), SQL_HANDLE_STMT, _statement, INFO_ODBC_BindParameter + message.str() );
			return errorType;
		}

		//! The explicitly typed value
		CDatabaseValue< EFieldType_TIME > & _value;
		//! Holds the value sent to the server
		mutable SQL_TIME_STRUCT _holder;
	};

	/** COutOdbcBind specialisation for EFieldType_DATETIME
	*/
	template<>
	struct COutOdbcBind< EFieldType_DATETIME >
		: public COutOdbcBindBase
	{
		/** Constructor.
		@param[in] statement
			The statement.
		@param[in] index
			Internal index.
		@param[in] parameterType
			Parameter type.
		@param[in] value
			Parameter value.
		*/
		COutOdbcBind( HSTMT statement, uint16_t index, EParameterType parameterType, const String & name, CDatabaseValue< EFieldType_DATETIME > & value )
			: COutOdbcBindBase( statement, index, EFieldType_DATETIME, parameterType, name, value )
			, _value( value )
			, _holder()
		{
		}

		/** @copydoc Database::Odbc::COutOdbcBindBase::UpdateValue
		*/
		virtual EErrorType UpdateValue()const
		{
			EErrorType errorType = EErrorType_NONE;
			StringStream message;
			message << INFO_ODBC_BIND_PARAMETER_NAME << _name << STR( ", " ) << INFO_ODBC_BIND_PARAMETER_VALUE << STR( "[" ) << _value.GetValue() << STR( "]" );
			auto columnSize = _columnSize;

			DateTimeType const & date = _value.GetValue();
			_holder.year = date.date().year();
			_holder.month = date.date().month();
			_holder.day = date.date().day();
			_holder.hour = date.time_of_day().hours();
			_holder.minute = date.time_of_day().minutes();
			_holder.second = date.time_of_day().seconds();

			if ( _value.IsNull() || _value.GetPtrSize() == 0 )
			{
				_columnLenOrInd = SQL_NULL_DATA;
			}
			else
			{
				_columnLenOrInd = _value.GetPtrSize();
				columnSize = _value.GetPtrSize();
			}

			OdbcCheck( SQLBindParameter( _statement, _index, _inputOutputType, _valueType, _parameterType, columnSize, 0, SQLPOINTER( &_holder ), sizeof( _holder ), &_columnLenOrInd ), SQL_HANDLE_STMT, _statement, INFO_ODBC_BindParameter + message.str() );
			return errorType;
		}

		//! The explicitly typed value
		CDatabaseValue< EFieldType_DATETIME > & _value;
		//! Holds the value sent to the server
		mutable SQL_TIMESTAMP_STRUCT _holder;
	};

	/** COutOdbcBind specialisation for EFieldType_CHAR
	*/
	template<>
	struct COutOdbcBind< EFieldType_CHAR >
		: public COutOdbcBindBase
	{
		/** Constructor.
		@param[in] statement
			The statement.
		@param[in] index
			Internal index.
		@param[in] parameterType
			Parameter type.
		@param[in] value
			Parameter value.
		*/
		COutOdbcBind( HSTMT statement, uint16_t index, EParameterType parameterType, const String & name, CDatabaseValue< EFieldType_CHAR > & value )
			: COutOdbcBindBase( statement, index, EFieldType_CHAR, parameterType, name, value )
			, _value( value )
		{
		}

		/** @copydoc Database::Odbc::COutOdbcBindBase::UpdateValue
		*/
		virtual EErrorType UpdateValue()const
		{
			EErrorType errorType = EErrorType_NONE;
			StringStream message;
			message << INFO_ODBC_BIND_PARAMETER_NAME << _name << STR( ", " ) << INFO_ODBC_BIND_PARAMETER_VALUE << STR( "[" ) << _value.GetValue() << STR( "]" );
			auto columnSize = _columnSize;

			if ( _value.IsNull() )
			{
				_columnLenOrInd = SQL_NULL_DATA;
			}
			else
			{
				_columnLenOrInd = _value.GetPtrSize();
				columnSize = _value.GetPtrSize();
			}

			OdbcCheck( SQLBindParameter( _statement, _index, _inputOutputType, _valueType, _parameterType, columnSize, 0, _value.GetPtrValue(), _value.GetPtrSize(), &_columnLenOrInd ), SQL_HANDLE_STMT, _statement, INFO_ODBC_BindParameter + message.str() );
			return errorType;
		}

		//! The explicitly typed value
		CDatabaseValue< EFieldType_CHAR > & _value;
	};

	/** COutOdbcBind specialisation for EFieldType_NCHAR
	*/
	template<>
	struct COutOdbcBind< EFieldType_NCHAR >
		: public COutOdbcBindBase
	{
		/** Constructor.
		@param[in] statement
			The statement.
		@param[in] index
			Internal index.
		@param[in] parameterType
			Parameter type.
		@param[in] value
			Parameter value.
		*/
		COutOdbcBind( HSTMT statement, uint16_t index, EParameterType parameterType, const String & name, CDatabaseValue< EFieldType_NCHAR > & value )
			: COutOdbcBindBase( statement, index, EFieldType_NCHAR, parameterType, name, value )
			, _value( value )
		{
		}

		/** @copydoc Database::Odbc::COutOdbcBindBase::UpdateValue
		*/
		virtual EErrorType UpdateValue()const
		{
			EErrorType errorType = EErrorType_NONE;
			StringStream message;
			message << INFO_ODBC_BIND_PARAMETER_NAME << _name << STR( ", " ) << INFO_ODBC_BIND_PARAMETER_VALUE << STR( "[" ) << _value.GetPtrValue() << STR( "]" );
			auto columnSize = _columnSize;

			if ( _value.IsNull() )
			{
				_columnLenOrInd = SQL_NULL_DATA;
			}
			else
			{
				_columnLenOrInd = _value.GetPtrSize();
				columnSize = _value.GetPtrSize();
			}

			OdbcCheck( SQLBindParameter( _statement, _index, _inputOutputType, _valueType, _parameterType, columnSize, 0, _value.GetPtrValue(), _value.GetPtrSize(), &_columnLenOrInd ), SQL_HANDLE_STMT, _statement, INFO_ODBC_BindParameter + message.str() );
			return errorType;
		}

		//! The explicitly typed value
		CDatabaseValue< EFieldType_NCHAR > & _value;
	};

	/** COutOdbcBind specialisation for EFieldType_VARCHAR
	*/
	template<>
	struct COutOdbcBind< EFieldType_VARCHAR >
		: public COutOdbcBindBase
	{
		/** Constructor.
		@param[in] statement
			The statement.
		@param[in] index
			Internal index.
		@param[in] parameterType
			Parameter type.
		@param[in] value
			Parameter value.
		*/
		COutOdbcBind( HSTMT statement, uint16_t index, EParameterType parameterType, const String & name, CDatabaseValue< EFieldType_VARCHAR > & value )
			: COutOdbcBindBase( statement, index, EFieldType_VARCHAR, parameterType, name, value )
			, _value( value )
		{
		}

		/** @copydoc Database::Odbc::COutOdbcBindBase::UpdateValue
		*/
		virtual EErrorType UpdateValue()const
		{
			EErrorType errorType = EErrorType_NONE;
			StringStream message;
			message << INFO_ODBC_BIND_PARAMETER_NAME << _name << STR( ", " ) << INFO_ODBC_BIND_PARAMETER_VALUE << STR( "[" ) << _value.GetValue() << STR( "]" );
			auto columnSize = _columnSize;

			if ( _value.IsNull() )
			{
				_columnLenOrInd = SQL_NULL_DATA;
			}
			else
			{
				_columnLenOrInd = _value.GetPtrSize();
				columnSize = _value.GetPtrSize();
			}

			OdbcCheck( SQLBindParameter( _statement, _index, _inputOutputType, _valueType, _parameterType, columnSize, 0, _value.GetPtrValue(), _value.GetPtrSize(), &_columnLenOrInd ), SQL_HANDLE_STMT, _statement, INFO_ODBC_BindParameter + message.str() );
			return errorType;
		}

		//! The explicitly typed value
		CDatabaseValue< EFieldType_VARCHAR > & _value;
	};

	/** COutOdbcBind specialisation for EFieldType_NVARCHAR
	*/
	template<>
	struct COutOdbcBind< EFieldType_NVARCHAR >
		: public COutOdbcBindBase
	{
		/** Constructor.
		@param[in] statement
			The statement.
		@param[in] index
			Internal index.
		@param[in] parameterType
			Parameter type.
		@param[in] value
			Parameter value.
		*/
		COutOdbcBind( HSTMT statement, uint16_t index, EParameterType parameterType, const String & name, CDatabaseValue< EFieldType_NVARCHAR > & value )
			: COutOdbcBindBase( statement, index, EFieldType_NVARCHAR, parameterType, name, value )
			, _value( value )
		{
		}

		/** @copydoc Database::Odbc::COutOdbcBindBase::UpdateValue
		*/
		virtual EErrorType UpdateValue()const
		{
			EErrorType errorType = EErrorType_NONE;
			StringStream message;
			message << INFO_ODBC_BIND_PARAMETER_NAME << _name << STR( ", " ) << INFO_ODBC_BIND_PARAMETER_VALUE << STR( "[" ) << _value.GetPtrValue() << STR( "]" );
			auto columnSize = _columnSize;

			if ( _value.IsNull() )
			{
				_columnLenOrInd = SQL_NULL_DATA;
			}
			else
			{
				_columnLenOrInd = _value.GetPtrSize();
				columnSize = _value.GetPtrSize();
			}

			OdbcCheck( SQLBindParameter( _statement, _index, _inputOutputType, _valueType, _parameterType, columnSize, 0, _value.GetPtrValue(), _value.GetPtrSize(), &_columnLenOrInd ), SQL_HANDLE_STMT, _statement, INFO_ODBC_BindParameter + message.str() );
			return errorType;
		}

		//! The explicitly typed value
		CDatabaseValue< EFieldType_NVARCHAR > & _value;
	};

	/** COutOdbcBind specialisation for EFieldType_TEXT
	@remarks
		Sends data at execution, not at binding
	*/
	template<>
	struct COutOdbcBind< EFieldType_TEXT >
		: public COutOdbcBindBase
	{
		/** Constructor.
		@param[in] statement
			The statement.
		@param[in] index
			Internal index.
		@param[in] parameterType
			Parameter type.
		@param[in] value
			Parameter value.
		*/
		COutOdbcBind( HSTMT statement, uint16_t index, EParameterType parameterType, const String & name, CDatabaseValue< EFieldType_TEXT > & value )
			: COutOdbcBindBase( statement, index, EFieldType_TEXT, parameterType, name, value )
			, _value( value )
		{
		}

		/** @copydoc Database::Odbc::COutOdbcBindBase::UpdateValue
		*/
		virtual EErrorType UpdateValue()const
		{
			EErrorType errorType = EErrorType_NONE;
			StringStream message;
			message << INFO_ODBC_BIND_PARAMETER_NAME << _name << STR( ", " ) << INFO_ODBC_BIND_PARAMETER_VALUE << STR( "[" ) << _value.GetValue() << STR( "]" );
			auto columnSize = _columnSize;

			if ( _value.IsNull() )
			{
				_columnLenOrInd = SQL_NULL_DATA;
			}
			else
			{
				if ( _value.GetPtrSize() == 0 )
				{
					_columnLenOrInd = SQL_NTS;
				}
				else
				{
					_columnLenOrInd = SQL_LEN_DATA_AT_EXEC( static_cast< const long & >( _value.GetPtrSize() ) );
				}

				columnSize = _value.GetPtrSize();
			}

			OdbcCheck( SQLBindParameter( _statement, _index, _inputOutputType, _valueType, _parameterType, columnSize, 0, _value.GetPtrValue(), _value.GetPtrSize(), &_columnLenOrInd ), SQL_HANDLE_STMT, _statement, INFO_ODBC_BindParameter + message.str() );
			return errorType;
		}

		//! The explicitly typed value
		CDatabaseValue< EFieldType_TEXT > & _value;
	};

	/** COutOdbcBind specialisation for EFieldType_NTEXT
	@remarks
		Sends data at execution, not at binding
	*/
	template<>
	struct COutOdbcBind< EFieldType_NTEXT >
		: public COutOdbcBindBase
	{
		/** Constructor.
		@param[in] statement
			The statement.
		@param[in] index
			Internal index.
		@param[in] parameterType
			Parameter type.
		@param[in] value
			Parameter value.
		*/
		COutOdbcBind( HSTMT statement, uint16_t index, EParameterType parameterType, const String & name, CDatabaseValue< EFieldType_NTEXT > & value )
			: COutOdbcBindBase( statement, index, EFieldType_NTEXT, parameterType, name, value )
			, _value( value )
		{
		}

		/** @copydoc Database::Odbc::COutOdbcBindBase::UpdateValue
		*/
		virtual EErrorType UpdateValue()const
		{
			EErrorType errorType = EErrorType_NONE;
			StringStream message;
			message << INFO_ODBC_BIND_PARAMETER_NAME << _name << STR( ", " ) << INFO_ODBC_BIND_PARAMETER_VALUE << STR( "[" ) << _value.GetPtrValue() << STR( "]" );
			auto columnSize = _columnSize;

			if ( _value.IsNull() )
			{
				_columnLenOrInd = SQL_NULL_DATA;
			}
			else
			{
				if ( _value.GetPtrSize() == 0 )
				{
					_columnLenOrInd = SQL_NTS;
				}
				else
				{
					_columnLenOrInd = SQL_LEN_DATA_AT_EXEC( static_cast< const long & >( _value.GetPtrSize() ) );
				}

				columnSize = _value.GetPtrSize();
			}

			OdbcCheck( SQLBindParameter( _statement, _index, _inputOutputType, _valueType, _parameterType, columnSize, 0, _value.GetPtrValue(), _value.GetPtrSize(), &_columnLenOrInd ), SQL_HANDLE_STMT, _statement, INFO_ODBC_BindParameter + message.str() );
			return errorType;
		}

		//! The explicitly typed value
		CDatabaseValue< EFieldType_NTEXT > & _value;
	};

	/** COutOdbcBind specialisation for EFieldType_BINARY
	@remarks
		Sends data at execution, not at binding
	*/
	template<>
	struct COutOdbcBind< EFieldType_BINARY >
		: public COutOdbcBindBase
	{
		/** Constructor.
		@param[in] statement
			The statement.
		@param[in] index
			Internal index.
		@param[in] parameterType
			Parameter type.
		@param[in] value
			Parameter value.
		*/
		COutOdbcBind( HSTMT statement, uint16_t index, EParameterType parameterType, const String & name, CDatabaseValue< EFieldType_BINARY > & value )
			: COutOdbcBindBase( statement, index, EFieldType_BINARY, parameterType, name, value )
			, _value( value )
		{
		}

		/** @copydoc Database::Odbc::COutOdbcBindBase::UpdateValue
		*/
		virtual EErrorType UpdateValue()const
		{
			EErrorType errorType = EErrorType_NONE;
			StringStream message;
			message << INFO_ODBC_BIND_PARAMETER_NAME << _name << STR( ", " ) << INFO_ODBC_BIND_PARAMETER_VALUE << STR( "[" ) << _value.GetValue() << STR( "]" );
			auto columnSize = _columnSize;

			if ( _value.IsNull() || _value.GetPtrSize() == 0 )
			{
				_columnLenOrInd = SQL_NULL_DATA;
			}
			else
			{
				_columnLenOrInd = SQL_LEN_DATA_AT_EXEC( static_cast< const long & >( _value.GetPtrSize() ) );
				columnSize = _value.GetPtrSize();
			}

			OdbcCheck( SQLBindParameter( _statement, _index, _inputOutputType, _valueType, _parameterType, columnSize, 0, _value.GetPtrValue(), _value.GetPtrSize(), &_columnLenOrInd ), SQL_HANDLE_STMT, _statement, INFO_ODBC_BindParameter + message.str() );
			return errorType;
		}

		//! The explicitly typed value
		CDatabaseValue< EFieldType_BINARY > & _value;
	};

	/** COutOdbcBind specialisation for EFieldType_VARBINARY
	@remarks
		Sends data at execution, not at binding
	*/
	template<>
	struct COutOdbcBind< EFieldType_VARBINARY >
		: public COutOdbcBindBase
	{
		/** Constructor.
		@param[in] statement
			The statement.
		@param[in] index
			Internal index.
		@param[in] parameterType
			Parameter type.
		@param[in] value
			Parameter value.
		*/
		COutOdbcBind( HSTMT statement, uint16_t index, EParameterType parameterType, const String & name, CDatabaseValue< EFieldType_VARBINARY > & value )
			: COutOdbcBindBase( statement, index, EFieldType_VARBINARY, parameterType, name, value )
			, _value( value )
		{
		}

		/** @copydoc Database::Odbc::COutOdbcBindBase::UpdateValue
		*/
		virtual EErrorType UpdateValue()const
		{
			EErrorType errorType = EErrorType_NONE;
			StringStream message;
			message << INFO_ODBC_BIND_PARAMETER_NAME << _name << STR( ", " ) << INFO_ODBC_BIND_PARAMETER_VALUE << STR( "[" ) << _value.GetValue() << STR( "]" );
			auto columnSize = _columnSize;

			if ( _value.IsNull() || _value.GetPtrSize() == 0 )
			{
				_columnLenOrInd = SQL_NULL_DATA;
			}
			else
			{
				_columnLenOrInd = SQL_LEN_DATA_AT_EXEC( static_cast< const long & >( _value.GetPtrSize() ) );
				columnSize = _value.GetPtrSize();
			}

			OdbcCheck( SQLBindParameter( _statement, _index, _inputOutputType, _valueType, _parameterType, columnSize, 0, _value.GetPtrValue(), _value.GetPtrSize(), &_columnLenOrInd ), SQL_HANDLE_STMT, _statement, INFO_ODBC_BindParameter + message.str() );
			return errorType;
		}

		//! The explicitly typed value
		CDatabaseValue< EFieldType_VARBINARY > & _value;
	};

	/** COutOdbcBind specialisation for EFieldType_LONG_VARBINARY
	@remarks
		Sends data at execution, not at binding
	*/
	template<>
	struct COutOdbcBind< EFieldType_LONG_VARBINARY >
		: public COutOdbcBindBase
	{
		/** Constructor.
		@param[in] statement
			The statement.
		@param[in] index
			Internal index.
		@param[in] parameterType
			Parameter type.
		@param[in] value
			Parameter value.
		*/
		COutOdbcBind( HSTMT statement, uint16_t index, EParameterType parameterType, const String & name, CDatabaseValue< EFieldType_LONG_VARBINARY > & value )
			: COutOdbcBindBase( statement, index, EFieldType_LONG_VARBINARY, parameterType, name, value )
			, _value( value )
		{
		}

		/** @copydoc Database::Odbc::COutOdbcBindBase::UpdateValue
		*/
		virtual EErrorType UpdateValue()const
		{
			EErrorType errorType = EErrorType_NONE;
			StringStream message;
			message << INFO_ODBC_BIND_PARAMETER_NAME << _name << STR( ", " ) << INFO_ODBC_BIND_PARAMETER_VALUE << STR( "[" ) << _value.GetValue() << STR( "]" );
			auto columnSize = _columnSize;

			if ( _value.IsNull() || _value.GetPtrSize() == 0 )
			{
				_columnLenOrInd = SQL_NULL_DATA;
			}
			else
			{
				_columnLenOrInd = SQL_LEN_DATA_AT_EXEC( static_cast< const long & >( _value.GetPtrSize() ) );
				columnSize = _value.GetPtrSize();
			}

			OdbcCheck( SQLBindParameter( _statement, _index, _inputOutputType, _valueType, _parameterType, columnSize, 0, _value.GetPtrValue(), _value.GetPtrSize(), &_columnLenOrInd ), SQL_HANDLE_STMT, _statement, INFO_ODBC_BindParameter + message.str() );
			return errorType;
		}

		//! The explicitly typed value
		CDatabaseValue< EFieldType_LONG_VARBINARY > & _value;
	};

	/** Function to facilitate the COutOdbcBindBase creation
	@param[in] statement
		The statement.
	@param[in] parameter
		The parameter.
	*/
	template< EFieldType Type >
	std::unique_ptr< COutOdbcBindBase > MakeOutBind( HSTMT statement, const CDatabaseParameter & parameter, CDatabaseValueBase & value )
	{
		return std::make_unique< COutOdbcBind< Type > >( statement, parameter.GetIndex(), parameter.GetParamType(), parameter.GetName(), static_cast< CDatabaseValue< Type > & >( value ) );
	}

	/** Describes a parameter for ODBC.
	*/
	class CDatabaseParameterOdbc
	{
	public:
		/** Constructor
		*/
		CDatabaseParameterOdbc();

		/** Destructor
		*/
		virtual ~CDatabaseParameterOdbc();

		/** Initializes parameter members from the given statement handle
		@param statementHandle
		    The statement handle
		*/
		void Initialize( SQLHSTMT statementHandle, CDatabaseParameter & parameter );

		/** Retrieves the parameter binding
		@return
			The binding
		*/
		const COutOdbcBindBase & GetBinding()const
		{
			return *_binding;
		}

		/** Retrieves the parameter binding
		@return
			The binding
		*/
		COutOdbcBindBase & GetBinding()
		{
			return *_binding;
		}

	private:
		std::unique_ptr< COutOdbcBindBase > _binding;
	};
}
END_NAMESPACE_DATABASE_ODBC

#endif // ___DATABASE_PARAMETER_ODBC_H___
