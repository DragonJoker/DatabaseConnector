/************************************************************************//**
 * @file DatabaseOdbcMsSqlTest.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 16/07/2014 12:08:57
 *
 *
 * @brief Database ODBC plugin test
 *
 * @details Database ODBC plugin test
 *
 ***************************************************************************/

#include "DatabaseTestPch.h"

#include "DatabaseOdbcMsSqlTest.h"

BEGIN_NAMESPACE_DATABASE_TEST
{
	static const String DB_DATABASE = STR( "db_test_odbc_mssql" );
	static const String DB_SERVER = STR( "." );
	static const String DB_USER = STR( "test_user" );
	static const String DB_PASS = STR( "test_pwd" );

	static const String DATABASE_ODBC_MSSQL_TYPE = STR( "Database.Odbc.MsSql" );

	CDatabaseOdbcMsSqlTest::CDatabaseOdbcMsSqlTest()
		: CDatabaseTest( DATABASE_ODBC_MSSQL_TYPE, DB_SERVER, DB_DATABASE, DB_USER, DB_PASS, true, false, STR( "=" ), false, true )
	{
		_createTable += STR( "CREATE TABLE Test" );
		_createTable += STR( "(	IDTest INTEGER NOT NULL IDENTITY( 1, 1 ) PRIMARY KEY\n" );
		_createTable += STR( ",	IntField INT\n" );
		_createTable += STR( ",	IntegerField INTEGER\n" );
		_createTable += STR( ",	TinyIntField TINYINT\n" );
		_createTable += STR( ",	SmallIntField SMALLINT\n" );
		_createTable += STR( ",	MediumIntField INT\n" );
		_createTable += STR( ",	BigIntField BIGINT\n" );
		_createTable += STR( ",	Int2Field SMALLINT\n" );
		_createTable += STR( ",	Int8Field BIGINT\n" );
		_createTable += STR( ",	RealField FLOAT\n" );
		_createTable += STR( ",	DoubleField FLOAT\n" );
		_createTable += STR( ",	DoublePrecisionField FLOAT\n" );
		_createTable += STR( ",	FloatField REAL\n" );
		_createTable += STR( ",	NumericField NUMERIC\n" );
		_createTable += STR( ",	DecimalField DECIMAL(10,5)\n" );
		_createTable += STR( ",	BooleanField BIT\n" );
		_createTable += STR( ",	DateField DATE\n" );
		_createTable += STR( ",	DateTimeField DATETIME\n" );
		_createTable += STR( ",	CharacterField CHARACTER(20)\n" );
		_createTable += STR( ",	VarcharField VARCHAR(255)\n" );
		_createTable += STR( ",	NcharField NCHAR(55)\n" );
		_createTable += STR( ",	NVarcharField NVARCHAR(100)\n" );
		_createTable += STR( ",	TextField TEXT\n" );
		_createTable += STR( ",	BlobField VARBINARY\n" );
		_createTable += STR( ");" );
	}

	CDatabaseOdbcMsSqlTest::~CDatabaseOdbcMsSqlTest()
	{
	}

	void CDatabaseOdbcMsSqlTest::DoLoadPlugins()
	{
		LoadPlugins( InitializeSingletons(), false, false, false, true );
	}

	void CDatabaseOdbcMsSqlTest::TestCase_CreateDatabase()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_CreateDatabase ****" );
#if defined( _WIN32 )
		//InstallDatabaseMsSql( DB_DATABASE, DB_USER, DB_PASS );
		//InstallSourceOdbcMsSql( DB_DATABASE );
#endif
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
					connection->ExecuteUpdate( _createTable );
				}
			}
		}
		CLogger::LogInfo( StringStream() << "**** End TestCase_CreateDatabase ****" );
	}

	void CDatabaseOdbcMsSqlTest::TestCase_DestroyDatabase()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_DestroyDatabase ****" );
#if defined( _WIN32 )
		UninstallSourceOdbcMsSql( DB_DATABASE );
		UninstallDatabaseMsSql( DB_DATABASE, DB_USER, DB_PASS );
#endif
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
					connection->ExecuteUpdate( QUERY_DROP_TABLE );
				}
				catch ( std::exception & )
				{
					BOOST_CHECK( false );
				}

				database->RemoveConnection();
			}
		}
		CLogger::LogInfo( StringStream() << "**** End TestCase_DestroyDatabase ****" );
	}
}
END_NAMESPACE_DATABASE_TEST
