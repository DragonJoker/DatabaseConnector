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
	String const QUERY_DROP_TABLE = STR( "DROP TABLE Test" );
	String const QUERY_SELECT_MIN = STR( "SELECT MIN( IDTest ) AS TestID\n" )
	STR( "FROM\n" )
	STR( "	Test" );
	String const QUERY_GET_COUNT = STR( "SELECT COUNT( IDTest ) AS TestID\n" )
	STR( "FROM\n" )
	STR( "	Test" );
	String const QUERY_INSERT_ELEMENT = STR( "INSERT INTO Test\n" )
	STR( "(\n" )
	STR( "	IntField, IntegerField, TinyIntField, SmallIntField, MediumIntField, BigIntField, Int2Field, Int8Field,\n" )
	STR( "	RealField, DoubleField, DoublePrecisionField, FloatField, NumericField, DecimalField, BooleanField,\n" )
	STR( "	DateField, DateTimeField,\n" )
	STR( "	CharacterField, VarcharField,\n" )
	STR( "	NcharField, NVarcharField,\n" )
	STR( "	TextField, BlobField" )
	STR( ")\n" )
	STR( "VALUES\n" )
	STR( "(\n" )
	STR( "	?, ?, ?, ?, ?, ?, ?, ?,\n" )
	STR( "	?, ?, ?, ?, ?, ?, ?,\n" )
	STR( "	?, ?,\n" )
	STR( "	?, ?,\n" )
	STR( "	?, ?,\n" )
	STR( "	?, ?\n" )
	STR( ")" );
	String const QUERY_DIRECT_SELECT_ELEMENT = STR( "SELECT IntField\n" )
	STR( "	, IntegerField\n" )
	STR( "	, TinyIntField\n" )
	STR( "	, SmallIntField\n" )
	STR( "	, MediumIntField\n" )
	STR( "	, BigIntField\n" )
	STR( "	, Int2Field\n" )
	STR( "	, Int8Field\n" )
	STR( "	, RealField\n" )
	STR( "	, DoubleField\n" )
	STR( "	, DoublePrecisionField\n" )
	STR( "	, FloatField\n" )
	STR( "	, NumericField\n" )
	STR( "	, DecimalField\n" )
	STR( "	, BooleanField\n" )
	STR( "	, DateField\n" )
	STR( "	, DateTimeField\n" )
	STR( "	, CharacterField\n" )
	STR( "	, VarcharField\n" )
	STR( "	, NcharField\n" )
	STR( "	, NVarcharField\n" )
	STR( "	, TextField\n" )
	STR( "	, BlobField\n" )
	STR( "FROM\n" )
	STR( "	Test\n" )
	STR( "WHERE\n" )
	STR( "	IDTest = ?" );
	String const QUERY_DIRECT_SELECT_ALL_ELEMENTS = STR( "SELECT IntField\n" )
	STR( "	, IntegerField\n" )
	STR( "	, TinyIntField\n" )
	STR( "	, SmallIntField\n" )
	STR( "	, MediumIntField\n" )
	STR( "	, BigIntField\n" )
	STR( "	, Int2Field\n" )
	STR( "	, Int8Field\n" )
	STR( "	, RealField\n" )
	STR( "	, DoubleField\n" )
	STR( "	, DoublePrecisionField\n" )
	STR( "	, FloatField\n" )
	STR( "	, NumericField\n" )
	STR( "	, DecimalField\n" )
	STR( "	, BooleanField\n" )
	STR( "	, DateField\n" )
	STR( "	, DateTimeField\n" )
	STR( "	, CharacterField\n" )
	STR( "	, VarcharField\n" )
	STR( "	, NcharField\n" )
	STR( "	, NVarcharField\n" )
	STR( "	, TextField\n" )
	STR( "	, BlobField\n" )
	STR( "FROM\n" )
	STR( "	Test" );
	String const QUERY_DIRECT_UPDATE_ELEMENT = STR( "UPDATE\n" )
	STR( "	Test\n" )
	STR( "SET IntField=?\n" )
	STR( "	, IntegerField=?\n" )
	STR( "	, TinyIntField=?\n" )
	STR( "	, SmallIntField=?\n" )
	STR( "	, MediumIntField=?\n" )
	STR( "	, BigIntField=?\n" )
	STR( "	, Int2Field=?\n" )
	STR( "	, Int8Field=?\n" )
	STR( "	, RealField=?\n" )
	STR( "	, DoubleField=?\n" )
	STR( "	, DoublePrecisionField=?\n" )
	STR( "	, FloatField=?\n" )
	STR( "	, NumericField=?\n" )
	STR( "	, DecimalField=?\n" )
	STR( "	, BooleanField=?\n" )
	STR( "	, DateField=?\n" )
	STR( "	, DateTimeField=?\n" )
	STR( "	, CharacterField=?\n" )
	STR( "	, VarcharField=?\n" )
	STR( "	, NcharField=?\n" )
	STR( "	, NVarcharField=?\n" )
	STR( "	, TextField=?\n" )
	STR( "	, BlobField=?\n" )
	STR( "WHERE\n" )
	STR( "	IDTest=?" );
	String const QUERY_DIRECT_DELETE_ELEMENT = STR( "DELETE FROM Test WHERE IDTest=?" );

	CDatabaseTest::CDatabaseTest( const String & type, const String & server, const String & database, const String & user, const String & password, bool hasNChar )
		: _type( type )
		, _server( server )
		, _database( database )
		, _user( user )
		, _password( password )
		, _hasNChar( hasNChar )
	{
	}

	CDatabaseTest::~CDatabaseTest()
	{
	}

	boost::unit_test::test_suite * CDatabaseTest::Init_Test_Suite()
	{
		//!@remarks Create the internal TS instance.
		testSuite = new boost::unit_test::test_suite( "CDatabaseSqliteTest" );

		//!@remarks Add the TC to the internal TS.
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_CreateDatabase, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseQueryFieldsInsertRetrieve, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseQueryFieldsInsertRetrieveOtherIndex, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseQueryFieldsInsertRetrieveFast, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseQueryFieldsInsertRetrieveFastOtherIndex, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseQueryDirectQuery, this ) ) );
		//testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseQueryStoredProcedure, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseStatementFieldsInsertRetrieve, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseStatementFieldsInsertRetrieveOtherIndex, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseStatementFieldsInsertRetrieveFast, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseStatementFieldsInsertRetrieveFastOtherIndex, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseStatementDirectQuery, this ) ) );
		//testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseStatementStoredProcedure, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DestroyDatabase, this ) ) );

		//!@remarks Return the TS instance.
		return testSuite;
	}

	void CDatabaseTest::TestCase_CreateDatabase()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_CreateDatabase ****" );
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
						connection->CreateDatabase( _database );
						connection->SelectDatabase( _database );
						connection->ExecuteUpdate( _createTable );
					}

					database->RemoveConnection();
				}
			}
		}
		CLogger::LogInfo( StringStream() << "**** End TestCase_CreateDatabase ****" );
	}

	void CDatabaseTest::TestCase_DatabaseQueryFieldsInsertRetrieve()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseQueryFieldsInsertRetrieve ****" );
		TestCase_DatabaseFieldsInsertRetrieve< CDatabaseQuery >();
		CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseQueryFieldsInsertRetrieve ****" );
	}

	void CDatabaseTest::TestCase_DatabaseQueryFieldsInsertRetrieveOtherIndex()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseQueryFieldsInsertRetrieveOtherIndex ****" );
		TestCase_DatabaseFieldsInsertRetrieveOtherIndex< CDatabaseQuery >();
		CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseQueryFieldsInsertRetrieveOtherIndex ****" );
	}

	void CDatabaseTest::TestCase_DatabaseQueryFieldsInsertRetrieveFast()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseQueryFieldsInsertRetrieveFast ****" );
		TestCase_DatabaseFieldsInsertRetrieveFast< CDatabaseQuery >();
		CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseQueryFieldsInsertRetrieveFast ****" );
	}

	void CDatabaseTest::TestCase_DatabaseQueryFieldsInsertRetrieveFastOtherIndex()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseQueryFieldsInsertRetrieveFastOtherIndex ****" );
		TestCase_DatabaseFieldsInsertRetrieveFastOtherIndex< CDatabaseQuery >();
		CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseQueryFieldsInsertRetrieveFastOtherIndex ****" );
	}

	void CDatabaseTest::TestCase_DatabaseQueryDirectQuery()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseQueryDirectQuery ****" );
		TestCase_DatabaseDirectQuery< CDatabaseQuery >();
		CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseQueryDirectQuery ****" );
	}

	void CDatabaseTest::TestCase_DatabaseQueryStoredProcedure()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseQueryStoredProcedure ****" );
		TestCase_DatabaseStoredProcedure< CDatabaseQuery >();
		CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseQueryStoredProcedure ****" );
	}

	void CDatabaseTest::TestCase_DatabaseStatementFieldsInsertRetrieve()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseStatementFieldsInsertRetrieve ****" );
		TestCase_DatabaseFieldsInsertRetrieve< CDatabaseStatement >();
		CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseStatementFieldsInsertRetrieve ****" );
	}

	void CDatabaseTest::TestCase_DatabaseStatementFieldsInsertRetrieveOtherIndex()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseStatementFieldsInsertRetrieveOtherIndex ****" );
		TestCase_DatabaseFieldsInsertRetrieveOtherIndex< CDatabaseStatement >();
		CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseStatementFieldsInsertRetrieveOtherIndex ****" );
	}

	void CDatabaseTest::TestCase_DatabaseStatementFieldsInsertRetrieveFast()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseStatementFieldsInsertRetrieve ****" );
		TestCase_DatabaseFieldsInsertRetrieveFast< CDatabaseStatement >();
		CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseStatementFieldsInsertRetrieve ****" );
	}

	void CDatabaseTest::TestCase_DatabaseStatementFieldsInsertRetrieveFastOtherIndex()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseStatementFieldsInsertRetrieveFastOtherIndex ****" );
		TestCase_DatabaseFieldsInsertRetrieveFastOtherIndex< CDatabaseStatement >();
		CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseStatementFieldsInsertRetrieveFastOtherIndex ****" );
	}

	void CDatabaseTest::TestCase_DatabaseStatementDirectQuery()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseStatementDirectQuery ****" );
		TestCase_DatabaseDirectQuery< CDatabaseStatement >();
		CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseStatementDirectQuery ****" );
	}

	void CDatabaseTest::TestCase_DatabaseStatementStoredProcedure()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseStatementStoredProcedure ****" );
		TestCase_DatabaseStoredProcedure< CDatabaseStatement >();
		CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseStatementStoredProcedure ****" );
	}

	void CDatabaseTest::TestCase_DestroyDatabase()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_DestroyDatabase ****" );
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
						connection->ExecuteUpdate( QUERY_DROP_TABLE );
						connection->DestroyDatabase( _database );
					}

					database->RemoveConnection();
				}
			}
		}
		CLogger::LogInfo( StringStream() << "**** End TestCase_DestroyDatabase ****" );
	}

#if defined( PERF_TEST )
	void CDatabaseTest::TestCase_DatabaseStatementPerformances()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseStatementPerformances ****" );
		TestCase_DatabasePerformances< CDatabaseQuery >();
		CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseStatementPerformances ****" );
	}

	void CDatabaseTest::TestCase_DatabaseQueryPerformances()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseQueryPerformances ****" );
		TestCase_DatabasePerformances< CDatabaseStatement >();
		CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseQueryPerformances ****" );
	}
#endif

	void CDatabaseTest::DoFlushTable( DatabaseConnectionPtr connection )
	{
		connection->ExecuteUpdate( STR( "DELETE FROM Test" ) );
	}
}
END_NAMESPACE_DATABASE_TEST
