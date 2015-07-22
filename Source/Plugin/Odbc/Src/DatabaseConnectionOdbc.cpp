/************************************************************************//**
* @file DatabaseConnectionOdbc.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 3/14/2014 5:03:15 PM
*
*
* @brief CConnectionOdbc class definition.
*
* @details Describes a connection to a database via an ODBC driver.
*
***************************************************************************/

#include "DatabaseOdbcPch.h"

#include "DatabaseConnectionOdbc.h"

#include "DatabaseOdbcHelper.h"
#include "DatabaseStatementOdbc.h"
#include "ExceptionDatabaseOdbc.h"

#include <DatabaseDate.h>
#include <DatabaseDateTime.h>
#include <DatabaseDateTimeHelper.h>
#include <DatabaseTime.h>

#include <DatabaseStringUtils.h>

BEGIN_NAMESPACE_DATABASE_ODBC
{
	static const String ODBC_SQL_TRANSACTION_BEGIN = STR( "BEGIN TRANSACTION " );
	static const String ODBC_SQL_TRANSACTION_COMMIT = STR( "COMMIT TRANSACTION " );
	static const String ODBC_SQL_TRANSACTION_ROLLBACK = STR( "ROLLBACK TRANSACTION " );
	static const String ODBC_SQL_NULL = STR( "NULL" );

	static const std::string ODBC_SQL_SNULL = "NULL";
	static const std::wstring ODBC_SQL_WNULL = L"NULL";

	static const String ERROR_ODBC_QUERY_NEEDING_EXEC_PARAMS = STR( "The query needs to send parameters values at execution, consider using a statement" );
	static const String ERROR_ODBC_QUERY_WITH_OUTPUT_PARAMS = STR( "The query has output parameters values, consider using a statement" );
	static const String ERROR_ODBC_QUERY_EXECUTION = STR( "The query execution failed" );

	static const String INFO_ODBC_ExecDirect = STR( "SQLExecDirect: " );
	static const String INFO_ODBC_AllocHandle = STR( "SQLAllocHandle: " );
	static const String INFO_ODBC_CloseCursor = STR( "SQLCloseCursor" );
	static const String INFO_ODBC_FreeStmt = STR( "SQLFreeStmt" );

	namespace
	{
		SQLHSTMT InitialiseStatement( String const & query, HDBC connection )
		{
			SQLHSTMT statement = SQL_NULL_HSTMT;
			EErrorType errorType = SqlSuccess( SQLAllocHandle( SQL_HANDLE_STMT, connection, &statement ), SQL_HANDLE_DBC, connection, INFO_ODBC_AllocHandle );

			if ( errorType == EErrorType_NONE )
			{
				SQLRETURN code = SQLExecDirectA( statement, const_cast< SqlChar * >( reinterpret_cast< const SqlChar * >( query.c_str() ) ), SQLINTEGER( query.size() ) );
				errorType = SqlSuccess( code, SQL_HANDLE_STMT, statement, INFO_ODBC_ExecDirect );

				if ( code == SQL_NEED_DATA )
				{
					DB_EXCEPT( EDatabaseExceptionCodes_QueryError, ERROR_ODBC_QUERY_NEEDING_EXEC_PARAMS );
				}
				else if ( errorType == EErrorType_ERROR )
				{
					SQLFreeHandle( SQL_HANDLE_STMT, statement );
					statement = SQL_NULL_HSTMT;
					DB_EXCEPT( EDatabaseExceptionCodes_QueryError, ERROR_ODBC_QUERY_EXECUTION );
				}
			}

			return statement;
		}

		void FinaliseStatement( SQLHSTMT statementHandle, SQLRETURN info )
		{
#if defined( _WIN32 )

			if ( info == SQL_PARAM_DATA_AVAILABLE )
			{
				DB_EXCEPT( EDatabaseExceptionCodes_QueryError, ERROR_ODBC_QUERY_WITH_OUTPUT_PARAMS );
			}

#endif
			EErrorType errorType = EErrorType_NONE;
			//OdbcCheck( SQLCloseCursor( statementHandle ), SQL_HANDLE_STMT, statementHandle, INFO_ODBC_CloseCursor );
			OdbcCheck( SQLFreeStmt( statementHandle, SQL_CLOSE ), SQL_HANDLE_STMT, statementHandle, INFO_ODBC_FreeStmt + STR( " (Close)" ) );
			OdbcCheck( SQLFreeStmt( statementHandle, SQL_UNBIND ), SQL_HANDLE_STMT, statementHandle, INFO_ODBC_FreeStmt + STR( " (Unbind)" ) );
			OdbcCheck( SQLFreeStmt( statementHandle, SQL_RESET_PARAMS ), SQL_HANDLE_STMT, statementHandle, INFO_ODBC_FreeStmt + STR( " (ResetParams)" ) );
			SQLFreeHandle( SQL_HANDLE_STMT, statementHandle );
		}
	}

	CDatabaseConnectionOdbc::CDatabaseConnectionOdbc( SQLHENV sqlEnvironmentHandle, const String & server, const String & userName, const String & password, String & connectionString )
		: CDatabaseConnection( server, userName, password )
		, _connectionHandle( NULL )
		, _environmentHandle( sqlEnvironmentHandle )
	{
	}

	CDatabaseConnectionOdbc::~CDatabaseConnectionOdbc()
	{
	}

	std::string CDatabaseConnectionOdbc::WriteText( const std::string & text ) const
	{
		std::string strReturn( text );

		if ( strReturn != ODBC_SQL_SNULL )
		{
			Replace( strReturn, "'", "''" );
			Replace( strReturn, "\\", "\\\\" );
			strReturn = "'" + strReturn + "'";
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionOdbc::WriteNText( const std::wstring & text ) const
	{
		String strReturn( CStrUtils::ToString( text ) );

		if ( strReturn != ODBC_SQL_NULL )
		{
			Replace( strReturn, STR( "'" ), STR( "''" ) );
			Replace( strReturn, STR( "\\" ), STR( "\\\\" ) );
			strReturn = STR( "N'" ) + strReturn + STR( "'" );
		}

		return CStrUtils::ToWStr( strReturn );
	}

	String CDatabaseConnectionOdbc::WriteName( const String & text ) const
	{
		return STR( "[" ) + text + STR( "]" );
	}

	String CDatabaseConnectionOdbc::WriteBool( bool value ) const
	{
		return ( value ? STR( "1" ) : STR( "0" ) );
	}

	String CDatabaseConnectionOdbc::WriteBool( const String & value ) const
	{
		const String lowerCaseValue = CStrUtils::LowerCase( value );
		return ( lowerCaseValue == STR( "x" ) || lowerCaseValue == STR( "oui" ) || lowerCaseValue == STR( "yes" ) || lowerCaseValue == STR( "y" ) || value == STR( "1" ) ? STR( "1" ) : STR( "0" ) );
	}

	HDBC CDatabaseConnectionOdbc::GetHdbc() const
	{
		return _connectionHandle;
	}

	void CDatabaseConnectionOdbc::DoDisconnect()
	{
		if ( _connectionHandle != SQL_NULL_HDBC )
		{
			SQLDisconnect( _connectionHandle );
			SQLFreeHandle( SQL_HANDLE_DBC, _connectionHandle );
			_connectionHandle = NULL;
		}
	}

	bool CDatabaseConnectionOdbc::DoBeginTransaction( const String & name )
	{
		return DoExecuteUpdate( ODBC_SQL_TRANSACTION_BEGIN + name );
	}

	bool CDatabaseConnectionOdbc::DoCommit( const String & name )
	{
		return DoExecuteUpdate( ODBC_SQL_TRANSACTION_COMMIT + name );
	}

	bool CDatabaseConnectionOdbc::DoRollBack( const String & name )
	{
		return DoExecuteUpdate( ODBC_SQL_TRANSACTION_ROLLBACK + name );
	}

	bool CDatabaseConnectionOdbc::DoExecuteUpdate( const String & query)
	{
		DatabaseResultSPtr ret;
		return SqlExecute( shared_from_this(), InitialiseStatement( query, _connectionHandle ), &FinaliseStatement, ret ) == EErrorType_NONE;
	}

	DatabaseResultSPtr CDatabaseConnectionOdbc::DoExecuteSelect( const String & query)
	{
		DatabaseResultSPtr ret;
		SqlExecute( shared_from_this(), InitialiseStatement( query, _connectionHandle ), &FinaliseStatement, ret );
		return ret;
	}
}
END_NAMESPACE_DATABASE_ODBC
