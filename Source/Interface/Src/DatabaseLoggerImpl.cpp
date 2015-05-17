#include "DatabasePch.h"

#include "DatabaseLoggerImpl.h"
#include "DatabaseLogger.h"
#include "DatabaseStringUtils.h"

BEGIN_NAMESPACE_DATABASE
{
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
			m_strHeaders[i]		= p_pLogger->m_strHeaders[i];
		}
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

	void ILoggerImpl::LogDebug( String const & p_strToLog )
	{
		DoLogMessage( p_strToLog,  eLOG_TYPE_DEBUG );
	}

	void ILoggerImpl::LogMessage( String const & p_strToLog )
	{
		DoLogMessage( p_strToLog,  eLOG_TYPE_MESSAGE );
	}

	void ILoggerImpl::LogWarning( String const & p_strToLog )
	{
		DoLogMessage( p_strToLog,  eLOG_TYPE_WARNING );
	}

	bool ILoggerImpl::LogError( String const & p_strToLog )
	{
		DoLogMessage( p_strToLog,  eLOG_TYPE_ERROR );
		return true;
	}

	void ILoggerImpl::DoLogMessage( String const & p_strToLog, eLOG_TYPE p_eLogType )
	{
		std::unique_lock< std::mutex > lock( m_mutex );
		StringStream l_strToLog;
		LoggerCallbackMapConstIt l_it;
		std::tm l_dtToday = { 0 };
		time_t l_tTime;

		time( &l_tTime );
		l_dtToday = *localtime( &l_tTime );

		l_strToLog << ( l_dtToday.tm_year + 1900 ) << STR( "-" );
		l_strToLog << ( l_dtToday.tm_mon + 1 < 10 ? STR( "0" ) : STR( "" ) ) << ( l_dtToday.tm_mon + 1 ) << STR( "-" ) << ( l_dtToday.tm_mday < 10 ? STR( "0" ) : STR( "" ) ) << l_dtToday.tm_mday;
		l_strToLog << STR( " - " ) << ( l_dtToday.tm_hour < 10 ? STR( "0" ) : STR( "" ) ) << l_dtToday.tm_hour << STR( ":" ) << ( l_dtToday.tm_min < 10 ? STR( "0" ) : STR( "" ) ) << l_dtToday.tm_min << STR( ":" ) << ( l_dtToday.tm_sec < 10 ? STR( "0" ) : STR( "" ) ) << l_dtToday.tm_sec << STR( "s" );
		l_strToLog << STR( " - " ) << m_strHeaders[p_eLogType];

		l_strToLog << p_strToLog;

		m_pConsole->BeginLog( p_eLogType );
		m_pConsole->Print( p_strToLog, true );

		try
		{
			FILE * l_logFile = NULL;
			Database::FOpen( l_logFile, CStrUtils::ToStr( m_logFilePath[p_eLogType] ).c_str(), "a" );

			if ( l_logFile )
			{
				String l_strLog = l_strToLog.str();
				fwrite( l_strLog.c_str(), 1, l_strLog.size(), l_logFile );
				fwrite( "\n", 1, 1, l_logFile );
				l_it = m_mapCallbacks.find( std::this_thread::get_id() );

				if ( l_it != m_mapCallbacks.end() && l_it->second.m_pfnCallback )
				{
					l_it->second.m_pfnCallback( l_it->second.m_pCaller, l_strLog, p_eLogType );
				}

				fclose( l_logFile );
			}
		}
		catch ( std::exception & exc )
		{
			m_pConsole->Print( STR( "Couldn't open log file : " ) + CStrUtils::ToString( exc.what() ), true );
		}
	}
}
END_NAMESPACE_DATABASE
