/************************************************************************//**
* @file DatabaseLoggerConsole.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 7/12/2015 7:51 PM
*
*
* @brief IConsoleInfo interface
*
* @details Console info class, platform dependant
*
***************************************************************************/

#include "DatabasePch.h"

#if defined _WIN32
#	if defined( _MSC_VER )
#		pragma warning( push )
#		pragma warning( disable:4311 )
#		pragma warning( disable:4312 )
#	endif
#	include <Windows.h>
#	include <tchar.h>
#	if defined( _MSC_VER )
#		pragma warning( pop )
#	endif
#endif

#include "DatabaseLoggerConsole.h"
#include "DatabaseStringUtils.h"

BEGIN_NAMESPACE_DATABASE
{
#if defined( _MSC_VER )
	class CMsvcConsoleInfo
		: public IConsole
	{
	public:
		CMsvcConsoleInfo()
			: _oldCodePage( 0 )
			, _screenBuffer( INVALID_HANDLE_VALUE )
			, _oldInfos( NULL )
			, _allocated( false )
			, _console( false )
		{
			if ( ::AllocConsole() )
			{
				_allocated = true;
				DoInitialiseConsole();
			}
			else
			{
				DWORD lastError = ::GetLastError();

				if ( lastError == ERROR_ACCESS_DENIED )
				{
					DoInitialiseConsole();
				}
				else
				{
					std::cout << "Failed to create to a new console with error " << lastError << std::endl;
				}
			}
		}

		virtual ~CMsvcConsoleInfo()
		{
			if ( _oldInfos )
			{
				::SetCurrentConsoleFontEx( _screenBuffer, FALSE, _oldInfos );
				delete _oldInfos;
			}

			if ( _screenBuffer != INVALID_HANDLE_VALUE )
			{
				::CloseHandle( _screenBuffer );
			}

			if ( _console )
			{
				::SetConsoleOutputCP( _oldCodePage );
			}

			if ( _allocated )
			{
				::FreeConsole();
			}
		}

		void BeginLog( ELogType logLevel )
		{
			WORD attributes;

			switch ( logLevel )
			{
			case ELogType_DEBUG:
				attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
				break;

			case ELogType_WARNING:
				attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
				break;

			case ELogType_ERROR:
				attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
				break;

			default:
				attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
				break;
			}

			::SetConsoleTextAttribute( _screenBuffer, attributes );
		}

		void Print( String const & toLog, bool newLine )
		{
			::OutputDebugStringA( toLog.c_str() );
			DWORD written = 0;

			if ( newLine )
			{
				::OutputDebugStringA( "\n" );
				CONSOLE_SCREEN_BUFFER_INFO csbiInfo;

				if ( ::GetConsoleScreenBufferInfo( _screenBuffer, &csbiInfo ) )
				{
					csbiInfo.dwCursorPosition.X = 0;
					::WriteConsoleA( _screenBuffer, toLog.c_str(), DWORD( toLog.size() ), &written, NULL );
					SHORT offsetY = SHORT( 1 + written / csbiInfo.dwSize.X );

					if ( ( csbiInfo.dwSize.Y - offsetY ) <= csbiInfo.dwCursorPosition.Y )
					{
						// The cursor is on the last row
						// The scroll rectangle is from second row to last displayed row
						SMALL_RECT scrollRect;
						scrollRect.Top = 1;
						scrollRect.Bottom = csbiInfo.dwSize.Y - 1;
						scrollRect.Left = 0;
						scrollRect.Right = csbiInfo.dwSize.X - 1;
						// The destination for the scroll rectangle is one row up.
						COORD coordDest;
						coordDest.X = 0;
						coordDest.Y = 0;
						// Set the fill character and attributes.
						CHAR_INFO fill;
						fill.Attributes = 0;
						fill.Char.AsciiChar = char( ' ' );
						// Scroll
						::ScrollConsoleScreenBuffer( _screenBuffer, &scrollRect, NULL, coordDest, &fill );
					}
					else
					{
						// The cursor isn't on the last row
						csbiInfo.dwCursorPosition.Y += offsetY;
					}

					::SetConsoleCursorPosition( _screenBuffer, csbiInfo.dwCursorPosition );
				}
			}
			else
			{
				::WriteConsole( _screenBuffer, toLog.c_str(), DWORD( toLog.size() ), &written, NULL );
			}
		}

	private:
		void DoInitialiseConsole()
		{
			_screenBuffer = ::CreateConsoleScreenBuffer( GENERIC_WRITE | GENERIC_READ, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL );

			if ( _screenBuffer != INVALID_HANDLE_VALUE && ::SetConsoleActiveScreenBuffer( _screenBuffer ) )
			{
				_oldInfos = new CONSOLE_FONT_INFOEX;
				CONSOLE_FONT_INFOEX * oldInfos = _oldInfos;
				oldInfos->cbSize = sizeof( CONSOLE_FONT_INFOEX );

				if ( ::GetCurrentConsoleFontEx( _screenBuffer, FALSE, oldInfos ) )
				{
					CONSOLE_FONT_INFOEX newInfos = { 0 };
					newInfos.cbSize = sizeof( CONSOLE_FONT_INFOEX );
					newInfos.dwFontSize = oldInfos->dwFontSize;
					newInfos.FontWeight = oldInfos->FontWeight;
					newInfos.nFont = 6;
					newInfos.FontFamily = 54;
					wcscpy( newInfos.FaceName, L"Lucida Console" );

					if ( !::SetCurrentConsoleFontEx( _screenBuffer, FALSE, &newInfos ) )
					{
						delete _oldInfos;
						_oldInfos = NULL;
					}
				}
				else
				{
					delete _oldInfos;
					_oldInfos = NULL;
				}

				COORD coord = { 160, 9999 };
				::SetConsoleScreenBufferSize( _screenBuffer, coord );
			}

			_oldCodePage = ::GetConsoleOutputCP();
			::EnumSystemCodePagesA( &DoCodePageProc, CP_INSTALLED );

			FILE * dump;
			freopen_s( &dump, "conout$", "w", stdout );
			freopen_s( &dump, "conout$", "w", stderr );
			_console = true;
		}
		static BOOL __stdcall DoCodePageProc( TChar * szCodePage )
		{
			BOOL ret = TRUE;
			String codePage( szCodePage );
			int cp = stoi( codePage );

			if ( cp == CP_UTF8 )
			{
				::SetConsoleCP( cp );
				::SetConsoleOutputCP( cp );
				ret = FALSE;
			}

			return ret;
		}

	private:
		uint32_t _oldCodePage;
		HANDLE _screenBuffer;
		CONSOLE_FONT_INFOEX * _oldInfos;
		bool _allocated;
		bool _console;
	};
#elif defined( _WIN32 )
	class CMswConsoleInfo
		: public IConsoleInfo
	{
	public:
		CMswConsoleInfo()
			:	_oldCodePage( 0	)
			,	_screenBuffer( INVALID_HANDLE_VALUE	)
			,	_allocated( false	)
			,	_console( false	)
		{
			if ( ::AllocConsole() )
			{
				_allocated = true;
				DoInitialiseConsole();
			}
			else
			{
				DWORD l_dwLastError = ::GetLastError();

				if ( l_dwLastError == ERROR_ACCESS_DENIED )
				{
					DoInitialiseConsole();
				}
				else
				{
					std::cout << "Failed to create to a new console with error " << l_dwLastError << std::endl;
				}
			}
		}

		virtual ~CMswConsoleInfo()
		{
			if ( _screenBuffer != INVALID_HANDLE_VALUE )
			{
				::CloseHandle( _screenBuffer );
			}

			if ( _console )
			{
				::SetConsoleOutputCP( _oldCodePage );
			}

			if ( _allocated )
			{
				::FreeConsole();
			}
		}

		void BeginLog( ELogType p_eLogType )
		{
			WORD l_wAttributes;

			switch ( p_eLogType )
			{
			case ELogType_DEBUG	:
				l_wAttributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
				break;

			case ELogType_WARNING	:
				l_wAttributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
				break;

			case ELogType_ERROR	:
				l_wAttributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
				break;

			default:
				l_wAttributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
				break;
			}

			::SetConsoleTextAttribute( _screenBuffer, l_wAttributes );
		}

		void Print( String const & toLog, bool newLine )
		{
			::OutputDebugStringA( toLog.c_str() );
			DWORD written = 0;

			if ( newLine )
			{
				::OutputDebugStringA( STR( "\n" ) );
				CONSOLE_SCREEN_BUFFER_INFO csbiInfo;

				if ( ::GetConsoleScreenBufferInfo( _screenBuffer, &csbiInfo ) )
				{
					csbiInfo.dwCursorPosition.X = 0;
					::WriteConsoleA( _screenBuffer, toLog.c_str(), DWORD( toLog.size() ), &written, NULL );
					SHORT offsetY = SHORT( 1 + written / csbiInfo.dwSize.Y );

					if ( ( csbiInfo.dwSize.Y - offsetY ) <= csbiInfo.dwCursorPosition.Y )
					{
						// The cursor is on the last row
						// The scroll rectangle is from second row to last displayed row
						SMALL_RECT scrollRect;
						scrollRect.Top = 1;
						scrollRect.Bottom = csbiInfo.dwSize.Y - 1;
						scrollRect.Left = 0;
						scrollRect.Right = csbiInfo.dwSize.X - 1;
						// The destination for the scroll rectangle is one row up.
						COORD coordDest;
						coordDest.X = 0;
						coordDest.Y = 0;
						// Set the fill character and attributes.
						CHAR_INFO fill;
						fill.Attributes = 0;
						fill.Char.AsciiChar = char( ' ' );
						// Scroll
						::ScrollConsoleScreenBuffer( _screenBuffer, &scrollRect, NULL, coordDest, &fill );
					}
					else
					{
						// The cursor isn't on the last row
						csbiInfo.dwCursorPosition.Y += offsetY;
					}

					::SetConsoleCursorPosition( _screenBuffer, csbiInfo.dwCursorPosition );
				}
			}
			else
			{
				::WriteConsoleA( _screenBuffer, toLog.c_str(), DWORD( toLog.size() ), &written, NULL );
			}
		}

	private:
		void DoInitialiseConsole()
		{
			_screenBuffer = ::CreateConsoleScreenBuffer( GENERIC_WRITE | GENERIC_READ, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL );

			if ( _screenBuffer != INVALID_HANDLE_VALUE && ::SetConsoleActiveScreenBuffer( _screenBuffer ) )
			{
				COORD coord = { 160, 9999 };
				::SetConsoleScreenBufferSize( _screenBuffer, coord );
			}

			_oldCodePage = ::GetConsoleOutputCP();
			::EnumSystemCodePages( & DoCodePageProc, CP_INSTALLED );
			FILE * dump;
			dump = freopen( "conout$", "w", stdout );
			dump = freopen( "conout$", "w", stderr );
			_console = true;
		}
		static BOOL __stdcall DoCodePageProc( TChar * szCodePage )
		{
			BOOL ret = TRUE;
			String codePage( szCodePage );
			int cp = stoi( codePage );

			if ( cp == CP_UTF8 )
			{
				::SetConsoleCP( cp );
				::SetConsoleOutputCP( cp );
				ret = FALSE;
			}

			return ret;
		}

	private:
		uint32_t _oldCodePage;
		HANDLE _screenBuffer;
		bool _allocated;
		bool _console;
	};
#else
	class CLinuxConsoleInfo
		: public IConsole
	{
	public:
		CLinuxConsoleInfo()
		{
		}

		virtual ~CLinuxConsoleInfo()
		{
		}

		void BeginLog( ELogType logLevel )
		{
			switch ( logLevel )
			{
			case ELogType_DEBUG:
				_header = STR( "\033[36m" );
				break;

			case ELogType_INFO:
				_header = STR( "\033[0m" );
				break;

			case ELogType_WARNING:
				_header = STR( "\033[33m" );
				break;

			case ELogType_ERROR:
				_header = STR( "\033[31m" );
				break;
			}
		}

		void Print( String const & toLog, bool newLine )
		{
			printf( "%s%s\033[0m", _header.c_str(), toLog.c_str() );

			if ( newLine )
			{
				printf( "\n" );
			}
		}

	private:
		String _header;
	};
#endif
	class CGenericConsoleInfo
		: public IConsole
	{
	public:
		CGenericConsoleInfo()
		{
		}

		virtual ~CGenericConsoleInfo()
		{
		}

		void BeginLog( ELogType logLevel )
		{
		}

		void Print( String const & toLog, bool newLine )
		{
			printf( "%s", toLog.c_str() );

			if ( newLine )
			{
				printf( "\n" );
			}
		}
	};

	CDefaultConsole::CDefaultConsole()
	{
		_console = std::make_unique< CGenericConsoleInfo >();
	}

	CDefaultConsole::~CDefaultConsole()
	{
		_console.reset();
	}

	void CDefaultConsole::BeginLog( ELogType p_eLogType )
	{
		_console->BeginLog( p_eLogType );
	}

	void CDefaultConsole::Print( String const & toLog, bool newLine )
	{
		_console->Print( toLog, newLine );
	}

	CDebugConsole::CDebugConsole()
	{
#if defined( _MSC_VER )
		_console = std::make_unique< CMsvcConsoleInfo >();
#elif defined( _WIN32 )
		_console = std::make_unique< CMswConsoleInfo >();
#else
		_console = std::make_unique< CLinuxConsoleInfo >();
#endif
	}

	CDebugConsole::~CDebugConsole()
	{
		_console.reset();
	}

	void CDebugConsole::BeginLog( ELogType logLevel )
	{
		_console->BeginLog( logLevel );
	}

	void CDebugConsole::Print( String const & toLog, bool newLine )
	{
		_console->Print( toLog, newLine );
	}
}
END_NAMESPACE_DATABASE
