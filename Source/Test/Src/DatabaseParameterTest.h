/************************************************************************//**
* @file DatabaseParameterTest.h
* @author Sylvain Doremus
* @version 1.0
* @date 12/02/2014 14:29:35
*
*
* @brief Class testing CDatabaseParameter class
*
***************************************************************************/

#ifndef ___DATABASE_PARAMETER_TEST_H___
#define ___DATABASE_PARAMETER_TEST_H___

#include "DatabaseTestPrerequisites.h"

#include <boost/test/unit_test_suite.hpp>

BEGIN_NAMESPACE_DATABASE_TEST
{
	/** Database unit test class
	*/
	class CDatabaseParameterTest
	{
		/** @name Default constructor / Destructor */
		//!@{
	public:
		/** Default constructor.
		*/
		CDatabaseParameterTest();

		/** Destructor.
		*/
		~CDatabaseParameterTest();
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

		/** Test the basic CDatabaseParameter::GetValue functions
		*/
		void TestCase_ParameterGetValue();

		/** Test the basic CDatabaseParameter::SetValue functions
		*/
		void TestCase_ParameterSetValue();

		/** Test the basic CDatabaseParameter::SetValueFast functions
		*/
		void TestCase_ParameterSetValueFast();

		/** Test the basic CDatabaseParameter::SetValue(CDatabaseValue) functions
		*/
		void TestCase_ParameterSetValueFromValue();

		/** Test the basic CDatabaseParameter::SetValue(CDatabaseValuedObject) functions
		*/
		void TestCase_ParameterSetValueFromValuedObject();

		//!@}
	};
}
END_NAMESPACE_DATABASE_TEST

#endif // ___DATABASE_PARAMETER_TEST_H___
