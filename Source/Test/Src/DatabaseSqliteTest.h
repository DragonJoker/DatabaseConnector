/************************************************************************//**
 * @file DatabaseMySqlTest.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 16/07/2014 12:08:50
 *
 *
 * @brief Database MySQL plugin test
 *
 * @details Database MySQL plugin test
 *
 ***************************************************************************/

#ifndef ___DATABASE_SQLITE_TEST_H___
#define ___DATABASE_SQLITE_TEST_H___

#include "DatabaseTestPrerequisites.h"

#include <boost/test/unit_test_suite.hpp>

BEGIN_NAMESPACE_DATABASE_TEST
{
	/** Database Odbc unit test class
	*/
	class CDatabaseSqliteTest
	{
		/** @name Default constructor / Destructor */
		///@{
	public:
		/** Default constructor.
		*/
		CDatabaseSqliteTest();

		/** Destructor.
		*/
		~CDatabaseSqliteTest();
		///@}

		/** @name Master TS implementation
		*  Required Master TS implementation in TC
		*/
		///@{
	public:
		/** @brief  Initialization of the Internal TS
		 @return testSuite Pointer on the TS to be included in the Master TS.
		*/
		boost::unit_test::test_suite * Init_Test_Suite();

	private:
		boost::unit_test::test_suite * testSuite; //!< Instance of the internal TS.
		///@}

		/** @name TCs' implementation
		*  TCs' implementation
		*/
		///@{
	private:
		/** Test database creation
		*/
		void TestCase_CreateDatabase();

		/** Test direct query execution through DatabaseQuery
		*/
		void TestCase_DatabaseQueryDirectQuery();

		/** Test stored procedures execution through DatabaseQuery
		*/
		void TestCase_DatabaseQueryStoredProcedure();

		/** Test direct query execution through DatabaseStatement
		*/
		void TestCase_DatabaseStatementDirectQuery();

		/** Test stored procedures execution through DatabaseStatement
		*/
		void TestCase_DatabaseStatementStoredProcedure();

		/** Test database destruction
		*/
		void TestCase_DestroyDatabase();

		///@}
	public:
	};
}
END_NAMESPACE_DATABASE_TEST

#endif // ___DATABASE_SQLITE_TEST_H___
