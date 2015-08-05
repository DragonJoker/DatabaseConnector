/************************************************************************//**
* @file DatabaseUInt24Test.h
* @author Sylvain Doremus
* @version 1.0
* @date 12/02/2014 14:29:35
*
*
* @brief Class testing uint24_t type
*
***************************************************************************/

#ifndef ___DATABASE_UINT24_TEST_H___
#define ___DATABASE_UINT24_TEST_H___

#include "DatabaseTestPrerequisites.h"

#include <boost/test/unit_test_suite.hpp>

BEGIN_NAMESPACE_DATABASE_TEST
{
	/** Database unit test class
	*/
	class CDatabaseUInt24Test
	{
		/** @name Default constructor / Destructor */
		//!@{
	public:
		/** Default constructor.
		*/
		CDatabaseUInt24Test();

		/** Destructor.
		*/
		~CDatabaseUInt24Test();
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

		/** Test uint24_t construction
		*/
		void TestCase_UInt24Construction();

		/** Test uint24_t addition
		*/
		void TestCase_UInt24Addition();

		/** Test uint24_t subtraction
		*/
		void TestCase_UInt24Subtraction();

		/** Test uint24_t multiplication
		*/
		void TestCase_UInt24Multiplication();

		/** Test uint24_t division
		*/
		void TestCase_UInt24Division();

		/** Test uint24_t left shift
		*/
		void TestCase_UInt24LeftShift();

		/** Test uint24_t right shift
		*/
		void TestCase_UInt24RightShift();

		//!@}
	};
}
END_NAMESPACE_DATABASE_TEST

#endif // ___DATABASE_UINT24_TEST_H___
