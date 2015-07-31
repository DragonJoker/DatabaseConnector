/************************************************************************//**
* @file DatabaseFieldInfos.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief CDatabaseFieldInfos class declaration.
*
* @details Describes database field information.
*
***************************************************************************/

#ifndef ___DATABASE_FIELD_INFOS_H___
#define ___DATABASE_FIELD_INFOS_H___

#include "DatabasePrerequisites.h"

#include "EFieldType.h"

BEGIN_NAMESPACE_DATABASE
{
	/** Describes database field information.
	*/
	class CDatabaseFieldInfos
	{
	public:
		/** Constructor.
		@param[in] connection
			Database connection.
		@param[in] name
			Field name.
			*/
		DatabaseExport CDatabaseFieldInfos( const String & name );
		/** Constructor.
		@param[in] connection
			Database connection.
		@param[in] name
			Field name.
		@param[in] type
			Field type
			*/
		DatabaseExport CDatabaseFieldInfos( const String & name, EFieldType type );
		/** Constructor.
		@param[in] connection
			Database connection.
		@param[in] name
			Field name.
		@param[in] type
			Field type
		@param[in] limits
			Field limits
			*/
		DatabaseExport CDatabaseFieldInfos( const String & name, EFieldType type, uint32_t limits );

		/** Constructor.
		@param[in] connection
			Database connection.
		@param[in] name
			Field name.
		@param[in] type
			Field type
		@param[in] precision
			Field precision
			*/
		DatabaseExport CDatabaseFieldInfos( const String & name, EFieldType type, const std::pair< uint32_t, uint32_t > & precision );

		/** Destructor.
		*/
		DatabaseExport virtual ~CDatabaseFieldInfos();

		/** Get field type.
		@return
			Field type.
		*/
		DatabaseExport EFieldType GetType() const;

		/** Set field type.
		@param[in] type
			Field type
		*/
		DatabaseExport void SetType( EFieldType type );

		/** Set field type, for types needing a limit.
		@param[in] type
			Field type
		@param[in] limits
			Field limits
		*/
		DatabaseExport void SetType( EFieldType type, uint32_t limits );

		/** Set field type, for types needing a precision.
		@param[in] type
			Field type
		@param[in] precision
			Field precision
		*/
		DatabaseExport void SetType( EFieldType type, const std::pair< uint32_t, uint32_t > & precision );

		/** Get field name.
		@return
			Field name.
		*/
		DatabaseExport const String & GetName() const;

		/** Get field limits.
		@return
			Field limits.
		*/
		DatabaseExport const uint32_t & GetLimits() const;

		/** Get field limits.
		@return
			Field limits.
		*/
		DatabaseExport const std::pair< uint32_t, uint32_t > & GetPrecision() const;

	protected:
		//! Field name.
		String _name;
		//! Field type.
		EFieldType _type;
		//! Precision (floating and fixed points), _precision.first is also used as field limit (char, varchar, ...)
		std::pair< uint32_t, uint32_t > _precision;
	};
}
END_NAMESPACE_DATABASE

#endif //__DATABASE_FIELD_INFOS_H___
