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

#include "DatabaseQueryParameterOdbc.h"

#include "DatabaseConnectionOdbc.h"
#include "DatabaseOdbcHelper.h"

#include "DatabaseOdbcHelper.h"

BEGIN_NAMESPACE_DATABASE_ODBC
{
	CDatabaseQueryParameterOdbc::CDatabaseQueryParameterOdbc( DatabaseConnectionOdbcPtr connection, const String & name, unsigned short index, EFieldType fieldType, EParameterType parameterType, SValueUpdater * updater )
		: CDatabaseParameter( connection, name, index, fieldType, parameterType, updater )
		, CDatabaseParameterOdbc( index, fieldType, parameterType )
	{
		// Empty
	}

	CDatabaseQueryParameterOdbc::CDatabaseQueryParameterOdbc( DatabaseConnectionOdbcPtr connection, const String & name, unsigned short index, EFieldType fieldType, uint32_t limits, EParameterType parameterType, SValueUpdater * updater )
		: CDatabaseParameter( connection, name, index, fieldType, limits, parameterType, updater )
		, CDatabaseParameterOdbc( index, fieldType, parameterType )
	{
		// Empty
	}

	CDatabaseQueryParameterOdbc::~CDatabaseQueryParameterOdbc()
	{
		// Empty
	}
}
END_NAMESPACE_DATABASE_ODBC
