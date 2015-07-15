/************************************************************************//**
* @file DatabasePch.h
* @author Sylvain Doremus
* @version 1.0
* @date 03/14/2013 09:21:05 AM
*
*
* @brief Database precompiled header
*
* @details This file generates the Database precompiled headers
*
***************************************************************************/

#ifndef ___DATABASE_PCH_H___
#define ___DATABASE_PCH_H___

#include <string>
#include <vector>
#include <map>
#include <list>
#include <thread>
#include <mutex>
#include <memory>
#include <iostream>
#include <sstream>
#include <locale>

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdarg>

#include <wchar.h>
#include <sys/stat.h>

#if defined( _WIN32 )
#	include <Windows.h>
#	undef min
#	undef max
#	undef abs
#endif

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/locale.hpp>
#include <boost/format.hpp>

#endif // ___DATABASE_PCH_H___
