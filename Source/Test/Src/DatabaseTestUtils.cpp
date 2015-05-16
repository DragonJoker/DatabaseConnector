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
#   define AriaSleep( x ) Sleep( x )
#else
#   include <unistd.h>
#   include <sys/wait.h>
#   define AriaSleep( x ) usleep( 1000 * x )
#endif

#define yield() AriaSleep( 1 )

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
            commandStream << path << PATH_DELIM << script;
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
        
            TCHAR * szCommandLine = new TCHAR[commandLine.size() + 1];
            memset( szCommandLine, 0, commandLine.size() + 1 );
            _tcscpy_s( szCommandLine, commandLine.size() + 1, commandLine.c_str() );
        
            ///@remarks Define the security attributes for the process, we need at least PROCESS_QUERY_INFORMATION to be able to query for its ID in IsRunning function
        
            ///@remarks Create and execute a new process.
            STARTUPINFO si = { 0 };
            BOOL result = ::CreateProcess( NULL,
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
                ::GetCurrentDirectory( 1000, szBuffer );
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
                const char ** args = new const char *[_processArgs.size() + 1];
                size_t i = 0;
        
                for ( std::vector< const char * >::const_iterator it = _processArgs.begin(); it != _processArgs.end(); ++it )
                {
                    args[i++] = it->c_str();
                }
        
                args[i] = NULL;
        
                execv( _processPath.c_str(), const_cast< char * const * >( args ) );
                delete [] args;
                exit( 0 );
            }
            else
            {
                ///@remarks Parent process
                if ( processId == -1 )
                {
                    StringStream ss;
                    ss << ERROR_UNABLE_TO_FORK_CHILD_PROCESS << CSystemError::GetErrorDescription();
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
    
    static const String SCRIPT_DATABASE_INSTALL = STR( "CreateDatabase.bat" );
    static const String SCRIPT_DATABASE_UNINSTALL = STR( "DeleteDatabase.bat" );
    static const String SCRIPT_ODBC_INSTALL = STR( "Install_odbc_aria.bat" );
    static const String SCRIPT_ODBC_UNINSTALL = STR( "Uninstall_odbc_aria.bat" );
    
    int InstallDatabase( const String & database )
    {
        return ExecuteScriptSingleArg( DATABASE_BATCH_FILES_DIR, SCRIPT_DATABASE_INSTALL, database );
    }
    
    int UninstallDatabase( const String & database )
    {
        return ExecuteScriptSingleArg( DATABASE_BATCH_FILES_DIR, SCRIPT_DATABASE_UNINSTALL, database );
    }
    
    int InstallSourceOdbcAria( const String & dsn )
    {
        return ExecuteScriptSingleArg( ODBC_BATCH_FILES_DIR, SCRIPT_ODBC_INSTALL, dsn );
    }
    
    int UninstallSourceOdbcAria( const String & dsn )
    {
        return ExecuteScriptSingleArg( ODBC_BATCH_FILES_DIR, SCRIPT_ODBC_UNINSTALL, dsn );
    }
}
END_NAMESPACE_DATABASE_TEST

