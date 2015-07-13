/************************************************************************//**
 * @file Database/Interface/Src/Database.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 *
 *
 * @brief CDatabase class definition.
 *
 * @details Describes a database.
 *
 ***************************************************************************/

#include "DatabasePch.h"

#include "Database.h"

#include "DatabaseConnection.h"
#include "DatabaseDate.h"
#include "DatabaseDateTime.h"
#include "DatabaseTime.h"
#include "DatabaseStringUtils.h"

BEGIN_NAMESPACE_DATABASE
{
	const String LOGGER_NAME_CDATABASE = STR( "Database.CDatabase" );

	CDatabase::CDatabase()
	{
		// Empty
	}

	CDatabase::~CDatabase()
	{
		std::unique_lock< std::recursive_mutex > lock( _mutex );
		_mapConnections.clear();
	}

	void CDatabase::Initialize( const String & server, const String & userName, const String & password )
	{
		std::unique_lock< std::recursive_mutex > lock( _mutex );
		_server = server;
		_userName = userName;
		_password = password;
	}

	void CDatabase::CreateConnection( String & connectionString )
	{
		std::unique_lock< std::recursive_mutex > lock( _mutex );
		std::thread::id threadId = std::this_thread::get_id();
		auto && it = _mapConnections.find( threadId );

		if ( it == _mapConnections.end() )
		{
			_mapConnections.insert( std::make_pair( threadId, DoCreateConnection( connectionString ) ) );
		}
	}

	DatabaseConnectionPtr CDatabase::RetrieveConnection()
	{
		DatabaseConnectionPtr pReturn = DoGetConnection();

		if ( ! pReturn )
		{
			String strResult;
			CreateConnection( strResult );
			pReturn = DoGetConnection();
		}

		return pReturn;
	}

	void CDatabase::RemoveConnection()
	{
		std::unique_lock< std::recursive_mutex > lock( _mutex );
		std::thread::id threadId = std::this_thread::get_id();
		auto && it = _mapConnections.find( threadId );

		if ( it != _mapConnections.end() )
		{
			_mapConnections.erase( it );
		}
	}

	DatabaseConnectionPtr CDatabase::DoGetConnection() const
	{
		std::unique_lock< std::recursive_mutex > lock( _mutex );
		std::thread::id threadId = std::this_thread::get_id();
		auto && it = _mapConnections.find( threadId );
		DatabaseConnectionPtr pReturn;

		if ( it != _mapConnections.end() )
		{
			pReturn = it->second;
		}

		return pReturn;
	}
}
END_NAMESPACE_DATABASE
