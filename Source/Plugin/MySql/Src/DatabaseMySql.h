/************************************************************************//**
* @file DatabaseMySql.h
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

#ifndef ___DATABASE_MYSQL_H___
#define ___DATABASE_MYSQL_H___

#include "DatabaseMySqlPrerequisites.h"

#include <Database.h>

#include <mysql/mysql.h>

BEGIN_NAMESPACE_DATABASE_MYSQL
{
	/** Describes an MYSQL database.
	*/
	class CDatabaseMySql
		: public CDatabase
	{
	public:
		/** Default constructor.
		*/
		CDatabaseMySql();

		/** Destructor.
		*/
		virtual ~CDatabaseMySql();

		///@copydoc Database::CDatabase::GetType
		virtual const String & GetType()const;

		/** Creator function
		*/
		static CDatabase * Create();

	protected:
		/** Create a connection to the database.
		@param[out] connectionString
			Created connection string.
		@return
			Database connection.
		*/
		virtual DatabaseConnectionSPtr DoCreateConnection( String & connectionString ) const;
	};
}
END_NAMESPACE_DATABASE_MYSQL

#endif // ___DATABASE_MYSQL_H___
