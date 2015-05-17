/************************************************************************//**
 * @file FactoryDatabase.h
 * @author Sylvain Doremus
 * @version 1.0
 * @date 4/8/2014 10:11:22 AM
 *
 *
 * @brief Database factory class.
 *
 * @details CFactoryDatabase is used to manage CDatabase creation.
 *
 ***************************************************************************/

#ifndef ___DATABASE_FACTORY_DATABASE_H___
#define ___DATABASE_FACTORY_DATABASE_H___

#include "DatabasePrerequisites.h"

#include "DatabaseException.h"

BEGIN_NAMESPACE_DATABASE
{
	const String ERROR_DUPLICATE_FUNCTION = STR( "Duplicate creator function" ); //!< Duplicate function error.

	/** Database factory class.
	    CObject factory used to manage CDatabase creation.
	*/
	class CFactoryDatabase
	{
	public:
		/** Default constructor.
		*/
		DatabaseExport CFactoryDatabase();

		/** Destructor.
		*/
		DatabaseExport virtual ~CFactoryDatabase();

		/** Perform the factory initialization.
		@remarks
		    This function calls the RegisterServices() and RegisterObjectTypes() function.
		*/
		DatabaseExport void Initialize();

		/** Returns the factory type name.
		*/
		DatabaseExport String GetType()
		{
			return STR( "Factory.Database" );
		}

		/** Get the list of object type hat the factory can create.
		@return
		    The list of object type.
		*/
		inline const std::list<String> & GetObjectTypes()
		{
			return _objTypes;
		}

		typedef CDatabase * ( *CreatorFunction )( void );
		typedef std::map< String, CreatorFunction > MapFactory;
		typedef MapFactory::iterator iterator; //!< Iterator on the map factory

		/** Register creation functions to create new objects.
		@param idKey
		    key identifier.
		@param classCreator
		    Class creator function.

		\b Example
		@code
		    class CCmdAbort
		        : public CCmdPusherControl
		    {
		    public:
		        CCmdAbort();
		        virtual ~CCmdAbort();

		        // Creator function
		        static CCmdPusherControl* CreateInstance() { return new CCmdAbort(); }

		        // Execute command
		        void Execute();
		        ...
		    };
		@endcode
		@code
		    void CCmdPusherControlFactory::RegisterServices()
		    {
		        RegisterCreatorFunction( ABORT, CCmdAbort::CreateInstance );
		        RegisterCreatorFunction( ACCELERATION_CONTROL, CCmdAccelerationCtrl::CreateInstance );
		        ...
		    }
		@throw ExceptionCodes_NullPointer, ExceptionCodes_DuplicateItem
		@endcode
		*/
		DatabaseExport void RegisterCreatorFunction( const String & idKey, CreatorFunction classCreator );

		/** Create a Base object instance linked by a key.
		@param idKey
		    key identifier.
		@return
		    Return a new _Base object created.

		\b Example
		@code
		    CCmdPusherControl* cmd = _cmdFactory.CreateInstance( sIdCmd );
		    if( cmd )
		    {
		    ...
		    }
		@endcode
		*/
		DatabaseExport CDatabase * CreateInstance( const String & idKey );

	protected:
		/** Register creation functions to create new objects.
		@remarks
		    This function is called by Initialize().
		*/
		DatabaseExport virtual void RegisterServices() = 0;

		/** Register the object types.
		@remarks
		    This function is called by Initialize().
		*/
		DatabaseExport virtual void RegisterObjectTypes() = 0;

	protected:
		/** Get the Map where the construction info is stored.
		@return
		    Return the Map where the construction info is stored.
		*/
		inline MapFactory * GetMapFactory()
		{
			return &_mapFactory;
		}

		std::list<String> _objTypes; //!< List of object type that the factory can create.

	private:
		MapFactory _mapFactory;  //!< Map where the construction info is stored.
	};

}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_FACTORY_DATABASE_H___
