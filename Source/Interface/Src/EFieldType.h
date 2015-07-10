/************************************************************************//**
 * @file FieldType.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 3/17/2014 12:02:44 PM
 *
 *
 * @brief EFieldType enumeration declaration.
 *
 * @details EFieldType
 *
 ***************************************************************************/

#ifndef ___DATABASE_FIELD_TYPE_H___
#define ___DATABASE_FIELD_TYPE_H___

BEGIN_NAMESPACE_DATABASE
{
	/// Enumeration of supported field types.
	typedef enum EFieldType
	{
		EFieldType_NULL,
		EFieldType_BOOL,
		EFieldType_SMALL_INTEGER,
		EFieldType_INTEGER,
		EFieldType_LONG_INTEGER,
		EFieldType_FLOAT,
		EFieldType_DOUBLE,
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
	} EFieldType;
}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_FIELD_TYPE_H___
