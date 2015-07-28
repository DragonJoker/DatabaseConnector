/************************************************************************//**
* @file DatabaseFileUtils.cpp
* @author Sylvain Doremus
* @version 1.0
* @date 7/28/2014 2:47:39 PM
*
*
* @brief File helper functions
*
***************************************************************************/

#ifndef ___DATABASE_FILE_UTILS_H___
#define ___DATABASE_FILE_UTILS_H___

#include "DatabasePrerequisites.h"

BEGIN_NAMESPACE_DATABASE
{
	namespace FileUtils
	{
		/** Opens a file
		*\param[out] p_pFile
			Receives the file descriptor
		*\param[in] p_pszPath
			The file path
		*\param[in] p_pszMode
			The opening mode
		*\return
			true on success
		*/
		DatabaseExport bool FOpen( FILE *& p_pFile, char const * p_pszPath, char const * p_pszMode );

		/** Checks the file existence
		@param name
			The file name
		@return true if the file exists
		*/
		DatabaseExport bool FileExists( const String & name );

		/** Checks the folder existence
		@param name
			The folder name
		@return true if the folder exists
		*/
		DatabaseExport bool FolderExists( String const & p_filename );

		/** Creates a folder
		@param name
			The folder name
		@return true if the folder was correctly created
		*/
		DatabaseExport bool CreateFolder( String const & pathFolder );
	}
}
END_NAMESPACE_DATABASE

#endif //___DATABASE_FILE_UTILS_H___
