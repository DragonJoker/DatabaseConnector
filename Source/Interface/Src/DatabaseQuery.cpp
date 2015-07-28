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

#include "Database.h"
#include "DatabaseConnection.h"
#include "DatabaseParameter.h"
#include "DatabaseException.h"
#include "DatabaseResult.h"
#include "DatabaseRow.h"

#include "DatabaseStringUtils.h"

#include <algorithm>

BEGIN_NAMESPACE_DATABASE
{
	static const String ERROR_DB_QUERY_INDEX = STR( "No query parameter at index: " );
	static const String ERROR_DB_QUERY_NAME = STR( "No query parameter named: " );
	static const String ERROR_DB_QUERY_INCONSISTENCY = STR( "Number of parameters doesn't match the sizes of parameter containers." );
	static const String ERROR_DB_QUERY_ALREADY_ADDED_PARAMETER = STR( "Parameter with name [%1%] already exists." );
	static const String WARNING_DB_QUERY_NULL_PARAMETER = STR( "Trying to add a null parameter." );

	CDatabaseQuery::CDatabaseQuery( DatabaseConnectionSPtr connection, const String & query )
		: _connection( connection )
		, _paramsCount( 0 )
		, _query( query )
	{
		// Empty
	}

	CDatabaseQuery::~CDatabaseQuery()
	{
		Cleanup();
	}

	EErrorType CDatabaseQuery::Initialize()
	{
		EErrorType eReturn = EErrorType_ERROR;

		if ( !_query.empty() )
		{
			_paramsCount = uint32_t( std::count( _query.begin(), _query.end(), STR( '?' ) ) );
			_arrayQueries = StringUtils::Split( _query, STR( "?" ), _paramsCount + 1 );
			eReturn = EErrorType_NONE;
		}

		return eReturn;
	}

	bool CDatabaseQuery::ExecuteUpdate( EErrorType * result )
	{
		bool bReturn = false;
		DatabaseConnectionSPtr connection = DoGetConnection();

		if ( connection && connection->IsConnected() && _arrayQueries.size() > 0 )
		{
			bReturn = connection->ExecuteUpdate( DoPreExecute() );
		}

		return bReturn;
	}

	DatabaseResultSPtr CDatabaseQuery::ExecuteSelect( EErrorType * result )
	{
		DatabaseResultSPtr pReturn;
		DatabaseConnectionSPtr connection = DoGetConnection();

		if ( connection && connection->IsConnected() && _arrayQueries.size() > 0 )
		{
			pReturn = connection->ExecuteSelect( DoPreExecute() );
		}

		return pReturn;
	}

	void CDatabaseQuery::Cleanup()
	{
		_paramsCount = 0;
		_arrayQueries.clear();
	}

	DatabaseParameterSPtr CDatabaseQuery::CreateParameter( const String & name, EFieldType fieldType )
	{
		DatabaseParameterSPtr pReturn = std::make_shared< CDatabaseParameter >( DoGetConnection(), name, ( unsigned short )_arrayParams.size() + 1, fieldType, EParameterType_IN, std::make_unique< SDummyValueUpdater >() );

		if ( !DoAddParameter( pReturn ) )
		{
			pReturn.reset();
		}

		return pReturn;
	}

	DatabaseParameterSPtr CDatabaseQuery::CreateParameter( const String & name, EFieldType fieldType, uint32_t limits )
	{
		DatabaseParameterSPtr pReturn = std::make_shared< CDatabaseParameter >( DoGetConnection(), name, ( unsigned short )_arrayParams.size() + 1, fieldType, limits, EParameterType_IN, std::make_unique< SDummyValueUpdater >() );

		if ( !DoAddParameter( pReturn ) )
		{
			pReturn.reset();
		}

		return pReturn;
	}

	DatabaseParameterSPtr CDatabaseQuery::CreateParameter( const String & name, EFieldType fieldType, const std::pair< uint32_t, uint32_t > & precision )
	{
		DatabaseParameterSPtr pReturn = std::make_shared< CDatabaseParameter >( DoGetConnection(), name, ( unsigned short )_arrayParams.size() + 1, fieldType, precision, EParameterType_IN, std::make_unique< SDummyValueUpdater >() );

		if ( !DoAddParameter( pReturn ) )
		{
			pReturn.reset();
		}

		return pReturn;
	}

	DatabaseParameterSPtr CDatabaseQuery::GetParameter( uint32_t index )const
	{
		try
		{
			return _arrayParams[index];
		}
		catch ( CDatabaseException & exc )
		{
			CLogger::LogError( exc.GetFullDescription() );
			throw;
		}
		catch ( ... )
		{
			StringStream message;
			message << ERROR_DB_QUERY_INDEX << index;
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, message.str() );
		}
	}

	DatabaseParameterSPtr CDatabaseQuery::GetParameter( const String & name )const
	{
		auto it = std::find_if( _arrayParams.begin(), _arrayParams.end(), [&name]( DatabaseParameterSPtr parameter )
		{
			return parameter->GetName() == name;
		} );

		if ( it == _arrayParams.end() )
		{
			StringStream message;
			message << ERROR_DB_QUERY_NAME << name;
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, message.str() );
		}

		return ( *it );
	}

	EFieldType CDatabaseQuery::GetParameterType( uint32_t index )
	{
		return GetParameter( index )->GetType();
	}

	void CDatabaseQuery::SetParameterNull( uint32_t index )
	{
		GetParameter( index )->SetNull();
	}

	void CDatabaseQuery::SetParameterNull( const String & name )
	{
		GetParameter( name )->SetNull();
	}

	void CDatabaseQuery::SetParameterValue( uint32_t index, const CDatabaseParameter & parameter )
	{
		GetParameter( index )->SetValue( parameter );
	}

	void CDatabaseQuery::SetParameterValue( const String & name, const CDatabaseParameter & parameter )
	{
		GetParameter( name )->SetValue( parameter );
	}

	bool CDatabaseQuery::DoAddParameter( DatabaseParameterSPtr parameter )
	{
		bool bReturn = false;

		if ( parameter )
		{
			auto it = std::find_if( _arrayParams.begin(), _arrayParams.end(), [&parameter]( DatabaseParameterSPtr param )
			{
				return param->GetName() == parameter->GetName();
			} );

			if ( it == _arrayParams.end() )
			{
				_arrayParams.push_back( parameter );
				bReturn = true;
			}
			else
			{
				Format format( ERROR_DB_QUERY_ALREADY_ADDED_PARAMETER );
				format % parameter->GetName();
				CLogger::LogError( format.str() );
			}
		}
		else
		{
			CLogger::LogWarning( WARNING_DB_QUERY_NULL_PARAMETER );
		}

		return bReturn;
	}

	String CDatabaseQuery::DoPreExecute()
	{
		if ( _paramsCount != _arrayParams.size() )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_QueryError, ERROR_DB_QUERY_INCONSISTENCY );
		}

		StringStream query;
		auto itQueries = _arrayQueries.begin();

		for ( auto && parameter : _arrayParams )
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
