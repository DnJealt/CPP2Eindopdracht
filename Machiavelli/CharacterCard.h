#pragma once

#include "BaseCard.h"

class CharacterCard : public BaseCard
{
public:
	CharacterCard();
	~CharacterCard();

	bool isDead();

	void setDead(bool value);
	void setNumber(int number);
private:
	bool _isDead;
	int _number;
};

