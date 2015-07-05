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
	static const String DB_USER = STR( "user" );
	static const String DB_PASS = STR( "pwd" );

	static const String DATABASE_SQLITE_TYPE = STR( "Database.SQLite" );

	CDatabaseSqliteTest::CDatabaseSqliteTest()
		: CDatabaseTest( DATABASE_SQLITE_TYPE, DB_SERVER, DB_DATABASE, DB_USER, DB_PASS )
	{
		_createTable += STR( "CREATE TABLE Test" );
		_createTable += STR( "(	IDTest INTEGER PRIMARY KEY\n" );
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

	CDatabaseSqliteTest::~CDatabaseSqliteTest()
	{
	}

	void CDatabaseSqliteTest::DoLoadPlugins()
	{
		LoadPlugins( InitializeSingletons(), false, true, false, false );
	}
}
END_NAMESPACE_DATABASE_TEST
