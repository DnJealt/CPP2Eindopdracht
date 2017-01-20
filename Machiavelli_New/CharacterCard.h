#pragma once

#include "BaseCard.h"
#include <memory>

class CharacterCard : public BaseCard
{
public:
	CharacterCard();
	~CharacterCard();

	bool isDead();
	bool isStolenFrom();

	void setDead(bool value);
	void setStolenFrom(bool value);
	void setNumber(int number);
	void setThief(std::string thiefName);

	std::string getThief();
private:
	bool _isDead;
	int _number;
	bool _stolenFrom;
	std::string thief;
};

