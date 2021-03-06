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
#include <DatabaseFileUtils.h>

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
		int ExecuteScript( const String & path, const String & script, const StringArray & arguments )
		{
			// Command path
			StringStream commandStream;
			commandStream << path << PATH_SEP << script;
			std::string commandString = commandStream.str();

			// Execute
			spawn_pid_t processId = INVALID_PROCESS_ID;

			if ( !FileUtils::FileExists( commandString ) )
			{
				StringStream ss;
				ss << ERROR_UNABLE_TO_SPAWN_PROCESS << STR( "File does not exist\n" );
				DB_EXCEPT( EDatabaseExceptionCodes_InternalError, ss.str() );
			}

#if defined( _WIN32 )

			String commandLine = STR( "\"" ) + commandString + STR( "\"" );
			String sep;

			for ( auto && argument : arguments )
			{
				commandLine += STR( " " ) + argument;
			}

			TChar * szCommandLine = new TChar[commandLine.size() + 1];
			memset( szCommandLine, 0, commandLine.size() + 1 );
			strcpy_s( szCommandLine, commandLine.size() + 1, commandLine.c_str() );

			//!@remarks Create and execute a new process.
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

			int nChildResult = 0;
			::WaitForSingleObject( processId.hProcess, INFINITE );
			DWORD code;
			::GetExitCodeProcess( processId.hProcess, &code );
			::CloseHandle( processId.hThread );
			::CloseHandle( processId.hProcess );
			nChildResult = int( code );

#else

			// Command arguments
			std::vector< const char * > args;
			args.push_back( commandString.c_str() );

			for ( auto && argument : arguments )
			{
				args.push_back( argument.c_str() );
			}

			//!@remarks Create new process.
			processId = vfork();

			if ( !processId )
			{
				//!@remarks Execute child process.
				execv( commandString.c_str(), const_cast< char * const * >( args.data() ) );
				exit( 0 );
			}
			else
			{
				//!@remarks Parent process
				if ( processId == -1 )
				{
					StringStream ss;
					ss << ERROR_UNABLE_TO_FORK_CHILD_PROCESS;
					DB_EXCEPT( EDatabaseExceptionCodes_InternalError, ss.str() );
				}
			}

			int nChildResult = 0;
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

#if defined( _WIN32 )
	static const String SCRIPT_EXT = STR( ".bat" );
#else
	static const String SCRIPT_EXT = STR( ".sh" );
#endif

	static const String MYSQL = STR( "MySql" );
	static const String MSSQL = STR( "MsSql" );

	static const String SCRIPT_DATABASE_CREATE = STR( "CreateDatabase" );
	static const String SCRIPT_DATABASE_DELETE = STR( "DeleteDatabase" );
	static const String SCRIPT_ODBC_INSTALL = STR( "InstallOdbc" );
	static const String SCRIPT_ODBC_UNINSTALL = STR( "UninstallOdbc" );

	String MakeMySqlCommand()
	{
		StringStream ss;
		ss << STR( "\"" ) << MYSQL_COMMAND << STR( "\"" );
		return ss.str();
	}

	int InstallDatabaseMySql( const String & database, const String & user, const String & pass )
	{
		return ExecuteScript( SCRIPT_FILES_DIR, SCRIPT_DATABASE_CREATE + MYSQL + SCRIPT_EXT, { MakeMySqlCommand(), database, user, pass } );
	}

	int UninstallDatabaseMySql( const String & database, const String & user, const String & pass )
	{
		return ExecuteScript( SCRIPT_FILES_DIR, SCRIPT_DATABASE_DELETE + MYSQL + SCRIPT_EXT, { MakeMySqlCommand(), database, user, pass } );
	}

	int InstallSourceOdbcMySql( const String & database )
	{
		return ExecuteScript( SCRIPT_FILES_DIR, SCRIPT_ODBC_INSTALL + MYSQL + SCRIPT_EXT, { database } );
	}

	int UninstallSourceOdbcMySql( const String & database )
	{
		return ExecuteScript( SCRIPT_FILES_DIR, SCRIPT_ODBC_UNINSTALL + SCRIPT_EXT, { database } );
	}

#if defined( _WIN32 )
	int InstallDatabaseMsSql( const String & database, const String & user, const String & pass )
	{
		return ExecuteScript( SCRIPT_FILES_DIR, SCRIPT_DATABASE_CREATE + MSSQL + SCRIPT_EXT, { database, user, pass } );
	}

	int UninstallDatabaseMsSql( const String & database, const String & user, const String & pass )
	{
		return ExecuteScript( SCRIPT_FILES_DIR, SCRIPT_DATABASE_DELETE + MSSQL + SCRIPT_EXT, { database, user, pass } );
	}

	int InstallSourceOdbcMsSql( const String & database )
	{
		return ExecuteScript( SCRIPT_FILES_DIR, SCRIPT_ODBC_INSTALL + MSSQL + SCRIPT_EXT, { database } );
	}

	int UninstallSourceOdbcMsSql( const String & database )
	{
		return ExecuteScript( SCRIPT_FILES_DIR, SCRIPT_ODBC_UNINSTALL + SCRIPT_EXT, { database } );
	}
#endif

	namespace DatabaseUtils
	{
		const uint32_t Helpers< EFieldType_BIT >::Limit = -1;
		const uint32_t Helpers< EFieldType_SINT8 >::Limit = -1;
		const uint32_t Helpers< EFieldType_SINT16 >::Limit = -1;
		const uint32_t Helpers< EFieldType_SINT24 >::Limit = -1;
		const uint32_t Helpers< EFieldType_SINT32 >::Limit = -1;
		const uint32_t Helpers< EFieldType_SINT64 >::Limit = -1;
		const uint32_t Helpers< EFieldType_UINT8 >::Limit = -1;
		const uint32_t Helpers< EFieldType_UINT16 >::Limit = -1;
		const uint32_t Helpers< EFieldType_UINT24 >::Limit = -1;
		const uint32_t Helpers< EFieldType_UINT32 >::Limit = -1;
		const uint32_t Helpers< EFieldType_UINT64 >::Limit = -1;
		const uint32_t Helpers< EFieldType_FLOAT32 >::Limit = -1;
		const uint32_t Helpers< EFieldType_FLOAT64 >::Limit = -1;
		const std::pair< uint32_t, uint32_t > Helpers< EFieldType_FIXED_POINT >::Precision = std::make_pair( 10, 5 );
		const uint32_t Helpers< EFieldType_CHAR >::Limit = 20;
		const uint32_t Helpers< EFieldType_VARCHAR >::Limit = 20;
		const uint32_t Helpers< EFieldType_TEXT >::Limit = -1;
		const uint32_t Helpers< EFieldType_NCHAR >::Limit = 55;
		const uint32_t Helpers< EFieldType_NVARCHAR >::Limit = 55;
		const uint32_t Helpers< EFieldType_NTEXT >::Limit = -1;
		const uint32_t Helpers< EFieldType_DATE >::Limit = -1;
		const uint32_t Helpers< EFieldType_TIME >::Limit = -1;
		const uint32_t Helpers< EFieldType_DATETIME >::Limit = -1;
		const uint32_t Helpers< EFieldType_BINARY >::Limit = 20;
		const uint32_t Helpers< EFieldType_VARBINARY >::Limit = 255;
		const uint32_t Helpers< EFieldType_BLOB >::Limit = -1;
	}
}
END_NAMESPACE_DATABASE_TEST
