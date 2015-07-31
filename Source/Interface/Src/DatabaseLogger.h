/************************************************************************//**
* @file DatabaseLogger.h
* @author Sylvain Doremus
* @version 1.0
* @date 7/12/2015 7:51 PM
*
*
* @brief CLogger class
*
* @details Allows Debug, Info, Warning and Error logs
*
***************************************************************************/

#ifndef ___DATABASE_LOGGER_H___
#define ___DATABASE_LOGGER_H___

#include "DatabasePrerequisites.h"

#include "ELogType.h"

#include <condition_variable>
#include <mutex>
#include <atomic>

BEGIN_NAMESPACE_DATABASE
{
	/** Implements log facilities. Create a Log with a filename, then write logs into that file
	*/
	class CLogger
	{
	private:
		/** Constructor
		*/
		CLogger();
		/** Destructor
		*/
		~CLogger();

	public:
		/** Initialises this Logger instance to another one
		@param[in] logger
			The logger
		*/
		DatabaseExport static void Initialise( CLogger * logger );

		/** Initialises this logger instance level
		@param[in] logLevel
			The log level
		*/
		DatabaseExport static void Initialise( ELogType logLevel );

		/* Destroys the Logger instance
		*/
		DatabaseExport static void Cleanup();

		/** Sets the log file address
		@param[in] logFilePath
			The log file path
		@param[in] logType
			The log type
		*/
		DatabaseExport static void SetFileName( String const & logFilePath, ELogType logType = ELogType_COUNT );

		/** Logs a debug message in the log file, using va_args
		@param[in] format
			The line format
		@param[in] ...
			POD arguments, following printf format
		*/
		DatabaseExport static void LogDebug( char const * format, ... );

		/** Logs a debug message, from a std::string
		@param[in] msg
			The line to log
		*/
		DatabaseExport static void LogDebug( std::string const & msg );

		/** Logs a debug message, from a std::stringstream
		@param[in] msg
			The line to log
		*/
		DatabaseExport static void LogDebug( std::ostream const & msg );

		/** Logs a unicode debug message in the log file, using va_args
		@param[in] format
			The line format
		@param[in] ...
			POD arguments, following printf format
		*/
		DatabaseExport static void LogDebug( wchar_t const * format , ... );

		/** Logs a unicode debug message, from a std::wstring
		@param[in] msg
			The line to log
		*/
		DatabaseExport static void LogDebug( std::wstring const & msg );

		/** Logs a debug message, from a std::wstringstream
		@param[in] msg
			The line to log
		*/
		DatabaseExport static void LogDebug( std::wostream const & msg );

		/** Logs a message in the log file, using va_args
		@param[in] format
			The line format
		@param[in] ...
			POD arguments, following printf format
		*/
		DatabaseExport static void LogInfo( char const * format, ... );

		/** Logs a message, from a std::string
		@param[in] msg
			The line to log
		*/
		DatabaseExport static void LogInfo( std::string const & msg );

		/** Logs a debug message, from a std::stringstream
		@param[in] msg
			The line to log
		*/
		DatabaseExport static void LogInfo( std::ostream const & msg );

		/** Logs a unicode line in the log file, using va_args
		@param[in] format
			The line format
		@param[in] ...
			POD arguments, following printf format
		*/
		DatabaseExport static void LogInfo( wchar_t const * format , ... );

		/** Logs a message, from a std::wstring
		@param[in] msg
			The line to log
		*/
		DatabaseExport static void LogInfo( std::wstring const & msg );

		/** Logs a debug message, from a std::wstringstream
		@param[in] msg
			The line to log
		*/
		DatabaseExport static void LogInfo( std::wostream const & msg );

		/** Logs a warning in the log file, using va_args
		@param[in] format
			The line format
		@param[in] ...
			POD arguments, following printf format
		*/
		DatabaseExport static void LogWarning( char const * format, ... );

		/** Logs a warning, from a std::string
		@param[in] msg
			The line to log
		*/
		DatabaseExport static void LogWarning( std::string const & msg );

		/** Logs a debug message, from a std::stringstream
		@param[in] msg
			The line to log
		*/
		DatabaseExport static void LogWarning( std::ostream const & msg );

		/** Logs a unicode warning in the log file, using va_args
		@param[in] format
			The line format
		@param[in] ...
			POD arguments, following printf format
		*/
		DatabaseExport static void LogWarning( wchar_t const * format, ... );

		/** Logs a warning, from a std::wstring
		@param[in] msg
			The line to log
		*/
		DatabaseExport static void LogWarning( std::wstring const & msg );

		/** Logs a debug message, from a std::wstringstream
		@param[in] msg
			The line to log
		*/
		DatabaseExport static void LogWarning( std::wostream const & msg );

		/** Logs an error in the log file, using va_args
		@param[in] format
			The line format
		@param[in]	...
			POD arguments, following printf format
		*/
		DatabaseExport static void LogError( char const * format, ... );

		/** Logs an error, from a std::string
		@param[in] msg
			The line to log
		*/
		DatabaseExport static void LogError( std::string const & msg );

		/** Logs a debug message, from a std::stringstream
		@param[in] msg
			The line to log
		*/
		DatabaseExport static void LogError( std::ostream const & msg );

		/** Logs a unicode error in the log file, using va_args
		@param[in] format
			The line format
		@param[in] ...
			POD arguments, following printf format
		*/
		DatabaseExport static void LogError( wchar_t const * format, ... );

		/** Logs an error, from a std::wstring
		@param[in] msg
			The line to log
		*/
		DatabaseExport static void LogError( std::wstring const & msg );

		/** Logs a debug message, from a std::wstringstream
		@param[in] msg
			The line to log
		*/
		DatabaseExport static void LogError( std::wostream const & msg );

		/** Returns a reference over the instance
		@return
			The instance
		*/
		DatabaseExport static CLogger & GetSingleton();

		/** Returns a pointer over the instance
		@return
			The instance
		*/
		DatabaseExport static CLogger * GetSingletonPtr();

	private:
		void DoSetFileName( String const & logFilePath, ELogType logType = ELogType_COUNT );
		void DoPushMessage( ELogType type, std::string const & message );
		void DoPushMessage( ELogType type, std::wstring const & message );
		void DoInitialiseThread();
		void DoCleanupThread();
		void DoFlushQueue( bool display );

	private:
		friend class CLoggerImpl;

		//! Tells if the logger owns its instance or not
		static bool _ownInstance;
		//! The logger
		static CLogger * _singleton;
		//! The instances count
		static uint32_t _counter;
		//! The streambuf used to log info messages
		std::streambuf * _cout;
		//! The streambuf used to log error messages
		std::streambuf * _cerr;
		//! The streambuf used to log debug messages
		std::streambuf * _clog;
		//! The wstreambuf used to log info messages
		std::wstreambuf * _wcout;
		//! The wstreambuf used to log error messages
		std::wstreambuf * _wcerr;
		//! The wstreambuf used to log info messages
		std::wstreambuf * _wclog;
		//! The logger implementation
		CLoggerImpl * _impl;
		//! the mutex used to protect the implementation
		std::mutex _mutex;
		//! the current logging level, all logs lower than this level are ignored
		ELogType _logLevel;
		//! The header for each lg line of given log level
		String _headers[ELogType_COUNT];
		//! The message queue
		MessageQueue _queue;
		//! The mutex protecting the message queue
		std::mutex _mutexQueue;
		//! The logging thread
		std::thread _logThread;
		//! Tells if the thread must be stopped
		std::atomic_bool _stopped;
		//! Event raised when the thread is ended
		std::condition_variable _threadEnded;
		//! Mutex used to wait for the thread end
		std::mutex _mutexThreadEnded;
	};
}
END_NAMESPACE_DATABASE

#endif
