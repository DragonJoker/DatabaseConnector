/************************************************************************//**
 * @file DatabaseValuePolicy.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 11/08/2014 14:12:58
 *
 *
 * @brief [your brief comment here]
 *
 * @details [your detailled comment here]
 *
 ***************************************************************************/

#ifndef ___DATABASE_VALUE_POLICY_H___
#define ___DATABASE_VALUE_POLICY_H___

#include "DatabasePrerequisites.h" // Help doxygen

#include "DatabaseStringUtils.h"

BEGIN_NAMESPACE_DATABASE
{
	/** Structure used to retrieve the data type from the field type
	*/
	template< EFieldType Type > class SFieldTypeDataTyper;

	/** Specialization for EFieldType_BOOL
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_BOOL >
	{
		typedef bool value_type;
	};

	/** Specialization for EFieldType_SMALL_INTEGER
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_SMALL_INTEGER >
	{
		typedef int16_t value_type;
	};

	/** Specialization for EFieldType_INTEGER
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_INTEGER >
	{
		typedef int32_t value_type;
	};

	/** Specialization for EFieldType_LONG_INTEGER
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_LONG_INTEGER >
	{
		typedef int64_t value_type;
	};

	/** Specialization for EFieldType_FLOAT
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_FLOAT >
	{
		typedef float value_type;
	};

	/** Specialization for EFieldType_DOUBLE
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_DOUBLE >
	{
		typedef double value_type;
	};

	/** Specialization for EFieldType_DATE
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_DATE >
	{
		typedef std::string value_type;
	};

	/** Specialization for EFieldType_DATETIME
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_DATETIME >
	{
		typedef std::string value_type;
	};

	/** Specialization for EFieldType_TIME
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_TIME >
	{
		typedef std::string value_type;
	};

	/** Specialization for EFieldType_VARCHAR
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_VARCHAR >
	{
		typedef std::string value_type;
	};

	/** Specialization for EFieldType_TEXT
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_TEXT >
	{
		typedef std::string value_type;
	};

	/** Specialization for EFieldType_NVARCHAR
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_NVARCHAR >
	{
		typedef std::wstring value_type;
	};

	/** Specialization for EFieldType_NTEXT
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_NTEXT >
	{
		typedef std::wstring value_type;
	};

	/** Specialization for EFieldType_BINARY
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_BINARY >
	{
		typedef std::vector< uint8_t > value_type;
	};

	/** Specialization for EFieldType_VARBINARY
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_VARBINARY >
	{
		typedef std::vector< uint8_t > value_type;
	};

	/** Specialization for EFieldType_LONG_VARBINARY
	*/
	template<> struct SFieldTypeDataTyper< EFieldType_LONG_VARBINARY >
	{
		typedef std::vector< uint8_t > value_type;
	};

	/** Structure used to retrieve the field type from the data type
	*/
	template < typename T > struct SDataTypeFieldTyper;

	/** Specialization for bool
	*/
	template<> struct SDataTypeFieldTyper< bool >
	{
		static const EFieldType Value = EFieldType_BOOL;
	};

	/** Specialization for int16_t
	*/
	template<> struct SDataTypeFieldTyper< int16_t >
	{
		static const EFieldType Value = EFieldType_SMALL_INTEGER;
	};

	/** Specialization for int32_t
	*/
	template<> struct SDataTypeFieldTyper< int32_t >
	{
		static const EFieldType Value = EFieldType_INTEGER;
	};

	/** Specialization for int64_t
	*/
	template<> struct SDataTypeFieldTyper< int64_t >
	{
		static const EFieldType Value = EFieldType_LONG_INTEGER;
	};

	/** Specialization for float
	*/
	template<> struct SDataTypeFieldTyper< float >
	{
		static const EFieldType Value = EFieldType_FLOAT;
	};

	/** Specialization for double
	*/
	template<> struct SDataTypeFieldTyper< double >
	{
		static const EFieldType Value = EFieldType_DOUBLE;
	};

	/** Specialization for std::array< char, N >
	*/
	template< size_t N > struct SDataTypeFieldTyper< std::array< char, N > >
	{
		static const EFieldType Value = EFieldType_VARCHAR;
	};

	/** Specialization for char *
	*/
	template<> struct SDataTypeFieldTyper< char * >
	{
		static const EFieldType Value = EFieldType_VARCHAR;
	};

	/** Specialization for std::array< wchar_t, N >
	*/
	template< size_t N > struct SDataTypeFieldTyper< std::array< wchar_t, N > >
	{
		static const EFieldType Value = EFieldType_NVARCHAR;
	};

	/** Specialization for wchar_t *
	*/
	template<> struct SDataTypeFieldTyper< wchar_t * >
	{
		static const EFieldType Value = EFieldType_NVARCHAR;
	};

	/** Specialization for CDateTime
	*/
	template<> struct SDataTypeFieldTyper< CDateTime >
	{
		static const EFieldType Value = EFieldType_DATETIME;
	};

	/** Specialization for CDate
	*/
	template<> struct SDataTypeFieldTyper< CDate >
	{
		static const EFieldType Value = EFieldType_DATE;
	};

	/** Specialization for CTime
	*/
	template<> struct SDataTypeFieldTyper< CTime >
	{
		static const EFieldType Value = EFieldType_TIME;
	};

	/** Specialization for std::string
	*/
	template<> struct SDataTypeFieldTyper< std::string >
	{
		static const EFieldType Value = EFieldType_TEXT;
	};

	/** Specialization for std::wstring
	*/
	template<> struct SDataTypeFieldTyper< std::wstring >
	{
		static const EFieldType Value = EFieldType_NTEXT;
	};

	/** Specialization for uint8_t *
	*/
	template<> struct SDataTypeFieldTyper< uint8_t * >
	{
		static const EFieldType Value = EFieldType_VARBINARY;
	};

	/** Specialization for std::vector< uint8_t >
	*/
	template<> struct SDataTypeFieldTyper< std::vector< uint8_t > >
	{
		static const EFieldType Value = EFieldType_VARBINARY;
	};

	static const String NULL_VALUE = STR( "NULL" );

	/** Structure used to specialize functions for given data type
	*/
	template< typename T > class CDatabaseValuePolicy
	{
	private:
		typedef T value_type;

	public:
		/** Reinitializes the given value
		@param value
		    The value
		*/
		static void Reset( value_type & value )
		{
			value = value_type( 0 );
		}

		/** Sets the value to the given one
		@param in
		    The input value
		@param out
		    The output value
		@param valSet
		    Receives the new setting status for the value
		@param strSet
		    Receives the new setting status for the string value
		@param size
		    Receives the new value size
		*/
		static void Set( const value_type & in, value_type & out, bool & valSet, bool & strSet, long & size )
		{
			strSet = valSet && out == in;
			out = in;
			valSet = true;
			size = sizeof( value_type );
		}

		/** Retrieves a pointer from the given value
		@param value
		    The value
		*/
		static void * Ptr( value_type & value )
		{
			return &value;
		}

		/** Retrieves the value from a string
		@param string
		    The string containing the value
		@param strSet
		    Tells that the string effectively contains a value
		@param value
		    Receives the value
		@param valSet
		    Tells that the value was set, receives the new set status
		@param size
		    The old size, receives the new value size
		*/
		static void FromStr( const String & string, bool strSet, value_type & value, bool & valSet, long & size )
		{
			if ( !valSet )
			{
				if ( strSet )
				{
					StringStream stream( string );
					stream >> value;
					valSet = true;
					size = sizeof( value_type );
				}
				else
				{
					//value = value_type();
					//size = 0;
				}
			}
		}

		/** Puts the value into the given string
		@param value
		    The value
		@param valSet
		    Tells that the value is set
		@param string
		    Receives the string value
		@param strSet
		    Tells that the string was set, receives the new set status
		*/
		static void ToStr( const value_type & value, bool valSet, String & string, bool & strSet )
		{
			if ( !strSet )
			{
				if ( valSet )
				{
					string += CStrUtils::ToString( value );
					strSet = true;
				}
				else
				{
					string.clear();
				}
			}
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
		static String ToInsertable( const value_type & value, bool valSet, DatabaseConnectionPtr connection )
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

	/** Specialization for float data type
	*/
	template<> class CDatabaseValuePolicy< float >
	{
	private:
		typedef float value_type;

	public:
		/** Reinitializes the given value
		@param value
		    The value
		*/
		static void Reset( value_type & value )
		{
			value = value_type( 0 );
		}

		/** Sets the value to the given one
		@param in
		    The input value
		@param out
		    The output value
		@param valSet
		    Receives the new setting status for the value
		@param strSet
		    Receives the new setting status for the string value
		@param size
		    Receives the new value size
		*/
		static void Set( const value_type & in, value_type & out, bool & valSet, bool & strSet, long & size )
		{
			strSet = valSet && out == in;
			out = in;
			valSet = true;
			size = sizeof( value_type );
		}

		/** Retrieves a pointer from the given value
		@param value
		    The value
		*/
		static void * Ptr( value_type & value )
		{
			return &value;
		}

		/** Retrieves the value from a string
		@param string
		    The string containing the value
		@param strSet
		    Tells that the string effectively contains a value
		@param value
		    Receives the value
		@param valSet
		    Tells that the value was set, receives the new set status
		@param size
		    The old size, receives the new value size
		*/
		static void FromStr( const String & string, bool strSet, value_type & value, bool & valSet, long & size )
		{
			if ( !valSet )
			{
				if ( strSet )
				{
					value = DoFromStr( string );
					valSet = true;
					size = sizeof( value_type );
				}
				else
				{
					//value = value_type();
					//size = 0;
				}
			}
		}

		/** Puts the value into the given string
		@param value
		    The value
		@param valSet
		    Tells that the value is set
		@param string
		    Receives the string value
		@param strSet
		    Tells that the string was set, receives the new set status
		*/
		static void ToStr( const value_type & value, bool valSet, String & string, bool & strSet )
		{
			if ( !strSet )
			{
				if ( valSet )
				{
					string += DoToStr( value );
					strSet = true;
				}
				else
				{
					string.clear();
				}
			}
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
		static String ToInsertable( const value_type & value, bool valSet, DatabaseConnectionPtr connection )
		{
			if ( valSet )
			{
				return DoToStr( value );
			}
			else
			{
				return NULL_VALUE;
			}
		}

	private:
		static String DoToStr( const value_type & value )
		{
			StringStream stream;
			stream.precision( 10 );
			stream << value;
			return stream.str();
		}

		static value_type DoFromStr( const String & value )
		{
			value_type result = value_type();
			StringStream stream( value );
			stream.precision( 10 );
			stream >> result;
			return result;
		}
	};

	/** Specialization for double data type
	*/
	template<> class CDatabaseValuePolicy< double >
	{
	private:
		typedef double value_type;

	public:
		/** Reinitializes the given value
		@param value
		    The value
		*/
		static void Reset( value_type & value )
		{
			value = value_type( 0 );
		}

		/** Sets the value to the given one
		@param in
		    The input value
		@param out
		    The output value
		@param valSet
		    Receives the new setting status for the value
		@param strSet
		    Receives the new setting status for the string value
		@param size
		    Receives the new value size
		*/
		static void Set( const value_type & in, value_type & out, bool & valSet, bool & strSet, long & size )
		{
			strSet = valSet && out == in;
			out = in;
			valSet = true;
			size = sizeof( value_type );
		}

		/** Retrieves a pointer from the given value
		@param value
		    The value
		*/
		static void * Ptr( value_type & value )
		{
			return &value;
		}

		/** Retrieves the value from a string
		@param string
		    The string containing the value
		@param strSet
		    Tells that the string effectively contains a value
		@param value
		    Receives the value
		@param valSet
		    Tells that the value was set, receives the new set status
		@param size
		    The old size, receives the new value size
		*/
		static void FromStr( const String & string, bool strSet, value_type & value, bool & valSet, long & size )
		{
			if ( !valSet )
			{
				if ( strSet )
				{
					value = DoFromStr( string );
					valSet = true;
					size = sizeof( value_type );
				}
				else
				{
					//value = value_type();
					//size = 0;
				}
			}
		}

		/** Puts the value into the given string
		@param value
		    The value
		@param valSet
		    Tells that the value is set
		@param string
		    Receives the string value
		@param strSet
		    Tells that the string was set, receives the new set status
		*/
		static void ToStr( const value_type & value, bool valSet, String & string, bool & strSet )
		{
			if ( !strSet )
			{
				if ( valSet )
				{
					string += DoToStr( value );
					strSet = true;
				}
				else
				{
					string.clear();
				}
			}
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
		static String ToInsertable( const value_type & value, bool valSet, DatabaseConnectionPtr connection )
		{
			if ( valSet )
			{
				return DoToStr( value );
			}
			else
			{
				return NULL_VALUE;
			}
		}

	private:
		static String DoToStr( const value_type & value )
		{
			StringStream stream;
			stream.precision( 20 );
			stream << value;
			return stream.str();
		}

		static value_type DoFromStr( const String & value )
		{
			value_type result = value_type();
			StringStream stream( value );
			stream.precision( 20 );
			stream >> result;
			return result;
		}
	};

	/** Specialization for bool data type
	*/
	template<> class CDatabaseValuePolicy< bool >
	{
	private:
		typedef bool value_type;

	public:
		/** Reinitializes the given value
		@param value
		    The value
		*/
		static void Reset( value_type & value )
		{
			value = value_type();
		}

		/** Sets the value to the given one
		@param in
		    The input value
		@param out
		    The output value
		@param valSet
		    Receives the new setting status for the value
		@param strSet
		    Receives the new setting status for the string value
		@param size
		    Receives the new value size
		*/
		static void Set( const value_type & in, value_type & out, bool & valSet, bool & strSet, long & size )
		{
			strSet = valSet && out == in;
			out = in;
			valSet = true;
			size = sizeof( value_type );
		}

		/** Retrieves a pointer from the given value
		@param value
		    The value
		*/
		static void * Ptr( value_type & value )
		{
			return &value;
		}

		/** Retrieves the value from a string
		@param string
		    The string containing the value
		@param strSet
		    Tells that the string effectively contains a value
		@param value
		    Receives the value
		@param valSet
		    Tells that the value was set, receives the new set status
		@param size
		    The old size, receives the new value size
		*/
		static void FromStr( const String & string, bool strSet, value_type & value, bool & valSet, long & size )
		{
			if ( !valSet )
			{
				if ( strSet )
				{
					String strTmp( CStrUtils::LowerCase( string ) );
					value = strTmp == STR( "1" ) || strTmp == STR( "X" ) || strTmp == STR( "true" ) || strTmp == STR( "vrai" );
					valSet = true;
					size = sizeof( value_type );
				}
				else
				{
					//value = value_type();
				}
			}
		}

		/** Puts the value into the given string
		@param value
		    The value
		@param valSet
		    Tells that the value is set
		@param string
		    Receives the string value
		@param strSet
		    Tells that the string was set, receives the new set status
		*/
		static void ToStr( const value_type & value, bool valSet, String & string, bool & strSet )
		{
			if ( !strSet )
			{
				if ( valSet )
				{
					if ( value )
					{
						string = STR( "1" );
					}
					else
					{
						string = STR( "0" );
					}

					strSet = true;
				}
				else
				{
					string.clear();
				}
			}
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
		static String ToInsertable( const value_type & value, bool valSet, DatabaseConnectionPtr connection )
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

	/** Specialization for std::string data type
	*/
	template<> class CDatabaseValuePolicy< std::string >
	{
	private:
		typedef std::string value_type;

	public:
		/** Reinitializes the given value
		@param value
		    The value
		*/
		static void Reset( value_type & value )
		{
			value.clear();
		}

		/** Sets the value to the given one
		@param in
		    The input value
		@param out
		    The output value
		@param valSet
		    Receives the new setting status for the value
		@param strSet
		    Receives the new setting status for the string value
		@param size
		    Receives the new value size
		*/
		static void Set( const value_type & in, value_type & out, bool & valSet, bool & strSet, long & size )
		{
			strSet = valSet && in == out;
			out = in;
			valSet = true;
			size = long( in.size() );
		}

		/** Retrieves a pointer from the given value
		@param value
		    The value
		*/
		static void * Ptr( value_type & value )
		{
			void * result = NULL;

			if ( !value.empty() )
			{
				result = ( void * )value.data();
			}

			return result;
		}

		/** Retrieves the value from a string
		@param string
		    The string containing the value
		@param strSet
		    Tells that the string effectively contains a value
		@param value
		    Receives the value
		@param valSet
		    Tells that the value was set, receives the new set status
		@param size
		    The old size, receives the new value size
		*/
		static void FromStr( const String & string, bool strSet, value_type & value, bool & valSet, long & size )
		{
			if ( !valSet )
			{
				if ( strSet )
				{
					value = CStrUtils::ToStr( string );
					valSet = true;
					size = long( value.size() );
				}
				else
				{
					//value = value_type();
				}
			}
		}

		/** Puts the value into the given string
		@param value
		    The value
		@param valSet
		    Tells that the value is set
		@param string
		    Receives the string value
		@param strSet
		    Tells that the string was set, receives the new set status
		*/
		static void ToStr( const value_type & value, bool valSet, String & string, bool & strSet )
		{
			if ( !strSet )
			{
				if ( valSet )
				{
					string += CStrUtils::ToString( value );
					strSet = true;
				}
				else
				{
					string.clear();
				}
			}
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
		static String ToInsertable( const value_type & value, bool valSet, DatabaseConnectionPtr connection )
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

	/** Specialization for std::wstring data type
	*/
	template<> class CDatabaseValuePolicy< std::wstring >
	{
	private:
		typedef std::wstring value_type;

	public:
		/** Reinitializes the given value
		@param value
		    The value
		*/
		static void Reset( value_type & value )
		{
			value.clear();
		}

		/** Sets the value to the given one
		@param in
		    The input value
		@param out
		    The output value
		@param valSet
		    Receives the new setting status for the value
		@param strSet
		    Receives the new setting status for the string value
		@param size
		    Receives the new value size
		*/
		static void Set( const value_type & in, value_type & out, bool & valSet, bool & strSet, long & size )
		{
			strSet = valSet && in == out;
			out = in;
			valSet = true;
			size = long( in.size() ) * sizeof( wchar_t );
		}

		/** Retrieves a pointer from the given value
		@param value
		    The value
		*/
		static void * Ptr( value_type & value )
		{
			void * result = NULL;

			if ( !value.empty() )
			{
				result = ( void * )value.data();
			}

			return result;
		}

		/** Retrieves the value from a string
		@param string
		    The string containing the value
		@param strSet
		    Tells that the string effectively contains a value
		@param value
		    Receives the value
		@param valSet
		    Tells that the value was set, receives the new set status
		@param size
		    The old size, receives the new value size
		*/
		static void FromStr( const String & string, bool strSet, value_type & value, bool & valSet, long & size )
		{
			if ( !valSet )
			{
				if ( strSet )
				{
					value = CStrUtils::ToWStr( string );
					valSet = true;
					size = long( value.size() ) * sizeof( wchar_t );
				}
				else
				{
					//value = value_type();
				}
			}
		}

		/** Puts the value into the given string
		@param value
		    The value
		@param valSet
		    Tells that the value is set
		@param string
		    Receives the string value
		@param strSet
		    Tells that the string was set, receives the new set status
		*/
		static void ToStr( const value_type & value, bool valSet, String & string, bool & strSet )
		{
			if ( !strSet )
			{
				if ( valSet )
				{
					string += CStrUtils::ToString( value );
					strSet = true;
				}
				else
				{
					string.clear();
				}
			}
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
		static String ToInsertable( const value_type & value, bool valSet, DatabaseConnectionPtr connection )
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

	/** Specialization for std::vector< uint8_t > data type
	*/
	template<> class CDatabaseValuePolicy< std::vector< uint8_t > >
	{
	private:
		typedef std::vector< uint8_t > value_type;

	public:
		/** Reinitializes the given value
		@param value
		    The value
		*/
		static void Reset( value_type & value )
		{
			value.clear();
		}

		/** Sets the value to the given one
		@param in
		    The input value
		@param out
		    The output value
		@param valSet
		    Receives the new setting status for the value
		@param strSet
		    Receives the new setting status for the string value
		@param size
		    Receives the new value size
		*/
		static void Set( const value_type & in, value_type & out, bool & valSet, bool & strSet, long & size )
		{
			strSet = false;
			out.clear();

			if ( !in.empty() )
			{
				out.reserve( in.size() );
				out.insert( out.end(), in.begin(), in.end() );
				size = long( in.size() );
			}

			valSet = true;
		}

		/** Retrieves a pointer from the given value
		@param value
		    The value
		*/
		static void * Ptr( value_type & value )
		{
			void * result = NULL;

			if ( !value.empty() )
			{
				result = ( void * )value.data();
			}

			return result;
		}

		/** Retrieves the value from a string
		@param string
		    The string containing the value
		@param strSet
		    Tells that the string effectively contains a value
		@param value
		    Receives the value
		@param valSet
		    Tells that the value was set, receives the new set status
		@param size
		    The old size, receives the new value size
		*/
		static void FromStr( const String & string, bool strSet, value_type & value, bool & valSet, long & size )
		{
			if ( !valSet )
			{
				if ( strSet )
				{
					if ( !string.empty() )
					{
						//StringStream stream( _value ); // Don't use this
						std::istringstream stream( CStrUtils::ToStr( string ) );
						size = long( string.size() );
						value.reserve( size );

						for ( long i = 0; i < size; ++i )
						{
							uint8_t byte;
							stream >> byte;
							value.push_back( byte );
						}
					}
				}
				else
				{
					//value = value_type();
				}
			}
		}

		/** Puts the value into the given string
		@param value
		    The value
		@param valSet
		    Tells that the value is set
		@param string
		    Receives the string value
		@param strSet
		    Tells that the string was set, receives the new set status
		*/
		static void ToStr( const value_type & value, bool valSet, String & string, bool & strSet )
		{
			if ( !strSet )
			{
				if ( valSet )
				{
					if ( !value.empty() )
					{
						StringStream stream;

						for ( std::vector< uint8_t >::const_iterator it = value.begin(); it != value.end(); ++it )
						{
							stream << ( *it );
						}

						string = stream.str();
						strSet = true;
					}
				}
				else
				{
					string.clear();
				}
			}
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
		static String ToInsertable( const value_type & value, bool valSet, DatabaseConnectionPtr connection )
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
}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_VALUE_POLICY_H___
