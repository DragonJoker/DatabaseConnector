/************************************************************************//**
* @file DatabaseTestConnection.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 3/14/2014 5:03:15 PM
*
*
* @brief CDatabaseTestConnection class declaration.
*
* @details Describes a connection used to run the tests.
*
***************************************************************************/

#include "DatabaseTestPch.h"

#include "DatabaseTestConnection.h"

#include "DatabaseTestStatement.h"
#include "DatabaseTestUtils.h"

#include <DatabaseDate.h>
#include <DatabaseDateTime.h>
#include <DatabaseDateTimeHelper.h>
#include <DatabaseTime.h>
#include <DatabaseValuedObjectInfos.h>

#include <DatabaseStringUtils.h>
#include <DatabaseLogger.h>

BEGIN_NAMESPACE_DATABASE_TEST
{
	namespace
	{
		DatabaseFieldSPtr CreateField( DatabaseConnectionSPtr connection, DatabaseValuedObjectInfosSPtr infos )
		{
			std::random_device generator;
			DatabaseFieldSPtr field = std::make_shared< CDatabaseField >( connection, infos );

			switch ( infos->GetType() )
			{
			case EFieldType_BIT:
				static_cast< CDatabaseValue< EFieldType_BIT > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_BIT >::GetRandomValue( generator ) );
				break;
			case EFieldType_SINT8:
				static_cast< CDatabaseValue< EFieldType_SINT8 > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_SINT8 >::GetRandomValue( generator ) );
				break;
			case EFieldType_SINT16:
				static_cast< CDatabaseValue< EFieldType_SINT16 > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_SINT16 >::GetRandomValue( generator ) );
				break;
			case EFieldType_SINT24:
				static_cast< CDatabaseValue< EFieldType_SINT24 > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_SINT24 >::GetRandomValue( generator ) );
				break;
			case EFieldType_SINT32:
				static_cast< CDatabaseValue< EFieldType_SINT32 > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_SINT32 >::GetRandomValue( generator ) );
				break;
			case EFieldType_SINT64:
				static_cast< CDatabaseValue< EFieldType_SINT64 > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_SINT64 >::GetRandomValue( generator ) );
				break;
			case EFieldType_UINT8:
				static_cast< CDatabaseValue< EFieldType_UINT8 > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_UINT8 >::GetRandomValue( generator ) );
				break;
			case EFieldType_UINT16:
				static_cast< CDatabaseValue< EFieldType_UINT16 > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_UINT16 >::GetRandomValue( generator ) );
				break;
			case EFieldType_UINT24:
				static_cast< CDatabaseValue< EFieldType_UINT24 > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_UINT24 >::GetRandomValue( generator ) );
				break;
			case EFieldType_UINT32:
				static_cast< CDatabaseValue< EFieldType_UINT32 > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_UINT32 >::GetRandomValue( generator ) );
				break;
			case EFieldType_UINT64:
				static_cast< CDatabaseValue< EFieldType_UINT64 > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_UINT64 >::GetRandomValue( generator ) );
				break;
			case EFieldType_FLOAT32:
				static_cast< CDatabaseValue< EFieldType_FLOAT32 > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_FLOAT32 >::GetRandomValue( generator ) );
				break;
			case EFieldType_FLOAT64:
				static_cast< CDatabaseValue< EFieldType_FLOAT64 > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_FLOAT64 >::GetRandomValue( generator ) );
				break;
			case EFieldType_FIXED_POINT:
				static_cast< CDatabaseValue< EFieldType_FIXED_POINT > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_FIXED_POINT >::GetRandomValue( generator, infos->GetPrecision() ) );
				break;
			case EFieldType_CHAR:
				static_cast< CDatabaseValue< EFieldType_CHAR > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_CHAR >::GetRandomValue( generator, infos->GetLimits() ) );
				break;
			case EFieldType_VARCHAR:
				static_cast< CDatabaseValue< EFieldType_VARCHAR > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_VARCHAR >::GetRandomValue( generator, infos->GetLimits() ) );
				break;
			case EFieldType_TEXT:
				static_cast< CDatabaseValue< EFieldType_TEXT > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_TEXT >::GetRandomValue( generator ) );
				break;
			case EFieldType_NCHAR:
				static_cast< CDatabaseValue< EFieldType_NCHAR > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_NCHAR >::GetRandomValue( generator, infos->GetLimits() ) );
				break;
			case EFieldType_NVARCHAR:
				static_cast< CDatabaseValue< EFieldType_NVARCHAR > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_NVARCHAR >::GetRandomValue( generator, infos->GetLimits() ) );
				break;
			case EFieldType_NTEXT:
				static_cast< CDatabaseValue< EFieldType_NTEXT > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_NTEXT >::GetRandomValue( generator ) );
				break;
			case EFieldType_DATE:
				static_cast< CDatabaseValue< EFieldType_DATE > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_DATE >::GetRandomValue( generator ) );
				break;
			case EFieldType_DATETIME:
				static_cast< CDatabaseValue< EFieldType_DATETIME > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_DATETIME >::GetRandomValue( generator ) );
				break;
			case EFieldType_TIME:
				static_cast< CDatabaseValue< EFieldType_TIME > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_TIME >::GetRandomValue( generator ) );
				break;
			case EFieldType_BINARY:
				static_cast< CDatabaseValue< EFieldType_BINARY > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_BINARY >::GetRandomValue( generator, infos->GetLimits() ) );
				break;
			case EFieldType_VARBINARY:
				static_cast< CDatabaseValue< EFieldType_VARBINARY > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_VARBINARY >::GetRandomValue( generator, infos->GetLimits() ) );
				break;
			case EFieldType_BLOB:
				static_cast< CDatabaseValue< EFieldType_BLOB > & >( field->GetObjectValue() ).SetValue( DatabaseUtils::Helpers< EFieldType_BLOB >::GetRandomValue( generator ) );
				break;
			}

			return field;
		}
	}

	DatabaseResultSPtr BuildResult( DatabaseConnectionSPtr connection )
	{
		DatabaseValuedObjectInfosPtrArray fieldInfos;
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseValuedObjectInfos >( STR( "EFieldType_BIT" ), EFieldType_BIT ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseValuedObjectInfos >( STR( "EFieldType_SINT8" ), EFieldType_SINT8 ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseValuedObjectInfos >( STR( "EFieldType_SINT16" ), EFieldType_SINT16 ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseValuedObjectInfos >( STR( "EFieldType_SINT24" ), EFieldType_SINT24 ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseValuedObjectInfos >( STR( "EFieldType_SINT32" ), EFieldType_SINT32 ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseValuedObjectInfos >( STR( "EFieldType_SINT64" ), EFieldType_SINT64 ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseValuedObjectInfos >( STR( "EFieldType_UINT8" ), EFieldType_UINT8 ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseValuedObjectInfos >( STR( "EFieldType_UINT16" ), EFieldType_UINT16 ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseValuedObjectInfos >( STR( "EFieldType_UINT24" ), EFieldType_UINT24 ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseValuedObjectInfos >( STR( "EFieldType_UINT32" ), EFieldType_UINT32 ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseValuedObjectInfos >( STR( "EFieldType_UINT64" ), EFieldType_UINT64 ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseValuedObjectInfos >( STR( "EFieldType_FLOAT32" ), EFieldType_FLOAT32 ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseValuedObjectInfos >( STR( "EFieldType_FLOAT64" ), EFieldType_FLOAT64 ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseValuedObjectInfos >( STR( "EFieldType_FIXED_POINT( 10, 0 )" ), EFieldType_FIXED_POINT, std::make_pair( 10, 0 ) ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseValuedObjectInfos >( STR( "EFieldType_FIXED_POINT( 10, 5 )" ), EFieldType_FIXED_POINT, std::make_pair( 10, 5 ) ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseValuedObjectInfos >( STR( "EFieldType_CHAR( 20 )" ), EFieldType_CHAR, 20 ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseValuedObjectInfos >( STR( "EFieldType_VARCHAR( 100 )" ), EFieldType_VARCHAR, 100 ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseValuedObjectInfos >( STR( "EFieldType_TEXT" ), EFieldType_TEXT ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseValuedObjectInfos >( STR( "EFieldType_NCHAR( 55 )" ), EFieldType_NCHAR, 55 ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseValuedObjectInfos >( STR( "EFieldType_NVARCHAR( 255 )" ), EFieldType_NVARCHAR, 255 ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseValuedObjectInfos >( STR( "EFieldType_NTEXT" ), EFieldType_NTEXT ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseValuedObjectInfos >( STR( "EFieldType_DATE" ), EFieldType_DATE ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseValuedObjectInfos >( STR( "EFieldType_DATETIME" ), EFieldType_DATETIME ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseValuedObjectInfos >( STR( "EFieldType_TIME" ), EFieldType_TIME ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseValuedObjectInfos >( STR( "EFieldType_BINARY( 20 )" ), EFieldType_BINARY, 20 ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseValuedObjectInfos >( STR( "EFieldType_VARBINARY( 255 )" ), EFieldType_VARBINARY, 255 ) ) );
		BOOST_CHECK_NO_THROW( fieldInfos.push_back( std::make_shared< CDatabaseValuedObjectInfos >( STR( "EFieldType_BLOB" ), EFieldType_BLOB ) ) );

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
	static const String ERROR_TEST_WRONG_DATABASE = STR( "Wrong database name" );

	CDatabaseTestConnection::CDatabaseTestConnection( const String & server, const String & userName, const String & password, String & connectionString )
		: CDatabaseConnection( server, userName, password )
	{
		DoConnect( connectionString );
	}

	CDatabaseTestConnection::~CDatabaseTestConnection()
	{
		DoDisconnect();
	}

	uint32_t CDatabaseTestConnection::GetPrecision( EFieldType type ) const
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

	unsigned long CDatabaseTestConnection::GetStmtDateSize()const
	{
		return sizeof( DateType );
	}

	unsigned long CDatabaseTestConnection::GetStmtDateTimeSize()const
	{
		return sizeof( DateTimeType );
	}

	unsigned long CDatabaseTestConnection::GetStmtTimeSize()const
	{
		return sizeof( TimeType );
	}

	void CDatabaseTestConnection::DoCreateDatabase( const String & database )
	{
		if ( !IsConnected() )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_TEST_NOT_CONNECTED );
		}

		if ( database != TEST_GOOD_DATABASE )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_TEST_WRONG_DATABASE );
		}
	}

	void CDatabaseTestConnection::DoSelectDatabase( const String & database )
	{
		if ( !IsConnected() )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_TEST_NOT_CONNECTED );
		}

		if ( database != TEST_GOOD_DATABASE )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_TEST_WRONG_DATABASE );
		}

		_database = database;
	}

	void CDatabaseTestConnection::DoDestroyDatabase( const String & database )
	{
		if ( !IsConnected() )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_TEST_NOT_CONNECTED );
		}

		if ( database != TEST_GOOD_DATABASE )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_ConnectionError, ERROR_TEST_WRONG_DATABASE );
		}
	}

	std::string CDatabaseTestConnection::DoWriteText( const std::string & text ) const
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

	std::wstring CDatabaseTestConnection::DoWriteNText( const std::wstring & text ) const
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

	String CDatabaseTestConnection::DoWriteBinary( const ByteArray & array ) const
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

	String CDatabaseTestConnection::DoWriteName( const String & text ) const
	{
		return STR( "[" ) + text + STR( "]" );
	}

	String CDatabaseTestConnection::DoWriteDate( const DateType & date ) const
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

	String CDatabaseTestConnection::DoWriteTime( const TimeType & time ) const
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

	String CDatabaseTestConnection::DoWriteDateTime( const DateTimeType & dateTime ) const
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

	String CDatabaseTestConnection::DoWriteDateTime( const DateType & date ) const
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

	String CDatabaseTestConnection::DoWriteDateTime( const TimeType & time ) const
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

	String CDatabaseTestConnection::DoWriteStmtDate( const DateType & date ) const
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

	String CDatabaseTestConnection::DoWriteStmtTime( const TimeType & time ) const
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

	String CDatabaseTestConnection::DoWriteStmtDateTime( const DateTimeType & dateTime ) const
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

	String CDatabaseTestConnection::DoWriteBool( bool value ) const
	{
		return ( value ? STR( "1" ) : STR( "0" ) );
	}

	DateType CDatabaseTestConnection::DoParseDate( const String & date ) const
	{
		DateType dateObj;

		if ( !Date::IsDate( date, TEST_FORMAT_DATE, dateObj )
		&& !Date::IsDate( date, TEST_FORMAT_STMT_DATE, dateObj ) )
		{
			// date is already invalid
		}

		return dateObj;
	}

	TimeType CDatabaseTestConnection::DoParseTime( const String & time ) const
	{
		TimeType timeObj;

		if ( !Time::IsTime( time, TEST_FORMAT_TIME, timeObj )
		&& !Time::IsTime( time, TEST_FORMAT_STMT_TIME, timeObj ) )
		{
			// time is already invalid
		}

		return timeObj;
	}

	DateTimeType CDatabaseTestConnection::DoParseDateTime( const String & dateTime ) const
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

	EErrorType CDatabaseTestConnection::DoConnect( String & connectionString )
	{
		EErrorType ret = EErrorType_NONE;

		try
		{
			connectionString = STR( "server=" ) + _server + STR( ";user=" ) + _userName + STR( ";password=" ) + _password;
			DoSetConnected( _server == TEST_GOOD_SERVER && _userName == TEST_GOOD_USER && _password == TEST_GOOD_PASSWORD );
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

	void CDatabaseTestConnection::DoDisconnect()
	{
		DoSetConnected( false );
	}

	bool CDatabaseTestConnection::DoBeginTransaction( const String & name )
	{
		return name.empty() || name == STR( "TestTransaction" );
	}

	bool CDatabaseTestConnection::DoCommit( const String & name )
	{
		return name.empty() || name == STR( "TestTransaction" );
	}

	bool CDatabaseTestConnection::DoRollBack( const String & name )
	{
		return name.empty() || name == STR( "TestTransaction" );
	}

	bool CDatabaseTestConnection::DoExecuteUpdate( const String & query )
	{
		return query == STR( "TestUpdate" );
	}

	DatabaseResultSPtr CDatabaseTestConnection::DoExecuteSelect( const String & query )
	{
		return query == STR( "TestSelect" ) ? BuildResult( shared_from_this() ) : DatabaseResultSPtr();
	}

	DatabaseStatementSPtr CDatabaseTestConnection::DoCreateStatement( const String & request )
	{
		return std::make_shared< CDatabaseTestStatement >( std::static_pointer_cast< CDatabaseTestConnection >( shared_from_this() ), request );
	}
}
END_NAMESPACE_DATABASE_TEST
