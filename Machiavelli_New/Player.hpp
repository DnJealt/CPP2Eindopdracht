//
//  Player.hpp
//  socketexample
//
//  Created by Bob Polis on 23-11-15.
//  Copyright © 2015 Avans Hogeschool, 's-Hertogenbosch. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp

#include <string>
#include <memory>

#include "Socket.h"
#include "Game.h"
#include "BuildingCard.h"
#include "CharacterCard.h"
#include "Choose.h"

class Player {
public:
	Player(const std::string& name, const int age, Socket& socket) : name{ name }, age{ age }, socket{ socket }, gold{ 0 } {}
	   
    void set_name(const std::string& new_name) { name = new_name; }
	void addGold(int amount) { gold += amount; }
	void removeGold(int amount) { gold -= amount; }
	void addBuildingCard(std::shared_ptr<BuildingCard> card) { buildingCards.emplace_back(card); }
	void addCharacterCard(std::shared_ptr<CharacterCard> card) { characterCards.emplace_back(card); }
	void turnWith(std::shared_ptr<CharacterCard> character);


	std::string get_name() const { return name; }
	int get_age() const { return age; }
	int amountGold() const { return gold; }
	std::vector<std::shared_ptr<CharacterCard>> getCharacters();
	std::vector<std::shared_ptr<BuildingCard>> getBuildings();
	std::vector<std::shared_ptr<CharacterCard>> pickCharacter(std::vector<std::shared_ptr<CharacterCard>> cards);
	void pickCommand(std::string command);
	bool hasCharacter(std::shared_ptr<CharacterCard> character);


	const Player & operator<<(const std::string & message) const;
private:    
	int age;
	int gold;
	std::string name;
	Socket& socket;
	std::shared_ptr<Choose> choose;

	std::vector<std::shared_ptr<BuildingCard>> buildingCards;
	std::vector<std::shared_ptr<CharacterCard>> characterCards;
};

#endif /* Player_hpp */
