#ifndef AS_SCRIPTCONTEXT_HPP
#define AS_SCRIPTCONTEXT_HPP

class asIScriptContext;

namespace as
{
	class ScriptEngine;
	
	class ScriptContext
	{
		public:
			~ScriptContext();
			
			int getId() const;
			
			asIScriptContext* getRaw();
			const asIScriptContext* getRaw() const;
		
		private:
			ScriptEngine& engine;
			asIScriptContext* context;
			int id;
			
			ScriptContext( ScriptEngine& theEngine, asIScriptContext* theContext, int theId );
			ScriptContext( const ScriptContext& sc ) = delete;
			ScriptContext& operator = ( const ScriptContext& sc ) = delete;
			
			friend class ScriptEngine;
	};
}

#endif // AS_SCRIPTCONTEXT_HPP
