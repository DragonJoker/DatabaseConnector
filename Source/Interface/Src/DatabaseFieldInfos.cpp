/************************************************************************//**
 * @file DatabaseFieldInfos.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 *
 *
 * @brief CDatabaseFieldInfos class definition.
 *
 * @details Describes database field information.
 *
 ***************************************************************************/

#include "DatabasePch.h"

#include "DatabaseFieldInfos.h"

#include "DatabaseStringUtils.h"

BEGIN_NAMESPACE_DATABASE
{
	CDatabaseFieldInfos::CDatabaseFieldInfos( DatabaseConnectionPtr connection, const String & name, EFieldType type, uint32_t limits )
		:   _name( name )
		,   _type( type )
		,   _limits( limits )
		,   _connection( connection )
	{
		// Empty
	}

	CDatabaseFieldInfos::CDatabaseFieldInfos( DatabaseConnectionPtr connection, const String & name, const String & type, int length )
		:   _name( name )
		,   _type( EFieldType_NULL )
		,   _limits( 0 )
		,   _connection( connection )
	{
		String strTypel = CStrUtils::UpperCase( type );

		if ( strTypel.find( STR( "BIGINT" ) ) != String::npos || strTypel == STR( "MAX" ) || strTypel == STR( "COUNT" ) || strTypel == STR( "MIN" ) )
		{
			_type = EFieldType_LONG_INTEGER;
		}
		else if ( strTypel.find( STR( "SMALLINT" ) ) != String::npos )
		{
			_type = EFieldType_SMALL_INTEGER;
		}
		else if ( strTypel.find( STR( "INT" ) ) != String::npos )
		{
			_type = EFieldType_INTEGER;
		}
		else if ( strTypel.find( STR( "NCHAR" ) ) != String::npos || strTypel.find( STR( "NVARCHAR" ) ) != String::npos )
		{
			_type = EFieldType_NVARCHAR;

			if ( length < 0 )
			{
				std::size_t nIndex = strTypel.find( STR( "(" ) );

				if ( nIndex != String::npos )
				{
					strTypel = type.substr( nIndex + 1, type.find( STR( ")" ) ) - nIndex );
					_limits = CStrUtils::ToInt( CStrUtils::Trim( strTypel ) );
				}
			}
			else
			{
				_limits = length;
			}
		}
		else if ( strTypel.find( STR( "CHAR" ) ) != String::npos )
		{
			_type = EFieldType_VARCHAR;

			if ( length < 0 )
			{
				std::size_t nIndex = strTypel.find( STR( "(" ) );

				if ( nIndex != String::npos )
				{
					strTypel = type.substr( nIndex + 1, type.find( STR( ")" ) ) - nIndex );
					_limits = CStrUtils::ToInt( CStrUtils::Trim( strTypel ) );
				}
			}
			else
			{
				_limits = length;
			}
		}
		else if ( strTypel.find( STR( "NTEXT" ) ) != String::npos )
		{
			_type = EFieldType_NTEXT;
		}
		else if ( strTypel.find( STR( "CLOB" ) ) != String::npos || strTypel.find( STR( "TEXT" ) ) != String::npos )
		{
			_type = EFieldType_TEXT;
		}
		else if ( strTypel.find( STR( "REAL" ) ) != String::npos || strTypel.find( STR( "FLOA" ) ) != String::npos || strTypel == STR( "SUM" ) )
		{
			_type = EFieldType_FLOAT;
		}
		else if ( strTypel.find( STR( "DOUB" ) ) != String::npos || strTypel.find( STR( "DECIMAL" ) ) != String::npos )
		{
			_type = EFieldType_DOUBLE;
		}
		else if ( strTypel == STR( "DATETIME" ) )
		{
			_type = EFieldType_DATETIME;
		}
		else if ( strTypel == STR( "DATE" ) )
		{
			_type = EFieldType_DATE;
		}
		else if ( strTypel == STR( "TIME" ) )
		{
			_type = EFieldType_TIME;
		}
		else if ( strTypel.find( STR( "bool" ) ) != String::npos )
		{
			_type = EFieldType_BOOL;
		}
		else if ( strTypel.find( STR( "BLOB" ) ) != String::npos )
		{
			_type = EFieldType_LONG_VARBINARY;
		}
		else if ( strTypel.find( STR( "VARBINARY" ) ) != String::npos )
		{
			_type = EFieldType_VARBINARY;
		}
		else if ( strTypel.find( STR( "BINARY" ) ) != String::npos )
		{
			_type = EFieldType_BINARY;
		}
	}

	CDatabaseFieldInfos::CDatabaseFieldInfos( const CDatabaseFieldInfos & fieldInfos )
		:   _name( fieldInfos._name )
		,   _type( fieldInfos._type )
		,   _limits( fieldInfos._limits )
		,   _connection( fieldInfos._connection )
	{
		// Empty
	}

	CDatabaseFieldInfos::~CDatabaseFieldInfos()
	{
		// Empty
	}

	EFieldType CDatabaseFieldInfos::GetType() const
	{
		return _type;
	}

	const String & CDatabaseFieldInfos::GetName() const
	{
		return _name;
	}

	const uint32_t & CDatabaseFieldInfos::GetLimits() const
	{
		return _limits;
	}

	DatabaseConnectionPtr CDatabaseFieldInfos::GetConnection() const
	{
		return _connection;
	}
}
END_NAMESPACE_DATABASE
