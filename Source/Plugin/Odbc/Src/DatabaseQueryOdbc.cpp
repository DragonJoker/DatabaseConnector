/************************************************************************//**
 * @file DatabaseQueryOdbc.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 *
 *
 * @brief CDatabaseQueryOdbc class definition.
 *
 * @details Describes a statement for ODBC database.
 *
 ***************************************************************************/

#include "DatabaseOdbcPch.h"

#include "DatabaseQueryOdbc.h"

#include "DatabaseConnectionOdbc.h"
#include "DatabaseOdbcHelper.h"
#include "DatabaseQueryParameterOdbc.h"
#include "ExceptionDatabaseOdbc.h"

#include <DatabaseStringUtils.h>
#include <functional>

#define SQL_SOPT_SS_DEFER_PREPARE (SQL_SOPT_SS_BASE+7)

BEGIN_NAMESPACE_DATABASE_ODBC
{
	static const String ERROR_ODBC_QUERY_UNKNOWN_POINTER = STR( "The pointer given by SQLParamData is unknown to the query" );

	static const String INFO_ODBC_EXECUTE_QUERY = STR( "Execute query: " );

	static const String INFO_ODBC_ExecDirect = STR( "SQLExecDirect: " );
	static const String INFO_ODBC_AllocHandle = STR( "SQLAllocHandle: " );
	static const String INFO_ODBC_SetStmtAttr = STR( "SQLSetStmtAttr: " );
	static const String INFO_ODBC_ParamData = STR( "SQLParamData " );

	CDatabaseQueryOdbc::CDatabaseQueryOdbc( DatabaseConnectionOdbcPtr connection, const String & query )
		: CDatabaseQuery( connection, query )
		, _connectionOdbc( connection )
	{
	}

	CDatabaseQueryOdbc::~CDatabaseQueryOdbc()
	{
		Cleanup();
	}

	EErrorType CDatabaseQueryOdbc::Initialize()
	{
		return EErrorType_NONE;
	}

	void CDatabaseQueryOdbc::Cleanup()
	{
	}

	bool CDatabaseQueryOdbc::ExecuteUpdate( EErrorType * result )
	{
		DatabaseResultPtr rs;
		EErrorType ret = DoExecute( rs );

		if ( result )
		{
			*result = ret;
		}

		return ret == EErrorType_NONE;
	}

	DatabaseResultPtr CDatabaseQueryOdbc::ExecuteSelect( EErrorType * result )
	{
		DatabaseResultPtr rs;
		EErrorType ret = DoExecute( rs );

		if ( result )
		{
			*result = ret;
		}

		return rs;
	}

	DatabaseParameterPtr CDatabaseQueryOdbc::CreateParameter( const String & name, EFieldType fieldType, EParameterType parameterType )
	{
		DatabaseParameterPtr pReturn = std::make_shared< CDatabaseQueryParameterOdbc >( _connectionOdbc, name, uint16_t( _arrayParams.size() + 1 ), fieldType, parameterType, std::make_unique< SValueUpdater >( this ) );

		if ( !DoAddParameter( pReturn ) )
		{
			pReturn.reset();
		}

		return pReturn;
	}

	DatabaseParameterPtr CDatabaseQueryOdbc::CreateParameter( const String & name, EFieldType fieldType, uint32_t limits, EParameterType parameterType )
	{
		DatabaseParameterPtr pReturn = std::make_shared< CDatabaseQueryParameterOdbc >( _connectionOdbc, name, uint16_t( _arrayParams.size() + 1 ), fieldType, limits, parameterType, std::make_unique< SValueUpdater >( this ) );

		if ( !DoAddParameter( pReturn ) )
		{
			pReturn.reset();
		}

		return pReturn;
	}

	EErrorType CDatabaseQueryOdbc::DoPreExecute( HSTMT statementHandle )
	{
		EErrorType eResult = EErrorType_NONE;

		for ( auto && parameter : _arrayParams )
		{
			eResult = std::static_pointer_cast< CDatabaseQueryParameterOdbc >( parameter )->GetBinding().UpdateValue();
		}

		if ( eResult == EErrorType_NONE )
		{
			SQLRETURN retCode = SQLExecDirectA( statementHandle, ( SqlChar * )_query.c_str(), SQLINTEGER( _query.size() ) );
			eResult = SqlSuccess( retCode, SQL_HANDLE_STMT, statementHandle, INFO_ODBC_ExecDirect + _query );

			if ( retCode == SQL_NEED_DATA )
			{
				SQLPOINTER pAddress;
				std::map< void *, DatabaseParameterPtr >::iterator it;

				while ( ( retCode = SQLParamData( statementHandle, &pAddress ) ) == SQL_NEED_DATA && eResult == EErrorType_NONE )
				{
					it = _mapParamsByPointer.find( pAddress );

					if ( it != _mapParamsByPointer.end() )
					{
						eResult = std::static_pointer_cast< CDatabaseQueryParameterOdbc >( it->second )->GetBinding().PutData();
					}
					else
					{
						CLogger::LogError( ERROR_ODBC_QUERY_UNKNOWN_POINTER );
					}
				}

				SqlSuccess( retCode, SQL_HANDLE_STMT, statementHandle, INFO_ODBC_EXECUTE_QUERY + _query );
			}
		}

		return eResult;
	}

	EErrorType CDatabaseQueryOdbc::DoExecute( DatabaseResultPtr & ret )
	{
		HDBC hParentStmt = _connectionOdbc->GetHdbc();
		CLogger::LogInfo( STR( "Executing query : " ) + _query );
		SQLHSTMT statementHandle;
		EErrorType eResult = SqlSuccess( SQLAllocHandle( SQL_HANDLE_STMT, hParentStmt, &statementHandle ), SQL_HANDLE_DBC, hParentStmt, INFO_ODBC_AllocHandle );

		for ( auto && parameter : _arrayParams )
		{
			std::static_pointer_cast< CDatabaseQueryParameterOdbc >( parameter )->Initialize( statementHandle );
		}

		if ( eResult == EErrorType_NONE )
		{
			eResult = DoPreExecute( statementHandle );
		}

		if ( eResult == EErrorType_NONE )
		{
			eResult = SqlExecute( _connection, statementHandle, std::bind( &CDatabaseQueryOdbc::OnResultSetFullyFetched, this, std::placeholders::_1, std::placeholders::_2 ), ret );
		}

		if ( statementHandle != SQL_NULL_HSTMT )
		{
			SQLFreeHandle( SQL_HANDLE_STMT, statementHandle );
			statementHandle = SQL_NULL_HSTMT;
		}

		return eResult;
	}

	void CDatabaseQueryOdbc::OnResultSetFullyFetched( HSTMT statementHandle, SQLRETURN info )
	{
#if defined( _WIN32 )

		if ( info == SQL_PARAM_DATA_AVAILABLE )
		{
			EErrorType eResult = EErrorType_NONE;
			SQLPOINTER pAddress = NULL;
			SQLRETURN retCode = 0;

			while ( ( retCode = SQLParamData( statementHandle, &pAddress ) ) == SQL_PARAM_DATA_AVAILABLE && eResult == EErrorType_NONE )
			{
				auto && it = _mapParamsByPointer.find( pAddress );

				if ( it != _mapParamsByPointer.end() )
				{
					eResult = std::static_pointer_cast< CDatabaseQueryParameterOdbc >( it->second )->GetBinding().GetData();
				}
				else
				{
					CLogger::LogError( ERROR_ODBC_QUERY_UNKNOWN_POINTER );
				}
			}

			SqlSuccess( retCode, SQL_HANDLE_STMT, statementHandle, INFO_ODBC_ParamData );
		}

#endif
	}
}
END_NAMESPACE_DATABASE_ODBC
