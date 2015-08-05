/************************************************************************//**
* @file DatabaseTestHelpers.h
* @author Sylvain Doremus
* @version 1.0
* @date 06/18/2014 12:04:36
*
*
* @brief Helpers for Database tests.
*
***************************************************************************/

#ifndef ___DATABASE_TEST_HELPERS_H___
#define ___DATABASE_TEST_HELPERS_H___

#include <random>

BEGIN_NAMESPACE_DATABASE_TEST
{
	namespace DatabaseUtils
	{
		template< EFieldType FieldType > struct Helpers;

		template<> struct Helpers< EFieldType_BIT >
		{
			static const uint32_t Limit;
			typedef bool ParamType;
			typedef ParamType FieldType;

			static ParamType GetRandomValue( std::random_device & generator )
			{
				std::uniform_int_distribution< int > distribution( 0, 1 );
				return distribution( generator ) == 1;
			}
		};

		template<> struct Helpers< EFieldType_SINT8 >
		{
			static const uint32_t Limit;
			typedef int8_t ParamType;
			typedef ParamType FieldType;

			static ParamType GetRandomValue( std::random_device & generator )
			{
				std::uniform_int_distribution< int > distribution( std::numeric_limits< ParamType >::lowest(), std::numeric_limits< ParamType >::max() );
				return ParamType( distribution( generator ) );
			}
		};

		template<> struct Helpers< EFieldType_UINT8 >
		{
			static const uint32_t Limit;
			typedef uint8_t ParamType;
			typedef ParamType FieldType;

			static ParamType GetRandomValue( std::random_device & generator )
			{
				std::uniform_int_distribution< int > distribution( std::numeric_limits< ParamType >::lowest(), std::numeric_limits< ParamType >::max() );
				return distribution( generator );
			}
		};

		template<> struct Helpers< EFieldType_SINT16 >
		{
			static const uint32_t Limit;
			typedef int16_t ParamType;
			typedef ParamType FieldType;

			static ParamType GetRandomValue( std::random_device & generator )
			{
				std::uniform_int_distribution< ParamType > distribution( std::numeric_limits< ParamType >::lowest(), std::numeric_limits< ParamType >::max() );
				return distribution( generator );
			}
		};

		template<> struct Helpers< EFieldType_UINT16 >
		{
			static const uint32_t Limit;
			typedef uint16_t ParamType;
			typedef ParamType FieldType;

			static ParamType GetRandomValue( std::random_device & generator )
			{
				std::uniform_int_distribution< ParamType > distribution( std::numeric_limits< ParamType >::lowest(), std::numeric_limits< ParamType >::max() );
				return distribution( generator );
			}
		};

		template<> struct Helpers< EFieldType_SINT24 >
		{
			static const uint32_t Limit;
			typedef int24_t ParamType;
			typedef ParamType FieldType;

			static ParamType GetRandomValue( std::random_device & generator )
			{
				std::uniform_int_distribution< int32_t > distribution( 0xFF800000, 0x007FFFFF );
				return ParamType( distribution( generator ) );
			}
		};

		template<> struct Helpers< EFieldType_UINT24 >
		{
			static const uint32_t Limit;
			typedef uint24_t ParamType;
			typedef ParamType FieldType;

			static ParamType GetRandomValue( std::random_device & generator )
			{
				std::uniform_int_distribution< uint32_t > distribution( 0, 0x00FFFFFF );
				return ParamType( distribution( generator ) );
			}
		};

		template<> struct Helpers< EFieldType_SINT32 >
		{
			static const uint32_t Limit;
			typedef int32_t ParamType;
			typedef ParamType FieldType;

			static ParamType GetRandomValue( std::random_device & generator )
			{
				std::uniform_int_distribution< ParamType > distribution( std::numeric_limits< ParamType >::lowest(), std::numeric_limits< ParamType >::max() );
				return distribution( generator );
			}
		};

		template<> struct Helpers< EFieldType_UINT32 >
		{
			static const uint32_t Limit;
			typedef uint32_t ParamType;
			typedef ParamType FieldType;

			static ParamType GetRandomValue( std::random_device & generator )
			{
				std::uniform_int_distribution< ParamType > distribution( std::numeric_limits< ParamType >::lowest(), std::numeric_limits< ParamType >::max() );
				return distribution( generator );
			}
		};

		template<> struct Helpers< EFieldType_SINT64 >
		{
			static const uint32_t Limit;
			typedef int64_t ParamType;
			typedef ParamType FieldType;

			static ParamType GetRandomValue( std::random_device & generator )
			{
				std::uniform_int_distribution< ParamType > distribution( std::numeric_limits< ParamType >::lowest(), std::numeric_limits< ParamType >::max() );
				return distribution( generator );
			}
		};

		template<> struct Helpers< EFieldType_UINT64 >
		{
			static const uint32_t Limit;
			typedef uint64_t ParamType;
			typedef ParamType FieldType;

			static ParamType GetRandomValue( std::random_device & generator )
			{
				std::uniform_int_distribution< ParamType > distribution( std::numeric_limits< ParamType >::lowest(), std::numeric_limits< ParamType >::max() );
				return distribution( generator );
			}
		};

		template<> struct Helpers< EFieldType_FLOAT32 >
		{
			static const uint32_t Limit;
			typedef float ParamType;
			typedef ParamType FieldType;

			static ParamType GetRandomValue( std::random_device & generator )
			{
				std::uniform_real_distribution< ParamType > fdistribution( -1000000.0000, 1000000.0000 );
				return ParamType( fdistribution( generator ) );
			}
		};

		template<> struct Helpers< EFieldType_FLOAT64 >
		{
			static const uint32_t Limit;
			typedef double ParamType;
			typedef ParamType FieldType;

			static ParamType GetRandomValue( std::random_device & generator )
			{
				std::uniform_real_distribution< ParamType > fdistribution( -1000000000000.0000000, 1000000000000.0000000 );
				return ParamType( fdistribution( generator ) );
			}
		};

		template<> struct Helpers< EFieldType_FIXED_POINT >
		{
			static const std::pair< uint32_t, uint32_t > Precision;
			typedef CFixedPoint ParamType;
			typedef ParamType FieldType;

			static ParamType GetRandomValue( std::random_device & generator, const uint8_t precision = 10, const uint8_t decimals = 3 )
			{
				std::uniform_int_distribution< int64_t > distribution( std::numeric_limits< int64_t >::lowest(), std::numeric_limits< int64_t >::max() );
				return CFixedPoint( distribution( generator ) % int64_t( pow( 10, precision ) ), precision, decimals );
			}

			static ParamType GetRandomValue( std::random_device & generator, const std::pair< uint32_t, uint32_t > & precision )
			{
				return GetRandomValue( generator, precision.first, precision.second );
			}
		};

		template<> struct Helpers< EFieldType_DATE >
		{
			static const uint32_t Limit;
			typedef DateType ParamType;
			typedef ParamType FieldType;

			static ParamType GetRandomValue( std::random_device & generator )
			{
				return boost::gregorian::day_clock::local_day();
			}
		};

		template<> struct Helpers< EFieldType_DATETIME >
		{
			static const uint32_t Limit;
			typedef DateTimeType ParamType;
			typedef ParamType FieldType;

			static ParamType GetRandomValue( std::random_device & generator )
			{
				return boost::posix_time::second_clock::local_time();
			}
		};

		template<> struct Helpers< EFieldType_TIME >
		{
			static const uint32_t Limit;
			typedef TimeType ParamType;
			typedef ParamType FieldType;

			static ParamType GetRandomValue( std::random_device & generator )
			{
				return boost::posix_time::second_clock::local_time().time_of_day();
			}
		};

		template<> struct Helpers< EFieldType_CHAR >
		{
			static const uint32_t Limit;
			typedef std::string ParamType;
			typedef std::string FieldType;

			static ParamType GetRandomValue( std::random_device & generator, size_t size )
			{
				std::uniform_int_distribution< int > distribution( 32, 126 );
				std::stringstream text;

				for ( size_t i = 0; i < size; ++i )
				{
					char c = char( distribution( generator ) );

					if ( c == '\\' )
					{
						c = '/';
					}

					text << c;
				};

				return text.str();
			}

			static ParamType GetRandomValue( std::random_device & generator )
			{
				return GetRandomValue( generator, Limit );
			}
		};

		template<> struct Helpers< EFieldType_VARCHAR >
		{
			static const uint32_t Limit;
			typedef std::string ParamType;
			typedef std::string FieldType;

			static ParamType GetRandomValue( std::random_device & generator, size_t size )
			{
				std::uniform_int_distribution< int > distribution( 32, 126 );
				std::stringstream text;

				for ( size_t i = 0; i < size; ++i )
				{
					char c = char( distribution( generator ) );

					if ( c == '\\' )
					{
						c = '/';
					}

					text << c;
				};

				return text.str();
			}

			static ParamType GetRandomValue( std::random_device & generator )
			{
				return GetRandomValue( generator, Limit );
			}
		};

		template<> struct Helpers< EFieldType_NCHAR >
		{
			static const uint32_t Limit;
			typedef std::wstring ParamType;
			typedef std::wstring FieldType;

			static ParamType GetRandomValue( std::random_device & generator, size_t size )
			{
				std::uniform_int_distribution< int > distribution( 32, 126 );
				std::wstringstream text;

				for ( size_t i = 0; i < size; ++i )
				{
					wchar_t c = wchar_t( distribution( generator ) );

					if ( c == L'\\' )
					{
						c = L'/';
					}

					text << c;
				};

				return text.str();
			}

			static ParamType GetRandomValue( std::random_device & generator )
			{
				return GetRandomValue( generator, Limit );
			}
		};

		template<> struct Helpers< EFieldType_NVARCHAR >
		{
			static const uint32_t Limit;
			typedef std::wstring ParamType;
			typedef std::wstring FieldType;

			static ParamType GetRandomValue( std::random_device & generator, size_t size )
			{
				std::uniform_int_distribution< int > distribution( 32, 126 );
				std::wstringstream text;

				for ( size_t i = 0; i < size; ++i )
				{
					wchar_t c = wchar_t( distribution( generator ) );

					if ( c == L'\\' )
					{
						c = L'/';
					}

					text << c;
				};

				return text.str();
			}

			static ParamType GetRandomValue( std::random_device & generator )
			{
				return GetRandomValue( generator, Limit );
			}
		};

		template<> struct Helpers< EFieldType_TEXT >
		{
			static const uint32_t Limit;
			typedef std::string ParamType;
			typedef std::string FieldType;

			static ParamType GetRandomValue( std::random_device & generator, size_t size )
			{
				std::uniform_int_distribution< int > distribution( 32, 126 );
				std::stringstream text;

				for ( size_t i = 0; i < size; ++i )
				{
					char c = char( distribution( generator ) );

					if ( c == '\\' )
					{
						c = '/';
					}

					text << c;
				};

				return text.str();
			}

			static ParamType GetRandomValue( std::random_device & generator )
			{
				return GetRandomValue( generator, 500 );
			}
		};

		template<> struct Helpers< EFieldType_NTEXT >
		{
			static const uint32_t Limit;
			typedef std::wstring ParamType;
			typedef std::wstring FieldType;

			static ParamType GetRandomValue( std::random_device & generator, size_t size )
			{
				std::uniform_int_distribution< int > distribution( 32, 126 );
				std::wstringstream text;

				for ( size_t i = 0; i < size; ++i )
				{
					char c = char( distribution( generator ) );

					if ( c == '\\' )
					{
						c = '/';
					}

					text << c;
				};

				return text.str();
			}

			static ParamType GetRandomValue( std::random_device & generator )
			{
				return GetRandomValue( generator, 500 );
			}
		};

		template<> struct Helpers< EFieldType_BINARY >
		{
			static const uint32_t Limit;
			typedef ByteArray ParamType;
			typedef ParamType FieldType;

			static ParamType GetRandomValue( std::random_device & generator, size_t size )
			{
				std::uniform_int_distribution< int > distribution( 32, 126 );
				ByteArray blob( size );

				for ( auto & value : blob )
				{
					char c = char( distribution( generator ) );

					if ( c == '\\' )
					{
						c = '/';
					}

					value = c;
				};

				return blob;
			}

			static ParamType GetRandomValue( std::random_device & generator )
			{
				return GetRandomValue( generator, Limit );
			}
		};

		template<> struct Helpers< EFieldType_VARBINARY >
		{
			static const uint32_t Limit;
			typedef ByteArray ParamType;
			typedef ParamType FieldType;

			static ParamType GetRandomValue( std::random_device & generator, size_t size )
			{
				std::uniform_int_distribution< int > distribution( 32, 126 );
				ByteArray blob( size );

				for ( auto & value : blob )
				{
					char c = char( distribution( generator ) );

					if ( c == '\\' )
					{
						c = '/';
					}

					value = c;
				};

				return blob;
			}

			static ParamType GetRandomValue( std::random_device & generator )
			{
				return GetRandomValue( generator, Limit );
			}
		};

		template<> struct Helpers< EFieldType_BLOB >
		{
			static const uint32_t Limit;
			typedef ByteArray ParamType;
			typedef ParamType FieldType;

			static ParamType GetRandomValue( std::random_device & generator, size_t size )
			{
				std::uniform_int_distribution< int > distribution( 32, 126 );
				ByteArray blob( size );

				for ( auto & value : blob )
				{
					char c = char( distribution( generator ) );

					if ( c == '\\' )
					{
						c = '/';
					}

					value = c;
				};

				return blob;
			}

			static ParamType GetRandomValue( std::random_device & generator )
			{
				return GetRandomValue( generator, 1024 );
			}
		};
	}
}
END_NAMESPACE_DATABASE_TEST

#endif // ___DATABASE_TEST_HELPERS_H___
