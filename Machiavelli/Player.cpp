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
	std::string messageToShow = "Deze kaarten zijn nog beschikbaar:";
	std::vector<std::string> availableCharacters;
	messageToShow += "\r\n";
	for (int i = 0; i < cards.size(); ++i)
	{
		messageToShow += cards.at(i)->getName(); if (i != cards.size() - 1) messageToShow += ", ";
		availableCharacters.push_back(cards.at(i)->getName());
	}

	messageToShow += "\r\nKies welk karakter je af wilt leggen.\r\n";

	int indexToDelete = toChoose(availableCharacters, messageToShow);

	cards.erase(cards.begin() + indexToDelete);


	return cards;
}

int Player::toChoose(std::vector<std::string> availableCharacters, std::string messageToShow)
{
	int userInput;
	*(this) << "\r\n" << messageToShow << "\r\n";

	for (int i = 0; i < availableCharacters.size(); ++i)
	{
		*(this) << std::to_string(i) << ": " << availableCharacters.at(i) << "\r\n";
	}

	bool picked = false;
	while (!picked) {
		userInput = std::stoi(socket->readline());
		if (userInput < availableCharacters.size() - 1 && userInput >= 0){
			picked = true;
		}
		else {
			*(this) << "Deze optie bestaat niet. Probeer het nog een keer.\n";
		}
	}
	
	return userInput;
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
