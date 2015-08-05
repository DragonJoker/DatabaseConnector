/************************************************************************//**
* @file DatabaseValuedObjectInfos.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDatabaseValuedObjectInfos class declaration.
*
* @details Describes database valued object information.
*
***************************************************************************/

#ifndef ___DATABASE_FIELD_INFOS_H___
#define ___DATABASE_FIELD_INFOS_H___

#include "DatabasePrerequisites.h"

#include "EFieldType.h"

BEGIN_NAMESPACE_DATABASE
{
	/** Describes database valued object information.
	*/
	class CDatabaseValuedObjectInfos
	{
	public:
		/** Constructor.
		@param[in] connection
			Database connection.
		@param[in] name
			Valued object name.
			*/
		DatabaseExport CDatabaseValuedObjectInfos( const String & name );

		/** Constructor.
		@param[in] connection
			Database connection.
		@param[in] name
			Valued object name.
		@param[in] type
			Valued object type
			*/
		DatabaseExport CDatabaseValuedObjectInfos( const String & name, EFieldType type );

		/** Constructor.
		@param[in] connection
			Database connection.
		@param[in] name
			Valued object name.
		@param[in] type
			Valued object type
		@param[in] limits
			Valued object limits
			*/
		DatabaseExport CDatabaseValuedObjectInfos( const String & name, EFieldType type, uint32_t limits );

		/** Constructor.
		@param[in] connection
			Database connection.
		@param[in] name
			Valued object name.
		@param[in] type
			Valued object type
		@param[in] precision
			Valued object precision
			*/
		DatabaseExport CDatabaseValuedObjectInfos( const String & name, EFieldType type, const std::pair< uint32_t, uint32_t > & precision );

		/** Destructor.
		*/
		DatabaseExport virtual ~CDatabaseValuedObjectInfos();

		/** Get valued object type.
		@return
			Valued object type.
		*/
		DatabaseExport EFieldType GetType() const;

		/** Set valued object type.
		@param[in] type
			Valued object type
		*/
		DatabaseExport void SetType( EFieldType type );

		/** Set valued object type, for types needing a limit.
		@param[in] type
			Valued object type
		@param[in] limits
			Valued object limits
		*/
		DatabaseExport void SetType( EFieldType type, uint32_t limits );

		/** Set valued object type, for types needing a precision.
		@param[in] type
			Valued object type
		@param[in] precision
			Valued object precision
		*/
		DatabaseExport void SetType( EFieldType type, const std::pair< uint32_t, uint32_t > & precision );

		/** Get valued object name.
		@return
			Valued object name.
		*/
		DatabaseExport const String & GetName() const;

		/** Get valued object limits.
		@return
			Valued object limits.
		*/
		DatabaseExport const uint32_t & GetLimits() const;

		/** Get valued object limits.
		@return
			Valued object limits.
		*/
		DatabaseExport const std::pair< uint32_t, uint32_t > & GetPrecision() const;

	protected:
		//! Valued object name.
		String _name;
		//! Valued object type.
		EFieldType _type;
		//! Precision (floating and fixed points), _precision.first is also used as valued object limit (char, varchar, ...)
		std::pair< uint32_t, uint32_t > _precision;
	};
}
END_NAMESPACE_DATABASE

#endif //__DATABASE_FIELD_INFOS_H___
