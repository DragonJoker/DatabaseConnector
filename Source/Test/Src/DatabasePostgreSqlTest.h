/************************************************************************//**
 * @file DatabasePostgreSqlTest.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 16/07/2014 12:08:50
 *
 *
 * @brief Database MySQL plugin test
 *
 * @details Database MySQL plugin test
 *
 ***************************************************************************/

#ifndef ___DATABASE_POSTGRESQL_TEST_H___
#define ___DATABASE_POSTGRESQL_TEST_H___

#include "DatabaseTestBase.h"

BEGIN_NAMESPACE_DATABASE_TEST
{
	/** Database PostgreSQL unit test class
	*/
	class CDatabasePostgreSqlTest
		: public CDatabaseTest
	{
		/** @name Default constructor / Destructor */
		//!@{
	public:
		/** Default constructor.
		*/
		CDatabasePostgreSqlTest();

		/** Destructor.
		*/
		~CDatabasePostgreSqlTest();
		//!@}

	private:
		/** Loads the database plugin
		*/
		void DoLoadPlugins();
	};
}
END_NAMESPACE_DATABASE_TEST

#endif // ___DATABASE_POSTGRESQL_TEST_H___
