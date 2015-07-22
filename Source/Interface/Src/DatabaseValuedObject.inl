/************************************************************************//**
* @file DatabaseValuedObject.inl
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDatabaseValuedObject class declaration.
*
* @details Describes a an object (field or parameter) with a value.
*
***************************************************************************/

#include "DatabasePrerequisites.h" // Help doxygen

BEGIN_NAMESPACE_DATABASE
{
	inline CDatabaseValueBase const & CDatabaseValuedObject::GetObjectValue() const
	{
		return *_value;
	}

	inline CDatabaseValueBase & CDatabaseValuedObject::GetObjectValue()
	{
		return *_value;
	}

	inline DatabaseConnectionSPtr CDatabaseValuedObject::GetConnection() const
	{
		return _connection.lock();
	}
}
END_NAMESPACE_DATABASE
