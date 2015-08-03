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
#include "DatabaseValuedObjectInfos.h"
#include "DatabaseValue.h"
#include "DatabaseException.h"

BEGIN_NAMESPACE_DATABASE
{
	CDatabaseField::CDatabaseField( DatabaseConnectionSPtr connection, DatabaseValuedObjectInfosSPtr infos )
		: CDatabaseValuedObject( connection, infos )
	{
		DoCreateValue();
	}

	CDatabaseField::~CDatabaseField()
	{
	}
}
END_NAMESPACE_DATABASE
