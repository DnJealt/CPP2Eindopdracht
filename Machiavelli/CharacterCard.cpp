#include "CharacterCard.h"

CharacterCard::CharacterCard()
{
}

CharacterCard::~CharacterCard()
{
}

bool CharacterCard::isDead()
{
	return this->_isDead;
}

void CharacterCard::setDead(bool value)
{
	this->_isDead = value;
}

void CharacterCard::setNumber(int number)
{
	this->_number = number;
}
