/************************************************************************//**
* @file DatabaseMySql.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDatabaseMySql class declaration.
*
* @details Describes an MYSQL database.
*
***************************************************************************/

#include "DatabaseMySqlPch.h"

#include "DatabaseMySql.h"
#include "DatabaseConnectionMySql.h"

#include <mysql/mysql.h>

#undef min
#undef max
#undef abs

BEGIN_NAMESPACE_DATABASE_MYSQL
{
	static const String ERROR_MYSQL_INITIALISATION = STR( "Couldn't initialise MySQL" );

	CDatabaseMySql::CDatabaseMySql()
		: CDatabase()
	{
		if ( mysql_library_init( 0, NULL, NULL ) )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_MYSQL_INITIALISATION );
		}
	}

	CDatabaseMySql::~CDatabaseMySql()
	{
		mysql_library_end();
	}

	const String & CDatabaseMySql::GetType()const
	{
		return DATABASE_MYSQL_TYPE;
	}

	CDatabase * CDatabaseMySql::Create()
	{
		return new CDatabaseMySql;
	}

	DatabaseConnectionSPtr CDatabaseMySql::DoCreateConnection( String & connectionString ) const
	{
		return std::make_shared< CDatabaseConnectionMySql >( _server, _userName, _password, connectionString );
	}
}
END_NAMESPACE_DATABASE_MYSQL
