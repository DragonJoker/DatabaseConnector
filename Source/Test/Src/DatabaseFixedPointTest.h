/************************************************************************//**
 * @file DatabaseFixedPointTest.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 12/02/2014 14:29:35
 *
 *
 * @brief Class testing CFixedPoint class
*
***************************************************************************/

#ifndef ___DATABASE_FIXEDPOINT_TEST_H___
#define ___DATABASE_FIXEDPOINT_TEST_H___

#include "DatabaseTestPrerequisites.h"

#include <boost/test/unit_test_suite.hpp>

BEGIN_NAMESPACE_DATABASE_TEST
{
	/** Database unit test class
	*/
	class CDatabaseFixedPointTest
	{
		/** @name Default constructor / Destructor */
		//!@{
	public:
		/** Default constructor.
		*/
		CDatabaseFixedPointTest();

		/** Destructor.
		*/
		~CDatabaseFixedPointTest();
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

		/** Test the construction of invalid CFixedPoint, by invalid precision and/or decimals
		*/
		void TestCase_FixedPointOutOfRange();

		/** Test the construction of CFixedPoint with minimal precision
		*/
		void TestCase_FixedPointMinPrecision();

		/** Test the construction of CFixedPoint with maximal precision and minimal decimals
		*/
		void TestCase_FixedPointMaxPrecisionMinDecimals();

		/** Test the construction of CFixedPoint with maximal precision and maximal decimals
		*/
		void TestCase_FixedPointMaxPrecisionMaxDecimals();

		/** Test the construction of CFixedPoint with medium precision and medium decimals
		*/
		void TestCase_FixedPointMedPrecisionMedDecimals();

		/** Test the construction of CFixedPoint from supported types
		*/
		void TestCase_FixedPointConstruction();

		/** Test CFixedPoint addition
		*/
		void TestCase_FixedPointAddition();

		/** Test CFixedPoint subtraction
		*/
		void TestCase_FixedPointSubtraction();

		/** Test CFixedPoint multiplication
		*/
		void TestCase_FixedPointMultiplication();

		/** Test CFixedPoint division
		*/
		void TestCase_FixedPointDivision();

		//!@}
	};
}
END_NAMESPACE_DATABASE_TEST

#endif // ___DATABASE_FIXEDPOINT_TEST_H___
