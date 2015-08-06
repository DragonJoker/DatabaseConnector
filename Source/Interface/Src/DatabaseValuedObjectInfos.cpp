/************************************************************************//**
* @file DatabaseValuedObjectInfos.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDatabaseValuedObjectInfos class definition.
*
* @details Describes database valued object information.
*
***************************************************************************/

#include "DatabasePch.h"

#include "DatabaseValuedObjectInfos.h"

#include "DatabaseStringUtils.h"
#include "DatabaseException.h"
#include "DatabaseValuePolicy.h"

BEGIN_NAMESPACE_DATABASE
{
	namespace
	{
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
	}

	const String & ERROR_DB_MISSING_LIMITS = STR( "Missing limits for field " );
	const String & ERROR_DB_MISSING_PRECISION = STR( "Missing precision for field " );

	bool AreTypesCompatibleSet( EFieldType typeFrom, EFieldType typeTo )
	{
		bool result = typeFrom == typeTo;

		if ( !result )
		{
			switch ( typeFrom )
			{
			case EFieldType_NULL:
				result = false;
				break;

			case EFieldType_BIT:
				result = ( typeTo == EFieldType_SINT8 ) || ( typeTo == EFieldType_SINT16 ) || ( typeTo == EFieldType_SINT24 ) || ( typeTo == EFieldType_SINT32 ) || ( typeTo == EFieldType_SINT64 )
						 || ( typeTo == EFieldType_UINT8 ) || ( typeTo == EFieldType_UINT16 ) || ( typeTo == EFieldType_UINT24 ) || ( typeTo == EFieldType_UINT32 ) || ( typeTo == EFieldType_UINT64 )
						 || ( typeTo == EFieldType_VARCHAR ) || ( typeTo == EFieldType_TEXT )
						 || ( typeTo == EFieldType_NVARCHAR ) || ( typeTo == EFieldType_NTEXT );
				break;

			case EFieldType_SINT8:
				result = ( typeTo == EFieldType_BIT ) || ( typeTo == EFieldType_SINT16 ) || ( typeTo == EFieldType_SINT24 ) || ( typeTo == EFieldType_SINT32 ) || ( typeTo == EFieldType_SINT64 )
						 || ( typeTo == EFieldType_UINT8 ) || ( typeTo == EFieldType_UINT16 ) || ( typeTo == EFieldType_UINT24 ) || ( typeTo == EFieldType_UINT32 ) || ( typeTo == EFieldType_UINT64 )
						 || ( typeTo == EFieldType_FLOAT32 ) || ( typeTo == EFieldType_FLOAT64 );
				break;

			case EFieldType_SINT16:
				result = ( typeTo == EFieldType_BIT ) || ( typeTo == EFieldType_SINT24 ) || ( typeTo == EFieldType_SINT32 ) || ( typeTo == EFieldType_SINT64 )
						 || ( typeTo == EFieldType_UINT16 ) || ( typeTo == EFieldType_UINT24 ) || ( typeTo == EFieldType_UINT32 ) || ( typeTo == EFieldType_UINT64 )
						 || ( typeTo == EFieldType_FLOAT32 ) || ( typeTo == EFieldType_FLOAT64 );
				break;

			case EFieldType_SINT24:
				result = ( typeTo == EFieldType_BIT ) || ( typeTo == EFieldType_SINT32 ) || ( typeTo == EFieldType_SINT64 )
						 || ( typeTo == EFieldType_UINT24 ) || ( typeTo == EFieldType_UINT32 ) || ( typeTo == EFieldType_UINT64 )
						 || ( typeTo == EFieldType_FLOAT32 ) || ( typeTo == EFieldType_FLOAT64 );
				break;

			case EFieldType_SINT32:
				result = ( typeTo == EFieldType_BIT ) || ( typeTo == EFieldType_SINT64 )
						 || ( typeTo == EFieldType_UINT32 ) || ( typeTo == EFieldType_UINT64 )
						 || ( typeTo == EFieldType_FLOAT32 ) || ( typeTo == EFieldType_FLOAT64 );
				break;

			case EFieldType_SINT64:
				result = ( typeTo == EFieldType_BIT ) || ( typeTo == EFieldType_UINT64 );
				break;

			case EFieldType_UINT8:
				result = ( typeTo == EFieldType_SINT8 ) || ( typeTo == EFieldType_SINT16 ) || ( typeTo == EFieldType_SINT24 ) || ( typeTo == EFieldType_SINT32 ) || ( typeTo == EFieldType_SINT64 )
						 || ( typeTo == EFieldType_BIT ) || ( typeTo == EFieldType_UINT16 ) || ( typeTo == EFieldType_UINT24 ) || ( typeTo == EFieldType_UINT32 ) || ( typeTo == EFieldType_UINT64 )
						 || ( typeTo == EFieldType_FLOAT32 ) || ( typeTo == EFieldType_FLOAT64 );
				break;

			case EFieldType_UINT16:
				result = ( typeTo == EFieldType_SINT16 ) || ( typeTo == EFieldType_SINT24 ) || ( typeTo == EFieldType_SINT32 ) || ( typeTo == EFieldType_SINT64 )
						 || ( typeTo == EFieldType_BIT ) || ( typeTo == EFieldType_UINT24 ) || ( typeTo == EFieldType_UINT32 ) || ( typeTo == EFieldType_UINT64 )
						 || ( typeTo == EFieldType_FLOAT32 ) || ( typeTo == EFieldType_FLOAT64 );
				break;

			case EFieldType_UINT24:
				result = ( typeTo == EFieldType_SINT24 ) || ( typeTo == EFieldType_SINT32 ) || ( typeTo == EFieldType_SINT64 )
						 || ( typeTo == EFieldType_BIT ) || ( typeTo == EFieldType_UINT32 ) || ( typeTo == EFieldType_UINT64 )
						 || ( typeTo == EFieldType_FLOAT32 ) || ( typeTo == EFieldType_FLOAT64 );
				break;

			case EFieldType_UINT32:
				result = ( typeTo == EFieldType_SINT32 ) || ( typeTo == EFieldType_SINT64 )
						 || ( typeTo == EFieldType_BIT ) || ( typeTo == EFieldType_UINT64 )
						 || ( typeTo == EFieldType_FLOAT32 ) || ( typeTo == EFieldType_FLOAT64 );
				break;

			case EFieldType_UINT64:
				result = ( typeTo == EFieldType_SINT64 ) || ( typeTo == EFieldType_BIT );
				break;

			case EFieldType_FLOAT32:
				result = ( typeTo == EFieldType_SINT16 ) || ( typeTo == EFieldType_SINT24 ) || ( typeTo == EFieldType_SINT32 ) || ( typeTo == EFieldType_SINT64 )
						 || ( typeTo == EFieldType_UINT16 ) || ( typeTo == EFieldType_UINT24 ) || ( typeTo == EFieldType_UINT32 ) || ( typeTo == EFieldType_UINT64 )
						 || ( typeTo == EFieldType_FLOAT32 ) || ( typeTo == EFieldType_FLOAT64 );
				break;

			case EFieldType_FLOAT64:
				result = ( typeTo == EFieldType_SINT16 ) || ( typeTo == EFieldType_SINT24 ) || ( typeTo == EFieldType_SINT32 ) || ( typeTo == EFieldType_SINT64 )
						 || ( typeTo == EFieldType_UINT16 ) || ( typeTo == EFieldType_UINT24 ) || ( typeTo == EFieldType_UINT32 ) || ( typeTo == EFieldType_UINT64 )
						 || ( typeTo == EFieldType_FLOAT32 ) || ( typeTo == EFieldType_FLOAT64 );
				break;

			case EFieldType_FIXED_POINT:
				result = ( typeTo == EFieldType_FLOAT32 ) || ( typeTo == EFieldType_FLOAT64 );
				break;

			case EFieldType_CHAR:
				result = ( typeTo == EFieldType_VARCHAR ) || ( typeTo == EFieldType_TEXT )
						 || ( typeTo == EFieldType_NCHAR ) || ( typeTo == EFieldType_NVARCHAR ) || ( typeTo == EFieldType_NTEXT );
				break;

			case EFieldType_VARCHAR:
				result = ( typeTo == EFieldType_CHAR ) || ( typeTo == EFieldType_TEXT )
						 || ( typeTo == EFieldType_NCHAR ) || ( typeTo == EFieldType_NVARCHAR ) || ( typeTo == EFieldType_NTEXT );
				break;

			case EFieldType_TEXT:
				result = ( typeTo == EFieldType_CHAR ) || ( typeTo == EFieldType_VARCHAR )
						 || ( typeTo == EFieldType_NCHAR ) || ( typeTo == EFieldType_NVARCHAR ) || ( typeTo == EFieldType_NTEXT );
				break;

			case EFieldType_NCHAR:
				result = ( typeTo == EFieldType_NVARCHAR ) || ( typeTo == EFieldType_NTEXT );
				break;

			case EFieldType_NVARCHAR:
				result = ( typeTo == EFieldType_NCHAR ) || ( typeTo == EFieldType_NTEXT );
				break;

			case EFieldType_NTEXT:
				result = ( typeTo == EFieldType_NCHAR ) || ( typeTo == EFieldType_NVARCHAR );
				break;

			case EFieldType_DATE:
				result = ( typeTo == EFieldType_DATE ) || ( typeTo == EFieldType_DATETIME );
				break;

			case EFieldType_DATETIME:
				result = ( typeTo == EFieldType_DATE ) || ( typeTo == EFieldType_DATETIME ) || ( typeTo == EFieldType_TIME );
				break;

			case EFieldType_TIME:
				result = ( typeTo == EFieldType_TIME ) || ( typeTo == EFieldType_DATETIME );
				break;

			case EFieldType_BINARY:
				result = ( typeTo == EFieldType_BINARY ) || ( typeTo == EFieldType_VARBINARY ) || ( typeTo == EFieldType_BLOB );
				break;

			case EFieldType_VARBINARY:
				result = ( typeTo == EFieldType_BINARY ) || ( typeTo == EFieldType_VARBINARY ) || ( typeTo == EFieldType_BLOB );
				break;

			case EFieldType_BLOB:
				result = ( typeTo == EFieldType_BINARY ) || ( typeTo == EFieldType_VARBINARY ) || ( typeTo == EFieldType_BLOB );
				break;

			default:
				result = false;
				break;
			}
		}

		return result;
	}

	bool AreTypesCompatibleGet( EFieldType typeFrom, EFieldType typeTo )
	{
		bool result = typeFrom == typeTo;

		if ( !result )
		{
			switch ( typeTo )
			{
			case EFieldType_NULL:
				result = false;
				break;

			case EFieldType_BIT:
				result = ( typeFrom == EFieldType_SINT8 ) || ( typeFrom == EFieldType_SINT16 ) || ( typeFrom == EFieldType_SINT24 ) || ( typeFrom == EFieldType_SINT32 )
						 || ( typeFrom == EFieldType_UINT8 ) || ( typeFrom == EFieldType_UINT16 ) || ( typeFrom == EFieldType_UINT24 ) || ( typeFrom == EFieldType_UINT32 )
						 || ( typeFrom == EFieldType_VARCHAR ) || ( typeFrom == EFieldType_NVARCHAR );
				break;

			case EFieldType_SINT8:
				result = ( typeFrom == EFieldType_BIT ) || ( typeFrom == EFieldType_UINT8 );
				break;

			case EFieldType_SINT16:
				result = ( typeFrom == EFieldType_BIT ) || ( typeFrom == EFieldType_SINT8 )
						 || ( typeFrom == EFieldType_UINT8 ) || ( typeFrom == EFieldType_UINT16 );
				break;

			case EFieldType_SINT24:
				result = ( typeFrom == EFieldType_BIT ) || ( typeFrom == EFieldType_SINT8 ) || ( typeFrom == EFieldType_SINT16 )
						 || ( typeFrom == EFieldType_UINT8 ) || ( typeFrom == EFieldType_UINT16 ) || ( typeFrom == EFieldType_UINT24 );
				break;

			case EFieldType_SINT32:
				result = ( typeFrom == EFieldType_BIT ) || ( typeFrom == EFieldType_SINT8 ) || ( typeFrom == EFieldType_SINT16 ) || ( typeFrom == EFieldType_SINT24 )
						 || ( typeFrom == EFieldType_UINT8 ) || ( typeFrom == EFieldType_UINT16 ) || ( typeFrom == EFieldType_UINT24 ) || ( typeFrom == EFieldType_UINT32 )
						 || ( typeFrom == EFieldType_FLOAT32 ) || ( typeFrom == EFieldType_FLOAT64 );
				break;

			case EFieldType_SINT64:
				result = ( typeFrom == EFieldType_SINT8 ) || ( typeFrom == EFieldType_SINT16 ) || ( typeFrom == EFieldType_SINT24 ) || ( typeFrom == EFieldType_SINT32 )
						 || ( typeFrom == EFieldType_UINT8 ) || ( typeFrom == EFieldType_UINT16 ) || ( typeFrom == EFieldType_UINT24 ) || ( typeFrom == EFieldType_UINT32 ) || ( typeFrom == EFieldType_UINT64 )
						 || ( typeFrom == EFieldType_FLOAT32 ) || ( typeFrom == EFieldType_FLOAT64 );
				break;

			case EFieldType_UINT8:
				result = ( typeFrom == EFieldType_BIT ) || ( typeFrom == EFieldType_SINT8 );
				break;

			case EFieldType_UINT16:
				result = ( typeFrom == EFieldType_SINT8 ) || ( typeFrom == EFieldType_SINT16 )
						 || ( typeFrom == EFieldType_BIT ) || ( typeFrom == EFieldType_UINT8 );
				break;

			case EFieldType_UINT24:
				result = ( typeFrom == EFieldType_SINT8 ) || ( typeFrom == EFieldType_SINT16 ) || ( typeFrom == EFieldType_SINT24 )
						 || ( typeFrom == EFieldType_BIT ) || ( typeFrom == EFieldType_UINT8 ) || ( typeFrom == EFieldType_UINT16 );
				break;

			case EFieldType_UINT32:
				result = ( typeFrom == EFieldType_SINT8 ) || ( typeFrom == EFieldType_SINT16 ) || ( typeFrom == EFieldType_SINT24 ) || ( typeFrom == EFieldType_SINT32 )
						 || ( typeFrom == EFieldType_BIT ) || ( typeFrom == EFieldType_UINT8 ) || ( typeFrom == EFieldType_UINT16 ) || ( typeFrom == EFieldType_UINT24 )
						 || ( typeFrom == EFieldType_FLOAT32 ) || ( typeFrom == EFieldType_FLOAT64 );
				break;

			case EFieldType_UINT64:
				result = ( typeFrom == EFieldType_SINT8 ) || ( typeFrom == EFieldType_SINT16 ) || ( typeFrom == EFieldType_SINT24 ) || ( typeFrom == EFieldType_SINT32 ) || ( typeFrom == EFieldType_SINT64 )
						 || ( typeFrom == EFieldType_UINT8 ) || ( typeFrom == EFieldType_UINT16 ) || ( typeFrom == EFieldType_UINT24 ) || ( typeFrom == EFieldType_UINT32 )
						 || ( typeFrom == EFieldType_FLOAT32 ) || ( typeFrom == EFieldType_FLOAT64 );
				break;

			case EFieldType_FLOAT32:
				result = ( typeFrom == EFieldType_SINT16 ) || ( typeFrom == EFieldType_SINT24 ) || ( typeFrom == EFieldType_SINT32 ) || ( typeFrom == EFieldType_SINT64 )
						 || ( typeFrom == EFieldType_UINT16 ) || ( typeFrom == EFieldType_UINT24 ) || ( typeFrom == EFieldType_UINT32 ) || ( typeFrom == EFieldType_UINT64 )
						 || ( typeFrom == EFieldType_FLOAT32 ) || ( typeFrom == EFieldType_FLOAT64 ) || ( typeFrom == EFieldType_FIXED_POINT );
				break;

			case EFieldType_FLOAT64:
				result = ( typeFrom == EFieldType_SINT16 ) || ( typeFrom == EFieldType_SINT24 ) || ( typeFrom == EFieldType_SINT32 ) || ( typeFrom == EFieldType_SINT64 )
						 || ( typeFrom == EFieldType_UINT16 ) || ( typeFrom == EFieldType_UINT24 ) || ( typeFrom == EFieldType_UINT32 ) || ( typeFrom == EFieldType_UINT64 )
						 || ( typeFrom == EFieldType_FLOAT32 ) || ( typeFrom == EFieldType_FLOAT64 ) || ( typeFrom == EFieldType_FIXED_POINT );
				break;

			case EFieldType_FIXED_POINT:
				result = ( typeFrom == EFieldType_SINT8 ) || ( typeFrom == EFieldType_SINT16 ) || ( typeFrom == EFieldType_SINT24 ) || ( typeFrom == EFieldType_SINT32 ) || ( typeFrom == EFieldType_SINT64 )
						 || ( typeFrom == EFieldType_UINT8 ) || ( typeFrom == EFieldType_UINT16 ) || ( typeFrom == EFieldType_UINT24 ) || ( typeFrom == EFieldType_UINT32 ) || ( typeFrom == EFieldType_UINT64 )
						 || ( typeFrom == EFieldType_FLOAT32 ) || ( typeFrom == EFieldType_FLOAT64 );
				break;

			case EFieldType_CHAR:
				result = ( typeFrom == EFieldType_VARCHAR ) || ( typeFrom == EFieldType_TEXT );
				break;

			case EFieldType_VARCHAR:
				result = ( typeFrom == EFieldType_CHAR ) || ( typeFrom == EFieldType_TEXT );
				break;

			case EFieldType_TEXT:
				result = ( typeFrom == EFieldType_CHAR ) || ( typeFrom == EFieldType_VARCHAR );
				break;

			case EFieldType_NCHAR:
				result = ( typeFrom == EFieldType_CHAR ) || ( typeFrom == EFieldType_VARCHAR ) || ( typeFrom == EFieldType_TEXT )
						 || ( typeFrom == EFieldType_NVARCHAR ) || ( typeFrom == EFieldType_NTEXT );
				break;

			case EFieldType_NVARCHAR:
				result = ( typeFrom == EFieldType_CHAR ) || ( typeFrom == EFieldType_VARCHAR ) || ( typeFrom == EFieldType_TEXT )
						 || ( typeFrom == EFieldType_NCHAR ) || ( typeFrom == EFieldType_NTEXT );
				break;

			case EFieldType_NTEXT:
				result = ( typeFrom == EFieldType_CHAR ) || ( typeFrom == EFieldType_VARCHAR ) || ( typeFrom == EFieldType_TEXT )
						 || ( typeFrom == EFieldType_NCHAR ) || ( typeFrom == EFieldType_NVARCHAR );
				break;

			case EFieldType_DATE:
				result = ( typeFrom == EFieldType_DATETIME ) || ( typeFrom == EFieldType_NVARCHAR );// ODBC stores Dates as NVARCHAR
				break;

			case EFieldType_DATETIME:
				result = ( typeFrom == EFieldType_DATE ) || ( typeFrom == EFieldType_TIME );
				break;

			case EFieldType_TIME:
				result = ( typeFrom == EFieldType_DATETIME );
				break;

			case EFieldType_BINARY:
				result = ( typeFrom == EFieldType_BINARY ) || ( typeFrom == EFieldType_VARBINARY ) || ( typeFrom == EFieldType_BLOB );
				break;

			case EFieldType_VARBINARY:
				result = ( typeFrom == EFieldType_BINARY ) || ( typeFrom == EFieldType_VARBINARY ) || ( typeFrom == EFieldType_BLOB );
				break;

			case EFieldType_BLOB:
				result = ( typeFrom == EFieldType_BINARY ) || ( typeFrom == EFieldType_VARBINARY ) || ( typeFrom == EFieldType_BLOB );
				break;

			default:
				result = false;
				break;
			}
		}

		return result;
	}

	bool NeedsLimits( EFieldType type )
	{
		bool result = false;

		switch ( type )
		{
		case EFieldType_NULL:
			result = SFieldTypeNeedsLimits< EFieldType_NULL >::value;
			break;

		case EFieldType_BIT:
			result = SFieldTypeNeedsLimits< EFieldType_BIT >::value;
			break;

		case EFieldType_SINT8:
			result = SFieldTypeNeedsLimits< EFieldType_SINT8 >::value;
			break;

		case EFieldType_SINT16:
			result = SFieldTypeNeedsLimits< EFieldType_SINT16 >::value;
			break;

		case EFieldType_SINT24:
			result = SFieldTypeNeedsLimits< EFieldType_SINT24 >::value;
			break;

		case EFieldType_SINT32:
			result = SFieldTypeNeedsLimits< EFieldType_SINT32 >::value;
			break;

		case EFieldType_SINT64:
			result = SFieldTypeNeedsLimits< EFieldType_SINT64 >::value;
			break;

		case EFieldType_UINT8:
			result = SFieldTypeNeedsLimits< EFieldType_UINT8 >::value;
			break;

		case EFieldType_UINT16:
			result = SFieldTypeNeedsLimits< EFieldType_UINT16 >::value;
			break;

		case EFieldType_UINT24:
			result = SFieldTypeNeedsLimits< EFieldType_UINT24 >::value;
			break;

		case EFieldType_UINT32:
			result = SFieldTypeNeedsLimits< EFieldType_UINT32 >::value;
			break;

		case EFieldType_UINT64:
			result = SFieldTypeNeedsLimits< EFieldType_UINT64 >::value;
			break;

		case EFieldType_FLOAT32:
			result = SFieldTypeNeedsLimits< EFieldType_FLOAT32 >::value;
			break;

		case EFieldType_FLOAT64:
			result = SFieldTypeNeedsLimits< EFieldType_FLOAT64 >::value;
			break;

		case EFieldType_FIXED_POINT:
			result = SFieldTypeNeedsLimits< EFieldType_FIXED_POINT >::value;
			break;

		case EFieldType_CHAR:
			result = SFieldTypeNeedsLimits< EFieldType_CHAR >::value;
			break;

		case EFieldType_VARCHAR:
			result = SFieldTypeNeedsLimits< EFieldType_VARCHAR >::value;
			break;

		case EFieldType_TEXT:
			result = SFieldTypeNeedsLimits< EFieldType_TEXT >::value;
			break;

		case EFieldType_NCHAR:
			result = SFieldTypeNeedsLimits< EFieldType_NCHAR >::value;
			break;

		case EFieldType_NVARCHAR:
			result = SFieldTypeNeedsLimits< EFieldType_NVARCHAR >::value;
			break;

		case EFieldType_NTEXT:
			result = SFieldTypeNeedsLimits< EFieldType_NTEXT >::value;
			break;

		case EFieldType_DATE:
			result = SFieldTypeNeedsLimits< EFieldType_DATE >::value;
			break;

		case EFieldType_DATETIME:
			result = SFieldTypeNeedsLimits< EFieldType_DATETIME >::value;
			break;

		case EFieldType_TIME:
			result = SFieldTypeNeedsLimits< EFieldType_TIME >::value;
			break;

		case EFieldType_BINARY:
			result = SFieldTypeNeedsLimits< EFieldType_BINARY >::value;
			break;

		case EFieldType_VARBINARY:
			result = SFieldTypeNeedsLimits< EFieldType_VARBINARY >::value;
			break;

		case EFieldType_BLOB:
			result = SFieldTypeNeedsLimits< EFieldType_BLOB >::value;
			break;

		default:
			result = false;
			break;
		}

		return result;
	}

	bool NeedsPrecision( EFieldType type )
	{
		bool result = false;

		switch ( type )
		{
		case EFieldType_NULL:
			result = SFieldTypeNeedsPrecision< EFieldType_NULL >::value;
			break;

		case EFieldType_BIT:
			result = SFieldTypeNeedsPrecision< EFieldType_BIT >::value;
			break;

		case EFieldType_SINT8:
			result = SFieldTypeNeedsPrecision< EFieldType_SINT8 >::value;
			break;

		case EFieldType_SINT16:
			result = SFieldTypeNeedsPrecision< EFieldType_SINT16 >::value;
			break;

		case EFieldType_SINT24:
			result = SFieldTypeNeedsPrecision< EFieldType_SINT24 >::value;
			break;

		case EFieldType_SINT32:
			result = SFieldTypeNeedsPrecision< EFieldType_SINT32 >::value;
			break;

		case EFieldType_SINT64:
			result = SFieldTypeNeedsPrecision< EFieldType_SINT64 >::value;
			break;

		case EFieldType_UINT8:
			result = SFieldTypeNeedsPrecision< EFieldType_UINT8 >::value;
			break;

		case EFieldType_UINT16:
			result = SFieldTypeNeedsPrecision< EFieldType_UINT16 >::value;
			break;

		case EFieldType_UINT24:
			result = SFieldTypeNeedsPrecision< EFieldType_UINT24 >::value;
			break;

		case EFieldType_UINT32:
			result = SFieldTypeNeedsPrecision< EFieldType_UINT32 >::value;
			break;

		case EFieldType_UINT64:
			result = SFieldTypeNeedsPrecision< EFieldType_UINT64 >::value;
			break;

		case EFieldType_FLOAT32:
			result = SFieldTypeNeedsPrecision< EFieldType_FLOAT32 >::value;
			break;

		case EFieldType_FLOAT64:
			result = SFieldTypeNeedsPrecision< EFieldType_FLOAT64 >::value;
			break;

		case EFieldType_FIXED_POINT:
			result = SFieldTypeNeedsPrecision< EFieldType_FIXED_POINT >::value;
			break;

		case EFieldType_CHAR:
			result = SFieldTypeNeedsPrecision< EFieldType_CHAR >::value;
			break;

		case EFieldType_VARCHAR:
			result = SFieldTypeNeedsPrecision< EFieldType_VARCHAR >::value;
			break;

		case EFieldType_TEXT:
			result = SFieldTypeNeedsPrecision< EFieldType_TEXT >::value;
			break;

		case EFieldType_NCHAR:
			result = SFieldTypeNeedsPrecision< EFieldType_NCHAR >::value;
			break;

		case EFieldType_NVARCHAR:
			result = SFieldTypeNeedsPrecision< EFieldType_NVARCHAR >::value;
			break;

		case EFieldType_NTEXT:
			result = SFieldTypeNeedsPrecision< EFieldType_NTEXT >::value;
			break;

		case EFieldType_DATE:
			result = SFieldTypeNeedsPrecision< EFieldType_DATE >::value;
			break;

		case EFieldType_DATETIME:
			result = SFieldTypeNeedsPrecision< EFieldType_DATETIME >::value;
			break;

		case EFieldType_TIME:
			result = SFieldTypeNeedsPrecision< EFieldType_TIME >::value;
			break;

		case EFieldType_BINARY:
			result = SFieldTypeNeedsPrecision< EFieldType_BINARY >::value;
			break;

		case EFieldType_VARBINARY:
			result = SFieldTypeNeedsPrecision< EFieldType_VARBINARY >::value;
			break;

		case EFieldType_BLOB:
			result = SFieldTypeNeedsPrecision< EFieldType_BLOB >::value;
			break;

		default:
			result = false;
			break;
		}

		return result;
	}

	CDatabaseValuedObjectInfos::CDatabaseValuedObjectInfos( const String & name )
		: CDatabaseValuedObjectInfos( name, EFieldType_NULL, -1 )
	{
		// Empty
	}

	CDatabaseValuedObjectInfos::CDatabaseValuedObjectInfos( const String & name, EFieldType type )
		: CDatabaseValuedObjectInfos( name, type, -1 )
	{
		if ( NeedsLimits( type ) )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_MISSING_LIMITS + name );
		}

		if ( NeedsPrecision( type ) )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_MISSING_PRECISION + name );
		}
	}

	CDatabaseValuedObjectInfos::CDatabaseValuedObjectInfos( const String & name, EFieldType type, uint32_t limits )
		: _name( name )
		, _type( type )
		, _precision( std::make_pair( limits, 0 ) )
	{
		if ( NeedsPrecision( type ) )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_MISSING_PRECISION + name );
		}
	}

	CDatabaseValuedObjectInfos::CDatabaseValuedObjectInfos( const String & name, EFieldType type, const std::pair< uint32_t, uint32_t > & precision )
		: _name( name )
		, _type( type )
		, _precision( precision )
	{
		// Empty
	}

	CDatabaseValuedObjectInfos::~CDatabaseValuedObjectInfos()
	{
		// Empty
	}

	EFieldType CDatabaseValuedObjectInfos::GetType() const
	{
		return _type;
	}

	void CDatabaseValuedObjectInfos::SetType( EFieldType type )
	{
		if ( NeedsLimits( type ) )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_MISSING_LIMITS + GetName() );
		}

		if ( NeedsPrecision( type ) )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_MISSING_PRECISION + GetName() );
		}

		_type = type;
	}

	void CDatabaseValuedObjectInfos::SetType( EFieldType type, uint32_t limits )
	{
		if ( NeedsPrecision( type ) )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_FieldError, ERROR_DB_MISSING_PRECISION + GetName() );
		}

		_type = type;
		_precision = std::make_pair( limits, 0 );
	}

	void CDatabaseValuedObjectInfos::SetType( EFieldType type, const std::pair< uint32_t, uint32_t > & precision )
	{
		_type = type;
		_precision = precision;
	}

	const String & CDatabaseValuedObjectInfos::GetName() const
	{
		return _name;
	}

	const uint32_t & CDatabaseValuedObjectInfos::GetLimits() const
	{
		return _precision.first;
	}

	const std::pair< uint32_t, uint32_t > & CDatabaseValuedObjectInfos::GetPrecision() const
	{
		return _precision;
	}
}
END_NAMESPACE_DATABASE
