/************************************************************************//**
* @file DatabaseLoggerImpl.h
* @author Sylvain Doremus
* @version 1.0
* @date 7/12/2015 7:51 PM
*
*
* @brief CLoggerImpl class
*
* @details Helper class for Logger, used to share one instance across multiple DLLs
*
***************************************************************************/

#include "DatabasePch.h"

#include "DatabaseLoggerImpl.h"

#include "DatabaseLoggerConsole.h"
#include "DatabaseLogger.h"
#include "DatabaseStringUtils.h"
#include "DatabaseDateTime.h"
#include "DatabaseFileUtils.h"

BEGIN_NAMESPACE_DATABASE
{
	class CLoggerImpl;

	CLoggerImpl::CLoggerImpl()
	{
#if defined( NDEBUG )
		_console = std::make_unique< CDefaultConsole >();
#else
		_console = std::make_unique< CDebugConsole >();
#endif
	}

	CLoggerImpl::~CLoggerImpl()
	{
		_console.reset();
	}

	void CLoggerImpl::Initialise( CLogger const & logger )
	{
		for ( int i = 0; i < ELogType_COUNT; i++ )
		{
			_headers[i] = logger._headers[i];
		}
	}

	void CLoggerImpl::Cleanup()
	{
	}

	void CLoggerImpl::SetFileName( String const & logFilePath, ELogType logLevel )
	{
		if ( logLevel == ELogType_COUNT )
		{
			for ( auto & path : _logFilePath )
			{
				path = logFilePath;
			}
		}
		else
		{
			_logFilePath[logLevel] = logFilePath;
		}

		FILE * file;
		FileUtils::FOpen( file, StringUtils::ToStr( logFilePath ).c_str(), "w" );

		if ( file )
		{
			fclose( file );
		}
	}

	void CLoggerImpl::PrintMessage( ELogType logLevel, std::string const & message )
	{
		DoPrintMessage( logLevel, StringUtils::ToString( message ) );
	}

	void CLoggerImpl::PrintMessage( ELogType logLevel, std::wstring const & message )
	{
		DoPrintMessage( logLevel, StringUtils::ToString( message ) );
	}

	void CLoggerImpl::LogMessageQueue( MessageQueue const & p_queue )
	{
		static const String LOG_TIMESTAMP = STR( "%Y-%m-%d %H:%M:%S" );
		DateTimeType today = boost::posix_time::second_clock::local_time();
		String timeStamp = DateTime::Format( today, LOG_TIMESTAMP );

		FILE * logFiles[ELogType_COUNT] = { NULL };
		std::map< String, FILE * > opened;

		try
		{
			for ( auto && message : p_queue )
			{
				FILE * file = logFiles[message->m_type];

				if ( !file )
				{
					auto && l_it = opened.find( _logFilePath[message->m_type] );

					if ( l_it == opened.end() )
					{
						FileUtils::FOpen( file, StringUtils::ToStr( _logFilePath[message->m_type] ).c_str(), "a" );

						if ( file )
						{
							opened.insert( std::make_pair( _logFilePath[message->m_type], file ) );
						}
					}
					else
					{
						file = l_it->second;
					}

					logFiles[message->m_type] = file;
				}

				if ( file )
				{
					String toLog = message->GetMessage();

					if ( toLog.find( STR( '\n' ) ) != String::npos )
					{
						StringArray array = StringUtils::Split( toLog, STR( "\n" ), uint32_t( std::count( toLog.begin(), toLog.end(), STR( '\n' ) ) + 1 ) );

						for ( auto && line : array )
						{
							DoLogLine( timeStamp, line, file, message->m_type );
						}
					}
					else
					{
						DoLogLine( timeStamp, toLog, file, message->m_type );
					}
				}
			}

			for ( auto && it : opened )
			{
				fclose( it.second );
			}
		}
		catch ( std::exception & )
		{
			//m_pConsole->Print( STR( "Couldn't open log file : " ) + StringUtils::ToString( exc.what() ), true );
		}
	}

	void CLoggerImpl::DoPrintMessage( ELogType logLevel, String const & message )
	{
		if ( message.find( STR( '\n' ) ) != String::npos )
		{
			StringArray array = StringUtils::Split( message, STR( "\n" ), uint32_t( std::count( message.begin(), message.end(), STR( '\n' ) ) + 1 ) );

			for ( auto && line : array )
			{
				DoPrintLine( line, logLevel );
			}
		}
		else
		{
			DoPrintLine( message, logLevel );
		}
	}

	void CLoggerImpl::DoPrintLine( String const & line, ELogType logLevel )
	{
		_console->BeginLog( logLevel );
		_console->Print( line, true );
	}

	void CLoggerImpl::DoLogLine( String const & timestamp, String const & line, FILE * logFile, ELogType logLevel )
	{
#if defined( NDEBUG )
		DoPrintLine( line, logLevel );
#endif

		if ( logFile )
		{
			std::string logLine = StringUtils::ToStr( timestamp + STR( " - " ) + _headers[logLevel] + line );
			fwrite( logLine.c_str(), 1, logLine.size(), logFile );
			fwrite( "\n", 1, 1, logFile );
		}
	}
}
END_NAMESPACE_DATABASE
