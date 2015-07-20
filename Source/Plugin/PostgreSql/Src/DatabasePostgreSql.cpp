/************************************************************************//**
* @file DatabasePostgreSql.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDatabasePostgreSql class declaration.
*
* @details Describes an MYSQL database.
*
***************************************************************************/

#include "DatabasePostgreSqlPch.h"

#include "DatabasePostgreSql.h"
#include "DatabaseConnectionPostgreSql.h"

BEGIN_NAMESPACE_DATABASE_POSTGRESQL
{
	static const String ERROR_POSTGRESQL_INITIALISATION = STR( "Couldn't initialise PostgreSQL" );

	CDatabasePostgreSql::CDatabasePostgreSql()
		: CDatabase()
	{
	}

	CDatabasePostgreSql::~CDatabasePostgreSql()
	{
	}

	const String & CDatabasePostgreSql::GetType()const
	{
		return DATABASE_POSTGRESQL_TYPE;
	}

	CDatabase * CDatabasePostgreSql::Create()
	{
		return new CDatabasePostgreSql;
	}

	DatabaseConnectionPtr CDatabasePostgreSql::DoCreateConnection( String & connectionString ) const
	{
		DatabaseConnectionPtr pReturn;

		// std::make_shared limited to 5 parameters with VS2012
		pReturn = std::make_shared< CDatabaseConnectionPostgreSql >( _server, _userName, _password, connectionString );

		return pReturn;
	}
}
END_NAMESPACE_DATABASE_POSTGRESQL
