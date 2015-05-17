/************************************************************************//**
 * @file ParameterType.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/17/2014 12:06:39 PM
 *
 *
 * @brief EParameterType enumeration declaration.
 *
 * @details EParameterType
 *
 ***************************************************************************/

#ifndef ___DATABASE_PARAMETER_TYPE_H___
#define ___DATABASE_PARAMETER_TYPE_H___

#include "DatabasePrerequisites.h"

BEGIN_NAMESPACE_DATABASE
{

	/// Enumeration of supported parameter types for statements.
	typedef enum EParameterType
	{
		EParameterType_IN,
		EParameterType_OUT,
		EParameterType_INOUT,
		EParameterType_COUNT
	} EParameterType;

}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_PARAMETER_TYPE_H___
