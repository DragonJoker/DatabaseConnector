/************************************************************************//**
 * @file DatabaseSqlitePch.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 03/14/2014 11:00:50 AM
 * 
 *
 * @brief Database SQLITE precompiled header.
 *
 * @details This file generates the Database SQLITE precompiled headers.
 *
 ***************************************************************************/

#include "DatabaseSqlitePch.h"

BEGIN_NAMESPACE_DATABASE_SQLITE
{
    DatabaseFieldInfosPtrArray SqliteGetColumns( SQLite::Statement * pStatement, DatabaseConnectionPtr pConnexion )
    {
        DatabaseFieldInfosPtrArray arrayReturn;
        StringArray aColumns;
        String strColumnName;
        char const * pszName;
        char const * pszType;
        int iColumnCount = SQLite::ColumnCount( pStatement );
        
        for( int i = 0 ; i < iColumnCount ; i++ )
        {
            pszName = SQLite::ColumnName( pStatement, i );
            
            if( pszName )
            {
                strColumnName = CStrUtils::ToString( pszName );
                aColumns.push_back( strColumnName );
                strColumnName = CStrUtils::LowerCase( strColumnName );
                pszType = SQLite::ColumnDecltype( pStatement, i );
                
                if( pszType )
                {
                    arrayReturn.push_back( std::make_shared< CDatabaseFieldInfos >( pConnexion, aColumns[i], CStrUtils::ToString( pszType ) ) );
                }
                else if( strColumnName.find( _T( "max(" ) ) != String::npos )
                {
                    arrayReturn.push_back( std::make_shared< CDatabaseFieldInfos >( pConnexion, aColumns[i], _T( "max" ) ) );
                }
                else if( strColumnName.find( _T( "min(" ) ) != String::npos )
                {
                    arrayReturn.push_back( std::make_shared< CDatabaseFieldInfos >( pConnexion, aColumns[i], _T( "min" ) ) );
                }
                else if( strColumnName.find( _T( "count(" ) ) != String::npos )
                {
                    arrayReturn.push_back( std::make_shared< CDatabaseFieldInfos >( pConnexion, aColumns[i], _T( "count" ) ) );
                }
                else if( strColumnName.find( _T( "sum(" ) ) != String::npos )
                {
                    arrayReturn.push_back( std::make_shared< CDatabaseFieldInfos >( pConnexion, aColumns[i], _T( "sum" ) ) );
                }
                else if( strColumnName.find( _T( "calculfloat" ) ) != String::npos )
                {
                    arrayReturn.push_back( std::make_shared< CDatabaseFieldInfos >( pConnexion, aColumns[i], _T( "FLOAT" ) ) );
                }
                else if( strColumnName.find( _T( "calculint" ) ) != String::npos )
                {
                    arrayReturn.push_back( std::make_shared< CDatabaseFieldInfos >( pConnexion, aColumns[i], _T( "INTEGER" ) ) );
                }
                else if( strColumnName.find( _T( "intitule" ) ) != String::npos )
                {
                    arrayReturn.push_back( std::make_shared< CDatabaseFieldInfos >( pConnexion, aColumns[i], _T( "TEXT" ) ) );
                }
                else
                {
                    arrayReturn.push_back( std::make_shared< CDatabaseFieldInfos >( pConnexion, aColumns[i], _T( "TEXT" ) ) );
                }
            }
        }
        
        return arrayReturn;
    }
    
    DatabaseResultPtr SqliteExecute( SQLite::Statement * pStatement, EErrorType & eResult, DatabaseFieldInfosPtrArray const & arrayColumns, DatabaseConnectionPtr pConnexion )
    {
        DatabaseResultPtr pReturn;
        eResult = EErrorType_ERROR;
        
        if( pConnexion->IsConnected() )
        {
            pReturn = std::make_unique< CDatabaseResult >( pConnexion, arrayColumns );
            eResult = EErrorType_NONE;
            
            bool bNull = false;
            DatabaseFieldInfosPtr pInfos;
            DatabaseRowPtr pRow;
            DatabaseFieldPtr pField;
            char * szValue;
            unsigned char * pbyValue;
            wchar_t * wszValue;
            int iValue;
            int64_t i64Value;
            float fValue;
            double dValue;
            int iSize;
            int iResult;
            int iNbColumns = int( arrayColumns.size() );
            
            while(( iResult = SQLite::Step( pStatement ) ) != SQLITE_DONE )
            {
                if( iResult == SQLITE_ROW )
                {
                    pRow = std::make_shared< CDatabaseRow >( pConnexion );
                    
                    for( int i = 0 ; i < iNbColumns ; i++ )
                    {
                        try
                        {
                        	pInfos = pReturn->GetFieldInfos( i );
                        }
                        catch ( CExceptionDatabase & )
                        {
                        	throw;
                        }
                        
                        switch( pInfos->GetType() )
                        {
                        case EFieldType_BOOL:
                            iValue = SQLite::ColumnInt( pStatement, i );
                            iSize = SQLite::ColumnBytes( pStatement, i );
                            if( iSize == 0 )
                            {
                                pField = std::make_shared< CDatabaseField >( pInfos, true, _T( "" ) );
                            }
                            else
                            {
                                pField = std::make_shared< CDatabaseField >( pInfos, bool( iValue != 0 ) );
                            }
                            break;
                            
                        case EFieldType_INTEGER:
                            iValue = SQLite::ColumnInt( pStatement, i );
                            iSize = SQLite::ColumnBytes( pStatement, i );
                            if( iSize == 0 )
                            {
                                pField = std::make_shared< CDatabaseField >( pInfos, true, _T( "" ) );
                            }
                            else
                            {
                                pField = std::make_shared< CDatabaseField >( pInfos, iValue );
                            }
                            break;
                            
                        case EFieldType_LONG_INTEGER:
                            i64Value = SQLite::ColumnInt64( pStatement, i );
                            iSize = SQLite::ColumnBytes( pStatement, i );
                            if( iSize == 0 )
                            {
                                pField = std::make_shared< CDatabaseField >( pInfos, true, _T( "" ) );
                            }
                            else
                            {
                                pField = std::make_shared< CDatabaseField >( pInfos, i64Value );
                            }
                            break;
                            
                        case EFieldType_FLOAT:
                            fValue = float( SQLite::ColumnDouble( pStatement, i ) );
                            iSize = SQLite::ColumnBytes( pStatement, i );
                            if( iSize == 0 )
                            {
                                pField = std::make_shared< CDatabaseField >( pInfos, true, _T( "" ) );
                            }
                            else
                            {
                                pField = std::make_shared< CDatabaseField >( pInfos, fValue );
                            }
                            break;
                            
                        case EFieldType_DOUBLE:
                            dValue = SQLite::ColumnDouble( pStatement, i );
                            iSize = SQLite::ColumnBytes( pStatement, i );
                            if( iSize == 0 )
                            {
                                pField = std::make_shared< CDatabaseField >( pInfos, true, _T( "" ) );
                            }
                            else
                            {
                                pField = std::make_shared< CDatabaseField >( pInfos, dValue );
                            }
                            break;
                            
                        case EFieldType_VARCHAR:
                            szValue = (char *)SQLite::ColumnText( pStatement, i );
                            iSize = SQLite::ColumnBytes( pStatement, i );
                            bNull = szValue ? false : true;
                            if( bNull )
                            {
                                pField = std::make_shared< CDatabaseField >( pInfos, true, _T( "" ) );
                            }
                            else
                            {
                                pField = std::make_shared< CDatabaseField >( pInfos, szValue );
                            }
                            break;
                            
                        case EFieldType_TEXT:
                            szValue = (char *)SQLite::ColumnText( pStatement, i );
                            iSize = SQLite::ColumnBytes( pStatement, i );
                            bNull = szValue ? false : true;
                            if( bNull )
                            {
                                pField = std::make_shared< CDatabaseField >( pInfos, true, _T( "" ) );
                            }
                            else
                            {
                                pField = std::make_shared< CDatabaseField >( pInfos, std::string( szValue ) );
                            }
                            break;
                            
                        case EFieldType_NVARCHAR:
                            wszValue = (wchar_t *)SQLite::ColumnText16( pStatement, i );
                            iSize = SQLite::ColumnBytes16( pStatement, i );
                            bNull = wszValue ? false : true;
                            if( bNull )
                            {
                                pField = std::make_shared< CDatabaseField >( pInfos, true, _T( "" ) );
                            }
                            else
                            {
                                pField = std::make_shared< CDatabaseField >( pInfos, wszValue );
                            }
                            break;
                            
                        case EFieldType_NTEXT:
                            wszValue = (wchar_t *)SQLite::ColumnText16( pStatement, i );
                            iSize = SQLite::ColumnBytes16( pStatement, i );
                            bNull = wszValue ? false : true;
                            if( bNull )
                            {
                                pField = std::make_shared< CDatabaseField >( pInfos, true, _T( "" ) );
                            }
                            else
                            {
                                pField = std::make_shared< CDatabaseField >( pInfos, std::wstring( wszValue ) );
                            }
                            break;
                            
                        case EFieldType_BINARY:
                        case EFieldType_VARBINARY:
                        case EFieldType_LONG_VARBINARY:
                            pbyValue = (unsigned char *)SQLite::ColumnBlob( pStatement, i );
                            iSize = SQLite::ColumnBytes( pStatement, i );
                            bNull = pbyValue ? false : true;
                            if( bNull || iSize == 0 )
                            {
                                pField = std::make_shared< CDatabaseField >( pInfos, true, _T( "" ) );
                            }
                            else
                            {
                                pField = std::make_shared< CDatabaseField >( pInfos, pbyValue, iSize );
                            }
                            break;
                            
                        default:
                            szValue = (char *)SQLite::ColumnText( pStatement, i );
                            iSize = SQLite::ColumnBytes( pStatement, i );
                            bNull = szValue ? false : true;
                            if( bNull || iSize == 0 )
                            {
                                pField = std::make_shared< CDatabaseField >( pInfos, true, _T( "" ) );
                            }
                            else
                            {
                                pField = std::make_shared< CDatabaseField >( pInfos, bNull, CStrUtils::ToString( szValue ) );
                            }
                            break;
                        }
                        
                        pRow->AddField( pField );
                    }
                    
                    pReturn->AddRow( pRow );
                }
            }
        }
        
        return pReturn;
    }
}
END_NAMESPACE_DATABASE_SQLITE
