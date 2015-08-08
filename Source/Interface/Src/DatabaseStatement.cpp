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

#if defined( _X64 )
	static const TChar * INFO_DB_CREATING_STATEMENT = STR( "Creating statement 0x%016X, with query %s" );
	static const TChar * INFO_DB_DELETING_STATEMENT = STR( "Deleting statement 0x%016X" );
	static const TChar * INFO_DB_EXECUTING_UPDATE_STATEMENT = STR( "Executing Update on statement 0x%016X" );
	static const TChar * INFO_DB_EXECUTING_SELECT_STATEMENT = STR( "Executing Select on statement 0x%016X" );
#else
	static const TChar * INFO_DB_CREATING_STATEMENT = STR( "Creating statement 0x%08X, with query %s" );
	static const TChar * INFO_DB_DELETING_STATEMENT = STR( "Deleting statement 0x%08X" );
	static const TChar * INFO_DB_EXECUTING_UPDATE_STATEMENT = STR( "Executing Update on statement 0x%08X" );
	static const TChar * INFO_DB_EXECUTING_SELECT_STATEMENT = STR( "Executing Select on statement 0x%08X" );
#endif

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
		CLogger::LogInfo( INFO_DB_CREATING_STATEMENT, this, query.c_str() );
	}

	CDatabaseStatement::~CDatabaseStatement()
	{
		CLogger::LogInfo( INFO_DB_DELETING_STATEMENT, this );
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
			CLogger::LogInfo( INFO_DB_EXECUTING_UPDATE_STATEMENT, this );
			ret = DoExecuteUpdate();
		}
		COMMON_CATCH( ERROR_STATEMENT_EXECUTION )

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
			CLogger::LogInfo( INFO_DB_EXECUTING_SELECT_STATEMENT, this );
			ret = DoExecuteSelect();
		}
		COMMON_CATCH( ERROR_STATEMENT_EXECUTION )

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
