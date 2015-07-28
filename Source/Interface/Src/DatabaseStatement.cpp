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
	static const String ERROR_STATEMENT_INITIALISATION = STR( "Statement initialisation failed: " );
	static const String ERROR_STATEMENT_EXECUTION = STR( "Statement execution failed: " );
	static const String ERROR_STATEMENT_CLEANUP = STR( "Statement cleanup failed: " );

	static const String WARNING_DB_STATEMENT_NULL_PARAMETER = STR( "Trying to add a null parameter." );

	static const String INFO_UNKNOWN_ERROR = "Unknown error";

	CDatabaseStatement::SValueUpdater::SValueUpdater( CDatabaseStatement * stmt )
		: _stmt( stmt )
	{
	}

	void CDatabaseStatement::SValueUpdater::Update( const CDatabaseParameter & value )
	{
		_stmt->_mapParamsByPointer[value.GetObjectValue().GetPtrValue()] = &value;
	}

	CDatabaseStatement::CDatabaseStatement( DatabaseConnectionSPtr connection, const String & query )
		: _connection( connection )
		, _query( query )
	{
		// Empty
	}

	CDatabaseStatement::~CDatabaseStatement()
	{
		// Empty
	}

	EErrorType CDatabaseStatement::Initialize()
	{
		EErrorType ret = EErrorType_ERROR;

		try
		{
			ret = DoInitialize();
		}
		catch( CDatabaseException & exc )
		{
			CLogger::LogError( ERROR_STATEMENT_INITIALISATION + exc.GetFullDescription() );
		}
		catch( std::exception & exc )
		{
			CLogger::LogError( ERROR_STATEMENT_INITIALISATION + exc.what() );
		}
		catch( ... )
		{
			CLogger::LogError( ERROR_STATEMENT_INITIALISATION + INFO_UNKNOWN_ERROR );
		}

		return ret;
	}

	bool CDatabaseStatement::ExecuteUpdate( EErrorType * result )
	{
		bool ret = false;

		try
		{
			ret = DoExecuteUpdate( result );
		}
		catch( CDatabaseException & exc )
		{
			CLogger::LogError( ERROR_STATEMENT_EXECUTION + exc.GetFullDescription() );
		}
		catch( std::exception & exc )
		{
			CLogger::LogError( ERROR_STATEMENT_EXECUTION + exc.what() );
		}
		catch( ... )
		{
			CLogger::LogError( ERROR_STATEMENT_EXECUTION + INFO_UNKNOWN_ERROR );
		}

		return ret;
	}

	DatabaseResultSPtr CDatabaseStatement::ExecuteSelect( EErrorType * result )
	{
		DatabaseResultSPtr ret;

		try
		{
			ret = DoExecuteSelect( result );
		}
		catch( CDatabaseException & exc )
		{
			CLogger::LogError( ERROR_STATEMENT_EXECUTION + exc.GetFullDescription() );
		}
		catch( std::exception & exc )
		{
			CLogger::LogError( ERROR_STATEMENT_EXECUTION + exc.what() );
		}
		catch( ... )
		{
			CLogger::LogError( ERROR_STATEMENT_EXECUTION + INFO_UNKNOWN_ERROR );
		}

		return ret;
	}

	void CDatabaseStatement::Cleanup()
	{
		try
		{
			DoCleanup();
		}
		catch( CDatabaseException & exc )
		{
			CLogger::LogError( ERROR_STATEMENT_CLEANUP + exc.GetFullDescription() );
		}
		catch( std::exception & exc )
		{
			CLogger::LogError( ERROR_STATEMENT_CLEANUP + exc.what() );
		}
		catch( ... )
		{
			CLogger::LogError( ERROR_STATEMENT_CLEANUP + INFO_UNKNOWN_ERROR );
		}
	}

	DatabaseParameterSPtr CDatabaseStatement::GetParameter( uint32_t index )const
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
			message << ERROR_DB_STATEMENT_INDEX << index;
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, message.str() );
		}
	}

	DatabaseParameterSPtr CDatabaseStatement::GetParameter( const String & name )const
	{
		auto && it = std::find_if( _arrayParams.begin(), _arrayParams.end(), [&name]( DatabaseParameterSPtr parameter )
		{
			return parameter->GetName() == name;
		} );

		if ( it == _arrayParams.end() )
		{
			StringStream message;
			message << ERROR_DB_STATEMENT_NAME << name;
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

	void CDatabaseStatement::SetParameterValue( uint32_t index, const CDatabaseParameter & parameter )
	{
		GetParameter( index )->SetValue( parameter );
	}

	void CDatabaseStatement::SetParameterValue( const String & name, const CDatabaseParameter & parameter )
	{
		GetParameter( name )->SetValue( parameter );
	}

	bool CDatabaseStatement::DoAddParameter( DatabaseParameterSPtr parameter )
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
