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

BEGIN_NAMESPACE_DATABASE
{
	/** Structure used to retrieve the data type from the field type
	*/
	template< EFieldType Type > class SFieldTypeDataTyper;

	/** SFieldTypeDataTyper specialization for EFieldType_BIT
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_BIT >
	{
		typedef bool value_type;
	};

	/** SFieldTypeDataTyper specialization for EFieldType_TINY_INTEGER
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_TINY_INTEGER >
	{
		typedef int8_t value_type;
	};

	/** SFieldTypeDataTyper specialization for EFieldType_SMALL_INTEGER
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_SMALL_INTEGER >
	{
		typedef int16_t value_type;
	};

	/** SFieldTypeDataTyper specialization for EFieldType_INTEGER
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_INTEGER >
	{
		typedef int32_t value_type;
	};

	/** SFieldTypeDataTyper specialization for EFieldType_LONG_INTEGER
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_LONG_INTEGER >
	{
		typedef int64_t value_type;
	};

	/** SFieldTypeDataTyper specialization for EFieldType_FLOATING_POINT_SIMPLE
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_FLOATING_POINT_SIMPLE >
	{
		typedef float value_type;
	};

	/** SFieldTypeDataTyper specialization for EFieldType_FLOATING_POINT_DOUBLE
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_FLOATING_POINT_DOUBLE >
	{
		typedef double value_type;
	};

	/** SFieldTypeDataTyper specialization for EFieldType_FIXED_POINT
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_FIXED_POINT >
	{
		typedef CFixedPoint value_type;
	};

	/** SFieldTypeDataTyper specialization for EFieldType_DATE
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_DATE >
	{
		typedef CDate value_type;
	};

	/** SFieldTypeDataTyper specialization for EFieldType_DATETIME
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_DATETIME >
	{
		typedef CDateTime value_type;
	};

	/** SFieldTypeDataTyper specialization for EFieldType_TIME
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_TIME >
	{
		typedef CTime value_type;
	};

	/** SFieldTypeDataTyper specialization for EFieldType_VARCHAR
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_VARCHAR >
	{
		typedef std::string value_type;
	};

	/** SFieldTypeDataTyper specialization for EFieldType_TEXT
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_TEXT >
	{
		typedef std::string value_type;
	};

	/** SFieldTypeDataTyper specialization for EFieldType_NVARCHAR
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_NVARCHAR >
	{
		typedef std::wstring value_type;
	};

	/** SFieldTypeDataTyper specialization for EFieldType_NTEXT
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_NTEXT >
	{
		typedef std::wstring value_type;
	};

	/** SFieldTypeDataTyper specialization for EFieldType_BINARY
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_BINARY >
	{
		typedef ByteArray value_type;
	};

	/** SFieldTypeDataTyper specialization for EFieldType_VARBINARY
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_VARBINARY >
	{
		typedef ByteArray value_type;
	};

	/** SFieldTypeDataTyper specialization for EFieldType_LONG_VARBINARY
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_LONG_VARBINARY >
	{
		typedef ByteArray value_type;
	};

	/** Structure used to retrieve the field type from the data type
	*/
	template < typename T > struct SDataTypeFieldTyper;

	/** SDataTypeFieldTyper specialization for bool
	*/
	template<> struct SDataTypeFieldTyper< bool >
	{
		static const EFieldType Value = EFieldType_BIT;
	};

	/** SDataTypeFieldTyper specialization for int8_t
	*/
	template<> struct SDataTypeFieldTyper< int8_t >
	{
		static const EFieldType Value = EFieldType_TINY_INTEGER;
	};

	/** SDataTypeFieldTyper specialization for int16_t
	*/
	template<> struct SDataTypeFieldTyper< int16_t >
	{
		static const EFieldType Value = EFieldType_SMALL_INTEGER;
	};

	/** SDataTypeFieldTyper specialization for int32_t
	*/
	template<> struct SDataTypeFieldTyper< int32_t >
	{
		static const EFieldType Value = EFieldType_INTEGER;
	};

	/** SDataTypeFieldTyper specialization for int64_t
	*/
	template<> struct SDataTypeFieldTyper< int64_t >
	{
		static const EFieldType Value = EFieldType_LONG_INTEGER;
	};

	/** SDataTypeFieldTyper specialization for float
	*/
	template<> struct SDataTypeFieldTyper< float >
	{
		static const EFieldType Value = EFieldType_FLOATING_POINT_SIMPLE;
	};

	/** SDataTypeFieldTyper specialization for double
	*/
	template<> struct SDataTypeFieldTyper< double >
	{
		static const EFieldType Value = EFieldType_FLOATING_POINT_DOUBLE;
	};

	/** SDataTypeFieldTyper specialization for CFixedPoint
	*/
	template<> struct SDataTypeFieldTyper< CFixedPoint >
	{
		static const EFieldType Value = EFieldType_FIXED_POINT;
	};

	/** SDataTypeFieldTyper specialization for std::array< char, N >
	*/
	template< size_t N > struct SDataTypeFieldTyper< std::array< char, N > >
	{
		static const EFieldType Value = EFieldType_VARCHAR;
	};

	/** SDataTypeFieldTyper specialization for char *
	*/
	template<> struct SDataTypeFieldTyper< char * >
	{
		static const EFieldType Value = EFieldType_VARCHAR;
	};

	/** SDataTypeFieldTyper specialization for std::array< wchar_t, N >
	*/
	template< size_t N > struct SDataTypeFieldTyper< std::array< wchar_t, N > >
	{
		static const EFieldType Value = EFieldType_NVARCHAR;
	};

	/** SDataTypeFieldTyper specialization for wchar_t *
	*/
	template<> struct SDataTypeFieldTyper< wchar_t * >
	{
		static const EFieldType Value = EFieldType_NVARCHAR;
	};

	/** SDataTypeFieldTyper specialization for CDateTime
	*/
	template<> struct SDataTypeFieldTyper< CDateTime >
	{
		static const EFieldType Value = EFieldType_DATETIME;
	};

	/** SDataTypeFieldTyper specialization for CDate
	*/
	template<> struct SDataTypeFieldTyper< CDate >
	{
		static const EFieldType Value = EFieldType_DATE;
	};

	/** SDataTypeFieldTyper specialization for CTime
	*/
	template<> struct SDataTypeFieldTyper< CTime >
	{
		static const EFieldType Value = EFieldType_TIME;
	};

	/** SDataTypeFieldTyper specialization for std::string
	*/
	template<> struct SDataTypeFieldTyper< std::string >
	{
		static const EFieldType Value = EFieldType_TEXT;
	};

	/** SDataTypeFieldTyper specialization for std::wstring
	*/
	template<> struct SDataTypeFieldTyper< std::wstring >
	{
		static const EFieldType Value = EFieldType_NTEXT;
	};

	/** SDataTypeFieldTyper specialization for uint8_t *
	*/
	template<> struct SDataTypeFieldTyper< uint8_t * >
	{
		static const EFieldType Value = EFieldType_VARBINARY;
	};

	/** SDataTypeFieldTyper specialization for ByteArray
	*/
	template<> struct SDataTypeFieldTyper< ByteArray >
	{
		static const EFieldType Value = EFieldType_VARBINARY;
	};

	static const String NULL_VALUE = STR( "NULL" );

	/** Structure used to specialize functions for given data type
	*/
	template< typename T > class CDatabaseValuePolicy
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
				stream.precision( 20 );
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
