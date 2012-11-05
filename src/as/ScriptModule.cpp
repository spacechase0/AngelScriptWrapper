#include "as/ScriptModule.hpp"

#include <angelscript.h>
#include <scriptbuilder/scriptbuilder.h>
#include <stdexcept>
#include <util/String.hpp>

#include "as/ScriptEngine.hpp"

namespace as
{
	ScriptModule::~ScriptModule()
	{
		if ( module )
		{
			engine.getRaw()->DiscardModule( getName().c_str() );
		}
	}
	
	std::string ScriptModule::getName() const
	{
		return std::string( module->GetName() );
	}
	
	bool ScriptModule::addSectionFromFile( const std::string& filename )
	{
		int ret = builder->AddSectionFromFile( filename.c_str() );
		return ( ret == 0 );
	}
	
	bool ScriptModule::addSectionFromMemory( const std::string& section, const std::string& code )
	{
		int ret = builder->AddSectionFromMemory( code.c_str(), section.c_str() );
		return ( ret == 0 );
	}
	
	void ScriptModule::define( const std::string& str )
	{
		builder->DefineWord( str.c_str() );
	}
	
	void ScriptModule::build()
	{
		int ret = builder->BuildModule();
		if ( ret != 0 )
		{
			throw std::runtime_error( "Failed to build module: " + util::toString( ret ) );
		}
	}
	
	std::string ScriptModule::getTypeMetadata( const std::string& name )
	{
		int id = module->GetTypeIdByDecl( name.c_str() );
		if ( id < 0 )
		{
			throw std::runtime_error( "Failed to get metadata for type \"" + name + "\": " + util::toString( id ) );
		}
		
		return builder->GetMetadataStringForType( id );
	}
	
	std::map< std::string, std::string > ScriptModule::getAllTypeMetadata() const
	{
		std::map< std::string, std::string > ret;
		
		std::size_t typeCount = module->GetObjectTypeCount();
		for ( std::size_t i = 0; i < typeCount; ++i )
		{
			asIObjectType* type = module->GetObjectTypeByIndex( i );
			std::string typeStr = type->GetName();
			if ( type->GetNamespace() != std::string( "" ) )
			{
				typeStr = type->GetNamespace() + std::string( "::" ) + typeStr;
			}
			
			ret.insert( std::make_pair( typeStr, builder->GetMetadataStringForType( type->GetTypeId() ) ) );
		}
		
		return ret;
	}
	
	asIScriptModule* ScriptModule::getRaw()
	{
		return module;
	}
	
	const asIScriptModule* ScriptModule::getRaw() const
	{
		return module;
	}
	
	ScriptModule::ScriptModule( ScriptEngine& theEngine, const std::string& name )
	   : engine( theEngine ),
	     builder( new CScriptBuilder() )
	{
		int ret = builder->StartNewModule( engine.getRaw(), name.c_str() );
		if ( ret != 0 )
		{
			throw std::runtime_error( "Failed to start new module: " + util::toString( ret ) );
		}
		
		module = builder->GetModule();
	}
}

