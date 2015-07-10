/************************************************************************//**
 * @file DatabaseStatement.inl
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 *
 *
 * @brief CDatabaseStatement class definition.
 *
 * @details Describes a database statement.
 *
***************************************************************************/

#include "DatabaseLogger.h"

BEGIN_NAMESPACE_DATABASE
{
	namespace
	{
		struct StatementParameterFindCondition
		{
			const String & _name;

			StatementParameterFindCondition( const String & name )
				: _name( name )
			{
			}

			bool operator()( DatabaseParameterPtr parameter )
			{
				return parameter->GetName() == _name;
			}
		};
	}

	static const String DATABASE_STATEMENT_INDEX_ERROR = STR( "No statement parameter at index: " );
	static const String DATABASE_STATEMENT_NAME_ERROR = STR( "No statement parameter named: " );

	template< typename T >
	void CDatabaseStatement::SetParameterValue( uint32_t index, const T & value )
	{
		try
		{
			DatabaseParameterPtr parameter = _arrayParams[index];
			parameter->SetValue( value );
			_mapParamsByPointer[parameter->GetObjectValue().GetPtrValue()] = parameter;
		}
		catch ( CExceptionDatabase & exc )
		{
			CLogger::LogError( exc.GetFullDescription() );
			throw;
		}
		catch ( ... )
		{
			StringStream message;
			message << DATABASE_STATEMENT_INDEX_ERROR << index;
			CLogger::LogError( message.str() );
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, message.str() );
		}
	}

	template< typename T >
	void CDatabaseStatement::SetParameterValue( const String & name, const T & value )
	{
		DatabaseParameterPtrArray::iterator it = std::find_if( _arrayParams.begin(), _arrayParams.end(), StatementParameterFindCondition( name ) );

		if ( it != _arrayParams.end() )
		{
			DatabaseParameterPtr parameter = ( *it );
			parameter->SetValue( value );
			_mapParamsByPointer[parameter->GetObjectValue().GetPtrValue()] = parameter;
		}
		else
		{
			StringStream message;
			message << DATABASE_STATEMENT_NAME_ERROR << name;
			CLogger::LogError( message.str() );
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, message.str() );
		}
	}

	template <typename T>
	const T & CDatabaseStatement::GetOutputValue( uint32_t index )
	{
		try
		{
			return _arrayParams[index]->GetValue< T >();
		}
		catch ( CExceptionDatabase & exc )
		{
			CLogger::LogError( exc.GetFullDescription() );
			throw;
		}
		catch ( ... )
		{
			StringStream message;
			message << DATABASE_STATEMENT_INDEX_ERROR << index;
			CLogger::LogError( message.str() );
			DB_EXCEPT( EDatabaseExceptionCodes_QueryError, message.str() );
		}
	}

	template <typename T>
	const T & CDatabaseStatement::GetOutputValue( const String & name )
	{
		DatabaseParameterPtrArray::iterator it = std::find_if( _arrayParams.begin(), _arrayParams.end(), StatementParameterFindCondition( name ) );

		if ( it != _arrayParams.end() )
		{
			return ( *it )->GetValue< T >();
		}
		else
		{
			StringStream message;
			message << DATABASE_STATEMENT_NAME_ERROR << name;
			CLogger::LogError( message.str() );
			DB_EXCEPT( EDatabaseExceptionCodes_QueryError, message.str() );
		}
	}

	template <typename T>
	void CDatabaseStatement::GetOutputValue( uint32_t index, T & value )
	{
		try
		{
			_arrayParams[index]->GetValue( value );
		}
		catch ( CExceptionDatabase & exc )
		{
			CLogger::LogError( exc.GetFullDescription() );
			throw;
		}
		catch ( ... )
		{
			StringStream message;
			message << DATABASE_STATEMENT_INDEX_ERROR << index;
			CLogger::LogError( message.str() );
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, message.str() );
		}
	}

	template <typename T>
	void CDatabaseStatement::GetOutputValue( const String & name, T & value )
	{
		DatabaseParameterPtrArray::iterator it = std::find_if( _arrayParams.begin(), _arrayParams.end(), StatementParameterFindCondition( name ) );

		if ( it != _arrayParams.end() )
		{
			( *it )->GetValue( value );
		}
		else
		{
			StringStream message;
			message << DATABASE_STATEMENT_NAME_ERROR << name;
			CLogger::LogError( message.str() );
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, message.str() );
		}
	}

}
END_NAMESPACE_DATABASE
