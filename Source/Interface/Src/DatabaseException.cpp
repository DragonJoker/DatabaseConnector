/************************************************************************//**
* @file ExceptionDatabase.cpp
* @author [pnom]
* @version 1.0
* @date 11/08/2014 15:46:49
*
*
* @brief [your brief comment here]
*
* @details [your detailled comment here]
*
***************************************************************************/

#include "DatabasePch.h"

#include "DatabaseException.h"
#include "DatabaseStringUtils.h"

BEGIN_NAMESPACE_DATABASE
{
	CExceptionDatabase::CExceptionDatabase( int number, const String & description, const std::string & source, const std::string & file, long line )
		: _number( number )
		, _description( description )
		, _source( CStrUtils::ToString( source ) )
		, _typeName( STR( "CExceptionDatabase" ) )
		, _file( CStrUtils::ToString( file ) )
		, _line( line )
	{
		// Empty
	}

	CExceptionDatabase::CExceptionDatabase( int number, const String & description, const std::string & source, const String & type, const std::string & file, long line )
		: _number( number )
		, _description( description )
		, _source( CStrUtils::ToString( source ) )
		, _typeName( type )
		, _file( CStrUtils::ToString( file ) )
		, _line( line )
	{
		// Empty
	}

	const String & CExceptionDatabase::GetFullDescription() const
	{
		if ( _fullDesc.empty() )
		{
			StringStream desc;

			desc <<  "DATABASE EXCEPTION ( " << "number" << ":" << _typeName << " ) : "
			<< _description
			<< " in " << _source;

			if ( _line > 0 )
			{
				desc << " at " << _file << " ( line " << _line << " )";
			}

			_fullDesc = desc.str();
			_what = CStrUtils::ToStr( _fullDesc );
		}

		return _fullDesc;
	}

	const char * CExceptionDatabase::what()const throw()
	{
		GetFullDescription();
		return _what.c_str();
	}
}
END_NAMESPACE_DATABASE
