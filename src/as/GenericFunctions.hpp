#ifndef AS_GENERICFUNCTIONS_HPP
#define AS_GENERICFUNCTIONS_HPP

#include <angelscript.h>
#include <map>

namespace as
{
	// Temp hack
	template< class CLASS >
	class ConstructorDestructorTest
	{
		private:
			 void test()
			 {
				CLASS* c = new CLASS();
				delete c;
			}
	};
	
	template< class CLASS >
	void doConstruct( void* mem )
	{
		new ( mem ) CLASS();
	}
	
	template< class CLASS >
	void doDestruct( void* mem )
	{
		static_cast< CLASS* >( mem )->~CLASS();
	}
	
	template< class CLASS, typename TYPE, TYPE CLASS::* PTR >
	void setProperty( CLASS* obj, TYPE newVal )
	{
		obj->* PTR = newVal;
	}
	
	template< class CLASS, typename TYPE, TYPE CLASS::* PTR >
	TYPE getProperty( const CLASS* obj )
	{
		return obj->* PTR;
	}
	
	template< class CLASS >
	class Reffer
	{
		public:
			static void addRef( CLASS* var )
			{
				auto it = refs.find( var );
				if ( it == refs.end() )
				{
					it = refs.insert( std::make_pair( var, 0 ) ).first;
				}
				
				++it->second;
			}
			
			static void releaseRef( CLASS* var )
			{
				auto it = refs.find( var );
				if ( it == refs.end() )
				{
					return;
				}
				
				if ( --it->second == 0 )
				{
					delete var;
					refs.erase( it );
				}
			}
		
		private:
			static std::map< CLASS*, unsigned int > refs;
	};
	
	template< class CLASS >
	std::map< CLASS*, unsigned int > Reffer< CLASS >::refs;
	
	// I wish functions could be partially specialized
	template< class CLASS, bool DO >
	class Factory
	{
		public:
			inline static void registerBehavior( asIScriptEngine* engine, const std::string& name )
			{
			}
	};
	
	template< class CLASS >
	class Factory< CLASS, true >
	{
		public:
			inline static void registerBehavior( asIScriptEngine* engine, const std::string& name )
			{
				// The compiler gets confused and thinks I'm passing the macro two parameters.
				// I guess because of the comma for the second template parameter.
				auto funcPtr = &Factory< CLASS, true >::factoryFunc;
				auto func = asFUNCTION( funcPtr );
				
				int res = engine->RegisterObjectBehaviour( name.c_str(), asBEHAVE_FACTORY, ( name + "@ f()" ).c_str(), func, asCALL_CDECL );
				if ( res < 0 )
				{
					throw std::runtime_error( "Failed to register object constructor: " + util::toString( res ) );
				}
			}
		
		private:
			static CLASS* factoryFunc()
			{
				CLASS* var = new CLASS();
				Reffer< CLASS >::addRef( var );
				return var;
			}
	};
}

#endif // AS_GENERICFUNCTIONS_HPP
