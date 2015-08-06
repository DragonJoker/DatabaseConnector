/************************************************************************//**
* @file DatabaseOdbcPrerequisites.h
* @author Sylvain Doremus
* @version 1.0
* @date 03/14/2014 11:48:00 AM
*
*
* @brief Database ODBC prerequisite header.
*
* @details This file contains all Database ODBC prerequisite instructions.
*
***************************************************************************/

#ifndef ___DATABASE_ODBC_PREREQUISITES_H___
#define ___DATABASE_ODBC_PREREQUISITES_H___

#include <DatabasePrerequisites.h>

#if defined( _WIN32 )
#   include <windows.h>
#   include <sql.h>
#   include <sqlext.h>

#   undef min
#   undef max
#   undef abs
#else
#   include <sql.h>
#   include <sqlext.h>
#endif

#define BEGIN_NAMESPACE_DATABASE_ODBC BEGIN_NAMESPACE_DATABASE { namespace Odbc
#define NAMESPACE_DATABASE_ODBC NAMESPACE_DATABASE::Odbc
#define END_NAMESPACE_DATABASE_ODBC } END_NAMESPACE_DATABASE

BEGIN_NAMESPACE_DATABASE
{
	namespace Odbc
	{
		static const String ERROR_ODBC_DRIVER = STR( "ODBC Driver error" );

		// Pre-declare classes
		// Allows use of pointers in header files without including individual .h
		// so decreases dependencies between files
		class CDatabaseConnectionOdbc;
		class CDatabaseOdbc;
		class CDatabaseStatementOdbc;
		class CDatabaseQueryOdbc;
		class CDatabaseStatementParameterOdbc;
		class CDatabaseQueryParameterOdbc;
		class CDatabaseExceptionOdbc;
		class CPluginDatabaseOdbc;
		class CFactoryDatabaseOdbc;

		// Pointers
		DECLARE_SMART_PTR( DatabaseConnectionOdbc );
		DECLARE_SMART_PTR( DatabaseStatementOdbc );
		DECLARE_SMART_PTR( DatabaseQueryOdbc );
		DECLARE_SMART_PTR( DatabaseStatementParameterOdbc );
		DECLARE_SMART_PTR( DatabaseQueryParameterOdbc );

		// Plugin constants
		const String DATABASE_ODBC_TYPE = STR( "Database.Odbc" );
		const String PLUGIN_NAME_DATABASE_ODBC = STR( "Plugin Database Odbc" );

		// Unicode and SQL types
		typedef SQLCHAR SqlChar;
		static const int SQL_C_XCHAR = SQL_C_CHAR;
		static const int SQL_XCHAR = SQL_CHAR;
		static const int SQL_XVARCHAR = SQL_VARCHAR;
		static const int SQL_XLONGVARCHAR = SQL_LONGVARCHAR;

		// SQL execution
#define OdbcCheck( func, handle_type, handle, stream )\
		{\
			int attemptCount = 0;\
			errorType = SqlSuccess( func, handle_type, handle, StringStream() << stream );\
			while( errorType == EErrorType_RETRY && attemptCount < 10 )\
			{\
				errorType = SqlSuccess( func, handle_type, handle, StringStream() << stream );\
				attemptCount++;\
			}\
			if( attemptCount == 10 )\
			{\
				errorType = EErrorType_RECONNECT;\
			}\
		}

	}
}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_ODBC_PREREQUISITES_H___
