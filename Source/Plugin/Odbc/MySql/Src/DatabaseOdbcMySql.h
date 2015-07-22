/************************************************************************//**
* @file DatabaseOdbcMySql.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDatabaseOdbc class declaration.
*
* @details Describes an ODBC database.
*
***************************************************************************/

#ifndef ___DATABASE_ODBC_MYSQL_H___
#define ___DATABASE_ODBC_MYSQL_H___

#include "DatabaseOdbcMySqlPrerequisites.h"

#include <Database.h>

BEGIN_NAMESPACE_DATABASE_ODBC_MYSQL
{
	/** Describes an ODBC database.
	*/
	class CDatabaseOdbcMySql
		: public CDatabase
	{
	public:
		/** Default constructor.
			*/
		CDatabaseOdbcMySql();

		/** Destructor.
			*/
		virtual ~CDatabaseOdbcMySql();

		//!@copydoc CObject::GetType
		virtual const String & GetType()const;

		/** Creator function
			*/
		static CDatabase * Create();

	private:
		/** Create a connection to the database.
		@param[out] connectionString
			Created connection string.
		@return
			Database connection.
		*/
		virtual DatabaseConnectionSPtr DoCreateConnection( String & connectionString ) const;

	private:
		//! SQL environment handle.
		SQLHENV _sqlEnvironmentHandle;
	};
}
END_NAMESPACE_DATABASE_ODBC_MYSQL

#endif // ___DATABASE_ODBC_MYSQL_H___
