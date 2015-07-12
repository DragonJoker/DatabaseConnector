#include "DatabasePch.h"

#include "DatabaseLoggerImpl.h"
#include "DatabaseLogger.h"
#include "DatabaseStringUtils.h"
#include "DatabaseDateTime.h"

BEGIN_NAMESPACE_DATABASE
{
	class ILoggerImpl;

	ILoggerImpl::ILoggerImpl( eLOG_TYPE p_eLogLevel )
		:	m_eLogLevel( p_eLogLevel	)
		,	m_pConsole( NULL	)
	{
#if defined( NDEBUG )

		if ( p_eLogLevel == eLOG_TYPE_DEBUG )
		{
			m_pConsole = new DebugConsole;
		}
		else
		{
			m_pConsole = new DummyConsole;
		}

#else
		m_pConsole = new DebugConsole;
#endif
	}

	ILoggerImpl::~ILoggerImpl()
	{
		delete m_pConsole;
	}

	void ILoggerImpl::Initialise( CLogger * p_pLogger )
	{
		for ( int i = 0; i < eLOG_TYPE_COUNT; i++ )
		{
			m_strHeaders[i] = p_pLogger->m_strHeaders[i];
		}
	}

	void ILoggerImpl::Cleanup()
	{
	}

	void ILoggerImpl::SetCallback( PLogCallback p_pfnCallback, void * p_pCaller )
	{
		if ( p_pfnCallback )
		{
			stLOGGER_CALLBACK l_callback = { p_pfnCallback, p_pCaller };
			m_mapCallbacks[std::this_thread::get_id()] = l_callback;
		}
		else
		{
			LoggerCallbackMapIt l_it = m_mapCallbacks.find( std::this_thread::get_id() );

			if ( l_it != m_mapCallbacks.end() )
			{
				m_mapCallbacks.erase( l_it );
			}
		}
	}

	void ILoggerImpl::SetFileName( String const & p_logFilePath, eLOG_TYPE p_eLogType )
	{
		if ( p_eLogType == eLOG_TYPE_COUNT )
		{
			for ( int i = 0; i < eLOG_TYPE_COUNT; i++ )
			{
				m_logFilePath[i] = p_logFilePath;
			}
		}
		else
		{
			m_logFilePath[p_eLogType] = p_logFilePath;
		}

		FILE * l_pFile;
		FOpen( l_pFile, CStrUtils::ToStr( p_logFilePath ).c_str(), "w" );

		if ( l_pFile )
		{
			fclose( l_pFile );
		}
	}

	void ILoggerImpl::DoLogLine( String const & timestamp, String const & p_line, FILE * p_logFile, eLOG_TYPE p_eLogType, bool p_display )
	{
		if ( p_display )
		{
			m_pConsole->BeginLog( p_eLogType );
			m_pConsole->Print( p_line, true );
		}

		if ( p_logFile )
		{
			String l_strLog = timestamp + STR( " - " ) + m_strHeaders[p_eLogType] + p_line;
			fwrite( l_strLog.c_str(), 1, l_strLog.size(), p_logFile );
			fwrite( "\n", 1, 1, p_logFile );
			auto && l_it = m_mapCallbacks.find( std::this_thread::get_id() );

			if ( l_it != m_mapCallbacks.end() && l_it->second.m_pfnCallback )
			{
				l_it->second.m_pfnCallback( l_it->second.m_pCaller, l_strLog, p_eLogType );
			}
		}
	}

	void ILoggerImpl::LogMessageQueue( MessageQueue const & p_queue, bool p_display )
	{
		std::unique_lock< std::mutex > l_outlock( m_outMutex );
		static const String LOG_TIMESTAMP = STR( "%Y-%m-%d %H:%M:%S" );
		std::unique_lock< std::mutex > lock( m_mutex );
		CDateTime l_today = CDateTime::Now();
		String l_timeStamp = l_today.Format( LOG_TIMESTAMP );

		FILE * l_logFiles[eLOG_TYPE_COUNT] = { NULL };
		std::map< String, FILE * > l_opened;

		try
		{
			for ( auto && l_message : p_queue )
			{
				FILE * l_file = l_logFiles[l_message->m_type];

				if ( !l_file )
				{
					auto && l_it = l_opened.find( m_logFilePath[l_message->m_type] );

					if ( l_it == l_opened.end() )
					{
						Database::FOpen( l_file, CStrUtils::ToStr( m_logFilePath[l_message->m_type] ).c_str(), "a" );

						if ( l_file )
						{
							l_opened.insert( std::make_pair( m_logFilePath[l_message->m_type], l_file ) );
						}
					}
					else
					{
						l_file = l_it->second;
					}

					l_logFiles[l_message->m_type] = l_file;
				}

				if ( l_file )
				{
					String l_toLog = l_message->GetMessage();

					if ( l_toLog.find( STR( '\n' ) ) != String::npos )
					{
						StringArray array = CStrUtils::Split( l_toLog, STR( "\n" ), uint32_t( std::count( l_toLog.begin(), l_toLog.end(), STR( '\n' ) ) + 1 ) );

						for ( auto && line : array )
						{
							DoLogLine( l_timeStamp, line, l_file, l_message->m_type, p_display );
						}
					}
					else
					{
						DoLogLine( l_timeStamp, l_toLog, l_file, l_message->m_type, p_display );
					}
				}
			}

			for ( auto && l_it : l_opened )
			{
				fclose( l_it.second );
			}
		}
		catch ( std::exception & )
		{
			//m_pConsole->Print( STR( "Couldn't open log file : " ) + CStrUtils::ToString( exc.what() ), true );
		}
	}

	void ILoggerImpl::LogMessage( eLOG_TYPE p_eLogType, String const & p_strToLog, bool p_display )
	{
		std::unique_lock< std::mutex > l_outlock( m_outMutex );
		static const String LOG_TIMESTAMP = STR( "%Y-%m-%d %H:%M:%S" );
		std::unique_lock< std::mutex > lock( m_mutex );
		CDateTime l_today = CDateTime::Now();
		StringStream l_timeStamp;
		l_timeStamp << l_today.Format( LOG_TIMESTAMP ) << STR( " - " ) << m_strHeaders[p_eLogType];
		m_pConsole->BeginLog( p_eLogType );

		try
		{
			FILE * l_logFile = NULL;
			Database::FOpen( l_logFile, CStrUtils::ToStr( m_logFilePath[p_eLogType] ).c_str(), "a" );

			auto && l_printLine = [&p_display, &l_timeStamp, &l_logFile, &p_eLogType, this]( String const p_line )
			{
				if ( p_display )
				{
					m_pConsole->Print( p_line, true );
				}

				if ( l_logFile )
				{
					String l_strLog = l_timeStamp.str() + p_line;
					fwrite( l_strLog.c_str(), 1, l_strLog.size(), l_logFile );
					fwrite( "\n", 1, 1, l_logFile );
					auto && l_it = m_mapCallbacks.find( std::this_thread::get_id() );

					if ( l_it != m_mapCallbacks.end() && l_it->second.m_pfnCallback )
					{
						l_it->second.m_pfnCallback( l_it->second.m_pCaller, l_strLog, p_eLogType );
					}
				}
			};

			if ( p_strToLog.find( STR( '\n' ) ) != String::npos )
			{
				StringArray array = CStrUtils::Split( p_strToLog, STR( "\n" ), uint32_t( std::count( p_strToLog.begin(), p_strToLog.end(), STR( '\n' ) ) + 1 ) );

				for ( auto && line : array )
				{
					l_printLine( line );
				}
			}
			else
			{
				l_printLine( p_strToLog );
			}

			if ( l_logFile )
			{
				fclose( l_logFile );
			}
		}
		catch ( std::exception & )
		{
			//m_pConsole->Print( STR( "Couldn't open log file : " ) + CStrUtils::ToString( exc.what() ), true );
		}
	}
}
END_NAMESPACE_DATABASE
