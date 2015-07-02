/************************************************************************//**
* @file DatabaseConnectionMySql.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 3/14/2014 5:03:15 PM
*
*
* @brief CConnectionMySql class definition.
*
* @details Describes a connection to a database via an MYSQL driver.
*
***************************************************************************/

#include "DatabaseMySqlPch.h"

#include "DatabaseConnectionMySql.h"

#include "DatabaseStatementMySql.h"
#include "ExceptionDatabaseMySql.h"

#include <DatabaseQuery.h>
#include <DatabaseDate.h>
#include <DatabaseDateTime.h>
#include <DatabaseDateTimeHelper.h>
#include <DatabaseTime.h>

#include <DatabaseStringUtils.h>
#include <DatabaseLogger.h>

#include <cppconn/statement.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/metadata.h>

BEGIN_NAMESPACE_DATABASE_MYSQL
{
	static const String MYSQL_TRANSACTION_BEGIN = STR( "BEGIN TRANSACTION " );
	static const String MYSQL_TRANSACTION_COMMIT = STR( "COMMIT TRANSACTION " );
	static const String MYSQL_TRANSACTION_ROLLBACK = STR( "ROLLBACK TRANSACTION " );

	static const String ERROR_MYSQL_CONNECTION = STR( "Couldn't create the connection" );
	static const String ERROR_MYSQL_EXECUTION_ERROR = STR( "Couldn't execute the query" );
	static const String ERROR_MYSQL_UNKNOWN_ERROR = STR( "Unknown error" );
	static const String ERROR_MYSQL_NOT_CONNECTED = STR( "Not connected" );
	static const String ERROR_MYSQL_ALREADY_IN_TRANSACTION = STR( "Already in a transaction" );
	static const String ERROR_MYSQL_NOT_IN_TRANSACTION = STR( "Not in a transaction" );
	static const String ERROR_MYSQL_DRIVER = STR( "MYSQL Driver error : " );
	static const String ERROR_MYSQL_UNKNOWN = STR( "Unknown error encountered while executing query" );

	static const std::string MYSQL_NULL_STDSTRING = "NULL";
	static const std::wstring MYSQL_NULL_STDWSTRING = L"NULL";
	static const String MYSQL_NULL_STRING = STR( "NULL" );

	CDatabaseConnectionMySql::CDatabaseConnectionMySql( sql::Driver * driver, const String & server, const String & userName, const String & password, String & connectionString )
		:   CDatabaseConnection( server, userName, password )
		,   _connection( NULL )
		,   _driver( driver )
	{
		DoConnect( connectionString );
	}

	CDatabaseConnectionMySql::~CDatabaseConnectionMySql()
	{
		DoDisconnect();
	}

	EErrorType CDatabaseConnectionMySql::BeginTransaction( const String & name )
	{
		EErrorType eReturn = EErrorType_ERROR;

		if ( !IsConnected() )
		{
			CLogger::LogError( ERROR_MYSQL_NOT_CONNECTED );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_MYSQL_NOT_CONNECTED, __FUNCTION__, __FILE__, __LINE__ );
		}

		if ( IsInTransaction() )
		{
			CLogger::LogError( ERROR_MYSQL_ALREADY_IN_TRANSACTION );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_MYSQL_ALREADY_IN_TRANSACTION, __FUNCTION__, __FILE__, __LINE__ );
		}

		_transaction = _connection->setSavepoint( CStrUtils::ToStr( name ) );
		DoSetInTransaction( true );
		eReturn = EErrorType_NONE;
		return eReturn;
	}

	EErrorType CDatabaseConnectionMySql::Commit( const String & name )
	{
		EErrorType eReturn = EErrorType_ERROR;

		if ( !IsConnected() )
		{
			CLogger::LogError( ERROR_MYSQL_NOT_CONNECTED );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_MYSQL_NOT_CONNECTED, __FUNCTION__, __FILE__, __LINE__ );
		}

		if ( !IsInTransaction() )
		{
			CLogger::LogError( ERROR_MYSQL_NOT_IN_TRANSACTION );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_MYSQL_NOT_IN_TRANSACTION, __FUNCTION__, __FILE__, __LINE__ );
		}

		_connection->commit();
		DoSetInTransaction( false );
		eReturn = EErrorType_NONE;
		return eReturn;
	}

	EErrorType CDatabaseConnectionMySql::RollBack( const String & name )
	{
		EErrorType eReturn = EErrorType_ERROR;

		if ( !IsConnected() )
		{
			CLogger::LogError( ERROR_MYSQL_NOT_CONNECTED );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_MYSQL_NOT_CONNECTED, __FUNCTION__, __FILE__, __LINE__ );
		}

		if ( !IsInTransaction() )
		{
			CLogger::LogError( ERROR_MYSQL_NOT_IN_TRANSACTION );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_MYSQL_NOT_IN_TRANSACTION, __FUNCTION__, __FILE__, __LINE__ );
		}

		_connection->rollback( _transaction );
		_connection->releaseSavepoint( _transaction );
		DoSetInTransaction( false );
		eReturn = EErrorType_NONE;
		return eReturn;
	}

	std::string CDatabaseConnectionMySql::WriteText( const std::string & text ) const
	{
		std::string strReturn( text );

		if ( strReturn != MYSQL_NULL_STDSTRING )
		{
			Replace( strReturn, "'", "''" );
			Replace( strReturn, "\\", "\\\\" );
			strReturn = "'" + strReturn + "'";
		}

		return strReturn;
	}

	std::wstring CDatabaseConnectionMySql::WriteNText( const std::wstring & text ) const
	{
		String strReturn( CStrUtils::ToString( text ) );

		if ( strReturn != MYSQL_NULL_STRING )
		{
			Replace( strReturn, STR( "'" ), STR( "''" ) );
			Replace( strReturn, STR( "\\" ), STR( "\\\\" ) );
			strReturn = STR( "N'" ) + strReturn + STR( "'" );
		}

		return CStrUtils::ToWStr( strReturn );
	}

	String CDatabaseConnectionMySql::WriteName( const String & text ) const
	{
		return STR( "[" ) + text + STR( "]" );
	}

	std::string CDatabaseConnectionMySql::WriteDate( const CDate & date ) const
	{
		std::string strReturn;

		if ( date.IsValid() )
		{
			if ( date.GetYear() < 0 )
			{
				Formalize( strReturn, 1024, "CAST('%05i%02i%02i' AS DATE)", date.GetYear(), date.GetMonth(), date.GetMonthDay() );
			}
			else
			{
				Formalize( strReturn, 1024, "CAST('%04i%02i%02i' AS DATE)", date.GetYear(), date.GetMonth(), date.GetMonthDay() );
			}
		}
		else
		{
			strReturn += MYSQL_NULL_STDSTRING;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionMySql::WriteDate( const std::string & date, const std::string & format ) const
	{
		std::string strReturn;
		CDate dateObj;

		if ( CDate::IsDate( date, format, dateObj ) )
		{
			strReturn = WriteDate( dateObj );
		}
		else
		{
			strReturn += MYSQL_NULL_STDSTRING;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionMySql::WriteTime( const CTime & time ) const
	{
		std::string strReturn;

		if ( time.IsValid() )
		{
			Formalize( strReturn, 1024, "CAST('%02i:%02i%:02i' AS TIME)", time.GetHour(), time.GetMinute(), time.GetSecond() );
		}
		else
		{
			strReturn += MYSQL_NULL_STDSTRING;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionMySql::WriteTime( const std::string & time, const std::string & format ) const
	{
		std::string strReturn;
		CTime timeObj;

		if ( CTime::IsTime( time, format, timeObj ) )
		{
			strReturn = WriteTime( timeObj );
		}
		else
		{
			strReturn += MYSQL_NULL_STDSTRING;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionMySql::WriteDateTime( const CDateTime & dateTime ) const
	{
		std::string strReturn;

		if ( dateTime.GetYear() <= 0 )
		{
			strReturn += MYSQL_NULL_STDSTRING;
		}
		else
		{
			Formalize( strReturn, 1024, "CAST('%04i-%02i-%02i %02i:%02i:%02i' as DATETIME)", dateTime.GetYear(), dateTime.GetMonth(), dateTime.GetMonthDay(), dateTime.GetHour(), dateTime.GetMinute(), dateTime.GetSecond() );
		}

		return strReturn;
	}

	std::string CDatabaseConnectionMySql::WriteDateTime( const CDate & date ) const
	{
		std::string strReturn;

		if ( !date.IsValid() || date.GetYear() <= 0 )
		{
			strReturn += MYSQL_NULL_STDSTRING;
		}
		else
		{
			Formalize( strReturn, 1024, "CAST('%04i-%02i-%02i 00:00:00' as DATETIME)", date.GetYear(), date.GetMonth(), date.GetMonthDay() );
		}

		return strReturn;
	}

	std::string CDatabaseConnectionMySql::WriteDateTime( const CTime & time ) const
	{
		std::string strReturn;

		if ( !time.IsValid() )
		{
			strReturn += MYSQL_NULL_STDSTRING;
		}
		else
		{
			Formalize( strReturn, 1024, "CAST('0000-00-00 %02i:%02i:%02i' as DATETIME)", time.GetHour(), time.GetMinute(), time.GetSecond() );
		}

		return strReturn;
	}

	std::string CDatabaseConnectionMySql::WriteDateTime( const std::string & dateTime, const std::string & format ) const
	{
		std::string strReturn;
		CDateTime dateTimeObj;

		if ( CDateTime::IsDateTime( dateTime,  dateTimeObj ) )
		{
			strReturn = WriteDateTime( dateTimeObj );
		}
		else
		{
			strReturn += MYSQL_NULL_STDSTRING;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionMySql::WriteStmtDate( const CDate & date ) const
	{
		std::string strReturn;

		if ( date.IsValid() )
		{
			Formalize( strReturn, 1024, "{-d %04i-%02i-%02i}", date.GetYear(), date.GetMonth(), date.GetMonthDay() );
		}
		else
		{
			strReturn += MYSQL_NULL_STDSTRING;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionMySql::WriteStmtDate( const std::string & date, const std::string & format ) const
	{
		std::string strReturn;
		CDate dateObj;

		if ( CDate::IsDate( date, format, dateObj ) )
		{
			strReturn = WriteStmtDate( dateObj );
		}
		else
		{
			strReturn += MYSQL_NULL_STDSTRING;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionMySql::WriteStmtTime( const CTime & time ) const
	{
		std::string strReturn;

		if ( time.IsValid() )
		{
			Formalize( strReturn, 1024, "{-t %02i:%02i:%02i}", time.GetHour(), time.GetMinute(), time.GetSecond() );
		}
		else
		{
			strReturn += MYSQL_NULL_STDSTRING;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionMySql::WriteStmtTime( const std::string & time, const std::string & format ) const
	{
		std::string strReturn;
		CTime timeObj;

		if ( CTime::IsTime( time, format, timeObj ) )
		{
			strReturn = WriteStmtTime( timeObj );
		}
		else
		{
			strReturn += MYSQL_NULL_STDSTRING;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionMySql::WriteStmtDateTime( const CDateTime & dateTime ) const
	{
		std::string strReturn;

		if ( dateTime.GetYear() > 0 )
		{
			Formalize( strReturn, 1024, "{-ts %04i-%02i-%02i %02i:%02i:%02i}", dateTime.GetYear(), dateTime.GetMonth(), dateTime.GetMonthDay(), dateTime.GetHour(), dateTime.GetMinute(), dateTime.GetSecond() );
		}
		else
		{
			strReturn += MYSQL_NULL_STDSTRING;
		}

		return strReturn;
	}

	std::string CDatabaseConnectionMySql::WriteStmtDateTime( const std::string & dateTime, const std::string & format ) const
	{
		std::string strReturn;
		CDateTime dateTimeObj;

		if ( CDateTime::IsDateTime( dateTime, dateTimeObj ) )
		{
			strReturn = WriteStmtDateTime( dateTimeObj );
		}
		else
		{
			strReturn += MYSQL_NULL_STDSTRING;
		}

		return strReturn;
	}

	String CDatabaseConnectionMySql::WriteBool( bool value ) const
	{
		return ( value ? STR( "1" ) : STR( "0" ) );
	}

	String CDatabaseConnectionMySql::WriteBool( const String & value ) const
	{
		const String lowerCaseValue = CStrUtils::LowerCase( value );
		return ( lowerCaseValue == STR( "x" ) || lowerCaseValue == STR( "oui" ) || lowerCaseValue == STR( "yes" ) || lowerCaseValue == STR( "y" ) || value == STR( "1" ) ? STR( "1" ) : STR( "0" ) );
	}

	CDate CDatabaseConnectionMySql::ParseDate( const std::string & date ) const
	{
		CDate dateObj;

		if ( !CDate::IsDate( date, "CAST('%Y%M%D' AS DATE)", dateObj )
		&& !CDate::IsDate( date, "{-d %Y-%M-%D}", dateObj )
		&& !CDate::IsDate( date, "%Y%M%D", dateObj )
		&& !CDate::IsDate( date, "%Y-%M-%D", dateObj )
		&& !CDate::IsDate( date, "%D/%M/%Y", dateObj ) )
		{
			dateObj = CDate( 0, EDateMonth_UNDEF, 0 );
		}

		return dateObj;
	}

	CTime CDatabaseConnectionMySql::ParseTime( const std::string & time ) const
	{
		CTime timeObj;
		CTime::IsTime( time, timeObj );
		return timeObj;
	}

	CDateTime CDatabaseConnectionMySql::ParseDateTime( const std::string & dateTime ) const
	{
		CDateTime dateTimeObj;
		CDateTime::IsDateTime( dateTime, dateTimeObj );
		return dateTimeObj;
	}

	CDate CDatabaseConnectionMySql::ParseDate( const std::wstring & date ) const
	{
		CDate dateObj;

		if ( !CDate::IsDate( date, L"CAST('%Y%M%D' AS DATE)", dateObj )
		&& !CDate::IsDate( date, L"{-d %Y-%M-%D}", dateObj )
		&& !CDate::IsDate( date, L"%Y%M%D", dateObj )
		&& !CDate::IsDate( date, L"%Y-%M-%D", dateObj )
		&& !CDate::IsDate( date, L"%D/%M/%Y", dateObj ) )
		{
			dateObj = CDate( 0, EDateMonth_UNDEF, 0 );
		}

		return dateObj;
	}

	CTime CDatabaseConnectionMySql::ParseTime( const std::wstring & time ) const
	{
		CTime timeObj;
		CTime::IsTime( time, timeObj );
		return timeObj;
	}

	CDateTime CDatabaseConnectionMySql::ParseDateTime( const std::wstring & dateTime ) const
	{
		CDateTime dateTimeObj;
		CDateTime::IsDateTime( dateTime, dateTimeObj );
		return dateTimeObj;
	}

	sql::Connection * CDatabaseConnectionMySql::GetConnection() const
	{
		return _connection;
	}

	bool CDatabaseConnectionMySql::ExecuteUpdate( std::shared_ptr< sql::PreparedStatement > statement )
	{
		bool result = false;

		try
		{
			MySQLTry( statement->executeUpdate(), STR( "Statement execution" ) );
			result = true;
		}
		catch ( std::exception & exc )
		{
			StringStream stream;
			stream << ERROR_MYSQL_EXECUTION_ERROR << STR( " - " ) << exc.what();
			CLogger::LogError( stream );
		}

		return result;
	}

	DatabaseResultPtr CDatabaseConnectionMySql::ExecuteSelect( std::shared_ptr< sql::PreparedStatement > statement )
	{
		DatabaseResultPtr result;

		try
		{
			bool hasResults = false;
			std::shared_ptr< sql::ResultSet > rs;
			MySQLTry( rs.reset( statement->executeQuery() ), STR( "Statement execution" ) );
			result = DoRetrieveResults( statement, rs );
			rs.reset();
		}
		catch ( std::exception & exc )
		{
			StringStream stream;
			stream << ERROR_MYSQL_EXECUTION_ERROR << STR( " - " ) << exc.what();
			CLogger::LogError( stream );
		}

		return result;
	}

	bool CDatabaseConnectionMySql::ExecuteUpdate( std::shared_ptr< sql::Statement > statement, const String & query )
	{
		CLogger::LogMessage( STR( "Executing update : " ) + query );
		bool result = false;

		try
		{
			MySQLTry( statement->executeUpdate( CStrUtils::ToStr( query ) ), STR( "Statement execution" ) );
			result = true;
		}
		catch ( std::exception & exc )
		{
			StringStream stream;
			stream << ERROR_MYSQL_EXECUTION_ERROR << STR( " - " ) << exc.what();
			CLogger::LogError( stream );
		}

		return result;
	}

	DatabaseResultPtr CDatabaseConnectionMySql::ExecuteSelect( std::shared_ptr< sql::Statement > statement, const String & query )
	{
		CLogger::LogMessage( STR( "Executing select : " ) + query );
		DatabaseResultPtr result;

		try
		{
			bool hasResults = false;
			std::shared_ptr< sql::ResultSet > rs;
			MySQLTry( rs.reset( statement->executeQuery( CStrUtils::ToStr( query ) ) ), STR( "Statement execution" ) );
			result = DoRetrieveResults( statement, rs );
			rs.reset();
		}
		catch ( std::exception & exc )
		{
			StringStream stream;
			stream << ERROR_MYSQL_EXECUTION_ERROR << STR( " - " ) << exc.what();
			CLogger::LogError( stream );
		}

		return result;
	}

	void CDatabaseConnectionMySql::CreateDatabase( const String & database )
	{
		if ( !IsConnected() )
		{
			CLogger::LogError( ERROR_MYSQL_NOT_CONNECTED );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_MYSQL_NOT_CONNECTED, __FUNCTION__, __FILE__, __LINE__ );
		}

		DoExecuteUpdate( STR( "CREATE DATABASE " ) + database + STR( " CHARACTER SET utf8 COLLATE utf8_general_ci" ), NULL );
	}

	void CDatabaseConnectionMySql::SelectDatabase( const String & database )
	{
		if ( !IsConnected() )
		{
			CLogger::LogError( ERROR_MYSQL_NOT_CONNECTED );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_MYSQL_NOT_CONNECTED, __FUNCTION__, __FILE__, __LINE__ );
		}

		if ( _connection )
		{
			_database = database;
			MySQLTry( _connection->setSchema( CStrUtils::ToStr( database ) ), STR( "Database selection" ) );

			CLogger::LogDebug( STR( "Database Metadata" ) );
			CLogger::LogDebug( STR( "-----------------" ) );

			sql::DatabaseMetaData * dbcon_meta = _connection->getMetaData();

			CLogger::LogDebug( StringStream() << STR( "Database Product Name: " ) << dbcon_meta->getDatabaseProductName().c_str() );
			CLogger::LogDebug( StringStream() << STR( "Database Product Version: " ) << dbcon_meta->getDatabaseProductVersion().c_str() );
			CLogger::LogDebug( StringStream() << STR( "Database User Name: " ) << dbcon_meta->getUserName().c_str() );

			CLogger::LogDebug( StringStream() << STR( "Driver name: " ) << dbcon_meta->getDriverName().c_str() );
			CLogger::LogDebug( StringStream() << STR( "Driver version: " ) << dbcon_meta->getDriverVersion().c_str() );

			CLogger::LogDebug( StringStream() << STR( "Database in Read-Only Mode?: " ) << dbcon_meta->isReadOnly() );
			CLogger::LogDebug( StringStream() << STR( "Supports Transactions?: " ) << dbcon_meta->supportsTransactions() );
			CLogger::LogDebug( StringStream() << STR( "Supports DML Transactions only?: " ) << dbcon_meta->supportsDataManipulationTransactionsOnly() );
			CLogger::LogDebug( StringStream() << STR( "Supports Batch Updates?: " ) << dbcon_meta->supportsBatchUpdates() );
			CLogger::LogDebug( StringStream() << STR( "Supports Outer Joins?: " ) << dbcon_meta->supportsOuterJoins() );
			CLogger::LogDebug( StringStream() << STR( "Supports Multiple Transactions?: " ) << dbcon_meta->supportsMultipleTransactions() );
			CLogger::LogDebug( StringStream() << STR( "Supports Named Parameters?: " ) << dbcon_meta->supportsNamedParameters() );
			CLogger::LogDebug( StringStream() << STR( "Supports Statement Pooling?: " ) << dbcon_meta->supportsStatementPooling() );
			CLogger::LogDebug( StringStream() << STR( "Supports Stored Procedures?: " ) << dbcon_meta->supportsStoredProcedures() );
			CLogger::LogDebug( StringStream() << STR( "Supports Union?: " ) << dbcon_meta->supportsUnion() );

			CLogger::LogDebug( StringStream() << STR( "Maximum Connections: " ) << dbcon_meta->getMaxConnections() );
			CLogger::LogDebug( StringStream() << STR( "Maximum Columns per Table: " ) << dbcon_meta->getMaxColumnsInTable() );
			CLogger::LogDebug( StringStream() << STR( "Maximum Columns per Index: " ) << dbcon_meta->getMaxColumnsInIndex() );
			CLogger::LogDebug( StringStream() << STR( "Maximum Row Size per Table: " ) << dbcon_meta->getMaxRowSize() << " bytes" );
			DoExecuteUpdate( STR( "SET NAMES 'utf8';" ), NULL );
		}
	}

	void CDatabaseConnectionMySql::DestroyDatabase( const String & database )
	{
		if ( !IsConnected() )
		{
			CLogger::LogError( ERROR_MYSQL_NOT_CONNECTED );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_MYSQL_NOT_CONNECTED, __FUNCTION__, __FILE__, __LINE__ );
		}

		DoExecuteUpdate( STR( "DROP DATABASE " ) + database, NULL );
	}

	EErrorType CDatabaseConnectionMySql::DoConnect( String & connectionString )
	{
		EErrorType ret = EErrorType_NONE;

		try
		{
			MySQLTry( _connection = _driver->connect( CStrUtils::ToStr( _server ), CStrUtils::ToStr( _userName ), CStrUtils::ToStr( _password ) ), STR( "Connection" ) );
			_statement.reset( _connection->createStatement() );
			DoSetConnected( true );
		}
		catch ( std::exception & exc )
		{
			CLogger::LogError( StringStream() << ERROR_MYSQL_CONNECTION << STR( " - " ) << exc.what() );
			ret = EErrorType_ERROR;
		}
		catch ( ... )
		{
			CLogger::LogError( ERROR_MYSQL_CONNECTION );
			ret = EErrorType_ERROR;
		}

		return ret;
	}

	void CDatabaseConnectionMySql::DoDisconnect()
	{
		DoSetConnected( false );
		_statement.reset();
		delete _connection;
		_connection = NULL;
	}

	bool CDatabaseConnectionMySql::DoExecuteUpdate( const String & query, EErrorType * result )
	{
		if ( !IsConnected() )
		{
			CLogger::LogError( ERROR_MYSQL_NOT_CONNECTED );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_MYSQL_NOT_CONNECTED, __FUNCTION__, __FILE__, __LINE__ );
		}

		return ExecuteUpdate( _statement, query );
	}

	DatabaseResultPtr CDatabaseConnectionMySql::DoExecuteSelect( const String & query, EErrorType * result )
	{
		if ( !IsConnected() )
		{
			CLogger::LogError( ERROR_MYSQL_NOT_CONNECTED );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_MYSQL_NOT_CONNECTED, __FUNCTION__, __FILE__, __LINE__ );
		}

		return ExecuteSelect( _statement, query );
	}

	DatabaseStatementPtr CDatabaseConnectionMySql::DoCreateStatement( const String & request, EErrorType * result )
	{
		DatabaseStatementPtr pReturn;
		EErrorType eResult = EErrorType_ERROR;

		if ( !IsConnected() )
		{
			CLogger::LogError( ERROR_MYSQL_NOT_CONNECTED );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_MYSQL_NOT_CONNECTED, __FUNCTION__, __FILE__, __LINE__ );
		}

		pReturn = std::make_shared< CDatabaseStatementMySql >( shared_from_this(), request );
		eResult = EErrorType_NONE;

		if ( result )
		{
			*result = eResult;
		}

		return pReturn;
	}

	DatabaseQueryPtr CDatabaseConnectionMySql::DoCreateQuery( const String & request, EErrorType * result )
	{
		DatabaseQueryPtr pReturn;
		EErrorType eResult = EErrorType_ERROR;

		if ( !IsConnected() )
		{
			CLogger::LogError( ERROR_MYSQL_NOT_CONNECTED );
			throw CExceptionDatabase( EDatabaseExceptionCodes_ConnectionError, ERROR_MYSQL_NOT_CONNECTED, __FUNCTION__, __FILE__, __LINE__ );
		}

		pReturn = std::make_shared< CDatabaseQuery >( shared_from_this(), request );
		eResult = EErrorType_NONE;

		if ( result )
		{
			*result = eResult;
		}

		return pReturn;
	}

	EFieldType GetFieldTypeFromSqlType( int sqlType )
	{
		static EFieldType types[] =
		{
			EFieldType_NULL,            //!< UNKNOWN = 0
			EFieldType_BOOL,            //!< BIT
			EFieldType_SMALL_INTEGER,   //!< TINYINT
			EFieldType_SMALL_INTEGER,   //!< SMALLINT
			EFieldType_INTEGER,         //!< MEDIUMINT
			EFieldType_INTEGER,         //!< INTEGER
			EFieldType_LONG_INTEGER,    //!< BIGINT
			EFieldType_FLOAT,           //!< REAL
			EFieldType_DOUBLE,          //!< DOUBLE
			EFieldType_DOUBLE,          //!< DECIMAL
			EFieldType_DOUBLE,          //!< NUMERIC
			EFieldType_VARCHAR,         //!< CHAR
			EFieldType_BINARY,          //!< BINARY
			EFieldType_VARCHAR,         //!< VARCHAR
			EFieldType_VARBINARY,       //!< VARBINARY
			EFieldType_TEXT,            //!< LONGVARCHAR
			EFieldType_LONG_VARBINARY,  //!< LONGVARBINARY
			EFieldType_DATETIME,        //!< TIMESTAMP
			EFieldType_DATE,            //!< DATE
			EFieldType_TIME,            //!< TIME
			EFieldType_INTEGER,         //!< YEAR
			EFieldType_NULL,            //!< GEOMETRY
			EFieldType_NULL,            //!< ENUM
			EFieldType_NULL,            //!< SET
			EFieldType_NULL,            //!< SQLNULL
		};

		return types[sqlType];
	}

	DatabaseFieldInfosPtrArray GetColumns( std::shared_ptr< sql::ResultSet > rs, DatabaseConnectionPtr pConnexion )
	{
		DatabaseFieldInfosPtrArray arrayReturn;
		StringArray aColumns;
		String strColumnName;
		sql::ResultSetMetaData * data = rs->getMetaData();
		int iColumnCount = data->getColumnCount();

		for ( int i = 1; i <= iColumnCount; i++ )
		{
			strColumnName = CStrUtils::ToString( data->getColumnName( i ).asStdString() );
			aColumns.push_back( strColumnName );
			arrayReturn.push_back( std::make_shared< CDatabaseFieldInfos >( pConnexion, aColumns[i - 1], GetFieldTypeFromSqlType( data->getColumnType( i ) ), data->getColumnDisplaySize( i ) ) );
		}

		return arrayReturn;
	}

	DatabaseResultPtr Execute( std::shared_ptr< sql::Statement > statement, std::shared_ptr< sql::ResultSet > rs, DatabaseFieldInfosPtrArray const & arrayColumns, DatabaseConnectionPtr pConnexion )
	{
		DatabaseResultPtr pReturn;

		try
		{
			if ( pConnexion->IsConnected() )
			{
				pReturn = std::make_unique< CDatabaseResult >( pConnexion, arrayColumns );
				int iNbColumns = int( arrayColumns.size() );

				while ( rs->next() )
				{
					DatabaseRowPtr pRow = std::make_shared< CDatabaseRow >( pConnexion );

					for ( int i = 1; i <= iNbColumns; i++ )
					{
						DatabaseFieldInfosPtr pInfos;
						DatabaseFieldPtr field;
						bool null = true;

						try
						{
							pInfos = pReturn->GetFieldInfos( i - 1 );
						}
						catch ( CExceptionDatabase & )
						{
							throw;
						}

						field = std::make_shared< CDatabaseField >( pInfos, null, String() );

						MySQLTry( null = rs->isNull( i ), STR( "Is parameter null retrieval" ) );

						if ( !null )
						{
							switch ( pInfos->GetType() )
							{
							case EFieldType_BOOL:
								field->SetValueFast( rs->getBoolean( i ) );
								break;

							case EFieldType_SMALL_INTEGER:
								field->SetValueFast( short( rs->getInt( i ) ) );
								break;

							case EFieldType_INTEGER:
								field->SetValueFast( rs->getInt( i ) );
								break;

							case EFieldType_LONG_INTEGER:
								field->SetValueFast( rs->getInt64( i ) );
								break;

							case EFieldType_FLOAT:
								field->SetValueFast( float( rs->getDouble( i ) ) );
								break;

							case EFieldType_DOUBLE:
								field->SetValueFast( double( rs->getDouble( i ) ) );
								break;

							case EFieldType_VARCHAR:
								field->SetValueFast( std::string( rs->getString( i ) ).c_str() );
								break;

							case EFieldType_TEXT:
								field->SetValueFast( std::string( rs->getString( i ) ) );
								break;

							case EFieldType_NVARCHAR:
								field->SetValueFast( CStrUtils::ToWStr( rs->getString( i ) ).c_str() );
								break;

							case EFieldType_NTEXT:
								field->SetValueFast( CStrUtils::ToWStr( rs->getString( i ) ) );
								break;

							case EFieldType_DATE:
								field->SetValue( rs->getString( i ).asStdString() );
								break;

							case EFieldType_DATETIME:
								field->SetValue( rs->getString( i ).asStdString() );
								break;

							case EFieldType_TIME:
								field->SetValue( rs->getString( i ).asStdString() );
								break;

							case EFieldType_BINARY:
							case EFieldType_VARBINARY:
							case EFieldType_LONG_VARBINARY:
							{
								std::istream * blob = rs->getBlob( i );
								std::vector< uint8_t > in;
								std::copy(
									std::istream_iterator< uint8_t >( *blob ),
									std::istream_iterator< uint8_t >(),
									std::back_inserter( in ) );
								field->SetValueFast( in );
								delete blob;
							}
							break;
							}
						}

						pRow->AddField( field );
					}

					pReturn->AddRow( pRow );
				}

				if ( rs )
				{
					///@remarks Consume the result set
					while ( rs->next() );

					rs.reset();
				}

				///@remarks Flush the statement
				try
				{
					bool hasResult = false;

					while ( hasResult = statement->getMoreResults() )
					{
						rs.reset( statement->getResultSet() );
						CLogger::LogDebug( "Success : Result set retrieval" );
					}
				}
				catch ( sql::SQLException & e )
				{
					StringStream stream;
					stream << "Failure : " << "Get more results" << std::endl;
					stream << "    MySQL Error code : " << e.getErrorCode() << std::endl;
					stream << "    SQL State : " << e.getSQLState().c_str() << std::endl;
					stream << "    Why : " << e.what() << std::endl;
					CLogger::LogDebug( stream.str() );
				}
			}
		}
		catch ( const CExceptionDatabase & e )
		{
			StringStream message;
			message << ERROR_MYSQL_DRIVER << STR( " - " )
					<< e.what();
			CLogger::LogError( message );
			throw CExceptionDatabaseMySql( EDatabaseMySqlExceptionCodes_GenericError, message.str(), __FUNCTION__, __FILE__, __LINE__ );
		}
		catch ( const std::exception & e )
		{
			StringStream message;
			message << ERROR_MYSQL_DRIVER << STR( " - " )
					<< e.what();
			CLogger::LogError( message );
			throw CExceptionDatabaseMySql( EDatabaseMySqlExceptionCodes_GenericError, message.str(), __FUNCTION__, __FILE__, __LINE__ );
		}
		catch ( ... )
		{
			StringStream message;
			message << ERROR_MYSQL_DRIVER << STR( " - " )
					<< ERROR_MYSQL_UNKNOWN;
			CLogger::LogError( message );
			throw CExceptionDatabaseMySql( EDatabaseMySqlExceptionCodes_UnknownError, message.str(), __FUNCTION__, __FILE__, __LINE__ );
		}

		return pReturn;
	}

	DatabaseResultPtr CDatabaseConnectionMySql::DoRetrieveResults( std::shared_ptr< sql::Statement > statement, std::shared_ptr< sql::ResultSet > rs )
	{
		DatabaseResultPtr pResult;

		try
		{
			if ( rs )
			{
				DatabaseConnectionPtr connection = shared_from_this();
				DatabaseFieldInfosPtrArray columns = GetColumns( rs, connection );
				pResult = Execute( statement, rs, columns, connection );
			}
		}
		catch ( std::exception & exc )
		{
			StringStream stream;
			stream << ERROR_MYSQL_EXECUTION_ERROR << exc.what();
			CLogger::LogError( stream.str() );
		}

		return pResult;
	}
}
END_NAMESPACE_DATABASE_MYSQL
