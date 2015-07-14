/************************************************************************//**
* @file ELogType.h
* @author Sylvain Doremus
* @version 1.0
* @date 7/12/2015 7:51 PM
*
*
* @brief ELogType enumerator
*
* @details Defines the various log types
*
***************************************************************************/

#ifndef ___DATABASE_LOG_TYPE_H___
#define ___DATABASE_LOG_TYPE_H___

#include "DatabasePrerequisites.h"

BEGIN_NAMESPACE_DATABASE
{
	/** Defines the various log types
	*/
	typedef enum ELogType
		: uint8_t
	{
		ELogType_DEBUG,		//!< Debug type log
		ELogType_INFO,		//!< Info type log
		ELogType_WARNING,	//!< Warning type log
		ELogType_ERROR,		//!< Error type log
		ELogType_COUNT,		//!< Number of log types
	}	ELogType;
}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_LOG_TYPE_H___
