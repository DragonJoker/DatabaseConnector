/************************************************************************//**
* @file DatabaseQuerySqlite.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDatabaseQuerySqlite class declaration.
*
* @details Describes a query for SQLite database.
*
***************************************************************************/

#include "DatabaseSqlitePch.h"

#include "DatabaseQuerySqlite.h"

#include "DatabaseConnectionSqlite.h"
#include "DatabaseSqlite.h"
#include "DatabaseQueryParameterSqlite.h"
#include "ExceptionDatabaseSqlite.h"
#include "DatabaseSqliteHelper.h"

#include <DatabaseStringUtils.h>

#include <DatabaseRow.h>

BEGIN_NAMESPACE_DATABASE_SQLITE
{
	static const String ERROR_SQLITE_MISSING_INITIALIZATION = STR( "Method Initialize must be called before calling method CreateParameter" );
	static const String ERROR_SQLITE_CANT_PREPARE_STATEMENT = STR( "Couldn't prepare the statement" );
	static const String ERROR_SQLITE_EXECUTION = STR( "Couldn't execute the statement : " );
	static const String ERROR_SQLITE_QUERY_INCONSISTENCY = STR( "Number of parameters doesn't match the sizes of parameter containers: " );

	static const TChar * INFO_SQLITE_STATEMENT_PREPARATION = STR( "Statement preparation" );
	static const TChar * INFO_SQLITE_STATEMENT_FINALISATION = STR( "Statement finalisation" );
	static const TChar * INFO_SQLITE_STATEMENT_CLEAR_BINDINGS = STR( "Statement bindings cleanup" );
	static const TChar * INFO_SQLITE_STATEMENT_RESET = STR( "Statement resetting" );
	static const TChar * INFO_SQLITE_STMT_PARAMS_COUNT = STR( "Bind Parameters count: " );

	static const String SQLITE_SQL_DELIM = STR( "?" );
	static const String SQLITE_SQL_PARAM = STR( "@" );
	static const String SQLITE_SQL_SET = STR( "SET @" );
	static const String SQLITE_SQL_NULL = STR( " = NULL;" );
	static const String SQLITE_SQL_SELECT = STR( "SELECT " );
	static const String SQLITE_SQL_AS = STR( " AS " );
	static const String SQLITE_SQL_COMMA = STR( "," );

	CDatabaseQuerySqlite::CDatabaseQuerySqlite( DatabaseConnectionSqlitePtr connection, const String & query )
		: CDatabaseQuery( connection, query )
		, _statement()
		, _connectionSqlite( connection )
		, _paramsCount( 0 )
	{
	}

	CDatabaseQuerySqlite::~CDatabaseQuerySqlite()
	{
		Cleanup();
	}

	EErrorType CDatabaseQuerySqlite::Initialize()
	{
		EErrorType eReturn = EErrorType_ERROR;

		if ( !_query.empty() )
		{
			_paramsCount = uint32_t( std::count( _query.begin(), _query.end(), STR( '?' ) ) );
			_arrayQueries = CStrUtils::Split( _query, STR( "?" ), _paramsCount + 1 );
		}

		CLogger::LogInfo( STR( "Preparing statement for query : " ) + _query );
		assert( _paramsCount == _arrayParams.size() );

		StringStream query;
		unsigned short i = 0;
		auto && itQueries = _arrayQueries.begin();
		auto && itParams = _arrayParams.begin();

		_outInitializers.clear();
		_arrayOutParams.clear();

		_outInitializers.reserve( _arrayParams.size() );
		_arrayOutParams.reserve( _arrayParams.size() );

		while ( itQueries != _arrayQueries.end() && itParams != _arrayParams.end() )
		{
			query << ( *itQueries );
			DatabaseParameterPtr parameter = ( *itParams );

			if ( parameter->GetParamType() == EParameterType_IN )
			{
				query << SQLITE_SQL_DELIM;
			}
			else if ( parameter->GetParamType() == EParameterType_INOUT )
			{
				query << SQLITE_SQL_PARAM + parameter->GetName();
				DatabaseQueryPtr stmt = _connection->CreateQuery( SQLITE_SQL_SET + parameter->GetName() + STR( " = " ) + SQLITE_SQL_DELIM );
				stmt->CreateParameter( parameter->GetName(), parameter->GetType(), parameter->GetLimits(), EParameterType_IN );
				stmt->Initialize();
				_inOutInitializers.push_back( std::make_pair( stmt, parameter ) );
				_arrayOutParams.push_back( parameter );
			}
			else if ( parameter->GetParamType() == EParameterType_OUT )
			{
				query << SQLITE_SQL_PARAM + parameter->GetName();
				DatabaseQueryPtr stmt = _connection->CreateQuery( SQLITE_SQL_SET + parameter->GetName() + SQLITE_SQL_NULL );
				stmt->Initialize();
				_outInitializers.push_back( stmt );
				_arrayOutParams.push_back( parameter );
			}

			++i;
			++itQueries;
			++itParams;
		}

		while ( itQueries != _arrayQueries.end() )
		{
			query << ( *itQueries );
			++itQueries;
		}

		_query = query.str();

		if ( !_arrayOutParams.empty() )
		{
			String sep;
			StringStream queryInOutParam;
			queryInOutParam << SQLITE_SQL_SELECT;

			for ( auto && parameter : _arrayOutParams )
			{
				queryInOutParam << sep << SQLITE_SQL_PARAM << parameter->GetName() << SQLITE_SQL_AS << parameter->GetName();
				sep = SQLITE_SQL_COMMA;
			}

			_stmtOutParams = _connection->CreateQuery( queryInOutParam.str() );
			_stmtOutParams->Initialize();
		}

		sqlite3 * connection = _connectionSqlite->GetConnection();
		std::string query2 = CStrUtils::ToStr( _query );
		SQLiteCheck( sqlite3_prepare_v2( connection, query2.c_str(), int( query2.size() ), &_statement, NULL ), INFO_SQLITE_STATEMENT_PREPARATION, EDatabaseExceptionCodes_StatementError, connection );

		if ( !_statement )
		{
			CLogger::LogError( ERROR_SQLITE_CANT_PREPARE_STATEMENT );
		}
		else
		{
			int count = sqlite3_bind_parameter_count( _statement );

			if ( count == _arrayParams.size() )
			{
				CLogger::LogDebug( StringStream() << INFO_SQLITE_STMT_PARAMS_COUNT << count );
				eReturn = EErrorType_NONE;
			}
			else
			{
				StringStream error;
				error << ERROR_SQLITE_QUERY_INCONSISTENCY << _arrayParams.size() << STR( ", Expected: " ) << count;
				CLogger::LogError( error );
				DB_EXCEPT( EDatabaseExceptionCodes_StatementError, error.str() );
			}
		}

		for ( auto && parameter : _arrayInParams )
		{
			std::static_pointer_cast< CDatabaseQueryParameterSqlite >( parameter )->SetStatement( _statement );
		}

		return eReturn;
	}

	bool CDatabaseQuerySqlite::ExecuteUpdate( EErrorType * result )
	{
		DoPreExecute( result );

		bool bReturn;
		EErrorType eResult = EErrorType_NONE;
		bReturn = _connectionSqlite->ExecuteUpdate( _statement );
		DoPostExecute( result );

		if ( result )
		{
			*result = eResult;
		}

		return bReturn;
	}

	DatabaseResultPtr CDatabaseQuerySqlite::ExecuteSelect( EErrorType * result )
	{
		DoPreExecute( result );

		DatabaseResultPtr pReturn;
		EErrorType eResult = EErrorType_NONE;
		pReturn = _connectionSqlite->ExecuteSelect( _statement );
		DoPostExecute( result );

		if ( result )
		{
			*result = eResult;
		}

		return pReturn;
	}

	void CDatabaseQuerySqlite::Cleanup()
	{
		if ( _statement )
		{
			SQLiteCheck( sqlite3_finalize( _statement ), INFO_SQLITE_STATEMENT_FINALISATION, EDatabaseExceptionCodes_StatementError, _connectionSqlite->GetConnection() );
			_statement = NULL;
		}

		_arrayInParams.clear();
		_arrayOutParams.clear();
		_outInitializers.clear();
		_arrayQueries.clear();
		_paramsCount = 0;
		_stmtOutParams.reset();
	}

	DatabaseParameterPtr CDatabaseQuerySqlite::CreateParameter( const String & name, EFieldType fieldType, EParameterType parameterType )
	{
		DatabaseParameterPtr pReturn = std::make_shared< CDatabaseQueryParameterSqlite >( _connectionSqlite, name, ( unsigned short )_arrayInParams.size() + 1, fieldType, parameterType, std::make_unique< SValueUpdater >( this ) );

		if ( !DoAddParameter( pReturn ) )
		{
			pReturn.reset();
		}
		else if ( parameterType == EParameterType_IN )
		{
			_arrayInParams.push_back( pReturn );
		}

		return pReturn;
	}

	DatabaseParameterPtr CDatabaseQuerySqlite::CreateParameter( const String & name, EFieldType fieldType, uint32_t limits, EParameterType parameterType )
	{
		DatabaseParameterPtr pReturn = std::make_shared< CDatabaseQueryParameterSqlite >( _connectionSqlite, name, ( unsigned short )_arrayInParams.size() + 1, fieldType, limits, parameterType, std::make_unique< SValueUpdater >( this ) );

		if ( !DoAddParameter( pReturn ) )
		{
			pReturn.reset();
		}
		else if ( parameterType == EParameterType_IN )
		{
			_arrayInParams.push_back( pReturn );
		}

		return pReturn;
	}

	void CDatabaseQuerySqlite::DoPreExecute( EErrorType * result )
	{
		for ( auto && it : _inOutInitializers )
		{
			if ( it.second->GetObjectValue().IsNull() )
			{
				it.first->SetParameterNull( 0 );
			}
			else
			{
				it.first->SetParameterValue( 0, it.second );
			}

			it.first->ExecuteUpdate( result );
		}

		for ( auto && query : _outInitializers )
		{
			query->ExecuteUpdate( result );
		}
	}

	void CDatabaseQuerySqlite::DoPostExecute( EErrorType * result )
	{
		if ( !_arrayOutParams.empty() )
		{
			DatabaseResultPtr pReturn = _stmtOutParams->ExecuteSelect();

			if ( pReturn && pReturn->GetRowCount() )
			{
				DatabaseRowPtr row = pReturn->GetFirstRow();

				for ( auto && parameter : _arrayOutParams )
				{
					if ( parameter->GetParamType() == EParameterType_INOUT || parameter->GetParamType() == EParameterType_OUT )
					{
						DatabaseFieldPtr field;

						try
						{
							field = row->GetField( parameter->GetName() );
						}
						catch ( CExceptionDatabase & exc )
						{
							CLogger::LogError( exc.GetFullDescription() );
						}

						if ( field )
						{
							parameter->SetValue( field );
						}
					}
				}
			}
		}

		SQLiteCheck( sqlite3_clear_bindings( _statement ), INFO_SQLITE_STATEMENT_CLEAR_BINDINGS, EDatabaseExceptionCodes_StatementError, _connectionSqlite->GetConnection() );
		SQLiteCheck( sqlite3_reset( _statement ), INFO_SQLITE_STATEMENT_RESET, EDatabaseExceptionCodes_StatementError, _connectionSqlite->GetConnection() );
	}
}
END_NAMESPACE_DATABASE_SQLITE