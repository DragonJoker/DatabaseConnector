/************************************************************************//**
 * @file DatabaseOdbcMySqlTest.cpp
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

#include "DatabaseOdbcMySqlTest.h"

BEGIN_NAMESPACE_DATABASE_TEST
{
	static const String DB_DATABASE = STR( "db_test_odbc_mysql" );
	static const String DB_SERVER = STR( "127.0.0.1" );
	static const String DB_USER = STR( "test_user" );
	static const String DB_PASS = STR( "test_pwd" );

	static const String DATABASE_ODBC_MYSQL_TYPE = STR( "Database.Odbc.MySql" );

	CDatabaseOdbcMySqlTest::CDatabaseOdbcMySqlTest()
		: CDatabaseTest( DATABASE_ODBC_MYSQL_TYPE, DB_SERVER, DB_DATABASE, DB_USER, DB_PASS, true )
	{
		_createTable += STR( "CREATE TABLE Test" );
		_createTable += STR( "(	IDTest INTEGER PRIMARY KEY AUTO_INCREMENT\n" );
		_createTable += STR( ",	IntField INT\n" );
		_createTable += STR( ",	IntegerField INTEGER\n" );
		_createTable += STR( ",	TinyIntField TINYINT\n" );
		_createTable += STR( ",	SmallIntField SMALLINT\n" );
		_createTable += STR( ",	MediumIntField MEDIUMINT\n" );
		_createTable += STR( ",	BigIntField BIGINT\n" );
		_createTable += STR( ",	Int2Field INT2\n" );
		_createTable += STR( ",	Int8Field INT8\n" );
		_createTable += STR( ",	RealField REAL\n" );
		_createTable += STR( ",	DoubleField DOUBLE\n" );
		_createTable += STR( ",	DoublePrecisionField DOUBLE PRECISION\n" );
		_createTable += STR( ",	FloatField FLOAT\n" );
		_createTable += STR( ",	NumericField NUMERIC\n" );
		_createTable += STR( ",	DecimalField DECIMAL(10,5)\n" );
		_createTable += STR( ",	BooleanField BOOLEAN\n" );
		_createTable += STR( ",	DateField DATE\n" );
		_createTable += STR( ",	DateTimeField DATETIME\n" );
		_createTable += STR( ",	CharacterField CHARACTER(20)\n" );
		_createTable += STR( ",	VarcharField VARCHAR(255)\n" );
		_createTable += STR( ",	NcharField NCHAR(55)\n" );
		_createTable += STR( ",	NVarcharField NVARCHAR(100)\n" );
		_createTable += STR( ",	TextField TEXT\n" );
		_createTable += STR( ",	BlobField BLOB\n" );
		_createTable += STR( ");" );
	}

	CDatabaseOdbcMySqlTest::~CDatabaseOdbcMySqlTest()
	{
	}

	void CDatabaseOdbcMySqlTest::DoLoadPlugins()
	{
		LoadPlugins( InitializeSingletons(), false, false, true, false );
	}

	void CDatabaseOdbcMySqlTest::TestCase_CreateDatabase()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_CreateDatabase ****" );
		InstallDatabaseMySql( DB_DATABASE, DB_USER, DB_PASS );
		InstallSourceOdbcMySql( DB_DATABASE );
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

	void CDatabaseOdbcMySqlTest::TestCase_DestroyDatabase()
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

				try
				{
					connection->SelectDatabase( _database );
					connection->ExecuteUpdate( QUERY_DROP_TABLE );
				}
				catch ( std::exception & )
				{
					BOOST_CHECK( false );
				}

				database->RemoveConnection();
			}
		}
		UninstallSourceOdbcMySql( DB_DATABASE );
		UninstallDatabaseMySql( DB_DATABASE, DB_USER, DB_PASS );
		CLogger::LogInfo( StringStream() << "**** End TestCase_DestroyDatabase ****" );
	}
}
END_NAMESPACE_DATABASE_TEST
