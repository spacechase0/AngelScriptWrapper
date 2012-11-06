#include "as/TypeGetter.hpp"

#include <stdexcept>
#include <tuple>
#include <util/BindTuple.hpp>
#include <util/GenericPlaceholder.hpp>
#include <util/String.hpp>

#include "as/ArgConverter.hpp"
#include "as/ArgSetter.hpp"
#include "as/ScriptContext.hpp"
#include "as/TypeGetter.hpp"

namespace as
{
	template< typename RET, typename... ARGS >
	RET runFunction( asIScriptFunction* func, as::ScriptContext& theContext, ARGS... args )
	{
		asIScriptContext* context = theContext.getRaw();
		context->Prepare( func );

		as::ArgSetter< sizeof...( ARGS ) - 1 >::template setArg< ARGS... >( context, args... );

		int ret = context->Execute();
		if ( ret != asEXECUTION_FINISHED )
		{
			throw std::runtime_error( "Failed to execute function: " + util::toString( ret ) );
		}

		return as::ArgConverter< RET >::getReturn( context );
	};
	
	template< typename RET, typename... ARGS >
	std::function< RET ( ScriptContext&, ARGS... ) > ScriptModule::getFunction( const std::string& name, bool isDecl )
	{
		std::string decl = name;
		if ( !isDecl )
		{
			decl = TypeGetter< typename std::remove_pointer< RET ( ARGS... ) >::type >::getType();
			
			std::string find = "$__FUNC__$";
			std::size_t namePos = decl.find( find );
			decl = decl.replace( namePos, find.length(), name );
		}
		
		asIScriptFunction* func = module->GetFunctionByDecl( decl.c_str() );
		if ( func == NULL )
		{
			throw std::runtime_error( "Failed to get function." );
		}
		
		/*
		auto funcRaw = [ =, func ]( ScriptContext& theContext, ARGS... args ) -> RET
		               {
						asIScriptContext* context = theContext.getRaw();
						context->Prepare( func );
						
						ArgSetter< sizeof...( ARGS ) - 1 >::setArg( theContext, args... );
						
						int ret = context->Execute();
						if ( ret != asEXECUTION_FINISHED )
						{
							throw std::runtime_error( "Failed to execute function: " + util::toString( ret ) );
						}
						
						return ArgConverter< RET >::getReturn( context );
		               };
		//*/
		
		auto a = std::make_tuple( func );
		auto b = util::getPlaceholders< sizeof...( ARGS ) + 1 >();
		auto c = std::tuple_cat( a, b );
		
		std::function< RET ( ScriptContext&, ARGS... ) > ret;
		util::bindTuple( &runFunction< RET, ARGS... >, c, ret );
		return ret;
	}
}
