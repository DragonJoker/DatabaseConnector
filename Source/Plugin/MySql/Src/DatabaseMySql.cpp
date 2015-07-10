/************************************************************************//**
 * @file DatabaseMySql.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 *
 *
 * @brief CDatabaseMySql class declaration.
 *
 * @details Describes an MYSQL database.
 *
 ***************************************************************************/

#include "DatabaseMySqlPch.h"

#include "DatabaseMySql.h"
#include "DatabaseConnectionMySql.h"

#include <mysql.h>

BEGIN_NAMESPACE_DATABASE_MYSQL
{
	static const String ERROR_MYSQL_INITIALISATION = STR( "Couldn't initialise MySQL" );

	CDatabaseMySql::CDatabaseMySql()
		: CDatabase()
	{
		if ( mysql_library_init( 0, NULL, NULL ) )
		{
			CLogger::LogError( ERROR_MYSQL_INITIALISATION );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_MYSQL_INITIALISATION, __FUNCTION__, __FILE__, __LINE__ );
		}
	}

	CDatabaseMySql::~CDatabaseMySql()
	{
		mysql_library_end();
	}

	const String & CDatabaseMySql::GetType()const
	{
		return DATABASE_MYSQL_TYPE;
	}

	CDatabase * CDatabaseMySql::Create()
	{
		return new CDatabaseMySql;
	}

	DatabaseConnectionPtr CDatabaseMySql::DoCreateConnection( String & connectionString ) const
	{
		DatabaseConnectionPtr pReturn;

		// std::make_shared limited to 5 parameters with VS2012
		pReturn = std::make_shared< CDatabaseConnectionMySql >( _server, _userName, _password, connectionString );

		return pReturn;
	}

	void MySQLTry( int result, TChar const * msg, EDatabaseExceptionCodes code, MYSQL * connection )
	{
		if ( result )
		{
			StringStream error( "Failure: " );
			error << msg << std::endl;
			error << mysql_error( connection );
			DB_EXCEPT( code, error.str() );
		}

#if !defined( NDEBUG )

		else
		{
			CLogger::LogDebug( StringStream() << STR( "Success : " ) << msg );
		}

#endif
	}

	CDate CDateFromMySqlTime( MYSQL_TIME const & ts )
	{
		assert( ts.time_type == MYSQL_TIMESTAMP_DATE );
		return CDate( ts.year, EDateMonth( ts.month - 1 ), ts.day );
	}

	CDateTime CDateTimeFromMySqlTime( MYSQL_TIME const & ts )
	{
		assert( ts.time_type == MYSQL_TIMESTAMP_DATETIME );
		std::tm tm = { ts.second, ts.minute, ts.hour, ts.day, ts.month - 1, ts.year - 1900, 0 };
		return CDateTime( tm );
	}

	CTime CTimeFromMySqlTime( MYSQL_TIME const & ts )
	{
		assert( ts.time_type == MYSQL_TIMESTAMP_TIME );
		return CTime( ts.hour, ts.minute, ts.second );
	}

	MYSQL_TIME MySqlTimeFromCDate( CDate const & value )
	{
		MYSQL_TIME ts = { 0 };
		ts.time_type = MYSQL_TIMESTAMP_DATE;
		ts.year = value.GetYear();
		ts.month = value.GetMonth() + 1;
		ts.day = value.GetMonthDay();
		return ts;
	}

	MYSQL_TIME MySqlTimeFromCDateTime( CDateTime const & value )
	{
		MYSQL_TIME ts = { 0 };
		ts.time_type = MYSQL_TIMESTAMP_DATETIME;
		ts.year = value.GetYear();
		ts.month = value.GetMonth() + 1;
		ts.day = value.GetMonthDay();
		ts.hour = value.GetHour();
		ts.minute = value.GetMinute();
		ts.second = value.GetSecond();
		return ts;
	}

	MYSQL_TIME MySqlTimeFromCTime( CTime const & value )
	{
		MYSQL_TIME ts = { 0 };
		ts.time_type = MYSQL_TIMESTAMP_TIME;
		ts.hour = value.GetHour();
		ts.minute = value.GetMinute();
		ts.second = value.GetSecond();
		return ts;
	}

	std::string StringFromMySqlString( MYSQL_BIND const & bind, bool truncated )
	{
		char * value = reinterpret_cast< char * >( bind.buffer );
		std::string result;

		if ( truncated )
		{
			if ( bind.buffer_length )
			{
				result.assign( value, value + bind.buffer_length );
			}
		}
		else
		{
			result.assign( value, value + *bind.length );
		}

		return result;
	}
}
END_NAMESPACE_DATABASE_MYSQL
