/************************************************************************//**
* @file DatabaseNullable.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDatabaseNullable class declaration.
*
* @details Describes a way to have nullable (optional) values
*
***************************************************************************/

#ifndef ___DATABASE_NULLABLE_H___
#define ___DATABASE_NULLABLE_H___

#include "DatabaseValuedObject.h"
#include "DatabaseException.h"

BEGIN_NAMESPACE_DATABASE
{
	static const String ERROR_DB_VALUE_NOT_SET = STR( "The value for the nullable type is not set" );

	/** Used to initialise a CDatabaseNullable to null
	*/
	struct CDatabaseNullableNoneType
	{
	};

	/** Describes a way to have nullable (optional) values
	*/
	template< typename T >
	class CDatabaseNullable
	{
	public:
		/** Default constructor
		@remarks
			The value is null
		*/
		CDatabaseNullable()
			: _isSet( false )
		{
		}

		/** Specified constructor
		@remarks
			The value is not null
		@param[in] value
			The value
		*/
		CDatabaseNullable( T const & value )
			: _isSet( true )
			, _value( value )
		{
		}

		/** Assignment operator from a value
		@param value
			The new value
		@return
			A reference to this object
		*/
		CDatabaseNullable & operator =( T const & value )
		{
			_isSet = true;
			_value = value;
			return *this;
		}

		/** Assignment operator from a null object
		@remarks
			The value is null
		*/
		CDatabaseNullable & operator =( CDatabaseNullableNoneType const & )
		{
			_isSet = false;
			return *this;
		}

		/** Bool operator, to test for value's null attribute
		@return
			true if the value is set
		*/
		operator bool ()
		{
			return _isSet;
		}

		/** Dereference operator, to retrieve the value
		@return
			The value
		*/
		T const & operator *()const
		{
			if ( !_isSet )
			{
				DB_EXCEPT( EDatabaseExceptionCodes_InternalError, ERROR_DB_VALUE_NOT_SET );
			}

			return _value;
		}

		/** Dereference operator, to retrieve the value
		@return
			The value
		*/
		T & operator *()
		{
			return _value;
		}

	private:
		//! Tells if the value is set or null
		bool _isSet;
		//! The value
		T _value;
	};

	//! The null value object
	static CDatabaseNullableNoneType None;
}
END_NAMESPACE_DATABASE

#endif
