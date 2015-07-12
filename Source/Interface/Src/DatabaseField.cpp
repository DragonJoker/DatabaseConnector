/************************************************************************//**
 * @file DatabaseField.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 *
 *
 * @brief CDatabaseField class definition.
 *
 * @details Describes a database field.
 *
 ***************************************************************************/

#include "DatabasePch.h"

#include "DatabaseField.h"

#include "Database.h"
#include "DatabaseConnection.h"
#include "DatabaseField.h"
#include "DatabaseFieldInfos.h"
#include "DatabaseValue.h"
#include "DatabaseException.h"
#include "EDateMonth.h"

BEGIN_NAMESPACE_DATABASE
{
	static const String DATABASE_FIELD_CREATION_TYPE_ERROR = STR( "Type error while creating the field : " );
	static const String DATABASE_FIELD_GETVALUE_TYPE_ERROR = STR( "Type error while getting value from the field : " );

	CDatabaseField::CDatabaseField( DatabaseFieldInfosPtr infos )
		: CDatabaseValuedObject( infos->GetConnection() )
		, _infos( infos )
	{
		DoCreateValue();
	}

	CDatabaseField::~CDatabaseField()
	{
	}

	EFieldType CDatabaseField::GetType() const
	{
		return _infos->GetType();
	}

	const String & CDatabaseField::GetName() const
	{
		return _infos->GetName();
	}

	const uint32_t & CDatabaseField::GetLimits() const
	{
		return _infos->GetLimits();
	}

	const std::pair< uint32_t, uint32_t > & CDatabaseField::GetPrecision() const
	{
		return _infos->GetPrecision();
	}
}
END_NAMESPACE_DATABASE
