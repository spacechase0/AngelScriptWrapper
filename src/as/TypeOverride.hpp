#ifndef AS_TYPEOVERRIDE_HPP
#define AS_TYPEOVERRIDE_HPP

#include <string>
#include <tuple>

#include "as/TypeGetter.hpp"

namespace as
{
	template< int NUM, const char* STR >
	class TypeOverride
	{
		public:
			static constexpr int Index = NUM;
			static const char* String;
	};
	
	template< int NUM, const char* STR >
	const char* TypeOverride< NUM, STR >::String = STR;
	
	namespace priv
	{
		template< bool DO, typename OVERRIDE, typename NEXT >
		class GetImplHelper
		{
			public:
				static std::string str()
				{
					return NEXT::str();
				}
				
				typedef typename NEXT::Type Type;
		};
		
		template< typename OVERRIDE, typename NEXT >
		class GetImplHelper< true, OVERRIDE, NEXT >
		{
			public:
				static std::string str()
				{
					return OVERRIDE::String;
				}
				
				//typedef typename OVERRIDE::Type Type;
		};
		
		template< int NUM, typename FALLBACK, typename... TYPES >
		class GetImpl;
		
		template< int NUM, typename FALLBACK, typename TYPE, typename... TYPES >
		class GetImpl< NUM, FALLBACK, TYPE, TYPES... >
		{
			public:
				typedef GetImplHelper< ( TYPE::Index == NUM ), TYPE, GetImpl< NUM, FALLBACK, TYPES... > > Helper;
				
				static std::string str()
				{
					return Helper::str();
				}
				
				//typedef typename Helper::Type Type;
		};
		
		template< int NUM, typename FALLBACK >
		class GetImpl< NUM, FALLBACK >
		{
			public:
				static std::string str()
				{
					return TypeGetter< FALLBACK >::getType();
				}
				
				typedef FALLBACK Type;
		};
	}
	
	// const char[] arrayInt = "array< int >";
	// const char[] any = "?";
	// TypeOverrides< TypeOverride< 0, arrayInt >, TypeOverride< 1, any > >
	// ( NUM == 0 ) == return
	template< typename... OVERRIDES >
	class TypeOverrides
	{
		public:
			template< int NUM, typename FALLBACK >
			class Get
			{
				public:
					static std::string str()
					{
						return priv::GetImpl< NUM, FALLBACK, OVERRIDES... >::str();
					}
					
					//typedef typename priv::GetImpl< NUM, FALLBACK, OVERRIDES... >::Type Type;
			};
	};
}

#endif // AS_TYPEOVERRIDE_HPP
