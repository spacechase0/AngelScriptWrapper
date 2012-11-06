#ifndef AS_ARGSETTER_HPP
#define AS_ARGSETTER_HPP

#include <angelscript.h>

#include "as/ArgConverter.hpp"

//#include<AS/tYPEgETTER.HPP>
//#include"IOSTREAM"
namespace as
{
	template< int ARGC >
	class ArgSetter;
	
	template<>
	class ArgSetter< -1 > // 0 is the first argument
	{
		public:
			template< typename... ARGS >
			static void setArg( asIScriptContext* context )
			{
				//std::cout << "Done setting args" << std::endl;
			}
	};
	
	template< int N >
	class ArgSetter
	{
		public:
			template< typename ARG, typename... ARGS >
			static void setArg( asIScriptContext* context, ARG arg, ARGS... args )
			{
				//std::cout << "Setting arg " << N << " of type " << TypeGetter< ARG >::getType() << std::endl;
				ArgConverter< ARG >::setArg( context, N, arg );
				ArgSetter< N - 1 >::template setArg< ARGS... >( context, args... );
			}
	};
}

#endif // AS_ARGSETTER_HPP
