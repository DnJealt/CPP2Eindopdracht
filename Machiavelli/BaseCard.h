#pragma once

#include <iostream>
#include <string>

class BaseCard
{
public:
	BaseCard();
	~BaseCard();
	std::string getName();
	void setName(std::string name);
protected:
	std::string _name;
};

