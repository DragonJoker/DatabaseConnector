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
	static const String DB_USER = STR( "ariaUser" );
	static const String DB_PASS = STR( "ariaPwd" );

	static const String DATABASE_ODBC_MYSQL_TYPE = STR( "Database.Odbc.MySql" );

	CDatabaseOdbcMySqlTest::CDatabaseOdbcMySqlTest()
		: CDatabaseTest( DATABASE_ODBC_MYSQL_TYPE, DB_SERVER, DB_DATABASE, DB_USER, DB_PASS )
	{
	}

	CDatabaseOdbcMySqlTest::~CDatabaseOdbcMySqlTest()
	{
	}

	void CDatabaseOdbcMySqlTest::DoLoadPlugins()
	{
		LoadPlugins( InitializeSingletons(), false, false, true, false );
	}
}
END_NAMESPACE_DATABASE_TEST
