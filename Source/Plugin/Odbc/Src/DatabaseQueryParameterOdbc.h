/************************************************************************//**
* @file DatabaseQueryParameterOdbc.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDatabaseStatementParameterOdbc class declaration.
*
* @details Describes a statement parameter for ODBC database.
*
***************************************************************************/

#ifndef ___DATABASE_QUERY_PARAMETER_ODBC_H___
#define ___DATABASE_QUERY_PARAMETER_ODBC_H___

#include "DatabaseOdbcPrerequisites.h"

#include <DatabaseParameter.h>
#include "DatabaseParameterOdbc.h"

BEGIN_NAMESPACE_DATABASE_ODBC
{
	/** Describes a statement parameter for ODBC database.
	*/
	class CDatabaseQueryParameterOdbc
		: public CDatabaseParameter
		, public CDatabaseParameterOdbc
	{
	public:
		/** Constructor.
		@param[in] connection
			Connection to database.
		@param[in] name
			Parameter name.
		@param[in] index
			Internal index.
		@param[in] fieldType
			Field type.
		@param[in] parameterType
			Parameter type.
		@param[in] updater
			The parent updater
		*/
		CDatabaseQueryParameterOdbc( DatabaseConnectionOdbcPtr connection, const String & name, unsigned short index, EFieldType fieldType, EParameterType parameterType, std::unique_ptr< SValueUpdater > updater );

		/** Constructor.
		@param[in] connection
			Connection to database.
		@param[in] name
			Parameter name.
		@param[in] index
			Internal index.
		@param[in] fieldType
			Field type.
		@param[in] limits
			Field limits.
		@param[in] parameterType
			Parameter type.
		@param[in] updater
			The parent updater
		*/
		CDatabaseQueryParameterOdbc( DatabaseConnectionOdbcPtr connection, const String & name, unsigned short index, EFieldType fieldType, uint32_t limits, EParameterType parameterType, std::unique_ptr< SValueUpdater > updater );

		/** Destructor.
		*/
		virtual ~CDatabaseQueryParameterOdbc();

		/** Initializes parameter members from the given statement handle
		@param statementHandle
			The statement handle
		*/
		void Initialize( SQLHSTMT statementHandle );
	};
}
END_NAMESPACE_DATABASE_ODBC

#endif // ___DATABASE_STATEMENT_PARAMETER_ODBC_H___
