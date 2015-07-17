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
	static const String DB_SERVER = STR( "127.0.0.1" );
	static const String DB_USER = STR( "test_user" );
	static const String DB_PASS = STR( "test_pwd" );

	static const String DATABASE_ODBC_MSSQL_TYPE = STR( "Database.Odbc.MsSql" );

	CDatabaseOdbcMsSqlTest::CDatabaseOdbcMsSqlTest()
		: CDatabaseTest( DATABASE_ODBC_MSSQL_TYPE, DB_SERVER, DB_DATABASE, DB_USER, DB_PASS, true, false, STR( "=" ) )
	{
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
		InstallDatabaseMsSql( DB_DATABASE, DB_USER, DB_PASS );
		InstallSourceOdbcMsSql( DB_DATABASE );
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
