/************************************************************************//**
 * @file DatabaseMySqlTest.h
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

#ifndef ___DATABASE_MYSQL_TEST_H___
#define ___DATABASE_MYSQL_TEST_H___

#include "DatabaseTestPrerequisites.h"

#include <boost/test/unit_test_suite.hpp>

BEGIN_NAMESPACE_DATABASE_TEST
{
    /** Database Odbc unit test class
    */
    class CDatabaseMySqlTest
    {
        /** @name Default constructor / Destructor */
        ///@{
    public:
        /** Default constructor.
        */
        CDatabaseMySqlTest();
        
        /** Destructor.
        */
        ~CDatabaseMySqlTest();
        ///@}
        
        /** @name Master TS implementation
        *  Required Master TS implementation in TC
        */
        ///@{
    public:
        /** @brief  Initialization of the Internal TS
            @return testSuite Pointer on the TS to be included in the Master TS.
        */
        boost::unit_test::test_suite * Init_Test_Suite();
        
    private:
        boost::unit_test::test_suite * testSuite; //!< Instance of the internal TS.
        ///@}
        
        /** @name TCs' implementation
        *  TCs' implementation
        */
        ///@{
    private:
        /** Test direct query execution through DatabaseQuery
        */
        void TestCase_DatabaseQueryDirectQuery();
        
        /** Test stored procedures execution through DatabaseQuery
        */
        void TestCase_DatabaseQueryStoredProcedure();
        
        /** Test direct query execution through DatabaseStatement
        */
        void TestCase_DatabaseStatementDirectQuery();
        
        /** Test stored procedures execution through DatabaseStatement
        */
        void TestCase_DatabaseStatementStoredProcedure();
        
#if defined( PERF_TEST )
        /** Performance test through DatabaseStatement
        */
        void TestCase_DatabaseStatementPerformances();
        
        /** Performance test through DatabaseQuery
        */
        void TestCase_DatabaseQueryPerformances();
#endif
        ///@}
    public:
    };
}
END_NAMESPACE_DATABASE_TEST

#endif // ___DATABASE_MYSQL_TEST_H___
