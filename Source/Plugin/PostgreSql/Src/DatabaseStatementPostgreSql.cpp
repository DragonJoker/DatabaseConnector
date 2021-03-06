/************************************************************************//**
* @file DatabaseStatementPostgreSql.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDatabaseStatementPostgreSql class definition.
*
* @details Describes a statement for MYSQL database.
*
***************************************************************************/

#include "DatabasePostgreSqlPch.h"

#include "DatabaseStatementPostgreSql.h"

#include "DatabaseConnectionPostgreSql.h"
#include "DatabasePostgreSql.h"
#include "DatabaseParameterPostgreSql.h"
#include "ExceptionDatabasePostgreSql.h"

#include <DatabaseStringUtils.h>

#include <DatabaseRow.h>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <libpq-fe.h>

BEGIN_NAMESPACE_DATABASE_POSTGRESQL
{
	static const String ERROR_POSTGRESQL_MISSING_INITIALIZATION = STR( "Method Initialise must be called before calling method CreateParameter" );
	static const String ERROR_POSTGRESQL_CANT_CREATE_STATEMENT = STR( "Couldn't create the statement" );
	static const String ERROR_POSTGRESQL_LOST_CONNECTION = STR( "The statement has lost his connection" );

	static const String INFO_POSTGRESQL_BIND_PARAMETER_NAME = STR( "BindParameter : " );
	static const String INFO_POSTGRESQL_BIND_PARAMETER_VALUE = STR( ", Value : " );

	static const TChar * INFO_POSTGRESQL_STATEMENT_PREPARATION = STR( "Statement preparation" );
	static const TChar * INFO_POSTGRESQL_STATEMENT_DESCRIBE = STR( "Statement describe" );
	static const TChar * INFO_POSTGRESQL_STATEMENT_EXECUTION = STR( "Statement execution" );

	static const String DEBUG_POSTGRESQL_PREPARING_STATEMENT = STR( "Preparing statement 0x%08X" );

	static const String POSTGRESQL_SQL_DELIM = STR( "$" );
	static const String POSTGRESQL_SQL_PARAM = STR( "@" );
	static const String POSTGRESQL_SQL_SET = STR( "SET @" );
	static const String POSTGRESQL_SQL_NULL = STR( " = NULL;" );
	static const String POSTGRESQL_SQL_SELECT = STR( "SELECT " );
	static const String POSTGRESQL_SQL_AS = STR( " AS " );
	static const String POSTGRESQL_SQL_COMMA = STR( "," );

	CDatabaseStatementPostgreSql::CDatabaseStatementPostgreSql( DatabaseConnectionPostgreSqlSPtr connection, const String & query )
		: CDatabaseStatement( connection, query )
		, _statement( boost::uuids::to_string( boost::uuids::random_generator()() ) )
		, _connectionPostgreSql( connection )
		, _paramsCount( 0 )
	{
	}

	CDatabaseStatementPostgreSql::~CDatabaseStatementPostgreSql()
	{
		Cleanup();
	}

	DatabaseParameterSPtr CDatabaseStatementPostgreSql::DoCreateParameter( DatabaseValuedObjectInfosSPtr infos, EParameterType parameterType )
	{
		DatabaseConnectionPostgreSqlSPtr connection = DoGetPostgreSqlConnection();

		if ( !connection )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, ERROR_POSTGRESQL_LOST_CONNECTION );
		}

		return DoAddParameter( std::make_shared< CDatabaseParameterPostgreSql >( connection, infos, GetParametersCount() + 1, parameterType, std::make_unique< SValueUpdater >( this ) ) );
	}

	EErrorType CDatabaseStatementPostgreSql::DoInitialise()
	{
		DatabaseConnectionPostgreSqlSPtr connection = DoGetPostgreSqlConnection();

		if ( !connection )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, ERROR_POSTGRESQL_LOST_CONNECTION );
		}

		EErrorType eReturn = EErrorType_ERROR;

		if ( !_query.empty() )
		{
			_paramsCount = uint32_t( std::count( _query.begin(), _query.end(), STR( '?' ) ) );
			_arrayQueries = StringUtils::Split( _query, STR( "?" ), _paramsCount + 1 );
		}

		CLogger::LogDebug( ( Format( DEBUG_POSTGRESQL_PREPARING_STATEMENT ) % this ).str() );
		assert( _paramsCount == GetParametersCount() );

		StringStream query;
		unsigned short i = 0;
		auto && itQueries = _arrayQueries.begin();
		auto && itParams = DoGetParameters().begin();
		auto && itParamsEnd = DoGetParameters().end();

		_arrayOutParams.clear();
		std::vector< DatabaseParameterPostgreSqlSPtr > arrayParams;

		arrayParams.reserve( GetParametersCount() );
		_arrayOutParams.reserve( GetParametersCount() );
		_bindings.reserve( GetParametersCount() );
		_arrayValues.resize( GetParametersCount() );
		_arrayLengths.resize( GetParametersCount() );
		_arrayFormats.resize( GetParametersCount() );
		_arrayOids.reserve( GetParametersCount() );
		size_t index = 1;

		while ( itQueries != _arrayQueries.end() && itParams != itParamsEnd )
		{
			query << ( *itQueries );
			DatabaseParameterPostgreSqlSPtr parameter = std::static_pointer_cast< CDatabaseParameterPostgreSql >( *itParams );
			arrayParams.push_back( parameter );
			PGbind bind = { _arrayValues[i], _arrayLengths[i], _arrayFormats[i] };
			_bindings.push_back( bind );
			_arrayOids.push_back( GetOidFromFieldType( parameter->GetType() ) );
			query << POSTGRESQL_SQL_DELIM << index++;

			if ( parameter->GetParamType() != EParameterType_IN )
			{
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

		PGresult * result = PQprepare( connection->GetConnection(), _statement.c_str(), _query.c_str(), int( GetParametersCount() ), _arrayOids.data() );
		PostgreSQLCheck( result, INFO_POSTGRESQL_STATEMENT_PREPARATION, EDatabaseExceptionCodes_StatementError, connection->GetConnection() );
		PQclear( result );

		result = PQdescribePrepared( connection->GetConnection(), _statement.c_str() );
		PostgreSQLCheck( result, INFO_POSTGRESQL_STATEMENT_DESCRIBE, EDatabaseExceptionCodes_StatementError, connection->GetConnection() );
		int count = PQnparams( result );
		index = 0;

		for ( auto && parameter : arrayParams )
		{
			parameter->SetStatement( _statement );
			parameter->SetBinding( &_bindings[index++] );
		}

		eReturn = EErrorType_NONE;
		return eReturn;
	}

	bool CDatabaseStatementPostgreSql::DoExecuteUpdate()
	{
		DatabaseConnectionPostgreSqlSPtr connection = DoGetPostgreSqlConnection();

		if ( !connection )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, ERROR_POSTGRESQL_LOST_CONNECTION );
		}

		EErrorType eResult = EErrorType_NONE;
		PGresult * pgresult = PQexecPrepared( connection->GetConnection(), _statement.c_str(), int( GetParametersCount() ), _arrayValues.data(), _arrayLengths.data(), _arrayFormats.data(), 0 );
		PostgreSQLCheck( pgresult, INFO_POSTGRESQL_STATEMENT_EXECUTION, EDatabaseExceptionCodes_StatementError, connection->GetConnection() );
		return true;
	}

	DatabaseResultSPtr CDatabaseStatementPostgreSql::DoExecuteSelect()
	{
		DatabaseConnectionPostgreSqlSPtr connection = DoGetPostgreSqlConnection();

		if ( !connection )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, ERROR_POSTGRESQL_LOST_CONNECTION );
		}

		DatabaseResultSPtr pReturn;
		EErrorType eResult = EErrorType_NONE;
		PGresult * pgresult = PQexecPrepared( connection->GetConnection(), _statement.c_str(), int( GetParametersCount() ), _arrayValues.data(), _arrayLengths.data(), _arrayFormats.data(), 0 );
		PostgreSQLCheck( pgresult, INFO_POSTGRESQL_STATEMENT_EXECUTION, EDatabaseExceptionCodes_StatementError, connection->GetConnection() );

		if ( _infos.empty() )
		{
			_infos = PostgreSqlGetColumns( pgresult );
		}

		return PostgreSqlFetchResult( pgresult, _infos, connection );
	}

	void CDatabaseStatementPostgreSql::DoCleanup()
	{
		_arrayValues.clear();
		_arrayLengths.clear();
		_arrayFormats.clear();
		_arrayOids.clear();
		_arrayOutParams.clear();
		_arrayQueries.clear();
		_paramsCount = 0;
	}
}
END_NAMESPACE_DATABASE_POSTGRESQL
