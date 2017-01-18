#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <memory>
#include <algorithm>
#include <random>
#include <time.h>

#include "CharacterCard.h"
#include "BuildingCard.h"

class CardReader
{
private:
	void read(std::string fileName);
	std::default_random_engine generator;
	std::vector<std::shared_ptr<CharacterCard>> characters;
	std::vector<std::shared_ptr<BuildingCard>> buildings;
public:
	CardReader();
	~CardReader();

	//characters
	std::vector<std::shared_ptr<CharacterCard>> getCharactersInOrder();
	std::vector<std::shared_ptr<CharacterCard>> getCharactersShuffled();

	//buildings
	std::vector<std::shared_ptr<BuildingCard>> getBuildings();


};

