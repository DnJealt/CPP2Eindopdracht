#include "CardReader.h"

CardReader::CardReader()
{
	read("bouwkaarten.csv");
	read("karakterkaarten.csv");
}

void CardReader::read(std::string fileName)
{
	srand(time(0));
	std::ifstream input_file(fileName);
	std::string line;
	while (std::getline(input_file, line)) {
		std::string buffer;
		std::stringstream stringStream(line);

		std::vector<std::string> tokens;
		while (std::getline(stringStream, buffer, ';'))
			tokens.push_back(buffer);
		if (tokens.size() == 2)
		{
			//character card
			std::shared_ptr<CharacterCard> characterCard{ new CharacterCard() };
			characterCard->setNumber(atoi(tokens[0].c_str()));
			characterCard->setName(tokens[1]);

			characters.push_back(characterCard);
		}
		else if (tokens.size() > 2){
			if (tokens.size() <= 3) {
				//building card
				std::shared_ptr<BuildingCard> buildingCard{ new BuildingCard() };
				buildingCard->setName(tokens[0]);
				buildingCard->setPrice(std::stoi(tokens[1]));
				buildingCard->setColor(tokens[2]);

				buildings.push_back(buildingCard);
			}
		}
	}
	std::random_shuffle(buildings.begin(), buildings.end());
}

std::vector<std::shared_ptr<CharacterCard>> CardReader::getCharactersInOrder()
{
	return characters;
}

std::vector<std::shared_ptr<CharacterCard>> CardReader::getCharactersShuffled()
{ 
	// return characters in shuffeled order
	std::vector<std::shared_ptr<CharacterCard>> shuffled{ characters };
	std::random_shuffle(shuffled.begin(), shuffled.end());

	return shuffled;
}

std::vector<std::shared_ptr<BuildingCard>> CardReader::getBuildings()
{
	return buildings;
}

CardReader::~CardReader()
{
}
