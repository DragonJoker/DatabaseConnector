/************************************************************************//**
* @file DatabaseTestStatement.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDatabaseTestStatement class declaration.
*
* @details Describes a statement for tests
*
***************************************************************************/

#include "DatabaseTestPch.h"

#if defined( TESTING_INTERFACE )

#include "DatabaseTestStatement.h"

#include "DatabaseTestConnection.h"

#include <DatabaseStringUtils.h>
#include <DatabaseRow.h>

BEGIN_NAMESPACE_DATABASE_TEST
{
	static const String ERROR_TEST_STATEMENT_EMPTY = STR( "The query is empty." );
	static const String ERROR_TEST_STATEMENT_INCONSISTENCY = STR( "Number of parameters doesn't match the sizes of parameter containers." );
	static const String ERROR_TEST_STATEMENT_INVALID = STR( "The query text is invalid." );

	extern DatabaseRowSPtr CreateRow( DatabaseConnectionSPtr connection, const DatabaseValuedObjectInfosPtrArray & fieldInfos );
	extern DatabaseValuedObjectInfosPtrArray CreateFieldsInfos();

	static const String ERROR_TEST_LOST_CONNECTION = STR( "The statement has lost his connection" );

	CDatabaseTestStatement::CDatabaseTestStatement( DatabaseTestConnectionSPtr connection, const String & query )
		: CDatabaseStatement( connection, query )
		, _paramsCount( 0 )
		, _connectionTest( connection )
	{
	}

	CDatabaseTestStatement::~CDatabaseTestStatement()
	{
		Cleanup();
	}

	DatabaseParameterSPtr CDatabaseTestStatement::DoCreateParameter( DatabaseValuedObjectInfosSPtr infos, EParameterType parameterType )
	{
		DatabaseTestConnectionSPtr connection = DoGetTestConnection();

		if ( !connection )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, ERROR_TEST_LOST_CONNECTION );
		}

		DatabaseParameterSPtr pReturn = std::make_shared< CDatabaseParameter >( connection, infos, GetParametersCount() + 1, parameterType, std::make_unique< SValueUpdater >( this ) );

		if ( !DoAddParameter( pReturn ) )
		{
			pReturn.reset();
		}

		return pReturn;
	}

	EErrorType CDatabaseTestStatement::DoInitialise()
	{
		if ( _query.empty() )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, ERROR_TEST_STATEMENT_EMPTY );
		}

		_paramsCount = uint32_t( std::count( _query.begin(), _query.end(), STR( '?' ) ) );

		if ( _paramsCount != GetParametersCount() )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, ERROR_TEST_STATEMENT_INCONSISTENCY );
		}

		_arrayQueries = StringUtils::Split( _query, STR( "?" ), _paramsCount + 1, false );

		if ( _arrayQueries.empty() )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, ERROR_TEST_STATEMENT_INVALID );
		}

		return EErrorType_NONE;
	}

	bool CDatabaseTestStatement::DoExecuteUpdate()
	{
		DatabaseTestConnectionSPtr connection = DoGetTestConnection();

		if ( !connection )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, ERROR_TEST_LOST_CONNECTION );
		}

		return _query == STR( "TestUpdate" );
	}

	DatabaseResultSPtr CDatabaseTestStatement::DoExecuteSelect()
	{
		DatabaseTestConnectionSPtr connection = DoGetTestConnection();

		if ( !connection )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, ERROR_TEST_LOST_CONNECTION );
		}

		DatabaseValuedObjectInfosPtrArray fieldsInfos = CreateFieldsInfos();
		DatabaseResultSPtr result = std::make_shared< CDatabaseResult >( fieldsInfos );
		result->AddRow( CreateRow( connection, fieldsInfos ) );

		return _query == STR( "TestSelect" ) ? result : DatabaseResultSPtr();
	}

	void CDatabaseTestStatement::DoCleanup()
	{
		_arrayQueries.clear();
		_paramsCount = 0;
	}
}
END_NAMESPACE_DATABASE_TEST

#endif
