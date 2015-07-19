/************************************************************************//**
* @file DatabaseStatementMySql.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDatabaseStatementMySql class definition.
*
* @details Describes a statement for MYSQL database.
*
***************************************************************************/

#include "DatabaseMySqlPch.h"

#include "DatabaseStatementMySql.h"

#include "DatabaseConnectionMySql.h"
#include "DatabaseMySql.h"
#include "DatabaseParameterMySql.h"
#include "ExceptionDatabaseMySql.h"

#include <DatabaseStringUtils.h>

#include <DatabaseRow.h>

#include <mysql.h>

BEGIN_NAMESPACE_DATABASE_MYSQL
{
	static const String ERROR_MYSQL_MISSING_INITIALIZATION = STR( "Method Initialize must be called before calling method CreateParameter" );
	static const String ERROR_MYSQL_CANT_CREATE_STATEMENT = STR( "Couldn't create the statement" );
	static const String ERROR_MYSQL_EXECUTION_ERROR = STR( "Couldn't execute the statement : " );
	static const String ERROR_MYSQL_QUERY_INCONSISTENCY_ERROR = STR( "Number of parameters doesn't match the sizes of parameter containers." );
	static const String INFO_MYSQL_PREPARING_STATEMENT = STR( "Preparing statement for query: " );
	static const String INFO_MYSQL_BIND_PARAMETER_NAME = STR( "BindParameter : " );
	static const String INFO_MYSQL_BIND_PARAMETER_VALUE = STR( ", Value : " );

	static const TChar * INFO_MYSQL_STATEMENT_PREPARATION = STR( "Statement preparation" );
	static const TChar * INFO_MYSQL_STATEMENT_PARAMS_BINDING = STR( "Statement parameters binding" );
	static const TChar * INFO_MYSQL_STATEMENT_RESET = STR( "Statement reset" );

	static const String MYSQL_SQL_DELIM = STR( "?" );
	static const String MYSQL_SQL_PARAM = STR( "@" );
	static const String MYSQL_SQL_SET = STR( "SET @" );
	static const String MYSQL_SQL_NULL = STR( " = NULL;" );
	static const String MYSQL_SQL_SELECT = STR( "SELECT " );
	static const String MYSQL_SQL_AS = STR( " AS " );
	static const String MYSQL_SQL_COMMA = STR( "," );

	CDatabaseStatementMySql::CDatabaseStatementMySql( DatabaseConnectionMySqlPtr connection, const String & query )
		: CDatabaseStatement( connection, query )
		, _statement( NULL )
		, _connectionMySql( connection )
		, _paramsCount( 0 )
	{
	}

	CDatabaseStatementMySql::~CDatabaseStatementMySql()
	{
		Cleanup();
	}

	DatabaseParameterPtr CDatabaseStatementMySql::CreateParameter( const String & name, EFieldType fieldType, EParameterType parameterType )
	{
		DatabaseParameterMySqlPtr pReturn = std::make_shared< CDatabaseParameterMySql >( _connectionMySql, name, uint16_t( _arrayInParams.size() + 1 ), fieldType, parameterType, std::make_unique< SValueUpdater >( this ) );

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

	DatabaseParameterPtr CDatabaseStatementMySql::CreateParameter( const String & name, EFieldType fieldType, uint32_t limits, EParameterType parameterType )
	{
		DatabaseParameterMySqlPtr pReturn = std::make_shared< CDatabaseParameterMySql >( _connectionMySql, name, uint16_t( _arrayInParams.size() + 1 ), fieldType, limits, parameterType, std::make_unique< SValueUpdater >( this ) );

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

	DatabaseParameterPtr CDatabaseStatementMySql::CreateParameter( const String & name, EFieldType fieldType, const std::pair< uint32_t, uint32_t > & precision, EParameterType parameterType )
	{
		DatabaseParameterMySqlPtr pReturn = std::make_shared< CDatabaseParameterMySql >( _connectionMySql, name, uint16_t( _arrayInParams.size() + 1 ), fieldType, precision, parameterType, std::make_unique< SValueUpdater >( this ) );

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

	EErrorType CDatabaseStatementMySql::DoInitialize()
	{
		EErrorType eReturn = EErrorType_ERROR;

		if ( !_query.empty() )
		{
			_paramsCount = uint32_t( std::count( _query.begin(), _query.end(), STR( '?' ) ) );
			_arrayQueries = CStrUtils::Split( _query, STR( "?" ), _paramsCount + 1 );
		}

		CLogger::LogInfo( INFO_MYSQL_PREPARING_STATEMENT + _query );
		assert( _paramsCount == _arrayParams.size() );

		StringStream query;
		unsigned short i = 0;
		auto && itQueries = _arrayQueries.begin();
		auto && itParams = _arrayParams.begin();

		_outInitializers.clear();
		_arrayOutParams.clear();

		_outInitializers.reserve( _arrayParams.size() );
		_arrayOutParams.reserve( _arrayParams.size() );
		_bindings.reserve( _arrayParams.size() );

		while ( itQueries != _arrayQueries.end() && itParams != _arrayParams.end() )
		{
			query << ( *itQueries );
			DatabaseParameterMySqlPtr parameter = std::static_pointer_cast< CDatabaseParameterMySql >( *itParams );

			if ( parameter->GetParamType() == EParameterType_OUT )
			{
				query << MYSQL_SQL_PARAM + parameter->GetName();
				DatabaseStatementPtr stmt = _connection->CreateStatement( MYSQL_SQL_SET + parameter->GetName() + MYSQL_SQL_NULL );
				stmt->Initialize();
				_outInitializers.push_back( stmt );
				_arrayOutParams.push_back( parameter );
			}
			else if ( parameter->GetParamType() == EParameterType_IN )
			{
				MYSQL_BIND bind = { 0 };
				_bindings.push_back( bind );
				query << MYSQL_SQL_DELIM;
			}
			else
			{
				query << MYSQL_SQL_PARAM + parameter->GetName();
				DatabaseStatementPtr stmt = _connection->CreateStatement( MYSQL_SQL_SET + parameter->GetName() + STR( " = " ) + MYSQL_SQL_DELIM );
				stmt->CreateParameter( parameter->GetName(), parameter->GetType(), parameter->GetLimits(), EParameterType_IN );
				stmt->Initialize();
				_inOutInitializers.push_back( std::make_pair( stmt, parameter ) );
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
			queryInOutParam << MYSQL_SQL_SELECT;

			for ( auto && parameter : _arrayOutParams )
			{
				queryInOutParam << sep << MYSQL_SQL_PARAM << parameter->GetName() << MYSQL_SQL_AS << parameter->GetName();
				sep = MYSQL_SQL_COMMA;
			}

			_stmtOutParams = _connection->CreateStatement( queryInOutParam.str() );
			_stmtOutParams->Initialize();
		}

		_statement = mysql_stmt_init( _connectionMySql->GetConnection() );

		if ( _statement )
		{
			eReturn = EErrorType_NONE;
		}
		else
		{
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, ERROR_MYSQL_CANT_CREATE_STATEMENT );
		}

		std::string strQuery = CStrUtils::ToStr( _query );
		MySQLCheck( mysql_stmt_prepare( _statement, strQuery.c_str(), uint32_t( strQuery.size() ) ), INFO_MYSQL_STATEMENT_PREPARATION, EDatabaseExceptionCodes_StatementError, _connectionMySql->GetConnection() );
		MYSQL_RES * meta = mysql_stmt_param_metadata( _statement );
		size_t index = 0;

		for ( auto && it : _arrayInParams )
		{
			it->SetStatement( _statement );
			it->SetBinding( &_bindings[index++] );
		}

		MySQLCheck( mysql_stmt_bind_param( _statement, _bindings.data() ), INFO_MYSQL_STATEMENT_PARAMS_BINDING, EDatabaseExceptionCodes_StatementError, _connectionMySql->GetConnection() );
		return eReturn;
	}

	bool CDatabaseStatementMySql::DoExecuteUpdate( EErrorType * result )
	{
		DoPreExecute( result );

		bool bReturn;
		EErrorType eResult = EErrorType_NONE;
		bReturn = _connectionMySql->ExecuteUpdate( _statement );
		DoPostExecute( result );

		if ( result )
		{
			*result = eResult;
		}

		return bReturn;
	}

	DatabaseResultPtr CDatabaseStatementMySql::DoExecuteSelect( EErrorType * result )
	{
		DoPreExecute( result );
		EErrorType eResult = EErrorType_NONE;
		DatabaseResultPtr pReturn = _connectionMySql->ExecuteSelect( _statement );
		DoPostExecute( result );

		if ( result )
		{
			*result = eResult;
		}

		return pReturn;
	}

	void CDatabaseStatementMySql::DoCleanup()
	{
		if ( _statement )
		{
			mysql_stmt_close( _statement );
			_statement = NULL;
		}

		_arrayInParams.clear();
		_arrayOutParams.clear();
		_outInitializers.clear();
		_arrayQueries.clear();
		_paramsCount = 0;
		_stmtOutParams.reset();
	}

	void CDatabaseStatementMySql::DoPreExecute( EErrorType * result )
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

		for ( auto && it : _outInitializers )
		{
			it->ExecuteUpdate( result );
		}
	}

	void CDatabaseStatementMySql::DoPostExecute( EErrorType * result )
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

		MySQLCheck( mysql_stmt_reset( _statement ), INFO_MYSQL_STATEMENT_RESET, EDatabaseExceptionCodes_StatementError, _connectionMySql->GetConnection() );
	}
}
END_NAMESPACE_DATABASE_MYSQL
