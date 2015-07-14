/************************************************************************//**
* @file FieldType.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/17/2014 12:02:44 PM
*
*
* @brief EFieldType enumeration declaration.
*
* @details Enumeration of supported field types.
*
***************************************************************************/

#ifndef ___DATABASE_FIELD_TYPE_H___
#define ___DATABASE_FIELD_TYPE_H___

BEGIN_NAMESPACE_DATABASE
{
	/** Enumeration of supported field types.
	*/
	typedef enum EFieldType
	{
		EFieldType_NULL,
		EFieldType_BIT,
		EFieldType_INT8,
		EFieldType_INT16,
		EFieldType_INT24,
		EFieldType_INT32,
		EFieldType_INT64,
		EFieldType_FLOAT32,
		EFieldType_FLOAT64,
		EFieldType_FIXED_POINT,
		EFieldType_VARCHAR,
		EFieldType_TEXT,
		EFieldType_NVARCHAR,
		EFieldType_NTEXT,
		EFieldType_DATE,
		EFieldType_DATETIME,
		EFieldType_TIME,
		EFieldType_BINARY,
		EFieldType_VARBINARY,
		EFieldType_LONG_VARBINARY,
		EFieldType_COUNT
	}	EFieldType;
}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_FIELD_TYPE_H___
