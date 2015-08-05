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

#include <DatabaseValuedObjectInfos.h>
#include <DatabaseField.h>
#include <DatabaseResult.h>
#include <DatabaseRow.h>
#include <DatabaseException.h>

#include <DatabaseStringUtils.h>

BEGIN_NAMESPACE_DATABASE_ODBC
{
	static const String ERROR_ODBC_QUERY = STR( "Error encountered while executing query: " );
	static const String ERROR_ODBC_UNKNOWN = STR( "Unknown error encountered while executing query: " );
	static const String ERROR_ODBC_INCONSISTENCY = STR( "Number of columns is less than retrieved data." );
	static const String ERROR_ODBC_UNDEFINED_VALUE_TYPE = STR( "Undefined field value type." );

	static const String INFO_ODBC_QUERY_SUCCESS = STR( "Success executing action: " );
	static const String INFO_ODBC_STATUS = STR( "Status: " );
	static const String INFO_ODBC_NATIVE = STR( "Native error: " );
	static const String INFO_ODBC_REQUEST = STR( "Request: " );
	static const String INFO_ODBC_EXECUTE_REQUEST = STR( "Execute request: " );

	static const String INFO_ODBC_NumResultCols = STR( "SQLNumResultCols: " );
	static const String INFO_ODBC_ColAttribute = STR( "SQLColAttribute: " );
	static const String INFO_ODBC_BindCol = STR( "SQLBindCol: " );
	static const String INFO_ODBC_Fetch = STR( "SQLFetch: " );
	static const String INFO_ODBC_MoreResults = STR( "SQLMoreResults" );
	static const String INFO_ODBC_RowCount = STR( "SQLRowCount" );
	static const String INFO_ODBC_FreeStmt = STR( "SQLFreeStmt" );
	static const String INFO_ODBC_CloseCursor = STR( "SQLCloseCursor" );
	static const String INFO_ODBC_GetStmtAttr = STR( "SQLGetStmtAttr: " );
	static const String INFO_ODBC_SetDescRec = STR( "SQLSetDescRec: " );
	static const String INFO_ODBC_SetDescField = STR( "SQLSetDescField: " );

	static const String ODBC_OPTION_DESC_TYPE = STR( "DESC_TYPE" );
	static const String ODBC_OPTION_DESC_PRECISION = STR( "DESC_PRECISION" );
	static const String ODBC_OPTION_DESC_SCALE = STR( "DESC_SCALE" );
	static const String ODBC_OPTION_DESC_LABEL = STR( "DESC_LABEL" );
	static const String ODBC_OPTION_DESC_CONCISE_TYPE = STR( "DESC_CONCISE_TYPE" );
	static const String ODBC_OPTION_DESC_LENGTH = STR( "DESC_LENGTH" );
	static const String ODBC_OPTION_DESC_TYPE_NAME = STR( "DESC_TYPE_NAME" );

	static const String DEBUG_ODBC_EMPTY_RESULT = STR( "Empty result" );

	static const SQLINTEGER ODBC_RERUN_TRANSACTION_CODE = 1205;
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
			@param bufferLength
				Size of the buffer containing the value.
			*/
			CInOdbcBindBase( SQLSMALLINT targetType, SQLINTEGER bufferLength )
				: _targetType( targetType )
				, _bufferLength( bufferLength )
				, _strLenOrInd( 0 )
				, _targetValuePtr( NULL )
			{
			}

			/** Initialised the column binding
			@param stmt
				The statement.
			@param index
				The column index.
			@param desc
				The row descriptor
			*/
			virtual EErrorType Initialise( HSTMT stmt, SQLUSMALLINT index, SQLHDESC desc )
			{
				EErrorType errorType = EErrorType_NONE;
				OdbcCheck( SQLBindCol( stmt, index, _targetType, _targetValuePtr, _bufferLength, &( _strLenOrInd ) ), SQL_HANDLE_STMT, stmt, INFO_ODBC_BindCol );
				return errorType;
			}

			virtual void ResetValue()
			{
			}

			//! Data type.
			SQLSMALLINT _targetType;
			//! Pointer to the value.
			SQLPOINTER  _targetValuePtr;
			//! Size of the buffer containing the value.
			SQLINTEGER  _bufferLength;
			//! String length or indicator.
			SQLLEN _strLenOrInd;
		};

		/** Template class to link the database data during the execution of a SELECT request.
		*/
		template< typename T, typename U = T >
		struct CInOdbcBind
			: public CInOdbcBindBase
		{
			T _value;

			/** Constructor
			@param targetType
				Data type.
			*/
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

			/** Constructor
			@param targetType
				Data type.
			*/
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

			/** Constructor
			@param targetType
				Data type.
			@param limits
				Data limits.
			*/
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

			/** Constructor
			@param targetType
				Data type.
			@param limits
				Data limits.
			*/
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

		/** Specialisation for CHAR fields
		*/
		template<>
		struct CInOdbcBind< char *, uint32_t >
			: public CInOdbcBindBase
		{
			std::vector< char > _value;

			/** Constructor
			@param targetType
				Data type.
			@param stmt
				The statement.
			@param index
				The column index.
			*/
			CInOdbcBind( SQLSMALLINT targetType, uint32_t limits )
				: CInOdbcBindBase( targetType, limits + 1 )
				, _value( limits + 1 )
			{
				_targetValuePtr = _value.data();
			}

			std::string GetValue()const
			{
				std::string ret = _value.data();

				if ( ret.size() < _value.size() - 1 )
				{
					std::stringstream stream;
					stream.width( _value.size() - 1 );
					stream << std::left << ret;
					ret = stream.str();
				}

				return ret;
			}
		};

		/** Specialisation for pointer types
		*/
		template<>
		struct CInOdbcBind< wchar_t *, wchar_t * >
			: public CInOdbcBindBase
		{
			std::vector< wchar_t > _value;

			/** Constructor
			@param targetType
				Data type.
			@param limits
				Data limits.
			*/
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

		/** Specialisation for NCHAR fields
		*/
		template<>
		struct CInOdbcBind< wchar_t *, uint32_t >
			: public CInOdbcBindBase
		{
			std::vector< wchar_t > _value;

			/** Constructor
			@param targetType
				Data type.
			@param stmt
				The statement.
			@param index
				The column index.
			*/
			CInOdbcBind( SQLSMALLINT targetType, uint32_t limits )
				: CInOdbcBindBase( targetType, limits + 1 )
				, _value( limits + 1 )
			{
				_targetValuePtr = _value.data();
			}

			std::wstring GetValue()const
			{
				std::wstring ret = _value.data();

				if ( ret.size() < _value.size() - 1 )
				{
					std::wstringstream stream;
					stream.width( _value.size() - 1 );
					stream << std::left << ret;
					ret = stream.str();
				}

				return ret;
			}
		};

		/** Specialisation for double stored as char pointers
		*/
		template<>
		struct CInOdbcBind< char *, double >
			: public CInOdbcBindBase
		{
			char _value[8192];

			/** Constructor
			@param targetType
				Data type.
			*/
			CInOdbcBind( SQLSMALLINT targetType )
				: CInOdbcBindBase( targetType, 8192 )
			{
				memset( _value, 0, sizeof( _value ) );
				_targetValuePtr = _value;
			}

			double GetValue()const
			{
				return std::stod( _value );
			}
		};

		/** Specialisation for int32_t stored as char pointers
		*/
		template<>
		struct CInOdbcBind< char *, int32_t >
			: public CInOdbcBindBase
		{
			char _value[8192];

			/** Constructor
			@param targetType
				Data type.
			@param stmt
				The statement.
			@param index
				The column index.
			*/
			CInOdbcBind( SQLSMALLINT targetType )
				: CInOdbcBindBase( targetType, 8192 )
			{
				memset( _value, 0, sizeof( _value ) );
				_targetValuePtr = _value;
			}

			int32_t GetValue()const
			{
				return std::stoi( _value );
			}
		};

		/** Specialisation for int32_t stored as char pointers
		*/
		template<>
		struct CInOdbcBind< SQL_NUMERIC_STRUCT, CFixedPoint >
			: public CInOdbcBindBase
		{
			mutable SQL_NUMERIC_STRUCT _value;
			HSTMT _statement;
			SQLUSMALLINT _index;
			SQLSMALLINT _precision;
			SQLSMALLINT _scale;

			/** Constructor
			@param targetType
				Data type.
			@param precision
				The precision (total digits count).
			@param scale
				The scale (decimale count).
			*/
			CInOdbcBind( SQLSMALLINT targetType, uint32_t precision, uint32_t scale )
				: CInOdbcBindBase( targetType, 8192 )
				, _precision( precision )
				, _scale( scale )
			{
				_targetValuePtr = &_value;
			}

			/** Initialised the column binding
			@param stmt
				The statement.
			@param index
				The column index.
			@param desc
				The row descriptor
			*/
			EErrorType Initialise( HSTMT stmt, SQLUSMALLINT index, SQLHDESC desc )
			{
				EErrorType errorType = EErrorType_NONE;
				_statement = stmt;
				_index = index;
				SQLLEN precision = _precision;
				SQLLEN scale = _scale;

				OdbcCheck( SQLSetDescField( desc, index, SQL_DESC_TYPE, SQLPOINTER( SQL_C_NUMERIC ), 0 ), SQL_HANDLE_DESC, desc, INFO_ODBC_SetDescField + ODBC_OPTION_DESC_TYPE );
				OdbcCheck( SQLSetDescField( desc, index, SQL_DESC_PRECISION, SQLPOINTER( precision ), 0 ), SQL_HANDLE_DESC, desc, INFO_ODBC_SetDescField + ODBC_OPTION_DESC_PRECISION );
				OdbcCheck( SQLSetDescField( desc, index, SQL_DESC_SCALE, SQLPOINTER( scale ), 0 ), SQL_HANDLE_DESC, desc, INFO_ODBC_SetDescField + ODBC_OPTION_DESC_SCALE );
				return errorType;
			}

			void ResetValue()
			{
				memset( ( void * )&_value, 0, sizeof( _value ) );
			}

			CFixedPoint GetValue()const
			{
				SQLLEN length = sizeof( _value );
				EErrorType errorType = EErrorType_NONE;
				OdbcCheck( SQLGetData( _statement, _index, SQL_ARD_TYPE, &_value, 19, &length ), SQL_HANDLE_STMT, _statement, INFO_ODBC_BindCol );
				int64_t value = *( int64_t * )( _value.val );

				if ( !_value.sign )
				{
					value = -value;
				}

				return CFixedPoint( value, _value.precision, _value.scale );
			}
		};

		std::string StringFromOdbcString( CInOdbcBind< char * > const & bind )
		{
			return std::string( bind.GetValue(), bind.GetValue() + std::min< size_t >( strlen( bind.GetValue() ), bind._strLenOrInd ) );
		}

		std::wstring StringFromOdbcWString( CInOdbcBind< wchar_t * > const & bind )
		{
			return std::wstring( bind.GetValue(), bind.GetValue() + std::min< size_t >( wcslen( bind.GetValue() ), bind._strLenOrInd ) );
		}

		ByteArray VectorFromOdbcBinary( CInOdbcBind< uint8_t * > const & bind )
		{
			return ByteArray( bind.GetValue(), bind.GetValue() + bind._strLenOrInd );
		}

		DateType DateFromOdbcDate( SQL_DATE_STRUCT const & ts )
		{
			return DateType( ts.year, ts.month, ts.day );
		}

		DateTimeType DateTimeFromOdbcTimestamp( SQL_TIMESTAMP_STRUCT const & ts )
		{
			DateTimeType dateTime( DateType( ts.year, ts.month, ts.day ), TimeType( ts.hour, ts.minute, ts.second ) );
			return dateTime;
		}

		TimeType TimeFromOdbcTime( SQL_TIME_STRUCT const & ts )
		{
			return TimeType( ts.hour, ts.minute, ts.second );
		}

		std::unique_ptr< CInOdbcBindBase > GetBindFromFieldType( EFieldType type, uint32_t limits, uint32_t precision, uint32_t scale )
		{
			std::unique_ptr< CInOdbcBindBase > result;

			switch ( type )
			{
			case EFieldType_BIT:
				result = std::make_unique< CInOdbcBind< bool > >( SQL_C_BIT );
				break;

			case EFieldType_SINT8:
				result = std::make_unique< CInOdbcBind< int8_t > >( SQL_C_STINYINT );
				break;

			case EFieldType_SINT16:
				result = std::make_unique< CInOdbcBind< int16_t > >( SQL_C_SSHORT );
				break;

			case EFieldType_SINT24:
				result = std::make_unique< CInOdbcBind< int32_t > >( SQL_C_SLONG );
				break;

			case EFieldType_SINT32:
				result = std::make_unique< CInOdbcBind< int32_t > >( SQL_C_SLONG );
				break;

			case EFieldType_SINT64:
				result = std::make_unique< CInOdbcBind< int64_t > >( SQL_C_SBIGINT );
				break;

			case EFieldType_UINT8:
				result = std::make_unique< CInOdbcBind< int16_t > >( SQL_C_UTINYINT );
				break;

			case EFieldType_UINT16:
				result = std::make_unique< CInOdbcBind< int16_t > >( SQL_C_USHORT );
				break;

			case EFieldType_UINT24:
				result = std::make_unique< CInOdbcBind< int32_t > >( SQL_C_ULONG );
				break;

			case EFieldType_UINT32:
				result = std::make_unique< CInOdbcBind< int32_t > >( SQL_C_ULONG );
				break;

			case EFieldType_UINT64:
				result = std::make_unique< CInOdbcBind< int64_t > >( SQL_C_UBIGINT );
				break;

			case EFieldType_FLOAT32:
				result = std::make_unique< CInOdbcBind< float > >( SQL_C_FLOAT );
				break;

			case EFieldType_FLOAT64:
				result = std::make_unique< CInOdbcBind< double > >( SQL_C_DOUBLE );
				break;

			case EFieldType_FIXED_POINT:
				result = std::make_unique< CInOdbcBind< SQL_NUMERIC_STRUCT, CFixedPoint > >( SQL_C_NUMERIC, precision, scale );
				break;

			case EFieldType_CHAR:
				result = std::make_unique< CInOdbcBind< char *, uint32_t > >( SQL_C_CHAR, limits );
				break;

			case EFieldType_VARCHAR:
			case EFieldType_TEXT:
				result = std::make_unique< CInOdbcBind< char * > >( SQL_C_CHAR, limits );
				break;

			case EFieldType_NCHAR:
				result = std::make_unique< CInOdbcBind< wchar_t *, uint32_t > >( SQL_C_WCHAR, limits );
				break;

			case EFieldType_NVARCHAR:
			case EFieldType_NTEXT:
				result = std::make_unique< CInOdbcBind< wchar_t * > >( SQL_C_WCHAR, limits );
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
			case EFieldType_BLOB:
				result = std::make_unique< CInOdbcBind< uint8_t * > >( SQL_C_BINARY, limits );
				break;
			}

			return result;
		}

		std::pair< int, int > RetrieveLimits( String const & type )
		{
			std::pair< int, int > result( -1, -1 );
			size_t index = type.find( STR( "(" ) );

			if ( index != String::npos )
			{
				size_t dotIndex = type.find( STR( "," ), index );

				if ( dotIndex == String::npos )
				{
					String limit = type.substr( index + 1, type.find( STR( ")" ) ) - index );
					result.first = std::stoi( StringUtils::Trim( limit ) );
				}
				else
				{
					String limit1 = type.substr( index + 1, dotIndex - index );
					result.first = std::stoi( StringUtils::Trim( limit1 ) );
					String limit2 = type.substr( dotIndex + 1, type.find( STR( ")" ) ) - dotIndex );
					result.second = std::stoi( StringUtils::Trim( limit2 ) );
				}
			}

			return result;
		}

		EFieldType GetFieldTypeFromTypeName( const String & type, std::pair< uint32_t, uint32_t > & limprec )
		{
			String strTypel = StringUtils::UpperCase( type );
			size_t index;
			EFieldType result;

			if ( strTypel.find( STR( "BIGINT" ) ) != String::npos
					|| strTypel == STR( "MAX" )
					|| strTypel == STR( "COUNT" )
					|| strTypel == STR( "MIN" ) )
			{
				result = EFieldType_SINT64;
			}
			else if ( strTypel.find( STR( "SMALLINT" ) ) != String::npos )
			{
				result = EFieldType_SINT16;
			}
			else if ( strTypel.find( STR( "BOOL" ) ) != String::npos
					  || strTypel.find( STR( "BIT" ) ) != String::npos )
			{
				result = EFieldType_BIT;
			}
			else if ( strTypel.find( STR( "TINYINT" ) ) != String::npos )
			{
				result = EFieldType_UINT8;
			}
			else if ( ( index = strTypel.find( STR( "MEDIUMINT" ) ) ) != String::npos )
			{
				result = EFieldType_SINT24;
			}
			else if ( ( index = strTypel.find( STR( "INT" ) ) ) != String::npos )
			{
				if ( index != std::string::npos && strTypel.size() > 3 )
				{
					int prec = 0;
					std::stringstream stream( strTypel.substr( index + 3 ) );
					stream >> prec;

					if ( prec )
					{
						if ( prec == 1 )
						{
							result = EFieldType_UINT8;
						}
						else if ( prec == 2 )
						{
							result = EFieldType_SINT16;
						}
						else if ( prec <= 3 )
						{
							result = EFieldType_SINT24;
						}
						else if ( prec <= 4 )
						{
							result = EFieldType_SINT32;
						}
						else
						{
							result = EFieldType_SINT64;
						}
					}
					else
					{
						result = EFieldType_SINT32;
					}
				}
				else
				{
					result = EFieldType_SINT32;
				}
			}
			else if ( strTypel.find( STR( "NCHAR" ) ) != String::npos )
			{
				result = EFieldType_NCHAR;

				if ( limprec.first == -1 )
				{
					limprec = RetrieveLimits( strTypel );
				}
			}
			else if ( strTypel.find( STR( "NVARCHAR" ) ) != String::npos )
			{
				result = EFieldType_NVARCHAR;

				if ( limprec.first == -1 )
				{
					limprec = RetrieveLimits( strTypel );
				}
			}
			else if ( strTypel.find( STR( "VARCHAR" ) ) != String::npos )
			{
				result = EFieldType_VARCHAR;

				if ( limprec.first == -1 )
				{
					limprec = RetrieveLimits( strTypel );
				}
			}
			else if ( strTypel.find( STR( "CHAR" ) ) != String::npos )
			{
				result = EFieldType_CHAR;

				if ( limprec.first == -1 )
				{
					limprec = RetrieveLimits( strTypel );
				}
			}
			else if ( strTypel.find( STR( "NTEXT" ) ) != String::npos )
			{
				result = EFieldType_NTEXT;
				limprec.first = -1;
			}
			else if ( strTypel.find( STR( "CLOB" ) ) != String::npos
					  || strTypel.find( STR( "TEXT" ) ) != String::npos )
			{
				result = EFieldType_TEXT;
				limprec.first = -1;
			}
			else if ( strTypel.find( STR( "FLOA" ) ) != String::npos
					  || strTypel == STR( "SUM" ) )
			{
				result = EFieldType_FLOAT32;
			}
			else if ( strTypel.find( STR( "REAL" ) ) != String::npos
					  || strTypel.find( STR( "DOUB" ) ) != String::npos )
			{
				result = EFieldType_FLOAT64;
			}
			else if ( strTypel.find( STR( "DECIMAL" ) ) != String::npos
					  || strTypel.find( STR( "NUMERIC" ) ) != String::npos )
			{
				result = EFieldType_FIXED_POINT;

				if ( limprec.first == -1 )
				{
					limprec = RetrieveLimits( strTypel );
				}
			}
			else if ( strTypel == STR( "DATETIME" ) )
			{
				result = EFieldType_DATETIME;
			}
			else if ( strTypel == STR( "DATE" ) )
			{
				result = EFieldType_DATE;
			}
			else if ( strTypel == STR( "TIME" ) )
			{
				result = EFieldType_TIME;
			}
			else if ( strTypel.find( STR( "BLOB" ) ) != String::npos )
			{
				result = EFieldType_BLOB;
				limprec.first = -1;
			}
			else if ( strTypel.find( STR( "VARBINARY" ) ) != String::npos )
			{
				result = EFieldType_VARBINARY;

				if ( limprec.first == -1 )
				{
					limprec = RetrieveLimits( strTypel );
				}
			}
			else if ( strTypel.find( STR( "BINARY" ) ) != String::npos )
			{
				result = EFieldType_BINARY;

				if ( limprec.first == -1 )
				{
					limprec = RetrieveLimits( strTypel );
				}
			}

			return result;
		}

		DatabaseValuedObjectInfosPtrArray InitialiseColumns( SQLSMALLINT columnCount, std::vector< std::unique_ptr< CInOdbcBindBase > > & columns, SQLHSTMT stmt )
		{
			static const SQLSMALLINT BUFFER_SIZE = 255;
			EErrorType errorType = EErrorType_NONE;
			DatabaseValuedObjectInfosPtrArray result;

			for ( SQLSMALLINT i = 1; i <= columnCount; ++i )
			{
				TCHAR buffer[BUFFER_SIZE] = { 0 };
				SQLSMALLINT stringLength = 0;
				SQLLEN numericAttribute = 0;

				// Retrieve the column name
				OdbcCheck( SQLColAttribute( stmt, i, SQL_DESC_LABEL, SQLPOINTER( buffer ), BUFFER_SIZE, &stringLength, &numericAttribute ), SQL_HANDLE_STMT, stmt, INFO_ODBC_ColAttribute + ODBC_OPTION_DESC_LABEL );
				String name = StringUtils::ToString( buffer );

				// Its length
				std::memset( buffer, 0, BUFFER_SIZE );
				stringLength = 0;
				numericAttribute = 0;
				OdbcCheck( SQLColAttribute( stmt, i, SQL_DESC_LENGTH, SQLPOINTER( buffer ), BUFFER_SIZE, &stringLength, &numericAttribute ), SQL_HANDLE_STMT, stmt, INFO_ODBC_ColAttribute + ODBC_OPTION_DESC_LENGTH );
				uint32_t limits = uint32_t( numericAttribute );

				// Its precision (numeric types)
				std::memset( buffer, 0, BUFFER_SIZE );
				stringLength = 0;
				numericAttribute = 0;
				OdbcCheck( SQLColAttribute( stmt, i, SQL_DESC_PRECISION, SQLPOINTER( buffer ), BUFFER_SIZE, &stringLength, &numericAttribute ), SQL_HANDLE_STMT, stmt, INFO_ODBC_ColAttribute + ODBC_OPTION_DESC_PRECISION );
				uint32_t precision = uint32_t( numericAttribute );

				// Its scale (numeric types)
				std::memset( buffer, 0, BUFFER_SIZE );
				stringLength = 0;
				numericAttribute = 0;
				OdbcCheck( SQLColAttribute( stmt, i, SQL_DESC_SCALE, SQLPOINTER( buffer ), BUFFER_SIZE, &stringLength, &numericAttribute ), SQL_HANDLE_STMT, stmt, INFO_ODBC_ColAttribute + ODBC_OPTION_DESC_SCALE );
				uint32_t scale = uint32_t( numericAttribute );

				// Its SQL type
				std::memset( buffer, 0, BUFFER_SIZE );
				stringLength = 0;
				numericAttribute = 0;
				OdbcCheck( SQLColAttribute( stmt, i, SQL_DESC_TYPE, SQLPOINTER( buffer ), BUFFER_SIZE, &stringLength, &numericAttribute ), SQL_HANDLE_STMT, stmt, INFO_ODBC_ColAttribute + ODBC_OPTION_DESC_TYPE );
				SQLINTEGER sqlType = uint32_t( numericAttribute );

				// Its concise type
				std::memset( buffer, 0, BUFFER_SIZE );
				stringLength = 0;
				numericAttribute = 0;
				OdbcCheck( SQLColAttribute( stmt, i, SQL_DESC_CONCISE_TYPE, SQLPOINTER( buffer ), BUFFER_SIZE, &stringLength, &numericAttribute ), SQL_HANDLE_STMT, stmt, INFO_ODBC_ColAttribute + ODBC_OPTION_DESC_CONCISE_TYPE );
				SQLINTEGER conciseType = SQLINTEGER( numericAttribute );

				// Its type name
				std::memset( buffer, 0, BUFFER_SIZE );
				stringLength = 0;
				numericAttribute = 0;
				OdbcCheck( SQLColAttribute( stmt, i, SQL_DESC_TYPE_NAME, SQLPOINTER( buffer ), BUFFER_SIZE, &stringLength, &numericAttribute ), SQL_HANDLE_STMT, stmt, INFO_ODBC_ColAttribute + ODBC_OPTION_DESC_TYPE_NAME );
				String typeName = StringUtils::ToString( buffer );

				DatabaseValuedObjectInfosSPtr infos;
				std::unique_ptr< CInOdbcBindBase > bind;

				if ( conciseType == SQL_NTS || conciseType == SQL_TINYINT || conciseType == SQL_CHAR || conciseType == SQL_VARCHAR )
				{
					if ( errorType == EErrorType_NONE )
					{
						std::pair< uint32_t, uint32_t > limprec( precision, scale );
						infos = std::make_shared< CDatabaseValuedObjectInfos >( name, GetFieldTypeFromTypeName( typeName, limprec ), std::make_pair( precision, scale ) );
						bind = GetBindFromFieldType( infos->GetType(), limits, precision, scale );
					}
				}
				else
				{
					EFieldType type = GetFieldTypeFromConciseType( conciseType, limits );
					bind = GetBindFromFieldType( type, limits, precision, scale );
					infos = std::make_shared< CDatabaseValuedObjectInfos >( name, type, std::make_pair( precision, scale ) );
				}

				columns.push_back( std::move( bind ) );
				result.push_back( infos );
			}

			return result;
		}

		void SetFieldValue( EFieldType type, CDatabaseValueBase & value, CInOdbcBindBase const & bind )
		{
			switch ( type )
			{
			case EFieldType_BIT:
				static_cast< CDatabaseValue< EFieldType_BIT > & >( value ).SetValue( static_cast< CInOdbcBind< bool > const & >( bind ).GetValue() != 0 );
				break;

			case EFieldType_SINT8:
				static_cast< CDatabaseValue< EFieldType_SINT8 > & >( value ).SetValue( static_cast< CInOdbcBind< int8_t > const & >( bind ).GetValue() );
				break;

			case EFieldType_SINT16:
				static_cast< CDatabaseValue< EFieldType_SINT16 > & >( value ).SetValue( static_cast< CInOdbcBind< int16_t > const & >( bind ).GetValue() );
				break;

			case EFieldType_SINT24:
				static_cast< CDatabaseValue< EFieldType_SINT24 > & >( value ).SetValue( static_cast< CInOdbcBind< int24_t > const & >( bind ).GetValue() );
				break;

			case EFieldType_SINT32:
				static_cast< CDatabaseValue< EFieldType_SINT32 > & >( value ).SetValue( static_cast< CInOdbcBind< int32_t > const & >( bind ).GetValue() );
				break;

			case EFieldType_SINT64:
				static_cast< CDatabaseValue< EFieldType_SINT64 > & >( value ).SetValue( static_cast< CInOdbcBind< int64_t > const & >( bind ).GetValue() );
				break;

			case EFieldType_UINT8:
				static_cast< CDatabaseValue< EFieldType_UINT8 > & >( value ).SetValue( static_cast< CInOdbcBind< uint8_t > const & >( bind ).GetValue() );
				break;

			case EFieldType_UINT16:
				static_cast< CDatabaseValue< EFieldType_UINT16 > & >( value ).SetValue( static_cast< CInOdbcBind< uint16_t > const & >( bind ).GetValue() );
				break;

			case EFieldType_UINT24:
				static_cast< CDatabaseValue< EFieldType_UINT24 > & >( value ).SetValue( static_cast< CInOdbcBind< uint24_t > const & >( bind ).GetValue() );
				break;

			case EFieldType_UINT32:
				static_cast< CDatabaseValue< EFieldType_UINT32 > & >( value ).SetValue( static_cast< CInOdbcBind< uint32_t > const & >( bind ).GetValue() );
				break;

			case EFieldType_UINT64:
				static_cast< CDatabaseValue< EFieldType_UINT64 > & >( value ).SetValue( static_cast< CInOdbcBind< uint64_t > const & >( bind ).GetValue() );
				break;

			case EFieldType_FLOAT32:
				static_cast< CDatabaseValue< EFieldType_FLOAT32 > & >( value ).SetValue( static_cast< CInOdbcBind< float > const & >( bind ).GetValue() );
				break;

			case EFieldType_FLOAT64:
				static_cast< CDatabaseValue< EFieldType_FLOAT64 > & >( value ).SetValue( static_cast< CInOdbcBind< double > const & >( bind ).GetValue() );
				break;

			case EFieldType_FIXED_POINT:
				static_cast< CDatabaseValue< EFieldType_FIXED_POINT > & >( value ).SetValue( static_cast< CInOdbcBind< SQL_NUMERIC_STRUCT, CFixedPoint > const & >( bind ).GetValue() );
				break;

			case EFieldType_CHAR:
				static_cast< CDatabaseValue< EFieldType_CHAR > & >( value ).SetValue( static_cast< CInOdbcBind< char *, uint32_t > const & >( bind ).GetValue() );
				break;

			case EFieldType_VARCHAR:
				static_cast< CDatabaseValue< EFieldType_VARCHAR > & >( value ).SetValue( StringFromOdbcString( static_cast< CInOdbcBind< char * > const & >( bind ) ) );
				break;

			case EFieldType_TEXT:
				static_cast< CDatabaseValue< EFieldType_TEXT > & >( value ).SetValue( StringFromOdbcString( static_cast< CInOdbcBind< char * > const & >( bind ) ) );
				break;

			case EFieldType_NCHAR:
				static_cast< CDatabaseValue< EFieldType_NCHAR > & >( value ).SetValue( static_cast< CInOdbcBind< wchar_t *, uint32_t > const & >( bind ).GetValue() );
				break;

			case EFieldType_NVARCHAR:
				static_cast< CDatabaseValue< EFieldType_NVARCHAR > & >( value ).SetValue( StringFromOdbcWString( static_cast< CInOdbcBind< wchar_t * > const & >( bind ) ) );
				break;

			case EFieldType_NTEXT:
				static_cast< CDatabaseValue< EFieldType_NTEXT > & >( value ).SetValue( StringFromOdbcWString( static_cast< CInOdbcBind< wchar_t * > const & >( bind ) ) );
				break;

			case EFieldType_DATE:
				static_cast< CDatabaseValue< EFieldType_DATE > & >( value ).SetValue( DateFromOdbcDate( static_cast< CInOdbcBind< DATE_STRUCT > const & >( bind ).GetValue() ) );
				break;

			case EFieldType_DATETIME:
				static_cast< CDatabaseValue< EFieldType_DATETIME > & >( value ).SetValue( DateTimeFromOdbcTimestamp( static_cast< CInOdbcBind< TIMESTAMP_STRUCT > const & >( bind ).GetValue() ) );
				break;

			case EFieldType_TIME:
				static_cast< CDatabaseValue< EFieldType_TIME > & >( value ).SetValue( TimeFromOdbcTime( static_cast< CInOdbcBind< TIME_STRUCT > const & >( bind ).GetValue() ) );
				break;

			case EFieldType_BINARY:
				static_cast< CDatabaseValue< EFieldType_BINARY > & >( value ).SetValue( VectorFromOdbcBinary( static_cast< CInOdbcBind< uint8_t * > const & >( bind ) ) );
				break;

			case EFieldType_VARBINARY:
				static_cast< CDatabaseValue< EFieldType_VARBINARY > & >( value ).SetValue( VectorFromOdbcBinary( static_cast< CInOdbcBind< uint8_t * > const & >( bind ) ) );
				break;

			case EFieldType_BLOB:
				static_cast< CDatabaseValue< EFieldType_BLOB > & >( value ).SetValue( VectorFromOdbcBinary( static_cast< CInOdbcBind< uint8_t * > const & >( bind ) ) );
				break;

			default:
				break;
			}
		}

		EErrorType FetchResultSet( DatabaseConnectionSPtr connection, DatabaseResultSPtr results, std::vector< std::unique_ptr< CInOdbcBindBase > > & bindings, SQLHSTMT statementHandle )
		{
			EErrorType errorType = EErrorType_NONE;
			SQLRETURN res;
			SQLLEN rowCount = 0;
			OdbcCheck( SQLRowCount( statementHandle, &rowCount ), SQL_HANDLE_STMT, statementHandle, INFO_ODBC_RowCount );

			if ( errorType == EErrorType_NONE && rowCount )
			{
				try
				{
					SQLHDESC descriptor = NULL;
					OdbcCheck( SQLGetStmtAttr( statementHandle, SQL_ATTR_APP_ROW_DESC, &descriptor, 0, NULL ), SQL_HANDLE_STMT, statementHandle, INFO_ODBC_GetStmtAttr );
					SQLUSMALLINT index = 1;

					for ( auto && binding : bindings )
					{
						binding->Initialise( statementHandle, index++, descriptor );
					}

					res = SQLFetch( statementHandle );

					if ( res != SQL_NO_DATA && res != SQL_SUCCESS )
					{
						OdbcCheck( res, SQL_HANDLE_STMT, statementHandle, INFO_ODBC_Fetch );
					}
					else if ( res == SQL_NO_DATA )
					{
						CLogger::LogDebug( DEBUG_ODBC_EMPTY_RESULT );
					}

					while ( errorType == EErrorType_NONE && res != SQL_NO_DATA )
					{
						DatabaseRowSPtr row = std::make_shared< CDatabaseRow >();
						uint32_t index = 0;

						for ( auto && bind : bindings )
						{
							bool isNull = bind->_strLenOrInd == SQL_NULL_DATA;
							DatabaseFieldSPtr field;

							try
							{
								field = std::make_shared< CDatabaseField >( connection, results->GetFieldInfos( index++ ) );

								if ( !isNull )
								{
									SetFieldValue( field->GetType(), field->GetObjectValue(), *bind );
								}
							}
							catch ( const CDatabaseException & e )
							{
								StringStream message;
								message << ERROR_ODBC_DRIVER << std::endl;
								message << e.what();
								ODBC_EXCEPT( EDatabaseExceptionCodes_GenericError, message.str() );
							}

							row->AddField( field );
						}

						results->AddRow( row );

						res = SQLFetch( statementHandle );

						if ( res != SQL_NO_DATA && res != SQL_SUCCESS )
						{
							OdbcCheck( res, SQL_HANDLE_STMT, statementHandle, INFO_ODBC_Fetch );
						}
					}

					// free memory from the binding
					OdbcCheck( SQLFreeStmt( statementHandle, SQL_UNBIND ), SQL_HANDLE_STMT, statementHandle, INFO_ODBC_FreeStmt );
				}
				catch ( const std::exception & e )
				{
					StringStream message;
					message << ERROR_ODBC_DRIVER << std::endl;
					message << e.what();
					ODBC_EXCEPT( EDatabaseExceptionCodes_GenericError, message.str() );
				}
				catch ( ... )
				{
					StringStream message;
					message << ERROR_ODBC_DRIVER << std::endl;
					message << ERROR_ODBC_UNKNOWN;
					ODBC_EXCEPT( EDatabaseExceptionCodes_UnknownError, message.str() );
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
			StringStream LogInfo;
			LogInfo << INFO_ODBC_STATUS << state
					<< STR( ", " )
					<< INFO_ODBC_NATIVE << nativeError
					<< STR( ", " ) << sqlMessage
					<< STR( ", " )
					<< INFO_ODBC_REQUEST << query;

			if ( nativeError == ODBC_RERUN_TRANSACTION_CODE )
			{
				errorType = EErrorType_RETRY;
				CLogger::LogWarning( LogInfo.str() );
			}
			else
			{
				if ( error )
				{
					CLogger::LogError( LogInfo.str() );
				}
				else
				{
					CLogger::LogWarning( LogInfo.str() );
				}
			}
		}

		if ( ( errorCount == 2 && errorType != EErrorType_RETRY )
				|| ( errorCount > 2 && errorType == EErrorType_RETRY ) )
		{
			StringStream LogInfo;
			LogInfo << ERROR_ODBC_QUERY << query;

			if ( error )
			{
				CLogger::LogError( LogInfo.str() );
			}
			else
			{
				CLogger::LogWarning( LogInfo.str() );
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

			StringStream LogInfo;
			LogInfo << INFO_ODBC_QUERY_SUCCESS << query;
			CLogger::LogDebug( LogInfo.str() );
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

	EFieldType GetFieldTypeFromConciseType( SQLLEN sqlType, uint32_t limits )
	{
		EFieldType fieldType = EFieldType_NULL;

		switch ( sqlType )
		{
		case SQL_CHAR:
			fieldType = EFieldType_CHAR;
			break;

		case SQL_VARCHAR:
			fieldType = EFieldType_VARCHAR;
			break;

		case SQL_WCHAR:
			fieldType = EFieldType_NCHAR;
			break;

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
			if ( limits < 25 )
			{
				fieldType = EFieldType_FLOAT32;
			}
			else
			{
				fieldType = EFieldType_FLOAT64;
			}

			break;

		case SQL_DOUBLE:
			fieldType = EFieldType_FLOAT64;
			break;

		case SQL_DECIMAL:
		case SQL_NUMERIC:
			fieldType = EFieldType_FIXED_POINT;
			break;

		case SQL_INTEGER:
			if ( limits == 8 )
			{
				fieldType = EFieldType_SINT24;
			}
			else
			{
				fieldType = EFieldType_SINT32;
			}

			break;

		case SQL_SMALLINT:
			fieldType = EFieldType_SINT16;
			break;

		case SQL_BIGINT:
			fieldType = EFieldType_SINT64;
			break;

		case SQL_TINYINT:
			fieldType = EFieldType_UINT8;
			break;

		case SQL_BIT:
			fieldType = EFieldType_BIT;
			break;

		case SQL_BINARY:
			fieldType = EFieldType_BINARY;
			break;

		case SQL_VARBINARY:
			fieldType = EFieldType_VARBINARY;
			break;

		case SQL_LONGVARBINARY:
			fieldType = EFieldType_BLOB;
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

	EErrorType SqlExecute( DatabaseConnectionSPtr connection, SQLHSTMT statementHandle, FuncResultSetFullyFetched onFullyfetched, DatabaseResultSPtr & pReturn )
	{
		SQLRETURN res = SQL_SUCCESS;
		EErrorType errorType = EErrorType_NONE;

		do
		{
			SQLSMALLINT numColumns;
			OdbcCheck( SQLNumResultCols( statementHandle, &numColumns ), SQL_HANDLE_STMT, statementHandle, INFO_ODBC_NumResultCols );

			if ( numColumns )
			{
				std::vector< std::unique_ptr< CInOdbcBindBase > > arrayColumnData;
				DatabaseValuedObjectInfosPtrArray columns = InitialiseColumns( numColumns, arrayColumnData, statementHandle );
				pReturn = std::make_shared< CDatabaseResult >( columns );
				FetchResultSet( connection, pReturn, arrayColumnData, statementHandle );
			}

			res = SQLMoreResults( statementHandle );
			OdbcCheck( res, SQL_HANDLE_STMT, statementHandle, INFO_ODBC_MoreResults );

			if ( res != SQL_NO_DATA && errorType == EErrorType_NONE )
			{
				CLogger::LogInfo( STR( "Additional result detected" ) );
			}
			else
			{
				onFullyfetched( statementHandle, res );
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
