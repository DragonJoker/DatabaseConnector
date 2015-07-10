/************************************************************************//**
 * @file ErrorType.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/17/2014 12:09:52 PM
 *
 *
 * @brief EErrorType enumeration declaration.
 *
 * @details EErrorType
 *
 ***************************************************************************/

#ifndef ___DATABASE_ERROR_TYPE_H___
#define ___DATABASE_ERROR_TYPE_H___

#include "DatabasePrerequisites.h"

BEGIN_NAMESPACE_DATABASE
{
	/// Enumeration of supported error types.
	typedef enum EErrorType
	{
		EErrorType_NONE,        ///< No error.
		EErrorType_RETRY,       ///< Retry current action.
		EErrorType_ERROR,       ///< True error.
		EErrorType_RECONNECT    ///< Reconnection request.
	}	EErrorType;
}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_ERROR_TYPE_H___
