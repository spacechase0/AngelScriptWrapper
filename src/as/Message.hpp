#ifndef AS_MESSAGE_HPP
#define AS_MESSAGE_HPP

#include <string>

namespace as
{
	class Message
	{
		public:
			enum Type
			{
				Error,
				Warning,
				Info,
			};
			
			void setType( Type theType );
			Type getType() const;
			
			void setSection( const std::string& theSection );
			const std::string& getSection() const;
			
			void setMessage( const std::string& theMessage );
			const std::string& getMessage() const;
			
			void setRow( int theRow );
			int getRow() const;
			
			void setColumn( int theColumn );
			int getColumn() const;
		
		private:
			Type type;
			std::string section;
			std::string message;
			int row;
			int column;
	};
}

#endif // AS_MESSAGE_HPP
