#include <angelscript.h> // TO DO: Find a way to remove this
#include <stdexcept>
#include <util/Convert.h>
#include <util/FuncTraits.hpp>
#include <util/MemberFunctionProxy.hpp>
#include <util/MemberObjectType.hpp>

#include "as/ClassFlags.hpp"
#include "as/GenericFunctions.hpp"
#include "as/TypeGetter.hpp"

// http://accu.org/index.php/journals/1608

namespace as
{
	template< typename FUNC >
	typename std::enable_if< std::is_pointer< FUNC >::value and std::is_function< typename std::remove_pointer< FUNC >::type >::value, void >::type
	ScriptEngine::registerGlobalFunction( FUNC func, const std::string& name )
	{
		std::string decl = TypeGetter< typename std::remove_pointer< FUNC >::type >::getType();
		
		std::string find = "$__FUNC__$";
		std::size_t namePos = decl.find( find );
		decl = decl.replace( namePos, find.length(), name );
		
		int res = engine->RegisterGlobalFunction( decl.c_str(), asFUNCTION( func ), asCALL_CDECL );
		if ( res < 0 )
		{
			throw std::runtime_error( "Failed to register function: " + util::toString( res ) );
		}
	}
	
	template< typename VAR >
	typename std::enable_if< !std::is_reference< VAR >::value and !std::is_same< VAR, long >::value, void >::type
	ScriptEngine::registerGlobalProperty( VAR& var, const std::string& name )
	{
		std::string decl = TypeGetter< VAR >::getType();
		decl = decl + " " + name;
		
		int res = engine->RegisterGlobalProperty( decl.c_str(), &var );
		if ( res < 0 )
		{
			throw std::runtime_error( "Failed to register function: " + util::toString( res ) );
		}
	}
	
	template< class CLASS >
	void ScriptEngine::registerClassType( const std::string& name )
	{
		int flags = getClassFlags< CLASS >();
		
		int res = engine->RegisterObjectType( name.c_str(), sizeof( CLASS ), asOBJ_VALUE | flags );
		if ( res < 0 )
		{
			throw std::runtime_error( "Failed register class type: " + util::toString( res ) );
		}
		
		res = engine->RegisterObjectBehaviour( name.c_str(), asBEHAVE_CONSTRUCT, "void f()", asFUNCTION( &doConstruct< CLASS > ), asCALL_CDECL_OBJFIRST );
		if ( res < 0 )
		{
			throw std::runtime_error( "Failed to register object constructor: " + util::toString( res ) );
		}
		
		res = engine->RegisterObjectBehaviour( name.c_str(), asBEHAVE_DESTRUCT, "void f()", asFUNCTION( &doDestruct< CLASS > ), asCALL_CDECL_OBJFIRST );
		if ( res < 0 )
		{
			throw std::runtime_error( "Failed to register object constructor: " + util::toString( res ) );
		}
	}
	
	template< typename FUNC >
	typename std::enable_if< std::is_pointer< FUNC >::value and std::is_function< typename std::remove_pointer< FUNC >::type >::value, void >::type
	ScriptEngine::registerClassFunction( FUNC func, const std::string& name )
	{
		typedef util::FuncTraits< typename std::remove_pointer< FUNC >::type > Traits;
		static_assert( Traits::ArgCount >= 1, "Need at least one argument, for \"this\"." );
		static_assert( std::is_pointer< typename Traits::template Arg< 0 >::Type >::value, "Need first argument to be a pointer, for \"this\"." );
		
		typedef typename std::remove_pointer< typename Traits::template Arg< 0 >::Type >::type Class;
		
		std::string decl = TypeGetter< typename std::remove_pointer< FUNC >::type >::getType();
		
		std::size_t start = decl.find( "(" );
		std::size_t comma = decl.find( "," );
		if ( comma == std::string::npos )
		{
			std::size_t end = decl.find( ")" );
			decl = decl.erase( start + 1, end - 1 - start );
		}
		else
		{
			decl = decl.erase( start + 1, comma - start );
		}
		
		std::string find = "$__FUNC__$";
		std::size_t namePos = decl.find( find );
		decl = decl.replace( namePos, find.length(), name );
		
		int res = engine->RegisterObjectMethod( TypeGetter< Class >::getType().c_str(), decl.c_str(), asFUNCTION( func ), asCALL_CDECL_OBJFIRST );
		if ( res < 0 )
		{
			throw std::runtime_error( "Failed to register class function: " + util::toString( res ) );
		}
	}
	
	template< typename FUNCPTR, FUNCPTR FUNC >
	typename std::enable_if< std::is_member_function_pointer< FUNCPTR >::value, void >::type
	ScriptEngine::registerClassFunction( const std::string& name )
	{
		registerClassFunction( util::makeMemberFunctionProxy< FUNCPTR, FUNC >(), name );
	}
	
	template< typename PROPPTR, PROPPTR PROP >
	typename std::enable_if< std::is_member_object_pointer< PROPPTR >::value, void >::type
	ScriptEngine::registerClassProperty( const std::string& name )
	{
		// Cheesy solution, I know. :P
		typedef util::MemberObjectType< PROPPTR > Mot; // Need a better name :P
		
		registerClassFunction( &setProperty< typename Mot::Class, typename Mot::Type, PROP >, "set_" + name );
		registerClassFunction( &getProperty< typename Mot::Class, typename Mot::Type, PROP >, "get_" + name );
	}
}
