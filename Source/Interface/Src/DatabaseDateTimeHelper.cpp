/************************************************************************//**
 * @file DateTimeHelper.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/19/2014 3:03:29 PM
 * 
 *
 * @brief Helping functions for date/time management.
 *
 * @details Helping functions for date/time management.
 *
 ***************************************************************************/

#include "DatabasePch.h"

#include "DatabaseDateTimeHelper.h"

#include "Database.h"
#include "DatabaseException.h"
#include "DatabaseLogger.h"

#include <cstdarg>

#pragma warning( push )
#pragma warning( disable:4996 )

BEGIN_NAMESPACE_DATABASE
{
    static const String DATETIME_FORMALIZE_ERROR = STR( "Error while formatting: " );
    
    bool IsLeap( int year )
    {
        return ( year % 4 == 0 ) && ( year % 100 != 0 || year % 400 == 0 );
    }
    
    bool IsInteger( const std::string & value )
    {
        bool bReturn = true;
        
        if ( !value.empty() )
        {
            bReturn = ( value[0] >= '0' && value[0] <= '9' ) || value[0] == '-';
            
            for ( std::size_t i = 1 ; i < value.size() && bReturn ; i++ )
            {
                bReturn = value[i] >= '0' && value[i] <= '9';
            }
        }
        
        return bReturn;
    }
    
    int Replace( std::string & originalString, const std::string & searchedSubString, const std::string & replacementSubString )
    {
        int iReturn = 0;
        std::size_t uiIndex = 0;
        std::size_t uiPrev = 0;
        std::string strReplaced;
        
        if ( !originalString.empty() )
        {
            uiIndex = originalString.find( searchedSubString );
            
            if ( uiIndex == std::string::npos )
            {
                strReplaced = originalString;
            }
            else
            {
                while ( uiIndex != std::string::npos )
                {
                    strReplaced += originalString.substr( uiPrev, uiIndex - uiPrev ) + replacementSubString;
                    uiPrev = uiIndex + searchedSubString.size();
                    uiIndex = originalString.find( searchedSubString, uiPrev );
                }
                
                strReplaced += originalString.substr( uiPrev );
            }
            
            originalString = strReplaced;
        }
        
        return iReturn;
    }
    
    void Formalize( std::string & formattedString, int maxSize, const char * format, ... )
    {
        formattedString.clear();
        
        std::string strText( maxSize, 0 );
        
        try
        {
            va_list vaList;
            
            if ( format != NULL )
            {
                va_start( vaList, format );
                vsnprintf( ( char * )strText.data(), maxSize, format, vaList );
                va_end( vaList );
                formattedString = strText;
            }
        }
        catch ( ... )
        {
            StringStream message;
            message << DATETIME_FORMALIZE_ERROR << formattedString.c_str();
            CLogger::LogError( message.str() );
            DB_EXCEPT( EDatabaseExceptionCodes_DateTimeError, message.str() );
        }
    }
    
    bool IsInteger( const std::wstring & value )
    {
        bool bReturn = true;
        
        if ( !value.empty() )
        {
            bReturn = ( value[0] >= L'0' && value[0] <= L'9' ) || value[0] == L'-';
            
            for ( std::size_t i = 1 ; i < value.size() && bReturn ; i++ )
            {
                bReturn = value[i] >= L'0' && value[i] <= L'9';
            }
        }
        
        return bReturn;
    }
    
    int Replace( std::wstring & originalString, const std::wstring & searchedSubString, const std::wstring & replacementSubString )
    {
        int iReturn = 0;
        std::size_t uiIndex = 0;
        std::size_t uiPrev = 0;
        std::wstring strReplaced;
        
        if ( !originalString.empty() )
        {
            uiIndex = originalString.find( searchedSubString );
            
            if ( uiIndex == std::wstring::npos )
            {
                strReplaced = originalString;
            }
            else
            {
                while ( uiIndex != std::wstring::npos )
                {
                    strReplaced += originalString.substr( uiPrev, uiIndex - uiPrev ) + replacementSubString;
                    uiPrev = uiIndex + searchedSubString.size();
                    uiIndex = originalString.find( searchedSubString, uiPrev );
                }
                
                strReplaced += originalString.substr( uiPrev );
            }
            
            originalString = strReplaced;
        }
        
        return iReturn;
    }
    
    void Formalize( std::wstring & formattedString, int maxSize, const wchar_t * format, ... )
    {
        formattedString.clear();
        
        String strText( maxSize, 0 );
        
        try
        {
            va_list vaList;
            
            if ( format != NULL )
            {
                va_start( vaList, format );
                vswprintf( ( wchar_t * )strText.data(), maxSize, format, vaList );
                va_end( vaList );
                formattedString = strText;
            }
        }
        catch ( ... )
        {
            StringStream message;
            message << DATETIME_FORMALIZE_ERROR << formattedString.c_str();
            CLogger::LogError( message.str() );
            DB_EXCEPT( EDatabaseExceptionCodes_DateTimeError, message.str() );
        }
    }
}
END_NAMESPACE_DATABASE

#pragma warning( pop )
