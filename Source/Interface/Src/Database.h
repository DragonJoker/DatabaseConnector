/************************************************************************//**
* @file Database.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDatabase class declaration.
*
* @details Describes a database.
*
***************************************************************************/

#ifndef ___DATABASE_H___
#define ___DATABASE_H___

#include "DatabasePrerequisites.h"

#include "EErrorType.h"

BEGIN_NAMESPACE_DATABASE
{
	/** Describes a database.
	*/
	class CDatabase
	{
	public:
		/** Constructor.
		@param[in] type
			Database type.
		*/
		DatabaseExport CDatabase();

		/** Destructor.
		*/
		DatabaseExport virtual ~CDatabase();

		/** Initialize data.
		@param[in] server
			Server identifier (name or address).
		@param[in] userName
			User name.
		@param[in] password
			User password.
		*/
		DatabaseExport void Initialize( const String & server, const String & userName, const String & password );

		/** Create a connection.
		@param[out] connectionString
			Created connection string.
		*/
		DatabaseExport void CreateConnection( String & connectionString );

		/** Retrieve connection.
		@return
			Found connection.
		*/
		DatabaseExport DatabaseConnectionPtr RetrieveConnection();

		/** Remove connection.
		*/
		DatabaseExport void RemoveConnection();

	protected:
		/** Create a connection.
		@param[out] connectionString
			Created connection string.
		@return
			Database connection.
		*/
		DatabaseExport virtual DatabaseConnectionPtr DoCreateConnection( String & connectionString ) const = 0;

	private:
		/** Get connection.
		@return
			Database connection.
		*/
		DatabaseConnectionPtr DoGetConnection() const;

	protected:
		typedef std::map< unsigned long, DatabaseConnectionPtr > DatabaseConnectionPtrDwordMap;
		//! Mutex.
		mutable std::recursive_mutex _mutex;
		//! Map of connections (multiple threads).
		DatabaseConnectionPtrIdMap _mapConnections;
		//! Server identifier (name or address).
		String _server;
		//! User name.
		String _userName;
		//! User password.
		String _password;
	};
}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_H___
