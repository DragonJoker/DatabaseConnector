/************************************************************************//**
 * @file DatabaseFieldValueInitializer.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 11/08/2014 14:24:28
 *
 *
 * @brief [your brief comment here]
 *
 * @details [your detailled comment here]
 *
 ***************************************************************************/

#ifndef ___DATABASE_FIELD_VALUE_INITIALIZER_H___
#define ___DATABASE_FIELD_VALUE_INITIALIZER_H___

#include "DatabaseFieldInfos.h"
#include "DatabaseValue.h"

BEGIN_NAMESPACE_DATABASE
{
	/** Functor for setting the value of generic type.
	*/
	template <typename T> struct SDatabaseFieldValueInitializer
	{
		typedef T value_type;

		/** Operator () for functor.
		@param[in] value
		    Field value.
		@param[in] fieldInfos
		    Field information.
		*/
		CDatabaseValueBase * operator()( const value_type & value, CDatabaseFieldInfos * fieldInfos )
		{
			CDatabaseValue< SDataTypeFieldTyper< value_type >::Value > * pReturn = new CDatabaseValue< SDataTypeFieldTyper< value_type >::Value >( fieldInfos->GetConnection() );
			pReturn->SetValue( value );
			return pReturn;
		}

		/** Operator () for functor.
		@param[in] value
		    Field value.
		@param[in] fieldInfos
		    Field information.
		*/
		CDatabaseValueBase * operator()( const value_type & value, DatabaseFieldInfosPtr fieldInfos )
		{
			return operator()( value, fieldInfos.get() );
		}
	};

	/** Functor for setting the value of generic type.
	*/
	template <> struct SDatabaseFieldValueInitializer< std::string >
	{
		typedef std::string value_type;

		/** Operator () for functor.
		@param[in] value
		    Field value.
		@param[in] fieldInfos
		    Field information.
		*/
		CDatabaseValueBase * operator()( const value_type & value, CDatabaseFieldInfos * fieldInfos )
		{
			CDatabaseValueBase * ret = NULL;

			switch ( fieldInfos->GetType() )
			{
			case EFieldType_DATE:
			{
				CDatabaseValue< EFieldType_DATE > * pReturn = new CDatabaseValue< EFieldType_DATE >( fieldInfos->GetConnection() );
				pReturn->SetValue( value );
				ret = pReturn;
			}
			break;

			case EFieldType_TIME:
			{
				CDatabaseValue< EFieldType_TIME > * pReturn = new CDatabaseValue< EFieldType_TIME >( fieldInfos->GetConnection() );
				pReturn->SetValue( value );
				ret = pReturn;
			}
			break;

			case EFieldType_DATETIME:
			{
				CDatabaseValue< EFieldType_DATETIME > * pReturn = new CDatabaseValue< EFieldType_DATETIME >( fieldInfos->GetConnection() );
				pReturn->SetValue( value );
				ret = pReturn;
			}
			break;

			case EFieldType_VARCHAR:
			{
				CDatabaseValue< EFieldType_VARCHAR > * pReturn = new CDatabaseValue< EFieldType_VARCHAR >( fieldInfos->GetConnection() );
				pReturn->SetValue( value.c_str(), uint32_t( value.size() ) );
				ret = pReturn;
			}
			break;

			case EFieldType_TEXT:
			{
				CDatabaseValue< EFieldType_TEXT > * pReturn = new CDatabaseValue< EFieldType_TEXT >( fieldInfos->GetConnection() );
				pReturn->SetValue( value );
				ret = pReturn;
			}
			break;
			}

			return ret;
		}

		/** Operator () for functor.
		@param[in] value
		    Field value.
		@param[in] fieldInfos
		    Field information.
		*/
		CDatabaseValueBase * operator()( const value_type & value, DatabaseFieldInfosPtr fieldInfos )
		{
			return operator()( value, fieldInfos.get() );
		}
	};

	/** Functor for setting the value of char* type.
	*/
	template<> struct SDatabaseFieldValueInitializer< char * >
	{
		typedef char * value_type;

		/** Operator () for functor.
		@param[in] value
		    Field value.
		@param[in] fieldInfos
		    Field information.
		*/
		CDatabaseValueBase * operator()( const value_type & value, CDatabaseFieldInfos * fieldInfos )
		{
			CDatabaseValue< SDataTypeFieldTyper< value_type >::Value > * pReturn = new CDatabaseValue< SDataTypeFieldTyper< value_type >::Value >( fieldInfos->GetConnection() );
			pReturn->SetValue( value, uint32_t( strlen( value ) ) );
			return pReturn;
		}

		/** Operator () for functor.
		@param[in] value
		    Field value.
		@param[in] fieldInfos
		    Field information.
		*/
		CDatabaseValueBase * operator()( const value_type & value, DatabaseFieldInfosPtr fieldInfos )
		{
			return operator()( value, fieldInfos.get() );
		}
	};

	/** Functor for setting the value of wchar_t* type.
	*/
	template<> struct SDatabaseFieldValueInitializer< wchar_t * >
	{
		typedef wchar_t * value_type;

		/** Operator () for functor.
		@param[in] value
		    Field value.
		@param[in] fieldInfos
		    Field information.
		*/
		CDatabaseValueBase * operator()( const value_type & value, CDatabaseFieldInfos * fieldInfos )
		{
			CDatabaseValue< SDataTypeFieldTyper< value_type >::Value > * pReturn = new CDatabaseValue< SDataTypeFieldTyper< value_type >::Value >( fieldInfos->GetConnection() );
			pReturn->SetValue( value, uint32_t( wcslen( value ) ) );
			return pReturn;
		}

		/** Operator () for functor.
		@param[in] value
		    Field value.
		@param[in] fieldInfos
		    Field information.
		*/
		CDatabaseValueBase * operator()( const value_type & value, DatabaseFieldInfosPtr fieldInfos )
		{
			return operator()( value, fieldInfos.get() );
		}
	};

	/** Functor for setting the value of byte type.
	*/
	template<> struct SDatabaseFieldValueInitializer< std::vector< uint8_t > >
	{
		typedef std::vector< uint8_t > value_type;

		/** Operator () for functor.
		@param[in] value
		    Field value.
		@param[in] fieldInfos
		    Field information.
		*/
		CDatabaseValueBase * operator()( const value_type & value, CDatabaseFieldInfos * fieldInfos )
		{
			CDatabaseValue< EFieldType_VARBINARY > * pReturn = new CDatabaseValue< EFieldType_VARBINARY >( fieldInfos->GetConnection() );
			pReturn->SetValue( value.data(), uint32_t( value.size() ) );
			return pReturn;
		}

		/** Operator () for functor.
		@param[in] value
		    Field value.
		@param[in] fieldInfos
		    Field information.
		*/
		CDatabaseValueBase * operator()( const value_type & value, DatabaseFieldInfosPtr fieldInfos )
		{
			return operator()( value, fieldInfos.get() );
		}
	};

	/** Functor for setting the value of byte type.
	*/
	template<> struct SDatabaseFieldValueInitializer< std::istream * >
	{
		typedef std::istream * value_type;

		/** Operator () for functor.
		@param[in] value
		    Field value.
		@param[in] fieldInfos
		    Field information.
		*/
		CDatabaseValueBase * operator()( const value_type & value, CDatabaseFieldInfos * fieldInfos )
		{
			CDatabaseValue< EFieldType_VARBINARY > * pReturn = new CDatabaseValue< EFieldType_VARBINARY >( fieldInfos->GetConnection() );
			std::vector< uint8_t > out;
			std::copy(
				std::istream_iterator< uint8_t >( *value ),
				std::istream_iterator< uint8_t >(),
				std::back_inserter( out ) );
			pReturn->SetValue( out.data(), uint32_t( out.size() ) );
			return pReturn;
		}

		/** Operator () for functor.
		@param[in] value
		    Field value.
		@param[in] fieldInfos
		    Field information.
		*/
		CDatabaseValueBase * operator()( const value_type & value, DatabaseFieldInfosPtr fieldInfos )
		{
			return operator()( value, fieldInfos.get() );
		}
	};
}
END_NAMESPACE_DATABASE

#endif //___DATABASE_FIELD_VALUE_INITIALIZER_H___
