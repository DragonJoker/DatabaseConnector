/************************************************************************//**
 * @file ExceptionDatabase.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/18/2014 2:47:39 PM
 * 
 *
 * @brief CExceptionDatabase class declaration and definition.
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
        
        EDatabaseExceptionCodes_LastCode ///< Represent the maximum number of exception code. Must be always the last.
    };
    
    /** Should be thrown when a problem occured in the database interface.
    */
    class DatabaseExport CExceptionDatabase
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
        CExceptionDatabase( int number, const String & description, const std::string & source, const std::string & file, long line );
        
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
        CExceptionDatabase( int number, const String & description, const std::string & source, const String & type, const std::string & file, long line );
        
        /** Get the error code.
        @return
            Return the error code.
        */
        virtual int GetNumber() const throw() { return _number; }
        
        /** Get the source function.
        @return
            Return the error source function.
        */
        virtual const String & GetSource() const { return _source; }
        
        /** Get the source file name.
        @return
            Return error source file name.
        */
        virtual const String & GetFile() const { return _file; }
        
        /** Get the line number.
        @return
            Return error line number.
        */
        virtual long GetLine() const { return _line; }
        
        /** Return a string with only the 'description' field of this exception.
            @remarks
                Use GetFullDescriptionton to get a full description of the error including
                line number, error number and what function threw the exception.
        */
        virtual const String & GetDescription() const { return _description; }
        
        /** Return a string with the full description of this error.
            @remarks
                The description contains the error number, the description
                supplied by the thrower, what routine threw the exception,
                and will also supply extra platform-specific information if applicable.
        */
        virtual const String & GetFullDescription() const;
        
        /** Override std::exception::what */
        const char * what() const throw();
        
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
        //!< Full ARIA string error description.
        mutable String _fullDesc;
        //!< Full std::string error description.
        mutable std::string _what;
    };
    
#   define DB_EXCEPT( number, description ) throw CExceptionDatabase( number, description, __FUNCTION__, __FILE__, __LINE__ )
    
} END_NAMESPACE_DATABASE

#endif // ___DATABASE_EXCEPTION_DATABASE_H___
