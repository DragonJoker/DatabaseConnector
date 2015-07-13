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

	template< typename Type, typename Function >
	void BatchTests( Function function, DatabaseConnectionPtr connection, String const & name, typename DatabaseUtils::Helpers< Type >::ParamType valueIn = DatabaseUtils::Helpers< Type >::InitialiseValue() )
	{
		function( connection, name, std::numeric_limits< DatabaseUtils::Helpers< Type >::ParamType >::lowest() );
		function( connection, name, std::numeric_limits< DatabaseUtils::Helpers< Type >::ParamType >::max() );
		function( connection, name, std::numeric_limits< DatabaseUtils::Helpers< Type >::ParamType >::min() );
		function( connection, name, Type() );
		function( connection, name, valueIn );
	}

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

			if ( connection && connection->IsConnected() )
			{
				connection->SelectDatabase( _database );
				DoFlushTable( connection );
				BatchTests< int32_t >( &DatabaseUtils::InsertAndRetrieve< StmtType, int32_t >, connection, STR( "IntegerField" ) );
				BatchTests< bool >( &DatabaseUtils::InsertAndRetrieve< StmtType, bool >, connection, STR( "TinyIntField" ) );
				BatchTests< int16_t >( &DatabaseUtils::InsertAndRetrieve< StmtType, int16_t >, connection, STR( "SmallIntField" ) );
				BatchTests< int32_t >( &DatabaseUtils::InsertAndRetrieve< StmtType, int32_t >, connection, STR( "MediumIntField" ) );
				BatchTests< int64_t >( &DatabaseUtils::InsertAndRetrieve< StmtType, int64_t >, connection, STR( "BigIntField" ) );
				BatchTests< int16_t >( &DatabaseUtils::InsertAndRetrieve< StmtType, int16_t >, connection, STR( "Int2Field" ) );
				BatchTests< int64_t >( &DatabaseUtils::InsertAndRetrieve< StmtType, int64_t >, connection, STR( "Int8Field" ) );
				BatchTests< double >( &DatabaseUtils::InsertAndRetrieve< StmtType, double >, connection, STR( "RealField" ) );
				BatchTests< double >( &DatabaseUtils::InsertAndRetrieve< StmtType, double >, connection, STR( "DoubleField" ) );
				BatchTests< double >( &DatabaseUtils::InsertAndRetrieve< StmtType, double >, connection, STR( "DoublePrecisionField" ) );
				BatchTests< float >( &DatabaseUtils::InsertAndRetrieve< StmtType, float >, connection, STR( "FloatField" ) );
				//BatchTests< int32_t >( &DatabaseUtils::InsertAndRetrieve< StmtType, int32_t >, connection, STR( "NumericField" ) );
				BatchTests< double >( &DatabaseUtils::InsertAndRetrieve< StmtType, double >, connection, STR( "DecimalField" ) );
				BatchTests< bool >( &DatabaseUtils::InsertAndRetrieve< StmtType, bool >, connection, STR( "BooleanField" ) );
				DatabaseUtils::InsertAndRetrieve< StmtType, CDate >( connection, STR( "DateField" ) );
				DatabaseUtils::InsertAndRetrieve< StmtType, CDateTime >( connection, STR( "DateTimeField" ) );
				DatabaseUtils::InsertAndRetrieve< StmtType, char * >( connection, STR( "CharacterField" ) );
				DatabaseUtils::InsertAndRetrieve< StmtType, char * >( connection, STR( "VarcharField" ) );
				DatabaseUtils::InsertAndRetrieve< StmtType, wchar_t * >( connection, STR( "NcharField" ) );
				DatabaseUtils::InsertAndRetrieve< StmtType, wchar_t * >( connection, STR( "NVarcharField" ) );
				DatabaseUtils::InsertAndRetrieve< StmtType, std::string >( connection, STR( "TextField" ) );
				DatabaseUtils::InsertAndRetrieve< StmtType, std::vector< uint8_t > >( connection, STR( "BlobField" ) );
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

			if ( connection && connection->IsConnected() )
			{
				connection->SelectDatabase( _database );
				DoFlushTable( connection );
				BatchTests< int32_t >( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, int32_t >, connection, STR( "IntegerField" ) );
				BatchTests< bool >( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, bool >, connection, STR( "TinyIntField" ) );
				BatchTests< int16_t >( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, int16_t >, connection, STR( "SmallIntField" ) );
				BatchTests< int32_t >( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, int32_t >, connection, STR( "MediumIntField" ) );
				BatchTests< int64_t >( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, int64_t >, connection, STR( "BigIntField" ) );
				BatchTests< int16_t >( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, int16_t >, connection, STR( "Int2Field" ) );
				BatchTests< int64_t >( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, int64_t >, connection, STR( "Int8Field" ) );
				BatchTests< double >( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, double >, connection, STR( "RealField" ) );
				BatchTests< double >( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, double >, connection, STR( "DoubleField" ) );
				BatchTests< double >( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, double >, connection, STR( "DoublePrecisionField" ) );
				BatchTests< float >( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, float >, connection, STR( "FloatField" ) );
				//BatchTests< int32_t >( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, int32_t >, connection, STR( "NumericField" ) );
				BatchTests< double >( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, double >, connection, STR( "DecimalField" ) );
				BatchTests< bool >( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, bool >, connection, STR( "BooleanField" ) );
				DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, CDate >( connection, STR( "DateField" ) );
				DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, CDateTime >( connection, STR( "DateTimeField" ) );
				DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, char * >( connection, STR( "CharacterField" ) );
				DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, char * >( connection, STR( "VarcharField" ) );
				DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, wchar_t * >( connection, STR( "NcharField" ) );
				DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, wchar_t * >( connection, STR( "NVarcharField" ) );
				DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, std::string >( connection, STR( "TextField" ) );
				DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, std::vector< uint8_t > >( connection, STR( "BlobField" ) );
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

			if ( connection && connection->IsConnected() )
			{
				connection->SelectDatabase( _database );
				DoFlushTable( connection );
				BatchTests< int32_t >( &DatabaseUtils::InsertAndRetrieveFast< StmtType, int32_t >, connection, STR( "IntegerField" ) );
				BatchTests< bool >( &DatabaseUtils::InsertAndRetrieveFast< StmtType, bool >, connection, STR( "TinyIntField" ) );
				BatchTests< int16_t >( &DatabaseUtils::InsertAndRetrieveFast< StmtType, int16_t >, connection, STR( "SmallIntField" ) );
				BatchTests< int32_t >( &DatabaseUtils::InsertAndRetrieveFast< StmtType, int32_t >, connection, STR( "MediumIntField" ) );
				BatchTests< int64_t >( &DatabaseUtils::InsertAndRetrieveFast< StmtType, int64_t >, connection, STR( "BigIntField" ) );
				BatchTests< int16_t >( &DatabaseUtils::InsertAndRetrieveFast< StmtType, int16_t >, connection, STR( "Int2Field" ) );
				BatchTests< int64_t >( &DatabaseUtils::InsertAndRetrieveFast< StmtType, int64_t >, connection, STR( "Int8Field" ) );
				BatchTests< double >( &DatabaseUtils::InsertAndRetrieveFast< StmtType, double >, connection, STR( "RealField" ) );
				BatchTests< double >( &DatabaseUtils::InsertAndRetrieveFast< StmtType, double >, connection, STR( "DoubleField" ) );
				BatchTests< double >( &DatabaseUtils::InsertAndRetrieveFast< StmtType, double >, connection, STR( "DoublePrecisionField" ) );
				BatchTests< float >( &DatabaseUtils::InsertAndRetrieveFast< StmtType, float >, connection, STR( "FloatField" ) );
				//BatchTests< int32_t >( &DatabaseUtils::InsertAndRetrieveFast< StmtType, int32_t >, connection, STR( "NumericField" ) );
				BatchTests< double >( &DatabaseUtils::InsertAndRetrieveFast< StmtType, double >, connection, STR( "DecimalField" ) );
				BatchTests< bool >( &DatabaseUtils::InsertAndRetrieveFast< StmtType, bool >, connection, STR( "BooleanField" ) );
				DatabaseUtils::InsertAndRetrieveFast< StmtType, CDate >( connection, STR( "DateField" ) );
				DatabaseUtils::InsertAndRetrieveFast< StmtType, CDateTime >( connection, STR( "DateTimeField" ) );
				DatabaseUtils::InsertAndRetrieveFast< StmtType, char * >( connection, STR( "CharacterField" ) );
				DatabaseUtils::InsertAndRetrieveFast< StmtType, char * >( connection, STR( "VarcharField" ) );
				// Can't do that in fast, since at least MySQL and SQLite don't support them
				// DatabaseUtils::InsertAndRetrieveFast< StmtType, wchar_t * >( connection, STR( "NcharField" ) );
				// DatabaseUtils::InsertAndRetrieveFast< StmtType, wchar_t * >( connection, STR( "NVarcharField" ) );
				DatabaseUtils::InsertAndRetrieveFast< StmtType, std::string >( connection, STR( "TextField" ) );
				DatabaseUtils::InsertAndRetrieveFast< StmtType, std::vector< uint8_t > >( connection, STR( "BlobField" ) );
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

			if ( connection && connection->IsConnected() )
			{
				connection->SelectDatabase( _database );
				DoFlushTable( connection );
				BatchTests< int32_t >( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, int32_t >, connection, STR( "IntegerField" ) );
				BatchTests< bool >( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, bool >, connection, STR( "TinyIntField" ) );
				BatchTests< int16_t >( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, int16_t >, connection, STR( "SmallIntField" ) );
				BatchTests< int32_t >( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, int32_t >, connection, STR( "MediumIntField" ) );
				BatchTests< int64_t >( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, int64_t >, connection, STR( "BigIntField" ) );
				BatchTests< int16_t >( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, int16_t >, connection, STR( "Int2Field" ) );
				BatchTests< int64_t >( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, int64_t >, connection, STR( "Int8Field" ) );
				BatchTests< double >( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, double >, connection, STR( "RealField" ) );
				BatchTests< double >( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, double >, connection, STR( "DoubleField" ) );
				BatchTests< double >( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, double >, connection, STR( "DoublePrecisionField" ) );
				BatchTests< float >( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, float >, connection, STR( "FloatField" ) );
				//BatchTests< int32_t >( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, int32_t >, connection, STR( "NumericField" ) );
				BatchTests< double >( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, double >, connection, STR( "DecimalField" ) );
				BatchTests< bool >( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, bool >, connection, STR( "BooleanField" ) );
				DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, CDate >( connection, STR( "DateField" ) );
				DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, CDateTime >( connection, STR( "DateTimeField" ) );
				DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, char * >( connection, STR( "CharacterField" ) );
				DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, char * >( connection, STR( "VarcharField" ) );
				// Can't do that in fast, since at least MySQL and SQLite don't support them
				//DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, wchar_t * >( connection, STR( "NcharField" ) );
				//DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, wchar_t * >( connection, STR( "NVarcharField" ) );
				DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, std::string >( connection, STR( "TextField" ) );
				DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, std::vector< uint8_t > >( connection, STR( "BlobField" ) );
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

			if ( connection && connection->IsConnected() )
			{
				connection->SelectDatabase( _database );
				DoFlushTable( connection );
				CLogger::LogMessage( StringStream() << " Insertions" );
				DatabaseUtils::TestDirectInsert< StmtType >( connection );
				CLogger::LogMessage( StringStream() << " Selection" );
				DatabaseUtils::TestDirectSelect< StmtType >( connection );
				CLogger::LogMessage( StringStream() << " Update" );
				DatabaseUtils::TestDirectUpdate< StmtType >( connection );
				CLogger::LogMessage( StringStream() << " Deletion" );
				DatabaseUtils::TestDirectDelete< StmtType >( connection );
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

			if ( connection && connection->IsConnected() )
			{
				connection->SelectDatabase( _database );
				CLogger::LogMessage( StringStream() << " No parameter, No Return" );
				DatabaseUtils::TestStoredNoParamNoReturn< StmtType >( connection );
				CLogger::LogMessage( StringStream() << " No parameter, Return" );
				DatabaseUtils::TestStoredNoParamReturn< StmtType >( connection, STR( "" ) );
				CLogger::LogMessage( StringStream() << " In parameters, No Return" );
				DatabaseUtils::TestStoredInParamNoReturn< StmtType >( connection );
				CLogger::LogMessage( StringStream() << " In parameters, Return" );
				DatabaseUtils::TestStoredNoParamReturn< StmtType >( connection, STR( "WHERE ACTOR_ID > 5" ) );
				CLogger::LogMessage( StringStream() << " In/Out INTEGER parameter, No Return" );
				DatabaseUtils::TestStoredInOutParamNoReturn< StmtType >( connection );
				CLogger::LogMessage( StringStream() << " In/Out INTEGER and DATETIME parameters, No Return" );
				DatabaseUtils::TestStoredInOutDtParamNoReturn< StmtType >( connection );
			}

			database->RemoveConnection();
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

			if ( connection && connection->IsConnected() )
			{
#if defined( NDEBUG )
				uint32_t tests = 100000;
#else
				uint32_t tests = 10;
#endif
				connection->SelectDatabase( _database );
				DoFlushTable( connection );
				CLogger::LogMessage( StringStream() << " Insert elements" );
				PerfDirectInsertActors< StmtType >( connection, tests, String() );
				CLogger::LogMessage( StringStream() << " Select all elements" );
				PerfDirectSelectActors< StmtType >( connection, tests, String() );
				CLogger::LogMessage( StringStream() << " Delete all elements" );
				PerfDirectDeleteActors< StmtType >( connection, tests );
			}

			database->RemoveConnection();
		}
	}
#endif
}
END_NAMESPACE_DATABASE_TEST
