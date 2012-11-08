#include <angelscript.h> // TO DO: Find a way to remove this
#include <stdexcept>
#include <util/FuncTraits.hpp>
#include <util/MemberFunctionProxy.hpp>
#include <util/MemberObjectType.hpp>
#include <util/String.hpp>

#include "as/ClassFlags.hpp"
#include "as/DeclMaker.hpp"
#include "as/GenericFunctions.hpp"
#include "as/TypeGetter.hpp"

// http://accu.org/index.php/journals/1608

namespace as
{
	template< typename FUNC, typename... OVERRIDES >
	typename std::enable_if< std::is_pointer< FUNC >::value and std::is_function< typename std::remove_pointer< FUNC >::type >::value, void >::type
	ScriptEngine::registerGlobalFunction( FUNC func, const std::string& theName )
	{
		std::string name = theName;
		processNamespace( name );
		
		typedef TypeOverrides< OVERRIDES... > Overrides;
		std::string decl = DeclMaker< Overrides, typename std::remove_pointer< FUNC >::type >::getDecl();
		
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
	ScriptEngine::registerGlobalProperty( VAR& var, const std::string& theName )
	{
		std::string name = theName;
		processNamespace( name );
		
		std::string decl = TypeGetter< VAR >::getType();
		decl = decl + " " + name;
		
		int res = engine->RegisterGlobalProperty( decl.c_str(), &var );
		if ( res < 0 )
		{
			throw std::runtime_error( "Failed to register function: " + util::toString( res ) );
		}
	}
	
	template< class CLASS >
	void ScriptEngine::registerClassValueType()
	{
		std::string name = TypeGetter< CLASS >::getType();
		processNamespace( name );
		
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
			throw std::runtime_error( "Failed to register object destructor: " + util::toString( res ) );
		}
	}
	
	template< class CLASS, bool DO_FACTORY >
	void ScriptEngine::registerClassReferenceType()
	{
		std::string name = TypeGetter< CLASS >::getType();
		processNamespace( name );
		
		int res = engine->RegisterObjectType( name.c_str(), sizeof( CLASS ), asOBJ_REF );
		if ( res < 0 )
		{
			throw std::runtime_error( "Failed register class type: " + util::toString( res ) );
		}
		
		res = engine->RegisterObjectBehaviour( name.c_str(), asBEHAVE_ADDREF, "void f()", asFUNCTION( &Reffer< CLASS >::addRef ), asCALL_CDECL_OBJFIRST );
		if ( res < 0 )
		{
			throw std::runtime_error( "Failed to register object add reference behavior: " + util::toString( res ) );
		}
		
		res = engine->RegisterObjectBehaviour( name.c_str(), asBEHAVE_RELEASE, "void f()", asFUNCTION( &Reffer< CLASS >::releaseRef ), asCALL_CDECL_OBJFIRST );
		if ( res < 0 )
		{
			throw std::runtime_error( "Failed to register object add reference behavior: " + util::toString( res ) );
		}
		
		// This is a template because with an if statement, if you don't want the factory method,
		// it still compiles the call, which will fail if the default constructor isn't accessible,
		// like it being private.
		Factory< CLASS, DO_FACTORY >::registerBehavior( engine, name );
	}
	
	template< typename FUNC, typename... OVERRIDES >
	typename std::enable_if< std::is_pointer< FUNC >::value and std::is_function< typename std::remove_pointer< FUNC >::type >::value, void >::type
	ScriptEngine::registerClassFunction( FUNC func, const std::string& name )
	{
		typedef util::FuncTraits< typename std::remove_pointer< FUNC >::type > Traits;
		static_assert( Traits::ArgCount >= 1, "Need at least one argument, for \"this\"." );
		static_assert( std::is_pointer< typename Traits::template Arg< 0 >::Type >::value, "Need first argument to be a pointer, for \"this\"." );
		
		typedef typename std::remove_pointer< typename Traits::template Arg< 0 >::Type >::type ClassRaw;
		typedef typename std::remove_const< ClassRaw >::type Class;
		
		typedef TypeOverrides< OVERRIDES... > Overrides;
		std::string decl = DeclMaker< Overrides, typename std::remove_pointer< FUNC >::type >::getDecl();
		
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
		
		if ( std::is_const< ClassRaw >::value )
		{
			decl += " const";
		}
		
		std::string find = "$__FUNC__$";
		std::size_t namePos = decl.find( find );
		decl = decl.replace( namePos, find.length(), name );
		
		std::string classRaw = TypeGetter< Class >::getType();
		std::string className = classRaw;
		processNamespace( className );
		
		std::size_t nsPos = classRaw.rfind( className );
		if ( nsPos != std::string::npos and nsPos != 0 )
		{
			std::string ns = classRaw.substr( 0, nsPos );
			
			// Probably needs improvement, like if the class was a::B and a parameter
			// was b::a::C.
			for ( std::size_t found = decl.find( ns ); found != std::string::npos; found = decl.find( ns ) )
			{
				decl = decl.erase( found, ns.length() );
			}
		}
		
		int res = engine->RegisterObjectMethod( className.c_str(), decl.c_str(), asFUNCTION( func ), asCALL_CDECL_OBJFIRST );
		if ( res < 0 )
		{
			throw std::runtime_error( "Failed to register class function: " + util::toString( res ) );
		}
	}
	
	template< typename FUNCPTR, FUNCPTR FUNC, typename... OVERRIDES >
	typename std::enable_if< std::is_member_function_pointer< FUNCPTR >::value, void >::type
	ScriptEngine::registerClassFunction( const std::string& name )
	{
		typedef decltype( util::makeMemberFunctionProxy< FUNCPTR, FUNC >() ) FUNCTYPE;
		registerClassFunction< FUNCTYPE, OVERRIDES... >( util::makeMemberFunctionProxy< FUNCPTR, FUNC >(), name );
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
