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
#include "DatabaseValuedObjectInfos.h"
#include "DatabaseException.h"

#include "DatabaseStringUtils.h"
#include "DatabaseLogger.h"

BEGIN_NAMESPACE_DATABASE
{
	static const String ERROR_DB_STATEMENT_NOT_INITIALISED = STR( "Statement is not initialised" );
	static const String ERROR_STATEMENT_EXECUTION = STR( "Statement execution failed: " );

	static const String WARNING_DB_STATEMENT_NULL_PARAMETER = STR( "Trying to add a null parameter." );

	static const String INFO_UNKNOWN_ERROR = STR( "Unknown error" );

	CDatabaseStatement::SValueUpdater::SValueUpdater( CDatabaseStatement * stmt )
		: _stmt( stmt )
	{
	}

	void CDatabaseStatement::SValueUpdater::Update( const CDatabaseParameter & value )
	{
		_stmt->_mapParamsByPointer[value.GetObjectValue().GetPtrValue()] = &value;
	}

	CDatabaseStatement::CDatabaseStatement( DatabaseConnectionSPtr connection, const String & query )
		: CDatabaseParameteredObject()
		, _connection( connection )
		, _query( query )
	{
		// Empty
	}

	CDatabaseStatement::~CDatabaseStatement()
	{
		// Empty
	}

	bool CDatabaseStatement::ExecuteUpdate()
	{
		if ( !_initialised )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, ERROR_DB_STATEMENT_NOT_INITIALISED );
		}

		bool ret = false;

		try
		{
			ret = DoExecuteUpdate();
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

	DatabaseResultSPtr CDatabaseStatement::ExecuteSelect()
	{
		if ( !_initialised )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, ERROR_DB_STATEMENT_NOT_INITIALISED );
		}

		DatabaseResultSPtr ret;

		try
		{
			ret = DoExecuteSelect();
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

	DatabaseParameterSPtr CDatabaseStatement::CreateParameter( const String & name, EFieldType fieldType, EParameterType parameterType )
	{
		return DoCreateParameter( std::make_shared< CDatabaseValuedObjectInfos >( name, fieldType ), parameterType );
	}

	DatabaseParameterSPtr CDatabaseStatement::CreateParameter( const String & name, EFieldType fieldType, uint32_t limits, EParameterType parameterType )
	{
		return DoCreateParameter( std::make_shared< CDatabaseValuedObjectInfos >( name, fieldType, limits ), parameterType );
	}

	DatabaseParameterSPtr CDatabaseStatement::CreateParameter( const String & name, EFieldType fieldType, const std::pair< uint32_t, uint32_t > & precision, EParameterType parameterType )
	{
		return DoCreateParameter( std::make_shared< CDatabaseValuedObjectInfos >( name, fieldType, precision ), parameterType );
	}
}
END_NAMESPACE_DATABASE
