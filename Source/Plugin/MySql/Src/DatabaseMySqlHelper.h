/************************************************************************//**
* @file DatabaseConnectionMySql.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/14/2014 5:03:05 PM
*
*
* @brief CDatabaseConnectionMySql class declaration.
*
* @details Describes a connection to a database via an MYSQL driver.
*
***************************************************************************/

#ifndef ___DATABASE_MYSQL_HELPER_H___
#define ___DATABASE_MYSQL_HELPER_H___

#include "DatabaseMySqlPrerequisites.h"

#include <DatabaseException.h>

#include <mysql.h>

BEGIN_NAMESPACE_DATABASE_MYSQL
{
	/** Base binding holder for MySQL parameters or fields
	*/
	struct SMySqlBindBase
	{
		/** Constructor
		@param[in]
			The bind
		*/
		SMySqlBindBase( MYSQL_BIND & bind )
			: _bind( bind )
		{
			bind.error = &_error;
			bind.is_null = &_null;
		}

		//! Tells if the value is NULL
		my_bool _null = 0;
		//! Receives the error code
		my_bool _error = 0;
		//! The bind
		MYSQL_BIND & _bind;
	};

	/** Base binding class for fields
	*/
	struct SInMySqlBindBase
		: public SMySqlBindBase
	{
		/** Constructor
		@param[in] bind
			The bind
		*/
		SInMySqlBindBase( MYSQL_BIND & bind )
			: SMySqlBindBase( bind )
		{
			bind.length = &_length;
		}

		//! The reserved length
		unsigned long _length = 0;
	};

	/** Base binding class for parameters
	*/
	struct SOutMySqlBindBase
		: public SMySqlBindBase
	{
		/** Constructor
		@param[in] bind
			The bind
		@param[in] type
			The parameter type
		@param[in] parameter
			The parameter
		*/
		SOutMySqlBindBase( MYSQL_BIND & bind, enum_field_types type, CDatabaseStatementParameterMySql & parameter );

		/** Function called when CDatabaseParameter::SetValue/SetValueFast is called, to update the bind
		*/
		virtual void UpdateValue() = 0;

		//! The connection to the MySQL database
		MYSQL * _connection;
		//! The statement
		MYSQL_STMT * _statement;
	};

	/** Retrieves the columns from the statement
	@param[in] stmt
		The statement
	@param[in] connection
		The database connection
	@param[out] inbinds
		Receives the internal bindings
	@param[out]
		Receives the MySQL bindings
	@return
		The columns
	*/
	DatabaseFieldInfosPtrArray MySqlGetColumns( MYSQL_STMT * stmt, DatabaseConnectionMySqlPtr connection, std::vector< std::unique_ptr< SInMySqlBindBase > > & inbinds, std::vector< MYSQL_BIND > & binds );

	/** Fetches the statement's results
	@param[in] stmt
		The statement
	@param[in] columns
		The columns
	@param[in] inbinds
		The internal bindings
	@param[in] binds
		The MySQL bindings
	@return
		The result set, which can be empty, for UPDATE or INSERT queries
	*/
	DatabaseResultPtr MySqlFetchResult( MYSQL_STMT * statement, DatabaseFieldInfosPtrArray const & columns, DatabaseConnectionMySqlPtr connection, std::vector< std::unique_ptr< SInMySqlBindBase > > const & inbinds, std::vector< MYSQL_BIND > & binds );

	/** Checks if the given code is acceptable, and if not, throws an exception
	@param[in] code
		The return code, from a MySQL command
	@param[in] msg
		The message to put in the exception
	@param[in] exc
		The exception code
	@param[in] connection
		The MySQL database connection, to retrieve a detailed error message
	*/
	void MySQLCheck( int code, TChar const * msg, EDatabaseExceptionCodes exc, MYSQL * connection );

	/** Converts a CDate to a MYSQL_TIME
	@param[in] date
		The CDate
	@return
		The MYSQL_TIME
	*/
	MYSQL_TIME MySqlTimeFromCDate( CDate const & date );

	/** Converts a CDateTime to a MYSQL_TIME
	@param[in] dateTime
		The CDateTime
	@return
		The MYSQL_TIME
	*/
	MYSQL_TIME MySqlTimeFromCDateTime( CDateTime const & dateTime );

	/** Converts a CTime to a MYSQL_TIME
	@param[in] time
		The CTime
	@return
		The MYSQL_TIME
	*/
	MYSQL_TIME MySqlTimeFromCTime( CTime const & time );

	/** Sends data to the server, using mysql_stmt_send_long_data
	@param[in] value
		The value from which the data comes
	@param[in] index
		The parameter index
	@param[in] statement
		The statement
	@param[in] connection
		The MySQL database connection
	*/
	void MySqlSendLongData( CDatabaseValueBase & value, unsigned int index, MYSQL_STMT * statement, MYSQL * connection );

}
END_NAMESPACE_DATABASE_MYSQL

#endif // ___DATABASE_CONNECTION_MYSQL_H___