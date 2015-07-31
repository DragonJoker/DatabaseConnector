/************************************************************************//**
* @file DatabaseInterfaceTest.h
* @author Sylvain Doremus
* @version 1.0
* @date 12/02/2014 14:29:35
*
*
* @brief Class testing all Interface members
*
***************************************************************************/

#ifndef ___DATABASE_INTERFACE_TEST_H___
#define ___DATABASE_INTERFACE_TEST_H___

#include "DatabaseTestPrerequisites.h"

#include <boost/test/unit_test_suite.hpp>

BEGIN_NAMESPACE_DATABASE_TEST
{
	/** Database unit test class
	*/
	class CDatabaseInterfaceTest
	{
		/** @name Default constructor / Destructor */
		//!@{
	public:
		/** Default constructor.
		*/
		CDatabaseInterfaceTest();

		/** Destructor.
		*/
		~CDatabaseInterfaceTest();
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

		/** Test the basic CDatabaseValuedObject functions
		*/
		void TestCase_DatabaseValuedObject();

		/** Test the basic CDatabaseParameter functions
		*/
		void TestCase_DatabaseParameter();

		/** Test the basic CDatabaseField functions
		*/
		void TestCase_DatabaseField();

		/** Test the basic CDatabaseRow functions
		*/
		void TestCase_DatabaseRow();

		/** Test the basic CDatabaseRow functions
		*/
		void TestCase_DatabaseResult();

		/** Test the basic CDatabaseQuery functions
		*/
		void TestCase_DatabaseQuery();

		/** Test the basic CDatabaseStatement functions
		*/
		void TestCase_DatabaseStatement();

		//!@}
	};
}
END_NAMESPACE_DATABASE_TEST

#endif // ___DATABASE_INTERFACE_TEST_H___
