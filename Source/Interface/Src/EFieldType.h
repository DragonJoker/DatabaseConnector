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

#include "DatabasePrerequisites.h"

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
		EFieldType_BLOB,
		EFieldType_COUNT
	}	EFieldType;

	/** Checks if the two given field types are compatible to set a value from typeFrom to typeTo
	@param typeFrom, typeTo
		The field types
	@return
		True if typeA and typeB are compatible
	*/
	DatabaseExport bool AreTypesCompatibleSet( EFieldType typeFrom, EFieldType typeTo );

	/** Checks if the two given field types are compatible to get a value from typeFrom to typeTo
	@param typeFrom, typeTo
		The field types
	@return
		True if typeA and typeB are compatible
	*/
	DatabaseExport bool AreTypesCompatibleGet( EFieldType typeFrom, EFieldType typeTo );

	/** Function used to tell if a field type needs limits
	@param[in] type
		The field type
	*/
	DatabaseExport bool NeedsLimits( EFieldType type );

	/** Function used to tell if a field type needs precision
	@param[in] type
		The field type
	*/
	DatabaseExport bool NeedsPrecision( EFieldType type );
}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_FIELD_TYPE_H___
