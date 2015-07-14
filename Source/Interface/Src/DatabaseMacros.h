/************************************************************************//**
* @file DatabaseMacros.h
* @author Sylvain Doremus
* @version 1.0
* @date 7/12/2015 7:51 PM
*
*
* @brief Useful macros and preprocessor checks
*
***************************************************************************/

#ifndef ___DATABASE_MACROS_H___
#define ___DATABASE_MACROS_H___

#	if !defined( __FUNCTION__ )
//! Defines this macro if not in debug
#		define __FUNCTION__ ""
#	endif

#undef DB_HAS_MAKE_UNIQUE

#if defined( _MSC_VER)
#	include <tchar.h>
#	pragma warning( disable: 4061 )    // enum value is not *explicitly* handled in switch
#	pragma warning( disable: 4099 )    // first seen using 'struct' now seen using 'class'
#	pragma warning( disable: 4127 )    // conditional expression is constant
#	pragma warning( disable: 4217 )    // member template isn't copy constructor
#	pragma warning( disable: 4250 )    // inherits (implements) some member via dominance
#	pragma warning( disable: 4251 )    // needs to have dll-interface to be used by clients
#	pragma warning( disable: 4275 )    // exported class derived from non-exported class
#	pragma warning( disable: 4347 )    // "behavior change", function called instead of template
#	pragma warning( disable: 4355 )    // "'this': used in member initializer list
#	pragma warning( disable: 4428 )    // MSVC 9: universal-character-name encountered in source
#	pragma warning( disable: 4505 )    // unreferenced function has been removed
#	pragma warning( disable: 4510 )    // default constructor could not be generated
#	pragma warning( disable: 4511 )    // copy constructor could not be generated
#	pragma warning( disable: 4512 )    // assignment operator could not be generated
#	pragma warning( disable: 4513 )    // destructor could not be generated
#	pragma warning( disable: 4610 )    // can never be instantiated user defined constructor required
#	pragma warning( disable: 4623 )    // default constructor could not be generated
#	pragma warning( disable: 4624 )    // destructor could not be generated
#	pragma warning( disable: 4625 )    // copy constructor could not be generated
#	pragma warning( disable: 4626 )    // assignment operator could not be generated
#	pragma warning( disable: 4640 )    // a local static object is not thread-safe
#	pragma warning( disable: 4661 )    // a member of the template class is not defined.
#	pragma warning( disable: 4670 )    // a base class of an exception class is inaccessible for catch
#	pragma warning( disable: 4672 )    // a base class of an exception class is ambiguous for catch
#	pragma warning( disable: 4673 )    // a base class of an exception class is inaccessible for catch
#	pragma warning( disable: 4675 )    // resolved overload was found by argument-dependent lookup
#	pragma warning( disable: 4702 )    // unreachable code, e.g. in <list> header.
#	pragma warning( disable: 4710 )    // call was not inlined
#	pragma warning( disable: 4711 )    // call was inlined
#	pragma warning( disable: 4820 )    // some padding was added
#	pragma warning( disable: 4917 )    // a GUID can only be associated with a class, interface or namespace
#	pragma warning( disable: 4996 )    // MSVC 9: a C std library function has been "deprecated" (says MS)
#	if _MSC_VER >= 1800
#		define DB_HAS_MAKE_UNIQUE 					1
#	else
#		define DB_HAS_MAKE_UNIQUE 					0
#	endif
#	define cvsnprintf								_vsnprintf_s
#elif defined( __clang__)
#	define DB_HAS_MAKE_UNIQUE 						has_feature(cxx_variadic_templates)
#	if !defined( _WIN32 )
#		define _FILE_OFFSET_BITS					64
#		define cvsnprintf( buf, sz, cnt, fmt, arg )	vsnprintf( buf, cnt, fmt, arg )
#	else
#		define _FILE_OFFSET_BITS					64
#		define cvsnprintf( buf, sz, cnt, fmt, arg )	vsnprintf( buf, cnt, fmt, arg )
#	endif
#elif defined( __GNUG__)
#	define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#	if GCC_VERSION >= 40900
#		define DB_HAS_MAKE_UNIQUE					1
#	else
#		define DB_HAS_MAKE_UNIQUE					0
#	endif
#	if !defined( _WIN32 )
#		define _FILE_OFFSET_BITS					64
#		define cvsnprintf( buf, sz, cnt, fmt, arg )	vsnprintf( buf, cnt, fmt, arg )
#	else
#		define _FILE_OFFSET_BITS					64
#		define cvsnprintf( buf, sz, cnt, fmt, arg )	vsnprintf( buf, cnt, fmt, arg )
#	endif
#elif defined( __BORLANDC__ )
#	define DB_HAS_MAKE_UNIQUE						0
#	if !defined( _WIN32 )
#		define _FILE_OFFSET_BITS					64
#		define cvsnprintf( buf, sz, cnt, fmt, arg )	vsnprintf( buf, cnt, fmt, arg )
#	else
#		define _FILE_OFFSET_BITS					64
#		define cvsnprintf( buf, sz, cnt, fmt, arg )	vsnprintf( buf, cnt, fmt, arg )
#	endif
#else
#	error "Yet unsupported compiler"
#endif

