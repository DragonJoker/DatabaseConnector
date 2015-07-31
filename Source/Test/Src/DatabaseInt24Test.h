/************************************************************************//**
* @file DatabaseInt24Test.h
* @author Sylvain Doremus
* @version 1.0
* @date 12/02/2014 14:29:35
*
*
* @brief Class testing int24_t type
*
***************************************************************************/

#ifndef ___DATABASE_INT24_TEST_H___
#define ___DATABASE_INT24_TEST_H___

#include "DatabaseTestPrerequisites.h"

#include <boost/test/unit_test_suite.hpp>

BEGIN_NAMESPACE_DATABASE_TEST
{
	/** Database unit test class
	*/
	class CDatabaseInt24Test
	{
		/** @name Default constructor / Destructor */
		//!@{
	public:
		/** Default constructor.
		*/
		CDatabaseInt24Test();

		/** Destructor.
		*/
		~CDatabaseInt24Test();
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

		/** Test int24_t construction
		*/
		void TestCase_Int24Construction();

		/** Test int24_t addition
		*/
		void TestCase_Int24Addition();

		/** Test int24_t subtraction
		*/
		void TestCase_Int24Subtraction();

		/** Test int24_t multiplication
		*/
		void TestCase_Int24Multiplication();

		/** Test int24_t division
		*/
		void TestCase_Int24Division();

		/** Test int24_t left shift
		*/
		void TestCase_Int24LeftShift();

		/** Test int24_t right shift
		*/
		void TestCase_Int24RightShift();

		//!@}
	};
}
END_NAMESPACE_DATABASE_TEST

#endif // ___DATABASE_INT24_TEST_H___
