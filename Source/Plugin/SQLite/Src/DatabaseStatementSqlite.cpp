/************************************************************************//**
* @file DatabaseStatementSqlite.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDatabaseStatementSqlite class definition.
*
* @details Describes a statement for SQLITE database.
*
***************************************************************************/

#include "DatabaseSqlitePch.h"

#include "DatabaseStatementSqlite.h"

#include "DatabaseConnectionSqlite.h"
#include "DatabaseSqlite.h"
#include "DatabaseStatementParameterSqlite.h"
#include "ExceptionDatabaseSqlite.h"
#include "DatabaseSqliteHelper.h"

#include <DatabaseStringUtils.h>

#include <DatabaseRow.h>

BEGIN_NAMESPACE_DATABASE_SQLITE
{
	static const String ERROR_SQLITE_MISSING_INITIALIZATION = STR( "Method Initialize must be called before calling method CreateParameter" );
	static const String ERROR_SQLITE_CANT_PREPARE_STATEMENT = STR( "Couldn't prepare the statement" );
	static const String ERROR_SQLITE_QUERY_INCONSISTENCY = STR( "Number of parameters doesn't match the sizes of parameter containers: " );
	static const String ERROR_SQLITE_LOST_CONNECTION = STR( "The statement has lost his connection" );

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

	CDatabaseStatementSqlite::CDatabaseStatementSqlite( DatabaseConnectionSqliteSPtr connection, const String & query )
		: CDatabaseStatement( connection, query )
		, _statement()
		, _connectionSqlite( connection )
		, _paramsCount( 0 )
	{
	}

	CDatabaseStatementSqlite::~CDatabaseStatementSqlite()
	{
		Cleanup();
	}

	DatabaseParameterSPtr CDatabaseStatementSqlite::CreateParameter( const String & name, EFieldType fieldType, EParameterType parameterType )
	{
		DatabaseConnectionSqliteSPtr connection = DoGetSqliteConnection();

		if ( !connection )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, ERROR_SQLITE_LOST_CONNECTION );
		}

		DatabaseParameterSPtr pReturn = std::make_shared< CDatabaseStatementParameterSqlite >( connection, name, ( unsigned short )_arrayInParams.size() + 1, fieldType, parameterType, std::make_unique< SValueUpdater >( this ) );

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

	DatabaseParameterSPtr CDatabaseStatementSqlite::CreateParameter( const String & name, EFieldType fieldType, uint32_t limits, EParameterType parameterType )
	{
		DatabaseConnectionSqliteSPtr connection = DoGetSqliteConnection();

		if ( !connection )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, ERROR_SQLITE_LOST_CONNECTION );
		}

		DatabaseParameterSPtr pReturn = std::make_shared< CDatabaseStatementParameterSqlite >( connection, name, ( unsigned short )_arrayInParams.size() + 1, fieldType, limits, parameterType, std::make_unique< SValueUpdater >( this ) );

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

	DatabaseParameterSPtr CDatabaseStatementSqlite::CreateParameter( const String & name, EFieldType fieldType, const std::pair< uint32_t, uint32_t > & precision, EParameterType parameterType )
	{
		DatabaseConnectionSqliteSPtr connection = DoGetSqliteConnection();

		if ( !connection )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, ERROR_SQLITE_LOST_CONNECTION );
		}

		DatabaseParameterSPtr pReturn = std::make_shared< CDatabaseStatementParameterSqlite >( connection, name, ( unsigned short )_arrayInParams.size() + 1, fieldType, precision, parameterType, std::make_unique< SValueUpdater >( this ) );

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

	EErrorType CDatabaseStatementSqlite::DoInitialize()
	{
		DatabaseConnectionSqliteSPtr connection = DoGetSqliteConnection();

		if ( !connection )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, ERROR_SQLITE_LOST_CONNECTION );
		}

		EErrorType eReturn = EErrorType_ERROR;

		if ( !_query.empty() )
		{
			_paramsCount = uint32_t( std::count( _query.begin(), _query.end(), STR( '?' ) ) );
			_arrayQueries = StringUtils::Split( _query, STR( "?" ), _paramsCount + 1 );
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
			DatabaseParameterSPtr parameter = ( *itParams );

			if ( parameter->GetParamType() == EParameterType_IN )
			{
				query << SQLITE_SQL_DELIM;
			}
			else if ( parameter->GetParamType() == EParameterType_INOUT )
			{
				query << SQLITE_SQL_PARAM + parameter->GetName();
				DatabaseStatementSPtr stmt = connection->CreateStatement( SQLITE_SQL_SET + parameter->GetName() + STR( " = " ) + SQLITE_SQL_DELIM );
				stmt->CreateParameter( parameter->GetName(), parameter->GetType(), parameter->GetLimits(), EParameterType_IN );
				stmt->Initialize();
				_inOutInitializers.push_back( std::make_pair( stmt, parameter ) );
				_arrayOutParams.push_back( parameter );
			}
			else if ( parameter->GetParamType() == EParameterType_OUT )
			{
				query << SQLITE_SQL_PARAM + parameter->GetName();
				DatabaseStatementSPtr stmt = connection->CreateStatement( SQLITE_SQL_SET + parameter->GetName() + SQLITE_SQL_NULL );
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

			_stmtOutParams = connection->CreateStatement( queryInOutParam.str() );
			_stmtOutParams->Initialize();
		}

		sqlite3 * sqlite = connection->GetConnection();
		std::string query2 = StringUtils::ToStr( _query );
		SQLiteCheck( sqlite3_prepare_v2( sqlite, query2.c_str(), int( query2.size() ), &_statement, NULL ), INFO_SQLITE_STATEMENT_PREPARATION, EDatabaseExceptionCodes_StatementError, sqlite );

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
				DB_EXCEPT( EDatabaseExceptionCodes_StatementError, error.str() );
			}
		}

		for ( DatabaseParameterPtrArray::iterator it = _arrayInParams.begin(); it != _arrayInParams.end(); ++it )
		{
			std::static_pointer_cast< CDatabaseStatementParameterSqlite >( *it )->SetStatement( _statement );
		}

		return eReturn;
	}

	bool CDatabaseStatementSqlite::DoExecuteUpdate()
	{
		DatabaseConnectionSqliteSPtr connection = DoGetSqliteConnection();

		if ( !connection )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, ERROR_SQLITE_LOST_CONNECTION );
		}

		DoPreExecute();
		bool bReturn = connection->ExecuteUpdate( _statement );

		if ( bReturn )
		{
			DoPostExecute();
		}

		return bReturn;
	}

	DatabaseResultSPtr CDatabaseStatementSqlite::DoExecuteSelect()
	{
		DatabaseConnectionSqliteSPtr connection = DoGetSqliteConnection();

		if ( !connection )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, ERROR_SQLITE_LOST_CONNECTION );
		}

		DoPreExecute();
		DatabaseResultSPtr pReturn = connection->ExecuteSelect( _statement );

		if ( pReturn )
		{
			DoPostExecute();
		}

		return pReturn;
	}

	void CDatabaseStatementSqlite::DoCleanup()
	{
		DatabaseConnectionSqliteSPtr connection = DoGetSqliteConnection();

		if ( !connection )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, ERROR_SQLITE_LOST_CONNECTION );
		}

		if ( _statement )
		{
			SQLiteCheck( sqlite3_finalize( _statement ), INFO_SQLITE_STATEMENT_FINALISATION, EDatabaseExceptionCodes_StatementError, connection->GetConnection() );
			_statement = NULL;
		}

		_arrayInParams.clear();
		_arrayOutParams.clear();
		_outInitializers.clear();
		_arrayQueries.clear();
		_paramsCount = 0;
		_stmtOutParams.reset();
	}

	void CDatabaseStatementSqlite::DoPreExecute()
	{
		for ( auto && it : _inOutInitializers )
		{
			DatabaseParameterSPtr parameter = it.second.lock();

			if ( parameter->GetObjectValue().IsNull() )
			{
				it.first->SetParameterNull( 0 );
			}
			else
			{
				it.first->SetParameterValue( 0, *parameter );
			}

			it.first->ExecuteUpdate();
		}

		for ( auto && statement : _outInitializers )
		{
			statement->ExecuteUpdate();
		}
	}

	void CDatabaseStatementSqlite::DoPostExecute()
	{
		if ( !_arrayOutParams.empty() )
		{
			DatabaseResultSPtr pReturn = _stmtOutParams->ExecuteSelect();

			if ( pReturn && pReturn->GetRowCount() )
			{
				DatabaseRowSPtr row = pReturn->GetFirstRow();

				for ( auto && parameter : _arrayOutParams )
				{
					if ( parameter->GetParamType() == EParameterType_INOUT || parameter->GetParamType() == EParameterType_OUT )
					{
						DatabaseFieldSPtr field;

						try
						{
							field = row->GetField( parameter->GetName() );
						}
						catch ( CDatabaseException & exc )
						{
							CLogger::LogError( exc.GetFullDescription() );
						}

						if ( field )
						{
							parameter->SetValue( *field );
						}
					}
				}
			}
		}

		DatabaseConnectionSqliteSPtr connection = DoGetSqliteConnection();
		SQLiteCheck( sqlite3_clear_bindings( _statement ), INFO_SQLITE_STATEMENT_CLEAR_BINDINGS, EDatabaseExceptionCodes_StatementError, connection->GetConnection() );
		SQLiteCheck( sqlite3_reset( _statement ), INFO_SQLITE_STATEMENT_RESET, EDatabaseExceptionCodes_StatementError, connection->GetConnection() );
	}
}
END_NAMESPACE_DATABASE_SQLITE
