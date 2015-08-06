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
	static const String ERROR_MYSQL_MISSING_INITIALIZATION = STR( "Method Initialise must be called before calling method CreateParameter" );
	static const String ERROR_MYSQL_CANT_CREATE_STATEMENT = STR( "Couldn't create the statement" );
	static const String ERROR_MYSQL_LOST_CONNECTION = STR( "The statement has lost his connection" );
	static const String ERROR_FIELD_RETRIEVAL = STR( "Field retrieval error" );

	static const String INFO_MYSQL_BIND_PARAMETER_NAME = STR( "BindParameter : " );
	static const String INFO_MYSQL_BIND_PARAMETER_VALUE = STR( ", Value : " );

	static const String DEBUG_MYSQL_PREPARING_STATEMENT = STR( "Preparing statement 0x%08X" );

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

	CDatabaseStatementMySql::CDatabaseStatementMySql( DatabaseConnectionMySqlSPtr connection, const String & query )
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

	DatabaseParameterSPtr CDatabaseStatementMySql::DoCreateParameter( DatabaseValuedObjectInfosSPtr infos, EParameterType parameterType )
	{
		DatabaseConnectionMySqlSPtr connection = DoGetMySqlConnection();

		if ( !connection )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, ERROR_MYSQL_LOST_CONNECTION );
		}

		DatabaseParameterMySqlSPtr parameter = std::make_shared< CDatabaseParameterMySql >( connection, infos, uint16_t( _arrayInParams.size() + 1 ), parameterType, std::make_unique< SValueUpdater >( this ) );
		DatabaseParameterSPtr ret = DoAddParameter( parameter );

		if ( ret && parameterType == EParameterType_IN )
		{
			_arrayInParams.push_back( parameter );
		}

		return ret;
	}

	EErrorType CDatabaseStatementMySql::DoInitialise()
	{
		DatabaseConnectionMySqlSPtr connection = DoGetMySqlConnection();

		if ( !connection )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, ERROR_MYSQL_LOST_CONNECTION );
		}

		EErrorType eReturn = EErrorType_ERROR;

		if ( !_query.empty() )
		{
			_paramsCount = uint32_t( std::count( _query.begin(), _query.end(), STR( '?' ) ) );
			_arrayQueries = StringUtils::Split( _query, STR( "?" ), _paramsCount + 1 );
		}

		CLogger::LogDebug( ( Format( DEBUG_MYSQL_PREPARING_STATEMENT ) % this ).str() );
		assert( _paramsCount == GetParametersCount() );

		StringStream query;
		unsigned short i = 0;
		auto && itQueries = _arrayQueries.begin();
		auto && itParams = DoGetParameters().begin();
		auto && itParamsEnd = DoGetParameters().end();

		_outInitialisers.clear();
		_arrayOutParams.clear();

		_outInitialisers.reserve( GetParametersCount() );
		_arrayOutParams.reserve( GetParametersCount() );
		_bindings.reserve( GetParametersCount() );

		while ( itQueries != _arrayQueries.end() && itParams != itParamsEnd )
		{
			query << ( *itQueries );
			DatabaseParameterMySqlSPtr parameter = std::static_pointer_cast< CDatabaseParameterMySql >( *itParams );

			if ( parameter->GetParamType() == EParameterType_OUT )
			{
				query << MYSQL_SQL_PARAM + parameter->GetName();
				DatabaseStatementSPtr stmt = connection->CreateStatement( MYSQL_SQL_SET + parameter->GetName() + MYSQL_SQL_NULL );
				stmt->Initialise();
				_outInitialisers.push_back( stmt );
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
				DatabaseStatementSPtr stmt = connection->CreateStatement( MYSQL_SQL_SET + parameter->GetName() + STR( " = " ) + MYSQL_SQL_DELIM );
				stmt->CreateParameter( parameter->GetName(), parameter->GetType(), parameter->GetLimits(), EParameterType_IN );
				stmt->Initialise();
				_inOutInitialisers.push_back( std::make_pair( stmt, parameter ) );
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
				queryInOutParam << sep << MYSQL_SQL_PARAM << parameter.lock()->GetName() << MYSQL_SQL_AS << parameter.lock()->GetName();
				sep = MYSQL_SQL_COMMA;
			}

			_stmtOutParams = connection->CreateStatement( queryInOutParam.str() );
			_stmtOutParams->Initialise();
		}

		_statement = mysql_stmt_init( connection->GetConnection() );

		if ( _statement )
		{
			eReturn = EErrorType_NONE;
		}
		else
		{
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, ERROR_MYSQL_CANT_CREATE_STATEMENT );
		}

		MySQLCheck( mysql_stmt_prepare( _statement, _query.c_str(), uint32_t( _query.size() ) ), INFO_MYSQL_STATEMENT_PREPARATION, EDatabaseExceptionCodes_StatementError, connection->GetConnection() );
		MYSQL_RES * meta = mysql_stmt_param_metadata( _statement );
		size_t index = 0;

		for ( auto && it : _arrayInParams )
		{
			DatabaseParameterMySqlSPtr parameter = it.lock();
			parameter->SetStatement( _statement );
			parameter->SetBinding( &_bindings[index++] );
		}

		MySQLCheck( mysql_stmt_bind_param( _statement, _bindings.data() ), INFO_MYSQL_STATEMENT_PARAMS_BINDING, EDatabaseExceptionCodes_StatementError, connection->GetConnection() );
		return eReturn;
	}

	bool CDatabaseStatementMySql::DoExecuteUpdate()
	{
		DatabaseConnectionMySqlSPtr connection = DoGetMySqlConnection();

		if ( !connection )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, ERROR_MYSQL_LOST_CONNECTION );
		}

		DoPreExecute();
		bool bReturn = connection->ExecuteUpdate( _statement );

		if ( bReturn )
		{
			DoPostExecute();
		}

		return bReturn;
	}

	DatabaseResultSPtr CDatabaseStatementMySql::DoExecuteSelect()
	{
		DatabaseConnectionMySqlSPtr connection = DoGetMySqlConnection();

		if ( !connection )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, ERROR_MYSQL_LOST_CONNECTION );
		}

		DoPreExecute();
		DatabaseResultSPtr pReturn = connection->ExecuteSelect( _statement, _infos );

		if ( pReturn )
		{
			DoPostExecute();
		}

		return pReturn;
	}

	void CDatabaseStatementMySql::DoCleanup()
	{
		_bindings.clear();
		_arrayInParams.clear();
		_arrayOutParams.clear();
		_outInitialisers.clear();
		_arrayQueries.clear();
		_paramsCount = 0;
		_stmtOutParams.reset();

		if ( _statement )
		{
			mysql_stmt_close( _statement );
			_statement = NULL;
		}
	}

	void CDatabaseStatementMySql::DoPreExecute()
	{
		for ( auto && it : _inOutInitialisers )
		{
			DatabaseParameterSPtr parameter = it.second.lock();

			if ( parameter->GetObjectValue().IsNull() )
			{
				it.first->SetParameterNull( 0 );
			}
			else
			{
				it.first->SetParameterValue( 0, static_cast< const CDatabaseValuedObject & >( *parameter ) );
			}

			it.first->ExecuteUpdate();
		}

		for ( auto && it : _outInitialisers )
		{
			it->ExecuteUpdate();
		}
	}

	void CDatabaseStatementMySql::DoPostExecute()
	{
		if ( !_arrayOutParams.empty() )
		{
			DatabaseResultSPtr pReturn = _stmtOutParams->ExecuteSelect();

			if ( pReturn && pReturn->GetRowCount() )
			{
				DatabaseRowSPtr row = pReturn->GetFirstRow();

				for ( auto && wparameter : _arrayOutParams )
				{
					DatabaseParameterSPtr parameter = wparameter.lock();

					if ( parameter->GetParamType() == EParameterType_INOUT || parameter->GetParamType() == EParameterType_OUT )
					{
						DatabaseFieldSPtr field;

						try
						{
							field = row->GetField( parameter->GetName() );
						}
						COMMON_CATCH( ERROR_FIELD_RETRIEVAL )

						if ( field )
						{
							parameter->SetValue( static_cast< const CDatabaseValuedObject & >( *field ) );
						}
					}
				}
			}
		}

		MySQLCheck( mysql_stmt_reset( _statement ), INFO_MYSQL_STATEMENT_RESET, EDatabaseExceptionCodes_StatementError, DoGetMySqlConnection()->GetConnection() );
	}
}
END_NAMESPACE_DATABASE_MYSQL
