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

#include "DatabaseSqliteTest.h"

BEGIN_NAMESPACE_DATABASE_TEST
{
	static const String DB_DATABASE = STR( "db_test.sqlite" );
	static const String DB_SERVER = STR( "./" );
	static const String DB_USER = STR( "ariaUser" );
	static const String DB_PASS = STR( "ariaPwd" );

	static const String DATABASE_SQLITE_TYPE = STR( "Database.SQLite" );

	CDatabaseSqliteTest::CDatabaseSqliteTest()
		: CDatabaseTest( DATABASE_SQLITE_TYPE, DB_SERVER, DB_DATABASE, DB_USER, DB_PASS )
	{
	}

	CDatabaseSqliteTest::~CDatabaseSqliteTest()
	{
	}

	void CDatabaseSqliteTest::DoLoadPlugins()
	{
		LoadPlugins( InitializeSingletons(), false, true, false, false );
	}
}
END_NAMESPACE_DATABASE_TEST
