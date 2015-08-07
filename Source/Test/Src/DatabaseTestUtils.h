/************************************************************************//**
* @file DatabaseTestUtils.h
* @author Sylvain Doremus
* @version 1.0
* @date 06/18/2014 12:04:36
*
*
* @brief Utils for Database tests.
*
* @details Utils for Database tests.
*
***************************************************************************/

#ifndef ___DATABASE_TEST_UTILS_H___
#define ___DATABASE_TEST_UTILS_H___

#include "DatabaseTestPrerequisites.h"

#include "DatabaseTestHelpers.h"

#include <DatabaseConnection.h>
#include <DatabaseStatement.h>
#include <DatabaseQuery.h>
#include <DatabaseResult.h>
#include <DatabaseRow.h>
#include <DatabaseInt24.h>
#include <random>

BEGIN_NAMESPACE_DATABASE_TEST
{
	/** Creates a MySql database.
	@param database
		Database name.
	@param user
		User name.
	@param pass
		User password.
	@return
		Exit code of the CProcess executing the script file.
	*/
	int InstallDatabaseMySql( const String & database, const String & user, const String & pass );

	/** Deletes a MySql database.
	@param database
		Database name.
	@param user
		User name.
	@param pass
		User password.
	@return
		Exit code of the CProcess executing the script file.
	*/
	int UninstallDatabaseMySql( const String & database, const String & user, const String & pass );

	/** Creates an ODBC source for a MySql database.
	@param dsn
		ODBC source name.
	@return
		Exit code of the CProcess executing the script file.
	*/
	int InstallSourceOdbcMySql( const String & database );

	/** Deletes an ODBC source for a MySql database.
	@param dsn
		ODBC source name.
	@return
		Exit code of the CProcess executing the script file.
	*/
	int UninstallSourceOdbcMySql( const String & database );

	/** Creates a MsSql database.
	@param database
		Database name.
	@param user
		User name.
	@param pass
		User password.
	@return
		Exit code of the CProcess executing the script file.
	*/
	int InstallDatabaseMsSql( const String & database, const String & user, const String & pass );

	/** Deletes a MsSql database.
	@param database
		Database name.
	@param user
		User name.
	@param pass
		User password.
	@return
		Exit code of the CProcess executing the script file.
	*/
	int UninstallDatabaseMsSql( const String & database, const String & user, const String & pass );

	/** Creates an ODBC source for a MsSql database.
	@param dsn
		ODBC source name.
	@return
		Exit code of the CProcess executing the script file.
	*/
	int InstallSourceOdbcMsSql( const String & database );

	/** Deletes an ODBC source for a MsSql database.
	@param dsn
		ODBC source name.
	@return
		Exit code of the CProcess executing the script file.
	*/
	int UninstallSourceOdbcMsSql( const String & database );

	namespace DatabaseUtils
	{
		template< size_t StmtType > struct StatementTyper;

		static const String Name[2] =
		{
			STR( "Query" ),
			STR( "Statement" ),
		};

		template<>
		struct StatementTyper< 0 >
		{
			typedef CDatabaseQuery Type;
		};

		template<>
		struct StatementTyper< 1 >
		{
			typedef CDatabaseStatement Type;
		};

		template< size_t StmtType >
		std::shared_ptr< typename StatementTyper< StmtType >::Type > CreateStmt( DatabaseConnectionSPtr connection, const String & query );

		template<>
		inline std::shared_ptr< CDatabaseStatement > CreateStmt< 1 >( DatabaseConnectionSPtr connection, const String & query )
		{
			std::shared_ptr< CDatabaseStatement > result;

			try
			{
				result = connection->CreateStatement( query );
			}
			catch ( std::exception & e )
			{
				CLogger::LogWarning( e.what() );
			}

			return result;
		}

		template<>
		inline std::shared_ptr< CDatabaseQuery > CreateStmt< 0 >( DatabaseConnectionSPtr connection, const String & query )
		{
			std::shared_ptr< CDatabaseQuery > result;

			try
			{
				result = connection->CreateQuery( query );
			}
			catch ( std::exception & e )
			{
				CLogger::LogWarning( e.what() );
			}

			return result;
		}

		template< EFieldType FieldType, typename Enable = void >
		struct ParameterCreator
		{
			template< typename Statement >
			static void Create( std::shared_ptr< Statement > stmt, const String & name )
			{
				BOOST_CHECK( stmt->CreateParameter( name, FieldType ) );
			}
		};

		template< EFieldType FieldType >
		struct ParameterCreator< FieldType, typename std::enable_if< SFieldTypeNeedsLimits< FieldType >::value >::type >
		{
			template< typename Statement >
			static void Create( std::shared_ptr< Statement > stmt, const String & name )
			{
				BOOST_CHECK( stmt->CreateParameter( name, FieldType, Helpers< FieldType >::Limit ) );
			}
		};

		template< EFieldType FieldType >
		struct ParameterCreator< FieldType, typename std::enable_if< SFieldTypeNeedsPrecision< FieldType >::value >::type >
		{
			template< typename Statement >
			static void Create( std::shared_ptr< Statement > stmt, const String & name )
			{
				BOOST_CHECK( stmt->CreateParameter( name, FieldType, Helpers< FieldType >::Precision ) );
			}
		};

		template< typename Statement >
		inline void CreateParameters( std::shared_ptr< Statement > stmt )
		{
			BOOST_CHECK( stmt->CreateParameter( STR( "IntField" ), EFieldType_SINT32 ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "IntegerField" ), EFieldType_SINT32 ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "TinyIntField" ), EFieldType_UINT8 ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "SmallIntField" ), EFieldType_SINT16 ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "MediumIntField" ), EFieldType_SINT24 ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "BigIntField" ), EFieldType_SINT64 ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "Int2Field" ), EFieldType_SINT16 ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "Int8Field" ), EFieldType_SINT64 ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "RealField" ), EFieldType_FLOAT64 ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "DoubleField" ), EFieldType_FLOAT64 ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "DoublePrecisionField" ), EFieldType_FLOAT64 ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "FloatField" ), EFieldType_FLOAT32 ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "NumericField" ), EFieldType_FIXED_POINT, std::make_pair( 10, 0 ) ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "DecimalField" ), EFieldType_FIXED_POINT, std::make_pair( 10, 5 ) ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "BooleanField" ), EFieldType_BIT ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "DateField" ), EFieldType_DATE ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "DateTimeField" ), EFieldType_DATETIME ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "CharacterField" ), EFieldType_CHAR, 20 ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "VarcharField" ), EFieldType_VARCHAR, 255 ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "NcharField" ), EFieldType_NCHAR, 55 ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "NVarcharField" ), EFieldType_NVARCHAR, 100 ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "TextField" ), EFieldType_TEXT ) );
			BOOST_CHECK( stmt->CreateParameter( STR( "BlobField" ), EFieldType_BLOB ) );
		}

		template< typename Statement >
		inline void UncheckedCreateParameters( std::shared_ptr< Statement > stmt )
		{
			stmt->CreateParameter( STR( "IntField" ), EFieldType_SINT32 );
			stmt->CreateParameter( STR( "IntegerField" ), EFieldType_SINT32 );
			stmt->CreateParameter( STR( "TinyIntField" ), EFieldType_UINT8 );
			stmt->CreateParameter( STR( "SmallIntField" ), EFieldType_SINT16 );
			stmt->CreateParameter( STR( "MediumIntField" ), EFieldType_SINT24 );
			stmt->CreateParameter( STR( "BigIntField" ), EFieldType_SINT64 );
			stmt->CreateParameter( STR( "Int2Field" ), EFieldType_SINT16 );
			stmt->CreateParameter( STR( "Int8Field" ), EFieldType_SINT64 );
			stmt->CreateParameter( STR( "RealField" ), EFieldType_FLOAT64 );
			stmt->CreateParameter( STR( "DoubleField" ), EFieldType_FLOAT64 );
			stmt->CreateParameter( STR( "DoublePrecisionField" ), EFieldType_FLOAT64 );
			stmt->CreateParameter( STR( "FloatField" ), EFieldType_FLOAT32 );
			stmt->CreateParameter( STR( "NumericField" ), EFieldType_FIXED_POINT, std::make_pair( 10, 0 ) );
			stmt->CreateParameter( STR( "DecimalField" ), EFieldType_FIXED_POINT, std::make_pair( 10, 5 ) );
			stmt->CreateParameter( STR( "BooleanField" ), EFieldType_BIT );
			stmt->CreateParameter( STR( "DateField" ), EFieldType_DATE );
			stmt->CreateParameter( STR( "DateTimeField" ), EFieldType_DATETIME );
			stmt->CreateParameter( STR( "CharacterField" ), EFieldType_CHAR, 20 );
			stmt->CreateParameter( STR( "VarcharField" ), EFieldType_VARCHAR, 255 );
			stmt->CreateParameter( STR( "NcharField" ), EFieldType_NCHAR, 55 );
			stmt->CreateParameter( STR( "NVarcharField" ), EFieldType_NVARCHAR, 100 );
			stmt->CreateParameter( STR( "TextField" ), EFieldType_TEXT );
			stmt->CreateParameter( STR( "BlobField" ), EFieldType_BLOB );
		}

		template< typename Statement >
		inline void SetParametersValue( std::random_device & generator, uint32_t & index, int mult, int i, std::shared_ptr< Statement > stmt )
		{
			stmt->SetParameterValue( index++, Helpers< EFieldType_SINT32 >::GetRandomValue( generator ) );
			stmt->SetParameterValue( index++, Helpers< EFieldType_SINT32 >::GetRandomValue( generator ) );
			stmt->SetParameterValue( index++, Helpers< EFieldType_UINT8 >::GetRandomValue( generator ) );
			stmt->SetParameterValue( index++, Helpers< EFieldType_SINT16 >::GetRandomValue( generator ) );
			stmt->SetParameterValue( index++, Helpers< EFieldType_SINT24 >::GetRandomValue( generator ) );
			stmt->SetParameterValue( index++, Helpers< EFieldType_SINT64 >::GetRandomValue( generator ) );
			stmt->SetParameterValue( index++, Helpers< EFieldType_SINT16 >::GetRandomValue( generator ) );
			stmt->SetParameterValue( index++, Helpers< EFieldType_SINT64 >::GetRandomValue( generator ) );
			stmt->SetParameterValue( index++, Helpers< EFieldType_FLOAT64 >::GetRandomValue( generator ) );
			stmt->SetParameterValue( index++, Helpers< EFieldType_FLOAT64 >::GetRandomValue( generator ) );
			stmt->SetParameterValue( index++, Helpers< EFieldType_FLOAT64 >::GetRandomValue( generator ) );
			stmt->SetParameterValue( index++, Helpers< EFieldType_FLOAT32 >::GetRandomValue( generator ) );
			stmt->SetParameterValue( index++, Helpers< EFieldType_FIXED_POINT >::GetRandomValue( generator, 10, 0 ) );
			stmt->SetParameterValue( index++, Helpers< EFieldType_FIXED_POINT >::GetRandomValue( generator, 10, 5 ) );
			stmt->SetParameterValue( index++, Helpers< EFieldType_BIT >::GetRandomValue( generator ) );
			stmt->SetParameterValue( index++, Helpers< EFieldType_DATE >::GetRandomValue( generator ) );
			stmt->SetParameterValue( index++, Helpers< EFieldType_DATETIME >::GetRandomValue( generator ) );
			stmt->SetParameterValue( index++, Helpers< EFieldType_CHAR >::GetRandomValue( generator ) );
			stmt->SetParameterValue( index++, Helpers< EFieldType_VARCHAR >::GetRandomValue( generator ) );
			stmt->SetParameterValue( index++, Helpers< EFieldType_NCHAR >::GetRandomValue( generator ) );
			stmt->SetParameterValue( index++, Helpers< EFieldType_NVARCHAR >::GetRandomValue( generator ) );
			stmt->SetParameterValue( index++, Helpers< EFieldType_TEXT >::GetRandomValue( generator ) );
			stmt->SetParameterValue( index++, Helpers< EFieldType_BLOB >::GetRandomValue( generator ) );
		}

		inline std::wstring GetWString( uint32_t index, DatabaseRowSPtr row )
		{
			std::wstring ret;
			DatabaseFieldSPtr field = row->GetField( index );

			switch ( field->GetType() )
			{
			case EFieldType_CHAR:
			case EFieldType_VARCHAR:
			case EFieldType_TEXT:
				ret = StringUtils::ToWStr( field->GetValue< std::string >() );
				break;

			case EFieldType_NCHAR:
			case EFieldType_NVARCHAR:
			case EFieldType_NTEXT:
				ret = field->GetValue< std::wstring >();
				break;
			}

			return ret;
		}

		inline void DisplayValues( uint32_t & index, DatabaseRowSPtr row )
		{
			CLogger::LogInfo( StringStream() << STR( "IntField : " ) << row->Get< int32_t >( index++ ) );
			CLogger::LogInfo( StringStream() << STR( "IntegerField : " ) << row->Get< int32_t >( index++ ) );
			CLogger::LogInfo( StringStream() << STR( "TinyIntField : " ) << int( row->Get< int16_t >( index++ ) ) );
			CLogger::LogInfo( StringStream() << STR( "SmallIntField : " ) << row->Get< int16_t >( index++ ) );
			CLogger::LogInfo( StringStream() << STR( "MediumIntField : " ) << row->Get< int32_t >( index++ ) );
			CLogger::LogInfo( StringStream() << STR( "BigIntField : " ) << row->Get< int64_t >( index++ ) );
			CLogger::LogInfo( StringStream() << STR( "Int2Field : " ) << row->Get< int16_t >( index++ ) );
			CLogger::LogInfo( StringStream() << STR( "Int8Field : " ) << row->Get< int64_t >( index++ ) );
			CLogger::LogInfo( StringStream() << STR( "RealField : " ) << row->Get< double >( index++ ) );
			CLogger::LogInfo( StringStream() << STR( "DoubleField : " ) << row->Get< double >( index++ ) );
			CLogger::LogInfo( StringStream() << STR( "DoublePrecisionField : " ) << row->Get< double >( index++ ) );
			CLogger::LogInfo( StringStream() << STR( "FloatField : " ) << row->Get< float >( index++ ) );
			CLogger::LogInfo( StringStream() << STR( "NumericField : " ) << row->Get< CFixedPoint >( index++ ).ToString() );
			CLogger::LogInfo( StringStream() << STR( "DecimalField : " ) << row->Get< CFixedPoint >( index++ ).ToString() );
			CLogger::LogInfo( StringStream() << STR( "BooleanField : " ) << row->Get< bool >( index++ ) );
			CLogger::LogInfo( StringStream() << STR( "DateField : " ) << row->Get< DateType >( index++ ) );
			CLogger::LogInfo( StringStream() << STR( "DateTimeField : " ) << row->Get< DateTimeType >( index++ ) );
			CLogger::LogInfo( StringStream() << STR( "CharacterField : " ) << row->Get< std::string >( index++ ) );
			CLogger::LogInfo( StringStream() << STR( "VarcharField : " ) << row->Get< std::string >( index++ ) );
			CLogger::LogInfo( std::wstringstream() << L"NcharField : " << GetWString( index++, row ) );
			CLogger::LogInfo( std::wstringstream() << L"NVarcharField : " << GetWString( index++, row ) );
			CLogger::LogInfo( StringStream() << STR( "TextField : " ) << row->Get< std::string >( index++ ) );
			CLogger::LogInfo( StringStream() << STR( "BlobField : " ) << row->Get< ByteArray >( index++ ) );
		}

		template< size_t StmtType, EFieldType FieldType >
		inline void FillStatement( std::shared_ptr< typename StatementTyper< StmtType >::Type > statement, const String & name, bool other, typename Helpers< FieldType >::ParamType const * valueIn )
		{
			uint32_t index = 0;

			if ( other )
			{
				ParameterCreator< EFieldType_SINT32 >::Create( statement, STR( "IntField" ) );
				ParameterCreator< FieldType >::Create( statement, name );
				BOOST_CHECK( statement->Initialise() == EErrorType_NONE );
				BOOST_CHECK_NO_THROW( statement->SetParameterValue( index++, 18 ) );
			}
			else
			{
				ParameterCreator< FieldType >::Create( statement, name );
				BOOST_CHECK( statement->Initialise() == EErrorType_NONE );
			}

			if ( valueIn )
			{
				BOOST_CHECK_NO_THROW( statement->SetParameterValue( index++, *valueIn ) );
			}
			else
			{
				BOOST_CHECK_NO_THROW( statement->SetParameterNull( index++ ) );
			}
		}

		template< size_t StmtType, EFieldType FieldType >
		inline std::shared_ptr< typename StatementTyper< StmtType >::Type > CreateInsertStatement( DatabaseConnectionSPtr connection, const String & name, bool other, typename Helpers< FieldType >::ParamType const * valueIn, String const & is )
		{
			auto statement = CreateStmt< StmtType >( connection, STR( "INSERT INTO Test (" ) + ( other ? String( "IntField, " ) : String() ) + name + STR( ") VALUES (" ) + ( other ? String( "?, " ) : String() ) + STR( "?)" ) );
			
			if ( statement )
			{
				FillStatement< StmtType, FieldType >( statement, name, other, valueIn );
			}
			else
			{
				BOOST_CHECK( statement );
			}

			return statement;
		}

		template< size_t StmtType, EFieldType FieldType >
		inline std::shared_ptr< typename StatementTyper< StmtType >::Type > CreateSelectStatement( DatabaseConnectionSPtr connection, const String & name, bool other, typename Helpers< FieldType >::ParamType const * valueIn, String const & is )
		{
			auto statement = CreateStmt< StmtType >( connection, STR( "SELECT " ) + name + STR( " FROM Test WHERE " ) + ( other ? String( "IntField = ? AND " ) : String() ) + name + STR( " " ) + ( valueIn ? String( "=" ) : is ) + STR( " ?" ) );

			if ( statement )
			{
				FillStatement< StmtType, FieldType >( statement, name, other, valueIn );
			}
			else
			{
				BOOST_CHECK( statement );
			}

			return statement;
		}

		template<>
		inline std::shared_ptr< typename StatementTyper< 0 >::Type > CreateSelectStatement< 0, EFieldType_FLOAT32 >( DatabaseConnectionSPtr connection, const String & name, bool other, Helpers< EFieldType_FLOAT32 >::ParamType const * valueIn, String const & is )
		{
			auto statement = CreateStmt< 0 >( connection, STR( "SELECT " ) + name + STR( " FROM Test WHERE " ) + ( other ? String( "IntField = ? AND " ) : String() ) + STR( "CAST( " ) + name + STR( " AS DECIMAL( " ) + StringUtils::ToString( 39 + connection->GetPrecision( EFieldType_FLOAT32 ) ) + STR( ", " ) + StringUtils::ToString( connection->GetPrecision( EFieldType_FLOAT32 ) ) + STR( " ) )" ) + STR( " " ) + ( valueIn ? String( "=" ) : is ) + STR( " ?" ) );

			if ( statement )
			{
				FillStatement< 0, EFieldType_FLOAT32 >( statement, name, other, valueIn );
			}
			else
			{
				BOOST_CHECK( statement );
			}

			return statement;
		}

		template< size_t StmtType, EFieldType FieldType, typename Function >
		inline void DoInsertAndRetrieve( DatabaseConnectionSPtr connection, const String & name, typename Helpers< FieldType >::ParamType const * valueIn, bool equal, String const & is, bool other, Function function )
		{
			try
			{
				auto stmtInsert = CreateInsertStatement< StmtType, FieldType >( connection, name, other, valueIn, is );
				auto stmtSelect = CreateSelectStatement< StmtType, FieldType >( connection, name, other, valueIn, is );

				if ( stmtInsert && stmtSelect )
				{
					BOOST_CHECK( stmtInsert->ExecuteUpdate() );
					DatabaseResultSPtr result = stmtSelect->ExecuteSelect();

					if ( result )
					{
						if ( result->GetRowCount() )
						{
							if ( valueIn )
							{
								typename Helpers< FieldType >::FieldType valueOut;
								BOOST_CHECK_NO_THROW( function( result, valueOut ) );
								Helpers< FieldType >::Compare( equal, static_cast< CDatabaseValue< FieldType > const & >( stmtInsert->GetParameter( other ? 1 : 0 )->GetObjectValue() ).GetValue(), valueOut );
							}
							else
							{
								try
								{
									BOOST_CHECK( result->GetFirstRow()->GetField( 0 )->GetObjectValue().IsNull() );
								}
								catch ( CDatabaseException & exc )
								{
									CLogger::LogError( exc.GetFullDescription() );
									BOOST_CHECK_NO_THROW( result->GetFirstRow()->GetField( 0 ) );
								}
							}
						}
						else
						{
							BOOST_CHECK( result->GetRowCount() );
						}
					}
					else
					{
						BOOST_CHECK( result );
					}
				}
			}
			catch ( CDatabaseException & exc )
			{
				CLogger::LogError( exc.GetFullDescription() );
				throw;
			}
			catch ( std::exception & exc )
			{
				CLogger::LogError( exc.what() );
				throw;
			}
			catch ( ... )
			{
				CLogger::LogError( "Unknown exception when executing DoInsertAndRetrieve" );
				throw;
			}
		}

		template< size_t StmtType, EFieldType FieldType >
		inline void InsertAndRetrieve( DatabaseConnectionSPtr connection, const String & name, typename Helpers< FieldType >::ParamType const * valueIn, bool equal, String const & is )
		{
			DoInsertAndRetrieve< StmtType, FieldType >( connection, name, valueIn, equal, is, false, []( DatabaseResultSPtr result, typename Helpers< FieldType >::FieldType & valueOut )
			{
				result->GetFirstRow()->Get( 0, valueOut );
			} );
		}

		template< size_t StmtType, EFieldType FieldType >
		inline void InsertAndRetrieveOtherIndex( DatabaseConnectionSPtr connection, const String & name, typename Helpers< FieldType >::ParamType const * valueIn, bool equal, String const & is )
		{
			DoInsertAndRetrieve< StmtType, FieldType >( connection, name, valueIn, equal, is, true, []( DatabaseResultSPtr result, typename Helpers< FieldType >::FieldType & valueOut )
			{
				result->GetFirstRow()->Get( 0, valueOut );
			} );
		}

		template< size_t StmtType, EFieldType FieldType >
		inline void InsertAndRetrieveFast( DatabaseConnectionSPtr connection, const String & name, typename Helpers< FieldType >::ParamType const * valueIn, bool equal, String const & is )
		{
			DoInsertAndRetrieve< StmtType, FieldType >( connection, name, valueIn, equal, is, false, []( DatabaseResultSPtr result, typename Helpers< FieldType >::FieldType & valueOut )
			{
				result->GetFirstRow()->GetFast( 0, valueOut );
			} );
		}

		template< size_t StmtType, EFieldType FieldType >
		inline void InsertAndRetrieveFastOtherIndex( DatabaseConnectionSPtr connection, const String & name, typename Helpers< FieldType >::ParamType const * valueIn, bool equal, String const & is )
		{
			DoInsertAndRetrieve< StmtType, FieldType >( connection, name, valueIn, equal, is, true, []( DatabaseResultSPtr result, typename Helpers< FieldType >::FieldType & valueOut )
			{
				result->GetFirstRow()->GetFast( 0, valueOut );
			} );
		}

		template< size_t StmtType >
		struct Countor
		{
			Countor( DatabaseConnectionSPtr connection )
				: _statement( CreateStmt< StmtType >( connection, QUERY_GET_COUNT ) )
			{
				if ( _statement )
				{
					_statement->Initialise();
				}
			}

			~Countor()
			{
			}

			inline int64_t GetValue()
			{
				int64_t count = 0;

				if ( _statement )
				{
					DatabaseResultSPtr result = _statement->ExecuteSelect();

					if ( result && result->GetRowCount() )
					{
						result->GetFirstRow()->Get( 0, count );
					}
				}

				return count;
			}

			std::shared_ptr< typename StatementTyper< StmtType >::Type > _statement;
		};

		template< size_t StmtType >
		struct Minor
		{
			Minor( DatabaseConnectionSPtr connection )
				: _stmtGetMin( CreateStmt< StmtType >( connection, QUERY_SELECT_MIN ) )
			{
				if ( _stmtGetMin )
				{
					_stmtGetMin->Initialise();
				}
			}

			~Minor()
			{
			}

			inline int64_t GetValue()
			{
				int64_t max = 0;

				if ( _stmtGetMin )
				{
					DatabaseResultSPtr result = _stmtGetMin->ExecuteSelect();

					if ( result && result->GetRowCount() )
					{
						result->GetFirstRow()->Get( 0, max );
					}
				}

				return max;
			}

		private:
			std::shared_ptr< typename StatementTyper< StmtType >::Type > _stmtGetMin;
		};

		template< size_t StmtType >
		struct Maxor
		{
			Maxor( DatabaseConnectionSPtr connection )
				: _stmtGetMax( CreateStmt< StmtType >( connection, QUERY_SELECT_MAX ) )
			{
				if ( _stmtGetMax )
				{
					_stmtGetMax->Initialise();
				}
			}

			~Maxor()
			{
			}

			inline int64_t GetValue()
			{
				int64_t max = 0;

				if ( _stmtGetMax )
				{
					DatabaseResultSPtr result = _stmtGetMax->ExecuteSelect();

					if ( result && result->GetRowCount() )
					{
						result->GetFirstRow()->Get( 0, max );
					}
				}

				return max;
			}

			std::shared_ptr< typename StatementTyper< StmtType >::Type > _stmtGetMax;
		};

		template< size_t StmtType >
		struct IdSelector
		{
			IdSelector( DatabaseConnectionSPtr connection )
				: _maxor( connection )
				, _minor( connection )
			{
			}

			~IdSelector()
			{
			}

			inline int64_t GetValue()
			{
				int64_t min = _minor.GetValue();
				int64_t max = _maxor.GetValue();

				int64_t result = 0;

				if ( max && min )
				{
					std::uniform_int_distribution< int64_t > distribution( min, max );
					result = distribution( _generator );
				}

				return result;
			}

			std::random_device _generator;
			Minor< StmtType > _minor;
			Maxor< StmtType > _maxor;
		};

		template< size_t StmtType >
		struct Insertor
		{
			Insertor( DatabaseConnectionSPtr connection )
				: _statement( CreateStmt< StmtType >( connection, QUERY_DIRECT_INSERT_ELEMENT ) )
			{
				if ( _statement )
				{
					UncheckedCreateParameters( _statement );
					_statement->Initialise();
				}
			}

			~Insertor()
			{
			}

			inline bool Run()
			{
				bool ret = false;

				if ( _statement )
				{
					uint32_t index = 0;
					SetParametersValue( _generator, index, 1, 0, _statement );
					ret = _statement->ExecuteUpdate();
				}

				return ret;
			}

			std::random_device _generator;
			std::shared_ptr< typename StatementTyper< StmtType >::Type > _statement;
		};

		template< size_t StmtType >
		struct Selector
		{
			Selector( DatabaseConnectionSPtr connection )
				: _selector( connection )
				, _statement( CreateStmt< StmtType >( connection, QUERY_DIRECT_SELECT_ELEMENT ) )
			{
				if ( _statement )
				{
					_statement->CreateParameter( STR( "IDTest" ), EFieldType_SINT64, EParameterType_IN );
					_statement->Initialise();
				}
			}

			~Selector()
			{
			}

			inline bool Run()
			{
				bool ret = false;

				if ( _statement )
				{
					int64_t id = _selector.GetValue();

					if ( id )
					{
						_statement->SetParameterValue( 0, id );
						ret = _statement->ExecuteUpdate();
					}
				}

				return ret;
			}

			std::random_device _generator;
			IdSelector< StmtType > _selector;
			std::shared_ptr< typename StatementTyper< StmtType >::Type > _statement;
		};

		template< size_t StmtType >
		struct Updator
		{
			Updator( DatabaseConnectionSPtr connection )
				: _selector( connection )
				, _statement( CreateStmt< StmtType >( connection, QUERY_DIRECT_UPDATE_ELEMENT ) )
			{
				if ( _statement )
				{
					UncheckedCreateParameters( _statement );
					_statement->CreateParameter( STR( "IDTest" ), EFieldType_SINT64, EParameterType_IN );
					_statement->Initialise();
				}
			}

			~Updator()
			{
			}

			inline bool Run()
			{
				bool ret = false;

				if ( _statement )
				{
					int64_t id = _selector.GetValue();

					if ( id )
					{
						uint32_t index = 0;
						SetParametersValue( _generator, index, 1, 0, _statement );
						_statement->SetParameterValue( index++, id );
						ret = _statement->ExecuteUpdate();
					}
				}

				return ret;
			}

			std::random_device _generator;
			IdSelector< StmtType > _selector;
			std::shared_ptr< typename StatementTyper< StmtType >::Type > _statement;
		};

		template< size_t StmtType >
		struct Deletor
		{
			Deletor( DatabaseConnectionSPtr connection )
				: _stmtDelete( CreateStmt< StmtType >( connection, QUERY_DIRECT_DELETE_ELEMENT ) )
				, _minor( connection )
			{
				if ( _stmtDelete )
				{
					_stmtDelete->CreateParameter( STR( "IDTest" ), EFieldType_SINT64, EParameterType_IN );
					_stmtDelete->Initialise();
				}
			}

			~Deletor()
			{
			}

			inline bool Run()
			{
				bool ret = false;

				if ( _stmtDelete )
				{
					int64_t min = _minor.GetValue();

					if ( min )
					{
						_stmtDelete->SetParameterValue( 0, min );
						ret = _stmtDelete->ExecuteUpdate();
					}
				}

				return ret;
			}

			std::shared_ptr< typename StatementTyper< StmtType >::Type > _stmtDelete;
			Minor< StmtType > _minor;
		};

		template< size_t StmtType, typename Action >
		inline void TransactedAction( std::random_device & generator, Action action, DatabaseConnectionSPtr connection )
		{
			if ( connection->BeginTransaction( String() ) )
			{
				action( generator, connection );
				connection->Commit( String() );
			}
		}

		template< size_t StmtType >
		inline void TestDirectInsert( std::random_device & generator, DatabaseConnectionSPtr connection )
		{
			try
			{
				Countor< StmtType > countor( connection );
				int64_t count = 0;
				BOOST_CHECK_NO_THROW( count = countor.GetValue() + 1 );

				if ( count )
				{
					int const inserts = 20;
					Insertor< StmtType > insertor( connection );

					for ( int i = 0; i < inserts; i++ )
					{
						BOOST_CHECK( insertor.Run() );
					}

					count--;
					BOOST_CHECK_EQUAL( countor.GetValue(), count + inserts );
				}
			}
			catch ( CDatabaseException & exc )
			{
				CLogger::LogError( exc.GetFullDescription() );
				throw;
			}
			catch ( std::exception & exc )
			{
				CLogger::LogError( exc.what() );
				throw;
			}
			catch ( ... )
			{
				CLogger::LogError( "Unknown exception when executing TestDirectInsert" );
				throw;
			}
		}

		template< size_t StmtType >
		inline void TestDirectSelect( std::random_device & generator, DatabaseConnectionSPtr connection )
		{
			try
			{
				int const selects = 10;
				Selector< StmtType > selector( connection );

				for ( int i = 0; i < selects; i++ )
				{
					BOOST_CHECK( selector.Run() );
				}
			}
			catch ( CDatabaseException & exc )
			{
				CLogger::LogError( exc.GetFullDescription() );
				throw;
			}
			catch ( std::exception & exc )
			{
				CLogger::LogError( exc.what() );
				throw;
			}
			catch ( ... )
			{
				CLogger::LogError( "Unknown exception when executing TestDirectSelect" );
				throw;
			}
		}

		template< size_t StmtType >
		inline void TestDirectUpdate( std::random_device & generator, DatabaseConnectionSPtr connection )
		{
			try
			{
				int const updates = 10;
				Updator< StmtType > updator( connection );

				for ( int i = 0; i < updates; i++ )
				{
					BOOST_CHECK( updator.Run() );
				}
			}
			catch ( CDatabaseException & exc )
			{
				CLogger::LogError( exc.GetFullDescription() );
				throw;
			}
			catch ( std::exception & exc )
			{
				CLogger::LogError( exc.what() );
				throw;
			}
			catch ( ... )
			{
				CLogger::LogError( "Unknown exception when executing TestDirectUpdate" );
				throw;
			}
		}

		template< size_t StmtType >
		inline void TestDirectDelete( std::random_device & generator, DatabaseConnectionSPtr connection )
		{
			try
			{
				int const deletes = 10;
				Deletor< StmtType > deletor( connection );

				for ( int i = 0; i < deletes; i++ )
				{
					BOOST_CHECK( deletor.Run() );
				}
			}
			catch ( CDatabaseException & exc )
			{
				CLogger::LogError( exc.GetFullDescription() );
				throw;
			}
			catch ( std::exception & exc )
			{
				CLogger::LogError( exc.what() );
				throw;
			}
			catch ( ... )
			{
				CLogger::LogError( "Unknown exception when executing TestDirectDelete" );
				throw;
			}
		}

		template< size_t StmtType >
		inline void PerfDirectSelectActors( DatabaseConnectionSPtr connection, uint32_t testCount, const String & whereClause )
		{
			try
			{
				std::shared_ptr< typename StatementTyper< StmtType >::Type > stmtGetActors;

				if ( whereClause.empty() )
				{
					stmtGetActors = CreateStmt< StmtType >( connection, QUERY_DIRECT_SELECT_ALL_ELEMENTS );
				}
				else
				{
					stmtGetActors = CreateStmt< StmtType >( connection, QUERY_DIRECT_SELECT_ALL_ELEMENTS + STR( " " ) + whereClause );
				}

				if ( stmtGetActors )
				{
					stmtGetActors->Initialise();
					std::clock_t start = std::clock();
					DatabaseResultSPtr result = stmtGetActors->ExecuteSelect();
					CLogger::LogInfo( StringStream() << "    Selected the elements in " << float( std::clock() - start ) / CLOCKS_PER_SEC << "seconds (no fetch)" );

					start = std::clock();

					if ( result )
					{
						uint32_t count = result->GetRowCount();

						if ( count )
						{
							DatabaseRowSPtr row = result->GetFirstRow();

							while ( row )
							{
								row = result->GetNextRow();
							}
						}

						CLogger::LogInfo( StringStream() << "    Fetched " << count << " elements in " << float( std::clock() - start ) / CLOCKS_PER_SEC << "seconds" );
					}
				}
			}
			catch ( CDatabaseException & exc )
			{
				CLogger::LogError( exc.GetFullDescription() );
				throw;
			}
			catch ( std::exception & exc )
			{
				CLogger::LogError( exc.what() );
				throw;
			}
			catch ( ... )
			{
				CLogger::LogError( "Unknown exception when executing PerfDirectSelectActors" );
				throw;
			}
		}

		template< size_t StmtType >
		inline void PerfStoredProcedureSelectActors( DatabaseConnectionSPtr connection, uint32_t testCount, const String & whereClause )
		{
			try
			{
				auto stmtGetActors = CreateStmt< StmtType >( connection, STR( "CALL SpGetActors(?)" ) );

				if ( stmtGetActors )
				{
					stmtGetActors->CreateParameter( STR( "Where" ), EFieldType_VARCHAR, 500 );
					stmtGetActors->Initialise();
					std::clock_t start = std::clock();
					stmtGetActors->SetParameterValue( 0, whereClause );
					DatabaseResultSPtr result = stmtGetActors->ExecuteSelect();
					CLogger::LogInfo( StringStream() << "    Selected the elements in " << float( std::clock() - start ) / CLOCKS_PER_SEC << "seconds (no fetch)" );

					start = std::clock();

					if ( result )
					{
						uint32_t count = result->GetRowCount();

						if ( count )
						{
							DatabaseRowSPtr row = result->GetFirstRow();

							while ( row )
							{
								row = result->GetNextRow();
							}
						}

						CLogger::LogInfo( StringStream() << "    Fetched " << count << " elements in " << float( std::clock() - start ) / CLOCKS_PER_SEC << "seconds" );
					}
				}
			}
			catch ( CDatabaseException & exc )
			{
				CLogger::LogError( exc.GetFullDescription() );
				throw;
			}
			catch ( std::exception & exc )
			{
				CLogger::LogError( exc.what() );
				throw;
			}
			catch ( ... )
			{
				CLogger::LogError( "Unknown exception when executing PerfStoredProcedureSelectActors" );
				throw;
			}
		}

		template< size_t StmtType >
		inline void PerfDirectDeleteActors( DatabaseConnectionSPtr connection, uint32_t testCount )
		{
			try
			{
				auto stmtGetMin = CreateStmt< StmtType >( connection, QUERY_SELECT_MIN );
				int64_t min = 0;

				if ( stmtGetMin )
				{
					stmtGetMin->Initialise();
					DatabaseResultSPtr result = stmtGetMin->ExecuteSelect();

					if ( result && result->GetRowCount() )
					{
						result->GetFirstRow()->Get( 0, min );
					}
				}

				if ( min )
				{
					auto stmtDelete = CreateStmt< StmtType >( connection, QUERY_DIRECT_DELETE_ELEMENT );

					if ( stmtDelete )
					{
						stmtDelete->CreateParameter( STR( "id" ), EFieldType_SINT64 );
						stmtDelete->Initialise();
						std::clock_t const start = std::clock();

						for ( uint32_t i = 0; i < testCount; i++ )
						{
							int64_t id( min + i );
							stmtDelete->SetParameterValue( 0, id );
							stmtDelete->ExecuteUpdate();
						}

						CLogger::LogInfo( StringStream() << "    Deleted " << testCount << " elements in " << float( std::clock() - start ) / CLOCKS_PER_SEC << "seconds" );
					}
				}
			}
			catch ( CDatabaseException & exc )
			{
				CLogger::LogError( exc.GetFullDescription() );
				throw;
			}
			catch ( std::exception & exc )
			{
				CLogger::LogError( exc.what() );
				throw;
			}
			catch ( ... )
			{
				CLogger::LogError( "Unknown exception when executing PerfDirectDeleteActors" );
				throw;
			}
		}

		template< size_t StmtType >
		inline void PerfStoredProcedureDeleteActors( DatabaseConnectionSPtr connection, uint32_t testCount )
		{
			try
			{
				auto stmtGetMin = CreateStmt< StmtType >( connection, QUERY_SELECT_MIN );
				int64_t min = 0;

				if ( stmtGetMin )
				{
					stmtGetMin->Initialise();
					DatabaseResultSPtr result = stmtGetMin->ExecuteSelect();

					if ( result )
					{
						if ( result->GetRowCount() )
						{
							result->GetFirstRow()->Get( 0, min );
						}
					}
				}

				if ( min )
				{
					auto stmtDelete = CreateStmt< StmtType >( connection, STR( "CALL SpDeleteActor(?)" ) );

					if ( stmtDelete )
					{
						stmtDelete->CreateParameter( STR( "id" ), EFieldType_SINT64 );
						stmtDelete->Initialise();
						std::clock_t const start = std::clock();

						for ( uint32_t i = 0; i < testCount; i++ )
						{
							int64_t id( min + i );
							stmtDelete->SetParameterValue( 0, id );
							stmtDelete->ExecuteUpdate();
						}

						CLogger::LogInfo( StringStream() << "    Deleted " << testCount << " elements in " << float( std::clock() - start ) / CLOCKS_PER_SEC << "seconds" );
					}
				}
			}
			catch ( CDatabaseException & exc )
			{
				CLogger::LogError( exc.GetFullDescription() );
				throw;
			}
			catch ( std::exception & exc )
			{
				CLogger::LogError( exc.what() );
				throw;
			}
			catch ( ... )
			{
				CLogger::LogError( "Unknown exception when executing PerfStoredProcedureDeleteActors" );
				throw;
			}
		}
	}
}
END_NAMESPACE_DATABASE_TEST

#endif // ___DATABASE_TEST_UTILS_H___
