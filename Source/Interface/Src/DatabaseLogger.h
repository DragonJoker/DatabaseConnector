/*
This source file is part of Castor3D (http://castor3d.developpez.com/castor3d.htm)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#ifndef ___DATABASE_LOGGER_H___
#define ___DATABASE_LOGGER_H___

#include "DatabaseLoggerImpl.h"

#include <deque>
#include <mutex>
#include <atomic>

BEGIN_NAMESPACE_DATABASE
{
	/*!
	\author		Sylvain DOREMUS
	\version	0.6.1.0
	\date		19/10/2011
	\~english
	\brief		Log management class
	\remark		Implements log facilities. Create a Log with a filename, then write logs into that file
	\~french
	\brief		Classe de gestion de logs
	\remark		Implémente les fonctions de logging. Initialise un log avec un nom de fichier puis écrit dedans
	*/
	class CLogger
	{
	private:
		/**
		 *\~english
		 *\brief		Constructor
		 *\~french
		 *\brief		Constructeur
		 */
		CLogger();
		/**
		 *\~english
		 *\brief		Destructor
		 *\~french
		 *\brief		Destructeur
		 */
		~CLogger();

	public:
		/**
		 *\~english
		 *\brief		Initialises this Logger instance to another one
		 *\param[in]	p_pLogger	The logger
		 *\~french
		 *\brief		Initialise l'instance de ce Logger à une autre
		 *\param[in]	p_pLogger	Le logger
		 */
		DatabaseExport static void Initialise( CLogger * p_pLogger );
		/**
		 *\~english
		 *\brief		Initialises this logger instance level
		 *\param[in]	p_eLogLevel		The log level
		 *\~french
		 *\brief		Initialise l'instance du logger avec le niveau donné
		 *\param[in]	p_eLogLevel		Le niveau de log
		 */
		DatabaseExport static void Initialise( eLOG_TYPE p_eLogLevel );
		/**
		 *\~english
		 *\brief		Destroys the Logger instance
		 *\~french
		 *\brief		Détruit l'instance du Logger
		 */
		DatabaseExport static void Cleanup();
		/**
		 *\~english
		 *\brief		Defines the logging callback
		 *\param[in]	p_pfnCallback	The callback
		 *\param[in]	p_pCaller		Pointer to user data
		 *\~french
		 *\brief		Définit la callback de log
		 *\param[in]	p_pfnCallback	La callback
		 *\param[in]	p_pCaller		Pointeur sur des données utilisateur
		 */
		DatabaseExport static void SetCallback( PLogCallback p_pfnCallback, void * p_pCaller );
		/**
		 *\~english
		 *\brief		Sets the log file address
		 *\param[in]	p_logFilePath	The log file path
		 *\param[in]	p_eLogType		The log type
		 *\~french
		 *\brief		Définit le chemin du fichier de log
		 *\param[in]	p_logFilePath	Le chemin du fichier
		 *\param[in]	p_eLogType		Le type de log concerné
		 */
		DatabaseExport static void SetFileName( String const & p_logFilePath, eLOG_TYPE p_eLogType = eLOG_TYPE_COUNT );
		/**
		 *\~english
		 *\brief		Logs a debug message in the log file, using va_args
		 *\param[in]	p_format	The line format
		 *\param[in]	...			POD arguments, following printf format
		 *\~french
		 *\brief		Log un message debug dans le fichier de log, en utilisant va_args
		 *\param[in]	p_format	Le format de la ligne
		 *\param[in]	...			Paramètres POD, utilise le format de printf
		 */
		DatabaseExport static void LogDebug( char const * p_format, ... );
		/**
		 *\~english
		 *\brief		Logs a debug message, from a std::string
		 *\param[in]	p_msg	The line to log
		 *\~french
		 *\brief		Log un message debug, à partir d'un std::string
		 *\param[in]	p_msg	The line to log
		 */
		DatabaseExport static void LogDebug( std::string const & p_msg );
		/**
		 *\~english
		 *\brief		Logs a debug message, from a std::stringstream
		 *\param[in]	p_msg	The line to log
		 *\~french
		 *\brief		Log un message debug, à partir d'un std::stringstream
		 *\param[in]	p_msg	The line to log
		 */
		DatabaseExport static void LogDebug( std::ostream const & p_msg );
		/**
		 *\~english
		 *\brief		Logs a unicode debug message in the log file, using va_args
		 *\param[in]	p_format	The line format
		 *\param[in]	...			POD arguments, following printf format
		 *\~french
		 *\brief		Log un message unicode debug dans le fichier de log, en utilisant va_args
		 *\param[in]	p_format	Le format de la ligne
		 *\param[in]	...			Paramètres POD, utilise le format de printf
		 */
		DatabaseExport static void LogDebug( wchar_t const * p_format , ... );
		/**
		 *\~english
		 *\brief		Logs a unicode debug message, from a std::wstring
		 *\param[in]	p_msg	The line to log
		 *\~french
		 *\brief		Log un message debug, à partir d'un std::wstring
		 *\param[in]	p_msg	The line to log
		 */
		DatabaseExport static void LogDebug( std::wstring const & p_msg );
		/**
		 *\~english
		 *\brief		Logs a debug message, from a std::wstringstream
		 *\param[in]	p_msg	The line to log
		 *\~french
		 *\brief		Log un message debug, à partir d'un std::wstringstream
		 *\param[in]	p_msg	The line to log
		 */
		DatabaseExport static void LogDebug( std::wostream const & p_msg );
		/**
		 *\~english
		 *\brief		Logs a message in the log file, using va_args
		 *\param[in]	p_format	The line format
		 *\param[in]	...			POD arguments, following printf format
		 *\~french
		 *\brief		Log un message dans le fichier de log, en utilisant va_args
		 *\param[in]	p_format	Le format de la ligne
		 *\param[in]	...			Paramètres POD, utilise le format de printf
		 */
		DatabaseExport static void LogMessage( char const * p_format, ... );
		/**
		 *\~english
		 *\brief		Logs a message, from a std::string
		 *\param[in]	p_msg	The line to log
		 *\~french
		 *\brief		Log un message, à partir d'un std::string
		 *\param[in]	p_msg	The line to log
		 */
		DatabaseExport static void LogMessage( std::string const & p_msg );
		/**
		 *\~english
		 *\brief		Logs a debug message, from a std::stringstream
		 *\param[in]	p_msg	The line to log
		 *\~french
		 *\brief		Log un message debug, à partir d'un std::stringstream
		 *\param[in]	p_msg	The line to log
		 */
		DatabaseExport static void LogMessage( std::ostream const & p_msg );
		/**
		 *\~english
		 *\brief		Logs a unicode line in the log file, using va_args
		 *\param[in]	p_format	The line format
		 *\param[in]	...			POD arguments, following printf format
		 *\~french
		 *\brief		Log une ligne unicode dans le fichier de log, en utilisant va_args
		 *\param[in]	p_format	Le format de la ligne
		 *\param[in]	...			Paramètres POD, utilise le format de printf
		 */
		DatabaseExport static void LogMessage( wchar_t const * p_format , ... );
		/**
		 *\~english
		 *\brief		Logs a message, from a std::wstring
		 *\param[in]	p_msg	The line to log
		 *\~french
		 *\brief		Log un message, à partir d'un std::wstring
		 *\param[in]	p_msg	The line to log
		 */
		DatabaseExport static void LogMessage( std::wstring const & p_msg );
		/**
		 *\~english
		 *\brief		Logs a debug message, from a std::wstringstream
		 *\param[in]	p_msg	The line to log
		 *\~french
		 *\brief		Log un message debug, à partir d'un std::wstringstream
		 *\param[in]	p_msg	The line to log
		 */
		DatabaseExport static void LogMessage( std::wostream const & p_msg );
		/**
		 *\~english
		 *\brief		Logs a warning in the log file, using va_args
		 *\param[in]	p_format	The line format
		 *\param[in]	...			POD arguments, following printf format
		 *\~french
		 *\brief		Log un avertissement dans le fichier de log, en utilisant va_args
		 *\param[in]	p_format	Le format de la ligne
		 *\param[in]	...			Paramètres POD, utilise le format de printf
		 */
		DatabaseExport static void LogWarning( char const * p_format, ... );
		/**
		 *\~english
		 *\brief		Logs a warning, from a std::string
		 *\param[in]	p_msg	The line to log
		 *\~french
		 *\brief		Log un avertissement, à partir d'un std::string
		 *\param[in]	p_msg	The line to log
		 */
		DatabaseExport static void LogWarning( std::string const & p_msg );
		/**
		 *\~english
		 *\brief		Logs a debug message, from a std::stringstream
		 *\param[in]	p_msg	The line to log
		 *\~french
		 *\brief		Log un message debug, à partir d'un std::stringstream
		 *\param[in]	p_msg	The line to log
		 */
		DatabaseExport static void LogWarning( std::ostream const & p_msg );
		/**
		 *\~english
		 *\brief		Logs a unicode warning in the log file, using va_args
		 *\param[in]	p_format	The line format
		 *\param[in]	...			POD arguments, following printf format
		 *\~french
		 *\brief		Log un avertissement en unicode dans le fichier de log, en utilisant va_args
		 *\param[in]	p_format	Le format de la ligne
		 *\param[in]	...			Paramètres POD, utilise le format de printf
		 */
		DatabaseExport static void LogWarning( wchar_t const * p_format , ... );
		/**
		 *\~english
		 *\brief		Logs a warning, from a std::wstring
		 *\param[in]	p_msg	The line to log
		 *\~french
		 *\brief		Log un avertissement, à partir d'un std::wstring
		 *\param[in]	p_msg	The line to log
		 */
		DatabaseExport static void LogWarning( std::wstring const & p_msg );
		/**
		 *\~english
		 *\brief		Logs a debug message, from a std::wstringstream
		 *\param[in]	p_msg	The line to log
		 *\~french
		 *\brief		Log un message debug, à partir d'un std::wstringstream
		 *\param[in]	p_msg	The line to log
		 */
		DatabaseExport static void LogWarning( std::wostream const & p_msg );
		/**
		 *\~english
		 *\brief		Logs an error in the log file, using va_args
		 *\param[in]	p_format	The line format
		 *\param[in]	...			POD arguments, following printf format
		 *\~french
		 *\brief		Log une erreur dans le fichier de log, en utilisant va_args
		 *\param[in]	p_format	Le format de la ligne
		 *\param[in]	...			Paramètres POD, utilise le format de printf
		 */
		DatabaseExport static void LogError( char const * p_format, ... );
		/**
		 *\~english
		 *\brief		Logs an error, from a std::string
		 *\param[in]	p_msg		The line to log
		 *\~french
		 *\brief		Log une erreur, à partir d'un std::string
		 *\param[in]	p_msg		The line to log
		 */
		DatabaseExport static void LogError( std::string const & p_msg );
		/**
		 *\~english
		 *\brief		Logs a debug message, from a std::stringstream
		 *\param[in]	p_msg	The line to log
		 *\~french
		 *\brief		Log un message debug, à partir d'un std::stringstream
		 *\param[in]	p_msg	The line to log
		 */
		DatabaseExport static void LogError( std::ostream const & p_msg );
		/**
		 *\~english
		 *\brief		Logs a unicode error in the log file, using va_args
		 *\param[in]	p_format	The line format
		 *\param[in]	...			POD arguments, following printf format
		 *\~french
		 *\brief		Log une erreur en unicode dans le fichier de log, en utilisant va_args
		 *\param[in]	p_format	Le format de la ligne
		 *\param[in]	...			Paramètres POD, utilise le format de printf
		 */
		DatabaseExport static void LogError( wchar_t const * p_format , ... );
		/**
		 *\~english
		 *\brief		Logs an error, from a std::wstring
		 *\param[in]	p_msg		The line to log
		 *\~french
		 *\brief		Log une erreur, à partir d'un std::wstring
		 *\param[in]	p_msg		The line to log
		 */
		DatabaseExport static void LogError( std::wstring const & p_msg );
		/**
		 *\~english
		 *\brief		Logs a debug message, from a std::wstringstream
		 *\param[in]	p_msg	The line to log
		 *\~french
		 *\brief		Log un message debug, à partir d'un std::wstringstream
		 *\param[in]	p_msg	The line to log
		 */
		DatabaseExport static void LogError( std::wostream const & p_msg );
		/**
		 *\~english
		 *\brief		Returns a reference over the instance
		 *\return		The instance
		 *\~french
		 *\brief		Retourne une référence sur l'instance
		 *\return		L'instance
		 */
		DatabaseExport static CLogger & GetSingleton();
		/**
		 *\~english
		 *\brief		Returns a pointer over the instance
		 *\return		The instance
		 *\~french
		 *\brief		Retourne un pointeur sur l'instance
		 *\return		L'instance
		 */
		DatabaseExport static CLogger * GetSingletonPtr();

	private:
		void DoSetCallback( PLogCallback p_pfnCallback, void * p_pCaller );
		void DoSetFileName( String const & p_logFilePath, eLOG_TYPE p_eLogType = eLOG_TYPE_COUNT );
		void DoPushMessage( eLOG_TYPE p_type, std::string const & p_message );
		void DoPushMessage( eLOG_TYPE p_type, std::wstring const & p_message );
		void DoInitialiseThread();
		void DoCleanupThread();
		void DoFlushQueue( bool p_display );

	private:
		friend class ILoggerImpl;

		static bool m_bOwnInstance;
		static CLogger * m_pSingleton;
		static uint32_t m_uiCounter;

		std::streambuf * m_cout;
		std::streambuf * m_cerr;
		std::streambuf * m_clog;
		std::wstreambuf * m_wcout;
		std::wstreambuf * m_wcerr;
		std::wstreambuf * m_wclog;

		ILoggerImpl * m_pImpl;
		eLOG_TYPE m_logLevel;
		std::mutex m_mutex;
		String m_strHeaders[eLOG_TYPE_COUNT];
		std::mutex m_mutexQueue;
		MessageQueue m_queue;
		std::thread m_logThread;
		std::atomic_bool m_stopped;
		std::mutex m_mutexThreadEnded;
		std::condition_variable m_threadEnded;
	};
}
END_NAMESPACE_DATABASE

#endif
