/************************************************************************//**
 * @file DatabaseTestBase.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 12/02/2014 14:29:35
 *
 *
 * @brief Base class for database tests
 *
 ***************************************************************************/

#include "DatabaseTestPch.h"

#include "DatabaseTestBase.h"
#include "DatabaseTestUtilsUtf8.h"

BEGIN_NAMESPACE_DATABASE_TEST
{
	template< typename CleanFunc >
	struct SBlockGuard
	{
		template< typename InitFunc >
		SBlockGuard( InitFunc init, CleanFunc clean )
			: _clean( clean )
		{
			init();
		};

		~SBlockGuard()
		{
			_clean();
		}

		CleanFunc _clean;
	};

	template< typename InitFunc, typename CleanFunc >
	SBlockGuard< CleanFunc > make_block_guard( InitFunc init, CleanFunc clean )
	{
		return SBlockGuard< CleanFunc >( init, clean );
	}

	template< EFieldType FieldType, uint8_t Precision=0 >
	struct BatchTests
	{
		template< typename Function >
		void operator()( Function function, DatabaseConnectionPtr connection, String const & name, typename DatabaseUtils::Helpers< FieldType >::ParamType valueIn = DatabaseUtils::Helpers< FieldType >::InitialiseValue() )
		{
			CLogger::LogInfo( StringStream() << "  Lowest" );
			function( connection, name, std::numeric_limits< DatabaseUtils::Helpers< FieldType >::ParamType >::lowest() );
			CLogger::LogInfo( StringStream() << "  Max" );
			function( connection, name, std::numeric_limits< DatabaseUtils::Helpers< FieldType >::ParamType >::max() );
			CLogger::LogInfo( StringStream() << "  Min" );
			function( connection, name, std::numeric_limits< DatabaseUtils::Helpers< FieldType >::ParamType >::min() );
			CLogger::LogInfo( StringStream() << "  Default" );
			function( connection, name, DatabaseUtils::Helpers< FieldType >::ParamType() );
			CLogger::LogInfo( StringStream() << "  Given" );
			function( connection, name, valueIn );
		}
	};

	template<>
	struct BatchTests< EFieldType_FLOAT64 >
	{
		template< typename Function >
		void operator()( Function function, DatabaseConnectionPtr connection, String const & name, typename DatabaseUtils::Helpers< EFieldType_FLOAT64 >::ParamType valueIn = DatabaseUtils::Helpers< EFieldType_FLOAT64 >::InitialiseValue() )
		{
			CLogger::LogInfo( StringStream() << "  Lowest / 2" );
			function( connection, name, std::numeric_limits< double >::lowest() / 2 );
			CLogger::LogInfo( StringStream() << "  Max / 2" );
			function( connection, name, std::numeric_limits< double >::max() / 2 );
			CLogger::LogInfo( StringStream() << "  Min / 2" );
			function( connection, name, std::numeric_limits< double >::min() );
			CLogger::LogInfo( StringStream() << "  Default" );
			function( connection, name, double() );
			CLogger::LogInfo( StringStream() << "  Given" );
			function( connection, name, valueIn );
		}
	};

	template<>
	struct BatchTests< EFieldType_DATE >
	{
		template< typename Function >
		void operator()( Function function, DatabaseConnectionPtr connection, String const & name, typename DatabaseUtils::Helpers< EFieldType_DATE >::ParamType valueIn = DatabaseUtils::Helpers< EFieldType_DATE >::InitialiseValue() )
		{
			CLogger::LogInfo( StringStream() << "  Given" );
			function( connection, name, valueIn );
		}
	};

	template<>
	struct BatchTests< EFieldType_DATETIME >
	{
		template< typename Function >
		void operator()( Function function, DatabaseConnectionPtr connection, String const & name, typename DatabaseUtils::Helpers< EFieldType_DATETIME >::ParamType valueIn = DatabaseUtils::Helpers< EFieldType_DATETIME >::InitialiseValue() )
		{
			CLogger::LogInfo( StringStream() << "  Given" );
			function( connection, name, valueIn );
		}
	};

	template<>
	struct BatchTests< EFieldType_TIME >
	{
		template< typename Function >
		void operator()( Function function, DatabaseConnectionPtr connection, String const & name, typename DatabaseUtils::Helpers< EFieldType_TIME >::ParamType valueIn = DatabaseUtils::Helpers< EFieldType_TIME >::InitialiseValue() )
		{
			CLogger::LogInfo( StringStream() << "  Given" );
			function( connection, name, valueIn );
		}
	};

	template< uint8_t Precision >
	struct BatchTests< EFieldType_FIXED_POINT, Precision >
	{
		template< typename Function >
		void operator()( Function function, DatabaseConnectionPtr connection, String const & name, typename DatabaseUtils::Helpers< EFieldType_FIXED_POINT >::ParamType valueIn = DatabaseUtils::Helpers< EFieldType_FIXED_POINT >::InitialiseValue( Precision ) )
		{
			CLogger::LogInfo( StringStream() << "  Given" );
			function( connection, name, valueIn );
			CLogger::LogInfo( StringStream() << "  Specific value, precision test" );
			function( connection, name, CFixedPoint( 602872352, Precision ) );
		}
	};

	template<>
	struct BatchTests< EFieldType_VARCHAR >
	{
		template< typename Function >
		void operator()( Function function, DatabaseConnectionPtr connection, String const & name, typename DatabaseUtils::Helpers< EFieldType_VARCHAR >::ParamType valueIn = DatabaseUtils::Helpers< EFieldType_VARCHAR >::InitialiseValue() )
		{
			CLogger::LogInfo( StringStream() << "  Given" );
			function( connection, name, valueIn );
		}
	};

	template<>
	struct BatchTests< EFieldType_NVARCHAR >
	{
		template< typename Function >
		void operator()( Function function, DatabaseConnectionPtr connection, String const & name, typename DatabaseUtils::Helpers< EFieldType_NVARCHAR >::ParamType valueIn = DatabaseUtils::Helpers< EFieldType_NVARCHAR >::InitialiseValue() )
		{
			CLogger::LogInfo( StringStream() << "  Given" );
			function( connection, name, valueIn );
		}
	};

	template<>
	struct BatchTests< EFieldType_TEXT >
	{
		template< typename Function >
		void operator()( Function function, DatabaseConnectionPtr connection, String const & name, typename DatabaseUtils::Helpers< EFieldType_TEXT >::ParamType valueIn = DatabaseUtils::Helpers< EFieldType_TEXT >::InitialiseValue() )
		{
			CLogger::LogInfo( StringStream() << "  Given" );
			function( connection, name, valueIn );
		}
	};

	template<>
	struct BatchTests< EFieldType_VARBINARY >
	{
		template< typename Function >
		void operator()( Function function, DatabaseConnectionPtr connection, String const & name, typename DatabaseUtils::Helpers< EFieldType_VARBINARY >::ParamType valueIn = DatabaseUtils::Helpers< EFieldType_VARBINARY >::InitialiseValue() )
		{
			CLogger::LogInfo( StringStream() << "  Given" );
			function( connection, name, valueIn );
		}
	};

	template< typename StmtType >
	void CDatabaseTest::TestCase_DatabaseFieldsInsertRetrieve()
	{
		auto const guard = make_block_guard( [this]()
		{
			DoLoadPlugins();
		}, []()
		{
			UnloadPlugins();
		} );
		std::unique_ptr< CDatabase > database( InstantiateDatabase( _type ) );

		if ( database )
		{
			DatabaseConnectionPtr connection = CreateConnection( *database, _server, _user, _password );

			if ( connection )
			{
				if ( connection->IsConnected() )
				{
					connection->SelectDatabase( _database );
					DoFlushTable( connection );
					//BatchTests< EFieldType_INT32 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_INT32 >, connection, STR( "IntegerField" ) );
					BatchTests< EFieldType_INT8 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_INT8 >, connection, STR( "TinyIntField" ) );
					BatchTests< EFieldType_INT16 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_INT16 >, connection, STR( "SmallIntField" ) );
					BatchTests< EFieldType_INT24 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_INT24 >, connection, STR( "MediumIntField" ) );
					BatchTests< EFieldType_INT64 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_INT64 >, connection, STR( "BigIntField" ) );
					BatchTests< EFieldType_INT16 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_INT16 >, connection, STR( "Int2Field" ) );
					BatchTests< EFieldType_INT64 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_INT64 >, connection, STR( "Int8Field" ) );
					BatchTests< EFieldType_FLOAT64 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_FLOAT64 >, connection, STR( "RealField" ) );
					BatchTests< EFieldType_FLOAT64 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_FLOAT64 >, connection, STR( "DoubleField" ) );
					BatchTests< EFieldType_FLOAT64 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_FLOAT64 >, connection, STR( "DoublePrecisionField" ) );
					BatchTests< EFieldType_FLOAT32 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_FLOAT32 >, connection, STR( "FloatField" ) );
					BatchTests< EFieldType_FIXED_POINT, 0 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_FIXED_POINT >, connection, STR( "NumericField" ) );
					BatchTests< EFieldType_FIXED_POINT, 5 >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_FIXED_POINT >, connection, STR( "DecimalField" ) );
					BatchTests< EFieldType_BIT >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_BIT >, connection, STR( "BooleanField" ) );
					BatchTests< EFieldType_DATE >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_DATE >, connection, STR( "DateField" ) );
					BatchTests< EFieldType_DATETIME >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_DATETIME >, connection, STR( "DateTimeField" ) );
					BatchTests< EFieldType_VARCHAR >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_VARCHAR >, connection, STR( "CharacterField" ) );
					BatchTests< EFieldType_VARCHAR >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_VARCHAR >, connection, STR( "VarcharField" ) );
					BatchTests< EFieldType_NVARCHAR >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_NVARCHAR >, connection, STR( "NcharField" ) );
					BatchTests< EFieldType_NVARCHAR >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_NVARCHAR >, connection, STR( "NVarcharField" ) );
					BatchTests< EFieldType_TEXT >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_TEXT >, connection, STR( "TextField" ) );
					BatchTests< EFieldType_VARBINARY >()( &DatabaseUtils::InsertAndRetrieve< StmtType, EFieldType_VARBINARY >, connection, STR( "BlobField" ) );
				}

				database->RemoveConnection();
			}
		}
	}

	template< typename StmtType >
	void CDatabaseTest::TestCase_DatabaseFieldsInsertRetrieveOtherIndex()
	{
		auto const guard = make_block_guard( [this]()
		{
			DoLoadPlugins();
		}, []()
		{
			UnloadPlugins();
		} );
		std::unique_ptr< CDatabase > database( InstantiateDatabase( _type ) );

		if ( database )
		{
			DatabaseConnectionPtr connection = CreateConnection( *database, _server, _user, _password );

			if ( connection )
			{
				if ( connection->IsConnected() )
				{
					connection->SelectDatabase( _database );
					DoFlushTable( connection );
					//BatchTests< EFieldType_INT32 >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_INT32 >, connection, STR( "IntegerField" ) );
					BatchTests< EFieldType_INT8 >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_INT8 >, connection, STR( "TinyIntField" ) );
					BatchTests< EFieldType_INT16 >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_INT16 >, connection, STR( "SmallIntField" ) );
					BatchTests< EFieldType_INT24 >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_INT24 >, connection, STR( "MediumIntField" ) );
					BatchTests< EFieldType_INT64 >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_INT64 >, connection, STR( "BigIntField" ) );
					BatchTests< EFieldType_INT16 >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_INT16 >, connection, STR( "Int2Field" ) );
					BatchTests< EFieldType_INT64 >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_INT64 >, connection, STR( "Int8Field" ) );
					BatchTests< EFieldType_FLOAT64 >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_FLOAT64 >, connection, STR( "RealField" ) );
					BatchTests< EFieldType_FLOAT64 >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_FLOAT64 >, connection, STR( "DoubleField" ) );
					BatchTests< EFieldType_FLOAT64 >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_FLOAT64 >, connection, STR( "DoublePrecisionField" ) );
					BatchTests< EFieldType_FLOAT32 >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_FLOAT32 >, connection, STR( "FloatField" ) );
					BatchTests< EFieldType_FIXED_POINT, 0 >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_FIXED_POINT >, connection, STR( "NumericField" ) );
					BatchTests< EFieldType_FIXED_POINT, 5 >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_FIXED_POINT >, connection, STR( "DecimalField" ) );
					BatchTests< EFieldType_BIT >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_BIT >, connection, STR( "BooleanField" ) );
					BatchTests< EFieldType_DATE >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_DATE >, connection, STR( "DateField" ) );
					BatchTests< EFieldType_DATETIME >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_DATETIME >, connection, STR( "DateTimeField" ) );
					BatchTests< EFieldType_VARCHAR >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_VARCHAR >, connection, STR( "CharacterField" ) );
					BatchTests< EFieldType_VARCHAR >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_VARCHAR >, connection, STR( "VarcharField" ) );
					BatchTests< EFieldType_NVARCHAR >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_NVARCHAR >, connection, STR( "NcharField" ) );
					BatchTests< EFieldType_NVARCHAR >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_NVARCHAR >, connection, STR( "NVarcharField" ) );
					BatchTests< EFieldType_TEXT >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_TEXT >, connection, STR( "TextField" ) );
					BatchTests< EFieldType_VARBINARY >()( &DatabaseUtils::InsertAndRetrieveOtherIndex< StmtType, EFieldType_VARBINARY >, connection, STR( "BlobField" ) );
				}

				database->RemoveConnection();
			}
		}
	}

	template< typename StmtType >
	void CDatabaseTest::TestCase_DatabaseFieldsInsertRetrieveFast()
	{
		auto const guard = make_block_guard( [this]()
		{
			DoLoadPlugins();
		}, []()
		{
			UnloadPlugins();
		} );
		std::unique_ptr< CDatabase > database( InstantiateDatabase( _type ) );

		if ( database )
		{
			DatabaseConnectionPtr connection = CreateConnection( *database, _server, _user, _password );

			if ( connection )
			{
				if ( connection->IsConnected() )
				{
					connection->SelectDatabase( _database );
					DoFlushTable( connection );
					//BatchTests< EFieldType_INT32 >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_INT32 >, connection, STR( "IntegerField" ) );
					BatchTests< EFieldType_INT8 >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_INT8 >, connection, STR( "TinyIntField" ) );
					BatchTests< EFieldType_INT16 >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_INT16 >, connection, STR( "SmallIntField" ) );
					BatchTests< EFieldType_INT24 >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_INT24 >, connection, STR( "MediumIntField" ) );
					BatchTests< EFieldType_INT64 >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_INT64 >, connection, STR( "BigIntField" ) );
					BatchTests< EFieldType_INT16 >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_INT16 >, connection, STR( "Int2Field" ) );
					BatchTests< EFieldType_INT64 >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_INT64 >, connection, STR( "Int8Field" ) );
					BatchTests< EFieldType_FLOAT64 >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_FLOAT64 >, connection, STR( "RealField" ) );
					BatchTests< EFieldType_FLOAT64 >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_FLOAT64 >, connection, STR( "DoubleField" ) );
					BatchTests< EFieldType_FLOAT64 >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_FLOAT64 >, connection, STR( "DoublePrecisionField" ) );
					BatchTests< EFieldType_FLOAT32 >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_FLOAT32 >, connection, STR( "FloatField" ) );
					BatchTests< EFieldType_FIXED_POINT, 0 >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_FIXED_POINT >, connection, STR( "NumericField" ) );
					BatchTests< EFieldType_FIXED_POINT, 5 >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_FIXED_POINT >, connection, STR( "DecimalField" ) );
					BatchTests< EFieldType_BIT >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_BIT >, connection, STR( "BooleanField" ) );
					BatchTests< EFieldType_DATE >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_DATE >, connection, STR( "DateField" ) );
					BatchTests< EFieldType_DATETIME >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_DATETIME >, connection, STR( "DateTimeField" ) );
					BatchTests< EFieldType_VARCHAR >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_VARCHAR >, connection, STR( "CharacterField" ) );
					BatchTests< EFieldType_VARCHAR >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_VARCHAR >, connection, STR( "VarcharField" ) );
					BatchTests< EFieldType_TEXT >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_TEXT >, connection, STR( "TextField" ) );
					BatchTests< EFieldType_VARBINARY >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_VARBINARY >, connection, STR( "BlobField" ) );

					if ( _hasNChar )
					{
						BatchTests< EFieldType_NVARCHAR >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_NVARCHAR >, connection, STR( "NcharField" ) );
						BatchTests< EFieldType_NVARCHAR >()( &DatabaseUtils::InsertAndRetrieveFast< StmtType, EFieldType_NVARCHAR >, connection, STR( "NVarcharField" ) );
					}
				}

				database->RemoveConnection();
			}
		}
	}

	template< typename StmtType >
	void CDatabaseTest::TestCase_DatabaseFieldsInsertRetrieveFastOtherIndex()
	{
		auto const guard = make_block_guard( [this]()
		{
			DoLoadPlugins();
		}, []()
		{
			UnloadPlugins();
		} );
		std::unique_ptr< CDatabase > database( InstantiateDatabase( _type ) );

		if ( database )
		{
			DatabaseConnectionPtr connection = CreateConnection( *database, _server, _user, _password );

			if ( connection )
			{
				if ( connection->IsConnected() )
				{
					connection->SelectDatabase( _database );
					DoFlushTable( connection );
					//BatchTests< EFieldType_INT32 >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_INT32 >, connection, STR( "IntegerField" ) );
					BatchTests< EFieldType_INT8 >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_INT8 >, connection, STR( "TinyIntField" ) );
					BatchTests< EFieldType_INT16 >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_INT16 >, connection, STR( "SmallIntField" ) );
					BatchTests< EFieldType_INT24 >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_INT24 >, connection, STR( "MediumIntField" ) );
					BatchTests< EFieldType_INT64 >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_INT64 >, connection, STR( "BigIntField" ) );
					BatchTests< EFieldType_INT16 >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_INT16 >, connection, STR( "Int2Field" ) );
					BatchTests< EFieldType_INT64 >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_INT64 >, connection, STR( "Int8Field" ) );
					BatchTests< EFieldType_FLOAT64 >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_FLOAT64 >, connection, STR( "RealField" ) );
					BatchTests< EFieldType_FLOAT64 >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_FLOAT64 >, connection, STR( "DoubleField" ) );
					BatchTests< EFieldType_FLOAT64 >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_FLOAT64 >, connection, STR( "DoublePrecisionField" ) );
					BatchTests< EFieldType_FLOAT32 >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_FLOAT32 >, connection, STR( "FloatField" ) );
					BatchTests< EFieldType_FIXED_POINT, 0 >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_FIXED_POINT >, connection, STR( "NumericField" ) );
					BatchTests< EFieldType_FIXED_POINT, 5 >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_FIXED_POINT >, connection, STR( "DecimalField" ) );
					BatchTests< EFieldType_BIT >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_BIT >, connection, STR( "BooleanField" ) );
					BatchTests< EFieldType_DATE >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_DATE >, connection, STR( "DateField" ) );
					BatchTests< EFieldType_DATETIME >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_DATETIME >, connection, STR( "DateTimeField" ) );
					BatchTests< EFieldType_VARCHAR >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_VARCHAR >, connection, STR( "CharacterField" ) );
					BatchTests< EFieldType_VARCHAR >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_VARCHAR >, connection, STR( "VarcharField" ) );
					BatchTests< EFieldType_TEXT >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_TEXT >, connection, STR( "TextField" ) );
					BatchTests< EFieldType_VARBINARY >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_VARBINARY >, connection, STR( "BlobField" ) );

					if ( _hasNChar )
					{
						BatchTests< EFieldType_NVARCHAR >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_NVARCHAR >, connection, STR( "NcharField" ) );
						BatchTests< EFieldType_NVARCHAR >()( &DatabaseUtils::InsertAndRetrieveFastOtherIndex< StmtType, EFieldType_NVARCHAR >, connection, STR( "NVarcharField" ) );
					}
				}

				database->RemoveConnection();
			}
		}
	}

	template< typename StmtType >
	void CDatabaseTest::TestCase_DatabaseDirectQuery()
	{
		auto const guard = make_block_guard( [this]()
		{
			DoLoadPlugins();
		}, []()
		{
			UnloadPlugins();
		} );
		std::unique_ptr< CDatabase > database( InstantiateDatabase( _type ) );

		if ( database )
		{
			DatabaseConnectionPtr connection = CreateConnection( *database, _server, _user, _password );

			if ( connection )
			{
				if ( connection->IsConnected() )
				{
					connection->SelectDatabase( _database );
					DoFlushTable( connection );
					CLogger::LogInfo( StringStream() << " Insertions" );
					DatabaseUtils::TestDirectInsert< StmtType >( connection );
					CLogger::LogInfo( StringStream() << " Selection" );
					DatabaseUtils::TestDirectSelect< StmtType >( connection );
					CLogger::LogInfo( StringStream() << " Update" );
					DatabaseUtils::TestDirectUpdate< StmtType >( connection );
					CLogger::LogInfo( StringStream() << " Deletion" );
					DatabaseUtils::TestDirectDelete< StmtType >( connection );
				}

				database->RemoveConnection();
			}

			database->RemoveConnection();
		}
	}

	template< typename StmtType >
	void CDatabaseTest::TestCase_DatabaseStoredProcedure()
	{
		auto const guard = make_block_guard( [this]()
		{
			DoLoadPlugins();
		}, []()
		{
			UnloadPlugins();
		} );
		std::unique_ptr< CDatabase > database( InstantiateDatabase( _type ) );

		if ( database )
		{
			DatabaseConnectionPtr connection = CreateConnection( *database, _server, _user, _password );

			if ( connection )
			{
				if ( connection->IsConnected() )
				{
					connection->SelectDatabase( _database );
					CLogger::LogInfo( StringStream() << " No parameter, No Return" );
					DatabaseUtils::TestStoredNoParamNoReturn< StmtType >( connection );
					CLogger::LogInfo( StringStream() << " No parameter, Return" );
					DatabaseUtils::TestStoredNoParamReturn< StmtType >( connection, STR( "" ) );
					CLogger::LogInfo( StringStream() << " In parameters, No Return" );
					DatabaseUtils::TestStoredInParamNoReturn< StmtType >( connection );
					CLogger::LogInfo( StringStream() << " In parameters, Return" );
					DatabaseUtils::TestStoredNoParamReturn< StmtType >( connection, STR( "WHERE ACTOR_ID > 5" ) );
					CLogger::LogInfo( StringStream() << " In/Out INTEGER parameter, No Return" );
					DatabaseUtils::TestStoredInOutParamNoReturn< StmtType >( connection );
					CLogger::LogInfo( StringStream() << " In/Out INTEGER and DATETIME parameters, No Return" );
					DatabaseUtils::TestStoredInOutDtParamNoReturn< StmtType >( connection );
				}

				database->RemoveConnection();
			}
		}
	}

