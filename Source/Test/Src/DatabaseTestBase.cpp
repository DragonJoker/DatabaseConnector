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

	CDatabaseTest::CDatabaseTest( const String & type, const String & server, const String & database, const String & user, const String & password )
		: _type( type )
		, _server( server )
		, _database( database )
		, _user( user )
		, _password( password )
	{
	}

	CDatabaseTest::~CDatabaseTest()
	{
	}

	boost::unit_test::test_suite * CDatabaseTest::Init_Test_Suite()
	{
		///@remarks Create the internal TS instance.
		testSuite = new boost::unit_test::test_suite( "CDatabaseSqliteTest" );

		///@remarks Add the TC to the internal TS.
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_CreateDatabase, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseQueryDirectQuery, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseQueryStoredProcedure, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseStatementDirectQuery, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseStatementStoredProcedure, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DestroyDatabase, this ) ) );

		///@remarks Return the TS instance.
		return testSuite;
	}

	void CDatabaseTest::TestCase_CreateDatabase()
	{
		CLogger::LogMessage( StringStream() << "**** Start TestCase_CreateDatabase ****" );
		{
			auto const guard = make_block_guard( [this]()
			{
				DoLoadPlugins();
			}, []()
			{
				UnloadPlugins();
			} );
			std::unique_ptr< Database::CDatabase > database( InstantiateDatabase( _type ) );

			if ( database )
			{
				Database::DatabaseConnectionPtr connection = CreateConnection( *database, _server, _user, _password );
				connection->CreateDatabase( _database );
				connection->SelectDatabase( _database );

				if ( connection )
				{
					String query;
					query += STR( "CREATE TABLE Test" );
					query += STR( "(	IDTest INTEGER PRIMARY KEY\n" );
					query += STR( ",	IntField INT\n" );
					query += STR( ",	IntegerField INTEGER\n" );
					query += STR( ",	TinyIntField TINYINT\n" );
					query += STR( ",	SmallIntField SMALLINT\n" );
					query += STR( ",	MediumIntField MEDIUMINT\n" );
					query += STR( ",	BigIntField BIGINT\n" );
					query += STR( ",	Int2Field INT2\n" );
					query += STR( ",	Int8Field INT8\n" );
					query += STR( ",	RealField REAL\n" );
					query += STR( ",	DoubleField DOUBLE\n" );
					query += STR( ",	DoublePrecisionField DOUBLE PRECISION\n" );
					query += STR( ",	FloatField FLOAT\n" );
					query += STR( ",	NumericField NUMERIC\n" );
					query += STR( ",	DecimalField DECIMAL(10,5)\n" );
					query += STR( ",	BooleanField BOOLEAN\n" );
					query += STR( ",	DateField DATE\n" );
					query += STR( ",	DateTimeField DATETIME\n" );
					query += STR( ",	CharacterField CHARACTER(20)\n" );
					query += STR( ",	VarcharField VARCHAR(255)\n" );
					query += STR( ",	NcharField NCHAR(55)\n" );
					query += STR( ",	NVarcharField NVARCHAR(100)\n" );
					query += STR( ",	TextField TEXT\n" );
					query += STR( ",	BlobField BLOB\n" );
					query += STR( ");" );
					connection->ExecuteUpdate( query );
				}
			}
		}
		CLogger::LogMessage( StringStream() << "**** End TestCase_CreateDatabase ****" );
	}

	void CDatabaseTest::TestCase_DatabaseQueryDirectQuery()
	{
		CLogger::LogMessage( StringStream() << "**** Start TestCase_DatabaseQueryDirectQuery ****" );
		{
			auto const guard = make_block_guard( [this]()
			{
				DoLoadPlugins();
			}, []()
			{
				UnloadPlugins();
			} );
			std::unique_ptr< Database::CDatabase > database( InstantiateDatabase( _type ) );

			if ( database )
			{
				Database::DatabaseConnectionPtr connection = CreateConnection( *database, _server, _user, _password );
				connection->SelectDatabase( _database );

				if ( connection )
				{
					CLogger::LogMessage( StringStream() << " Insertions" );
					DatabaseUtils::TestDirectInsert< CDatabaseQuery >( connection );
					CLogger::LogMessage( StringStream() << " Selection" );
					DatabaseUtils::TestDirectSelect< CDatabaseQuery >( connection );
					CLogger::LogMessage( StringStream() << " Update" );
					DatabaseUtils::TestDirectUpdate< CDatabaseQuery >( connection );
					CLogger::LogMessage( StringStream() << " Deletion" );
					DatabaseUtils::TestDirectDelete< CDatabaseQuery >( connection );

					DatabaseUtils::InsertLanguage< CDatabaseQuery >( connection );
				}

				database->RemoveConnection();
			}
		}
		CLogger::LogMessage( StringStream() << "**** End TestCase_DatabaseQueryDirectQuery ****" );
	}

	void CDatabaseTest::TestCase_DatabaseQueryStoredProcedure()
	{
		CLogger::LogMessage( StringStream() << "**** Start TestCase_DatabaseQueryStoredProcedure ****" );
		{
			auto const guard = make_block_guard( [this]()
			{
				DoLoadPlugins();
			}, []()
			{
				UnloadPlugins();
			} );
			std::unique_ptr< Database::CDatabase > database( InstantiateDatabase( _type ) );

			if ( database )
			{
				Database::DatabaseConnectionPtr connection = CreateConnection( *database, _server, _user, _password );
				connection->SelectDatabase( _database );

				if ( connection )
				{
					CLogger::LogMessage( StringStream() << " No parameter, No Return" );
					DatabaseUtils::TestStoredNoParamNoReturn< CDatabaseQuery >( connection );
					CLogger::LogMessage( StringStream() << " No parameter, Return" );
					DatabaseUtils::TestStoredNoParamReturn< CDatabaseQuery >( connection, STR( "" ) );
					CLogger::LogMessage( StringStream() << " In parameters, No Return" );
					DatabaseUtils::TestStoredInParamNoReturn< CDatabaseQuery >( connection );
					CLogger::LogMessage( StringStream() << " In parameters, Return" );
					DatabaseUtils::TestStoredNoParamReturn< CDatabaseQuery >( connection, STR( "WHERE ACTOR_ID > 5" ) );
					CLogger::LogMessage( StringStream() << " In/Out INTEGER parameter, No Return" );
					DatabaseUtils::TestStoredInOutParamNoReturn< CDatabaseQuery >( connection );
					CLogger::LogMessage( StringStream() << " In/Out INTEGER and DATETIME parameters, No Return" );
					DatabaseUtils::TestStoredInOutDtParamNoReturn< CDatabaseQuery >( connection );
					CLogger::LogMessage( StringStream() << " Update" );
					DatabaseUtils::TestStoredUpdate< CDatabaseQuery >( connection );
				}

				database->RemoveConnection();
			}
		}
		CLogger::LogMessage( StringStream() << "**** End TestCase_DatabaseQueryStoredProcedure ****" );
	}

	void CDatabaseTest::TestCase_DatabaseStatementDirectQuery()
	{
		CLogger::LogMessage( StringStream() << "**** Start TestCase_DatabaseStatementDirectQuery ****" );
		{
			auto const guard = make_block_guard( [this]()
			{
				DoLoadPlugins();
			}, []()
			{
				UnloadPlugins();
			} );
			std::unique_ptr< Database::CDatabase > database( InstantiateDatabase( _type ) );

			if ( database )
			{
				Database::DatabaseConnectionPtr connection = CreateConnection( *database, _server, _user, _password );
				connection->SelectDatabase( _database );

				if ( connection )
				{
					CLogger::LogMessage( StringStream() << " Insertions" );
					DatabaseUtils::TestDirectInsert< CDatabaseStatement >( connection );
					CLogger::LogMessage( StringStream() << " Selection" );
					DatabaseUtils::TestDirectSelect< CDatabaseStatement >( connection );
					CLogger::LogMessage( StringStream() << " Update" );
					DatabaseUtils::TestDirectUpdate< CDatabaseStatement >( connection );
					CLogger::LogMessage( StringStream() << " Deletion" );
					DatabaseUtils::TestDirectDelete< CDatabaseStatement >( connection );

					DatabaseUtils::InsertLanguage< CDatabaseStatement >( connection );
				}

				database->RemoveConnection();
			}
		}
		CLogger::LogMessage( StringStream() << "**** End TestCase_DatabaseStatementDirectQuery ****" );
	}

	void CDatabaseTest::TestCase_DatabaseStatementStoredProcedure()
	{
		CLogger::LogMessage( StringStream() << "**** Start TestCase_DatabaseStatementStoredProcedure ****" );
		{
			auto const guard = make_block_guard( [this]()
			{
				DoLoadPlugins();
			}, []()
			{
				UnloadPlugins();
			} );
			std::unique_ptr< Database::CDatabase > database( InstantiateDatabase( _type ) );

			if ( database )
			{
				Database::DatabaseConnectionPtr connection = CreateConnection( *database, _server, _user, _password );
				connection->SelectDatabase( _database );

				if ( connection )
				{
					CLogger::LogMessage( StringStream() << " No parameter, No Return" );
					DatabaseUtils::TestStoredNoParamNoReturn< CDatabaseStatement >( connection );
					CLogger::LogMessage( StringStream() << " No parameter, Return" );
					DatabaseUtils::TestStoredNoParamReturn< CDatabaseStatement >( connection, STR( "" ) );
					CLogger::LogMessage( StringStream() << " In parameters, No Return" );
					DatabaseUtils::TestStoredInParamNoReturn< CDatabaseStatement >( connection );
					CLogger::LogMessage( StringStream() << " In parameters, Return" );
					DatabaseUtils::TestStoredNoParamReturn< CDatabaseStatement >( connection, STR( "WHERE ACTOR_ID > 5" ) );
					CLogger::LogMessage( StringStream() << " In/Out INTEGER parameter, No Return" );
					DatabaseUtils::TestStoredInOutParamNoReturn< CDatabaseStatement >( connection );
					CLogger::LogMessage( StringStream() << " In/Out INTEGER and DATETIME parameters, No Return" );
					DatabaseUtils::TestStoredInOutDtParamNoReturn< CDatabaseStatement >( connection );
					CLogger::LogMessage( StringStream() << " Update" );
					DatabaseUtils::TestStoredUpdate< CDatabaseStatement >( connection );
				}

				database->RemoveConnection();
			}
		}
		CLogger::LogMessage( StringStream() << "**** End TestCase_DatabaseStatementStoredProcedure ****" );
	}

	void CDatabaseTest::TestCase_DestroyDatabase()
	{
		CLogger::LogMessage( StringStream() << "**** Start TestCase_DestroyDatabase ****" );
		{
			auto const guard = make_block_guard( [this]()
			{
				DoLoadPlugins();
			}, []()
			{
				UnloadPlugins();
			} );
			std::unique_ptr< Database::CDatabase > database( InstantiateDatabase( _type ) );

			if ( database )
			{
				Database::DatabaseConnectionPtr connection = CreateConnection( *database, _server, _user, _password );

				try
				{
					connection->DestroyDatabase( _database );
				}
				catch ( std::exception & )
				{
					BOOST_CHECK( false );
				}

				database->RemoveConnection();
			}
		}
		CLogger::LogMessage( StringStream() << "**** End TestCase_DestroyDatabase ****" );
	}
}
END_NAMESPACE_DATABASE_TEST
