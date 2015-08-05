/************************************************************************//**
* @file DatabaseFieldTest.h
* @author Sylvain Doremus
* @version 1.0
* @date 12/02/2014 14:29:35
*
*
* @brief Class testing CDatabaseField class
*
***************************************************************************/

#ifndef ___DATABASE_FIELD_TEST_H___
#define ___DATABASE_FIELD_TEST_H___

#include "DatabaseTestPrerequisites.h"

#include <boost/test/unit_test_suite.hpp>

BEGIN_NAMESPACE_DATABASE_TEST
{
	/** Database unit test class
	*/
	class CDatabaseFieldTest
	{
		/** @name Default constructor / Destructor */
		//!@{
	public:
		/** Default constructor.
		*/
		CDatabaseFieldTest();

		/** Destructor.
		*/
		~CDatabaseFieldTest();
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

		/** Test the basic CDatabaseField::GetValue functions
		*/
		void TestCase_FieldGetValue();

		/** Test the basic CDatabaseField::GetValueOpt functions
		*/
		void TestCase_FieldGetValueOpt();

		/** Test the basic CDatabaseField::GetValueFast functions
		*/
		void TestCase_FieldGetValueFast();

		/** Test the basic CDatabaseField::GetValueOptFast functions
		*/
		void TestCase_FieldGetValueOptFast();

		//!@}
	};
}
END_NAMESPACE_DATABASE_TEST

#endif // ___DATABASE_FIELD_TEST_H___
