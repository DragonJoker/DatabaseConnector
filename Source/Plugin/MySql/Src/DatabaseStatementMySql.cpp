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
#include "DatabaseStatementParameterMySql.h"
#include "ExceptionDatabaseMySql.h"

#include <DatabaseStringUtils.h>

#include <DatabaseRow.h>

#include <mysql.h>

BEGIN_NAMESPACE_DATABASE_MYSQL
{
	static const String MYSQL_MISSING_INITIALIZATION_ERROR = STR( "Method Initialize must be called before calling method CreateParameter" );
	static const String ERROR_MYSQL_CANT_CREATE_STATEMENT = STR( "Couldn't create the statement" );
	static const String ERROR_MYSQL_EXECUTION_ERROR = STR( "Couldn't execute the statement : " );
	static const String ERROR_MYSQL_QUERY_INCONSISTENCY_ERROR = STR( "Number of parameters doesn't match the sizes of parameter containers." );

	static const String MYSQL_BIND_PARAMETER_NAME_MSG = STR( "BindParameter : " );
	static const String MYSQL_BIND_PARAMETER_VALUE_MSG = STR( ", Value : " );

	static const String SQL_DELIM = STR( "?" );
	static const String SQL_PARAM = STR( "@" );
	static const String SQL_SET = STR( "SET @" );
	static const String SQL_NULL = STR( " = NULL;" );
	static const String SQL_SELECT = STR( "SELECT " );
	static const String SQL_AS = STR( " AS " );
	static const String SQL_COMMA = STR( "," );

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

	EErrorType CDatabaseStatementMySql::Initialize()
	{
		EErrorType eReturn = EErrorType_ERROR;

		if ( !_query.empty() )
		{
			_paramsCount = uint32_t( std::count( _query.begin(), _query.end(), STR( '?' ) ) );
			_arrayQueries = CStrUtils::Split( _query, STR( "?" ), _paramsCount + 1 );
		}

		CLogger::LogMessage( STR( "Preparing statement for query : " ) + _query );
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
			DatabaseStatementParameterMySqlPtr parameter = std::static_pointer_cast< CDatabaseStatementParameterMySql >( *itParams );

			if ( parameter->GetParamType() == EParameterType_OUT )
			{
				query << SQL_PARAM + parameter->GetName();
				DatabaseStatementPtr stmt = _connection->CreateStatement( SQL_SET + parameter->GetName() + SQL_NULL );
				stmt->Initialize();
				_outInitializers.push_back( stmt );
				_arrayOutParams.push_back( parameter );
			}
			else
			{
				if ( parameter->GetParamType() == EParameterType_IN )
				{
					MYSQL_BIND bind = { 0 };
					_bindings.push_back( bind );
					query << SQL_DELIM;
				}
				else
				{
					query << SQL_PARAM + parameter->GetName();
					DatabaseStatementPtr stmt = _connection->CreateStatement( SQL_SET + parameter->GetName() + STR( " = " ) + SQL_DELIM );
					stmt->CreateParameter( parameter->GetName(), parameter->GetType(), parameter->GetLimits(), EParameterType_IN );
					stmt->Initialize();
					_inOutInitializers.push_back( std::make_pair( stmt, parameter ) );
					_arrayOutParams.push_back( parameter );
				}
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
			queryInOutParam << SQL_SELECT;

			for ( auto && parameter : _arrayOutParams )
			{
				queryInOutParam << sep << SQL_PARAM << parameter->GetName() << SQL_AS << parameter->GetName();
				sep = SQL_COMMA;
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
			CLogger::LogError( ERROR_MYSQL_CANT_CREATE_STATEMENT );
			throw CExceptionDatabase( EDatabaseExceptionCodes_StatementError, ERROR_MYSQL_CANT_CREATE_STATEMENT, __FUNCTION__, __FILE__, __LINE__ );
		}

		std::string strQuery = CStrUtils::ToStr( _query );
		MySQLTry( mysql_stmt_prepare( _statement, strQuery.c_str(), uint32_t( strQuery.size() ) ), STR( "Statement preparation" ), EDatabaseExceptionCodes_StatementError, _connectionMySql->GetConnection() );
		size_t index = 0;

		for ( auto && it : _arrayInParams )
		{
			it->SetStatement( _statement );
			it->SetBinding( &_bindings[index++] );
		}

		return eReturn;
	}

	bool CDatabaseStatementMySql::ExecuteUpdate( EErrorType * result )
	{
		DoPreExecute( result );

		bool bReturn;
		EErrorType eResult = EErrorType_NONE;

		if ( !_bindings.empty() )
		{
			MySQLTry( mysql_stmt_bind_param( _statement, _bindings.data() ), STR( "Statement parameters binding" ), EDatabaseExceptionCodes_StatementError, _connectionMySql->GetConnection() );
		}

		bReturn = _connectionMySql->ExecuteUpdate( _statement );
		DoPostExecute( result );

		if ( result )
		{
			*result = eResult;
		}

		return bReturn;
	}

	DatabaseResultPtr CDatabaseStatementMySql::ExecuteSelect( EErrorType * result )
	{
		DoPreExecute( result );
		EErrorType eResult = EErrorType_NONE;

		if ( !_bindings.empty() )
		{
			MySQLTry( mysql_stmt_bind_param( _statement, _bindings.data() ), STR( "Statement parameters binding" ), EDatabaseExceptionCodes_StatementError, _connectionMySql->GetConnection() );
		}

		DatabaseResultPtr pReturn = _connectionMySql->ExecuteSelect( _statement );
		DoPostExecute( result );

		if ( result )
		{
			*result = eResult;
		}

		return pReturn;
	}

	void CDatabaseStatementMySql::Cleanup()
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

	DatabaseParameterPtr CDatabaseStatementMySql::CreateParameter( const String & name, EFieldType fieldType, EParameterType parameterType )
	{
		DatabaseStatementParameterMySqlPtr pReturn = std::make_shared< CDatabaseStatementParameterMySql >( _connectionMySql, name, uint16_t( _arrayInParams.size() + 1 ), fieldType, parameterType, new SValueUpdater( this ) );

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
		DatabaseStatementParameterMySqlPtr pReturn = std::make_shared< CDatabaseStatementParameterMySql >( _connectionMySql, name, uint16_t( _arrayInParams.size() + 1 ), fieldType, limits, parameterType, new SValueUpdater( this ) );

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
					if ( ( ( parameter->GetParamType() == EParameterType_INOUT ) ||
							( ( parameter->GetParamType() == EParameterType_OUT ) ) ) &&
							( row->HasField( parameter->GetName() ) ) )
					{
						parameter->SetValue( row->GetField( parameter->GetName() ) );
					}
				}
			}
		}
	}
}
END_NAMESPACE_DATABASE_MYSQL
