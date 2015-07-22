/************************************************************************//**
* @file DatabaseOdbcHelper.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/18/2014 2:47:20 PM
*
*
* @brief Helper functions for ODBC.
*
* @details Helper functions for ODBC.
*
***************************************************************************/

#ifndef ___DATABASE_ODBC_HELPER_H___
#define ___DATABASE_ODBC_HELPER_H___

#include "DatabaseOdbcPrerequisites.h"

#include <EErrorType.h>
#include <EFieldType.h>

BEGIN_NAMESPACE_DATABASE_ODBC
{
	typedef std::function< void ( SQLHSTMT statementHandle, SQLRETURN info ) > FuncResultSetFullyFetched;
	/** Manages an SQL error.
	@param[in] typeHandle
		Handle of type.
	@param[in] handle
		Handle.
	@param[in] query
		Query text.
	@param[in] error
		Tells if the messages are errors (true) or warnings (false)
	@return
		Error code.
	*/
	EErrorType SqlError( SQLSMALLINT typeHandle, SQLHANDLE handle, const String & query, bool error );

	/** Manages an SQL success.
	@param[in] rc
		Return code of SQL function.
	@param[in] typeHandle
		Handle of type.
	@param[in] handle
		Handle.
	@param[in] query
		Query text.
	@return
		Error code.
	*/
	EErrorType SqlSuccess( SQLRETURN rc, SQLSMALLINT typeHandle = 0, SQLHANDLE handle = NULL, const String & query = STR( "" ) );

	/** Retrieves the field type corresponding to concise type.
	@param[in] sqlType
		SQL type identifier.
	@param[in] limits
		digits count for integer type, used to select int24 or int32.
	@return
		Database field type.
	*/
	EFieldType GetFieldTypeFromConciseType( SQLLEN sqlType, uint32_t limits );

	/** Retrieves the field type corresponding to C type.
	@param[in] sqlType
		SQL type identifier.
	@return
		Database field type.
	*/
	EFieldType GetFieldTypeFromCType( SQLSMALLINT sqlType );

	/** Execute an SQL operation.
	@param[in] connection
		Database connection.
	@param[in] statementHandle
		Handle of statement to execute.
	@param[in] onFullyfetched
		Method called when the result set is fetched
	@param[out] pReturn
		Receives the result set, if any
	@return
		the result.
	*/
	EErrorType SqlExecute( DatabaseConnectionSPtr connection, SQLHSTMT statementHandle, FuncResultSetFullyFetched onFullyfetched, DatabaseResultSPtr & pReturn );
}
END_NAMESPACE_DATABASE_ODBC

#endif // ___DATABASE_ODBC_HELPER_H___
