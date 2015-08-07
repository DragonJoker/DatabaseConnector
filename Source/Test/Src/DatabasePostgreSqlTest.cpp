/************************************************************************//**
 * @file DatabasePostgreSqlTest.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 16/07/2014 12:08:57
 *
 *
 * @brief Database PostgreSQL plugin test
 *
 * @details Database PostgreSQL plugin test
 *
 ***************************************************************************/

#include "DatabaseTestPch.h"

#if defined( TESTING_PLUGIN_POSTGRE )

#include "DatabasePostgreSqlTest.h"

BEGIN_NAMESPACE_DATABASE_TEST
{
	static const String DB_DATABASE = STR( "db_test_postgresql" );
	static const String DB_SERVER = STR( "localhost" );
	static const String DB_USER = STR( "test_user" );
	static const String DB_PASS = STR( "test_pwd" );

	static const String DATABASE_POSTGRESQL_TYPE = STR( "Database.PostgreSql" );

	static STestConfig g_config = { false, true, STR( "=" ), false, false, false, false };

	CDatabasePostgreSqlTest::CDatabasePostgreSqlTest()
		: CDatabaseTest( DATABASE_POSTGRESQL_TYPE, DB_SERVER, DB_DATABASE, DB_USER, DB_PASS, g_config )
	{
		_createTable += STR( "CREATE TABLE Test" );
		_createTable += STR( "(	IDTest SERIAL PRIMARY KEY\n" );
		_createTable += STR( ",	IntField INT\n" );
		_createTable += STR( ",	IntegerField INTEGER\n" );
		_createTable += STR( ",	TinyIntField SMALLINT\n" );
		_createTable += STR( ",	SmallIntField SMALLINT\n" );
		_createTable += STR( ",	MediumIntField INTEGER\n" );
		_createTable += STR( ",	BigIntField BIGINT\n" );
		_createTable += STR( ",	Int2Field SMALLINT\n" );
		_createTable += STR( ",	Int8Field BIGINT\n" );
		_createTable += STR( ",	RealField DOUBLE PRECISION\n" );
		_createTable += STR( ",	DoubleField DOUBLE PRECISION\n" );
		_createTable += STR( ",	DoublePrecisionField DOUBLE PRECISION\n" );
		_createTable += STR( ",	FloatField REAL\n" );
		_createTable += STR( ",	NumericField NUMERIC\n" );
		_createTable += STR( ",	DecimalField DECIMAL(10,5)\n" );
		_createTable += STR( ",	BooleanField BOOLEAN\n" );
		_createTable += STR( ",	DateField DATE\n" );
		_createTable += STR( ",	DateTimeField TIMESTAMP WITHOUT TIME ZONE\n" );
		_createTable += STR( ",	CharacterField CHARACTER(20)\n" );
		_createTable += STR( ",	VarcharField VARCHAR(255)\n" );
		_createTable += STR( ",	NcharField CHARACTER(55)\n" );
		_createTable += STR( ",	NVarcharField VARCHAR(100)\n" );
		_createTable += STR( ",	TextField TEXT\n" );
		_createTable += STR( ",	BlobField BYTEA\n" );
		_createTable += STR( ");" );
	}

	CDatabasePostgreSqlTest::~CDatabasePostgreSqlTest()
	{
	}

	void CDatabasePostgreSqlTest::DoLoadPlugins()
	{
		LoadPlugins( InitialiseSingletons(), false, false, false, false, true );
	}
}
END_NAMESPACE_DATABASE_TEST

#endif
