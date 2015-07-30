/************************************************************************//**
* @file DatabaseStatementTest.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDatabaseStatementTest class declaration.
*
* @details Describes a statement for tests
*
***************************************************************************/

#include "DatabaseTestPch.h"

#include "DatabaseStatementTest.h"

#include "DatabaseConnectionTest.h"

#include <DatabaseStringUtils.h>

#include <DatabaseRow.h>

BEGIN_NAMESPACE_DATABASE_TEST
{
	extern DatabaseResultSPtr BuildResult( DatabaseConnectionSPtr connection );

	static const String ERROR_TEST_LOST_CONNECTION = STR( "The statement has lost his connection" );

	CDatabaseStatementTest::CDatabaseStatementTest( DatabaseConnectionTestSPtr connection, const String & query )
		: CDatabaseStatement( connection, query )
		, _paramsCount( 0 )
		, _connectionTest( connection )
	{
	}

	CDatabaseStatementTest::~CDatabaseStatementTest()
	{
		Cleanup();
	}

	DatabaseParameterSPtr CDatabaseStatementTest::CreateParameter( const String & name, EFieldType fieldType, EParameterType parameterType )
	{
		DatabaseConnectionTestSPtr connection = DoGetTestConnection();

		if ( !connection )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, ERROR_TEST_LOST_CONNECTION );
		}

		DatabaseParameterSPtr pReturn = std::make_shared< CDatabaseParameter >( connection, name, uint16_t( _arrayParams.size() + 1 ), fieldType, parameterType, std::make_unique< SValueUpdater >( this ) );

		if ( !DoAddParameter( pReturn ) )
		{
			pReturn.reset();
		}

		return pReturn;
	}

	DatabaseParameterSPtr CDatabaseStatementTest::CreateParameter( const String & name, EFieldType fieldType, uint32_t limits, EParameterType parameterType )
	{
		DatabaseConnectionTestSPtr connection = DoGetTestConnection();

		if ( !connection )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, ERROR_TEST_LOST_CONNECTION );
		}

		DatabaseParameterSPtr pReturn = std::make_shared< CDatabaseParameter >( connection, name, uint16_t( _arrayParams.size() + 1 ), fieldType, limits, parameterType, std::make_unique< SValueUpdater >( this ) );

		if ( !DoAddParameter( pReturn ) )
		{
			pReturn.reset();
		}

		return pReturn;
	}

	DatabaseParameterSPtr CDatabaseStatementTest::CreateParameter( const String & name, EFieldType fieldType, const std::pair< uint32_t, uint32_t > & precision, EParameterType parameterType )
	{
		DatabaseConnectionTestSPtr connection = DoGetTestConnection();

		if ( !connection )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, ERROR_TEST_LOST_CONNECTION );
		}

		DatabaseParameterSPtr pReturn = std::make_shared< CDatabaseParameter >( connection, name, uint16_t( _arrayParams.size() + 1 ), fieldType, precision, parameterType, std::make_unique< SValueUpdater >( this ) );

		if ( !DoAddParameter( pReturn ) )
		{
			pReturn.reset();
		}

		return pReturn;
	}

	EErrorType CDatabaseStatementTest::DoInitialize()
	{
		DatabaseConnectionTestSPtr connection = DoGetTestConnection();

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

	bool CDatabaseStatementTest::DoExecuteUpdate()
	{
		DatabaseConnectionTestSPtr connection = DoGetTestConnection();

		if ( !connection )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, ERROR_TEST_LOST_CONNECTION );
		}

		return _query == STR( "TestUpdate" );
	}

	DatabaseResultSPtr CDatabaseStatementTest::DoExecuteSelect()
	{
		DatabaseConnectionTestSPtr connection = DoGetTestConnection();

		if ( !connection )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_StatementError, ERROR_TEST_LOST_CONNECTION );
		}

		return _query == STR( "TestSelect" ) ? BuildResult( DoGetTestConnection() ) : DatabaseResultSPtr();
	}

	void CDatabaseStatementTest::DoCleanup()
	{
		_arrayQueries.clear();
		_paramsCount = 0;
	}
}
END_NAMESPACE_DATABASE_TEST
