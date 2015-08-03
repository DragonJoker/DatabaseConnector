/************************************************************************//**
* @file DatabaseValuedObjectTest.h
* @author Sylvain Doremus
* @version 1.0
* @date 12/02/2014 14:29:35
*
*
* @brief Class testing CDatabaseValuedObject class
*
***************************************************************************/

#ifndef ___DATABASE_VALUEDOBJECT_TEST_H___
#define ___DATABASE_VALUEDOBJECT_TEST_H___

#include "DatabaseTestPrerequisites.h"

#include <DatabaseValuedObject.h>

#include <boost/test/unit_test_suite.hpp>

BEGIN_NAMESPACE_DATABASE_TEST
{
	template< EFieldType Type >
	class CDatabaseTestValuedObject
		: public CDatabaseValuedObject
	{
	public:
		/** Constructor.
		@param[in] connection
			The database connection.
		@param[in] name
			The value name
		@param[in] limits
			The value limits
		*/
		CDatabaseTestValuedObject( DatabaseConnectionSPtr connection, DatabaseValuedObjectInfosSPtr infos )
			: CDatabaseValuedObject( connection, infos )
		{
			CDatabaseValuedObject::DoCreateValue();
		}

		/** Set parameter value.
		@param[in] value
			New parameter value.
		@remarks
			If field type is different than the value type, the value is ignored.
		*/
		template< typename T > inline void SetValue( const T & value )
		{
			CDatabaseValuedObject::DoSetValue( value );
		}

		/** Set parameter value.
		@param[in] value
			New parameter value.
		*/
		template< typename T > inline void SetValueFast( const T & value )
		{
			CDatabaseValuedObject::DoSetValueFast( value );
		}

		/** Get field value.
		@param[out] value
			Field value.
		*/
		template< typename T > inline void GetValue( T & value ) const
		{
			CDatabaseValuedObject::DoGetValue( value );
		}

		/** Get field value.
		@param[out] value
			Field value.
		*/
		template< typename T > inline void GetValueFast( T & value ) const
		{
			CDatabaseValuedObject::DoGetValueFast( value );
		}
	};

	/** Database unit test class
	*/
	class CDatabaseValuedObjectTest
	{
		/** @name Default constructor / Destructor */
		//!@{
	public:
		/** Default constructor.
		*/
		CDatabaseValuedObjectTest();

		/** Destructor.
		*/
		~CDatabaseValuedObjectTest();
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

		/** Test the basic CDatabaseValuedObject::DoGetValue functions
		*/
		void TestCase_ValuedObjectGetValue();

		/** Test the basic CDatabaseValuedObject::DoGetValueFast functions
		*/
		void TestCase_ValuedObjectGetValueFast();

		/** Test the basic CDatabaseValuedObject::DoSetValue functions
		*/
		void TestCase_ValuedObjectSetValue();

		/** Test the basic CDatabaseValuedObject::DoSetValueFast functions
		*/
		void TestCase_ValuedObjectSetValueFast();

		//!@}
	};
}
END_NAMESPACE_DATABASE_TEST

#endif // ___DATABASE_INTERFACE_TEST_H___
