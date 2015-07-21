/************************************************************************//**
* @file DatabasePostgreSql.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDatabase class declaration.
*
* @details Describes an MYSQL database.
*
***************************************************************************/

#ifndef ___DATABASE_POSTGRESQL_H___
#define ___DATABASE_POSTGRESQL_H___

#include "DatabasePostgreSqlPrerequisites.h"

#include <Database.h>

BEGIN_NAMESPACE_DATABASE_POSTGRESQL
{
	/** Describes an MYSQL database.
	*/
	class CDatabasePostgreSql
		: public CDatabase
	{
	public:
		/** Default constructor.
		*/
		DatabasePostgreSqlExport CDatabasePostgreSql();

		/** Destructor.
		*/
		DatabasePostgreSqlExport virtual ~CDatabasePostgreSql();

		///@copydoc Database::CDatabase::GetType
		DatabasePostgreSqlExport virtual const String & GetType()const;

		/** Creator function
		*/
		DatabasePostgreSqlExport static CDatabase * Create();

	protected:
		/** Create a connection to the database.
		@param[out] connectionString
			Created connection string.
		@return
			Database connection.
		*/
		DatabasePostgreSqlExport virtual DatabaseConnectionPtr DoCreateConnection( String & connectionString ) const;
	};
}
END_NAMESPACE_DATABASE_POSTGRESQL

#endif // ___DATABASE_POSTGRESQL_H___
