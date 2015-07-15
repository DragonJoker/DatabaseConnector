/************************************************************************//**
 * @file DatabaseOdbcMsSqlTest.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 16/07/2014 12:08:50
 *
 *
 * @brief Database ODBC plugin test
 *
 * @details Database ODBC plugin test
 *
 ***************************************************************************/

#ifndef ___DATABASE_ODBC_MSSQL_TEST_H___
#define ___DATABASE_ODBC_MSSQL_TEST_H___

#include "DatabaseTestBase.h"

BEGIN_NAMESPACE_DATABASE_TEST
{
	/** Database Odbc unit test class
	*/
	class CDatabaseOdbcMsSqlTest
		: public CDatabaseTest
	{
		/** @name Default constructor / Destructor */
		//!@{
	public:
		/** Default constructor.
		*/
		CDatabaseOdbcMsSqlTest();

		/** Destructor.
		*/
		~CDatabaseOdbcMsSqlTest();
		//!@}

	private:
		/** Loads the database plugin
		*/
		void DoLoadPlugins();

		/** Test database creation
		*/
		virtual void TestCase_CreateDatabase();

		/** Test database destruction
		*/
		virtual void TestCase_DestroyDatabase();
	};
}
END_NAMESPACE_DATABASE_TEST

#endif // ___DATABASE_ODBC_MSSQL_TEST_H___
