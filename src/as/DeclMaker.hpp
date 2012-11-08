#ifndef AS_DECLMAKER_HPP
#define AS_DECLMAKER_HPP

#include "as/TypeGetter.hpp"
#include "as/TypeOverride.hpp"

namespace as
{
	namespace priv
	{
		template< typename THIS, int NUM, int MAX, typename... TYPES >
		class DeclHelper;
		
		template< typename THIS, int NUM, int MAX, typename TYPE, typename... ARGS >
		class DeclHelper< THIS, NUM, MAX, TYPE, ARGS... >
		{
			public:
				static std::string str()
				{
					std::string suffix = "";
					if ( sizeof...( ARGS ) > 0 and NUM + 1 < MAX )
					{
						suffix = ", " + DeclHelper< THIS, NUM + 1, MAX, ARGS... >::str();
					}
					return THIS::template ArgHelper< NUM, TYPE >::Type::str() + suffix;
				}
		};
		
		template< typename THIS, int MAX, typename TYPE, typename... ARGS >
		class DeclHelper< THIS, MAX, MAX, TYPE, ARGS... >
		{
			public:
				static std::string str()
				{
					return "";
				}
		};
		
		template< typename THIS, int NUM, int MAX >
		class DeclHelper< THIS, NUM, MAX >
		{
			public:
				static std::string str()
				{
					return "";
				}
		};
	}
	
	template< typename OVERRIDES, typename TYPE >
	class DeclMaker;
	
	template< typename OVERRIDES, typename RET, typename... ARGS >
	class DeclMaker< OVERRIDES, RET ( ARGS... ) >
	{
		public:
			typedef DeclMaker< OVERRIDES, RET ( ARGS... ) > This;
			
			typedef typename OVERRIDES::template Get< 0, RET > RetHelper;
			
			template< int NUM, typename TYPE >
			class ArgHelper
			{
				public:
					typedef typename OVERRIDES::template Get< NUM + 1, TYPE > Type;
			};
			
			static std::string getDecl()
			{
				return RetHelper::str() + " $__FUNC__$( " + priv::DeclHelper< This, 0, sizeof...( ARGS ), ARGS... >::str() + " )";
				//return TypeGetter< RET, 0 >::getType() + " $__FUNC__$( " + TypeGetter< ARGS... >::getType() + " )";
			}
	};
}

#endif // AS_DECLMAKER_HPP
