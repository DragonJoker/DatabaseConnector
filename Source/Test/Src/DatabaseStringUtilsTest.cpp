/************************************************************************//**
 * @file DatabaseStringUtilsTest.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 12/02/2014 14:29:35
 *
 *
 * @brief Class testing StringUtils functions
*
***************************************************************************/

#include "DatabaseTestPch.h"

#include "DatabaseStringUtilsTest.h"

#include "DatabaseTestHelpers.h"

namespace std
{
	inline ostream & operator <<( ostream & out, const wstring & value )
	{
		out << NAMESPACE_DATABASE::StringUtils::ToStr( value );
		return out;
	}
}

BEGIN_NAMESPACE_DATABASE_TEST
{
	CDatabaseStringUtilsTest::CDatabaseStringUtilsTest()
	{
	}

	CDatabaseStringUtilsTest::~CDatabaseStringUtilsTest()
	{
	}

	boost::unit_test::test_suite * CDatabaseStringUtilsTest::Init_Test_Suite()
	{
		//!@remarks Create the internal TS instance.
		testSuite = new boost::unit_test::test_suite( "CDatabaseStringUtilsTest" );

		//!@remarks Add the TC to the internal TS.
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseStringUtilsTest::TestCase_StringUtilsIsUpperIsLower, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseStringUtilsTest::TestCase_StringUtilsUpperLower, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseStringUtilsTest::TestCase_StringUtilsToUpperToLower, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseStringUtilsTest::TestCase_StringUtilsConversions, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseStringUtilsTest::TestCase_StringUtilsSplit, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseStringUtilsTest::TestCase_StringUtilsTrim, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseStringUtilsTest::TestCase_StringUtilsReplace, this ) ) );
		testSuite->add( BOOST_TEST_CASE( std::bind( &CDatabaseStringUtilsTest::TestCase_StringUtilsFormalize, this ) ) );

		//!@remarks Return the TS instance.
		return testSuite;
	}

	void CDatabaseStringUtilsTest::TestCase_StringUtilsIsUpperIsLower()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_StringUtilsIsUpperIsLower ****" );

		BOOST_CHECK( !StringUtils::IsUpperCase( "NoTuPpErCaSe" ) );
		BOOST_CHECK( StringUtils::IsUpperCase( "UPPERCASE" ) );
		BOOST_CHECK( !StringUtils::IsLowerCase( "NoTlOwErCaSe" ) );
		BOOST_CHECK( StringUtils::IsLowerCase( "lowercase" ) );
		BOOST_CHECK( !StringUtils::IsUpperCase( L"NoTuPpErCaSe" ) );
		BOOST_CHECK( StringUtils::IsUpperCase( L"UPPERCASE" ) );
		BOOST_CHECK( !StringUtils::IsLowerCase( L"NoTlOwErCaSe" ) );
		BOOST_CHECK( StringUtils::IsLowerCase( L"lowercase" ) );

		CLogger::LogInfo( StringStream() << "**** End TestCase_StringUtilsIsUpperIsLower ****" );
	}

	void CDatabaseStringUtilsTest::TestCase_StringUtilsUpperLower()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_DatabaseConnection ****" );

		BOOST_CHECK_EQUAL( StringUtils::UpperCase( "NoTuPpErCaSe" ), "NOTUPPERCASE" );
		BOOST_CHECK_EQUAL( StringUtils::UpperCase( "UPPERCASE" ), "UPPERCASE" );
		BOOST_CHECK_EQUAL( StringUtils::LowerCase( "NoTlOwErCaSe" ), "notlowercase" );
		BOOST_CHECK_EQUAL( StringUtils::LowerCase( "lowercase" ), "lowercase" );
		BOOST_CHECK_EQUAL( StringUtils::UpperCase( L"NoTuPpErCaSe" ), L"NOTUPPERCASE" );
		BOOST_CHECK_EQUAL( StringUtils::UpperCase( L"UPPERCASE" ), L"UPPERCASE" );
		BOOST_CHECK_EQUAL( StringUtils::LowerCase( L"NoTlOwErCaSe" ), L"notlowercase" );
		BOOST_CHECK_EQUAL( StringUtils::LowerCase( L"lowercase" ), L"lowercase" );

		CLogger::LogInfo( StringStream() << "**** End TestCase_DatabaseStringUtils ****" );
	}

	void CDatabaseStringUtilsTest::TestCase_StringUtilsToUpperToLower()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_StringUtilsToUpperToLower ****" );

		std::string snotupper = "NoTuPpErCaSe";
		std::string supper = "UPPERCASE";
		std::string snotlower = "NoTlOwErCaSe";
		std::string slower = "lowercase";
		BOOST_CHECK_EQUAL( StringUtils::ToUpperCase( snotupper ), "NOTUPPERCASE" );
		BOOST_CHECK_EQUAL( StringUtils::ToUpperCase( supper ), "UPPERCASE" );
		BOOST_CHECK_EQUAL( StringUtils::ToLowerCase( snotlower ), "notlowercase" );
		BOOST_CHECK_EQUAL( StringUtils::ToLowerCase( slower ), "lowercase" );
		BOOST_CHECK_EQUAL( snotupper, "NOTUPPERCASE" );
		BOOST_CHECK_EQUAL( supper, "UPPERCASE" );
		BOOST_CHECK_EQUAL( snotlower, "notlowercase" );
		BOOST_CHECK_EQUAL( slower, "lowercase" );

		std::wstring wnotupper = L"NoTuPpErCaSe";
		std::wstring wupper = L"UPPERCASE";
		std::wstring wnotlower = L"NoTlOwErCaSe";
		std::wstring wlower = L"lowercase";
		BOOST_CHECK_EQUAL( StringUtils::ToUpperCase( wnotupper ), L"NOTUPPERCASE" );
		BOOST_CHECK_EQUAL( StringUtils::ToUpperCase( wupper ), L"UPPERCASE" );
		BOOST_CHECK_EQUAL( StringUtils::ToLowerCase( wnotlower ), L"notlowercase" );
		BOOST_CHECK_EQUAL( StringUtils::ToLowerCase( wlower ), L"lowercase" );
		BOOST_CHECK_EQUAL( wnotupper, L"NOTUPPERCASE" );
		BOOST_CHECK_EQUAL( wupper, L"UPPERCASE" );
		BOOST_CHECK_EQUAL( wnotlower, L"notlowercase" );
		BOOST_CHECK_EQUAL( wlower, L"lowercase" );

		CLogger::LogInfo( StringStream() << "**** End TestCase_StringUtilsToUpperToLower ****" );
	}

	void CDatabaseStringUtilsTest::TestCase_StringUtilsConversions()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_StringUtilsConversions ****" );
		std::wstring ws = L"����������";
		std::string s = "����������";
		std::string utf8 = "ÂÄÊËÎÏÔÖÛÜ";

		CLogger::LogInfo( StringStream() << "  ToWStr" );
		BOOST_CHECK_EQUAL( ws, StringUtils::ToWStr( s ) );
		BOOST_CHECK_EQUAL( ws, StringUtils::ToWStr( ws ) );
		CLogger::LogInfo( StringStream() << "  ToStr" );
		BOOST_CHECK_EQUAL( s, StringUtils::ToStr( ws ) );
		BOOST_CHECK_EQUAL( s, StringUtils::ToStr( s ) );
		CLogger::LogInfo( StringStream() << "  ToString" );
		BOOST_CHECK_EQUAL( StringUtils::ToString( ws ), s );
		BOOST_CHECK_EQUAL( StringUtils::ToString( s ), s );
		CLogger::LogInfo( StringStream() << "  ToUtf8" );
		BOOST_CHECK_EQUAL( StringUtils::ToUtf8( s, "Windows-1252" ), utf8 );
		BOOST_CHECK_EQUAL( StringUtils::ToUtf8( ws, L"Windows-1252" ), utf8 );

		CLogger::LogInfo( StringStream() << "**** End TestCase_StringUtilsConversions ****" );
	}

	void CDatabaseStringUtilsTest::TestCase_StringUtilsSplit()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_StringUtilsSplit ****" );

		std::string stosplit = "dsfs,gsdg,,sdfh,sdh,dshgh,dfh,dsfh,dsfhsdfhsd,fhsdfh,dfhdsh";
		auto ssplitted = StringUtils::Split( stosplit, ",", 1, false );
		BOOST_CHECK_EQUAL( ssplitted.size(), 2 );
		BOOST_CHECK_EQUAL( ssplitted[0], "dsfs" );
		BOOST_CHECK_EQUAL( ssplitted[1], "gsdg,,sdfh,sdh,dshgh,dfh,dsfh,dsfhsdfhsd,fhsdfh,dfhdsh" );
		uint32_t scount = uint32_t( std::count( stosplit.begin(), stosplit.end(), ',' ) );
		BOOST_CHECK_NO_THROW( ssplitted = StringUtils::Split( stosplit, ",", scount, false ) );
		BOOST_CHECK_EQUAL( ssplitted.size(), scount );
		BOOST_CHECK_NO_THROW( ssplitted = StringUtils::Split( stosplit, ",", scount, true ) );
		BOOST_CHECK_EQUAL( ssplitted.size(), scount + 1 );
		std::wstring wtosplit = L"dsfs,gsdg,,sdfh,sdh,dshgh,dfh,dsfh,dsfhsdfhsd,fhsdfh,dfhdsh";
		auto wsplitted = StringUtils::Split( wtosplit, L",", 1, false );
		BOOST_CHECK_EQUAL( wsplitted.size(), 2 );
		BOOST_CHECK_EQUAL( wsplitted[0], L"dsfs" );
		BOOST_CHECK_EQUAL( wsplitted[1], L"gsdg,,sdfh,sdh,dshgh,dfh,dsfh,dsfhsdfhsd,fhsdfh,dfhdsh" );
		uint32_t wcount = uint32_t( std::count( wtosplit.begin(), wtosplit.end(), L',' ) );
		BOOST_CHECK_NO_THROW( wsplitted = StringUtils::Split( wtosplit, L",", wcount, false ) );
		BOOST_CHECK_EQUAL( wsplitted.size(), wcount );
		BOOST_CHECK_NO_THROW( wsplitted = StringUtils::Split( wtosplit, L",", wcount, true ) );
		BOOST_CHECK_EQUAL( wsplitted.size(), wcount + 1 );

		CLogger::LogInfo( StringStream() << "**** End TestCase_StringUtilsSplit ****" );
	}

	void CDatabaseStringUtilsTest::TestCase_StringUtilsTrim()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_StringUtilsTrim ****" );

		std::string stotrim = "AA";
		BOOST_CHECK_EQUAL( StringUtils::Trim( stotrim ), "AA" );
		BOOST_CHECK_EQUAL( stotrim, "AA" );
		stotrim = "AA ";
		BOOST_CHECK_EQUAL( StringUtils::Trim( stotrim ), "AA" );
		BOOST_CHECK_EQUAL( stotrim, "AA" );
		stotrim = " AA";
		BOOST_CHECK_EQUAL( StringUtils::Trim( stotrim ), "AA" );
		BOOST_CHECK_EQUAL( stotrim, "AA" );
		stotrim = " AA ";
		BOOST_CHECK_EQUAL( StringUtils::Trim( stotrim, false ), " AA" );
		BOOST_CHECK_EQUAL( stotrim, " AA" );
		stotrim = " AA ";
		BOOST_CHECK_EQUAL( StringUtils::Trim( stotrim, true, false ), "AA " );
		BOOST_CHECK_EQUAL( stotrim, "AA " );
		stotrim = " AA ";
		BOOST_CHECK_EQUAL( StringUtils::Trim( stotrim, false, false ), " AA " );
		BOOST_CHECK_EQUAL( stotrim, " AA " );
		std::wstring wtotrim = L"AA";
		BOOST_CHECK_EQUAL( StringUtils::Trim( wtotrim ), L"AA" );
		BOOST_CHECK_EQUAL( wtotrim, L"AA" );
		wtotrim = L"AA ";
		BOOST_CHECK_EQUAL( StringUtils::Trim( wtotrim ), L"AA" );
		BOOST_CHECK_EQUAL( wtotrim, L"AA" );
		wtotrim = L" AA";
		BOOST_CHECK_EQUAL( StringUtils::Trim( wtotrim ), L"AA" );
		BOOST_CHECK_EQUAL( wtotrim, L"AA" );
		wtotrim = L" AA ";
		BOOST_CHECK_EQUAL( StringUtils::Trim( wtotrim, false ), L" AA" );
		BOOST_CHECK_EQUAL( wtotrim, L" AA" );
		wtotrim = L" AA ";
		BOOST_CHECK_EQUAL( StringUtils::Trim( wtotrim, true, false ), L"AA " );
		BOOST_CHECK_EQUAL( wtotrim, L"AA " );
		wtotrim = L" AA ";
		BOOST_CHECK_EQUAL( StringUtils::Trim( wtotrim, false, false ), L" AA " );
		BOOST_CHECK_EQUAL( wtotrim, L" AA " );

		CLogger::LogInfo( StringStream() << "**** End TestCase_StringUtilsTrim ****" );
	}

	void CDatabaseStringUtilsTest::TestCase_StringUtilsReplace()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_StringUtilsReplace ****" );

		std::string sreplace = "gsdg,,sdfh,sdh,dshgh,dfh,dsfh,dsfhsdfhsd,fhsdfh,dfhdsh";
		BOOST_CHECK_EQUAL( StringUtils::Replace( sreplace, ",", " " ), "gsdg  sdfh sdh dshgh dfh dsfh dsfhsdfhsd fhsdfh dfhdsh" );
		BOOST_CHECK_EQUAL( sreplace, "gsdg  sdfh sdh dshgh dfh dsfh dsfhsdfhsd fhsdfh dfhdsh" );
		BOOST_CHECK_EQUAL( StringUtils::Replace( sreplace, "gsdg", "je" ), "je  sdfh sdh dshgh dfh dsfh dsfhsdfhsd fhsdfh dfhdsh" );
		BOOST_CHECK_EQUAL( sreplace, "je  sdfh sdh dshgh dfh dsfh dsfhsdfhsd fhsdfh dfhdsh" );
		BOOST_CHECK_EQUAL( StringUtils::Replace( sreplace, "dfhdsh", "voil�" ), "je  sdfh sdh dshgh dfh dsfh dsfhsdfhsd fhsdfh voil�" );
		BOOST_CHECK_EQUAL( sreplace, "je  sdfh sdh dshgh dfh dsfh dsfhsdfhsd fhsdfh voil�" );
		BOOST_CHECK_EQUAL( StringUtils::Replace( sreplace, "coin", "glop" ), "je  sdfh sdh dshgh dfh dsfh dsfhsdfhsd fhsdfh voil�" );
		BOOST_CHECK_EQUAL( sreplace, "je  sdfh sdh dshgh dfh dsfh dsfhsdfhsd fhsdfh voil�" );
		std::wstring wreplace = L"gsdg,,sdfh,sdh,dshgh,dfh,dsfh,dsfhsdfhsd,fhsdfh,dfhdsh";
		BOOST_CHECK_EQUAL( StringUtils::Replace( wreplace, L",", L" " ), L"gsdg  sdfh sdh dshgh dfh dsfh dsfhsdfhsd fhsdfh dfhdsh" );
		BOOST_CHECK_EQUAL( wreplace, L"gsdg  sdfh sdh dshgh dfh dsfh dsfhsdfhsd fhsdfh dfhdsh" );
		BOOST_CHECK_EQUAL( StringUtils::Replace( wreplace, L"gsdg", L"je" ), L"je  sdfh sdh dshgh dfh dsfh dsfhsdfhsd fhsdfh dfhdsh" );
		BOOST_CHECK_EQUAL( wreplace, L"je  sdfh sdh dshgh dfh dsfh dsfhsdfhsd fhsdfh dfhdsh" );
		BOOST_CHECK_EQUAL( StringUtils::Replace( wreplace, L"dfhdsh", L"voil�" ), L"je  sdfh sdh dshgh dfh dsfh dsfhsdfhsd fhsdfh voil�" );
		BOOST_CHECK_EQUAL( wreplace, L"je  sdfh sdh dshgh dfh dsfh dsfhsdfhsd fhsdfh voil�" );
		BOOST_CHECK_EQUAL( StringUtils::Replace( wreplace, L"coin", L"glop" ), L"je  sdfh sdh dshgh dfh dsfh dsfhsdfhsd fhsdfh voil�" );
		BOOST_CHECK_EQUAL( wreplace, L"je  sdfh sdh dshgh dfh dsfh dsfhsdfhsd fhsdfh voil�" );

		CLogger::LogInfo( StringStream() << "**** End TestCase_StringUtilsReplace ****" );
	}

	void CDatabaseStringUtilsTest::TestCase_StringUtilsFormalize()
	{
		CLogger::LogInfo( StringStream() << "**** Start TestCase_StringUtilsFormalize ****" );

		std::string sformalized;
		auto previousHandler = CDatabaseException::LinkSystemErrors();
		BOOST_CHECK_NO_THROW( StringUtils::Formalize( sformalized, 10, "%010i", 100 ) );
		BOOST_CHECK_EQUAL( sformalized, "0000000100" );
		BOOST_CHECK_NO_THROW( StringUtils::Formalize( sformalized, 9, "%010i", 100 ) );
		BOOST_CHECK_EQUAL( sformalized, "000000010" );
		BOOST_CHECK_NO_THROW( StringUtils::Formalize( sformalized, 9, "%.2f", 100 ) );
		BOOST_CHECK_NE( sformalized, "100" );
		BOOST_CHECK_NO_THROW( StringUtils::Formalize( sformalized, 9, "%.2f", 100.0f ) );
		BOOST_CHECK_EQUAL( sformalized, "100.00" );
		BOOST_CHECK_THROW( StringUtils::Formalize( sformalized, 9, "%s", 100 ), CDatabaseException );
		CDatabaseException::UnlinkSystemErrors( std::move( previousHandler ) );

		CLogger::LogInfo( StringStream() << "**** End TestCase_StringUtilsFormalize ****" );
	}
}
END_NAMESPACE_DATABASE_TEST
