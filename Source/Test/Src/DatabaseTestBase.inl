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
				DatabaseUtils::InsertAndRetrieve< StmtType, int32_t >( connection, STR( "IntegerField" ) );
				DatabaseUtils::InsertAndRetrieve< StmtType, bool >( connection, STR( "TinyIntField" ) );
				DatabaseUtils::InsertAndRetrieve< StmtType, int16_t >( connection, STR( "SmallIntField" ) );
				DatabaseUtils::InsertAndRetrieve< StmtType, int32_t >( connection, STR( "MediumIntField" ) );
				DatabaseUtils::InsertAndRetrieve< StmtType, int64_t >( connection, STR( "BigIntField" ) );
				DatabaseUtils::InsertAndRetrieve< StmtType, int16_t >( connection, STR( "Int2Field" ) );
				DatabaseUtils::InsertAndRetrieve< StmtType, int64_t >( connection, STR( "Int8Field" ) );
				DatabaseUtils::InsertAndRetrieve< StmtType, double >( connection, STR( "RealField" ) );
				DatabaseUtils::InsertAndRetrieve< StmtType, double >( connection, STR( "DoubleField" ) );
				DatabaseUtils::InsertAndRetrieve< StmtType, double >( connection, STR( "DoublePrecisionField" ) );
				DatabaseUtils::InsertAndRetrieve< StmtType, float >( connection, STR( "FloatField" ) );
				//DatabaseUtils::InsertAndRetrieve< StmtType, int32_t >( connection, STR( "NumericField" ) );
				DatabaseUtils::InsertAndRetrieve< StmtType, double >( connection, STR( "DecimalField" ) );
				DatabaseUtils::InsertAndRetrieve< StmtType, bool >( connection, STR( "BooleanField" ) );
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
				DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, int32_t >( connection, STR( "IntegerField" ) );
				DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, bool >( connection, STR( "TinyIntField" ) );
				DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, int16_t >( connection, STR( "SmallIntField" ) );
				DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, int32_t >( connection, STR( "MediumIntField" ) );
				DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, int64_t >( connection, STR( "BigIntField" ) );
				DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, int16_t >( connection, STR( "Int2Field" ) );
				DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, int64_t >( connection, STR( "Int8Field" ) );
				DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, double >( connection, STR( "RealField" ) );
				DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, double >( connection, STR( "DoubleField" ) );
				DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, double >( connection, STR( "DoublePrecisionField" ) );
				DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, float >( connection, STR( "FloatField" ) );
				//DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, int32_t >( connection, STR( "NumericField" ) );
				DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, double >( connection, STR( "DecimalField" ) );
				DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, bool >( connection, STR( "BooleanField" ) );
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
				DatabaseUtils::InsertAndRetrieveFast< StmtType, int32_t >( connection, STR( "IntegerField" ) );
				DatabaseUtils::InsertAndRetrieveFast< StmtType, bool >( connection, STR( "TinyIntField" ) );
				DatabaseUtils::InsertAndRetrieveFast< StmtType, int16_t >( connection, STR( "SmallIntField" ) );
				DatabaseUtils::InsertAndRetrieveFast< StmtType, int32_t >( connection, STR( "MediumIntField" ) );
				DatabaseUtils::InsertAndRetrieveFast< StmtType, int64_t >( connection, STR( "BigIntField" ) );
				DatabaseUtils::InsertAndRetrieveFast< StmtType, int16_t >( connection, STR( "Int2Field" ) );
				DatabaseUtils::InsertAndRetrieveFast< StmtType, int64_t >( connection, STR( "Int8Field" ) );
				DatabaseUtils::InsertAndRetrieveFast< StmtType, double >( connection, STR( "RealField" ) );
				DatabaseUtils::InsertAndRetrieveFast< StmtType, double >( connection, STR( "DoubleField" ) );
				DatabaseUtils::InsertAndRetrieveFast< StmtType, double >( connection, STR( "DoublePrecisionField" ) );
				DatabaseUtils::InsertAndRetrieveFast< StmtType, float >( connection, STR( "FloatField" ) );
				//DatabaseUtils::InsertAndRetrieveFast< StmtType, int32_t >( connection, STR( "NumericField" ) );
				DatabaseUtils::InsertAndRetrieveFast< StmtType, double >( connection, STR( "DecimalField" ) );
				DatabaseUtils::InsertAndRetrieveFast< StmtType, bool >( connection, STR( "BooleanField" ) );
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
				DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, int32_t >( connection, STR( "IntegerField" ) );
				DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, bool >( connection, STR( "TinyIntField" ) );
				DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, int16_t >( connection, STR( "SmallIntField" ) );
				DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, int32_t >( connection, STR( "MediumIntField" ) );
				DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, int64_t >( connection, STR( "BigIntField" ) );
				DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, int16_t >( connection, STR( "Int2Field" ) );
				DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, int64_t >( connection, STR( "Int8Field" ) );
				DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, double >( connection, STR( "RealField" ) );
				DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, double >( connection, STR( "DoubleField" ) );
				DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, double >( connection, STR( "DoublePrecisionField" ) );
				DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, float >( connection, STR( "FloatField" ) );
				//DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, int32_t >( connection, STR( "NumericField" ) );
				DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, double >( connection, STR( "DecimalField" ) );
				DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, bool >( connection, STR( "BooleanField" ) );
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
