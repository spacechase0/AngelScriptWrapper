#ifndef AS_ARGCONVERTER_HPP
#define AS_ARGCONVERTER_HPP

#include <angelscript.h>
#include <SFML/Config.hpp>
#include <type_traits>

namespace as
{
	template< typename T >
	class ArgConverter
	{
		public:
			static T getReturn( asIScriptContext* context )
			{
				return ( * static_cast< T* >( context->GetReturnObject() ) );
			}
			
			static
			typename std::enable_if< std::is_reference< T >::value, void >::type
			setArg( asIScriptContext* context, int num, T t )
			{
				// I hope this is safe
				context->SetArgObject( num, &const_cast< typename std::remove_const< typename std::remove_reference< T >::type >::type& >( t ) );
			}
	};
	
	#define doArgConverterTemplate(a,b,c) \
			template< typename T > \
			class ArgConverter< a > \
			{ \
				public: \
					static a getReturn( asIScriptContext* context ) \
					{ \
						return b;\
					} \
					\
					static void setArg( asIScriptContext* context, int num, a arg ) \
					{ \
						context->SetArg ## c; \
					} \
			}
	
	#define doArgConverter(a,b) \
			template<> \
			class ArgConverter< a > \
			{ \
				public: \
					static a getReturn( asIScriptContext* context ) \
					{ \
						auto ret = context->GetReturn ## b(); \
						return ( * reinterpret_cast< a* >( &ret ) ); \
					} \
					\
					static void setArg( asIScriptContext* context, int num, a arg ) \
					{ \
						context->SetArg ## b( num, arg ); \
					} \
			}
	
	static_assert( sizeof( sf::Uint8  ) == sizeof( asBYTE  ), "Need sf::Uint8 to be a byte." );
	static_assert( sizeof( sf::Uint16 ) == sizeof( asWORD  ), "Need sf::Uint16 to be a word." );
	static_assert( sizeof( sf::Uint32 ) == sizeof( asDWORD ), "Need sf::Uint32 to be a double-word." );
	static_assert( sizeof( sf::Uint64 ) == sizeof( asQWORD ), "Need sf::Uint64 to be a quad-word." );
	
	template<>
	class ArgConverter< void >
	{
		public:
			static void getReturn( asIScriptContext* context )
			{
			}
	};
	
	doArgConverter( sf::Uint8, Byte );
	doArgConverter( sf::Int8, Byte );
	doArgConverter( sf::Uint16, Word );
	doArgConverter( sf::Int16, Word );
	doArgConverter( sf::Uint32, DWord );
	doArgConverter( sf::Int32, DWord );
	doArgConverter( sf::Uint64, QWord );
	doArgConverter( sf::Int64, QWord );
	doArgConverter( float, Float );
	doArgConverter( double, Double );
	doArgConverterTemplate( T*, static_cast< T* >( context->GetReturnObject() ), Object( num, arg ) );
	
	#undef doReturnGetter
	#undef doReturnGetterTemplate
}

#endif // AS_ARGCONVERTER_HPP
