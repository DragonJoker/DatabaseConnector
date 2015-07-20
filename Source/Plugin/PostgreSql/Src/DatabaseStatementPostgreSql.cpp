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
	static const String ERROR_POSTGRESQL_MISSING_INITIALIZATION = STR( "Method Initialize must be called before calling method CreateParameter" );
	static const String ERROR_POSTGRESQL_CANT_CREATE_STATEMENT = STR( "Couldn't create the statement" );
	static const String ERROR_POSTGRESQL_EXECUTION_ERROR = STR( "Couldn't execute the statement : " );
	static const String ERROR_POSTGRESQL_QUERY_INCONSISTENCY_ERROR = STR( "Number of parameters doesn't match the sizes of parameter containers." );
	static const String INFO_POSTGRESQL_PREPARING_STATEMENT = STR( "Preparing statement for query: " );
	static const String INFO_POSTGRESQL_BIND_PARAMETER_NAME = STR( "BindParameter : " );
	static const String INFO_POSTGRESQL_BIND_PARAMETER_VALUE = STR( ", Value : " );

	static const TChar * INFO_POSTGRESQL_STATEMENT_PREPARATION = STR( "Statement preparation" );
	static const TChar * INFO_POSTGRESQL_STATEMENT_DESCRIBE = STR( "Statement describe" );
	static const TChar * INFO_POSTGRESQL_STATEMENT_EXECUTION = STR( "Statement execution" );

	static const String POSTGRESQL_SQL_DELIM = STR( "?" );
	static const String POSTGRESQL_SQL_PARAM = STR( "@" );
	static const String POSTGRESQL_SQL_SET = STR( "SET @" );
	static const String POSTGRESQL_SQL_NULL = STR( " = NULL;" );
	static const String POSTGRESQL_SQL_SELECT = STR( "SELECT " );
	static const String POSTGRESQL_SQL_AS = STR( " AS " );
	static const String POSTGRESQL_SQL_COMMA = STR( "," );

	CDatabaseStatementPostgreSql::CDatabaseStatementPostgreSql( DatabaseConnectionPostgreSqlPtr connection, const String & query )
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

	DatabaseParameterPtr CDatabaseStatementPostgreSql::CreateParameter( const String & name, EFieldType fieldType, EParameterType parameterType )
	{
		DatabaseParameterPostgreSqlPtr pReturn = std::make_shared< CDatabaseParameterPostgreSql >( _connectionPostgreSql, name, uint16_t( _arrayParams.size() + 1 ), fieldType, parameterType, std::make_unique< SValueUpdater >( this ) );

		if ( !DoAddParameter( pReturn ) )
		{
			pReturn.reset();
		}

		return pReturn;
	}

	DatabaseParameterPtr CDatabaseStatementPostgreSql::CreateParameter( const String & name, EFieldType fieldType, uint32_t limits, EParameterType parameterType )
	{
		DatabaseParameterPostgreSqlPtr pReturn = std::make_shared< CDatabaseParameterPostgreSql >( _connectionPostgreSql, name, uint16_t( _arrayParams.size() + 1 ), fieldType, limits, parameterType, std::make_unique< SValueUpdater >( this ) );

		if ( !DoAddParameter( pReturn ) )
		{
			pReturn.reset();
		}

		return pReturn;
	}

	DatabaseParameterPtr CDatabaseStatementPostgreSql::CreateParameter( const String & name, EFieldType fieldType, const std::pair< uint32_t, uint32_t > & precision, EParameterType parameterType )
	{
		DatabaseParameterPostgreSqlPtr pReturn = std::make_shared< CDatabaseParameterPostgreSql >( _connectionPostgreSql, name, uint16_t( _arrayParams.size() + 1 ), fieldType, precision, parameterType, std::make_unique< SValueUpdater >( this ) );

		if ( !DoAddParameter( pReturn ) )
		{
			pReturn.reset();
		}

		return pReturn;
	}

	EErrorType CDatabaseStatementPostgreSql::DoInitialize()
	{
		EErrorType eReturn = EErrorType_ERROR;

		if ( !_query.empty() )
		{
			_paramsCount = uint32_t( std::count( _query.begin(), _query.end(), STR( '?' ) ) );
			_arrayQueries = CStrUtils::Split( _query, STR( "?" ), _paramsCount + 1 );
		}

		CLogger::LogInfo( INFO_POSTGRESQL_PREPARING_STATEMENT + _query );
		assert( _paramsCount == _arrayParams.size() );

		StringStream query;
		unsigned short i = 0;
		auto && itQueries = _arrayQueries.begin();
		auto && itParams = _arrayParams.begin();

		_arrayOutParams.clear();
		std::vector< DatabaseParameterPostgreSqlPtr > arrayParams;

		arrayParams.reserve( _arrayParams.size() );
		_arrayOutParams.reserve( _arrayParams.size() );
		_bindings.reserve( _arrayParams.size() );
		_arrayValues.resize( _arrayParams.size() );
		_arrayLengths.resize( _arrayParams.size() );
		_arrayFormats.resize( _arrayParams.size() );
		_arrayOids.reserve( _arrayParams.size() );

		while ( itQueries != _arrayQueries.end() && itParams != _arrayParams.end() )
		{
			query << ( *itQueries );
			DatabaseParameterPostgreSqlPtr parameter = std::static_pointer_cast< CDatabaseParameterPostgreSql >( *itParams );
			arrayParams.push_back( parameter );
			PGbind bind = { _arrayValues[i], _arrayLengths[i], _arrayFormats[i] };
			_bindings.push_back( bind );
			_arrayOids.push_back( GetOidFromFieldType( parameter->GetType() ) );
			query << POSTGRESQL_SQL_DELIM;

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

		std::string strQuery = CStrUtils::ToStr( _query );
		PGresult * result = PQprepare( _connectionPostgreSql->GetConnection(), _statement.c_str(), strQuery.c_str(), int( _arrayParams.size() ), _arrayOids.data() );
		PostgreSQLCheck( result, INFO_POSTGRESQL_STATEMENT_PREPARATION, EDatabaseExceptionCodes_StatementError, _connectionPostgreSql->GetConnection() );
		PQclear( result );

		result = PQdescribePrepared( _connectionPostgreSql->GetConnection(), _statement.c_str() );
		PostgreSQLCheck( result, INFO_POSTGRESQL_STATEMENT_DESCRIBE, EDatabaseExceptionCodes_StatementError, _connectionPostgreSql->GetConnection() );
		int count = PQnparams ( result );
		size_t index = 0;

		for ( auto && parameter : arrayParams )
		{
			parameter->SetStatement( _statement );
			parameter->SetBinding( &_bindings[index++] );
		}

		return eReturn;
	}

	bool CDatabaseStatementPostgreSql::DoExecuteUpdate( EErrorType * result )
	{
		EErrorType eResult = EErrorType_NONE;
		PGresult * pgresult = PQexecPrepared( _connectionPostgreSql->GetConnection(), _statement.c_str(), int( _arrayParams.size() ), _arrayValues.data(), _arrayLengths.data(), _arrayFormats.data(), 0 );
		PostgreSQLCheck( pgresult, INFO_POSTGRESQL_STATEMENT_EXECUTION, EDatabaseExceptionCodes_StatementError, _connectionPostgreSql->GetConnection() );

		if ( result )
		{
			*result = eResult;
		}

		return true;
	}

	DatabaseResultPtr CDatabaseStatementPostgreSql::DoExecuteSelect( EErrorType * result )
	{
		DatabaseResultPtr pReturn;
		EErrorType eResult = EErrorType_NONE;
		PGresult * pgresult = PQexecPrepared( _connectionPostgreSql->GetConnection(), _statement.c_str(), int( _arrayParams.size() ), _arrayValues.data(), _arrayLengths.data(), _arrayFormats.data(), 0 );
		PostgreSQLCheck( pgresult, INFO_POSTGRESQL_STATEMENT_EXECUTION, EDatabaseExceptionCodes_StatementError, _connectionPostgreSql->GetConnection() );

		std::vector< std::unique_ptr< SInPostgreSqlBindBase > > binds;
		pReturn = PostgreSqlFetchResult( pgresult, PostgreSqlGetColumns( pgresult, _connectionPostgreSql, binds ), _connectionPostgreSql, binds );

		if ( result )
		{
			*result = eResult;
		}

		return pReturn;
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
