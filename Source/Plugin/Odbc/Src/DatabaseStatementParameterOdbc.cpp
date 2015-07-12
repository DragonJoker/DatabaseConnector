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
	CDatabaseStatementParameterOdbc::CDatabaseStatementParameterOdbc( DatabaseConnectionOdbcPtr connection, const String & name, unsigned short index, EFieldType fieldType, EParameterType parameterType, SValueUpdater * updater )
		: CDatabaseParameter( connection, name, index, fieldType, parameterType, updater )
		, CDatabaseParameterOdbc()
	{
		// Empty
	}

	CDatabaseStatementParameterOdbc::CDatabaseStatementParameterOdbc( DatabaseConnectionOdbcPtr connection, const String & name, unsigned short index, EFieldType fieldType, uint32_t limits, EParameterType parameterType, SValueUpdater * updater )
		: CDatabaseParameter( connection, name, index, fieldType, limits, parameterType, updater )
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
