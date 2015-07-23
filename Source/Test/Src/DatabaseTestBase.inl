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

BEGIN_NAMESPACE_DATABASE_TEST
{
	template< typename CleanFunc >
	struct SBlockGuard
	{
		template< typename InitFunc >
		SBlockGuard( InitFunc init, CleanFunc clean )
			: _clean( clean )
		{
			init();
		};

		~SBlockGuard()
		{
			_clean();
		}

		CleanFunc _clean;
	};

	template< typename InitFunc, typename CleanFunc >
	SBlockGuard< CleanFunc > make_block_guard( InitFunc init, CleanFunc clean )
	{
		return SBlockGuard< CleanFunc >( init, clean );
	}

	template< EFieldType FieldType, uint8_t Precision = 0, uint8_t Decimals = 0 >
	struct BatchTests
	{
		typedef DatabaseUtils::Helpers< FieldType > helper_type;
		typedef typename helper_type::ParamType param_type;

		template< typename Function >
		void operator()( Function function, DatabaseConnectionSPtr connection, String const & name, String const & is, param_type valueIn = helper_type::InitialiseValue() )
		{
			param_type value = param_type();

			if ( is == STR( "IS" ) )
			{
				CLogger::LogInfo( StringStream() << "  NULL" );
				function( connection, name, NULL, true, is );
			}

			CLogger::LogInfo( StringStream() << "  Default" );
			function( connection, name, &value, true, is );
			CLogger::LogInfo( StringStream() << "  Lowest" );
			function( connection, name, &( value = std::numeric_limits< param_type >::lowest() ), true, is );
			CLogger::LogInfo( StringStream() << "  Max" );
			function( connection, name, &( value = std::numeric_limits< param_type >::max() ), true, is );
			CLogger::LogInfo( StringStream() << "  Min" );
			function( connection, name, &( value = std::numeric_limits< param_type >::min() ), true, is );
			CLogger::LogInfo( StringStream() << "  Given" );
			function( connection, name, &valueIn, true, is );
		}
	};

	template<>
	struct BatchTests< EFieldType_FLOAT32 >
	{
		typedef DatabaseUtils::Helpers< EFieldType_FLOAT32 > helper_type;
		typedef helper_type::ParamType param_type;

		template< typename Function >
		void operator()( Function function, DatabaseConnectionSPtr connection, String const & name, String const & is, param_type valueIn = helper_type::InitialiseValue() )
		{
			param_type value = param_type();

			if ( is == STR( "IS" ) )
			{
				CLogger::LogInfo( StringStream() << "  NULL" );
				function( connection, name, NULL, true, is );
			}

			CLogger::LogInfo( StringStream() << "  Default" );
			function( connection, name, &value, true, is );
			CLogger::LogInfo( StringStream() << "  Lowest" );
			function( connection, name, &( value = std::numeric_limits< param_type >::lowest() ), true, is );
			CLogger::LogInfo( StringStream() << "  Max" );
			function( connection, name, &( value = std::numeric_limits< param_type >::max() ), true, is );
			CLogger::LogInfo( StringStream() << "  Min" );
			function( connection, name, &( value = std::numeric_limits< param_type >::min() ), true, is );
			CLogger::LogInfo( StringStream() << "  Given" );
			function( connection, name, &valueIn, true, is );
		}
	};

	template<>
	struct BatchTests< EFieldType_FLOAT64 >
	{
		typedef DatabaseUtils::Helpers< EFieldType_FLOAT64 > helper_type;
		typedef helper_type::ParamType param_type;

		template< typename Function >
		void operator()( Function function, DatabaseConnectionSPtr connection, String const & name, String const & is, param_type valueIn = helper_type::InitialiseValue() )
		{
			param_type value = param_type();

			if ( is == STR( "IS" ) )
			{
				CLogger::LogInfo( StringStream() << "  NULL" );
				function( connection, name, NULL, true, is );
			}

			CLogger::LogInfo( StringStream() << "  Default" );
			function( connection, name, &value, true, is );
			CLogger::LogInfo( StringStream() << "  Lowest / 2" );
			function( connection, name, &( value = std::numeric_limits< param_type >::lowest() / 2 ), true, is );
			CLogger::LogInfo( StringStream() << "  Max / 2" );
			function( connection, name, &( value = std::numeric_limits< param_type >::max() / 2 ), true, is );
			CLogger::LogInfo( StringStream() << "  Min" );
			function( connection, name, &( value = std::numeric_limits< param_type >::min() ), true, is );
			CLogger::LogInfo( StringStream() << "  Given" );
			function( connection, name, &valueIn, true, is );
		}
	};

	template<>
	struct BatchTests< EFieldType_DATE >
	{
		typedef DatabaseUtils::Helpers< EFieldType_DATE > helper_type;
		typedef helper_type::ParamType param_type;

		template< typename Function >
		void operator()( Function function, DatabaseConnectionSPtr connection, String const & name, String const & is, param_type valueIn = helper_type::InitialiseValue() )
		{
			param_type value = param_type();

			if ( is == STR( "IS" ) )
			{
				CLogger::LogInfo( StringStream() << "  NULL" );
				function( connection, name, NULL, true, is );
				CLogger::LogInfo( StringStream() << "  Default" );
				function( connection, name, &value, true, is );
			}

			CLogger::LogInfo( StringStream() << "  Given" );
			function( connection, name, &valueIn, true, is );
		}
	};

	template<>
	struct BatchTests< EFieldType_DATETIME >
	{
		typedef DatabaseUtils::Helpers< EFieldType_DATETIME > helper_type;
		typedef helper_type::ParamType param_type;

		template< typename Function >
		void operator()( Function function, DatabaseConnectionSPtr connection, String const & name, String const & is, param_type valueIn = helper_type::InitialiseValue() )
		{
			param_type value = param_type();

			if ( is == STR( "IS" ) )
			{
				CLogger::LogInfo( StringStream() << "  NULL" );
				function( connection, name, NULL, true, is );
				CLogger::LogInfo( StringStream() << "  Default" );
				function( connection, name, &value, true, is );
			}

			CLogger::LogInfo( StringStream() << "  Given" );
			function( connection, name, &valueIn, true, is );
		}
	};

	template<>
	struct BatchTests< EFieldType_TIME >
	{
		typedef DatabaseUtils::Helpers< EFieldType_TIME > helper_type;
		typedef helper_type::ParamType param_type;

		template< typename Function >
		void operator()( Function function, DatabaseConnectionSPtr connection, String const & name, String const & is, param_type valueIn = helper_type::InitialiseValue() )
		{
			param_type value = param_type();

			if ( is == STR( "IS" ) )
			{
				CLogger::LogInfo( StringStream() << "  NULL" );
				function( connection, name, NULL, true, is );
			}

			CLogger::LogInfo( StringStream() << "  Default" );
			function( connection, name, &value, true, is );
			CLogger::LogInfo( StringStream() << "  Given" );
			function( connection, name, &valueIn, true, is );
		}
	};

	template< uint8_t Precision, uint8_t Decimals >
	struct BatchTests< EFieldType_FIXED_POINT, Precision, Decimals >
	{
		typedef DatabaseUtils::Helpers< EFieldType_FIXED_POINT > helper_type;
		typedef helper_type::ParamType param_type;

		template< typename Function >
		void operator()( Function function, DatabaseConnectionSPtr connection, String const & name, String const & is, param_type valueIn = helper_type::InitialiseValue( Precision, Decimals ) )
		{
			typedef DatabaseUtils::Helpers< EFieldType_FIXED_POINT >::ParamType param_type;
			param_type value = param_type();

			if ( is == STR( "IS" ) )
			{
				CLogger::LogInfo( StringStream() << "  NULL" );
				function( connection, name, NULL, true, is );
			}

			CLogger::LogInfo( StringStream() << "  Default" );
			function( connection, name, &value, true, is );
			CLogger::LogInfo( StringStream() << "  Max" );
			function( connection, name, &( value = param_type( ( std::numeric_limits< int64_t >::max() ) % int64_t( pow( 10, Precision ) ), Precision, Decimals ) ), true, is );
			CLogger::LogInfo( StringStream() << "  Min" );
			function( connection, name, &( value = param_type( ( std::numeric_limits< int64_t >::min() ) % int64_t( pow( 10, Precision ) ), Precision, Decimals ) ), true, is );
			CLogger::LogInfo( StringStream() << "  Lowest" );
			function( connection, name, &( value = param_type( ( std::numeric_limits< int64_t >::lowest() ) % int64_t( pow( 10, Precision ) ), Precision, Decimals ) ), true, is );
			CLogger::LogInfo( StringStream() << "  Given" );
			function( connection, name, &valueIn, true, is );
			CLogger::LogInfo( StringStream() << "  Specific value, precision test" );
			function( connection, name, &( value = param_type( 602872352, Precision, Decimals ) ), true, is );
			CLogger::LogInfo( StringStream() << "  Specific value, underflow" );
			function( connection, name, &( value = param_type( 6210, Precision, Decimals ) ), true, is );
		}
	};

	template< uint8_t Size >
	struct BatchTests< EFieldType_CHAR, Size >
	{
		typedef DatabaseUtils::Helpers< EFieldType_CHAR > helper_type;
		typedef helper_type::ParamType param_type;

		template< typename Function >
		void operator()( Function function, DatabaseConnectionSPtr connection, String const & name, String const & is, param_type valueIn = helper_type::InitialiseValue( Size ) )
		{
			typedef DatabaseUtils::Helpers< EFieldType_CHAR >::ParamType param_type;
			param_type value = param_type();

			if ( is == STR( "IS" ) )
			{
				CLogger::LogInfo( StringStream() << "  NULL" );
				function( connection, name, NULL, true, is );
			}

			CLogger::LogInfo( StringStream() << "  Default" );
			function( connection, name, &value, true, is );
			CLogger::LogInfo( StringStream() << "  Truncate" );
			function( connection, name, &( value = DatabaseUtils::Helpers< EFieldType_CHAR >::InitialiseValue( 1024 ) ), true, is );
			CLogger::LogInfo( StringStream() << "  Given" );
			function( connection, name, &valueIn, true, is );
		}
	};

	template<>
	struct BatchTests< EFieldType_VARCHAR >
	{
		typedef DatabaseUtils::Helpers< EFieldType_VARCHAR > helper_type;
		typedef helper_type::ParamType param_type;

		template< typename Function >
		void operator()( Function function, DatabaseConnectionSPtr connection, String const & name, String const & is, param_type valueIn = helper_type::InitialiseValue() )
		{
			typedef DatabaseUtils::Helpers< EFieldType_VARCHAR >::ParamType param_type;
			param_type value = param_type();

			if ( is == STR( "IS" ) )
			{
				CLogger::LogInfo( StringStream() << "  NULL" );
				function( connection, name, NULL, true, is );
			}

			CLogger::LogInfo( StringStream() << "  Default" );
			function( connection, name, &value, true, is );
			CLogger::LogInfo( StringStream() << "  Truncate" );
			function( connection, name, &( value = DatabaseUtils::Helpers< EFieldType_VARCHAR >::InitialiseValue( 1024 ) ), true, is );
			CLogger::LogInfo( StringStream() << "  Given" );
			function( connection, name, &valueIn, true, is );
		}
	};

	template< uint8_t Size >
	struct BatchTests< EFieldType_NCHAR, Size >
	{
		typedef DatabaseUtils::Helpers< EFieldType_NCHAR > helper_type;
		typedef helper_type::ParamType param_type;

		template< typename Function >
		void operator()( Function function, DatabaseConnectionSPtr connection, String const & name, String const & is, param_type valueIn = helper_type::InitialiseValue() )
		{
			typedef DatabaseUtils::Helpers< EFieldType_NCHAR >::ParamType param_type;
			param_type value = param_type();

			if ( is == STR( "IS" ) )
			{
				CLogger::LogInfo( StringStream() << "  NULL" );
				function( connection, name, NULL, true, is );
			}

			CLogger::LogInfo( StringStream() << "  Default" );
			function( connection, name, &value, true, is );
			CLogger::LogInfo( StringStream() << "  Truncate" );
			function( connection, name, &( value = DatabaseUtils::Helpers< EFieldType_NCHAR >::InitialiseValue( 1024 ) ), true, is );
			CLogger::LogInfo( StringStream() << "  Given" );
			function( connection, name, &valueIn, true, is );
		}
	};

	template<>
	struct BatchTests< EFieldType_NVARCHAR >
	{
		typedef DatabaseUtils::Helpers< EFieldType_NVARCHAR > helper_type;
		typedef helper_type::ParamType param_type;

		template< typename Function >
		void operator()( Function function, DatabaseConnectionSPtr connection, String const & name, String const & is, param_type valueIn = helper_type::InitialiseValue() )
		{
			typedef DatabaseUtils::Helpers< EFieldType_NVARCHAR >::ParamType param_type;
			param_type value = param_type();

			if ( is == STR( "IS" ) )
			{
				CLogger::LogInfo( StringStream() << "  NULL" );
				function( connection, name, NULL, true, is );
			}

			CLogger::LogInfo( StringStream() << "  Default" );
			function( connection, name, &value, true, is );
			CLogger::LogInfo( StringStream() << "  Truncate" );
			function( connection, name, &( value = DatabaseUtils::Helpers< EFieldType_NVARCHAR >::InitialiseValue( 1024 ) ), true, is );
			CLogger::LogInfo( StringStream() << "  Given" );
			function( connection, name, &valueIn, true, is );
		}
	};

	template<>
	struct BatchTests< EFieldType_TEXT >
	{
		typedef DatabaseUtils::Helpers< EFieldType_TEXT > helper_type;
		typedef helper_type::ParamType param_type;

		template< typename Function >
		void operator()( Function function, DatabaseConnectionSPtr connection, String const & name, String const & is, param_type valueIn = helper_type::InitialiseValue() )
		{
			typedef DatabaseUtils::Helpers< EFieldType_TEXT >::ParamType param_type;
			param_type value = param_type();

			if ( is == STR( "IS" ) )
			{
				CLogger::LogInfo( StringStream() << "  NULL" );
				function( connection, name, NULL, true, is );
			}

			CLogger::LogInfo( StringStream() << "  Default" );
			function( connection, name, &value, true, is );
			CLogger::LogInfo( StringStream() << "  Big" );
			function( connection, name, &( value = DatabaseUtils::Helpers< EFieldType_TEXT >::InitialiseValue( 1024 * 16 ) ), true, is );
			CLogger::LogInfo( StringStream() << "  Given" );
			function( connection, name, &valueIn, true, is );
		}
	};

	template<>
	struct BatchTests< EFieldType_VARBINARY >
	{
		typedef DatabaseUtils::Helpers< EFieldType_VARBINARY > helper_type;
		typedef helper_type::ParamType param_type;

		template< typename Function >
		void operator()( Function function, DatabaseConnectionSPtr connection, String const & name, String const & is, param_type valueIn = helper_type::InitialiseValue() )
		{
			typedef DatabaseUtils::Helpers< EFieldType_VARBINARY >::ParamType param_type;
			param_type value = param_type();

			if ( is == STR( "IS" ) )
			{
				CLogger::LogInfo( StringStream() << "  NULL" );
				function( connection, name, NULL, true, is );
			}

			CLogger::LogInfo( StringStream() << "  Big" );
			function( connection, name, &( value = DatabaseUtils::Helpers< EFieldType_VARBINARY >::InitialiseValue( 1024 * 16 ) ), true, is );
			CLogger::LogInfo( StringStream() << "  Given" );
			function( connection, name, &valueIn, true, is );
		}
	};

	template< typename StmtType >
	void CDatabaseTest::TestCase_DatabaseFieldsInsertRetrieve( const String & name )
	{
		auto const guard = make_block_guard( [&name, this]()
		{
			CLogger::LogInfo( StringStream() << "**** Start TestCase_Database" << name << "InsertAndRetrieve ****" );
			DoLoadPlugins();
		}, [&name]()
		{
			UnloadPlugins();
			CLogger::LogInfo( StringStream() << "**** End TestCase_Database" << name << "InsertAndRetrieve ****" );
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
					BatchTests< EFieldType_SINT32 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_SINT32 >, connection, STR( "IntegerField" ), _config.is );
					BatchTests< EFieldType_SINT16 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_SINT16 >, connection, STR( "SmallIntField" ), _config.is );
					BatchTests< EFieldType_SINT64 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_SINT64 >, connection, STR( "BigIntField" ), _config.is );
					BatchTests< EFieldType_SINT16 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_SINT16 >, connection, STR( "Int2Field" ), _config.is );
					BatchTests< EFieldType_SINT64 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_SINT64 >, connection, STR( "Int8Field" ), _config.is );
					BatchTests< EFieldType_FLOAT64 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_FLOAT64 >, connection, STR( "RealField" ), _config.is );
					BatchTests< EFieldType_FLOAT64 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_FLOAT64 >, connection, STR( "DoubleField" ), _config.is );
					BatchTests< EFieldType_FLOAT64 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_FLOAT64 >, connection, STR( "DoublePrecisionField" ), _config.is );
					BatchTests< EFieldType_FLOAT32 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_FLOAT32 >, connection, STR( "FloatField" ), _config.is );
					BatchTests< EFieldType_FIXED_POINT, 10, 0 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_FIXED_POINT >, connection, STR( "NumericField" ), _config.is );
					BatchTests< EFieldType_FIXED_POINT, 10, 5 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_FIXED_POINT >, connection, STR( "DecimalField" ), _config.is );
					BatchTests< EFieldType_BIT >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_BIT >, connection, STR( "BooleanField" ), _config.is );
					BatchTests< EFieldType_DATE >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_DATE >, connection, STR( "DateField" ), _config.is );
					BatchTests< EFieldType_DATETIME >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_DATETIME >, connection, STR( "DateTimeField" ), _config.is );
					BatchTests< EFieldType_CHAR, 20 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_CHAR >, connection, STR( "CharacterField" ), _config.is );
					BatchTests< EFieldType_VARCHAR >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_VARCHAR >, connection, STR( "VarcharField" ), _config.is );
					BatchTests< EFieldType_NCHAR, 55 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_NCHAR >, connection, STR( "NcharField" ), _config.is );
					BatchTests< EFieldType_NVARCHAR >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_NVARCHAR >, connection, STR( "NVarcharField" ), _config.is );
					BatchTests< EFieldType_TEXT >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_TEXT >, connection, STR( "TextField" ), _config.is );
					BatchTests< EFieldType_VARBINARY >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_VARBINARY >, connection, STR( "BlobField" ), _config.is );

					if ( _config.hasUnsignedTiny )
					{
						BatchTests< EFieldType_UINT8 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_UINT8 >, connection, STR( "TinyIntField" ), _config.is );
					}
					else
					{
						BatchTests< EFieldType_SINT8 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_SINT8 >, connection, STR( "TinyIntField" ), _config.is );
					}

					if ( _config.hasInt24 )
					{
						BatchTests< EFieldType_SINT24 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_SINT24 >, connection, STR( "MediumIntField" ), _config.is );
					}
				}

				database->RemoveConnection();
			}
		}
	}

	template< typename StmtType >
	void CDatabaseTest::TestCase_DatabaseFieldsInsertRetrieveOtherIndex( const String & name )
	{
		auto const guard = make_block_guard( [&name, this]()
		{
			CLogger::LogInfo( StringStream() << "**** Start TestCase_Database" << name << "InsertAndRetrieveOtherIndex ****" );
			DoLoadPlugins();
		}, [&name]()
		{
			UnloadPlugins();
			CLogger::LogInfo( StringStream() << "**** End TestCase_Database" << name << "InsertAndRetrieveOtherIndex ****" );
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
					DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_SINT32 >( connection, STR( "IntegerField" ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_SINT16 >( connection, STR( "SmallIntField" ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_SINT64 >( connection, STR( "BigIntField" ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_SINT16 >( connection, STR( "Int2Field" ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_SINT64 >( connection, STR( "Int8Field" ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_FLOAT64 >( connection, STR( "RealField" ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_FLOAT64 >( connection, STR( "DoubleField" ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_FLOAT64 >( connection, STR( "DoublePrecisionField" ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_FLOAT32 >( connection, STR( "FloatField" ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_FIXED_POINT >( connection, STR( "NumericField" ), DatabaseUtils::Helpers< EFieldType_FIXED_POINT >::InitialiseValue( 10, 0 ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_FIXED_POINT >( connection, STR( "DecimalField" ), DatabaseUtils::Helpers< EFieldType_FIXED_POINT >::InitialiseValue( 10, 5 ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_BIT >( connection, STR( "BooleanField" ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_DATE >( connection, STR( "DateField" ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_DATETIME >( connection, STR( "DateTimeField" ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_CHAR >( connection, STR( "CharacterField" ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_VARCHAR >( connection, STR( "VarcharField" ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_NCHAR >( connection, STR( "NcharField" ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_NVARCHAR >( connection, STR( "NVarcharField" ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_TEXT >( connection, STR( "TextField" ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_VARBINARY >( connection, STR( "BlobField" ), true, _config.is );

					if ( _config.hasUnsignedTiny )
					{
						DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_UINT8 >( connection, STR( "TinyIntField" ), true, _config.is );
					}
					else
					{
						DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_SINT8 >( connection, STR( "TinyIntField" ), true, _config.is );
					}

					if ( _config.hasInt24 )
					{
						DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_SINT24 >( connection, STR( "MediumIntField" ), true, _config.is );
					}
				}

				database->RemoveConnection();
			}
		}
	}

	template< typename StmtType >
	void CDatabaseTest::TestCase_DatabaseFieldsInsertRetrieveFast( const String & name )
	{
		auto const guard = make_block_guard( [&name, this]()
		{
			CLogger::LogInfo( StringStream() << "**** Start TestCase_Database" << name << "InsertAndRetrieveFast ****" );
			DoLoadPlugins();
		}, [&name]()
		{
			UnloadPlugins();
			CLogger::LogInfo( StringStream() << "**** End TestCase_Database" << name << "InsertAndRetrieveFast ****" );
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
					DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_SINT32 >( connection, STR( "IntegerField" ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_SINT16 >( connection, STR( "SmallIntField" ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_SINT64 >( connection, STR( "BigIntField" ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_SINT16 >( connection, STR( "Int2Field" ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_SINT64 >( connection, STR( "Int8Field" ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_FLOAT64 >( connection, STR( "RealField" ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_FLOAT64 >( connection, STR( "DoubleField" ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_FLOAT64 >( connection, STR( "DoublePrecisionField" ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_FLOAT32 >( connection, STR( "FloatField" ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_FIXED_POINT >( connection, STR( "NumericField" ), DatabaseUtils::Helpers< EFieldType_FIXED_POINT >::InitialiseValue( 10, 0 ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_FIXED_POINT >( connection, STR( "DecimalField" ), DatabaseUtils::Helpers< EFieldType_FIXED_POINT >::InitialiseValue( 10, 5 ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_DATE >( connection, STR( "DateField" ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_DATETIME >( connection, STR( "DateTimeField" ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_CHAR >( connection, STR( "CharacterField" ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_VARCHAR >( connection, STR( "VarcharField" ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_TEXT >( connection, STR( "TextField" ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_VARBINARY >( connection, STR( "BlobField" ), true, _config.is );

					if ( _config.hasTinyInt )
					{
						if ( _config.hasUnsignedTiny )
						{
							DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_UINT8 >( connection, STR( "TinyIntField" ), true, _config.is );
						}
						else
						{
							DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_SINT8 >( connection, STR( "TinyIntField" ), true, _config.is );
						}
					}

					if ( _config.hasInt24 )
					{
						DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_SINT24 >( connection, STR( "MediumIntField" ), true, _config.is );
					}

					if ( _config.hasNChar )
					{
						DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_NCHAR >( connection, STR( "NcharField" ), true, _config.is );
						DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_NVARCHAR >( connection, STR( "NVarcharField" ), true, _config.is );
					}

					if ( _config.hasSeparateBooleanAndTinyInt )
					{
						DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_BIT >( connection, STR( "BooleanField" ), true, _config.is );
					}
				}

				database->RemoveConnection();
			}
		}
	}

	template< typename StmtType >
	void CDatabaseTest::TestCase_DatabaseFieldsInsertRetrieveFastOtherIndex( const String & name )
	{
		auto const guard = make_block_guard( [&name, this]()
		{
			CLogger::LogInfo( StringStream() << "**** Start TestCase_Database" << name << "InsertAndRetrieveFastOtherIndex ****" );
			DoLoadPlugins();
		}, [&name]()
		{
			UnloadPlugins();
			CLogger::LogInfo( StringStream() << "**** End TestCase_Database" << name << "InsertAndRetrieveFastOtherIndex ****" );
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
					DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_SINT32 >( connection, STR( "IntegerField" ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_SINT16 >( connection, STR( "SmallIntField" ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_SINT64 >( connection, STR( "BigIntField" ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_SINT16 >( connection, STR( "Int2Field" ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_SINT64 >( connection, STR( "Int8Field" ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_FLOAT64 >( connection, STR( "RealField" ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_FLOAT64 >( connection, STR( "DoubleField" ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_FLOAT64 >( connection, STR( "DoublePrecisionField" ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_FLOAT32 >( connection, STR( "FloatField" ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_FIXED_POINT >( connection, STR( "NumericField" ), DatabaseUtils::Helpers< EFieldType_FIXED_POINT >::InitialiseValue( 10, 0 ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_FIXED_POINT >( connection, STR( "DecimalField" ), DatabaseUtils::Helpers< EFieldType_FIXED_POINT >::InitialiseValue( 10, 5 ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_DATE >( connection, STR( "DateField" ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_DATETIME >( connection, STR( "DateTimeField" ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_CHAR >( connection, STR( "CharacterField" ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_VARCHAR >( connection, STR( "VarcharField" ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_TEXT >( connection, STR( "TextField" ), true, _config.is );
					DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_VARBINARY >( connection, STR( "BlobField" ), true, _config.is );

					if ( _config.hasTinyInt )
					{
						if ( _config.hasUnsignedTiny )
						{
							DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_UINT8 >( connection, STR( "TinyIntField" ), true, _config.is );
						}
						else
						{
							DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_SINT8 >( connection, STR( "TinyIntField" ), true, _config.is );
						}
					}

					if ( _config.hasInt24 )
					{
						DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_SINT24 >( connection, STR( "MediumIntField" ), true, _config.is );
					}

					if ( _config.hasNChar )
					{
						DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_NCHAR >( connection, STR( "NcharField" ), true, _config.is );
						DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_NVARCHAR >( connection, STR( "NVarcharField" ), true, _config.is );
					}

					if ( _config.hasSeparateBooleanAndTinyInt )
					{
						DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_BIT >( connection, STR( "BooleanField" ), true, _config.is );
					}
				}

				database->RemoveConnection();
			}
		}
	}

	template< typename StmtType >
	void CDatabaseTest::TestCase_DatabaseDirectQuery( const String & name )
	{
		auto const guard = make_block_guard( [&name, this]()
		{
			CLogger::LogInfo( StringStream() << "**** Start TestCase_Database" << name << "DirectQuery ****" );
			DoLoadPlugins();
		}, [&name]()
		{
			UnloadPlugins();
			CLogger::LogInfo( StringStream() << "**** End TestCase_Database" << name << "DirectQuery ****" );
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
					CLogger::LogInfo( StringStream() << " Insertions" );
					DatabaseUtils::TestDirectInsert< StmtType >( connection );
					CLogger::LogInfo( StringStream() << " Selection" );
					DatabaseUtils::TestDirectSelect< StmtType >( connection );
					CLogger::LogInfo( StringStream() << " Update" );
					DatabaseUtils::TestDirectUpdate< StmtType >( connection );
					CLogger::LogInfo( StringStream() << " Deletion" );
					DatabaseUtils::TestDirectDelete< StmtType >( connection );
				}

				database->RemoveConnection();
			}

			database->RemoveConnection();
		}
	}

	template< typename StmtType >
	void CDatabaseTest::TestCase_DatabaseStoredProcedure( const String & name )
	{
		auto const guard = make_block_guard( [&name, this]()
		{
			CLogger::LogInfo( StringStream() << "**** Start TestCase_Database" << name << "StoredProcedure ****" );
			DoLoadPlugins();
		}, [&name]()
		{
			UnloadPlugins();
			CLogger::LogInfo( StringStream() << "**** End TestCase_Database" << name << "StoredProcedure ****" );
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
					CLogger::LogInfo( StringStream() << " No parameter, No Return" );
					DatabaseUtils::TestStoredNoParamNoReturn< StmtType >( connection );
					CLogger::LogInfo( StringStream() << " No parameter, Return" );
					DatabaseUtils::TestStoredNoParamReturn< StmtType >( connection, STR( "" ) );
					CLogger::LogInfo( StringStream() << " In parameters, No Return" );
					DatabaseUtils::TestStoredInParamNoReturn< StmtType >( connection );
					CLogger::LogInfo( StringStream() << " In parameters, Return" );
					DatabaseUtils::TestStoredNoParamReturn< StmtType >( connection, STR( "WHERE ACTOR_ID > 5" ) );
					CLogger::LogInfo( StringStream() << " In/Out INTEGER parameter, No Return" );
					DatabaseUtils::TestStoredInOutParamNoReturn< StmtType >( connection );
					CLogger::LogInfo( StringStream() << " In/Out INTEGER and DATETIME parameters, No Return" );
					DatabaseUtils::TestStoredInOutDtParamNoReturn< StmtType >( connection );
				}

				database->RemoveConnection();
			}
		}
	}

#if defined( PERF_TEST )
	template< typename StmtType >
	void CDatabaseTest::TestCase_DatabasePerformances( const String & name )
	{
		auto const guard = make_block_guard( [&name, this]()
		{
			CLogger::LogInfo( StringStream() << "**** Start TestCase_Database" << name << "Performances ****" );
			DoLoadPlugins();
		}, [&name]()
		{
			UnloadPlugins();
			CLogger::LogInfo( StringStream() << "**** End TestCase_Database" << name << "Performances ****" );
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
