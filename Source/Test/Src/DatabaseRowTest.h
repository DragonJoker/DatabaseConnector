/************************************************************************//**
* @file DatabaseRowTest.h
* @author Sylvain Doremus
* @version 1.0
* @date 12/02/2014 14:29:35
*
*
* @brief Class testing CDatabaseRow class
*
***************************************************************************/

#ifndef ___DATABASE_ROW_TEST_H___
#define ___DATABASE_ROW_TEST_H___

#include "DatabaseTestPrerequisites.h"

#include <boost/test/unit_test_suite.hpp>

BEGIN_NAMESPACE_DATABASE_TEST
{
	/** Database unit test class
	*/
	class CDatabaseRowTest
	{
		/** @name Default constructor / Destructor */
		//!@{
	public:
		/** Default constructor.
		*/
		CDatabaseRowTest();

		/** Destructor.
		*/
		~CDatabaseRowTest();
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

		/** Test AddField and GetField functions
		*/
		void TestCase_RowFieldManagement();

		/** Test the basic CDatabaseRow::Get functions
		*/
		void TestCase_RowGet();

		/** Test the basic CDatabaseRow::GetOpt functions
		*/
		void TestCase_RowGetOpt();

		/** Test the basic CDatabaseRow::GetFast functions
		*/
		void TestCase_RowGetFast();

		/** Test the basic CDatabaseRow::GetOptFast functions
		*/
		void TestCase_RowGetOptFast();

		//!@}
	};
}
END_NAMESPACE_DATABASE_TEST

#endif // ___DATABASE_ROW_TEST_H___
