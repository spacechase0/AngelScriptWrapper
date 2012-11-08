#ifndef AS_SCRIPTENGINE_HPP
#define AS_SCRIPTENGINE_HPP

#include <functional>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

#include "as/Message.hpp"

class asIScriptEngine;
class asSMessageInfo;

namespace as
{
	class ScriptContext;
	class ScriptModule;
	
	class ScriptEngine
	{
		public:
			ScriptEngine();
			~ScriptEngine();
			ScriptEngine( const ScriptEngine& se ) = delete;
			ScriptEngine& operator = ( const ScriptEngine& se ) = delete;
			
			typedef std::function< void ( Message& ) > MessageCallback;
			void setMessageCallback( MessageCallback theMessageCallback );
			
			template< typename FUNC, typename... OVERRIDES >
			typename std::enable_if< std::is_pointer< FUNC >::value and std::is_function< typename std::remove_pointer< FUNC >::type >::value, void >::type
			registerGlobalFunction( FUNC func, const std::string& theName );
			
			template< typename VAR >
			typename std::enable_if< !std::is_reference< VAR >::value and !std::is_same< VAR, long >::value, void >::type
			registerGlobalProperty( VAR& var, const std::string& theName );
			
			template< class CLASS >
			void registerClassValueType();
			
			template< class CLASS, bool DO_FACTORY = true >
			void registerClassReferenceType();
			
			template< typename FUNC, typename... OVERRIDES >
			typename std::enable_if< std::is_pointer< FUNC >::value and std::is_function< typename std::remove_pointer< FUNC >::type >::value, void >::type
			registerClassFunction( FUNC func, const std::string& name );
			
			template< typename FUNCPTR, FUNCPTR FUNC, typename... OVERRIDES >
			typename std::enable_if< std::is_member_function_pointer< FUNCPTR >::value, void >::type
			registerClassFunction( const std::string& name );
			
			// Isn't there something other than "property" that is more specific than "variable"/"member"? :P
			template< typename PROPPTR, PROPPTR PROP >
			typename std::enable_if< std::is_member_object_pointer< PROPPTR >::value, void >::type
			registerClassProperty( const std::string& name );
			
			ScriptModule* createModule( const std::string& module );
			ScriptModule* getModule( const std::string& module );
			const ScriptModule* getModule( const std::string& module ) const;
			void removeModule( const std::string& module );
			
			ScriptContext* createContext();
			ScriptContext* getContext( int id );
			const ScriptContext* getContext( int id ) const;
			void removeContext( int id );
			
			asIScriptEngine* getRaw();
			const asIScriptEngine* getRaw() const;
		
		private:
			asIScriptEngine* engine;
			std::vector< std::unique_ptr< ScriptModule > > modules; // unique_ptr because emplace_back didn't like private constructor/no copying :P
			std::vector< std::unique_ptr< ScriptContext > > contexts;
			MessageCallback messageCallback;
			
			int contextIdCounter;
			
			void rawMessageCallback( asSMessageInfo* msg, void* param );
			void processNamespace( std::string& name );
	};
}

#include "as/ScriptEngine.inl"

#endif // AS_SCRIPTENGINE_HPP
