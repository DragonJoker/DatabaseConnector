/************************************************************************//**
 * @file DatabaseTestBase.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 12/02/2014 14:29:35
 *
 *
 * @brief Base class for database tests
 *
 ***************************************************************************/

#include "DatabaseTestPch.h"

#include "DatabaseTestBase.h"
#include "DatabaseTestUtils.h"

#include <DatabaseBlockGuard.h>

BEGIN_NAMESPACE_DATABASE_TEST
{
	template< EFieldType FieldType, uint8_t Precision = 0, uint8_t Decimals = 0 >
	struct UniqueTest
	{
		typedef DatabaseUtils::Helpers< FieldType > helper_type;
		typedef typename helper_type::ParamType param_type;

		template< typename Function >
		void operator()( Function function, DatabaseConnectionSPtr connection, String const & name, String const & is, std::random_device & generator )
		{
			param_type value( helper_type::GetRandomValue( generator ) );
			BOOST_CHECK_NO_THROW( function( connection, name, &value, true, is ) );
		}
	};

	template< uint8_t Precision, uint8_t Decimals >
	struct UniqueTest< EFieldType_FIXED_POINT, Precision, Decimals >
	{
		typedef DatabaseUtils::Helpers< EFieldType_FIXED_POINT > helper_type;
		typedef helper_type::ParamType param_type;

		template< typename Function >
		void operator()( Function function, DatabaseConnectionSPtr connection, String const & name, String const & is, std::random_device & generator )
		{
			param_type value( helper_type::GetRandomValue( generator, Precision, Decimals ) );
			BOOST_CHECK_NO_THROW( function( connection, name, &value, true, is ) );
		}
	};

	template< EFieldType FieldType, uint8_t Precision = 0, uint8_t Decimals = 0 >
	struct BatchTests
	{
		typedef DatabaseUtils::Helpers< FieldType > helper_type;
		typedef typename helper_type::ParamType param_type;

		template< typename Function >
		void operator()( Function function, DatabaseConnectionSPtr connection, String const & name, String const & is, std::random_device & generator )
		{
			param_type value = param_type();

			if ( is == STR( "IS" ) )
			{
				CLogger::LogInfo( StringStream() << "  NULL" );
				BOOST_CHECK_NO_THROW( function( connection, name, NULL, true, is ) );
			}

			CLogger::LogInfo( StringStream() << "  Default" );
			BOOST_CHECK_NO_THROW( function( connection, name, &value, true, is ) );
			CLogger::LogInfo( StringStream() << "  Lowest" );
			BOOST_CHECK_NO_THROW( function( connection, name, &( value = std::numeric_limits< param_type >::lowest() ), true, is ) );
			CLogger::LogInfo( StringStream() << "  Max" );
			BOOST_CHECK_NO_THROW( function( connection, name, &( value = std::numeric_limits< param_type >::max() ), true, is ) );
			CLogger::LogInfo( StringStream() << "  Min" );
			BOOST_CHECK_NO_THROW( function( connection, name, &( value = std::numeric_limits< param_type >::min() ), true, is ) );
			CLogger::LogInfo( StringStream() << "  Given" );
			BOOST_CHECK_NO_THROW( function( connection, name, &( value = helper_type::GetRandomValue( generator ) ), true, is ) );
		}
	};

	template<>
	struct BatchTests< EFieldType_FLOAT32 >
	{
		typedef DatabaseUtils::Helpers< EFieldType_FLOAT32 > helper_type;
		typedef helper_type::ParamType param_type;

		template< typename Function >
		void operator()( Function function, DatabaseConnectionSPtr connection, String const & name, String const & is, std::random_device & generator )
		{
			param_type value = param_type();

			if ( is == STR( "IS" ) )
			{
				CLogger::LogInfo( StringStream() << "  NULL" );
				BOOST_CHECK_NO_THROW( function( connection, name, NULL, true, is ) );
			}

			CLogger::LogInfo( StringStream() << "  Default" );
			BOOST_CHECK_NO_THROW( function( connection, name, &value, true, is ) );
			CLogger::LogInfo( StringStream() << "  Lowest" );
			BOOST_CHECK_NO_THROW( function( connection, name, &( value = std::numeric_limits< param_type >::lowest() ), true, is ) );
			CLogger::LogInfo( StringStream() << "  Max" );
			BOOST_CHECK_NO_THROW( function( connection, name, &( value = std::numeric_limits< param_type >::max() ), true, is ) );
			CLogger::LogInfo( StringStream() << "  Min" );
			BOOST_CHECK_NO_THROW( function( connection, name, &( value = std::numeric_limits< param_type >::min() ), true, is ) );
			CLogger::LogInfo( StringStream() << "  Given" );
			BOOST_CHECK_NO_THROW( function( connection, name, &( value = helper_type::GetRandomValue( generator ) ), true, is ) );
		}
	};

	template<>
	struct BatchTests< EFieldType_FLOAT64 >
	{
		typedef DatabaseUtils::Helpers< EFieldType_FLOAT64 > helper_type;
		typedef helper_type::ParamType param_type;

		template< typename Function >
		void operator()( Function function, DatabaseConnectionSPtr connection, String const & name, String const & is, std::random_device & generator )
		{
			param_type value = param_type();

			if ( is == STR( "IS" ) )
			{
				CLogger::LogInfo( StringStream() << "  NULL" );
				BOOST_CHECK_NO_THROW( function( connection, name, NULL, true, is ) );
			}

			CLogger::LogInfo( StringStream() << "  Default" );
			BOOST_CHECK_NO_THROW( function( connection, name, &value, true, is ) );
			CLogger::LogInfo( StringStream() << "  Lowest / 2" );
			BOOST_CHECK_NO_THROW( function( connection, name, &( value = std::numeric_limits< param_type >::lowest() / 2 ), true, is ) );
			CLogger::LogInfo( StringStream() << "  Max / 2" );
			BOOST_CHECK_NO_THROW( function( connection, name, &( value = std::numeric_limits< param_type >::max() / 2 ), true, is ) );
			CLogger::LogInfo( StringStream() << "  Min" );
			BOOST_CHECK_NO_THROW( function( connection, name, &( value = std::numeric_limits< param_type >::min() ), true, is ) );
			CLogger::LogInfo( StringStream() << "  Given" );
			BOOST_CHECK_NO_THROW( function( connection, name, &( value = helper_type::GetRandomValue( generator ) ), true, is ) );
		}
	};

	template<>
	struct BatchTests< EFieldType_DATE >
	{
		typedef DatabaseUtils::Helpers< EFieldType_DATE > helper_type;
		typedef helper_type::ParamType param_type;

		template< typename Function >
		void operator()( Function function, DatabaseConnectionSPtr connection, String const & name, String const & is, std::random_device & generator )
		{
			param_type value = param_type();

			if ( is == STR( "IS" ) )
			{
				CLogger::LogInfo( StringStream() << "  NULL" );
				BOOST_CHECK_NO_THROW( function( connection, name, NULL, true, is ) );
				CLogger::LogInfo( StringStream() << "  Default" );
				BOOST_CHECK_NO_THROW( function( connection, name, &value, true, is ) );
			}

			CLogger::LogInfo( StringStream() << "  Given" );
			BOOST_CHECK_NO_THROW( function( connection, name, &( value = helper_type::GetRandomValue( generator ) ), true, is ) );
		}
	};

	template<>
	struct BatchTests< EFieldType_DATETIME >
	{
		typedef DatabaseUtils::Helpers< EFieldType_DATETIME > helper_type;
		typedef helper_type::ParamType param_type;

		template< typename Function >
		void operator()( Function function, DatabaseConnectionSPtr connection, String const & name, String const & is, std::random_device & generator )
		{
			param_type value = param_type();

			if ( is == STR( "IS" ) )
			{
				CLogger::LogInfo( StringStream() << "  NULL" );
				BOOST_CHECK_NO_THROW( function( connection, name, NULL, true, is ) );
				CLogger::LogInfo( StringStream() << "  Default" );
				BOOST_CHECK_NO_THROW( function( connection, name, &value, true, is ) );
			}

			CLogger::LogInfo( StringStream() << "  Given" );
			BOOST_CHECK_NO_THROW( function( connection, name, &( value = helper_type::GetRandomValue( generator ) ), true, is ) );
		}
	};

	template<>
	struct BatchTests< EFieldType_TIME >
	{
		typedef DatabaseUtils::Helpers< EFieldType_TIME > helper_type;
		typedef helper_type::ParamType param_type;

		template< typename Function >
		void operator()( Function function, DatabaseConnectionSPtr connection, String const & name, String const & is, std::random_device & generator )
		{
			param_type value = param_type();

			if ( is == STR( "IS" ) )
			{
				CLogger::LogInfo( StringStream() << "  NULL" );
				BOOST_CHECK_NO_THROW( function( connection, name, NULL, true, is ) );
			}

			CLogger::LogInfo( StringStream() << "  Default" );
			BOOST_CHECK_NO_THROW( function( connection, name, &value, true, is ) );
			CLogger::LogInfo( StringStream() << "  Given" );
			BOOST_CHECK_NO_THROW( function( connection, name, &( value = helper_type::GetRandomValue( generator ) ), true, is ) );
		}
	};

	template< uint8_t Precision, uint8_t Decimals >
	struct BatchTests< EFieldType_FIXED_POINT, Precision, Decimals >
	{
		typedef DatabaseUtils::Helpers< EFieldType_FIXED_POINT > helper_type;
		typedef helper_type::ParamType param_type;

		template< typename Function >
		void operator()( Function function, DatabaseConnectionSPtr connection, String const & name, String const & is, std::random_device & generator )
		{
			typedef DatabaseUtils::Helpers< EFieldType_FIXED_POINT >::ParamType param_type;
			param_type value = param_type();

			if ( is == STR( "IS" ) )
			{
				CLogger::LogInfo( StringStream() << "  NULL" );
				BOOST_CHECK_NO_THROW( function( connection, name, NULL, true, is ) );
			}

			CLogger::LogInfo( StringStream() << "  Default" );
			BOOST_CHECK_NO_THROW( function( connection, name, &value, true, is ) );
			CLogger::LogInfo( StringStream() << "  Max" );
			BOOST_CHECK_NO_THROW( function( connection, name, &( value = param_type( ( std::numeric_limits< int64_t >::max() ) % int64_t( pow( 10, Precision ) ), Precision, Decimals ) ), true, is ) );
			CLogger::LogInfo( StringStream() << "  Min" );
			BOOST_CHECK_NO_THROW( function( connection, name, &( value = param_type( ( std::numeric_limits< int64_t >::min() ) % int64_t( pow( 10, Precision ) ), Precision, Decimals ) ), true, is ) );
			CLogger::LogInfo( StringStream() << "  Lowest" );
			BOOST_CHECK_NO_THROW( function( connection, name, &( value = param_type( ( std::numeric_limits< int64_t >::lowest() ) % int64_t( pow( 10, Precision ) ), Precision, Decimals ) ), true, is ) );
			CLogger::LogInfo( StringStream() << "  Given" );
			BOOST_CHECK_NO_THROW( function( connection, name, &( value = helper_type::GetRandomValue( generator, Precision, Decimals ) ), true, is ) );
			CLogger::LogInfo( StringStream() << "  Specific value, precision test" );
			BOOST_CHECK_NO_THROW( function( connection, name, &( value = param_type( 602872352, Precision, Decimals ) ), true, is ) );
			CLogger::LogInfo( StringStream() << "  Specific value, underflow" );
			BOOST_CHECK_NO_THROW( function( connection, name, &( value = param_type( 6210, Precision, Decimals ) ), true, is ) );
		}
	};

	template< uint8_t Size >
	struct BatchTests< EFieldType_CHAR, Size >
	{
		typedef DatabaseUtils::Helpers< EFieldType_CHAR > helper_type;
		typedef helper_type::ParamType param_type;

		template< typename Function >
		void operator()( Function function, DatabaseConnectionSPtr connection, String const & name, String const & is, std::random_device & generator )
		{
			typedef DatabaseUtils::Helpers< EFieldType_CHAR >::ParamType param_type;
			param_type value = param_type();

			if ( is == STR( "IS" ) )
			{
				CLogger::LogInfo( StringStream() << "  NULL" );
				BOOST_CHECK_NO_THROW( function( connection, name, NULL, true, is ) );
			}

			CLogger::LogInfo( StringStream() << "  Default" );
			BOOST_CHECK_NO_THROW( function( connection, name, &value, true, is ) );
			CLogger::LogInfo( StringStream() << "  Truncate" );
			BOOST_CHECK_NO_THROW( function( connection, name, &( value = DatabaseUtils::Helpers< EFieldType_CHAR >::GetRandomValue( generator, 1024 ) ), true, is ) );
			CLogger::LogInfo( StringStream() << "  Given" );
			BOOST_CHECK_NO_THROW( function( connection, name, &( value = helper_type::GetRandomValue( generator, Size ) ), true, is ) );
		}
	};

	template<>
	struct BatchTests< EFieldType_VARCHAR >
	{
		typedef DatabaseUtils::Helpers< EFieldType_VARCHAR > helper_type;
		typedef helper_type::ParamType param_type;

		template< typename Function >
		void operator()( Function function, DatabaseConnectionSPtr connection, String const & name, String const & is, std::random_device & generator )
		{
			typedef DatabaseUtils::Helpers< EFieldType_VARCHAR >::ParamType param_type;
			param_type value = param_type();

			if ( is == STR( "IS" ) )
			{
				CLogger::LogInfo( StringStream() << "  NULL" );
				BOOST_CHECK_NO_THROW( function( connection, name, NULL, true, is ) );
			}

			CLogger::LogInfo( StringStream() << "  Default" );
			BOOST_CHECK_NO_THROW( function( connection, name, &value, true, is ) );
			CLogger::LogInfo( StringStream() << "  Truncate" );
			BOOST_CHECK_NO_THROW( function( connection, name, &( value = DatabaseUtils::Helpers< EFieldType_VARCHAR >::GetRandomValue( generator, 1024 ) ), true, is ) );
			CLogger::LogInfo( StringStream() << "  Given" );
			BOOST_CHECK_NO_THROW( function( connection, name, &( value = helper_type::GetRandomValue( generator ) ), true, is ) );
		}
	};

	template< uint8_t Size >
	struct BatchTests< EFieldType_NCHAR, Size >
	{
		typedef DatabaseUtils::Helpers< EFieldType_NCHAR > helper_type;
		typedef helper_type::ParamType param_type;

		template< typename Function >
		void operator()( Function function, DatabaseConnectionSPtr connection, String const & name, String const & is, std::random_device & generator )
		{
			typedef DatabaseUtils::Helpers< EFieldType_NCHAR >::ParamType param_type;
			param_type value = param_type();

			if ( is == STR( "IS" ) )
			{
				CLogger::LogInfo( StringStream() << "  NULL" );
				BOOST_CHECK_NO_THROW( function( connection, name, NULL, true, is ) );
			}

			CLogger::LogInfo( StringStream() << "  Default" );
			BOOST_CHECK_NO_THROW( function( connection, name, &value, true, is ) );
			CLogger::LogInfo( StringStream() << "  Truncate" );
			BOOST_CHECK_NO_THROW( function( connection, name, &( value = DatabaseUtils::Helpers< EFieldType_NCHAR >::GetRandomValue( generator, 1024 ) ), true, is ) );
			CLogger::LogInfo( StringStream() << "  Given" );
			BOOST_CHECK_NO_THROW( function( connection, name, &( value = helper_type::GetRandomValue( generator ) ), true, is ) );
		}
	};

	template<>
	struct BatchTests< EFieldType_NVARCHAR >
	{
		typedef DatabaseUtils::Helpers< EFieldType_NVARCHAR > helper_type;
		typedef helper_type::ParamType param_type;

		template< typename Function >
		void operator()( Function function, DatabaseConnectionSPtr connection, String const & name, String const & is, std::random_device & generator )
		{
			typedef DatabaseUtils::Helpers< EFieldType_NVARCHAR >::ParamType param_type;
			param_type value = param_type();

			if ( is == STR( "IS" ) )
			{
				CLogger::LogInfo( StringStream() << "  NULL" );
				BOOST_CHECK_NO_THROW( function( connection, name, NULL, true, is ) );
			}

			CLogger::LogInfo( StringStream() << "  Default" );
			BOOST_CHECK_NO_THROW( function( connection, name, &value, true, is ) );
			CLogger::LogInfo( StringStream() << "  Truncate" );
			BOOST_CHECK_NO_THROW( function( connection, name, &( value = DatabaseUtils::Helpers< EFieldType_NVARCHAR >::GetRandomValue( generator, 1024 ) ), true, is ) );
			CLogger::LogInfo( StringStream() << "  Given" );
			BOOST_CHECK_NO_THROW( function( connection, name, &( value = helper_type::GetRandomValue( generator ) ), true, is ) );
		}
	};

	template<>
	struct BatchTests< EFieldType_TEXT >
	{
		typedef DatabaseUtils::Helpers< EFieldType_TEXT > helper_type;
		typedef helper_type::ParamType param_type;

		template< typename Function >
		void operator()( Function function, DatabaseConnectionSPtr connection, String const & name, String const & is, std::random_device & generator )
		{
			typedef DatabaseUtils::Helpers< EFieldType_TEXT >::ParamType param_type;
			param_type value = param_type();

			if ( is == STR( "IS" ) )
			{
				CLogger::LogInfo( StringStream() << "  NULL" );
				BOOST_CHECK_NO_THROW( function( connection, name, NULL, true, is ) );
			}

			CLogger::LogInfo( StringStream() << "  Default" );
			BOOST_CHECK_NO_THROW( function( connection, name, &value, true, is ) );
			CLogger::LogInfo( StringStream() << "  Big" );
			BOOST_CHECK_NO_THROW( function( connection, name, &( value = DatabaseUtils::Helpers< EFieldType_TEXT >::GetRandomValue( generator, 1024 * 16 ) ), true, is ) );
			CLogger::LogInfo( StringStream() << "  Given" );
			BOOST_CHECK_NO_THROW( function( connection, name, &( value = helper_type::GetRandomValue( generator ) ), true, is ) );
		}
	};

	template<>
	struct BatchTests< EFieldType_VARBINARY >
	{
		typedef DatabaseUtils::Helpers< EFieldType_VARBINARY > helper_type;
		typedef helper_type::ParamType param_type;

		template< typename Function >
		void operator()( Function function, DatabaseConnectionSPtr connection, String const & name, String const & is, std::random_device & generator )
		{
			typedef DatabaseUtils::Helpers< EFieldType_VARBINARY >::ParamType param_type;
			param_type value = param_type();

			if ( is == STR( "IS" ) )
			{
				CLogger::LogInfo( StringStream() << "  NULL" );
				BOOST_CHECK_NO_THROW( function( connection, name, NULL, true, is ) );
			}

			CLogger::LogInfo( StringStream() << "  Big" );
			BOOST_CHECK_NO_THROW( function( connection, name, &( value = DatabaseUtils::Helpers< EFieldType_VARBINARY >::GetRandomValue( generator, 1024 * 16 ) ), true, is ) );
			CLogger::LogInfo( StringStream() << "  Given" );
			BOOST_CHECK_NO_THROW( function( connection, name, &( value = helper_type::GetRandomValue( generator ) ), true, is ) );
		}
	};

	template< size_t StmtType >
	void CDatabaseTest::TestCase_DatabaseFieldsInsertRetrieve()
	{
		auto const guard = make_block_guard( [this]()
		{
			CLogger::LogInfo( StringStream() << "**** Start " << _type << "_TestCase_Database" << DatabaseUtils::Name[StmtType] << "FieldsInsertRetrieve  ****" );
			DoLoadPlugins();
		}, [this]()
		{
			UnloadPlugins();
			CLogger::LogInfo( StringStream() << "**** End " << _type << "_TestCase_Database" << DatabaseUtils::Name[StmtType] << "FieldsInsertRetrieve  ****" );
		} );
		std::unique_ptr< CDatabase > database( InstantiateDatabase( _type ) );

		if ( database )
		{
			DatabaseConnectionSPtr connection = CreateConnection( *database, _server, _user, _password );

			if ( connection )
			{
				if ( connection->IsConnected() )
				{
					connection->SelectDatabase( _database );
					DoFlushTable( connection );
					std::random_device generator;
					BatchTests< EFieldType_SINT32 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_SINT32 >, connection, STR( "IntegerField" ), _config.is, generator );
					BatchTests< EFieldType_SINT16 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_SINT16 >, connection, STR( "SmallIntField" ), _config.is, generator );
					BatchTests< EFieldType_SINT64 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_SINT64 >, connection, STR( "BigIntField" ), _config.is, generator );
					BatchTests< EFieldType_SINT16 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_SINT16 >, connection, STR( "Int2Field" ), _config.is, generator );
					BatchTests< EFieldType_SINT64 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_SINT64 >, connection, STR( "Int8Field" ), _config.is, generator );
					BatchTests< EFieldType_FLOAT64 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_FLOAT64 >, connection, STR( "RealField" ), _config.is, generator );
					BatchTests< EFieldType_FLOAT64 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_FLOAT64 >, connection, STR( "DoubleField" ), _config.is, generator );
					BatchTests< EFieldType_FLOAT64 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_FLOAT64 >, connection, STR( "DoublePrecisionField" ), _config.is, generator );
					BatchTests< EFieldType_FIXED_POINT, 10, 0 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_FIXED_POINT >, connection, STR( "NumericField" ), _config.is, generator );
					BatchTests< EFieldType_FIXED_POINT, 10, 5 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_FIXED_POINT >, connection, STR( "DecimalField" ), _config.is, generator );
					BatchTests< EFieldType_BIT >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_BIT >, connection, STR( "BooleanField" ), _config.is, generator );
					BatchTests< EFieldType_DATE >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_DATE >, connection, STR( "DateField" ), _config.is, generator );
					BatchTests< EFieldType_DATETIME >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_DATETIME >, connection, STR( "DateTimeField" ), _config.is, generator );
					BatchTests< EFieldType_CHAR, 20 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_CHAR >, connection, STR( "CharacterField" ), _config.is, generator );
					BatchTests< EFieldType_VARCHAR >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_VARCHAR >, connection, STR( "VarcharField" ), _config.is, generator );
					BatchTests< EFieldType_NCHAR, 55 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_NCHAR >, connection, STR( "NcharField" ), _config.is, generator );
					BatchTests< EFieldType_NVARCHAR >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_NVARCHAR >, connection, STR( "NVarcharField" ), _config.is, generator );
					BatchTests< EFieldType_TEXT >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_TEXT >, connection, STR( "TextField" ), _config.is, generator );
					BatchTests< EFieldType_VARBINARY >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_VARBINARY >, connection, STR( "BlobField" ), _config.is, generator );

					if ( StmtType || _config.hasDecentFloat32 )
					{
						BatchTests< EFieldType_FLOAT32 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_FLOAT32 >, connection, STR( "FloatField" ), _config.is, generator );
					}

					if ( _config.hasTinyInt )
					{
						if ( _config.hasUnsignedTiny )
						{
							BatchTests< EFieldType_UINT8 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_UINT8 >, connection, STR( "TinyIntField" ), _config.is, generator );
						}
						else
						{
							BatchTests< EFieldType_SINT8 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_SINT8 >, connection, STR( "TinyIntField" ), _config.is, generator );
						}
					}

					if ( _config.hasInt24 )
					{
						BatchTests< EFieldType_SINT24 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_SINT24 >, connection, STR( "MediumIntField" ), _config.is, generator );
					}
				}

				database->RemoveConnection();
			}
		}
	}

	template< size_t StmtType >
	void CDatabaseTest::TestCase_DatabaseFieldsInsertRetrieveOtherIndex()
	{
		auto const guard = make_block_guard( [this]()
		{
			CLogger::LogInfo( StringStream() << "**** Start " << _type << "_TestCase_Database" << DatabaseUtils::Name[StmtType] << "InsertAndRetrieveOtherIndex ****" );
			DoLoadPlugins();
		}, [this]()
		{
			UnloadPlugins();
			CLogger::LogInfo( StringStream() << "**** End " << _type << "_TestCase_Database" << DatabaseUtils::Name[StmtType] << "InsertAndRetrieveOtherIndex ****" );
		} );
		std::unique_ptr< CDatabase > database( InstantiateDatabase( _type ) );

		if ( database )
		{
			DatabaseConnectionSPtr connection = CreateConnection( *database, _server, _user, _password );

			if ( connection )
			{
				if ( connection->IsConnected() )
				{
					connection->SelectDatabase( _database );
					DoFlushTable( connection );
					std::random_device generator;
					UniqueTest< EFieldType_SINT32 >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_SINT32 >, connection, STR( "IntegerField" ), _config.is, generator );
					UniqueTest< EFieldType_SINT16 >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_SINT16 >, connection, STR( "SmallIntField" ), _config.is, generator );
					UniqueTest< EFieldType_SINT64 >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_SINT64 >, connection, STR( "BigIntField" ), _config.is, generator );
					UniqueTest< EFieldType_SINT16 >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_SINT16 >, connection, STR( "Int2Field" ), _config.is, generator );
					UniqueTest< EFieldType_SINT64 >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_SINT64 >, connection, STR( "Int8Field" ), _config.is, generator );
					UniqueTest< EFieldType_FLOAT64 >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_FLOAT64 >, connection, STR( "RealField" ), _config.is, generator );
					UniqueTest< EFieldType_FLOAT64 >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_FLOAT64 >, connection, STR( "DoubleField" ), _config.is, generator );
					UniqueTest< EFieldType_FLOAT64 >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_FLOAT64 >, connection, STR( "DoublePrecisionField" ), _config.is, generator );
					UniqueTest< EFieldType_FIXED_POINT, 10, 0 >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_FIXED_POINT >, connection, STR( "NumericField" ), _config.is, generator );
					UniqueTest< EFieldType_FIXED_POINT, 10, 5 >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_FIXED_POINT >, connection, STR( "DecimalField" ), _config.is, generator );
					UniqueTest< EFieldType_BIT >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_BIT >, connection, STR( "BooleanField" ), _config.is, generator );
					UniqueTest< EFieldType_DATE >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_DATE >, connection, STR( "DateField" ), _config.is, generator );
					UniqueTest< EFieldType_DATETIME >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_DATETIME >, connection, STR( "DateTimeField" ), _config.is, generator );
					UniqueTest< EFieldType_CHAR, 20 >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_CHAR >, connection, STR( "CharacterField" ), _config.is, generator );
					UniqueTest< EFieldType_VARCHAR >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_VARCHAR >, connection, STR( "VarcharField" ), _config.is, generator );
					UniqueTest< EFieldType_NCHAR, 55 >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_NCHAR >, connection, STR( "NcharField" ), _config.is, generator );
					UniqueTest< EFieldType_NVARCHAR >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_NVARCHAR >, connection, STR( "NVarcharField" ), _config.is, generator );
					UniqueTest< EFieldType_TEXT >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_TEXT >, connection, STR( "TextField" ), _config.is, generator );
					UniqueTest< EFieldType_VARBINARY >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_VARBINARY >, connection, STR( "BlobField" ), _config.is, generator );

					if ( StmtType || _config.hasDecentFloat32 )
					{
						UniqueTest< EFieldType_FLOAT32 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_FLOAT32 >, connection, STR( "FloatField" ), _config.is, generator );
					}

					if ( _config.hasTinyInt )
					{
						if ( _config.hasUnsignedTiny )
						{
							UniqueTest< EFieldType_UINT8 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_UINT8 >, connection, STR( "TinyIntField" ), _config.is, generator );
						}
						else
						{
							UniqueTest< EFieldType_SINT8 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_SINT8 >, connection, STR( "TinyIntField" ), _config.is, generator );
						}
					}

					if ( _config.hasInt24 )
					{
						UniqueTest< EFieldType_SINT24 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_SINT24 >, connection, STR( "MediumIntField" ), _config.is, generator );
					}
				}

				database->RemoveConnection();
			}
		}
	}

	template< size_t StmtType >
	void CDatabaseTest::TestCase_DatabaseFieldsInsertRetrieveFast()
	{
		auto const guard = make_block_guard( [this]()
		{
			CLogger::LogInfo( StringStream() << "**** Start " << _type << "_TestCase_Database" << DatabaseUtils::Name[StmtType] << "InsertAndRetrieveFast ****" );
			DoLoadPlugins();
		}, [this]()
		{
			UnloadPlugins();
			CLogger::LogInfo( StringStream() << "**** End " << _type << "_TestCase_Database" << DatabaseUtils::Name[StmtType] << "InsertAndRetrieveFast ****" );
		} );
		std::unique_ptr< CDatabase > database( InstantiateDatabase( _type ) );

		if ( database )
		{
			DatabaseConnectionSPtr connection = CreateConnection( *database, _server, _user, _password );

			if ( connection )
			{
				if ( connection->IsConnected() )
				{
					connection->SelectDatabase( _database );
					DoFlushTable( connection );
					std::random_device generator;
					UniqueTest< EFieldType_SINT32 >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_SINT32 >, connection, STR( "IntegerField" ), _config.is, generator );
					UniqueTest< EFieldType_SINT16 >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_SINT16 >, connection, STR( "SmallIntField" ), _config.is, generator );
					UniqueTest< EFieldType_SINT64 >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_SINT64 >, connection, STR( "BigIntField" ), _config.is, generator );
					UniqueTest< EFieldType_SINT16 >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_SINT16 >, connection, STR( "Int2Field" ), _config.is, generator );
					UniqueTest< EFieldType_SINT64 >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_SINT64 >, connection, STR( "Int8Field" ), _config.is, generator );
					UniqueTest< EFieldType_FLOAT64 >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_FLOAT64 >, connection, STR( "RealField" ), _config.is, generator );
					UniqueTest< EFieldType_FLOAT64 >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_FLOAT64 >, connection, STR( "DoubleField" ), _config.is, generator );
					UniqueTest< EFieldType_FLOAT64 >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_FLOAT64 >, connection, STR( "DoublePrecisionField" ), _config.is, generator );
					UniqueTest< EFieldType_FIXED_POINT, 10, 0 >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_FIXED_POINT >, connection, STR( "NumericField" ), _config.is, generator );
					UniqueTest< EFieldType_FIXED_POINT, 10, 5 >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_FIXED_POINT >, connection, STR( "DecimalField" ), _config.is, generator );
					UniqueTest< EFieldType_DATE >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_DATE >, connection, STR( "DateField" ), _config.is, generator );
					UniqueTest< EFieldType_DATETIME >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_DATETIME >, connection, STR( "DateTimeField" ), _config.is, generator );
					UniqueTest< EFieldType_CHAR, 20 >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_CHAR >, connection, STR( "CharacterField" ), _config.is, generator );
					UniqueTest< EFieldType_VARCHAR >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_VARCHAR >, connection, STR( "VarcharField" ), _config.is, generator );
					UniqueTest< EFieldType_TEXT >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_TEXT >, connection, STR( "TextField" ), _config.is, generator );
					UniqueTest< EFieldType_VARBINARY >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_VARBINARY >, connection, STR( "BlobField" ), _config.is, generator );

					if ( StmtType || _config.hasDecentFloat32 )
					{
						UniqueTest< EFieldType_FLOAT32 >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_FLOAT32 >, connection, STR( "FloatField" ), _config.is, generator );
					}

					if ( _config.hasTinyInt )
					{
						if ( _config.hasUnsignedTiny )
						{
							UniqueTest< EFieldType_UINT8 >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_UINT8 >, connection, STR( "TinyIntField" ), _config.is, generator );
						}
						else
						{
							UniqueTest< EFieldType_SINT8 >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_SINT8 >, connection, STR( "TinyIntField" ), _config.is, generator );
						}
					}

					if ( _config.hasInt24 )
					{
						UniqueTest< EFieldType_SINT24 >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_SINT24 >, connection, STR( "MediumIntField" ), _config.is, generator );
					}

					if ( _config.hasNChar )
					{
						UniqueTest< EFieldType_NCHAR, 55 >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_NCHAR >, connection, STR( "NcharField" ), _config.is, generator );
						UniqueTest< EFieldType_NVARCHAR >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_NVARCHAR >, connection, STR( "NVarcharField" ), _config.is, generator );
					}

					if ( _config.hasSeparateBooleanAndTinyInt )
					{
						UniqueTest< EFieldType_BIT >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_BIT >, connection, STR( "BooleanField" ), _config.is, generator );
					}
				}

				database->RemoveConnection();
			}
		}
	}

	template< size_t StmtType >
	void CDatabaseTest::TestCase_DatabaseFieldsInsertRetrieveFastOtherIndex()
	{
		auto const guard = make_block_guard( [this]()
		{
			CLogger::LogInfo( StringStream() << "**** Start " << _type << "_TestCase_Database" << DatabaseUtils::Name[StmtType] << "InsertAndRetrieveFastOtherIndex ****" );
			DoLoadPlugins();
		}, [this]()
		{
			UnloadPlugins();
			CLogger::LogInfo( StringStream() << "**** End " << _type << "_TestCase_Database" << DatabaseUtils::Name[StmtType] << "InsertAndRetrieveFastOtherIndex ****" );
		} );
		std::unique_ptr< CDatabase > database( InstantiateDatabase( _type ) );

		if ( database )
		{
			DatabaseConnectionSPtr connection = CreateConnection( *database, _server, _user, _password );

			if ( connection )
			{
				if ( connection->IsConnected() )
				{
					connection->SelectDatabase( _database );
					DoFlushTable( connection );
					std::random_device generator;
					UniqueTest< EFieldType_SINT32 >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_SINT32 >, connection, STR( "IntegerField" ), _config.is, generator );
					UniqueTest< EFieldType_SINT16 >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_SINT16 >, connection, STR( "SmallIntField" ), _config.is, generator );
					UniqueTest< EFieldType_SINT64 >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_SINT64 >, connection, STR( "BigIntField" ), _config.is, generator );
					UniqueTest< EFieldType_SINT16 >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_SINT16 >, connection, STR( "Int2Field" ), _config.is, generator );
					UniqueTest< EFieldType_SINT64 >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_SINT64 >, connection, STR( "Int8Field" ), _config.is, generator );
					UniqueTest< EFieldType_FLOAT64 >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_FLOAT64 >, connection, STR( "RealField" ), _config.is, generator );
					UniqueTest< EFieldType_FLOAT64 >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_FLOAT64 >, connection, STR( "DoubleField" ), _config.is, generator );
					UniqueTest< EFieldType_FLOAT64 >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_FLOAT64 >, connection, STR( "DoublePrecisionField" ), _config.is, generator );
					UniqueTest< EFieldType_FIXED_POINT, 10, 0 >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_FIXED_POINT >, connection, STR( "NumericField" ), _config.is, generator );
					UniqueTest< EFieldType_FIXED_POINT, 10, 5 >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_FIXED_POINT >, connection, STR( "DecimalField" ), _config.is, generator );
					UniqueTest< EFieldType_DATE >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_DATE >, connection, STR( "DateField" ), _config.is, generator );
					UniqueTest< EFieldType_DATETIME >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_DATETIME >, connection, STR( "DateTimeField" ), _config.is, generator );
					UniqueTest< EFieldType_CHAR, 20 >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_CHAR >, connection, STR( "CharacterField" ), _config.is, generator );
					UniqueTest< EFieldType_VARCHAR >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_VARCHAR >, connection, STR( "VarcharField" ), _config.is, generator );
					UniqueTest< EFieldType_TEXT >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_TEXT >, connection, STR( "TextField" ), _config.is, generator );
					UniqueTest< EFieldType_VARBINARY >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_VARBINARY >, connection, STR( "BlobField" ), _config.is, generator );

					if ( StmtType || _config.hasDecentFloat32 )
					{
						UniqueTest< EFieldType_FLOAT32 >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_FLOAT32 >, connection, STR( "FloatField" ), _config.is, generator );
					}

					if ( _config.hasTinyInt )
					{
						if ( _config.hasUnsignedTiny )
						{
							UniqueTest< EFieldType_UINT8 >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_UINT8 >, connection, STR( "TinyIntField" ), _config.is, generator );
						}
						else
						{
							UniqueTest< EFieldType_SINT8 >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_SINT8 >, connection, STR( "TinyIntField" ), _config.is, generator );
						}
					}

					if ( _config.hasInt24 )
					{
						UniqueTest< EFieldType_SINT24 >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_SINT24 >, connection, STR( "MediumIntField" ), _config.is, generator );
					}

					if ( _config.hasNChar )
					{
						UniqueTest< EFieldType_NCHAR, 55 >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_NCHAR >, connection, STR( "NcharField" ), _config.is, generator );
						UniqueTest< EFieldType_NVARCHAR >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_NVARCHAR >, connection, STR( "NVarcharField" ), _config.is, generator );
					}

					if ( _config.hasSeparateBooleanAndTinyInt )
					{
						UniqueTest< EFieldType_BIT >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_BIT >, connection, STR( "BooleanField" ), _config.is, generator );
					}
				}

				database->RemoveConnection();
			}
		}
	}

	template< size_t StmtType >
	void CDatabaseTest::TestCase_DatabaseDirectQuery()
	{
		auto const guard = make_block_guard( [this]()
		{
			CLogger::LogInfo( StringStream() << "**** Start " << _type << "_TestCase_Database" << DatabaseUtils::Name[StmtType] << "DirectQuery ****" );
			DoLoadPlugins();
		}, [this]()
		{
			UnloadPlugins();
			CLogger::LogInfo( StringStream() << "**** End " << _type << "_TestCase_Database" << DatabaseUtils::Name[StmtType] << "DirectQuery ****" );
		} );
		std::unique_ptr< CDatabase > database( InstantiateDatabase( _type ) );

		if ( database )
		{
			DatabaseConnectionSPtr connection = CreateConnection( *database, _server, _user, _password );

			if ( connection )
			{
				if ( connection->IsConnected() )
				{
					connection->SelectDatabase( _database );
					DoFlushTable( connection );
					std::random_device generator;
					CLogger::LogInfo( StringStream() << " Insertions" );
					BOOST_CHECK_NO_THROW( DatabaseUtils::TestDirectInsert< StmtType >( generator, connection ) );
					CLogger::LogInfo( StringStream() << " Selection" );
					BOOST_CHECK_NO_THROW( DatabaseUtils::TestDirectSelect< StmtType >( generator, connection ) );
					CLogger::LogInfo( StringStream() << " Update" );
					BOOST_CHECK_NO_THROW( DatabaseUtils::TestDirectUpdate< StmtType >( generator, connection ) );
					CLogger::LogInfo( StringStream() << " Deletion" );
					BOOST_CHECK_NO_THROW( DatabaseUtils::TestDirectDelete< StmtType >( generator, connection ) );
				}

				database->RemoveConnection();
			}
		}
	}

	template< size_t StmtType >
	void CDatabaseTest::TestCase_DatabaseStoredProcedure()
	{
		auto const guard = make_block_guard( [this]()
		{
			CLogger::LogInfo( StringStream() << "**** Start " << _type << "_TestCase_Database" << DatabaseUtils::Name[StmtType] << "StoredProcedure ****" );
			DoLoadPlugins();
		}, [this]()
		{
			UnloadPlugins();
			CLogger::LogInfo( StringStream() << "**** End " << _type << "_TestCase_Database" << DatabaseUtils::Name[StmtType] << "StoredProcedure ****" );
		} );
		std::unique_ptr< CDatabase > database( InstantiateDatabase( _type ) );

		if ( database )
		{
			DatabaseConnectionSPtr connection = CreateConnection( *database, _server, _user, _password );

			if ( connection )
			{
				if ( connection->IsConnected() )
				{
					connection->SelectDatabase( _database );
					std::random_device generator;
					CLogger::LogInfo( StringStream() << " No parameter, No Return" );
					BOOST_CHECK_NO_THROW( DatabaseUtils::TestStoredNoParamNoReturn< StmtType >( generator, connection ) );
					CLogger::LogInfo( StringStream() << " No parameter, Return" );
					BOOST_CHECK_NO_THROW( DatabaseUtils::TestStoredNoParamReturn< StmtType >( generator, connection, STR( "" ) ) );
					CLogger::LogInfo( StringStream() << " In parameters, No Return" );
					BOOST_CHECK_NO_THROW( DatabaseUtils::TestStoredInParamNoReturn< StmtType >( generator, connection ) );
					CLogger::LogInfo( StringStream() << " In parameters, Return" );
					BOOST_CHECK_NO_THROW( DatabaseUtils::TestStoredNoParamReturn< StmtType >( generator, connection, STR( "WHERE ACTOR_ID > 5" ) ) );
					CLogger::LogInfo( StringStream() << " In/Out INTEGER parameter, No Return" );
					BOOST_CHECK_NO_THROW( DatabaseUtils::TestStoredInOutParamNoReturn< StmtType >( generator, connection ) );
					CLogger::LogInfo( StringStream() << " In/Out INTEGER and DATETIME parameters, No Return" );
					BOOST_CHECK_NO_THROW( DatabaseUtils::TestStoredInOutDtParamNoReturn< StmtType >( generator, connection ) );
				}

				database->RemoveConnection();
			}
		}
	}

	template< typename Action1, typename Action2 >
	void CDatabaseTest::MultithreadActions()
	{
		std::unique_ptr< CDatabase > database( InstantiateDatabase( _type ) );

		if ( database )
		{
#if defined( NDEBUG )
			int count = 200;
#else
			int count = 200;
#endif
			CDatabase & db = *database;
			String const & dbname = _database;
			db.Initialise( _server, _user, _password );

			std::atomic_bool thread1;
			std::atomic_bool thread2;

			thread1 = false;
			thread2 = false;

			std::thread threads[2] =
			{
				std::thread( [&count, &db, &dbname, &thread1]()
				{
					DatabaseConnectionSPtr connection = db.RetrieveConnection();

					if ( connection )
					{
						if ( connection->IsConnected() )
						{
							connection->SelectDatabase( dbname );
							Action1 action1( connection );

							for ( int i = 0; i < count; ++i )
							{
								action1.Run();
							}

							thread1 = true;
						}

						connection.reset();
						db.RemoveConnection();
					}
				} ),
				std::thread( [&count, &db, &dbname, &thread2]()
				{
					DatabaseConnectionSPtr connection = db.RetrieveConnection();

					if ( connection )
					{
						if ( connection->IsConnected() )
						{
							connection->SelectDatabase( dbname );
							Action2 action2( connection );

							for ( int i = 0; i < count; ++i )
							{
								action2.Run();
							}

							thread2 = true;
						}

						connection.reset();
						db.RemoveConnection();
					}
				} )
			};

			while ( !thread1 || !thread2 )
			{
				std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
			}
					
			threads[0].join();
			threads[1].join();
		}
	}

	template< size_t StmtTypeA, size_t StmtTypeB >
	void CDatabaseTest::TestCase_MultithreadInsertAndInsert()
	{
		auto const guard = make_block_guard( [this]()
		{
			CLogger::LogInfo( StringStream() << "**** Start " << _type << "_TestCase_Multithread" << DatabaseUtils::Name[StmtTypeA] << "InsertAnd" << DatabaseUtils::Name[StmtTypeB] << "Insert ****" );
			DoLoadPlugins();
		}, [this]()
		{
			UnloadPlugins();
			CLogger::LogInfo( StringStream() << "**** End " << _type << "_TestCase_Multithread" << DatabaseUtils::Name[StmtTypeA] << "InsertAnd" << DatabaseUtils::Name[StmtTypeB] << "Insert ****" );
		} );
		MultithreadActions< DatabaseUtils::Insertor< StmtTypeA >, DatabaseUtils::Insertor< StmtTypeB > >();
	}

	template< size_t StmtTypeA, size_t StmtTypeB >
	void CDatabaseTest::TestCase_MultithreadInsertAndUpdate()
	{
		auto const guard = make_block_guard( [this]()
		{
			CLogger::LogInfo( StringStream() << "**** Start " << _type << "_TestCase_Multithread" << DatabaseUtils::Name[StmtTypeA] << "InsertAnd" << DatabaseUtils::Name[StmtTypeB] << "Update ****" );
			DoLoadPlugins();
		}, [this]()
		{
			UnloadPlugins();
			CLogger::LogInfo( StringStream() << "**** End " << _type << "_TestCase_Multithread" << DatabaseUtils::Name[StmtTypeA] << "InsertAnd" << DatabaseUtils::Name[StmtTypeB] << "Update ****" );
		} );
		MultithreadActions< DatabaseUtils::Insertor< StmtTypeA >, DatabaseUtils::Updator< StmtTypeB > >();
	}

	template< size_t StmtTypeA, size_t StmtTypeB >
	void CDatabaseTest::TestCase_MultithreadInsertAndSelect()
	{
		auto const guard = make_block_guard( [this]()
		{
			CLogger::LogInfo( StringStream() << "**** Start " << _type << "_TestCase_Multithread" << DatabaseUtils::Name[StmtTypeA] << "InsertAnd" << DatabaseUtils::Name[StmtTypeB] << "Select ****" );
			DoLoadPlugins();
		}, [this]()
		{
			UnloadPlugins();
			CLogger::LogInfo( StringStream() << "**** End " << _type << "_TestCase_Multithread" << DatabaseUtils::Name[StmtTypeA] << "InsertAnd" << DatabaseUtils::Name[StmtTypeB] << "Select ****" );
		} );
		MultithreadActions< DatabaseUtils::Insertor< StmtTypeA >, DatabaseUtils::Selector< StmtTypeB > >();
	}

	template< size_t StmtTypeA, size_t StmtTypeB >
	void CDatabaseTest::TestCase_MultithreadInsertAndDelete()
	{
		auto const guard = make_block_guard( [this]()
		{
			CLogger::LogInfo( StringStream() << "**** Start " << _type << "_TestCase_Multithread" << DatabaseUtils::Name[StmtTypeA] << "InsertAnd" << DatabaseUtils::Name[StmtTypeB] << "Delete ****" );
			DoLoadPlugins();
		}, [this]()
		{
			UnloadPlugins();
			CLogger::LogInfo( StringStream() << "**** End " << _type << "_TestCase_Multithread" << DatabaseUtils::Name[StmtTypeA] << "InsertAnd" << DatabaseUtils::Name[StmtTypeB] << "Delete ****" );
		} );
		MultithreadActions< DatabaseUtils::Insertor< StmtTypeA >, DatabaseUtils::Deletor< StmtTypeB > >();
	}

	template< size_t StmtTypeA, size_t StmtTypeB >
	void CDatabaseTest::TestCase_MultithreadUpdateAndUpdate()
	{
		auto const guard = make_block_guard( [this]()
		{
			CLogger::LogInfo( StringStream() << "**** Start " << _type << "_TestCase_Multithread" << DatabaseUtils::Name[StmtTypeA] << "UpdateAnd" << DatabaseUtils::Name[StmtTypeB] << "Update ****" );
			DoLoadPlugins();
		}, [this]()
		{
			UnloadPlugins();
			CLogger::LogInfo( StringStream() << "**** End " << _type << "_TestCase_Multithread" << DatabaseUtils::Name[StmtTypeA] << "UpdateAnd" << DatabaseUtils::Name[StmtTypeB] << "Update ****" );
		} );
		MultithreadActions< DatabaseUtils::Updator< StmtTypeA >, DatabaseUtils::Updator< StmtTypeB > >();
	}

	template< size_t StmtTypeA, size_t StmtTypeB >
	void CDatabaseTest::TestCase_MultithreadUpdateAndSelect()
	{
		auto const guard = make_block_guard( [this]()
		{
			CLogger::LogInfo( StringStream() << "**** Start " << _type << "_TestCase_Multithread" << DatabaseUtils::Name[StmtTypeA] << "UpdateAnd" << DatabaseUtils::Name[StmtTypeB] << "Select ****" );
			DoLoadPlugins();
		}, [this]()
		{
			UnloadPlugins();
			CLogger::LogInfo( StringStream() << "**** End " << _type << "_TestCase_Multithread" << DatabaseUtils::Name[StmtTypeA] << "UpdateAnd" << DatabaseUtils::Name[StmtTypeB] << "Select ****" );
		} );
		MultithreadActions< DatabaseUtils::Updator< StmtTypeA >, DatabaseUtils::Selector< StmtTypeB > >();
	}

	template< size_t StmtTypeA, size_t StmtTypeB >
	void CDatabaseTest::TestCase_MultithreadUpdateAndDelete()
	{
		auto const guard = make_block_guard( [this]()
		{
			CLogger::LogInfo( StringStream() << "**** Start " << _type << "_TestCase_Multithread" << DatabaseUtils::Name[StmtTypeA] << "UpdateAnd" << DatabaseUtils::Name[StmtTypeB] << "Delete ****" );
			DoLoadPlugins();
		}, [this]()
		{
			UnloadPlugins();
			CLogger::LogInfo( StringStream() << "**** End " << _type << "_TestCase_Multithread" << DatabaseUtils::Name[StmtTypeA] << "UpdateAnd" << DatabaseUtils::Name[StmtTypeB] << "Delete ****" );
		} );
		MultithreadActions< DatabaseUtils::Updator< StmtTypeA >, DatabaseUtils::Deletor< StmtTypeB > >();
	}

	template< size_t StmtTypeA, size_t StmtTypeB >
	void CDatabaseTest::TestCase_MultithreadSelectAndSelect()
	{
		auto const guard = make_block_guard( [this]()
		{
			CLogger::LogInfo( StringStream() << "**** Start " << _type << "_TestCase_Multithread" << DatabaseUtils::Name[StmtTypeA] << "SelectAnd" << DatabaseUtils::Name[StmtTypeB] << "Select ****" );
			DoLoadPlugins();
		}, [this]()
		{
			UnloadPlugins();
			CLogger::LogInfo( StringStream() << "**** End " << _type << "_TestCase_Multithread" << DatabaseUtils::Name[StmtTypeA] << "SelectAnd" << DatabaseUtils::Name[StmtTypeB] << "Select ****" );
		} );
		MultithreadActions< DatabaseUtils::Selector< StmtTypeA >, DatabaseUtils::Selector< StmtTypeB > >();
	}

	template< size_t StmtTypeA, size_t StmtTypeB >
	void CDatabaseTest::TestCase_MultithreadSelectAndDelete()
	{
		auto const guard = make_block_guard( [this]()
		{
			CLogger::LogInfo( StringStream() << "**** Start " << _type << "_TestCase_Multithread" << DatabaseUtils::Name[StmtTypeA] << "SelectAnd" << DatabaseUtils::Name[StmtTypeB] << "Delete ****" );
			DoLoadPlugins();
		}, [this]()
		{
			UnloadPlugins();
			CLogger::LogInfo( StringStream() << "**** End " << _type << "_TestCase_Multithread" << DatabaseUtils::Name[StmtTypeA] << "SelectAnd" << DatabaseUtils::Name[StmtTypeB] << "Delete ****" );
		} );
		MultithreadActions< DatabaseUtils::Selector< StmtTypeA >, DatabaseUtils::Deletor< StmtTypeB > >();
	}

