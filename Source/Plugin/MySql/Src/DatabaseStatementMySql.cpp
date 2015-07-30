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
	static const String ERROR_MYSQL_LOST_CONNECTION = STR( "The statement has lost his connection" );

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

	DatabaseParameterSPtr CDatabaseStatementMySql::CreateParameter( const String & name, EFieldType fieldType, EParameterType parameterType )
	{
		DatabaseConnectionMySqlSPtr connection = DoGetMySqlConnection();

		if ( !connection )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, ERROR_MYSQL_LOST_CONNECTION );
		}

		DatabaseParameterMySqlSPtr pReturn = std::make_shared< CDatabaseParameterMySql >( connection, name, uint16_t( _arrayInParams.size() + 1 ), fieldType, parameterType, std::make_unique< SValueUpdater >( this ) );

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

	DatabaseParameterSPtr CDatabaseStatementMySql::CreateParameter( const String & name, EFieldType fieldType, uint32_t limits, EParameterType parameterType )
	{
		DatabaseConnectionMySqlSPtr connection = DoGetMySqlConnection();

		if ( !connection )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, ERROR_MYSQL_LOST_CONNECTION );
		}

		DatabaseParameterMySqlSPtr pReturn = std::make_shared< CDatabaseParameterMySql >( connection, name, uint16_t( _arrayInParams.size() + 1 ), fieldType, limits, parameterType, std::make_unique< SValueUpdater >( this ) );

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

	DatabaseParameterSPtr CDatabaseStatementMySql::CreateParameter( const String & name, EFieldType fieldType, const std::pair< uint32_t, uint32_t > & precision, EParameterType parameterType )
	{
		DatabaseConnectionMySqlSPtr connection = DoGetMySqlConnection();

		if ( !connection )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, ERROR_MYSQL_LOST_CONNECTION );
		}

		DatabaseParameterMySqlSPtr pReturn = std::make_shared< CDatabaseParameterMySql >( connection, name, uint16_t( _arrayInParams.size() + 1 ), fieldType, precision, parameterType, std::make_unique< SValueUpdater >( this ) );

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
			DatabaseParameterMySqlSPtr parameter = std::static_pointer_cast< CDatabaseParameterMySql >( *itParams );

			if ( parameter->GetParamType() == EParameterType_OUT )
			{
				query << MYSQL_SQL_PARAM + parameter->GetName();
				DatabaseStatementSPtr stmt = connection->CreateStatement( MYSQL_SQL_SET + parameter->GetName() + MYSQL_SQL_NULL );
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
				DatabaseStatementSPtr stmt = connection->CreateStatement( MYSQL_SQL_SET + parameter->GetName() + STR( " = " ) + MYSQL_SQL_DELIM );
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
				queryInOutParam << sep << MYSQL_SQL_PARAM << parameter.lock()->GetName() << MYSQL_SQL_AS << parameter.lock()->GetName();
				sep = MYSQL_SQL_COMMA;
			}

			_stmtOutParams = connection->CreateStatement( queryInOutParam.str() );
			_stmtOutParams->Initialize();
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

		std::string strQuery = StringUtils::ToStr( _query );
		MySQLCheck( mysql_stmt_prepare( _statement, strQuery.c_str(), uint32_t( strQuery.size() ) ), INFO_MYSQL_STATEMENT_PREPARATION, EDatabaseExceptionCodes_StatementError, connection->GetConnection() );
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
		DatabaseResultSPtr pReturn = connection->ExecuteSelect( _statement );

		if ( pReturn )
		{
			DoPostExecute();
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

	void CDatabaseStatementMySql::DoPreExecute()
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

		for ( auto && it : _outInitializers )
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

		MySQLCheck( mysql_stmt_reset( _statement ), INFO_MYSQL_STATEMENT_RESET, EDatabaseExceptionCodes_StatementError, DoGetMySqlConnection()->GetConnection() );
	}
}
END_NAMESPACE_DATABASE_MYSQL
