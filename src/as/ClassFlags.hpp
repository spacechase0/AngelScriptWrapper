#ifndef AS_CLASSFLAGS_HPP
#define AS_CLASSFLAGS_HPP

#include <angelscript.h>
#include <type_traits>

namespace as
{
	// Apparentally I don't actually need this... I'll it here just in case
	template< class CLASS >
	int getClassFlags()
	{
		int ret = asOBJ_APP_CLASS;
		
		// Apparentally these are from an older version of the standard?
		// I'll put the proper once I upgrade to some version where GCC
		// has the proper ones :P
		if ( !std::has_trivial_default_constructor< CLASS >::value )
		{
			ret |= asOBJ_APP_CLASS_CONSTRUCTOR;
		}
		
		if ( !std::has_trivial_destructor< CLASS >::value )
		{
			ret |= asOBJ_APP_CLASS_DESTRUCTOR;
		}
		
		if ( !std::has_trivial_assign< CLASS >::value )
		{
			ret |= asOBJ_APP_CLASS_ASSIGNMENT;
		}
		
		if ( !std::has_trivial_copy_constructor< CLASS >::value )
		{
			ret |= asOBJ_APP_CLASS_COPY_CONSTRUCTOR;
		}
		
		/*
		if ( !std::is_trivially_default_constructible< CLASS >::value )
		{
			ret |= asOBJ_APP_CLASS_CONSTRUCTOR;
		}
		
		if ( !std::is_trivially_destructible< CLASS >::value )
		{
			ret |= asOBJ_APP_CLASS_DESTRUCTOR;
		}
		
		if ( !std::is_trivially_copy_assignable< CLASS >::value )
		{
			ret |= asOBJ_APP_CLASS_ASSIGNMENT;
		}
		
		if ( !std::is_trivially_copy_constructible< CLASS >::value )
		{
			ret |= asOBJ_APP_COPY_CONSTRUCTOR;
		}
		*/
		
		return ret;
	}
}

#endif // AS_CLASSFLAGS_HPP
