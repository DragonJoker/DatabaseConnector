/************************************************************************//**
 * @file DatabaseTestBase.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 12/02/2014 14:29:35
 *
 *
 * @brief Base class for database tests
 * @todo
 *	Transactions test:
 *		Commit, close conn.
 *		Rollback, close conn.
 *		Begin, no modif, close conn.
 *		Begin, no modif, commit, close conn.
 *		Begin, no modif, rollback, close conn.
 *		Begin, modif, close conn.
 *		Begin, modif, commit, close conn.
 *		Begin, modif, rollback, close conn.
 *		Begin, begin, close conn.
 *	Stored procedures
 *	Multithread test
 *	Multistatmeent test
 *
 ***************************************************************************/

#include "DatabaseTestPch.h"

#include "DatabaseTestBase.h"

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

	CDatabaseTest::CDatabaseTest( const String & type, const String & server, const String & database, const String & user, const String & password, const STestConfig & config )
		: _type( type )
		, _server( server )
		, _database( database )
		, _user( user )
		, _password( password )
		, _config( config )
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

		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseFieldsInsertRetrieve< CDatabaseQuery >, this, STR( "Query" ) ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseFieldsInsertRetrieveOtherIndex< CDatabaseQuery >, this, STR( "Query" ) ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseFieldsInsertRetrieveFast< CDatabaseQuery >, this, STR( "Query" ) ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseFieldsInsertRetrieveFastOtherIndex< CDatabaseQuery >, this , STR( "Query" )) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseDirectQuery< CDatabaseQuery >, this, STR( "Query" ) ) ) );

		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseFieldsInsertRetrieve< CDatabaseStatement >, this, STR( "Statement" ) ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseFieldsInsertRetrieveOtherIndex< CDatabaseStatement >, this, STR( "Statement" ) ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseFieldsInsertRetrieveFast< CDatabaseStatement >, this, STR( "Statement" ) ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseFieldsInsertRetrieveFastOtherIndex< CDatabaseStatement >, this, STR( "Statement" ) ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseDirectQuery< CDatabaseStatement >, this, STR( "Statement" ) ) ) );

		//testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseStoredProcedure< CDatabaseQuery >, this ) ) );
		//testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseStoredProcedure< CDatabaseStatement >, this ) ) );

		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DestroyDatabase, this ) ) );

		//!@remarks Return the TS instance.
		return testSuite;
	}

	void CDatabaseTest::TestCase_CreateDatabase()
	{
		auto const guard = make_block_guard( [this]()
		{
			CLogger::LogInfo( StringStream() << "**** Start TestCase_CreateDatabase ****" );
			DoLoadPlugins();
		}, []()
		{
			UnloadPlugins();
			CLogger::LogInfo( StringStream() << "**** End TestCase_CreateDatabase ****" );
		} );
		std::unique_ptr< CDatabase > database( InstantiateDatabase( _type ) );

		if ( database )
		{
			DatabaseConnectionSPtr connection = CreateConnection( *database, _server, _user, _password );

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
				DatabaseConnectionSPtr connection = CreateConnection( *database, _server, _user, _password );

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

	void CDatabaseTest::DoFlushTable( DatabaseConnectionSPtr connection )
	{
		connection->ExecuteUpdate( STR( "DELETE FROM Test" ) );
	}
}
END_NAMESPACE_DATABASE_TEST
