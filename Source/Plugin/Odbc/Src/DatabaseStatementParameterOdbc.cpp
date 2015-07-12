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
	static const String ODBC_DescribeParam_MSG = STR( "SQLDescribeParam" );

	CDatabaseStatementParameterOdbc::CDatabaseStatementParameterOdbc( DatabaseConnectionOdbcPtr connection, const String & name, unsigned short index, EFieldType fieldType, EParameterType parameterType, SValueUpdater * updater )
		: CDatabaseParameter( connection, name, index, fieldType, parameterType, updater )
		, CDatabaseParameterOdbc( index, fieldType, parameterType )
	{
		// Empty
	}

	CDatabaseStatementParameterOdbc::CDatabaseStatementParameterOdbc( DatabaseConnectionOdbcPtr connection, const String & name, unsigned short index, EFieldType fieldType, uint32_t limits, EParameterType parameterType, SValueUpdater * updater )
		: CDatabaseParameter( connection, name, index, fieldType, limits, parameterType, updater )
		, CDatabaseParameterOdbc( index, fieldType, parameterType )
	{
		// Empty
	}

	CDatabaseStatementParameterOdbc::~CDatabaseStatementParameterOdbc()
	{
		// Empty
	}

	void CDatabaseStatementParameterOdbc::Initialize( SQLHSTMT statementHandle )
	{
		SQLSMALLINT iDataType = 0;
		SQLSMALLINT iNullable = 0;

		EErrorType errorType;
		SqlTry( SQLDescribeParam( statementHandle, ( SQLUSMALLINT )GetIndex(), &iDataType, &_columnSize, &_decimalDigits, &iNullable ), SQL_HANDLE_STMT, statementHandle, ODBC_DescribeParam_MSG );

		_isNullable = iNullable != 0;
	}
}
END_NAMESPACE_DATABASE_ODBC