#define DECLARE_SMART_PTR( class_name )		typedef std::shared_ptr<	class_name			>	class_name##SPtr;	\
											typedef std::weak_ptr<		class_name			>	class_name##WPtr;	\
											typedef std::unique_ptr<	class_name			>	class_name##UPtr;	\
											typedef std::auto_ptr<		class_name			>	class_name##APtr;	\
											typedef class_name *								class_name##RPtr

#define DECLARE_MAP( key, value, name )		typedef std::map<			key,		value	>	name##Map;			\
											typedef name##Map::iterator							name##MapIt;		\
											typedef name##Map::reverse_iterator					name##MapRIt;		\
											typedef name##Map::const_iterator					name##MapConstIt;	\
											typedef name##Map::const_reverse_iterator			name##MapConstRIt;	\
											typedef name##Map::value_type						name##Pair

#define DECLARE_MULTIMAP( key, value, name )typedef std::multimap<		key,		value	>	name##MMap;			\
											typedef name##MMap::iterator						name##MMapIt;		\
											typedef name##MMap::reverse_iterator				name##MMapRIt;		\
											typedef name##MMap::const_iterator					name##MMapConstIt;	\
											typedef name##MMap::const_reverse_iterator			name##MMapConstRIt;	\
											typedef name##MMap::value_type						name##Pair

#define DECLARE_SET( key, name )			typedef std::set<			key					>	name##Set;			\
											typedef name##Set::iterator							name##SetIt;		\
											typedef name##Set::reverse_iterator					name##SetRIt;		\
											typedef name##Set::const_iterator					name##SetConstIt;	\
											typedef name##Set::const_reverse_iterator			name##SetConstRIt

#define DECLARE_VECTOR( key, name )			typedef std::vector<		key					>	name##Array;		\
											typedef name##Array::iterator						name##ArrayIt;		\
											typedef name##Array::reverse_iterator				name##ArrayRIt;		\
											typedef name##Array::const_iterator					name##ArrayConstIt;	\
											typedef name##Array::const_reverse_iterator			name##ArrayConstRIt

#define DECLARE_ARRAY( key, count, name )	typedef std::array<			key,		count	>	name##Array;		\
											typedef name##Array::iterator						name##ArrayIt;		\
											typedef name##Array::reverse_iterator				name##ArrayRIt;		\
											typedef name##Array::const_iterator					name##ArrayConstIt;	\
											typedef name##Array::const_reverse_iterator			name##ArrayConstRIt

#define DECLARE_LIST( key, name )			typedef std::list<			key					>	name##List;			\
											typedef name##List::iterator						name##ListIt;		\
											typedef name##List::reverse_iterator				name##ListRIt;		\
											typedef name##List::const_iterator					name##ListConstIt;	\
											typedef name##List::const_reverse_iterator			name##ListConstRIt

#define DECLARE_TPL_MAP( key, value, name )		typedef std::map<			key,		value	>		name##Map;			\
												typedef typename name##Map::iterator					name##MapIt;		\
												typedef typename name##Map::reverse_iterator			name##MapRIt;		\
												typedef typename name##Map::const_iterator				name##MapConstIt;	\
												typedef typename name##Map::const_reverse_iterator		name##MapConstRIt;	\
												typedef typename name##Map::value_type					name##Pair

#define DECLARE_TPL_SET( key, name )			typedef std::set<			key					>		name##Set;			\
												typedef typename name##Set::iterator					name##SetIt;		\
												typedef typename name##Set::reverse_iterator			name##SetRIt;		\
												typedef typename name##Set::const_iterator				name##SetConstIt;	\
												typedef typename name##Set::const_reverse_iterator		name##SetConstRIt

#define DECLARE_TPL_VECTOR( key, name )			typedef std::vector<		key					>		name##Array;		\
												typedef typename name##Array::iterator					name##ArrayIt;		\
												typedef typename name##Array::reverse_iterator			name##ArrayRIt;		\
												typedef typename name##Array::const_iterator			name##ArrayConstIt;	\
												typedef typename name##Array::const_reverse_iterator	name##ArrayConstRIt

#define DECLARE_TPL_ARRAY( key, count, name )	typedef std::array<			key,		count	>		name##Array;		\
												typedef typename name##Array::iterator					name##ArrayIt;		\
												typedef typename name##Array::reverse_iterator			name##ArrayRIt;		\
												typedef typename name##Array::const_iterator			name##ArrayConstIt;	\
												typedef typename name##Array::const_reverse_iterator	name##ArrayConstRIt

#define DECLARE_TPL_LIST( key, name )			typedef std::list<			key					>		name##List;			\
												typedef typename name##List::iterator					name##ListIt;		\
												typedef typename name##List::reverse_iterator			name##ListRIt;		\
												typedef typename name##List::const_iterator				name##ListConstIt;	\
												typedef typename name##List::const_reverse_iterator		name##ListConstRIt

#if !defined( DB_PARAM_UNUSED )
#	define DB_PARAM_UNUSED( x )
#endif

#endif
