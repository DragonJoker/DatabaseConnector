/************************************************************************//**
 * @file DatabaseTestUtils.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 06/18/2014 12:04:46
 *
 *
 * @brief Utils for Database tests.
 *
 * @details Utils for Database tests.
 *
 ***************************************************************************/

#include "DatabaseTestPch.h"

#include "DatabaseTestUtils.h"

#include "DatabaseTestUtilities.h"

#include <DatabaseQuery.h>
#include <DatabaseResult.h>

#if defined( _WIN32 )
#   include <windows.h>
#   include <process.h>
#else
#   include <unistd.h>
#   include <sys/wait.h>
#endif

BEGIN_NAMESPACE_DATABASE_TEST
{
	namespace
	{
		const String ERROR_UNABLE_TO_SPAWN_PROCESS      = STR( "Unable to spawn the process in asynchronous mode! System Error: " );
		const String ERROR_UNABLE_TO_FORK_CHILD_PROCESS = STR( "Unable to fork a child process in asynchronous mode! System Error: " );
#if defined( _WIN32 )
		typedef PROCESS_INFORMATION spawn_pid_t;
		const DWORD INVALID_ID = 0xFFFFFFFF;
		static const PROCESS_INFORMATION INVALID_PROCESS_ID = { INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE, INVALID_ID, INVALID_ID };
#else
		typedef pid_t spawn_pid_t;
		static const pid_t INVALID_PROCESS_ID = -1;
#endif
		/** Executes a script with a single argument in a CProcess.
		@param path
		    Directory path to the script.
		@param script
		    Name of the script to execute.
		@param argument
		    Argument of the script.
		@return
		    Exit code of the CProcess executing the batch file.
		*/
		int ExecuteScriptSingleArg( const String & path, const String & script, const String & argument )
		{
			// Command path
			StringStream commandStream;
			commandStream << path << Database::PATH_SEP << script;
			std::string commandString = CStrUtils::ToStr( commandStream.str() );

			// Command arguments
			std::vector< const char * > args;
			args.push_back( commandString.c_str() );
			std::string arg = CStrUtils::ToStr( argument );
			args.push_back( arg.c_str() );

			// Execute
			spawn_pid_t processId = INVALID_PROCESS_ID;
			String processPath = CStrUtils::ToString( commandString );

			if ( !FileExists( processPath ) )
			{
				StringStream ss;
				ss << ERROR_UNABLE_TO_SPAWN_PROCESS << STR( "File does not exist\n" );
				DB_EXCEPT( EDatabaseExceptionCodes_InternalError, ss.str() );
			}

#if defined( _WIN32 )
			String command = processPath;
			String commandLine = STR( "\"" ) + command + STR( "\"" );
			String sep;

			for ( std::vector< const char * >::const_iterator it = args.begin() + 1; it != args.end(); ++it )
			{
				commandLine += STR( " " ) + CStrUtils::ToString( *it );
			}

			TChar * szCommandLine = new TChar[commandLine.size() + 1];
			memset( szCommandLine, 0, commandLine.size() + 1 );
			strcpy_s( szCommandLine, commandLine.size() + 1, commandLine.c_str() );

			///@remarks Define the security attributes for the process, we need at least PROCESS_QUERY_INFORMATION to be able to query for its ID in IsRunning function

			///@remarks Create and execute a new process.
			STARTUPINFOA si = { 0 };
			BOOL result = ::CreateProcessA( NULL,
											szCommandLine,
											NULL,
											NULL,
											FALSE,
											0,
											NULL,
											NULL,
											&si,
											&processId );

			delete [] szCommandLine;
			szCommandLine = NULL;

			if ( !result )
			{
				TChar szBuffer[1000];
				::GetCurrentDirectoryA( 1000, szBuffer );
				StringStream ss;
				ss << ERROR_UNABLE_TO_SPAWN_PROCESS << ::GetLastError() << "\nWorking directory : " << szBuffer << STR( "\n" );
				DB_EXCEPT( EDatabaseExceptionCodes_InternalError, ss.str() );
			}

#else

			///@remarks Create new process.
			processId = vfork();

			if ( !processId )
			{
				///@remarks Execute child process.
				execv( processPath.c_str(), const_cast< char * const * >( args.data() ) );
				exit( 0 );
			}
			else
			{
				///@remarks Parent process
				if ( processId == -1 )
				{
					StringStream ss;
					ss << ERROR_UNABLE_TO_FORK_CHILD_PROCESS;
					DB_EXCEPT( EDatabaseExceptionCodes_InternalError, ss.str() );
				}
			}

#endif
			int nChildResult = 0;
#if defined( _WIN32 )
			::WaitForSingleObject( processId.hProcess, INFINITE );
			DWORD code;
			::GetExitCodeProcess( processId.hProcess, &code );
			::CloseHandle( processId.hThread );
			::CloseHandle( processId.hProcess );
			nChildResult = int( code );
#else
			waitpid( processId, &nChildResult, 0 );

			if ( WIFEXITED( nChildResult ) )
			{
				nChildResult = WEXITSTATUS( nChildResult );
			}
			else
			{
				nChildResult = -1;
			}

#endif
			return nChildResult;
		}
	}
}
END_NAMESPACE_DATABASE_TEST

