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
#include "DatabaseLogger.h"

#if !defined( NDEBUG )
#	if !defined( _WIN32 )
#		include <execinfo.h>
#		include <stdlib.h>
#	else
#		include <Windows.h>
#		include <Dbghelp.h>
#	endif
#endif

BEGIN_NAMESPACE_DATABASE
{
	const int NumOfFnCallsToCapture( 20 );
	const int NumOfFnCallsToSkip( 2 );

#if defined( NDEBUG )

	namespace
	{
		static void ShowBacktrace( std::stringstream & stream )
		{
		}
	}

#else
#	if !defined( _WIN32 )

	namespace
	{
		void ShowBacktrace( std::stringstream & stream )
		{
			stream << std::endl << "== Call Stack ==" << std::endl;

			// For now we just print out a message on sterr.
			void * backTrace[NumOfFnCallsToCapture];
			unsigned int num( ::backtrace( backTrace, NumOfFnCallsToCapture ) );
			char ** fnStrings( ::backtrace_symbols( backTrace, num ) );

			for ( unsigned i = NumOfFnCallsToSkip; i < num; ++i )
			{
				stream << "== " << fnStrings[i] << std::endl;
			}

			free( fnStrings );
		}
	}

#	else

	namespace
	{
		void ShowBacktrace( std::stringstream & stream )
		{
			const int MaxFnNameLen( 255 );

			void * backTrace[NumOfFnCallsToCapture - NumOfFnCallsToSkip];
			unsigned int num( ::CaptureStackBackTrace( NumOfFnCallsToSkip, NumOfFnCallsToCapture - NumOfFnCallsToSkip, backTrace, NULL ) );

			// symbol->Name type is char [1] so there is space for \0 already
			SYMBOL_INFO * symbol( ( SYMBOL_INFO * ) malloc( sizeof( SYMBOL_INFO ) + ( MaxFnNameLen * sizeof( char ) ) ) );
			symbol->MaxNameLen = MaxFnNameLen;
			symbol->SizeOfStruct = sizeof( SYMBOL_INFO );

			::HANDLE process( ::GetCurrentProcess() );
			SymInitialize( process, NULL, TRUE );

			stream << std::endl << "== Call Stack ==" << std::endl;

			// For now we just print out a message on sterr.
			for ( unsigned int i = 0; i < num; ++i )
			{
				SymFromAddr( process, reinterpret_cast< DWORD64 >( backTrace[i] ), 0, symbol );
				stream << "== " << std::string( symbol->Name, symbol->Name + symbol->NameLen ) << std::endl;
			}

			free( symbol );
		}
	}

#	endif
#endif

	CExceptionDatabase::CExceptionDatabase( int number, const String & description, const std::string & source, const std::string & file, long line )
		: _number( number )
		, _description( description )
		, _source( StringUtils::ToString( source ) )
		, _typeName( STR( "CExceptionDatabase" ) )
		, _file( StringUtils::ToString( file ) )
		, _line( line )
	{
		std::stringstream stream;
		ShowBacktrace( stream );
		_callstack = stream.str();
	}

	CExceptionDatabase::CExceptionDatabase( int number, const String & description, const std::string & source, const String & type, const std::string & file, long line )
		: _number( number )
		, _description( description )
		, _source( StringUtils::ToString( source ) )
		, _typeName( type )
		, _file( StringUtils::ToString( file ) )
		, _line( line )
	{
		std::stringstream stream;
		ShowBacktrace( stream );
		_callstack = stream.str();
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

			desc << _callstack;
			_fullDesc = desc.str();
			_what = StringUtils::ToStr( _fullDesc );
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
