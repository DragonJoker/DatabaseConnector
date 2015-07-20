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
	static const String DB_SERVER = STR( "127.0.0.1" );
	static const String DB_USER = STR( "test_user" );
	static const String DB_PASS = STR( "test_pwd" );

	static const String DATABASE_MYSQL_TYPE = STR( "Database.MySql" );

	CDatabaseMySqlTest::CDatabaseMySqlTest()
		: CDatabaseTest( DATABASE_MYSQL_TYPE, DB_SERVER, DB_DATABASE, DB_USER, DB_PASS, false, true, STR( "=" ), true, false )
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

	CDatabaseMySqlTest::~CDatabaseMySqlTest()
	{
	}

	void CDatabaseMySqlTest::DoLoadPlugins()
	{
		LoadPlugins( InitializeSingletons(), true, false, false, false, false );
	}
}
END_NAMESPACE_DATABASE_TEST
