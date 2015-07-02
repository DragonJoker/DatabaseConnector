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
	static const String DB_DATABASE = STR( "db_test_odbc_mysql" );
	static const String DB_SERVER = STR( "127.0.0.1" );
	static const String DB_USER = STR( "DragonJoker" );
	static const String DB_PASS = STR( "b1cr0sS" );

	static const String DATABASE_ODBC_MSSQL_TYPE = STR( "Database.Odbc.MsSql" );

	CDatabaseOdbcMsSqlTest::CDatabaseOdbcMsSqlTest()
		: CDatabaseTest( DATABASE_ODBC_MSSQL_TYPE, DB_SERVER, DB_DATABASE, DB_USER, DB_PASS )
	{
	}

	CDatabaseOdbcMsSqlTest::~CDatabaseOdbcMsSqlTest()
	{
	}

	void CDatabaseOdbcMsSqlTest::DoLoadPlugins()
	{
		LoadPlugins( InitializeSingletons(), false, false, false, true );
	}
}
END_NAMESPACE_DATABASE_TEST
