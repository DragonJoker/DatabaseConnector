DatabaseConnector
=================

This library allows connecting to and working with a database, independently from teh chosen driver.

Features
-------

- Immediate mode for queries.
- Prepared statements, with support for output values.
- Transactions.

Drivers
-------

- SQLite.
- MySQL.
- PostgreSQL.
- ODBC.


Examples
--------
- **Connection to a database:**
```cpp
// Select database driver
auto database = Database::CFactoryManager::Instance().CreateInstance( "Database.MySql" );
database->Initialise( dbHost, dbUser, dbPass );

// Connect to database
auto connection = database->RetrieveConnection();
connection->SelectDatabase( dbName );
```

- **Execute a select in immediate mode:**
```cpp
auto result = connection->ExecuteSelect( "SELECT Id, Name FROM MyTable" );
if ( result && result->GetRowCount() )
{
	uint64_t rowCount = result->GetRowCount();

	for ( uint64_t i = 0 ; i < rowCount ; i++ )
	{
		auto row = result->GetNextRow();
		uint32_t id;
		row->Get( 0, id );
		String name;
		row->Get( 1, name );
		std::cout << id << " - " << name << std::endl;
	}
}
```

- **Execute an update in immediate mode:**
```cpp
connection->ExecuteUpdate( "UPDATE MyTable SET Name='coin' WHERE Id=1" );
```

- **Prepare and execute statement:**
```cpp
auto statement = connection->CreateStatement( "UPDATE MyTable SET Name=? WHERE Id=?" );
auto name = statement->CreateParameter( "Name", EFieldType_VARCHAR, 255 );
auto id = statement->CreateParameter( "Id", EFieldType_UINT32, 255 );
name->SetValue( "coin" );
id->SetValue( 1u );
statement->ExecuteUpdate();
name->SetValue( "glop" );
id->SetValue( 2u );
statement->ExecuteUpdate();
```

- **Handle transactions:**
```cpp
auto statement = connection->CreateStatement( "UPDATE MyTable SET Name=? WHERE Id=?" );
auto name = statement->CreateParameter( "Name", EFieldType_VARCHAR, 255 );
auto id = statement->CreateParameter( "Id", EFieldType_UINT32, 255 );

connection->BeginTransaction( "UpdateTransaction1" );
name->SetValue( "coin" );
id->SetValue( 1u );
statement->ExecuteUpdate();
connection->RollBack( "UpdateTransaction1" );

connection->BeginTransaction( "UpdateTransaction2" );
name->SetValue( "glop" );
id->SetValue( 2u );
statement->ExecuteUpdate();
connection->Commit( "UpdateTransaction2" );
```
