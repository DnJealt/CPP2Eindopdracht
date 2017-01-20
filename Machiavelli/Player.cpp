//
//  Player.cpp
//  socketexample
//
//  Created by Bob Polis on 23-11-15.
//  Copyright © 2015 Avans Hogeschool, 's-Hertogenbosch. All rights reserved.
//

#include "Player.hpp"

std::vector<std::shared_ptr<CharacterCard>> Player::pickCharacter(std::vector<std::shared_ptr<CharacterCard>> cards)
{
	//show available cards
	std::vector<std::string> availableCharacters;
	std::string messageToShow = "Deze kaarten zijn nog beschikbaar:";	
	messageToShow += "\r\n";
	for (int i = 0; i < cards.size(); ++i)
	{
		messageToShow += cards.at(i)->getName(); if (i != cards.size() - 1) messageToShow += ", ";
		availableCharacters.push_back(cards.at(i)->getName());
	}

	//afleggen
	messageToShow += "\r\nKies welk karakter je af wilt leggen.\r\n";

	choose = std::make_shared<Choose>(messageToShow, availableCharacters, socket);
	int indexToDelete = choose->activate();

	cards.erase(cards.begin() + indexToDelete);
	availableCharacters.erase(availableCharacters.begin() + indexToDelete);

	//1 kiezen
	messageToShow = "\r\nKies welk karakter je wilt kiezen\r\n";

	choose = std::make_shared<Choose>(messageToShow, availableCharacters, socket);
	int chosenIndex = choose->activate();
	
	addCharacterCard(cards.at(chosenIndex));

	cards.erase(cards.begin() + chosenIndex);
	availableCharacters.erase(availableCharacters.begin() + chosenIndex);
	
	//overgebleven karakters doorgeven
	return cards;
}

void Player::pickCommand(std::string command)
{
	if (choose) {
		if (choose->pick(command)) {
			choose = nullptr;
		}
	}
	else {
		*socket << "Dit is niet jouw beurt, " << name << ".\r\n";
	}
}

bool Player::isPlayerBusy()
{
	return _busyPlayer;
}

bool Player::hasCharacter(std::shared_ptr<CharacterCard> character)
{
	for each (auto character in this->characterCards)
	{
		if (character == character)
			return true;
	}
	return false;
}

void Player::turnWith(std::shared_ptr<CharacterCard> character)
{
	*this << this->get_name() << ", het is jouw beurt! Je bent een " << character->getName() << "\n";

	//TODO: Options to choose a turn

}

void Player::setPlayerBusy(bool value)
{
	this->_busyPlayer = value;
}

std::vector<std::shared_ptr<CharacterCard>> Player::getCharacters()
{
	return this->characterCards;
}

const Player & Player::operator<<(const std::string & message) const
{
	socket->write(message);
	return *this;
}
