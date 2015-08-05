/************************************************************************//**
* @file DatabaseValuedObjectInfosTest.h
* @author Sylvain Doremus
* @version 1.0
* @date 12/02/2014 14:29:35
*
*
* @brief Class testing CDatabaseValuedObjectInfos class
*
***************************************************************************/

#ifndef ___DATABASE_FIELDINFOS_TEST_H___
#define ___DATABASE_FIELDINFOS_TEST_H___

#include "DatabaseTestPrerequisites.h"

#include <boost/test/unit_test_suite.hpp>

BEGIN_NAMESPACE_DATABASE_TEST
{
	/** Database unit test class
	*/
	class CDatabaseValuedObjectInfosTest
	{
		/** @name Default constructor / Destructor */
		//!@{
	public:
		/** Default constructor.
		*/
		CDatabaseValuedObjectInfosTest();

		/** Destructor.
		*/
		~CDatabaseValuedObjectInfosTest();
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

		/** Test the CDatabaseValuedObjectInfos functions
		*/
		void TestCase_FieldInfos();

		//!@}
	};
}
END_NAMESPACE_DATABASE_TEST

#endif // ___DATABASE_FIELDINFOS_TEST_H___
