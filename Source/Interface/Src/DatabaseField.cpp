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
	CDatabaseField::CDatabaseField( DatabaseConnectionSPtr connection, DatabaseFieldInfosSPtr infos )
		: CDatabaseValuedObject( connection )
		, _infos( infos )
	{
		DoCreateValue();
	}

	CDatabaseField::~CDatabaseField()
	{
	}

	EFieldType CDatabaseField::GetType() const
	{
		return DoGetInfos()->GetType();
	}

	const String & CDatabaseField::GetName() const
	{
		return DoGetInfos()->GetName();
	}

	const uint32_t & CDatabaseField::GetLimits() const
	{
		return DoGetInfos()->GetLimits();
	}

	const std::pair< uint32_t, uint32_t > & CDatabaseField::GetPrecision() const
	{
		return DoGetInfos()->GetPrecision();
	}
}
END_NAMESPACE_DATABASE
