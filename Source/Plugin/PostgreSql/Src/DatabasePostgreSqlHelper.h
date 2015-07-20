/************************************************************************//**
* @file DatabaseConnectionPostgreSql.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/14/2014 5:03:05 PM
*
*
* @brief CDatabaseConnectionPostgreSql class declaration.
*
* @details Describes a connection to a database via an MYSQL driver.
*
***************************************************************************/

#ifndef ___DATABASE_POSTGRESQL_HELPER_H___
#define ___DATABASE_POSTGRESQL_HELPER_H___

#include "DatabasePostgreSqlPrerequisites.h"

#include <DatabaseException.h>

#include <libpq-fe.h>

BEGIN_NAMESPACE_DATABASE_POSTGRESQL
{
	/** The structure holding the base PostgreSQL parameters binding
	*/
	struct PGbind
	{
		//! The parameter value
		char *& value;
		//! The parameter value length
		int & length;
		//! The parameter value format (0=>text, 1=>binary)
		int & format;
	};

	/** Base binding class for fields
	*/
	struct SInPostgreSqlBindBase
	{
		/** Constructor
		@param[in] index
			The field index
		@param[in] result
			The result set
		*/
		SInPostgreSqlBindBase( int index, PGresult * result );

		/** Tells if the value is NULL for this field
		@param[in] row
			The row index
		*/
		bool IsNull( int row )const;

		//! The field index
		int _index;
		//! The result set
		PGresult * _result;
	};

	/** Base binding class for parameters
	*/
	struct SOutPostgreSqlBindBase
	{
		/** Constructor
		@param[in] bind
			The bind
		@param[in] type
			The parameter type
		@param[in] parameter
			The parameter
		*/
		SOutPostgreSqlBindBase( PGbind & bind, Oid const & type, CDatabaseParameterPostgreSql & parameter );

		/** Function called when CDatabaseParameter::SetValue/SetValueFast is called, to update the bind
		*/
		void UpdateValue();

		/** Function called by UpdateValue, if the value is not null
		*/
		virtual void DoUpdateValue() = 0;

		//! The connection to the PostgreSQL database
		PGconn * _connection;
		//! The statement
		std::string const & _statement;
		//! The value
		CDatabaseValueBase & _valueBase;
		//! The bind
		PGbind & _bind;
	};

	/** Retrieves the columns from the result set
	@param[in] result
		The result set
	@param[in] connection
		The database connection
	@param[out] binds
		Receives the internal bindings
	@return
		The columns
	*/
	DatabaseFieldInfosPtrArray PostgreSqlGetColumns( PGresult * result, DatabaseConnectionPostgreSqlPtr connection, std::vector< std::unique_ptr< SInPostgreSqlBindBase > > & binds );

	/** Fetches the result set
	@param[in] result
		The result set
	@param[in] columns
		The columns
	@param[in] binds
		The internal bindings
	@return
		The result set, which can be empty, for UPDATE or INSERT queries
	*/
	DatabaseResultPtr PostgreSqlFetchResult( PGresult * result, DatabaseFieldInfosPtrArray const & columns, DatabaseConnectionPostgreSqlPtr connection, std::vector< std::unique_ptr< SInPostgreSqlBindBase > > const & binds );

	/** Checks if the given code is acceptable, and if not, throws an exception
	@param[in] code
		The return code, from a PostgreSQL command
	@param[in] msg
		The message to put in the exception
	@param[in] exc
		The exception code
	@param[in] connection
		The PostgreSQL database connection, to retrieve a detailed error message
	*/
	void PostgreSQLCheck( PGresult * result, TChar const * msg, EDatabaseExceptionCodes exc, PGconn * connection );

	/** Sends data to the server, using mysql_stmt_send_long_data
	@param[in] value
		The value from which the data comes
	@param[in] statement
		The statement
	@param[in] connection
		The PostgreSQL database connection
	*/
	void PostgreSqlSendLongData( CDatabaseValueBase & value, std::string const & statement, PGconn * connection );
	
	/** Retrieves the Oid correponding to the given field type
	@param[in] type
		The field type
	*/
	Oid GetOidFromFieldType( EFieldType type );

	/** Retrieves the textual connection status name
	@param[in] status
		The connection status
	*/
	String GetStatusName( ConnStatusType status );

}
END_NAMESPACE_DATABASE_POSTGRESQL

#endif // ___DATABASE_CONNECTION_POSTGRESQL_H___
