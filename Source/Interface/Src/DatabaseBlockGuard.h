/************************************************************************//**
* @file DatabaseBlockGuard.h
* @author Sylvain Doremus
* @version 1.0
* @date 3/20/2014 2:47:39 PM
*
*
* @brief SBlockGuard class declaration.
*
* @details Allows to declare a scoped variable with an action on construction
*	and an action on destruction.
*
***************************************************************************/

#ifndef ___DATABASE_BLOCK_GUARD_H___
#define ___DATABASE_BLOCK_GUARD_H___

#include "DatabasePrerequisites.h"

#include "EErrorType.h"

#include <unordered_set>

BEGIN_NAMESPACE_DATABASE
{
	/** Allows to declare a scoped variable with an action on construction
		and an action on destruction.
		Useful to force execution of code when an exception is thrown.
	\b Example
	@code
		char * buffer = NULL;
		{
			auto guard = make_block_guard( [&buffer, &size]()
			{
				buffer = new char[size + 1];
			},
			[&buffer]()
			{
				delete [] buffer;
			} );
			//
			// ... Code that might throw an exception ...
			//
		}
	@endcode
	*/
	template< typename CleanFunc >
	struct SBlockGuard
	{
		/** Constructor
		@remarks
			Executes the init action
		@param[in] init
			The action executed on construction
		@param[in] clean
			The action executed on destruction
		*/
		template< typename InitFunc >
		SBlockGuard( InitFunc init, CleanFunc clean )
			: _clean( clean )
		{
			init();
		};

		/** Destructor
		@remarks
			Executes the clean action
		*/
		~SBlockGuard()
		{
			_clean();
		}

	private:
		//! The clean action
		CleanFunc _clean;
	};

	/** Helper method to declare a SBlockGuard
	@param[in] init
		The action executed on construction
	@param[in] clean
		The action executed on destruction
	@return
		The block guard
	*/
	template< typename InitFunc, typename CleanFunc >
	SBlockGuard< CleanFunc > make_block_guard( InitFunc init, CleanFunc clean )
	{
		return SBlockGuard< CleanFunc >( init, clean );
	}
}
END_NAMESPACE_DATABASE

#endif // ___DATABASE_BLOCK_GUARD_H___