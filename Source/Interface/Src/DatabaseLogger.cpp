/************************************************************************//**
* @file DatabaseLogger.cpp
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

#include "DatabasePch.h"

#include "DatabaseLogger.h"

#include "DatabaseLoggerImpl.h"
#include "DatabaseStringUtils.h"
#include "DatabaseException.h"

BEGIN_NAMESPACE_DATABASE
{
	static const String ERROR_DB_LOGGER_ALREADY_INITIALISED = STR( "Logger instance already initialised" );

	template< typename CharType, typename LogStreambufTraits >
	class CLogStreambuf
		: public std::basic_streambuf< CharType >
	{
	public:
		typedef typename std::basic_streambuf< CharType >::int_type int_type;
		typedef typename std::basic_streambuf< CharType >::traits_type traits_type;

		CLogStreambuf( std::basic_ostream< CharType > & stream )
			: _stream( stream )
		{
			_old = _stream.rdbuf( this );
		}

		virtual ~CLogStreambuf()
		{
			_stream.rdbuf( _old );
		}

		virtual int_type overflow( int_type c = traits_type::eof() )
		{
			if ( traits_type::eq_int_type( c, traits_type::eof() ) )
			{
				sync();
			}
			else
			{
				_buffer += traits_type::to_char_type( c );
			}

			return c;
		}

		virtual int sync()
		{
			if ( !_buffer.empty() )
			{
				LogStreambufTraits::Log( _buffer );
				_buffer.clear();
			}

			return 0;
		}

	private:
		std::basic_string< CharType > _buffer;
		std::basic_ostream< CharType > & _stream;
		std::basic_streambuf< CharType > * _old;
	};

	template< typename CharType >
	struct STDebugLogStreambufTraits
	{
		static void Log( std::basic_string< CharType > const & text )
		{
			CLogger::LogDebug( text );
		}
	};

	template< typename CharType >
	struct STMessageLogStreambufTraits
	{
		static void Log( std::basic_string< CharType > const & text )
		{
			CLogger::LogInfo( text );
		}
	};

	template< typename CharType >
	struct STWarningLogStreambufTraits
	{
		static void Log( std::basic_string< CharType > const & text )
		{
			CLogger::LogWarning( text );
		}
	};

	template< typename CharType >
	struct STErrorLogStreambufTraits
	{
		static void Log( std::basic_string< CharType > const & text )
		{
			CLogger::LogError( text );
		}
	};

	CLogger * CLogger::_singleton = NULL;
	bool CLogger::_ownInstance = true;
	uint32_t CLogger::_counter = 0;

	CLogger::CLogger()
		: _impl( NULL )
	{
		std::unique_lock< std::mutex > lock( _mutex );
		_headers[ELogType_DEBUG] = STR( "***DEBUG*** " );
		_headers[ELogType_INFO] = STR( "" );
		_headers[ELogType_WARNING] = STR( "***WARNING*** " );
		_headers[ELogType_ERROR] = STR( "***ERROR*** " );

		std::cout.sync_with_stdio( false );
		std::cerr.sync_with_stdio( false );
		std::clog.sync_with_stdio( false );
		std::wcout.sync_with_stdio( false );
		std::wcerr.sync_with_stdio( false );
		std::wclog.sync_with_stdio( false );

		_cout = new CLogStreambuf< char, STMessageLogStreambufTraits< char > >( std::cout );
		_cerr = new CLogStreambuf< char, STErrorLogStreambufTraits< char > >( std::cerr );
		_clog = new CLogStreambuf< char, STDebugLogStreambufTraits< char > >( std::clog );
		_wcout = new CLogStreambuf< wchar_t, STMessageLogStreambufTraits< wchar_t > >( std::wcout );
		_wcerr = new CLogStreambuf< wchar_t, STErrorLogStreambufTraits< wchar_t > >( std::wcerr );
		_wclog = new CLogStreambuf< wchar_t, STDebugLogStreambufTraits< wchar_t > >( std::wclog );
	}

	CLogger::~CLogger()
	{
		delete _cout;
		delete _cerr;
		delete _clog;
		delete _wcout;
		delete _wcerr;
		delete _wclog;

		if ( _ownInstance )
		{
			_impl->Cleanup();
			delete _impl;
			_impl = NULL;
		}
	}

	void CLogger::Initialise( CLogger * logger )
	{
		_counter++;
#if defined( _WIN32 )

		if ( _singleton )
		{
			_counter--;
			DB_EXCEPT( EDatabaseExceptionCodes_InternalError, ERROR_DB_LOGGER_ALREADY_INITIALISED );
		}
		else
		{
			_ownInstance = false;
			CLogger & mylogger = GetSingleton();
			delete mylogger._impl;
			mylogger._impl = logger->_impl;

			for ( int i = 0; i < ELogType_COUNT; i++ )
			{
				mylogger._headers[i] = logger->_headers[i];
			}

			mylogger._logLevel = logger->_logLevel;
			mylogger.DoInitialiseThread();
		}

#endif
	}

	void CLogger::Initialise( ELogType logLevel )
	{
		_counter++;

		if ( _singleton )
		{
			DB_EXCEPT( EDatabaseExceptionCodes_InternalError, ERROR_DB_LOGGER_ALREADY_INITIALISED );
		}
		else
		{
			_ownInstance = true;
			CLogger & logger = GetSingleton();
			delete logger._impl;
			logger._impl = new CLoggerImpl;
			logger._impl->Initialise( logger );
			logger._logLevel = logLevel;
			logger.DoInitialiseThread();
		}
	}

	void CLogger::SetLevel( ELogType logLevel )
	{
		GetSingleton()._logLevel = logLevel;
	}

	void CLogger::Cleanup()
	{
		GetSingleton().DoCleanupThread();

		if ( _counter > 0 )
		{
			_counter--;
			delete _singleton;
			_singleton = NULL;
		}
	}

	void CLogger::SetFileName( String const & logFilePath, ELogType logLevel )
	{
		if ( GetSingleton()._impl )
		{
			GetSingleton().DoSetFileName( logFilePath, logLevel );
		}
	}

	void CLogger::LogDebug( char const * format, ... )
	{
		if ( format )
		{
			char buffer[256];
			va_list vaList;
			va_start( vaList, format );
			std::string format( format );
			vsnprintf( buffer, 256, format.c_str(), vaList );
			va_end( vaList );
			LogDebug( std::string( buffer ) );
		}
	}

	void CLogger::LogDebug( std::string const & msg )
	{
		GetSingleton().DoPushMessage( ELogType_DEBUG, msg );
	}

	void CLogger::LogDebug( std::ostream const & msg )
	{
		std::stringstream ss;
		ss << msg.rdbuf();
		LogDebug( ss.str() );
	}

	void CLogger::LogDebug( wchar_t const * format , ... )
	{
		if ( format )
		{
			wchar_t buffer[256];
			va_list vaList;
			va_start( vaList, format );
			std::wstring format( format );
#if defined( _MSC_VER ) || !defined( _WIN32 )
			vswprintf( buffer, 256, format.c_str(), vaList );
#else
			vswprintf( buffer, format.c_str(), vaList );
#endif
			va_end( vaList );
			LogDebug( std::wstring( buffer ) );
		}
	}

	void CLogger::LogDebug( std::wstring const & msg )
	{
		GetSingleton().DoPushMessage( ELogType_DEBUG, msg );
	}

	void CLogger::LogDebug( std::wostream const & msg )
	{
		std::wstringstream ss;
		ss << msg.rdbuf();
		LogDebug( ss.str() );
	}

	void CLogger::LogInfo( char const * format, ... )
	{
		if ( format )
		{
			char buffer[256];
			va_list vaList;
			va_start( vaList, format );
			std::string format( format );
			vsnprintf( buffer, 256, format.c_str(), vaList );
			va_end( vaList );
			LogInfo( std::string( buffer ) );
		}
	}

	void CLogger::LogInfo( std::string const & msg )
	{
		GetSingleton().DoPushMessage( ELogType_INFO, msg );
	}

	void CLogger::LogInfo( std::ostream const & msg )
	{
		std::stringstream ss;
		ss << msg.rdbuf();
		LogInfo( ss.str() );
	}

	void CLogger::LogInfo( wchar_t const * format , ... )
	{
		if ( format )
		{
			wchar_t buffer[256];
			va_list vaList;
			va_start( vaList, format );
			std::wstring format( format );
#if defined( _MSC_VER ) || !defined( _WIN32 )
			vswprintf( buffer, 256, format.c_str(), vaList );
#else
			vswprintf( buffer, format.c_str(), vaList );
#endif
			va_end( vaList );
			LogInfo( std::wstring( buffer ) );
		}
	}

	void CLogger::LogInfo( std::wstring const & msg )
	{
		GetSingleton().DoPushMessage( ELogType_INFO, msg );
	}

	void CLogger::LogInfo( std::wostream const & msg )
	{
		std::wstringstream ss;
		ss << msg.rdbuf();
		LogInfo( ss.str() );
	}

	void CLogger::LogWarning( char const * format, ... )
	{
		if ( format )
		{
			char buffer[256];
			va_list vaList;
			va_start( vaList, format );
			std::string format( format );
			vsnprintf( buffer, 256, format.c_str(), vaList );
			va_end( vaList );
			LogWarning( std::string( buffer ) );
		}
	}

	void CLogger::LogWarning( std::string const & msg )
	{
		GetSingleton().DoPushMessage( ELogType_WARNING, msg );
	}

	void CLogger::LogWarning( std::ostream const & msg )
	{
		std::stringstream ss;
		ss << msg.rdbuf();
		LogWarning( ss.str() );
	}

	void CLogger::LogWarning( wchar_t const * format , ... )
	{
		if ( format )
		{
			wchar_t buffer[256];
			va_list vaList;
			va_start( vaList, format );
			std::wstring format( format );
#if defined( _MSC_VER ) || !defined( _WIN32 )
			vswprintf( buffer, 256, format.c_str(), vaList );
#else
			vswprintf( buffer, format.c_str(), vaList );
#endif
			va_end( vaList );
			LogWarning( std::wstring( buffer ) );
		}
	}

	void CLogger::LogWarning( std::wstring const & msg )
	{
		GetSingleton().DoPushMessage( ELogType_WARNING, msg );
	}

	void CLogger::LogWarning( std::wostream const & msg )
	{
		std::wstringstream ss;
		ss << msg.rdbuf();
		LogWarning( ss.str() );
	}

	void CLogger::LogError( char const * format, ... )
	{
		if ( format )
		{
			char buffer[256];
			va_list vaList;
			va_start( vaList, format );
			std::string format( format );
			vsnprintf( buffer, 256, format.c_str(), vaList );
			va_end( vaList );
			LogError( std::string( buffer ) );
		}
	}

	void CLogger::LogError( std::string const & msg )
	{
		GetSingleton().DoPushMessage( ELogType_ERROR, msg );
	}

	void CLogger::LogError( std::ostream const & msg )
	{
		std::stringstream ss;
		ss << msg.rdbuf();
		LogError( ss.str() );
	}

	void CLogger::LogError( wchar_t const * format , ... )
	{
		if ( format )
		{
			wchar_t buffer[256];
			va_list vaList;
			va_start( vaList, format );
			std::wstring format( format );
#if defined( _MSC_VER ) || !defined( _WIN32 )
			vswprintf( buffer, 256, format.c_str(), vaList );
#else
			vswprintf( buffer, format.c_str(), vaList );
#endif
			va_end( vaList );
			LogError( std::wstring( buffer ) );
		}
	}

	void CLogger::LogError( std::wstring const & msg )
	{
		GetSingleton().DoPushMessage( ELogType_ERROR, msg );
	}

	void CLogger::LogError( std::wostream const & msg )
	{
		std::wstringstream ss;
		ss << msg.rdbuf();
		LogError( ss.str() );
	}

	CLogger & CLogger::GetSingleton()
	{
		if ( !_singleton )
		{
			_singleton = new CLogger();
		}

		return *_singleton;
	}

	CLogger * CLogger::GetSingletonPtr()
	{
		return &GetSingleton();
	}

	void CLogger::DoSetFileName( String const & logFilePath, ELogType logLevel )
	{
		std::unique_lock< std::mutex > lock( _mutex );
		_impl->SetFileName( logFilePath, logLevel );
	}

	void CLogger::DoPushMessage( ELogType logLevel, std::string const & message )
	{
		if ( logLevel >= _logLevel )
		{
#if !defined( NDEBUG )
			{
				std::unique_lock< std::mutex > lock( _mutex );
				_impl->PrintMessage( logLevel, message );
			}
#endif
			std::unique_lock< std::mutex > l_lock( _mutexQueue );
			_queue.push_back( std::make_unique< SMessage >( logLevel, message ) );
		}
	}

	void CLogger::DoPushMessage( ELogType logLevel, std::wstring const & message )
	{
		if ( logLevel >= _logLevel )
		{
#if !defined( NDEBUG )
			{
				std::unique_lock< std::mutex > lock( _mutex );
				_impl->PrintMessage( logLevel, message );
			}
#endif
			std::unique_lock< std::mutex > l_lock( _mutexQueue );
			_queue.push_back( std::make_unique< SWMessage >( logLevel, message ) );
		}
	}

	void CLogger::DoFlushQueue( bool display )
	{
		if ( !_queue.empty() )
		{
			MessageQueue queue;

			{
				std::unique_lock< std::mutex > l_lock( _mutexQueue );
				std::swap( queue, _queue );
			}

			_impl->LogMessageQueue( queue, display );
		}
	}

	void CLogger::DoInitialiseThread()
	{
		_stopped = false;
		_logThread = std::thread( [this]()
		{
			while ( !_stopped )
			{
				DoFlushQueue( true );
				std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
			}

			DoFlushQueue( false );
			{
				std::unique_lock< std::mutex > l_lock( _mutexThreadEnded );
				_threadEnded.notify_one();
			}
		} );
	}

	void CLogger::DoCleanupThread()
	{
		_stopped = true;
		{
			std::unique_lock< std::mutex > l_lock( _mutexThreadEnded );
			_threadEnded.wait( l_lock );
		}
		_logThread.join();
	}
}
END_NAMESPACE_DATABASE
