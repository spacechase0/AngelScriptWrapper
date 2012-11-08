#ifndef AS_TYPEGETTER_HPP
#define AS_TYPEGETTER_HPP

#include <SFML/Config.hpp>
#include <string>
#include <type_traits>

namespace as
{
	template< typename... ARGS >
	class TypeGetter
	{
		public:
			static
			typename std::enable_if< sizeof...( ARGS ) == 0, std::string >::type
			getType()
			{
				return "";
			}
	};
	
	template< typename T >
	class TypeGetter< T >
	{
		public:
			static std::string getType()
			{
				return typeid( T ).name();
			}
	};
	
	template< typename T, typename... ARGS >
	class TypeGetter< T, ARGS... >
	{
		public:
			static std::string getType()
			{
				std::string suffix = "";
				if ( sizeof...( ARGS ) > 0 )
				{
					suffix = ", " + TypeGetter< ARGS... >::getType();
				}
				return TypeGetter< T >::getType() + suffix;
			}
	};
	
	#define doGenericTypeGetterTemplate(a,b) \
		template< typename T > \
		class TypeGetter< a > \
		{ \
			public: \
				static std::string getType() \
				{ \
					return b; \
				} \
		}
	
	#define doGenericTypeGetter(a,b) \
		template<> \
		class TypeGetter< a > \
		{ \
			public: \
				static std::string getType() \
				{ \
					return #b; \
				} \
		}
	
	doGenericTypeGetter( void, void );
	doGenericTypeGetter( bool, bool );
	doGenericTypeGetter( sf::Uint8, uint8 );
	doGenericTypeGetter( sf::Uint16, uint16 );
	doGenericTypeGetter( sf::Uint32, uint32 );
	doGenericTypeGetter( sf::Uint64, uint64 );
	doGenericTypeGetter( sf::Int8, int8 );
	doGenericTypeGetter( sf::Int16, int16 );
	doGenericTypeGetter( sf::Int32, int32 );
	doGenericTypeGetter( sf::Int64, int64 );
	doGenericTypeGetter( float, float );
	doGenericTypeGetter( double, double );
	doGenericTypeGetterTemplate( const T, "const " + TypeGetter< typename std::remove_const< T >::type >::getType() );
	doGenericTypeGetterTemplate( T&, TypeGetter< typename std::remove_reference< T >::type >::getType() + "& out" );
	doGenericTypeGetterTemplate( const T&, TypeGetter< typename std::remove_reference< T >::type >::getType() + "& in" );
	doGenericTypeGetterTemplate( T*, TypeGetter< typename std::remove_pointer< T >::type >::getType() + "@" );
	
	#undef doGenericTypeGetter
	#undef doGenericTypeGetterTemplate
	
	/*
	template< typename RET, typename... ARGS >
	class TypeGetter< RET ( ARGS... ) >
	{
		public:
			static std::string getType()
			{
				return TypeGetter< RET, 0 >::getType() + " $__FUNC__$( " + TypeGetter< ARGS... >::getType() + " )";
			}
	};
	//*/
}

#endif // AS_TYPEGETTER_HPP
