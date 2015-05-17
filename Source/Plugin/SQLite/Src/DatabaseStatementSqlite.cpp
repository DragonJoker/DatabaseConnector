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

#include <DatabaseStringUtils.h>

#include <DatabaseRow.h>

BEGIN_NAMESPACE_DATABASE_SQLITE
{
	static const String SQLITE_MISSING_INITIALIZATION_ERROR = STR( "Method Initialize must be called before calling method CreateParameter" );
	static const String ERROR_SQLITE_CANT_PREPARE_STATEMENT = STR( "Couldn't prepare the statement" );
	static const String ERROR_SQLITE_EXECUTION_ERROR = STR( "Couldn't execute the statement : " );
	static const String ERROR_SQLITE_QUERY_INCONSISTENCY_ERROR = STR( "Number of parameters doesn't match the sizes of parameter containers." );

	static const String SQLITE_BIND_PARAMETER_NAME_MSG = STR( "BindParameter : " );
	static const String SQLITE_BIND_PARAMETER_VALUE_MSG = STR( ", Value : " );

	static const String SQL_DELIM = STR( "?" );
	static const String SQL_PARAM = STR( "@" );
	static const String SQL_SET = STR( "SET @" );
	static const String SQL_NULL = STR( " = NULL;" );
	static const String SQL_SELECT = STR( "SELECT " );
	static const String SQL_AS = STR( " AS " );
	static const String SQL_COMMA = STR( "," );

	CDatabaseStatementSqlite::CDatabaseStatementSqlite( DatabaseConnectionSqlitePtr connection, const String & query )
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

	EErrorType CDatabaseStatementSqlite::Initialize()
	{
		EErrorType eReturn = EErrorType_ERROR;

		if ( !_query.empty() )
		{
			_paramsCount = uint32_t( std::count( _query.begin(), _query.end(), STR( '?' ) ) );
			_arrayQueries = CStrUtils::Split( _query, STR( "?" ) );
		}

		CLogger::LogMessage( STR( "Preparing statement for query : " ) + _query );
		assert( _paramsCount == _arrayParams.size() );

		StringStream query;
		unsigned short i = 0;
		StringArray::iterator itQueries = _arrayQueries.begin();
		DatabaseParameterPtrArray::iterator itParams = _arrayParams.begin();
		DatabaseParameterPtr parameter;

		_outInitializers.clear();
		_arrayOutParams.clear();

		_outInitializers.reserve( _arrayParams.size() );
		_arrayOutParams.reserve( _arrayParams.size() );

		while ( itQueries != _arrayQueries.end() && itParams != _arrayParams.end() )
		{
			query << ( *itQueries );
			parameter = ( *itParams );

			if ( parameter->GetParamType() == EParameterType_IN )
			{
				query << SQL_DELIM;
			}
			else if ( parameter->GetParamType() == EParameterType_INOUT )
			{
				query << SQL_PARAM + parameter->GetName();
				DatabaseStatementPtr stmt = _connection->CreateStatement( SQL_SET + parameter->GetName() + STR( " = " ) + SQL_DELIM );
				stmt->CreateParameter( parameter->GetName(), parameter->GetType(), parameter->GetLimits(), EParameterType_IN );
				stmt->Initialize();
				_inOutInitializers.push_back( std::make_pair( stmt, parameter ) );
				_arrayOutParams.push_back( parameter );
			}
			else if ( parameter->GetParamType() == EParameterType_OUT )
			{
				query << SQL_PARAM + parameter->GetName();
				DatabaseStatementPtr stmt = _connection->CreateStatement( SQL_SET + parameter->GetName() + SQL_NULL );
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
			queryInOutParam << SQL_SELECT;

			for ( DatabaseParameterPtrArray::const_iterator it = _arrayOutParams.begin(); it != _arrayOutParams.end(); ++it )
			{
				parameter = ( *it );
				queryInOutParam << sep << SQL_PARAM << parameter->GetName() << SQL_AS << parameter->GetName();
				sep = SQL_COMMA;
			}

			_stmtOutParams = _connection->CreateStatement( queryInOutParam.str() );
			_stmtOutParams->Initialize();
		}

		SQLite::Database * connection = _connectionSqlite->GetConnection();
		std::string query2 = CStrUtils::ToStr( _query );
		SQLiteTry( SQLite::PrepareV2( connection, query2.c_str(), query2.size(), &_statement, NULL ), STR( "Statement preparation" ) );

		if ( _statement )
		{
			eReturn = EErrorType_NONE;
		}
		else
		{
			CLogger::LogError( ERROR_SQLITE_CANT_PREPARE_STATEMENT );
			throw CExceptionDatabase( EDatabaseExceptionCodes_StatementError, ERROR_SQLITE_CANT_PREPARE_STATEMENT, __FUNCTION__, __FILE__, __LINE__ );
		}

		for ( DatabaseParameterPtrArray::iterator it = _arrayInParams.begin(); it != _arrayInParams.end(); ++it )
		{
			std::static_pointer_cast< CDatabaseStatementParameterSqlite >( *it )->SetStatement( _statement );
		}

		return eReturn;
	}

	bool CDatabaseStatementSqlite::ExecuteUpdate( EErrorType * result )
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

	DatabaseResultPtr CDatabaseStatementSqlite::ExecuteSelect( EErrorType * result )
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

	void CDatabaseStatementSqlite::Cleanup()
	{
		if ( _statement )
		{
			SQLite::Database * connection = _connectionSqlite->GetConnection();
			SQLite::Finalize( _statement );
			_statement = NULL;
		}

		_arrayInParams.clear();
		_arrayOutParams.clear();
		_outInitializers.clear();
		_arrayQueries.clear();
		_paramsCount = 0;
		_stmtOutParams.reset();
	}

	DatabaseParameterPtr CDatabaseStatementSqlite::CreateParameter( const String & name, EFieldType fieldType, EParameterType parameterType )
	{
		// std::make_shared limited to 5 parameters with VS2012
		DatabaseParameterPtr pReturn( new CDatabaseStatementParameterSqlite( _connectionSqlite, name, ( unsigned short )_arrayInParams.size() + 1, fieldType, parameterType, new SValueUpdater( this ) ) );

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

	DatabaseParameterPtr CDatabaseStatementSqlite::CreateParameter( const String & name, EFieldType fieldType, uint32_t limits, EParameterType parameterType )
	{
		// std::make_shared limited to 5 parameters with VS2012
		DatabaseParameterPtr pReturn( new CDatabaseStatementParameterSqlite( _connectionSqlite, name, ( unsigned short )_arrayInParams.size() + 1, fieldType, limits, parameterType, new SValueUpdater( this ) ) );

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

	void CDatabaseStatementSqlite::DoPreExecute( EErrorType * result )
	{
		for ( std::vector< std::pair< DatabaseStatementPtr, DatabaseParameterPtr > >::iterator it = _inOutInitializers.begin(); it != _inOutInitializers.end(); ++it )
		{
			if ( it->second->IsNull() )
			{
				it->first->SetParameterNull( 0 );
			}
			else
			{
				it->first->SetParameterValue( 0, it->second );
			}

			it->first->ExecuteUpdate( result );
		}

		for ( std::vector< DatabaseStatementPtr >::iterator it = _outInitializers.begin(); it != _outInitializers.end(); ++it )
		{
			( *it )->ExecuteUpdate( result );
		}
	}

	void CDatabaseStatementSqlite::DoPostExecute( EErrorType * result )
	{
		if ( !_arrayOutParams.empty() )
		{
			DatabaseResultPtr pReturn = _stmtOutParams->ExecuteSelect();

			if ( pReturn && pReturn->GetRowCount() )
			{
				DatabaseParameterPtr parameter;
				DatabaseRowPtr row = pReturn->GetFirstRow();
				DatabaseParameterPtrArray::const_iterator itParams = _arrayOutParams.begin();

				while ( itParams != _arrayOutParams.end() )
				{
					parameter = ( *itParams );

					if ( ( ( parameter->GetParamType() == EParameterType_INOUT ) ||
							( ( parameter->GetParamType() == EParameterType_OUT ) ) ) &&
							( row->HasField( parameter->GetName() ) ) )
					{
						Database::DatabaseFieldPtr field = row->GetField( parameter->GetName() );
						parameter->SetValue( field );
					}

					++itParams;
				}
			}
		}
	}
}
END_NAMESPACE_DATABASE_SQLITE
