/************************************************************************//**
 * @file DatabaseTestBase.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 12/02/2014 14:29:35
 *
 *
 * @brief Base class for database tests
*
***************************************************************************/

#ifndef ___DATABASE_TEST_BASE_H___
#define ___DATABASE_TEST_BASE_H___

#include "DatabaseTestPrerequisites.h"

#include <boost/test/unit_test_suite.hpp>

BEGIN_NAMESPACE_DATABASE_TEST
{
	struct STestConfig
	{
		bool hasNChar;
		bool hasSeparateBooleanAndTinyInt;
		const String & is;
		bool hasInt24;
		bool hasTinyInt;
		bool hasUnsignedTiny;
		bool hasDecentFloat32;
	};

	/** Database unit test class
	*/
	class CDatabaseTest
	{
		/** @name Default constructor / Destructor */
		//!@{
	public:
		/** Default constructor.
		*/
		CDatabaseTest( const String & type, const String & server, const String & database, const String & user, const String & password, const STestConfig & config );

		/** Destructor.
		*/
		~CDatabaseTest();
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
		*  TCs' implementation
		*/

		/** Test insertion and retrieval of a value
		*/
		template< size_t StmtType >
		void TestCase_DatabaseFieldsInsertRetrieve();

		/** Test insertion and retrieval of a value
		*/
		template< size_t StmtType >
		void TestCase_DatabaseFieldsInsertRetrieveOtherIndex();

		/** Test insertion and retrieval of a value
		*/
		template< size_t StmtType >
		void TestCase_DatabaseFieldsInsertRetrieveFast();

		/** Test insertion and retrieval of a value
		*/
		template< size_t StmtType >
		void TestCase_DatabaseFieldsInsertRetrieveFastOtherIndex();

		/** Test direct query execution
		*/
		template< size_t StmtType >
		void TestCase_DatabaseDirectQuery();

		/** Test stored procedures execution
		*/
		template< size_t StmtType >
		void TestCase_DatabaseStoredProcedure();

		/** Applies two functions in separate threads
		*/
		template< typename Action1, typename Action2 >
		void MultithreadActions();

		/** Multithread test: one inserts, one inserts
		*/
		template< size_t StmtTypeA, size_t StmtTypeB >
		void TestCase_MultithreadInsertAndInsert();

		/** Multithread test: one inserts, one updates
		*/
		template< size_t StmtTypeA, size_t StmtTypeB >
		void TestCase_MultithreadInsertAndUpdate();

		/** Multithread test: one inserts, one selects
		*/
		template< size_t StmtTypeA, size_t StmtTypeB >
		void TestCase_MultithreadInsertAndSelect();

		/** Multithread test: one inserts, one deletes
		*/
		template< size_t StmtTypeA, size_t StmtTypeB >
		void TestCase_MultithreadInsertAndDelete();

		/** Multithread test: one updates, one updates
		*/
		template< size_t StmtTypeA, size_t StmtTypeB >
		void TestCase_MultithreadUpdateAndUpdate();

		/** Multithread test: one updates, one selects
		*/
		template< size_t StmtTypeA, size_t StmtTypeB >
		void TestCase_MultithreadUpdateAndSelect();

		/** Multithread test: one updates, one deletes
		*/
		template< size_t StmtTypeA, size_t StmtTypeB >
		void TestCase_MultithreadUpdateAndDelete();

		/** Multithread test: one selects, one selects
		*/
		template< size_t StmtTypeA, size_t StmtTypeB >
		void TestCase_MultithreadSelectAndSelect();

		/** Multithread test: one selects, one deletes
		*/
		template< size_t StmtTypeA, size_t StmtTypeB >
		void TestCase_MultithreadSelectAndDelete();

#if defined( PERF_TEST )
		/** Performance test
		*/
		template< size_t StmtType >
		void TestCase_DatabasePerformances();
#endif

		/** Test database creation
		*/
		virtual void TestCase_CreateDatabase();

		/** Test database destruction
		*/
		virtual void TestCase_DestroyDatabase();
		//!@}

	private:
		/** Loads the database plugin
		*/
		virtual void DoLoadPlugins() = 0;

		/** Flushes the Test table content
		*/
		void DoFlushTable( DatabaseConnectionSPtr connection );

	protected:
		String _createTable;
		const String _type;
		const String _server;
		const String _database;
		const String _user;
		const String _password;
		STestConfig _config;
	};
}
END_NAMESPACE_DATABASE_TEST

#include "DatabaseTestBase.inl"

#endif
