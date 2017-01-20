#pragma once

#include "BaseCard.h"

class BuildingCard : public BaseCard
{
private:
	int _price;
	std::string _color;
public:
	BuildingCard();
	~BuildingCard();

	int getPrice();
	std::string getColor();

	void setPrice(int price);
	void setColor(std::string color);
};

