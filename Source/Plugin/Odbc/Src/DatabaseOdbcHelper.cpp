/************************************************************************//**
 * @file DatabaseOdbcHelper.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/18/2014 2:47:39 PM
 *
 *
 * @brief Helper functions for ODBC.
 *
 * @details Helper functions for ODBC.
 *
 ***************************************************************************/

#include "DatabaseOdbcPch.h"

#include "DatabaseOdbcHelper.h"

#include "ExceptionDatabaseOdbc.h"

#include <DatabaseFieldInfos.h>
#include <DatabaseField.h>
#include <DatabaseResult.h>
#include <DatabaseRow.h>
#include <DatabaseException.h>

#include <DatabaseStringUtils.h>

BEGIN_NAMESPACE_DATABASE_ODBC
{
	static const String ODBC_DRIVER_ERROR = STR( "ODBC Driver error" );
	static const String ODBC_QUERY_ERROR = STR( "Error encountered while executing query: " );
	static const String ODBC_UNKNOWN_ERROR = STR( "Unknown error encountered while executing query: " );
	static const String ODBC_INCONSISTENCY_ERROR = STR( "Number of columns is less than retrieved data." );
	static const String ODBC_UNDEFINED_VALUE_TYPE = STR( "Undefined field value type." );
	static const String ODBC_QUERY_SUCCESS = STR( "Success executing action: " );

	static const SQLINTEGER   ODBC_RERUN_TRANSACTION_CODE = 1205;

	static const String ODBC_STATUS_MSG = STR( "Status: " );
	static const String ODBC_NATIVE_ERROR_MSG = STR( "Native error: " );
	static const String ODBC_REQUEST_MSG = STR( "Request: " );

	static const String ODBC_NumResultCols_MSG = STR( "SQLNumResultCols: " );
	static const String ODBC_ColAttributeLabel_MSG = STR( "SQLColAttribute - SQL_DESC_LABEL: " );
	static const String ODBC_ColAttributeDescLength_MSG = STR( "SQLColAttribute - SQL_DESC_TYPE: " );
	static const String ODBC_ColAttributeDescType_MSG = STR( "SQLColAttribute - SQL_DESC_LENGTH: " );
	static const String ODBC_ColAttributeTypeName_MSG = STR( "SQLColAttribute - SQL_TYPE_NAME: " );
	static const String ODBC_BindCol_MSG = STR( "SQLBindCol: " );
	static const String ODBC_Fetch_MSG = STR( "SQLFetch: " );
	static const String ODBC_MoreResults_MSG = STR( "SQLMoreResults" );
	static const String ODBC_RowCount_MSG = STR( "SQLRowCount" );
	static const String ODBC_FreeStmt_MSG = STR( "SQLFreeStmt" );
	static const String ODBC_CloseCursor_MSG = STR( "SQLCloseCursor" );

	static const String ODBC_EXECUTE_REQUEST_MSG = STR( "Execute request: " );

	static const int COLUMN_BUFFER_SIZE = 1024 * 1024;

	namespace
	{
		/** Links the database data during the execution of a SELECT request.
		*/
		struct CInOdbcBindBase
		{
			/** Constructor
			@param targetType
				Data type.
			@param targetValuePtr
				Pointer to the value.
			@param bufferLength
				Size of the buffer containing the value.
			@param strLenOrInd
				String length or indicator.
			*/
			CInOdbcBindBase( SQLSMALLINT targetType, SQLINTEGER bufferLength )
				: _targetType( targetType )
				, _bufferLength( bufferLength )
				, _strLenOrInd( 0 )
				, _targetValuePtr( NULL )
			{
			}

			/// Data type.
			SQLSMALLINT _targetType;
			/// Pointer to the value.
			SQLPOINTER  _targetValuePtr;
			/// Size of the buffer containing the value.
			SQLINTEGER  _bufferLength;
			/// String length or indicator.
			SQLLEN _strLenOrInd;
		};

		/** Template class to link the database data during the execution of a SELECT request.
		*/
		template< typename T, typename U = T >
		struct CInOdbcBind
				: public CInOdbcBindBase
		{
			T _value;

			CInOdbcBind( SQLSMALLINT targetType )
				: CInOdbcBindBase( targetType, sizeof( T ) )
				, _value()
			{
				_targetValuePtr = &_value;
			}

			T const & GetValue()const
			{
				return _value;
			}
		};

		/** Specialisation for bool type
		*/
		template<>
		struct CInOdbcBind< bool, bool >
				: public CInOdbcBindBase
		{
			int8_t _value;

			CInOdbcBind( SQLSMALLINT targetType )
				: CInOdbcBindBase( targetType, 1 )
				, _value()
			{
				_targetValuePtr = &_value;
			}

			bool GetValue()const
			{
				return _value != 0;
			}
		};

		/** Specialisation for pointer types
		*/
		template< typename T >
		struct CInOdbcBind< T *, T * >
				: public CInOdbcBindBase
		{
			std::vector< T > _value;

			CInOdbcBind( SQLSMALLINT targetType, uint32_t limits )
				: CInOdbcBindBase( targetType, limits * sizeof( T ) )
				, _value( limits )
			{
				_targetValuePtr = _value.data();
			}

			T const * GetValue()const
			{
				return _value.data();
			}
		};

		/** Specialisation for pointer types
		*/
		template<>
		struct CInOdbcBind< char *, char * >
				: public CInOdbcBindBase
		{
			std::vector< char > _value;

			CInOdbcBind( SQLSMALLINT targetType, uint32_t limits )
				: CInOdbcBindBase( targetType, limits + 1 )
				, _value( limits + 1 )
			{
				_targetValuePtr = _value.data();
			}

			char const * GetValue()const
			{
				return _value.data();
			}
		};

		/** Specialisation for pointer types
		*/
		template<>
		struct CInOdbcBind< wchar_t *, wchar_t * >
				: public CInOdbcBindBase
		{
			std::vector< wchar_t > _value;

			CInOdbcBind( SQLSMALLINT targetType, uint32_t limits )
				: CInOdbcBindBase( targetType, ( limits + 1 ) )
				, _value( limits + 1 )
			{
				_targetValuePtr = _value.data();
			}

			wchar_t const * GetValue()const
			{
				return _value.data();
			}
		};

		/** Specialisation for double stored as char pointers
		*/
		template<>
		struct CInOdbcBind< char *, double >
				: public CInOdbcBindBase
		{
			char _value[8192];

			CInOdbcBind( SQLSMALLINT targetType )
				: CInOdbcBindBase( targetType, 8192 )
			{
				memset( _value, 0, sizeof( _value ) );
				_targetValuePtr = _value;
			}

			double GetValue()const
			{
				return CStrUtils::ToDouble( _value );
			}
		};

		/** Specialisation for int32_t stored as char pointers
		*/
		template<>
		struct CInOdbcBind< char *, int32_t >
				: public CInOdbcBindBase
		{
			char _value[8192];

			CInOdbcBind( SQLSMALLINT targetType )
				: CInOdbcBindBase( targetType, 8192 )
			{
				memset( _value, 0, sizeof( _value ) );
				_targetValuePtr = _value;
			}

			int32_t GetValue()const
			{
				return CStrUtils::ToInt( _value );
			}
		};

		std::string StringFromOdbcString( CInOdbcBind< char * > const & bind )
		{
			return std::string( bind.GetValue(), bind.GetValue() + bind._strLenOrInd );
		}

		std::wstring StringFromOdbcWString( CInOdbcBind< wchar_t * > const & bind )
		{
			return std::wstring( bind.GetValue(), bind.GetValue() + bind._strLenOrInd );
		}

		std::vector< uint8_t > VectorFromOdbcBinary( CInOdbcBind< uint8_t * > const & bind )
		{
			return std::vector< uint8_t >( bind.GetValue(), bind.GetValue() + bind._strLenOrInd );
		}

		CDate CDateFromOdbcDate( SQL_DATE_STRUCT const & ts )
		{
			return CDate( ts.year, EDateMonth( ts.month - 1 ), ts.day );
		}

		CDateTime CDateTimeFromOdbcTimestamp( SQL_TIMESTAMP_STRUCT const & ts )
		{
			CDateTime dateTime;
			dateTime.SetDateTime( ts.year, EDateMonth( ts.month - 1 ), ts.day, ts.hour, ts.minute, ts.second );
			return dateTime;
		}

		CTime CTimeFromOdbcTime( SQL_TIME_STRUCT const & ts )
		{
			return CTime( ts.hour, ts.minute, ts.second );
		}

		std::unique_ptr< CInOdbcBindBase > GetBindFromConciseType( SQLLEN sqlType, uint32_t limits )
		{
			std::unique_ptr< CInOdbcBindBase > result;

			switch ( sqlType )
			{
			case SQL_CHAR:
			case SQL_VARCHAR:
			case SQL_LONGVARCHAR:
				result = std::make_unique< CInOdbcBind< char * > >( SQL_C_CHAR, limits );
				break;

			case SQL_WCHAR:
			case SQL_WVARCHAR:
			case SQL_WLONGVARCHAR:
				result = std::make_unique< CInOdbcBind< wchar_t * > >( SQL_C_WCHAR, limits );
				break;

			case SQL_FLOAT:
				result = std::make_unique< CInOdbcBind< float > >( SQL_C_FLOAT );
				break;

			case SQL_REAL:
			case SQL_DECIMAL:
			case SQL_DOUBLE:
				result = std::make_unique< CInOdbcBind< double > >( SQL_C_DOUBLE );
				break;

			case SQL_INTEGER:
				result = std::make_unique< CInOdbcBind< int32_t > >( SQL_C_SLONG );
				break;

			case SQL_SMALLINT:
				result = std::make_unique< CInOdbcBind< int16_t > >( SQL_C_SSHORT );
				break;

			case SQL_BIGINT:
				result = std::make_unique< CInOdbcBind< int64_t > >( SQL_C_SBIGINT );
				break;

			case SQL_BIT:
			case SQL_TINYINT:
				result = std::make_unique< CInOdbcBind< bool > >( SQL_C_BIT );
				break;

			case SQL_BINARY:
			case SQL_VARBINARY:
			case SQL_LONGVARBINARY:
				result = std::make_unique< CInOdbcBind< uint8_t * > >( SQL_C_BINARY, limits );
				break;

			case SQL_TYPE_DATE:
			case SQL_INTERVAL_MONTH:
			case SQL_INTERVAL_YEAR:
			case SQL_INTERVAL_YEAR_TO_MONTH:
			case SQL_INTERVAL_DAY:
				result = std::make_unique< CInOdbcBind< DATE_STRUCT > >( SQL_C_TYPE_DATE );
				break;

			case SQL_TYPE_TIME:
			case SQL_INTERVAL_HOUR:
			case SQL_INTERVAL_MINUTE:
			case SQL_INTERVAL_SECOND:
			case SQL_INTERVAL_HOUR_TO_MINUTE:
			case SQL_INTERVAL_HOUR_TO_SECOND:
				result = std::make_unique< CInOdbcBind< TIME_STRUCT > >( SQL_C_TYPE_TIME );
				break;

			case SQL_TYPE_TIMESTAMP:
			case SQL_INTERVAL_DAY_TO_HOUR:
			case SQL_INTERVAL_DAY_TO_MINUTE:
			case SQL_INTERVAL_DAY_TO_SECOND:
				result = std::make_unique< CInOdbcBind< TIMESTAMP_STRUCT > >( SQL_C_TYPE_TIMESTAMP );
				break;
			}

			return result;
		}

		std::unique_ptr< CInOdbcBindBase > GetBindFromFieldType( EFieldType type, uint32_t limits )
		{
			std::unique_ptr< CInOdbcBindBase > result;

			switch ( type )
			{
			case EFieldType_BOOL:
				result = std::make_unique< CInOdbcBind< bool > >( SQL_C_BIT );
				break;

			case EFieldType_SMALL_INTEGER:
				result = std::make_unique< CInOdbcBind< int16_t > >( SQL_C_SSHORT );
				break;

			case EFieldType_INTEGER:
				result = std::make_unique< CInOdbcBind< int32_t > >( SQL_C_SLONG );
				break;

			case EFieldType_LONG_INTEGER:
				result = std::make_unique< CInOdbcBind< int64_t > >( SQL_C_SBIGINT );
				break;

			case EFieldType_FLOAT:
				result = std::make_unique< CInOdbcBind< float > >( SQL_C_FLOAT );
				break;

			case EFieldType_DOUBLE:
				result = std::make_unique< CInOdbcBind< double > >( SQL_C_DOUBLE );
				break;

			case EFieldType_VARCHAR:
			case EFieldType_TEXT:
				result = std::make_unique< CInOdbcBind< char * > >( SQL_C_CHAR, limits );
				break;

			case EFieldType_NVARCHAR:
			case EFieldType_NTEXT:
				result = std::make_unique< CInOdbcBind< char * > >( SQL_C_WCHAR, limits );
				break;

			case EFieldType_DATE:
				result = std::make_unique< CInOdbcBind< DATE_STRUCT > >( SQL_C_TYPE_DATE );
				break;

			case EFieldType_DATETIME:
				result = std::make_unique< CInOdbcBind< TIMESTAMP_STRUCT > >( SQL_C_TYPE_TIMESTAMP );
				break;

			case EFieldType_TIME:
				result = std::make_unique< CInOdbcBind< TIME_STRUCT > >( SQL_C_TYPE_TIME );
				break;

			case EFieldType_BINARY:
			case EFieldType_VARBINARY:
			case EFieldType_LONG_VARBINARY:
				result = std::make_unique< CInOdbcBind< uint8_t * > >( SQL_C_BINARY, limits );
				break;
			}

			return result;
		}

		DatabaseFieldInfosPtrArray InitializeColumns( SQLSMALLINT columnCount, DatabaseConnectionPtr connection, std::vector< std::unique_ptr< CInOdbcBindBase > > & columns, SQLHSTMT stmt )
		{
			static const SQLSMALLINT BUFFER_SIZE = 255;
			EErrorType errorType = EErrorType_NONE;
			DatabaseFieldInfosPtrArray result;

			for ( SQLSMALLINT i = 1; i <= columnCount; ++i )
			{
				TCHAR buffer[BUFFER_SIZE] = { 0 };
				SQLSMALLINT stringLength = 0;
				SQLLEN numericAttribute = 0;
				DatabaseFieldInfosPtr infos;
				std::unique_ptr< CInOdbcBindBase > bind;

				// Retrieeve the column name
				SqlTry( SQLColAttribute( stmt, i, SQL_DESC_LABEL, SQLPOINTER( buffer ), BUFFER_SIZE, &stringLength, &numericAttribute ), SQL_HANDLE_STMT, stmt, ODBC_ColAttributeLabel_MSG );
				String name = CStrUtils::ToString( buffer );

				// Its length
				std::memset( buffer, 0, BUFFER_SIZE );
				stringLength = 0;
				numericAttribute = 0;
				SqlTry( SQLColAttribute( stmt, i, SQL_DESC_LENGTH, SQLPOINTER( buffer ), BUFFER_SIZE, &stringLength, &numericAttribute ), SQL_HANDLE_STMT, stmt, ODBC_ColAttributeDescLength_MSG );
				uint32_t limits = numericAttribute;

				// The column type
				std::memset( buffer, 0, BUFFER_SIZE );
				stringLength = 0;
				numericAttribute = 0;
				SqlTry( SQLColAttribute( stmt, i, SQL_DESC_CONCISE_TYPE, SQLPOINTER( buffer ), BUFFER_SIZE, &stringLength, &numericAttribute ), SQL_HANDLE_STMT, stmt, ODBC_ColAttributeDescType_MSG );

				if ( numericAttribute == SQL_NTS )
				{
					// The type name
					std::memset( buffer, 0, BUFFER_SIZE );
					stringLength = 0;
					numericAttribute = 0;
					SqlTry( SQLColAttribute( stmt, i, SQL_DESC_TYPE_NAME, SQLPOINTER( buffer ), BUFFER_SIZE, &stringLength, &numericAttribute ), SQL_HANDLE_STMT, stmt, ODBC_ColAttributeTypeName_MSG );

					if ( errorType == EErrorType_NONE )
					{
						infos = std::make_shared< CDatabaseFieldInfos >( connection, name, buffer );
						bind = GetBindFromFieldType( infos->GetType(), limits );
					}
				}
				else
				{
					bind = GetBindFromConciseType( numericAttribute, limits );
					infos = std::make_shared< CDatabaseFieldInfos >( connection, name, GetFieldTypeFromConciseType( numericAttribute ), limits );
				}

				SqlTry( SQLBindCol( stmt, i, bind->_targetType, bind->_targetValuePtr, bind->_bufferLength, &( bind->_strLenOrInd ) ), SQL_HANDLE_STMT, stmt, ODBC_BindCol_MSG );
				columns.push_back( std::move( bind ) );
				result.push_back( infos );
			}

			return result;
		}

		void SetFieldValue( EFieldType type, CDatabaseValueBase & value, CInOdbcBindBase const & bind )
		{
			switch ( type )
			{
			case EFieldType_BOOL:
				static_cast< CDatabaseValue< EFieldType_BOOL > & >( value ).SetValue( static_cast< CInOdbcBind< bool > const & >( bind ).GetValue() != 0 );
				break;

			case EFieldType_SMALL_INTEGER:
				static_cast< CDatabaseValue< EFieldType_SMALL_INTEGER > & >( value ).SetValue( static_cast< CInOdbcBind< int16_t > const & >( bind ).GetValue() );
				break;

			case EFieldType_INTEGER:
				static_cast< CDatabaseValue< EFieldType_INTEGER > & >( value ).SetValue( static_cast< CInOdbcBind< int32_t > const & >( bind ).GetValue() );
				break;

			case EFieldType_LONG_INTEGER:
				static_cast< CDatabaseValue< EFieldType_LONG_INTEGER > & >( value ).SetValue( static_cast< CInOdbcBind< int64_t > const & >( bind ).GetValue() );
				break;

			case EFieldType_FLOAT:
				static_cast< CDatabaseValue< EFieldType_FLOAT > & >( value ).SetValue( static_cast< CInOdbcBind< float > const & >( bind ).GetValue() );
				break;

			case EFieldType_DOUBLE:
				static_cast< CDatabaseValue< EFieldType_DOUBLE > & >( value ).SetValue( static_cast< CInOdbcBind< double > const & >( bind ).GetValue() );
				break;

			case EFieldType_VARCHAR:
				static_cast< CDatabaseValue< EFieldType_VARCHAR > & >( value ).SetValue( StringFromOdbcString( static_cast< CInOdbcBind< char * > const & >( bind ) ) );
				break;

			case EFieldType_TEXT:
				static_cast< CDatabaseValue< EFieldType_TEXT > & >( value ).SetValue( StringFromOdbcString( static_cast< CInOdbcBind< char * > const & >( bind ) ) );
				break;

			case EFieldType_NVARCHAR:
				static_cast< CDatabaseValue< EFieldType_NVARCHAR > & >( value ).SetValue( StringFromOdbcWString( static_cast< CInOdbcBind< wchar_t * > const & >( bind ) ) );
				break;

			case EFieldType_NTEXT:
				static_cast< CDatabaseValue< EFieldType_NTEXT > & >( value ).SetValue( StringFromOdbcWString( static_cast< CInOdbcBind< wchar_t * > const & >( bind ) ) );
				break;

			case EFieldType_DATE:
				static_cast< CDatabaseValue< EFieldType_DATE > & >( value ).SetValue( CDateFromOdbcDate( static_cast< CInOdbcBind< DATE_STRUCT > const & >( bind ).GetValue() ) );
				break;

			case EFieldType_DATETIME:
				static_cast< CDatabaseValue< EFieldType_DATETIME > & >( value ).SetValue( CDateTimeFromOdbcTimestamp( static_cast< CInOdbcBind< TIMESTAMP_STRUCT > const & >( bind ).GetValue() ) );
				break;

			case EFieldType_TIME:
				static_cast< CDatabaseValue< EFieldType_TIME > & >( value ).SetValue( CTimeFromOdbcTime( static_cast< CInOdbcBind< TIME_STRUCT > const & >( bind ).GetValue() ) );
				break;

			case EFieldType_BINARY:
				static_cast< CDatabaseValue< EFieldType_BINARY > & >( value ).SetValue( VectorFromOdbcBinary( static_cast< CInOdbcBind< uint8_t * > const & >( bind ) ) );
				break;

			case EFieldType_VARBINARY:
				static_cast< CDatabaseValue< EFieldType_VARBINARY > & >( value ).SetValue( VectorFromOdbcBinary( static_cast< CInOdbcBind< uint8_t * > const & >( bind ) ) );
				break;

			case EFieldType_LONG_VARBINARY:
				static_cast< CDatabaseValue< EFieldType_LONG_VARBINARY > & >( value ).SetValue( VectorFromOdbcBinary( static_cast< CInOdbcBind< uint8_t * > const & >( bind ) ) );
				break;

			default:
				break;
			}
		}

		EErrorType FetchResultSet( DatabaseConnectionPtr connection, DatabaseResultPtr results, std::vector< std::unique_ptr< CInOdbcBindBase > > & bindings, SQLHSTMT statementHandle )
		{
			EErrorType errorType = EErrorType_NONE;
			SQLRETURN res;
			SQLLEN rowCount = 0;
			SqlTry( SQLRowCount( statementHandle, &rowCount ), SQL_HANDLE_STMT, statementHandle, ODBC_RowCount_MSG );

			if ( errorType == EErrorType_NONE && rowCount )
			{
				try
				{
					res = SQLFetch( statementHandle );

					if ( res != SQL_NO_DATA && res != SQL_SUCCESS )
					{
						SqlTry( res, SQL_HANDLE_STMT, statementHandle, ODBC_Fetch_MSG );
					}
					else if ( res == SQL_NO_DATA )
					{
						CLogger::LogDebug( STR( "Void result" ) );
					}

					while ( errorType == EErrorType_NONE && res != SQL_NO_DATA )
					{
						DatabaseRowPtr row = std::make_shared< CDatabaseRow >( connection );
						uint32_t index = 0;

						for ( auto && bind : bindings )
						{
							bool isNull = bind->_strLenOrInd == SQL_NULL_DATA;
							DatabaseFieldPtr field;

							try
							{
								field = std::make_shared< CDatabaseField >( results->GetFieldInfos( index++ ) );

								if ( !isNull )
								{
									SetFieldValue( field->GetType(), field->GetObjectValue(), *bind );
								}
							}
							catch ( const CExceptionDatabase & e )
							{
								StringStream message;
								message << ODBC_DRIVER_ERROR << std::endl;
								message << e.what();
								CLogger::LogError( message );
								throw CExceptionDatabaseOdbc( EDatabaseOdbcExceptionCodes_GenericError, message.str(), __FUNCTION__, __FILE__, __LINE__ );
							}

							row->AddField( field );
						}

						results->AddRow( row );

						res = SQLFetch( statementHandle );

						if ( res != SQL_NO_DATA && res != SQL_SUCCESS )
						{
							SqlTry( res, SQL_HANDLE_STMT, statementHandle, ODBC_Fetch_MSG );
						}
					}

					// free memory from the binding
					SqlTry( SQLFreeStmt( statementHandle, SQL_UNBIND ), SQL_HANDLE_STMT, statementHandle, ODBC_FreeStmt_MSG );
				}
				catch ( const std::exception & e )
				{
					StringStream message;
					message << ODBC_DRIVER_ERROR << std::endl;
					message << e.what();
					CLogger::LogError( message );
					throw CExceptionDatabaseOdbc( EDatabaseOdbcExceptionCodes_GenericError, message.str(), __FUNCTION__, __FILE__, __LINE__ );
				}
				catch ( ... )
				{
					StringStream message;
					message << ODBC_DRIVER_ERROR << std::endl;
					message << ODBC_UNKNOWN_ERROR;
					CLogger::LogError( message );
					throw CExceptionDatabaseOdbc( EDatabaseOdbcExceptionCodes_UnknownError, message.str(), __FUNCTION__, __FILE__, __LINE__ );
				}
			}

			return errorType;
		}
	}

	EErrorType SqlError( SQLSMALLINT typeHandle, SQLHANDLE handle, const String & query, bool error )
	{
		EErrorType errorType = EErrorType_ERROR;
		SqlChar sqlState[10];
		SqlChar sqlMessage[1024 + 1];
		sqlMessage[1024] = 0;

		SQLSMALLINT sqlMessageSize = 0;
		SQLINTEGER nativeError = 0;
		SQLSMALLINT errorCount = 1;
		SQLRETURN sqlReturn;

		while ( ( sqlReturn = SQLGetDiagRecA( typeHandle, handle, errorCount, sqlState, &nativeError, sqlMessage, 1024, &sqlMessageSize ) ) == SQL_SUCCESS )
		{
			errorCount++;
			String message = ( char * )sqlMessage;
			String state = ( char * )sqlState;
			StringStream logMessage;
			logMessage << ODBC_STATUS_MSG << state
					   << STR( ", " )
					   << ODBC_NATIVE_ERROR_MSG << nativeError
					   << STR( ", " ) << sqlMessage
					   << STR( ", " )
					   << ODBC_REQUEST_MSG << query;

			if ( nativeError == ODBC_RERUN_TRANSACTION_CODE )
			{
				errorType = EErrorType_RETRY;
				CLogger::LogWarning( logMessage.str() );
			}
			else
			{
				if ( error )
				{
					CLogger::LogError( logMessage.str() );
				}
				else
				{
					CLogger::LogWarning( logMessage.str() );
				}
			}
		}

		if ( ( errorCount == 2 && errorType != EErrorType_RETRY )
				|| ( errorCount > 2 && errorCount == EErrorType_RETRY ) )
		{
			StringStream logMessage;
			logMessage << ODBC_QUERY_ERROR
					   << query;

			if ( error )
			{
				CLogger::LogError( logMessage.str() );
			}
			else
			{
				CLogger::LogWarning( logMessage.str() );
			}
		}
		else if ( sqlReturn == SQL_NO_DATA )
		{
			errorType = EErrorType_NONE;
		}

		return errorType;
	}

	EErrorType SqlSuccess( SQLRETURN rc, SQLSMALLINT typeHandle, SQLHANDLE handle, const String & query )
	{
		EErrorType errorType = EErrorType_ERROR;

		if ( rc == SQL_SUCCESS )
		{
			errorType = EErrorType_NONE;

			StringStream logMessage;
			logMessage << ODBC_QUERY_SUCCESS
					   << query;
			CLogger::LogDebug( logMessage.str() );
		}
		else if ( rc == SQL_SUCCESS_WITH_INFO )
		{
			SqlError( typeHandle, handle, query, false );
			errorType = EErrorType_NONE;
		}
		else if ( typeHandle && handle )
		{
			errorType = SqlError( typeHandle, handle, query, true );
		}

		return errorType;
	}

	EFieldType GetFieldTypeFromConciseType( SQLLEN sqlType )
	{
		EFieldType fieldType = EFieldType_NULL;

		switch ( sqlType )
		{
		case SQL_CHAR:
		case SQL_VARCHAR:
			fieldType = EFieldType_VARCHAR;
			break;

		case SQL_WCHAR:
		case SQL_WVARCHAR:
			fieldType = EFieldType_NVARCHAR;
			break;

		case SQL_LONGVARCHAR:
			fieldType = EFieldType_TEXT;
			break;

		case SQL_WLONGVARCHAR:
			fieldType = EFieldType_NTEXT;
			break;

		case SQL_FLOAT:
		case SQL_REAL:
			fieldType = EFieldType_FLOAT;
			break;

		case SQL_DECIMAL:
		case SQL_DOUBLE:
			fieldType = EFieldType_DOUBLE;
			break;

		case SQL_INTEGER:
			fieldType = EFieldType_INTEGER;
			break;

		case SQL_SMALLINT:
			fieldType = EFieldType_SMALL_INTEGER;
			break;

		case SQL_BIGINT:
			fieldType = EFieldType_LONG_INTEGER;
			break;

		case SQL_TINYINT:
		case SQL_BIT:
			fieldType = EFieldType_BOOL;
			break;

		case SQL_BINARY:
			fieldType = EFieldType_BINARY;
			break;

		case SQL_VARBINARY:
			fieldType = EFieldType_VARBINARY;
			break;

		case SQL_LONGVARBINARY:
			fieldType = EFieldType_LONG_VARBINARY;
			break;

		case SQL_TYPE_DATE:
		case SQL_INTERVAL_MONTH:
		case SQL_INTERVAL_YEAR:
		case SQL_INTERVAL_YEAR_TO_MONTH:
		case SQL_INTERVAL_DAY:
			fieldType = EFieldType_DATE;
			break;

		case SQL_TYPE_TIME:
		case SQL_INTERVAL_HOUR:
		case SQL_INTERVAL_MINUTE:
		case SQL_INTERVAL_SECOND:
		case SQL_INTERVAL_HOUR_TO_MINUTE:
		case SQL_INTERVAL_HOUR_TO_SECOND:
			fieldType = EFieldType_TIME;
			break;

		case SQL_TYPE_TIMESTAMP:
		case SQL_INTERVAL_DAY_TO_HOUR:
		case SQL_INTERVAL_DAY_TO_MINUTE:
		case SQL_INTERVAL_DAY_TO_SECOND:
			fieldType = EFieldType_DATETIME;
			break;

		default:
			fieldType = EFieldType_NULL;
			break;
		}

		return fieldType;
	}

	EErrorType SqlExecute( DatabaseConnectionPtr connection, SQLHSTMT statementHandle, FuncResultSetFullyFetched onFullyfetched, DatabaseResultPtr & pReturn )
	{
		SQLRETURN res = SQL_SUCCESS;
		EErrorType errorType = EErrorType_NONE;

		do
		{
			SQLSMALLINT numColumns;
			SqlTry( SQLNumResultCols( statementHandle, &numColumns ), SQL_HANDLE_STMT, statementHandle, ODBC_NumResultCols_MSG );

			if ( numColumns )
			{
				std::vector< std::unique_ptr< CInOdbcBindBase > > arrayColumnData;
				DatabaseFieldInfosPtrArray columns = InitializeColumns( numColumns, connection, arrayColumnData, statementHandle );
				pReturn = std::make_shared< CDatabaseResult >( connection, columns );
				FetchResultSet( connection, pReturn, arrayColumnData, statementHandle );
			}

			res = SQLMoreResults( statementHandle );
			SqlTry( res, SQL_HANDLE_STMT, statementHandle, ODBC_MoreResults_MSG );

			if ( res != SQL_NO_DATA && errorType == EErrorType_NONE )
			{
				CLogger::LogMessage( STR( "Additional result detected" ) );
			}
			else
			{
				onFullyfetched( statementHandle, res );
				EErrorType errorType = EErrorType_NONE;
				SqlTry( SQLCloseCursor( statementHandle ), SQL_HANDLE_STMT, statementHandle, ODBC_CloseCursor_MSG );
				SqlTry( SQLFreeStmt( statementHandle, SQL_CLOSE ), SQL_HANDLE_STMT, statementHandle, ODBC_FreeStmt_MSG + STR( " (Close)" ) );
				SqlTry( SQLFreeStmt( statementHandle, SQL_UNBIND ), SQL_HANDLE_STMT, statementHandle, ODBC_FreeStmt_MSG + STR( " (Unbind)" ) );
				SqlTry( SQLFreeStmt( statementHandle, SQL_RESET_PARAMS ), SQL_HANDLE_STMT, statementHandle, ODBC_FreeStmt_MSG + STR( " (ResetParams)" ) );
			}
		}

#if defined( _WIN32 )

		while ( errorType == EErrorType_NONE && res != SQL_NO_DATA && res != SQL_PARAM_DATA_AVAILABLE );

#else

		while ( errorType == EErrorType_NONE && res != SQL_NO_DATA );

#endif

		return errorType;
	}
}
END_NAMESPACE_DATABASE_ODBC
