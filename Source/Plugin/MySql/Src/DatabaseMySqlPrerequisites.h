/************************************************************************//**
 * @file DatabaseMySqlPrerequisites.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 03/14/2014 11:48:00 AM
 *
 *
 * @brief Database MYSQL prerequisite header.
 *
 * @details This file contains all Database MYSQL prerequisite instructions.
 *
 ***************************************************************************/

#ifndef ___DATABASE_MYSQL_PREREQUISITES_H___
#define ___DATABASE_MYSQL_PREREQUISITES_H___

#include <DatabasePrerequisites.h>

#if defined( _WIN32 ) && !defined ( __MINGW32__ ) && !defined ( STATIC_LIB )
#    ifdef DatabasePluginMySql_EXPORTS
#        define DatabaseMySqlExport __declspec ( dllexport )
#    else
#       if defined ( __MINGW32__ )
#           define DatabaseMySqlExport
#       else
#           define DatabaseMySqlExport __declspec ( dllimport )
#       endif
#   endif
#else
#    define DatabaseMySqlExport
#endif

#if defined( STATIC_LIB )
#   define CPPCONN_PUBLIC_FUNC
#endif

#define BEGIN_NAMESPACE_DATABASE_MYSQL     BEGIN_NAMESPACE_DATABASE { namespace MySql
#define NAMESPACE_DATABASE_MYSQL           NAMESPACE_DATABASE::MySql
#define END_NAMESPACE_DATABASE_MYSQL       END_NAMESPACE_DATABASE }

BEGIN_NAMESPACE_DATABASE
{
	/** MYSQL namespace
	*/
	namespace MySql
	{
		// Pre-declare classes
		// Allows use of pointers in header files without including individual .h
		// so decreases dependencies between files
		class CDatabaseConnectionMySql;
		class CDatabaseMySql;
		class CDatabaseStatementMySql;
		class CDatabaseStatementParameterMySql;
		class CExceptionDatabaseMySql;
		class CPluginDatabaseMySql;
		class CFactoryDatabaseMySql;

		// Pointers
		typedef std::shared_ptr< CDatabaseConnectionMySql > DatabaseConnectionMySqlPtr;
		typedef std::shared_ptr< CDatabaseStatementMySql > DatabaseStatementMySqlPtr;
		typedef std::shared_ptr< CDatabaseStatementParameterMySql > DatabaseStatementParameterMySqlPtr;

		typedef std::vector< DatabaseStatementParameterMySqlPtr > DatabaseStatementParameterMySqlPtrArray;

		// Factory constants
		const String FACTORY_DATABASE_MYSQL = STR( "Factory Database MySql" );

		// Plugin constants
		const String DATABASE_MYSQL_TYPE = STR( "Database.MySql" );
		const String PLUGIN_NAME_DATABASE_MYSQL = STR( "Plugin Database MySql" );

		struct CMySqlBindBase
		{
			my_bool _null = 0;
			my_bool _error = 0;
			MYSQL_BIND & _bind;

			CMySqlBindBase( MYSQL_BIND & bind )
				: _bind( bind )
			{
				bind.error = &_error;
				bind.is_null = &_null;
			}
		};

		struct CInMySqlBindBase
				: public CMySqlBindBase
		{
			unsigned long _length = 0;

			CInMySqlBindBase( MYSQL_BIND & bind )
				: CMySqlBindBase( bind )
			{
				bind.length = &_length;
			}
		};

		template< typename T, typename U = T >
		struct CInMySqlBind
				: public CInMySqlBindBase
		{
			T _value;

			CInMySqlBind( MYSQL_BIND & bind )
				: CInMySqlBindBase( bind )
			{
				bind.buffer = &_value;
			}

			T const & GetValue()const
			{
				return _value;
			}
		};

		template<>
		struct CInMySqlBind< bool, bool >
				: public CInMySqlBindBase
		{
			int8_t _value;

			CInMySqlBind( MYSQL_BIND & bind )
				: CInMySqlBindBase( bind )
			{
				bind.buffer = &_value;
			}

			bool GetValue()const
			{
				return _value != 0;
			}
		};

		template< typename T >
		struct CInMySqlBind< T *, T * >
				: public CInMySqlBindBase
		{
			T _value[8192];

			CInMySqlBind( MYSQL_BIND & bind )
				: CInMySqlBindBase( bind )
			{
				memset( _value, 0, sizeof( _value ) );
				bind.buffer = _value;
				bind.buffer_length = sizeof( _value ) / sizeof( *_value );
			}

			T const * GetValue()const
			{
				return _value;
			}
		};

		template<>
		struct CInMySqlBind< char *, CFixedPoint >
				: public CInMySqlBindBase
		{
			char _value[8192];

			CInMySqlBind( MYSQL_BIND & bind )
				: CInMySqlBindBase( bind )
			{
				memset( _value, 0, sizeof( _value ) );
				bind.buffer = _value;
				bind.buffer_length = sizeof( _value ) / sizeof( *_value );
			}

			CFixedPoint GetValue()const
			{
				return CFixedPoint( _value );
			}
		};

		template<>
		struct CInMySqlBind< char *, int32_t >
				: public CInMySqlBindBase
		{
			char _value[8192];

			CInMySqlBind( MYSQL_BIND & bind )
				: CInMySqlBindBase( bind )
			{
				memset( _value, 0, sizeof( _value ) );
				bind.buffer = _value;
				bind.buffer_length = sizeof( _value ) / sizeof( _value );
			}

			int32_t GetValue()const
			{
				return CStrUtils::ToInt( _value );
			}
		};

		struct COutMySqlBindBase
				: public CMySqlBindBase
		{
			COutMySqlBindBase( MYSQL_BIND & bind, enum_field_types type, CDatabaseStatementParameterMySql & parameter );

			virtual void UpdateValue() = 0;

			MYSQL * _connection;
			MYSQL_STMT * _statement;
		};

		CDate CDateFromMySqlTime( MYSQL_TIME const & ts );
		CDateTime CDateTimeFromMySqlTime( MYSQL_TIME const & ts );
		CTime CTimeFromMySqlTime( MYSQL_TIME const & ts );

		MYSQL_TIME MySqlTimeFromCDate( CDate const & ts );
		MYSQL_TIME MySqlTimeFromCDateTime( CDateTime const & ts );
		MYSQL_TIME MySqlTimeFromCTime( CTime const & ts );

		std::string StringFromMySqlString( MYSQL_BIND const & bind, bool truncated );
		void MySqlSendLongData( CDatabaseValueBase & value, MYSQL_BIND const & bind, MYSQL_STMT * statement, MYSQL * connection );
		void MySQLTry( int result, TChar const * msg, EDatabaseExceptionCodes code, MYSQL * connection );
	}
}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_MYSQL_PREREQUISITES_H___
