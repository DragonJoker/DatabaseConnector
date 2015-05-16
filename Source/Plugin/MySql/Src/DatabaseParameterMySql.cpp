/************************************************************************//**
 * @file DatabaseParameterMySql.cpp
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/20/2014 2:47:39 PM
 * 
 *
 * @brief CDatabaseParameterMySql class declaration.
 *
 * @details Describes the base class for a statement or query parameter for MYSQL database.
 *
 ***************************************************************************/

#include "DatabaseMySqlPch.h"

#include "DatabaseParameterMySql.h"

#include <cppconn/parameter_metadata.h>
#include <cppconn/datatype.h>

BEGIN_NAMESPACE_DATABASE_MYSQL
{
    int CDatabaseParameterMySql::MySQLDataTypes[EFieldType_COUNT] =
    {
        0,                              ///< EFieldType_NULL
        sql::DataType::BIT,             ///< EFieldType_BOOL
        sql::DataType::SMALLINT,        ///< EFieldType_SMALL_INTEGER
        sql::DataType::INTEGER,         ///< EFieldType_INTEGER
        sql::DataType::BIGINT,          ///< EFieldType_LONG_INTEGER
        sql::DataType::REAL,            ///< EFieldType_FLOAT
        sql::DataType::DOUBLE,          ///< EFieldType_DOUBLE
        sql::DataType::VARCHAR,         ///< EFieldType_VARCHAR
        sql::DataType::LONGVARCHAR,     ///< EFieldType_TEXT
        sql::DataType::VARCHAR,         ///< EFieldType_NVARCHAR
        sql::DataType::LONGVARCHAR,     ///< EFieldType_NTEXT
        sql::DataType::DATE,            ///< EFieldType_DATE
        sql::DataType::TIMESTAMP,       ///< EFieldType_DATETIME
        sql::DataType::TIME,            ///< EFieldType_TIME
        sql::DataType::BINARY,          ///< EFieldType_BINARY
        sql::DataType::VARBINARY,       ///< EFieldType_VARBINARY
        sql::DataType::LONGVARBINARY,   ///< EFieldType_LONG_VARBINARY
    };
    
    int CDatabaseParameterMySql::MySqlParameterTypes[EParameterType_COUNT] =
    {
        sql::ParameterMetaData::parameterModeIn,
        sql::ParameterMetaData::parameterModeOut,
        sql::ParameterMetaData::parameterModeInOut
    };
    
    CDatabaseParameterMySql::CDatabaseParameterMySql( EFieldType fieldType, EParameterType parameterType )
        : _dataType( MySQLDataTypes[fieldType] )
        , _parameterType( MySqlParameterTypes[parameterType] )
    {
    }
    
    CDatabaseParameterMySql::~CDatabaseParameterMySql()
    {
        // Empty
    }
    
    const int & CDatabaseParameterMySql::GetDataType() const
    {
        return _dataType;
    }
    
    const int & CDatabaseParameterMySql::GetParameterType() const
    {
        return _parameterType;
    }
}
END_NAMESPACE_DATABASE_MYSQL
