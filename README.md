<p align="center">
  <a href="https://github.com/DragonJoker/DatabaseConnector/actions?query=workflow%3ABuild+event%3Apush"><img alt="Build status" src="https://github.com/DragonJoker/DatabaseConnector/workflows/Build/badge.svg?event=push"></a>
</p>


DatabaseConnector
=================

This library allows connecting to and working with a database, independently from teh chosen driver.

Drivers
-------

- SQLite.
- MySQL.
- PostgreSQL.
- ODBC.


Examples
--------
```cpp
// Select database driver
auto database = Database::CFactoryManager::Instance().CreateInstance( "Database.MySql" );
database->Initialise( dbHost, dbUser, dbPass );

// Connect to database
Database::String res;
database->CreateConnection( res );
auto connection = database->RetrieveConnection();
connection->SelectDatabase( dbName );

// Execute a select
Database::DatabaseResultSPtr result = connection->ExecuteSelect( "SELECT Id, Name FROM MyTable" );
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

// Execute an update
connection->ExecuteUpdate( "UPDATE MyTable SET Name='coin' WHERE Id=1" );

// Prepare and execute statement
auto statement = connection->CreateStatement( "UPDATE MyTable SET Name=? WHERE Id=1" );
auto param = statement->CreateParameter( "Name", EFieldType_VARCHAR, 255 );
param->SetValue( "coin" );
statement->ExecuteUpdate();
```
