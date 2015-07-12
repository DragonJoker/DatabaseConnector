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
		@param[in] type
		    Field type
		@param[in] limprec
		    Field limits or precision (depends on field type)
		 */
		DatabaseExport CDatabaseFieldInfos( DatabaseConnectionPtr connection, const String & name, EFieldType type = EFieldType_NULL, uint32_t limprec = -1 );

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
		DatabaseExport CDatabaseFieldInfos( DatabaseConnectionPtr connection, const String & name, EFieldType type, std::pair< uint32_t, uint32_t > _precision );

		/** Constructor.
		@param[in] connection
		    Database connection.
		@param[in] name
		    Field name.
		@param[in] type
		    Field type as string
		@param[in] limprec
		    Field limits or precision (depends on field type)
		*/
		DatabaseExport CDatabaseFieldInfos( DatabaseConnectionPtr connection, const String & name, const String & type, uint32_t limprec = -1 );

		/** Destructor.
		*/
		DatabaseExport virtual ~CDatabaseFieldInfos();

		/** Get field type.
		@return
		    Field type.
		*/
		DatabaseExport EFieldType GetType() const;

		/** Get field type.
		@return
		    Field type.
		*/
		DatabaseExport void SetType( EFieldType type );

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

		/** Get connection.
		@return
		    Database connexion.
		*/
		DatabaseExport DatabaseConnectionPtr GetConnection() const;

	protected:
		/// Field name.
		String _name;
		/// Field type.
		EFieldType _type;
		/// Precision (floating and fixed points), _precision.first is also used as field limit (char, varchar, ...)
		std::pair< uint32_t, uint32_t > _precision;
		/// Database connection.
		DatabaseConnectionPtr _connection;

	}; // class CDatabaseFieldInfos

}
END_NAMESPACE_DATABASE

#endif //__DATABASE_FIELD_INFOS_H___
