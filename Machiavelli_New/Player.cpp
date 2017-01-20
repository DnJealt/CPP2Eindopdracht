//
//  Player.cpp
//  socketexample
//
//  Created by Bob Polis on 23-11-15.
//  Copyright © 2015 Avans Hogeschool, 's-Hertogenbosch. All rights reserved.
//

#include "Player.hpp"
#include "Game.h"

void Player::turnWith(std::shared_ptr<CharacterCard> character)
{
	*this << this->get_name() << ", het is jouw beurt! Je bent een " << character->getName() << "\n";
	//TODO: Options to choose a turn

	//TODO: Check of karakter gestolen character zou kunnen zijn, dan alle goudstukken naar de dief doen van speler.
	if (character->isStolenFrom()) {
		std::shared_ptr<Player> toGain = game->waitingPlayer(shared_from_this());
		if (toGain->get_name() == character->getName()) {
			toGain->addGold(gold);
			gold = 0;
		}
	}

	checkForNewGold(character);

	
	//ontvangt 2 goudstukken || trekt 2 gebouwenkaarten van trekstapel. houd er 1 en neemt die in de hand en legt de andere op de aflegstapel.
	bool ended = false;
	bool goldOrCards = false;
	bool specialPower = false;
	int possibleBuildBuildings;

	if (character->getName() == "Bouwmeester")
		possibleBuildBuildings = 3;
	else
		possibleBuildBuildings = 1;

	while (!ended) {
		std::vector<std::string> possibleActions;

		//status bekijken
		int choices = 0;
		int takeChoice = -1;
		int buildChoice = -1;
		int propertyChoice = -1;

		if (!goldOrCards) {
			possibleActions.push_back("Goud of kaarten pakken");
			takeChoice = choices;
			choices++;
		}
		if (possibleBuildBuildings > 0) {
			possibleActions.push_back("Gebouw bouwen");
			buildChoice = choices;
			choices++;
		}
		if (!specialPower) {
			possibleActions.push_back("Karaktereigenschap van " + character->getName() + " gebruiken.");
			propertyChoice = choices;
			choices++;
		}
		possibleActions.push_back("Status bekijken");
		possibleActions.push_back("Stoppen met deze beurt");
		choose = std::make_shared<Choose>("maak een keuze", possibleActions, socket);

		int choice = choose->activate();

		if (choice == takeChoice) {
			goldOrCards = true;
			//goud of kaarten pakken.
		}
		else if (choice == buildChoice) {
			// build a building
		}
		else if (choice == propertyChoice) {
			//use special power
			specialPower = true;
		}
		else if (choice == choices) {
			//show status
		}
		else if (choice == choices + 1) {
			ended = true;
		}		
	}	
}

std::vector<std::shared_ptr<CharacterCard>> Player::getCharacters()
{
	return this->characterCards;
}

std::vector<std::shared_ptr<BuildingCard>> Player::getBuildings()
{
	return this->buildingCards;
}

std::vector<std::shared_ptr<CharacterCard>> Player::pickCharacter(std::vector<std::shared_ptr<CharacterCard>> cards)
{
	//show available cards
	std::vector<std::string> availableCharacters;
	std::string messageToShow = "Deze kaarten zijn nog beschikbaar:";
	messageToShow += "\r\n";
	for (unsigned int i = 0; i < cards.size(); ++i)
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
		socket << "Dit is niet jouw beurt, " << name << ".\r\n";
	}
}

bool Player::hasCharacter(std::shared_ptr<CharacterCard> character)
{
	for each (auto c in this->characterCards)
	{
		if (c == character)
			return true;
	}
	return false;
}

const Player & Player::operator<<(const std::string & message) const
{
	socket.write(message);
	return *this;
}

void Player::checkForNewGold(std::shared_ptr<CharacterCard> card)
{
	//koning
	if (card->getName() == "Koning") {
		for each (auto building in buildedBuildings)
		{
			if (building->getColor() == "geel")
			{
				*(this) << "Je ontvangt 1 goudstuk omdat je een gele kaart hebt gebouwd.\r\n";
				*(game->waitingPlayer(shared_from_this())) << name << "ontvangt een goudstuk voor zijn gebouw\r\n";
				addGold(1);
			}				
		}
	}
	//koopman
	if (card->getName() == "Koopman")
	{
		*(this) << "Je ontvangt 1 goudstuk omdat je de koopman bent.\r\n";
		addGold(1);
		for each (auto building in buildedBuildings)
		{
			if (building->getColor() == "groen") {
				*(this) << "Je ontvangt 1 goudstuk omdat een groene kaart hebt gebouwd.\r\n";
				*(game->waitingPlayer(shared_from_this())) << name << "ontvangt een goudstuk voor zijn gebouw\r\n";
				addGold(1);
			}
		}
	}	
	//prediker
	if (card->getName() == "Prediker")
	{
		for each (auto building in buildedBuildings)
		{
			if (building->getColor() == "blauw") {
				*(this) << "Je ontvangt 1 goudstuk omdat je een blauwe kaart hebt gebouwd.\r\n";
				*(game->waitingPlayer(shared_from_this())) << name << "ontvangt een goudstuk voor zijn gebouw\r\n";
				addGold(1);
			}		
		}
	}
	//condotierre
	if (card->getName() == "Condottiere")
	{
		for each (auto building in buildedBuildings)
		{
			if (building->getColor() == "rood") {
				*(this) << "Je ontvangt 1 goudstuk omdat je een rode kaart hebt gebouwd.\r\n";
				*(game->waitingPlayer(shared_from_this())) << name << "ontvangt een goudstuk voor zijn gebouw\r\n";
				addGold(1);
			}	
		}
	}
}
