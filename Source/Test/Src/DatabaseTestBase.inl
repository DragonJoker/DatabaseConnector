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
#include "DatabaseTestUtilsUtf8.h"

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

	template< typename Type >
	struct BatchTests
	{
		template< typename Function >
		void operator()( Function function, DatabaseConnectionPtr connection, String const & name, typename DatabaseUtils::Helpers< Type >::ParamType valueIn = DatabaseUtils::Helpers< Type >::InitialiseValue() )
		{
			function( connection, name, std::numeric_limits< DatabaseUtils::Helpers< Type >::ParamType >::lowest() );
			function( connection, name, std::numeric_limits< DatabaseUtils::Helpers< Type >::ParamType >::max() );
			function( connection, name, std::numeric_limits< DatabaseUtils::Helpers< Type >::ParamType >::min() );
			function( connection, name, Type() );
			function( connection, name, valueIn );
		}
	};

	template<>
	struct BatchTests< double >
	{
		template< typename Function >
		void operator()( Function function, DatabaseConnectionPtr connection, String const & name, typename DatabaseUtils::Helpers< double >::ParamType valueIn = DatabaseUtils::Helpers< double >::InitialiseValue() )
		{
			function( connection, name, std::numeric_limits< DatabaseUtils::Helpers< double >::ParamType >::lowest() / 2 );
			function( connection, name, std::numeric_limits< DatabaseUtils::Helpers< double >::ParamType >::max() / 2 );
			function( connection, name, std::numeric_limits< DatabaseUtils::Helpers< double >::ParamType >::min() );
			function( connection, name, double() );
			function( connection, name, valueIn );
		}
	};

	template<>
	struct BatchTests< CDate >
	{
		template< typename Function >
		void operator()( Function function, DatabaseConnectionPtr connection, String const & name, typename DatabaseUtils::Helpers< CDate >::ParamType valueIn = DatabaseUtils::Helpers< CDate >::InitialiseValue() )
		{
			function( connection, name, valueIn );
		}
	};

	template<>
	struct BatchTests< CDateTime >
	{
		template< typename Function >
		void operator()( Function function, DatabaseConnectionPtr connection, String const & name, typename DatabaseUtils::Helpers< CDateTime >::ParamType valueIn = DatabaseUtils::Helpers< CDateTime >::InitialiseValue() )
		{
			function( connection, name, valueIn );
		}
	};

	template<>
	struct BatchTests< CTime >
	{
		template< typename Function >
		void operator()( Function function, DatabaseConnectionPtr connection, String const & name, typename DatabaseUtils::Helpers< CTime >::ParamType valueIn = DatabaseUtils::Helpers< CTime >::InitialiseValue() )
		{
			function( connection, name, valueIn );
		}
	};

	template<>
	struct BatchTests< CFixedPoint >
	{
		template< typename Function >
		void operator()( Function function, DatabaseConnectionPtr connection, String const & name, typename DatabaseUtils::Helpers< CFixedPoint >::ParamType valueIn = DatabaseUtils::Helpers< CFixedPoint >::InitialiseValue() )
		{
			function( connection, name, valueIn );
		}
	};

	template<>
	struct BatchTests< char * >
	{
		template< typename Function >
		void operator()( Function function, DatabaseConnectionPtr connection, String const & name, typename DatabaseUtils::Helpers< char * >::ParamType valueIn = DatabaseUtils::Helpers< char * >::InitialiseValue() )
		{
			function( connection, name, valueIn );
		}
	};

	template<>
	struct BatchTests< wchar_t * >
	{
		template< typename Function >
		void operator()( Function function, DatabaseConnectionPtr connection, String const & name, typename DatabaseUtils::Helpers< wchar_t * >::ParamType valueIn = DatabaseUtils::Helpers< wchar_t * >::InitialiseValue() )
		{
			function( connection, name, valueIn );
		}
	};

	template<>
	struct BatchTests< std::string >
	{
		template< typename Function >
		void operator()( Function function, DatabaseConnectionPtr connection, String const & name, typename DatabaseUtils::Helpers< std::string >::ParamType valueIn = DatabaseUtils::Helpers< std::string >::InitialiseValue() )
		{
			function( connection, name, valueIn );
		}
	};

	template<>
	struct BatchTests< ByteArray >
	{
		template< typename Function >
		void operator()( Function function, DatabaseConnectionPtr connection, String const & name, typename DatabaseUtils::Helpers< ByteArray >::ParamType valueIn = DatabaseUtils::Helpers< ByteArray >::InitialiseValue() )
		{
			function( connection, name, valueIn );
		}
	};

	template< typename StmtType >
	void CDatabaseTest::TestCase_DatabaseFieldsInsertRetrieve()
	{
		auto const guard = make_block_guard( [this]()
		{
			DoLoadPlugins();
		}, []()
		{
			UnloadPlugins();
		} );
		std::unique_ptr< CDatabase > database( InstantiateDatabase( _type ) );

		if ( database )
		{
			DatabaseConnectionPtr connection = CreateConnection( *database, _server, _user, _password );

			if ( connection )
			{
				if ( connection->IsConnected() )
				{
					connection->SelectDatabase( _database );
					DoFlushTable( connection );
					//BatchTests< int32_t >()( &DatabaseUtils::InsertAndRetrieve< StmtType, int32_t >, connection, STR( "IntegerField" ) );
					BatchTests< int8_t >()( &DatabaseUtils::InsertAndRetrieve< StmtType, int8_t >, connection, STR( "TinyIntField" ) );
					BatchTests< int16_t >()( &DatabaseUtils::InsertAndRetrieve< StmtType, int16_t >, connection, STR( "SmallIntField" ) );
					BatchTests< int32_t >()( &DatabaseUtils::InsertAndRetrieve< StmtType, int32_t >, connection, STR( "MediumIntField" ) );
					BatchTests< int64_t >()( &DatabaseUtils::InsertAndRetrieve< StmtType, int64_t >, connection, STR( "BigIntField" ) );
					BatchTests< int16_t >()( &DatabaseUtils::InsertAndRetrieve< StmtType, int16_t >, connection, STR( "Int2Field" ) );
					BatchTests< int64_t >()( &DatabaseUtils::InsertAndRetrieve< StmtType, int64_t >, connection, STR( "Int8Field" ) );
					BatchTests< double >()( &DatabaseUtils::InsertAndRetrieve< StmtType, double >, connection, STR( "RealField" ) );
					BatchTests< double >()( &DatabaseUtils::InsertAndRetrieve< StmtType, double >, connection, STR( "DoubleField" ) );
					BatchTests< double >()( &DatabaseUtils::InsertAndRetrieve< StmtType, double >, connection, STR( "DoublePrecisionField" ) );
					BatchTests< float >()( &DatabaseUtils::InsertAndRetrieve< StmtType, float >, connection, STR( "FloatField" ) );
					BatchTests< CFixedPoint >()( &DatabaseUtils::InsertAndRetrieve< StmtType, CFixedPoint >, connection, STR( "NumericField" ) );
					BatchTests< CFixedPoint >()( &DatabaseUtils::InsertAndRetrieve< StmtType, CFixedPoint >, connection, STR( "DecimalField" ) );
					BatchTests< bool >()( &DatabaseUtils::InsertAndRetrieve< StmtType, bool >, connection, STR( "BooleanField" ) );
					BatchTests< CDate >()( &DatabaseUtils::InsertAndRetrieve< StmtType, CDate >, connection, STR( "DateField" ) );
					BatchTests< CDateTime >()( &DatabaseUtils::InsertAndRetrieve< StmtType, CDateTime >, connection, STR( "DateTimeField" ) );
					BatchTests< char * >()( &DatabaseUtils::InsertAndRetrieve< StmtType, char * >, connection, STR( "CharacterField" ) );
					BatchTests< char * >()( &DatabaseUtils::InsertAndRetrieve< StmtType, char * >, connection, STR( "VarcharField" ) );
					BatchTests< wchar_t * >()( &DatabaseUtils::InsertAndRetrieve< StmtType, wchar_t * >, connection, STR( "NcharField" ) );
					BatchTests< wchar_t * >()( &DatabaseUtils::InsertAndRetrieve< StmtType, wchar_t * >, connection, STR( "NVarcharField" ) );
					BatchTests< std::string >()( &DatabaseUtils::InsertAndRetrieve< StmtType, std::string >, connection, STR( "TextField" ) );
					BatchTests< ByteArray >()( &DatabaseUtils::InsertAndRetrieve< StmtType, ByteArray >, connection, STR( "BlobField" ) );
				}

				database->RemoveConnection();
			}
		}
	}

	template< typename StmtType >
	void CDatabaseTest::TestCase_DatabaseFieldsInsertRetrieveOtherIndex()
	{
		auto const guard = make_block_guard( [this]()
		{
			DoLoadPlugins();
		}, []()
		{
			UnloadPlugins();
		} );
		std::unique_ptr< CDatabase > database( InstantiateDatabase( _type ) );

		if ( database )
		{
			DatabaseConnectionPtr connection = CreateConnection( *database, _server, _user, _password );

			if ( connection )
			{
				if ( connection->IsConnected() )
				{
					connection->SelectDatabase( _database );
					DoFlushTable( connection );
					BatchTests< int32_t >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, int32_t >, connection, STR( "IntegerField" ) );
					BatchTests< int8_t >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, int8_t >, connection, STR( "TinyIntField" ) );
					BatchTests< int16_t >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, int16_t >, connection, STR( "SmallIntField" ) );
					BatchTests< int32_t >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, int32_t >, connection, STR( "MediumIntField" ) );
					BatchTests< int64_t >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, int64_t >, connection, STR( "BigIntField" ) );
					BatchTests< int16_t >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, int16_t >, connection, STR( "Int2Field" ) );
					BatchTests< int64_t >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, int64_t >, connection, STR( "Int8Field" ) );
					BatchTests< double >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, double >, connection, STR( "RealField" ) );
					BatchTests< double >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, double >, connection, STR( "DoubleField" ) );
					BatchTests< double >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, double >, connection, STR( "DoublePrecisionField" ) );
					BatchTests< float >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, float >, connection, STR( "FloatField" ) );
					BatchTests< CFixedPoint >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, CFixedPoint >, connection, STR( "NumericField" ) );
					BatchTests< CFixedPoint >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, CFixedPoint >, connection, STR( "DecimalField" ) );
					BatchTests< bool >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, bool >, connection, STR( "BooleanField" ) );
					BatchTests< CDate >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, CDate >, connection, STR( "DateField" ) );
					BatchTests< CDateTime >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, CDateTime >, connection, STR( "DateTimeField" ) );
					BatchTests< char * >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, char * >, connection, STR( "CharacterField" ) );
					BatchTests< char * >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, char * >, connection, STR( "VarcharField" ) );
					BatchTests< wchar_t * >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, wchar_t * >, connection, STR( "NcharField" ) );
					BatchTests< wchar_t * >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, wchar_t * >, connection, STR( "NVarcharField" ) );
					BatchTests< std::string >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, std::string >, connection, STR( "TextField" ) );
					BatchTests< ByteArray >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, ByteArray >, connection, STR( "BlobField" ) );
				}

				database->RemoveConnection();
			}
		}
	}

	template< typename StmtType >
	void CDatabaseTest::TestCase_DatabaseFieldsInsertRetrieveFast()
	{
		auto const guard = make_block_guard( [this]()
		{
			DoLoadPlugins();
		}, []()
		{
			UnloadPlugins();
		} );
		std::unique_ptr< CDatabase > database( InstantiateDatabase( _type ) );

		if ( database )
		{
			DatabaseConnectionPtr connection = CreateConnection( *database, _server, _user, _password );

			if ( connection )
			{
				if ( connection->IsConnected() )
				{
					connection->SelectDatabase( _database );
					DoFlushTable( connection );
					BatchTests< int32_t >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, int32_t >, connection, STR( "IntegerField" ) );
					BatchTests< int8_t >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, int8_t >, connection, STR( "TinyIntField" ) );
					BatchTests< int16_t >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, int16_t >, connection, STR( "SmallIntField" ) );
					BatchTests< int32_t >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, int32_t >, connection, STR( "MediumIntField" ) );
					BatchTests< int64_t >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, int64_t >, connection, STR( "BigIntField" ) );
					BatchTests< int16_t >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, int16_t >, connection, STR( "Int2Field" ) );
					BatchTests< int64_t >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, int64_t >, connection, STR( "Int8Field" ) );
					BatchTests< double >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, double >, connection, STR( "RealField" ) );
					BatchTests< double >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, double >, connection, STR( "DoubleField" ) );
					BatchTests< double >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, double >, connection, STR( "DoublePrecisionField" ) );
					BatchTests< float >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, float >, connection, STR( "FloatField" ) );
					BatchTests< CFixedPoint >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, CFixedPoint >, connection, STR( "NumericField" ) );
					BatchTests< CFixedPoint >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, CFixedPoint >, connection, STR( "DecimalField" ) );
					BatchTests< bool >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, bool >, connection, STR( "BooleanField" ) );
					BatchTests< CDate >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, CDate >, connection, STR( "DateField" ) );
					BatchTests< CDateTime >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, CDateTime >, connection, STR( "DateTimeField" ) );
					BatchTests< char * >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, char * >, connection, STR( "CharacterField" ) );
					BatchTests< char * >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, char * >, connection, STR( "VarcharField" ) );
					BatchTests< std::string >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, std::string >, connection, STR( "TextField" ) );
					BatchTests< ByteArray >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, ByteArray >, connection, STR( "BlobField" ) );

					if ( _hasNChar )
					{
						BatchTests< wchar_t * >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, wchar_t * >, connection, STR( "NcharField" ) );
						BatchTests< wchar_t * >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, wchar_t * >, connection, STR( "NVarcharField" ) );
					}
				}

				database->RemoveConnection();
			}
		}
	}

	template< typename StmtType >
	void CDatabaseTest::TestCase_DatabaseFieldsInsertRetrieveFastOtherIndex()
	{
		auto const guard = make_block_guard( [this]()
		{
			DoLoadPlugins();
		}, []()
		{
			UnloadPlugins();
		} );
		std::unique_ptr< CDatabase > database( InstantiateDatabase( _type ) );

		if ( database )
		{
			DatabaseConnectionPtr connection = CreateConnection( *database, _server, _user, _password );

			if ( connection )
			{
				if ( connection->IsConnected() )
				{
					connection->SelectDatabase( _database );
					DoFlushTable( connection );
					BatchTests< int32_t >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, int32_t >, connection, STR( "IntegerField" ) );
					BatchTests< int8_t >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, int8_t >, connection, STR( "TinyIntField" ) );
					BatchTests< int16_t >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, int16_t >, connection, STR( "SmallIntField" ) );
					BatchTests< int32_t >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, int32_t >, connection, STR( "MediumIntField" ) );
					BatchTests< int64_t >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, int64_t >, connection, STR( "BigIntField" ) );
					BatchTests< int16_t >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, int16_t >, connection, STR( "Int2Field" ) );
					BatchTests< int64_t >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, int64_t >, connection, STR( "Int8Field" ) );
					BatchTests< double >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, double >, connection, STR( "RealField" ) );
					BatchTests< double >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, double >, connection, STR( "DoubleField" ) );
					BatchTests< double >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, double >, connection, STR( "DoublePrecisionField" ) );
					BatchTests< float >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, float >, connection, STR( "FloatField" ) );
					BatchTests< CFixedPoint >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, CFixedPoint >, connection, STR( "NumericField" ) );
					BatchTests< CFixedPoint >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, CFixedPoint >, connection, STR( "DecimalField" ) );
					BatchTests< bool >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, bool >, connection, STR( "BooleanField" ) );
					BatchTests< CDate >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, CDate >, connection, STR( "DateField" ) );
					BatchTests< CDateTime >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, CDateTime >, connection, STR( "DateTimeField" ) );
					BatchTests< char * >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, char * >, connection, STR( "CharacterField" ) );
					BatchTests< char * >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, char * >, connection, STR( "VarcharField" ) );
					BatchTests< std::string >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, std::string >, connection, STR( "TextField" ) );
					BatchTests< ByteArray >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, ByteArray >, connection, STR( "BlobField" ) );

					if ( _hasNChar )
					{
						BatchTests< wchar_t * >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, wchar_t * >, connection, STR( "NcharField" ) );
						BatchTests< wchar_t * >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, wchar_t * >, connection, STR( "NVarcharField" ) );
					}
				}

				database->RemoveConnection();
			}
		}
	}

	template< typename StmtType >
	void CDatabaseTest::TestCase_DatabaseDirectQuery()
	{
		auto const guard = make_block_guard( [this]()
		{
			DoLoadPlugins();
		}, []()
		{
			UnloadPlugins();
		} );
		std::unique_ptr< CDatabase > database( InstantiateDatabase( _type ) );

		if ( database )
		{
			DatabaseConnectionPtr connection = CreateConnection( *database, _server, _user, _password );

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
	void CDatabaseTest::TestCase_DatabaseStoredProcedure()
	{
		auto const guard = make_block_guard( [this]()
		{
			DoLoadPlugins();
		}, []()
		{
			UnloadPlugins();
		} );
		std::unique_ptr< CDatabase > database( InstantiateDatabase( _type ) );

		if ( database )
		{
			DatabaseConnectionPtr connection = CreateConnection( *database, _server, _user, _password );

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
	void CDatabaseTest::TestCase_DatabasePerformances()
	{
		auto const guard = make_block_guard( [this]()
		{
			DoLoadPlugins();
		}, []()
		{
			UnloadPlugins();
		} );
		std::unique_ptr< CDatabase > database( InstantiateDatabase( _type ) );

		if ( database )
		{
			DatabaseConnectionPtr connection = CreateConnection( *database, _server, _user, _password );

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
