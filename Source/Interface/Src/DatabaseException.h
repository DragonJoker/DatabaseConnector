/************************************************************************//**
* @file ExceptionDatabase.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/18/2014 2:47:39 PM
*
*
* @brief CDatabaseException class declaration and definition.
*
* @details Should be thrown when a problem occured in the database interface.
*
***************************************************************************/

#ifndef ___DATABASE_EXCEPTION_DATABASE_H___
#define ___DATABASE_EXCEPTION_DATABASE_H___

#include "DatabasePrerequisites.h"

BEGIN_NAMESPACE_DATABASE
{
	/** Static definitions of database error codes.
	*/
	enum EDatabaseExceptionCodes
	{
		EDatabaseExceptionCodes_GenericError = 0,
		EDatabaseExceptionCodes_ConnectionError,
		EDatabaseExceptionCodes_UnknownError,
		EDatabaseExceptionCodes_DateTimeError,
		EDatabaseExceptionCodes_FieldError,
		EDatabaseExceptionCodes_QueryError,
		EDatabaseExceptionCodes_ParameterError,
		EDatabaseExceptionCodes_RowError,
		EDatabaseExceptionCodes_StatementError,
		EDatabaseExceptionCodes_Unimplemented,
		EDatabaseExceptionCodes_DuplicateItem,
		EDatabaseExceptionCodes_NullPointer,
		EDatabaseExceptionCodes_ItemNotFound,
		EDatabaseExceptionCodes_InternalError,
		EDatabaseExceptionCodes_ArithmeticError,
		EDatabaseExceptionCodes_SystemError,

		EDatabaseExceptionCodes_LastCode //!< Represent the maximum number of exception code. Must be always the last.
	};

	/** Should be thrown when a problem occured in the database interface.
	*/
	class CDatabaseException
		: public std::exception
	{
	public:
		/** Create a exception for the database.
		@param number
			Error code.
		@param description
			Error description.
		@param source
			Error source function.
		@param file
			Source file name.
		@param line
			Source file line number.
		*/
		DatabaseExport CDatabaseException( int number, const String & description, const std::string & source, const std::string & file, long line );

		/** Create a exception for the database.
		@param number
			Error code.
		@param description
			Error description.
		@param source
			Error source function.
		@param type
			Error type.
		@param file
			Source file name.
		@param line
			Source file line number.
		*/
		DatabaseExport CDatabaseException( const String & type, int number, const String & description, const std::string & source, const std::string & file, long line );

		/** Get the error code.
		@return
			Return the error code.
		*/
		DatabaseExport virtual int GetNumber() const throw()
		{
			return _number;
		}

		/** Get the error code.
		@return
			Return the error code.
		*/
		DatabaseExport virtual const String & GetNumberName() const throw();

		/** Get the source function.
		@return
			Return the error source function.
		*/
		DatabaseExport virtual const String & GetSource() const
		{
			return _source;
		}

		/** Get the source file name.
		@return
			Return error source file name.
		*/
		DatabaseExport virtual const String & GetFile() const
		{
			return _file;
		}

		/** Get the line number.
		@return
			Return error line number.
		*/
		DatabaseExport virtual long GetLine() const
		{
			return _line;
		}

		/** Return a string with only the 'description' field of this exception.
			@remarks
				Use GetFullDescriptionton to get a full description of the error including
				line number, error number and what function threw the exception.
		*/
		DatabaseExport virtual const String & GetDescription() const
		{
			return _description;
		}

		/** Return a string with the full description of this error.
			@remarks
				The description contains the error number, the description
				supplied by the thrower, what routine threw the exception,
				and will also supply extra platform-specific information if applicable.
		*/
		DatabaseExport virtual const String & GetFullDescription() const;

		/** Override std::exception::what */
		DatabaseExport const char * what() const throw();

		/** Makes system errors to be translated in CDatabaseExceptions
		@return
			The previous system error handler
		*/
		DatabaseExport static std::unique_ptr< CSystemExceptionHandler > LinkSystemErrors();

		/** Resets the system errors as before the previous call to LinkSystemErrors
		@param[in] previous, to restore it
			The previous system error handler
		*/
		DatabaseExport static void UnlinkSystemErrors( std::unique_ptr< CSystemExceptionHandler > && previous );

	private:
		//! The exception number
		int _number;
		//! The exception description
		String _description;
		//! The exception source
		String _source;
		//! Th type name
		String _typeName;
		//! The source file
		String _file;
		//! The source line
		long _line;
		//!< Full String error description.
		mutable String _fullDesc;
		//!< Full std::string error description.
		mutable std::string _what;
		//! The stack trace
		std::string _callstack;
	};

#	define DB_EXCEPT( number, description ) throw CDatabaseException( number, description, __FUNCTION__, __FILE__, __LINE__ )
}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_EXCEPTION_DATABASE_H___
