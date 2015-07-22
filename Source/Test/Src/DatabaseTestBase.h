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
		void TestCase_DatabaseFieldsInsertRetrieve( const String & name );

		/** Test insertion and retrieval of a value
		*/
		template< size_t StmtType >
		void TestCase_DatabaseFieldsInsertRetrieveOtherIndex( const String & name );

		/** Test insertion and retrieval of a value
		*/
		template< size_t StmtType >
		void TestCase_DatabaseFieldsInsertRetrieveFast( const String & name );

		/** Test insertion and retrieval of a value
		*/
		template< size_t StmtType >
		void TestCase_DatabaseFieldsInsertRetrieveFastOtherIndex( const String & name );

		/** Test direct query execution
		*/
		template< size_t StmtType >
		void TestCase_DatabaseDirectQuery( const String & name );

		/** Test stored procedures execution
		*/
		template< size_t StmtType >
		void TestCase_DatabaseStoredProcedure( const String & name );

		/** Applies two functions in separate threads
		*/
		template< typename Action1, typename Action2 >
		void MultithreadActions( Action1 action1, Action2 action2 );

		/** Multithread test: one inserts, one inserts
		*/
		template< typename StmtTypeA, typename StmtTypeB >
		void TestCase_MultithreadInsertAndInsert( const String & nameA, const String & nameB );

		/** Multithread test: one inserts, one updates
		*/
		template< typename StmtTypeA, typename StmtTypeB >
		void TestCase_MultithreadInsertAndUpdate( const String & nameA, const String & nameB );

		/** Multithread test: one inserts, one selects
		*/
		template< typename StmtTypeA, typename StmtTypeB >
		void TestCase_MultithreadInsertAndSelect( const String & nameA, const String & nameB );

		/** Multithread test: one inserts, one deletes
		*/
		template< typename StmtTypeA, typename StmtTypeB >
		void TestCase_MultithreadInsertAndDelete( const String & nameA, const String & nameB );

		/** Multithread test: one updates, one updates
		*/
		template< typename StmtTypeA, typename StmtTypeB >
		void TestCase_MultithreadUpdateAndUpdate( const String & nameA, const String & nameB );

		/** Multithread test: one updates, one selects
		*/
		template< typename StmtTypeA, typename StmtTypeB >
		void TestCase_MultithreadUpdateAndSelect( const String & nameA, const String & nameB );

		/** Multithread test: one updates, one deletes
		*/
		template< typename StmtTypeA, typename StmtTypeB >
		void TestCase_MultithreadUpdateAndDelete( const String & nameA, const String & nameB );

		/** Multithread test: one selects, one selects
		*/
		template< typename StmtTypeA, typename StmtTypeB >
		void TestCase_MultithreadSelectAndSelect( const String & nameA, const String & nameB );

		/** Multithread test: one selects, one deletes
		*/
		template< typename StmtTypeA, typename StmtTypeB >
		void TestCase_MultithreadSelectAndDelete( const String & nameA, const String & nameB );

#if defined( PERF_TEST )
		/** Performance test
		*/
		template< size_t StmtType >
		void TestCase_DatabasePerformances( const String & name );
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