#if defined( PERF_TEST )
	template< size_t StmtType >
	void CDatabaseTest::TestCase_DatabasePerformances()
	{
		auto const guard = make_block_guard( [this]()
		{
			CLogger::LogInfo( StringStream() << "**** Start " << _type << "_TestCase_Database" << DatabaseUtils::Name[StmtType] << "Performances ****" );
			DoLoadPlugins();
		}, [this]()
		{
			UnloadPlugins();
			CLogger::LogInfo( StringStream() << "**** End " << _type << "_TestCase_Database" << DatabaseUtils::Name[StmtType] << "Performances ****" );
		} );
		std::unique_ptr< CDatabase > database( InstantiateDatabase( _type ) );

		if ( database )
		{
			DatabaseConnectionSPtr connection = CreateConnection( *database, _server, _user, _password );

			if ( connection )
			{
				if ( connection->IsConnected() )
				{
#if defined( NDEBUG )
					uint32_t tests = 100000;
#else
					uint32_t tests = 10;
#endif
					connection->SelectDatabase( _database );
					DoFlushTable( connection );
					CLogger::LogInfo( StringStream() << " Insert elements" );
					PerfDirectInsertActors< StmtType >( connection, tests, String() );
					CLogger::LogInfo( StringStream() << " Select all elements" );
					PerfDirectSelectActors< StmtType >( connection, tests, String() );
					CLogger::LogInfo( StringStream() << " Delete all elements" );
					PerfDirectDeleteActors< StmtType >( connection, tests );
				}

				database->RemoveConnection();
			}
		}
	}
#endif
}
END_NAMESPACE_DATABASE_TEST
