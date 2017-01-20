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
#include "BuildingCard.h"
#include "CharacterCard.h"
#include "Choose.h"

//forward declaration for undeclared identifier
class Game;


class Player : public std::enable_shared_from_this<Player> {
public:
	Player(const std::string& name, const int age, Socket& socket, std::shared_ptr<Game> game) : name{ name }, age{ age }, socket{ socket }, game{ game } {}
	   
    void set_name(const std::string& new_name) { name = new_name; }
	void addGold(int amount) { gold += amount; }
	void removeGold(int amount) { gold -= amount; }
	void addBuildingCard(std::shared_ptr<BuildingCard> card) { buildingCards.emplace_back(card); }
	void addCharacterCard(std::shared_ptr<CharacterCard> card) { characterCards.emplace_back(card); }
	void turnWith(std::shared_ptr<CharacterCard> character);
	
	std::string get_name() const { return name; }
	int get_age() const { return age; }
	int get_gold() const { return gold; }
	int amountGold() const { return gold; }
	std::vector<std::shared_ptr<CharacterCard>> getCharacters();
	std::vector<std::shared_ptr<BuildingCard>> getBuildings();
	std::vector<std::shared_ptr<BuildingCard>> getBuildedBuildings();
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

	std::vector<std::shared_ptr<BuildingCard>> buildedBuildings;
	std::vector<std::shared_ptr<BuildingCard>> buildingCards;
	std::vector<std::shared_ptr<CharacterCard>> characterCards;
	std::shared_ptr<Game> game;
	void checkForNewGold(std::shared_ptr<CharacterCard> card);
};

#endif /* Player_hpp */
