/*
#include <iostream>

#include <scriptstdstring/scriptstdstring.h>

#include "as/ScriptContext.hpp"
#include "as/ScriptEngine.hpp"
#include "as/ScriptModule.hpp"

namespace
{
	int add( int a, int b )
	{
		return a + b;
	}

	void mult( int a, int b, int& out )
	{
		out = a * b;
	}

	struct MyClass
	{
		float f;
		double d;
		
		MyClass()
		   : f( 1.f ),
			 d( 2.D )
		{
		}
		
		void print() const
		{
			std::cout << "MyClass " << this << ": f=" << f << " d=" << d << std::endl;
		}
	};

	void printSum( const MyClass* mc )
	{
		std::cout << "MyClass " << mc << ": f+d=" << mc->f + mc->d << std::endl;
	}

	void doStuff( MyClass& a, float b, double c )
	{
		a.f += b;
		a.d += c;
	}

	void print( const std::string& str )
	{
		std::cout << str;
	}

	void messageCallback( as::Message& message )
	{
		std::cout << '[';
		switch ( message.getType() )
		{
			case as::Message::Info: std::cout << "INFO"; break;
			case as::Message::Warning: std::cout << "WARNING"; break;
			case as::Message::Error: std::cout << "ERROR"; break;
		}
		std::cout << "] ";
		
		std::cout << message.getSection() << ':' << message.getRow() << ':' << message.getColumn() << ' ';
		std::cout << message.getMessage() << std::endl;
	}
	
	sf::Int32 i = 0;
	sf::Int64 l = 0;
}

namespace as
{
	template<>
	class TypeGetter< std::string >
	{
		public:
			static std::string getType()
			{
				return "string";
			}
	};
	
	template<>
	class TypeGetter< MyClass >
	{
		public:
			static std::string getType()
			{
				return "MyClass";
			}
	};
}


int main_as()
{
	try
	{
		as::ScriptEngine engine;
		engine.setMessageCallback( std::bind( &messageCallback, std::placeholders::_1 ) );
		
		RegisterStdString( engine.getRaw() );
		//RegisterStdStringUtils( engine.getRaw() );
		
		engine.registerClassType< MyClass >( "MyClass" );
		engine.registerClassProperty< decltype( &MyClass::f ), &MyClass::f >( "f" );
		engine.registerClassProperty< decltype( &MyClass::d ), &MyClass::d >( "d" );
		engine.registerClassFunction< decltype( &MyClass::print ), &MyClass::print >( "print" );
		engine.registerClassFunction( &printSum, "printSum" );
		
		engine.registerGlobalFunction( add, "add" );
		engine.registerGlobalFunction( mult, "multiply" );
		engine.registerGlobalFunction( doStuff, "doStuff" );
		engine.registerGlobalFunction( print, "print" );
		
		engine.registerGlobalProperty( i, "i" );
		engine.registerGlobalProperty( l, "l" );
		
		as::ScriptModule& module = ( * engine.createModule( "test" ) );
		module.define( "KITTY" );
		
		module.addSectionFromFile( "test.angelscript" );
		module.build();
		
		auto data = module.getAllTypeMetadata();
		for ( auto it = data.begin(); it != data.end(); ++it )
		{
			if ( it->second == "" )
			{
				continue;
			}
			
			std::cout << it->first << ": [" << it->second << "]" << std::endl;
		}
		
		auto oldI = i;
		auto oldL = l;
		{
			as::ScriptContext& context = ( * engine.createContext() );
			auto func = module.getFunction< void >( "main" );
			func( context );
		}
		std::cout << "i is now " << i << ", but it was " << oldI << std::endl;
		std::cout << "l is now " << l << ", but it was " << oldL << std::endl;
	}
	catch ( std::exception& exception )
	{
		std::cout << "Exception: " << exception.what() << std::endl;
	}
	
	return 0;
}
*/
