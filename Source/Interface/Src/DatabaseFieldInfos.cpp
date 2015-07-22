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

	CDatabaseFieldInfos::CDatabaseFieldInfos( const String & name, EFieldType type, uint32_t limprec )
		: _name( name )
		, _type( type )
		, _precision( std::make_pair( limprec, 0 ) )
	{
		// Empty
	}

	CDatabaseFieldInfos::CDatabaseFieldInfos( const String & name, EFieldType type, std::pair< uint32_t, uint32_t > precision )
		: _name( name )
		, _type( type )
		, _precision( precision )
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
		return _precision.first;
	}

	const std::pair< uint32_t, uint32_t > & CDatabaseFieldInfos::GetPrecision() const
	{
		return _precision;
	}
}
END_NAMESPACE_DATABASE
