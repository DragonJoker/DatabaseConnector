/************************************************************************//**
* @file DatabaseParameterSqlite.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDatabaseParameterSqlite class declaration.
*
* @details Describes the base class for a statement or query parameter for SQLite database.
*
***************************************************************************/

#ifndef ___DATABASE_PARAMETER_SQLITE_H___
#define ___DATABASE_PARAMETER_SQLITE_H___

#include "DatabaseSqlitePrerequisites.h"

#include <EFieldType.h>
#include <EParameterType.h>

BEGIN_NAMESPACE_DATABASE_SQLITE
{
	/** Describes a statement parameter for SQLite database.
	*/
	class CDatabaseParameterSqlite
	{
	public:
		//! The SQLite data types associated to Database::EFieldType
		static int SqliteDataTypes[EFieldType_COUNT];

	public:
		/** Constructor.
		@param[in] fieldType
			Field type.
		@param[in] parameterType
			Parameter type.
		*/
		CDatabaseParameterSqlite( EFieldType fieldType );

		/** Destructor.
		*/
		virtual ~CDatabaseParameterSqlite();

		/** Retrieve the data type
		@return
			The type
		*/
		const int & GetDataType()const;

	protected:
		//! The SQLite data type
		int _dataType;
	};
}
END_NAMESPACE_DATABASE_SQLITE

#endif // ___DATABASE_PARAMETER_SQLITE_H___
