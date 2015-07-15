/************************************************************************//**
* @file DatabaseStatement.cpp
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

#include "DatabasePch.h"

#include "DatabaseStatement.h"

#include "Database.h"
#include "DatabaseParameter.h"
#include "DatabaseException.h"

#include "DatabaseStringUtils.h"
#include "DatabaseLogger.h"

BEGIN_NAMESPACE_DATABASE
{
	static const String ERROR_DB_STATEMENT_INDEX = STR( "No statement parameter at index: " );
	static const String ERROR_DB_STATEMENT_NAME = STR( "No statement parameter named: " );
	static const String ERROR_DB_STATEMENT_ALREADY_ADDED_PARAMETER = STR( "Parameter with name [%1%] already exists." );
	static const String WARNING_DB_STATEMENT_NULL_PARAMETER = STR( "Trying to add a null parameter." );

	CDatabaseStatement::SValueUpdater::SValueUpdater( CDatabaseStatement * stmt )
		: _stmt( stmt )
	{
	}

	void CDatabaseStatement::SValueUpdater::Update( DatabaseParameterPtr value )
	{
		_stmt->_mapParamsByPointer[value->GetObjectValue().GetPtrValue()] = value;
	}

	CDatabaseStatement::CDatabaseStatement( DatabaseConnectionPtr connection, const String & query )
		: _connection( connection )
		, _query( query )
	{
		// Empty
	}

	CDatabaseStatement::~CDatabaseStatement()
	{
		// Empty
	}

	DatabaseParameterPtr CDatabaseStatement::GetParameter( uint32_t index )const
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
			message << ERROR_DB_STATEMENT_INDEX << index;
			CLogger::LogError( message.str() );
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, message.str() );
		}
	}

	DatabaseParameterPtr CDatabaseStatement::GetParameter( const String & name )const
	{
		auto && it = std::find_if( _arrayParams.begin(), _arrayParams.end(), [&name]( DatabaseParameterPtr parameter )
		{
			return parameter->GetName() == name;
		} );

		if ( it == _arrayParams.end() )
		{
			StringStream message;
			message << ERROR_DB_STATEMENT_NAME << name;
			CLogger::LogError( message.str() );
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, message.str() );
		}

		return ( *it );
	}

	EFieldType CDatabaseStatement::GetParameterType( uint32_t index )
	{
		return GetParameter( index )->GetType();
	}

	void CDatabaseStatement::SetParameterNull( uint32_t index )
	{
		GetParameter( index )->SetNull();
	}

	void CDatabaseStatement::SetParameterNull( const String & name )
	{
		GetParameter( name )->SetNull();
	}

	void CDatabaseStatement::SetParameterValue( uint32_t index, DatabaseParameterPtr parameter )
	{
		GetParameter( index )->SetValue( parameter );
	}

	void CDatabaseStatement::SetParameterValue( const String & name, DatabaseParameterPtr parameter )
	{
		GetParameter( name )->SetValue( parameter );
	}

	bool CDatabaseStatement::DoAddParameter( DatabaseParameterPtr parameter )
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
				Format format( ERROR_DB_STATEMENT_ALREADY_ADDED_PARAMETER );
				format % parameter->GetName();
				CLogger::LogError( format.str() );
			}
		}
		else
		{
			CLogger::LogError( WARNING_DB_STATEMENT_NULL_PARAMETER );
		}

		return bReturn;
	}
}
END_NAMESPACE_DATABASE
