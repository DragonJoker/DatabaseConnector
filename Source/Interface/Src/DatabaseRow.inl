/************************************************************************//**
 * @file DatabaseRow.inl
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/24/2014 8:37:01 AM
 *
 *
 * @brief CDatabaseRow class definition.
 *
 * @details Describes a row of the result set.
 *
 ***************************************************************************/

BEGIN_NAMESPACE_DATABASE
{
	static const String DATABASE_ROW_INDEX_ERROR = STR( "Unknown error for field at index: " );
	static const String DATABASE_ROW_NAME_ERROR = STR( "Unknown error for field named: " );

	template< typename T >
	inline T CDatabaseRow::Get( uint32_t index )
	{
		T tReturn;
		Get( index, tReturn );
		return tReturn;
	}

	template< typename T >
	inline T CDatabaseRow::Get( const String & name )
	{
		T tReturn;
		Get( name, tReturn );
		return tReturn;
	}

	template< typename T >
	inline void CDatabaseRow::Get( uint32_t index, T & value )
	{
		try
		{
			GetField( index )->GetValue( value );
		}
		catch ( const CExceptionDatabase & )
		{
			// Rethrow
			throw;
		}
		catch ( ... )
		{
			StringStream message;
			message << DATABASE_ROW_INDEX_ERROR << index;
			CLogger::LogError( message );
			DB_EXCEPT( EDatabaseExceptionCodes_RowError, message.str() );
		}
	}

	template< typename T >
	inline void CDatabaseRow::Get( const String & name, T & value )
	{
		try
		{
			GetField( name )->GetValue( value );
		}
		catch ( const CExceptionDatabase & )
		{
			// Rethrow
			throw;
		}
		catch ( ... )
		{
			StringStream message;
			message << DATABASE_ROW_NAME_ERROR << name;
			CLogger::LogError( message );
			DB_EXCEPT( EDatabaseExceptionCodes_RowError, message.str() );
		}
	}

	template< typename T >
	inline boost::optional< T > CDatabaseRow::GetOpt( uint32_t index )
	{
		boost::optional< T > tReturn;
		GetOpt( index, tReturn );
		return tReturn;
	}

	template< typename T >
	inline boost::optional< T > CDatabaseRow::GetOpt( const String & name )
	{
		boost::optional< T > tReturn;
		GetOpt( name, tReturn );
		return tReturn;
	}

	template< typename T >
	inline void CDatabaseRow::GetOpt( uint32_t index, boost::optional< T > & value )
	{
		try
		{
			GetField( name )->GetValueOpt( value );
		}
		catch ( const CExceptionDatabase & )
		{
			// Rethrow
			throw;
		}
		catch ( ... )
		{
			StringStream message;
			message << DATABASE_ROW_INDEX_ERROR << index;
			CLogger::LogError( message );
			DB_EXCEPT( EDatabaseExceptionCodes_RowError, message.str() );
		}
	}

	template< typename T >
	inline void CDatabaseRow::GetOpt( const String & name, boost::optional< T > & value )
	{
		try
		{
			GetField( name )->GetValueOpt( value );
		}
		catch ( const CExceptionDatabase & )
		{
			// Rethrow
			throw;
		}
		catch ( ... )
		{
			StringStream message;
			message << DATABASE_ROW_NAME_ERROR << name;
			CLogger::LogError( message );
			DB_EXCEPT( EDatabaseExceptionCodes_RowError, message.str() );
		}
	}

	template< typename T >
	inline void CDatabaseRow::GetFast( uint32_t index, T & value )
	{
		try
		{
			GetField( index )->GetValueFast( value );
		}
		catch ( const CExceptionDatabase & )
		{
			// Rethrow
			throw;
		}
		catch ( ... )
		{
			StringStream message;
			message << DATABASE_ROW_INDEX_ERROR << index;
			CLogger::LogError( message );
			DB_EXCEPT( EDatabaseExceptionCodes_RowError, message.str() );
		}
	}

	template< typename T >
	inline void CDatabaseRow::GetFast( const String & name, T & value )
	{
		try
		{
			GetField( name )->GetValueFast( value );
		}
		catch ( const CExceptionDatabase & )
		{
			// Rethrow
			throw;
		}
		catch ( ... )
		{
			StringStream message;
			message << DATABASE_ROW_NAME_ERROR << name;
			CLogger::LogError( message );
			DB_EXCEPT( EDatabaseExceptionCodes_RowError, message.str() );
		}
	}

	template< typename T >
	inline void CDatabaseRow::GetOptFast( uint32_t index, boost::optional< T > & value )
	{
		try
		{
			GetField( name )->GetValueOptFast( value );
		}
		catch ( const CExceptionDatabase & )
		{
			// Rethrow
			throw;
		}
		catch ( ... )
		{
			StringStream message;
			message << DATABASE_ROW_INDEX_ERROR << index;
			CLogger::LogError( message );
			DB_EXCEPT( EDatabaseExceptionCodes_RowError, message.str() );
		}
	}

	template< typename T >
	inline void CDatabaseRow::GetOptFast( const String & name, boost::optional< T > & value )
	{
		try
		{
			GetField( name )->GetValueOptFast( value );
		}
		catch ( const CExceptionDatabase & )
		{
			// Rethrow
			throw;
		}
		catch ( ... )
		{
			StringStream message;
			message << DATABASE_ROW_NAME_ERROR << name;
			CLogger::LogError( message );
			DB_EXCEPT( EDatabaseExceptionCodes_RowError, message.str() );
		}
	}

	template< typename T >
	inline T CDatabaseRow::GetDefault()
	{
		return T();
	}
}
END_NAMESPACE_DATABASE
