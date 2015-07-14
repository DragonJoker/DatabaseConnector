/************************************************************************//**
* @file DatabaseValuePolicy.h
* @author Sylvain Doremus
* @version 1.0
* @date 11/08/2014 14:12:58
*
*
* @brief SFieldTypeDataTyper, SDataTypeFieldTyper and CDatabaseValuePolicy classes
*
* @details Helps specifying value behaviour
*
***************************************************************************/

#ifndef ___DATABASE_VALUE_POLICY_H___
#define ___DATABASE_VALUE_POLICY_H___

#include "DatabasePrerequisites.h" // Help doxygen

#include "DatabaseStringUtils.h"
#include "DatabaseFixedPoint.h"
#include "DatabaseConnection.h"
#include "DatabaseDate.h"
#include "DatabaseDateTime.h"
#include "DatabaseTime.h"
#include "DatabaseFixedPoint.h"
#include "DatabaseInt24.h"

BEGIN_NAMESPACE_DATABASE
{
	/** Structure used to retrieve the data type from the field type
	*/
	template< EFieldType Type > class SFieldTypeDataTyper;

	/** SFieldTypeDataTyper specialization for EFieldType_BIT
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_BIT >
	{
		static const size_t Size = 1;
		typedef bool value_type;
	};

	/** SFieldTypeDataTyper specialization for EFieldType_INT8
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_INT8 >
	{
		static const size_t Size = 8;
		typedef int8_t value_type;
	};

	/** SFieldTypeDataTyper specialization for EFieldType_INT16
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_INT16 >
	{
		static const size_t Size = 16;
		typedef int16_t value_type;
	};

	/** SFieldTypeDataTyper specialization for EFieldType_INT24
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_INT24 >
	{
		static const size_t Size = 24;
		typedef int24_t value_type;
	};

	/** SFieldTypeDataTyper specialization for EFieldType_INT32
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_INT32 >
	{
		static const size_t Size = 32;
		typedef int32_t value_type;
	};

	/** SFieldTypeDataTyper specialization for EFieldType_INT64
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_INT64 >
	{
		static const size_t Size = 64;
		typedef int64_t value_type;
	};

	/** SFieldTypeDataTyper specialization for EFieldType_FLOAT32
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_FLOAT32 >
	{
		static const size_t Size = 32;
		typedef float value_type;
	};

	/** SFieldTypeDataTyper specialization for EFieldType_FLOAT64
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_FLOAT64 >
	{
		static const size_t Size = 64;
		typedef double value_type;
	};

	/** SFieldTypeDataTyper specialization for EFieldType_FIXED_POINT
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_FIXED_POINT >
	{
		static const size_t Size = 64;
		typedef CFixedPoint value_type;
	};

	/** SFieldTypeDataTyper specialization for EFieldType_DATE
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_DATE >
	{
		static const size_t Size = 0;
		typedef CDate value_type;
	};

	/** SFieldTypeDataTyper specialization for EFieldType_DATETIME
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_DATETIME >
	{
		static const size_t Size = 0;
		typedef CDateTime value_type;
	};

	/** SFieldTypeDataTyper specialization for EFieldType_TIME
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_TIME >
	{
		static const size_t Size = 0;
		typedef CTime value_type;
	};

	/** SFieldTypeDataTyper specialization for EFieldType_VARCHAR
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_VARCHAR >
	{
		static const size_t Size = 0;
		typedef std::string value_type;
	};

	/** SFieldTypeDataTyper specialization for EFieldType_TEXT
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_TEXT >
	{
		static const size_t Size = 0;
		typedef std::string value_type;
	};

	/** SFieldTypeDataTyper specialization for EFieldType_NVARCHAR
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_NVARCHAR >
	{
		static const size_t Size = 0;
		typedef std::wstring value_type;
	};

	/** SFieldTypeDataTyper specialization for EFieldType_NTEXT
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_NTEXT >
	{
		static const size_t Size = 0;
		typedef std::wstring value_type;
	};

	/** SFieldTypeDataTyper specialization for EFieldType_BINARY
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_BINARY >
	{
		static const size_t Size = 0;
		typedef ByteArray value_type;
	};

	/** SFieldTypeDataTyper specialization for EFieldType_VARBINARY
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_VARBINARY >
	{
		static const size_t Size = 0;
		typedef ByteArray value_type;
	};

	/** SFieldTypeDataTyper specialization for EFieldType_LONG_VARBINARY
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_LONG_VARBINARY >
	{
		static const size_t Size = 0;
		typedef ByteArray value_type;
	};

	static const String NULL_VALUE = STR( "NULL" );

	/** Structure used to specialize functions for given data type
	*/
	template< typename T, size_t Size > class CDatabaseValuePolicy
	{
	protected:
		typedef T value_type;

	public:
		/** Reinitializes the given value
		@param value
		    The value
		*/
		void Reset( value_type & value )
		{
			value = value_type();
		}

		/** Sets the value to the given one
		@param in
		    The input value
		@param out
		    The output value
		@param size
		    Receives the new value size
		@param connection
		    The connection used to format the value
		*/
		bool Set( const value_type & in, value_type & out, unsigned long & size, DatabaseConnectionPtr connection )
		{
			out = in;
			size = sizeof( value_type );
			return true;
		}

		/** Retrieves a pointer from the given value
		@param value
		    The value
		*/
		void * Ptr( value_type & value )
		{
			return &value;
		}

		/** Retrieves a pointer from the given value
		@param value
		    The value
		*/
		const void * Ptr( const value_type & value )const
		{
			return &value;
		}

		/** Puts the value into the given string
		@param value
		    The value
		@param valSet
		    Tells that the value is set
		@param connection
		    The connection used to format the value
		@param result
		    Receives the insertable value
		*/
		String ToQueryValue( const value_type & value, bool valSet, DatabaseConnectionPtr connection )const
		{
			if ( valSet )
			{
				return CStrUtils::ToString( value );
			}
			else
			{
				return NULL_VALUE;
			}
		}
	};

	/** CDatabaseValuePolicy specialization for int8_t data type
	*/
	template<> class CDatabaseValuePolicy< int8_t >
	{
	protected:
		typedef int8_t value_type;

	public:
		/** Reinitializes the given value
		@param value
		    The value
		*/
		void Reset( value_type & value )
		{
			value = value_type( 0 );
		}

		/** Sets the value to the given one
		@param in
		    The input value
		@param out
		    The output value
		@param size
		    Receives the new value size
		@param connection
		    The connection used to format the value
		*/
		bool Set( const value_type & in, value_type & out, unsigned long & size, DatabaseConnectionPtr connection )
		{
			out = in;
			size = sizeof( value_type );
			return true;
		}

		/** Retrieves a pointer from the given value
		@param value
		    The value
		*/
		void * Ptr( value_type & value )
		{
			return &value;
		}

		/** Retrieves a pointer from the given value
		@param value
		    The value
		*/
		const void * Ptr( const value_type & value )const
		{
			return &value;
		}

		/** Puts the value into the given string
		@param value
		    The value
		@param valSet
		    Tells that the value is set
		@param connection
		    The connection used to format the value
		@param result
		    Receives the insertable value
		*/
		String ToQueryValue( const value_type & value, bool valSet, DatabaseConnectionPtr connection )const
		{
			if ( valSet )
			{
				return CStrUtils::ToString( int( value ) );
			}
			else
			{
				return NULL_VALUE;
			}
		}
	};

	/** Structure used to specialize functions for given data type
	*/
	template<>
	class CDatabaseValuePolicy< int24_t >
	{
	protected:
		typedef uint32_t value_type;

	public:
		/** Reinitializes the given value
		@param value
		    The value
		*/
		void Reset( value_type & value )
		{
			value = value_type();
		}

		/** Sets the value to the given one
		@param in
		    The input value
		@param out
		    The output value
		@param size
		    Receives the new value size
		@param connection
		    The connection used to format the value
		*/
		bool Set( const value_type & in, value_type & out, unsigned long & size, DatabaseConnectionPtr connection )
		{
			out = in;
			size = sizeof( value_type );
			return true;
		}

		/** Retrieves a pointer from the given value
		@param value
		    The value
		*/
		void * Ptr( value_type & value )
		{
			return &value;
		}

		/** Retrieves a pointer from the given value
		@param value
		    The value
		*/
		const void * Ptr( const value_type & value )const
		{
			return &value;
		}

		/** Puts the value into the given string
		@param value
		    The value
		@param valSet
		    Tells that the value is set
		@param connection
		    The connection used to format the value
		@param result
		    Receives the insertable value
		*/
		String ToQueryValue( const value_type & value, bool valSet, DatabaseConnectionPtr connection )const
		{
			if ( valSet )
			{
				return CStrUtils::ToString( value );
			}
			else
			{
				return NULL_VALUE;
			}
		}
	};

	/** CDatabaseValuePolicy specialization for float data type
	*/
	template<> class CDatabaseValuePolicy< float >
	{
	protected:
		typedef float value_type;

	public:
		/** Reinitializes the given value
		@param value
		    The value
		*/
		void Reset( value_type & value )
		{
			value = value_type( 0 );
		}

		/** Sets the value to the given one
		@param in
		    The input value
		@param out
		    The output value
		@param size
		    Receives the new value size
		@param connection
		    The connection used to format the value
		*/
		bool Set( const value_type & in, value_type & out, unsigned long & size, DatabaseConnectionPtr connection )
		{
			out = in;
			size = sizeof( value_type );
			return true;
		}

		/** Retrieves a pointer from the given value
		@param value
		    The value
		*/
		void * Ptr( value_type & value )
		{
			return &value;
		}

		/** Retrieves a pointer from the given value
		@param value
		    The value
		*/
		const void * Ptr( const value_type & value )const
		{
			return &value;
		}

		/** Puts the value into the given string
		@param value
		    The value
		@param valSet
		    Tells that the value is set
		@param connection
		    The connection used to format the value
		@param result
		    Receives the insertable value
		*/
		String ToQueryValue( const value_type & value, bool valSet, DatabaseConnectionPtr connection )const
		{
			if ( valSet )
			{
				StringStream stream;
				stream.precision( 10 );
				stream << value;
				return stream.str();
			}
			else
			{
				return NULL_VALUE;
			}
		}
	};

	/** CDatabaseValuePolicy specialization for double data type
	*/
	template<> class CDatabaseValuePolicy< double >
	{
	protected:
		typedef double value_type;

	public:
		/** Reinitializes the given value
		@param value
		    The value
		*/
		void Reset( value_type & value )
		{
			value = value_type( 0 );
		}

		/** Sets the value to the given one
		@param in
		    The input value
		@param out
		    The output value
		@param size
		    Receives the new value size
		@param connection
		    The connection used to format the value
		*/
		bool Set( const value_type & in, value_type & out, unsigned long & size, DatabaseConnectionPtr connection )
		{
			out = in;
			size = sizeof( value_type );
			return true;
		}

		/** Retrieves a pointer from the given value
		@param value
		    The value
		*/
		void * Ptr( value_type & value )
		{
			return &value;
		}

		/** Retrieves a pointer from the given value
		@param value
		    The value
		*/
		const void * Ptr( const value_type & value )const
		{
			return &value;
		}

		/** Puts the value into the given string
		@param value
		    The value
		@param valSet
		    Tells that the value is set
		@param connection
		    The connection used to format the value
		@param result
		    Receives the insertable value
		*/
		String ToQueryValue( const value_type & value, bool valSet, DatabaseConnectionPtr connection )const
		{
			if ( valSet )
			{
				StringStream stream;
				stream.precision( 30 );
				stream << value;
				return stream.str();
			}
			else
			{
				return NULL_VALUE;
			}
		}
	};

	/** CDatabaseValuePolicy specialization for bool data type
	*/
	template<> class CDatabaseValuePolicy< bool >
	{
	protected:
		typedef bool value_type;

	public:
		/** Reinitializes the given value
		@param value
		    The value
		*/
		void Reset( value_type & value )
		{
			value = value_type();
		}

		/** Sets the value to the given one
		@param in
		    The input value
		@param out
		    The output value
		@param size
		    Receives the new value size
		@param connection
		    The connection used to format the value
		*/
		bool Set( const value_type & in, value_type & out, unsigned long & size, DatabaseConnectionPtr connection )
		{
			out = in;
			size = sizeof( value_type );
			return true;
		}

		/** Retrieves a pointer from the given value
		@param value
		    The value
		*/
		void * Ptr( value_type & value )
		{
			return &value;
		}

		/** Retrieves a pointer from the given value
		@param value
		    The value
		*/
		const void * Ptr( const value_type & value )const
		{
			return &value;
		}

		/** Puts the value into the given string
		@param value
		    The value
		@param valSet
		    Tells that the value is set
		@param connection
		    The connection used to format the value
		@param result
		    Receives the insertable value
		*/
		String ToQueryValue( const value_type & value, bool valSet, DatabaseConnectionPtr connection )const
		{
			if ( valSet )
			{
				return connection->WriteBool( value );
			}
			else
			{
				return NULL_VALUE;
			}
		}
	};

	/** CDatabaseValuePolicy specialization for std::string data type
	*/
	template<> class CDatabaseValuePolicy< std::string >
	{
	protected:
		typedef std::string value_type;

	public:
		/** Reinitializes the given value
		@param value
		    The value
		*/
		void Reset( value_type & value )
		{
			value.clear();
		}

		/** Sets the value to the given one
		@param in
		    The input value
		@param out
		    The output value
		@param size
		    Receives the new value size
		@param connection
		    The connection used to format the value
		*/
		bool Set( const value_type & in, value_type & out, unsigned long & size, DatabaseConnectionPtr connection )
		{
			out = in;
			size = ( unsigned long )( in.size() );
			return true;
		}

		/** Retrieves a pointer from the given value
		@param value
		    The value
		*/
		void * Ptr( value_type & value )
		{
			void * result = NULL;

			if ( !value.empty() )
			{
				result = &value[0];
			}

			return result;
		}

		/** Retrieves a pointer from the given value
		@param value
		    The value
		*/
		const void * Ptr( const value_type & value )const
		{
			void const * result = NULL;

			if ( !value.empty() )
			{
				result = value.data();
			}

			return result;
		}

		/** Puts the value into the given string
		@param value
		    The value
		@param valSet
		    Tells that the value is set
		@param connection
		    The connection used to format the value
		@param result
		    Receives the insertable value
		*/
		String ToQueryValue( const value_type & value, bool valSet, DatabaseConnectionPtr connection )const
		{
			if ( valSet )
			{
				return CStrUtils::ToString( connection->WriteText( value ) );
			}
			else
			{
				return NULL_VALUE;
			}
		}
	};

	/** CDatabaseValuePolicy specialization for std::wstring data type
	*/
	template<> class CDatabaseValuePolicy< std::wstring >
	{
	protected:
		typedef std::wstring value_type;

	public:
		/** Reinitializes the given value
		@param value
		    The value
		*/
		void Reset( value_type & value )
		{
			value.clear();
		}

		/** Sets the value to the given one
		@param in
		    The input value
		@param out
		    The output value
		@param size
		    Receives the new value size
		@param connection
		    The connection used to format the value
		*/
		bool Set( const value_type & in, value_type & out, unsigned long & size, DatabaseConnectionPtr connection )
		{
			out = in;
			size = ( unsigned long )( in.size() ) * sizeof( wchar_t );
			return true;
		}

		/** Retrieves a pointer from the given value
		@param value
		    The value
		*/
		void * Ptr( value_type & value )
		{
			void * result = NULL;

			if ( !value.empty() )
			{
				result = &value[0];
			}

			return result;
		}

		/** Retrieves a pointer from the given value
		@param value
		    The value
		*/
		const void * Ptr( const value_type & value )const
		{
			void const * result = NULL;

			if ( !value.empty() )
			{
				result = value.data();
			}

			return result;
		}

		/** Puts the value into the given string
		@param value
		    The value
		@param valSet
		    Tells that the value is set
		@param connection
		    The connection used to format the value
		@param result
		    Receives the insertable value
		*/
		String ToQueryValue( const value_type & value, bool valSet, DatabaseConnectionPtr connection )const
		{
			if ( valSet )
			{
				return CStrUtils::ToString( connection->WriteNText( value ) );
			}
			else
			{
				return NULL_VALUE;
			}
		}
	};

	/** CDatabaseValuePolicy specialization for ByteArray data type
	*/
	template<> class CDatabaseValuePolicy< ByteArray >
	{
	protected:
		typedef ByteArray value_type;

	public:
		/** Reinitializes the given value
		@param value
		    The value
		*/
		void Reset( value_type & value )
		{
			value.clear();
		}

		/** Sets the value to the given one
		@param in
		    The input value
		@param out
		    The output value
		@param size
		    Receives the new value size
		@param connection
		    The connection used to format the value
		*/
		bool Set( const value_type & in, value_type & out, unsigned long & size, DatabaseConnectionPtr connection )
		{
			out.clear();

			if ( !in.empty() )
			{
				out.reserve( in.size() );
				out.insert( out.end(), in.begin(), in.end() );
				size = ( unsigned long )( in.size() );
			}

			return true;
		}

		/** Retrieves a pointer from the given value
		@param value
		    The value
		*/
		void * Ptr( value_type & value )
		{
			void * result = NULL;

			if ( !value.empty() )
			{
				result = value.data();
			}

			return result;
		}

		/** Retrieves a pointer from the given value
		@param value
		    The value
		*/
		const void * Ptr( const value_type & value )const
		{
			void const * result = NULL;

			if ( !value.empty() )
			{
				result = value.data();
			}

			return result;
		}

		/** Puts the value into the given string
		@param value
		    The value
		@param valSet
		    Tells that the value is set
		@param connection
		    The connection used to format the value
		@param result
		    Receives the insertable value
		*/
		String ToQueryValue( const value_type & value, bool valSet, DatabaseConnectionPtr connection )const
		{
			if ( valSet )
			{
				StringStream stream;
				stream.setf( std::ios::hex, std::ios::basefield );

				for ( value_type::const_iterator it = value.begin(); it != value.end(); ++it )
				{
					stream.width( 2 );
					stream.fill( STR( '0' ) );
					stream << int( *it );
				}

				return STR( "X'" ) + stream.str() + STR( "'" );
			}
			else
			{
				return NULL_VALUE;
			}
		}
	};

	/** CDatabaseValuePolicy specialization for CDate data type
	*/
	template<> class CDatabaseValuePolicy< CDate >
	{
	protected:
		typedef CDate value_type;

	public:
		/** Reinitializes the given value
		@param value
		    The value
		*/
		void Reset( value_type & value )
		{
			value = value_type();
		}

		/** Sets the value to the given one
		@param in
		    The input value
		@param out
		    The output value
		@param size
		    Receives the new value size
		@param connection
		    The connection used to format the value
		*/
		bool Set( const value_type & in, value_type & out, unsigned long & size, DatabaseConnectionPtr connection )
		{
			out = in;
			size = ( unsigned long )( connection->GetStmtDateSize() );
			_value = connection->WriteStmtDateS( out );
			return true;
		}

		/** Retrieves a pointer from the given value
		@param value
		    The value
		*/
		void * Ptr( value_type & value )
		{
			void * result = NULL;

			if ( !_value.empty() )
			{
				result = &_value[0];
			}

			return result;
		}

		/** Retrieves a pointer from the given value
		@param value
		    The value
		*/
		const void * Ptr( const value_type & value )const
		{
			void const * result = NULL;

			if ( !_value.empty() )
			{
				result = _value.data();
			}

			return result;
		}

		/** Puts the value into the given string
		@param value
		    The value
		@param valSet
		    Tells that the value is set
		@param connection
		    The connection used to format the value
		@param result
		    Receives the insertable value
		*/
		String ToQueryValue( const value_type & value, bool valSet, DatabaseConnectionPtr connection )const
		{
			if ( valSet )
			{
				return CStrUtils::ToString( connection->WriteDate( value ) );
			}
			else
			{
				return NULL_VALUE;
			}
		}

	private:
		std::string _value;
	};

	/** CDatabaseValuePolicy specialization for CTime data type
	*/
	template<> class CDatabaseValuePolicy< CTime >
	{
	protected:
		typedef CTime value_type;

	public:
		/** Reinitializes the given value
		@param value
		    The value
		*/
		void Reset( value_type & value )
		{
			value = value_type();
		}

		/** Sets the value to the given one
		@param in
		    The input value
		@param out
		    The output value
		@param size
		    Receives the new value size
		@param connection
		    The connection used to format the value
		*/
		bool Set( const value_type & in, value_type & out, unsigned long & size, DatabaseConnectionPtr connection )
		{
			out = in;
			size = ( unsigned long )( connection->GetStmtTimeSize() );
			_value = connection->WriteStmtTime( out );
			return true;
		}

		/** Retrieves a pointer from the given value
		@param value
		    The value
		*/
		void * Ptr( value_type & value )
		{
			void * result = NULL;

			if ( !_value.empty() )
			{
				result = &_value[0];
			}

			return result;
		}

		/** Retrieves a pointer from the given value
		@param value
		    The value
		*/
		const void * Ptr( const value_type & value )const
		{
			void const * result = NULL;

			if ( !_value.empty() )
			{
				result = _value.data();
			}

			return result;
		}

		/** Puts the value into the given string
		@param value
		    The value
		@param valSet
		    Tells that the value is set
		@param connection
		    The connection used to format the value
		@param result
		    Receives the insertable value
		*/
		String ToQueryValue( const value_type & value, bool valSet, DatabaseConnectionPtr connection )const
		{
			if ( valSet )
			{
				return CStrUtils::ToString( connection->WriteTime( value ) );
			}
			else
			{
				return NULL_VALUE;
			}
		}

	private:
		std::string _value;
	};

	/** CDatabaseValuePolicy specialization for CDateTime data type
	*/
	template<> class CDatabaseValuePolicy< CDateTime >
	{
	protected:
		typedef CDateTime value_type;

	public:
		/** Reinitializes the given value
		@param value
		    The value
		*/
		void Reset( value_type & value )
		{
			value = value_type();
		}

		/** Sets the value to the given one
		@param in
		    The input value
		@param out
		    The output value
		@param size
		    Receives the new value size
		@param connection
		    The connection used to format the value
		*/
		bool Set( const value_type & in, value_type & out, unsigned long & size, DatabaseConnectionPtr connection )
		{
			out = in;
			size = ( unsigned long )( connection->GetStmtDateTimeSize() );
			_value = connection->WriteStmtDateTime( out );
			return true;
		}

		/** Retrieves a pointer from the given value
		@param value
		    The value
		*/
		void * Ptr( value_type & value )
		{
			void * result = NULL;

			if ( !_value.empty() )
			{
				result = &_value[0];
			}

			return result;
		}

		/** Retrieves a pointer from the given value
		@param value
		    The value
		*/
		const void * Ptr( const value_type & value )const
		{
			void const * result = NULL;

			if ( !_value.empty() )
			{
				result = _value.data();
			}

			return result;
		}

		/** Puts the value into the given string
		@param value
		    The value
		@param valSet
		    Tells that the value is set
		@param connection
		    The connection used to format the value
		@param result
		    Receives the insertable value
		*/
		String ToQueryValue( const value_type & value, bool valSet, DatabaseConnectionPtr connection )const
		{
			if ( valSet )
			{
				return CStrUtils::ToString( connection->WriteDateTime( value ) );
			}
			else
			{
				return NULL_VALUE;
			}
		}

	private:
		std::string _value;
	};

	/** CDatabaseValuePolicy specialization for CFixedPoint data type
	*/
	template<> class CDatabaseValuePolicy< CFixedPoint >
	{
	protected:
		typedef CFixedPoint value_type;

	public:
		/** Reinitializes the given value
		@param value
		    The value
		*/
		void Reset( value_type & value )
		{
			value = value_type();
		}

		/** Sets the value to the given one
		@param in
		    The input value
		@param out
		    The output value
		@param size
		    Receives the new value size
		@param connection
		    The connection used to format the value
		*/
		bool Set( const value_type & in, value_type & out, unsigned long & size, DatabaseConnectionPtr connection )
		{
			out = in;
			size = out.GetPrecision();
			return true;
		}

		/** Retrieves a pointer from the given value
		@param value
		    The value
		*/
		void * Ptr( value_type & value )
		{
			return &value;
		}

		/** Retrieves a pointer from the given value
		@param value
		    The value
		*/
		const void * Ptr( const value_type & value )const
		{
			return &value;
		}

		/** Puts the value into the given string
		@param value
		    The value
		@param valSet
		    Tells that the value is set
		@param connection
		    The connection used to format the value
		@param result
		    Receives the insertable value
		*/
		String ToQueryValue( const value_type & value, bool valSet, DatabaseConnectionPtr connection )const
		{
			if ( valSet )
			{
				return value.ToString();
			}
			else
			{
				return NULL_VALUE;
			}
		}
	};
}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_VALUE_POLICY_H___
