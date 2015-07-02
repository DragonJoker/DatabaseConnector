/************************************************************************//**
 * @file DatabaseMySqlTest.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 16/07/2014 12:08:57
 *
 *
 * @brief Database MySQL plugin test
 *
 * @details Database MySQL plugin test
 *
 ***************************************************************************/

#include "DatabaseTestPch.h"

#include "DatabaseMySqlTest.h"

BEGIN_NAMESPACE_DATABASE_TEST
{
	static const String DB_DATABASE = STR( "db_test_mysql" );
	static const String DB_SERVER = STR( "tcp://127.0.0.1:3306" );
	static const String DB_USER = STR( "root" );
	static const String DB_PASS = STR( "" );

	static const String DATABASE_MYSQL_TYPE = STR( "Database.MySql" );

	CDatabaseMySqlTest::CDatabaseMySqlTest()
		: CDatabaseTest( DATABASE_MYSQL_TYPE, DB_SERVER, DB_DATABASE, DB_USER, DB_PASS )
	{
	}

	CDatabaseMySqlTest::~CDatabaseMySqlTest()
	{
	}

	void CDatabaseMySqlTest::DoLoadPlugins()
	{
		LoadPlugins( InitializeSingletons(), true, false, false, false );
	}
}
END_NAMESPACE_DATABASE_TEST