#if defined( PERF_TEST )
	template< typename StmtType >
	void CDatabaseTest::TestCase_DatabasePerformances()
	{
		auto const guard = make_block_guard( [this]()
		{
			DoLoadPlugins();
		}, []()
		{
			UnloadPlugins();
		} );
		std::unique_ptr< CDatabase > database( InstantiateDatabase( _type ) );

		if ( database )
		{
			DatabaseConnectionPtr connection = CreateConnection( *database, _server, _user, _password );

			if ( connection )
			{
				if ( connection->IsConnected() )
				{
#if defined( NDEBUG )
					uint32_t tests = 100000;
#else
					uint32_t tests = 10;
#endif
					connection->SelectDatabase( _database );
					DoFlushTable( connection );
					CLogger::LogInfo( StringStream() << " Insert elements" );
					PerfDirectInsertActors< StmtType >( connection, tests, String() );
					CLogger::LogInfo( StringStream() << " Select all elements" );
					PerfDirectSelectActors< StmtType >( connection, tests, String() );
					CLogger::LogInfo( StringStream() << " Delete all elements" );
					PerfDirectDeleteActors< StmtType >( connection, tests );
				}

				database->RemoveConnection();
			}
		}
	}
#endif
}
END_NAMESPACE_DATABASE_TEST
