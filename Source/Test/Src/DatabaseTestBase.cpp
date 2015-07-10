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
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseQueryFieldsInsertRetrieve, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseQueryFieldsInsertRetrieveOtherIndex, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseQueryDirectQuery, this ) ) );
		//testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseQueryStoredProcedure, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseStatementFieldsInsertRetrieve, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseStatementFieldsInsertRetrieveOtherIndex, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseStatementDirectQuery, this ) ) );
		//testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseTest::TestCase_DatabaseStatementStoredProcedure, this ) ) );
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
			std::unique_ptr< CDatabase > database( InstantiateDatabase( _type ) );

			if ( database )
			{
				DatabaseConnectionPtr connection = CreateConnection( *database, _server, _user, _password );
				connection->CreateDatabase( _database );
				connection->SelectDatabase( _database );

				if ( connection )
				{
					connection->ExecuteUpdate( _createTable );
				}
			}
		}
		CLogger::LogMessage( StringStream() << "**** End TestCase_CreateDatabase ****" );
	}

	void CDatabaseTest::TestCase_DatabaseQueryFieldsInsertRetrieve()
	{
		CLogger::LogMessage( StringStream() << "**** Start TestCase_DatabaseQueryFieldsInsertRetrieve ****" );
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
				connection->SelectDatabase( _database );

				if ( connection )
				{
					DoFlushTable( connection );
					DatabaseUtils::InsertAndRetrieve< CDatabaseQuery, int32_t >( connection, STR( "IntegerField" ) );
					DatabaseUtils::InsertAndRetrieve< CDatabaseQuery, bool >( connection, STR( "TinyIntField" ) );
					DatabaseUtils::InsertAndRetrieve< CDatabaseQuery, int16_t >( connection, STR( "SmallIntField" ) );
					DatabaseUtils::InsertAndRetrieve< CDatabaseQuery, int32_t >( connection, STR( "MediumIntField" ) );
					DatabaseUtils::InsertAndRetrieve< CDatabaseQuery, int64_t >( connection, STR( "BigIntField" ) );
					DatabaseUtils::InsertAndRetrieve< CDatabaseQuery, int16_t >( connection, STR( "Int2Field" ) );
					DatabaseUtils::InsertAndRetrieve< CDatabaseQuery, int64_t >( connection, STR( "Int8Field" ) );
					DatabaseUtils::InsertAndRetrieve< CDatabaseQuery, float >( connection, STR( "RealField" ) );
					DatabaseUtils::InsertAndRetrieve< CDatabaseQuery, double >( connection, STR( "DoubleField" ) );
					DatabaseUtils::InsertAndRetrieve< CDatabaseQuery, double >( connection, STR( "DoublePrecisionField" ) );
					//DatabaseUtils::InsertAndRetrieve< CDatabaseQuery, float >( connection, STR( "FloatField" ) );
					DatabaseUtils::InsertAndRetrieve< CDatabaseQuery, int32_t >( connection, STR( "NumericField" ) );
					DatabaseUtils::InsertAndRetrieve< CDatabaseQuery, double >( connection, STR( "DecimalField" ) );
					DatabaseUtils::InsertAndRetrieve< CDatabaseQuery, bool >( connection, STR( "BooleanField" ) );
					DatabaseUtils::InsertAndRetrieve< CDatabaseQuery, CDate >( connection, STR( "DateField" ) );
					DatabaseUtils::InsertAndRetrieve< CDatabaseQuery, CDateTime >( connection, STR( "DateTimeField" ) );
					DatabaseUtils::InsertAndRetrieve< CDatabaseQuery, char * >( connection, STR( "CharacterField" ) );
					DatabaseUtils::InsertAndRetrieve< CDatabaseQuery, char * >( connection, STR( "VarcharField" ) );
					DatabaseUtils::InsertAndRetrieve< CDatabaseQuery, wchar_t * >( connection, STR( "NcharField" ) );
					DatabaseUtils::InsertAndRetrieve< CDatabaseQuery, wchar_t * >( connection, STR( "NVarcharField" ) );
					DatabaseUtils::InsertAndRetrieve< CDatabaseQuery, std::string >( connection, STR( "TextField" ) );
					DatabaseUtils::InsertAndRetrieve< CDatabaseQuery, std::vector< uint8_t > >( connection, STR( "BlobField" ) );
				}
			}
		}
		CLogger::LogMessage( StringStream() << "**** End TestCase_DatabaseQueryFieldsInsertRetrieve ****" );
	}

	void CDatabaseTest::TestCase_DatabaseQueryFieldsInsertRetrieveOtherIndex()
	{
		CLogger::LogMessage( StringStream() << "**** Start TestCase_DatabaseQueryFieldsInsertRetrieveOtherIndex ****" );
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
				connection->SelectDatabase( _database );

				if ( connection )
				{
					DoFlushTable( connection );
					DatabaseUtils::InsertAndRetrieveOtherIndex< CDatabaseQuery, int32_t >( connection, STR( "IntegerField" ) );
					DatabaseUtils::InsertAndRetrieveOtherIndex< CDatabaseQuery, bool >( connection, STR( "TinyIntField" ) );
					DatabaseUtils::InsertAndRetrieveOtherIndex< CDatabaseQuery, int16_t >( connection, STR( "SmallIntField" ) );
					DatabaseUtils::InsertAndRetrieveOtherIndex< CDatabaseQuery, int32_t >( connection, STR( "MediumIntField" ) );
					DatabaseUtils::InsertAndRetrieveOtherIndex< CDatabaseQuery, int64_t >( connection, STR( "BigIntField" ) );
					DatabaseUtils::InsertAndRetrieveOtherIndex< CDatabaseQuery, int16_t >( connection, STR( "Int2Field" ) );
					DatabaseUtils::InsertAndRetrieveOtherIndex< CDatabaseQuery, int64_t >( connection, STR( "Int8Field" ) );
					DatabaseUtils::InsertAndRetrieveOtherIndex< CDatabaseQuery, float >( connection, STR( "RealField" ) );
					DatabaseUtils::InsertAndRetrieveOtherIndex< CDatabaseQuery, double >( connection, STR( "DoubleField" ) );
					DatabaseUtils::InsertAndRetrieveOtherIndex< CDatabaseQuery, double >( connection, STR( "DoublePrecisionField" ) );
					DatabaseUtils::InsertAndRetrieveOtherIndex< CDatabaseQuery, float >( connection, STR( "FloatField" ) );
					DatabaseUtils::InsertAndRetrieveOtherIndex< CDatabaseQuery, int32_t >( connection, STR( "NumericField" ) );
					DatabaseUtils::InsertAndRetrieveOtherIndex< CDatabaseQuery, double >( connection, STR( "DecimalField" ) );
					DatabaseUtils::InsertAndRetrieveOtherIndex< CDatabaseQuery, bool >( connection, STR( "BooleanField" ) );
					DatabaseUtils::InsertAndRetrieveOtherIndex< CDatabaseQuery, CDate >( connection, STR( "DateField" ) );
					DatabaseUtils::InsertAndRetrieveOtherIndex< CDatabaseQuery, CDateTime >( connection, STR( "DateTimeField" ) );
					DatabaseUtils::InsertAndRetrieveOtherIndex< CDatabaseQuery, char * >( connection, STR( "CharacterField" ) );
					DatabaseUtils::InsertAndRetrieveOtherIndex< CDatabaseQuery, char * >( connection, STR( "VarcharField" ) );
					DatabaseUtils::InsertAndRetrieveOtherIndex< CDatabaseQuery, wchar_t * >( connection, STR( "NcharField" ) );
					DatabaseUtils::InsertAndRetrieveOtherIndex< CDatabaseQuery, wchar_t * >( connection, STR( "NVarcharField" ) );
					DatabaseUtils::InsertAndRetrieveOtherIndex< CDatabaseQuery, std::string >( connection, STR( "TextField" ) );
					DatabaseUtils::InsertAndRetrieveOtherIndex< CDatabaseQuery, std::vector< uint8_t > >( connection, STR( "BlobField" ) );
				}
			}
		}
		CLogger::LogMessage( StringStream() << "**** End TestCase_DatabaseQueryFieldsInsertRetrieveOtherIndex ****" );
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
			std::unique_ptr< CDatabase > database( InstantiateDatabase( _type ) );

			if ( database )
			{
				DatabaseConnectionPtr connection = CreateConnection( *database, _server, _user, _password );
				connection->SelectDatabase( _database );

				if ( connection )
				{
					DoFlushTable( connection );
					CLogger::LogMessage( StringStream() << " Insertions" );
					DatabaseUtils::TestDirectInsert< CDatabaseQuery >( connection );
					CLogger::LogMessage( StringStream() << " Selection" );
					DatabaseUtils::TestDirectSelect< CDatabaseQuery >( connection );
					CLogger::LogMessage( StringStream() << " Update" );
					DatabaseUtils::TestDirectUpdate< CDatabaseQuery >( connection );
					CLogger::LogMessage( StringStream() << " Deletion" );
					DatabaseUtils::TestDirectDelete< CDatabaseQuery >( connection );
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
			std::unique_ptr< CDatabase > database( InstantiateDatabase( _type ) );

			if ( database )
			{
				DatabaseConnectionPtr connection = CreateConnection( *database, _server, _user, _password );
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
				}

				database->RemoveConnection();
			}
		}
		CLogger::LogMessage( StringStream() << "**** End TestCase_DatabaseQueryStoredProcedure ****" );
	}

	void CDatabaseTest::TestCase_DatabaseStatementFieldsInsertRetrieve()
	{
		CLogger::LogMessage( StringStream() << "**** Start TestCase_DatabaseStatementFieldsInsertRetrieve ****" );
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
				connection->SelectDatabase( _database );

				if ( connection )
				{
					DoFlushTable( connection );
					DatabaseUtils::InsertAndRetrieve< CDatabaseStatement, int32_t >( connection, STR( "IntegerField" ) );
					DatabaseUtils::InsertAndRetrieve< CDatabaseStatement, bool >( connection, STR( "TinyIntField" ) );
					DatabaseUtils::InsertAndRetrieve< CDatabaseStatement, int16_t >( connection, STR( "SmallIntField" ) );
					DatabaseUtils::InsertAndRetrieve< CDatabaseStatement, int32_t >( connection, STR( "MediumIntField" ) );
					DatabaseUtils::InsertAndRetrieve< CDatabaseStatement, int64_t >( connection, STR( "BigIntField" ) );
					DatabaseUtils::InsertAndRetrieve< CDatabaseStatement, int16_t >( connection, STR( "Int2Field" ) );
					DatabaseUtils::InsertAndRetrieve< CDatabaseStatement, int64_t >( connection, STR( "Int8Field" ) );
					DatabaseUtils::InsertAndRetrieve< CDatabaseStatement, float >( connection, STR( "RealField" ) );
					DatabaseUtils::InsertAndRetrieve< CDatabaseStatement, double >( connection, STR( "DoubleField" ) );
					DatabaseUtils::InsertAndRetrieve< CDatabaseStatement, double >( connection, STR( "DoublePrecisionField" ) );
					DatabaseUtils::InsertAndRetrieve< CDatabaseStatement, float >( connection, STR( "FloatField" ) );
					DatabaseUtils::InsertAndRetrieve< CDatabaseStatement, int32_t >( connection, STR( "NumericField" ) );
					DatabaseUtils::InsertAndRetrieve< CDatabaseStatement, double >( connection, STR( "DecimalField" ) );
					DatabaseUtils::InsertAndRetrieve< CDatabaseStatement, bool >( connection, STR( "BooleanField" ) );
					DatabaseUtils::InsertAndRetrieve< CDatabaseStatement, CDate >( connection, STR( "DateField" ) );
					DatabaseUtils::InsertAndRetrieve< CDatabaseStatement, CDateTime >( connection, STR( "DateTimeField" ) );
					DatabaseUtils::InsertAndRetrieve< CDatabaseStatement, char * >( connection, STR( "CharacterField" ) );
					DatabaseUtils::InsertAndRetrieve< CDatabaseStatement, char * >( connection, STR( "VarcharField" ) );
					DatabaseUtils::InsertAndRetrieve< CDatabaseStatement, wchar_t * >( connection, STR( "NcharField" ) );
					DatabaseUtils::InsertAndRetrieve< CDatabaseStatement, wchar_t * >( connection, STR( "NVarcharField" ) );
					DatabaseUtils::InsertAndRetrieve< CDatabaseStatement, std::string >( connection, STR( "TextField" ) );
					DatabaseUtils::InsertAndRetrieve< CDatabaseStatement, std::vector< uint8_t > >( connection, STR( "BlobField" ) );
				}
			}
		}
		CLogger::LogMessage( StringStream() << "**** End TestCase_DatabaseStatementFieldsInsertRetrieve ****" );
	}

	void CDatabaseTest::TestCase_DatabaseStatementFieldsInsertRetrieveOtherIndex()
	{
		CLogger::LogMessage( StringStream() << "**** Start TestCase_DatabaseStatementFieldsInsertRetrieveOtherIndex ****" );
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
				connection->SelectDatabase( _database );

				if ( connection )
				{
					DoFlushTable( connection );
					DatabaseUtils::InsertAndRetrieveOtherIndex< CDatabaseStatement, int32_t >( connection, STR( "IntegerField" ) );
					DatabaseUtils::InsertAndRetrieveOtherIndex< CDatabaseStatement, bool >( connection, STR( "TinyIntField" ) );
					DatabaseUtils::InsertAndRetrieveOtherIndex< CDatabaseStatement, int16_t >( connection, STR( "SmallIntField" ) );
					DatabaseUtils::InsertAndRetrieveOtherIndex< CDatabaseStatement, int32_t >( connection, STR( "MediumIntField" ) );
					DatabaseUtils::InsertAndRetrieveOtherIndex< CDatabaseStatement, int64_t >( connection, STR( "BigIntField" ) );
					DatabaseUtils::InsertAndRetrieveOtherIndex< CDatabaseStatement, int16_t >( connection, STR( "Int2Field" ) );
					DatabaseUtils::InsertAndRetrieveOtherIndex< CDatabaseStatement, int64_t >( connection, STR( "Int8Field" ) );
					DatabaseUtils::InsertAndRetrieveOtherIndex< CDatabaseStatement, float >( connection, STR( "RealField" ) );
					DatabaseUtils::InsertAndRetrieveOtherIndex< CDatabaseStatement, double >( connection, STR( "DoubleField" ) );
					DatabaseUtils::InsertAndRetrieveOtherIndex< CDatabaseStatement, double >( connection, STR( "DoublePrecisionField" ) );
					DatabaseUtils::InsertAndRetrieveOtherIndex< CDatabaseStatement, float >( connection, STR( "FloatField" ) );
					DatabaseUtils::InsertAndRetrieveOtherIndex< CDatabaseStatement, int32_t >( connection, STR( "NumericField" ) );
					DatabaseUtils::InsertAndRetrieveOtherIndex< CDatabaseStatement, double >( connection, STR( "DecimalField" ) );
					DatabaseUtils::InsertAndRetrieveOtherIndex< CDatabaseStatement, bool >( connection, STR( "BooleanField" ) );
					DatabaseUtils::InsertAndRetrieveOtherIndex< CDatabaseStatement, CDate >( connection, STR( "DateField" ) );
					DatabaseUtils::InsertAndRetrieveOtherIndex< CDatabaseStatement, CDateTime >( connection, STR( "DateTimeField" ) );
					DatabaseUtils::InsertAndRetrieveOtherIndex< CDatabaseStatement, char * >( connection, STR( "CharacterField" ) );
					DatabaseUtils::InsertAndRetrieveOtherIndex< CDatabaseStatement, char * >( connection, STR( "VarcharField" ) );
					DatabaseUtils::InsertAndRetrieveOtherIndex< CDatabaseStatement, wchar_t * >( connection, STR( "NcharField" ) );
					DatabaseUtils::InsertAndRetrieveOtherIndex< CDatabaseStatement, wchar_t * >( connection, STR( "NVarcharField" ) );
					DatabaseUtils::InsertAndRetrieveOtherIndex< CDatabaseStatement, std::string >( connection, STR( "TextField" ) );
					DatabaseUtils::InsertAndRetrieveOtherIndex< CDatabaseStatement, std::vector< uint8_t > >( connection, STR( "BlobField" ) );
				}
			}
		}
		CLogger::LogMessage( StringStream() << "**** End TestCase_DatabaseStatementFieldsInsertRetrieveOtherIndex ****" );
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
			std::unique_ptr< CDatabase > database( InstantiateDatabase( _type ) );

			if ( database )
			{
				DatabaseConnectionPtr connection = CreateConnection( *database, _server, _user, _password );
				connection->SelectDatabase( _database );

				if ( connection )
				{
					DoFlushTable( connection );
					CLogger::LogMessage( StringStream() << " Insertions" );
					DatabaseUtils::TestDirectInsert< CDatabaseStatement >( connection );
					CLogger::LogMessage( StringStream() << " Selection" );
					DatabaseUtils::TestDirectSelect< CDatabaseStatement >( connection );
					CLogger::LogMessage( StringStream() << " Update" );
					DatabaseUtils::TestDirectUpdate< CDatabaseStatement >( connection );
					CLogger::LogMessage( StringStream() << " Deletion" );
					DatabaseUtils::TestDirectDelete< CDatabaseStatement >( connection );
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
			std::unique_ptr< CDatabase > database( InstantiateDatabase( _type ) );

			if ( database )
			{
				DatabaseConnectionPtr connection = CreateConnection( *database, _server, _user, _password );
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
			std::unique_ptr< CDatabase > database( InstantiateDatabase( _type ) );

			if ( database )
			{
				DatabaseConnectionPtr connection = CreateConnection( *database, _server, _user, _password );

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

#if defined( PERF_TEST )
	void CDatabaseTest::TestCase_DatabaseStatementPerformances()
	{
		CLogger::LogMessage( StringStream() << "**** Start TestCase_DatabaseStatementPerformances ****" );
		{
#if defined( NDEBUG )
			uint32_t tests = 100000;
#else
			uint32_t tests = 10;
#endif
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
				connection->SelectDatabase( _database );

				if ( connection )
				{
					DoFlushTable( connection );
					CLogger::LogMessage( StringStream() << " Insert elements" );
					PerfDirectInsertActors< CDatabaseStatement >( connection, tests, String() );
					CLogger::LogMessage( StringStream() << " Select all elements" );
					PerfDirectSelectActors< CDatabaseStatement >( connection, tests, String() );
					CLogger::LogMessage( StringStream() << " Delete all elements" );
					PerfDirectDeleteActors< CDatabaseStatement >( connection, tests );
				}

				database->RemoveConnection();
			}
		}
		CLogger::LogMessage( StringStream() << "**** End TestCase_DatabaseStatementPerformances ****" );
	}

	void CDatabaseTest::TestCase_DatabaseQueryPerformances()
	{
		CLogger::LogMessage( StringStream() << "**** Start TestCase_DatabaseQueryPerformances ****" );
		{
#if defined( NDEBUG )
			uint32_t tests = 100000;
#else
			uint32_t tests = 10;
#endif
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
				connection->SelectDatabase( _database );

				if ( connection )
				{
					DoFlushTable( connection );
					CLogger::LogMessage( StringStream() << " Insert elements" );
					PerfDirectInsertActors< CDatabaseQuery >( connection, tests, String() );
					CLogger::LogMessage( StringStream() << " Select all elements" );
					PerfDirectSelectActors< CDatabaseQuery >( connection, tests, String() );
					CLogger::LogMessage( StringStream() << " Delete all elements" );
					PerfDirectDeleteActors< CDatabaseQuery >( connection, tests );
				}

				database->RemoveConnection();
			}
		}
		CLogger::LogMessage( StringStream() << "**** End TestCase_DatabaseQueryPerformances ****" );
	}
#endif

	void CDatabaseTest::DoFlushTable( DatabaseConnectionPtr connection )
	{
		connection->ExecuteUpdate( STR( "DELETE FROM Test" ) );
	}
}
END_NAMESPACE_DATABASE_TEST
