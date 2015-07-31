/************************************************************************//**
 * @file DatabaseDateTest.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 12/02/2014 14:29:35
 *
 *
 * @brief Class testing Date functions
*
***************************************************************************/

#ifndef ___DATABASE_DATE_TEST_H___
#define ___DATABASE_DATE_TEST_H___

#include "DatabaseTestPrerequisites.h"

#include <boost/test/unit_test_suite.hpp>

BEGIN_NAMESPACE_DATABASE_TEST
{
	/** Database unit test class
	*/
	class CDatabaseDateTest
	{
		/** @name Default constructor / Destructor */
		//!@{
	public:
		/** Default constructor.
		*/
		CDatabaseDateTest();

		/** Destructor.
		*/
		~CDatabaseDateTest();
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

		/** Test Date::Format
		*/
		void TestCase_FormatDate();

		/** Test Date::Print
		*/
		void TestCase_PrintDate();

		/** Test Date::IsDate
		*/
		void TestCase_IsDate();

		/** Test Date::IsDate, filling a date
		*/
		void TestCase_IsDateWithDate();

		//!@}
	};
}
END_NAMESPACE_DATABASE_TEST

#endif // ___DATABASE_INTERFACE_TEST_H___
