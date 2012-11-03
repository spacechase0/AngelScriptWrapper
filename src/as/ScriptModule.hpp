#ifndef AS_SCRIPTMODULE_HPP
#define AS_SCRIPTMODULE_HPP

#include <functional>
#include <map>
#include <memory>
#include <string>

class asIScriptModule;
class CScriptBuilder;

namespace as
{
	class ScriptContext;
	class ScriptEngine;
	
	// TODO: Replace the AngelScript CScriptBuilder with a custom one, that has more
	// has more preprocessor stuff.
	class ScriptModule
	{
		public:
			~ScriptModule();
			
			std::string getName() const;
			
			bool addSectionFromFile( const std::string& filename );
			bool addSectionFromMemory( const std::string& section, const std::string& code );
			
			void define( const std::string& str );
			
			void build();
			
			template< typename RET, typename... ARGS >
			std::function< RET ( ScriptContext&, ARGS... ) > getFunction( const std::string& name, bool isDecl = false );
			
			std::string getTypeMetadata( const std::string& name );
			std::map< std::string, std::string > getAllTypeMetadata() const;
			
			asIScriptModule* getRaw();
			const asIScriptModule* getRaw() const;
		
		private:
			ScriptEngine& engine;
			asIScriptModule* module;
			std::unique_ptr< CScriptBuilder > builder; // Ptr to avoid include
			
			ScriptModule( ScriptEngine& theEngine, const std::string& name );
			ScriptModule( const ScriptModule& sc ) = delete;
			ScriptModule& operator = ( const ScriptModule& sc ) = delete;
			
			friend class ScriptEngine;
	};
}

#include "as/ScriptModule.inl"

#endif // AS_SCRIPTMODULE_HPP

