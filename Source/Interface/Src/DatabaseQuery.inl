/************************************************************************//**
 * @file DatabaseQuery.inl
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 *
 *
 * @brief CDatabaseQuery class definition.
 *
 * @details Describes CDatabaseQuery template methods.
 *
***************************************************************************/

#include "DatabaseLogger.h"

BEGIN_NAMESPACE_DATABASE
{
	namespace
	{
		struct QueryParameterFindCondition
		{
			const String & _name;

			QueryParameterFindCondition( const String & name )
				: _name( name )
			{
			}

			bool operator()( DatabaseParameterPtr parameter )
			{
				return parameter->GetName() == _name;
			}
		};
	}

	static const String DATABASE_QUERY_INDEX_ERROR = STR( "No query parameter at index: " );
	static const String DATABASE_QUERY_NAME_ERROR = STR( "No query parameter named: " );

	template< typename T >
	void CDatabaseQuery::SetParameterValue( uint32_t index, const T & value )
	{
		try
		{
			DatabaseParameterPtr parameter = _arrayParams[index];
			parameter->SetValue( value );
		}
		catch ( ... )
		{
			StringStream message;
			message << DATABASE_QUERY_INDEX_ERROR << index;
			CLogger::LogError( message );
			DB_EXCEPT( EDatabaseExceptionCodes_QueryError, message.str() );
		}
	}

	template< typename T >
	void CDatabaseQuery::SetParameterValue( const String & name, const T & value )
	{
		DatabaseParameterPtrArray::iterator it = std::find_if( _arrayParams.begin(), _arrayParams.end(), QueryParameterFindCondition( name ) );

		if ( it != _arrayParams.end() )
		{
			DatabaseParameterPtr parameter = ( *it );
			parameter->SetValue( value );
		}
		else
		{
			StringStream message;
			message << DATABASE_QUERY_NAME_ERROR << name;
			CLogger::LogError( message );
			DB_EXCEPT( EDatabaseExceptionCodes_QueryError, message.str() );
		}
	}

	template< typename T >
	const T & CDatabaseQuery::GetOutputValue( uint32_t index )
	{
		try
		{
			return _arrayParams[index]->GetValue< T >();
		}
		catch ( ... )
		{
			StringStream message;
			message << DATABASE_QUERY_INDEX_ERROR << index;
			CLogger::LogError( message );
			DB_EXCEPT( EDatabaseExceptionCodes_QueryError, message.str() );
		}
	}

	template< typename T >
	const T & CDatabaseQuery::GetOutputValue( const String & name )
	{
		DatabaseParameterPtrArray::iterator it = std::find_if( _arrayParams.begin(), _arrayParams.end(), QueryParameterFindCondition( name ) );

		if ( it != _arrayParams.end() )
		{
			return ( *it )->GetValue< T >();
		}
		else
		{
			StringStream message;
			message << DATABASE_QUERY_NAME_ERROR << name;
			CLogger::LogError( message );
			DB_EXCEPT( EDatabaseExceptionCodes_QueryError, message.str() );
		}
	}

	template< typename T >
	void CDatabaseQuery::GetOutputValue( uint32_t index, T & value )
	{
		try
		{
			_arrayParams[index]->GetValue( value );
		}
		catch ( ... )
		{
			StringStream message;
			message << DATABASE_QUERY_INDEX_ERROR << index;
			CLogger::LogError( message );
			DB_EXCEPT( EDatabaseExceptionCodes_QueryError, message.str() );
		}
	}

	template< typename T >
	void CDatabaseQuery::GetOutputValue( const String & name, T & value )
	{
		DatabaseParameterPtrArray::iterator it = std::find_if( _arrayParams.begin(), _arrayParams.end(), QueryParameterFindCondition( name ) );

		if ( it != _arrayParams.end() )
		{
			( *it )->GetValue( value );
		}
		else
		{
			StringStream message;
			message << DATABASE_QUERY_NAME_ERROR << name;
			CLogger::LogError( message );
			DB_EXCEPT( EDatabaseExceptionCodes_QueryError, message.str() );
		}
	}
}
END_NAMESPACE_DATABASE
