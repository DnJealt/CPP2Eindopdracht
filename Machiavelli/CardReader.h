#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <memory>
#include <algorithm>
#include <random>

#include "CharacterCard.h"
#include "BuildingCard.h"

class CardReader
{
private:
	std::string fileName;

	//characters
	void readCharacters();
	std::vector<std::shared_ptr<CharacterCard>> characters;
	std::default_random_engine generator;

	//buildings
	void readBuildings();
	std::vector<std::shared_ptr<BuildingCard>> buildings;

public:
	CardReader(std::string readerName);
	~CardReader();

	//characters
	std::vector<std::shared_ptr<CharacterCard>> getCharactersInOrder();
	std::vector<std::shared_ptr<CharacterCard>> getCharactersShuffled();

	//buildings
	std::vector<std::shared_ptr<BuildingCard>> getBuildings();


};

