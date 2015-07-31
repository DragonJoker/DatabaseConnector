/************************************************************************//**
* @file DatabaseTimeTest.h
* @author Sylvain Doremus
* @version 1.0
* @date 12/02/2014 14:29:35
*
*
* @brief Class testing Time functions
*
***************************************************************************/

#ifndef ___DATABASE_TIME_TEST_H___
#define ___DATABASE_TIME_TEST_H___

#include "DatabaseTestPrerequisites.h"

#include <boost/test/unit_test_suite.hpp>

BEGIN_NAMESPACE_DATABASE_TEST
{
	/** Database unit test class
	*/
	class CDatabaseTimeTest
	{
		/** @name Default constructor / Destructor */
		//!@{
	public:
		/** Default constructor.
		*/
		CDatabaseTimeTest();

		/** Destructor.
		*/
		~CDatabaseTimeTest();
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

		/** Test Time::Format
		*/
		void TestCase_FormatTime();

		/** Test Time::Print
		*/
		void TestCase_PrintTime();

		/** Test Time::IsTime
		*/
		void TestCase_IsTime();

		/** Test Time::IsTime, filling a time
		*/
		void TestCase_IsTimeWithTime();

		//!@}
	};
}
END_NAMESPACE_DATABASE_TEST

#endif // ___DATABASE_TIME_TEST_H___
