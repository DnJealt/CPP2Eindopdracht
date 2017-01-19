#pragma once

#include "BaseCard.h"

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
private:
	bool _isDead;
	int _number;
	bool _stolenFrom;
};

