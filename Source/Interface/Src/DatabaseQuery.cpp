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
	static const String DATABASE_QUERY_INCONSISTENCY_ERROR = STR( "Number of parameters doesn't match the sizes of parameter containers." );
	static const String DATABASE_QUERY_ALREADY_ADDED_PARAMETER = STR( "Parameter with name [%1%] already exists." );
	static const String DATABASE_QUERY_NULL_PARAMETER = STR( "Trying to add a null parameter." );
	static const String SQL_SELECT = STR( "SELECT " );
	static const String SQL_AS = STR( " AS " );
	static const String SQL_FROM_DUAL = STR( " FROM DUAL;" );
	static const String SQL_PARAM = STR( "@" );
	static const String SQL_COMMA = STR( "," );
	static const String SQL_SET = STR( "SET @" );
	static const String SQL_NULL = STR( " = NULL;" );

	CDatabaseQuery::SValueUpdater::SValueUpdater( CDatabaseQuery * query )
		: _query( query )
	{
	}

	void CDatabaseQuery::SValueUpdater::Update( DatabaseParameterPtr value )
	{
		_query->_mapParamsByPointer[value->GetPtrValue()] = value;
	}

	CDatabaseQuery::CDatabaseQuery( DatabaseConnectionPtr connection, const String & query )
		:   _connection( connection )
		,   _paramsCount( 0 )
		,   _query( query )
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
			_arrayQueries = CStrUtils::Split( _query, STR( "?" ) );
			eReturn = EErrorType_NONE;
		}

		return eReturn;
	}

	bool CDatabaseQuery::ExecuteUpdate( EErrorType * result )
	{
		bool bReturn = false;

		if ( _connection->IsConnected() && _arrayQueries.size() > 0 )
		{
			DatabaseParameterPtrArray outParams;
			String query = DoPreExecute( outParams, result );
			bReturn = _connection->ExecuteUpdate( query, result );

			if ( bReturn )
			{
				DoPostExecute( outParams, result );
			}
		}

		return bReturn;
	}

	DatabaseResultPtr CDatabaseQuery::ExecuteSelect( EErrorType * result )
	{
		DatabaseResultPtr pReturn;

		if ( _connection->IsConnected() && _arrayQueries.size() > 0 )
		{
			DatabaseParameterPtrArray outParams;
			String query = DoPreExecute( outParams, result );
			pReturn = _connection->ExecuteSelect( query, result );

			if ( pReturn )
			{
				DoPostExecute( outParams, result );
			}
		}

		return pReturn;
	}

	void CDatabaseQuery::Cleanup()
	{
		_paramsCount = 0;
		_arrayQueries.clear();
	}

	DatabaseParameterPtr CDatabaseQuery::CreateParameter( const String & name, EFieldType fieldType, EParameterType parameterType )
	{
		// std::make_shared limited to 5 parameters with VS2012
		DatabaseParameterPtr pReturn( new CDatabaseParameter( _connection, name, ( unsigned short )_arrayParams.size() + 1, fieldType, parameterType, new SValueUpdater( this ) ) );

		if ( !DoAddParameter( pReturn ) )
		{
			pReturn.reset();
		}

		return pReturn;
	}

	DatabaseParameterPtr CDatabaseQuery::CreateParameter( const String & name, EFieldType fieldType, uint32_t limits, EParameterType parameterType )
	{
		// std::make_shared limited to 5 parameters with VS2012
		DatabaseParameterPtr pReturn( new CDatabaseParameter( _connection, name, ( unsigned short )_arrayParams.size() + 1, fieldType, limits, parameterType, new SValueUpdater( this ) ) );

		if ( !DoAddParameter( pReturn ) )
		{
			pReturn.reset();
		}

		return pReturn;
	}

	void CDatabaseQuery::SetParameterNull( uint32_t index )
	{
		try
		{
			_arrayParams[index]->SetNull();
		}
		catch ( ... )
		{
			StringStream message;
			message << DATABASE_QUERY_INDEX_ERROR << index;
			CLogger::LogError( message );
			DB_EXCEPT( EDatabaseExceptionCodes_QueryError, message.str() );
		}
	}

	void CDatabaseQuery::SetParameterNull( const String & name )
	{
		DatabaseParameterPtrArray::iterator it = std::find_if( _arrayParams.begin(), _arrayParams.end(), QueryParameterFindCondition( name ) );

		if ( it != _arrayParams.end() )
		{
			( *it )->SetNull();
		}
		else
		{
			StringStream message;
			message << DATABASE_QUERY_NAME_ERROR << name;
			CLogger::LogError( message );
			DB_EXCEPT( EDatabaseExceptionCodes_QueryError, message.str() );
		}
	}

	bool CDatabaseQuery::DoAddParameter( DatabaseParameterPtr parameter )
	{
		bool bReturn = false;

		if ( parameter )
		{
			if ( std::find_if( _arrayParams.begin(), _arrayParams.end(), QueryParameterFindCondition( parameter->GetName() ) ) == _arrayParams.end() )
			{
				_arrayParams.push_back( parameter );
				bReturn = true;
			}
			else
			{
				Format format( DATABASE_QUERY_ALREADY_ADDED_PARAMETER );
				format % parameter->GetName();
				CLogger::LogError( format.str() );
			}
		}
		else
		{
			CLogger::LogError( DATABASE_QUERY_NULL_PARAMETER );
		}

		return bReturn;
	}

	String CDatabaseQuery::DoPreExecute( DatabaseParameterPtrArray & outParams, EErrorType * result )
	{
		assert( _paramsCount == _arrayParams.size() );

		StringStream query;
		StringArray inOutInitializers;
		unsigned short i = 0;
		StringArray::iterator itQueries = _arrayQueries.begin();
		DatabaseParameterPtrArray::iterator itParams = _arrayParams.begin();
		DatabaseParameterPtr parameter;

		inOutInitializers.reserve( _arrayParams.size() );
		outParams.reserve( _arrayParams.size() );

		while ( itQueries != _arrayQueries.end() && itParams != _arrayParams.end() )
		{
			query << ( *itQueries );
			parameter = ( *itParams );

			if ( parameter->GetParamType() == EParameterType_IN )
			{
				query << parameter->GetInsertValue().c_str();
			}
			else if ( parameter->GetParamType() == EParameterType_INOUT )
			{
				query << SQL_PARAM + parameter->GetName();
				inOutInitializers.push_back( SQL_SET + parameter->GetName() + STR( " = " ) + parameter->GetInsertValue() );
				outParams.push_back( parameter );
			}
			else if ( parameter->GetParamType() == EParameterType_OUT )
			{
				query << SQL_PARAM + parameter->GetName();
				inOutInitializers.push_back( SQL_SET + parameter->GetName() + SQL_NULL );
				outParams.push_back( parameter );
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

		for ( StringArray::iterator it = inOutInitializers.begin(); it != inOutInitializers.end(); ++it )
		{
			_connection->ExecuteUpdate( *it, result );
		}

		return query.str();
	}

	void CDatabaseQuery::DoPostExecute( DatabaseParameterPtrArray const & outParams, EErrorType * result )
	{
		if ( !outParams.empty() )
		{
			String sep;
			StringStream queryInOutParam;
			queryInOutParam << SQL_SELECT;
			DatabaseParameterPtr parameter;

			for ( DatabaseParameterPtrArray::const_iterator it = outParams.begin(); it != outParams.end(); ++it )
			{
				parameter = ( *it );
				queryInOutParam << sep << SQL_PARAM << parameter->GetName() << SQL_AS << parameter->GetName();
				sep = SQL_COMMA;
			}

			DatabaseResultPtr pReturnFromDual = _connection->ExecuteSelect( queryInOutParam.str(), result );

			if ( pReturnFromDual )
			{
				DatabaseRowPtr row = pReturnFromDual->GetFirstRow();
				DatabaseParameterPtrArray::const_iterator itParams = outParams.begin();

				while ( itParams != outParams.end() )
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
END_NAMESPACE_DATABASE
