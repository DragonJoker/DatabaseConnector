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

#ifndef ___DATABASE_LOGGER_IMPL_H___
#define ___DATABASE_LOGGER_IMPL_H___

#include "DatabasePrerequisites.h"

#include "ELogType.h"
#include "DatabaseStringUtils.h"

#pragma warning( push )
#pragma warning( disable:4290 )

BEGIN_NAMESPACE_DATABASE
{
	/** Base class for a message representation
	*/
	struct SMessageBase
	{
		/** Constructor
		@param[in] type
			The message type
		*/
		SMessageBase( ELogType type )
			: m_type( type )
		{
		}

		/** Retrieves the message content
		@return
			The message text
		*/
		virtual String GetMessage() = 0;

		//! The message type
		ELogType m_type;
	};

	/** Template class, holding character type dependant message text
	*/
	template< typename Char >
	struct SBasicMessage
		: public SMessageBase
	{
		typedef std::basic_string< Char > string_type;

		/** Constructor
		@param[in] type
			The message type
		@param[in] message
			The message text
		*/
		SBasicMessage( ELogType type, string_type const & message )
			: SMessageBase( type )
			, m_message( message )
		{
		}

		//@copydoc Database::SMessageBase::GetMessage
		virtual String GetMessage()
		{
			return StringUtils::ToString( m_message );
		}

		//! The message text
		string_type m_message;
	};

	//! A char message
	typedef SBasicMessage< char > SMessage;
	//! A wchar_t message
	typedef SBasicMessage< wchar_t > SWMessage;

	/** Helper class for Logger
	*/
	class CLoggerImpl
	{
	public:
		/** Constructor
		*/
		CLoggerImpl();

		/** Destructor
		*/
		virtual ~CLoggerImpl();

		/** Initialises the headers, from the given logger
		@param[in] logger
			The logger
		*/
		void Initialise( CLogger const & logger );

		/** Cleans up the class
		*/
		void Cleanup();

		/** Sets the file for given log level
		@param[in] logFilePath
			The file path
		@param[in] logLevel
			The log level.
			If ELogType_COUNT, sets the file for every log level
		*/
		void SetFileName( String const & logFilePath, ELogType logLevel );

		/** Prints a message to the console
		@param[in] logLevel
			The log level.
		@param[in] message
			The message.
		*/
		void PrintMessage( ELogType logLevel, std::string const & message );

		/** Prints a message to the console
		@param[in] logLevel
			The log level.
		@param[in] message
			The message.
		*/
		void PrintMessage( ELogType logLevel, std::wstring const & message );

		/** Logs a message queue
		@param[in] queue
			The messages
		*/
		void LogMessageQueue( MessageQueue const & queue );

	private:
		/** Prints a message to the console
		@param[in] logLevel
			The log level.
		@param[in] message
			The message.
		*/
		void DoPrintMessage( ELogType logLevel, String const & message );

		/** Prints a line to the console
		@param[in] line
			The line
		@param[in] logType
			The log level
		*/
		void DoPrintLine( String const & line, ELogType logType );

		/** Logs a line in the given file
		@param[in] timestamp
			The line timestamp
		@param[in] line
			The line
		@param logFile
			The file
		@param[in] logType
			The log level
		*/
		void DoLogLine( String const & timestamp, String const & line, FILE * logFile, ELogType logType );

	private:
		//! The files paths, per log level
		String _logFilePath[ELogType_COUNT];
		//! The headers, per log level
		String _headers[ELogType_COUNT];
		//! The console
		std::unique_ptr< CProgramConsole > _console;
	};
}
END_NAMESPACE_DATABASE

#pragma warning( pop)

#endif
