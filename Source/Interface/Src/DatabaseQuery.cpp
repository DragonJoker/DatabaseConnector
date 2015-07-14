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

	static const String DB_SQL_SELECT = STR( "SELECT " );
	static const String DB_SQL_AS = STR( " AS " );
	static const String DB_SQL_FROM_DUAL = STR( " FROM DUAL;" );
	static const String DB_SQL_PARAM = STR( "@" );
	static const String DB_SQL_COMMA = STR( "," );
	static const String DB_SQL_SET = STR( "SET @" );
	static const String DB_SQL_NULL = STR( " = NULL;" );

	CDatabaseQuery::SValueUpdater::SValueUpdater( CDatabaseQuery * query )
		: _query( query )
	{
	}

	void CDatabaseQuery::SValueUpdater::Update( DatabaseParameterPtr value )
	{
		_query->_mapParamsByPointer[value->GetObjectValue().GetPtrValue()] = value;
	}

	CDatabaseQuery::CDatabaseQuery( DatabaseConnectionPtr connection, const String & query )
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
			_arrayQueries = CStrUtils::Split( _query, STR( "?" ), _paramsCount + 1 );
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
		DatabaseParameterPtr pReturn = std::make_shared< CDatabaseParameter >( _connection, name, ( unsigned short )_arrayParams.size() + 1, fieldType, parameterType, std::make_unique< SValueUpdater >( this ) );

		if ( !DoAddParameter( pReturn ) )
		{
			pReturn.reset();
		}

		return pReturn;
	}

	DatabaseParameterPtr CDatabaseQuery::CreateParameter( const String & name, EFieldType fieldType, uint32_t limits, EParameterType parameterType )
	{
		DatabaseParameterPtr pReturn = std::make_shared< CDatabaseParameter >( _connection, name, ( unsigned short )_arrayParams.size() + 1, fieldType, limits, parameterType, std::make_unique< SValueUpdater >( this ) );

		if ( !DoAddParameter( pReturn ) )
		{
			pReturn.reset();
		}

		return pReturn;
	}

	DatabaseParameterPtr CDatabaseQuery::GetParameter( uint32_t index )const
	{
		try
		{
			return _arrayParams[index];
		}
		catch ( CExceptionDatabase & exc )
		{
			CLogger::LogError( exc.GetFullDescription() );
			throw;
		}
		catch ( ... )
		{
			StringStream message;
			message << ERROR_DB_QUERY_INDEX << index;
			CLogger::LogError( message.str() );
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, message.str() );
		}
	}

	DatabaseParameterPtr CDatabaseQuery::GetParameter( const String & name )const
	{
		auto it = std::find_if( _arrayParams.begin(), _arrayParams.end(), [&name]( DatabaseParameterPtr parameter )
		{
			return parameter->GetName() == name;
		} );

		if ( it == _arrayParams.end() )
		{
			StringStream message;
			message << ERROR_DB_QUERY_NAME << name;
			CLogger::LogError( message.str() );
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

	void CDatabaseQuery::SetParameterValue( uint32_t index, DatabaseParameterPtr parameter )
	{
		GetParameter( index )->SetValue( parameter );
	}

	void CDatabaseQuery::SetParameterValue( const String & name, DatabaseParameterPtr parameter )
	{
		GetParameter( name )->SetValue( parameter );
	}

	bool CDatabaseQuery::DoAddParameter( DatabaseParameterPtr parameter )
	{
		bool bReturn = false;

		if ( parameter )
		{
			auto it = std::find_if( _arrayParams.begin(), _arrayParams.end(), [&parameter]( DatabaseParameterPtr param )
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

	String CDatabaseQuery::DoPreExecute( DatabaseParameterPtrArray & outParams, EErrorType * result )
	{
		assert( _paramsCount == _arrayParams.size() );

		StringStream query;
		StringArray inOutInitializers;
		auto && itQueries = _arrayQueries.begin();
		auto && itParams = _arrayParams.begin();

		inOutInitializers.reserve( _arrayParams.size() );
		outParams.reserve( _arrayParams.size() );

		while ( itQueries != _arrayQueries.end() && itParams != _arrayParams.end() )
		{
			query << ( *itQueries );
			DatabaseParameterPtr parameter = ( *itParams );

			if ( parameter->GetParamType() == EParameterType_IN )
			{
				query << parameter->GetObjectValue().GetQueryValue( !parameter->GetObjectValue().IsNull() ).c_str();
			}
			else if ( parameter->GetParamType() == EParameterType_INOUT )
			{
				query << DB_SQL_PARAM + parameter->GetName();
				inOutInitializers.push_back( DB_SQL_SET + parameter->GetName() + STR( " = " ) + parameter->GetObjectValue().GetQueryValue( !parameter->GetObjectValue().IsNull() ) );
				outParams.push_back( parameter );
			}
			else if ( parameter->GetParamType() == EParameterType_OUT )
			{
				query << DB_SQL_PARAM + parameter->GetName();
				inOutInitializers.push_back( DB_SQL_SET + parameter->GetName() + DB_SQL_NULL );
				outParams.push_back( parameter );
			}

			++itQueries;
			++itParams;
		}

		while ( itQueries != _arrayQueries.end() )
		{
			query << ( *itQueries );
			++itQueries;
		}

		for ( auto query : inOutInitializers )
		{
			_connection->ExecuteUpdate( query, result );
		}

		return query.str();
	}

	void CDatabaseQuery::DoPostExecute( DatabaseParameterPtrArray const & outParams, EErrorType * result )
	{
		if ( !outParams.empty() )
		{
			String sep;
			StringStream queryInOutParam;
			queryInOutParam << DB_SQL_SELECT;
			DatabaseParameterPtr parameter;

			for ( auto && parameter : outParams )
			{
				queryInOutParam << sep << DB_SQL_PARAM << parameter->GetName() << DB_SQL_AS << parameter->GetName();
				sep = DB_SQL_COMMA;
			}

			DatabaseResultPtr pReturnFromDual = _connection->ExecuteSelect( queryInOutParam.str(), result );

			if ( pReturnFromDual )
			{
				DatabaseRowPtr row = pReturnFromDual->GetFirstRow();

				for ( auto && parameter : outParams )
				{
					if ( parameter->GetParamType() == EParameterType_INOUT || parameter->GetParamType() == EParameterType_OUT )
					{
						DatabaseFieldPtr field;

						try
						{
							field = row->GetField( parameter->GetName() );
						}
						catch( CExceptionDatabase & exc )
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
	}
}
END_NAMESPACE_DATABASE
