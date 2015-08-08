/************************************************************************//**
* @file DatabaseQuery.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDatabaseQuery class definition.
*
* @details Describes a database query.
*
***************************************************************************/

#include "DatabasePch.h"

#include "DatabaseQuery.h"

#include "DatabaseParameter.h"
#include "DatabaseValuedObjectInfos.h"

BEGIN_NAMESPACE_DATABASE
{
	namespace
	{
		/** Does nothing
		*/
		struct SDummyValueUpdater
			: public CDatabaseParameter::SValueUpdater
		{
			/** Constructor
			*/
			DatabaseExport SDummyValueUpdater() {}

			//!@copydoc CDatabaseParameter::SValueUpdater
			DatabaseExport virtual void Update( const CDatabaseParameter & value ) {}
		};
	}

	static const String ERROR_DB_QUERY_EMPTY = STR( "The query is empty." );
	static const String ERROR_DB_QUERY_INCONSISTENCY = STR( "Number of parameters doesn't match the sizes of parameter containers." );
	static const String ERROR_DB_QUERY_INVALID = STR( "The query text is invalid." );
	static const String ERROR_DB_QUERY_NOT_INITIALISED = STR( "The query is not initialised." );
	static const String ERROR_DB_QUERY_EXECUTION_ERROR = STR( "Query execution error: " );
	static const String ERROR_DB_QUERY_LOST_CONNECTION = STR( "Query lost  it's connection" );

#if defined( _X64 )
	static const TChar * INFO_DB_CREATING_QUERY = STR( "Creating query object 0x%016X, with query text %s" );
	static const TChar * INFO_DB_DELETING_QUERY = STR( "Deleting query object 0x%016X" );
	static const TChar * INFO_DB_EXECUTING_SELECT_QUERY = STR( "Executing Select on query object 0x%016X" );
	static const TChar * INFO_DB_EXECUTING_UPDATE_QUERY = STR( "Executing Update on query object 0x%016X" );
#else
	static const TChar * INFO_DB_CREATING_QUERY = STR( "Creating query object 0x%08X, with query text %s" );
	static const TChar * INFO_DB_DELETING_QUERY = STR( "Deleting query object 0x%08X" );
	static const TChar * INFO_DB_EXECUTING_SELECT_QUERY = STR( "Executing Select on query object 0x%08X" );
	static const TChar * INFO_DB_EXECUTING_UPDATE_QUERY = STR( "Executing Update on query object 0x%08X" );
#endif

	CDatabaseQuery::CDatabaseQuery( DatabaseConnectionSPtr connection, const String & query )
		: CDatabaseParameteredObject()
		, _connection( connection )
		, _paramsCount( 0 )
		, _query( query )
	{
		CLogger::LogInfo( INFO_DB_CREATING_QUERY, this, query.c_str() );
	}

	CDatabaseQuery::~CDatabaseQuery()
	{
		Cleanup();
		CLogger::LogInfo( INFO_DB_DELETING_QUERY, this );
	}

	bool CDatabaseQuery::ExecuteUpdate()
	{
		if ( _arrayQueries.empty() )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_QueryError, ERROR_DB_QUERY_NOT_INITIALISED );
		}

		DatabaseConnectionSPtr connection = DoGetConnection();

		if ( !connection  )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_QueryError, ERROR_DB_QUERY_LOST_CONNECTION );
		}

		bool bReturn = false;

		connection->DoCheckConnected();
		connection->DoCheckDatabaseSelected();

		try
		{
			CLogger::LogInfo( INFO_DB_EXECUTING_UPDATE_QUERY, this );
			bReturn = connection->DoExecuteUpdate( DoPreExecute() );
		}
		COMMON_CATCH( ERROR_DB_QUERY_EXECUTION_ERROR )

		return bReturn;
	}

	DatabaseResultSPtr CDatabaseQuery::ExecuteSelect()
	{
		if ( _arrayQueries.empty() )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_QueryError, ERROR_DB_QUERY_NOT_INITIALISED );
		}

		DatabaseConnectionSPtr connection = DoGetConnection();

		if ( !connection  )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_QueryError, ERROR_DB_QUERY_LOST_CONNECTION );
		}

		connection->DoCheckConnected();
		connection->DoCheckDatabaseSelected();
		DatabaseResultSPtr pReturn;

		try
		{
			CLogger::LogInfo( INFO_DB_EXECUTING_SELECT_QUERY, this );
			pReturn = connection->DoExecuteSelect( DoPreExecute(), _infos );
		}
		COMMON_CATCH( ERROR_DB_QUERY_EXECUTION_ERROR )

		return pReturn;
	}

	DatabaseParameterSPtr CDatabaseQuery::CreateParameter( const String & name, EFieldType fieldType )
	{
		return DoCreateParameter( std::make_shared< CDatabaseValuedObjectInfos >( name, fieldType ) );
	}

	DatabaseParameterSPtr CDatabaseQuery::CreateParameter( const String & name, EFieldType fieldType, uint32_t limits )
	{
		return DoCreateParameter( std::make_shared< CDatabaseValuedObjectInfos >( name, fieldType, limits ) );
	}

	DatabaseParameterSPtr CDatabaseQuery::CreateParameter( const String & name, EFieldType fieldType, const std::pair< uint32_t, uint32_t > & precision )
	{
		return DoCreateParameter( std::make_shared< CDatabaseValuedObjectInfos >( name, fieldType, precision ) );
	}

	EErrorType CDatabaseQuery::DoInitialise()
	{
		if ( _query.empty() )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_QueryError, ERROR_DB_QUERY_EMPTY );
		}

		_paramsCount = uint32_t( std::count( _query.begin(), _query.end(), STR( '?' ) ) );

		if ( _paramsCount != GetParametersCount() )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_QueryError, ERROR_DB_QUERY_INCONSISTENCY );
		}

		_arrayQueries = StringUtils::Split( _query, STR( "?" ), _paramsCount + 1, false );

		if ( _arrayQueries.empty() )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_QueryError, ERROR_DB_QUERY_INVALID );
		}

		return EErrorType_NONE;
	}

	void CDatabaseQuery::DoCleanup()
	{
		_paramsCount = 0;
		_arrayQueries.clear();
	}

	DatabaseParameterSPtr CDatabaseQuery::DoCreateParameter( DatabaseValuedObjectInfosSPtr infos, EParameterType parameterType )
	{
		return DoAddParameter( std::make_shared< CDatabaseParameter >( DoGetConnection(), infos, GetParametersCount() + 1, parameterType, std::make_unique< SDummyValueUpdater >() ) );
	}

	String CDatabaseQuery::DoPreExecute()
	{
		StringStream query;
		auto itQueries = _arrayQueries.begin();

		for ( auto && parameter : DoGetParameters() )
		{
			query << ( *itQueries++ );
			query << ( parameter )->GetObjectValue().GetQueryValue().c_str();
		}

		while ( itQueries != _arrayQueries.end() )
		{
			query << ( *itQueries );
			++itQueries;
		}

		return query.str();
	}
}
END_NAMESPACE_DATABASE
