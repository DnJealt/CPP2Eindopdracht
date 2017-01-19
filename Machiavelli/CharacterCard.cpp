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

bool CharacterCard::isStolenFrom()
{
	return _stolenFrom;
}

void CharacterCard::setDead(bool value)
{
	this->_isDead = value;
}

void CharacterCard::setStolenFrom(bool value)
{
	this->_stolenFrom = value;
}

void CharacterCard::setNumber(int number)
{
	this->_number = number;
}
