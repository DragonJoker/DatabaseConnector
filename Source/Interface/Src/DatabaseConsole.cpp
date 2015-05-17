#include "DatabasePch.h"

#if defined _WIN32
#	if defined( _MSC_VER )
#		pragma warning( push )
#		pragma warning( disable:4311 )
#		pragma warning( disable:4312 )
#	endif
#	include <windows.h>
#	include <tchar.h>
#	if defined( _MSC_VER )
#		pragma warning( pop )
#	endif
#endif

#include "DatabaseConsole.h"

BEGIN_NAMESPACE_DATABASE
{
#if defined( _MSC_VER )
	class MsvcConsoleInfo : public IConsoleInfo
	{
	private:
		uint32_t				m_uiOldCP;
		HANDLE					m_hScreenBuffer;
		PCONSOLE_FONT_INFOEX	m_pOldInfos;
		bool					m_bAllocated;
		bool					m_bConsole;

	public:
		MsvcConsoleInfo()
			:	m_uiOldCP( 0	)
			,	m_hScreenBuffer( INVALID_HANDLE_VALUE	)
			,	m_pOldInfos( NULL	)
			,	m_bAllocated( false	)
			,	m_bConsole( false	)
		{
			if ( ::AllocConsole() )
			{
				m_bAllocated = true;
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

		virtual ~MsvcConsoleInfo()
		{
			if ( m_pOldInfos )
			{
				::SetCurrentConsoleFontEx( m_hScreenBuffer, FALSE, m_pOldInfos );
				delete m_pOldInfos;
			}

			if ( m_hScreenBuffer != INVALID_HANDLE_VALUE )
			{
				::CloseHandle( m_hScreenBuffer );
			}

			if ( m_bConsole )
			{
				::SetConsoleOutputCP( m_uiOldCP );
			}

			if ( m_bAllocated )
			{
				::FreeConsole();
			}
		}

		void BeginLog( eLOG_TYPE p_eLogType )
		{
			WORD l_wAttributes;

			switch ( p_eLogType )
			{
			case eLOG_TYPE_DEBUG	:
				l_wAttributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
				break;

			case eLOG_TYPE_WARNING	:
				l_wAttributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
				break;

			case eLOG_TYPE_ERROR	:
				l_wAttributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
				break;

			default:
				l_wAttributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
				break;
			}

			::SetConsoleTextAttribute( m_hScreenBuffer, l_wAttributes );
		}

		void Print( String const & p_strToLog, bool p_bNewLine )
		{
			::OutputDebugStringA( p_strToLog.c_str() );
			DWORD l_dwWritten = 0;

			if ( p_bNewLine )
			{
				::OutputDebugStringA( STR( "\n" ) );
				CONSOLE_SCREEN_BUFFER_INFO l_csbiInfo;

				if ( ::GetConsoleScreenBufferInfo( m_hScreenBuffer, &l_csbiInfo ) )
				{
					l_csbiInfo.dwCursorPosition.X = 0;
					::WriteConsole( m_hScreenBuffer, p_strToLog.c_str(), DWORD( p_strToLog.size() ), &l_dwWritten, NULL );
					SHORT l_sOffsetY = SHORT( 1 + l_dwWritten / l_csbiInfo.dwSize.X );

					if ( ( l_csbiInfo.dwSize.Y - l_sOffsetY ) <= l_csbiInfo.dwCursorPosition.Y )
					{
						// The cursor is on the last row
						SMALL_RECT l_srctScrollRect;
						CHAR_INFO l_chiFill;
						COORD l_coordDest;
						// The scroll rectangle is from second row to last displayed row
						l_srctScrollRect.Top = 1;
						l_srctScrollRect.Bottom = l_csbiInfo.dwSize.Y - 1;
						l_srctScrollRect.Left = 0;
						l_srctScrollRect.Right = l_csbiInfo.dwSize.X - 1;
						// The destination for the scroll rectangle is one row up.
						l_coordDest.X = 0;
						l_coordDest.Y = 0;
						// Set the fill character and attributes.
						l_chiFill.Attributes = 0;
						l_chiFill.Char.AsciiChar = char( ' ' );
						// Scroll
						::ScrollConsoleScreenBuffer( m_hScreenBuffer, &l_srctScrollRect, NULL, l_coordDest, &l_chiFill );
					}
					else
					{
						// The cursor isn't on the last row
						l_csbiInfo.dwCursorPosition.Y += l_sOffsetY;
					}

					::SetConsoleCursorPosition( m_hScreenBuffer, l_csbiInfo.dwCursorPosition );
				}
			}
			else
			{
				::WriteConsole( m_hScreenBuffer, p_strToLog.c_str(), DWORD( p_strToLog.size() ), &l_dwWritten, NULL );
			}
		}

	private:
		void DoInitialiseConsole()
		{
			m_hScreenBuffer = ::CreateConsoleScreenBuffer( GENERIC_WRITE | GENERIC_READ, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL );

			if ( m_hScreenBuffer != INVALID_HANDLE_VALUE && ::SetConsoleActiveScreenBuffer( m_hScreenBuffer ) )
			{
				m_pOldInfos = new CONSOLE_FONT_INFOEX;
				PCONSOLE_FONT_INFOEX l_pOldInfos = m_pOldInfos;
				l_pOldInfos->cbSize = sizeof( CONSOLE_FONT_INFOEX );

				if ( ::GetCurrentConsoleFontEx( m_hScreenBuffer, FALSE, l_pOldInfos ) )
				{
					CONSOLE_FONT_INFOEX l_infos = { 0 };
					l_infos.cbSize = sizeof( CONSOLE_FONT_INFOEX );
					l_infos.dwFontSize = l_pOldInfos->dwFontSize;
					l_infos.FontWeight = l_pOldInfos->FontWeight;
					l_infos.nFont = 6;
					l_infos.FontFamily = 54;
					_tcscpy( l_infos.FaceName, _T( "Lucida Console" ) );

					if ( !::SetCurrentConsoleFontEx( m_hScreenBuffer, FALSE, &l_infos ) )
					{
						delete m_pOldInfos;
						m_pOldInfos = NULL;
					}
				}
				else
				{
					delete m_pOldInfos;
					m_pOldInfos = NULL;
				}
			}

			m_uiOldCP = ::GetConsoleOutputCP();
			::EnumSystemCodePagesA( &DoCodePageProc, CP_INSTALLED );

			FILE * l_dump;
			freopen_s( & l_dump, "conout$", "w", stdout );
			freopen_s( & l_dump, "conout$", "w", stderr );
			m_bConsole = true;
		}
		static BOOL __stdcall DoCodePageProc( TChar * pszCodePageString )
		{
			BOOL l_bReturn = TRUE;
			String l_strCP( pszCodePageString );
			int l_iCP = stoi( l_strCP );

			if ( l_iCP == CP_UTF8 )
			{
				::SetConsoleCP( l_iCP );
				::SetConsoleOutputCP( l_iCP );
				l_bReturn = FALSE;
			}

			return l_bReturn;
		}
	};
#endif
#if defined( _WIN32 )
	class MswConsoleInfo : public IConsoleInfo
	{
	private:
		uint32_t		m_uiOldCP;
		HANDLE				m_hScreenBuffer;

	public:
		MswConsoleInfo()
			:	m_uiOldCP( 0	)
			,	m_hScreenBuffer( INVALID_HANDLE_VALUE	)
		{
			if ( ::AllocConsole() )
			{
				m_hScreenBuffer = ::CreateConsoleScreenBuffer( GENERIC_WRITE | GENERIC_READ, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL );

				if ( m_hScreenBuffer != INVALID_HANDLE_VALUE )
				{
					::SetConsoleActiveScreenBuffer( m_hScreenBuffer );
				}

				m_uiOldCP = ::GetConsoleOutputCP();
				::EnumSystemCodePagesA( &DoCodePageProc, CP_INSTALLED );

				freopen( "conout$", "w", stdout );
				freopen( "conout$", "w", stderr );
			}
		}

		virtual ~MswConsoleInfo()
		{
			::CloseHandle( m_hScreenBuffer );
			::SetConsoleOutputCP( m_uiOldCP );
			::FreeConsole();
		}

		void BeginLog( eLOG_TYPE p_eLogType )
		{
			WORD l_wAttributes;

			switch ( p_eLogType )
			{
			case eLOG_TYPE_DEBUG	:
				l_wAttributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
				break;

			case eLOG_TYPE_WARNING	:
				l_wAttributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
				break;

			case eLOG_TYPE_ERROR	:
				l_wAttributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
				break;

			default:
				l_wAttributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
				break;
			}

			::SetConsoleTextAttribute( m_hScreenBuffer, l_wAttributes );
		}

		void Print( String const & p_strToLog, bool p_bNewLine )
		{
			DWORD l_dwWritten;
			::WriteConsole( m_hScreenBuffer, p_strToLog.c_str(), DWORD( p_strToLog.size() ), & l_dwWritten, NULL );

			if ( p_bNewLine )
			{
				CONSOLE_SCREEN_BUFFER_INFO l_csbiInfo;

				if ( ::GetConsoleScreenBufferInfo( m_hScreenBuffer, & l_csbiInfo ) )
				{
					l_csbiInfo.dwCursorPosition.X = 0;

					if ( ( l_csbiInfo.dwSize.Y - 1 ) == l_csbiInfo.dwCursorPosition.Y )
					{
						SMALL_RECT l_srctScrollRect, l_srctClipRect;
						CHAR_INFO l_chiFill;
						COORD l_coordDest;

						l_srctScrollRect.Left = 0;
						l_srctScrollRect.Top = 1;
						l_srctScrollRect.Right = l_csbiInfo.dwSize.X - SHORT( 1 );
						l_srctScrollRect.Bottom = l_csbiInfo.dwSize.Y - SHORT( 1 );
						// The destination for the scroll rectangle is one row up.
						l_coordDest.X = 0;
						l_coordDest.Y = 0;
						// The clipping rectangle is the same as the scrolling rectangle.
						// The destination row is left unchanged.
						l_srctClipRect = l_srctScrollRect;
						// Set the fill character and attributes.
						l_chiFill.Attributes = 0;
						l_chiFill.Char.AsciiChar = char( ' ' );
						// Scroll up one line.
						::ScrollConsoleScreenBuffer( m_hScreenBuffer, & l_srctScrollRect, & l_srctClipRect, l_coordDest, & l_chiFill );
					}
					else
					{
						l_csbiInfo.dwCursorPosition.Y += 1;
					}

					::SetConsoleCursorPosition( m_hScreenBuffer, l_csbiInfo.dwCursorPosition );
				}
			}
		}

	private:
		static BOOL __stdcall DoCodePageProc( TChar * pszCodePageString )
		{
			BOOL l_bReturn = TRUE;
			String l_strCP( pszCodePageString );
			int l_iCP = stoi( l_strCP );

			if ( l_iCP == CP_UTF8 )
			{
				::SetConsoleCP( l_iCP );
				::SetConsoleOutputCP( l_iCP );
				l_bReturn = FALSE;
			}

			return l_bReturn;
		}
	};
#endif
	class GenConsoleInfo : public IConsoleInfo
	{
	private:
	public:
		GenConsoleInfo()
		{
		}

		virtual ~GenConsoleInfo()
		{
		}

		void BeginLog( eLOG_TYPE p_eLogType )
		{
		}

		void Print( String const & p_strToLog, bool p_bNewLine )
		{
			tcout << p_strToLog;

			if ( p_bNewLine )
			{
				tcout << std::endl;
			}
		}
	};

	DebugConsole::DebugConsole()
#if 0//defined( _MSC_VER )
		:	m_pConsoleInfo( new MsvcConsoleInfo )
#elif 0//defined( _WIN32 )
		:	m_pConsoleInfo( new MswConsoleInfo )
#else
		:	m_pConsoleInfo( new GenConsoleInfo )
#endif
	{
	}

	DebugConsole::~DebugConsole()
	{
		delete m_pConsoleInfo;
	}

	void DebugConsole::BeginLog( eLOG_TYPE p_eLogType )
	{
		m_pConsoleInfo->BeginLog( p_eLogType );
	}

	void DebugConsole::Print( String const & p_strToLog, bool p_bNewLine )
	{
		m_pConsoleInfo->Print( p_strToLog, p_bNewLine );
	}
}
END_NAMESPACE_DATABASE
