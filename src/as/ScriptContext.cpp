#include "as/ScriptContext.hpp"

#include <angelscript.h>

namespace as
{
	ScriptContext::~ScriptContext()
	{
		context->Release();
	}
	
	int ScriptContext::getId() const
	{
		return id;
	}
	
	asIScriptContext* ScriptContext::getRaw()
	{
		return context;
	}
	
	const asIScriptContext* ScriptContext::getRaw() const
	{
		return context;
	}
	
	ScriptContext::ScriptContext( ScriptEngine& theEngine, asIScriptContext* theContext, int theId )
	   : engine( theEngine ),
	     context( theContext ),
	     id( theId )
	{
	}
}
