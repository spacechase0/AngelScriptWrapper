#include "as/Message.hpp"

namespace as
{
	void Message::setType( Type theType )
	{
		type = theType;
	}
	
	Message::Type Message::getType() const
	{
		return type;
	}
	
	void Message::setSection( const std::string& theSection )
	{
		section = theSection;
	}
	
	const std::string& Message::getSection() const
	{
		return section;
	}
	
	void Message::setMessage( const std::string& theMessage )
	{
		message = theMessage;
	}
	
	const std::string& Message::getMessage() const
	{
		return message;
	}
	
	void Message::setRow( int theRow )
	{
		row = theRow;
	}
	
	int Message::getRow() const
	{
		return row;
	}
	
	void Message::setColumn( int theColumn )
	{
		column = theColumn;
	}
	
	int Message::getColumn() const
	{
		return column;
	}
}
