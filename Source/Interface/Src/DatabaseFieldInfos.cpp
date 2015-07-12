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
					result.first = CStrUtils::ToInt( CStrUtils::Trim( limit ) );
				}
				else
				{
					String limit1 = type.substr( index + 1, dotIndex - index );
					result.first = CStrUtils::ToInt( CStrUtils::Trim( limit1 ) );
					String limit2 = type.substr( dotIndex + 1, type.find( STR( ")" ) ) - dotIndex );
					result.second = CStrUtils::ToInt( CStrUtils::Trim( limit2 ) );
				}
			}

			return result;
		}
	}

	CDatabaseFieldInfos::CDatabaseFieldInfos( DatabaseConnectionPtr connection, const String & name, EFieldType type, uint32_t limprec )
		:   _name( name )
		,   _type( type )
		,   _limits( limprec )
		,   _connection( connection )
	{
		// Empty
	}

	CDatabaseFieldInfos::CDatabaseFieldInfos( DatabaseConnectionPtr connection, const String & name, const String & type, uint32_t limprec )
		:   _name( name )
		,   _type( EFieldType_NULL )
		,   _limits( limprec )
		,   _connection( connection )
	{
		String strTypel = CStrUtils::UpperCase( type );
		size_t index;

		if ( strTypel.find( STR( "BIGINT" ) ) != String::npos || strTypel == STR( "MAX" ) || strTypel == STR( "COUNT" ) || strTypel == STR( "MIN" ) )
		{
			_type = EFieldType_LONG_INTEGER;
		}
		else if ( strTypel.find( STR( "SMALLINT" ) ) != String::npos )
		{
			_type = EFieldType_SMALL_INTEGER;
		}
		else if ( strTypel.find( STR( "TINYINT" ) ) != String::npos )
		{
			_type = EFieldType_BOOL;
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
						_type = EFieldType_BOOL;
					}
					else if ( prec == 2 )
					{
						_type = EFieldType_SMALL_INTEGER;
					}
					else if ( prec <= 4 )
					{
						_type = EFieldType_INTEGER;
					}
					else
					{
						_type = EFieldType_LONG_INTEGER;
					}
				}
				else
				{
					_type = EFieldType_INTEGER;
				}
			}
			else
			{
				_type = EFieldType_INTEGER;
			}
		}
		else if ( strTypel.find( STR( "NCHAR" ) ) != String::npos || strTypel.find( STR( "NVARCHAR" ) ) != String::npos )
		{
			_type = EFieldType_NVARCHAR;

			if ( limprec == -1 )
			{
				_limits = RetrieveLimits( strTypel ).first;
			}
			else
			{
				_limits = limprec;
			}
		}
		else if ( strTypel.find( STR( "CHAR" ) ) != String::npos )
		{
			_type = EFieldType_VARCHAR;

			if ( limprec == -1 )
			{
				_limits = RetrieveLimits( strTypel ).first;
			}
			else
			{
				_limits = limprec;
			}
		}
		else if ( strTypel.find( STR( "NTEXT" ) ) != String::npos )
		{
			_type = EFieldType_NTEXT;
			_limits = -1;
		}
		else if ( strTypel.find( STR( "CLOB" ) ) != String::npos || strTypel.find( STR( "TEXT" ) ) != String::npos )
		{
			_type = EFieldType_TEXT;
			_limits = -1;
		}
		else if ( strTypel.find( STR( "FLOA" ) ) != String::npos || strTypel == STR( "SUM" ) )
		{
			_type = EFieldType_FLOAT;
		}
		else if ( strTypel.find( STR( "REAL" ) ) != String::npos || strTypel.find( STR( "DOUB" ) ) != String::npos )
		{
			_type = EFieldType_DOUBLE;
		}
		else if ( strTypel.find( STR( "DECIMAL" ) ) != String::npos )
		{
			_type = EFieldType_DOUBLE;

			if ( limprec == -1 )
			{
				_limits = RetrieveLimits( strTypel ).second;
			}
			else
			{
				_limits = limprec;
			}
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
		else if ( strTypel.find( STR( "BOOL" ) ) != String::npos )
		{
			_type = EFieldType_BOOL;
		}
		else if ( strTypel.find( STR( "BLOB" ) ) != String::npos )
		{
			_type = EFieldType_LONG_VARBINARY;
			_limits = -1;
		}
		else if ( strTypel.find( STR( "VARBINARY" ) ) != String::npos )
		{
			_type = EFieldType_VARBINARY;

			if ( limprec == -1 )
			{
				_limits = RetrieveLimits( strTypel ).first;
			}
			else
			{
				_limits = limprec;
			}
		}
		else if ( strTypel.find( STR( "BINARY" ) ) != String::npos )
		{
			_type = EFieldType_BINARY;

			if ( limprec == -1 )
			{
				_limits = RetrieveLimits( strTypel ).first;
			}
			else
			{
				_limits = limprec;
			}
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

	void CDatabaseFieldInfos::SetType( EFieldType type )
	{
		_type = type;
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
