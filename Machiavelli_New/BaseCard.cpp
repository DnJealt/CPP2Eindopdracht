#include "BaseCard.h"



BaseCard::BaseCard()
{
}


BaseCard::~BaseCard()
{
}

std::string BaseCard::getName()
{
	return _name;
}

void BaseCard::setName(std::string name)
{
	this->_name = name;
}
