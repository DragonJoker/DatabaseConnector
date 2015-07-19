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

#include "DatabaseNullable.h"

BEGIN_NAMESPACE_DATABASE
{
	static const String ERROR_DB_ROW_INDEX = STR( "Unknown error for field at index: " );
	static const String ERROR_DB_ROW_NAME = STR( "Unknown error for field named: " );

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
			message << ERROR_DB_ROW_INDEX << index;
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
			message << ERROR_DB_ROW_NAME << name;
			DB_EXCEPT( EDatabaseExceptionCodes_RowError, message.str() );
		}
	}

	template< typename T >
	inline CDatabaseNullable< T > CDatabaseRow::GetOpt( uint32_t index )
	{
		CDatabaseNullable< T > tReturn;
		GetOpt( index, tReturn );
		return tReturn;
	}

	template< typename T >
	inline CDatabaseNullable< T > CDatabaseRow::GetOpt( const String & name )
	{
		CDatabaseNullable< T > tReturn;
		GetOpt( name, tReturn );
		return tReturn;
	}

	template< typename T >
	inline void CDatabaseRow::GetOpt( uint32_t index, CDatabaseNullable< T > & value )
	{
		try
		{
			GetField( index )->GetValueOpt( value );
		}
		catch ( const CExceptionDatabase & )
		{
			// Rethrow
			throw;
		}
		catch ( ... )
		{
			StringStream message;
			message << ERROR_DB_ROW_INDEX << index;
			DB_EXCEPT( EDatabaseExceptionCodes_RowError, message.str() );
		}
	}

	template< typename T >
	inline void CDatabaseRow::GetOpt( const String & name, CDatabaseNullable< T > & value )
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
			message << ERROR_DB_ROW_NAME << name;
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
			message << ERROR_DB_ROW_INDEX << index;
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
			message << ERROR_DB_ROW_NAME << name;
			DB_EXCEPT( EDatabaseExceptionCodes_RowError, message.str() );
		}
	}

	template< typename T >
	inline void CDatabaseRow::GetOptFast( uint32_t index, CDatabaseNullable< T > & value )
	{
		try
		{
			GetField( index )->GetValueOptFast( value );
		}
		catch ( const CExceptionDatabase & )
		{
			// Rethrow
			throw;
		}
		catch ( ... )
		{
			StringStream message;
			message << ERROR_DB_ROW_INDEX << index;
			DB_EXCEPT( EDatabaseExceptionCodes_RowError, message.str() );
		}
	}

	template< typename T >
	inline void CDatabaseRow::GetOptFast( const String & name, CDatabaseNullable< T > & value )
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
			message << ERROR_DB_ROW_NAME << name;
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
