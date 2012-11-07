#include "as/ScriptEngine.hpp"

#include <angelscript.h>

#include "as/ScriptContext.hpp"
#include "as/ScriptModule.hpp"

namespace as
{
	ScriptEngine::ScriptEngine()
	{
		engine = asCreateScriptEngine( ANGELSCRIPT_VERSION );
		engine->SetMessageCallback( asMETHOD( ScriptEngine, rawMessageCallback ), this, asCALL_THISCALL );
	}
	
	ScriptEngine::~ScriptEngine()
	{
		contexts.clear();
		modules.clear();
		engine->Release();
	}
	
	void ScriptEngine::setMessageCallback( MessageCallback theMessageCallback )
	{
		messageCallback = theMessageCallback;
	}
	
	ScriptModule* ScriptEngine::createModule( const std::string& module )
	{
		removeModule( module );
		modules.emplace_back( new ScriptModule( ( * this ), module ) );
		return modules.back().get();
	}
	
	ScriptModule* ScriptEngine::getModule( const std::string& module )
	{
		for ( std::size_t i = 0; i < modules.size(); ++i )
		{
			if ( modules[ i ]->getName() == module )
			{
				return modules[ i ].get();
			}
		}
		
		return NULL;
	}
	
	const ScriptModule* ScriptEngine::getModule( const std::string& module ) const
	{
		for ( std::size_t i = 0; i < modules.size(); ++i )
		{
			if ( modules[ i ]->getName() == module )
			{
				return modules[ i ].get();
			}
		}
		
		return NULL;
	}
	
	void ScriptEngine::removeModule( const std::string& module )
	{
		for ( std::size_t i = 0; i < modules.size(); ++i )
		{
			if ( modules[ i ]->getName() == module )
			{
				modules.erase( modules.begin() + i );
				return;
			}
		}
	}
	
	ScriptContext* ScriptEngine::createContext()
	{
		contexts.emplace_back( new ScriptContext( ( * this ), engine->CreateContext(), contextIdCounter++ ) );
		return contexts.back().get();
	}
	
	ScriptContext* ScriptEngine::getContext( int id )
	{
		for ( std::size_t i = 0; i < contexts.size(); ++i )
		{
			if ( contexts[ i ]->getId() == id )
			{
				return contexts[ i ].get();
			}
		}
		
		return NULL;
	}
	
	const ScriptContext* ScriptEngine::getContext( int id ) const
	{
		for ( std::size_t i = 0; i < contexts.size(); ++i )
		{
			if ( contexts[ i ]->getId() == id )
			{
				return contexts[ i ].get();
			}
		}
		
		return NULL;
	}
	
	void ScriptEngine::removeContext( int id )
	{
		for ( std::size_t i = 0; i < contexts.size(); ++i )
		{
			if ( contexts[ i ]->getId() == id )
			{
				contexts.erase( contexts.begin() + i );
				break;
			}
		}
	}
	
	asIScriptEngine* ScriptEngine::getRaw()
	{
		return engine;
	}
	
	const asIScriptEngine* ScriptEngine::getRaw() const
	{
		return engine;
	}
	
	void ScriptEngine::rawMessageCallback( asSMessageInfo* msg, void* param )
	{
		if ( messageCallback )
		{
			Message::Type type;
			switch ( msg->type )
			{
				case asMSGTYPE_INFORMATION: type = Message::Info;    break;
				case asMSGTYPE_WARNING    : type = Message::Warning; break;
				case asMSGTYPE_ERROR      : type = Message::Error;   break;
			}
			
			Message message;
			message.setType( type );
			message.setSection( msg->section );
			message.setMessage( msg->message );
			message.setRow( msg->row );
			message.setColumn( msg->col );
			
			messageCallback( message );
		}
	}
	
	void ScriptEngine::processNamespace( std::string& name )
	{
		std::string ns = "";
		
		std::size_t last = name.rfind( "::" );
		if ( last != std::string::npos )
		{
			ns = name.substr( 0, last );
			name = name.substr( last + 2 );
		}
		
		if ( ns.length() >= 2 and ns.substr( 0, 2 ) == "::" )
		{
			ns.erase( 0, 2 );
		}
		
		engine->SetDefaultNamespace( ns.c_str() );
	}
}
