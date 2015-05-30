/************************************************************************//**
 * @file DatabaseResultOdbc.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 *
 *
 * @brief CDatabaseResultOdbc class definition.
 *
 * @details Describes a result for ODBC database.
 *
 ***************************************************************************/

#include "DatabaseOdbcPch.h"

#include "DatabaseResultOdbc.h"
#include "DatabaseOdbcHelper.h"
#include "ExceptionDatabaseOdbc.h"
#include "DatabaseStatementOdbc.h"

#include "DatabaseOdbcHelper.h"
#include "ExceptionDatabaseOdbc.h"

#include <DatabaseField.h>
#include <DatabaseRow.h>

#include <DatabaseStringUtils.h>

BEGIN_NAMESPACE_DATABASE_ODBC
{
	static const String ODBC_DRIVER_ERROR = STR( "ODBC Driver error" );
	static const String ODBC_UNKNOWN_ERROR = STR( "Unknown error encountered while executing query: " );
	static const String ODBC_INCONSISTENCY_ERROR = STR( "Number of columns is less than retrieved data." );

	static const String ODBC_FETCHSCROLL_MSG = STR( "SQLFetchScroll" );
	static const String ODBC_FREESTMT_MSG = STR( "SQLFreeStmt" );
	static const String ODBC_COLATTRIBUTE_DESCLABEL_MSG = STR( "SQLColAttribute - SQL_DESC_LABEL: " );
	static const String ODBC_COLATTRIBUTE_DESCTYPE_MSG = STR( "SQLColAttribute - SQL_DESC_TYPE: " );
	static const String ODBC_COLATTRIBUTE_TYPENAME_MSG = STR( "SQLColAttribute - SQL_TYPE_NAME: " );
	static const String ODBC_BINDCOL_MSG = STR( "SQLBindCol" );
	static const String ODBC_NUMRESULTCOLS_MSG = STR( "SQLNumResultCols" );
	static const String ODBC_ROWCOUNT_MSG = STR( "SQLRowCount" );

	CDatabaseResultOdbc::CDatabaseResultOdbc( DatabaseConnectionPtr connection )
		:   CDatabaseResult( connection )
		,   _statementHandle( NULL )
		,   _rowCount( 0 )
	{
		// Empty
	}

	CDatabaseResultOdbc::~CDatabaseResultOdbc()
	{
		if ( _statementHandle != SQL_NULL_HSTMT )
		{
			int attemptCount = 0;
			EErrorType errorType = EErrorType_NONE;
			SQLRETURN res = 0;

			if ( !_arrayColumnData.empty() )
			{
				do
				{
					res = SQLFetchScroll( _statementHandle, SQL_FETCH_NEXT, 0 );

#if defined( _WIN32 )
					if ( res == SQL_NO_DATA || res == SQL_PARAM_DATA_AVAILABLE )
#else
					if ( res == SQL_NO_DATA )
#endif
					{
						sResultSetFullyFetched( _statementHandle, res );
						_statementHandle = SQL_NULL_HSTMT;
						CLogger::LogDebug( STR( "End of the result set" ) );
					}
					else if ( res != SQL_SUCCESS )
					{
						SqlTry( res, SQL_HANDLE_STMT, _statementHandle, ODBC_FETCHSCROLL_MSG );
					}
				}
				while ( errorType == EErrorType_NONE && res != SQL_NO_DATA );
			}
		}

		// free memory from the binding
		for ( std::size_t i = 0 ; i < _arrayColumnData.size() ; i++ )
		{
			delete [] ( unsigned char * )_arrayColumnData[i].TargetValuePtr;
		}
	}

	void CDatabaseResultOdbc::Initialize( HSTMT statementHandle )
	{
		_rowCount = 0;
		_statementHandle = statementHandle;
		_arrayColumnData.clear();

		static const SQLSMALLINT BUFFER_SIZE = 255;
		int attemptCount;
		int iBufferSize = 1024 * 1024;
		EErrorType errorType = EErrorType_NONE;
		String name;
		uint32_t index = 0;
		SQLSMALLINT numResults;
		SQLLEN numeric;
		char buffer[BUFFER_SIZE];
		DatabaseFieldPtr field;
		SQLSMALLINT numColumns;
		SQLLEN rowCount;
		SqlTry( SQLRowCount( _statementHandle, &rowCount ), SQL_HANDLE_STMT, _statementHandle, ODBC_ROWCOUNT_MSG );

		if ( errorType == EErrorType_NONE )
		{
			_rowCount = uint32_t( rowCount );
			SqlTry( SQLNumResultCols( _statementHandle, &numColumns ), SQL_HANDLE_STMT, _statementHandle, ODBC_NUMRESULTCOLS_MSG );
		}

		if ( errorType == EErrorType_NONE && numColumns )
		{
			_arrayColumnData.resize( numColumns );
			_row = std::make_shared< CDatabaseRow >( _connection );

			// allocate memory for the binding
			for ( SQLSMALLINT i = 0; i < numColumns; i++ )
			{
				SqlTry( SQLColAttributeA( _statementHandle, i + 1, SQL_DESC_LABEL, SQLPOINTER( buffer ), BUFFER_SIZE, &numResults, &numeric ), SQL_HANDLE_STMT, _statementHandle, ODBC_COLATTRIBUTE_DESCLABEL_MSG );

				if ( errorType == EErrorType_NONE )
				{
					name = buffer;
					numResults = 0;
					memset( buffer, 0, sizeof( buffer ) );
					SqlTry( SQLColAttributeA( _statementHandle, i + 1, SQL_DESC_CONCISE_TYPE, SQLPOINTER( buffer ), BUFFER_SIZE, &numResults, &numeric ), SQL_HANDLE_STMT, _statementHandle, ODBC_COLATTRIBUTE_DESCTYPE_MSG );

					if ( errorType == EErrorType_NONE )
					{
						if ( numeric == SQL_NTS )
						{
							SqlTry( SQLColAttributeA( _statementHandle, i + 1, SQL_DESC_TYPE_NAME, SQLPOINTER( buffer ), BUFFER_SIZE, &numResults, &numeric ), SQL_HANDLE_STMT, _statementHandle, ODBC_COLATTRIBUTE_TYPENAME_MSG );

							if ( errorType == EErrorType_NONE )
							{
								AddField( name, buffer );
								field = std::make_shared< CDatabaseField >( GetFieldInfos( i ), true, STR( "" ) );
								_row->AddField( field );
								memset( buffer, 0, sizeof( buffer ) );
							}
						}
						else
						{
							AddField( name, GetFieldConciseType( numeric ) );
							field = std::make_shared< CDatabaseField >( GetFieldInfos( i ), true, STR( "" ) );
							_row->AddField( field );
						}

						// allocate memory for the binding, and set it up
						_arrayColumnData[i].TargetType = SQL_C_WCHAR;
						_arrayColumnData[i].BufferLength = iBufferSize + 1;
						_arrayColumnData[i].TargetValuePtr = new unsigned char[_arrayColumnData[i].BufferLength];
						_arrayColumnData[i].StrLen_or_Ind = 0;
						SqlTry( SQLBindCol( _statementHandle, i + 1, _arrayColumnData[i].TargetType, _arrayColumnData[i].TargetValuePtr, _arrayColumnData[i].BufferLength, &( _arrayColumnData[i].StrLen_or_Ind ) ), SQL_HANDLE_STMT, _statementHandle, ODBC_BINDCOL_MSG );
					}
				}
			}
		}
	}

	uint32_t CDatabaseResultOdbc::GetRowCount()
	{
		return _rowCount;
	}

	DatabaseRowPtr CDatabaseResultOdbc::GetFirstRow()
	{
		return DoGetRow( SQL_FETCH_NEXT );
	}

	DatabaseRowPtr CDatabaseResultOdbc::GetNextRow()
	{
		return DoGetRow( SQL_FETCH_NEXT );
	}

	DatabaseRowPtr CDatabaseResultOdbc::DoGetRow( SQLSMALLINT orientation )
	{
		DatabaseRowPtr pReturn;

		if ( !_arrayColumnData.empty() )
		{
			try
			{
				int attemptCount = 0;
				EErrorType errorType = EErrorType_NONE;
				bool bNull = false;
				std::wstring strFieldValue;
				int iBufferSize = 1024 * 1024;
				SQLRETURN res = SQLFetchScroll( _statementHandle, orientation, 0 );

#if defined( _WIN32 )
				if ( res == SQL_NO_DATA || res == SQL_PARAM_DATA_AVAILABLE )
#else
				if ( res == SQL_NO_DATA )
#endif
				{
					sResultSetFullyFetched( _statementHandle, res );
					_statementHandle = SQL_NULL_HSTMT;
					CLogger::LogDebug( STR( "End of the result set" ) );
				}
				else if ( res != SQL_SUCCESS )
				{
					SqlTry( res, SQL_HANDLE_STMT, _statementHandle, ODBC_FETCHSCROLL_MSG );
				}

				if ( errorType == EErrorType_NONE && _statementHandle != SQL_NULL_HSTMT )
				{
					DatabaseFieldPtr field;
					std::vector< SDataBinding >::iterator it = _arrayColumnData.begin();

					for ( uint32_t i = 0 ; i < _arrayColumnData.size() ; i++ )
					{
						if ( it->StrLen_or_Ind == SQL_NULL_DATA )
						{
							bNull = true;
							strFieldValue.clear();
						}
						else
						{
							assert( it->StrLen_or_Ind < iBufferSize );
							bNull = false;

							if ( it->StrLen_or_Ind >= 0 && it->TargetValuePtr )
							{
								strFieldValue = ( wchar_t * )it->TargetValuePtr;
							}
							else
							{
								strFieldValue.clear();
							}
						}

						field = _row->GetField( i );

						if ( !bNull )
						{
							field->SetStrValue( CStrUtils::ToString( strFieldValue ) );
						}
						else
						{
							field->SetNull();
						}

						++it;
					}

					pReturn = _row;
				}
			}
			catch ( const std::exception & e )
			{
				StringStream message;
				message << ODBC_DRIVER_ERROR << std::endl
						<< e.what();
				CLogger::LogError( message );
				throw CExceptionDatabaseOdbc( EDatabaseOdbcExceptionCodes_GenericError, message.str(), __FUNCTION__, __FILE__, __LINE__ );
			}
			catch ( ... )
			{
				StringStream message;
				message << ODBC_DRIVER_ERROR << std::endl
						<< ODBC_UNKNOWN_ERROR
						<< _query;
				CLogger::LogError( message );
				throw CExceptionDatabaseOdbc( EDatabaseOdbcExceptionCodes_UnknownError, message.str(), __FUNCTION__, __FILE__, __LINE__ );
			}
		}

		return pReturn;
	}
}
END_NAMESPACE_DATABASE_ODBC
