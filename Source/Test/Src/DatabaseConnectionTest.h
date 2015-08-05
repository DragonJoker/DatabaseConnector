/************************************************************************//**
 * @file DatabaseConnectionTest.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 12/02/2014 14:29:35
 *
 *
 * @brief Class testing connection common methods
*
***************************************************************************/

#ifndef ___DATABASE_CONNECTION_TEST_H___
#define ___DATABASE_CONNECTION_TEST_H___

#include "DatabaseTestPrerequisites.h"

#include <boost/test/unit_test_suite.hpp>

BEGIN_NAMESPACE_DATABASE_TEST
{
	/** Database unit test class
	*/
	class CDatabaseConnectionTest
	{
		/** @name Default constructor / Destructor */
		//!@{
	public:
		/** Default constructor.
		*/
		CDatabaseConnectionTest();

		/** Destructor.
		*/
		~CDatabaseConnectionTest();
		//!@}

	public:
		/** @name Master TS implementation
		*  Required Master TS implementation in TC
		*/
		//!@{
		/** @brief  Initialization of the Internal TS
		 @return testSuite Pointer on the TS to be included in the Master TS.
		*/
		boost::unit_test::test_suite * Init_Test_Suite();

	private:
		boost::unit_test::test_suite * testSuite; //!< Instance of the internal TS.
		//!@}

	private:
		/** @name TCs' implementation
		*/
		//!@{

		/** Test the connection to server
		*/
		void TestCase_ConnectionChecks();

		/** Test the database creation, selection, and destruction
		*/
		void TestCase_DatabaseManagementChecks();

		/** Test the basic transaction checks
		*/
		void TestCase_TransactionChecks();

		/** Test the basic query and statement checks
		*/
		void TestCase_QueryStatementChecks();

		/** Test CDatabaseConnection::Write*** functions
		*/
		void TestCase_StringDataWrite();

		/** Test CDatabaseConnection::Parse*** functions
		*/
		void TestCase_StringDataParse();

		//!@}
	};
}
END_NAMESPACE_DATABASE_TEST

#endif // ___DATABASE_INTERFACE_TEST_H___
