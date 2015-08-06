/************************************************************************//**
* @file DatabaseParameteredObject.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDatabaseParameteredObject class declaration.
*
* @details Describes the base class for queries and statements.
*
***************************************************************************/

#include "DatabasePch.h"

#include "DatabaseParameteredObject.h"

#include "DatabaseException.h"
#include "DatabaseParameter.h"

#include <algorithm>

BEGIN_NAMESPACE_DATABASE
{
	static const String ERROR_DB_PARAMETER_INDEX = STR( "No query parameter at index: " );
	static const String ERROR_DB_PARAMETER_NAME = STR( "No query parameter named: " );
	static const String ERROR_DB_PARAMETER_ALREADY_ADDED = STR( "Parameter with name [%1%] already exists." );
	static const String ERROR_DB_PARAMETER_INITIALISATION = STR( "Parametered object initialisation failed: " );
	static const String ERROR_DB_PARAMETER_CLEANUP = STR( "Statement cleanup failed: " );

	static const String WARNING_DB_PARAMETER_NULL = STR( "Trying to add a null parameter." );

	CDatabaseParameteredObject::CDatabaseParameteredObject()
		: _initialised( false )
	{
		// Empty
	}

	CDatabaseParameteredObject::~CDatabaseParameteredObject()
	{
		_parameters.clear();
	}

	EErrorType CDatabaseParameteredObject::Initialise()
	{
		EErrorType ret = EErrorType_ERROR;

		try
		{
			ret = DoInitialise();
			_initialised = true;
		}
		COMMON_CATCH( ERROR_DB_PARAMETER_INITIALISATION )

		return ret;
	}

	void CDatabaseParameteredObject::Cleanup()
	{
		try
		{
			DoCleanup();
		}
		COMMON_CATCH( ERROR_DB_PARAMETER_CLEANUP )
	}

	DatabaseParameterSPtr CDatabaseParameteredObject::GetParameter( uint32_t index )const
	{
		if ( index >= _parameters.size() )
		{
			StringStream message;
			message << ERROR_DB_PARAMETER_INDEX << index;
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, message.str() );
		}

		return _parameters[index];
	}

	DatabaseParameterSPtr CDatabaseParameteredObject::GetParameter( const String & name )const
	{
		auto it = std::find_if( _parameters.begin(), _parameters.end(), [&name]( DatabaseParameterSPtr parameter )
		{
			return parameter->GetName() == name;
		} );

		if ( it == _parameters.end() )
		{
			StringStream message;
			message << ERROR_DB_PARAMETER_NAME << name;
			DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, message.str() );
		}

		return ( *it );
	}

	EFieldType CDatabaseParameteredObject::GetParameterType( uint32_t index )
	{
		return GetParameter( index )->GetType();
	}

	EFieldType CDatabaseParameteredObject::GetParameterType( const String & name )
	{
		return GetParameter( name )->GetType();
	}

	void CDatabaseParameteredObject::SetParameterNull( uint32_t index )
	{
		GetParameter( index )->SetNull();
	}

	void CDatabaseParameteredObject::SetParameterNull( const String & name )
	{
		GetParameter( name )->SetNull();
	}

	void CDatabaseParameteredObject::SetParameterValue( uint32_t index, const CDatabaseValuedObject & object )
	{
		GetParameter( index )->SetValue( object );
	}

	void CDatabaseParameteredObject::SetParameterValue( const String & name, const CDatabaseValuedObject & object )
	{
		GetParameter( name )->SetValue( object );
	}

	DatabaseParameterSPtr CDatabaseParameteredObject::DoAddParameter( DatabaseParameterSPtr parameter )
	{
		DatabaseParameterSPtr ret;

		if ( parameter )
		{
			auto it = std::find_if( _parameters.begin(), _parameters.end(), [&parameter]( DatabaseParameterSPtr param )
			{
				return param->GetName() == parameter->GetName();
			} );

			if ( it == _parameters.end() )
			{
				_parameters.push_back( parameter );
				ret = parameter;
			}
			else
			{
				Format format( ERROR_DB_PARAMETER_ALREADY_ADDED );
				format % parameter->GetName();
				DB_EXCEPT( EDatabaseExceptionCodes_ParameterError, format.str() );
			}
		}
		else
		{
			CLogger::LogWarning( WARNING_DB_PARAMETER_NULL );
		}

		return ret;
	}
}
END_NAMESPACE_DATABASE
