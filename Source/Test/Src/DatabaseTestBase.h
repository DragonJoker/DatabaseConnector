/************************************************************************//**
 * @file DatabaseTestBase.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 12/02/2014 14:29:35
 *
 *
 * @brief Base class for database tests
*
***************************************************************************/

#ifndef ___DATABASE_TEST_BASE_H___
#define ___DATABASE_TEST_BASE_H___

#include "DatabaseTestPrerequisites.h"

#include <boost/test/unit_test_suite.hpp>

BEGIN_NAMESPACE_DATABASE_TEST
{
	/** Database unit test class
	*/
	class CDatabaseTest
	{
		/** @name Default constructor / Destructor */
		///@{
	public:
		/** Default constructor.
		*/
		CDatabaseTest( const String & type, const String & server, const String & database, const String & user, const String & password );

		/** Destructor.
		*/
		~CDatabaseTest();
		///@}

	public:
		/** @name Master TS implementation
		*  Required Master TS implementation in TC
		*/
		///@{
		/** @brief  Initialization of the Internal TS
		 @return testSuite Pointer on the TS to be included in the Master TS.
		*/
		boost::unit_test::test_suite * Init_Test_Suite();

	private:
		boost::unit_test::test_suite * testSuite; //!< Instance of the internal TS.
		///@}

	private:
		/** @name TCs' implementation
		*  TCs' implementation
		*/
		///@{
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

#if defined( PERF_TEST )
		/** Performance test through DatabaseStatement
		*/
		void TestCase_DatabaseStatementPerformances();

		/** Performance test through DatabaseQuery
		*/
		void TestCase_DatabaseQueryPerformances();
#endif

		/** Test database destruction
		*/
		void TestCase_DestroyDatabase();
		///@}

	private:
		/** Loads the database plugin
		*/
		virtual void DoLoadPlugins() = 0;

		const String & _type;
		const String & _server;
		const String & _database;
		const String & _user;
		const String & _password;
	};
}
END_NAMESPACE_DATABASE_TEST

#endif
