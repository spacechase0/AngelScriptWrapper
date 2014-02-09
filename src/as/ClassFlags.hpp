#ifndef AS_CLASSFLAGS_HPP
#define AS_CLASSFLAGS_HPP

#include <angelscript.h>
#include <type_traits>

namespace as
{
	template< class CLASS >
	int getClassFlags()
	{
		int ret = asOBJ_APP_CLASS;
		
		// TODO: Update when I get a compiler that supports the right ones...
		
		//if ( !std::is_trivially_default_constructible< CLASS >::value )
		if ( !std::has_trivial_default_constructor< CLASS >::value )
		{
			ret |= asOBJ_APP_CLASS_CONSTRUCTOR;
		}
		
		if ( !std::is_trivially_destructible< CLASS >::value )
		{
			ret |= asOBJ_APP_CLASS_DESTRUCTOR;
		}
		
		//if ( !std::is_trivially_copy_assignable< CLASS >::value )
		if ( !std::has_trivial_copy_assign< CLASS >::value )
		{
			ret |= asOBJ_APP_CLASS_ASSIGNMENT;
		}
		
		if ( !std::has_trivial_copy_constructor< CLASS >::value )
		{
			ret |= asOBJ_APP_CLASS_COPY_CONSTRUCTOR;
		}
		
		return ret;
	}
}

#endif // AS_CLASSFLAGS_HPP
