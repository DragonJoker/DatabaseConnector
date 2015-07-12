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
	template< typename CharType, typename LogStreambufTraits >
	class CLogStreambuf
		: public std::basic_streambuf< CharType >
	{
	public:
		typedef typename std::basic_streambuf< CharType >::int_type int_type;
		typedef typename std::basic_streambuf< CharType >::traits_type traits_type;

		CLogStreambuf( std::basic_ostream< CharType > & p_stream )
			: m_stream( p_stream )
		{
			m_old = m_stream.rdbuf( this );
		}

		virtual ~CLogStreambuf()
		{
			m_stream.rdbuf( m_old );
		}

		virtual int_type overflow( int_type c = traits_type::eof() )
		{
			if ( traits_type::eq_int_type( c, traits_type::eof() ) )
			{
				sync();
			}
			else
			{
				m_buffer += traits_type::to_char_type( c );
			}

			return c;
		}

		virtual int sync()
		{
			if ( !m_buffer.empty() )
			{
				LogStreambufTraits::Log( m_buffer );
				m_buffer.clear();
			}

			return 0;
		}

	private:
		std::basic_string< CharType > m_buffer;
		std::basic_ostream< CharType > & m_stream;
		std::basic_streambuf< CharType > * m_old;
	};

	template< typename CharType >
	struct STDebugLogStreambufTraits
	{
		static void Log( std::basic_string< CharType > const & p_text )
		{
			CLogger::LogDebug( CStrUtils::ToString( p_text ) );
		}
	};

	template< typename CharType >
	struct STMessageLogStreambufTraits
	{
		static void Log( std::basic_string< CharType > const & p_text )
		{
			CLogger::LogMessage( CStrUtils::ToString( p_text ) );
		}
	};

	template< typename CharType >
	struct STWarningLogStreambufTraits
	{
		static void Log( std::basic_string< CharType > const & p_text )
		{
			CLogger::LogWarning( CStrUtils::ToString( p_text ) );
		}
	};

	template< typename CharType >
	struct STErrorLogStreambufTraits
	{
		static void Log( std::basic_string< CharType > const & p_text )
		{
			CLogger::LogError( CStrUtils::ToString( p_text ) );
		}
	};

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

		DatabaseExport virtual void LogDebug( String const & CU_PARAM_UNUSED( p_strToLog ) ) {}
	};

	template <> class LeveledLogger< eLOG_TYPE_WARNING > : public ILoggerImpl
	{
	public:
		LeveledLogger()
			:	ILoggerImpl( eLOG_TYPE_WARNING )
		{
		}

		DatabaseExport virtual void LogDebug( String const & CU_PARAM_UNUSED( p_strToLog ) ) {}
		DatabaseExport virtual void LogMessage( String const & CU_PARAM_UNUSED( p_strToLog ) ) {}
	};

	template <> class LeveledLogger< eLOG_TYPE_ERROR > : public ILoggerImpl
	{
	public:
		LeveledLogger()
			:	ILoggerImpl( eLOG_TYPE_ERROR )
		{
		}

		DatabaseExport virtual void LogDebug( String const & CU_PARAM_UNUSED( p_strToLog ) ) {}
		DatabaseExport virtual void LogMessage( String const & CU_PARAM_UNUSED( p_strToLog ) ) {}
		DatabaseExport virtual void LogWarning( String const & CU_PARAM_UNUSED( p_strToLog ) ) {}
	};

	CLogger *	CLogger::m_pSingleton		= NULL;
	bool		CLogger::m_bOwnInstance	= true;
	uint32_t	CLogger::m_uiCounter		= 0;

	CLogger::CLogger()
		:	m_pImpl( NULL	)
	{
		std::unique_lock< std::mutex > lock( m_mutex );
		m_strHeaders[eLOG_TYPE_DEBUG	] = STR( "***DEBUG*** " );
		m_strHeaders[eLOG_TYPE_MESSAGE	] = STR( "" );
		m_strHeaders[eLOG_TYPE_WARNING	] = STR( "***WARNING*** " );
		m_strHeaders[eLOG_TYPE_ERROR	] = STR( "***ERROR*** " );

		m_cout = new CLogStreambuf< char, STMessageLogStreambufTraits< char > >( std::cout );
		m_cerr = new CLogStreambuf< char, STErrorLogStreambufTraits< char > >( std::cerr );
		m_clog = new CLogStreambuf< char, STDebugLogStreambufTraits< char > >( std::clog );
		m_wcout = new CLogStreambuf< wchar_t, STMessageLogStreambufTraits< wchar_t > >( std::wcout );
		m_wcerr = new CLogStreambuf< wchar_t, STErrorLogStreambufTraits< wchar_t > >( std::wcerr );
		m_wclog = new CLogStreambuf< wchar_t, STDebugLogStreambufTraits< wchar_t > >( std::wclog );
	}

	CLogger::~CLogger()
	{
		delete m_cout;
		delete m_cerr;
		delete m_clog;
		delete m_wcout;
		delete m_wcerr;
		delete m_wclog;

		if ( m_bOwnInstance )
		{
			m_pImpl->Cleanup();
			delete m_pImpl;
			m_pImpl = NULL;
		}
	}

	void CLogger::Initialise( CLogger * p_pLogger )
	{
		m_uiCounter++;
#if defined( _WIN32 )

		if ( m_pSingleton )
		{
			m_uiCounter--;
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

			l_logger.m_logLevel = p_pLogger->m_logLevel;
			l_logger.DoInitialiseThread();
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

			l_logger.m_logLevel = p_eLogLevel;
			l_logger.DoInitialiseThread();
		}
	}

	void CLogger::Cleanup()
	{
		GetSingleton().DoCleanupThread();

		if ( m_uiCounter > 0 )
		{
			m_uiCounter--;
			delete m_pSingleton;
			m_pSingleton = NULL;
		}
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
			LogDebug( std::string( l_pText ) );
		}
	}

	void CLogger::LogDebug( std::string const & p_msg )
	{
		GetSingleton().DoPushMessage( eLOG_TYPE_DEBUG, p_msg );
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
			LogDebug( std::wstring( l_pText ) );
		}
	}

	void CLogger::LogDebug( std::wstring const & p_msg )
	{
		GetSingleton().DoPushMessage( eLOG_TYPE_DEBUG, p_msg );
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
			LogMessage( std::string( l_pText ) );
		}
	}

	void CLogger::LogMessage( std::string const & p_msg )
	{
		GetSingleton().DoPushMessage( eLOG_TYPE_MESSAGE, p_msg );
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
			LogMessage( std::wstring( l_pText ) );
		}
	}

	void CLogger::LogMessage( std::wstring const & p_msg )
	{
		GetSingleton().DoPushMessage( eLOG_TYPE_MESSAGE, p_msg );
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
			LogWarning( std::string( l_pText ) );
		}
	}

	void CLogger::LogWarning( std::string const & p_msg )
	{
		GetSingleton().DoPushMessage( eLOG_TYPE_WARNING, p_msg );
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
			LogWarning( std::wstring( l_pText ) );
		}
	}

	void CLogger::LogWarning( std::wstring const & p_msg )
	{
		GetSingleton().DoPushMessage( eLOG_TYPE_WARNING, p_msg );
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
			LogError( std::string( l_pText ) );
		}
	}

	void CLogger::LogError( std::string const & p_msg )
	{
		GetSingleton().DoPushMessage( eLOG_TYPE_ERROR, p_msg );
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
			LogError( std::wstring( l_pText ) );
		}
	}

	void CLogger::LogError( std::wstring const & p_msg )
	{
		GetSingleton().DoPushMessage( eLOG_TYPE_ERROR, p_msg );
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

	void CLogger::DoPushMessage( eLOG_TYPE p_type, std::string const & p_message )
	{
		if ( p_type >= m_logLevel )
		{
			std::unique_lock< std::mutex > l_lock( m_mutexQueue );
			m_queue.push_back( std::make_unique< SMessage >( p_type, p_message ) );
		}
	}

	void CLogger::DoPushMessage( eLOG_TYPE p_type, std::wstring const & p_message )
	{
		if ( p_type >= m_logLevel )
		{
			std::unique_lock< std::mutex > l_lock( m_mutexQueue );
			m_queue.push_back( std::make_unique< SWMessage >( p_type, p_message ) );
		}
	}

	void CLogger::DoFlushQueue( bool p_display )
	{
		if ( !m_queue.empty() )
		{
			MessageQueue l_queue;

			{
				std::unique_lock< std::mutex > l_lock( m_mutexQueue );
				std::swap( l_queue, m_queue );
			}

			m_pImpl->LogMessageQueue( l_queue, p_display );
		}
	}

	void CLogger::DoInitialiseThread()
	{
		m_stopped = false;

		m_logThread = std::thread( [this]()
		{
			while ( !m_stopped )
			{
				DoFlushQueue( true );
				std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
			}

			DoFlushQueue( true );

			{
				std::unique_lock< std::mutex > l_lock( m_mutexThreadEnded );
				m_threadEnded.notify_one();
			}
		} );
	}

	void CLogger::DoCleanupThread()
	{
		m_stopped = true;

		{
			std::unique_lock< std::mutex > l_lock( m_mutexThreadEnded );
			m_threadEnded.wait( l_lock );
		}

		m_logThread.join();
	}
}
END_NAMESPACE_DATABASE
