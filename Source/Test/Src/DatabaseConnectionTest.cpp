/************************************************************************//**
* @file DatabaseConnectionTest.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 3/14/2014 5:03:15 PM
*
*
* @brief CDatabaseConnectionTest class declaration.
*
* @details Describes a connection used to run the tests.
*
***************************************************************************/

#include "DatabaseTestPch.h"

#include "DatabaseConnectionTest.h"

#include "DatabaseStatementTest.h"
#include "DatabaseTestUtils.h"

#include <DatabaseDate.h>
#include <DatabaseDateTime.h>
#include <DatabaseDateTimeHelper.h>
#include <DatabaseTime.h>
#include <DatabaseFieldInfos.h>

#include <DatabaseStringUtils.h>
#include <DatabaseLogger.h>

BEGIN_NAMESPACE_DATABASE_TEST
{
	namespace
	{
		DatabaseFieldSPtr CreateField( DatabaseConnectionSPtr connection, DatabaseFieldInfosSPtr infos )
		{
			DatabaseFieldSPtr field = std::make_shared< CDatabaseField >( connection, infos );

			switch ( infos->GetType() )
			{
			case EFieldType_BIT:
				static_cast< CDatabaseValue< EFieldType_BIT > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_BIT >::InitialiseValue() );
				break;
			case EFieldType_SINT8:
				static_cast< CDatabaseValue< EFieldType_SINT8 > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_SINT8 >::InitialiseValue() );
				break;
			case EFieldType_SINT16:
				static_cast< CDatabaseValue< EFieldType_SINT16 > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_SINT16 >::InitialiseValue() );
				break;
			case EFieldType_SINT24:
				static_cast< CDatabaseValue< EFieldType_SINT24 > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_SINT24 >::InitialiseValue() );
				break;
			case EFieldType_SINT32:
				static_cast< CDatabaseValue< EFieldType_SINT32 > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_SINT32 >::InitialiseValue() );
				break;
			case EFieldType_SINT64:
				static_cast< CDatabaseValue< EFieldType_SINT64 > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_SINT64 >::InitialiseValue() );
				break;
			case EFieldType_UINT8:
				static_cast< CDatabaseValue< EFieldType_UINT8 > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_UINT8 >::InitialiseValue() );
				break;
			case EFieldType_UINT16:
				static_cast< CDatabaseValue< EFieldType_UINT16 > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_UINT16 >::InitialiseValue() );
				break;
			case EFieldType_UINT24:
				static_cast< CDatabaseValue< EFieldType_UINT24 > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_UINT24 >::InitialiseValue() );
				break;
			case EFieldType_UINT32:
				static_cast< CDatabaseValue< EFieldType_UINT32 > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_UINT32 >::InitialiseValue() );
				break;
			case EFieldType_UINT64:
				static_cast< CDatabaseValue< EFieldType_UINT64 > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_UINT64 >::InitialiseValue() );
				break;
			case EFieldType_FLOAT32:
				static_cast< CDatabaseValue< EFieldType_FLOAT32 > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_FLOAT32 >::InitialiseValue() );
				break;
			case EFieldType_FLOAT64:
				static_cast< CDatabaseValue< EFieldType_FLOAT64 > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_FLOAT64 >::InitialiseValue() );
				break;
			case EFieldType_FIXED_POINT:
				static_cast< CDatabaseValue< EFieldType_FIXED_POINT > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_FIXED_POINT >::InitialiseValue( infos->GetPrecision() ) );
				break;
			case EFieldType_CHAR:
				static_cast< CDatabaseValue< EFieldType_CHAR > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_CHAR >::InitialiseValue( infos->GetLimits() ) );
				break;
			case EFieldType_VARCHAR:
				static_cast< CDatabaseValue< EFieldType_VARCHAR > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_VARCHAR >::InitialiseValue( infos->GetLimits() ) );
				break;
			case EFieldType_TEXT:
				static_cast< CDatabaseValue< EFieldType_TEXT > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_TEXT >::InitialiseValue() );
				break;
			case EFieldType_NCHAR:
				static_cast< CDatabaseValue< EFieldType_NCHAR > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_NCHAR >::InitialiseValue( infos->GetLimits() ) );
				break;
			case EFieldType_NVARCHAR:
				static_cast< CDatabaseValue< EFieldType_NVARCHAR > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_NVARCHAR >::InitialiseValue( infos->GetLimits() ) );
				break;
			case EFieldType_NTEXT:
				static_cast< CDatabaseValue< EFieldType_NTEXT > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_NTEXT >::InitialiseValue() );
				break;
			case EFieldType_DATE:
				static_cast< CDatabaseValue< EFieldType_DATE > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_DATE >::InitialiseValue() );
				break;
			case EFieldType_DATETIME:
				static_cast< CDatabaseValue< EFieldType_DATETIME > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_DATETIME >::InitialiseValue() );
				break;
			case EFieldType_TIME:
				static_cast< CDatabaseValue< EFieldType_TIME > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_TIME >::InitialiseValue() );
				break;
			case EFieldType_BINARY:
				static_cast< CDatabaseValue< EFieldType_BINARY > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_BINARY >::InitialiseValue( infos->GetLimits() ) );
				break;
			case EFieldType_VARBINARY:
				static_cast< CDatabaseValue< EFieldType_VARBINARY > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_VARBINARY >::InitialiseValue( infos->GetLimits() ) );
				break;
			case EFieldType_LONG_VARBINARY:
				static_cast< CDatabaseValue< EFieldType_LONG_VARBINARY > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_LONG_VARBINARY >::InitialiseValue() );
				break;
			}

			return field;
		}
	}

	DatabaseResultSPtr BuildResult( DatabaseConnectionSPtr connection )
	{
		DatabaseFieldInfosPtrArray fieldInfos;
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseFieldInfos >( STR( "EFieldType_BIT" ), EFieldType_BIT ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseFieldInfos >( STR( "EFieldType_SINT8" ), EFieldType_SINT8 ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseFieldInfos >( STR( "EFieldType_SINT16" ), EFieldType_SINT16 ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseFieldInfos >( STR( "EFieldType_SINT24" ), EFieldType_SINT24 ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseFieldInfos >( STR( "EFieldType_SINT32" ), EFieldType_SINT32 ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseFieldInfos >( STR( "EFieldType_SINT64" ), EFieldType_SINT64 ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseFieldInfos >( STR( "EFieldType_UINT8" ), EFieldType_UINT8 ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseFieldInfos >( STR( "EFieldType_UINT16" ), EFieldType_UINT16 ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseFieldInfos >( STR( "EFieldType_UINT24" ), EFieldType_UINT24 ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseFieldInfos >( STR( "EFieldType_UINT32" ), EFieldType_UINT32 ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseFieldInfos >( STR( "EFieldType_UINT64" ), EFieldType_UINT64 ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseFieldInfos >( STR( "EFieldType_FLOAT32" ), EFieldType_FLOAT32 ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseFieldInfos >( STR( "EFieldType_FLOAT64" ), EFieldType_FLOAT64 ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseFieldInfos >( STR( "EFieldType_FIXED_POINT( 10, 0 )" ), EFieldType_FIXED_POINT, std::make_pair( 10, 0 ) ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseFieldInfos >( STR( "EFieldType_FIXED_POINT( 10, 5 )" ), EFieldType_FIXED_POINT, std::make_pair( 10, 5 ) ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseFieldInfos >( STR( "EFieldType_CHAR( 20 )" ), EFieldType_CHAR, 20 ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseFieldInfos >( STR( "EFieldType_VARCHAR( 100 )" ), EFieldType_VARCHAR, 100 ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseFieldInfos >( STR( "EFieldType_TEXT" ), EFieldType_TEXT ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseFieldInfos >( STR( "EFieldType_NCHAR( 55 )" ), EFieldType_NCHAR, 55 ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseFieldInfos >( STR( "EFieldType_NVARCHAR( 255 )" ), EFieldType_NVARCHAR, 255 ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseFieldInfos >( STR( "EFieldType_NTEXT" ), EFieldType_NTEXT ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseFieldInfos >( STR( "EFieldType_DATE" ), EFieldType_DATE ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseFieldInfos >( STR( "EFieldType_DATETIME" ), EFieldType_DATETIME ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseFieldInfos >( STR( "EFieldType_TIME" ), EFieldType_TIME ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseFieldInfos >( STR( "EFieldType_BINARY( 20 )" ), EFieldType_BINARY, 20 ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseFieldInfos >( STR( "EFieldType_VARBINARY( 255 )" ), EFieldType_VARBINARY, 255 ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseFieldInfos >( STR( "EFieldType_LONG_VARBINARY" ), EFieldType_LONG_VARBINARY ) ) );

		DatabaseResultSPtr result = std::make_shared< CDatabaseResult >( fieldInfos );
		DatabaseRowSPtr row = std::make_shared< CDatabaseRow >();
		
		for ( auto infos : fieldInfos )
		{
			row->AddField( CreateField( connection, infos ) );
		}

		result->AddRow( row );
		return result;
	}

	static const String ERROR_TEST_CONNECTION = STR( "Couldn't create the connection" );
	static const String ERROR_TEST_NOT_CONNECTED = STR( "Not connected" );

	CDatabaseConnectionTest::CDatabaseConnectionTest( const String & server, const String & userName, const String & password, String & connectionString )
		: CDatabaseConnection( server, userName, password )
	{
		DoConnect( connectionString );
	}

	CDatabaseConnectionTest::~CDatabaseConnectionTest()
	{
		DoDisconnect();
	}

	uint32_t CDatabaseConnectionTest::GetPrecision( EFieldType type ) const
	{
		uint32_t result = 0;

		switch ( type )
		{
		case EFieldType_FLOAT32:
			result = 6;
			break;

		case EFieldType_FLOAT64:
			result = 17;
			break;
		}

		return result;
	}

	unsigned long CDatabaseConnectionTest::GetStmtDateSize()const
	{
		return sizeof( DateType );
	}

	unsigned long CDatabaseConnectionTest::GetStmtDateTimeSize()const
	{
		return sizeof( DateTimeType );
	}

	unsigned long CDatabaseConnectionTest::GetStmtTimeSize()const
	{
		return sizeof( TimeType );
	}

	void CDatabaseConnectionTest::DoCreateDatabase( const String & database )
	{
		if ( !IsConnected() )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_TEST_NOT_CONNECTED );
		}
	}

	void CDatabaseConnectionTest::DoSelectDatabase( const String & database )
	{
		if ( !IsConnected() )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_TEST_NOT_CONNECTED );
		}
	}

	void CDatabaseConnectionTest::DoDestroyDatabase( const String & database )
	{
		if ( !IsConnected() )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_TEST_NOT_CONNECTED );
		}
	}

	std::string CDatabaseConnectionTest::DoWriteText( const std::string & text ) const
	{
		std::string strReturn( text );

		if ( strReturn != TEST_SQL_NULL )
		{
			StringUtils::Replace( strReturn, "'", "''" );
			StringUtils::Replace( strReturn, "\\", "\\\\" );
			strReturn = "'" + strReturn + "'";
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionTest::DoWriteNText( const std::wstring & text ) const
	{
		String strReturn( StringUtils::ToString( text ) );

		if ( strReturn != TEST_SQL_NULL )
		{
			StringUtils::Replace( strReturn, STR( "'" ), STR( "''" ) );
			StringUtils::Replace( strReturn, STR( "\\" ), STR( "\\\\" ) );
			strReturn = STR( "N'" ) + strReturn + STR( "'" );
		}

		return StringUtils::ToWStr( strReturn );
	}

	String CDatabaseConnectionTest::DoWriteBinary( const ByteArray & array ) const
	{
		StringStream stream;
		stream.setf( std::ios::hex, std::ios::basefield );

		for ( auto && it = array.begin(); it != array.end(); ++it )
		{
			stream.width( 2 );
			stream.fill( STR( '0' ) );
			stream << int( *it );
		}

		return STR( "X'" ) + stream.str() + STR( "'" );
	}

	String CDatabaseConnectionTest::DoWriteName( const String & text ) const
	{
		return STR( "[" ) + text + STR( "]" );
	}

	String CDatabaseConnectionTest::DoWriteDate( const DateType & date ) const
	{
		String strReturn;

		if ( Date::IsValid( date ) )
		{
			strReturn = Date::Format( date, TEST_FORMAT_DATE );
		}
		else
		{
			strReturn += TEST_SQL_NULL;
		}

		return strReturn;
	}

	String CDatabaseConnectionTest::DoWriteTime( const TimeType & time ) const
	{
		String strReturn;

		if ( Time::IsValid( time ) )
		{
			strReturn = Time::Format( time, TEST_FORMAT_TIME );
		}
		else
		{
			strReturn += TEST_SQL_NULL;
		}

		return strReturn;
	}

	String CDatabaseConnectionTest::DoWriteDateTime( const DateTimeType & dateTime ) const
	{
		String strReturn;

		if ( DateTime::IsValid( dateTime ) )
		{
			strReturn = DateTime::Format( dateTime, TEST_FORMAT_DATETIME );
		}
		else
		{
			strReturn += TEST_SQL_NULL;
		}

		return strReturn;
	}

	String CDatabaseConnectionTest::DoWriteDateTime( const DateType & date ) const
	{
		String strReturn;

		if ( Date::IsValid( date ) )
		{
			strReturn = Date::Format( date, TEST_FORMAT_DATETIME_DATE );
		}
		else
		{
			strReturn += TEST_SQL_NULL;
		}

		return strReturn;
	}

	String CDatabaseConnectionTest::DoWriteDateTime( const TimeType & time ) const
	{
		String strReturn;

		if ( Time::IsValid( time ) )
		{
			strReturn = Time::Format( time, TEST_FORMAT_DATETIME_TIME );
		}
		else
		{
			strReturn += TEST_SQL_NULL;
		}

		return strReturn;
	}

	String CDatabaseConnectionTest::DoWriteStmtDate( const DateType & date ) const
	{
		String strReturn;

		if ( Date::IsValid( date ) )
		{
			strReturn = Date::Format( date, TEST_FORMAT_STMT_DATE );
		}
		else
		{
			strReturn += TEST_SQL_NULL;
		}

		return strReturn;
	}

	String CDatabaseConnectionTest::DoWriteStmtTime( const TimeType & time ) const
	{
		String strReturn;

		if ( Time::IsValid( time ) )
		{
			strReturn = Time::Format( time, TEST_FORMAT_STMT_TIME );
		}
		else
		{
			strReturn += TEST_SQL_NULL;
		}

		return strReturn;
	}

	String CDatabaseConnectionTest::DoWriteStmtDateTime( const DateTimeType & dateTime ) const
	{
		String strReturn;

		if ( DateTime::IsValid( dateTime ) )
		{
			strReturn = DateTime::Format( dateTime, TEST_FORMAT_STMT_DATETIME );
		}
		else
		{
			strReturn += TEST_SQL_NULL;
		}

		return strReturn;
	}

	String CDatabaseConnectionTest::DoWriteBool( bool value ) const
	{
		return ( value ? STR( "1" ) : STR( "0" ) );
	}

	DateType CDatabaseConnectionTest::DoParseDate( const String & date ) const
	{
		DateType dateObj;

		if ( !Date::IsDate( date, TEST_FORMAT_DATE, dateObj )
		&& !Date::IsDate( date, TEST_FORMAT_STMT_DATE, dateObj ) )
		{
			// date is already invalid
		}

		return dateObj;
	}

	TimeType CDatabaseConnectionTest::DoParseTime( const String & time ) const
	{
		TimeType timeObj;

		if ( !Time::IsTime( time, TEST_FORMAT_TIME, timeObj )
		&& !Time::IsTime( time, TEST_FORMAT_STMT_TIME, timeObj ) )
		{
			// time is already invalid
		}

		return timeObj;
	}

	DateTimeType CDatabaseConnectionTest::DoParseDateTime( const String & dateTime ) const
	{
		DateTimeType dateTimeObj;

		if ( !DateTime::IsDateTime( dateTime, TEST_FORMAT_DATETIME, dateTimeObj )
		&& !DateTime::IsDateTime( dateTime, TEST_FORMAT_STMT_DATETIME, dateTimeObj )
		&& !DateTime::IsDateTime( dateTime, TEST_FORMAT_DATE, dateTimeObj )
		&& !DateTime::IsDateTime( dateTime, TEST_FORMAT_STMT_DATE, dateTimeObj ) )
		{
			// date/time is already invalid
		}

		return dateTimeObj;
	}

	EErrorType CDatabaseConnectionTest::DoConnect( String & connectionString )
	{
		EErrorType ret = EErrorType_NONE;

		try
		{
			connectionString = STR( "server=" ) + _server + STR( ";user=" ) + _userName + STR( ";password=" ) + _password;
			DoSetConnected( _server == STR( "TestServer" ) && _userName == STR( "TestUser" ) && _password == STR( "TestPassword" ) );
		}
		catch ( CDatabaseException & exc )
		{
			CLogger::LogError( StringStream() << ERROR_TEST_CONNECTION << STR( " - " ) << exc.GetFullDescription() );
			ret = EErrorType_ERROR;
		}
		catch ( std::exception & exc )
		{
			CLogger::LogError( StringStream() << ERROR_TEST_CONNECTION << STR( " - " ) << exc.what() );
			ret = EErrorType_ERROR;
		}
		catch ( ... )
		{
			CLogger::LogError( ERROR_TEST_CONNECTION );
			ret = EErrorType_ERROR;
		}

		return ret;
	}

	void CDatabaseConnectionTest::DoDisconnect()
	{
		DoSetConnected( false );
	}

	bool CDatabaseConnectionTest::DoBeginTransaction( const String & name )
	{
		return name.empty() || name == STR( "TestTransaction" );
	}

	bool CDatabaseConnectionTest::DoCommit( const String & name )
	{
		return name.empty() || name == STR( "TestTransaction" );
	}

	bool CDatabaseConnectionTest::DoRollBack( const String & name )
	{
		return name.empty() || name == STR( "TestTransaction" );
	}

	bool CDatabaseConnectionTest::DoExecuteUpdate( const String & query )
	{
		return query == STR( "TestUpdate" );
	}

	DatabaseResultSPtr CDatabaseConnectionTest::DoExecuteSelect( const String & query )
	{
		return query == STR( "TestSelect" ) ? BuildResult( shared_from_this() ) : DatabaseResultSPtr();
	}

	DatabaseStatementSPtr CDatabaseConnectionTest::DoCreateStatement( const String & request )
	{
		return std::make_shared< CDatabaseStatementTest >( std::static_pointer_cast< CDatabaseConnectionTest >( shared_from_this() ), request );
	}
}
END_NAMESPACE_DATABASE_TEST
