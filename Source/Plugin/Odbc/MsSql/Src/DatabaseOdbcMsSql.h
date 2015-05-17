/************************************************************************//**
 * @file DatabaseOdbcMsSql.h
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

#ifndef ___DATABASE_ODBC_MSSQL_H___
#define ___DATABASE_ODBC_MSSQL_H___

#include "DatabaseOdbcMsSqlPrerequisites.h"

#include <Database.h>

BEGIN_NAMESPACE_DATABASE_ODBC_MSSQL
{
	/** Describes an ODBC database.
	*/
	class DatabaseOdbcMsSqlExport CDatabaseOdbcMsSql
		: public CDatabase
	{
	public:

		/** Default constructor.
		 */
		CDatabaseOdbcMsSql();

		/** Destructor.
		 */
		virtual ~CDatabaseOdbcMsSql();

		///@copydoc Areva::ARIA::Native::CObject::GetType
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
		virtual DatabaseConnectionPtr DoCreateConnection( String & connectionString ) const;

	private:
		/// SQL environment handle.
		SQLHENV _sqlEnvironmentHandle;

	};
}
END_NAMESPACE_DATABASE_ODBC_MSSQL

#endif // ___DATABASE_ODBC_MSSQL_H___
