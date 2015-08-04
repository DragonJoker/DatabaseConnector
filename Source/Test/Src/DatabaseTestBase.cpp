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
		testSuite = new boost::unit_test::test_suite( "CDatabaseSqliteTest" );

		//!@remarks Add the TC to the internal TS.
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_CreateDatabase, this ) ) );

		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseQueryFieldsInsertRetrieve, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseFieldsInsertRetrieveOtherIndex< CDatabaseQuery >, this, STR( "Query" ) ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseFieldsInsertRetrieveFast< CDatabaseQuery >, this, STR( "Query" ) ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseFieldsInsertRetrieveFastOtherIndex< CDatabaseQuery >, this , STR( "Query" ) ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseDirectQuery< CDatabaseQuery >, this, STR( "Query" ) ) ) );

		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseStatementFieldsInsertRetrieve, this ) ) );
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
					DoFlushTable( connection );
				}

				database->RemoveConnection();
			}
		}
	}

	void CDatabaseTest::TestCase_DatabaseQueryFieldsInsertRetrieve()
	{
		auto const guard = make_block_guard( [this]()
		{
			CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseQueryFieldsInsertRetrieve ****" );
			DoLoadPlugins();
		}, []()
		{
			UnloadPlugins();
			CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseQueryFieldsInsertRetrieve ****" );
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
					BatchTests< EFieldType_SINT32 >()( &DatabaseUtils::InsertAndRetrieve< CDatabaseQuery, EFieldType_SINT32 >, connection, STR( "IntegerField" ), _config.is, generator );
					BatchTests< EFieldType_SINT16 >()( &DatabaseUtils::InsertAndRetrieve< CDatabaseQuery, EFieldType_SINT16 >, connection, STR( "SmallIntField" ), _config.is, generator );
					BatchTests< EFieldType_SINT64 >()( &DatabaseUtils::InsertAndRetrieve< CDatabaseQuery, EFieldType_SINT64 >, connection, STR( "BigIntField" ), _config.is, generator );
					BatchTests< EFieldType_SINT16 >()( &DatabaseUtils::InsertAndRetrieve< CDatabaseQuery, EFieldType_SINT16 >, connection, STR( "Int2Field" ), _config.is, generator );
					BatchTests< EFieldType_SINT64 >()( &DatabaseUtils::InsertAndRetrieve< CDatabaseQuery, EFieldType_SINT64 >, connection, STR( "Int8Field" ), _config.is, generator );
					BatchTests< EFieldType_FLOAT64 >()( &DatabaseUtils::InsertAndRetrieve< CDatabaseQuery, EFieldType_FLOAT64 >, connection, STR( "RealField" ), _config.is, generator );
					BatchTests< EFieldType_FLOAT64 >()( &DatabaseUtils::InsertAndRetrieve< CDatabaseQuery, EFieldType_FLOAT64 >, connection, STR( "DoubleField" ), _config.is, generator );
					BatchTests< EFieldType_FLOAT64 >()( &DatabaseUtils::InsertAndRetrieve< CDatabaseQuery, EFieldType_FLOAT64 >, connection, STR( "DoublePrecisionField" ), _config.is, generator );
					BatchTests< EFieldType_FIXED_POINT, 10, 0 >()( &DatabaseUtils::InsertAndRetrieve< CDatabaseQuery, EFieldType_FIXED_POINT >, connection, STR( "NumericField" ), _config.is, generator );
					BatchTests< EFieldType_FIXED_POINT, 10, 5 >()( &DatabaseUtils::InsertAndRetrieve< CDatabaseQuery, EFieldType_FIXED_POINT >, connection, STR( "DecimalField" ), _config.is, generator );
					BatchTests< EFieldType_BIT >()( &DatabaseUtils::InsertAndRetrieve< CDatabaseQuery, EFieldType_BIT >, connection, STR( "BooleanField" ), _config.is, generator );
					BatchTests< EFieldType_DATE >()( &DatabaseUtils::InsertAndRetrieve< CDatabaseQuery, EFieldType_DATE >, connection, STR( "DateField" ), _config.is, generator );
					BatchTests< EFieldType_DATETIME >()( &DatabaseUtils::InsertAndRetrieve< CDatabaseQuery, EFieldType_DATETIME >, connection, STR( "DateTimeField" ), _config.is, generator );
					BatchTests< EFieldType_CHAR, 20 >()( &DatabaseUtils::InsertAndRetrieve< CDatabaseQuery, EFieldType_CHAR >, connection, STR( "CharacterField" ), _config.is, generator );
					BatchTests< EFieldType_VARCHAR >()( &DatabaseUtils::InsertAndRetrieve< CDatabaseQuery, EFieldType_VARCHAR >, connection, STR( "VarcharField" ), _config.is, generator );
					BatchTests< EFieldType_NCHAR, 55 >()( &DatabaseUtils::InsertAndRetrieve< CDatabaseQuery, EFieldType_NCHAR >, connection, STR( "NcharField" ), _config.is, generator );
					BatchTests< EFieldType_NVARCHAR >()( &DatabaseUtils::InsertAndRetrieve< CDatabaseQuery, EFieldType_NVARCHAR >, connection, STR( "NVarcharField" ), _config.is, generator );
					BatchTests< EFieldType_TEXT >()( &DatabaseUtils::InsertAndRetrieve< CDatabaseQuery, EFieldType_TEXT >, connection, STR( "TextField" ), _config.is, generator );
					BatchTests< EFieldType_VARBINARY >()( &DatabaseUtils::InsertAndRetrieve< CDatabaseQuery, EFieldType_VARBINARY >, connection, STR( "BlobField" ), _config.is, generator );

					if ( _config.hasDecentFloat32 )
					{
						BatchTests< EFieldType_FLOAT32 >()( &DatabaseUtils::InsertAndRetrieve< CDatabaseQuery, EFieldType_FLOAT32 >, connection, STR( "FloatField" ), _config.is, generator, _config.hasDecentFloat32 );
					}

					if ( _config.hasTinyInt )
					{
						if ( _config.hasUnsignedTiny )
						{
							BatchTests< EFieldType_UINT8 >()( &DatabaseUtils::InsertAndRetrieve< CDatabaseQuery, EFieldType_UINT8 >, connection, STR( "TinyIntField" ), _config.is, generator );
						}
						else
						{
							BatchTests< EFieldType_SINT8 >()( &DatabaseUtils::InsertAndRetrieve< CDatabaseQuery, EFieldType_SINT8 >, connection, STR( "TinyIntField" ), _config.is, generator );
						}
					}

					if ( _config.hasInt24 )
					{
						BatchTests< EFieldType_SINT24 >()( &DatabaseUtils::InsertAndRetrieve< CDatabaseQuery, EFieldType_SINT24 >, connection, STR( "MediumIntField" ), _config.is, generator );
					}
				}

				database->RemoveConnection();
			}
		}
	}

	void CDatabaseTest::TestCase_DatabaseStatementFieldsInsertRetrieve()
	{
		auto const guard = make_block_guard( [this]()
		{
			CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseStatementFieldsInsertRetrieve ****" );
			DoLoadPlugins();
		}, []()
		{
			UnloadPlugins();
			CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseStatementFieldsInsertRetrieve ****" );
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
					BatchTests< EFieldType_SINT32 >()( &DatabaseUtils::InsertAndRetrieve< CDatabaseStatement, EFieldType_SINT32 >, connection, STR( "IntegerField" ), _config.is, generator );
					BatchTests< EFieldType_SINT16 >()( &DatabaseUtils::InsertAndRetrieve< CDatabaseStatement, EFieldType_SINT16 >, connection, STR( "SmallIntField" ), _config.is, generator );
					BatchTests< EFieldType_SINT64 >()( &DatabaseUtils::InsertAndRetrieve< CDatabaseStatement, EFieldType_SINT64 >, connection, STR( "BigIntField" ), _config.is, generator );
					BatchTests< EFieldType_SINT16 >()( &DatabaseUtils::InsertAndRetrieve< CDatabaseStatement, EFieldType_SINT16 >, connection, STR( "Int2Field" ), _config.is, generator );
					BatchTests< EFieldType_SINT64 >()( &DatabaseUtils::InsertAndRetrieve< CDatabaseStatement, EFieldType_SINT64 >, connection, STR( "Int8Field" ), _config.is, generator );
					BatchTests< EFieldType_FLOAT64 >()( &DatabaseUtils::InsertAndRetrieve< CDatabaseStatement, EFieldType_FLOAT64 >, connection, STR( "RealField" ), _config.is, generator );
					BatchTests< EFieldType_FLOAT64 >()( &DatabaseUtils::InsertAndRetrieve< CDatabaseStatement, EFieldType_FLOAT64 >, connection, STR( "DoubleField" ), _config.is, generator );
					BatchTests< EFieldType_FLOAT64 >()( &DatabaseUtils::InsertAndRetrieve< CDatabaseStatement, EFieldType_FLOAT64 >, connection, STR( "DoublePrecisionField" ), _config.is, generator );
					BatchTests< EFieldType_FLOAT32 >()( &DatabaseUtils::InsertAndRetrieve< CDatabaseStatement, EFieldType_FLOAT32 >, connection, STR( "FloatField" ), _config.is, generator, true );
					BatchTests< EFieldType_FIXED_POINT, 10, 0 >()( &DatabaseUtils::InsertAndRetrieve< CDatabaseStatement, EFieldType_FIXED_POINT >, connection, STR( "NumericField" ), _config.is, generator );
					BatchTests< EFieldType_FIXED_POINT, 10, 5 >()( &DatabaseUtils::InsertAndRetrieve< CDatabaseStatement, EFieldType_FIXED_POINT >, connection, STR( "DecimalField" ), _config.is, generator );
					BatchTests< EFieldType_BIT >()( &DatabaseUtils::InsertAndRetrieve< CDatabaseStatement, EFieldType_BIT >, connection, STR( "BooleanField" ), _config.is, generator );
					BatchTests< EFieldType_DATE >()( &DatabaseUtils::InsertAndRetrieve< CDatabaseStatement, EFieldType_DATE >, connection, STR( "DateField" ), _config.is, generator );
					BatchTests< EFieldType_DATETIME >()( &DatabaseUtils::InsertAndRetrieve< CDatabaseStatement, EFieldType_DATETIME >, connection, STR( "DateTimeField" ), _config.is, generator );
					BatchTests< EFieldType_CHAR, 20 >()( &DatabaseUtils::InsertAndRetrieve< CDatabaseStatement, EFieldType_CHAR >, connection, STR( "CharacterField" ), _config.is, generator );
					BatchTests< EFieldType_VARCHAR >()( &DatabaseUtils::InsertAndRetrieve< CDatabaseStatement, EFieldType_VARCHAR >, connection, STR( "VarcharField" ), _config.is, generator );
					BatchTests< EFieldType_NCHAR, 55 >()( &DatabaseUtils::InsertAndRetrieve< CDatabaseStatement, EFieldType_NCHAR >, connection, STR( "NcharField" ), _config.is, generator );
					BatchTests< EFieldType_NVARCHAR >()( &DatabaseUtils::InsertAndRetrieve< CDatabaseStatement, EFieldType_NVARCHAR >, connection, STR( "NVarcharField" ), _config.is, generator );
					BatchTests< EFieldType_TEXT >()( &DatabaseUtils::InsertAndRetrieve< CDatabaseStatement, EFieldType_TEXT >, connection, STR( "TextField" ), _config.is, generator );
					BatchTests< EFieldType_VARBINARY >()( &DatabaseUtils::InsertAndRetrieve< CDatabaseStatement, EFieldType_VARBINARY >, connection, STR( "BlobField" ), _config.is, generator );

					if ( _config.hasTinyInt )
					{
						if ( _config.hasUnsignedTiny )
						{
							BatchTests< EFieldType_UINT8 >()( &DatabaseUtils::InsertAndRetrieve< CDatabaseStatement, EFieldType_UINT8 >, connection, STR( "TinyIntField" ), _config.is, generator );
						}
						else
						{
							BatchTests< EFieldType_SINT8 >()( &DatabaseUtils::InsertAndRetrieve< CDatabaseStatement, EFieldType_SINT8 >, connection, STR( "TinyIntField" ), _config.is, generator );
						}
					}

					if ( _config.hasInt24 )
					{
						BatchTests< EFieldType_SINT24 >()( &DatabaseUtils::InsertAndRetrieve< CDatabaseStatement, EFieldType_SINT24 >, connection, STR( "MediumIntField" ), _config.is, generator );
					}
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
