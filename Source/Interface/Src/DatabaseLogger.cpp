#include "DatabasePch.h"

#if defined _WIN32
#	if defined( _MSC_VER )
#		pragma warning( push )
#		pragma warning( disable:4311 )
#		pragma warning( disable:4312 )
#	endif
#	include <windows.h>
#	if defined( _MSC_VER )
#		pragma warning( pop )
#	endif
#endif

#include "DatabaseLogger.h"
#include "DatabaseStringUtils.h"

BEGIN_NAMESPACE_DATABASE
{
	template< eLOG_TYPE Level >	class LeveledLogger;

	template <> class LeveledLogger< eLOG_TYPE_DEBUG > : public ILoggerImpl
	{
	public:
		LeveledLogger()
			:	ILoggerImpl( eLOG_TYPE_DEBUG )
		{
		}
	};

	template <> class LeveledLogger< eLOG_TYPE_MESSAGE > : public ILoggerImpl
	{
	public:
		LeveledLogger()
			:	ILoggerImpl( eLOG_TYPE_MESSAGE )
		{
		}

		virtual void LogDebug( String const & CU_PARAM_UNUSED( p_strToLog ) ) {}
	};

	template <> class LeveledLogger< eLOG_TYPE_WARNING > : public ILoggerImpl
	{
	public:
		LeveledLogger()
			:	ILoggerImpl( eLOG_TYPE_WARNING )
		{
		}

		virtual void LogDebug( String const & CU_PARAM_UNUSED( p_strToLog ) ) {}
		virtual void LogMessage( String const & CU_PARAM_UNUSED( p_strToLog ) ) {}
	};

	template <> class LeveledLogger< eLOG_TYPE_ERROR > : public ILoggerImpl
	{
	public:
		LeveledLogger()
			:	ILoggerImpl( eLOG_TYPE_ERROR )
		{
		}

		virtual void LogDebug( String const & CU_PARAM_UNUSED( p_strToLog ) ) {}
		virtual void LogMessage( String const & CU_PARAM_UNUSED( p_strToLog ) ) {}
		virtual void LogWarning( String const & CU_PARAM_UNUSED( p_strToLog ) ) {}
	};

	CLogger *	CLogger::m_pSingleton		= NULL;
	bool		CLogger::m_bOwnInstance	= true;
	uint32_t	CLogger::m_uiCounter		= 0;

	CLogger::CLogger()
		:	m_pImpl( NULL	)
	{
		std::unique_lock< std::mutex > lock( m_mutex );
		m_strHeaders[eLOG_TYPE_DEBUG	] = STR( "***DEBUG*** "	);
		m_strHeaders[eLOG_TYPE_MESSAGE	] = STR( "" );
		m_strHeaders[eLOG_TYPE_WARNING	] = STR( "***WARNING*** "	);
		m_strHeaders[eLOG_TYPE_ERROR	] = STR( "***ERROR*** "	);
	}

	CLogger::~CLogger()
	{
		if ( m_bOwnInstance )
		{
			delete m_pImpl;
		}
	}

	void CLogger::Initialise( CLogger * p_pLogger )
	{
		m_uiCounter++;
#if defined( _WIN32 )

		if ( m_pSingleton )
		{
			throw "Logger instance already initialised";
		}
		else
		{
			m_bOwnInstance = false;
			CLogger & l_logger = GetSingleton();
			delete l_logger.m_pImpl;

			l_logger.m_pImpl = p_pLogger->m_pImpl;

			for ( int i = 0; i < eLOG_TYPE_COUNT; i++ )
			{
				l_logger.m_strHeaders[i] = p_pLogger->m_strHeaders[i];
			}
		}

#endif
	}

	void CLogger::Initialise( eLOG_TYPE p_eLogLevel )
	{
		m_uiCounter++;

		if ( m_pSingleton )
		{
			throw "Logger instance already initialised";
		}
		else
		{
			m_bOwnInstance = true;

			CLogger & l_logger = GetSingleton();
			delete l_logger.m_pImpl;
			l_logger.m_pImpl = NULL;

			switch ( p_eLogLevel )
			{
			case eLOG_TYPE_DEBUG:
				l_logger.m_pImpl = new LeveledLogger< eLOG_TYPE_DEBUG >();
				break;

			case eLOG_TYPE_MESSAGE:
				l_logger.m_pImpl = new LeveledLogger< eLOG_TYPE_MESSAGE >();
				break;

			case eLOG_TYPE_WARNING:
				l_logger.m_pImpl = new LeveledLogger< eLOG_TYPE_WARNING >();
				break;

			case eLOG_TYPE_ERROR:
				l_logger.m_pImpl = new LeveledLogger< eLOG_TYPE_ERROR >();
				break;
			}

			if ( l_logger.m_pImpl )
			{
				l_logger.m_pImpl->Initialise( &l_logger );
			}
			else
			{
				throw std::range_error( "SetLogLevel subscript error" );
			}
		}
	}

	void CLogger::Cleanup()
	{
		m_uiCounter--;
#if !defined( _WIN32 )

		if ( m_uiCounter <= 0 )
		{
			delete m_pSingleton;
			m_pSingleton = NULL;
		}

#else
		delete m_pSingleton;
		m_pSingleton = NULL;
#endif
	}

	void CLogger::SetCallback( PLogCallback p_pfnCallback, void * p_pCaller )
	{
		GetSingleton().DoSetCallback( p_pfnCallback, p_pCaller );
	}

	void CLogger::SetFileName( String const & p_logFilePath, eLOG_TYPE p_eLogType )
	{
		if ( GetSingleton().m_pImpl )
		{
			GetSingleton().DoSetFileName( p_logFilePath, p_eLogType );
		}
	}

	void CLogger::LogDebug( char const * p_pFormat, ... )
	{
		if ( p_pFormat )
		{
			char l_pText[256];
			va_list l_vaList;
			va_start( l_vaList, p_pFormat );
			std::string l_strFormat( p_pFormat );
			vsnprintf( l_pText, 256, l_strFormat.c_str(), l_vaList );
			va_end( l_vaList );

			if ( GetSingleton().m_pImpl )
			{
				GetSingleton().m_pImpl->LogDebug( CStrUtils::ToString( l_pText ) );
			}
		}
	}

	void CLogger::LogDebug( std::string const & p_msg )
	{
		if ( GetSingleton().m_pImpl )
		{
			GetSingleton().m_pImpl->LogDebug( CStrUtils::ToString( p_msg ) );
		}
	}

	void CLogger::LogDebug( std::ostream const & p_msg )
	{
		std::stringstream ss;
		ss << p_msg.rdbuf();
		LogDebug( ss.str() );
	}

	void CLogger::LogDebug( wchar_t const * p_pFormat , ... )
	{
		if ( p_pFormat )
		{
			wchar_t l_pText[256];
			va_list l_vaList;
			va_start( l_vaList, p_pFormat );
			std::wstring l_strFormat( p_pFormat );
#if defined( _MSC_VER ) || !defined( _WIN32 )
			vswprintf( l_pText, 256, l_strFormat.c_str(), l_vaList );
#else
			vswprintf( l_pText, l_strFormat.c_str(), l_vaList );
#endif
			va_end( l_vaList );

			if ( GetSingleton().m_pImpl )
			{
				GetSingleton().m_pImpl->LogDebug( CStrUtils::ToString( l_pText ) );
			}
		}
	}

	void CLogger::LogDebug( std::wstring const & p_msg )
	{
		if ( GetSingleton().m_pImpl )
		{
			GetSingleton().m_pImpl->LogDebug( CStrUtils::ToString( p_msg ) );
		}
	}

	void CLogger::LogDebug( std::wostream const & p_msg )
	{
		std::wstringstream ss;
		ss << p_msg.rdbuf();
		LogDebug( ss.str() );
	}

	void CLogger::LogMessage( char const * p_pFormat, ... )
	{
		if ( p_pFormat )
		{
			char l_pText[256];
			va_list l_vaList;
			va_start( l_vaList, p_pFormat );
			std::string l_strFormat( p_pFormat );
			vsnprintf( l_pText, 256, l_strFormat.c_str(), l_vaList );
			va_end( l_vaList );

			if ( GetSingleton().m_pImpl )
			{
				GetSingleton().m_pImpl->LogMessage( CStrUtils::ToString( l_pText ) );
			}
		}
	}

	void CLogger::LogMessage( std::string const & p_msg )
	{
		if ( GetSingleton().m_pImpl )
		{
			GetSingleton().m_pImpl->LogMessage( CStrUtils::ToString( p_msg ) );
		}
	}

	void CLogger::LogMessage( std::ostream const & p_msg )
	{
		std::stringstream ss;
		ss << p_msg.rdbuf();
		LogMessage( ss.str() );
	}

	void CLogger::LogMessage( wchar_t const * p_pFormat , ... )
	{
		if ( p_pFormat )
		{
			wchar_t l_pText[256];
			va_list l_vaList;
			va_start( l_vaList, p_pFormat );
			std::wstring l_strFormat( p_pFormat );
#if defined( _MSC_VER ) || !defined( _WIN32 )
			vswprintf( l_pText, 256, l_strFormat.c_str(), l_vaList );
#else
			vswprintf( l_pText, l_strFormat.c_str(), l_vaList );
#endif
			va_end( l_vaList );

			if ( GetSingleton().m_pImpl )
			{
				GetSingleton().m_pImpl->LogMessage( CStrUtils::ToString( l_pText ) );
			}
		}
	}

	void CLogger::LogMessage( std::wstring const & p_msg )
	{
		if ( GetSingleton().m_pImpl )
		{
			GetSingleton().m_pImpl->LogMessage( CStrUtils::ToString( p_msg ) );
		}
	}

	void CLogger::LogMessage( std::wostream const & p_msg )
	{
		std::wstringstream ss;
		ss << p_msg.rdbuf();
		LogMessage( ss.str() );
	}

	void CLogger::LogWarning( char const * p_pFormat, ... )
	{
		if ( p_pFormat )
		{
			char l_pText[256];
			va_list l_vaList;
			va_start( l_vaList, p_pFormat );
			std::string l_strFormat( p_pFormat );
			vsnprintf( l_pText, 256, l_strFormat.c_str(), l_vaList );
			va_end( l_vaList );

			if ( GetSingleton().m_pImpl )
			{
				GetSingleton().m_pImpl->LogWarning( CStrUtils::ToString( l_pText ) );
			}
		}
	}

	void CLogger::LogWarning( std::string const & p_msg )
	{
		if ( GetSingleton().m_pImpl )
		{
			GetSingleton().m_pImpl->LogWarning( CStrUtils::ToString( p_msg ) );
		}
	}

	void CLogger::LogWarning( std::ostream const & p_msg )
	{
		std::stringstream ss;
		ss << p_msg.rdbuf();
		LogWarning( ss.str() );
	}

	void CLogger::LogWarning( wchar_t const * p_pFormat , ... )
	{
		if ( p_pFormat )
		{
			wchar_t l_pText[256];
			va_list l_vaList;
			va_start( l_vaList, p_pFormat );
			std::wstring l_strFormat( p_pFormat );
#if defined( _MSC_VER ) || !defined( _WIN32 )
			vswprintf( l_pText, 256, l_strFormat.c_str(), l_vaList );
#else
			vswprintf( l_pText, l_strFormat.c_str(), l_vaList );
#endif
			va_end( l_vaList );

			if ( GetSingleton().m_pImpl )
			{
				GetSingleton().m_pImpl->LogWarning( CStrUtils::ToString( l_pText ) );
			}
		}
	}

	void CLogger::LogWarning( std::wstring const & p_msg )
	{
		if ( GetSingleton().m_pImpl )
		{
			GetSingleton().m_pImpl->LogWarning( CStrUtils::ToString( p_msg ) );
		}
	}

	void CLogger::LogWarning( std::wostream const & p_msg )
	{
		std::wstringstream ss;
		ss << p_msg.rdbuf();
		LogWarning( ss.str() );
	}

	void CLogger::LogError( char const * p_pFormat, ... )
	{
		if ( p_pFormat )
		{
			char l_pText[256];
			va_list l_vaList;
			va_start( l_vaList, p_pFormat );
			std::string l_strFormat( p_pFormat );
			vsnprintf( l_pText, 256, l_strFormat.c_str(), l_vaList );
			va_end( l_vaList );

			if ( GetSingleton().m_pImpl )
			{
				GetSingleton().m_pImpl->LogError( CStrUtils::ToString( l_pText ) );
			}
		}
	}

	void CLogger::LogError( std::string const & p_msg )
	{
		if ( GetSingleton().m_pImpl )
		{
			GetSingleton().m_pImpl->LogError( CStrUtils::ToString( p_msg ) );
		}
	}

	void CLogger::LogError( std::ostream const & p_msg )
	{
		std::stringstream ss;
		ss << p_msg.rdbuf();
		LogError( ss.str() );
	}

	void CLogger::LogError( wchar_t const * p_pFormat , ... )
	{
		if ( p_pFormat )
		{
			wchar_t l_pText[256];
			va_list l_vaList;
			va_start( l_vaList, p_pFormat );
			std::wstring l_strFormat( p_pFormat );
#if defined( _MSC_VER ) || !defined( _WIN32 )
			vswprintf( l_pText, 256, l_strFormat.c_str(), l_vaList );
#else
			vswprintf( l_pText, l_strFormat.c_str(), l_vaList );
#endif
			va_end( l_vaList );

			if ( GetSingleton().m_pImpl )
			{
				GetSingleton().m_pImpl->LogError( CStrUtils::ToString( l_pText ) );
			}
		}
	}

	void CLogger::LogError( std::wstring const & p_msg )
	{
		if ( GetSingleton().m_pImpl )
		{
			GetSingleton().m_pImpl->LogError( CStrUtils::ToString( p_msg ) );
		}
	}

	void CLogger::LogError( std::wostream const & p_msg )
	{
		std::wstringstream ss;
		ss << p_msg.rdbuf();
		LogError( ss.str() );
	}

	CLogger & CLogger::GetSingleton()
	{
		if ( !m_pSingleton )
		{
			m_pSingleton = new CLogger();
		}

		return *m_pSingleton;
	}

	CLogger * CLogger::GetSingletonPtr()
	{
		return &GetSingleton();
	}

	void CLogger::DoSetCallback( PLogCallback p_pfnCallback, void * p_pCaller )
	{
		m_pImpl->SetCallback( p_pfnCallback, p_pCaller );
	}

	void CLogger::DoSetFileName( String const & p_logFilePath, eLOG_TYPE p_eLogType )
	{
		std::unique_lock< std::mutex > lock( m_mutex );
		m_pImpl->SetFileName( p_logFilePath, p_eLogType );
	}
}
END_NAMESPACE_DATABASE
