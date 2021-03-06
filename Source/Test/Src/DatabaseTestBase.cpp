/************************************************************************//**
 * @file DatabaseTestBase.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 12/02/2014 14:29:35
 *
 *
 * @brief Base class for database tests
 * @todo
 *	Transactions tests:
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
 *	Multithread tests:
 *		One transacting insert, one insert
 *		One transacting insert, one update
 *		One transacting insert, one select
 *		One transacting insert, one delete
 *		One transacting update, one insert
 *		One transacting update, one update
 *		One transacting update, one select
 *		One transacting update, one delete
 *		One transacting select, one insert
 *		One transacting select, one update
 *		One transacting select, one select
 *		One transacting select, one delete
 *		One transacting insert, one transacting insert
 *		One transacting insert, one transacting select
 *		One transacting insert, one transacting update
 *		One transacting insert, one transacting delete
 *		One transacting update, one transacting select
 *		One transacting update, one transacting update
 *		One transacting update, one transacting delete
 *		One transacting select, one transacting select
 *		One transacting select, one transacting delete
 *	Multistatement tests
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
	String const QUERY_SELECT_MAX = STR( "SELECT MAX( IDTest ) AS TestID\n" )
	STR( "FROM\n" )
	STR( "	Test" );
	String const QUERY_GET_COUNT = STR( "SELECT COUNT( IDTest ) AS TestID\n" )
	STR( "FROM\n" )
	STR( "	Test" );
	String const QUERY_DIRECT_INSERT_ELEMENT = STR( "INSERT INTO Test\n" )
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
		testSuite = new boost::unit_test::test_suite( "CDatabase" + _type + "Test", __FILE__, __LINE__ );

		//!@remarks Add the TC to the internal TS.
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_CreateDatabase, this ) ) );

		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseFieldsInsertRetrieve< 0 >, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseFieldsInsertRetrieveOtherIndex< 0 >, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseFieldsInsertRetrieveFast< 0 >, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseFieldsInsertRetrieveFastOtherIndex< 0 >, this  ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseDirectQuery< 0 >, this ) ) );

		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseFieldsInsertRetrieve< 1 >, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseFieldsInsertRetrieveOtherIndex< 1 >, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseFieldsInsertRetrieveFast< 1 >, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseFieldsInsertRetrieveFastOtherIndex< 1 >, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseDirectQuery< 1 >, this ) ) );

		//testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseStoredProcedure< 0 >, this ) ) );
		//testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseStoredProcedure< 1 >, this ) ) );

		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::LoggerToWarnings, this ) ) );

		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_MultithreadInsertAndInsert< 0, 0 >, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_MultithreadInsertAndUpdate< 0, 0 >, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_MultithreadInsertAndSelect< 0, 0 >, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_MultithreadInsertAndDelete< 0, 0 >, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_MultithreadUpdateAndUpdate< 0, 0 >, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_MultithreadUpdateAndSelect< 0, 0 >, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_MultithreadUpdateAndDelete< 0, 0 >, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_MultithreadSelectAndSelect< 0, 0 >, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_MultithreadSelectAndDelete< 0, 0 >, this ) ) );

		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_MultithreadInsertAndInsert< 0, 1 >, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_MultithreadInsertAndUpdate< 0, 1 >, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_MultithreadInsertAndSelect< 0, 1 >, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_MultithreadInsertAndDelete< 0, 1 >, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_MultithreadUpdateAndUpdate< 0, 1 >, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_MultithreadUpdateAndSelect< 0, 1 >, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_MultithreadUpdateAndDelete< 0, 1 >, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_MultithreadSelectAndSelect< 0, 1 >, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_MultithreadSelectAndDelete< 0, 1 >, this ) ) );

		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_MultithreadInsertAndInsert< 1, 0 >, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_MultithreadInsertAndUpdate< 1, 0 >, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_MultithreadInsertAndSelect< 1, 0 >, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_MultithreadInsertAndDelete< 1, 0 >, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_MultithreadUpdateAndUpdate< 1, 0 >, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_MultithreadUpdateAndSelect< 1, 0 >, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_MultithreadUpdateAndDelete< 1, 0 >, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_MultithreadSelectAndSelect< 1, 0 >, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_MultithreadSelectAndDelete< 1, 0 >, this ) ) );

		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_MultithreadInsertAndInsert< 1, 1 >, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_MultithreadInsertAndUpdate< 1, 1 >, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_MultithreadInsertAndSelect< 1, 1 >, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_MultithreadInsertAndDelete< 1, 1 >, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_MultithreadUpdateAndUpdate< 1, 1 >, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_MultithreadUpdateAndSelect< 1, 1 >, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_MultithreadUpdateAndDelete< 1, 1 >, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_MultithreadSelectAndSelect< 1, 1 >, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_MultithreadSelectAndDelete< 1, 1 >, this ) ) );

		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::LoggerToDefault, this ) ) );

		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DestroyDatabase, this ) ) );

		//!@remarks Return the TS instance.
		return testSuite;
	}

	void CDatabaseTest::TestCase_CreateDatabase()
	{
		auto const guard = make_block_guard( [this]()
		{
			CLogger::LogInfo( StringStream() << "**** Start " << _type << "_TestCase_CreateDatabase ****" );
			DoLoadPlugins();
		}, [this]()
		{
			UnloadPlugins();
			CLogger::LogInfo( StringStream() << "**** End " << _type << "_TestCase_CreateDatabase ****" );
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
					DoFlushTable( connection );
				}

				database->RemoveConnection();
			}
		}
	}

	void CDatabaseTest::LoggerToWarnings()
	{
		CLogger::SetLevel( ELogType_INFO );
	}

	void CDatabaseTest::LoggerToDefault()
	{
#if defined( NDEBUG )
		CLogger::SetLevel( ELogType_INFO );
#else
		CLogger::SetLevel( ELogType_DEBUG );
#endif
	}

	void CDatabaseTest::TestCase_DestroyDatabase()
	{
		auto const guard = make_block_guard( [this]()
		{
			CLogger::LogInfo( StringStream() << "**** Start " << _type << "_TestCase_DestroyDatabase ****" );
			DoLoadPlugins();
		}, [this]()
		{
			UnloadPlugins();
			CLogger::LogInfo( StringStream() << "**** End " << _type << "_TestCase_DestroyDatabase ****" );
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

	void CDatabaseTest::DoFlushTable( DatabaseConnectionSPtr connection )
	{
		connection->ExecuteUpdate( STR( "DELETE FROM Test" ) );
	}
}
END_NAMESPACE_DATABASE_TEST
