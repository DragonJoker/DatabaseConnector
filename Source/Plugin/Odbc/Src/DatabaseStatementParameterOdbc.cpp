/************************************************************************//**
 * @file DatabaseStatementParameterOdbc.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 *
 *
 * @brief CDatabaseStatementParameterOdbc class declaration.
 *
 * @details Describes a statement parameter for ODBC database.
 *
 ***************************************************************************/

#include "DatabaseOdbcPch.h"

#include "DatabaseStatementParameterOdbc.h"

#include "DatabaseConnectionOdbc.h"
#include "DatabaseOdbcHelper.h"

#include "DatabaseOdbcHelper.h"

BEGIN_NAMESPACE_DATABASE_ODBC
{
	CDatabaseStatementParameterOdbc::CDatabaseStatementParameterOdbc( DatabaseConnectionOdbcSPtr connection, const String & name, unsigned short index, EFieldType fieldType, EParameterType parameterType, std::unique_ptr< SValueUpdater > updater )
		: CDatabaseParameter( connection, name, index, fieldType, parameterType, std::move( updater ) )
		, CDatabaseParameterOdbc()
	{
		// Empty
	}

	CDatabaseStatementParameterOdbc::CDatabaseStatementParameterOdbc( DatabaseConnectionOdbcSPtr connection, const String & name, unsigned short index, EFieldType fieldType, uint32_t limits, EParameterType parameterType, std::unique_ptr< SValueUpdater > updater )
		: CDatabaseParameter( connection, name, index, fieldType, limits, parameterType, std::move( updater ) )
		, CDatabaseParameterOdbc()
	{
		// Empty
	}

	CDatabaseStatementParameterOdbc::CDatabaseStatementParameterOdbc( DatabaseConnectionOdbcSPtr connection, const String & name, unsigned short index, EFieldType fieldType, const std::pair< uint32_t, uint32_t > & precision, EParameterType parameterType, std::unique_ptr< SValueUpdater > updater )
		: CDatabaseParameter( connection, name, index, fieldType, precision, parameterType, std::move( updater ) )
		, CDatabaseParameterOdbc()
	{
		// Empty
	}

	CDatabaseStatementParameterOdbc::~CDatabaseStatementParameterOdbc()
	{
		// Empty
	}

	void CDatabaseStatementParameterOdbc::Initialize( SQLHSTMT statementHandle )
	{
		CDatabaseParameterOdbc::Initialize( statementHandle, *this );
		GetBinding().Initialize();
	}
}
END_NAMESPACE_DATABASE_ODBC
