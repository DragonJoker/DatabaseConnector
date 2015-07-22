/************************************************************************//**
* @file DatabaseSqliteHelper.h
* @author Sylvain Doremus
* @version 1.0
* @date 07/17/2015 10:27 AM
*
*
* @brief Helper classes and functions.
*
***************************************************************************/

#ifndef ___DATABASE_SQLITE_HELPER_H___
#define ___DATABASE_SQLITE_HELPER_H___

#include "DatabaseSqlitePrerequisites.h"

#include "DatabaseException.h"

BEGIN_NAMESPACE_DATABASE_SQLITE
{
	/** Retrieves The columns composing the statement result
	@param[in] statement
		The statement
	@param[in] connection
		The database connection
	*/
	DatabaseValuedObjectInfosPtrArray SqliteGetColumns( sqlite3_stmt * statement );

	/** Fetches given statement results
	@param[in] statement
		The statement
	@param[in] columns
		The result set columns
	@param[in] connection
		The database connection
	*/
	DatabaseResultSPtr SqliteFetchResult( sqlite3_stmt * statement, DatabaseValuedObjectInfosPtrArray const & columns, DatabaseConnectionSqliteSPtr connection );

	/** Prepares a statement.
	@remarks
		Takes care of the fact that the database could be locked
	@param[in] query
		The statement's query
	@param[in]
		The connection on which the statement is to be prepared
	*/
	sqlite3_stmt * SqlitePrepareStatement( const String & query, sqlite3 * connection );

	/** Prepares a statement.
	@remarks
		Takes care of the fact that the database could be locked
	@param[in] query
		The statement's query
	@param[in]
		The connection on which the statement is to be prepared
	*/
	sqlite3_stmt * SqlitePrepareStatement( const String & query, sqlite3 * connection );

	/** Prepares a statement.
	@remarks
		Takes care of the fact that the database could be locked
	@param[in] query
		The statement's query
	@param[in]
		The connection on which the statement is to be prepared
	*/
	sqlite3_stmt * SqlitePrepareStatement( const String & query, sqlite3 * connection );

	/** Prepares a statement.
	@remarks
		Takes care of the fact that the database could be locked
	@param[in] query
		The statement's query
	@param[in]
		The connection on which the statement is to be prepared
	*/
	sqlite3_stmt * SqlitePrepareStatement( const String & query, sqlite3 * connection );

	/** Checks the given code, throws an exception if it is not an acceptable one
	@param[in] code
		The code to test
	param[in] msg
		The message to put in the exception
	param[in] database
		The SQLite connection, to retrieve error text
	*/
	void SQLiteCheck( int code, TChar const * msg, EDatabaseExceptionCodes exc, sqlite3 * database );

	/** Checks the given code, throws an exception if it is not an acceptable one
	@param[in] code
		The code to test
	param[in] msg
		The message to put in the exception
	param[in] database
		The SQLite connection, to retrieve error text
	*/
	void SQLiteCheck( int code, std::ostream const & stream, EDatabaseExceptionCodes exc, sqlite3 * database );

	/** Base binding uupdater class
	*/
	struct SSqliteBindingBase
	{
		/** Constructor
		@param statement
			The statement
		@param connection
			The database connection
		@param index
			The parameter index
		*/
		SSqliteBindingBase( sqlite3_stmt * statement, sqlite3 * connection, uint16_t index )
			: _statement( statement )
			, _connection( connection )
			, _index( index )
		{
		}

		/** Destructor
		*/
		virtual ~SSqliteBindingBase()
		{
		}

		/** Updates the SQLite binding
		*/
		virtual void UpdateValue() = 0;

		//! The statement
		sqlite3_stmt * _statement;
		//! The database connection
		sqlite3 * _connection;
		// The parameter index
		uint16_t _index;
	};
}
END_NAMESPACE_DATABASE_SQLITE

#endif // ___DATABASE_CONNECTION_SQLITE_H___
