#ifndef AS_GENERICFUNCTIONS_HPP
#define AS_GENERICFUNCTIONS_HPP

namespace as
{
	// Temp hack
	template< class CLASS >
	class ConstructorDestructorTest
	{
		private:
			 void test()
			 {
				CLASS* c = new CLASS();
				delete c;
			}
	};
	
	template< class CLASS >
	void doConstruct( void* mem )
	{
		new ( mem ) CLASS();
	}
	
	template< class CLASS >
	void doDestruct( void* mem )
	{
		static_cast< CLASS* >( mem )->~CLASS();
	}
	
	template< class CLASS, typename TYPE, TYPE CLASS::* PTR >
	void setProperty( CLASS* obj, TYPE newVal )
	{
		obj->* PTR = newVal;
	}
	
	template< class CLASS, typename TYPE, TYPE CLASS::* PTR >
	TYPE getProperty( const CLASS* obj )
	{
		return obj->* PTR;
	}
}

#endif // AS_GENERICFUNCTIONS_HPP
