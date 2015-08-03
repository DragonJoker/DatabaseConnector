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

#include "DatabaseTestStatement.h"

#include "DatabaseTestConnection.h"

#include <DatabaseStringUtils.h>
#include <DatabaseRow.h>

BEGIN_NAMESPACE_DATABASE_TEST
{
	extern DatabaseResultSPtr BuildResult( DatabaseConnectionSPtr connection );

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

		DatabaseParameterSPtr pReturn = std::make_shared< CDatabaseParameter >( connection, infos, uint16_t( _arrayParams.size() + 1 ), parameterType, std::make_unique< SValueUpdater >( this ) );

		if ( !DoAddParameter( pReturn ) )
		{
			pReturn.reset();
		}

		return pReturn;
	}

	EErrorType CDatabaseTestStatement::DoInitialize()
	{
		DatabaseTestConnectionSPtr connection = DoGetTestConnection();

		if ( !connection )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, ERROR_TEST_LOST_CONNECTION );
		}

		EErrorType eReturn = EErrorType_ERROR;

		if ( !_query.empty() )
		{
			_paramsCount = uint32_t( std::count( _query.begin(), _query.end(), STR( '?' ) ) );
			_arrayQueries = StringUtils::Split( _query, STR( "?" ), _paramsCount + 1 );
		}

		BOOST_CHECK_EQUAL( _paramsCount, _arrayParams.size() );
		return eReturn;
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

		return _query == STR( "TestSelect" ) ? BuildResult( DoGetTestConnection() ) : DatabaseResultSPtr();
	}

	void CDatabaseTestStatement::DoCleanup()
	{
		_arrayQueries.clear();
		_paramsCount = 0;
	}
}
END_NAMESPACE_DATABASE_TEST
