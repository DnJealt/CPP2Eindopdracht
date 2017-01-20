#include "BuildingCard.h"



BuildingCard::BuildingCard()
{
}


BuildingCard::~BuildingCard()
{
}

int BuildingCard::getPrice()
{
	return _price;
}

std::string BuildingCard::getColor()
{
	return _color;
}

void BuildingCard::setPrice(int price)
{
	this->_price = price;
}

void BuildingCard::setColor(std::string color)
{
	this->_color = color;
}
