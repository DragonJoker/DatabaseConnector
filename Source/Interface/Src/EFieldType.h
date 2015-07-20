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
		EFieldType_SINT8,
		EFieldType_SINT16,
		EFieldType_SINT24,
		EFieldType_SINT32,
		EFieldType_SINT64,
		EFieldType_UINT8,
		EFieldType_UINT16,
		EFieldType_UINT24,
		EFieldType_UINT32,
		EFieldType_UINT64,
		EFieldType_FLOAT32,
		EFieldType_FLOAT64,
		EFieldType_FIXED_POINT,
		EFieldType_CHAR,
		EFieldType_VARCHAR,
		EFieldType_TEXT,
		EFieldType_NCHAR,
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
